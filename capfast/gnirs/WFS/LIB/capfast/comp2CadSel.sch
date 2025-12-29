[schematic2]
uniq 929
[tools]
[detail]
w 1858 1987 100 0 n#928 compCad.cad.STLKB 1440 1984 2336 1984 compSnlCmd.snl2.ISTL
w 554 2371 -100 0 n#927 bihier.VAL.p 560 2368 608 2368 compCad.cad.VAL
w 554 2339 100 0 n#926 bihier.MESS.p 560 2336 608 2336 compCad.cad.MESS
w 2308 1603 100 0 n#925 elutouts.Lut2.OUTA 2208 1200 2304 1200 2304 2016 2336 2016 compSnlCmd.snl2.ARG
w 2268 1971 100 0 n#924 elutouts.Lut1.OUTA 2208 1744 2272 1744 2272 2208 2336 2208 compSnlCmd.snl1.ARG
w 1924 1435 100 0 n#923 elutouts.Lut.OUTB 1824 1680 1920 1680 1920 1200 1952 1200 elutouts.Lut2.VAL
w 1858 1755 100 0 n#922 elutouts.Lut.OUTA 1824 1744 1952 1744 elutouts.Lut1.VAL
w 1468 1971 100 0 n#921 compCad.cad.OUTA 1440 2208 1472 2208 1472 1744 1568 1744 elutouts.Lut.VAL
w 1858 1955 100 0 n#920 compCad.cad.SPLKB 1440 1952 2336 1952 compSnlCmd.snl2.ISPL
w 1858 2051 100 0 n#918 compCad.cad.OCIDB 1440 2048 2336 2048 compSnlCmd.snl2.ICID
w 1858 2147 100 0 n#917 compCad.cad.SPLKA 1440 2144 2336 2144 compSnlCmd.snl1.ISPL
w 1858 2179 100 0 n#916 compCad.cad.STLKA 1440 2176 2336 2176 compSnlCmd.snl1.ISTL
w 1858 2243 100 0 n#915 compCad.cad.OCIDA 1440 2240 2336 2240 compSnlCmd.snl1.ICID
w 68 2283 100 0 n#914 embbis.Menu.FLNK 32 2320 64 2320 64 2256 128 2256 estringouts.MenuStr.SLNK
w 50 2299 100 0 n#913 embbis.Menu.VAL 32 2288 128 2288 estringouts.MenuStr.DOL
w 466 2243 100 0 n#912 estringouts.MenuStr.OUT 384 2240 608 2240 compCad.cad.CADA
w 1442 2339 -100 0 CFLNK compCad.cad.CFLNK 1440 2336 1504 2336 outhier.CFLNK.p
w 1440 2371 -100 0 CVAL compCad.cad.CVAL 1440 2368 1488 2368 bihier.CVAL.p
w 554 2403 100 0 n#907 bihier.CLID.p 560 2400 608 2400 compCad.cad.CLID
w 566 2435 -100 0 DIR bihier.DIR.p 560 2432 608 2432 compCad.cad.DIR
s 2032 192 400 1280 comp2CadSel
s 2080 2336 100 768 The "2" suffix is needed to previent conflicts with
s 2080 2304 100 768 records for single-axis commands.
n -384 224 96 576 100
This schematic contains a "generic" CAD
record, which takes an op code, which
requires no parameters, and passes it to
the corresponding SNL code.  By default,
no subroutine is needed, because the
output links trigger all necessary
actions.
.
.
.
.
.
.
.
_
[cell use]
use bihier 528 2432 100 2048 DIR
xform 0 560 2432
use bihier 528 2400 100 2048 CLID
xform 0 560 2400
use bihier 528 2368 100 2048 VAL
xform 0 560 2368
use bihier 528 2336 100 2048 MESS
xform 0 560 2336
use bihier 1520 2368 100 1536 CVAL
xform 0 1488 2368
use outhier 1520 2336 100 1536 CFLNK
xform 0 1488 2336
use compCad 1024 2464 100 1024 cad
xform 0 1024 1856
p 720 2208 100 768 1 setia:inpa $(sadtop)$(mech1)EngCyclic.VAL
p 720 2176 100 768 1 setib:inpb $(sadtop)$(mech1)EngMin.VAL
p 720 2144 100 768 1 setic:inpc $(sadtop)$(mech1)EngMax.VAL
p 720 2112 100 768 1 setid:inpd $(sadtop)$(mech2)EngCyclic.VAL
p 720 2080 100 768 1 setie:inpe $(sadtop)$(mech2)EngMin.VAL
p 720 2048 100 768 1 setif:inpf $(sadtop)$(mech2)EngMax.VAL
p 720 2304 100 768 1 setr:ignore $(ignore)
p 720 2272 100 768 1 sets:datumed $(sadtop)$(mech)Datumed.VAL
use elutouts 1632 1872 100 768 Lut
xform 0 1696 1680
p 1632 1456 100 768 1 FDIR:$(pvdir)
p 1632 1424 100 768 1 FNAM:$(mech).lut
p 1632 1392 100 768 1 PV:$(top)$(mech)$(cad)
p 1824 1744 75 768 -1 pproc(OUTA):PP
p 1824 1680 75 768 -1 pproc(OUTB):PP
use elutouts 2016 1872 100 768 Lut1
xform 0 2080 1680
p 2016 1456 100 768 1 FDIR:$(pvdir)
p 2016 1424 100 768 1 FNAM:$(mech1).lut
p 2016 1392 100 768 1 PV:$(top)$(mech)$(cad)
p 2208 1744 75 768 -1 pproc(OUTA):PP
use elutouts 2016 1328 100 768 Lut2
xform 0 2080 1136
p 2016 912 100 768 1 FDIR:$(pvdir)
p 2016 880 100 768 1 FNAM:$(mech2).lut
p 2016 848 100 768 1 PV:$(top)$(mech)$(cad)
p 2208 1200 75 768 -1 pproc(OUTA):PP
use embbis -160 2352 100 768 Menu
xform 0 -96 2304
p -160 2192 100 768 1 NOBT:4
p -160 2224 100 768 1 PV:$(top)$(mech)$(cad)
p -160 2160 100 768 1 ZRST:no_menu
use estringouts 192 2304 100 768 MenuStr
xform 0 256 2256
p 64 2126 100 0 0 EVNT:0
p 192 2176 100 768 1 OMSL:closed_loop
p 192 2144 100 768 1 PV:$(top)$(mech)$(cad)
p 384 2240 75 768 -1 pproc(OUT):NPP
use compSnlCmd 2528 2240 100 1024 snl1
xform 0 2528 2192
p 2416 2176 100 768 1 set0:mech $(mech1)
p 2416 2112 100 768 1 set2:cmd $(cad)2
use compSnlCmd 2528 2048 100 1024 snl2
xform 0 2528 2000
p 2416 1984 100 768 1 set0:mech $(mech2)
p 2416 1920 100 768 1 set2:cmd $(cad)2
use bc200tr -512 72 -100 0 frame
xform 0 1168 1376
p 2064 240 100 0 1 author:H.T.Yamada
p 2288 224 100 0 -1 border:C
p 2048 208 100 0 1 checked:H.T.Yamada
p 2320 224 100 0 -1 date:1998-10-09
p -512 72 100 0 0 id:frame
p 2288 352 100 0 -1 project:Near Infra-Red Imager
p -512 120 100 0 0 revision:1.0
p 2288 288 100 0 -1 title:One axis Generic Command CAD record
[comments]
