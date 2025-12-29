[schematic2]
uniq 164
[tools]
[detail]
w -1214 747 -100 0 c#163 inhier.SLNK.P -1248 736 -1120 736 elongouts.pushCar.SLNK
w -1214 779 -100 0 c#160 inhier.IVAL.P -1248 768 -1120 768 elongouts.pushCar.DOL
w 962 675 100 0 n#127 hwin.hwin#126.in 928 672 1056 672 ecars.applyC.ICID
w 164 -197 100 0 n#122 ecars.parkC.VAL -224 -832 160 -832 160 448 352 448 egenSub.combCar.INPH
w 132 -133 100 0 n#121 ecars.parkC.IVAL -544 -832 -576 -832 -576 -768 128 -768 128 512 352 512 egenSub.combCar.INPG
w 68 59 100 0 n#120 ecars.datumC.CLID -224 -448 64 -448 64 576 352 576 egenSub.combCar.INPF
w 36 139 100 0 n#119 ecars.datumC.IVAL -544 -416 -576 -416 -576 -352 32 -352 32 640 352 640 egenSub.combCar.INPE
w 82 227 100 0 n#118 ecars.compC.FLNK -224 480 -128 480 -128 224 352 224 egenSub.combCar.SLNK
w 2 -221 100 0 n#118 ecars.initC.FLNK -224 -224 288 -224 288 224 junction
w 2 -637 100 0 n#118 ecars.datumC.FLNK -224 -640 288 -640 288 -224 junction
w 2 -1053 100 0 n#118 ecars.parkC.FLNK -224 -1056 288 -1056 288 -640 junction
w -608 -861 100 0 n#113 elongins.parkID.VAL -624 -864 -544 -864 ecars.parkC.ICID
w -608 -445 100 0 n#108 elongins.datumID.VAL -624 -448 -544 -448 ecars.datumC.ICID
w -734 707 100 0 n#105 elongouts.pushCar.OUT -864 704 -544 704 ecars.compC.IVAL
w -142 899 100 0 n#105 junction -576 704 -576 896 352 896 egenSub.combCar.INPA
w -36 331 100 0 n#103 ecars.initC.CLID -224 -32 -32 -32 -32 704 352 704 egenSub.combCar.INPD
w -68 443 100 0 n#102 ecars.initC.IVAL -544 0 -576 0 -576 128 -64 128 -64 768 352 768 egenSub.combCar.INPC
w 82 835 100 0 n#101 ecars.compC.CLID -224 672 -128 672 -128 832 352 832 egenSub.combCar.INPB
w -648 675 -100 0 ICID inhier.ICID.P -704 672 -544 672 ecars.compC.ICID
w 776 195 100 0 n#92 egenSub.combCar.FLNK 640 192 960 192 960 512 1056 512 ecars.applyC.SLNK
w 808 899 100 0 n#90 egenSub.combCar.OUTA 640 896 1024 896 1024 704 1056 704 ecars.applyC.IVAL
w -608 -29 100 0 n#27 elongins.initID.VAL -624 -32 -544 -32 ecars.initC.ICID
s -912 1376 400 256 Gemini Near Infrared Imager - Command Action Response records
s 560 -1568 500 512 wfsSysCar.sch
[cell use]
use inhier -1280 736 100 2048 SLNK
xform 0 -1248 736
use inhier -1280 768 100 2048 IVAL
xform 0 -1248 768
use inhier -736 672 100 2048 ICID
xform 0 -704 672
use elongouts -1120 647 100 0 pushCar
xform 0 -992 736
p -1056 576 100 768 1 OMSL:closed_loop
p -1056 608 100 0 1 PV:$(top)
p -864 704 75 768 -1 pproc(OUT):PP
use hwin 736 631 100 0 hwin#126
xform 0 832 672
p 739 664 100 0 -1 val(in):$(top)apply.CLID
use egenSub 352 135 100 0 combCar
xform 0 496 560
p 192 912 100 0 1 FTA:LONG
p 192 848 100 0 1 FTB:LONG
p 192 784 100 0 1 FTC:LONG
p 192 720 100 0 1 FTD:LONG
p 192 656 100 0 1 FTE:LONG
p 192 592 100 0 1 FTF:LONG
p 192 528 100 0 1 FTG:LONG
p 192 464 100 0 1 FTH:LONG
p 192 400 100 0 1 FTI:LONG
p 192 336 100 0 1 FTJ:LONG
p 720 912 100 0 1 FTVA:LONG
p 720 848 100 0 1 FTVB:LONG
p 720 784 100 0 1 FTVC:LONG
p 416 80 100 0 1 PV:$(top)
p 416 112 100 0 1 SNAM:cicsCarValCombine
use bd200tr -2944 -1768 -100 0 frame
xform 0 -304 -64
p 672 -1536 200 0 1 author:S.M.Beard
p 672 -1616 200 0 1 checked:B.Goodrich
p 1280 -1552 200 0 -1 date:11 Apr 97
p 1200 -1280 200 0 -1 project:Gemini Near Infra-Red OIWFS
p 1200 -1424 200 0 -1 title:System Command Action Response records
use notes 736 -233 100 0 notes#125
xform 0 992 -48
p 1264 -82 100 0 0 author:S.M. Beard
p 752 64 100 768 -1 comment0:The client ID for the applyC CAR record
p 752 30 100 768 -1 comment1:comes directly from the CLID field of
p 752 -2 100 768 -1 comment2:the top level APPLY record rather than
p 752 -32 100 768 -1 comment3:from the output of the "combCar" genSub.
p 752 -64 100 768 -1 comment4:This is a Gemini requirement.
use notes -1504 -1129 100 0 notes#107
xform 0 -1248 -944
p -976 -978 100 0 0 author:S.M. Beard
p -1488 -832 100 768 -1 comment0:The client IDs for these CAR records
p -1488 -866 100 768 -1 comment1:cannot be set directly, due to the
p -1488 -898 100 768 -1 comment2:properties of its ICID link, so this
p -1488 -928 100 768 -1 comment3:information is fed through "longin"
p -1488 -960 100 768 -1 comment4:records, as shown.
use notes 736 -713 100 0 notes#106
xform 0 992 -528
p 1264 -562 100 0 0 author:S.M. Beard
p 752 -416 100 768 -1 comment0:This schematic contains the systemwide
p 752 -450 100 768 -1 comment1:Command Action Response (CAR) records
p 752 -482 100 768 -1 comment2:for the NIRI WFS, which are currently:
p 752 -544 100 768 -1 comment4:initC  - State of initialization action.
p 752 -576 100 768 -1 comment5:datumC - State of datum action.
p 752 -608 100 768 -1 comment6:parkC  - State of park action.
p 752 -640 100 768 -1 comment7:compC  - State of all component actions.
p 752 -672 100 768 -1 comment8:applyC - Overall state of ALL actions.
use elongins -880 -89 100 0 initID
xform 0 -752 -16
p -816 -112 100 0 1 EGU:client ID
p -816 -144 100 0 1 PV:$(top)
use elongins -880 -505 100 0 datumID
xform 0 -752 -432
p -800 -528 100 0 1 EGU:client ID
p -800 -560 100 0 1 PV:$(top)
use elongins -880 -921 100 0 parkID
xform 0 -752 -848
p -816 -960 100 0 1 EGU:client ID
p -816 -992 100 0 1 PV:$(top)
use ecars -544 423 100 0 compC
xform 0 -384 592
p -480 384 100 0 1 DESC:All components CAR record
p -480 352 100 0 1 PV:$(top)
use ecars -568 -264 100 0 initC
xform 0 -384 -112
p -464 -304 100 0 1 DESC:Initialisation CAR record
p -464 -336 100 0 1 PV:$(top)
use ecars 1056 423 100 0 applyC
xform 0 1216 592
p 1136 384 100 0 1 DESC:Top level $(name) CAR record
p 1136 352 100 0 1 PV:$(top)
use ecars -568 -680 100 0 datumC
xform 0 -384 -528
p -464 -704 100 0 1 DESC:Datum CAR record
p -464 -736 100 0 1 PV:$(top)
use ecars -568 -1096 100 0 parkC
xform 0 -384 -944
p -464 -1120 100 0 1 DESC:Park CAR record
p -464 -1152 100 0 1 PV:$(top)
[comments]
