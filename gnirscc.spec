# gnirscc -- the GNIRS Components Controller IOC.
#
# Cross-compiles for vxWorks 5.2 / mv167 (MC68040) against the GEM5 EPICS
# 3.12.2 tree and the vxWorks 5.2b headers, with ANL's Linux rebuild of the
# Wind River 68k GNU tools (gcc 2.96, a.out). See MIGRATION-PLAN.md for the
# validation (0c, 0d) and docs/BOOT-PARAMETERS.md for the boot story.
#
# The payload installs to the FIXED path /gemini/GEM5/gnirs/CC/CC, which is
# where the crate's boot parameters already point:
#
#   startup script (s): /gemini/GEM5/gnirs/CC/CC/bin/mv167/startup.IS
#
# On pisces that path is a selector symlink to the release of the day; the RPM
# makes it a real directory, so no boot parameter changes -- the transition
# gmoscc (setgmos) and hrwfs (hrwfs/hrwfs) made. rpm -q names what is
# installed and dnf downgrade is the rollback.
#
# The startup also loads the Instrument Sequencer from /gemini/GEM5/gnirs/IS/IS
# (package gnirs-is) -- there is no separate IS IOC.
#
# LIVE-TUNED FILES. gnirsConfig, gnirsMechanisms and mechanisms.pv are edited
# in operations on the deployed tree (and backed up there by date). The package
# must never overwrite them, so it does not own them: it ships the repository
# copies under data.dist/ and %post installs one into data/ only where data/
# has none. Migrating a running instrument means copying its current tuned
# files into data/ (docs/BOOT-PARAMETERS.md).

%global _build_id_links none
%global __os_install_post %{nil}
%global debug_package %{nil}
%global _binaries_in_noarch_packages_terminate_build 0

# Read from tools/linux-build/build.conf, the single source shared with
# setup.sh and the generated CONFIG.Defs. rpmbuild runs from the repository
# root (build_rpm.sh does `cd /work`).
%define buildconf() %(. tools/linux-build/build.conf 2>/dev/null && echo $%1)
%global deploy %{buildconf DEPLOY}
%if "%{deploy}" == ""
%{error:tools/linux-build/build.conf not readable -- rpmbuild must run from the repository root}
%endif
%global tuned gnirsConfig gnirsMechanisms mechanisms.pv

# $GIT_HASH first: build_rpm.sh computes it on the HOST and passes it in.
%define git_hash %(if [ -n "$GIT_HASH" ]; then echo "$GIT_HASH"; else git rev-parse --short HEAD 2>/dev/null || echo nogit; fi)

%define name    gnirscc
%define version 1.27
Name:           %{name}
Version:        %{version}
Release:        1.git%{git_hash}%{?dist}
Summary:        Gemini GNIRS Components Controller IOC (vxWorks 5.2 mv167)
License:        Gemini Observatory (org-internal)
Source0:        %{name}-%{version}.tar.gz
AutoReqProv:    no
# The payload is mv167 objects the host never executes; it serves them over
# NFS to a VME crate.
BuildArch:      noarch

BuildRequires:  gem-vxworks52-linux = 5.2b-1%{?dist}
BuildRequires:  gem-epics3122gem5 = 3.12.2-1%{?dist}
BuildRequires:  gem5-deplibs = 2026.10.08-1%{?dist}
BuildRequires:  make, tcsh, sed, file, findutils, diffutils

# Runtime deps are the OTHER /gemini trees the crate loads at boot -- nothing
# this host executes, so AutoReqProv finds none of them. Listed so installing
# gnirscc on the boot server pulls everything a crate needs. Loose, per the
# pipeline README: pinning runtime deps only creates co-install conflicts.
Requires:       gem5-epics-runtime
Requires:       gem5-deplibs
Requires:       gnirs-is

%description
EPICS 3.12.2 (GEM5) control software for the GNIRS Components Controller,
cross-compiled for the mv167 vxWorks 5.2 target. Installs the deployable IOC
tree under %{deploy}: loadable objects, generated startup scripts, databases,
LUT and .pv files. The operations-tuned configuration files are shipped as
defaults under data.dist/ and installed only where none exist.

%package devel
Summary:        Build environment for gnirscc development images
Requires:       gem-vxworks52-linux, gem-epics3122gem5, gem5-deplibs
Requires:       make, tcsh, sed, file, findutils, diffutils
%description devel
Pulls the pinned gnirscc build dependencies into a dev container.

%prep
%setup -q

%build
# Exactly what a developer runs (tools/linux-build/build.sh does the same in a
# container): applSetup, then make, then the checks that fail the build on
# anything that would be silent here and fatal at the crate.
. /etc/profile.d/gem5.sh
./tools/linux-build/setup.sh
make
./tools/linux-build/check-build.sh

%install
rm -rf $RPM_BUILD_ROOT
D=$RPM_BUILD_ROOT%{deploy}
mkdir -p $D/bin $D/data $D/data.dist
# Mirror the historical rdist payload (startup/UAE.dist): bin/mv167, data,
# include, RELEASE.NOTES.
cp -a bin/mv167 $D/bin/
rm -f $D/bin/mv167/Distfile*
cp -a include RELEASE.NOTES $D/
for f in data/*; do
    n=$(basename "$f")
    case "$n" in
        *.20[0-9][0-9].*|*.orig) continue ;;   # dated backups, editor leftovers
    esac
    cp -p "$f" $D/data/
done
for t in %{tuned}; do
    [ -f $D/data/$t ] || { echo "ERROR: tuned file $t missing from the build" >&2; exit 1; }
    mv $D/data/$t $D/data.dist/
done

%post
# Seed the tuned configuration only where data/ has none. Never overwrite.
for t in %{tuned}; do
    [ -e %{deploy}/data/$t ] || cp -p %{deploy}/data.dist/$t %{deploy}/data/$t
done
:

%files
%defattr(-,root,root,-)
%dir %{deploy}
%{deploy}/bin
%{deploy}/include
%{deploy}/RELEASE.NOTES
%dir %{deploy}/data
%{deploy}/data/*
%{deploy}/data.dist

%files devel

%changelog
* Thu Oct 08 2026 Hawi Stecher <hawi.stecher@noirlab.edu> - 1.27-1
- Initial RPM packaging via the Linux cross-build (REL-5050). Source is V1-27
  plus the move off pisces-control to mkotcsbootv2-lv1 and absolute loads in
  startup.IS.
