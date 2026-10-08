# Linux equivalent of polaris' ~/.gem5 + $EPICS/config/epics.csh, in sh syntax.
# Source this before `gmake`: HOST_ARCH in particular must be in the
# environment.
#
# GEM5 predates Tornado: there is no WIND_BASE, only VX_DIR. CONFIG_COMMON
# derives the compiler path as $(VX_DIR)/gnu/$(HOST_ARCH).$(ARCH_CLASS)/bin, so
# with HOST_ARCH=Linux the 68k tools live in $VX_DIR/gnu/Linux.68k -- beside
# polaris' gnu/solaris.68k.

export EPICS=${EPICS:-/usr/software/dev/packages/epics/epics3.12.2GEM5}
export EPICS_BASE=$EPICS/base
export EPICS_EXTENSIONS=$EPICS/extensions
export HOST_ARCH=Linux
export TARGET_ARCH=68k
export VX_DIR=${VX_DIR:-/usr/software/dev/packages/vxworks/v5.2b}

# polaris' epics.csh also sets VX_HOST_TYPE/VX_*_BASE/VX_CPU_FAMILY and
# GCC_EXEC_PREFIX. Nothing in the EPICS config reads the VX_* ones, and
# GCC_EXEC_PREFIX must NOT be set: the CC line already passes
# -B$(VX_GNU_LIB)/gcc-lib/, and a stale prefix would point at the SPARC tools.
unset GCC_EXEC_PREFIX

export PATH=$VX_DIR/gnu/$HOST_ARCH.$TARGET_ARCH/bin:$PATH
