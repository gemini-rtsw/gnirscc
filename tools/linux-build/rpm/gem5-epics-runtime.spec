# EPICS 3.12.2 GEM5 *target* binaries, and the GEM5 vxWorks 5.2 kernel, at the
# path the crates load them from.
#
# Not to be confused with gem-epics3122gem5, the BUILD tree under
# /usr/software. This is what the IOC needs: gnirscc's startup.IS does
#
#   ld < /gemini/external/GEM5/base/bin/mv167/{iocCore,drvSup,recSup,devSup}
#   ld < /gemini/external/GEM5/extensions/bin/mv167/pvload
#
# and nothing packaged them -- they exist on pisces only as a 1998 copy.
#
# It also carries the KERNEL. The GNIRS CC crate's boot parameters have long
# named /home/gemvx/cristian/epics/GEM5/base/bin/mv167/vxWorks, a developer's
# home directory on pisces. That file and its vxWorks.sym are bit-identical to
# the ones at /gemini/external/GEM5/base/bin/mv167 (sha256 below; vxWorks
# loads <file name>.sym from beside the kernel), so the new boot parameters
# name this package's copy and boot the same kernel bit for bit. The boot
# log's "Loading... 457932 + 50744 + 26218" is exactly its a.out
# text+data+bss.

%global _binaries_in_noarch_packages_terminate_build 0
%global _build_id_links none
%global __os_install_post %{nil}
%global debug_package %{nil}
%global gem5 /gemini/external/GEM5
%global kernel_sha256 fdec7df754a2098171718defa2f86869ec1c81339068b268a5ff239166bc9e9f
%global ksym_sha256   8e2045c5de4d9971ba0926e3fe5afecf3ae013162055f9e682ff8de1e67cc051

Name:           gem5-epics-runtime
Version:        3.12.2
Release:        1%{?dist}
Summary:        GEM5 EPICS target binaries and vxWorks 5.2 kernel served to mv167 crates
License:        EPICS Open License / Proprietary (Wind River / Gemini) -- org-internal
AutoReqProv:    no
# noarch: mv167 objects the host never executes, only serves over NFS/rsh.
BuildArch:      noarch

%description
The EPICS 3.12.2 GEM5 base and extensions mv167 binaries (iocCore, drvSup,
recSup, devSup, seq, pvload and companions) at %{gem5}, the path vxWorks
IOCs load them from over NFS, plus the vxWorks 5.2 kernel and symbol table
(base/bin/mv167/vxWorks{,.sym}) the GNIRS CC crate boots over rsh. Install
on the boot server that exports /gemini.

%install
mkdir -p %{buildroot}%{gem5}/base/bin %{buildroot}%{gem5}/extensions/bin
cp -a %{trees}%{gem5}/base/bin/mv167 %{buildroot}%{gem5}/base/bin/
cp -a %{trees}%{gem5}/extensions/bin/mv167 %{buildroot}%{gem5}/extensions/bin/

# Fail the build rather than ship a package that boots a crate into nothing:
# each of these is named literally by gnirscc's startup.IS.
for f in base/bin/mv167/iocCore base/bin/mv167/drvSup base/bin/mv167/recSup \
         base/bin/mv167/devSup extensions/bin/mv167/pvload; do
    [ -f "%{buildroot}%{gem5}/$f" ] || {
        echo "ERROR: missing $f -- crates load this at boot" >&2; exit 1; }
done
# The kernel must be THE kernel: a real file with the recorded hash.
K=%{buildroot}%{gem5}/base/bin/mv167
for f in vxWorks vxWorks.sym; do
    [ -f "$K/$f" ] && [ ! -L "$K/$f" ] || { echo "ERROR: $f is not a real file" >&2; exit 1; }
done
echo "%{kernel_sha256}  $K/vxWorks"     | sha256sum -c -
echo "%{ksym_sha256}  $K/vxWorks.sym" | sha256sum -c -

%files
%defattr(-,root,root,-)
%{gem5}

%changelog
* Thu Oct 08 2026 Hawi Stecher <hawi.stecher@noirlab.edu> - 3.12.2-1
- Initial packaging for the gnirscc Linux rehost (REL-5050), from
  pisces:/export/gemini/external/GEM5. Kernel base/bin/mv167/vxWorks
  (vxWorks 5.2, WIND 2.4, MVME167, BSP 1.0, Nov 1998) sha256
  fdec7df754a2098171718defa2f86869ec1c81339068b268a5ff239166bc9e9f,
  vxWorks.sym sha256
  8e2045c5de4d9971ba0926e3fe5afecf3ae013162055f9e682ff8de1e67cc051 --
  identical to the copies the crate booted from /home/gemvx/cristian.
