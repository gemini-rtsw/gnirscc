[schematic2]
uniq 202
[tools]
[detail]
w 50 1051 100 0 n#201 hwin.hwin#199.in 80 1040 80 1040 ecad8.Cad.INPH
w 1588 1587 100 0 OMSS ecars.C.OMSS 1552 1632 1584 1632 1584 1552 1632 1552 outhier.OMSS.p
w 1052 1467 100 0 n#197 elongouts.initCar.OUT 928 1248 1056 1248 1056 1696 1232 1696 ecars.C.IVAL
w 1026 1315 100 0 n#196 elongouts.initCar.FLNK 928 1312 1184 1312 1184 1504 1232 1504 ecars.C.SLNK
w 642 1323 100 0 n#195 hwin.hwin#194.in 672 1312 672 1312 elongouts.initCar.DOL
w 1560 1475 100 0 FLNK outhier.FLNK.p 1616 1472 1552 1472 ecars.C.FLNK
w 1560 1667 100 0 OCID bihier.OCID.p 1616 1664 1552 1664 ecars.C.CLID
w 556 1059 100 0 n#186 ecad8.Cad.STLK 400 848 560 848 560 1280 672 1280 elongouts.initCar.SLNK
w 1548 1739 100 0 OVAL outhier.OVAL.p 1536 1792 1552 1792 1552 1696 ecars.C.VAL
w 888 1667 100 0 n#49 ecad8.Cad.OCID 400 1616 592 1616 592 1664 1232 1664 ecars.C.ICID
w -8 1683 100 0 n#40 inhier.CLID.P -144 1776 -48 1776 -48 1680 80 1680 ecad8.Cad.ICID
w 472 1683 100 0 MESS ecad8.Cad.MESS 400 1680 592 1680 592 1712 688 1712 outhier.MESS.p
w 590 1843 100 0 VAL ecad8.Cad.VAL 400 1712 528 1712 528 1840 688 1840 outhier.VAL.p
w -20 1787 100 0 DIR inhier.DIR.P -112 1872 -16 1872 -16 1712 80 1712 ecad8.Cad.DIR
w -510 1635 100 0 SLNKA inhier.SLNKA.P -544 1632 -416 1632 -416 1664 -368 1664 estringouts.A.SLNK
w -494 1411 100 0 SLNKB inhier.SLNKB.P -544 1408 -384 1408 -384 1472 -368 1472 estringouts.B.SLNK
w -480 1507 100 0 DOLB inhier.DOLB.P -544 1504 -368 1504 estringouts.B.DOL
w -488 1763 100 0 DOLA inhier.DOLA.P -544 1760 -384 1760 -384 1696 -368 1696 estringouts.A.DOL
w -40 1459 100 0 n#47 estringouts.B.OUT -112 1456 80 1456 ecad8.Cad.B
w -36 1579 100 0 n#46 estringouts.A.OUT -112 1648 -32 1648 -32 1520 80 1520 ecad8.Cad.A
[cell use]
use hwin -112 999 100 0 hwin#199
xform 0 -16 1040
p -109 1032 100 0 -1 val(in):$(top)applyC
use outhier 656 1799 100 0 VAL
xform 0 672 1840
use outhier 656 1671 100 0 MESS
xform 0 672 1712
use outhier 1584 1431 100 0 FLNK
xform 0 1600 1472
use outhier 1504 1751 100 0 OVAL
xform 0 1520 1792
use outhier 1600 1511 100 0 OMSS
xform 0 1616 1552
use bihier 1600 1623 100 0 OCID
xform 0 1616 1664
use hwin 480 1271 100 0 hwin#194
xform 0 576 1312
p 483 1304 100 0 -1 val(in):$(CAR_BUSY)
use elongouts 672 1191 100 0 initCar
xform 0 800 1280
use ecars 1232 1415 100 0 C
xform 0 1392 1584
p 1408 1360 100 1024 1 name:$(top)$(name)$(I)
use ecad8 80 759 100 0 Cad
xform 0 240 1264
p 176 1504 100 0 1 FTVA:LONG
p 176 1456 100 0 1 FTVB:LONG
p 192 1072 100 0 1 FTVH:LONG
p 144 736 100 0 1 INAM:$(name)InitCad
p 128 704 100 0 1 SNAM:$(name)Cad
p 240 656 100 1024 1 name:$(top)$(name)
use inhier -160 1735 100 0 CLID
xform 0 -144 1776
use inhier -560 1719 100 0 DOLA
xform 0 -544 1760
use inhier -560 1591 100 0 SLNKA
xform 0 -544 1632
use inhier -560 1463 100 0 DOLB
xform 0 -544 1504
use inhier -560 1367 100 0 SLNKB
xform 0 -544 1408
use inhier -128 1831 100 0 DIR
xform 0 -112 1872
use bb200tr -688 392 -100 0 frame
xform 0 592 1216
p 1104 560 100 0 -1 author:Peter Ruckle
p 1120 528 100 0 -1 date:1-26-2001
p 1376 608 200 0 -1 filename:init.sch
p 1344 656 200 0 -1 system:GNIRS CC
use estringouts -368 1591 100 0 A
xform 0 -240 1664
p -352 1568 100 0 1 OMSL:closed_loop
p -208 1536 100 1024 1 name:$(top)$(name)$(I)
use estringouts -368 1399 100 0 B
xform 0 -240 1472
p -384 1328 100 0 1 OMSL:closed_loop
p -240 1360 100 1024 1 name:$(top)$(name)$(I)
[comments]
