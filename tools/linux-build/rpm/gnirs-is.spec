# The GNIRS Instrument Sequencer V1-7 (Dec 2012), frozen and prebuilt.
#
# There is no separate IS IOC: gnirscc's startup.IS loads it into the CC crate,
#
#   ld < /gemini/GEM5/gnirs/IS/IS/bin/mv167/gmSeqAppl
#   dbLoadRecords /gemini/GEM5/gnirs/IS/IS/data/nirsSeq{,Sad}Top.db
#   pvload /gemini/GEM5/gnirs/IS/IS/data/{gmSeq,startup,loadFOC*}.pv
#
# so the boot server must carry it. Its source is still in CVS and has not been
# rebuilt since 2012; packaging the deployed tree verbatim makes `rpm -q` name
# it. Porting the source is separate work.
#
# The focus tables are TUNED IN OPERATIONS (focus{LB,LR,SB,SR}-{imaging,
# spatial,spectral}.dat, edited and backed up by date through 2026). The
# package must never overwrite them, so it does not own them: it ships the
# deployed copies under data.dist/ and %post copies one into data/ only if
# data/ has none. A fresh server gets working tables; a live one is never
# touched. Dated backups are operational history and are not shipped.

%global _binaries_in_noarch_packages_terminate_build 0
%global _build_id_links none
%global __os_install_post %{nil}
%global debug_package %{nil}
%global isdir /gemini/GEM5/gnirs/IS/IS
%global gmseq_sha256 056c5dfb248c65e9597a9dbe8c97ca82f96d3967a8359b932a0172690dbc77a7
%global tuned focusLB-imaging.dat focusLB-spatial.dat focusLB-spectral.dat focusLR-imaging.dat focusLR-spatial.dat focusLR-spectral.dat focusSB-imaging.dat focusSB-spatial.dat focusSB-spectral.dat focusSR-imaging.dat focusSR-spatial.dat focusSR-spectral.dat

Name:           gnirs-is
Version:        1.7
Release:        1%{?dist}
Summary:        GNIRS Instrument Sequencer V1-7 (prebuilt, loaded by the gnirscc crate)
License:        Proprietary Gemini (org-internal)
AutoReqProv:    no
BuildArch:      noarch
Requires:       gem5-epics-runtime

%description
The deployed GNIRS Instrument Sequencer V1-7 tree at %{isdir}: gmSeqAppl
(a.out mc68020, built Dec 2012) and the startup scripts, databases, .pv and
LUT files the GNIRS CC crate's startup.IS loads. The operations-tuned focus
tables are shipped as defaults under data.dist/ and installed only where none
exist.

%install
D=%{buildroot}%{isdir}
mkdir -p $D/data $D/data.dist
cp -a %{trees}%{isdir}/bin %{trees}%{isdir}/include %{trees}%{isdir}/RELEASE.NOTES $D/
rm -rf $D/include/CVS $D/include/.cvsignore
for f in %{trees}%{isdir}/data/*; do
    n=$(basename "$f")
    case "$n" in
        *.20[0-9][0-9].*|README.*) continue ;;   # dated backups, ops notes
    esac
    cp -p "$f" $D/data/
done
for t in %{tuned}; do
    [ -f $D/data/$t ] || { echo "ERROR: tuned file $t missing from the source tree" >&2; exit 1; }
    mv $D/data/$t $D/data.dist/
done

# Every file startup.IS names.
for f in bin/mv167/gmSeqAppl data/nirsSeqTop.db data/nirsSeqSadTop.db data/gmSeq.pv \
         data/startup.pv data/loadFOCimaging.pv data/loadFOCspectral.pv data/loadFOCspatial.pv; do
    [ -f "$D/$f" ] || { echo "ERROR: missing $f -- startup.IS loads it" >&2; exit 1; }
done
echo "%{gmseq_sha256}  $D/bin/mv167/gmSeqAppl" | sha256sum -c -

%post
# Seed the tuned focus tables only where data/ has none. Never overwrite.
for t in %{tuned}; do
    [ -e %{isdir}/data/$t ] || cp -p %{isdir}/data.dist/$t %{isdir}/data/$t
done
:

%files
%defattr(-,root,root,-)
%dir %{isdir}
%{isdir}/bin
%{isdir}/include
%{isdir}/RELEASE.NOTES
%dir %{isdir}/data
%{isdir}/data/*
%{isdir}/data.dist

%changelog
* Thu Oct 08 2026 Hawi Stecher <hawi.stecher@noirlab.edu> - 1.7-1
- Initial packaging for the gnirscc Linux rehost (REL-5050) of the deployed
  pisces:/export/gemini/GEM5/gnirs/IS/IS (V1-7, built 3 Dec 2012).
  gmSeqAppl sha256
  056c5dfb248c65e9597a9dbe8c97ca82f96d3967a8359b932a0172690dbc77a7; focus tables as of 2026-10-08 under
  data.dist/. NOTE data/README.2024.05.20 on pisces records an open
  operational workaround (SR focus tables overwritten with SB copies); it is
  not shipped and is not addressed here.
