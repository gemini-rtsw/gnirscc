[schematic2]
uniq 146
[tools]
[detail]
w 714 539 100 0 focus_c ecad8.xdispPos.OUTH 624 528 864 528 outhier.focus_c.p
w 154 955 100 0 n#144 estringouts.nameString.OUT 144 944 224 944 224 1008 304 1008 ecad8.xdispPos.A
w -182 971 100 0 n#143 embbis.Select.FLNK -256 1024 -192 1024 -192 960 -112 960 estringouts.nameString.SLNK
w -214 1003 100 0 n#142 embbis.Select.VAL -256 992 -112 992 estringouts.nameString.DOL
w 2002 1371 100 0 n#137 eaos.xdispLambdaOffset.FLNK 1968 1360 2096 1360 estringouts.xdispSirUnknown1.SLNK
w 930 755 100 0 n#134 ecad8.xdispPos.VALE 624 752 1296 752 1296 1008 1632 1008 1632 1360 1712 1360 eaos.xdispLambdaOffset.DOL
w 1602 1331 100 0 n#133 eaos.xdispOffset.FLNK 1552 1328 1712 1328 eaos.xdispLambdaOffset.SLNK
w 692 1019 100 0 n#128 ecad8.xdispPos.OUTA 624 976 688 976 688 1072 752 1072 hwout.hwout#126.outp
w 1164 811 100 0 n#118 ecad8.xdispPos.STLK 624 336 1168 336 1168 1296 1296 1296 eaos.xdispOffset.SLNK
w 378 -437 100 0 n#108 ewait.xdispWaitForIdle.FLNK 288 -448 528 -448 528 -320 752 -320 eevents.xdispCadStartEvent.SLNK
w -412 -69 100 2 n#107 hwin.hwin#106.in -416 -64 -416 -64 ewait.xdispWaitForIdle.INAN
w 850 883 100 0 n#100 ecad8.xdispPos.VALC 624 880 1136 880 1136 1328 1296 1328 eaos.xdispOffset.DOL
w 1032 627 100 0 n#96 ecad8.xdispPos.VALG 624 624 1488 624 1488 352 1968 352 estringouts.xdispSad.DOL
w 680 -277 100 0 n#81 hwin.hwin#80.in 592 -240 656 -240 656 -288 752 -288 eevents.xdispCadStartEvent.INP
w 720 1267 100 0 n#46 ecad8.xdispPos.VAL 624 1200 624 1264 864 1264 outhier.VAL.p
w 726 1171 100 0 n#9 ecad8.xdispPos.MESS 624 1168 864 1168 outhier.MESS.p
w 126 1172 100 0 n#8 inhier.ICID.P -96 1104 -16 1104 -16 1168 304 1168 ecad8.xdispPos.ICID
w 86 1202 100 0 n#7 inhier.DIR.P -96 1200 304 1200 ecad8.xdispPos.DIR
s 2096 1504 120 0 Set SIRs 'unknown' on START
s 320 -128 150 0 Trigger SIR output when CAR goes IDLE
s 128 1504 100 0 As well as driving the CAD it also updates the specified SAD items.
s 128 1472 100 0 Note that event scanning is used between the CAD and the SAD to
s 128 1440 100 0 split any lock sets.
s 800 1104 100 0 xdisp name
s 2128 1712 140 0 seqXDisp.sch
s 1376 1488 100 0 Offset records now SCAN Passive rather than Event - SMB
[cell use]
use outhier 832 487 100 0 focus_c
xform 0 848 528
use embbis -512 935 100 0 Select
xform 0 -384 1008
p -384 926 100 0 0 FRST:ea
p -384 894 100 0 0 FVST:fa
p -384 1022 100 0 0 ONST:ba
p -384 958 100 0 0 THST:da
p -384 990 100 0 0 TWST:ca
p -384 1054 100 0 0 ZRST:aa
p -400 928 100 1024 0 name:$(top)$(mech)$(I)
use estringouts 2096 1271 100 0 xdispSirUnknown1
xform 0 2224 1360
p 1876 1546 100 0 0 DESC:Output of CAD
p 2064 1086 100 0 0 EGU:degs
p 2192 1440 100 0 0 EVNT:0
p 2160 1248 100 0 1 OMSL:closed_loop
p 2176 1376 100 0 1 PINI:YES
p 2144 1424 100 0 1 SCAN:Passive
p 2144 1456 100 0 1 VAL:unknown
p 2304 1296 100 0 -1 def(OUT):$(sad)xdispName
p 2208 1264 100 1024 0 name:$(top)$(I)
p 2352 1344 75 768 -1 pproc(OUT):PP
use estringouts 1968 231 100 0 xdispSad
xform 0 2096 320
p 1748 506 100 0 0 DESC:Output of CAD
p 1936 46 100 0 0 EGU:degs
p 2064 400 100 0 1 EVNT:$(event)
p 2032 192 100 0 1 OMSL:closed_loop
p 1872 400 100 0 1 SCAN:Event
p 2176 256 100 0 -1 def(OUT):$(sad)xdispName
p 2080 224 100 1024 0 name:$(top)$(I)
p 2224 304 75 768 -1 pproc(OUT):PP
use estringouts -112 887 100 0 nameString
xform 0 16 960
p -144 848 100 0 1 OMSL:closed_loop
p 0 880 100 1024 0 name:$(top)$(mech)$(I)
use eaos 1712 1239 100 0 xdispLambdaOffset
xform 0 1840 1328
p 1776 1184 100 0 1 EGU:microns
p 1648 1424 100 0 1 EVNT:$(event)
p 1776 1216 100 0 1 OMSL:closed_loop
p 1776 1152 100 0 1 PREC:2
p 1856 1424 100 0 1 SCAN:Passive
p 1680 1360 75 1280 -1 pproc(DOL):NPP
use eaos 1296 1207 100 0 xdispOffset
xform 0 1424 1296
p 1360 1152 100 0 1 EGU:microns
p 1232 1392 100 0 1 EVNT:$(event)
p 1360 1184 100 0 1 OMSL:closed_loop
p 1360 1120 100 0 1 PREC:2
p 1440 1392 100 0 1 SCAN:Passive
p 1264 1328 75 1280 -1 pproc(DOL):NPP
use hwout 752 1031 100 0 hwout#126
xform 0 848 1072
p 848 1063 100 0 -1 val(outp):$(cc)xdispPosCad.B
use hwin 400 -281 100 0 hwin#80
xform 0 496 -240
p 403 -248 100 0 -1 val(in):$(event)
use hwin -608 -105 100 0 hwin#106
xform 0 -512 -64
p -605 -72 100 0 -1 val(in):$(cc)xdispC.VAL
use ewait -416 -537 100 0 xdispWaitForIdle
xform 0 -64 -208
p -93 40 100 0 -1 CALC:A
p -32 -160 100 0 1 INAP:Yes
p -288 -384 100 0 1 OOPT:Transition To Zero
p -288 -66 100 0 1 SCAN:I/O Intr
use oslBorderC -720 -745 100 0 oslBorderC#87
xform 0 960 560
p 2220 -496 120 256 -1 Title:NIRS IS - filter commands
use eevents 752 -409 100 0 xdispCadStartEvent
xform 0 896 -320
p 832 -448 100 0 1 EVNT:0
p 864 -416 100 1024 0 name:$(top)$(I)
use outhier 856 1128 100 0 MESS
xform 0 848 1168
use outhier 856 1224 100 0 VAL
xform 0 848 1264
use ecad8 328 248 100 0 xdispPos
xform 0 464 752
p 256 1294 100 0 -1 DESC:Triggers a subsystem CAD on START only
p 416 992 100 0 1 FTVA:STRING
p 416 928 100 0 1 FTVB:STRING
p 416 864 100 0 1 FTVC:DOUBLE
p 416 800 100 0 1 FTVD:DOUBLE
p 416 736 100 0 1 FTVE:DOUBLE
p 416 672 100 0 1 FTVF:DOUBLE
p 416 608 100 0 1 FTVG:STRING
p 416 544 100 0 1 FTVH:STRING
p 368 208 100 0 1 INAM:gmSeqCadInitXDisp
p 400 592 100 0 0 PREC:4
p 368 144 100 0 1 SNAM:gmSeqCadXDisp
p 688 846 100 0 0 def(OUTC):0.0
p 688 782 100 0 0 def(OUTD):0.0
p 688 718 100 0 0 def(OUTE):0.0
p 688 654 100 0 0 def(OUTF):0.0
p 688 590 100 0 0 def(OUTG):0.0
p 704 526 100 0 0 def(OUTH):0.0
p 368 190 100 0 0 name:$(top)$(I)
p 624 976 75 768 -1 pproc(OUTA):NPP
p 624 912 75 768 -1 pproc(OUTB):NPP
p 624 848 75 768 -1 pproc(OUTC):NPP
p 624 784 75 768 -1 pproc(OUTD):NPP
p 624 528 75 768 -1 pproc(OUTH):NPP
use inhier -88 1064 100 0 ICID
xform 0 -96 1104
use inhier -88 1160 100 0 DIR
xform 0 -96 1200
[comments]
