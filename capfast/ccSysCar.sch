[schematic2]
uniq 214
[tools]
[detail]
w 1162 163 100 0 n#213 egenSub.applyMessSelect.OUTA 1392 -128 1504 -128 1504 160 880 160 880 640 1056 640 ecars.applyC.IMSS
w 1668 -309 100 0 n#212 egenSub.applyMessSelect.FLNK 1392 -832 1664 -832 1664 224 928 224 928 512 1056 512 ecars.applyC.SLNK
w 788 43 100 0 n#211 egenSub.combCar.VALC 640 800 784 800 784 -704 1104 -704 egenSub.applyMessSelect.INPJ
w 960 -125 100 0 IMSS inhier.IMSS.P 864 -128 1104 -128 egenSub.applyMessSelect.INPA
w 698 -797 100 0 n#207 egenSub.combCar.FLNK 640 192 720 192 720 -64 352 -64 352 -800 1104 -800 egenSub.applyMessSelect.SLNK
w 1026 683 100 0 n#203 hwin.hwin#199.in 1056 672 1056 672 ecars.applyC.ICID
w 762 867 100 0 n#202 egenSub.combCar.VALB 640 864 944 864 944 784 992 784 free
w -990 555 100 0 ICID inhier.ICID.P -1248 544 -672 544 -672 672 -544 672 ecars.compC.ICID
w -1150 747 100 0 SLNK inhier.SLNK.P -1248 736 -992 736 elongouts.compPush.SLNK
w -1214 907 100 0 IVAL inhier.IVAL.P -1248 896 -1120 896 -1120 768 -992 768 elongouts.compPush.DOL
w -670 715 100 0 n#105 ecars.compC.IVAL -544 704 -736 704 elongouts.compPush.OUT
w -142 899 100 0 n#105 junction -576 704 -576 896 352 896 egenSub.combCar.INPA
w 82 227 100 0 n#118 ecars.compC.FLNK -224 480 -128 480 -128 224 352 224 egenSub.combCar.SLNK
w 82 835 100 0 n#101 ecars.compC.CLID -224 672 -128 672 -128 832 352 832 egenSub.combCar.INPB
w 808 899 100 0 n#90 egenSub.combCar.OUTA 640 896 1024 896 1024 704 1056 704 ecars.applyC.IVAL
s -2816 -704 100 0 via EPICS channel access.
s -2816 -672 100 0 individual component CAR records
s -2816 -640 100 0 These inputs are written to by
s -912 1376 400 256 GNIRS - Command action Response records
s 560 -1568 500 512 ccSysCar.sch
[cell use]
use inhier -1264 855 100 0 IVAL
xform 0 -1248 896
use inhier -1264 695 100 0 SLNK
xform 0 -1248 736
use inhier -1264 503 100 0 ICID
xform 0 -1248 544
use inhier 848 -169 100 0 IMSS
xform 0 864 -128
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
use egenSub 1104 -889 100 0 applyMessSelect
xform 0 1248 -464
p 881 -1115 100 0 0 FTA:STRING
p 881 -1115 100 0 0 FTB:STRING
p 881 -1147 100 0 0 FTC:STRING
p 881 -1179 100 0 0 FTD:STRING
p 881 -1211 100 0 0 FTE:STRING
p 1280 -672 100 0 0 FTJ:LONG
p 881 -1115 100 0 0 FTVA:STRING
p 1248 -592 100 0 0 SNAM:cicsCarMessSelect
p 1216 -896 100 1024 0 name:$(top)$(I)
use hwin 864 631 100 0 hwin#199
xform 0 960 672
p 867 664 100 0 -1 val(in):$(top)apply.CLID
use elongouts -992 647 100 0 compPush
xform 0 -864 736
p -928 608 100 0 1 OMSL:closed_loop
p -736 704 75 768 -1 pproc(OUT):PP
use bd200tr -2944 -1768 -100 0 frame
xform 0 -304 -64
p 672 -1536 200 0 1 author:S.M.Beard
p 672 -1616 200 0 1 checked:B.Goodrich
p 1280 -1552 200 0 -1 date:11 Apr 97
p 1200 -1280 200 0 -1 project:Gemini Near Infrared Imager
p 1200 -1424 200 0 -1 title:System Command Action Response records
use notes 1712 983 100 0 notes#125
xform 0 1968 1168
p 2240 1134 100 0 0 author:S.M. Beard
p 1728 1278 100 768 -1 comment0:The client ID for the applyC CAR record
p 1728 1246 100 768 -1 comment1:comes directly from the CLID field of
p 1728 1216 100 768 -1 comment2:the top level APPLY record rather than
p 1728 1184 100 768 -1 comment3:from the output of the "combCar" genSub.
p 1728 1152 100 768 -1 comment4:This is a Gemini requirement.
use notes -1472 -1161 100 0 notes#107
xform 0 -1216 -976
p -944 -1010 100 0 0 author:S.M. Beard
p -1456 -866 100 768 -1 comment0:The client IDs for these CAR records
p -1456 -898 100 768 -1 comment1:cannot be set directly, due to the
p -1456 -928 100 768 -1 comment2:properties of its ICID link, so this
p -1456 -960 100 768 -1 comment3:information is fed through "longin"
p -1456 -992 100 768 -1 comment4:records, as shown.
use notes 1712 407 100 0 notes#106
xform 0 1968 592
p 2240 558 100 0 0 author:S.M. Beard
p 1724 702 100 768 -1 comment0:This schematic contains the systemwide
p 1724 670 100 768 -1 comment1:Command Action Response (CAR) records
p 1724 640 100 768 -1 comment2:for the NIRI CC, which are currently:
p 1724 576 100 768 -1 comment4:initC  - State of initialization action.
p 1724 544 100 768 -1 comment5:datumC - State of datum action.
p 1724 512 100 768 -1 comment6:parkC  - State of park action.
p 1724 480 100 768 -1 comment7:compC  - State of all component actions.
p 1724 448 100 768 -1 comment8:applyC - Overall state of ALL actions.
use ecars -544 423 100 0 compC
xform 0 -384 592
p -480 384 100 0 1 DESC:All components CAR record
p -480 352 100 0 1 PV:$(top)
use ecars 1056 423 100 0 applyC
xform 0 1216 592
p 1136 384 100 0 1 DESC:Top level $(name) CAR record
p 1136 352 100 0 1 PV:$(top)
[comments]
