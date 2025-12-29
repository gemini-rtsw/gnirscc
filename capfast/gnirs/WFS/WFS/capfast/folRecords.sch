[schematic2]
uniq 1124
[tools]
[detail]
w -548 -125 100 0 n#1119 hwin.hwin#1122.in -576 -128 -448 -128 egenSubD.AgOut.INPG
w -548 -157 100 0 n#1118 hwin.hwin#1121.in -576 -160 -448 -160 egenSubD.AgOut.INPH
w -548 -189 100 0 n#1117 hwin.hwin#1120.in -576 -192 -448 -192 egenSubD.AgOut.INPI
w -532 -629 -100 0 OSLNK inhier.OSLNK.P -544 -640 -448 -640 egenSubD.AgOut.SLNK
w -548 3 100 0 n#1114 hwin.hwin#1115.in -576 0 -448 0 egenSubD.AgOut.INPC
w -548 -413 100 0 n#1111 hwin.hwin#1112.in -576 -416 -448 -416 egenSubD.AgOut.INPP
w -548 -445 100 0 n#1110 egenSubD.AgOut.INPQ -448 -448 -576 -448 hwin.hwin#1113.in
w -1070 387 100 0 n#1106 hwout.hwout#1109.outp -992 384 -1088 384 egenSubA.lowA.OUTN
w -1070 259 100 0 n#1105 hwout.hwout#1107.outp -992 256 -1088 256 egenSubA.lowA.OUTR
w -1070 323 100 0 n#1104 hwout.hwout#1108.outp -992 320 -1088 320 egenSubA.lowA.OUTP
w -1460 459 100 0 n#1101 hwin.hwin#1103.in -1472 448 -1376 448 egenSubA.lowA.INPL
w -1460 395 100 0 n#1100 egenSubA.lowA.INPN -1376 384 -1472 384 hwin.hwin#1102.in
w -92 -541 100 0 n#1093 hwout.hwout#1092.outp 48 -544 -160 -544 egenSubD.AgOut.OUTJ
w -772 -61 100 0 n#1091 egenSubA.lowA.VALU -1088 160 -1024 160 -1024 -64 -448 -64 egenSubD.AgOut.INPE
w -756 -29 100 0 n#1090 egenSubA.lowA.VALS -1088 224 -992 224 -992 -32 -448 -32 egenSubD.AgOut.INPD
w -1070 771 100 0 n#1085 hwout.hwout#1084.outp -992 768 -1088 768 egenSubA.lowA.OUTB
w -1070 579 100 0 n#1082 hwout.hwout#1083.outp -992 576 -1088 576 egenSubA.lowA.OUTH
w -548 -349 100 0 n#1075 hwin.hwin#1077.in -576 -352 -448 -352 egenSubD.AgOut.INPN
w -548 -381 100 0 n#1074 egenSubD.AgOut.INPO -448 -384 -576 -384 hwin.hwin#1076.in
w -548 -317 100 0 n#1073 egenSubD.AgOut.INPM -448 -320 -576 -320 hwin.hwin#1079.in
w -548 -285 100 0 n#1072 hwin.hwin#1078.in -576 -288 -448 -288 egenSubD.AgOut.INPL
w -1460 651 100 0 n#1066 hwin.hwin#1068.in -1472 640 -1376 640 egenSubA.lowA.INPF
w -1460 587 100 0 n#1065 egenSubA.lowA.INPH -1376 576 -1472 576 hwin.hwin#1067.in
w -1460 715 100 0 n#1064 egenSubA.lowA.INPD -1376 704 -1472 704 hwin.hwin#1070.in
w -1460 779 100 0 n#1063 hwin.hwin#1069.in -1472 768 -1376 768 egenSubA.lowA.INPB
w -548 35 100 0 n#1021 hwin.hwin#1022.in -576 32 -448 32 egenSubD.AgOut.INPB
w -548 67 100 0 n#1020 hwin.hwin#1019.in -576 64 -448 64 egenSubD.AgOut.INPA
w -1070 451 100 0 n#1003 hwout.hwout#1004.outp -992 448 -1088 448 egenSubA.lowA.OUTL
w -1070 515 100 0 n#1002 hwout.hwout#1005.outp -992 512 -1088 512 egenSubA.lowA.OUTJ
w -1070 643 100 0 n#1000 hwout.hwout#999.outp -992 640 -1088 640 egenSubA.lowA.OUTF
w -1070 707 100 0 n#997 hwout.hwout#998.outp -992 704 -1088 704 egenSubA.lowA.OUTD
s -1920 -480 100 768 SAD.
s -1920 -448 100 768 in the controller, not the
s -1920 -416 100 768 This is required to be
s -16 -1392 400 1280 folRecords
s -512 544 100 768 This record is triggered
s 128 768 100 768 Note the strange value of PV!
s -512 512 100 768 by the "Out" records for the
s -512 480 100 768 individual components.
s 128 800 100 768 This simulates the A&G interface.
s -848 -96 100 0 Angle (init. via pvload) -->
[cell use]
use esirs -1824 -512 100 768 Present
xform 0 -1680 -640
p -1952 -896 100 0 0 DESC:Current Gemini time
p -1952 -992 100 0 0 EGU:seconds
p -1824 -960 100 768 1 FDSC:Current Gemini time
p -1824 -864 100 768 1 FTVL:DOUBLE
p -1824 -896 100 768 1 PREC:3
p -1824 -800 100 768 1 PV:$(top)$(mech)
p -1824 -928 100 768 1 SCAN:1 second
p -1824 -832 100 768 1 SNAM:SIRwfsFolPresent
use hwin -768 -169 100 0 hwin#1122
xform 0 -672 -128
p -765 -136 100 0 -1 val(in):$(top)$(mech1)AgErr
use hwin -768 -201 100 0 hwin#1121
xform 0 -672 -160
p -765 -168 100 0 -1 val(in):$(top)$(mech2)AgErr
use hwin -768 -233 100 0 hwin#1120
xform 0 -672 -192
p -765 -200 100 0 -1 val(in):$(top)$(mech3)AgErr
use hwin -1664 407 100 0 hwin#1103
xform 0 -1568 448
p -1661 440 100 0 -1 val(in):$(top)$(mech3)Scale
use hwin -1664 343 100 0 hwin#1102
xform 0 -1568 384
p -1661 376 100 0 -1 val(in):$(top)$(mech3)Offset
use hwin -768 -361 100 0 hwin#1079
xform 0 -672 -320
p -765 -328 100 0 -1 val(in):$(top)$(mech1)Offset
use hwin -768 -329 100 0 hwin#1078
xform 0 -672 -288
p -765 -296 100 0 -1 val(in):$(top)$(mech1)Scale
use hwin -768 -393 100 0 hwin#1077
xform 0 -672 -352
p -765 -360 100 0 -1 val(in):$(top)$(mech2)Scale
use hwin -768 -425 100 0 hwin#1076
xform 0 -672 -384
p -765 -392 100 0 -1 val(in):$(top)$(mech2)Offset
use hwin -1664 663 100 0 hwin#1070
xform 0 -1568 704
p -1661 696 100 0 -1 val(in):$(top)$(mech1)Offset
use hwin -1664 727 100 0 hwin#1069
xform 0 -1568 768
p -1661 760 100 0 -1 val(in):$(top)$(mech1)Scale
use hwin -1664 599 100 0 hwin#1068
xform 0 -1568 640
p -1661 632 100 0 -1 val(in):$(top)$(mech2)Scale
use hwin -1664 535 100 0 hwin#1067
xform 0 -1568 576
p -1661 568 100 0 -1 val(in):$(top)$(mech2)Offset
use hwin -768 -9 100 0 hwin#1022
xform 0 -672 32
p -765 24 100 0 -1 val(in):$(top)$(mech2)AgOut
use hwin -768 23 100 0 hwin#1019
xform 0 -672 64
p -765 56 100 0 -1 val(in):$(top)$(mech1)AgOut
use hwin -768 -457 100 0 hwin#1112
xform 0 -672 -416
p -765 -424 100 0 -1 val(in):$(top)$(mech3)Scale
use hwin -768 -489 100 0 hwin#1113
xform 0 -672 -448
p -765 -456 100 0 -1 val(in):$(top)$(mech3)Offset
use hwin -768 -41 100 0 hwin#1115
xform 0 -672 0
p -765 -8 100 0 -1 val(in):$(top)$(mech3)AgOut
use inhier -576 -640 100 2048 OSLNK
xform 0 -544 -640
use hwout -992 343 100 0 hwout#1109
xform 0 -896 384
p -896 375 100 0 -1 val(outp):$(top)$(mech3)AgTAppl1 PP NMS
use hwout -992 279 100 0 hwout#1108
xform 0 -896 320
p -896 311 100 0 -1 val(outp):$(top)$(mech3)AgTarget PP NMS
use hwout -992 215 100 0 hwout#1107
xform 0 -896 256
p -896 247 100 0 -1 val(outp):$(top)$(mech3)AgTAppl2 PP NMS
use hwout -992 727 100 0 hwout#1084
xform 0 -896 768
p -896 759 100 0 -1 val(outp):$(top)$(mech1)AgTAppl1 PP NMS
use hwout -992 535 100 0 hwout#1083
xform 0 -896 576
p -896 567 100 0 -1 val(outp):$(top)$(mech2)AgTAppl1 PP NMS
use hwout -992 471 100 0 hwout#1005
xform 0 -896 512
p -896 503 100 0 -1 val(outp):$(top)$(mech2)AgTarget PP NMS
use hwout -992 407 100 0 hwout#1004
xform 0 -896 448
p -896 439 100 0 -1 val(outp):$(top)$(mech2)AgTAppl2 PP NMS
use hwout -992 599 100 0 hwout#999
xform 0 -896 640
p -896 631 100 0 -1 val(outp):$(top)$(mech1)AgTAppl2 PP NMS
use hwout -992 663 100 0 hwout#998
xform 0 -896 704
p -896 695 100 0 -1 val(outp):$(top)$(mech1)AgTarget PP NMS
use hwout 48 -585 100 0 hwout#1092
xform 0 144 -544
p 144 -553 100 0 -1 val(outp):$(agtop)$(top)probeOffset.J PP NMS
use egenSubA -1312 832 100 768 lowA
xform 0 -1232 432
p -1440 512 100 1280 1 NOJ:6
p -1312 -64 100 768 1 PREC:3
p -1312 -32 100 768 1 PV:$(top)$(mech)
p -1312 0 100 768 1 SNAM:wfsFolInSub
use egenSubD -384 96 100 768 AgOut
xform 0 -304 -304
p -128 -512 100 768 1 FTVJ:DOUBLE
p -64 -544 100 768 1 NOVJ:9
p -384 -800 100 768 1 PREC:3
p -384 -736 100 768 1 PV:$(top)$(mech)
p -384 -768 100 768 1 SNAM:wfsFolOutSub
p -160 -534 75 0 -1 pproc(OUTJ):PP
use egenSub 224 704 100 768 probeOffset
xform 0 304 304
p 128 672 100 1280 1 FTA:DOUBLE
p 128 608 100 1280 1 FTB:DOUBLE
p 128 544 100 1280 1 FTC:DOUBLE
p 128 480 100 1280 1 FTD:DOUBLE
p 112 96 100 1280 1 FTJ:DOUBLE
p 480 672 100 768 1 FTVA:DOUBLE
p 480 608 100 768 1 FTVB:DOUBLE
p 480 544 100 768 1 FTVC:DOUBLE
p 480 480 100 768 1 FTVD:DOUBLE
p 480 416 100 768 1 FTVE:DOUBLE
p 480 352 100 768 1 FTVF:DOUBLE
p 48 64 100 1280 1 NOJ:9
p 224 -192 100 768 1 PREC:3
p 224 -128 100 768 1 PV:$(realtop)$(top)
p 224 -160 100 768 1 SNAM:wfsFolTestOutSub
use egenSub -2208 864 100 768 TestIn
xform 0 -2128 464
p -2304 832 100 1280 1 FTA:DOUBLE
p -2304 768 100 1280 1 FTB:DOUBLE
p -2304 704 100 1280 1 FTC:DOUBLE
p -2304 640 100 1280 1 FTD:DOUBLE
p -1952 832 100 768 1 FTVA:DOUBLE
p -1952 768 100 768 1 FTVB:DOUBLE
p -1952 704 100 768 1 FTVC:DOUBLE
p -1952 640 100 768 1 FTVD:DOUBLE
p -1952 576 100 768 1 FTVE:DOUBLE
p -1952 512 100 768 1 FTVF:DOUBLE
p -1952 256 100 768 1 FTVJ:DOUBLE
p -1888 224 100 768 1 NOVJ:6
p -2208 -32 100 768 1 PREC:3
p -2208 32 100 768 1 PV:$(top)$(mech)
p -2208 0 100 768 1 SNAM:wfsFolTestInSub
p -1984 234 75 0 -1 pproc(OUTJ):PP
use bc200tr -2560 -1528 -100 0 frame
xform 0 -880 -224
p 0 -1360 100 0 1 author:H.T.Yamada
p 240 -1376 100 0 -1 border:C
p 0 -1392 100 0 1 checked:H.T.Yamada
p 272 -1376 100 0 -1 date:1998-04-15
p 524 -1372 100 1792 -1 page:1
p 240 -1248 100 0 -1 project:Near Infra-Red Imager
p 240 -1312 100 0 -1 title:Two axis continuous component CAD records
[comments]
