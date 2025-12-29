[schematic2]
uniq 144
[tools]
[detail]
w -942 -1109 100 0 n#143 egenSub.combCars1.FLNK -1024 320 -960 320 -960 -96 -1856 -96 -1856 -1120 32 -1120 32 -800 100 -800 egenSub.ovrAllCar.SLNK
w -638 -85 100 0 n#143 egenSub.combCars2.FLNK -352 -32 -256 -32 -256 -96 -960 -96 junction
w -190 -789 100 0 n#143 egenSub.combCars5.FLNK -480 -1024 -352 -1024 -352 -800 32 -800 junction
w -140 -597 100 0 n#141 egenSub.combCars5.OUTC -480 -448 -320 -448 -320 -608 100 -608 egenSub.ovrAllCar.I
w -124 -533 100 0 n#140 egenSub.combCars5.OUTB -480 -384 -288 -384 -288 -544 100 -544 egenSub.ovrAllCar.H
w -108 -469 100 0 n#139 egenSub.combCars5.OUTA -480 -320 -256 -320 -256 -480 100 -480 egenSub.ovrAllCar.G
w -1022 -981 100 0 n#138 egenSub.combCars3.FLNK -1216 -992 -768 -992 egenSub.combCars5.SLNK
w -1022 -405 100 0 n#114 egenSub.combCars3.OUTC -1216 -416 -768 -416 egenSub.combCars5.C
w -1022 -341 100 0 n#113 egenSub.combCars3.OUTB -1216 -352 -768 -352 egenSub.combCars5.B
w -1022 -277 100 0 n#112 egenSub.combCars3.OUTA -1216 -288 -768 -288 egenSub.combCars5.A
w -54 -405 100 0 n#97 egenSub.combCars2.OUTC -352 544 -160 544 -160 -416 100 -416 egenSub.ovrAllCar.F
w -38 -341 100 0 n#96 egenSub.combCars2.OUTB -352 608 -128 608 -128 -352 100 -352 egenSub.ovrAllCar.E
w -248 683 100 0 n#95 egenSub.combCars2.OUTA -352 672 -96 672 -96 -288 100 -288 egenSub.ovrAllCar.D
w -536 907 100 0 n#94 egenSub.combCars1.OUTC -1024 896 0 896 0 -224 100 -224 egenSub.ovrAllCar.C
w -520 971 100 0 n#93 egenSub.combCars1.OUTB -1024 960 32 960 32 -160 100 -160 egenSub.ovrAllCar.B
w -504 1035 100 0 n#92 egenSub.combCars1.OUTA -1024 1024 64 1024 64 -96 100 -96 egenSub.ovrAllCar.A
w 612 -581 100 0 n#76 egenSub.ovrAllCar.FLNK 388 -832 608 -832 608 -320 832 -320 ecars.applyC.SLNK
w 696 -181 100 0 n#67 egenSub.ovrAllCar.OUTC 388 -256 608 -256 608 -192 832 -192 ecars.applyC.IMSS
w 664 -213 100 0 n#65 egenSub.ovrAllCar.OUTB 388 -192 544 -192 544 -224 832 -224 ecars.applyC.IERR
w 586 -117 100 0 n#53 egenSub.ovrAllCar.OUTA 388 -128 832 -128 ecars.applyC.IVAL
s 140 12 100 0 of individual CARs
s 132 44 100 0 overall combination
s -872 32 100 0 ------>
s -916 72 100 0 lower level CARs
s -904 104 100 0 inputs from
s -1780 -920 100 0 inputs from
s -1792 -952 100 0 lower level CARs
s -1748 -992 100 0 ------>
s -1744 820 100 0 inputs from
s -1756 788 100 0 lower level CARs
s -1712 748 100 0 ------>
s -1028 -864 100 0 ------>
s -1072 -824 100 0 lower level CARs
s -1060 -792 100 0 inputs from
[cell use]
use egenSub 188 -48 100 0 ovrAllCar
xform 0 244 -464
p 200 -108 65 0 1 FTA:LONG
p 200 -124 65 0 1 FTB:LONG
p 200 -140 65 0 1 FTC:STRING
p 200 -156 65 0 1 FTD:LONG
p 200 -172 65 0 1 FTE:LONG
p 200 -188 65 0 1 FTF:STRING
p 200 -204 65 0 1 FTG:LONG
p 200 -220 65 0 1 FTH:LONG
p 200 -236 65 0 1 FTI:STRING
p 200 -260 65 0 1 FTVA:LONG
p 200 -276 65 0 1 FTVB:LONG
p 200 -292 65 0 1 FTVC:STRING
p 200 -840 65 0 1 PV:$(top)
p 200 -820 65 0 1 SNAM:combCars
use egenSub -556 744 100 0 combCars2
xform 0 -496 336
p -544 636 65 0 1 FTA:LONG
p -544 616 65 0 1 FTB:LONG
p -544 596 65 0 1 FTC:STRING
p -544 576 65 0 1 FTD:LONG
p -544 556 65 0 1 FTE:LONG
p -544 536 65 0 1 FTF:STRING
p -544 516 65 0 1 FTG:LONG
p -544 496 65 0 1 FTH:LONG
p -544 476 65 0 1 FTI:STRING
p -544 448 65 0 1 FTVA:LONG
p -544 428 65 0 1 FTVB:LONG
p -544 408 65 0 1 FTVC:STRING
p -536 -40 65 0 1 PV:$(top)
p -548 -20 65 0 1 SNAM:combCars
p -900 676 65 0 1 def(INPA):$(top)initC.VAL
p -904 612 65 0 1 def(INPB):$(top)initC.OERR
p -908 548 65 0 1 def(INPC):$(top)initC.OMSS
p -900 484 65 0 1 def(INPD):$(top)testC.VAL
p -908 420 65 0 1 def(INPE):$(top)testC.OERR
p -904 356 65 0 1 def(INPF):$(top)testC.OMSS
p -896 292 65 0 1 def(INPG):$(top)gSysC.VAL
p -904 228 65 0 1 def(INPH):$(top)gSysC.OERR
p -904 164 65 0 1 def(INPI):$(top)gSysC.OMSS
use egenSub -1208 1100 100 0 combCars1
xform 0 -1168 688
p -1212 1028 65 0 1 FTA:LONG
p -1212 1008 65 0 1 FTB:LONG
p -1212 988 65 0 1 FTC:STRING
p -1212 960 65 0 1 FTD:LONG
p -1212 940 65 0 1 FTE:LONG
p -1212 920 65 0 1 FTF:STRING
p -1212 892 65 0 1 FTG:LONG
p -1212 872 65 0 1 FTH:LONG
p -1212 852 65 0 1 FTI:STRING
p -1208 916 65 0 0 FTJ:LONG
p -1212 820 65 0 1 FTVA:LONG
p -1212 800 65 0 1 FTVB:LONG
p -1212 780 65 0 1 FTVC:STRING
p -1228 848 65 0 0 FTVF:DOUBLE
p -1224 532 65 0 0 FTVG:DOUBLE
p -1208 476 65 0 0 FTVH:DOUBLE
p -1208 456 65 0 0 FTVI:DOUBLE
p -1208 436 65 0 0 FTVJ:DOUBLE
p -1224 352 65 0 1 PV:$(top)
p -1228 376 65 0 1 SNAM:combCars
p -1548 1064 65 0 1 def(INPA):$(top)arSetupC.VAL
p -1564 1004 65 0 1 def(INPB):$(top)arSetupC.OERR
p -1564 940 65 0 1 def(INPC):$(top)arSetupC.OMSS
p -1544 876 65 0 1 def(INPD):$(top)obsSetupC.VAL
p -1556 812 65 0 1 def(INPE):$(top)obsSetupC.OERR
p -1560 748 65 0 1 def(INPF):$(top)obsSetupC.OMSS
p -1552 688 65 0 1 def(INPG):$(top)drRoiSetC.VAL
p -1568 620 65 0 1 def(INPH):$(top)drRoiSetC.OERR
p -1556 552 65 0 1 def(INPI):$(top)drRoiSetC.OMSS
p -1024 1034 75 0 -1 pproc(OUTA):NPP
use egenSub -1412 -212 100 0 combCars3
xform 0 -1360 -624
p -1400 -348 65 0 1 FTA:LONG
p -1400 -364 65 0 1 FTB:LONG
p -1400 -380 65 0 1 FTC:STRING
p -1400 -396 65 0 1 FTD:LONG
p -1400 -412 65 0 1 FTE:LONG
p -1400 -428 65 0 1 FTF:STRING
p -1400 -444 65 0 1 FTG:LONG
p -1400 -460 65 0 1 FTH:LONG
p -1400 -476 65 0 1 FTI:STRING
p -1727 -1531 65 0 0 FTJ:DOUBLE
p -1400 -508 65 0 1 FTVA:LONG
p -1400 -524 65 0 1 FTVB:LONG
p -1400 -544 65 0 1 FTVC:STRING
p -1416 -964 65 0 1 PV:$(top)
p -1792 -354 100 0 0 SCAN:Passive
p -1416 -984 65 0 1 SNAM:combCars
p -1768 -296 65 0 1 def(INPA):0.0
p -1768 -360 65 0 1 def(INPB):0.0
p -1760 -416 65 0 1 def(INPC):0.0
p -1784 -480 65 0 1 def(INPD):$(top)noopC.VAL
p -1784 -544 65 0 1 def(INPE):$(top)noopC.OERR
p -1788 -608 65 0 1 def(INPF):$(top)noopC.OMSS
p -1780 -668 65 0 1 def(INPG):$(top)parkC.VAL
p -1780 -732 65 0 1 def(INPH):$(top)parkC.OERR
p -1780 -796 65 0 1 def(INPI):$(top)parkC.OMSS
use egenSub -676 -244 100 0 combCars5
xform 0 -624 -656
p -664 -380 65 0 1 FTA:LONG
p -664 -396 65 0 1 FTB:LONG
p -664 -412 65 0 1 FTC:STRING
p -664 -428 65 0 1 FTD:LONG
p -664 -444 65 0 1 FTE:LONG
p -664 -460 65 0 1 FTF:STRING
p -664 -476 65 0 1 FTG:LONG
p -664 -492 65 0 1 FTH:LONG
p -664 -508 65 0 1 FTI:STRING
p -991 -1563 65 0 0 FTJ:DOUBLE
p -664 -540 65 0 1 FTVA:LONG
p -664 -556 65 0 1 FTVB:LONG
p -664 -576 65 0 1 FTVC:STRING
p -680 -996 65 0 1 PV:$(top)
p -1056 -386 100 0 0 SCAN:Passive
p -680 -1016 65 0 1 SNAM:combCars
p -1032 -328 65 0 0 def(INPA):0.0000000000000000e+00
p -1032 -392 65 0 0 def(INPB):0.0000000000000000e+00
p -1024 -448 65 0 0 def(INPC):0.0000000000000000e+00
p -1048 -512 65 0 1 def(INPD):$(top)dcSetupC.VAL
p -1048 -576 65 0 1 def(INPE):$(top)dcSetupC.OERR
p -1052 -640 65 0 1 def(INPF):$(top)dcSetupC.OMSS
p -1044 -700 65 0 0 def(INPG):0.0000000000000000e+00
p -1044 -764 65 0 0 def(INPH):0.0000000000000000e+00
p -1044 -828 65 0 0 def(INPI):0.0000000000000000e+00
p -1056 -738 100 0 0 def(INPJ):0.0000000000000000e+00
use notes 560 695 100 0 notes#116
xform 0 816 880
p 588 1006 100 0 -1 COMMENT1:NOTES:  This schematic contains the
p 588 974 100 0 -1 COMMENT2:records for hierarchically combining
p 588 944 100 0 -1 COMMENT3:individual CAR records.  Each genSub
p 588 912 100 0 -1 COMMENT4:takes up to 3 CAR inputs and outputs
p 588 880 100 0 -1 COMMENT5:the CAR value that is the highest along
p 588 848 100 0 -1 COMMENT6:with its corresponding error code and
p 588 816 100 0 -1 COMMENT7:message.  The ovrAllCar genSub takes
p 588 784 100 0 -1 COMMENT8:the intermediate results and reports the
p 588 752 100 0 -1 COMMENT9:overall CAR status in the top level CAR.
use CBorder -2044 -1304 -100 0 frame
xform 0 -364 0
p 1028 -1172 75 1536 -1 Author:Janet E. Tvedt
p 524 -1168 100 1536 1 Date:24 Apr 97
p 628 -1072 300 1792 -1 Dnumber:
p 756 -1088 150 1536 -1 Title:sysCar.sch
use ecars 952 -84 100 0 applyC
xform 0 992 -240
p 932 -364 100 0 1 PV:$(top)
p 928 -480 100 0 0 SIMS:NO_ALARM
p 696 -40 100 0 1 def(ICID):$(top)apply.CLID
use inhier 432 439 100 0 CLID
xform 0 448 480
[comments]
