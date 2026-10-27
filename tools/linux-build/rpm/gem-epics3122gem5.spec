# EPICS 3.12.2 GEM5 BUILD tree: UAE config and rules, headers, record
# includes, the applSetup templates, and applSetup itself.
#
# Not to be confused with gem5-epics-runtime, which installs the mv167
# binaries the CRATE loads at /gemini/external/GEM5. This one is what gmake
# needs on the build host.
#
# No host tools: for an mv167 application build the 3.12 rules invoke only the
# 68k compiler and linker, sed and install (CONFIG.Vx.68k, RULES.Vx). The two
# generators that would need host binaries -- Capfast's sch2edif/e2sr and
# makesdr's bld* tools -- are dead or unneeded, and their outputs are
# committed in the application (gnirscc MIGRATION-PLAN.md 0b/0d). applSetup is
# a csh script, so the Solaris copy runs unchanged under tcsh.
#
# VERSIONING CONVENTION: tied to the GEM5 tree, so the version is in the name
# and the path, and it co-installs with the GEM7 / GEM8.x sets.

%global _binaries_in_noarch_packages_terminate_build 0
%global _build_id_links none
%global __os_install_post %{nil}
%global debug_package %{nil}
%global epicsdir /usr/software/dev/packages/epics/epics3.12.2GEM5

Name:           gem-epics3122gem5
Version:        3.12.2
Release:        1%{?dist}
Summary:        EPICS 3.12.2 GEM5 build tree (config, headers, UAE templates)
License:        EPICS Open License / Proprietary Gemini additions (org-internal)
AutoReqProv:    no
# Only text and headers; no binary the host runs.
BuildArch:      noarch
Requires:       gem-vxworks52-linux
Requires:       tcsh, make, sed, coreutils

%description
The GEM5 EPICS build tree at %{epicsdir}: the UAE config and rules (config/),
base and extensions headers, base/rec, the UAE templates applSetup copies
from, and applSetup (a csh script, kept at its Solaris path
extensions/bin/solaris/applSetup). Ships /etc/profile.d/gem5.{sh,csh} with
the HOST_ARCH=Linux environment -- the Linux equivalent of polaris's ~/.gem5.

%install
E=%{buildroot}%{epicsdir}
mkdir -p $E
cd %{trees}%{epicsdir}
for d in config base/include base/rec extensions/include extensions/src/uae/templates; do
    mkdir -p "$E/$(dirname $d)"
    cp -a "$d" "$E/$d"
done
install -Dpm 0755 extensions/bin/solaris/applSetup $E/extensions/bin/solaris/applSetup

# Solaris-era build litter.
find $E -type d \( -name 'O.*' -o -name CVS \) -prune -exec rm -rf {} + 2>/dev/null || :

mkdir -p %{buildroot}/etc/profile.d
cat > %{buildroot}/etc/profile.d/gem5.sh <<'EOF'
# GEM5 EPICS 3.12.2 / vxWorks 5.2b build environment (gem-epics3122gem5 RPM).
# The Linux equivalent of polaris's ~/.gem5 + epics.csh. GCC_EXEC_PREFIX is
# deliberately unset: the CC line passes -B$(VX_GNU_LIB)/gcc-lib/ itself.
export EPICS=/usr/software/dev/packages/epics/epics3.12.2GEM5
export EPICS_BASE=$EPICS/base
export EPICS_EXTENSIONS=$EPICS/extensions
export HOST_ARCH=Linux
export TARGET_ARCH=68k
export VX_DIR=/usr/software/dev/packages/vxworks/v5.2b
unset GCC_EXEC_PREFIX
export PATH=$VX_DIR/gnu/$HOST_ARCH.$TARGET_ARCH/bin:$PATH
EOF
cat > %{buildroot}/etc/profile.d/gem5.csh <<'EOF'
setenv EPICS /usr/software/dev/packages/epics/epics3.12.2GEM5
setenv EPICS_BASE $EPICS/base
setenv EPICS_EXTENSIONS $EPICS/extensions
setenv HOST_ARCH Linux
setenv TARGET_ARCH 68k
setenv VX_DIR /usr/software/dev/packages/vxworks/v5.2b
unsetenv GCC_EXEC_PREFIX
setenv PATH $VX_DIR/gnu/$HOST_ARCH.$TARGET_ARCH/bin\:$PATH
EOF

# A missing piece here is a build that fails much later and less clearly.
for f in config/CONFIG_APPLIC config/RULES_APPLIC.Build config/CONFIG.Vx.68k \
         config/CONFIG_ARCH.mv167 config/CONFIG.Unix.Linux \
         base/include/dbAccess.h extensions/src/uae/templates/startup/local.vws \
         extensions/bin/solaris/applSetup; do
    [ -e "$E/$f" ] || { echo "ERROR: missing $f" >&2; exit 1; }
done

%files
%{epicsdir}
/etc/profile.d/gem5.sh
/etc/profile.d/gem5.csh

%changelog
* Thu Oct 08 2026 Hawi Stecher <hawi.stecher@noirlab.edu> - 3.12.2-1
- Initial packaging for the gnirscc Linux rehost (REL-5050), from polaris's
  /usr/software/dev/packages/epics/epics3.12.2GEM5 (epicsVersion.h:
  R3.12.2GEM5, update level 5). Build tree only -- no host tools are needed.
