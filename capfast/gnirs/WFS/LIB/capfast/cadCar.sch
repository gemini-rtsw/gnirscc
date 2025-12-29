[schematic2]
uniq 61
[tools]
[detail]
w -88 -341 100 0 n#55 hwin.hwin#53.in -160 -352 32 -352 eseq.Seq.DOL2
w -92 -261 100 0 n#54 hwin.hwin#51.in -160 -192 -96 -192 -96 -320 32 -320 eseq.Seq.DOL1
w 232 -29 100 0 n#50 inhier.ICID.P -544 -32 1056 -32 1056 -352 1152 -352 ecars.C.ICID
w 856 -317 100 0 n#60 eseq.Seq.LNK1 608 -320 1152 -320 ecars.C.IVAL
w 712 -349 100 0 n#60 eseq.Seq.LNK2 608 -352 864 -352 864 -320 junction
w -280 -637 100 0 n#46 inhier.SLNK.P -544 -640 32 -640 eseq.Seq.SLNK
s -448 208 200 0 (TBD)
s 608 -1472 500 0 cadCar.sch
s -352 -144 100 0 CAR_BUSY = 2
s -352 -304 100 0 CAR_IDLE = 0
s 80 -80 100 0 This sequence record sets the CAR record to "BUSY"
s 80 -112 100 0 and then 0.5 seconds later sets it back to "IDLE".
n -1008 -1408 -656 -1056 100
This represents the contents
of a hierarchical symbol
"cadCar", which is used in
trivial commands to toggle
the CAR record to "busy" and
back to "idle".
_
[cell use]
use eseq 32 -729 100 0 Seq
xform 0 320 -432
p 352 -322 100 0 1 DLY2:0.5e+00
p 128 -768 100 0 1 PV:$(top)$(cad)
p 608 -320 75 768 -1 pproc(LNK1):PP
p 608 -352 75 768 -1 pproc(LNK2):PP
use hwin -352 -233 100 0 hwin#51
xform 0 -256 -192
p -349 -200 100 0 -1 val(in):2
use hwin -352 -393 100 0 hwin#53
xform 0 -256 -352
p -349 -360 100 0 -1 val(in):0
use inhier -560 183 100 0 STAT
xform 0 -544 224
use inhier -560 -681 100 0 SLNK
xform 0 -544 -640
use inhier -560 -73 100 0 ICID
xform 0 -544 -32
use ecars 1152 -601 100 0 C
xform 0 1312 -432
p 1216 -608 100 0 1 DESC:$(cad) CAR record
p 1216 -640 100 0 1 PV:$(top)$(cad)
use bc200tr -1248 -1656 -100 0 frame
xform 0 432 -352
p 1328 -1488 100 0 1 author:S.M.Beard
p 1552 -1488 100 0 -1 border:C
p 1312 -1520 100 0 1 checked:S.M.Beard
p 1584 -1488 100 0 -1 date:1 Nov 96
p 1552 -1376 100 0 -1 project:Core Instrument Control System
p 1552 -1440 100 0 -1 title:Wrap up for CAR "busy then idle"
[comments]
