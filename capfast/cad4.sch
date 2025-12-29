[schematic2]
uniq 68
[tools]
[detail]
w 2112 1099 100 0 OMSS ecars.C.OMSS 2048 1152 2096 1152 2096 1088 2176 1088 outhier.OMSS.p
w 1944 1419 100 0 OVAL ecars.C.VAL 2048 1216 2048 1408 outhier.OVAL.p
w 1128 1227 100 0 n#66 ecad8.Cad.OUTH 464 784 576 784 576 1216 1728 1216 ecars.C.IVAL
w 1176 1035 100 0 n#65 ecad8.Cad.STLK 464 592 672 592 672 1024 1728 1024 ecars.C.SLNK
w 1064 347 100 0 n#61 elongouts.elongouts#56.VAL 960 256 1040 256 1040 336 1136 336 eseqs.eseqs#51.DOL1
w 1012 147 100 0 n#60 elongouts.elongouts#56.FLNK 960 288 1008 288 1008 16 1136 16 eseqs.eseqs#51.SLNK
w 680 299 100 0 n#59 hwin.hwin#58.in 704 288 704 288 elongouts.elongouts#56.DOL
w 768 1363 100 0 n#50 ecad8.Cad.OCID 464 1360 1120 1360 1120 1184 1728 1184 ecars.C.ICID
w 2072 995 100 0 FLNK outhier.FLNK.p 2144 992 2048 992 ecars.C.FLNK
w 2072 1187 100 0 OCID bihier.OCID.p 2144 1184 2048 1184 ecars.C.CLID
w 40 1435 100 0 CLID inhier.CLID.P -80 1488 -16 1488 -16 1424 144 1424 ecad8.Cad.ICID
w 536 1427 100 0 MESS ecad8.Cad.MESS 464 1424 656 1424 outhier.MESS.p
w 558 1555 100 0 VAL ecad8.Cad.VAL 464 1456 496 1456 496 1552 656 1552 outhier.VAL.p
w 12 1515 100 0 n#39 inhier.DIR.P -80 1584 16 1584 16 1456 144 1456 ecad8.Cad.DIR
w -696 731 100 0 SLNKD inhier.SLNKD.P -752 720 -592 720 -592 752 -496 752 estringouts.stringD.SLNK
w -696 827 100 0 DOLD inhier.DOLD.P -752 816 -592 816 -592 784 -496 784 estringouts.stringD.DOL
w -696 955 100 0 SLNKC inhier.SLNKC.P -752 944 -592 944 -592 1008 -496 1008 estringouts.StringC.SLNK
w -648 1051 100 0 DOLC inhier.DOLC.P -752 1040 -496 1040 estringouts.StringC.DOL
w -712 1403 100 0 SLNKA inhier.SLNKA.P -752 1392 -624 1392 -624 1424 -512 1424 estringouts.StringA.SLNK
w -696 1179 100 0 SLNKB inhier.SLNKB.P -752 1168 -592 1168 -592 1232 -512 1232 estringouts.StringB.SLNK
w -656 1275 100 0 DOLB inhier.DOLB.P -752 1264 -512 1264 estringouts.StringB.DOL
w -696 1531 100 0 DOLA inhier.DOLA.P -752 1520 -592 1520 -592 1456 -512 1456 estringouts.StringA.DOL
w -84 899 100 0 n#55 estringouts.stringD.OUT -240 736 -80 736 -80 1072 144 1072 ecad8.Cad.D
w -16 1139 100 0 n#54 estringouts.StringC.OUT -240 992 -128 992 -128 1136 144 1136 ecad8.Cad.C
w -24 1203 100 0 n#53 estringouts.StringB.OUT -256 1216 -144 1216 -144 1200 144 1200 ecad8.Cad.B
w 16 1267 100 0 n#52 estringouts.StringA.OUT -256 1408 -64 1408 -64 1264 144 1264 ecad8.Cad.A
[cell use]
use outhier 2144 1047 100 0 OMSS
xform 0 2160 1088
use outhier 2112 951 100 0 FLNK
xform 0 2128 992
use outhier 624 1383 100 0 MESS
xform 0 640 1424
use outhier 624 1511 100 0 VAL
xform 0 640 1552
use outhier 2016 1367 100 0 OVAL
xform 0 2032 1408
use hwin 512 247 100 0 hwin#58
xform 0 608 288
p 515 280 100 0 -1 val(in):$(CAR_BUSY)
use elongouts 704 167 100 0 elongouts#56
xform 0 832 256
use eseqs 1136 -73 100 0 eseqs#51
xform 0 1296 176
use ecars 1728 935 100 0 C
xform 0 1888 1104
p 1840 928 100 1024 0 name:$(top)$(name)$(I)
use bihier 2128 1143 100 0 OCID
xform 0 2144 1184
use ecad8 144 503 100 0 Cad
xform 0 304 1008
p 224 1232 100 0 1 FTVA:LONG
p 224 1168 100 0 1 FTVB:LONG
p 224 1104 100 0 1 FTVC:LONG
p 224 1040 100 0 1 FTVD:LONG
p 240 976 100 0 0 FTVH:LONG
p 224 480 100 0 1 INAM:$(name)InitCad
p 224 432 100 0 1 SNAM:$(name)Cad
p 368 384 100 1024 1 name:$(top)$(name)
use inhier -96 1543 100 0 DIR
xform 0 -80 1584
use inhier -768 679 100 0 SLNKD
xform 0 -752 720
use inhier -768 775 100 0 DOLD
xform 0 -752 816
use inhier -768 903 100 0 SLNKC
xform 0 -752 944
use inhier -768 999 100 0 DOLC
xform 0 -752 1040
use inhier -768 1127 100 0 SLNKB
xform 0 -752 1168
use inhier -768 1223 100 0 DOLB
xform 0 -752 1264
use inhier -768 1351 100 0 SLNKA
xform 0 -752 1392
use inhier -768 1479 100 0 DOLA
xform 0 -752 1520
use inhier -96 1447 100 0 CLID
xform 0 -80 1488
use bc200tr -1008 -264 -100 0 frame
xform 0 672 1040
p 1584 -96 100 0 -1 author:Peter Ruckle
p 1584 -128 100 0 -1 date:1-26-2001
p 1824 -48 200 0 -1 filename:cad4.sch
p 1824 16 200 0 -1 system:GNIRS CC
use estringouts -496 679 100 0 stringD
xform 0 -368 752
p -512 624 100 0 1 OMSL:closed_loop
p -368 640 100 1024 1 name:$(top)$(name)$(I)
use estringouts -496 935 100 0 StringC
xform 0 -368 1008
p -496 864 100 0 1 OMSL:closed_loop
p -368 896 100 1024 1 name:$(top)$(name)$(I)
use estringouts -512 1159 100 0 StringB
xform 0 -384 1232
p -512 1104 100 0 1 OMSL:closed_loop
p -384 1120 100 1024 1 name:$(top)$(name)$(I)
use estringouts -512 1351 100 0 StringA
xform 0 -384 1424
p -512 1328 100 0 1 OMSL:closed_loop
p -352 1296 100 1024 1 name:$(top)$(name)$(I)
[comments]
