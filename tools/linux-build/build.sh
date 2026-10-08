#!/bin/bash
# Build gnirscc in the container from the staged GEM5 trees -- the developer
# loop for the build environment itself, until the dependency RPMs exist.
#
#   tools/linux-build/build.sh [OUTDIR]
#
# The checkout is copied into the container and built there; nothing is
# written back to it. Results (bin/, data/, include/, config/CONFIG.Defs and
# the full make log) land in OUTDIR, default $GNIRSCC_BUILDENV/build-out.
set -euo pipefail

TOP=$(cd "$(dirname "$0")/../.." && pwd)
BUILDENV=${GNIRSCC_BUILDENV:-$HOME/work/gnirscc-buildenv}
OUT=$(realpath -m "${1:-$BUILDENV/build-out}")
TOOLS_SHA256=3ea24b7322d815ec1e8f438ee5faf3ce00cbd4219574ed23621858728376fc7e
echo "$TOOLS_SHA256  $BUILDENV/toolchain/gnu-tools.tor2_2-m68k-rhel5.tgz" | sha256sum -c --quiet -
rm -rf "$OUT" && mkdir -p "$OUT"

# Everything in tmpfs: ANL's 32-bit cpp cannot stat() files on a filesystem
# with 64-bit inode numbers (EOVERFLOW, "Value too large for defined data
# type"), and that includes the build directory and its output files.
docker run --rm -i \
    -v "$BUILDENV/root:/stage:ro" \
    -v "$BUILDENV/toolchain:/toolchain:ro" \
    -v "$TOP:/repo:ro" \
    -v "$OUT:/out" \
    --tmpfs /usr/software:exec,size=1g \
    --tmpfs /gemini:exec,size=256m \
    --tmpfs /build:exec,size=512m \
    -e HOST_UID="$(id -u)" -e HOST_GID="$(id -g)" \
    gnirscc-build:el9 bash -s <<'EOF'
set -euo pipefail
cp -a /stage/usr/software/. /usr/software/
cp -a /stage/gemini/. /gemini/
VX_DIR=/usr/software/dev/packages/vxworks/v5.2b
mkdir -p /tmp/tc "$VX_DIR/gnu/Linux.68k"
tar xzf /toolchain/gnu-tools.tor2_2-m68k-rhel5.tgz -C /tmp/tc
cp -a /tmp/tc/host/x86-linux/. "$VX_DIR/gnu/Linux.68k/"

cp -a /repo/. /build/gnirscc
cd /build/gnirscc
. tools/linux-build/gem5-env.sh
set +e
{ ./tools/linux-build/setup.sh && gmake; } > /out/build.log 2>&1
status=$?
set -e
tail -40 /out/build.log
echo "== exit status $status"
for d in bin data include; do [ -d $d ] && cp -a $d /out/; done
cp -p config/CONFIG.Defs /out/ 2>/dev/null || true
chown -R "$HOST_UID:$HOST_GID" /out
exit $status
EOF
