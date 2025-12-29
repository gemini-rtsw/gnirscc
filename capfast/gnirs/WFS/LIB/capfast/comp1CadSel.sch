[schematic2]
uniq 843
[tools]
[detail]
w 624 1923 -100 0 MESS compCad.cad.MESS 672 1920 624 1920 bihier.MESS.p
w 630 1955 -100 0 VAL bihier.VAL.p 624 1952 672 1952 compCad.cad.VAL
w 2012 1659 100 0 n#840 elutouts.Lut.OUTA 1888 1536 2016 1536 2016 1792 2048 1792 compSnlCmd.snl.ARG
w 1596 1659 100 0 n#839 compCad.cad.OUTA 1504 1792 1600 1792 1600 1536 1632 1536 elutouts.Lut.VAL
w 1746 1763 100 0 n#838 compCad.cad.STLKA 1504 1760 2048 1760 compSnlCmd.snl.ISTL
w 1746 1827 100 0 n#837 compCad.cad.OCIDA 1504 1824 2048 1824 compSnlCmd.snl.ICID
w 1746 1731 100 0 n#836 compCad.cad.SPLKA 1504 1728 2048 1728 compSnlCmd.snl.ISPL
w 1498 1955 -100 0 c#834 bihier.CVAL.p 1552 1952 1504 1952 compCad.cad.CVAL
w 1512 1923 -100 0 FLNK outhier.CFLNK.p 1568 1920 1504 1920 compCad.cad.CFLNK
w 630 2019 -100 0 DIR bihier.DIR.p 624 2016 672 2016 compCad.cad.DIR
w 220 1867 100 0 n#796 embbis.Menu.FLNK 192 1904 224 1904 224 1840 288 1840 estringouts.MenuStr.SLNK
w 210 1875 100 0 n#795 embbis.Menu.VAL 192 1872 288 1872 estringouts.MenuStr.DOL
w 578 1827 -100 0 n#790 estringouts.MenuStr.OUT 544 1824 672 1824 compCad.cad.CADA
w 618 1987 100 0 n#121 bihier.CLID.p 624 1984 672 1984 compCad.cad.CLID
s 1024 2400 100 768 Note that this record is named using the name field.
s 2032 192 400 1280 comp1CadSel
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
use bihier 592 1984 100 2048 CLID
xform 0 624 1984
use bihier 592 2016 100 2048 DIR
xform 0 624 2016
use bihier 592 1920 100 2048 MESS
xform 0 624 1920
use bihier 592 1952 100 2048 VAL
xform 0 624 1952
use bihier 1584 1952 100 1536 CVAL
xform 0 1552 1952
use outhier 1584 1920 100 1536 CFLNK
xform 0 1552 1920
use compCad 1088 2048 100 1024 cad
xform 0 1088 1440
p 784 1888 100 768 1 setr:ignore $(ignore)
p 784 1856 100 768 1 sets:datumed $(sadtop)$(mech)Datumed.VAL
use elutouts 1696 1664 100 768 Lut
xform 0 1760 1472
p 1696 1232 100 768 1 FDIR:$(pvdir)
p 1696 1200 100 768 1 FNAM:$(mech).lut
p 1696 1168 100 768 1 PV:$(top)$(mech)$(cad)
p 1888 1536 75 768 -1 pproc(OUTA):PP
use estringouts 352 1888 100 768 MenuStr
xform 0 416 1840
p 224 1710 100 0 0 EVNT:0
p 352 1760 100 768 1 OMSL:closed_loop
p 352 1728 100 768 1 PV:$(top)$(mech)$(cad)
p 544 1824 75 768 -1 pproc(OUT):NPP
use embbis 0 1936 100 768 Menu
xform 0 64 1888
p 0 1808 100 768 1 NOBT:4
p 0 1776 100 768 1 PV:$(top)$(mech)$(cad)
p 0 1744 100 768 1 ZRST:no_menu
use compSnlCmd 2240 1824 100 1024 snl
xform 0 2240 1776
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
