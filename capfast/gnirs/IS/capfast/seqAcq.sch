[schematic2]
uniq 148
[tools]
[detail]
w 826 619 100 0 focus_c ecad8.acqPos.OUTH 720 608 992 608 outhier.focus_c.p
w 330 1099 100 0 n#146 estringouts.nameString.OUT 256 1024 320 1024 320 1088 400 1088 ecad8.acqPos.A
w -78 1051 100 0 n#145 embbis.Select.FLNK -144 1104 -96 1104 -96 1040 0 1040 estringouts.nameString.SLNK
w -102 1083 100 0 n#144 embbis.Select.VAL -144 1072 0 1072 estringouts.nameString.DOL
w 1026 835 100 0 n#134 ecad8.acqPos.VALE 720 832 1392 832 1392 1088 1744 1088 1744 1408 1824 1408 eaos.acqLambdaOffset.DOL
w 788 1099 100 0 n#128 ecad8.acqPos.OUTA 720 1056 784 1056 784 1152 848 1152 hwout.hwout#126.outp
w 2130 1411 100 0 n#120 eaos.acqLambdaOffset.FLNK 2080 1408 2240 1408 estringouts.acqSirUnknown1.SLNK
w 1722 1379 100 0 n#139 eaos.acqOffset.FLNK 1648 1408 1680 1408 1680 1376 1824 1376 eaos.acqLambdaOffset.SLNK
w 1260 891 100 0 n#118 ecad8.acqPos.STLK 720 416 1264 416 1264 1376 1392 1376 eaos.acqOffset.SLNK
w 378 -437 100 0 n#108 ewait.acqWaitForIdle.FLNK 288 -448 528 -448 528 -320 752 -320 eevents.acqCadStartEvent.SLNK
w -412 -69 100 2 n#107 hwin.hwin#106.in -416 -64 -416 -64 ewait.acqWaitForIdle.INAN
w 946 963 100 0 n#100 ecad8.acqPos.VALC 720 960 1232 960 1232 1408 1392 1408 eaos.acqOffset.DOL
w 1128 707 100 0 n#96 ecad8.acqPos.VALG 720 704 1584 704 1584 432 2064 432 estringouts.acqSad.DOL
w 680 -277 100 0 n#81 hwin.hwin#80.in 592 -240 656 -240 656 -288 752 -288 eevents.acqCadStartEvent.INP
w 816 1347 100 0 n#46 ecad8.acqPos.VAL 720 1280 720 1344 960 1344 outhier.VAL.p
w 822 1251 100 0 n#9 ecad8.acqPos.MESS 720 1248 960 1248 outhier.MESS.p
w 222 1252 100 0 n#8 inhier.ICID.P 0 1184 80 1184 80 1248 400 1248 ecad8.acqPos.ICID
w 182 1282 100 0 n#7 inhier.DIR.P 0 1280 400 1280 ecad8.acqPos.DIR
s 1472 1568 100 0 Offset records now SCAN Passive rather than Event - SMB
s 2128 1712 140 0 seqAcq.sch
s 64 1120 100 0 Acq Position
s 224 1520 100 0 split any lock sets.
s 224 1552 100 0 Note that event scanning is used between the CAD and the SAD to
s 224 1600 100 0 As well as driving the CAD it also updates the specified SAD items.
s 320 -128 150 0 Trigger SIR output when CAR goes IDLE
s 1968 1680 120 0 Set SIRs 'unknown' on START
[cell use]
use outhier 960 567 100 0 focus_c
xform 0 976 608
use embbis -400 1015 100 0 Select
xform 0 -272 1088
p -272 1102 100 0 0 ONST:Out
p -272 1134 100 0 0 ZRST:In
p -288 1008 100 1024 0 name:$(top)$(mech)$(I)
use estringouts 2064 311 100 0 acqSad
xform 0 2192 400
p 1844 586 100 0 0 DESC:Output of CAD
p 2032 126 100 0 0 EGU:degs
p 2160 480 100 0 1 EVNT:$(event)
p 2128 272 100 0 1 OMSL:closed_loop
p 1968 480 100 0 1 SCAN:Event
p 2272 336 100 0 -1 def(OUT):$(sad)acqName
p 2176 304 100 1024 0 name:$(top)$(I)
p 2320 384 75 768 -1 pproc(OUT):PP
use estringouts 2240 1319 100 0 acqSirUnknown1
xform 0 2368 1408
p 2020 1594 100 0 0 DESC:Output of CAD
p 2208 1134 100 0 0 EGU:degs
p 2336 1488 100 0 0 EVNT:0
p 2304 1296 100 0 1 OMSL:closed_loop
p 2320 1424 100 0 1 PINI:YES
p 2288 1472 100 0 1 SCAN:Passive
p 2288 1504 100 0 1 VAL:unknown
p 2448 1344 100 0 -1 def(OUT):$(sad)acqName
p 2352 1312 100 1024 0 name:$(top)$(I)
p 2496 1392 75 768 -1 pproc(OUT):PP
use estringouts 0 967 100 0 nameString
xform 0 128 1040
p -32 912 100 0 1 OMSL:closed_loop
p 112 960 100 1024 0 name:$(top)$(mech)$(I)
use eaos 1392 1287 100 0 acqOffset
xform 0 1520 1376
p 1456 1232 100 0 1 EGU:microns
p 1328 1472 100 0 1 EVNT:$(event)
p 1456 1264 100 0 1 OMSL:closed_loop
p 1456 1200 100 0 1 PREC:2
p 1536 1472 100 0 1 SCAN:Passive
p 1360 1408 75 1280 -1 pproc(DOL):NPP
use eaos 1824 1287 100 0 acqLambdaOffset
xform 0 1952 1376
p 1888 1232 100 0 1 EGU:microns
p 1760 1472 100 0 1 EVNT:$(event)
p 1888 1264 100 0 1 OMSL:closed_loop
p 1888 1200 100 0 1 PREC:2
p 1968 1472 100 0 1 SCAN:Passive
p 1792 1408 75 1280 -1 pproc(DOL):NPP
use hwout 848 1111 100 0 hwout#126
xform 0 944 1152
p 944 1143 100 0 -1 val(outp):$(cc)acqPosCad.B
use hwin -608 -105 100 0 hwin#106
xform 0 -512 -64
p -605 -72 100 0 -1 val(in):$(cc)acqC.VAL
use hwin 400 -281 100 0 hwin#80
xform 0 496 -240
p 403 -248 100 0 -1 val(in):$(event)
use ewait -416 -537 100 0 acqWaitForIdle
xform 0 -64 -208
p -93 40 100 0 -1 CALC:A
p -32 -160 100 0 1 INAP:Yes
p -288 -384 100 0 1 OOPT:Transition To Zero
p -288 -66 100 0 1 SCAN:I/O Intr
use oslBorderC -720 -745 100 0 oslBorderC#87
xform 0 960 560
p 2220 -496 120 256 -1 Title:NIRS IS - filter commands
use eevents 752 -409 100 0 acqCadStartEvent
xform 0 896 -320
p 832 -448 100 0 1 EVNT:0
p 864 -416 100 1024 0 name:$(top)$(I)
use outhier 952 1304 100 0 VAL
xform 0 944 1344
use outhier 952 1208 100 0 MESS
xform 0 944 1248
use ecad8 424 328 100 0 acqPos
xform 0 560 832
p 352 1374 100 0 -1 DESC:Triggers a subsystem CAD on START only
p 512 1072 100 0 1 FTVA:STRING
p 512 1008 100 0 1 FTVB:STRING
p 512 944 100 0 1 FTVC:DOUBLE
p 512 880 100 0 1 FTVD:DOUBLE
p 512 816 100 0 1 FTVE:DOUBLE
p 512 752 100 0 1 FTVF:DOUBLE
p 512 688 100 0 1 FTVG:STRING
p 512 624 100 0 1 FTVH:STRING
p 464 288 100 0 1 INAM:gmSeqCadInitAcq
p 496 672 100 0 0 PREC:4
p 464 224 100 0 1 SNAM:gmSeqCadAcq
p 784 926 100 0 0 def(OUTC):0.0
p 784 862 100 0 0 def(OUTD):0.0
p 784 798 100 0 0 def(OUTE):0.0
p 784 734 100 0 0 def(OUTF):0.0
p 784 670 100 0 0 def(OUTG):0.0
p 800 606 100 0 0 def(OUTH):0.0
p 464 270 100 0 0 name:$(top)$(I)
p 720 1056 75 768 -1 pproc(OUTA):PP
p 720 992 75 768 -1 pproc(OUTB):NPP
p 720 928 75 768 -1 pproc(OUTC):NPP
p 720 864 75 768 -1 pproc(OUTD):NPP
p 720 608 75 768 -1 pproc(OUTH):NPP
use inhier 8 1240 100 0 DIR
xform 0 0 1280
use inhier 8 1144 100 0 ICID
xform 0 0 1184
[comments]
