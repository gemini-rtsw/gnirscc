[schematic2]
uniq 988
[tools]
[detail]
w -824 675 -100 0 c#987 outhier.OFLNK.p -768 672 -832 672 combVal.combVal#971.OFLNK
w -1166 683 100 0 n#985 comp2mCad.comp2m.OFLNK -1152 672 -1120 672 combVal.combVal#971.OSLNKA
w -1166 427 100 0 n#984 comp2mCadAlt.Sky.CVAL -1152 416 -1120 416 combVal.combVal#971.INPB
w -1166 395 100 0 n#983 comp2mCadAlt.Sky.CFLNK -1152 384 -1120 384 combVal.combVal#971.SLNKB
w -1166 651 100 0 n#982 comp2mCad.comp2m.CVAL -1152 640 -1120 640 combVal.combVal#971.INPA
w -1166 619 100 0 n#981 comp2mCad.comp2m.CFLNK -1152 608 -1120 608 combVal.combVal#971.SLNKA
w -824 611 -100 0 FLNK outhier.CFLNK.p -768 608 -832 608 combVal.combVal#971.FLNK
w -832 643 -100 0 CVAL combVal.combVal#971.VAL -832 640 -784 640 bihier.CVAL.p
w -2014 387 100 0 n#909 comp2mCadAlt.Sky.MESS -1920 384 -2048 384 eapplyx.SkyApply.INMB
w -2014 419 100 0 n#908 comp2mCadAlt.Sky.VAL -1920 416 -2048 416 eapplyx.SkyApply.INPB
w -2014 451 100 0 n#907 eapplyx.SkyApply.OCLB -2048 448 -1920 448 comp2mCadAlt.Sky.CLID
w -2014 483 100 0 n#906 comp2mCadAlt.Sky.DIR -1920 480 -2048 480 eapplyx.SkyApply.OUTB
w -2014 611 100 0 n#849 comp2mCad.comp2m.MESS -1920 608 -2048 608 eapplyx.SkyApply.INMA
w -2014 643 100 0 n#848 eapplyx.SkyApply.INPA -2048 640 -1920 640 comp2mCad.comp2m.VAL
w -2014 675 100 0 n#847 comp2mCad.comp2m.CLID -1920 672 -2048 672 eapplyx.SkyApply.OCLA
w -2014 707 100 0 n#846 eapplyx.SkyApply.OUTA -2048 704 -1920 704 comp2mCad.comp2m.DIR
w -2338 707 -100 0 DIR eapplyx.SkyApply.DIR -2304 704 -2336 704 bihier.DIR.p
w -2344 675 -100 0 CLID bihier.CLID.p -2336 672 -2304 672 eapplyx.SkyApply.CLID
w -2350 611 100 0 n#829 eapplyx.SkyApply.MESS -2304 608 -2336 608 bihier.MESS.p
w -2350 643 100 0 n#830 eapplyx.SkyApply.VAL -2304 640 -2336 640 bihier.VAL.p
s -2240 -1216 100 0 The "apply" record sequences the underlying
s -2240 -1248 100 0 CAR records in the order A, B, C, D, E
s -16 -1408 400 1280 prbCad
[cell use]
use outhier -752 656 100 768 OFLNK
xform 0 -784 672
use outhier -752 592 100 768 CFLNK
xform 0 -784 608
use bihier -2368 640 100 2048 VAL
xform 0 -2336 640
use bihier -2368 608 100 2048 MESS
xform 0 -2336 608
use bihier -2368 704 100 2048 DIR
xform 0 -2336 704
use bihier -2368 672 100 2048 CLID
xform 0 -2336 672
use bihier -752 640 100 1536 CVAL
xform 0 -784 640
use combVal -1104 -1081 100 0 combVal#971
xform 0 -976 -112
p -1120 -1024 100 768 1 set0:mech $(mech)Sky
use comp2mCadAlt -1776 512 100 1024 Sky
xform 0 -1536 448
p -1840 480 100 768 1 set0:alt Sky
p -1520 480 100 768 1 set1:prefix Sky
use comp2mCad -1776 736 100 1024 comp2m
xform 0 -1536 672
use notes -800 -1161 100 0 notes#394
xform 0 -544 -976
p -800 -1184 100 768 0 author:S.M. Beard
p -784 -864 100 768 -1 comment0:To add new commands for this particular
p -784 -896 100 768 -1 comment1:type of component, attach more CAD
p -784 -928 100 768 -1 comment2:records to the APPLY record here, or
p -784 -960 100 768 -1 comment3:add more lower level schematics
p -784 -992 100 768 -1 comment4:connected to the APPLY record.
p -544 -832 100 1024 -1 title:ICD 14
use bc200tr -2560 -1528 -100 0 frame
xform 0 -880 -224
p 0 -1360 100 0 1 author:H.T.Yamada
p 240 -1376 100 0 -1 border:C
p 0 -1392 100 0 1 checked:H.T.Yamada
p 272 -1376 100 0 -1 date:1999-10-08
p 524 -1372 100 1792 -1 page:1
p 240 -1248 100 0 -1 project:Near Infra-Red Imager
p -2560 -1480 100 0 0 revision:1.0
p 240 -1312 100 0 -1 title:Two-Axis CAD Records
use eapplyx -2240 880 100 0 SkyApply
xform 0 -2176 -80
p -2240 -1056 100 768 1 DESC:CICS $(mech) APPLY Record
p -2240 -1088 100 768 1 PV:$(top)$(mech)
[comments]
