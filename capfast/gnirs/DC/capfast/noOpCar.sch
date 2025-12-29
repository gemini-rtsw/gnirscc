[schematic2]
uniq 118
[tools]
[detail]
w -686 -989 100 0 n#115 egenSub.noopCars2.FLNK -1024 -992 -288 -992 -288 -800 junction
w -654 -157 100 0 n#115 egenSub.noopCars1.FLNK -1024 320 -960 320 -960 -160 -288 -160 -288 -800 96 -800 egenSub.ovrNoopCar.SLNK
w -494 -413 100 0 n#114 egenSub.noopCars2.OUTC -1024 -416 96 -416 egenSub.ovrNoopCar.F
w -494 -349 100 0 n#113 egenSub.noopCars2.OUTB -1024 -352 96 -352 egenSub.ovrNoopCar.E
w -494 -285 100 0 n#112 egenSub.noopCars2.OUTA -1024 -288 96 -288 egenSub.ovrNoopCar.D
w -4 331 100 0 n#94 egenSub.noopCars1.OUTC -1024 896 0 896 0 -224 96 -224 egenSub.ovrNoopCar.C
w 28 395 100 0 n#93 egenSub.noopCars1.OUTB -1024 960 32 960 32 -160 96 -160 egenSub.ovrNoopCar.B
w 60 459 100 0 n#92 egenSub.noopCars1.OUTA -1024 1024 64 1024 64 -96 96 -96 egenSub.ovrNoopCar.A
w 604 -581 100 0 n#76 egenSub.ovrNoopCar.FLNK 384 -832 608 -832 608 -320 832 -320 ecars.noopC.SLNK
w 472 -253 100 0 n#67 egenSub.ovrNoopCar.OUTC 384 -256 608 -256 608 -192 832 -192 ecars.noopC.IMSS
w 664 -221 100 0 n#65 egenSub.ovrNoopCar.OUTB 384 -192 544 -192 544 -224 832 -224 ecars.noopC.IERR
w 584 -125 100 0 n#53 egenSub.ovrNoopCar.OUTA 384 -128 832 -128 ecars.noopC.IVAL
w 576 155 100 0 CLID inhier.CLID.P 448 480 572 480 572 -160 832 -160 ecars.noopC.ICID
s -1712 748 100 0 ------>
s -1756 788 100 0 lower level CARs
s -1744 820 100 0 inputs from
s -1732 -496 100 0 ------>
s -1776 -456 100 0 lower level CARs
s -1764 -424 100 0 inputs from
s 144 48 100 0 overall combination of
s 140 12 100 0 individual noop CARs
[cell use]
use ecars 960 -84 100 0 noopC
xform 0 992 -240
p 944 -372 65 0 1 PV:$(top)
p 1096 -372 65 0 1 def(FLNK):$(top)combCars3.VAL
use notes 688 699 100 0 notes#116
xform 0 944 884
p 716 1010 100 0 -1 COMMENT1:NOTES:  This schematic contains the
p 716 978 100 0 -1 COMMENT2:records for hierarchically combining
p 716 948 100 0 -1 COMMENT3:the "noop" CAR records.  Each genSub
p 716 916 100 0 -1 COMMENT4:takes up to 3 CAR inputs and outputs
p 716 884 100 0 -1 COMMENT5:the CAR value that is the highest along
p 716 852 100 0 -1 COMMENT6:with its corresponding error code and
p 716 820 100 0 -1 COMMENT7:message.  The ovrNoopCar genSub takes
p 716 788 100 0 -1 COMMENT8:the intermediate results and reports the
p 716 756 100 0 -1 COMMENT9:overall CAR status in the noopC CAR.
use CBorder -1916 -1300 -100 0 frame
xform 0 -236 4
p 1156 -1168 75 1536 -1 Author:Steven M. Beard
p 652 -1164 100 1536 1 Date:10 Jun 97
p 756 -1068 300 1792 -1 Dnumber:
p 884 -1084 150 1536 -1 Title:noOpCar.sch
use egenSub -1220 -212 100 0 noopCars2
xform 0 -1168 -624
p -1208 -348 65 0 1 FTA:LONG
p -1208 -364 65 0 1 FTB:LONG
p -1208 -380 65 0 1 FTC:STRING
p -1208 -396 65 0 1 FTD:LONG
p -1208 -412 65 0 1 FTE:LONG
p -1208 -428 65 0 1 FTF:STRING
p -1208 -444 65 0 1 FTG:LONG
p -1208 -460 65 0 1 FTH:LONG
p -1208 -476 65 0 1 FTI:STRING
p -1535 -1531 65 0 0 FTJ:DOUBLE
p -1208 -508 65 0 1 FTVA:LONG
p -1208 -524 65 0 1 FTVB:LONG
p -1208 -544 65 0 1 FTVC:STRING
p -1224 -964 65 0 1 PV:$(top)
p -1600 -354 100 0 0 SCAN:Passive
p -1224 -984 65 0 1 SNAM:combCars
p -1596 -288 65 0 1 def(INPA):$(top)guideC.VAL
p -1596 -352 65 0 1 def(INPB):$(top)guideC.OERR
p -1592 -416 65 0 1 def(INPC):$(top)guideC.OMSS
p -1592 -480 65 0 1 def(INPD):$(top)endGuideC.VAL
p -1592 -544 65 0 1 def(INPE):$(top)endGuideC.OERR
p -1596 -608 65 0 1 def(INPF):$(top)endGuideC.OMSS
p -1600 -672 65 0 1 def(INPG):$(top)endObserveC.VAL
p -1600 -736 65 0 1 def(INPH):$(top)endObserveC.OERR
p -1600 -800 65 0 1 def(INPI):$(top)endObserveC.OMSS
use egenSub -1208 1100 100 0 noopCars1
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
p -1548 1064 65 0 1 def(INPA):$(top)verifyC.VAL
p -1564 1004 65 0 1 def(INPB):$(top)verifyC.OERR
p -1564 940 65 0 1 def(INPC):$(top)verifyC.OMSS
p -1544 876 65 0 1 def(INPD):$(top)endVerifyC.VAL
p -1556 812 65 0 1 def(INPE):$(top)endVerifyC.OERR
p -1560 748 65 0 1 def(INPF):$(top)endVerifyC.OMSS
p -1552 688 65 0 1 def(INPG):$(top)datumC.VAL
p -1568 620 65 0 1 def(INPH):$(top)datumC.OERR
p -1556 552 65 0 1 def(INPI):$(top)datumC.OMSS
p -1024 1034 75 0 -1 pproc(OUTA):NPP
use egenSub 184 -48 100 0 ovrNoopCar
xform 0 240 -464
p 196 -108 65 0 1 FTA:LONG
p 196 -124 65 0 1 FTB:LONG
p 196 -140 65 0 1 FTC:STRING
p 196 -156 65 0 1 FTD:LONG
p 196 -172 65 0 1 FTE:LONG
p 196 -188 65 0 1 FTF:STRING
p 196 -204 65 0 1 FTG:LONG
p 196 -220 65 0 1 FTH:LONG
p 196 -236 65 0 1 FTI:STRING
p 196 -260 65 0 1 FTVA:LONG
p 196 -276 65 0 1 FTVB:LONG
p 196 -292 65 0 1 FTVC:STRING
p 196 -840 65 0 1 PV:$(top)
p 196 -820 65 0 1 SNAM:combCars
p -288 -512 100 0 1 def(INPG):$(top)alwaysC.VAL
p -288 -544 100 0 1 def(INPH):$(top)alwaysC.OERR
p -288 -576 100 0 1 def(INPI):$(top)alwaysC.OMSS
use inhier 432 439 100 0 CLID
xform 0 448 480
[comments]
