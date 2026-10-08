#!/bin/bash
# Check a finished build before it ships. The spec runs this after `make`, and
# a developer can run it after a local build -- same checks, same values. Each
# check is something that is silent at build time and fatal (or quietly wrong)
# at boot; most of them have bitten gmoscc or hrwfs for real.
#
#   ./tools/linux-build/check-build.sh            # in a built checkout
#   ./tools/linux-build/check-build.sh <dir>      # on an installed tree
set -u
TOP="$(cd "$(dirname "$0")/../.." && pwd -P)"
. "$TOP/tools/linux-build/build.conf"
TREE=$(cd "${1:-$TOP}" && pwd -P)
cd "$TREE"
B=bin/mv167
rc=0
fail() { echo "ERROR: $*" >&2; rc=1; }

# The payload startup.IS loads, plus the other scripts the UAE build makes.
for f in tnetDev sioSup drvAscii ccGlobal.o cicsLib.o epicsCAInt.o \
         hdwrControl.o epicsControl.o local startup.IS; do
    [ -f "$B/$f" ] || fail "$B/$f was not built"
done
for f in default.dctsdr default.sdrSum nirsCCTop.db nirsCCSadTop.db \
         temperatureControl.db resource.def cadVals.pv startupCC.pv \
         gnirsConfig gnirsMechanisms gnirsFilters SafetyFilters; do
    [ -f "data/$f" ] || fail "data/$f missing"
done

# Both files the crate executes must cd to the deploy tree's parent. A
# versioned path, the rpmbuild directory or an empty iocpath (cd "/..") all
# boot and then load the wrong thing or nothing.
for f in $B/startup.IS $B/local; do
    grep -qx "cd \"$DEPLOY/..\"" "$f" || fail "$f does not cd to $DEPLOY/..: $(grep '^cd ' "$f")"
done

# Every load in startup.IS must be absolute, so a boot cannot depend on the
# cd having landed somewhere sensible (gmoscc 03c3552).
grep -nE -e '^[^#]*("|< *|ld < *)\.\.?/' -e '^[^#]*"CC/' $B/startup.IS >&2 \
    && fail "relative paths remain in $B/startup.IS (above)"
grep -q "$IS_DEPLOY/bin/mv167/gmSeqAppl" $B/startup.IS \
    || fail "$B/startup.IS does not load the IS from $IS_DEPLOY"

# pisces-control is being decommissioned; nothing that ships may name it.
# Executed lines only: comments may explain the move.
grep -nE '^[^#]*(pisces|10\.2\.2\.57|/export/gem)' $B/local $B/startup.IS data/resource.def >&2 \
    && fail "pisces references remain (above)"
grep -q "nfsMount \"$IOCPATH_HOST\", \"/gemini\", \"/gemini\"" $B/local \
    || fail "$B/local does not NFS-mount /gemini from $IOCPATH_HOST"

# The build directory must not leak into anything that ships ($(install) used
# to do this through commented-out lines).
grep -rlIE -e '/root/rpmbuild' -e '/build/gnirscc' -e '/home/gemvx/' bin data/*.def 2>/dev/null \
    | sed 's/^/  /' | grep . >&2 \
    && fail "a build directory appears in files that will ship (above)"

# Every loadable object must be a.out for the 68020 family, as the vxWorks
# 5.2 loader expects.
for f in tnetDev sioSup drvAscii ccGlobal.o cicsLib.o hdwrControl.o epicsControl.o; do
    case "$(file -b "$B/$f")" in
        *"a.out SunOS mc68020"*) ;;
        *) fail "$B/$f is not a.out mc68020: $(file -b "$B/$f")" ;;
    esac
done

[ $rc -eq 0 ] && echo "check-build: OK"
exit $rc
