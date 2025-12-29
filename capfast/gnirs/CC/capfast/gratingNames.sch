[schematic2]
uniq 116
[tools]
[detail]
w 2402 1563 100 0 n#112 egenSub.wcsCheck.SLNK 2704 1552 2160 1552 2160 1168 junction
w 2322 11 100 0 n#112 estringouts.estringouts#113.FLNK 2288 0 2416 0 2416 240 2160 240 2160 1168 2080 1168 estringouts.estringouts#109.FLNK
w 1826 -149 100 0 n#115 embbi.SelectB.VAL 1776 -160 1936 -160 1936 16 2032 16 estringouts.estringouts#113.DOL
w 1922 -5 100 0 n#114 embbi.SelectB.FLNK 1776 32 1872 32 1872 -16 2032 -16 estringouts.estringouts#113.SLNK
w 1770 1067 100 0 n#111 embbi.SelectA.VAL 1776 688 1856 688 1856 1056 1744 1056 1744 1184 1824 1184 estringouts.estringouts#109.DOL
w 1770 1163 100 0 n#110 embbi.SelectA.FLNK 1776 880 1776 1152 1824 1152 estringouts.estringouts#109.SLNK
w 2856 1403 100 0 n#69 estringouts.wcsMark.SLNK 2752 1296 2624 1296 2624 1392 3136 1392 3136 1520 2992 1520 egenSub.wcsCheck.FLNK
w 2816 1419 100 0 n#67 egenSub.wcsCheck.VALA 2992 2256 3088 2256 3088 1408 2592 1408 2592 1264 2752 1264 estringouts.wcsMark.SDIS
w -184 859 100 0 n#55 ecad2.getNamesStart.STLK -384 848 64 848 64 832 192 832 egenSubB.getNames.SLNK
w 168 1131 100 0 n#53 hwin.hwin#52.in 192 1120 192 1120 egenSubB.getNames.INPN
w 168 1259 100 0 n#48 hwin.hwin#45.in 192 1248 192 1248 egenSubB.getNames.INPJ
w 168 1195 100 0 n#43 hwin.hwin#42.in 192 1184 192 1184 egenSubB.getNames.INPL
s 2496 2640 200 0 mechNames
s 480 1856 200 0 These gensub routines combine two
s 480 1792 200 0 lookup files and output a list of
s 480 1728 200 0 names into the MBBI state strings
s 3120 1648 100 0 Since this menu is abstract and expands for each mech,
s 3120 1616 100 0 we need a method to mark the selectWcs only when the camera changes.
[cell use]
use estringouts 2032 -89 100 0 estringouts#113
xform 0 2160 -16
p 2096 -128 100 0 1 OMSL:closed_loop
p 2192 -80 100 0 -1 def(OUT):$(top)$(mech)PosCad.B
p 2160 -16 100 1024 0 name:$(top)$(mech)Str2
p 2288 -32 75 768 -1 pproc(OUT):NPP
use estringouts 2752 1223 100 0 wcsMark
xform 0 2880 1296
p 2816 1184 100 0 1 OMSL:closed_loop
p 3024 1280 100 0 -1 def(OUT):nirs:dc:selectWcs.B
p 2864 1216 100 1024 0 name:$(top)$(mech)$(I)
use estringouts 1824 1079 100 0 estringouts#109
xform 0 1952 1152
p 1888 1040 100 0 1 OMSL:closed_loop
p 1984 1088 100 0 -1 def(OUT):$(top)$(mech)PosCad.B
p 1936 1072 100 1024 0 name:$(top)$(mech)Str1
p 2080 1136 75 768 -1 pproc(OUT):NPP
use embbi 736 272 100 0 SelectA
xform 0 1264 672
p 864 240 100 1024 0 name:$(top)$(mech)$(I)
p 720 592 75 1280 0 pproc(U7):NPP
p 1744 624 100 1024 -1 username(U0):FLD0
use embbi 752 -601 100 0 SelectB
xform 0 1264 -176
p 1408 944 100 1024 0 name:$(top)$(mech)$(I)
p 1056 126 100 0 0 primitive:embbi
p 1744 -224 100 1024 -1 username(U0):FLD0
use bd200tr -1216 -744 -100 0 frame
xform 0 1424 960
use hwin 0 1143 100 0 hwin#42
xform 0 96 1184
p -96 1184 100 0 -1 val(in):$(top)$(mech)Lut.VAL
use hwin 0 1207 100 0 hwin#45
xform 0 96 1248
p -96 1248 100 0 -1 val(in):$(top)dirLut.VAL
use hwin 0 1079 100 0 hwin#52
xform 0 96 1120
p -272 1120 100 0 -1 val(in):$(top)$(mech)SeqLut.VAL
use egenSub 2704 1463 100 0 wcsCheck
xform 0 2848 1888
p 2784 2224 100 0 1 FTA:STRING
p 3152 2208 100 0 1 FTVA:LONG
p 2704 1424 100 0 1 SNAM:cameraCheck
p 2816 2304 100 1024 1 name:$(top)$(mech)$(I)
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
[comments]
