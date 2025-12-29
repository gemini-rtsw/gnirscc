[schematic2]
uniq 135
[tools]
[detail]
w 1060 1195 100 2 n#134 hwout.hwout#133.outp 1056 1200 1056 1200 elongouts.subsysDir.OUT
w 4180 419 100 0 n#132 efanouts.fanStartTimer.LNK1 4080 624 4176 624 4176 224 4304 224 eaos.fanStartTimerT.SLNK
w 4098 571 100 0 n#132 efanouts.fanStartTimer.LNK3 4080 560 4176 560 junction
w 4098 539 100 0 n#132 efanouts.fanStartTimer.LNK4 4080 528 4176 528 junction
w 4098 507 100 0 n#132 efanouts.fanStartTimer.LNK5 4080 496 4176 496 junction
w 4098 475 100 0 n#132 efanouts.fanStartTimer.LNK6 4080 464 4176 464 junction
w 2514 475 100 0 n#130 efanouts.fan2.LNK4 2352 528 2464 528 2464 464 2624 464 eaos.fan2Term.SLNK
w 2378 507 100 0 n#130 efanouts.fan2.LNK5 2352 496 2464 496 junction
w 2378 475 100 0 n#130 efanouts.fan2.LNK6 2352 464 2464 464 junction
w 2364 1139 100 0 n#128 estringouts.errMess.SLNK 2848 1520 2368 1520 2368 768 2496 768 2496 624 2352 624 efanouts.fan2.LNK1
w 2506 603 100 0 n#127 efanouts.fan2.LNK2 2352 592 2720 592 2720 672 2880 672 elongouts.getSubapplyC.SLNK
w 4234 595 100 0 n#125 efanouts.fanStartTimer.LNK2 4080 592 4448 592 eaos.startApplyCTrig.SLNK
w 3754 555 100 0 n#123 ecalcs.calcApplyBusy.FLNK 3696 624 3728 624 3728 544 3840 544 efanouts.fanStartTimer.SLNK
w 3794 635 100 0 n#122 ecalcs.calcApplyBusy.VAL 3696 592 3760 592 3760 624 3888 624 efanouts.fanStartTimer.SELL
w 3298 795 100 0 n#120 elongouts.getSubapplyC.VAL 3136 672 3248 672 3248 784 3408 784 ecalcs.calcApplyBusy.INPA
w 3316 547 100 0 n#119 elongouts.getSubapplyC.FLNK 3136 704 3312 704 3312 400 3408 400 ecalcs.calcApplyBusy.SLNK
w 90 1675 100 0 DISV inhier.SDIS.P 32 1664 208 1664 208 1744 368 1744 estringouts.null.SDIS
w 2332 1163 100 0 n#107 estringouts.subApplyMess.FLNK 2304 864 2336 864 2336 1472 1968 1472 1968 1856 2016 1856 gmSeqTimeOut.gmSeqTimeOut#17.STOP
w 1520 2051 100 0 n#95 gmSeqTimeOut.gmSeqTimeOut#17.START 2016 2048 1072 2048 1072 1264 1056 1264 elongouts.subsysDir.FLNK
w 728 1243 100 0 n#93 elongouts.busy.FLNK 624 1536 704 1536 704 1232 800 1232 elongouts.subsysDir.SLNK
w 640 1275 100 0 DIR inhier.DIR.P 528 1264 800 1264 elongouts.subsysDir.DOL
w 1824 523 100 0 n#91 ecalcs.subApplyValCalc.VAL 1744 512 1952 512 1952 624 2160 624 efanouts.fan2.SELL
w 1900 211 100 0 SUBVAL junction 1280 464 1280 208 2592 208 outhier.SUBVAL.p
w 1352 715 100 0 SUBVAL eais.subApplyVal.VAL 1264 464 1280 464 1280 704 1456 704 ecalcs.subApplyValCalc.INPA
w 928 619 100 0 n#89 ewait.wait.VAL 912 608 992 608 992 512 1008 512 eais.subApplyVal.INP
w 1288 507 100 0 n#88 eais.subApplyVal.FLNK 1264 496 1360 496 1360 320 1456 320 ecalcs.subApplyValCalc.SLNK
w 912 331 100 0 n#87 ewait.wait.FLNK 912 320 960 320 960 480 1008 480 eais.subApplyVal.SLNK
w 2052 875 100 2 c#82 lboat.c#82.p 2048 880 2048 880 estringouts.subApplyMess.DOL
w 2136 731 100 0 n#80 efanouts.fan2.FLNK 2352 656 2400 656 2400 720 1920 720 1920 848 2048 848 estringouts.subApplyMess.SLNK
w 3108 1499 100 2 c#75 rboat.c#75.p 3104 1504 3104 1504 estringouts.errMess.OUT
w 3108 2027 100 2 c#74 rboat.c#74.p 3104 2032 3104 2032 estringouts.noResp.OUT
w 3108 1755 100 2 c#73 rboat.c#73.p 3104 1760 3104 1760 eaos.errVal.OUT
w 136 715 100 0 c#71 lboat.c#71.p 112 704 208 704 ewait.wait.INAN
w 3298 1608 100 0 n#57 eaos.errVal.FLNK 3104 1824 3296 1824 3296 1344 2976 1344 2976 1168 3040 1168 eaos.stopApplyCTrig.SLNK
w 2992 1202 100 0 n#54 hwin.hwin#56.in 2992 1200 3040 1200 eaos.stopApplyCTrig.DOL
w 2968 1635 100 0 n#53 estringouts.errMess.FLNK 3104 1536 3168 1536 3168 1632 2816 1632 2816 1792 junction
w 2968 1923 100 0 n#53 estringouts.noResp.FLNK 3104 2064 3168 2064 3168 1920 2816 1920 2816 1792 2848 1792 eaos.errVal.SLNK
w 1904 555 100 0 n#49 ecalcs.subApplyValCalc.FLNK 1744 544 2112 544 efanouts.fan2.SLNK
w 2632 2051 100 0 n#40 gmSeqTimeOut.gmSeqTimeOut#17.EXPIRED 2464 2048 2848 2048 estringouts.noResp.SLNK
w 4400 627 100 0 n#38 hwin.hwin#21.in 4400 624 4448 624 eaos.startApplyCTrig.DOL
w 2792 1826 100 0 n#29 hwin.hwin#28.in 2784 1824 2848 1824 eaos.errVal.DOL
w 472 1634 100 0 n#14 estringouts.null.FLNK 624 1792 720 1792 720 1632 272 1632 272 1504 368 1504 elongouts.busy.SLNK
w 102 1810 100 0 n#7 inhier.SLNK.P 32 1808 208 1808 208 1776 368 1776 estringouts.null.SLNK
w 294 1538 100 0 n#5 hwin.hwin#4.in 256 1536 368 1536 elongouts.busy.DOL
s 1408 128 130 0 (Command not rejected)
s 288 192 130 0 Wait for monitor on apply.VAL
s 3344 240 100 0 output 1 if BUSY, 2 if not
s 4160 512 100 0 LNK2 if applyC not BUSY
s 3376 272 100 0 Test for CAR_BUSY (2)
s 2432 560 100 0 LNK2 if apply.VAL positive
s 2448 336 130 0 Trigger timer only if apply.VAL positive AND applyC not BUSY
s 1360 176 140 0 Check for positive apply.VAL
s 1936 960 100 0 top-level apply record
s 1936 992 100 0 This string is input to the
s 4480 2976 150 0 gmSeqDriveSubApply
s 1984 1376 140 0 See schematic gmSeqCarMonitor
s 1984 1328 140 0 for CommSentC, StartapplyCTimer and
s 1984 1280 140 0 StopapplyCTimer records
s 2688 816 100 0 Get current applyC value from subsystem
s 3440 832 100 0 Check whether applyC is BUSY
s 4160 768 140 0 Trigger timeout start if not BUSY
s 2432 640 100 0 LNK1: subsys error
[cell use]
use hwout 1056 1159 100 0 hwout#133
xform 0 1152 1200
p 1136 1152 100 0 -1 val(outp):$(prefix)apply.DIR PP NMS
use eaos 4472 504 100 0 startApplyCTrig
xform 0 4576 592
p 4496 686 100 0 -1 DESC:Start timer on subsystem applyC
p 4512 448 100 0 1 OMSL:closed_loop
p 4432 384 100 0 1 def(OUT):$(nirs)$(subsys)StartapplyCTimer.PROC
p 4704 560 75 768 -1 pproc(OUT):PP
use eaos 2872 1704 100 0 errVal
xform 0 2976 1792
p 2880 1870 100 0 -1 DESC:Write error status
p 2912 1664 100 0 1 OMSL:closed_loop
p 3136 1790 100 0 -1 def(OUT):$(nirs)$(subsys)CommSentC.IVAL
p 3104 1760 75 768 -1 pproc(OUT):PP
use eaos 3064 1080 100 0 stopApplyCTrig
xform 0 3168 1168
p 3088 1262 100 0 -1 DESC:Cancel timer on subsystem applyC
p 3104 1024 100 0 1 OMSL:closed_loop
p 2944 1054 100 0 1 def(OUT):$(nirs)$(subsys)StopapplyCTimer.PROC
p 3296 1136 75 768 -1 pproc(OUT):PP
use eaos 2624 375 100 0 fan2Term
xform 0 2752 464
use eaos 4304 135 100 0 fanStartTimerT
xform 0 4432 224
use elongouts 824 1144 100 0 subsysDir
xform 0 928 1232
p 832 1326 100 0 -1 DESC:Set directive in subsystem APPLY
p 864 1104 100 0 1 OMSL:closed_loop
p 1168 1198 100 0 -1 def(OUT):
p 1056 1200 75 768 -1 pproc(OUT):PP
use elongouts 392 1416 100 0 busy
xform 0 496 1504
p 400 1598 100 0 -1 DESC:Output BUSY value
p 432 1376 100 0 1 OMSL:closed_loop
p 736 1470 100 0 -1 def(OUT):$(nirs)$(subsys)CommSentC.IVAL
p 624 1472 75 768 -1 pproc(OUT):PP
use elongouts 2880 583 100 0 getSubapplyC
xform 0 3008 672
p 2928 752 100 0 1 OMSL:closed_loop
p 2528 736 100 0 1 def(DOL):$(prefix)applyC.IVAL
use efanouts 2136 408 100 0 fan2
xform 0 2232 560
p 2096 672 100 0 1 SELM:Specified
use efanouts 3840 407 100 0 fanStartTimer
xform 0 3960 560
p 3824 672 100 0 1 SELM:Specified
use ecalcs 1480 232 100 0 subApplyValCalc
xform 0 1600 496
p 1536 446 100 0 1 CALC:A<0?1:2
use ecalcs 3408 311 100 0 calcApplyBusy
xform 0 3552 576
p 3472 640 100 0 1 CALC:A=2?1:2
use oslBorderD -208 -265 100 0 oslBorderD#114
xform 0 2432 1440
use inhier 448 1248 150 0 DIR
xform 0 528 1264
use inhier 40 1768 100 0 SLNK
xform 0 32 1808
use inhier 16 1623 100 0 SDIS
xform 0 32 1664
use rboat 3104 1463 100 0 c#75
xform 0 3184 1504
use rboat 3104 1991 100 0 c#74
xform 0 3184 2032
use rboat 3104 1719 100 0 c#73
xform 0 3184 1760
use hwin 88 1496 100 0 hwin#4
xform 0 160 1536
p 67 1528 100 0 -1 val(in):$(CAR_BUSY)
use hwin 4232 584 100 0 hwin#21
xform 0 4304 624
p 4211 616 100 0 -1 val(in):1
use hwin 2616 1784 100 0 hwin#28
xform 0 2688 1824
p 2595 1816 100 0 -1 val(in):$(CAR_ERROR)
use hwin 2824 1160 100 0 hwin#56
xform 0 2896 1200
p 2803 1192 100 0 -1 val(in):1
use eais 1008 407 100 0 subApplyVal
xform 0 1136 480
use outhier 2560 167 100 0 SUBVAL
xform 0 2576 208
use lboat -48 663 100 0 c#71
xform 0 32 704
use lboat 1888 839 100 0 c#82
xform 0 1968 880
use estringouts 392 1704 100 0 null
xform 0 496 1776
p 432 1854 100 0 -1 DESC:Empty string
p 432 1664 100 0 1 OMSL:closed_loop
p 304 1742 100 0 0 VAL:
p 752 1758 100 0 -1 def(OUT):$(nirs)$(subsys)CommSentC.IMSS
p 16 1744 100 0 0 def(SDIS):0.0
use estringouts 2872 1976 100 0 noResp
xform 0 2976 2048
p 2864 2126 100 0 -1 DESC:No response string
p 2912 1936 100 0 1 OMSL:closed_loop
p 2784 2014 100 0 0 VAL:No response from $(instru)
p 3184 2062 100 0 -1 def(OUT):$(nirs)$(subsys)CommSentC.IMSS
use estringouts 2872 1448 100 0 errMess
xform 0 2976 1520
p 2800 1424 100 0 -1 DESC:Subsys error message
p 2912 1392 100 0 1 OMSL:closed_loop
p 2784 1486 100 0 0 VAL:
p 2640 1584 100 0 -1 def(DOL):$(prefix)apply.MESS
p 3168 1534 100 0 -1 def(OUT):$(nirs)$(subsys)CommSentC.IMSS
use estringouts 2048 775 100 0 subApplyMess
xform 0 2176 848
p 2112 736 100 0 1 OMSL:closed_loop
p 1792 912 100 0 -1 def(DOL):$(prefix)apply.MESS
use ewait 232 232 100 0 wait
xform 0 560 560
p 336 350 100 0 0 ADEL:0.000000000000000e+00
p 531 808 100 0 1 CALC:A
p 336 768 100 0 -1 DESC:Wait for subsystem Apply accept/reject
p 592 608 100 0 1 INAP:Yes
p 336 702 100 0 1 SCAN:I/O Intr
p -80 720 100 0 -1 def(INAN):$(prefix)apply.VAL
use gmSeqTimeOut 2040 1800 100 0 gmSeqTimeOut#17
xform 0 2240 1952
p 2240 1854 100 0 -1 seta:timeout 3.0
[comments]
