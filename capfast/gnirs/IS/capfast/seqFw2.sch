[schematic2]
uniq 142
[tools]
[detail]
w 242 715 100 0 focus_c ecad8.Pos.OUTH 128 704 416 704 outhier.focus_c.p
w 1506 1515 100 0 n#140 eaos.lambdaOffset1.FLNK 1440 1504 1632 1504 estringouts.fltSirUnknown1.SLNK
w 434 931 100 0 n#134 ecad8.Pos.VALE 128 928 800 928 800 1248 1152 1248 1152 1504 1184 1504 eaos.lambdaOffset1.DOL
w 1106 1475 100 0 n#133 eaos.Offset1.FLNK 1056 1504 1088 1504 1088 1472 1184 1472 eaos.lambdaOffset1.SLNK
w 196 1195 100 0 n#128 ecad8.Pos.OUTA 128 1152 192 1152 192 1248 256 1248 hwout.hwout#126.outp
w 1378 139 100 0 n#125 elongouts.AssemblyMode.FLNK 1104 160 1232 160 1232 128 1584 128 elongouts.MarkDta.SLNK
w 1512 162 100 0 n#121 hwin.hwin#123.in 1488 160 1584 160 elongouts.MarkDta.DOL
w 668 987 100 0 n#118 ecad8.Pos.STLK 128 512 672 512 672 1472 800 1472 eaos.Offset1.SLNK
w 378 -437 100 0 n#108 ewait.fltWaitForIdle.FLNK 288 -448 528 -448 528 -320 752 -320 eevents.fltCadStartEvent.SLNK
w -412 -69 100 2 n#107 hwin.hwin#106.in -416 -64 -416 -64 ewait.fltWaitForIdle.INAN
w 594 131 100 0 n#102 ecad8.Pos.PLNK 128 544 400 544 400 128 848 128 elongouts.AssemblyMode.SLNK
w 354 1059 100 0 n#100 ecad8.Pos.VALC 128 1056 640 1056 640 1504 800 1504 eaos.Offset1.DOL
w 536 803 100 0 n#96 ecad8.Pos.VALG 128 800 992 800 992 528 1472 528 estringouts.Sad1.DOL
w 680 -277 100 0 n#81 hwin.hwin#80.in 592 -240 656 -240 656 -288 752 -288 eevents.fltCadStartEvent.INP
w 776 162 100 0 n#53 hwin.hwin#51.in 752 160 848 160 elongouts.AssemblyMode.DOL
w 224 1443 100 0 n#46 ecad8.Pos.VAL 128 1376 128 1440 368 1440 outhier.VAL.p
w 230 1347 100 0 n#9 ecad8.Pos.MESS 128 1344 368 1344 outhier.MESS.p
w -370 1348 100 0 n#8 inhier.ICID.P -592 1280 -512 1280 -512 1344 -192 1344 ecad8.Pos.ICID
w -410 1378 100 0 n#7 inhier.DIR.P -592 1376 -192 1376 ecad8.Pos.DIR
s 880 1664 100 0 Offset records now SCAN Passive rather than Event - SMB
s 2128 1712 140 0 seqFw2.sch
s 304 1280 100 0 Filter 2 Name
s -544 1152 100 0 A: Name of second filter
s -544 1200 100 0 Input attributes:
s -368 1616 100 0 split any lock sets.
s -368 1648 100 0 Note that event scanning is used between the CAD and the SAD to
s -368 1680 100 0 As well as driving the CAD it also updates the specified SAD items.
s 320 -128 150 0 Trigger SIR output when CAR goes IDLE
s 1632 1664 120 0 Set SIRs 'unknown' on START
[cell use]
use outhier 384 663 100 0 focus_c
xform 0 400 704
use seqMechNames 1616 -361 100 0 seqMechNames#137
xform 0 1736 -216
p 1632 -320 100 0 -1 set0:mech fw2
use eaos 800 1383 100 0 Offset1
xform 0 928 1472
p 864 1328 100 0 1 EGU:microns
p 736 1568 100 0 1 EVNT:$(event)
p 864 1360 100 0 1 OMSL:closed_loop
p 864 1296 100 0 1 PREC:2
p 944 1568 100 0 1 SCAN:Passive
p 912 1376 100 1024 0 name:$(top)$(mech)$(I)
p 768 1504 75 1280 -1 pproc(DOL):NPP
use eaos 1184 1383 100 0 lambdaOffset1
xform 0 1312 1472
p 1248 1328 100 0 1 EGU:microns
p 1120 1568 100 0 1 EVNT:$(event)
p 1248 1360 100 0 1 OMSL:closed_loop
p 1248 1296 100 0 1 PREC:2
p 1328 1568 100 0 1 SCAN:Passive
p 1296 1376 100 1024 0 name:$(top)$(mech)$(I)
p 1152 1504 75 1280 -1 pproc(DOL):NPP
use hwout 256 1207 100 0 hwout#126
xform 0 352 1248
p 352 1239 100 0 -1 val(outp):$(cc)fw2PosCad.B
use hwin -608 -105 100 0 hwin#106
xform 0 -512 -64
p -605 -72 100 0 -1 val(in):$(cc)motor1C.VAL
use hwin 584 120 100 0 hwin#51
xform 0 656 160
p 563 152 100 0 -1 val(in):$(MODE_MOVE)
use hwin 400 -281 100 0 hwin#80
xform 0 496 -240
p 403 -248 100 0 -1 val(in):$(event)
use hwin 1320 120 100 0 hwin#123
xform 0 1392 160
p 1299 152 100 0 -1 val(in):$(CAD_MARK)
use elongouts 872 40 100 0 AssemblyMode
xform 0 976 128
p 896 222 100 0 -1 DESC:Set assembly record mode MOVE
p 688 270 100 0 0 EGU:Assembly mode
p 912 0 100 0 1 OMSL:closed_loop
p 1184 94 100 0 -1 def(OUT):$(cc)fltAssembly.MODE
p 1072 32 100 1024 0 name:$(top)$(mech)$(I)
p 1104 96 75 768 -1 pproc(OUT):NPP
use elongouts 1608 40 100 0 MarkDta
xform 0 1712 128
p 1632 222 100 0 -1 DESC:Mark dtaTrack CAD
p 1424 270 100 0 0 EGU:CAD directive
p 1648 0 100 0 1 OMSL:closed_loop
p 1920 94 100 0 -1 def(OUT):$(nirs)dtaTrack.DIR
p 1808 32 100 1024 0 name:$(top)$(mech)$(I)
p 1840 96 75 768 -1 pproc(OUT):PP
use estringouts 1472 407 100 0 Sad1
xform 0 1600 496
p 1252 682 100 0 0 DESC:Output of CAD
p 1440 222 100 0 0 EGU:degs
p 1568 576 100 0 1 EVNT:$(event)
p 1536 368 100 0 1 OMSL:closed_loop
p 1376 576 100 0 1 SCAN:Event
p 1680 432 100 0 -1 def(OUT):$(sad)fw2Name
p 1584 400 100 1024 0 name:$(top)$(mech)$(I)
p 1728 480 75 768 -1 pproc(OUT):PP
use estringouts 1632 1415 100 0 fltSirUnknown1
xform 0 1760 1504
p 1412 1690 100 0 0 DESC:Output of CAD
p 1600 1230 100 0 0 EGU:degs
p 1728 1584 100 0 0 EVNT:0
p 1696 1392 100 0 1 OMSL:closed_loop
p 1712 1520 100 0 1 PINI:YES
p 1680 1568 100 0 1 SCAN:Passive
p 1680 1600 100 0 1 VAL:unknown
p 1840 1440 100 0 -1 def(OUT):$(sad)$(mech)Name
p 1744 1408 100 1024 0 name:$(top)$(mech)$(I)
p 1888 1488 75 768 -1 pproc(OUT):PP
use ewait -416 -537 100 0 fltWaitForIdle
xform 0 -64 -208
p -93 40 100 0 -1 CALC:A
p -32 -160 100 0 1 INAP:Yes
p -288 -384 100 0 1 OOPT:Transition To Zero
p -288 -66 100 0 1 SCAN:I/O Intr
use oslBorderC -720 -745 100 0 oslBorderC#87
xform 0 960 560
p 2220 -496 120 256 -1 Title:NIRS IS - filter commands
use eevents 752 -409 100 0 fltCadStartEvent
xform 0 896 -320
p 832 -448 100 0 1 EVNT:0
p 864 -416 100 1024 0 name:$(top)$(I)
use outhier 360 1400 100 0 VAL
xform 0 352 1440
use outhier 360 1304 100 0 MESS
xform 0 352 1344
use ecad8 -168 424 100 0 Pos
xform 0 -32 928
p -240 1470 100 0 -1 DESC:Triggers a subsystem CAD on START only
p -80 1168 100 0 1 FTVA:STRING
p -80 1104 100 0 1 FTVB:STRING
p -80 1040 100 0 1 FTVC:DOUBLE
p -80 976 100 0 1 FTVD:DOUBLE
p -80 912 100 0 1 FTVE:DOUBLE
p -80 848 100 0 1 FTVF:DOUBLE
p -80 784 100 0 1 FTVG:STRING
p -80 720 100 0 1 FTVH:STRING
p -128 384 100 0 1 INAM:gmSeqCadInitFw2
p -96 768 100 0 0 PREC:4
p -128 320 100 0 1 SNAM:gmSeqCadFw2
p 192 1022 100 0 0 def(OUTC):0.0
p 192 958 100 0 0 def(OUTD):0.0
p 192 894 100 0 0 def(OUTE):0.0
p 192 830 100 0 0 def(OUTF):0.0
p 192 766 100 0 0 def(OUTG):0.0
p 208 702 100 0 0 def(OUTH):0.0
p -128 272 100 0 1 name:$(top)$(mech)$(I)
p 128 1152 75 768 -1 pproc(OUTA):NPP
p 128 1088 75 768 -1 pproc(OUTB):NPP
p 128 1024 75 768 -1 pproc(OUTC):NPP
p 128 960 75 768 -1 pproc(OUTD):NPP
p 128 704 75 768 -1 pproc(OUTH):NPP
use inhier -584 1336 100 0 DIR
xform 0 -592 1376
use inhier -584 1240 100 0 ICID
xform 0 -592 1280
[comments]
