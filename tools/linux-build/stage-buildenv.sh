#!/bin/sh
# Collect everything the gnirscc Linux cross-build needs from the hosts that
# still have it. Read-only except for /var/tmp/gnirscc-stage.
#
# gnirscc shares NONE of the gmoscc/hrwfs artifacts. Those are EPICS 3.13 on
# PowerPC; gnirscc is GEM5 = EPICS 3.12.2 on mv167 (MC68040). So this is a
# from-scratch pass -- see MIGRATION-PLAN.md.
#
# Modes:
#
#   inventory          polaris, GEM5 env sourced   -- answers questions, copies nothing
#   buildenv           polaris, GEM5 env sourced   -- tars the EPICS / Tornado trees
#   runtime            host that sees /gemini      -- tars what the crate ld's at boot
#   deployed <path>    host that sees /gemini      -- tars the LIVE CC tree (escrow)
#
# Usage (csh on polaris -- the GEM5 alias must be run FIRST so this script
# inherits its environment; a csh alias is invisible to sh):
#
#   GEM5
#   sh stage-buildenv.sh inventory > /var/tmp/gnirscc-inventory.txt 2>&1
#
# Written for Solaris /bin/sh: no $(...), no `tar z`, no readlink, and /tmp is
# swap-backed -- hence /var/tmp and explicit gzip pipes.

OUT=/var/tmp/gnirscc-stage

# Symlink target without readlink (absent on older Solaris).
linkof() { ls -ld "$1" 2>/dev/null | sed -n 's/.* -> //p'; }

# The GCC banner a built object carries -- the only reliable record of which
# compiler produced it.
banner() { strings -a "$1" 2>/dev/null | grep 'GCC:' | sort -u | head -3; }

case "${1:-inventory}" in

inventory)
    echo "=== host ==="; uname -a; hostname; date
    echo
    echo "=== GEM5 environment (run the GEM5 alias before this script) ==="
    for v in EPICS EPICS_BASE EPICS_EXTENSIONS HOST_ARCH WIND_BASE WIND_HOST_TYPE \
             VX_DIR GCC_EXEC_PREFIX VW_GNU T_A ARCH; do
        eval "val=\${$v:-<unset>}"
        printf "%-18s = %s\n" $v "$val"
    done
    echo "PATH ="; echo "$PATH" | tr ':' '\n' | sed 's/^/    /'
    echo
    echo "# setup files the alias may source:"
    ls -la $HOME/.gem* $HOME/.epics* 2>/dev/null
    echo
    echo "=== 68k cross-compiler ==="
    for c in cc68k gcc68k cc68040 as68k ld68k nm68k munch ccppc; do
        printf "%-10s " $c; which $c 2>/dev/null || echo "MISSING"
    done
    CC68=`which cc68k 2>/dev/null`
    if [ -n "$CC68" ] && [ -f "$CC68" ]; then
        file "$CC68"
        echo "# version:"; "$CC68" -v 2>&1 | tail -3
    fi
    echo
    echo "=== Tornado / vxWorks version ==="
    echo "WIND_BASE: $WIND_BASE"; ls -la $WIND_BASE 2>/dev/null
    echo "# host dirs:"; ls $WIND_BASE/host 2>/dev/null
    cat $WIND_BASE/.wind_version $WIND_BASE/../.wind_version 2>/dev/null
    for h in $WIND_BASE/target/h/version.h $WIND_BASE/h/version.h; do
        [ -f $h ] && { echo "# $h:"; grep -i version $h; }
    done
    echo "# target CPU support present:"
    ls $WIND_BASE/target/h/arch 2>/dev/null
    ls -d $WIND_BASE/target/config/mv16* 2>/dev/null
    echo
    echo "=== EPICS ==="
    echo "EPICS: $EPICS"; ls -la $EPICS 2>/dev/null
    for f in $EPICS/base/include/epicsVersion.h $EPICS/base/include/version.h; do
        [ -f $f ] && { echo "# $f:"; grep -i 'define' $f | head -12; }
    done
    echo "# base/bin (host + target arches):"; ls $EPICS/base/bin 2>/dev/null
    echo "# base/config:"; ls $EPICS/base/config 2>/dev/null
    echo "# Linux support in this tree? (anything mentioning linux in config/ or src/)"
    ls $EPICS/base/config 2>/dev/null | grep -i linux
    grep -ril linux $EPICS/base/config 2>/dev/null | head -10
    ls -d $EPICS/base/src/*/os/* $EPICS/base/src/libCom/os* 2>/dev/null | head -20
    echo "# does the tree carry source (needed to rebuild host tools)?"
    du -sk $EPICS/base/src $EPICS/extensions/src 2>/dev/null
    echo
    echo "=== UAE tools (3.12 uses a SHELL applSetup, not applSetup.pl) ==="
    for t in applSetup applSetup.pl gmake make snc dbLoadTemplate dbExpand \
             sdrtodct dct2db sch2edif e2sr e2db edb adl2dl edd macTest antelope e_flex; do
        p=`which $t 2>/dev/null`
        printf "%-15s " $t
        if [ -n "$p" ] && [ -f "$p" ]; then echo "$p"; file "$p" | sed 's/^/                /'
        else echo "MISSING"; fi
    done
    AS=`which applSetup 2>/dev/null`
    [ -n "$AS" ] && { echo "# applSetup head:"; head -30 "$AS"; }
    echo
    echo "=== host tool binaries for HOST_ARCH ==="
    ls -la $EPICS/base/bin/$HOST_ARCH $EPICS/extensions/bin/$HOST_ARCH 2>/dev/null
    echo
    echo "=== mv167 target runtime the crate loads ==="
    for d in $EPICS/base/bin/mv167 $EPICS/extensions/bin/mv167 \
             /gemini/external/GEM5/base/bin/mv167 /gemini/external/GEM5/extensions/bin/mv167; do
        echo "# $d"; ls -la $d 2>/dev/null || echo "  absent"
    done
    for o in $EPICS/base/bin/mv167/iocCore /gemini/external/GEM5/base/bin/mv167/iocCore; do
        [ -f $o ] && { echo "# compiler of $o:"; file $o; banner $o; }
    done
    ls -la /gemini/external/GEM5 2>/dev/null
    echo
    echo "=== support libraries (astlib/slalib/timelib) ==="
    for l in /gemini/astlib /gemini/slalib /gemini/timelib \
             $EPICS/extensions/src/gemini/astlib \
             $EPICS/extensions/src/gemini/slalib \
             $EPICS/extensions/src/gemini/timelib; do
        t=`linkof $l`
        echo "# $l ${t:+-> $t}"
        ls -la $l 2>/dev/null | head -20
        ls -la $l/bin/mv167 2>/dev/null
        for o in $l/bin/mv167/*; do [ -f $o ] && { banner $o; break; }; done
    done
    echo
    echo "=== vxWorks kernels for mv167 ==="
    ls -la /gemini/external/vxWorks/*/mv167 /vw/config/mv167 2>/dev/null
    echo
    echo "=== existing gnirscc build trees on this host ==="
    for d in $HOME/gnirscc* /home/gemvx/*/gnirscc*; do
        [ -d "$d" ] || continue
        echo "## $d"
        ls -la "$d"/.applTop "$d"/.epics "$d"/.version 2>/dev/null
        [ -f "$d"/config/CONFIG.Defs ] && cat "$d"/config/CONFIG.Defs
        ls -la "$d"/bin/mv167 2>/dev/null
        for o in "$d"/bin/mv167/*.o; do
            [ -f "$o" ] && { echo "# compiler of $o:"; banner "$o"; break; }
        done
    done
    echo
    echo "=== sizes (for the buildenv tarballs) ==="
    du -sk $EPICS $WIND_BASE/target $WIND_BASE/host 2>/dev/null
    df -k /var/tmp | tail -1
    ;;

buildenv)
    [ -n "${EPICS:-}" ]     || { echo "ERROR: \$EPICS unset -- run GEM5 first"; exit 1; }
    [ -n "${WIND_BASE:-}" ] || { echo "ERROR: \$WIND_BASE unset -- run GEM5 first"; exit 1; }
    mkdir -p $OUT
    echo "Staging into $OUT"; df -k $OUT | tail -1

    stage() {
        name=$1; shift
        echo "-> $name.tar.gz  ($*)"
        # .part until complete: a full /var/tmp otherwise leaves a truncated
        # archive behind an encouraging "->" line.
        tar cf - "$@" 2>/dev/null | gzip -c > $OUT/$name.tar.gz.part
        if [ $? -ne 0 ]; then
            echo "  FAILED (disk full?) -- removed" >&2
            rm -f $OUT/$name.tar.gz.part; df -k $OUT | tail -1 >&2; return 1
        fi
        mv $OUT/$name.tar.gz.part $OUT/$name.tar.gz
    }

    stage gem5-config  $HOME/.gem* $HOME/.epics* 2>/dev/null
    # Whole EPICS tree, source included: 3.12 has no Linux host support that
    # we know of, so the host tools may need porting and we need the source.
    stage epics        $EPICS
    # Headers + BSP config only. The whole of $WIND_BASE/host is SPARC
    # binaries we cannot run, but its version files are worth keeping.
    stage wind-target  $WIND_BASE/target/h $WIND_BASE/target/config
    # The SPARC cc68k itself: useless to run on Linux, but its specs file and
    # lib/gcc-lib layout record the exact compiler configuration we must match.
    [ -d $WIND_BASE/host ] && stage wind-host-gnu-meta \
        `ls -d $WIND_BASE/host/*/lib/gcc-lib $WIND_BASE/.wind_version 2>/dev/null`

    echo; ls -l $OUT
    echo "Keep these -- they are the only copies of some of this software."
    ;;

runtime)
    mkdir -p $OUT
    echo "Staging the trees the crate ld's at boot into $OUT"
    set --
    for p in /gemini/external/GEM5/base/bin/mv167 \
             /gemini/external/GEM5/extensions/bin/mv167 \
             /gemini/external/GEM5/base/include \
             /gemini/astlib /gemini/slalib /gemini/timelib \
             /gemini/external/vxWorks/vxUsers; do
        if [ -f $p ] || [ -d $p ]; then
            t=`linkof $p`
            [ -n "$t" ] && echo "  $p -> $t   (symlink: staging its target too)"
            set -- "$@" $p
            # Follow version-selecting symlinks one level so the pinned
            # version is in the archive, not just a dangling link.
            case "$t" in
                /*) set -- "$@" $t ;;
                ?*) set -- "$@" `dirname $p`/$t ;;
            esac
        else
            echo "  absent: $p"
        fi
    done
    du -sk "$@" 2>/dev/null
    tar cf - "$@" 2>/dev/null | gzip -c > $OUT/gem5-runtime.tar.gz.part \
        && mv $OUT/gem5-runtime.tar.gz.part $OUT/gem5-runtime.tar.gz \
        || { echo "FAILED" >&2; rm -f $OUT/gem5-runtime.tar.gz.part; exit 1; }
    ls -l $OUT/gem5-runtime.tar.gz
    ;;

deployed)
    D="${2:-}"
    if [ -z "$D" ]; then
        echo "usage: $0 deployed /gemini/GEM5/gnirs/CC/<release>"
        echo; echo "Candidates:"
        ls -la /gemini/GEM5/gnirs/CC 2>/dev/null
        exit 1
    fi
    [ -d "$D" ] || { echo "ERROR: $D is not a directory"; exit 1; }
    mkdir -p $OUT
    echo "=== $D ==="; ls -la "$D"
    echo "# selector symlinks next to it:"
    ls -la `dirname "$D"` 2>/dev/null
    echo "# startup scripts -- which one the crate boots is in its boot params:"
    ls -la "$D"/bin/mv167 2>/dev/null
    for o in "$D"/bin/mv167/*.o; do
        [ -f "$o" ] && { echo "# compiler of $o:"; banner "$o"; }
    done
    echo "-> gnirscc-deployed.tar.gz (everything: escrow + comparison baseline)"
    ( cd "$D" && tar cf - . ) | gzip -c > $OUT/gnirscc-deployed.tar.gz
    ls -l $OUT/gnirscc-deployed.tar.gz
    ;;

*)
    echo "usage: $0 {inventory|buildenv|runtime|deployed <path>}"; exit 1 ;;
esac
