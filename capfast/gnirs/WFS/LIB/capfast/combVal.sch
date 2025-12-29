[schematic2]
uniq 611
[tools]
[detail]
w -918 99 -100 0 c#610 inhier.OSLNKA.P -944 96 -832 96 outhier.OFLNK.p
w -950 75 -100 0 c#610 inhier.OSLNKB.P -944 64 -896 64 -896 96 junction
w -950 43 -100 0 c#610 inhier.OSLNKC.P -944 32 -896 32 -896 64 junction
w -950 11 -100 0 c#610 inhier.OSLNKD.P -944 0 -896 0 -896 32 junction
w -950 -21 -100 0 c#610 inhier.OSLNKE.P -944 -32 -896 -32 -896 0 junction
w -950 -53 -100 0 c#610 inhier.OSLNKF.P -944 -64 -896 -64 -896 -32 junction
w -950 -85 -100 0 c#610 inhier.OSLNKG.P -944 -96 -896 -96 -896 -64 junction
w -950 -117 -100 0 c#610 inhier.OSLNKH.P -944 -128 -896 -128 -896 -96 junction
w -918 419 -100 0 c#580 inhier.SLNKA.P -944 416 -832 416 ecalcs.Val.SLNK
w -950 395 -100 0 c#580 inhier.SLNKB.P -944 384 -896 384 -896 416 junction
w -950 363 -100 0 c#580 inhier.SLNKC.P -944 352 -896 352 -896 384 junction
w -950 331 -100 0 c#580 inhier.SLNKD.P -944 320 -896 320 -896 352 junction
w -950 299 -100 0 c#580 inhier.SLNKE.P -944 288 -896 288 -896 320 junction
w -950 267 -100 0 c#580 inhier.SLNKF.P -944 256 -896 256 -896 288 junction
w -950 235 -100 0 c#580 inhier.SLNKG.P -944 224 -896 224 -896 256 junction
w -950 203 -100 0 c#580 inhier.SLNKH.P -944 192 -896 192 -896 224 junction
w -550 619 -100 0 VAL bihier.VAL.p -496 608 -544 608 ecalcs.Val.VAL
w -542 651 -100 0 FLNK ecalcs.Val.FLNK -544 640 -480 640 outhier.FLNK.p
w -918 579 -100 0 c#570 inhier.INPH.P -944 576 -832 576 ecalcs.Val.INPH
w -918 611 -100 0 c#566 inhier.INPG.P -944 608 -832 608 ecalcs.Val.INPG
w -918 643 -100 0 c#568 inhier.INPF.P -944 640 -832 640 ecalcs.Val.INPF
w -918 675 -100 0 c#567 inhier.INPE.P -944 672 -832 672 ecalcs.Val.INPE
w -918 739 100 0 n#557 inhier.INPC.P -944 736 -832 736 ecalcs.Val.INPC
w -918 707 100 0 n#556 inhier.INPD.P -944 704 -832 704 ecalcs.Val.INPD
w -918 771 100 0 n#554 inhier.INPB.P -944 768 -832 768 ecalcs.Val.INPB
w -912 803 -100 0 INPA inhier.INPA.P -944 800 -832 800 ecalcs.Val.INPA
s -16 -1264 400 1280 val
s -896 928 100 768 Combines values into a single
s -896 896 100 768 value, which is an error value
s -896 864 100 768 if any single value is an error.
[cell use]
use outhier -816 80 100 768 OFLNK
xform 0 -848 96
use inhier -976 96 100 2048 OSLNKA
xform 0 -944 96
use inhier -976 64 100 2048 OSLNKB
xform 0 -944 64
use inhier -976 32 100 2048 OSLNKC
xform 0 -944 32
use inhier -976 0 100 2048 OSLNKD
xform 0 -944 0
use inhier -976 -32 100 2048 OSLNKE
xform 0 -944 -32
use inhier -976 -64 100 2048 OSLNKF
xform 0 -944 -64
use inhier -976 -96 100 2048 OSLNKG
xform 0 -944 -96
use inhier -976 -128 100 2048 OSLNKH
xform 0 -944 -128
use inhier -976 800 100 2048 INPA
xform 0 -944 800
use inhier -976 416 100 2048 SLNKA
xform 0 -944 416
use inhier -976 768 100 2048 INPB
xform 0 -944 768
use inhier -976 736 100 2048 INPC
xform 0 -944 736
use inhier -976 704 100 2048 INPD
xform 0 -944 704
use inhier -976 608 100 2048 INPG
xform 0 -944 608
use inhier -976 672 100 2048 INPE
xform 0 -944 672
use inhier -976 640 100 2048 INPF
xform 0 -944 640
use inhier -976 576 100 2048 INPH
xform 0 -944 576
use inhier -976 384 100 2048 SLNKB
xform 0 -944 384
use inhier -976 352 100 2048 SLNKC
xform 0 -944 352
use inhier -976 320 100 2048 SLNKD
xform 0 -944 320
use inhier -976 288 100 2048 SLNKE
xform 0 -944 288
use inhier -976 256 100 2048 SLNKF
xform 0 -944 256
use inhier -976 224 100 2048 SLNKG
xform 0 -944 224
use inhier -976 192 100 2048 SLNKH
xform 0 -944 192
use bihier -464 608 100 1536 VAL
xform 0 -496 608
use outhier -464 624 100 768 FLNK
xform 0 -496 640
use ecalcs -768 832 100 768 Val
xform 0 -688 592
p -768 288 100 768 1 CALC:-{A||B||C||D||E||F||G||H}
p -768 320 100 768 1 PV:$(top)$(mech)
use bc200tr -2560 -1400 -100 0 frame
xform 0 -880 -96
p 0 -1232 100 0 1 author:H.T.Yamada
p 240 -1248 100 0 -1 border:C
p 0 -1264 100 0 1 checked:H.T.Yamada
p 272 -1248 100 0 -1 date:1998-04-15
p 524 -1244 100 1792 -1 page:1
p 240 -1120 100 0 -1 project:Near Infra-Red Imager
p 240 -1184 100 0 -1 title:One axis continuous component CAD records
[comments]
