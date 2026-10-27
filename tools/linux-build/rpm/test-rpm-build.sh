#!/bin/bash
# End-to-end test of the packaging, the way the pipeline will run it, before
# anything is published: install the dependency RPMs from rpm/out into a clean
# rockylinux:9, stage the source like gemini-rtsw-ci's build_rpm.sh, rpmbuild
# gnirscc.spec, then INSTALL the result (exercising %post) and check the
# installed tree with check-build.sh. Output: rpm/test-out/.
#
#   tools/linux-build/rpm/build-dep-rpms.sh
#   tools/linux-build/rpm/test-rpm-build.sh
#   tools/linux-build/check-objects.sh tools/linux-build/rpm/test-out/installed
set -euo pipefail

HERE="$(cd "$(dirname "$0")" && pwd)"
TOP="$(cd "$HERE/../../.." && pwd)"
OUT="$HERE/test-out"
ls "$HERE"/out/*/*.rpm >/dev/null || { echo "ERROR: run build-dep-rpms.sh first" >&2; exit 1; }
rm -rf "$OUT" && mkdir -p "$OUT"
GIT_HASH=$(git -C "$TOP" rev-parse --short HEAD)

# tmpfs everywhere the 32-bit cross tools read or write (see compile-test.sh).
docker run --rm -i \
    -v "$HERE/out:/deps:ro" -v "$TOP:/repo:ro" -v "$OUT:/out" \
    --tmpfs /usr/software:exec,size=512m --tmpfs /gemini:exec,size=256m \
    --tmpfs /root/rpmbuild:exec,size=512m --tmpfs /work:exec,size=256m \
    -e GIT_HASH="$GIT_HASH" -e HOST_UID="$(id -u)" -e HOST_GID="$(id -g)" \
    rockylinux:9 bash -s <<'EOF'
set -euo pipefail
# glibc.i686 must match the installed x86_64 glibc exactly, and the base image
# can lag the mirrors (gmoscc's custom-repo-setup.sh does the same).
dnf -y -q upgrade glibc >/dev/null
dnf -y -q install rpm-build git dnf-plugins-core /deps/*/*.rpm >/dev/null
rpm -qa 'gem*' gnirs-is | sort | sed 's/^/  installed: /'

# Stage the source like build_rpm.sh: every file but .git* and rpms/.
cd /repo
dir=gnirscc-1.27
mkdir -p /work/$dir
find . -name ".git*" -prune -o -name "rpms" -prune -o -name "test-out" -prune \
     -o -name "out" -prune -o -type f -print | xargs -I{} cp --parents {} /work/$dir/
mkdir -p /root/rpmbuild/SOURCES
tar -czf /root/rpmbuild/SOURCES/$dir.tar.gz -C /work $dir

cd /work/$dir
dnf -y -q builddep ./gnirscc.spec >/dev/null
set +e
rpmbuild -ba ./gnirscc.spec > /out/rpmbuild.log 2>&1
rc=$?
set -e
tail -5 /out/rpmbuild.log
[ $rc -eq 0 ] || { echo "== rpmbuild FAILED ($rc)"; grep -E 'ERROR|error:' /out/rpmbuild.log | head; exit $rc; }
cp /root/rpmbuild/RPMS/noarch/*.rpm /out/

echo "== install the built RPM (runs %post)"
dnf -y -q install /out/gnirscc-1.27-*.noarch.rpm >/dev/null
rpm -q gnirscc gnirscc-devel 2>/dev/null | sed 's/^/  /' || true
D=/gemini/GEM5/gnirs/CC/CC
for t in gnirsConfig gnirsMechanisms mechanisms.pv; do
    printf "  %-16s data/: %s   owned by rpm: %s\n" $t \
        "$([ -f $D/data/$t ] && echo seeded || echo MISSING)" \
        "$(rpm -qf $D/data/$t >/dev/null 2>&1 && echo yes || echo no)"
done
echo "== a tuned file edited in operations must survive a reinstall"
echo "# tuned in operations" >> $D/data/gnirsMechanisms
dnf -y -q reinstall /out/gnirscc-1.27-*.noarch.rpm >/dev/null
tail -1 $D/data/gnirsMechanisms | grep -q 'tuned in operations' && echo "  survived" || { echo "  OVERWRITTEN"; exit 1; }

echo "== check-build.sh on the INSTALLED tree"
/work/$dir/tools/linux-build/check-build.sh $D
mkdir -p /out/installed && cp -a $D/. /out/installed/
chown -R "$HOST_UID:$HOST_GID" /out
EOF
