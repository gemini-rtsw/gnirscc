[schematic2]
uniq 515
[tools]
[detail]
w -974 1867 100 0 n#514 elongins.Sim.VAL -1056 1856 -832 1856 -832 2048 -608 2048 ecalcs.Calc.INPB
w -862 2083 100 0 n#513 ecalcs.Calc.INPA -608 2080 -1056 2080 elongins.FastRd.VAL
w -286 1899 100 0 n#512 embbis.Mode.INP -192 1888 -320 1888 ecalcs.Calc.VAL
w -942 1579 100 0 n#378 embbos.FastSet.OUT -1184 1472 -1088 1472 -1088 1568 -736 1568 -736 1472 -768 1472 elongouts.FastWr.VAL
w -1342 2139 100 0 n#327 elongins.FastRd.INP -1312 2128 -1312 2128 hwin.hwin#332.in
w -798 1451 100 0 n#324 elongouts.FastWr.OUT -768 1440 -768 1440 hwout.hwout#330.outp
s -1440 1760 100 768 Set Sim to 0 for mechanism control.
s -1440 1728 100 768 Set Sim to non-0 for simulation.
[cell use]
use ecalcs -544 2112 100 768 Calc
xform 0 -464 1872
p -544 1600 100 768 1 CALC:B?A:3
p -640 2080 75 1280 -1 pproc(INPA):PP
p -640 2048 75 1280 -1 pproc(INPB):PP
use elongins -1248 2144 100 768 FastRd
xform 0 -1184 2096
p -1248 2016 100 768 1 DTYP:vxWorks Variable (INST_IO)
use elongins -1248 1920 100 768 Sim
xform 0 -1184 1872
use embbos -1376 1536 100 768 FastSet
xform 0 -1312 1472
p -1376 1344 100 768 1 ONST:FAST
p -1376 1376 100 768 1 ZRST:FULL
p -1184 1472 75 768 -1 pproc(OUT):PP
use elongouts -960 1536 100 768 FastWr
xform 0 -896 1472
p -960 1376 100 768 1 DTYP:vxWorks Variable (INST_IO)
use hwout -768 1399 100 0 hwout#330
xform 0 -672 1440
p -672 1431 100 0 -1 val(outp):@lockFast
use hwin -1504 2087 100 0 hwin#332
xform 0 -1408 2128
p -1501 2120 100 0 -1 val(in):@lockFast
use embbis -128 1904 100 768 Mode
xform 0 -64 1856
p -128 1712 100 768 1 ONST:FAST
p -128 1776 100 768 1 SCAN:.2 second
p -128 1648 100 768 1 THST:NONE
p -128 1680 100 768 1 TWST:VSM
p -128 1744 100 768 1 ZRST:FULL
p -224 1888 75 1280 -1 pproc(INP):PP
use common 1216 1831 100 0 common#406
xform 0 1376 1968
use eborderC 1600 -160 100 1280 $Id:
xform 0 16 1024
p 1136 -64 200 1536 -1 file:lock.sch
p 1600 -160 100 1280 -1 id:$Id: sim.sch,v 1.2 2009/05/27 19:34:04 fkraemer Exp $
[comments]
