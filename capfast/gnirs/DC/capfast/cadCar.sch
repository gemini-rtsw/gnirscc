[schematic2]
uniq 102
[tools]
[detail]
w 1172 123 100 0 n#87 junction 1168 48 1168 208 1280 208 ecars.C.IVAL
w 1220 -133 100 0 n#101 eseq.Seq.FLNK 1056 -272 1216 -272 1216 16 1280 16 ecars.C.SLNK
w -318 -53 100 0 n#100 hwin.hwin#99.in -288 -64 -288 -64 ewaits.alwaysWait.INAN
w -44 -301 100 0 n#98 ewaits.alwaysWait.FLNK -64 -96 -48 -96 -48 -496 80 -496 ecalcs.alwaysCalc.SLNK
w 8 -101 100 0 n#97 ewaits.alwaysWait.VAL -64 -128 -16 -128 -16 -112 80 -112 ecalcs.alwaysCalc.INPA
w 400 -293 100 0 n#96 ecalcs.alwaysCalc.VAL 368 -304 480 -304 eseq.Seq.SDIS
w 400 -261 100 0 n#95 ecalcs.alwaysCalc.FLNK 368 -272 480 -272 eseq.Seq.SLNK
w 1088 19 100 0 n#87 eseq.Seq.LNK2 1056 16 1168 16 1168 48 1056 48 eseq.Seq.LNK1
w 1472 275 100 0 n#87 junction 1232 208 1232 272 1760 272 outhier.IVAL.p
w 1664 -13 100 0 FLNK ecars.C.FLNK 1600 -16 1776 -16 outhier.FLNK.p
w 192 19 100 0 n#55 hwin.hwin#53.in -48 16 480 16 eseq.Seq.DOL2
w 224 51 100 0 n#54 hwin.hwin#51.in -48 176 16 176 16 48 480 48 eseq.Seq.DOL1
s 192 256 100 0 and then after a delay sets it back to "IDLE".
s 192 288 100 0 This sequence record sets the CAR record to "BUSY"
s -240 64 100 0 CAR_IDLE = 0
s -240 224 100 0 CAR_BUSY = 2
s 448 -1488 500 0 cadCar.sch
s -992 208 200 0 (TBD)
[cell use]
use hwin -240 -25 100 0 hwin#53
xform 0 -144 16
p -237 8 100 0 -1 val(in):0
use hwin -240 135 100 0 hwin#51
xform 0 -144 176
p -237 168 100 0 -1 val(in):2
use hwin -480 -105 100 0 hwin#99
xform 0 -384 -64
p -477 -72 100 0 -1 val(in):$(top)apply.DIR
use ecalcs 80 -585 100 0 alwaysCalc
xform 0 224 -320
p -13 -104 100 0 0 CALC:A#3
use ewaits -288 -249 100 0 alwaysWait
xform 0 -176 -128
p -176 -128 100 256 -1 CALC:A
p -288 -368 100 0 1 DOPT:Use VAL
p -304 -96 100 1280 -1 INBP:No
p -304 -128 100 1280 -1 INCP:No
p -304 -336 100 768 1 OOPT:Every Time
p -288 -288 100 0 1 SCAN:I/O Intr
use notes -576 -1401 100 0 notes#85
xform 0 -320 -1216
p -48 -1250 100 0 0 AUTHOR:S M Beard
p -548 -1090 100 0 -1 COMMENT1:This schematic represents the contents
p -548 -1122 100 0 -1 COMMENT2:of the heirarchical symbol "cadCar"
p -548 -1152 100 0 -1 COMMENT3:which is used by trivial commands
p -548 -1184 100 0 -1 COMMENT4:to toggle the CAR record to BUSY
p -548 -1216 100 0 -1 COMMENT5:and back to IDLE.
use outhier 1744 -57 100 0 FLNK
xform 0 1760 -16
use outhier 1728 231 100 0 IVAL
xform 0 1744 272
use eseq 480 -361 100 0 Seq
xform 0 768 -64
p 800 46 100 0 1 DLY2:1
p 544 -416 100 0 1 PV:$(top)$(cad)
p 1056 48 75 768 -1 pproc(LNK1):PP
p 1056 16 75 768 -1 pproc(LNK2):PP
use inhier -1056 -1177 100 0 SLNK
xform 0 -1040 -1136
use inhier -1104 183 100 0 STAT
xform 0 -1088 224
use ecars 1280 -73 100 0 C
xform 0 1440 96
p 1344 -80 100 0 1 DESC:$(cad) CAR record
p 1360 -144 100 0 1 PV:$(top)$(cad)
use bc200tr -1248 -1656 -100 0 frame
xform 0 432 -352
p 1328 -1488 100 0 1 author:S.M.Beard
p 1552 -1488 100 0 -1 border:C
p 1312 -1520 100 0 1 checked:B.Goodrich
p 1584 -1488 100 0 -1 date:18 Jan 2000
p 1552 -1376 100 0 -1 project:Core Instrument Control System
p 1552 -1440 100 0 -1 title:Wrap up for CAR "busy then idle"
[comments]
