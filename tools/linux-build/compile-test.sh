#!/bin/bash
# Toolchain experiment: compile every sys/ source with ANL's Linux 68k gcc 2.96
# against the GEM5 (vxWorks 5.2b / EPICS 3.12.2) headers, using the exact flags
# the UAE build passes, and link each product with ld68k -r the way
# sys/*/Makefile.Vx does. This tests the TOOLCHAIN, not the build -- the full
# gmake run comes after (cf. hrwfs MIGRATION-PLAN.md section 0b).
#
# Run on the workstation:
#   tools/linux-build/compile-test.sh [OUTDIR]
#
# Needs:
#   $GNIRSCC_BUILDENV/root          staged trees at their original paths
#   $GNIRSCC_BUILDENV/toolchain/gnu-tools.tor2_2-m68k-rhel5.tgz
#   docker image gnirscc-build:el9  (tools/linux-build/Dockerfile.el9)
set -euo pipefail

TOP=$(cd "$(dirname "$0")/../.." && pwd)
BUILDENV=${GNIRSCC_BUILDENV:-$HOME/work/gnirscc-buildenv}
OUT=$(realpath -m "${1:-$BUILDENV/compile-test}")
TOOLS=gnu-tools.tor2_2-m68k-rhel5.tgz
TOOLS_SHA256=3ea24b7322d815ec1e8f438ee5faf3ce00cbd4219574ed23621858728376fc7e

echo "$TOOLS_SHA256  $BUILDENV/toolchain/$TOOLS" | sha256sum -c --quiet -
rm -rf "$OUT" && mkdir -p "$OUT"

# The trees are COPIED into tmpfs, not bind-mounted: ANL's cpp is a 32-bit
# binary without large-file support, and stat() on a filesystem with 64-bit
# inode numbers (xfs /home here) fails with EOVERFLOW, which cpp reports as
# "Value too large for defined data type" against a header. tmpfs needs exec,
# because the compiler runs from it.
docker run --rm -i \
    -v "$BUILDENV/root:/stage:ro" \
    -v "$BUILDENV/toolchain:/toolchain:ro" \
    -v "$TOP:/repo:ro" \
    -v "$OUT:/out" \
    --tmpfs /usr/software:exec,size=1g \
    --tmpfs /gemini:exec,size=256m \
    --tmpfs /build:exec,size=256m \
    -e HOST_UID="$(id -u)" -e HOST_GID="$(id -g)" \
    gnirscc-build:el9 bash -s <<'EOF'
set -euo pipefail
cp -a /stage/usr/software/. /usr/software/
cp -a /stage/gemini/. /gemini/

export EPICS=/usr/software/dev/packages/epics/epics3.12.2GEM5
export VX_DIR=/usr/software/dev/packages/vxworks/v5.2b

# CONFIG_COMMON: VX_GNU_BIN = $(VX_DIR)/gnu/$(HOST_ARCH).$(ARCH_CLASS)/bin, so
# for HOST_ARCH=Linux the toolchain belongs in gnu/Linux.68k -- the same slot
# the Solaris tools occupy as gnu/solaris.68k. ANL's tarball unpacks to
# host/x86-linux/{bin,lib}.
mkdir -p /tmp/tc "$VX_DIR/gnu/Linux.68k"
tar xzf /toolchain/gnu-tools.tor2_2-m68k-rhel5.tgz -C /tmp/tc
cp -a /tmp/tc/host/x86-linux/. "$VX_DIR/gnu/Linux.68k/"
VX_GNU_BIN=$VX_DIR/gnu/Linux.68k/bin
VX_GNU_LIB=$VX_DIR/gnu/Linux.68k/lib

cp -a /repo/. /build/gnirscc
rm -rf /build/gnirscc/.git
APP=/build/gnirscc

# CONFIG.Vx.68k + CONFIG_SITE (VX_OPT=NO, VX_WARN=YES) + CONFIG_APPLIC's
# VX_INCLUDES, with APPLIC_DEPENDS = <checkout> /gemini/astlib /gemini/timelib
# (from the V1-27 CONFIG.Defs on polaris).
CC="$VX_GNU_BIN/cc68k -B$VX_GNU_LIB/gcc-lib/ -nostdinc"
LD="$VX_GNU_BIN/ld68k -r"
ARCH_DEP_CFLAGS="-DCPU=MC68040 -m68040 -DCPU_FAMILY=MC680X0"
VX_OP_SYS_FLAGS="-DvxWorks -DV5_vxWorks"
# Production compiles from <dir>/O.mv167, so -I. is the build dir and -I.. the
# source dir; /build/out/<dir> plays O.mv167 here.
INCS="-I$EPICS/base/include/rec -I$APP/include -I$APP/include \
      -I/gemini/astlib/include -I/gemini/timelib/include -I$VX_DIR/h \
      -I$EPICS/base/include -I$EPICS/base/rec"

echo "== compiler: $($VX_GNU_BIN/cc68k --version 2>&1 | head -1)"

declare -A PROD=(
    [global]="(each object)"   [drvAscii]=drvAscii [drvSerial]=sioSup
    [tnetDev]=tnetDev          [hdwrControl]=hdwrControl.o
    [epicsControl]=epicsControl.o
)
pass=0; fail=0
for d in global drvAscii drvSerial tnetDev hdwrControl epicsControl; do
    src=$APP/sys/$d
    # Build in tmpfs as well: -I. and the output file are stat()ed too, and
    # /out is a bind mount from the workstation's 64-bit-inode filesystem.
    mkdir -p /build/out/$d && cd /build/out/$d
    # epicsControl/Makefile.Vx: USR_INCLUDES = -I.applTop/../include
    usr=""; [ $d = epicsControl ] && usr="-I$APP/include"
    objs=()
    for c in "$src"/*.c; do
        o=$(basename "${c%.c}").o
        if $CC $usr $ARCH_DEP_CFLAGS -Wall -I. -I"$src" $INCS \
                $VX_OP_SYS_FLAGS -c "$c" -o "$o" >"$o.log" 2>&1; then
            pass=$((pass+1)); objs+=("$o")
            printf "  ok    %-14s %s\n" "$d" "$o"
        else
            fail=$((fail+1))
            printf "  FAIL  %-14s %s  -- %s\n" "$d" "$o" \
                "$(grep -m1 -i 'error' "$o.log" || tail -1 "$o.log")"
        fi
    done
    p=${PROD[$d]}
    if [ "$p" != "(each object)" ] && [ ${#objs[@]} -gt 0 ]; then
        if $LD -o "$p" "${objs[@]}" > "$p.link.log" 2>&1; then
            printf "  link  %-14s %s (%d objects)\n" "$d" "$p" ${#objs[@]}
        else
            printf "  LINK FAIL %-10s %s -- %s\n" "$d" "$p" "$(head -1 "$p.link.log")"
        fi
    fi
done
echo "== $pass compiled, $fail failed"
cp -a /build/out/. /out/
chown -R "$HOST_UID:$HOST_GID" /out
EOF
