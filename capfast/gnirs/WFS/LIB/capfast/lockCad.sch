[schematic2]
uniq 203
[tools]
[detail]
w 626 1763 100 0 n#202 comp1CadSel.Gen.CFLNK 640 1760 672 1760 combVal.combVal#151.SLNKA
w 626 1795 100 0 n#201 comp1CadSel.Gen.CVAL 640 1792 672 1792 combVal.combVal#151.INPA
w 626 1539 100 0 n#200 comp1CadSel.Obs.CFLNK 640 1536 672 1536 combVal.combVal#151.SLNKB
w 626 1571 100 0 n#199 comp1CadSel.Obs.CVAL 640 1568 672 1568 combVal.combVal#151.INPB
w 626 1315 100 0 n#198 comp1CadSel.Cfg.CFLNK 640 1312 672 1312 combVal.combVal#151.SLNKC
w 626 1347 100 0 n#197 comp1CadSel.Cfg.CVAL 640 1344 672 1344 combVal.combVal#151.INPC
w 626 1091 100 0 n#196 comp1CadSel.Tmp.CFLNK 640 1088 672 1088 combVal.combVal#151.SLNKD
w 626 1123 100 0 n#195 comp1CadSel.Tmp.CVAL 640 1120 672 1120 combVal.combVal#151.INPD
w -206 1827 100 0 n#194 comp1CadSel.Gen.CLID -128 1824 -224 1824 eapplyx.Apply.OCLA
w -206 1859 100 0 n#193 comp1CadSel.Gen.DIR -128 1856 -224 1856 eapplyx.Apply.OUTA
w -206 1795 100 0 n#192 comp1CadSel.Gen.VAL -128 1792 -224 1792 eapplyx.Apply.INPA
w -206 1763 100 0 n#191 comp1CadSel.Gen.MESS -128 1760 -224 1760 eapplyx.Apply.INMA
w -206 1603 100 0 n#190 comp1CadSel.Obs.CLID -128 1600 -224 1600 eapplyx.Apply.OCLB
w -206 1635 100 0 n#189 comp1CadSel.Obs.DIR -128 1632 -224 1632 eapplyx.Apply.OUTB
w -206 1571 100 0 n#188 comp1CadSel.Obs.VAL -128 1568 -224 1568 eapplyx.Apply.INPB
w -206 1539 100 0 n#187 comp1CadSel.Obs.MESS -128 1536 -224 1536 eapplyx.Apply.INMB
w -206 1379 100 0 n#186 comp1CadSel.Cfg.CLID -128 1376 -224 1376 eapplyx.Apply.OCLC
w -206 1411 100 0 n#185 comp1CadSel.Cfg.DIR -128 1408 -224 1408 eapplyx.Apply.OUTC
w -206 1347 100 0 n#184 comp1CadSel.Cfg.VAL -128 1344 -224 1344 eapplyx.Apply.INPC
w -206 1315 100 0 n#183 comp1CadSel.Cfg.MESS -128 1312 -224 1312 eapplyx.Apply.INMC
w -206 1155 100 0 n#182 comp1CadSel.Tmp.CLID -128 1152 -224 1152 eapplyx.Apply.OCLD
w -206 1187 100 0 n#181 comp1CadSel.Tmp.DIR -128 1184 -224 1184 eapplyx.Apply.OUTD
w -206 1123 100 0 n#180 comp1CadSel.Tmp.VAL -128 1120 -224 1120 eapplyx.Apply.INPD
w -206 1091 100 0 n#179 comp1CadSel.Tmp.MESS -128 1088 -224 1088 eapplyx.Apply.INMD
w 962 1771 -100 0 CFLNK combVal.combVal#151.FLNK 960 1760 1024 1760 outhier.CFLNK.p
w 954 1803 -100 0 CVAL bihier.CVAL.p 1008 1792 960 1792 combVal.combVal#151.VAL
w -520 1763 100 0 n#13 eapplyx.Apply.MESS -480 1760 -512 1760 bihier.MESS.p
w -520 1795 100 0 n#12 eapplyx.Apply.VAL -480 1792 -512 1792 bihier.VAL.p
w -520 1827 100 0 n#10 bihier.CLID.p -512 1824 -480 1824 eapplyx.Apply.CLID
w -514 1859 100 0 n#9 bihier.DIR.p -512 1856 -480 1856 eapplyx.Apply.DIR
s 1808 -256 400 1280 lock
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
use bihier 1040 1792 100 1536 CVAL
xform 0 1008 1792
use bihier -544 1792 100 2048 VAL
xform 0 -512 1792
use bihier -544 1760 100 2048 MESS
xform 0 -512 1760
use bihier -544 1856 100 2048 DIR
xform 0 -512 1856
use bihier -544 1824 100 2048 CLID
xform 0 -512 1824
use outhier 1040 1760 100 1536 CFLNK
xform 0 1008 1760
use combVal 688 71 100 0 combVal#151
xform 0 816 1040
use comp1CadSel 16 1216 100 1024 Tmp
xform 0 256 1152
p -48 1184 100 768 1 set0:cad Sel
p 272 1184 100 768 1 set1:op $(opSel)
p -48 1152 100 768 1 set2:inam CADcompInit
p 272 1152 100 768 1 set3:snam CADcompSel
p -48 1120 100 768 1 set4:ignore 1
p -48 1088 100 768 1 set5:desc Set $(desc4)
p -48 1056 100 768 1 set6:mech $(mech4)
use comp1CadSel 16 1440 100 1024 Cfg
xform 0 256 1376
p -48 1408 100 768 1 set0:cad Sel
p 272 1408 100 768 1 set1:op $(opSel)
p -48 1376 100 768 1 set2:inam CADcompInit
p 272 1376 100 768 1 set3:snam CADcompSel
p -48 1344 100 768 1 set4:ignore 1
p -48 1312 100 768 1 set5:desc Set $(desc3)
p -48 1280 100 768 1 set6:mech $(mech3)
use comp1CadSel 16 1664 100 1024 Obs
xform 0 256 1600
p -48 1632 100 768 1 set0:cad Sel
p 272 1632 100 768 1 set1:op $(opSel)
p -48 1600 100 768 1 set2:inam CADcompInit
p 272 1600 100 768 1 set3:snam CADcompSel
p -48 1568 100 768 1 set4:ignore 1
p -48 1536 100 768 1 set5:desc Set $(desc2)
p -48 1504 100 768 1 set6:mech $(mech2)
use comp1CadSel 16 1888 100 1024 Gen
xform 0 256 1824
p -48 1856 100 768 1 set0:cad Sel
p 272 1856 100 768 1 set1:op $(opSel)
p -48 1824 100 768 1 set2:inam CADcompInit
p 272 1824 100 768 1 set3:snam CADcompSel
p -48 1792 100 768 1 set4:ignore 1
p -48 1760 100 768 1 set5:desc Set $(desc1)
p -48 1728 100 768 1 set6:mech $(mech1)
use eapplyx -416 2016 100 768 Apply
xform 0 -352 1072
p -416 64 100 768 1 DESC:Interlock Apply Record
p -416 96 100 768 1 PV:$(top)$(mech)
use bc200tr -736 -376 -100 0 frame
xform 0 944 928
p 1824 -208 100 0 1 author:H.T. Yamada
p 2064 -224 100 0 -1 border:C
p 1824 -240 100 0 1 checked:H.T. Yamada
p 2096 -224 100 0 -1 date:1999-10-17
p 2080 -96 100 0 -1 project:NIRI/GNIRS WFS Interlocks
p 2080 -160 100 0 -1 title:Interlock Schematic
[comments]
