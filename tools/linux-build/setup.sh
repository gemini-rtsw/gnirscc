#!/bin/bash
# Per-checkout bootstrap for building gnirscc on Linux; then plain `gmake`.
#
#   . tools/linux-build/gem5-env.sh && ./tools/linux-build/setup.sh && gmake
#
# This is nirsSetup's applSetup call, made reproducible. It exists because the
# EPICS 3.12 applSetup stamps the checkout's ABSOLUTE path into the generated
# config and symlinks, so none of it can be committed or baked into an image.
set -e

# pwd -P: the physical path, which is what make's $(CURDIR) reports.
TOP="$(cd "$(dirname "$0")/../.." && pwd -P)"
cd "$TOP"

if [ -f /etc/profile.d/gem5.sh ]; then . /etc/profile.d/gem5.sh
else . "$TOP/tools/linux-build/gem5-env.sh"; fi
. "$TOP/tools/linux-build/build.conf"

# 3.12's applSetup is a csh script; polaris runs it from extensions/bin/solaris
# on the PATH. It is host-independent, so run that copy through csh.
APPLSETUP=$EPICS/extensions/bin/solaris/applSetup
[ -f "$APPLSETUP" ] || { echo "ERROR: $APPLSETUP not found -- is the GEM5 EPICS tree installed?" >&2; exit 1; }
command -v csh >/dev/null || { echo "ERROR: csh (tcsh) is required for applSetup" >&2; exit 1; }

# Scrub generated state. applSetup PRESERVES values from an existing
# config/CONFIG.Defs (install path, iocpath, depends), so a checkout set up
# elsewhere -- polaris, another container path -- would keep stale ones.
# data/ and include/ are NOT scrubbed: in this tree they are committed source
# that the build also installs into (APPLIC_INSTALL is the checkout).
rm -rf bin epics config/CONFIG.Defs config.par Distfile
find . -name .git -prune -o -type d -name 'O.*' -prune -exec rm -rf {} + 2>/dev/null || true
find . -name .git -prune -o \( -name .applTop -o -name .epics -o -name .version \) -type l -exec rm -f {} +
rm -f startup/Distfile.*

# applSetup OVERWRITES the application's own startup files with the UAE
# templates, every run (3.12 applSetup lines 785-789 and 834-838):
#
#   templates/uae/startup/local<SITE>.vws    -> startup/local.vws
#   templates/uae/startup/resource<SITE>.def -> startup/resource.def
#   templates/uae/startup/UAE.dist           -> startup/UAE.dist
#   templates/uae/capfast/cad3.02.rc         -> capfast/cad.rc
#
# nirsSetup stashed local.vws only. That protected the hostAdd line, but
# resource.def was silently reset each build -- which went unnoticed only
# because the committed copy IS the template. The moment it is edited (to move
# the IOC log/NTP host off pisces) the edit would vanish from every build. The
# same bug, fixed the same way, as hrwfs c41df27.
#
# Stash what the repository versions; restore it after; fail if a restore does
# not take. Only files that existed beforehand are restored.
VERSIONED="startup/local.vws startup/resource.def startup/UAE.dist capfast/cad.rc"
STASH=$(mktemp -d)
trap 'rm -rf "$STASH"' EXIT
for f in $VERSIONED; do
    [ -f "$f" ] && cp -p "$f" "$STASH/${f//\//_}"
done

# nirsSetup's arguments. -b/-c/-e are explicit because applSetup otherwise
# finds base via `which caRepeater` and extensions via GetVar -- Solaris host
# binaries that do not exist here. -P is HOST:PATH (see build.conf).
# Depends are the version-selecting /gemini/{astlib,timelib} links, exactly as
# production builds resolve them.
csh -f "$APPLSETUP" \
    -b "$EPICS/base" -c "$EPICS/config" -e "$EPICS/extensions" \
    -install "$TOP" -depends "$TOP" -depends /gemini/astlib \
    -targets mv167 -depends /gemini/timelib -P "$IOCPATH_HOST:$DEPLOY" \
    -I startup -I ascii -I src -I sys -I capfast -I pv -s Makefile.subdirs

for f in $VERSIONED; do
    k="$STASH/${f//\//_}"
    if [ -f "$k" ] && ! cmp -s "$k" "$f"; then
        echo "  restoring $f (applSetup replaced it with its template)"
        cp -p "$k" "$f"
    fi
done
for f in $VERSIONED; do
    k="$STASH/${f//\//_}"
    if [ -f "$k" ] && ! cmp -s "$k" "$f"; then
        echo "ERROR: $f was not restored after applSetup" >&2; exit 1
    fi
done

for f in config/CONFIG.Defs .applTop epics/base epics/config epics/extensions; do
    [ -e "$f" ] || { echo "ERROR: applSetup did not produce $f" >&2; exit 1; }
done

# Two generators cannot run on Linux, and their outputs are committed in data/.
# Seed them into the host build directories and make them NEWER than their
# inputs, so make treats the chains as satisfied:
#
#   capfast/: .sch -> sch2edif -> .edf -> e2sr -> .sr -> .db. sch2edif needs a
#     FlexLM licence server that no longer exists. Dead on Solaris too. The
#     .db files must now be edited directly (see GNFR-75349: the .sch change
#     never reached the .db until it was copied from production).
#   ascii/: cat_ascii/*.ascii -> base/tools/makesdr -> default.{dctsdr,sdrSum},
#     via the SPARC bld* tools. The committed data/default.dctsdr is
#     byte-identical to production's.
mkdir -p capfast/O.$HOST_ARCH ascii/O.$HOST_ARCH
cp -p data/nirsCCTop.db data/nirsCCSadTop.db data/temperatureControl.db capfast/O.$HOST_ARCH/
cp -p data/default.dctsdr data/default.sdrSum ascii/O.$HOST_ARCH/
touch capfast/O.$HOST_ARCH/*.db ascii/O.$HOST_ARCH/default.*

echo
echo "Setup complete:"
grep '^APPLIC_' config/CONFIG.Defs | sed 's/^/  /'
echo "  run 'gmake' to build."
