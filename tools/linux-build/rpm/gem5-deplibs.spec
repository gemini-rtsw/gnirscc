# The GEM5 support libraries astlib, slalib and timelib -- prebuilt mv167
# objects plus headers -- at the /gemini paths the GEM5 IOCs use, with the
# version-selecting links that name them.
#
# Both a runtime and a build dependency: gnirscc's startup.IS does
#
#   ld < /gemini/slalib/bin/mv167/slalib
#   ld < /gemini/timelib/bin/mv167/timelib
#   ld < /gemini/astlib/bin/mv167/astlib
#
# and its build resolves APPLIC_DEPENDS = /gemini/astlib /gemini/timelib for
# headers. For vxWorks the copy on the file server IS the running code, so the
# versions are pinned here and the package fails if the links point elsewhere
# -- which is what rpm -q can now tell you and a bare symlink never could (the
# same reasoning as gmoscc's gmos-deplibs).
#
# Copied, not rebuilt: these are 1999-2000 cygnus-built objects, loaded today,
# beside which gnirscc's gcc 2.96 objects have been verified to resolve
# (gnirscc MIGRATION-PLAN.md 0c). Their source is in the GEM5 EPICS tree's
# extensions/src/gemini if they ever need rebuilding.

%global _binaries_in_noarch_packages_terminate_build 0
%global _build_id_links none
%global __os_install_post %{nil}
%global debug_package %{nil}

# The versions the live /gemini links select (pisces, 2026-10-08).
%global astlib_dir  astlib-1.3
%global slalib_dir  slalib-1.1
%global timelib_dir timelib-1.3

Name:           gem5-deplibs
Version:        2026.10.08
Release:        1%{?dist}
Summary:        GEM5 astlib 1.3, slalib 1.1, timelib 1.3 for mv167 crates
License:        Proprietary Gemini (org-internal)
AutoReqProv:    no
BuildArch:      noarch
Provides:       gem5-astlib = 1.3
Provides:       gem5-slalib = 1.1
Provides:       gem5-timelib = 1.3

%description
astlib 1.3, slalib 1.1 and timelib 1.3 (with timeSeq) as prebuilt mv167
objects and headers at /gemini/{astlib-1.3,slalib-1.1,timelib-1.3}, plus the
links /gemini/{astlib,slalib,timelib} that GEM5 startup scripts and builds
name. Install on the GEM5 build host and on the boot server.

%install
mkdir -p %{buildroot}/gemini
for d in %{astlib_dir} %{slalib_dir} %{timelib_dir}; do
    cp -a %{trees}/gemini/$d %{buildroot}/gemini/
    # SPARC host archives; nothing on Linux can use them.
    rm -rf %{buildroot}/gemini/$d/lib/solaris
    rmdir %{buildroot}/gemini/$d/lib 2>/dev/null || :
done
ln -sfn %{astlib_dir}  %{buildroot}/gemini/astlib
ln -sfn %{slalib_dir}  %{buildroot}/gemini/slalib
ln -sfn %{timelib_dir} %{buildroot}/gemini/timelib

# A broken link here is a failed boot, not a missing file.
for f in astlib/bin/mv167/astlib slalib/bin/mv167/slalib timelib/bin/mv167/timelib \
         timelib/bin/mv167/timeSeq astlib/include/astLib.h timelib/include/timeLib.h \
         slalib/include/slalib.h; do
    [ -f "%{buildroot}/gemini/$f" ] || { echo "ERROR: /gemini/$f does not resolve" >&2; exit 1; }
done

%files
%defattr(-,root,root,-)
/gemini/%{astlib_dir}
/gemini/%{slalib_dir}
/gemini/%{timelib_dir}
/gemini/astlib
/gemini/slalib
/gemini/timelib

%changelog
* Thu Oct 08 2026 Hawi Stecher <hawi.stecher@noirlab.edu> - 2026.10.08-1
- Initial packaging for the gnirscc Linux rehost (REL-5050), from
  pisces:/export/gemini, pinning the versions the live links select:
  astlib -> astlib-1.3, slalib -> slalib-1.1, timelib -> timelib-1.3.
