[schematic2]
uniq 538
[tools]
[detail]
w -206 707 100 0 n#536 ebis.GISDemand.VAL -416 704 64 704 ecalcs.GISCalcDemand.INPA
w -302 419 100 0 n#534 ebis.GISNDemand.VAL -416 416 -128 416 -128 672 64 672 ecalcs.GISCalcDemand.INPB
w 370 523 100 0 n#531 ebos.GISCombDemand.DOL 448 512 352 512 ecalcs.GISCalcDemand.VAL
w 388 1083 100 0 n#529 ecalcs.GISCalcEvent.FLNK 352 1120 384 1120 384 1056 448 1056 ebos.GISCombEvent.SLNK
w 370 1099 100 0 n#528 ebos.GISCombEvent.DOL 448 1088 352 1088 ecalcs.GISCalcEvent.VAL
w -270 1035 100 0 n#525 ebos.SetGISNEvent.FLNK -416 1024 -64 1024 -64 896 64 896 ecalcs.GISCalcEvent.SLNK
w -270 1323 100 0 n#525 ebos.SetGISEvent.FLNK -416 1312 -64 1312 -64 1024 junction
w -302 995 100 0 n#524 ebos.SetGISNEvent.VAL -416 992 -128 992 -128 1248 64 1248 ecalcs.GISCalcEvent.INPB
w -206 1283 100 0 n#523 ebos.SetGISEvent.VAL -416 1280 64 1280 ecalcs.GISCalcEvent.INPA
w -702 475 100 0 n#451 ebis.GISNDemand.INP -672 464 -672 464 hwin.hwin#450.in
w -702 763 100 0 n#445 hwin.hwin#444.in -672 752 -672 752 ebis.GISDemand.INP
w -446 971 100 0 n#439 hwout.hwout#435.outp -416 960 -416 960 ebos.SetGISNEvent.OUT
w -446 1259 100 0 n#428 ebos.SetGISEvent.OUT -416 1248 -416 1248 hwout.hwout#432.outp
w -446 1547 100 0 n#419 hwout.hwout#418.outp -416 1536 -416 1536 ebos.WinPress.OUT
w -446 1835 100 0 n#417 hwout.hwout#416.outp -416 1824 -416 1824 ebos.WinCover.OUT
s -1536 2144 100 768 This diagram controls the binary input and output
s -1536 2112 100 768 (excluding the motor status and reset
s -1536 2080 100 768 lines).
s 800 1184 100 768 These records impliment the interface to the
s 800 1152 100 768 Gemini interlock system, as described in ICD12.
s 800 1120 100 768 The interlock system is currently unused by
s 800 1088 100 768 NIRI.  These are provided so that they
s 800 1056 100 768 will be available for future use.
[cell use]
use ecalcs 128 1312 100 768 GISCalcEvent
xform 0 208 1072
p 128 800 100 768 1 CALC:A&!B
use ecalcs 128 736 100 768 GISCalcDemand
xform 0 208 496
p 128 224 100 768 1 CALC:A&!B
p 32 704 75 1280 -1 pproc(INPA):PP
p 32 672 75 1280 -1 pproc(INPB):PP
use ebos -608 1056 100 768 SetGISNEvent
xform 0 -544 992
p -608 896 100 768 1 DTYP:$(mtype)
p -608 832 100 768 1 ONAM:Set (1)
p -608 864 100 768 1 ZNAM:Clear (0)
use ebos -608 1344 100 768 SetGISEvent
xform 0 -544 1280
p -608 1184 100 768 1 DTYP:$(mtype)
p -608 1120 100 768 1 ONAM:Clear (1)
p -608 1152 100 768 1 ZNAM:Set (0)
use ebos -608 1920 100 768 WinCover
xform 0 -544 1856
p -608 1760 100 768 1 DTYP:$(mtype)
p -608 1696 100 768 1 ONAM:Off
p -608 1728 100 768 1 ZNAM:On
use ebos -608 1632 100 768 WinPress
xform 0 -544 1568
p -608 1472 100 768 1 DTYP:$(mtype)
p -608 1408 100 768 1 ONAM:Off
p -608 1440 100 768 1 ZNAM:On
use ebos 512 1120 100 768 GISCombEvent
xform 0 576 1056
p 512 896 100 768 1 OMSL:closed_loop
p 512 928 100 768 1 ONAM:Clear
p 512 960 100 768 1 ZNAM:Set
use ebos 512 544 100 768 GISCombDemand
xform 0 576 480
p 512 288 100 768 1 OMSL:closed_loop
p 512 352 100 768 1 ONAM:Clear
p 512 320 100 768 1 SCAN:1 second
p 512 384 100 768 1 ZNAM:Set
p 416 512 75 1280 -1 pproc(DOL):PP
use hwin -864 423 100 0 hwin#450
xform 0 -768 464
p -861 456 100 0 -1 val(in):$(gisnotdemand)
use hwin -864 711 100 0 hwin#444
xform 0 -768 752
p -861 744 100 0 -1 val(in):$(gisdemand)
use ebis -608 480 100 768 GISNDemand
xform 0 -544 432
p -608 352 100 768 1 DTYP:$(mtype)
p -608 256 100 768 1 ONAM:Set (1)
p -608 320 100 768 1 SCAN:1 second
p -608 288 100 768 1 ZNAM:Clear (0)
use ebis -608 768 100 768 GISDemand
xform 0 -544 720
p -608 640 100 768 1 DTYP:$(mtype)
p -608 576 100 768 1 ONAM:Clear (1)
p -608 544 100 768 1 SCAN:1 second
p -608 608 100 768 1 ZNAM:Set (0)
use hwout -416 919 100 0 hwout#435
xform 0 -320 960
p -320 951 100 0 -1 val(outp):$(gisnotevent)
use hwout -416 1207 100 0 hwout#432
xform 0 -320 1248
p -320 1239 100 0 -1 val(outp):$(gisevent)
use hwout -416 1783 100 0 hwout#416
xform 0 -320 1824
p -320 1815 100 0 -1 val(outp):$(wincover)
use hwout -416 1495 100 0 hwout#418
xform 0 -320 1536
p -320 1527 100 0 -1 val(outp):$(winpress)
use eborderC 1600 -160 100 1280 binio.sch
xform 0 16 1024
p 1136 -64 200 1536 -1 file:binio.sch
p 1600 -160 100 1280 -1 id:$Id: binio.sch,v 1.2 2009/05/27 19:34:01 fkraemer Exp $
[comments]
