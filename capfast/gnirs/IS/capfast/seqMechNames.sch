[schematic2]
uniq 56
[tools]
[detail]
w 168 1131 100 0 n#53 hwin.hwin#52.in 192 1120 192 1120 egenSubB.getNames.INPN
w 168 1259 100 0 n#48 hwin.hwin#45.in 192 1248 192 1248 egenSubB.getNames.INPJ
w 168 1195 100 0 n#43 hwin.hwin#42.in 192 1184 192 1184 egenSubB.getNames.INPL
w 2000 483 100 0 n#34 embbi.Select.VAL 1952 480 2096 480 estringouts.NameOut.DOL
w 2012 555 100 0 n#38 embbi.Select.FLNK 1952 672 2016 672 2016 448 2096 448 estringouts.NameOut.SLNK
w -296 947 100 0 n#13 ecad2.start.STLK -464 944 -80 944 -80 832 192 832 egenSubB.getNames.SLNK
s 480 1728 200 0 names into the MBBI state strings
s 480 1792 200 0 lookup files and output a list of
s 480 1856 200 0 These gensub routines combine two
s 2176 2016 200 0 seqMechNames
[cell use]
use ecad2 -784 855 100 0 start
xform 0 -624 1168
p -672 848 100 1024 0 name:$(top)$(mech)$(I)
use ebos -480 631 100 0 startGet
xform 0 -352 720
p -368 624 100 1024 0 name:$(top)$(mech)$(I)
use hwin 0 1079 100 0 hwin#52
xform 0 96 1120
p -272 1120 100 0 -1 val(in):$(top)cc:$(mech)SeqLut.VAL
use hwin 0 1207 100 0 hwin#45
xform 0 96 1248
p -96 1248 100 0 -1 val(in):$(top)cc:dirLut.VAL
use hwin 0 1143 100 0 hwin#42
xform 0 96 1184
p -96 1184 100 0 -1 val(in):$(top)cc:$(mech)Lut.VAL
use estringouts 2096 375 100 0 NameOut
xform 0 2224 448
p 2160 336 100 0 1 OMSL:closed_loop
p 2256 384 100 0 -1 def(OUT):$(top)$(mech)Pos.B
p 2208 368 100 1024 0 name:$(top)$(mech)$(I)
p 2352 432 75 768 -1 pproc(OUT):NPP
use inhier -288 807 100 0 SLNK
xform 0 -272 848
use egenSubB 192 743 100 0 getNames
xform 0 336 1168
p 272 1536 100 0 1 FTA:LONG
p 272 1504 100 0 1 FTB:LONG
p 272 1472 100 0 1 FTC:STRING
p 272 1408 100 0 1 FTE:STRING
p -31 261 100 0 0 FTJ:STRING
p -31 261 100 0 0 FTL:STRING
p -31 261 100 0 0 FTN:STRING
p 976 1520 100 0 1 FTVA:STRING
p 976 1520 100 0 1 FTVB:STRING
p 976 1488 100 0 1 FTVC:STRING
p 976 1456 100 0 1 FTVD:STRING
p 976 1424 100 0 1 FTVE:STRING
p 976 1360 100 0 1 FTVF:STRING
p 976 1360 100 0 1 FTVG:STRING
p 976 1328 100 0 1 FTVH:STRING
p 976 1296 100 0 1 FTVI:STRING
p 976 1264 100 0 1 FTVJ:STRING
p 976 1232 100 0 1 FTVK:STRING
p 976 1200 100 0 1 FTVL:STRING
p 976 1168 100 0 1 FTVM:STRING
p 160 704 100 0 1 SNAM:seq$(mech)Names
p -11 -143 100 0 1 TPRO:TRUE
p 544 1568 100 0 -1 def(OUTA):$(top)$(mech)Select.ZRST
p 576 1536 100 0 -1 def(OUTB):$(top)$(mech)Select.ONST
p 576 1504 100 0 -1 def(OUTC):$(top)$(mech)Select.TWST
p 576 1472 100 0 -1 def(OUTD):$(top)$(mech)Select.THST
p 576 1440 100 0 -1 def(OUTE):$(top)$(mech)Select.FRST
p 576 1408 100 0 -1 def(OUTF):$(top)$(mech)Select.FVST
p 576 1376 100 0 -1 def(OUTG):$(top)$(mech)Select.SXST
p 576 1344 100 0 -1 def(OUTH):$(top)$(mech)Select.SVST
p 576 1312 100 0 -1 def(OUTI):$(top)$(mech)Select.EIST
p 576 1280 100 0 -1 def(OUTJ):$(top)$(mech)Select.NIST
p 576 1248 100 0 -1 def(OUTK):$(top)$(mech)Select.TEST
p 576 1232 100 0 -1 def(OUTL):$(top)$(mech)Select.ELST
p 288 672 100 1024 1 name:$(top)$(mech)$(I)
p 480 1546 75 0 -1 pproc(OUTA):NPP
p 480 1514 75 0 -1 pproc(OUTB):NPP
p 480 1482 75 0 -1 pproc(OUTC):NPP
p 480 1450 75 0 -1 pproc(OUTD):NPP
p 480 1418 75 0 -1 pproc(OUTE):NPP
p 480 1386 75 0 -1 pproc(OUTF):NPP
p 480 1354 75 0 -1 pproc(OUTG):NPP
p 480 1322 75 0 -1 pproc(OUTH):NPP
p 480 1290 75 0 -1 pproc(OUTI):NPP
p 480 1258 75 0 -1 pproc(OUTJ):NPP
p 480 1226 75 0 -1 pproc(OUTK):NPP
p 480 1194 75 0 -1 pproc(OUTL):NPP
use embbi 928 39 100 0 Select
xform 0 1440 464
p 1040 -16 100 1024 1 name:$(top)$(mech)$(I)
use oslBorderC -624 -425 100 0 oslBorderC#0
xform 0 1056 880
p 2324 -112 120 256 -1 Project:Gemini Near Infrared Spectrograph
[comments]
