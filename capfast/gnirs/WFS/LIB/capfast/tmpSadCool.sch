[schematic2]
uniq 525
[tools]
[detail]
w -1086 483 100 0 n#517 hwin.hwin#519.in -1120 480 -992 480 esirs.Rate2.INP
w -1086 1219 100 0 n#212 hwin.hwin#211.in -1120 1216 -992 1216 esirs.Rate1.INP
s 1040 -1024 500 512 tmpSadCool
s -1424 208 100 0 .
[cell use]
use hwin -1312 1175 100 0 hwin#211
xform 0 -1216 1216
p -1309 1208 100 0 -1 val(in):$(eng)$(mech)M1.RATE
use hwin -1312 439 100 0 hwin#519
xform 0 -1216 480
p -1309 472 100 0 -1 val(in):$(eng)$(mech)M2.RATE
use esirs -928 1248 100 768 Rate1
xform 0 -784 1120
p -928 736 100 768 1 DESC:Cooling motor 1 rate
p -928 768 100 768 1 EGU:ustep / s
p -928 960 100 768 1 FDSC:Cooling motor 1 rate
p -928 928 100 768 1 FTVL:DOUBLE
p -928 800 100 768 1 PREC:1
p -928 832 100 768 1 PV:$(sadtop)$(mech)
p -928 864 100 768 1 SCAN:1 second
p -928 896 100 768 1 SNAM:
p -992 1216 75 1280 -1 palrm(INP):MS
use esirs -928 512 100 768 Rate2
xform 0 -784 384
p -928 0 100 768 1 DESC:Cooling motor 2 rate
p -928 32 100 768 1 EGU:ustep / s
p -928 224 100 768 1 FDSC:Cooling motor 2 rate
p -928 192 100 768 1 FTVL:DOUBLE
p -928 64 100 768 1 PREC:1
p -928 96 100 768 1 PV:$(sadtop)$(mech)
p -928 128 100 768 1 SCAN:1 second
p -928 160 100 768 1 SNAM:
p -992 480 75 1280 -1 palrm(INP):MS
use bc200tr -1504 -1176 -100 0 frame
xform 0 176 128
p 1072 -1008 100 0 1 author:H.T. Yamada
p 1296 -1024 100 0 -1 border:C
p 1072 -1040 100 0 1 checked:H.T. Yamada
p 1328 -1024 100 0 -1 date:26 Nov 1997
p 1312 -896 100 0 -1 project:NIRI/GNIRS CC/WFS Interlocks
p 1312 -960 100 0 -1 title:Interlock Status/Alarm Database
[comments]
