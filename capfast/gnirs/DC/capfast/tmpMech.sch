[schematic2]
uniq 649
[tools]
[detail]
w -798 667 100 0 n#641 hwin.hwin#640.in -768 656 -768 656 elongins.Busy.INP
w 514 2051 100 0 n#633 hwout.hwout#479.outp 544 2048 544 2048 etcons.C1.OUT
w -190 1187 100 0 n#598 hwout.hwout#593.outp -160 1184 -160 1184 ebos.Timer.OUT
w -606 187 100 0 n#591 embbis.Mode.INP -528 176 -624 176 ecalcs.ModeCalc.VAL
w -1012 267 100 0 n#587 ebis.Normal.VAL -1040 208 -1008 208 -1008 336 -912 336 ecalcs.ModeCalc.INPB
w -1006 379 100 0 n#586 ebis.AWarm.VAL -1040 368 -912 368 ecalcs.ModeCalc.INPA
w -1326 427 100 0 n#565 ebis.AWarm.INP -1296 416 -1296 416 hwin.hwin#564.in
w -1326 267 100 0 n#562 hwin.hwin#561.in -1296 256 -1296 256 ebis.Normal.INP
w 220 1988 100 768 n#638 elutouts.Select.OUTA 128 1888 224 1888 224 2112 288 2112 etcons.C1.SETP
w 1282 2107 100 0 n#468 hwin.hwin#469.in 1312 2096 1312 2096 eais.C1Heat.INP
w 802 2107 100 0 n#415 hwin.hwin#414.in 832 2096 832 2096 eais.C1Tmp.INP
s -480 704 100 768 code expects this
s -480 672 100 768 record to exist.
s -480 736 100 768 The upper-level SNL
s -480 512 100 768 value.
s -480 544 100 768 contain a useful
s -480 576 100 768 record does not
s -480 608 100 768 indicates that this
s -480 640 100 768 A value of -1 
[cell use]
use hwin -960 615 100 0 hwin#640
xform 0 -864 656
p -957 648 100 0 -1 val(in):-1
use hwin -1488 375 100 0 hwin#564
xform 0 -1392 416
p -1485 408 100 0 -1 val(in):$(warm)
use hwin -1488 215 100 0 hwin#561
xform 0 -1392 256
p -1485 248 100 0 -1 val(in):$(norm)
use hwin 640 2055 100 0 hwin#414
xform 0 736 2096
p 643 2088 100 0 -1 val(in):@$(tc1) tmp
use hwin 1120 2055 100 0 hwin#469
xform 0 1216 2096
p 1123 2088 100 0 -1 val(in):@$(tc1) heat
use elongins -704 672 100 768 Busy
xform 0 -640 624
use ebos -352 1280 100 768 Timer
xform 0 -288 1216
p -352 1120 100 768 1 DTYP:$(bio)
p -352 1088 100 768 1 OMSL:closed_loop
p -352 1024 100 768 1 ONAM:Enabled
p -352 1056 100 768 1 ZNAM:Disabled
use hwout 544 2007 100 0 hwout#479
xform 0 640 2048
p 640 2039 100 0 -1 val(outp):@$(tc1)
use hwout -160 1143 100 0 hwout#593
xform 0 -64 1184
p -64 1175 100 0 -1 val(outp):$(timer)
use embbis -464 192 100 768 Mode
xform 0 -400 144
p -464 32 100 768 1 ONST:Acc Warm
p -464 -64 100 768 1 SCAN:1 second
p -464 -32 100 768 1 THST:Missing
p -464 0 100 768 1 TWST:Normal
p -464 64 100 768 1 ZRST:ERROR
p -560 176 75 1280 -1 pproc(INP):PP
use ecalcs -848 400 100 768 ModeCalc
xform 0 -768 160
p -800 112 100 768 -1 CALC:A+2*B
p -944 368 75 1280 -1 pproc(INPA):PP
p -944 336 75 1280 -1 pproc(INPB):PP
use ebis -1232 432 100 768 AWarm
xform 0 -1168 384
p -1232 304 100 768 1 DTYP:$(bio)
use ebis -1232 272 100 768 Normal
xform 0 -1168 224
p -1232 144 100 768 1 DTYP:$(bio)
use elutouts -64 2016 100 768 Select
xform 0 0 1824
p -64 1600 100 768 1 FDIR:./data
p -64 1568 100 768 1 FNAM:tmp.lut
p -64 1536 100 768 1 FTVA:DOUBLE
p -64 1504 100 768 1 FTVB:DOUBLE
p -64 1472 100 768 1 FTVC:DOUBLE
p -64 1440 100 768 1 FTVD:STRING
p 128 1696 75 768 -1 pproc(OUTD):PP
use eais 896 2112 100 768 C1Tmp
xform 0 960 2064
p 896 1952 100 768 1 DTYP:$(tcon)
p 896 1888 100 768 1 HYST:0.05
p 896 1920 100 768 1 PREC:2
p 896 1984 100 768 1 SCAN:10 second
use eais 1376 2112 100 768 C1Heat
xform 0 1440 2064
p 1376 1952 100 768 1 DTYP:$(tcon)
p 1376 1888 100 768 1 HYST:0.05
p 1376 1920 100 768 1 PREC:2
p 1376 1984 100 768 1 SCAN:10 second
use etcons 288 2144 100 768 C1
xform 0 416 2048
p 304 1984 100 768 1 DTYP:$(tcon)
p 288 1920 100 768 1 PREC:2
p 288 1888 100 768 1 SCAN:10 second
use common -1536 -153 100 0 common#400
xform 0 -1376 -16
use eborderC 1600 -160 100 1280 $Id:
xform 0 16 1024
p 1136 -64 200 1536 -1 file:tmp.sch
p 1600 -160 100 1280 -1 id:$Id: tmpMech.sch,v 1.2 2009/05/27 19:32:16 fkraemer Exp $
[comments]
