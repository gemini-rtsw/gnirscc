# vxWorks 5.2b (pre-Tornado) 68k target headers + an x86-linux cross-toolchain.
#
# GEM5 has no Tornado: its vxWorks tree is VX_DIR=/usr/software/dev/packages/
# vxworks/v5.2b, and polaris compiles with cygnus gcc 2.2.3.1 from
# gnu/solaris.68k -- SPARC binaries. The host tools here are ANL's Linux
# rebuild of the Wind River Tornado 2.2 68k GNU tools
# (https://epics.anl.gov/base/tornado-linux.php), gcc 2.96, emitting a.out --
# the same format production's objects are. Installed as gnu/Linux.68k, the
# slot EPICS 3.12's CONFIG_COMMON names for HOST_ARCH=Linux:
#
#   VX_GNU_BIN = $(VX_DIR)/gnu/$(HOST_ARCH).$(ARCH_CLASS)/bin
#
# Verified to build gnirscc: 63/63 sources compile against these headers, all
# products link, exports match production, and every module resolves against
# the vxWorks 5.2 kernel's symbol table in boot order (gnirscc
# MIGRATION-PLAN.md 0c). Build-time only; the crate needs none of this.

%global _build_id_links none
%global __os_install_post %{nil}
%global debug_package %{nil}
%global vxdir /usr/software/dev/packages/vxworks/v5.2b

Name:           gem-vxworks52-linux
Version:        5.2b
Release:        1%{?dist}
Summary:        vxWorks 5.2b 68k target headers + x86-linux 68k cross-toolchain
License:        GPL (host tools) / Proprietary (target headers, org-internal)
AutoReqProv:    no
# cc68k and friends are 32-bit i386 ELF
Requires:       glibc(x86-32)

%description
The vxWorks 5.2b headers (h/) and BSP config (config/) from the Gemini GEM5
installation, plus cc68k/ld68k/ar68k/nm68k/objdump68k etc. as 32-bit Linux
binaries (ANL's rebuild of the Wind River 68k GNU tools, gcc 2.96) under
gnu/Linux.68k. Installs at %{vxdir}, the VX_DIR the GEM5 build names.

The Solaris (SPARC) and sun4/mips toolchains of the original installation are
deliberately excluded; they cannot run here.

%install
mkdir -p %{buildroot}%{vxdir}
cp -a %{trees}%{vxdir}/h %{trees}%{vxdir}/config %{trees}%{vxdir}/README \
      %{buildroot}%{vxdir}/
mkdir -p %{buildroot}%{vxdir}/gnu/Linux.68k
tar xzf %{trees}/toolchain/gnu-tools.tor2_2-m68k-rhel5.tgz -C %{_builddir}
cp -a %{_builddir}/host/x86-linux/. %{buildroot}%{vxdir}/gnu/Linux.68k/

# Fail the build rather than ship a toolchain that cannot compile: these are
# the binaries CONFIG.Vx.68k invokes by name...
for f in cc68k cpp68k ld68k ar68k ranlib68k nm68k; do
    [ -x "%{buildroot}%{vxdir}/gnu/Linux.68k/bin/$f" ] || {
        echo "ERROR: missing gnu/Linux.68k/bin/$f" >&2; exit 1; }
done
# ...the per-CPU libgcc the -m68040 build links against...
[ -d %{buildroot}%{vxdir}/gnu/Linux.68k/lib/gcc-lib/m68k-wrs-vxworks/gcc-2.96/MC68040gnu ] || {
    echo "ERROR: missing the MC68040 gcc-lib" >&2; exit 1; }
# ...and the target headers, without which nothing compiles at all.
[ -f %{buildroot}%{vxdir}/h/vxWorks.h ] || { echo "ERROR: missing h/vxWorks.h" >&2; exit 1; }
grep -q '"5.2 Rev B"' %{buildroot}%{vxdir}/h/version.h || {
    echo "ERROR: h/version.h is not vxWorks 5.2 Rev B" >&2; exit 1; }

%files
%{vxdir}

%changelog
* Thu Oct 08 2026 Hawi Stecher <hawi.stecher@noirlab.edu> - 5.2b-1
- Initial packaging for the gnirscc Linux rehost (REL-5050). Target headers
  and config are vxWorks 5.2 Rev B from polaris; host tools are ANL's
  gnu-tools.tor2_2-m68k-rhel5.tgz, sha256
  3ea24b7322d815ec1e8f438ee5faf3ce00cbd4219574ed23621858728376fc7e
  (GPL source cum.tor2_2-m68k.tgz, sha256
  1e0fa2c16cc61478dd32a3a2170bf4aeac8a445e4810a88ab27bb299d3a24a24).
