[schematic2]
uniq 135
[tools]
[detail]
w 836 -1437 100 0 n#119 elongouts.seq_ColCnt.OUT 800 -1440 920 -1440 hwout.hwout#123.outp
w 40 -1365 100 0 n#118 hwin.hwin#121.in 0 -1368 128 -1368 elongins.seqColCnt.INP
w 836 -477 100 0 n#117 eaos.seq_FIntTimeSecs.OUT 804 -480 928 -480 hwout.hwout#115.outp
w 834 -309 100 0 n#116 eaos.seq_IntTimeSecs.OUT 800 -320 928 -320 hwout.hwout#114.outp
w 836 -1277 100 0 n#111 elongouts.seq_ROISize.OUT 800 -1280 920 -1280 hwout.hwout#110.outp
w 836 -1117 100 0 n#106 elongouts.seq_NAvg.OUT 800 -1120 920 -1120 hwout.hwout#107.outp
w 840 -957 100 0 n#105 elongouts.seq_Quadrant.OUT 804 -960 924 -960 hwout.hwout#104.outp
w 836 -797 100 0 n#100 elongouts.seq_HdwrPtr.OUT 800 -800 920 -800 hwout.hwout#101.outp
w 836 -637 100 0 n#99 elongouts.seq_SpadFilter.OUT 800 -640 920 -640 hwout.hwout#98.outp
w 836 -157 100 0 n#94 elongouts.seq_Coadds.OUT 800 -160 920 -160 hwout.hwout#95.outp
w 836 3 100 0 n#93 elongouts.seq_LNR.OUT 800 0 920 0 hwout.hwout#92.outp
w 1716 131 100 0 n#88 elongouts.seq_CntrlReg.OUT 1680 128 1800 128 hwout.hwout#89.outp
w 836 323 100 0 n#87 elongouts.seq_Frames.OUT 800 320 920 320 hwout.hwout#86.outp
w 840 483 100 0 n#84 elongouts.seq_TraceFlag.OUT 804 480 924 480 hwout.hwout#83.outp
w 40 -725 100 0 n#69 hwin.hwin#68.in 0 -728 128 -728 elongins.seqHdwrPtr.INP
w 40 395 100 0 n#66 hwin.hwin#65.in 0 392 128 392 elongins.seqFrames.INP
w 40 -1045 100 0 n#64 hwin.hwin#45.in 0 -1048 128 -1048 elongins.seqNAvg.INP
w 40 -1205 100 0 n#63 hwin.hwin#46.in 0 -1208 128 -1208 elongins.seqROISize.INP
w 40 -245 100 0 n#62 hwin.hwin#47.in 0 -248 128 -248 eais.seqIntTimeSecs.INP
w 40 -405 100 0 n#61 hwin.hwin#48.in 0 -408 128 -408 eais.seqFIntTimeSecs.INP
w 40 235 100 0 n#60 hwin.hwin#49.in 0 232 128 232 elongins.seqCntrlReg.INP
w 40 75 100 0 n#59 hwin.hwin#50.in 0 72 128 72 elongins.seqLNR.INP
w 40 -85 100 0 n#58 hwin.hwin#51.in 0 -88 128 -88 elongins.seqCoadds.INP
w 40 -565 100 0 n#57 hwin.hwin#52.in 0 -568 128 -568 elongins.seqSpadFilter.INP
w 40 -885 100 0 n#56 hwin.hwin#53.in 0 -888 128 -888 elongins.seqQuadrant.INP
w 40 555 100 0 n#55 hwin.hwin#54.in 0 552 128 552 elongins.seqTraceFlag.INP
s -568 616 500 0 Sequencer Variables
[cell use]
use hwout 928 -521 100 0 hwout#115
xform 0 1024 -480
p 1024 -489 100 0 -1 val(outp):@Node=10,Var=9,Grp=-1,Idx=0
use hwout 928 -361 100 0 hwout#114
xform 0 1024 -320
p 1024 -329 100 0 -1 val(outp):@Node=10,Var=7,Grp=-1,Idx=0
use hwout 920 -1321 100 0 hwout#110
xform 0 1016 -1280
p 1016 -1289 100 0 -1 val(outp):@Node=10,Var=13,Grp=-1,Idx=0
use hwout 920 -1161 100 0 hwout#107
xform 0 1016 -1120
p 1016 -1129 100 0 -1 val(outp):@Node=10,Var=12,Grp=-1,Idx=0
use hwout 924 -1001 100 0 hwout#104
xform 0 1020 -960
p 1020 -969 100 0 -1 val(outp):@Node=10,Var=11,Grp=-1,Idx=0
use hwout 920 -841 100 0 hwout#101
xform 0 1016 -800
p 1016 -809 100 0 -1 val(outp):@Node=10,Var=10,Grp=-1,Idx=0
use hwout 920 -681 100 0 hwout#98
xform 0 1016 -640
p 1016 -649 100 0 -1 val(outp):@Node=10,Var=9,Grp=-1,Idx=0
use hwout 920 -201 100 0 hwout#95
xform 0 1016 -160
p 1016 -169 100 0 -1 val(outp):@Node=10,Var=6,Grp=-1,Idx=0
use hwout 920 -41 100 0 hwout#92
xform 0 1016 0
p 1016 -9 100 0 -1 val(outp):@Node=10,Var=5,Grp=-1,Idx=0
use hwout 1800 87 100 0 hwout#89
xform 0 1896 128
p 1896 119 100 0 -1 val(outp):@Node=10,Var=4,Grp=-1,Idx=0
use hwout 920 279 100 0 hwout#86
xform 0 1016 320
p 1016 311 100 0 -1 val(outp):@Node=10,Var=2,Grp=-1,Idx=0
use hwout 924 439 100 0 hwout#83
xform 0 1020 480
p 1020 471 100 0 -1 val(outp):@Node=10,Var=0,Grp=-1,Idx=0
use hwout 920 -1481 100 0 hwout#123
xform 0 1016 -1440
p 1016 -1449 100 0 -1 val(outp):@Node=10,Var=14,Grp=-1,Idx=0
use elongouts 616 -1172 100 0 seq_ROISize
xform 0 672 -1248
p 500 -856 100 0 0 DTYP:wFireVarMsg
use elongouts 616 -1012 100 0 seq_NAvg
xform 0 672 -1088
p 500 -696 100 0 0 DTYP:wFireVarMsg
use elongouts 620 -852 100 0 seq_Quadrant
xform 0 676 -928
p 504 -536 100 0 0 DTYP:wFireVarMsg
use elongouts 616 -692 100 0 seq_HdwrPtr
xform 0 672 -768
p 500 -376 100 0 0 DTYP:wFireVarMsg
use elongouts 616 -532 100 0 seq_SpadFilter
xform 0 672 -608
p 500 -216 100 0 0 DTYP:wFireVarMsg
use elongouts 616 -52 100 0 seq_Coadds
xform 0 672 -128
p 500 264 100 0 0 DTYP:wFireVarMsg
p 384 14 100 0 0 EGU:
use elongouts 616 108 100 0 seq_LNR
xform 0 672 32
p 500 424 100 0 0 DTYP:wFireVarMsg
p 384 174 100 0 0 EGU:
use elongouts 1496 236 100 0 seq_CntrlReg
xform 0 1552 160
p 1380 552 100 0 0 DTYP:wFireVarMsg
p 1264 302 100 0 0 EGU:
p 1264 78 100 0 0 OMSL:supervisory
use elongouts 616 428 100 0 seq_Frames
xform 0 672 352
p 500 744 100 0 0 DTYP:wFireVarMsg
p 384 494 100 0 0 EGU:
use elongouts 620 588 100 0 seq_TraceFlag
xform 0 676 512
p 504 904 100 0 0 DTYP:wFireVarMsg
p 388 654 100 0 0 EGU:
use elongouts 616 -1332 100 0 seq_ColCnt
xform 0 672 -1408
p 500 -1016 100 0 0 DTYP:wFireVarMsg
use hwin -192 -769 100 0 hwin#68
xform 0 -96 -728
p -408 -736 100 0 -1 val(in):@Node=10,Var=10,Grp=-1,Idx=0
use hwin -192 351 100 0 hwin#65
xform 0 -96 392
p -392 384 100 0 -1 val(in):@Node=10,Var=2,Grp=-1,Idx=0
use hwin -192 511 100 0 hwin#54
xform 0 -96 552
p -392 544 100 0 -1 val(in):@Node=10,Var=0,Grp=-1,Idx=0
use hwin -192 -929 100 0 hwin#53
xform 0 -96 -888
p -400 -896 100 0 -1 val(in):@Node=10,Var=11,Grp=-1,Idx=0
use hwin -192 -609 100 0 hwin#52
xform 0 -96 -568
p -392 -576 100 0 -1 val(in):@Node=10,Var=9,Grp=-1,Idx=0
use hwin -192 -129 100 0 hwin#51
xform 0 -96 -88
p -392 -96 100 0 -1 val(in):@Node=10,Var=6,Grp=-1,Idx=0
use hwin -192 31 100 0 hwin#50
xform 0 -96 72
p -392 64 100 0 -1 val(in):@Node=10,Var=5,Grp=-1,Idx=0
use hwin -192 191 100 0 hwin#49
xform 0 -96 232
p -392 224 100 0 -1 val(in):@Node=10,Var=4,Grp=-1,Idx=0
use hwin -192 -449 100 0 hwin#48
xform 0 -96 -408
p -400 -416 100 0 -1 val(in):@Node=10,Var=8,Grp=-1,Idx=0
use hwin -192 -289 100 0 hwin#47
xform 0 -96 -248
p -400 -256 100 0 -1 val(in):@Node=10,Var=7,Grp=-1,Idx=0
use hwin -192 -1249 100 0 hwin#46
xform 0 -96 -1208
p -408 -1216 100 0 -1 val(in):@Node=10,Var=13,Grp=-1,Idx=0
use hwin -192 -1089 100 0 hwin#45
xform 0 -96 -1048
p -408 -1056 100 0 -1 val(in):@Node=10,Var=12,Grp=-1,Idx=0
use hwin -192 -1409 100 0 hwin#121
xform 0 -96 -1368
p -408 -1376 100 0 -1 val(in):@Node=10,Var=14,Grp=-1,Idx=0
use elongins 188 -696 100 0 seqHdwrPtr
xform 0 256 -760
p -12 -512 100 0 0 DTYP:wFireVarMsg
p -128 -762 100 0 0 EGU:none
use elongins 188 -1016 100 0 seqNAvg
xform 0 256 -1080
p -12 -832 100 0 0 DTYP:wFireVarMsg
p -128 -1082 100 0 0 EGU:none
use elongins 188 584 100 0 seqTraceFlag
xform 0 256 520
p -12 768 100 0 0 DTYP:wFireVarMsg
p -128 518 100 0 0 EGU:none
use elongins 188 424 100 0 seqFrames
xform 0 256 360
p -12 608 100 0 0 DTYP:wFireVarMsg
p -128 358 100 0 0 EGU:none
use elongins 188 -1176 100 0 seqROISize
xform 0 256 -1240
p -12 -992 100 0 0 DTYP:wFireVarMsg
p -128 -1242 100 0 0 EGU:none
use elongins 188 264 100 0 seqCntrlReg
xform 0 256 200
p -12 448 100 0 0 DTYP:wFireVarMsg
p -128 198 100 0 0 EGU:none
use elongins 188 104 100 0 seqLNR
xform 0 256 40
p -12 288 100 0 0 DTYP:wFireVarMsg
p -128 38 100 0 0 EGU:none
use elongins 188 -56 100 0 seqCoadds
xform 0 256 -120
p -12 128 100 0 0 DTYP:wFireVarMsg
p -128 -122 100 0 0 EGU:none
use elongins 188 -536 100 0 seqSpadFilter
xform 0 256 -600
p -12 -352 100 0 0 DTYP:wFireVarMsg
p -128 -602 100 0 0 EGU:none
use elongins 188 -856 100 0 seqQuadrant
xform 0 256 -920
p -12 -672 100 0 0 DTYP:wFireVarMsg
p -128 -922 100 0 0 EGU:none
use elongins 188 -1336 100 0 seqColCnt
xform 0 256 -1400
p -12 -1152 100 0 0 DTYP:wFireVarMsg
p -128 -1402 100 0 0 EGU:none
use eaos 596 -380 100 0 seq_FIntTimeSecs
xform 0 676 -448
p 435 -214 100 0 0 DTYP:wFireVarMsg
p 516 -722 100 0 0 EGU:Secs
p 292 -594 100 0 0 PREC:3
use eaos 616 -216 100 0 seq_IntTimeSecs
xform 0 672 -288
p 431 -54 100 0 0 DTYP:wFireVarMsg
p 512 -562 100 0 0 EGU:Secs
p 288 -434 100 0 0 PREC:3
use eborderC -760 -1717 100 0 eborderC#23
xform 0 920 -412
p 1816 -1564 100 768 -1 author:K. Ramey
p 1800 -1596 100 768 -1 date:25-Aug-97
p 2040 -1516 200 768 -1 file:iconRegs.sch
p 2088 -1564 100 0 -1 revision:1.0
use eais 196 -220 100 0 seqIntTimeSecs
xform 0 256 -280
p 14 -160 100 0 0 DTYP:wFireVarMsg
p -128 -538 100 0 0 EGU:Secs
use eais 196 -384 100 0 seqFIntTimeSecs
xform 0 256 -440
p 14 -320 100 0 0 DTYP:wFireVarMsg
p -128 -698 100 0 0 EGU:Seconds
[comments]
