[schematic2]
uniq 192
[tools]
[detail]
w 434 1067 100 0 n#191 comp1Car.TmpC.FLNK 448 1056 480 1056 combCar.combCar.SLNKD
w 434 1091 100 0 n#190 comp1Car.TmpC.OCID 448 1088 480 1088 combCar.combCar.ICIDD
w 434 1123 100 0 n#189 comp1Car.TmpC.OVAL 448 1120 480 1120 combCar.combCar.IVALD
w 434 1347 100 0 n#188 comp1Car.CfgC.OVAL 448 1344 480 1344 combCar.combCar.IVALC
w 434 1315 100 0 n#187 comp1Car.CfgC.OCID 448 1312 480 1312 combCar.combCar.ICIDC
w 434 1291 100 0 n#186 comp1Car.CfgC.FLNK 448 1280 480 1280 combCar.combCar.SLNKC
w 434 1515 100 0 n#185 comp1Car.ObsC.FLNK 448 1504 480 1504 combCar.combCar.SLNKB
w 434 1539 100 0 n#184 comp1Car.ObsC.OCID 448 1536 480 1536 combCar.combCar.ICIDB
w 434 1571 100 0 n#183 comp1Car.ObsC.OVAL 448 1568 480 1568 combCar.combCar.IVALB
w 434 1739 100 0 n#182 comp1Car.GenC.FLNK 448 1728 480 1728 combCar.combCar.SLNKA
w 434 1763 100 0 n#181 comp1Car.GenC.OCID 448 1760 480 1760 combCar.combCar.ICIDA
w 434 1795 100 0 n#180 comp1Car.GenC.OVAL 448 1792 480 1792 combCar.combCar.IVALA
w 874 1731 -100 0 c#122 outhier.FLNK.p 944 1728 864 1728 combCar.combCar.FLNK
w 866 1763 -100 0 c#120 bihier.OCID.p 928 1760 864 1760 combCar.combCar.OCID
w 866 1795 -100 0 c#121 bihier.OVAL.p 928 1792 864 1792 combCar.combCar.OVAL
s 1808 -256 400 1280 lockCar
n 1408 -32 1988 320 100
This schematic contains all the database
records pertaining to the NIRI CC/WFS
and GNIRS WFS interlocks.
.
The schematic is divided into the
following subschematics:
.
lockCad  - Lock CAD records.
lockSad  - Lock Status/Alarm database
.
.
.
The SNL connects these databases.
_
[cell use]
use outhier 960 1728 100 1536 FLNK
xform 0 928 1728
use bihier 960 1760 100 1536 OCID
xform 0 928 1760
use bihier 960 1792 100 1536 OVAL
xform 0 928 1792
use combCar 672 1792 100 1024 combCar
xform 0 672 1280
use comp1Car -176 1792 100 1024 GenC
xform 0 64 1728
p -208 1664 100 768 1 FNAM:compPseudoSt.stpp
p -208 1632 100 768 1 SS:compPseudo_ss
p -240 1760 100 768 1 set0:mech $(mech)Gen
p -240 1728 100 768 1 set1:desc General Interlock
use comp1Car -176 1568 100 1024 ObsC
xform 0 64 1504
p -208 1440 100 768 1 FNAM:compPseudoSt.stpp
p -208 1408 100 768 1 SS:compPseudo_ss
p -240 1536 100 768 1 set0:mech $(mech)Obs
p -240 1504 100 768 1 set1:desc Observation Interlock
use comp1Car -176 1344 100 1024 CfgC
xform 0 64 1280
p -208 1216 100 768 1 FNAM:compPseudoSt.stpp
p -208 1184 100 768 1 SS:compPseudo_ss
p -240 1312 100 768 1 set0:mech $(mech)Cfg
p -240 1280 100 768 1 set1:desc Config Interlock
use comp1Car -176 1120 100 1024 TmpC
xform 0 64 1056
p -208 992 100 768 1 FNAM:compPseudoSt.stpp
p -208 960 100 768 1 SS:compPseudo_ss
p -240 1088 100 768 1 set0:mech $(mech)Tmp
p -240 1056 100 768 1 set1:desc Temperature Interlock
use bc200tr -736 -376 -100 0 frame
xform 0 944 928
p 1824 -208 100 0 1 author:H.T. Yamada
p 2064 -224 100 0 -1 border:C
p 1824 -240 100 0 1 checked:H.T. Yamada
p 2096 -224 100 0 -1 date:1999-10-17
p 2080 -96 100 0 -1 project:NIRI/GNIRS WFS Interlocks
p 2080 -160 100 0 -1 title:Interlock Schematic
[comments]
