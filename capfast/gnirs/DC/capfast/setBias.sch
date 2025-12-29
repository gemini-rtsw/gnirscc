[schematic2]
uniq 199
[tools]
[detail]
w 482 1611 100 0 n#196 ecalcs.biasCalc.FLNK 448 1600 576 1600 576 1520 672 1520 eais.achvdBias.SLNK
w 546 1563 100 0 n#195 ecalcs.biasCalc.VAL 448 1568 480 1568 480 1552 672 1552 eais.achvdBias.INP
w 1010 1323 100 0 n#189 ebis.biasFailed.VAL 928 1312 1152 1312 outhier.{dBiasFailed}.p
w 994 1515 100 0 n#188 eais.achvdBias.VAL 928 1504 1120 1504 outhier.{achvdBias}.p
w 930 1091 100 0 n#175 eaos.dBias.OUT 928 1088 992 1088 hwout.hwout#174.outp
s -368 1648 100 0 Thats (c80_95:a10) - (c80_95:a1)
s -368 1680 100 0 Bias is VDDUC - VDET
[cell use]
use ecalcs 160 1287 100 0 biasCalc
xform 0 304 1552
p 208 1264 100 0 1 CALC:A-B
p 176 1200 100 0 1 SCAN:.5 second
p -256 1568 100 0 1 def(INPA):$(top)c80_95:a10
p -256 1520 100 0 1 def(INPB):$(top)c80_95:a1
p 128 1760 75 1280 -1 pproc(INPA):NPP
p 128 1728 75 1280 -1 pproc(INPB):NPP
use hwout 992 1047 100 0 hwout#174
xform 0 1088 1088
p 1080 1088 65 1536 -1 val(outp):@Node=2,Var=9,Grp=2,Idx=0
use eaos 740 1192 100 0 dBias
xform 0 800 1120
p 559 1354 100 0 0 DTYP:wFireVarMsg
p 640 1006 100 0 0 HHSV:MAJOR
p 756 968 65 0 1 HIGH:.9
p 756 948 65 0 1 HIHI:1.2
p 640 974 100 0 0 HSV:MINOR
p 640 910 100 0 0 LLSV:MAJOR
p 756 988 65 0 1 LOLO:0.0000000e+00
p 756 1008 65 0 1 LOW:0.1
p 640 942 100 0 0 LSV:MINOR
p 416 974 100 0 0 PREC:4
p 740 1036 65 1536 1 PV:$(top)
use eaos 740 904 100 0 vDet
xform 0 800 832
p 559 1066 100 0 0 DTYP:Soft Channel
p 756 680 65 0 1 HIGH:5.0
p 756 660 65 0 1 HIHI:5.0
p 756 700 65 0 1 LOLO:0.0
p 756 720 65 0 1 LOW:0.0
p 416 686 100 0 0 PREC:4
p 740 748 65 1536 1 PV:$(top)
use CBorder -616 -28 -100 0 frame
xform 0 1064 1276
p 1952 108 100 1536 1 Date:23 Apr 97
p 2056 204 300 1792 -1 Dnumber:
p 2184 188 150 1536 -1 Title:setBias.sch
use outhier 1120 1271 100 0 {dBiasFailed}
xform 0 1136 1312
use outhier 1088 1463 100 0 {achvdBias}
xform 0 1104 1504
use ebis 736 1384 100 0 biasFailed
xform 0 800 1328
p 499 1403 100 0 0 DESC:True if Bias was not correctly set
p 448 1166 100 0 0 ONAM:Bias_Failed
p 672 1230 100 0 0 OSV:MAJOR
p 752 1296 65 1536 1 PV:$(top)
p 448 1198 100 0 0 ZNAM:Bias_OK
p 672 1262 100 0 0 ZSV:NO_ALARM
p 640 1360 75 1280 -1 pproc(INP):PP
use eais 736 1576 100 0 achvdBias
xform 0 800 1520
p 816 1488 100 0 0 PREC:3
p 744 1496 65 1536 1 PV:$(top)
p 416 1518 100 0 0 SCAN:Passive
p 640 1552 75 1280 -1 pproc(INP):NPP
[comments]
