#!/bin/bash
# Build the GEM5 dependency RPMs for gnirscc from the staged trees. These copy
# prepared trees rather than building from source, so they have no repo of
# their own and are not in the pipeline -- the same arrangement as gmoscc and
# hrwfs. Output: rpm/out/.
#
#   gem-vxworks52-linux   build   /usr/software/dev/packages/vxworks/v5.2b (+ ANL 68k tools)
#   gem-epics3122gem5     build   /usr/software/dev/packages/epics/epics3.12.2GEM5
#   gem5-deplibs          both    /gemini/{astlib-1.3,slalib-1.1,timelib-1.3} + links
#   gem5-epics-runtime    runtime /gemini/external/GEM5 (incl. the boot kernel)
#   gnirs-is              runtime /gemini/GEM5/gnirs/IS/IS
#
# Publish with gemini-rtsw-repo/upload-rpm.sh --tag-only, then run the
# rebuild-latest workflow so they land in the served rpm-repo:latest.
#
# Trees come from $GNIRSCC_BUILDENV (see MIGRATION-PLAN.md "Trees staged"):
#   root/usr/software/...  root/gemini/...  toolchain/gnu-tools.tor2_2-m68k-rhel5.tgz
set -euo pipefail

HERE="$(cd "$(dirname "$0")" && pwd)"
BUILDENV="${GNIRSCC_BUILDENV:-$HOME/work/gnirscc-buildenv}"
OUT="$HERE/out"
SPECS="${*:-gem-vxworks52-linux gem-epics3122gem5 gem5-deplibs gem5-epics-runtime gnirs-is}"
rm -rf "$OUT" && mkdir -p "$OUT"

echo "3ea24b7322d815ec1e8f438ee5faf3ce00cbd4219574ed23621858728376fc7e  $BUILDENV/toolchain/gnu-tools.tor2_2-m68k-rhel5.tgz" \
    | sha256sum -c --quiet -

docker run --rm -i \
    -v "$BUILDENV/root/usr:/trees/usr:ro" \
    -v "$BUILDENV/root/gemini:/trees/gemini:ro" \
    -v "$BUILDENV/toolchain:/trees/toolchain:ro" \
    -v "$HERE:/specs:ro" \
    -v "$OUT:/out" \
    -e SPECS="$SPECS" -e HOST_UID="$(id -u)" -e HOST_GID="$(id -g)" \
    rockylinux:9 bash -s <<'EOF'
set -euo pipefail
dnf install -y -q rpm-build >/dev/null 2>&1
for s in $SPECS; do
    echo "==== $s"
    rpmbuild -bb --quiet --define "trees /trees" --define "_rpmdir /out" /specs/$s.spec
done
chown -R "$HOST_UID:$HOST_GID" /out
EOF
echo
echo "RPMs in $OUT/:"
find "$OUT" -name '*.rpm' -printf '  %f  (%s bytes)\n' | sort
