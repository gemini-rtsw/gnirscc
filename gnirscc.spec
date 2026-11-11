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
#
# CONFIG SUB-REPO. The tuned files (and every dated backup the scientists keep
# beside them) live in github.com/gemini-rtsw/gnirscc-config, a submodule at
# gnirscc-config/. data.dist/ is seeded from it, and the package ships its git
# history so that, on first install, data/ becomes a git work tree of that repo
# (data/.git). That step is additive only: it never overwrites or deletes a
# file in data/, never runs git, and cannot fail the install (see %%posttrans).

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

# The config sub-repo's git history is read from the checkout: the source
# tarball carries the submodule's files but not its .git. Override with
# --define "cfgrepo <path>" when rpmbuild does not run from the checkout root.
%{!?cfgrepo:%global cfgrepo %(pwd)/gnirscc-config}
%global cfgremote git@github.com:gemini-rtsw/gnirscc-config.git

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
BuildRequires:  git, tar, gzip

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

# Config sub-repo: a shallow clone at the pinned submodule commit, on main,
# tracking origin/main (as bootsmith ships bootsmith-config). CI checks out
# submodules at depth 1, so one commit is all the history there is; on a host
# that reaches GitHub, git fetch --unshallow gets the rest.
for t in %{tuned}; do
    [ -f gnirscc-config/$t ] || { echo "ERROR: gnirscc-config/$t missing -- git submodule update --init" >&2; exit 1; }
done
pin=$(git -c safe.directory='*' -C "%{cfgrepo}" rev-parse HEAD)
rm -rf config-repo
git -c safe.directory='*' clone -q --depth 1 --no-local "file://%{cfgrepo}" config-repo
git -C config-repo config remote.origin.fetch '+refs/heads/*:refs/remotes/origin/*'
git -C config-repo checkout -q -B main
git -C config-repo update-ref refs/remotes/origin/main HEAD
git -C config-repo branch -q --set-upstream-to=origin/main main
git -C config-repo remote set-url origin %{cfgremote}
# Sci/ops commit from their own accounts: new objects world-writable.
git -C config-repo config core.sharedRepository world
[ "$(git -C config-repo rev-parse HEAD)" = "$pin" ] || { echo "ERROR: config-repo is not at the submodule pin $pin" >&2; exit 1; }
# The shipped work tree must be the same files the source tarball carries.
diff -r -x .git config-repo gnirscc-config >/dev/null || { echo "ERROR: gnirscc-config files differ from commit $pin" >&2; exit 1; }
tar -C config-repo -czf config-repo.tar.gz .

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
# The tuned files are never packaged in data/. Their defaults in data.dist/
# come from the config sub-repo, not from this repo's data/ copies.
for t in %{tuned}; do
    [ -f $D/data/$t ] || { echo "ERROR: tuned file $t missing from the build" >&2; exit 1; }
    rm -f $D/data/$t
    cp -p gnirscc-config/$t $D/data.dist/$t
done
install -Dpm 0644 config-repo.tar.gz $RPM_BUILD_ROOT%{_datadir}/%{name}/config-repo.tar.gz

%post
# Seed the tuned configuration only where data/ has none. Never overwrite.
for t in %{tuned}; do
    [ -e %{deploy}/data/$t ] || cp -p %{deploy}/data.dist/$t %{deploy}/data/$t
done
# Sci/ops edit the tuned files and write their dated backups from their own
# accounts, so data/ and the tuned files are writable by any user.
for t in %{tuned}; do
    chmod 0666 %{deploy}/data/$t
done
# data/ is root's and git refuses repos owned by someone else; allow this one.
# Only if git is installed: the package does not require it.
if command -v git >/dev/null 2>&1; then
    git config --system --get-all safe.directory 2>/dev/null | grep -qx '%{deploy}/data' || \
        git config --system --add safe.directory '%{deploy}/data' || :
fi
:

%posttrans
# First install only (no data/.git yet): make data/ a git work tree of
# gnirscc-config. Strictly additive:
#  - a file already in data/ is never touched, so live tuned files, the
#    package's own files and local backups stay as they are; git status then
#    shows how they differ from the repo;
#  - repo files missing from data/ (e.g. dated backups) are copied in;
#  - .git is moved in last, so a half-done run leaves no .git and the next
#    install or upgrade simply tries again.
# No git needed. Any failure only prints a warning: the install still succeeds.
d=%{deploy}/data
tb=%{_datadir}/%{name}/config-repo.tar.gz
if [ -d "$d" ] && [ ! -e "$d/.git" ] && [ -r "$tb" ]; then
    t=$(mktemp -d "%{deploy}/.config-repo.XXXXXX" 2>/dev/null) || t=
    if [ -n "$t" ] && [ -d "$t" ] && tar -xzf "$tb" -C "$t" --no-same-owner; then
        (cd "$t" && find . -path ./.git -prune -o -type f -print) | while IFS= read -r f; do
            f=${f#./}
            if [ ! -e "$d/$f" ] && [ ! -L "$d/$f" ]; then
                mkdir -p "$(dirname "$d/$f")" && cp -p "$t/$f" "$d/$f"
            fi
        done
        if chmod -R a+rwX "$t/.git" && mv "$t/.git" "$d/.git"; then
            echo "gnirscc: $d is now a git work tree of gnirscc-config (git status there)" >&2
        else
            echo "gnirscc: WARNING: could not set up $d/.git; the IOC files are unaffected" >&2
        fi
    else
        echo "gnirscc: WARNING: could not unpack $tb; the IOC files are unaffected" >&2
    fi
    [ -n "$t" ] && rm -rf "$t"
fi
exit 0

%files
%defattr(-,root,root,-)
%dir %{deploy}
%{deploy}/bin
%{deploy}/include
%{deploy}/RELEASE.NOTES
%dir %attr(0777,root,root) %{deploy}/data
%{deploy}/data/*
%{deploy}/data.dist
%dir %{_datadir}/%{name}
%{_datadir}/%{name}/config-repo.tar.gz

%files devel

%changelog
* Tue Nov 10 2026 Hawi Stecher <hawi.stecher@noirlab.edu> - 1.27-1
- Tuned files move to the gnirscc-config sub-repo: data.dist/ is seeded from
  it, and on first install data/ becomes a git work tree of it (additive
  only; no file in data/ is overwritten).

* Mon Nov 02 2026 Hawi Stecher <hawi.stecher@noirlab.edu> - 1.27-1
- data/ and the tuned files are writable by any user so sci/ops can edit
  them in place (GNFR-75641).
- GNFR-75641 one direction camera mode.

* Thu Oct 08 2026 Hawi Stecher <hawi.stecher@noirlab.edu> - 1.27-1
- Initial RPM packaging via the Linux cross-build (REL-5050). Source is V1-27
  plus the move off pisces-control to mkotcsbootv2-lv1 and absolute loads in
  startup.IS.
