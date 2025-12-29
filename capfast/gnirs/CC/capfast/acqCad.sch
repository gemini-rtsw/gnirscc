[schematic2]
uniq 78
[tools]
[detail]
w 304 43 100 0 n#77 efanouts.startFan.LNK2 160 32 496 32 elongouts.car.SLNK
w 264 779 100 0 n#76 efanouts.startFan.LNK1 160 64 224 64 224 768 352 768 elongouts.setMARK.SLNK
w -192 -5 100 0 n#75 ecad8.Cad.STLK -256 -16 -80 -16 efanouts.startFan.SLNK
w 500 59 100 0 n#73 hwin.hwin#48.in 496 64 496 64 elongouts.car.DOL
w -104 539 100 0 n#71 ecad8.Cad.VALC -256 528 96 528 96 736 352 736 elongouts.setMARK.SDIS
w 612 731 100 2 n#69 hwout.hwout#68.outp 608 736 608 736 elongouts.setMARK.OUT
w 356 795 100 2 n#67 hwin.hwin#66.in 352 800 352 800 elongouts.setMARK.DOL
w -1024 507 100 0 n#64 estringouts.B.OUT -1040 496 -960 496 free
w -760 507 100 0 n#63 hwin.hwin#62.in -960 240 -896 240 -896 496 -576 496 ecad8.Cad.INPC
w -744 443 100 0 n#61 hwin.hwin#60.in -960 176 -864 176 -864 432 -576 432 ecad8.Cad.INPD
w -956 107 100 2 n#59 hwin.hwin#57.in -960 112 -800 112 -800 368 -576 368 ecad8.Cad.INPE
w -984 -69 100 0 n#56 hwin.hwin#55.in -960 -80 -704 -80 -704 176 -576 176 ecad8.Cad.INPH
w 744 347 100 0 OMSS ecars.C.OMSS 672 432 704 432 704 336 832 336 outhier.OMSS.p
w -120 763 100 0 n#53 ecad8.Cad.OCID -256 752 64 752 64 464 352 464 ecars.C.ICID
w 504 203 100 0 n#52 elongouts.car.OUT 752 0 800 0 800 192 256 192 256 496 352 496 ecars.C.IVAL
w 504 171 100 0 n#72 elongouts.car.FLNK 752 64 768 64 768 160 288 160 288 304 352 304 ecars.C.SLNK
w 728 403 100 0 OCID bihier.OCID.p 832 400 752 400 752 464 672 464 ecars.C.CLID
w -1336 795 100 0 SLNKA inhier.SLNKA.P -1376 784 -1248 784 estringouts.A.SLNK
w -1384 523 100 0 SLNKB inhier.SLNKB.P -1424 512 -1296 512 estringouts.B.SLNK
w -912 771 100 0 n#39 estringouts.A.OUT -992 768 -784 768 -784 656 -576 656 ecad8.Cad.A
w -1308 851 100 0 DOLA inhier.DOLA.P -1376 896 -1312 896 -1312 816 -1248 816 estringouts.A.DOL
w -1416 603 100 0 n#34 inhier.DOLB.P -1424 592 -1360 592 -1360 544 -1296 544 estringouts.B.DOL
w -984 59 100 0 n#32 hwin.hwin#23.in -960 48 -768 48 -768 304 -576 304 ecad8.Cad.INPF
w -984 -5 100 0 n#31 hwin.hwin#25.in -960 -16 -736 -16 -736 240 -576 240 ecad8.Cad.INPG
w 728 499 100 0 OVAL ecars.C.VAL 672 496 832 496 outhier.OVAL.p
w 736 275 100 0 FLNK ecars.C.FLNK 672 272 848 272 outhier.FLNK.p
w -680 819 100 0 CLID inhier.CLID.P -736 816 -576 816 ecad8.Cad.ICID
w -722 931 100 0 DIR inhier.DIR.P -768 928 -640 928 -640 848 -576 848 ecad8.Cad.DIR
w -88 827 100 0 MESS ecad8.Cad.MESS -256 816 128 816 128 800 outhier.MESS.p
w -162 859 100 0 VAL ecad8.Cad.VAL -256 848 -32 848 -32 896 128 896 outhier.VAL.p
s 160 832 100 0 CLEAR
[cell use]
use efanouts -48 -208 100 0 startFan
xform 0 40 0
p 32 -176 100 1024 1 name:$(top)$(mech)$(op)$(I)
use hwout 608 695 100 0 hwout#68
xform 0 704 736
p 544 688 100 0 -1 val(outp):$(top)fw1PosCad.DIR PP NMS
use hwin -1152 135 100 0 hwin#60
xform 0 -1056 176
p -1408 160 100 0 -1 val(in):$(top)fw1PosCad.VALB
use hwin -1152 -57 100 0 hwin#25
xform 0 -1056 -16
p -1344 -32 100 0 -1 val(in):$(top)init.VALA
use hwin -1152 7 100 0 hwin#23
xform 0 -1056 48
p -1440 32 100 0 -1 val(in):$(sadtop)$(mech)Datumed
use hwin 304 23 100 0 hwin#48
xform 0 400 64
p 307 56 100 0 -1 val(in):$(CAR_BUSY)
use hwin -1152 -121 100 0 hwin#55
xform 0 -1056 -80
p -1424 -96 100 0 -1 val(in):nirs:motionDisable.VAL
use hwin -1152 71 100 0 hwin#57
xform 0 -1056 112
p -1408 96 100 0 -1 val(in):$(top)fw1PosCad.MARK
use hwin -1152 199 100 0 hwin#62
xform 0 -1056 240
p -1376 224 100 0 -1 val(in):$(top)fw1PosCad.B
use hwin 160 759 100 0 hwin#66
xform 0 256 800
p 163 792 100 0 -1 val(in):1
use elongouts 496 -57 100 0 car
xform 0 624 32
p 608 -64 100 1024 0 name:$(top)$(mech)$(op)$(I)
use elongouts 432 800 100 0 setMARK
xform 0 480 768
p 256 640 100 0 1 DISV:0
p 256 688 100 0 1 OMSL:closed_loop
p 416 656 100 1024 1 name:$(top)$(mech)$(op)$(I)
use outhier 800 295 100 0 OMSS
xform 0 816 336
use outhier 816 231 100 0 FLNK
xform 0 832 272
use outhier 96 759 100 0 MESS
xform 0 112 800
use outhier 96 855 100 0 VAL
xform 0 112 896
use outhier 800 455 100 0 OVAL
xform 0 816 496
use ecars 352 215 100 0 C
xform 0 512 384
p 464 208 100 1024 0 name:$(top)$(mech)$(op)$(I)
use bihier 816 359 100 0 OCID
xform 0 832 400
use ecad8 -576 -105 100 0 Cad
xform 0 -416 400
p -480 592 100 0 1 FTVA:LONG
p -480 560 100 0 1 FTVB:STRING
p -480 528 100 0 1 FTVC:LONG
p -480 496 100 0 1 FTVD:LONG
p -480 464 100 0 1 FTVE:LONG
p -480 432 100 0 0 FTVF:LONG
p -480 400 100 0 0 FTVG:LONG
p -480 368 100 0 0 FTVH:STRING
p -512 -192 100 0 1 INAM:$(mech)$(op)$(I)Init
p -512 -144 100 0 1 SNAM:$(mech)$(op)$(I)
p -352 -176 100 1024 1 name:$(top)$(mech)$(op)$(I)
p -608 368 75 1280 -1 pproc(INPE):NPP
use inhier -752 775 100 0 CLID
xform 0 -736 816
use inhier -784 887 100 0 DIR
xform 0 -768 928
use inhier -1392 855 100 0 DOLA
xform 0 -1376 896
use inhier -1392 743 100 0 SLNKA
xform 0 -1376 784
use inhier -1440 471 100 0 SLNKB
xform 0 -1424 512
use inhier -1440 551 100 0 DOLB
xform 0 -1424 592
use estringouts -1248 711 100 0 A
xform 0 -1120 784
p -1248 640 100 0 1 OMSL:closed_loop
p -1152 656 100 1024 1 name:$(top)$(mech)$(op)$(I)
use estringouts -1296 439 100 0 B
xform 0 -1168 512
p -1296 368 100 0 1 OMSL:closed_loop
p -1184 400 100 1024 1 name:$(top)$(mech)$(op)$(I)
use bb200tr -1568 -536 -100 0 frame
xform 0 -288 288
p 208 -368 100 0 -1 author:Dr Who
p 224 -400 100 0 -1 date:1-25-2001
p 464 -320 200 0 -1 filename:acqCad
p 432 -272 200 0 -1 system:GNIRS CC
[comments]
