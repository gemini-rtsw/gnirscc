[schematic2]
uniq 142
[tools]
[detail]
w -518 1195 100 0 n#139 embbis.pattern.VAL -608 1104 -576 1104 -576 1184 -400 1184 -400 1120 embbos.embbos#124.DOL
w -486 1099 100 0 n#138 embbis.pattern.FLNK -608 1136 -512 1136 -512 1088 -400 1088 embbos.embbos#124.SLNK
w -110 1131 100 0 n#131 embbos.embbos#124.OUT -144 1088 -112 1088 -112 1120 -48 1120 egenSub.dataSimul.A
w -706 875 100 0 n#123 ebis.dataSimulation.FLNK -480 768 -480 864 -872 864 -872 608 -736 608 ebos.ebos#120.SLNK
w -622 699 100 0 n#122 junction -448 736 -448 688 -736 688 -736 640 ebos.ebos#120.DOL
w -678 963 100 0 n#122 ebis.dataSimulation.VAL -480 736 -448 736 -448 960 -848 960 inhier.ADCSimIn.P
w -462 587 100 0 n#121 ebos.ebos#120.OUT -480 576 -384 576 junction
w -246 547 100 0 n#121 junction -384 576 -384 544 -48 544 egenSub.dataSimul.J
[cell use]
use embbos -400 999 100 0 embbos#124
xform 0 -272 1088
p -656 894 100 0 0 OMSL:closed_loop
p -144 1088 75 768 -1 pproc(OUT):PP
use ebos -736 519 100 0 ebos#120
xform 0 -608 608
p -1056 558 100 0 0 OMSL:closed_loop
p -480 576 75 768 -1 pproc(OUT):PP
use egenSub 24 1160 100 0 dataSimul
xform 0 96 752
p 48 776 60 1536 1 FTA:LONG
p 48 760 60 1536 1 FTB:LONG
p -271 -155 100 0 0 FTJ:LONG
p -271 101 100 0 0 FTVA:LONG
p -271 101 100 0 0 FTVB:LONG
p -336 734 100 0 0 INAM:dataSimInit
p -336 702 100 0 0 SNAM:dataSimCalc
use inhier -864 919 100 0 ADCSimIn
xform 0 -848 960
use ebis -676 816 100 0 dataSimulation
xform 0 -608 752
p -960 590 100 0 0 ONAM:ON
p -736 654 100 0 0 OSV:MINOR
p -960 622 100 0 0 ZNAM:OFF
use notes 1444 1387 100 0 notes#81
xform 0 1700 1572
p 1472 1698 100 0 -1 COMMENT1:NOTES:  This will set the system into 
p 1472 1666 100 0 -1 COMMENT2:data simulation mode.
use CBorder -1056 -552 -100 0 frame
xform 0 624 752
p 1512 -416 100 1536 1 Date:24 Apr 97
p 1744 -336 150 1536 -1 Title:dataSimul.sch
use embbis -796 1176 100 0 pattern
xform 0 -736 1120
p -736 1038 100 0 0 FRST:Rows
p -928 1038 100 0 0 FRVL:4
p -736 1006 100 0 0 FVST:Rings
p -928 1006 100 0 0 FVVL:5
p -736 1134 100 0 0 ONST:ADC Pattern
p -928 1134 100 0 0 ONVL:1
p -800 1056 65 1536 1 PV:$(top)
p -736 942 100 0 0 SVST:Cars
p -928 942 100 0 0 SVVL:7
p -736 974 100 0 0 SXST:AndOr
p -928 974 100 0 0 SXVL:6
p -736 1070 100 0 0 THST:KPNO
p -928 1070 100 0 0 THVL:3
p -736 1102 100 0 0 TWST:Check
p -928 1102 100 0 0 TWVL:2
p -736 1166 100 0 0 ZRST:Live Video
p -896 1152 75 1280 -1 pproc(INP):PP
[comments]
