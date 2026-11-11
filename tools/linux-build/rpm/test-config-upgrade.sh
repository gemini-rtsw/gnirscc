#!/bin/bash
# Upgrade test for the gnirscc-config sub-repo install step (%posttrans), in a
# clean rockylinux:9 WITHOUT git, the way the boot server installs from the
# mirror. Simulates a boot-server tree that was tuned in operations, upgrades
# to the new RPM, and fails unless every file that was already in data/ is
# unchanged (content, mode, owner). Then checks git use as a non-root user,
# reinstall, and rollback to the old RPM.
#
#   tools/linux-build/rpm/test-rpm-build.sh            # builds the new RPM
#   tools/linux-build/rpm/test-config-upgrade.sh <old gnirscc RPM>
set -euo pipefail

HERE="$(cd "$(dirname "$0")" && pwd)"
OLD="$(cd "$(dirname "$1")" && pwd)/$(basename "$1")"
NEW="$(ls "$HERE"/test-out/gnirscc-1.27-*.noarch.rpm | grep -v devel)"
[ -f "$OLD" ] && [ -f "$NEW" ] || { echo "usage: $0 <old gnirscc rpm> (and run test-rpm-build.sh first)" >&2; exit 1; }
echo "old: $(basename "$OLD")"
echo "new: $(basename "$NEW")"

docker run --rm -i \
    -v "$HERE/out:/deps:ro" -v "$OLD:/rpms/old.rpm:ro" -v "$NEW:/rpms/new.rpm:ro" \
    rockylinux:9 bash -s <<'EOF'
set -euo pipefail
fail() { echo "FAIL: $*"; exit 1; }
D=/gemini/GEM5/gnirs/CC/CC
# Move to an RPM file the way it is done on the boot server: install, or
# downgrade when dnf sorts the git hash lower. Fails unless that NVR is installed.
to() { nvr=$(rpm -qp --qf '%{NVRA}' "$1" 2>/dev/null)
       dnf -y -q install "$1" >/dev/null 2>&1 || :
       [ "$(rpm -q gnirscc)" = "$nvr" ] || dnf -y -q downgrade "$1" >/dev/null
       [ "$(rpm -q gnirscc)" = "$nvr" ] || fail "could not switch to $nvr"; echo "  now $nvr"; }
snap() { (cd $D/data && find . -path ./.git -prune -o -type f -printf '%p %m %u:%g ' -exec md5sum {} \; | awk '{print $1,$2,$3,$4}' | sort); }

dnf -y -q upgrade glibc >/dev/null
dnf -y -q install diffutils /deps/*/*.rpm >/dev/null
rpm -q git >/dev/null && fail "git is installed; this test needs a host without it"

echo "== old RPM, then operations edits like the boot server's"
dnf -y -q install /rpms/old.rpm >/dev/null
rpm -q gnirscc | sed 's/^/  /'
echo "oneWayTravel = -1" >> $D/data/gnirsConfig
echo "# retuned in operations" >> $D/data/gnirsMechanisms
cp -p $D/data/gnirsConfig $D/data/gnirsConfig.2026.11.06-local     # a local backup
echo "local copy, not the repo's" > $D/data/gnirsConfig.2026.09.11  # name clash with a repo backup
snap > /tmp/before

echo "== upgrade to the new RPM (no git on the host)"
to /rpms/new.rpm
snap > /tmp/after
changed=$(join -j1 <(sort /tmp/before) <(sort /tmp/after) | awk '$2!=$5||$3!=$6||$4!=$7{print $1}')
[ -z "$changed" ] || { echo "$changed"; fail "files already in data/ changed"; }
echo "  every file already in data/ unchanged ($(wc -l < /tmp/before) files)"
added=$(comm -13 <(cut -d' ' -f1 /tmp/before) <(cut -d' ' -f1 /tmp/after))
echo "  added: $(echo "$added" | wc -l) files, e.g. $(echo "$added" | head -2 | tr '\n' ' ')"
echo "$added" | grep -qvE '(\.20[0-9]{2}[.0-9:-]*|\.orig)$' && { echo "$added" | grep -vE '(\.20[0-9]{2}[.0-9:-]*|\.orig)$'; fail "added a file that is not a dated backup or .orig"; }
[ -d $D/data/.git ] || fail "data/.git missing"
ls -d $D/.config-repo.* 2>/dev/null && fail "temp dir left behind"
grep -q 'oneWayTravel = -1' $D/data/gnirsConfig || fail "tuned gnirsConfig lost its edit"
grep -q "local copy" $D/data/gnirsConfig.2026.09.11 || fail "local backup overwritten"
echo "  tuned edits and the clashing local backup kept"

echo "== git as a non-root user (git installed afterwards, %post rerun by reinstall)"
dnf -y -q install git >/dev/null
dnf -y -q reinstall /rpms/new.rpm >/dev/null
git config --system --get-all safe.directory | grep -qx $D/data || fail "safe.directory not set"
useradd -m sci
su - sci -c "cd $D/data && git status --short | sed 's/^/  /'"
su - sci -c "cd $D/data && cp -p gnirsMechanisms gnirsMechanisms.2026.11.10-sci && echo '# sci edit' >> gnirsMechanisms && git add gnirsMechanisms gnirsMechanisms.2026.11.10-sci && git -c user.name=sci -c user.email=sci@example.invalid commit -qm 'sci tune' && git log --oneline | sed 's/^/  /'"
head=$(git -C $D/data rev-parse HEAD)

echo "== reinstall: no-op for data/"
snap > /tmp/before
dnf -y -q reinstall /rpms/new.rpm >/dev/null
snap > /tmp/after
cmp -s /tmp/before /tmp/after || { diff /tmp/before /tmp/after; fail "reinstall changed data/"; }
[ "$(git -C $D/data rev-parse HEAD)" = "$head" ] || fail "reinstall changed data/.git"
echo "  data/ and its git history unchanged"

echo "== rollback to the old RPM"
to /rpms/old.rpm
snap > /tmp/after
cmp -s /tmp/before /tmp/after || { diff /tmp/before /tmp/after; fail "rollback changed data/"; }
grep -q '# sci edit' $D/data/gnirsMechanisms || fail "rollback lost a tuned edit"
echo "  data/ unchanged; data/.git stays (unowned, harmless)"
echo "PASS"
EOF
