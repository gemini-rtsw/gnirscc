[schematic2]
uniq 145
[tools]
[detail]
w 266 1051 100 0 n#144 estringouts.nameString.OUT 208 976 256 976 256 1040 336 1040 ecad8.deckerPos.A
w -134 1003 100 0 n#143 embbis.Select.FLNK -224 1056 -160 1056 -160 992 -48 992 estringouts.nameString.SLNK
w -166 1035 100 0 n#142 embbis.Select.VAL -224 1024 -48 1024 estringouts.nameString.DOL
w 2034 1403 100 0 n#137 eaos.deckerLambdaOffset.FLNK 2000 1392 2128 1392 estringouts.deckerSirUnknown1.SLNK
w 962 787 100 0 n#134 ecad8.deckerPos.VALE 656 784 1328 784 1328 1040 1664 1040 1664 1392 1744 1392 eaos.deckerLambdaOffset.DOL
w 1634 1363 100 0 n#133 eaos.deckerOffset.FLNK 1584 1360 1744 1360 eaos.deckerLambdaOffset.SLNK
w 724 1051 100 0 n#128 ecad8.deckerPos.OUTA 656 1008 720 1008 720 1104 784 1104 hwout.hwout#126.outp
w 1196 843 100 0 n#118 ecad8.deckerPos.STLK 656 368 1200 368 1200 1328 1328 1328 eaos.deckerOffset.SLNK
w 378 -437 100 0 n#108 ewait.deckerWaitForIdle.FLNK 288 -448 528 -448 528 -320 752 -320 eevents.deckerCadStartEvent.SLNK
w -412 -69 100 2 n#107 hwin.hwin#106.in -416 -64 -416 -64 ewait.deckerWaitForIdle.INAN
w 882 915 100 0 n#100 ecad8.deckerPos.VALC 656 912 1168 912 1168 1360 1328 1360 eaos.deckerOffset.DOL
w 1064 659 100 0 n#96 ecad8.deckerPos.VALG 656 656 1520 656 1520 384 2000 384 estringouts.deckerSad.DOL
w 680 -277 100 0 n#81 hwin.hwin#80.in 592 -240 656 -240 656 -288 752 -288 eevents.deckerCadStartEvent.INP
w 752 1299 100 0 n#46 ecad8.deckerPos.VAL 656 1232 656 1296 896 1296 outhier.VAL.p
w 758 1203 100 0 n#9 ecad8.deckerPos.MESS 656 1200 896 1200 outhier.MESS.p
w 158 1204 100 0 n#8 inhier.ICID.P -64 1136 16 1136 16 1200 336 1200 ecad8.deckerPos.ICID
w 118 1234 100 0 n#7 inhier.DIR.P -64 1232 336 1232 ecad8.deckerPos.DIR
s 1408 1520 100 0 Offset records now SCAN Passive rather than Event - SMB
s 2128 1712 140 0 seqDecker.sch
s 832 1136 100 0 decker name
s -288 1136 100 0 A: Decker Name
s 160 1472 100 0 split any lock sets.
s 160 1504 100 0 Note that event scanning is used between the CAD and the SAD to
s 160 1536 100 0 As well as driving the CAD it also updates the specified SAD items.
s 320 -128 150 0 Trigger SIR output when CAR goes IDLE
s 2128 1536 120 0 Set SIRs 'unknown' on START
[cell use]
use embbis -480 967 100 0 Select
xform 0 -352 1040
p -352 958 100 0 0 FRST:e
p -352 926 100 0 0 FVST:f
p -352 1054 100 0 0 ONST:b
p -352 990 100 0 0 THST:d
p -352 1022 100 0 0 TWST:c
p -352 1086 100 0 0 ZRST:a
p -368 960 100 1024 0 name:$(top)$(mech)$(I)
use estringouts -48 919 100 0 nameString
xform 0 80 992
p -80 864 100 0 1 OMSL:closed_loop
p 64 912 100 1024 0 name:$(top)$(mech)$(I)
use estringouts 2000 263 100 0 deckerSad
xform 0 2128 352
p 1780 538 100 0 0 DESC:Output of CAD
p 1968 78 100 0 0 EGU:degs
p 2096 432 100 0 1 EVNT:$(event)
p 2064 224 100 0 1 OMSL:closed_loop
p 1904 432 100 0 1 SCAN:Event
p 2208 288 100 0 -1 def(OUT):$(sad)deckerName
p 2112 256 100 1024 0 name:$(top)$(I)
p 2256 336 75 768 -1 pproc(OUT):PP
use estringouts 2128 1303 100 0 deckerSirUnknown1
xform 0 2256 1392
p 1908 1578 100 0 0 DESC:Output of CAD
p 2096 1118 100 0 0 EGU:degs
p 2224 1472 100 0 0 EVNT:0
p 2192 1280 100 0 1 OMSL:closed_loop
p 2208 1408 100 0 1 PINI:YES
p 2176 1456 100 0 1 SCAN:Passive
p 2176 1488 100 0 1 VAL:unknown
p 2336 1328 100 0 -1 def(OUT):$(sad)deckerName
p 2240 1296 100 1024 0 name:$(top)$(I)
p 2384 1376 75 768 -1 pproc(OUT):PP
use eaos 1328 1239 100 0 deckerOffset
xform 0 1456 1328
p 1392 1184 100 0 1 EGU:microns
p 1264 1424 100 0 1 EVNT:$(event)
p 1392 1216 100 0 1 OMSL:closed_loop
p 1392 1152 100 0 1 PREC:2
p 1472 1424 100 0 1 SCAN:Passive
p 1296 1360 75 1280 -1 pproc(DOL):NPP
use eaos 1744 1271 100 0 deckerLambdaOffset
xform 0 1872 1360
p 1808 1216 100 0 1 EGU:microns
p 1680 1456 100 0 1 EVNT:$(event)
p 1808 1248 100 0 1 OMSL:closed_loop
p 1808 1184 100 0 1 PREC:2
p 1888 1456 100 0 1 SCAN:Passive
p 1712 1392 75 1280 -1 pproc(DOL):NPP
use hwout 784 1063 100 0 hwout#126
xform 0 880 1104
p 880 1095 100 0 -1 val(outp):$(cc)deckerPosCad.B
use hwin -608 -105 100 0 hwin#106
xform 0 -512 -64
p -605 -72 100 0 -1 val(in):$(cc)deckerC.VAL
use hwin 400 -281 100 0 hwin#80
xform 0 496 -240
p 403 -248 100 0 -1 val(in):$(event)
use ewait -416 -537 100 0 deckerWaitForIdle
xform 0 -64 -208
p -93 40 100 0 -1 CALC:A
p -32 -160 100 0 1 INAP:Yes
p -288 -384 100 0 1 OOPT:Transition To Zero
p -288 -66 100 0 1 SCAN:I/O Intr
use oslBorderC -720 -745 100 0 oslBorderC#87
xform 0 960 560
p 2220 -496 120 256 -1 Title:NIRS IS - filter commands
use eevents 752 -409 100 0 deckerCadStartEvent
xform 0 896 -320
p 832 -448 100 0 1 EVNT:0
p 864 -416 100 1024 0 name:$(top)$(I)
use outhier 888 1256 100 0 VAL
xform 0 880 1296
use outhier 888 1160 100 0 MESS
xform 0 880 1200
use ecad8 360 280 100 0 deckerPos
xform 0 496 784
p 288 1326 100 0 -1 DESC:Triggers a subsystem CAD on START only
p 448 1024 100 0 1 FTVA:STRING
p 448 960 100 0 1 FTVB:STRING
p 448 896 100 0 1 FTVC:DOUBLE
p 448 832 100 0 1 FTVD:DOUBLE
p 448 768 100 0 1 FTVE:DOUBLE
p 448 704 100 0 1 FTVF:DOUBLE
p 448 640 100 0 1 FTVG:STRING
p 448 576 100 0 1 FTVH:STRING
p 400 240 100 0 1 INAM:gmSeqCadInitDecker
p 432 624 100 0 0 PREC:4
p 400 176 100 0 1 SNAM:gmSeqCadDecker
p 720 878 100 0 0 def(OUTC):0.0
p 720 814 100 0 0 def(OUTD):0.0
p 720 750 100 0 0 def(OUTE):0.0
p 720 686 100 0 0 def(OUTF):0.0
p 720 622 100 0 0 def(OUTG):0.0
p 736 558 100 0 0 def(OUTH):0.0
p 400 222 100 0 0 name:$(top)$(I)
p 656 1008 75 768 -1 pproc(OUTA):NPP
p 656 944 75 768 -1 pproc(OUTB):NPP
p 656 880 75 768 -1 pproc(OUTC):NPP
p 656 816 75 768 -1 pproc(OUTD):NPP
p 656 560 75 768 -1 pproc(OUTH):NPP
use inhier -56 1192 100 0 DIR
xform 0 -64 1232
use inhier -56 1096 100 0 ICID
xform 0 -64 1136
[comments]
