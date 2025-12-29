[schematic2]
uniq 125
[tools]
[detail]
w 994 -917 100 0 n#120 eais.hkVSet.INP 1024 -928 1024 -928 hwin.hwin#119.in
w 994 -725 100 0 n#117 eais.hkVDet.INP 1024 -736 1024 -736 hwin.hwin#116.in
w 994 -533 100 0 n#112 hwin.hwin#113.in 1024 -544 1024 -544 eais.hkVggCl2.INP
w 994 -341 100 0 n#111 eais.hkVggCl1.INP 1024 -352 1024 -352 hwin.hwin#110.in
w 994 -149 100 0 n#106 hwin.hwin#107.in 1024 -160 1024 -160 eais.hkVddCl2.INP
w 994 43 100 0 n#105 eais.hkVddCl1.INP 1024 32 1024 32 hwin.hwin#104.in
w 994 235 100 0 n#102 hwin.hwin#101.in 1024 224 1024 224 eais.hkVddUc.INP
w 360 43 100 0 n#97 hwout.hwout#98.outp 384 32 384 32 elongouts.inst_NumArrays.OUT
w 360 -149 100 0 n#96 elongouts.inst_EchoMe.OUT 384 -160 384 -160 hwout.hwout#95.outp
w 360 -341 100 0 n#91 hwout.hwout#92.outp 384 -352 384 -352 elongouts.inst_StatusReport.OUT
w 360 -533 100 0 n#88 hwout.hwout#89.outp 384 -544 384 -544 elongouts.inst_Deactivate.OUT
w 360 -725 100 0 n#87 elongouts.inst_Protection.OUT 384 -736 384 -736 hwout.hwout#86.outp
w 360 -917 100 0 n#82 hwout.hwout#83.outp 384 -928 384 -928 elongouts.inst_SCBRegVal.OUT
w 360 -1109 100 0 n#81 elongouts.inst_CPState.OUT 384 -1120 384 -1120 hwout.hwout#80.outp
w 360 -1301 100 0 n#78 hwout.hwout#77.outp 384 -1312 384 -1312 elongouts.inst_DataSimul.OUT
w 360 -1493 100 0 n#75 hwout.hwout#74.outp 384 -1504 384 -1504 elongouts.inst_HKFreeze.OUT
w 360 235 100 0 n#72 hwout.hwout#71.outp 384 224 384 224 elongouts.inst_TraceFlag.OUT
w -296 291 100 0 n#44 hwin.hwin#24.in -288 288 -256 288 elongins.instTraceFlag.INP
w -296 99 100 0 n#43 hwin.hwin#25.in -288 96 -256 96 elongins.instNumArrays.INP
w -296 -93 100 0 n#42 hwin.hwin#26.in -288 -96 -256 -96 elongins.instEchoMe.INP
w -296 -285 100 0 n#41 hwin.hwin#27.in -288 -288 -256 -288 elongins.instStatusReport.INP
w -296 -477 100 0 n#40 hwin.hwin#28.in -288 -480 -256 -480 elongins.instDeactivate.INP
w -296 -669 100 0 n#39 hwin.hwin#29.in -288 -672 -256 -672 elongins.instProtection.INP
w -296 -861 100 0 n#37 hwin.hwin#30.in -288 -864 -256 -864 elongins.instSCBRegVal.INP
w -296 -1053 100 0 n#36 hwin.hwin#31.in -288 -1056 -256 -1056 elongins.instCPState.INP
w -296 -1245 100 0 n#35 hwin.hwin#32.in -288 -1248 -256 -1248 elongins.instDataSimul.INP
w -296 -1437 100 0 n#34 hwin.hwin#33.in -288 -1440 -256 -1440 elongins.instHKFreeze.INP
s -480 416 500 0 Icon variables
s 808 360 250 0 HouseKeeping channels
[cell use]
use ebis 1088 -1112 100 0 hkState
xform 0 1152 -1168
p 1088 -1240 60 1536 1 ONAM:FROZEN
p 1240 -1240 60 1536 1 OSV:MINOR
p 1088 -1224 60 1536 1 ZNAM:RUNNING
p 1240 -1224 60 1536 1 ZSV:NO_ALARM
p 776 -1136 50 1536 1 def(INP):$(top)instHKFreeze.VAL
p 992 -1136 75 1280 -1 pproc(INP):PP
use hwin -480 247 100 0 hwin#24
xform 0 -384 288
p -568 288 60 1536 -1 val(in):@Node=2,Var=0,Grp=-1,Idx=0
use hwin -480 55 100 0 hwin#25
xform 0 -384 96
p -568 96 60 1536 -1 val(in):@Node=2,Var=1,Grp=-1,Idx=0
use hwin -480 -137 100 0 hwin#26
xform 0 -384 -96
p -576 -96 60 1536 -1 val(in):@Node=2,Var=2,Grp=-1,Idx=0
use hwin -480 -329 100 0 hwin#27
xform 0 -384 -288
p -568 -288 60 1536 -1 val(in):@Node=2,Var=3,Grp=-1,Idx=0
use hwin -480 -521 100 0 hwin#28
xform 0 -384 -480
p -576 -480 60 1536 -1 val(in):@Node=2,Var=4,Grp=-1,Idx=0
use hwin -480 -713 100 0 hwin#29
xform 0 -384 -672
p -576 -672 60 1536 -1 val(in):@Node=2,Var=6,Grp=-1,Idx=0
use hwin -480 -905 100 0 hwin#30
xform 0 -384 -864
p -576 -864 60 1536 -1 val(in):@Node=2,Var=7,Grp=-1,Idx=0
use hwin -480 -1097 100 0 hwin#31
xform 0 -384 -1056
p -568 -1056 60 1536 -1 val(in):@Node=2,Var=8,Grp=-1,Idx=0
use hwin -480 -1289 100 0 hwin#32
xform 0 -384 -1248
p -576 -1248 60 1536 -1 val(in):@Node=2,Var=11,Grp=-1,Idx=0
use hwin -480 -1481 100 0 hwin#33
xform 0 -384 -1440
p -576 -1448 60 0 -1 val(in):@Node=2,Var=12,Grp=-1,Idx=0
use hwin 832 183 100 0 hwin#101
xform 0 928 224
p 744 224 60 1536 -1 val(in):@Node=2,Var=13,Grp=-1,Idx=90
use hwin 832 -9 100 0 hwin#104
xform 0 928 32
p 744 32 60 1536 -1 val(in):@Node=2,Var=13,Grp=-1,Idx=61
use hwin 832 -201 100 0 hwin#107
xform 0 928 -160
p 744 -160 60 1536 -1 val(in):@Node=2,Var=13,Grp=-1,Idx=78
use hwin 832 -393 100 0 hwin#110
xform 0 928 -352
p 744 -352 60 1536 -1 val(in):@Node=2,Var=13,Grp=-1,Idx=64
use hwin 832 -585 100 0 hwin#113
xform 0 928 -544
p 744 -544 60 1536 -1 val(in):@Node=2,Var=13,Grp=-1,Idx=75
use hwin 832 -777 100 0 hwin#116
xform 0 928 -736
p 744 -736 60 1536 -1 val(in):@Node=2,Var=13,Grp=-1,Idx=81
use hwin 832 -969 100 0 hwin#119
xform 0 928 -928
p 744 -928 60 1536 -1 val(in):@Node=2,Var=13,Grp=-1,Idx=76
use eais 1088 248 100 0 hkVddUc
xform 0 1152 192
p 910 312 100 0 0 DTYP:wFireVarMsg
use eais 1088 56 100 0 hkVddCl1
xform 0 1152 0
p 910 120 100 0 0 DTYP:wFireVarMsg
use eais 1088 -136 100 0 hkVddCl2
xform 0 1152 -192
p 910 -72 100 0 0 DTYP:wFireVarMsg
use eais 1088 -328 100 0 hkVggCl1
xform 0 1152 -384
p 910 -264 100 0 0 DTYP:wFireVarMsg
use eais 1088 -520 100 0 hkVggCl2
xform 0 1152 -576
p 910 -456 100 0 0 DTYP:wFireVarMsg
use eais 1088 -712 100 0 hkVDet
xform 0 1152 -768
p 910 -648 100 0 0 DTYP:wFireVarMsg
use eais 1088 -904 100 0 hkVSet
xform 0 1152 -960
p 910 -840 100 0 0 DTYP:wFireVarMsg
use elongouts 192 136 100 0 inst_NumArrays
xform 0 256 64
p 84 456 100 0 0 DTYP:wFireVarMsg
use elongouts 192 -56 100 0 inst_EchoMe
xform 0 256 -128
p 84 264 100 0 0 DTYP:wFireVarMsg
use elongouts 192 -248 100 0 inst_StatusReport
xform 0 256 -320
p 84 72 100 0 0 DTYP:wFireVarMsg
use elongouts 192 -440 100 0 inst_Deactivate
xform 0 256 -512
p 84 -120 100 0 0 DTYP:wFireVarMsg
use elongouts 192 -632 100 0 inst_Protection
xform 0 256 -704
p 84 -312 100 0 0 DTYP:wFireVarMsg
use elongouts 192 -824 100 0 inst_SCBRegVal
xform 0 256 -896
p 84 -504 100 0 0 DTYP:wFireVarMsg
use elongouts 192 -1016 100 0 inst_CPState
xform 0 256 -1088
p 84 -696 100 0 0 DTYP:wFireVarMsg
use elongouts 192 -1208 100 0 inst_DataSimul
xform 0 256 -1280
p 84 -888 100 0 0 DTYP:wFireVarMsg
use elongouts 192 -1400 100 0 inst_HKFreeze
xform 0 256 -1472
p 84 -1080 100 0 0 DTYP:wFireVarMsg
use elongouts 192 328 80 0 inst_TraceFlag
xform 0 256 256
p 84 648 100 0 0 DTYP:wFireVarMsg
use hwout 384 -9 100 0 hwout#98
xform 0 480 32
p 480 32 60 1536 -1 val(outp):@Node=2,Var=1,Grp=-1,Idx=0
use hwout 384 -201 100 0 hwout#95
xform 0 480 -160
p 480 -160 60 1536 -1 val(outp):@Node=2,Var=2,Grp=-1,Idx=0
use hwout 384 -393 100 0 hwout#92
xform 0 480 -352
p 480 -352 60 1536 -1 val(outp):@Node=2,Var=3,Grp=-1,Idx=0
use hwout 384 -585 100 0 hwout#89
xform 0 480 -544
p 480 -544 60 1536 -1 val(outp):@Node=2,Var=4,Grp=-1,Idx=0
use hwout 384 -777 100 0 hwout#86
xform 0 480 -736
p 480 -736 60 1536 -1 val(outp):@Node=2,Var=6,Grp=-1,Idx=0
use hwout 384 -969 100 0 hwout#83
xform 0 480 -928
p 480 -928 60 1536 -1 val(outp):@Node=2,Var=7,Grp=-1,Idx=0
use hwout 384 -1161 100 0 hwout#80
xform 0 480 -1120
p 480 -1120 60 1536 -1 val(outp):@Node=2,Var=8,Grp=-1,Idx=0
use hwout 384 -1353 100 0 hwout#77
xform 0 480 -1312
p 480 -1312 60 1536 -1 val(outp):@Node=2,Var=11,Grp=-1,Idx=0
use hwout 384 -1545 100 0 hwout#74
xform 0 480 -1504
p 480 -1504 60 1536 -1 val(outp):@Node=2,Var=12,Grp=-1,Idx=0
use hwout 384 183 100 0 hwout#71
xform 0 480 224
p 472 224 60 1536 -1 val(outp):@Node=2,Var=0,Grp=-1,Idx=0
use elongins -196 -1408 100 0 instHKFreeze
xform 0 -128 -1472
p -396 -1224 100 0 0 DTYP:wFireVarMsg
p -512 -1474 100 0 0 EGU:none
use elongins -196 -1216 100 0 instDataSimul
xform 0 -128 -1280
p -396 -1032 100 0 0 DTYP:wFireVarMsg
p -512 -1282 100 0 0 EGU:none
use elongins -196 -1024 100 0 instCPState
xform 0 -128 -1088
p -396 -840 100 0 0 DTYP:wFireVarMsg
p -512 -1090 100 0 0 EGU:none
use elongins -196 -832 100 0 instSCBRegVal
xform 0 -128 -896
p -396 -648 100 0 0 DTYP:wFireVarMsg
p -512 -898 100 0 0 EGU:none
use elongins -196 -640 100 0 instProtection
xform 0 -128 -704
p -396 -456 100 0 0 DTYP:wFireVarMsg
p -512 -706 100 0 0 EGU:none
use elongins -196 -448 100 0 instDeactivate
xform 0 -128 -512
p -396 -264 100 0 0 DTYP:wFireVarMsg
p -512 -514 100 0 0 EGU:none
use elongins -196 -256 100 0 instStatusReport
xform 0 -128 -320
p -396 -72 100 0 0 DTYP:wFireVarMsg
p -512 -322 100 0 0 EGU:none
use elongins -196 -64 100 0 instEchoMe
xform 0 -128 -128
p -396 120 100 0 0 DTYP:wFireVarMsg
p -512 -130 100 0 0 EGU:none
use elongins -196 128 100 0 instNumArrays
xform 0 -128 64
p -396 312 100 0 0 DTYP:wFireVarMsg
p -512 62 100 0 0 EGU:none
use elongins -196 320 100 0 instTraceFlag
xform 0 -128 256
p -396 504 100 0 0 DTYP:wFireVarMsg
p -512 254 100 0 0 EGU:none
use eborderC -768 -1713 100 0 eborderC#23
xform 0 912 -408
p 1808 -1560 100 768 -1 author:K. Ramey
p 1792 -1592 100 768 -1 date:25-Aug-97
p 2032 -1512 200 768 -1 file:iconRegs.sch
p 2080 -1560 100 0 -1 revision:1.0
[comments]
