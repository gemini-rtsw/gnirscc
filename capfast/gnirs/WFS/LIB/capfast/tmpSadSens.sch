[schematic2]
uniq 476
[tools]
[detail]
w 474 1123 -100 0 c#474 bihier.T5.p 528 1120 480 1120 esirs.Tmp5.VAL
w 474 579 -100 0 c#473 bihier.T6.p 528 576 480 576 esirs.Tmp6.VAL
w 474 35 -100 0 c#470 bihier.T7.p 528 32 480 32 esirs.Tmp7.VAL
w 474 -509 -100 0 c#469 bihier.T8.p 528 -512 480 -512 esirs.Tmp8.VAL
w -462 -509 -100 0 c#466 bihier.T4.p -416 -512 -448 -512 esirs.Tmp4.VAL
w -462 35 -100 0 c#465 bihier.T3.p -416 32 -448 32 esirs.Tmp3.VAL
w -462 579 -100 0 c#462 bihier.T2.p -416 576 -448 576 esirs.Tmp2.VAL
w -444 1123 -100 0 T0 bihier.T1.p -416 1120 -448 1120 esirs.Tmp1.VAL
w -78 67 100 0 n#456 esirs.Tmp7.INP 64 64 -160 64 hwin.hwin#460.in
w -78 -477 100 0 n#455 hwin.hwin#459.in -160 -480 64 -480 esirs.Tmp8.INP
w -78 1155 100 0 n#450 esirs.Tmp5.INP 64 1152 -160 1152 hwin.hwin#454.in
w -78 611 100 0 n#449 hwin.hwin#453.in -160 608 64 608 esirs.Tmp6.INP
w -1038 67 100 0 n#444 esirs.Tmp3.INP -864 64 -1152 64 hwin.hwin#448.in
w -1038 -477 100 0 n#443 hwin.hwin#447.in -1152 -480 -864 -480 esirs.Tmp4.INP
w 1376 1091 -100 0 HLNK esirs.Health.FLNK 1376 1088 1424 1088 outhier.HLNK.p
w 1368 1027 -100 0 OMSS esirs.Health.OMSS 1376 1024 1408 1024 bihier.OMSS.p
w 1368 1059 -100 0 OHLT esirs.Health.VAL 1376 1056 1408 1056 bihier.OHLT.p
w 826 1091 100 0 n#442 hwin.hwin#309.in 752 1088 960 1088 esirs.Health.INP
w -1038 1155 100 0 n#199 esirs.Tmp1.INP -864 1152 -1152 1152 hwin.hwin#198.in
w -1038 611 100 0 n#195 hwin.hwin#194.in -1152 608 -864 608 esirs.Tmp2.INP
s 432 1168 100 0 .
s 1040 -1024 500 512 tmpSadSens
[cell use]
use bihier 1440 1056 100 1536 OHLT
xform 0 1408 1056
use bihier 1440 1024 100 1536 OMSS
xform 0 1408 1024
use bihier -368 1120 100 2048 T1
xform 0 -416 1120
use bihier -368 576 100 2048 T2
xform 0 -416 576
use bihier -368 32 100 2048 T3
xform 0 -416 32
use bihier -368 -512 100 2048 T4
xform 0 -416 -512
use bihier 576 -512 100 2048 T8
xform 0 528 -512
use bihier 576 32 100 2048 T7
xform 0 528 32
use bihier 576 576 100 2048 T6
xform 0 528 576
use bihier 576 1120 100 2048 T5
xform 0 528 1120
use hwin -352 23 100 0 hwin#460
xform 0 -256 64
p -349 56 100 0 -1 val(in):$(eng)$(mech)Tmp6.VAL
use hwin -352 -521 100 0 hwin#459
xform 0 -256 -480
p -349 -488 100 0 -1 val(in):$(eng)$(mech)Tmp8.VAL
use hwin -352 1111 100 0 hwin#454
xform 0 -256 1152
p -349 1144 100 0 -1 val(in):$(eng)$(mech)Tmp5.VAL
use hwin -352 567 100 0 hwin#453
xform 0 -256 608
p -349 600 100 0 -1 val(in):$(eng)$(mech)Tmp6.VAL
use hwin -1344 23 100 0 hwin#448
xform 0 -1248 64
p -1341 56 100 0 -1 val(in):$(eng)$(mech)Tmp3.VAL
use hwin -1344 -521 100 0 hwin#447
xform 0 -1248 -480
p -1341 -488 100 0 -1 val(in):$(eng)$(mech)Tmp4.VAL
use hwin 560 1047 100 0 hwin#309
xform 0 656 1088
p 563 1080 100 0 -1 val(in):$(eng)$(mech).VAL
use hwin -1344 1111 100 0 hwin#198
xform 0 -1248 1152
p -1341 1144 100 0 -1 val(in):$(eng)$(mech)Tmp1.VAL
use hwin -1344 567 100 0 hwin#194
xform 0 -1248 608
p -1341 600 100 0 -1 val(in):$(eng)$(mech)Tmp2.VAL
use esirs 128 96 100 768 Tmp7
xform 0 272 -32
p 128 -416 100 768 1 DESC:Temperature sensor unused channel (7)
p 128 -384 100 768 1 EGU:K
p 128 -192 100 768 1 FDSC:Temperature sensor unused channel (7)
p 128 -224 100 768 1 FTVL:DOUBLE
p 128 -352 100 768 1 PREC:1
p 128 -320 100 768 1 PV:$(sadtop)$(mech)
p 128 -288 100 768 1 SCAN:1 second
p 128 -256 100 768 1 SNAM:
use esirs 128 -448 100 768 Tmp8
xform 0 272 -576
p 128 -960 100 768 1 DESC:Temperature sensor unused channel (8)
p 128 -928 100 768 1 EGU:K
p 128 -736 100 768 1 FDSC:Temperature sensor unused channel (8)
p 128 -768 100 768 1 FTVL:DOUBLE
p 128 -896 100 768 1 PREC:1
p 128 -864 100 768 1 PV:$(sadtop)$(mech)
p 128 -832 100 768 1 SCAN:1 second
p 128 -800 100 768 1 SNAM:
p 64 -480 75 1280 -1 palrm(INP):MS
use esirs 128 1184 100 768 Tmp5
xform 0 272 1056
p 128 672 100 768 1 DESC:Temperature sensor unused channel (5)
p 128 704 100 768 1 EGU:K
p 128 896 100 768 1 FDSC:Temperature sensor unused channel (5)
p 128 864 100 768 1 FTVL:DOUBLE
p 128 736 100 768 1 PREC:1
p 128 768 100 768 1 PV:$(sadtop)$(mech)
p 128 800 100 768 1 SCAN:1 second
p 128 832 100 768 1 SNAM:
use esirs 128 640 100 768 Tmp6
xform 0 272 512
p 128 128 100 768 1 DESC:Temperature sensor unused channel (6)
p 128 160 100 768 1 EGU:K
p 128 352 100 768 1 FDSC:Temperature sensor unused channel (6)
p 128 320 100 768 1 FTVL:DOUBLE
p 128 192 100 768 1 PREC:1
p 128 224 100 768 1 PV:$(sadtop)$(mech)
p 128 256 100 768 1 SCAN:1 second
p 128 288 100 768 1 SNAM:
p 64 608 75 1280 -1 palrm(INP):MS
use esirs -800 96 100 768 Tmp3
xform 0 -656 -32
p -800 -416 100 768 1 DESC:Engineering test point temperature
p -800 -384 100 768 1 EGU:K
p -800 -192 100 768 1 FDSC:Engineering test point temperature
p -800 -224 100 768 1 FTVL:DOUBLE
p -800 -352 100 768 1 PREC:1
p -800 -320 100 768 1 PV:$(sadtop)$(mech)
p -800 -288 100 768 1 SCAN:1 second
p -800 -256 100 768 1 SNAM:
use esirs -800 -448 100 768 Tmp4
xform 0 -656 -576
p -800 -960 100 768 1 DESC:Temperature at cold-plate edge
p -800 -928 100 768 1 EGU:K
p -800 -736 100 768 1 FDSC:Temperature at cold-plate edge
p -800 -768 100 768 1 FTVL:DOUBLE
p -800 -896 100 768 1 PREC:1
p -800 -864 100 768 1 PV:$(sadtop)$(mech)
p -800 -832 100 768 1 SCAN:1 second
p -800 -800 100 768 1 SNAM:
p -864 -480 75 1280 -1 palrm(INP):MS
use esirs 1024 1120 100 768 Health
xform 0 1168 992
p 1024 704 100 768 1 DESC:Health: Temperature-sensor
p 911 416 100 0 0 EGU:
p 1024 832 100 768 1 FDSC:Health: Temperature-sensor
p 1024 800 100 768 1 FTVL:STRING
p 1024 736 100 768 1 PV:$(sadtop)$(mech)
p 1024 768 100 768 1 SNAM:SIRengSensHealth
use esirs -800 1184 100 768 Tmp1
xform 0 -656 1056
p -800 672 100 768 1 DESC:Temperature at top of strap
p -800 704 100 768 1 EGU:K
p -800 896 100 768 1 FDSC:Temperature at top of strap
p -800 864 100 768 1 FTVL:DOUBLE
p -800 736 100 768 1 PREC:1
p -800 768 100 768 1 PV:$(sadtop)$(mech)
p -800 800 100 768 1 SCAN:1 second
p -800 832 100 768 1 SNAM:
use esirs -800 640 100 768 Tmp2
xform 0 -656 512
p -800 128 100 768 1 DESC:Temperature at bottom of strap
p -800 160 100 768 1 EGU: K
p -800 352 100 768 1 FDSC:Temperature at bottom of strap
p -800 320 100 768 1 FTVL:DOUBLE
p -800 192 100 768 1 PREC:1
p -800 224 100 768 1 PV:$(sadtop)$(mech)
p -800 256 100 768 1 SCAN:1 second
p -800 288 100 768 1 SNAM:
p -864 608 75 1280 -1 palrm(INP):MS
use outhier 1440 1088 100 1536 HLNK
xform 0 1408 1088
use bc200tr -1504 -1176 -100 0 frame
xform 0 176 128
p 1072 -1008 100 0 1 author:H.T. Yamada
p 1296 -1024 100 0 -1 border:C
p 1072 -1040 100 0 1 checked:H.T. Yamada
p 1328 -1024 100 0 -1 date:26 Nov 1997
p 1312 -896 100 0 -1 project:NIRI/GNIRS CC/WFS Interlocks
p 1312 -960 100 0 -1 title:Interlock Status/Alarm Database
[comments]
