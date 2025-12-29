[schematic2]
uniq 14
[tools]
[detail]
w 776 779 100 0 n#13 hwin.hwin#8.in 800 768 800 768 ecalcs.hBCalc.SDIS
w 1166 1003 100 0 $(top)Health ecalcs.hBCalc.VAL 1088 992 1280 992 outhier.hBeatOut.p
w 654 1355 100 0 $(top)Health junction 1152 992 1152 1344 192 1344 192 1216 224 1216 elongouts.hBLo.DOL
w 622 1195 100 0 n#2 elongouts.hBLo.VAL 480 1184 800 1184 ecalcs.hBCalc.INPA
[cell use]
use hwin 608 727 100 0 hwin#8
xform 0 704 768
p 611 760 100 0 -1 val(in):1
use notes 1816 1615 100 0 notes#7
xform 0 2072 1800
p 1844 1926 100 0 -1 COMMENT1:NOTES:  This schematic implements a
p 1844 1894 100 0 -1 COMMENT2:counter which connects to the heartBeat
p 1844 1864 100 0 -1 COMMENT3:record.  This serves as an indication
p 1844 1832 100 0 -1 COMMENT4:that the software system is functional.
use CBorder -620 -312 -100 0 frame
xform 0 1060 992
p 1948 -176 100 1536 1 Date:24 Apr 97
p 2052 -80 300 1792 -1 Dnumber:
p 2180 -96 150 1536 -1 Title:heartBeat.sch
use outhier 1248 951 100 0 hBeatOut
xform 0 1264 992
use ecalcs 872 1224 100 0 hBCalc
xform 0 944 976
p 880 952 65 1536 1 CALC:A+1
p 880 936 65 1536 1 PV:$(top)
p 768 1184 75 1280 -1 pproc(INPA):PP
use elongouts 288 1256 100 0 hBLo
xform 0 352 1184
p 304 1104 80 1792 1 OMSL:closed_loop
p 232 1064 100 0 1 PV:$(top)
[comments]
