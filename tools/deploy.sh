#!/bin/sh
#
# Safe wrapper around rdist for deploying the CC build to /gemini/GEM5/...
#
# Rationale: plain rdist will happily mirror a build tree into whatever
# destination the tracked Distfile points at. A stale Distfile once caused
# a V1-24 build to be deployed over V1-21 on pisces, overwriting binaries
# and deleting ~220 dated data backups before it was caught. This script
# forces a human to confirm the source/destination and to re-confirm when
# the destination already has contents.
#
# Usage: ./tools/deploy.sh   (run from the CC repo root)
#

set -e

DISTFILE=./Distfile

if [ ! -f "$DISTFILE" ]; then
    echo "ERROR: no Distfile in $(pwd). Run 'make Distfile' first." >&2
    exit 1
fi

# Pull the first source and destination out of the Distfile. rdist supports
# many src/dst pairs; we only report the first as a sanity check for the
# operator -- they're all under the same version dir in our builds.
SRC=`grep -m1 '^FILES1' "$DISTFILE" | sed 's|.*= *( *||; s| *).*||'`
DST=`grep -m1 'install -R' "$DISTFILE" | sed 's|.*install -R *||; s|/bin/mv167;.*||; s|/bin/mv167.*||'`

if [ -z "$SRC" ] || [ -z "$DST" ]; then
    echo "ERROR: could not parse Distfile (SRC='$SRC' DST='$DST')." >&2
    exit 1
fi

# On pisces, /gemini is a local path. On polaris it's NFS-mounted the same
# way. Either way we can stat the destination directly.
echo ""
echo "Deploy plan from $DISTFILE:"
echo "  src: $SRC"
echo "  dst: $DST"
echo ""

if [ -d "$DST" ]; then
    FILE_COUNT=`find "$DST" -type f 2>/dev/null | wc -l | tr -d ' '`
    echo "============================================================"
    echo "   WARNING: DESTINATION ALREADY EXISTS"
    echo "============================================================"
    echo ""
    echo "   $DST"
    echo "   contains $FILE_COUNT existing file(s)."
    echo ""
    echo "   rdist will OVERWRITE any files that differ and DELETE"
    echo "   any files present at the destination but not in the"
    echo "   source tree. Dated backups, hand-edits, and any other"
    echo "   unversioned content under this path will be lost."
    echo ""
    echo "   If you are not absolutely certain you want to replace"
    echo "   the current contents of this directory, answer no."
    echo ""
    echo "============================================================"
    echo ""
    printf "Type the version name exactly to confirm replacement: "
    read CONFIRM
    EXPECTED=`basename "$DST"`
    if [ "$CONFIRM" != "$EXPECTED" ]; then
        echo "Confirmation did not match '$EXPECTED'. Aborting." >&2
        exit 1
    fi
else
    echo "Destination does not exist. It will be created by rdist."
    echo ""
    printf "Deploy now? [yes/no]: "
    read CONFIRM
    if [ "$CONFIRM" != "yes" ]; then
        echo "Aborted." >&2
        exit 1
    fi
fi

echo ""
echo "Running rdist..."
echo ""
exec /usr/bin/rdist -f "$DISTFILE"
