[schematic2]
uniq 209
[tools]
[detail]
w 1282 1387 100 0 n#207 ecalcs.ecalcs#203.VAL 1504 1056 1568 1056 1568 1376 1056 1376 1056 1248 1216 1248 ecalcs.ecalcs#203.INPA
w -190 1419 100 0 n#201 ecad8.observe.PLNK -224 1408 -96 1408 hwout.hwout#202.outp
w -190 1771 100 0 n#199 ecad8.observe.OUTE -224 1760 -96 1760 hwout.hwout#200.outp
w -190 1835 100 0 n#193 ecad8.observe.OUTD -224 1824 -96 1824 hwout.hwout#196.outp
w -190 1899 100 0 n#192 ecad8.observe.OUTC -224 1888 -96 1888 hwout.hwout#198.outp
w -206 2027 100 0 n#191 ecad8.observe.OUTA -224 2016 -128 2016 hwout.hwout#197.outp
w -62 2347 -100 0 off ecad8.observe.VAL -224 2240 -160 2240 -160 2336 96 2336 outhier.VAL.p
w -94 2219 -100 0 c#195 ecad8.observe.MESS -224 2208 96 2208 outhier.MESS.p
w -286 395 100 0 n#181 estringouts.estringouts#186.FLNK -320 384 -192 384 hwout.hwout#180.outp
w -286 587 100 0 n#178 estringouts.estringouts#176.FLNK -320 576 -192 576 hwout.hwout#177.outp
w -736 2315 100 0 n#159 inhier.DIR.P -768 2304 -644 2304 -644 2240 -544 2240 ecad8.observe.DIR
s -688 2056 100 0 (data label)
s -680 1984 100 0 (filename)
s -220 2056 100 0 (data label)
s -216 1992 100 0 (filename)
s 0 2080 100 0 This link updates the data label record
s 0 2048 100 0 in the Status Alarm Database
s 744 2048 150 0 doObserve
[cell use]
use hwout -96 1847 100 0 hwout#198
xform 0 0 1888
p 112 1888 100 0 -1 val(outp):nirsg:sad:dc:utstart.VAL .PP
use hwout -128 1975 100 0 hwout#197
xform 0 -32 2016
p -32 2007 100 0 -1 val(outp):$(sadtop)dataLabel.VAL PP MS
use hwout -96 1783 100 0 hwout#196
xform 0 0 1824
p 112 1824 100 0 -1 val(outp):nirsg:sad:dc:datestart.VAL .PP
use hwout -192 535 100 0 hwout#177
xform 0 -96 576
p -96 608 100 0 -1 val(outp):nirsg:sad:dc:utnow.VAL
use hwout -192 343 100 0 hwout#180
xform 0 -96 384
p -96 416 100 0 -1 val(outp):nirsg:sad:dc:utend.VAL
use hwout -96 1719 100 0 hwout#200
xform 0 0 1760
p 112 1760 100 0 -1 val(outp):nirsg:sad:dc:obsepoch.VAL .PP
use hwout -96 1367 100 0 hwout#202
xform 0 0 1408
p 112 1408 100 0 -1 val(outp):$(top)astCtx
use ecalcs 1216 775 100 0 ecalcs#203
xform 0 1360 1040
p 1216 1312 100 0 1 CALC:(A>0)?(A-1):0
p 928 1150 100 0 0 SCAN:1 second
p 1328 768 100 1024 -1 name:$(top)timeLeft
use outhier 64 2167 100 0 MESS
xform 0 80 2208
use outhier 64 2295 100 0 VAL
xform 0 80 2336
use ecad8 -432 2304 100 0 observe
xform 0 -384 1792
p -448 1856 100 0 0 FTVE:DOUBLE
p -448 1600 100 0 0 PRIO:LOW
p -432 1408 100 0 1 PV:$(top)
p -480 1264 100 0 1 SNAM:observeProc
p -832 1888 100 0 -1 def(INPC):$(top)observeC.VAL
p -192 2016 75 768 -1 palrm(OUTA):MS
p -192 1888 75 768 -1 palrm(OUTC):NMS
p -224 2016 75 768 -1 pproc(OUTA):PP
p -224 1888 75 768 -1 pproc(OUTC):PP
p -224 1824 75 768 -1 pproc(OUTD):PP
p -224 1760 75 768 -1 pproc(OUTE):PP
use estringouts -576 295 100 0 estringouts#186
xform 0 -448 368
p 2020 -148 75 1536 -1 Author:Janet E. Tvedt
p 1516 -144 100 1536 1 Date:24 Apr 97
p 1620 -48 300 1792 -1 Dnumber:
p 1748 -64 150 1536 -1 Title:observeCad.sch
p -464 288 100 1024 -1 name:$(top)endTime
use estringouts -512 1123 100 0 sciProgstr
xform 0 -384 1196
use estringouts -576 711 100 0 estringouts#175
xform 0 -448 784
p -464 704 100 1024 -1 name:$(top)timeNow
use estringouts -576 487 100 0 estringouts#176
xform 0 -448 560
p 2020 44 75 1536 -1 Author:Janet E. Tvedt
p 1516 48 100 1536 1 Date:24 Apr 97
p 1620 144 300 1792 -1 Dnumber:
p 1748 128 150 1536 -1 Title:observeCad.sch
p -464 480 100 1024 -1 name:$(top)startTime
use ebos 480 1488 100 0 DCAReady
xform 0 544 1408
p 96 1262 100 0 0 ONAM:Ready
p 480 1312 100 0 1 PV:$(top)
p 96 1294 100 0 0 ZNAM:Not Ready
use ebos 432 1111 100 0 frameReady
xform 0 560 1200
p 112 1054 100 0 0 ONAM:ready
p 112 1086 100 0 0 ZNAM:waiting
use task 544 1927 100 0 task#167
xform 0 840 2096
use CBorder -1052 -88 -100 0 frame
xform 0 628 1216
use ecars 1284 1832 100 0 observeC
xform 0 1344 1680
p 1316 1548 65 0 1 PV:$(top)
p 1540 1564 65 0 1 def(FLNK):0.0
p 964 1752 65 0 1 def(ICID):$(top)apply.CLID
use notes 1636 1899 100 0 notes#160
xform 0 1892 2084
p 1664 2210 100 0 -1 COMMENT1:This implements the observe command.
use inhier -784 2263 100 0 DIR
xform 0 -768 2304
[comments]
