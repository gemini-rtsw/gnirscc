[schematic2]
uniq 149
[tools]
[detail]
w 690 587 100 0 focus_c ecad8.cameraPos.OUTH 608 576 832 576 outhier.focus_c.p
w 610 1035 100 0 n#146 ecad8.cameraPos.OUTA 608 1024 672 1024 672 1120 736 1120 hwout.hwout#126.outp
w 210 1067 100 0 n#144 estringouts.nameString.OUT 160 992 192 992 192 1056 288 1056 ecad8.cameraPos.A
w -174 1019 100 0 n#143 embbis.Select.FLNK -224 1072 -192 1072 -192 1008 -96 1008 estringouts.nameString.SLNK
w -190 1051 100 0 n#142 embbis.Select.VAL -224 1040 -96 1040 estringouts.nameString.DOL
w 1986 1419 100 0 n#137 eaos.cameraLambdaOffset.FLNK 1952 1408 2080 1408 estringouts.cameraSirUnknown1.SLNK
w 914 803 100 0 n#134 ecad8.cameraPos.VALE 608 800 1280 800 1280 1056 1616 1056 1616 1408 1696 1408 eaos.cameraLambdaOffset.DOL
w 1586 1379 100 0 n#133 eaos.cameraOffset.FLNK 1536 1376 1696 1376 eaos.cameraLambdaOffset.SLNK
w 1148 859 100 0 n#118 ecad8.cameraPos.STLK 608 384 1152 384 1152 1344 1280 1344 eaos.cameraOffset.SLNK
w 378 -437 100 0 n#108 ewait.cameraWaitForIdle.FLNK 288 -448 528 -448 528 -320 752 -320 eevents.cameraCadStartEvent.SLNK
w -412 -69 100 2 n#107 hwin.hwin#106.in -416 -64 -416 -64 ewait.cameraWaitForIdle.INAN
w 834 931 100 0 n#100 ecad8.cameraPos.VALC 608 928 1120 928 1120 1376 1280 1376 eaos.cameraOffset.DOL
w 1016 675 100 0 n#96 ecad8.cameraPos.VALG 608 672 1472 672 1472 400 1952 400 estringouts.cameraSad.DOL
w 680 -277 100 0 n#81 hwin.hwin#80.in 592 -240 656 -240 656 -288 752 -288 eevents.cameraCadStartEvent.INP
w 704 1315 100 0 n#46 ecad8.cameraPos.VAL 608 1248 608 1312 848 1312 outhier.VAL.p
w 710 1219 100 0 n#9 ecad8.cameraPos.MESS 608 1216 848 1216 outhier.MESS.p
w 110 1220 100 0 n#8 inhier.ICID.P -112 1152 -32 1152 -32 1216 288 1216 ecad8.cameraPos.ICID
w 70 1250 100 0 n#7 inhier.DIR.P -112 1248 288 1248 ecad8.cameraPos.DIR
s 2080 1552 120 0 Set SIRs 'unknown' on START
s 320 -128 150 0 Trigger SIR output when CAR goes IDLE
s 112 1552 100 0 As well as driving the CAD it also updates the specified SAD items.
s 112 1520 100 0 Note that event scanning is used between the CAD and the SAD to
s 112 1488 100 0 split any lock sets.
s -448 1152 100 0 A: Camera Name
s 784 1152 100 0 camera name
s 2128 1712 140 0 seqCamera.sch
s 1360 1536 100 0 Offset records now SCAN Passive rather than Event - SMB
s 816 1056 100 0 camera name
[cell use]
use outhier 800 535 100 0 focus_c
xform 0 816 576
use hwout 736 1079 100 0 hwout#126
xform 0 832 1120
p 832 1111 100 0 -1 val(outp):$(cc)cameraPosCad.B
use hwout 768 983 100 0 hwout#145
xform 0 864 1024
p 832 976 100 0 -1 val(outp):nirs:dc:selectWcs.A
use embbis -480 983 100 0 Select
xform 0 -352 1056
p -352 1070 100 0 0 ONST:Long_Red
p -352 1006 100 0 0 THST:Long_Blue
p -352 1038 100 0 0 TWST:Short_Blue
p -352 1102 100 0 0 ZRST:Short_Red
p -368 976 100 1024 0 name:$(top)$(mech)$(I)
use estringouts -96 935 100 0 nameString
xform 0 32 1008
p -112 880 100 0 1 OMSL:closed_loop
p 16 928 100 1024 0 name:$(top)$(mech)$(I)
use estringouts 2080 1319 100 0 cameraSirUnknown1
xform 0 2208 1408
p 1860 1594 100 0 0 DESC:Output of CAD
p 2048 1134 100 0 0 EGU:degs
p 2176 1488 100 0 0 EVNT:0
p 2144 1296 100 0 1 OMSL:closed_loop
p 2160 1424 100 0 1 PINI:YES
p 2128 1472 100 0 1 SCAN:Passive
p 2128 1504 100 0 1 VAL:unknown
p 2288 1344 100 0 -1 def(OUT):$(sad)cameraName
p 2192 1312 100 1024 0 name:$(top)$(I)
p 2336 1392 75 768 -1 pproc(OUT):PP
use estringouts 1952 279 100 0 cameraSad
xform 0 2080 368
p 1732 554 100 0 0 DESC:Output of CAD
p 1920 94 100 0 0 EGU:degs
p 2048 448 100 0 1 EVNT:$(event)
p 2016 240 100 0 1 OMSL:closed_loop
p 1856 448 100 0 1 SCAN:Event
p 2160 304 100 0 -1 def(OUT):$(sad)cameraName
p 2064 272 100 1024 0 name:$(top)$(I)
p 2208 352 75 768 -1 pproc(OUT):PP
use eaos 1696 1287 100 0 cameraLambdaOffset
xform 0 1824 1376
p 1760 1232 100 0 1 EGU:microns
p 1632 1472 100 0 1 EVNT:$(event)
p 1760 1264 100 0 1 OMSL:closed_loop
p 1760 1200 100 0 1 PREC:2
p 1840 1472 100 0 1 SCAN:Passive
p 1664 1408 75 1280 -1 pproc(DOL):NPP
use eaos 1280 1255 100 0 cameraOffset
xform 0 1408 1344
p 1344 1200 100 0 1 EGU:microns
p 1216 1440 100 0 1 EVNT:$(event)
p 1344 1232 100 0 1 OMSL:closed_loop
p 1344 1168 100 0 1 PREC:2
p 1424 1440 100 0 1 SCAN:Passive
p 1248 1376 75 1280 -1 pproc(DOL):NPP
use hwin 400 -281 100 0 hwin#80
xform 0 496 -240
p 403 -248 100 0 -1 val(in):$(event)
use hwin -608 -105 100 0 hwin#106
xform 0 -512 -64
p -605 -72 100 0 -1 val(in):$(cc)cameraC.VAL
use ewait -416 -537 100 0 cameraWaitForIdle
xform 0 -64 -208
p -93 40 100 0 -1 CALC:A
p -32 -160 100 0 1 INAP:Yes
p -288 -384 100 0 1 OOPT:Transition To Zero
p -288 -66 100 0 1 SCAN:I/O Intr
use oslBorderC -720 -745 100 0 oslBorderC#87
xform 0 960 560
p 2220 -496 120 256 -1 Title:NIRS IS - filter commands
use eevents 752 -409 100 0 cameraCadStartEvent
xform 0 896 -320
p 832 -448 100 0 1 EVNT:0
p 864 -416 100 1024 0 name:$(top)$(I)
use outhier 840 1176 100 0 MESS
xform 0 832 1216
use outhier 840 1272 100 0 VAL
xform 0 832 1312
use ecad8 312 296 100 0 cameraPos
xform 0 448 800
p 240 1342 100 0 -1 DESC:Triggers a subsystem CAD on START only
p 400 1040 100 0 1 FTVA:STRING
p 400 976 100 0 1 FTVB:STRING
p 400 912 100 0 1 FTVC:DOUBLE
p 400 848 100 0 1 FTVD:DOUBLE
p 400 784 100 0 1 FTVE:DOUBLE
p 400 720 100 0 1 FTVF:DOUBLE
p 400 656 100 0 1 FTVG:STRING
p 400 592 100 0 1 FTVH:STRING
p 352 256 100 0 1 INAM:gmSeqCadInitCamera
p 384 640 100 0 0 PREC:4
p 352 192 100 0 1 SNAM:gmSeqCadCamera
p 672 894 100 0 0 def(OUTC):0.0
p 672 830 100 0 0 def(OUTD):0.0
p 672 766 100 0 0 def(OUTE):0.0
p 672 702 100 0 0 def(OUTF):0.0
p 672 638 100 0 0 def(OUTG):0.0
p 688 574 100 0 0 def(OUTH):0.0
p 352 238 100 0 0 name:$(top)$(I)
p 608 458 75 0 -1 pproc(CLNK):.PP
p 608 1024 75 768 -1 pproc(OUTA):NPP
p 608 960 75 768 -1 pproc(OUTB):NPP
p 608 896 75 768 -1 pproc(OUTC):NPP
p 608 832 75 768 -1 pproc(OUTD):NPP
p 608 576 75 768 -1 pproc(OUTH):NPP
use inhier -104 1112 100 0 ICID
xform 0 -112 1152
use inhier -104 1208 100 0 DIR
xform 0 -112 1248
[comments]
