[schematic2]
uniq 37
[tools]
[detail]
w -512 51 100 0 n#36 hwin.hwin#35.in -608 48 -368 48 ecad8.Cad.H
w -472 667 100 0 CLID inhier.CLID.P -656 752 -528 752 -528 656 -368 656 ecad8.Cad.ICID
w -472 443 100 0 n#25 estringouts.StringB.OUT -656 352 -528 352 -528 432 -368 432 ecad8.Cad.B
w -472 507 100 0 n#24 estringouts.StringA.OUT -624 576 -528 576 -528 496 -368 496 ecad8.Cad.A
w -984 379 100 0 SLNKB inhier.SLNKB.P -1072 304 -1008 304 -1008 368 -912 368 estringouts.StringB.SLNK
w -1016 411 100 0 DOLB inhier.DOLB.P -1072 400 -912 400 estringouts.StringB.DOL
w -1048 539 100 0 SLNKA inhier.SLNKA.P -1072 528 -976 528 -976 592 -880 592 estringouts.StringA.SLNK
w -1000 635 100 0 DOLA inhier.DOLA.P -1072 624 -880 624 estringouts.StringA.DOL
w -24 659 100 0 n#29 ecad8.Cad.MESS -48 656 48 656 48 672 128 672 outhier.MESS.p
w 40 771 100 0 n#28 ecad8.Cad.VAL -48 688 0 688 0 768 128 768 outhier.VAL.p
w -432 864 100 0 DIR inhier.DIR.P -496 848 -400 848 -400 688 -368 688 ecad8.Cad.DIR
w 332 227 100 0 n#10 cadFanout.cadFanout#0.OUTB 272 272 336 272 336 192 384 192 hwout.hwout#8.outp
w 310 307 100 0 n#9 cadFanout.cadFanout#0.OUTA 272 304 384 304 384 288 hwout.hwout#7.outp
w -8 179 100 0 n#34 ecad8.Cad.VALF -48 176 80 176 cadFanout.cadFanout#0.SPLK
w -8 211 100 0 n#33 ecad8.Cad.OUTE -48 208 80 208 cadFanout.cadFanout#0.STLK
w -8 243 100 0 n#32 ecad8.Cad.VALE -48 240 80 240 cadFanout.cadFanout#0.PLNK
w -8 275 100 0 n#31 ecad8.Cad.OUTD -48 272 80 272 cadFanout.cadFanout#0.CLNK
w -8 307 100 0 n#30 ecad8.Cad.VALD -48 304 80 304 cadFanout.cadFanout#0.MLNK
[cell use]
use hwin -800 7 100 0 hwin#35
xform 0 -704 48
p -1088 32 100 0 -1 val(in):$(sadtop)$(mech)Datumed
use ecad8 -368 -265 100 0 Cad
xform 0 -208 240
p -304 -304 100 0 1 SNAM:$(mech)$(op)$(I)
p -224 -352 100 1024 1 name:$(top)$(mech)$(op)$(I)
use inhier -512 807 100 0 DIR
xform 0 -496 848
use inhier -1088 487 100 0 SLNKA
xform 0 -1072 528
use inhier -1088 263 100 0 SLNKB
xform 0 -1072 304
use inhier -1088 583 100 0 DOLA
xform 0 -1072 624
use inhier -1088 359 100 0 DOLB
xform 0 -1072 400
use inhier -672 711 100 0 CLID
xform 0 -656 752
use estringouts -912 295 100 0 StringB
xform 0 -784 368
p -896 224 100 0 1 OMSL:closed_loop
p -768 240 100 1024 1 name:$(top)$(mech)$(op)$(I)
use estringouts -880 519 100 0 StringA
xform 0 -752 592
p -880 448 100 0 1 OMSL:closed_loop
p -752 480 100 1024 1 name:$(top)$(mech)$(op)$(I)
use bb200tr -1408 -600 -100 0 frame
xform 0 -128 224
p 384 -432 100 0 -1 author:Peter Ruckle
p 416 -464 100 0 -1 date:1-25-2001
p 640 -384 200 0 -1 filename:mech2Cad
p 624 -336 200 0 -1 system:GNIRS CC
use outhier 96 727 100 0 VAL
xform 0 112 768
use outhier 96 631 100 0 MESS
xform 0 112 672
use hwout 384 247 100 0 hwout#7
xform 0 480 288
p 592 272 100 0 -1 val(outp):$(top$(mech)1$(op)
use hwout 384 151 100 0 hwout#8
xform 0 480 192
p 592 176 100 0 -1 val(outp):$(top)$(mech)2$(op)
use cadFanout 80 55 100 0 cadFanout#0
xform 0 176 208
[comments]
