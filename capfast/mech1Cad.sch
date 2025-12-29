[schematic2]
uniq 57
[tools]
[detail]
w -440 171 100 0 n#56 hwin.hwin#55.in -416 160 -416 160 ecad8.Cad.INPH
w 744 427 100 0 OMSS ecars.C.OMSS 672 512 704 512 704 416 832 416 outhier.OMSS.p
w 184 555 100 0 n#53 ecad8.Cad.OCID -96 736 64 736 64 544 352 544 ecars.C.ICID
w 344 267 100 0 n#52 elongouts.car.OUT 528 32 608 32 608 256 128 256 128 576 352 576 ecars.C.IVAL
w 344 235 100 0 n#51 elongouts.car.FLNK 528 96 576 96 576 224 160 224 160 384 352 384 ecars.C.SLNK
w 728 483 100 0 OCID bihier.OCID.p 832 480 752 480 752 544 672 544 ecars.C.CLID
w 248 107 100 0 n#49 hwin.hwin#48.in 272 96 272 96 elongouts.car.DOL
w -16 -21 100 0 n#47 ecad8.Cad.STLK -96 -32 112 -32 112 64 272 64 elongouts.car.SLNK
w -1176 779 100 0 SLNKA inhier.SLNKA.P -1216 768 -1088 768 estringouts.A.SLNK
w -768 483 100 0 n#44 estringouts.B.OUT -880 480 -608 480 -608 576 -512 576 free
w -1224 507 100 0 SLNKB inhier.SLNKB.P -1264 496 -1136 496 estringouts.B.SLNK
w -752 755 100 0 n#39 estringouts.A.OUT -832 752 -624 752 -624 640 -416 640 ecad8.Cad.A
w -1148 835 100 0 DOLA inhier.DOLA.P -1216 880 -1152 880 -1152 800 -1088 800 estringouts.A.DOL
w -1256 587 100 0 n#34 inhier.DOLB.P -1264 576 -1200 576 -1200 528 -1136 528 estringouts.B.DOL
w -440 299 100 0 n#32 hwin.hwin#23.in -416 288 -416 288 ecad8.Cad.INPF
w -440 235 100 0 n#31 hwin.hwin#25.in -416 224 -416 224 ecad8.Cad.INPG
w 728 579 100 0 OVAL ecars.C.VAL 672 576 832 576 outhier.OVAL.p
w 736 355 100 0 FLNK ecars.C.FLNK 672 352 848 352 outhier.FLNK.p
w -520 803 100 0 CLID inhier.CLID.P -576 800 -416 800 ecad8.Cad.ICID
w -562 915 100 0 DIR inhier.DIR.P -608 912 -480 912 -480 832 -416 832 ecad8.Cad.DIR
w -8 803 100 0 MESS ecad8.Cad.MESS -96 800 128 800 outhier.MESS.p
w 30 899 100 0 VAL ecad8.Cad.VAL -96 832 -32 832 -32 896 128 896 outhier.VAL.p
[cell use]
use hwin -608 183 100 0 hwin#25
xform 0 -512 224
p -832 224 100 0 -1 val(in):$(top)init.VALA
use hwin -608 247 100 0 hwin#23
xform 0 -512 288
p -896 272 100 0 -1 val(in):$(sadtop)$(mech)Datumed
use hwin 80 55 100 0 hwin#48
xform 0 176 96
p 83 88 100 0 -1 val(in):$(CAR_BUSY)
use hwin -608 119 100 0 hwin#55
xform 0 -512 160
p -880 144 100 0 -1 val(in):nirs:motionDisable.VAL
use outhier 800 375 100 0 OMSS
xform 0 816 416
use outhier 816 311 100 0 FLNK
xform 0 832 352
use outhier 96 759 100 0 MESS
xform 0 112 800
use outhier 96 855 100 0 VAL
xform 0 112 896
use outhier 800 535 100 0 OVAL
xform 0 816 576
use ecars 352 295 100 0 C
xform 0 512 464
p 464 288 100 1024 0 name:$(top)$(mech)$(op)$(I)
use elongouts 272 -25 100 0 car
xform 0 400 64
p 384 -32 100 1024 0 name:$(top)$(mech)$(op)$(I)
use bihier 816 439 100 0 OCID
xform 0 832 480
use ecad8 -416 -121 100 0 Cad
xform 0 -256 384
p -320 576 100 0 1 FTVA:$(INA)
p -320 544 100 0 1 FTVB:$(INB)
p -320 512 100 0 1 FTVC:$(INC)
p -320 480 100 0 1 FTVD:$(IND)
p -320 416 100 0 1 FTVE:$(INE)
p -320 416 100 0 0 FTVF:LONG
p -320 384 100 0 0 FTVG:LONG
p -320 352 100 0 0 FTVH:STRING
p -432 -240 100 0 1 INAM:$(mech)$(op)$(I)Init
p -352 -160 100 0 1 SNAM:$(mech)$(op)$(I)
p -256 -208 100 1024 1 name:$(top)$(mech)$(op)$(I)
use inhier -592 759 100 0 CLID
xform 0 -576 800
use inhier -624 871 100 0 DIR
xform 0 -608 912
use inhier -1232 839 100 0 DOLA
xform 0 -1216 880
use inhier -1232 727 100 0 SLNKA
xform 0 -1216 768
use inhier -1280 455 100 0 SLNKB
xform 0 -1264 496
use inhier -1280 535 100 0 DOLB
xform 0 -1264 576
use estringouts -1088 695 100 0 A
xform 0 -960 768
p -1088 624 100 0 1 OMSL:closed_loop
p -992 640 100 1024 1 name:$(top)$(mech)$(op)$(I)
use estringouts -1136 423 100 0 B
xform 0 -1008 496
p -1136 352 100 0 1 OMSL:closed_loop
p -1024 384 100 1024 1 name:$(top)$(mech)$(op)$(I)
use bb200tr -1568 -536 -100 0 frame
xform 0 -288 288
p 208 -368 100 0 -1 author:Peter Ruckle
p 224 -400 100 0 -1 date:1-25-2001
p 464 -320 200 0 -1 filename:mech1Cad
p 432 -272 200 0 -1 system:GNIRS CC
[comments]
