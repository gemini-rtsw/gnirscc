[schematic2]
uniq 214
[tools]
[detail]
w 658 1763 100 0 n#204 comp1pCadCmd.Tol.FLNK 640 1760 736 1760 combVal.combVal#197.SLNKA
w 658 1795 100 0 n#203 comp1pCadCmd.Tol.VAL 640 1792 736 1792 combVal.combVal#197.INPA
w 1056 1756 -100 1024 FLNK outhier.FLNK.p 1088 1760 1024 1760 combVal.combVal#197.FLNK
w 1048 1788 -100 1024 CVAL combVal.combVal#197.VAL 1024 1792 1072 1792 bihier.CVAL.p
w 1948 1883 100 0 n#137 elongins.Busy.INP 1984 1872 1984 1872 hwin.hwin#187.in
w -200 1859 100 0 n#87 eapplyx.Apply.OUTA -224 1856 -128 1856 comp1pCadCmd.Tol.DIR
w -200 1763 100 0 n#70 comp1pCadCmd.Tol.MESS -128 1760 -224 1760 eapplyx.Apply.INMA
w -200 1795 100 0 n#69 comp1pCadCmd.Tol.VAL -128 1792 -224 1792 eapplyx.Apply.INPA
w -200 1827 100 0 n#68 eapplyx.Apply.OCLA -224 1824 -128 1824 comp1pCadCmd.Tol.CLID
w -520 1763 100 0 n#13 eapplyx.Apply.MESS -480 1760 -512 1760 bihier.MESS.p
w -520 1795 100 0 n#12 eapplyx.Apply.VAL -480 1792 -512 1792 bihier.VAL.p
w -520 1827 100 0 n#10 bihier.CLID.p -512 1824 -480 1824 eapplyx.Apply.CLID
w -514 1859 100 0 n#9 bihier.DIR.p -512 1856 -480 1856 eapplyx.Apply.DIR
s 1808 -256 400 1280 folSetCad
n 1920 -32 2500 320 100
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
use bihier -544 1824 100 2048 CLID
xform 0 -512 1824
use bihier -544 1856 100 2048 DIR
xform 0 -512 1856
use bihier -544 1760 100 2048 MESS
xform 0 -512 1760
use bihier -544 1792 100 2048 VAL
xform 0 -512 1792
use bihier 1104 1792 100 1536 CVAL
xform 0 1072 1792
use outhier 1104 1760 100 1536 FLNK
xform 0 1072 1760
use combVal 752 71 100 0 combVal#197
xform 0 880 1040
p 736 128 100 768 1 set0:mech $(mech)Par
use hwin 1792 1831 100 0 hwin#187
xform 0 1888 1872
p 1795 1864 100 0 -1 val(in):-1
use elongins 2048 1888 100 768 Busy
xform 0 2112 1840
p 2048 1760 100 768 1 PV:$(top)$(mech)
use elongins 2048 1696 100 768 Select
xform 0 2112 1648
p 2048 1568 100 768 1 PV:$(top)$(mech)
use comp1pCadCmd 16 1888 100 1024 Tol
xform 0 256 1824
p -48 1856 100 768 1 set0:cad Tol
p -48 1824 100 768 1 set1:op $(opDiag)
p -48 1792 100 768 1 set2:inam CADcompInit
p -48 1760 100 768 1 set3:snam CADwfsSetTolerance
p -128 1696 100 768 1 setim:inpm $(top)$(mech1)Scale
p -128 1664 100 768 1 setin:inpn $(top)$(mech1)Offset
p -128 1632 100 768 1 setio:inpo $(top)$(mech2)Scale
p -128 1600 100 768 1 setip:inpp $(top)$(mech2)Offset
p -128 1568 100 768 1 setiq:inpq $(top)$(mech3)Scale
p -128 1536 100 768 1 setir:inpr $(top)$(mech3)Offset
p 256 1696 100 768 1 setom:outm $(top)$(mech1)EngTol PP NMS
p 256 1632 100 768 1 setoo:outo $(top)$(mech2)EngTol PP NMS
p 256 1568 100 768 1 setoq:outq $(top)$(mech3)EngTol PP NMS
use eapplyx -416 2016 100 768 Apply
xform 0 -352 1072
p -416 64 100 768 1 DESC:Interlock Apply Record
p -416 96 100 768 1 PV:$(top)$(mech)
use bc200tr -736 -376 -100 0 frame
xform 0 944 928
p 1824 -208 100 0 1 author:H.Yamada
p 2064 -224 100 0 -1 border:C
p 1824 -240 100 0 1 checked:H.T. Yamada
p 2096 -224 100 0 -1 date:1999-10-17
p 2080 -96 100 0 -1 project:NIRI/GNIRS WFS Interlocks
p 2080 -160 100 0 -1 title:Interlock Schematic
[comments]
