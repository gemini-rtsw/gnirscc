[schematic2]
uniq 150
[tools]
[detail]
w -442 -29 100 0 n#142 elongouts.ReadHK.OUT -448 -32 -376 -32 hwout.hwout#139.outp
w 582 195 100 0 n#141 elongouts.StartUC.OUT 576 192 648 192 hwout.hwout#138.outp
w 578 419 100 0 n#140 elongouts.KillUC.OUT 576 416 640 416 hwout.hwout#137.outp
w 1202 779 100 0 n#132 hwin.hwin#99.in 1152 768 1312 768 estringins.Jsi.INP
w 738 675 100 0 n#131 estringouts.Jso.OUT 672 672 864 672 hwout.hwout#97.outp
w 1190 963 100 0 n#130 hwin.hwin#95.in 1152 960 1288 960 1288 956 elongins.Jli.INP
w 738 867 100 0 n#129 elongouts.Jlo.OUT 672 864 864 864 hwout.hwout#93.outp
w 1198 1155 100 0 n#128 hwin.hwin#39.in 1152 1152 1304 1152 eais.Jai.INP
w 738 1059 100 0 n#127 eaos.Jao.OUT 672 1056 864 1056 hwout.hwout#36.outp
w 578 -725 100 0 n#122 elongouts.imNum.OUT 576 -736 640 -736 hwout.hwout#121.outp
w -142 291 100 0 n#112 estringouts.dcHealthStr.OUT -128 288 -96 288 hwout.hwout#111.outp
w -454 307 100 0 n#110 embbis.dcHealth.FLNK -480 368 -464 368 -464 304 -384 304 estringouts.dcHealthStr.SLNK
w -462 339 100 0 n#109 embbis.dcHealth.VAL -480 336 -384 336 estringouts.dcHealthStr.DOL
w -446 -341 100 0 n#107 eaos.naacHeartBeat.OUT -448 -352 -384 -352 hwout.hwout#106.outp
w -446 611 100 0 n#103 estringouts.naacState.OUT -448 600 -384 600 hwout.hwout#102.outp
w -786 387 100 0 n#7 embbis.dcHealth.INP -736 384 -800 384 healthCheck.healthChk.healthOut
w -770 -285 100 0 n#5 heartBeat.heartBeat#78.hBeatOut -800 -288 -704 -288 eaos.naacHeartBeat.DOL
s -496 696 100 0 The "PP MS" strings have to be explicitly included.
s -496 728 100 0 is a channel access connection from the OUT link
s -496 760 100 0 the PP and MS fields are ignored when the
s -496 792 100 0 BUG WORK AROUND: e2sr has a bug in which
s 272 -256 100 0 NOTE; imPath, imName and imNum are initialized by pvload from initVals.par
[cell use]
use egenSub 1640 264 100 0 clearErrors
xform 0 1712 -144
p 1345 -795 100 0 0 FTA:STRING
p 1656 -160 60 1536 1 SNAM:clrError
use seqVars -484 -745 100 0 seqVars#145
xform 0 -416 -608
use hwout 640 -777 100 0 hwout#121
xform 0 736 -736
p 736 -745 100 0 -1 val(outp):$(sadtop)dataFrame.VAL PP MS
use hwout -96 247 100 0 hwout#111
xform 0 0 288
p 0 279 100 0 -1 val(outp):$(sadtop)health.VAL PP MS
use hwout -384 -393 100 0 hwout#106
xform 0 -288 -352
p -288 -361 100 0 -1 val(outp):$(sadtop)heartBeat.VAL PP MS
use hwout -384 559 100 0 hwout#102
xform 0 -288 600
p -288 591 100 0 -1 val(outp):$(sadtop)detState.VAL PP MS
use hwout 864 631 100 0 hwout#97
xform 0 960 672
p 960 663 100 0 -1 val(outp):@Node=2,Var=0,Grp=-1,Idx=0
use hwout 864 823 100 0 hwout#93
xform 0 960 864
p 960 855 100 0 -1 val(outp):@Node=2,Var=0,Grp=-1,Idx=0
use hwout 864 1015 100 0 hwout#36
xform 0 960 1056
p 956 1048 100 0 -1 val(outp):@Node=2,Var=0,Grp=-1,Idx=0
use hwout 640 375 100 0 hwout#137
xform 0 736 416
p 736 407 100 0 -1 val(outp):@Node=10,Cmd=15,Grp=-1,Idx=0
use hwout 648 151 100 0 hwout#138
xform 0 744 192
p 744 184 100 0 -1 val(outp):@Node=10,Cmd=16,Grp=-1,Idx=0
use hwout -376 -73 100 0 hwout#139
xform 0 -280 -32
p -280 -41 100 0 -1 val(outp):@Node=2,Cmd=11,Grp=-1,Idx=0
use elongouts 384 -624 100 0 imNum
xform 0 448 -704
p 384 -800 65 0 1 EGU:number
p 384 -784 65 0 1 PV:$(top)
p 608 -736 75 768 -1 palrm(OUT):MS
p 576 -736 75 768 -1 pproc(OUT):PP
use elongouts 508 968 100 0 Jlo
xform 0 544 896
p 488 796 65 0 1 DTYP:wFireVarMsg
p 488 812 65 0 1 PV:$(top)DevSup:
use elongouts 384 528 100 0 KillUC
xform 0 448 448
p 276 840 100 0 0 DTYP:wFireCmdMsg
use elongouts 384 296 100 0 StartUC
xform 0 448 224
p 276 616 100 0 0 DTYP:wFireCmdMsg
use elongouts -640 72 100 0 ReadHK
xform 0 -576 0
p -864 46 100 0 0 DISS:MINOR
p -748 392 100 0 0 DTYP:wFireCmdMsg
p -632 -40 60 1536 1 SCAN:Passive
p -928 -24 60 1536 -1 def(SDIS):$(top)instHKFreeze.VAL
p -736 -32 75 1280 -1 pproc(SDIS):PP
use iconRegs -752 -440 -100 0 tpVars
xform 0 -608 -608
p -616 -752 60 1792 1 set1:top $(top)
use notes -1056 967 100 0 notes#123
xform 0 -800 1152
p -1028 1278 100 0 -1 COMMENT1:The hardware outputs connecting to
p -1028 1246 100 0 -1 COMMENT2:the field names beginning with
p -1028 1216 100 0 -1 COMMENT3:$(sadtop) are channel access
p -1028 1184 100 0 -1 COMMENT4:links to the Status Alarm Database.
use notes 392 1191 100 0 notes#101
xform 0 648 1376
p 420 1502 100 0 -1 COMMENT1:NOTES:  The device support routines are 
p 420 1470 100 0 -1 COMMENT2:needed to send messages to the
p 420 1440 100 0 -1 COMMENT3:transputers.  When these routines are
p 420 1408 100 0 -1 COMMENT4:incorporated the records below must be
p 420 1376 100 0 -1 COMMENT5:modified by:
p 420 1344 100 0 -1 COMMENT6:1) re-connect the hwin and hwout symbols
p 420 1312 100 0 -1 COMMENT7:2) change DTYPE to wFireVarMsg
use estringouts 384 -464 100 0 imName
xform 0 448 -528
p 384 -592 65 0 1 PV:$(top)
use estringouts 384 -304 100 0 imPath
xform 0 448 -368
p 384 -432 65 0 1 PV:$(top)
use estringouts -320 368 100 0 dcHealthStr
xform 0 -256 304
p -320 240 65 0 1 OMSL:closed_loop
p -320 224 65 0 1 PV:$(top)
p -384 336 75 1280 -1 palrm(DOL):MS
p -96 288 75 768 -1 palrm(OUT):MS
p -128 288 75 768 -1 pproc(OUT):PP
use estringouts 512 744 100 0 Jso
xform 0 544 688
p 492 604 65 0 1 DTYP:wFireVarMsg
p 492 620 65 0 1 PV:$(top)DevSup:
use estringouts -636 668 100 0 naacState
xform 0 -576 616
p -624 552 65 0 1 PV:$(top)
p -624 536 65 0 1 VAL:IDLE
p -704 648 75 1280 -1 palrm(DOL):MS
p -416 600 75 768 -1 palrm(OUT):MS
p -448 600 75 768 -1 pproc(OUT):PP
use eaos -640 -240 100 0 naacHeartBeat
xform 0 -576 -320
p -736 -594 100 0 0 EGU:
p -624 -400 65 0 1 OMSL:closed_loop
p -624 -416 65 0 1 PV:$(top)
p -624 -432 65 0 1 SCAN:1 second
p -592 -416 100 1024 0 name:$(top)$(I)
p -704 -288 75 1280 -1 palrm(DOL):MS
p -416 -352 75 768 -1 palrm(OUT):MS
p -736 -288 75 1280 -1 pproc(DOL):PP
p -448 -352 75 768 -1 pproc(OUT):PP
use eaos 484 1168 100 0 Jao
xform 0 544 1088
p 484 996 65 1536 1 DTYP:wFireVarMsg
p 484 1012 65 1536 1 PV:$(top)DevSup:
use CBorder -1288 -908 -100 0 frame
xform 0 392 396
p 1280 -772 100 1536 1 Date:24 Apr 97
p 1384 -676 300 1792 -1 Dnumber:
p 1512 -692 150 1536 -1 Title:hdwrNEngFuncs.sch
use hwin 960 727 100 0 hwin#99
xform 0 1056 768
p 828 760 100 0 -1 val(in):@Node=2,Var=0,Grp=-1,Idx=0
use hwin 960 919 100 0 hwin#95
xform 0 1056 960
p 836 956 100 0 -1 val(in):@Node=2,Var=0,Grp=-1,Idx=0
use hwin 960 1111 100 0 hwin#39
xform 0 1056 1152
p 840 1144 100 0 -1 val(in):@Node=2,Var=9,Grp=4,Idx=0
use estringins 1408 800 100 0 Jsi
xform 0 1440 736
p 1388 648 65 0 1 DTYP:wFireVarMsg
p 1388 668 65 0 1 PV:$(top)DevSup:
use elongins 1384 980 100 0 Jli
xform 0 1416 924
p 1360 836 65 0 1 DTYP:wFireVarMsg
p 1360 852 65 0 1 PV:$(top)DevSup:
use embbis -672 408 100 0 dcHealth
xform 0 -608 352
p -672 256 65 0 1 ONST:WARNING
p -560 256 65 0 1 ONSV:MINOR
p -800 366 100 0 0 ONVL:1
p -664 328 65 1536 1 PV:$(top)
p -664 312 60 1536 1 SCAN:Passive
p -672 240 65 0 1 TWST:BAD
p -560 240 65 0 1 TWSV:MAJOR
p -800 334 100 0 0 TWVL:2
p -672 272 65 0 1 ZRST:GOOD
p -560 272 65 0 1 ZRSV:NO_ALARM
p -736 384 75 1280 -1 palrm(INP):MS
p -768 384 75 1280 -1 pproc(INP):PP
use diagFuncs -896 -825 100 0 diagFuncs#81
xform 0 -800 -608
p -864 -748 65 1536 1 set1:top $(top)
use heartBeat -1072 -441 100 0 heartBeat#78
xform 0 -928 -272
p -1056 -400 65 1536 1 set1:top $(top)
use dataSimul -1072 -809 100 0 dataSimul#75
xform 0 -992 -608
p -1056 -752 65 1536 1 set1:top $(top)
use eais 1372 1176 100 0 Jai
xform 0 1432 1120
p 1372 1044 65 1536 1 DTYP:wFireVarMsg
p 1372 1060 65 1536 1 PV:$(top)DevSup:
use healthCheck -1056 488 100 0 healthChk
xform 0 -928 384
p -1056 272 65 1536 1 set1:top $(top)
[comments]
