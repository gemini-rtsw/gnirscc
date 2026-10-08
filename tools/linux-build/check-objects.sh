#!/bin/bash
# Compare Linux-built mv167 objects against the deployed production build, and
# check that every one of them will LOAD on the crate: each undefined symbol,
# taken in startup.IS's ld order, must be defined by the vxWorks 5.2 kernel's
# symbol table or by a module loaded before it. A failure here is a crate that
# stops mid-boot with "undefined symbol", so run this before any crate test.
#
#   tools/linux-build/check-objects.sh [BUILT_DIR]
#
# BUILT_DIR defaults to compile-test.sh's output. Exit status is non-zero if
# any module would fail to load, or a product exports different symbols than
# production does.
set -euo pipefail

BUILDENV=${GNIRSCC_BUILDENV:-$HOME/work/gnirscc-buildenv}
BUILT=$(realpath "${1:-$BUILDENV/compile-test}")

docker run --rm -i \
    -v "$BUILDENV:/b:ro" -v "$BUILT:/c:ro" \
    --tmpfs /t:exec,size=256m \
    gnirscc-build:el9 bash -s <<'EOF'
set -euo pipefail
mkdir -p /t/tc && tar xzf /b/toolchain/gnu-tools.tor2_2-m68k-rhel5.tgz -C /t/tc
BIN=/t/tc/host/x86-linux/bin
G=/b/root/gemini; E=$G/external/GEM5; P=$G/GEM5/gnirs/CC/V1-27/bin/mv167; C=/c
rc=0

defs()  { $BIN/nm68k -g "$1" | awk 'NF==3 && $2!="U" {print $3}' | sort -u; }
undef() { $BIN/nm68k -g "$1" | awk '$1=="U" {print $2}' | sort -u; }

# product name -> path under the built tree
declare -A B=(
    [tnetDev]=tnetDev/tnetDev [sioSup]=drvSerial/sioSup [drvAscii]=drvAscii/drvAscii
    [ccGlobal.o]=global/ccGlobal.o [cicsLib.o]=global/cicsLib.o
    [hdwrControl.o]=hdwrControl/hdwrControl.o [epicsControl.o]=epicsControl/epicsControl.o
)
ORDER="tnetDev sioSup drvAscii ccGlobal.o cicsLib.o hdwrControl.o epicsControl.o"

echo "== format, and text/data/bss: production -> built"
for n in $ORDER; do
    f=$(file -b "$C/${B[$n]}")
    case "$f" in *"a.out SunOS mc68020"*) ;; *) echo "  $n: WRONG FORMAT: $f"; rc=1 ;; esac
    printf "  %-16s %-24s -> %s\n" "$n" \
        "$($BIN/size68k "$P/$n" | awk 'NR==2{print $1"/"$2"/"$3}')" \
        "$($BIN/size68k "$C/${B[$n]}" | awk 'NR==2{print $1"/"$2"/"$3}')"
done

echo "== exported symbols, built vs production"
for n in $ORDER; do
    if ! diff -q <(defs "$P/$n") <(defs "$C/${B[$n]}") >/dev/null; then
        echo "  $n DIFFERS:"; { diff <(defs "$P/$n") <(defs "$C/${B[$n]}") || true; } | sed "s/^/    /"; rc=1
    fi
done
[ $rc -eq 0 ] && echo "  identical for all ${#B[@]} products"

# startup.IS load order. The IS module is the production 2012 build either way.
load_check() {
    label=$1; shift
    defs $E/base/bin/mv167/vxWorks.sym > /t/have
    total=0
    for m in "$@"; do
        undef "$m" | comm -23 - /t/have > /t/miss || true
        n=$(wc -l < /t/miss); total=$((total + n))
        [ $n -gt 0 ] && echo "  $(basename "$m"): $n unresolved: $(tr '\n' ' ' < /t/miss)"
        sort -u /t/have <(defs "$m") -o /t/have
    done
    echo "  $label: $total unresolved"
    # Not `return $total`: exit statuses wrap at 256.
    [ $total -eq 0 ]
}
RT="$E/base/bin/mv167/iocCore $E/base/bin/mv167/drvSup $E/base/bin/mv167/recSup $E/base/bin/mv167/devSup"
LIBS="$E/extensions/bin/mv167/pvload $G/slalib/bin/mv167/slalib $G/timelib/bin/mv167/timelib $G/astlib/bin/mv167/astlib"
IS=$G/GEM5/gnirs/IS/IS/bin/mv167/gmSeqAppl

echo "== boot-order load check against the GEM5 vxWorks 5.2 kernel ($(defs $E/base/bin/mv167/vxWorks.sym | wc -l) symbols)"
load_check "production (baseline)" $RT $P/tnetDev $P/sioSup $P/drvAscii $LIBS \
    $P/ccGlobal.o $P/cicsLib.o $P/hdwrControl.o $P/epicsControl.o $IS || rc=1
load_check "built" $RT $C/${B[tnetDev]} $C/${B[sioSup]} $C/${B[drvAscii]} $LIBS \
    $C/${B[ccGlobal.o]} $C/${B[cicsLib.o]} $C/${B[hdwrControl.o]} $C/${B[epicsControl.o]} $IS || rc=1

[ $rc -eq 0 ] && echo "== PASS" || echo "== FAIL"
exit $rc
EOF
