[schematic2]
uniq 194
[tools]
[detail]
w -1582 -21 -100 0 c#177 egenSub.CombHlt.SLNK -1440 -32 -1664 -32 inhier.SLNKA.P
w -1654 -53 -100 0 c#177 inhier.SLNKB.P -1664 -64 -1600 -64 -1600 -32 junction
w -1654 -85 -100 0 c#177 inhier.SLNKC.P -1664 -96 -1600 -96 -1600 -64 junction
w -1654 -117 -100 0 c#177 inhier.SLNKD.P -1664 -128 -1600 -128 -1600 -96 junction
w -1654 -149 -100 0 c#177 inhier.SLNKE.P -1664 -160 -1600 -160 -1600 -128 junction
w -1006 651 100 0 n#176 egenSub.CombHlt.OUTB -1152 576 -1088 576 -1088 640 -864 640 esirs.Health.IMSS
w -1038 683 100 0 n#175 egenSub.CombHlt.VALA -1152 672 -864 672 esirs.Health.INP
w -462 611 -100 0 c#149 bihier.OMSS.p -416 608 -448 608 esirs.Health.OMSS
w -450 643 -100 0 VAL bihier.VAL.p -416 640 -448 640 esirs.Health.VAL
w -448 675 -100 0 FLNK outhier.FLNK.p -400 672 -448 672 esirs.Health.FLNK
w -1582 139 -100 0 c#119 inhier.INPE.P -1664 128 -1440 128 egenSub.CombHlt.INPI
w -1582 75 -100 0 c#120 egenSub.CombHlt.INPJ -1440 64 -1664 64 inhier.IMSSE.P
w -1582 203 -100 0 c#117 egenSub.CombHlt.INPH -1440 192 -1664 192 inhier.IMSSD.P
w -1582 267 -100 0 c#118 inhier.INPD.P -1664 256 -1440 256 egenSub.CombHlt.INPG
w -1582 395 -100 0 c#111 inhier.INPC.P -1664 384 -1440 384 egenSub.CombHlt.INPE
w -1582 331 -100 0 c#112 egenSub.CombHlt.INPF -1440 320 -1664 320 inhier.IMSSC.P
w -1582 459 -100 0 c#109 egenSub.CombHlt.INPD -1440 448 -1664 448 inhier.IMSSB.P
w -1582 523 -100 0 c#110 inhier.INPB.P -1664 512 -1440 512 egenSub.CombHlt.INPC
w -1582 587 -100 0 IMSSA egenSub.CombHlt.INPB -1440 576 -1664 576 inhier.IMSSA.P
w -1582 651 -100 0 INPA inhier.INPA.P -1664 640 -1440 640 egenSub.CombHlt.INPA
s -1680 992 500 0 Template for NIRI schematics
s -80 -1168 400 1280 combSadHealth
n -2432 352 -1952 704 100
LARGE NOTEBOX
Edit the .sch file and type your
comments in here.
Please leave the dots as placeholders
for blank lines.
.
.
.
.
.
.
.
.
.
_
n -2432 768 -2080 1120 100
SMALL NOTEBOX
Edit the .sch file and
type your comments in here.
Please leave the dots as
placeholders for blank lines.
.
_
[cell use]
use inhier -1696 576 100 2048 IMSSA
xform 0 -1664 576
use inhier -1696 640 100 2048 INPA
xform 0 -1664 640
use inhier -1696 448 100 2048 IMSSB
xform 0 -1664 448
use inhier -1696 512 100 2048 INPB
xform 0 -1664 512
use inhier -1696 384 100 2048 INPC
xform 0 -1664 384
use inhier -1696 320 100 2048 IMSSC
xform 0 -1664 320
use inhier -1696 192 100 2048 IMSSD
xform 0 -1664 192
use inhier -1696 256 100 2048 INPD
xform 0 -1664 256
use inhier -1696 128 100 2048 INPE
xform 0 -1664 128
use inhier -1696 64 100 2048 IMSSE
xform 0 -1664 64
use inhier -1696 -32 100 2048 SLNKA
xform 0 -1664 -32
use inhier -1696 -64 100 2048 SLNKB
xform 0 -1664 -64
use inhier -1696 -96 100 2048 SLNKC
xform 0 -1664 -96
use inhier -1696 -128 100 2048 SLNKD
xform 0 -1664 -128
use inhier -1696 -160 100 2048 SLNKE
xform 0 -1664 -160
use esirs -800 704 100 768 Health
xform 0 -656 576
p -800 416 100 768 1 DESC:Health:  $(desc) (combined)
p -913 0 100 0 0 EGU:
p -913 304 100 0 0 FDSC:Health:  $(desc) (combined)
p -800 384 100 768 1 FTVL:STRING
p -800 320 100 768 1 PV:$(sadtop)$(mech)
p -800 352 100 768 1 SNAM:
use bihier -384 640 100 1536 VAL
xform 0 -416 640
use bihier -384 608 100 1536 OMSS
xform 0 -416 608
use outhier -384 672 100 1536 FLNK
xform 0 -416 672
use bc200tr -2624 -1304 -100 0 frame
xform 0 -944 0
use egenSub -1376 704 100 768 CombHlt
xform 0 -1296 304
p -1616 672 100 0 1 FTA:STRING
p -1616 608 100 0 1 FTB:STRING
p -1616 544 100 0 1 FTC:STRING
p -1616 480 100 0 1 FTD:STRING
p -1616 416 100 0 1 FTE:STRING
p -1616 352 100 0 1 FTF:STRING
p -1616 288 100 0 1 FTG:STRING
p -1616 224 100 0 1 FTH:STRING
p -1616 160 100 0 1 FTI:STRING
p -1616 96 100 0 1 FTJ:STRING
p -1072 672 100 0 1 FTVA:STRING
p -1072 608 100 0 1 FTVB:STRING
p -1072 544 100 0 1 FTVC:LONG
p -1376 -160 100 768 1 PV:$(sadtop)$(mech)
p -1376 -128 100 768 1 SNAM:cicsHealthCombine
p -1152 586 75 0 -1 pproc(OUTB):PP
[comments]
