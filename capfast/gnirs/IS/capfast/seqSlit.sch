[schematic2]
uniq 148
[tools]
[detail]
w 330 1067 100 0 n#147 estringouts.nameString.OUT 256 992 320 992 320 1056 400 1056 ecad8.slitPos.A
w -28 1035 100 0 n#146 embbis.Select.FLNK -80 1072 -32 1072 -32 1008 0 1008 estringouts.nameString.SLNK
w -70 1051 100 0 n#144 embbis.Select.VAL -80 1040 0 1040 estringouts.nameString.DOL
w 1026 803 100 0 n#134 ecad8.slitPos.VALE 720 800 1392 800 1392 1056 1744 1056 1744 1376 1824 1376 eaos.slitLambdaOffset.DOL
w 788 1067 100 0 n#128 ecad8.slitPos.OUTA 720 1024 784 1024 784 1120 848 1120 hwout.hwout#126.outp
w 2130 1379 100 0 n#120 eaos.slitLambdaOffset.FLNK 2080 1376 2240 1376 estringouts.slitSirUnknown1.SLNK
w 1722 1347 100 0 n#139 eaos.slitOffset.FLNK 1648 1376 1680 1376 1680 1344 1824 1344 eaos.slitLambdaOffset.SLNK
w 1260 859 100 0 n#118 ecad8.slitPos.STLK 720 384 1264 384 1264 1344 1392 1344 eaos.slitOffset.SLNK
w 378 -437 100 0 n#108 ewait.slitWaitForIdle.FLNK 288 -448 528 -448 528 -320 752 -320 eevents.slitCadStartEvent.SLNK
w -412 -69 100 2 n#107 hwin.hwin#106.in -416 -64 -416 -64 ewait.slitWaitForIdle.INAN
w 946 931 100 0 n#100 ecad8.slitPos.VALC 720 928 1232 928 1232 1376 1392 1376 eaos.slitOffset.DOL
w 1128 675 100 0 n#96 ecad8.slitPos.VALG 720 672 1584 672 1584 400 2064 400 estringouts.slitSad.DOL
w 680 -277 100 0 n#81 hwin.hwin#80.in 592 -240 656 -240 656 -288 752 -288 eevents.slitCadStartEvent.INP
w 816 1315 100 0 n#46 ecad8.slitPos.VAL 720 1248 720 1312 960 1312 outhier.VAL.p
w 822 1219 100 0 n#9 ecad8.slitPos.MESS 720 1216 960 1216 outhier.MESS.p
w 222 1220 100 0 n#8 inhier.ICID.P 0 1152 80 1152 80 1216 400 1216 ecad8.slitPos.ICID
w 182 1250 100 0 n#7 inhier.DIR.P 0 1248 400 1248 ecad8.slitPos.DIR
s 1472 1536 100 0 Offset records now SCAN Passive rather than Event - SMB
s 2256 1712 140 0 seqSlit.sch
s -96 1312 100 0 Slit Position
s -96 1360 100 0 Input attributes:
s 224 1488 100 0 split any lock sets.
s 224 1520 100 0 Note that event scanning is used between the CAD and the SAD to
s 224 1552 100 0 As well as driving the CAD it also updates the specified SAD items.
s 320 -128 150 0 Trigger SIR output when CAR goes IDLE
s 2096 1680 120 0 Set SIRs 'unknown' on START
[cell use]
use embbis -336 983 100 0 Select
xform 0 -208 1056
p -208 1070 100 0 0 ONST:Low_res_IFU
p -208 1038 100 0 0 TWST:Pupil_Viewer
p -208 1102 100 0 0 ZRST:60u_slit
p -224 976 100 1024 0 name:$(top)$(mech)$(I)
use estringouts 0 935 100 0 nameString
xform 0 128 1008
p -48 864 100 0 1 OMSL:closed_loop
p 112 928 100 1024 0 name:$(top)$(mech)$(I)
use estringouts 2064 279 100 0 slitSad
xform 0 2192 368
p 1844 554 100 0 0 DESC:Output of CAD
p 2032 94 100 0 0 EGU:degs
p 2160 448 100 0 1 EVNT:$(event)
p 2128 240 100 0 1 OMSL:closed_loop
p 1968 448 100 0 1 SCAN:Event
p 2272 304 100 0 -1 def(OUT):$(sad)slitName
p 2176 272 100 1024 0 name:$(top)$(I)
p 2320 352 75 768 -1 pproc(OUT):PP
use estringouts 2240 1287 100 0 slitSirUnknown1
xform 0 2368 1376
p 2020 1562 100 0 0 DESC:Output of CAD
p 2208 1102 100 0 0 EGU:degs
p 2336 1456 100 0 0 EVNT:0
p 2304 1264 100 0 1 OMSL:closed_loop
p 2320 1392 100 0 1 PINI:YES
p 2288 1440 100 0 1 SCAN:Passive
p 2288 1472 100 0 1 VAL:unknown
p 2448 1312 100 0 -1 def(OUT):$(sad)slitName
p 2352 1280 100 1024 0 name:$(top)$(I)
p 2496 1360 75 768 -1 pproc(OUT):PP
use eaos 1392 1255 100 0 slitOffset
xform 0 1520 1344
p 1456 1200 100 0 1 EGU:microns
p 1328 1440 100 0 1 EVNT:$(event)
p 1456 1232 100 0 1 OMSL:closed_loop
p 1456 1168 100 0 1 PREC:2
p 1536 1440 100 0 1 SCAN:Passive
p 1360 1376 75 1280 -1 pproc(DOL):NPP
use eaos 1824 1255 100 0 slitLambdaOffset
xform 0 1952 1344
p 1888 1200 100 0 1 EGU:microns
p 1760 1440 100 0 1 EVNT:$(event)
p 1888 1232 100 0 1 OMSL:closed_loop
p 1888 1168 100 0 1 PREC:2
p 1968 1440 100 0 1 SCAN:Passive
p 1792 1376 75 1280 -1 pproc(DOL):NPP
use hwout 848 1079 100 0 hwout#126
xform 0 944 1120
p 944 1111 100 0 -1 val(outp):$(cc)slitPosCad.B
use hwin -608 -105 100 0 hwin#106
xform 0 -512 -64
p -605 -72 100 0 -1 val(in):$(cc)slitC.VAL
use hwin 400 -281 100 0 hwin#80
xform 0 496 -240
p 403 -248 100 0 -1 val(in):$(event)
use ewait -416 -537 100 0 slitWaitForIdle
xform 0 -64 -208
p -93 40 100 0 -1 CALC:A
p -32 -160 100 0 1 INAP:Yes
p -288 -384 100 0 1 OOPT:Transition To Zero
p -288 -66 100 0 1 SCAN:I/O Intr
use oslBorderC -720 -745 100 0 oslBorderC#87
xform 0 960 560
p 2220 -496 120 256 -1 Title:NIRS IS - filter commands
use eevents 752 -409 100 0 slitCadStartEvent
xform 0 896 -320
p 832 -448 100 0 1 EVNT:0
p 864 -416 100 1024 0 name:$(top)$(I)
use outhier 952 1272 100 0 VAL
xform 0 944 1312
use outhier 952 1176 100 0 MESS
xform 0 944 1216
use ecad8 424 296 100 0 slitPos
xform 0 560 800
p 352 1342 100 0 -1 DESC:Triggers a subsystem CAD on START only
p 512 1040 100 0 1 FTVA:STRING
p 512 976 100 0 1 FTVB:STRING
p 512 912 100 0 1 FTVC:DOUBLE
p 512 848 100 0 1 FTVD:DOUBLE
p 512 784 100 0 1 FTVE:DOUBLE
p 512 720 100 0 1 FTVF:DOUBLE
p 512 656 100 0 1 FTVG:STRING
p 512 592 100 0 1 FTVH:STRING
p 464 256 100 0 1 INAM:seqCadInitSlit
p 496 640 100 0 0 PREC:4
p 464 192 100 0 1 SNAM:seqCadSlit
p 784 894 100 0 0 def(OUTC):0.0
p 784 830 100 0 0 def(OUTD):0.0
p 784 766 100 0 0 def(OUTE):0.0
p 784 702 100 0 0 def(OUTF):0.0
p 784 638 100 0 0 def(OUTG):0.0
p 800 574 100 0 0 def(OUTH):0.0
p 464 238 100 0 0 name:$(top)$(I)
p 720 1024 75 768 -1 pproc(OUTA):PP
p 720 960 75 768 -1 pproc(OUTB):NPP
p 720 896 75 768 -1 pproc(OUTC):NPP
p 720 832 75 768 -1 pproc(OUTD):NPP
p 720 576 75 768 -1 pproc(OUTH):NPP
use inhier 8 1208 100 0 DIR
xform 0 0 1248
use inhier 8 1112 100 0 ICID
xform 0 0 1152
[comments]
