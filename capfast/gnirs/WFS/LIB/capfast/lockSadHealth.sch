[schematic2]
uniq 489
[tools]
[detail]
w -452 -501 100 0 n#488 esirs.TmpBusyHealth.OMSS -576 -800 -448 -800 -448 -192 -64 -192 egenSub.CombHealth1.INPF
w -484 -453 100 0 n#487 esirs.TmpBusyHealth.VAL -576 -768 -480 -768 -480 -128 -64 -128 egenSub.CombHealth1.INPE
w -1086 -733 100 0 n#484 hwin.hwin#486.in -1120 -736 -992 -736 esirs.TmpBusyHealth.INP
w -318 -61 100 0 n#483 egenSub.CombHealth1.INPD -64 -64 -512 -64 -512 -320 -576 -320 esirs.TmpHBHealth.OMSS
w -334 3 100 0 n#482 esirs.TmpHBHealth.VAL -576 -288 -544 -288 -544 0 -64 0 egenSub.CombHealth1.INPC
w -334 67 100 0 n#481 egenSub.CombHealth1.INPB -64 64 -544 64 -544 128 -576 128 esirs.TmpHealth.OMSS
w -318 131 100 0 n#480 egenSub.CombHealth1.INPA -64 128 -512 128 -512 160 -576 160 esirs.TmpHealth.VAL
w 476 283 100 0 n#479 egenSub.CombHealth.INPJ 672 480 480 480 480 96 224 96 egenSub.CombHealth1.VALB
w 444 347 100 0 n#478 egenSub.CombHealth1.VALA 224 160 448 160 448 544 672 544 egenSub.CombHealth.INPI
w -94 259 100 0 n#476 esirs.InitHealth.OMSS -672 608 -544 608 -544 256 416 256 416 608 672 608 egenSub.CombHealth.INPH
w -94 291 100 0 n#475 esirs.InitHealth.VAL -672 640 -512 640 -512 288 384 288 384 672 672 672 egenSub.CombHealth.INPG
w 482 739 100 0 n#474 esirs.CfgHealth.OMSS 224 608 352 608 352 736 672 736 egenSub.CombHealth.INPF
w 466 803 100 0 n#473 esirs.CfgHealth.VAL 224 640 320 640 320 800 672 800 egenSub.CombHealth.INPE
w -62 771 100 0 n#472 egenSub.CombHealth.INPD 672 864 256 864 256 768 -320 768 -320 1120 -672 1120 esirs.GenHealth.OMSS
w -62 803 100 0 n#471 esirs.GenHealth.VAL -672 1152 -288 1152 -288 800 224 800 224 928 672 928 egenSub.CombHealth.INPC
w 434 995 100 0 n#470 egenSub.CombHealth.INPB 672 992 256 992 256 1120 224 1120 esirs.ObsHealth.OMSS
w 450 1059 100 0 n#469 egenSub.CombHealth.INPA 672 1056 288 1056 288 1152 224 1152 esirs.ObsHealth.VAL
w -1086 203 100 0 n#406 hwin.hwin#381.in -1120 192 -992 192 esirs.TmpHealth.INP
w 1648 1027 -100 0 HMSS bihier.HMSS.p 1680 1024 1664 1024 esirs.Health.OMSS
w 1648 1059 -100 0 HVAL esirs.Health.VAL 1664 1056 1680 1056 bihier.HVAL.p
w 1656 1091 -100 0 HLNK esirs.Health.FLNK 1664 1088 1696 1088 outhier.HLNK.p
w -1086 -253 100 0 n#396 hwin.hwin#398.in -1120 -256 -992 -256 esirs.TmpHBHealth.INP
w 1042 995 100 0 n#165 egenSub.CombHealth.OUTB 960 992 1184 992 1184 1056 1248 1056 esirs.Health.IMSS
w 1074 1091 100 0 n#164 egenSub.CombHealth.VALA 960 1088 1248 1088 esirs.Health.INP
w -1118 683 100 0 n#212 hwin.hwin#211.in -1184 672 -1088 672 esirs.InitHealth.INP
w -278 675 100 0 n#207 esirs.CfgHealth.INP -192 672 -304 672 hwin.hwin#208.in
w -270 1187 100 0 n#203 esirs.ObsHealth.INP -192 1184 -288 1184 hwin.hwin#204.in
w -1166 1187 100 0 n#202 esirs.GenHealth.INP -1088 1184 -1184 1184 hwin.hwin#201.in
s 736 128 100 768 each axis and generates a combined health.
s 736 160 100 768 The "gensub" record takes the health of
s 1040 -1024 500 512 lockSadHealth.sch
s 288 -768 100 768 This schematic contains the health component of the Status/Alarm
s 288 -800 100 768 Database for the NIRI/GNIRS/WFS interlock system.
s -544 -880 100 0 .
s 288 -832 100 768 Health indications are taken from various components
s 288 -864 100 768 and are combined into an overall interlock health.
[cell use]
use hwin -1376 1143 100 0 hwin#201
xform 0 -1280 1184
p -1373 1176 100 0 -1 val(in):$(eng)$(mech)GenRd
use hwin -480 1143 100 0 hwin#204
xform 0 -384 1184
p -477 1176 100 0 -1 val(in):$(eng)$(mech)ObsRd
use hwin -496 631 100 0 hwin#208
xform 0 -400 672
p -493 664 100 0 -1 val(in):$(eng)$(mech)CfgRd
use hwin -1376 631 100 0 hwin#211
xform 0 -1280 672
p -1373 664 100 0 -1 val(in):$(eng)$(mech)InitRd
use hwin -1312 151 100 0 hwin#381
xform 0 -1216 192
p -1309 184 100 0 -1 val(in):$(eng)$(mech)TmpRd
use hwin -1312 -297 100 0 hwin#398
xform 0 -1216 -256
p -1309 -264 100 0 -1 val(in):$(eng)$(mech)TmpHB
use hwin -1312 -777 100 0 hwin#486
xform 0 -1216 -736
p -1309 -744 100 0 -1 val(in):$(eng)$(mech)TmpBusy
use esirs -1024 704 100 768 InitHealth
xform 0 -880 576
p -1024 288 100 768 1 DESC:Health:  Initialization lock
p -1137 0 100 0 0 EGU:
p -1024 416 100 768 1 FDSC:Health:  Initialization lock
p -1024 384 100 768 1 FTVL:STRING
p -1024 320 100 768 1 PV:$(sadtop)$(mech)
p -1024 352 100 768 1 SNAM:engLockInitHealth
p -1088 672 75 1280 -1 palrm(INP):MS
use esirs -1024 1216 100 768 GenHealth
xform 0 -880 1088
p -1024 800 100 768 1 DESC:Health:  Generic lock
p -1137 512 100 0 0 EGU:
p -1024 928 100 768 1 FDSC:Health:  Generic lock
p -1024 896 100 768 1 FTVL:STRING
p -1024 832 100 768 1 PV:$(sadtop)$(mech)
p -1024 864 100 768 1 SNAM:engLockGenHealth
p -1088 1184 75 1280 -1 palrm(INP):MS
use esirs -928 224 100 768 TmpHealth
xform 0 -784 96
p -928 -192 100 768 1 DESC:Health:  Temperature interlock
p -1041 -480 100 0 0 EGU:
p -928 -64 100 768 1 FDSC:Health:  Temperature interlock
p -928 -96 100 768 1 FTVL:STRING
p -928 -160 100 768 1 PV:$(sadtop)$(mech)
p -928 -128 100 768 1 SNAM:engLockTmpHealth
use esirs -128 1216 100 768 ObsHealth
xform 0 16 1088
p -128 800 100 768 1 DESC:Health:  Observation lock
p -241 512 100 0 0 EGU:
p -128 928 100 768 1 FDSC:Health:  Observation lock
p -128 896 100 768 1 FTVL:STRING
p -128 832 100 768 1 PV:$(sadtop)$(mech)
p -128 864 100 768 1 SNAM:engLockObsHealth
p -192 1184 75 1280 -1 palrm(INP):MS
use esirs -128 704 100 768 CfgHealth
xform 0 16 576
p -128 288 100 768 1 DESC:Health:  Configuration lock
p -241 0 100 0 0 EGU:
p -128 416 100 768 1 FDSC:Health:  Configuration lock
p -128 384 100 768 1 FTVL:STRING
p -128 320 100 768 1 PV:$(sadtop)$(mech)
p -128 352 100 768 1 SNAM:engLockCfgHealth
p -192 672 75 1280 -1 palrm(INP):MS
use esirs 1312 1120 100 768 Health
xform 0 1456 992
p 1312 832 100 768 1 DESC:$(desc) health
p 1199 416 100 0 0 EGU:
p 1312 800 100 768 1 FDSC:$(desc) health
p 1312 768 100 768 1 FTVL:STRING
p 1312 704 100 768 1 PV:$(sadtop)$(mech)
p 1312 672 100 768 1 SCAN:.5 second
p 1312 736 100 768 1 SNAM:
p 1248 1088 75 1280 -1 palrm(INP):MS
p 1200 1088 75 1024 -1 pproc(INP):PP
use esirs -928 -224 100 768 TmpHBHealth
xform 0 -784 -352
p -928 -672 100 768 1 DESC:Health: Temperature communications
p -928 -640 100 768 1 EGU:
p -928 -512 100 768 1 FDSC:Health: Temperature communications
p -928 -544 100 768 1 FTVL:STRING
p -928 -608 100 768 1 PV:$(sadtop)$(mech)
p -928 -576 100 768 1 SNAM:engLockTmpHBHealth
use esirs -928 -704 100 768 TmpBusyHealth
xform 0 -784 -832
p -928 -1120 100 768 1 DESC:Health: Temperature info updating
p -1041 -1408 100 0 0 EGU:
p -928 -992 100 768 1 FDSC:Health: Temperature info updating
p -928 -1024 100 768 1 FTVL:STRING
p -928 -1088 100 768 1 PV:$(sadtop)$(mech)
p -928 -1056 100 768 1 SNAM:engLockTmpBusyHealth
use egenSub 0 192 100 768 CombHealth1
xform 0 80 -208
p -240 144 100 0 1 FTA:STRING
p -240 80 100 0 1 FTB:STRING
p -240 16 100 0 1 FTC:STRING
p -240 -48 100 0 1 FTD:STRING
p -240 -128 100 768 1 FTE:STRING
p -240 -192 100 768 1 FTF:STRING
p -240 -256 100 768 1 FTG:STRING
p -240 -320 100 768 1 FTH:STRING
p -240 -384 100 768 1 FTI:STRING
p -240 -448 100 768 1 FTJ:STRING
p 304 176 100 0 1 FTVA:STRING
p 304 112 100 0 1 FTVB:STRING
p 304 32 100 0 1 FTVC:LONG
p 0 -640 100 768 1 INAM:
p 0 -704 100 768 1 PV:$(sadtop)$(mech)
p 0 -672 100 768 1 SNAM:cicsHealthCombine
p -80 138 75 0 -1 palrm(INPA):MS
p -80 74 75 0 -1 palrm(INPB):MS
p -80 10 75 0 -1 palrm(INPC):MS
p -80 -54 75 0 -1 palrm(INPD):MS
p -80 -118 75 0 -1 palrm(INPE):MS
p -80 -182 75 0 -1 palrm(INPF):MS
p -80 -246 75 0 -1 palrm(INPG):MS
p -80 -310 75 0 -1 palrm(INPH):MS
p -80 -374 75 0 -1 palrm(INPI):MS
p -80 -438 75 0 -1 palrm(INPJ):MS
p -112 138 75 0 -1 pproc(INPA):PP
p -112 10 75 0 -1 pproc(INPC):PP
p -112 -118 75 0 -1 pproc(INPE):PP
p -112 -246 75 0 -1 pproc(INPG):PP
p -112 -374 75 0 -1 pproc(INPI):PP
use egenSub 736 1120 100 768 CombHealth
xform 0 816 720
p 496 1072 100 0 1 FTA:STRING
p 496 1008 100 0 1 FTB:STRING
p 496 944 100 0 1 FTC:STRING
p 496 880 100 0 1 FTD:STRING
p 496 800 100 768 1 FTE:STRING
p 496 736 100 768 1 FTF:STRING
p 496 672 100 768 1 FTG:STRING
p 496 608 100 768 1 FTH:STRING
p 496 544 100 768 1 FTI:STRING
p 496 480 100 768 1 FTJ:STRING
p 1040 1104 100 0 1 FTVA:STRING
p 1040 1040 100 0 1 FTVB:STRING
p 1040 960 100 0 1 FTVC:LONG
p 736 288 100 768 1 INAM:
p 736 224 100 768 1 PV:$(sadtop)$(mech)
p 736 256 100 768 1 SNAM:cicsHealthCombine
p 656 1066 75 0 -1 palrm(INPA):MS
p 656 1002 75 0 -1 palrm(INPB):MS
p 656 938 75 0 -1 palrm(INPC):MS
p 656 874 75 0 -1 palrm(INPD):MS
p 656 810 75 0 -1 palrm(INPE):MS
p 656 746 75 0 -1 palrm(INPF):MS
p 656 682 75 0 -1 palrm(INPG):MS
p 656 618 75 0 -1 palrm(INPH):MS
p 656 554 75 0 -1 palrm(INPI):MS
p 656 490 75 0 -1 palrm(INPJ):MS
p 624 1066 75 0 -1 pproc(INPA):PP
p 624 938 75 0 -1 pproc(INPC):PP
p 624 810 75 0 -1 pproc(INPE):PP
p 624 682 75 0 -1 pproc(INPG):PP
p 624 554 75 0 -1 pproc(INPI):PP
use bihier 1712 1056 100 1536 HVAL
xform 0 1680 1056
use bihier 1712 1024 100 1536 HMSS
xform 0 1680 1024
use outhier 1712 1088 100 1536 HLNK
xform 0 1680 1088
use bc200tr -1504 -1176 -100 0 frame
xform 0 176 128
p 1072 -1008 100 0 1 author:H.T. Yamada
p 1296 -1024 100 0 -1 border:C
p 1072 -1040 100 0 1 checked:H.T. Yamada
p 1328 -1024 100 0 -1 date:26 Nov 1997
p 1312 -896 100 0 -1 project:NIRI/GNIRS CC/WFS Interlocks
p 1312 -960 100 0 -1 title:Interlock Status/Alarm Database
[comments]
