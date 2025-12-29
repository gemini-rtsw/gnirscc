[schematic2]
uniq 144
[tools]
[detail]
w -124 1115 100 0 n#143 embbis.Select.FLNK -160 1152 -128 1152 -128 1088 -64 1088 estringouts.nameString.SLNK
w -142 1131 100 0 n#142 embbis.Select.VAL -160 1120 -64 1120 estringouts.nameString.DOL
w 218 1083 100 0 n#141 estringouts.nameString.OUT 192 1072 304 1072 ecad8.coverPos.A
w 2002 1435 100 0 n#137 eaos.coverLambdaOffset.FLNK 1968 1424 2096 1424 estringouts.coverSirUnknown1.SLNK
w 930 819 100 0 n#134 ecad8.coverPos.VALE 624 816 1296 816 1296 1072 1632 1072 1632 1424 1712 1424 eaos.coverLambdaOffset.DOL
w 1602 1395 100 0 n#133 eaos.coverOffset.FLNK 1552 1392 1712 1392 eaos.coverLambdaOffset.SLNK
w 692 1083 100 0 n#128 ecad8.coverPos.OUTA 624 1040 688 1040 688 1136 752 1136 hwout.hwout#126.outp
w 1164 875 100 0 n#118 ecad8.coverPos.STLK 624 400 1168 400 1168 1360 1296 1360 eaos.coverOffset.SLNK
w 378 -437 100 0 n#108 ewait.coverWaitForIdle.FLNK 288 -448 528 -448 528 -320 752 -320 eevents.coverCadStartEvent.SLNK
w -412 -69 100 2 n#107 hwin.hwin#106.in -416 -64 -416 -64 ewait.coverWaitForIdle.INAN
w 850 947 100 0 n#100 ecad8.coverPos.VALC 624 944 1136 944 1136 1392 1296 1392 eaos.coverOffset.DOL
w 1032 691 100 0 n#96 ecad8.coverPos.VALG 624 688 1488 688 1488 416 1968 416 estringouts.coverSad.DOL
w 680 -277 100 0 n#81 hwin.hwin#80.in 592 -240 656 -240 656 -288 752 -288 eevents.coverCadStartEvent.INP
w 720 1331 100 0 n#46 ecad8.coverPos.VAL 624 1264 624 1328 864 1328 outhier.VAL.p
w 726 1235 100 0 n#9 ecad8.coverPos.MESS 624 1232 864 1232 outhier.MESS.p
w 126 1236 100 0 n#8 inhier.ICID.P -96 1168 -16 1168 -16 1232 304 1232 ecad8.coverPos.ICID
w 86 1266 100 0 n#7 inhier.DIR.P -96 1264 304 1264 ecad8.coverPos.DIR
s 2096 1568 120 0 Set SIRs 'unknown' on START
s 320 -128 150 0 Trigger SIR output when CAR goes IDLE
s 128 1568 100 0 As well as driving the CAD it also updates the specified SAD items.
s 128 1536 100 0 Note that event scanning is used between the CAD and the SAD to
s 128 1504 100 0 split any lock sets.
s -336 1136 100 0 A: Cover Name
s 800 1168 100 0 cover name
s 2128 1712 140 0 seqCover.sch
s 1376 1552 100 0 Offset records now SCAN Passive rather than Event - SMB
[cell use]
use embbis -416 1063 100 0 Select
xform 0 -288 1136
p -288 1150 100 0 0 ONST:Open
p -288 1182 100 0 0 ZRST:Closed
p -304 1056 100 1024 0 name:$(top)$(mech)$(I)
use estringouts 2096 1335 100 0 coverSirUnknown1
xform 0 2224 1424
p 1876 1610 100 0 0 DESC:Output of CAD
p 2064 1150 100 0 0 EGU:degs
p 2192 1504 100 0 0 EVNT:0
p 2160 1312 100 0 1 OMSL:closed_loop
p 2176 1440 100 0 1 PINI:YES
p 2144 1488 100 0 1 SCAN:Passive
p 2144 1520 100 0 1 VAL:unknown
p 2304 1360 100 0 -1 def(OUT):$(sad)coverName
p 2208 1328 100 1024 0 name:$(top)$(I)
p 2352 1408 75 768 -1 pproc(OUT):PP
use estringouts 1968 295 100 0 coverSad
xform 0 2096 384
p 1748 570 100 0 0 DESC:Output of CAD
p 1936 110 100 0 0 EGU:degs
p 2064 464 100 0 1 EVNT:$(event)
p 2032 256 100 0 1 OMSL:closed_loop
p 1872 464 100 0 1 SCAN:Event
p 2176 320 100 0 -1 def(OUT):$(sad)coverName
p 2080 288 100 1024 0 name:$(top)$(I)
p 2224 368 75 768 -1 pproc(OUT):PP
use estringouts -64 1015 100 0 nameString
xform 0 64 1088
p -96 976 100 0 1 OMSL:closed_loop
p 48 1008 100 1024 0 name:$(top)$(mech)$(I)
use eaos 1712 1303 100 0 coverLambdaOffset
xform 0 1840 1392
p 1776 1248 100 0 1 EGU:microns
p 1648 1488 100 0 1 EVNT:$(event)
p 1776 1280 100 0 1 OMSL:closed_loop
p 1776 1216 100 0 1 PREC:2
p 1856 1488 100 0 1 SCAN:Passive
p 1680 1424 75 1280 -1 pproc(DOL):NPP
use eaos 1296 1271 100 0 coverOffset
xform 0 1424 1360
p 1360 1216 100 0 1 EGU:microns
p 1232 1456 100 0 1 EVNT:$(event)
p 1360 1248 100 0 1 OMSL:closed_loop
p 1360 1184 100 0 1 PREC:2
p 1440 1456 100 0 1 SCAN:Passive
p 1264 1392 75 1280 -1 pproc(DOL):NPP
use hwout 752 1095 100 0 hwout#126
xform 0 848 1136
p 848 1127 100 0 -1 val(outp):$(cc)coverPosCad.B
use hwin 400 -281 100 0 hwin#80
xform 0 496 -240
p 403 -248 100 0 -1 val(in):$(event)
use hwin -608 -105 100 0 hwin#106
xform 0 -512 -64
p -605 -72 100 0 -1 val(in):$(cc)coverC.VAL
use ewait -416 -537 100 0 coverWaitForIdle
xform 0 -64 -208
p -93 40 100 0 -1 CALC:A
p -32 -160 100 0 1 INAP:Yes
p -288 -384 100 0 1 OOPT:Transition To Zero
p -288 -66 100 0 1 SCAN:I/O Intr
use oslBorderC -720 -745 100 0 oslBorderC#87
xform 0 960 560
p 2220 -496 120 256 -1 Title:NIRS IS - filter commands
use eevents 752 -409 100 0 coverCadStartEvent
xform 0 896 -320
p 832 -448 100 0 1 EVNT:0
p 864 -416 100 1024 0 name:$(top)$(I)
use outhier 856 1192 100 0 MESS
xform 0 848 1232
use outhier 856 1288 100 0 VAL
xform 0 848 1328
use ecad8 328 312 100 0 coverPos
xform 0 464 816
p 256 1358 100 0 -1 DESC:Triggers a subsystem CAD on START only
p 416 1056 100 0 1 FTVA:STRING
p 416 992 100 0 1 FTVB:STRING
p 416 928 100 0 1 FTVC:DOUBLE
p 416 864 100 0 1 FTVD:DOUBLE
p 416 800 100 0 1 FTVE:DOUBLE
p 416 736 100 0 1 FTVF:DOUBLE
p 416 672 100 0 1 FTVG:STRING
p 416 608 100 0 1 FTVH:STRING
p 368 272 100 0 1 INAM:gmSeqCadInitCover
p 400 656 100 0 0 PREC:4
p 368 208 100 0 1 SNAM:gmSeqCadCover
p 688 910 100 0 0 def(OUTC):0.0
p 688 846 100 0 0 def(OUTD):0.0
p 688 782 100 0 0 def(OUTE):0.0
p 688 718 100 0 0 def(OUTF):0.0
p 688 654 100 0 0 def(OUTG):0.0
p 704 590 100 0 0 def(OUTH):0.0
p 368 254 100 0 0 name:$(top)$(I)
p 624 1040 75 768 -1 pproc(OUTA):PP
p 624 976 75 768 -1 pproc(OUTB):NPP
p 624 912 75 768 -1 pproc(OUTC):NPP
p 624 848 75 768 -1 pproc(OUTD):NPP
p 624 592 75 768 -1 pproc(OUTH):NPP
use inhier -88 1128 100 0 ICID
xform 0 -96 1168
use inhier -88 1224 100 0 DIR
xform 0 -96 1264
[comments]
