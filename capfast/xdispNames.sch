[schematic2]
uniq 98
[tools]
[detail]
w 1840 -133 100 0 n#97 embbi.embbi#80.VAL 1728 -144 2000 -144 2000 96 2096 96 estringouts.estringouts#82.DOL
w 1968 75 100 0 n#96 embbi.embbi#80.FLNK 1728 48 1888 48 1888 64 2096 64 estringouts.estringouts#82.SLNK
w 1960 491 100 0 n#95 embbi.Select.VAL 1712 704 1872 704 1872 480 2096 480 estringouts.NameOut.DOL
w 1808 907 100 0 n#94 embbi.Select.FLNK 1712 896 1952 896 1952 448 2096 448 estringouts.NameOut.SLNK
w 2424 91 100 0 n#68 estringouts.estringouts#82.FLNK 2352 80 2544 80 2544 544 2368 544 junction
w 3024 667 100 0 n#69 estringouts.wcsMark.SLNK 3280 656 2816 656 2816 896 2672 896 egenSub.wcsCheck.FLNK
w 2288 939 100 0 n#68 egenSub.wcsCheck.SLNK 2384 928 2240 928 2240 608 2368 608 2368 464 2352 464 estringouts.NameOut.FLNK
w 3000 635 100 0 n#67 egenSub.wcsCheck.VALA 2672 1632 2768 1632 2768 624 3280 624 estringouts.wcsMark.SDIS
w -184 859 100 0 n#55 ecad2.getNamesStart.STLK -384 848 64 848 64 832 192 832 egenSubB.getNames.SLNK
w 168 1131 100 0 n#53 hwin.hwin#52.in 192 1120 192 1120 egenSubB.getNames.INPN
w 168 1259 100 0 n#48 hwin.hwin#45.in 192 1248 192 1248 egenSubB.getNames.INPJ
w 168 1195 100 0 n#43 hwin.hwin#42.in 192 1184 192 1184 egenSubB.getNames.INPL
s 2800 992 100 0 we need a method to mark the selectWcs only when the camera changes.
s 2800 1024 100 0 Since this menu is abstract and expands for each mech,
s 480 1728 200 0 names into the MBBI state strings
s 480 1792 200 0 lookup files and output a list of
s 480 1856 200 0 These gensub routines combine two
s 2176 2016 200 0 mechNames
[cell use]
use estringouts 2096 -9 100 0 estringouts#82
xform 0 2224 64
p 2160 -48 100 0 1 OMSL:closed_loop
p 2256 0 100 0 -1 def(OUT):$(top)$(mech)PosCad.B
p 2208 -16 100 1024 0 name:$(top)$(mech)$(I)
p 2352 48 75 768 -1 pproc(OUT):NPP
use embbi 704 -585 100 0 embbi#80
xform 0 1216 -160
p 816 -592 100 1024 0 name:$(top)$(mech)SelectB
use bd200tr -1216 -744 -100 0 frame
xform 0 1424 960
use hwin 0 1079 100 0 hwin#52
xform 0 96 1120
p -272 1120 100 0 -1 val(in):$(top)$(mech)SeqLut.VAL
use hwin 0 1207 100 0 hwin#45
xform 0 96 1248
p -96 1248 100 0 -1 val(in):$(top)dirLut.VAL
use hwin 0 1143 100 0 hwin#42
xform 0 96 1184
p -96 1184 100 0 -1 val(in):$(top)$(mech)Lut.VAL
use egenSub 2384 839 100 0 wcsCheck
xform 0 2528 1264
p 2464 1600 100 0 1 FTA:STRING
p 2832 1584 100 0 1 FTVA:LONG
p 2384 800 100 0 1 SNAM:cameraCheck
p 2496 1680 100 1024 1 name:$(top)$(mech)$(I)
use estringouts 2096 375 100 0 NameOut
xform 0 2224 448
p 2160 336 100 0 1 OMSL:closed_loop
p 2256 384 100 0 -1 def(OUT):$(top)$(mech)PosCad.B
p 2208 368 100 1024 0 name:$(top)$(mech)$(I)
p 2352 432 75 768 -1 pproc(OUT):NPP
use estringouts 3280 583 100 0 wcsMark
xform 0 3408 656
p 3344 544 100 0 1 OMSL:closed_loop
p 3552 640 100 0 -1 def(OUT):nirs:dc:selectWcs.B
p 3392 576 100 1024 0 name:$(top)$(mech)$(I)
use ecad2 -704 759 100 0 getNamesStart
xform 0 -544 1072
p -592 752 100 1024 0 name:$(top)$(mech)$(I)
use inhier -320 231 100 0 SLNK
xform 0 -304 272
use egenSubB 192 743 100 0 getNames
xform 0 336 1168
p 272 1536 100 0 1 FTA:LONG
p 272 1504 100 0 1 FTB:LONG
p 272 1472 100 0 1 FTC:STRING
p 272 1408 100 0 1 FTE:STRING
p -31 261 100 0 0 FTJ:STRING
p -31 261 100 0 0 FTL:STRING
p -31 261 100 0 0 FTN:STRING
p 976 1568 100 0 1 FTVA:STRING
p 976 1536 100 0 1 FTVB:STRING
p 976 1488 100 0 1 FTVC:STRING
p 976 1456 100 0 1 FTVD:STRING
p 976 1424 100 0 1 FTVE:STRING
p 976 1392 100 0 1 FTVF:STRING
p 976 1360 100 0 1 FTVG:STRING
p 976 1328 100 0 1 FTVH:STRING
p 976 1296 100 0 1 FTVI:STRING
p 976 1264 100 0 1 FTVJ:STRING
p 976 1232 100 0 1 FTVK:STRING
p 976 1200 100 0 1 FTVL:STRING
p 976 1168 100 0 1 FTVM:STRING
p 160 704 100 0 1 SNAM:$(mech)Names
p -224 960 100 0 1 TPRO:TRUE
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
p 576 1200 100 0 -1 def(OUTM):$(top)$(mech)Select.TVST
p -31 613 100 0 0 def(OUTN):$(top)$(mech)Select.TTST
p -31 613 100 0 0 def(OUTO):0.000000000000000e+00
p 288 672 100 1024 1 name:$(top)$(mech)$(I)
p 144 1514 75 0 -1 pproc(INPB):PP
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
use embbi 688 263 100 0 Select
xform 0 1200 688
p 800 256 100 1024 0 name:$(top)$(mech)SelectA
p 1680 640 100 1024 -1 username(U0):FLD0
[comments]
