[schematic2]
uniq 414
[tools]
[detail]
w -1054 -309 -100 0 c#413 bihier.HOMSS.p -992 -320 -1056 -320 lockSadHealth.lockSadHealth#401.HMSS
w -1054 -277 -100 0 HVAL bihier.HVAL.p -992 -288 -1056 -288 lockSadHealth.lockSadHealth#401.HVAL
w -1046 -245 -100 0 HFLNK outhier.HFLNK.p -976 -256 -1056 -256 lockSadHealth.lockSadHealth#401.HLNK
w 770 683 100 0 n#409 hwin.hwin#408.in 736 672 864 672 ecalcs.LockCalc.INPF
w -14 739 100 0 n#405 hwin.hwin#407.in -32 736 64 736 esirs.TmpUpdate.INP
w -206 259 100 0 n#396 hwin.hwin#398.in -224 256 -128 256 esirs.TmpHB.INP
w 1162 67 100 0 n#393 esirs.Active.INP 1248 64 1136 64 hwin.hwin#395.in
w 1162 -445 100 0 n#390 esirs.Moving.INP 1248 -448 1136 -448 hwin.hwin#392.in
w 490 -509 100 0 n#387 esirs.MaxMove.INP 576 -512 464 -512 hwin.hwin#389.in
w 714 803 100 0 n#268 ecalcs.LockCalc.INPB 864 800 736 800 hwin.hwin#301.in
w 770 835 100 0 n#269 ecalcs.LockCalc.INPA 864 832 736 832 hwin.hwin#300.in
w 770 771 100 0 n#270 ecalcs.LockCalc.INPC 864 768 736 768 hwin.hwin#299.in
w 770 707 100 0 n#273 ecalcs.LockCalc.INPE 864 704 736 704 hwin.hwin#303.in
w 770 739 100 0 n#272 ecalcs.LockCalc.INPD 864 736 736 736 hwin.hwin#302.in
w 1170 643 100 0 n#304 ecalcs.LockCalc.VAL 1152 640 1248 640 esirs.Lock.INP
w -878 739 100 0 n#253 hwin.hwin#252.in -896 736 -800 736 esirs.TmpVal.INP
w -1150 291 100 0 n#250 hwin.hwin#206.in -1184 288 -1056 288 ecalcs.TmpCalc.INPA
w -750 99 100 0 n#249 ecalcs.TmpCalc.VAL -768 96 -672 96 esirs.Tmp.INP
w 354 1259 100 0 n#212 hwin.hwin#211.in 288 1248 384 1248 esirs.Init.INP
w 1098 1187 100 0 n#207 esirs.Cfg.INP 1184 1184 1072 1184 hwin.hwin#208.in
w -1166 1251 100 0 n#203 esirs.Obs.INP -1088 1248 -1184 1248 hwin.hwin#204.in
w -430 1251 100 0 n#202 esirs.Gen.INP -352 1248 -448 1248 hwin.hwin#201.in
s -1376 -864 100 0 These "stringout" records update the "genSub" record combHlt,
s -1376 -896 100 0 which determines the overall health of the instrument.
s -1376 -928 100 0 They are necessary to convert the value fields of the SIR record
s -1376 -960 100 0 into link fields, which can use a channel access put.
s -1376 -992 100 0 Variables "mindex" and "hindex" direct the output to the
s -1376 -1024 100 0 approriate "genSub" record fields for this component.
s 304 224 100 0 .
s 1232 880 100 0 .
s -1376 -800 100 768 which is why there are very few links.
s -1376 -768 100 768 from the state notation code or other databases,
s -1376 -736 100 768 The records are updated by channel access writes
s -1376 -704 100 768 Database for the NIRI/GNIRS/WFS interlock system.
s -1376 -672 100 768 This schematic contains the Status/Alarm
s 1040 -1024 500 512 lockSad.sch
s 496 864 100 0 .
[cell use]
use bihier -960 -288 100 1536 HVAL
xform 0 -992 -288
use bihier -960 -320 100 1536 HOMSS
xform 0 -992 -320
use outhier -960 -256 100 1536 HFLNK
xform 0 -992 -256
use hwin -416 215 100 0 hwin#398
xform 0 -320 256
p -413 248 100 0 -1 val(in):$(eng)$(mech)TmpHB
use hwin -1088 695 100 0 hwin#252
xform 0 -992 736
p -1085 728 100 0 -1 val(in):$(eng)$(mech)Tmp
use hwin 96 1207 100 0 hwin#211
xform 0 192 1248
p 99 1240 100 0 -1 val(in):$(eng)$(mech)InitRd
use hwin 880 1143 100 0 hwin#208
xform 0 976 1184
p 883 1176 100 0 -1 val(in):$(eng)$(mech)CfgRd
use hwin -1376 247 100 0 hwin#206
xform 0 -1280 288
p -1373 280 100 0 -1 val(in):$(eng)$(mech)TmpRd
use hwin -1376 1207 100 0 hwin#204
xform 0 -1280 1248
p -1373 1240 100 0 -1 val(in):$(eng)$(mech)ObsRd
use hwin -640 1207 100 0 hwin#201
xform 0 -544 1248
p -637 1240 100 0 -1 val(in):$(eng)$(mech)GenRd
use hwin 544 727 100 0 hwin#299
xform 0 640 768
p 547 760 100 0 -1 val(in):$(eng)$(mech)CfgRd
use hwin 544 695 100 0 hwin#302
xform 0 640 736
p 547 728 100 0 -1 val(in):$(eng)$(mech)InitRd
use hwin 544 663 100 0 hwin#303
xform 0 640 704
p 547 696 100 0 -1 val(in):$(eng)$(mech)TmpRd
use hwin 544 759 100 0 hwin#301
xform 0 640 800
p 547 792 100 0 -1 val(in):$(eng)$(mech)GenRd
use hwin 544 791 100 0 hwin#300
xform 0 640 832
p 547 824 100 0 -1 val(in):$(eng)$(mech)ObsRd
use hwin 272 -553 100 0 hwin#389
xform 0 368 -512
p 275 -520 100 0 -1 val(in):$(eng)$(mech)MaxMove
use hwin 944 -489 100 0 hwin#392
xform 0 1040 -448
p 947 -456 100 0 -1 val(in):$(eng)$(mech)Moving
use hwin 944 23 100 0 hwin#395
xform 0 1040 64
p 947 56 100 0 -1 val(in):$(eng)$(mech)Active
use hwin -224 695 100 0 hwin#407
xform 0 -128 736
p -221 728 100 0 -1 val(in):$(eng)$(mech)TmpUpdateRd
use hwin 544 631 100 0 hwin#408
xform 0 640 672
p 547 664 100 0 -1 val(in):$(eng)$(mech)TmpBusy
use esirs -96 -512 100 768 Label
xform 0 48 -640
p -96 -800 100 768 1 DESC:The FDSC field describes the $(desc)
p -64 -976 100 0 0 EGU:
p -96 -832 100 768 1 FDSC:$(name) $(desc)
p -96 -864 100 768 1 FTVL:STRING
p -96 -928 100 768 1 PV:$(sadtop)$(mech)
p -96 -896 100 768 1 SNAM:
use esirs -64 288 100 768 TmpHB
xform 0 80 160
p -64 -192 100 768 1 DESC:Temperature state comm. [GOOD=0|BAD=1]
p -64 -160 100 768 1 EGU:0/1
p -64 0 100 768 1 FDSC:Temperature state comm. [GOOD=0|BAD=1]
p -64 -32 100 768 1 FTVL:LONG
p -64 -128 100 768 1 PV:$(sadtop)$(mech)
p -64 -96 100 768 1 SCAN:.5 second
p -64 -64 100 768 1 SNAM:
p -176 256 75 1024 -1 pproc(INP):PP
use esirs 544 0 100 768 State
xform 0 688 -128
p 544 -288 100 768 1 DESC:$(desc) state
p 576 -464 100 0 0 EGU:
p 544 -320 100 768 1 FDSC:$(desc) state
p 544 -352 100 768 1 FTVL:STRING
p 544 -416 100 768 1 PV:$(sadtop)$(mech)
p 544 -384 100 768 1 SNAM:
use esirs -736 768 100 768 TmpVal
xform 0 -592 640
p -736 320 100 768 1 DESC:Temperature state
p -849 64 100 0 0 EGU:
p -736 480 100 768 1 FDSC:Temperature state
p -736 448 100 768 1 FTVL:STRING
p -736 352 100 768 1 PV:$(sadtop)$(mech)
p -736 384 100 768 1 SCAN:.5 second
p -736 416 100 768 1 SNAM:
use esirs 1248 1216 100 768 Cfg
xform 0 1392 1088
p 1248 768 100 768 1 DESC:Locked (config)? [UNLOCKED=0|LOCKED=1]
p 1135 512 100 0 0 EGU:0/1
p 1248 928 100 768 1 FDSC:Locked (config)? [UNLOCKED=0|LOCKED=1]
p 1248 896 100 768 1 FTVL:LONG
p 1248 800 100 768 1 PV:$(sadtop)$(mech)
p 1248 832 100 768 1 SCAN:.5 second
p 1248 864 100 768 1 SNAM:
p 1184 1184 75 1280 -1 palrm(INP):MS
use esirs -1024 1280 100 768 Obs
xform 0 -880 1152
p -1024 832 100 768 1 DESC:Locked (obs)? [UNLOCKED=0|LOCKED=1]
p -1137 576 100 0 0 EGU:0/1
p -1024 992 100 768 1 FDSC:Locked (obs)? [UNLOCKED=0|LOCKED=1]
p -1024 960 100 768 1 FTVL:LONG
p -1024 864 100 768 1 PV:$(sadtop)$(mech)
p -1024 896 100 768 1 SCAN:.5 second
p -1024 928 100 768 1 SNAM:
p -1088 1248 75 1280 -1 palrm(INP):MS
use esirs -608 128 100 768 Tmp
xform 0 -464 0
p -608 -352 100 768 1 DESC:Locked (temp state)? [UNLOCKED=0|LOCKED=1]
p -608 -320 100 768 1 EGU:0/1
p -608 -160 100 768 1 FDSC:Locked (temp state)? [UNLOCKED=0|LOCKED=1]
p -608 -192 100 768 1 FTVL:LONG
p -608 -288 100 768 1 PV:$(sadtop)$(mech)
p -608 -256 100 768 1 SCAN:.5 second
p -608 -224 100 768 1 SNAM:
p -720 96 75 1024 -1 pproc(INP):PP
use esirs -288 1280 100 768 Gen
xform 0 -144 1152
p -288 800 100 768 1 DESC:Locked (generic)? [UNLOCKED=0|LOCKED=1]
p -288 832 100 768 1 EGU:0/1
p -288 992 100 768 1 FDSC:Locked (generic)? [UNLOCKED=0|LOCKED=1]
p -288 960 100 768 1 FTVL:LONG
p -288 864 100 768 1 PV:$(sadtop)$(mech)
p -288 896 100 768 1 SCAN:.5 second
p -288 928 100 768 1 SNAM:
p -352 1248 75 1280 -1 palrm(INP):MS
use esirs 448 1280 100 768 Init
xform 0 592 1152
p 448 800 100 768 1 DESC:Locked for initialization? [UNLOCKED=0|LOCKED=1]
p 448 832 100 768 1 EGU:0/1
p 448 992 100 768 1 FDSC:Locked for initialization? [UNLOCKED=0|LOCKED=1]
p 448 960 100 768 1 FTVL:LONG
p 448 864 100 768 1 PV:$(sadtop)$(mech)
p 448 896 100 768 1 SCAN:.5 second
p 448 928 100 768 1 SNAM:
p 384 1248 75 1280 -1 palrm(INP):MS
use esirs 1312 672 100 768 Lock
xform 0 1456 544
p 1312 192 100 768 1 DESC:Combined state of all interlocks
p 1312 224 100 768 1 EGU: 0/1
p 1312 384 100 768 1 FDSC:Combined state of all interlocks
p 1312 352 100 768 1 FTVL:LONG
p 1312 256 100 768 1 PV:$(sadtop)$(mech)
p 1312 288 100 768 1 SCAN:.5 second
p 1312 320 100 768 1 SNAM:
p 1248 640 75 1280 -1 palrm(INP):MS
p 1200 640 75 1024 -1 pproc(INP):PP
use esirs 640 -480 100 768 MaxMove
xform 0 784 -608
p 640 -960 100 768 1 DESC:Maximum number of moving motors
p 640 -928 100 768 1 EGU:count
p 640 -768 100 768 1 FDSC:Maximum number of moving motors
p 640 -800 100 768 1 FTVL:LONG
p 640 -896 100 768 1 PV:$(sadtop)$(mech)
p 640 -864 100 768 1 SCAN:1 second
p 640 -832 100 768 1 SNAM:
p 576 -512 75 1280 -1 palrm(INP):MS
use esirs 1312 -416 100 768 Moving
xform 0 1456 -544
p 1312 -896 100 768 1 DESC:Number of motors currently moving
p 1312 -864 100 768 1 EGU:count
p 1312 -704 100 768 1 FDSC:Number of motors currently moving
p 1312 -736 100 768 1 FTVL:LONG
p 1312 -832 100 768 1 PV:$(sadtop)$(mech)
p 1312 -800 100 768 1 SCAN:.1 second
p 1312 -768 100 768 1 SNAM:
p 1248 -448 75 1280 -1 palrm(INP):MS
use esirs 1312 96 100 768 Active
xform 0 1456 -32
p 1312 -384 100 768 1 DESC:Number of uncompleted motor actions
p 1312 -352 100 768 1 EGU: count
p 1312 -192 100 768 1 FDSC:Number of uncompleted motor actions
p 1312 -224 100 768 1 FTVL:LONG
p 1312 -320 100 768 1 PV:$(sadtop)$(mech)
p 1312 -288 100 768 1 SCAN:.1 second
p 1312 -256 100 768 1 SNAM:
p 1248 64 75 1280 -1 palrm(INP):MS
use esirs 128 768 100 768 TmpUpdate
xform 0 272 640
p 128 288 100 768 1 DESC:Temperature updating state? [UNLOCKED=0|LOCKED=1]
p 128 320 100 768 1 EGU:0/1
p 128 480 100 768 1 FDSC:Temperature updating state? [UNLOCKED=0|LOCKED=1]
p 128 448 100 768 1 FTVL:LONG
p 128 352 100 768 1 PV:$(sadtop)$(mech)
p 128 384 100 768 1 SCAN:.5 second
p 128 416 100 768 1 SNAM:
p 16 736 75 1024 -1 pproc(INP):PP
use lockSadHealth -1376 -537 100 0 lockSadHealth#401
xform 0 -1216 -368
use ecalcs -992 320 100 768 TmpCalc
xform 0 -912 80
p -992 -192 100 768 1 CALC:!A
p -992 -224 100 768 1 PV:$(sadtop)$(mech)
use ecalcs 928 864 100 768 LockCalc
xform 0 1008 624
p 928 352 100 768 1 CALC:A||B||C||D||!E||F
p 928 320 100 768 1 PV:$(sadtop)$(mech)
use bc200tr -1504 -1176 -100 0 frame
xform 0 176 128
p 1072 -1008 100 0 1 author:H.T. Yamada
p 1296 -1024 100 0 -1 border:C
p 1072 -1040 100 0 1 checked:H.T. Yamada
p 1328 -1024 100 0 -1 date:26 Nov 1997
p 1312 -896 100 0 -1 project:NIRI/GNIRS CC/WFS Interlocks
p 1312 -960 100 0 -1 title:Interlock Status/Alarm Database
[comments]
