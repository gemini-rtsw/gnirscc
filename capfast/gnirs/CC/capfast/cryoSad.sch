[schematic2]
uniq 12
[tools]
[detail]
w 1352 483 100 0 n#11 hwin.hwin#10.in 1376 480 1376 480 esirs.cryoState.INP
w 1358 835 100 0 n#9 hwin.hwin#8.in 1376 832 1376 832 esirs.M_SW.INP
w 654 483 100 0 n#7 hwin.hwin#6.in 672 480 672 480 esirs.C_SW.INP
w 654 835 100 0 n#5 hwin.hwin#4.in 672 832 672 832 esirs.C_M_SW.INP
[cell use]
use hwin 480 791 100 0 hwin#4
xform 0 576 832
p 483 824 100 0 -1 val(in):$(top)cryoSub.VALA
use hwin 480 439 100 0 hwin#6
xform 0 576 480
p 483 472 100 0 -1 val(in):$(top)cryoSub.VALC
use hwin 1184 791 100 0 hwin#8
xform 0 1280 832
p 1187 824 100 0 -1 val(in):$(top)cryoSub.VALB
use hwin 1184 439 100 0 hwin#10
xform 0 1280 480
p 1187 472 100 0 -1 val(in):$(top)cryoSub.VALD
use esirs 672 583 100 0 C_M_SW
xform 0 880 736
p 720 896 100 0 -1 DESC:Cryo Computer/Manual Switch
p 768 704 100 0 1 FTVL:STRING
p 768 672 100 0 1 SCAN:1 second
p 784 576 100 1024 0 name:$(sadtop)$(I)
p 624 832 75 1024 -1 pproc(INP):PP
use esirs 1376 583 100 0 M_SW
xform 0 1584 736
p 1424 896 100 0 -1 DESC:Cryo Manual Switch State
p 1472 704 100 0 1 FTVL:STRING
p 1456 672 100 0 1 SCAN:1 second
p 1488 576 100 1024 0 name:$(sadtop)$(I)
p 1328 832 75 1024 -1 pproc(INP):PP
use esirs 672 231 100 0 C_SW
xform 0 880 384
p 720 544 100 0 -1 DESC:Cryo Computer Switch State
p 768 352 100 0 1 FTVL:STRING
p 768 320 100 0 1 SCAN:1 second
p 784 224 100 1024 0 name:$(sadtop)$(I)
p 624 480 75 1024 -1 pproc(INP):PP
use esirs 1376 231 100 0 cryoState
xform 0 1584 384
p 1424 544 100 0 -1 DESC:Cryo Head State
p 1472 352 100 0 1 FTVL:STRING
p 1472 320 100 0 1 SCAN:1 second
p 1488 224 100 1024 0 name:$(sadtop)$(I)
p 1328 480 75 1024 -1 pproc(INP):PP
use ba200tr 320 -136 -100 0 frame
xform 0 1120 488
p 1376 80 200 0 -1 filename:cryosad.sch
[comments]
