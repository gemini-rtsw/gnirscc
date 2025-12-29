[schematic2]
uniq 144
[tools]
[detail]
w -668 1419 100 0 n#142 embbis.simulMode.FLNK -864 1792 -672 1792 -672 1056 -448 1056 egenSub.simCalc.SLNK
w -686 1771 100 0 n#141 embbis.simulMode.VAL -864 1760 -448 1760 egenSub.simCalc.A
w 834 427 100 0 n#133 ebos.isFULL.VAL 704 416 1024 416 outhier.notFull.p
w 834 651 100 0 n#132 ebos.notFULL.VAL 704 640 1024 640 outhier.isFull.p
w 834 875 100 0 n#131 ebos.notFAST.VAL 704 864 1024 864 outhier.notFast.p
w 834 1099 100 0 n#130 ebos.isFAST.VAL 704 1088 1024 1088 outhier.isFast.p
w 834 1323 100 0 n#129 ebos.notVSM.VAL 704 1312 1024 1312 outhier.notVsm.p
w 834 1547 100 0 n#128 ebos.isVSM.VAL 704 1536 1024 1536 outhier.isVsm.p
w 786 1731 100 0 n#121 ebos.simulating.VAL 704 1728 928 1728 928 1760 1024 1760 outhier.simulatingOut.p
w -1310 1675 100 0 n#120 inhier.SLNK.P -1376 1664 -1184 1664 -1184 1776 -1120 1776 embbis.simulMode.SLNK
w -1326 1835 100 0 n#119 inhier.simModeIn.P -1376 1824 -1216 1824 -1216 1808 -1120 1808 embbis.simulMode.INP
w 4 907 100 0 n#108 egenSub.simCalc.VALG -160 1376 0 1376 0 448 448 448 ebos.isFULL.DOL
w 68 1051 100 0 n#107 egenSub.simCalc.VALF -160 1440 64 1440 64 672 448 672 ebos.notFULL.DOL
w 132 1195 100 0 n#106 egenSub.simCalc.VALE -160 1504 128 1504 128 896 448 896 ebos.notFAST.DOL
w 196 1339 100 0 n#105 egenSub.simCalc.VALD -160 1568 192 1568 192 1120 448 1120 ebos.isFAST.DOL
w 18 1643 100 0 n#104 egenSub.simCalc.VALC -160 1632 256 1632 256 1344 448 1344 ebos.notVSM.DOL
w 50 1707 100 0 n#103 egenSub.simCalc.VALB -160 1696 320 1696 320 1568 448 1568 ebos.isVSM.DOL
w 104 1771 100 0 n#118 egenSub.simCalc.VALA -160 1760 448 1760 ebos.simulating.DOL
[cell use]
use notes -1376 -117 100 0 notes#143
xform 0 -1120 68
p -1348 194 100 0 -1 COMMENT1:NOTES:  The genSub on this schematic
p -1348 162 100 0 -1 COMMENT2:sets the records according to the
p -1348 132 100 0 -1 COMMENT3:simulation mode defined during an init.
use CBorder -1628 -436 -100 0 frame
xform 0 52 868
p 940 -300 100 1536 1 Date:24 Apr 97
p 1044 -204 300 1792 -1 Dnumber:
p 1172 -220 150 1536 -1 Title:simulMode.sch
use embbis -1056 1840 100 0 simulMode
xform 0 -992 1776
p -1044 1680 65 0 1 ONST:VSM
p -1036 1736 65 0 1 PV:$(top)
p -1044 1648 65 0 1 THST:FULL
p -1044 1664 65 0 1 TWST:FAST
p -1044 1696 65 0 1 ZRST:NONE
use outhier 992 1719 100 0 simulatingOut
xform 0 1008 1760
use outhier 992 1495 100 0 isVsm
xform 0 1008 1536
use outhier 992 1271 100 0 notVsm
xform 0 1008 1312
use outhier 992 1047 100 0 isFast
xform 0 1008 1088
use outhier 992 823 100 0 notFast
xform 0 1008 864
use outhier 992 599 100 0 isFull
xform 0 1008 640
use outhier 992 375 100 0 notFull
xform 0 1008 416
use inhier -1392 1783 100 0 simModeIn
xform 0 -1376 1824
use inhier -1392 1623 100 0 SLNK
xform 0 -1376 1664
use ebos 512 1608 100 0 isVSM
xform 0 576 1536
p 128 1390 100 0 0 ONAM:TRUE
p 520 1520 65 1536 1 PV:$(top)
p 128 1422 100 0 0 ZNAM:FALSE
p 416 1568 75 1280 -1 pproc(DOL):PP
use ebos 512 1384 100 0 notVSM
xform 0 576 1312
p 128 1166 100 0 0 ONAM:TRUE
p 520 1288 65 1536 1 PV:$(top)
p 128 1198 100 0 0 ZNAM:FALSE
p 416 1344 75 1280 -1 pproc(DOL):PP
use ebos 512 1160 100 0 isFAST
xform 0 576 1088
p 128 942 100 0 0 ONAM:TRUE
p 520 1072 65 1536 1 PV:$(top)
p 128 974 100 0 0 ZNAM:FALSE
p 416 1120 75 1280 -1 pproc(DOL):PP
use ebos 512 936 100 0 notFAST
xform 0 576 864
p 128 718 100 0 0 ONAM:TRUE
p 520 840 65 1536 1 PV:$(top)
p 128 750 100 0 0 ZNAM:FALSE
p 416 896 75 1280 -1 pproc(DOL):PP
use ebos 512 712 100 0 isFULL
xform 0 576 416
p 128 270 100 0 0 ONAM:TRUE
p 520 392 65 1536 1 PV:$(top)
p 128 302 100 0 0 ZNAM:FALSE
p 416 448 75 1280 -1 pproc(DOL):PP
use ebos 512 488 100 0 notFULL
xform 0 576 640
p 128 494 100 0 0 ONAM:TRUE
p 520 624 65 1536 1 PV:$(top)
p 128 526 100 0 0 ZNAM:FALSE
p 416 672 75 1280 -1 pproc(DOL):PP
use ebos 512 1800 100 0 simulating
xform 0 576 1728
p 128 1582 100 0 0 ONAM:TRUE
p 520 1712 65 1536 1 PV:$(top)
p 128 1614 100 0 0 ZNAM:FALSE
p 416 1760 75 1280 -1 pproc(DOL):PP
use egenSub -384 1808 100 0 simCalc
xform 0 -304 1392
p -368 1392 65 1536 1 PV:$(top)
p -368 1368 65 1536 1 SNAM:simCalcProc
p -671 741 100 0 0 UFB:
[comments]
