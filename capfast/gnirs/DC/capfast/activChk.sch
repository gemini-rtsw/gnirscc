[schematic2]
uniq 53
[tools]
[detail]
w -408 395 100 0 n#50 hwin.hwin#49.in -384 384 -384 384 ewaits.ewaits#45.INAN
w -4 139 100 0 n#48 ewaits.ewaits#45.FLNK -160 352 0 352 0 -64 192 -64 esubs.deactivateSub.SLNK
w -8 323 100 0 n#47 ewaits.ewaits#45.VAL -160 320 192 320 esubs.deactivateSub.INPA
w 520 1159 100 0 n#44 ebos.activate.OUT 68 960 256 960 256 1148 832 1148 832 988 772 988 elongouts.doActivate.VAL
w -174 1155 100 0 n#21 ebos.activate.VAL 68 992 164 992 164 1152 -476 1152 inhier.activInp.P
w 866 967 100 0 n#7 elongouts.doActivate.OUT 772 956 996 956 hwout.hwout#6.outp
[cell use]
use hwin -48 455 100 0 hwin#51
xform 0 48 496
p -128 496 100 0 -1 val(in):$(top)footLGain
use hwin -576 343 100 0 hwin#49
xform 0 -480 384
p -656 384 100 0 -1 val(in):$(top)footLGain
use esubs 192 -153 100 0 deactivateSub
xform 0 336 112
p -96 -34 100 0 0 INAM:initSub
p -96 254 100 0 0 SCAN:Passive
p -96 -66 100 0 0 SNAM:deactivateWarm
use ewaits -384 199 100 0 ewaits#45
xform 0 -272 320
p -320 160 100 256 1 CALC:A
p -352 -32 100 0 1 DOPT:Use VAL
p -400 352 100 1280 -1 INBP:No
p -400 320 100 1280 -1 INCP:No
p -352 32 100 768 1 OOPT:Every Time
p -352 96 100 0 1 SCAN:I/O Intr
use CBorder -948 -428 -100 0 frame
xform 0 732 876
p 1620 -292 100 1536 1 Date:23 Apr 97
p 1724 -196 300 1792 -1 Dnumber:
p 1852 -212 150 1536 -1 Title:activChk.sch
use embbis 584 840 100 0 activChk
xform 0 640 784
p 640 798 100 0 0 ONST:Activated
p 448 798 100 0 0 ONVL:1
p 584 720 65 1536 1 PV:$(top)
p 640 766 100 0 0 TWST:Activation Error
p 832 766 100 0 0 TWSV:MAJOR
p 448 766 100 0 0 TWVL:2
p 640 830 100 0 0 ZRST:Deactivated
p 832 830 100 0 0 ZRSV:MINOR
use hwout 996 915 100 0 hwout#6
xform 0 1092 956
p 1084 956 65 1536 -1 val(outp):@Node=2,Var=8,Grp=-1,Idx=0
use elongouts 580 1060 100 0 doActivate
xform 0 644 988
p 472 1380 100 0 0 DTYP:wFireVarMsg
p 580 908 65 1536 1 PV:$(top)
p 772 956 75 768 -1 pproc(OUT):PP
use inhier -492 1111 100 0 activInp
xform 0 -476 1152
use ebos -120 1064 100 0 activate
xform 0 -60 992
p -508 846 100 0 0 ONAM:Activate
p -120 916 65 1536 1 PV:$(top)
p -508 878 100 0 0 ZNAM:Deactivate
p 68 960 75 768 -1 pproc(OUT):PP
[comments]
