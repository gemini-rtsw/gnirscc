[schematic2]
uniq 907
[tools]
[detail]
w 170 1827 100 0 n#906 bihier.VAL.p 176 1824 224 1824 compCad.cad.VAL
w 170 1795 100 0 n#905 bihier.MESS.p 176 1792 224 1792 compCad.cad.MESS
w 1042 1635 100 0 n#896 compSnlCmd.snl1.ISTL 1088 1632 1056 1632 compCad.cad.STLKA
w 1042 1667 100 0 n#895 compSnlCmd.snl1.ARG 1088 1664 1056 1664 compCad.cad.OUTA
w 1042 1603 100 0 n#894 compSnlCmd.snl1.ISPL 1088 1600 1056 1600 compCad.cad.SPLKA
w 1042 1699 100 0 n#893 compSnlCmd.snl1.ICID 1088 1696 1056 1696 compCad.cad.OCIDA
w 1050 1827 -100 0 c#888 compCad.cad.CVAL 1056 1824 1104 1824 bihier.CVAL.p
w 1058 1795 -100 0 c#886 outhier.CFLNK.p 1120 1792 1056 1792 compCad.cad.CFLNK
w 170 1859 100 0 n#121 bihier.CLID.p 176 1856 224 1856 compCad.cad.CLID
w 176 1891 100 0 n#94 bihier.DIR.p 176 1888 224 1888 compCad.cad.DIR
s 2032 192 400 1280 comp1mCadCmd
n -384 224 96 576 100
This schematic contains a "generic" CAD
record, which takes an op code, which
requires no parameters, and passes it to
the corresponding SNL code.  By default,
no subroutine is needed, because the
output links trigger all necessary
actions.
.
.
.
.
.
.
.
_
[cell use]
use bihier 144 1824 100 2048 VAL
xform 0 176 1824
use bihier 144 1792 100 2048 MESS
xform 0 176 1792
use bihier 1136 1824 100 1536 CVAL
xform 0 1104 1824
use bihier 144 1856 100 2048 CLID
xform 0 176 1856
use bihier 144 1888 100 2048 DIR
xform 0 176 1888
use outhier 1136 1792 100 1536 CFLNK
xform 0 1104 1792
use compCad 640 1920 100 1024 cad
xform 0 640 1312
p 336 1664 100 768 1 setia:inpa $(sadtop)$(mech)EngCyclic.VAL
p 336 1632 100 768 1 setib:inpb $(sadtop)$(mech)EngMin.VAL
p 336 1600 100 768 1 setic:inpc $(sadtop)$(mech)EngMax.VAL
p 336 1760 100 768 1 setr:ignore $(ignore)
p 336 1728 100 768 1 sets:datumed $(sadtop)$(mech)Datumed.VAL
use compSnlCmd 1280 1696 100 1024 snl1
xform 0 1280 1648
p 1168 1632 100 768 1 set0:mech $(mech)
use bc200tr -512 72 -100 0 frame
xform 0 1168 1376
p 2064 240 100 0 1 author:H.Yamada
p 2288 224 100 0 -1 border:C
p 2064 192 100 768 1 checked:H.Yamada
p 2320 224 100 0 -1 date:1999-11-08
p -512 72 100 0 0 id:frame
p 2288 352 100 0 -1 project:Near Infra-Red Imager
p -512 120 100 0 0 revision:1.0
p 2288 288 100 0 -1 title:One-Axis Generic Command CAD record
[comments]
