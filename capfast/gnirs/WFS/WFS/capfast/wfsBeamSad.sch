[schematic2]
uniq 126
[tools]
[detail]
w 900 1851 100 0 n#125 esirs.wfsBeam.INP 992 1952 896 1952 896 1760 832 1760 ecalcs.wfsBeamCalc.VAL
w 882 1803 100 0 n#124 ecalcs.wfsBeamCalc.FLNK 832 1792 992 1792 esirs.wfsBeam.SLNK
w 386 1955 100 0 n#117 ecalcs.wfsBeamCalc.INPA 544 1952 288 1952 elutouts.filtBeamFlag.VALA
w 444 1787 100 0 n#110 elutouts.filtBeamFlag.FLNK 288 2016 448 2016 448 1568 544 1568 ecalcs.wfsBeamCalc.SLNK
w 226 1571 100 0 n#110 elongins.filtBeamIdle.FLNK 64 1568 448 1568 junction
w 380 1723 100 0 n#111 ecalcs.wfsBeamCalc.INPB 544 1920 384 1920 384 1536 64 1536 elongins.filtBeamIdle.VAL
s 1584 0 400 1280 wfsBeamSad
[cell use]
use esirs 1056 1984 100 768 wfsBeam
xform 0 1200 1856
p 1056 1600 100 768 1 DESC:Wave-Front Sensor unobstructed? [YES=0|NO=1]
p 928 1504 100 0 0 EGU:0/1
p 1056 1664 100 768 1 FDSC:Wave-Front Sensor unobstructed? [YES=0|NO=1]
p 1056 1632 100 768 1 FTVL:LONG
p 1056 1696 100 768 1 PV:$(sadtop)
use ecalcs 608 1984 100 768 wfsBeamCalc
xform 0 688 1744
p 608 1440 100 768 1 CALC:A||B
p 608 1472 100 768 1 PV:$(sadtop)
use elutouts 96 2048 100 768 filtBeamFlag
xform 0 160 1856
p 96 1632 100 768 1 FDIR:$(pvdir)
p 96 1600 100 768 1 FNAM:filtBeam.lut
p 96 1568 100 768 1 PV:$(sadtop)
use elongins -128 1600 100 768 filtBeamIdle
xform 0 -64 1552
p -128 1472 100 768 1 PV:$(sadtop)
use bc200tr -960 -120 -100 0 frame
xform 0 720 1184
p 1616 48 100 0 1 author:H.T.Yamada
p 1840 32 100 0 -1 border:C
p 1616 16 100 0 1 checked:H.Yamada
p 1872 32 100 0 -1 date:1999-10-27
p 1840 160 100 0 -1 project:Gemini Near Infra-Red OIWFS
p 1616 160 100 768 1 revised:
p 1840 0 100 768 -1 revision:1.0
p 1840 96 100 0 -1 title:wfsBeam schematic
[comments]
