[schematic2]
uniq 611
[tools]
[detail]
w 258 1035 100 0 n#610 ecalcs.MinCalc.FLNK 224 1024 352 1024 esirs.Min.SLNK
w 178 643 100 0 n#462 junction -480 1152 -480 640 896 640 ecalcs.MaxCalc.INPB
w 508 523 100 0 n#608 esirs.Min.FLNK 768 1184 928 1184 928 768 512 768 512 288 896 288 ecalcs.MaxCalc.SLNK
w -366 1187 100 0 n#461 esirs.EngMin.VAL -608 1184 -64 1184 ecalcs.MinCalc.INPA
w 162 675 100 0 n#461 junction -512 1184 -512 672 896 672 ecalcs.MaxCalc.INPA
w -548 891 100 0 n#462 esirs.EngMax.VAL -608 640 -544 640 -544 1152 -64 1152 ecalcs.MinCalc.INPB
w 130 -629 100 0 n#606 ecalcs.Recalc.FLNK 96 -640 224 -640 esirs.Pos.SLNK
w -580 -661 100 0 n#604 esirs.EngPos.FLNK -608 -448 -576 -448 -576 -864 -192 -864 ecalcs.Recalc.SLNK
w 1218 523 100 0 n#603 ecalcs.MaxCalc.FLNK 1184 512 1312 512 esirs.Max.SLNK
w -350 803 100 0 n#600 esirs.EngMin.FLNK -608 1216 -576 1216 -576 800 -64 800 ecalcs.MinCalc.SLNK
w -580 731 100 0 n#600 esirs.EngMax.FLNK -608 672 -576 672 -576 800 junction
w -1006 163 -100 0 n#568 ecalcs.TolCalc.INPA -864 160 -1088 160 hwin.hwin#561.in
w -1006 99 100 0 n#567 hwin.hwin#571.in -1088 96 -864 96 ecalcs.TolCalc.INPC
w -1006 131 100 0 n#566 hwin.hwin#572.in -1088 128 -864 128 ecalcs.TolCalc.INPB
w -558 -29 100 0 n#565 ecalcs.TolCalc.VAL -576 -32 -480 -32 esirs.Tol.INP
w -418 -477 -100 0 VAL ecalcs.Recalc.INPA -192 -480 -608 -480 esirs.EngPos.VAL
w 786 579 100 0 n#464 hwin.hwin#476.in 736 576 896 576 ecalcs.MaxCalc.INPD
w -174 1091 100 0 n#463 hwin.hwin#477.in -224 1088 -64 1088 ecalcs.MinCalc.INPD
w 1212 571 100 0 n#460 ecalcs.MaxCalc.VAL 1184 480 1216 480 1216 672 1312 672 esirs.Max.INP
w 786 611 100 0 n#459 hwin.hwin#479.in 736 608 896 608 ecalcs.MaxCalc.INPC
w -174 1123 100 0 n#458 hwin.hwin#481.in -224 1120 -64 1120 ecalcs.MinCalc.INPC
w 252 1083 100 0 n#457 ecalcs.MinCalc.VAL 224 992 256 992 256 1184 352 1184 esirs.Min.INP
w -302 -541 100 0 n#448 hwin.hwin#484.in -352 -544 -192 -544 ecalcs.Recalc.INPC
w -302 -509 100 0 n#447 hwin.hwin#483.in -352 -512 -192 -512 ecalcs.Recalc.INPB
w 124 -581 100 0 n#605 ecalcs.Recalc.VAL 96 -672 128 -672 128 -480 224 -480 esirs.Pos.INP
w -1134 1219 100 0 n#406 hwin.hwin#414.in -1184 1216 -1024 1216 esirs.EngMin.INP
w -1134 675 100 0 n#405 hwin.hwin#413.in -1184 672 -1024 672 esirs.EngMax.INP
w -158 515 100 0 n#387 hwin.hwin#392.in -224 512 -32 512 esirs.RdtmVal.INP
w -1134 -445 100 0 n#357 hwin.hwin#373.in -1184 -448 -1024 -448 esirs.EngPos.INP
s -384 -192 100 768 triggered by
s -384 -160 100 768 This record is
s 656 -240 100 768 Initialize with pvload.
s 656 -112 100 768 If this is 0, then the
s 656 -208 100 768 to be respected.
s 96 832 100 768 This "calc" record
s 96 800 100 768 converts engineering
s 1040 -1024 400 1280 comp1mSadEng
s -944 -576 100 768 Engineering position
s -944 -608 100 768 output and link
s 48 400 100 768 Returned value for
s 48 336 100 768 output and link.
s -912 480 100 768 output and link
s -912 544 100 768 Maximum engineering
s -912 1088 100 768 Minimum engineering
s -912 1056 100 768 position
s 96 768 100 768 position back to
s 96 736 100 768 user units.
s 1120 320 100 768 This "calc" record
s 1120 288 100 768 converts engineering
s 1120 256 100 768 position back to
s 1120 224 100 768 user units.
s 32 -832 100 768 This "calc" record
s 32 -864 100 768 converts engineering
s 32 -896 100 768 position back to
s 32 -928 100 768 user units.
s 48 368 100 768 redatum command,
s 656 -144 100 768 mechanism is circular,
s 656 -176 100 768 the limits don't need
s -608 160 100 768 This "calc" record
s -608 128 100 768 converts engineering
s -608 96 100 768 position back to
s -608 64 100 768 user units.
s -384 -224 100 768 EngTol.
s -912 1024 100 768 output and link
s -912 512 100 768 position
[cell use]
use esirs 640 16 100 768 EngCyclic
xform 0 784 -112
p 640 -272 100 768 1 DESC:Ignore limits? [YES=0|NO=1]
p 640 -400 100 768 1 EGU:0/1
p 640 -304 100 768 1 FDSC:Ignore limits? [YES=0|NO=1]
p 640 -336 100 768 1 FTVL:LONG
p 640 -432 100 768 1 PV:$(sadtop)$(mech)
p 640 -368 100 768 1 SNAM:
p 576 -16 75 1280 -1 palrm(INP):MS
use esirs 288 -448 100 768 Pos
xform 0 432 -576
p 288 -736 100 768 1 DESC:Current position in real-world units
p 175 -1152 100 0 0 EGU:user units
p 288 -768 100 768 1 FDSC:Current position in real-world units
p 288 -800 100 768 1 FTVL:DOUBLE
p 288 -896 100 768 1 PREC:3
p 288 -864 100 768 1 PV:$(sadtop)$(mech)
p 288 -832 100 768 1 SNAM:
p 224 -480 75 1280 -1 palrm(INP):MS
use esirs 416 1216 100 768 Min
xform 0 560 1088
p 416 928 100 768 1 DESC:Minimum position in real-world units
p 303 512 100 0 0 EGU:user units
p 416 896 100 768 1 FDSC:Minimum position in real-world units
p 416 864 100 768 1 FTVL:DOUBLE
p 416 768 100 768 1 PREC:3
p 416 800 100 768 1 PV:$(sadtop)$(mech)
p 416 832 100 768 1 SNAM:
use esirs 1376 704 100 768 Max
xform 0 1520 576
p 1376 416 100 768 1 DESC:Maximum position in real-world units
p 1263 0 100 0 0 EGU:user units
p 1376 384 100 768 1 FDSC:Maximum position in real-world units
p 1376 352 100 768 1 FTVL:DOUBLE
p 1376 256 100 768 1 PREC:3
p 1376 288 100 768 1 PV:$(sadtop)$(mech)
p 1376 320 100 768 1 SNAM:
use esirs -960 -416 100 768 EngPos
xform 0 -816 -544
p -960 -704 100 768 1 DESC:Current Engineering position
p -960 -896 100 768 1 EGU:ustep
p -960 -736 100 768 1 FDSC:Current Engineering position
p -960 -768 100 768 1 FTVL:LONG
p -960 -864 100 768 1 PV:$(sadtop)$(mech)
p -960 -832 100 768 1 SCAN:.5 second
p -960 -800 100 768 1 SNAM:
p -1024 -448 75 1280 -1 palrm(INP):MS
use esirs 32 544 100 768 RdtmVal
xform 0 176 416
p 32 256 100 768 1 DESC:Returned value from redatum command
p 32 64 100 768 1 EGU:ustep
p 32 224 100 768 1 FDSC:Returned value from redatum command
p 32 192 100 768 1 FTVL:LONG
p 32 96 100 768 1 PV:$(sadtop)$(mech)
p 32 128 100 768 1 SCAN:.5 second
p 32 160 100 768 1 SNAM:
p -32 512 75 1280 -1 palrm(INP):MS
use esirs -960 1248 100 768 EngMin
xform 0 -816 1120
p -960 960 100 768 1 DESC:Minimum position in engineering units
p -960 768 100 768 1 EGU:ustep
p -960 928 100 768 1 FDSC:Minimum position in engineering units
p -960 896 100 768 1 FTVL:LONG
p -960 800 100 768 1 PV:$(sadtop)$(mech)
p -960 832 100 768 1 SCAN:1 second
p -960 864 100 768 1 SNAM:
p -1024 1216 75 1280 -1 palrm(INP):MS
use esirs -960 704 100 768 EngMax
xform 0 -816 576
p -960 416 100 768 1 DESC:Maximum position (motor microsteps)
p -960 224 100 768 1 EGU:ustep
p -960 384 100 768 1 FDSC:Maximum position (motor microsteps)
p -960 352 100 768 1 FTVL:LONG
p -960 256 100 768 1 PV:$(sadtop)$(mech)
p -960 288 100 768 1 SCAN:1 second
p -960 320 100 768 1 SNAM:
p -1024 672 75 1280 -1 palrm(INP):MS
use esirs 1376 1168 100 768 Units
xform 0 1520 1040
p 1376 880 100 768 1 DESC:Definition of real-world units
p 1263 464 100 0 0 EGU:
p 1376 848 100 768 1 FDSC:Definition of real-world units
p 1376 816 100 768 1 FTVL:STRING
p 1376 752 100 768 1 PV:$(sadtop)$(mech)
p 1376 784 100 768 1 SNAM:
p 1312 1136 75 1280 -1 palrm(INP):MS
p 1264 1136 75 1024 -1 pproc(INP):PP
use esirs -416 0 100 768 Tol
xform 0 -272 -128
p -416 -288 100 768 1 DESC:Tolerance of offset in real-world units
p -529 -704 100 0 0 EGU:user units
p -416 -320 100 768 1 FDSC:Tolerance of offset in real-world units
p -416 -352 100 768 1 FTVL:DOUBLE
p -416 -448 100 768 1 PREC:3
p -416 -416 100 768 1 PV:$(sadtop)$(mech)
p -416 -480 100 768 1 SCAN:.5 second
p -416 -384 100 768 1 SNAM:
p -480 -32 75 1280 -1 palrm(INP):MS
p -528 -32 75 1024 -1 pproc(INP):PP
use hwin -544 -585 100 0 hwin#484
xform 0 -448 -544
p -528 -560 100 768 -1 val(in):$(top)$(mech)Offset.VAL
use hwin -544 -553 100 0 hwin#483
xform 0 -448 -512
p -528 -528 100 768 -1 val(in):$(top)$(mech)Scale.VAL
use hwin -416 1079 100 0 hwin#481
xform 0 -320 1120
p -400 1104 100 768 -1 val(in):$(top)$(mech)Scale.VAL
use hwin 544 567 100 0 hwin#479
xform 0 640 608
p 560 592 100 768 -1 val(in):$(top)$(mech)Scale.VAL
use hwin -416 1047 100 0 hwin#477
xform 0 -320 1088
p -400 1072 100 768 -1 val(in):$(top)$(mech)Offset.VAL
use hwin 544 535 100 0 hwin#476
xform 0 640 576
p 560 560 100 768 -1 val(in):$(top)$(mech)Offset.VAL
use hwin -1376 -489 100 0 hwin#373
xform 0 -1280 -448
p -1373 -456 100 0 -1 val(in):$(eng)$(mech)Mpos.VAL
use hwin -416 471 100 0 hwin#392
xform 0 -320 512
p -413 504 100 0 -1 val(in):$(eng)$(mech)Hallstep.RDTM
use hwin -1376 631 100 0 hwin#413
xform 0 -1280 672
p -1373 664 100 0 -1 val(in):$(eng)$(mech)Hallstep.HOPR
use hwin -1376 1175 100 0 hwin#414
xform 0 -1280 1216
p -1373 1208 100 0 -1 val(in):$(eng)$(mech)Hallstep.LOPR
use hwin -1280 55 100 0 hwin#571
xform 0 -1184 96
p -1264 80 100 768 -1 val(in):$(top)$(mech)Offset.VAL
use hwin -1280 87 100 0 hwin#572
xform 0 -1184 128
p -1264 112 100 768 -1 val(in):$(top)$(mech)Scale.VAL
use hwin -1280 119 100 0 hwin#561
xform 0 -1184 160
p -1264 144 100 768 -1 val(in):$(top)$(mech)EngTol.VAL
use ecalcs -128 -448 100 768 Recalc
xform 0 -48 -688
p -128 -960 100 768 1 CALC:{{A-C}/B}
p -480 -802 100 0 0 EGU:user units
p -128 -1024 100 768 1 PREC:7
p -128 -992 100 768 1 PV:$(sadtop)$(mech)
p -192 -480 75 1280 -1 palrm(INPA):MS
p -192 -512 75 1280 -1 palrm(INPB):MS
p -192 -544 75 1280 -1 palrm(INPC):MS
p -224 -480 75 1280 -1 pproc(INPA):PP
use ecalcs 0 1216 100 768 MinCalc
xform 0 80 976
p 0 704 100 768 1 CALC:{C>0}?{{A-D}/C}:{{B-D}/C}
p -352 862 100 0 0 EGU:user units
p 0 640 100 768 1 PREC:7
p 0 672 100 768 1 PV:$(sadtop)$(mech)
p -64 1184 75 1280 -1 palrm(INPA):MS
p -64 1152 75 1280 -1 palrm(INPB):MS
p -64 1120 75 1280 -1 palrm(INPC):MS
use ecalcs 960 704 100 768 MaxCalc
xform 0 1040 464
p 960 192 100 768 1 CALC:{C>0}?{{B-D}/C}:{{A-D}/C}
p 608 350 100 0 0 EGU:user units
p 960 128 100 768 1 PREC:7
p 960 160 100 768 1 PV:$(sadtop)$(mech)
p 896 672 75 1280 -1 palrm(INPA):MS
p 896 640 75 1280 -1 palrm(INPB):MS
p 896 608 75 1280 -1 palrm(INPC):MS
use ecalcs -800 192 100 768 TolCalc
xform 0 -720 -48
p -800 -320 100 768 1 CALC:{A-C}/B
p -1152 -162 100 0 0 EGU:user units
p -800 -384 100 768 1 PREC:7
p -800 -352 100 768 1 PV:$(sadtop)$(mech)
p -864 160 75 1280 -1 palrm(INPA):MS
p -864 128 75 1280 -1 palrm(INPB):MS
p -864 96 75 1280 -1 palrm(INPC):MS
p -896 160 75 1280 -1 pproc(INPA):PP
use notes 1216 -473 100 0 notes#542
xform 0 1472 -272
p 1216 -480 100 768 0 author:Hubert Yamada
p 1232 -160 100 768 -1 comment0:This schematic provides a standard
p 1232 -194 100 768 -1 comment1:interface from the low-level engineering
p 1232 -226 100 768 -1 comment2:database to the high level SIR-based
p 1232 -256 100 768 -1 comment3:status and alarm database.  It also
p 1232 -288 100 768 -1 comment4:provides a few SIR records which are
p 1232 -320 100 768 -1 comment5:present in all components.
p 1472 -128 100 1024 -1 title:comp1mSadEng
use notes 1216 -857 100 0 notes#543
xform 0 1472 -656
p 1216 -864 100 768 0 author:S.M. Beard
p 1232 -544 100 768 -1 comment0:SIR records are linked together in
p 1232 -578 100 768 -1 comment1:situations where the same quantity is
p 1232 -610 100 768 -1 comment2:stored in two or more different forms
p 1232 -640 100 768 -1 comment3:(e.g. engineering position can be
p 1232 -672 100 768 -1 comment4:translated automatically into name).
use bc200tr -1504 -1176 -100 0 frame
xform 0 176 128
p 1072 -896 100 768 1 author:S.M.Beard
p 1296 -1024 100 0 -1 border:C
p 1072 -1024 100 768 1 checked:H.Yamada
p 1328 -1024 100 0 -1 date:1999-10-19
p 1580 -1020 100 1792 -1 page:1
p 1296 -896 100 0 -1 project:Near Infra-Red Imager / IR OIWFS
p 1072 -928 100 768 1 revised:H.Yamada
p 1072 -1040 100 0 1 revision:1.0
p 1296 -944 100 0 -1 title:Interface to low-level engineering
p 1296 -976 100 768 -1 title2:status records.
[comments]
