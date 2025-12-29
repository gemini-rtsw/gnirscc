[schematic2]
uniq 517
[tools]
[detail]
w 508 827 100 0 n#516 esirs.PosErrHealth.VAL 160 1184 512 1184 512 480 704 480 egenSub.combHlt.INPA
w 476 779 100 0 n#515 esirs.PosErrHealth.OMSS 160 1152 480 1152 480 416 704 416 egenSub.combHlt.INPB
w 444 523 100 0 n#514 esirs.DatumHealth.VAL 160 704 448 704 448 352 704 352 egenSub.combHlt.INPC
w 412 475 100 0 n#513 esirs.DatumHealth.OMSS 160 672 416 672 416 288 704 288 egenSub.combHlt.INPD
w 508 -405 100 0 n#512 esirs.ModuleHealth.OMSS 160 -704 512 -704 512 -96 704 -96 egenSub.combHlt.INPJ
w 476 -357 100 0 n#511 esirs.ModuleHealth.VAL 160 -672 480 -672 480 -32 704 -32 egenSub.combHlt.INPI
w 274 -253 100 0 n#510 esirs.SnlHealth.OMSS 160 -256 448 -256 448 32 704 32 egenSub.combHlt.INPH
w 412 -69 100 0 n#509 esirs.SnlHealth.VAL 160 -224 416 -224 416 96 704 96 egenSub.combHlt.INPG
w 530 163 100 0 n#508 esirs.HallHealth.OMSS 160 192 416 192 416 160 704 160 egenSub.combHlt.INPF
w 402 227 100 0 n#503 esirs.HallHealth.VAL 160 224 704 224 egenSub.combHlt.INPE
w 1058 419 100 0 n#477 egenSub.combHlt.OUTB 992 416 1184 416 1184 480 1248 480 esirs.Health.IMSS
w 1090 515 100 0 n#476 egenSub.combHlt.VALA 992 512 1248 512 esirs.Health.INP
w -398 739 100 0 n#403 hwin.hwin#404.in -480 736 -256 736 esirs.DatumHealth.INP
w 1664 515 -100 0 HLNK outhier.FLNK.p 1712 512 1664 512 esirs.Health.FLNK
w 1656 451 -100 0 HMSS bihier.OMSS.p 1696 448 1664 448 esirs.Health.OMSS
w 1656 488 -100 1536 OHLT esirs.Health.VAL 1664 480 1696 480 bihier.VAL.p
w -446 227 100 0 n#502 egenSubA.HallHealthComp.OUTB -576 224 -256 224 esirs.HallHealth.IMSS
w -446 259 100 0 n#159 egenSubA.HallHealthComp.VALA -576 256 -256 256 esirs.HallHealth.INP
w -1054 35 100 0 n#154 hwin.hwin#155.in -1184 32 -864 32 egenSubA.HallHealthComp.INPH
w -1054 99 100 0 n#153 hwin.hwin#152.in -1184 96 -864 96 egenSubA.HallHealthComp.INPF
w -1054 163 100 0 n#150 hwin.hwin#151.in -1184 160 -864 160 egenSubA.HallHealthComp.INPD
w -1054 227 100 0 n#149 hwin.hwin#148.in -1184 224 -864 224 egenSubA.HallHealthComp.INPB
w -366 -637 100 0 n#142 hwin.hwin#141.in -416 -640 -256 -640 esirs.ModuleHealth.INP
w -398 1219 100 0 n#140 hwin.hwin#137.in -480 1216 -256 1216 esirs.PosErrHealth.INP
s -176 -352 100 768 to by SNL code.
s -1024 320 100 768 Health output, message and link
s 1056 -1056 400 1280 comp1mSadHealth
s -176 -320 100 768 SnlHealth is written
[cell use]
use egenSub 768 544 100 768 combHlt
xform 0 848 144
p 528 512 100 0 1 FTA:STRING
p 528 448 100 0 1 FTB:STRING
p 528 384 100 0 1 FTC:STRING
p 528 320 100 0 1 FTD:STRING
p 528 256 100 0 1 FTE:STRING
p 528 192 100 0 1 FTF:STRING
p 528 128 100 0 1 FTG:STRING
p 528 64 100 0 1 FTH:STRING
p 528 0 100 0 1 FTI:STRING
p 528 -64 100 0 1 FTJ:STRING
p 1040 528 100 0 1 FTVA:STRING
p 1040 464 100 0 1 FTVB:STRING
p 1040 400 100 0 1 FTVC:LONG
p 768 -320 100 768 1 PV:$(sadtop)$(mech)
p 768 -288 100 768 1 SNAM:cicsHealthCombine
p 656 490 75 0 -1 pproc(INPA):PP
p 656 426 75 0 -1 pproc(INPB):PP
p 656 362 75 0 -1 pproc(INPC):PP
p 656 298 75 0 -1 pproc(INPD):PP
p 656 234 75 0 -1 pproc(INPE):PP
p 656 170 75 0 -1 pproc(INPF):PP
p 656 106 75 0 -1 pproc(INPG):PP
p 656 42 75 0 -1 pproc(INPH):PP
p 656 -22 75 0 -1 pproc(INPI):PP
p 656 -86 75 0 -1 pproc(INPJ):PP
use esirs 1312 544 100 768 Health
xform 0 1456 416
p 1312 256 100 768 1 DESC:Health:  Combined
p 1199 -160 100 0 0 EGU:
p 1312 192 100 768 1 FDSC:Health:  Combined
p 1312 224 100 768 1 FTVL:STRING
p 1312 128 100 768 1 PV:$(sadtop)$(mech)
p 1312 96 100 768 1 SCAN:.5 second
p 1312 160 100 768 1 SNAM:
p 1200 512 75 1024 -1 pproc(INP):PP
use esirs -192 1248 100 768 PosErrHealth
xform 0 -48 1120
p -192 800 100 768 1 DESC:Health: Backlash corrected
p -192 864 100 768 1 EGU:
p -192 896 100 768 1 FDSC:Health: Backlash corrected
p -192 928 100 768 1 FTVL:STRING
p -192 960 100 768 1 PV:$(sadtop)$(mech)
p -192 832 100 768 1 SNAM:engBacklashHealth
use esirs -192 288 100 768 HallHealth
xform 0 -48 160
p -192 -128 100 768 1 DESC:Health:  Hall-effect sensor
p -192 -96 100 768 1 EGU:
p -192 -64 100 768 1 FDSC:Health:  Hall-effect sensor
p -192 -32 100 768 1 FTVL:STRING
p -192 0 100 768 1 PV:$(sadtop)$(mech)
p -304 256 75 1024 -1 pproc(INP):PP
use esirs -192 -608 100 768 ModuleHealth
xform 0 -48 -736
p -192 -1056 100 768 1 DESC:Health:  Steppermotor driver
p -192 -992 100 768 1 EGU:
p -192 -960 100 768 1 FDSC:Health:  Steppermotor driver
p -192 -928 100 768 1 FTVL:STRING
p -192 -896 100 768 1 PV:$(sadtop)$(mech)
p -192 -1024 100 768 1 SNAM:engModuleHealth
use esirs -192 -160 100 768 SnlHealth
xform 0 -48 -288
p -192 -576 100 768 1 DESC:Health:  SNL status/messages
p -192 -544 100 768 1 EGU:
p -192 -512 100 768 1 FDSC:Health:  SNL status/messages
p -192 -480 100 768 1 FTVL:STRING
p -192 -448 100 768 1 PV:$(sadtop)$(mech)
use esirs -192 768 100 768 DatumHealth
xform 0 -48 640
p -192 320 100 768 1 DESC:Health: Is datumed
p -192 384 100 768 1 EGU:
p -192 416 100 768 1 FDSC:Health: Is datumed
p -192 448 100 768 1 FTVL:STRING
p -192 480 100 768 1 PV:$(sadtop)$(mech)
p -192 352 100 768 1 SNAM:engDatumHealth
use hwin -1376 -9 100 0 hwin#155
xform 0 -1280 32
p -1360 16 100 768 -1 val(in):$(eng)$(mech)Hallstep.EN2B
use hwin -1376 55 100 0 hwin#152
xform 0 -1280 96
p -1360 80 100 768 -1 val(in):$(eng)$(mech)Hallstep.EN2P
use hwin -1376 119 100 0 hwin#151
xform 0 -1280 160
p -1360 144 100 768 -1 val(in):$(eng)$(mech)Hallstep.EN1B
use hwin -1376 183 100 0 hwin#148
xform 0 -1280 224
p -1360 208 100 768 -1 val(in):$(eng)$(mech)Hallstep.EN1P
use hwin -608 -681 100 0 hwin#141
xform 0 -512 -640
p -592 -656 100 768 -1 val(in):$(eng)$(mech)Stat.VAL
use hwin -672 1175 100 0 hwin#137
xform 0 -576 1216
p -656 1200 100 768 -1 val(in):$(eng)$(mech)PosErr.VAL
use hwin -672 695 100 0 hwin#404
xform 0 -576 736
p -656 720 100 768 -1 val(in):$(eng)$(mech)Hallstep.DATM
use bihier 1728 448 100 1536 OMSS
xform 0 1696 448
use bihier 1728 480 100 1536 VAL
xform 0 1696 480
use outhier 1728 512 100 1536 FLNK
xform 0 1696 512
use egenSubA -800 288 100 768 HallHealthComp
xform 0 -720 -112
p -928 224 100 1280 1 FTB:LONG
p -928 160 100 1280 1 FTD:LONG
p -928 96 100 1280 1 FTF:LONG
p -928 32 100 1280 1 FTH:LONG
p -496 256 100 768 1 FTVA:STRING
p -496 224 100 768 1 FTVB:STRING
p -800 -544 100 768 1 PV:$(sadtop)$(mech)
p -800 -608 100 768 1 SNAM:engHallHealth
use bc200tr -1504 -1176 -100 0 Interface
xform 0 176 128
p 1072 -1008 100 0 1 author:H.T.Yamada
p 1296 -1024 100 0 -1 border:C
p 1072 -1040 100 0 1 checked:H.T.Yamada
p 1328 -1024 100 0 -1 date:1999-08-02
p 1312 -896 100 0 -1 project:Near Infra-Red Imager
p 1312 -960 100 0 -1 title:Interface to low-level engineering health
[comments]
