[schematic2]
uniq 54
[tools]
[detail]
w 248 283 100 0 c#52 inhier.SLF.P 176 272 368 272 efanouts.Fanf.SLNK
w 720 355 100 0 c#51 efanouts.Fanf.LNK1 608 352 880 352 outhier.FLF1.p
w 656 331 100 0 c#50 efanouts.Fanf.LNK2 608 320 752 320 752 208 880 208 outhier.FLF2.p
w 656 715 100 0 c#41 efanouts.Fane.LNK2 608 704 752 704 752 592 880 592 outhier.FLE2.p
w 720 739 100 0 c#40 efanouts.Fane.LNK1 608 736 880 736 outhier.FLE1.p
w 248 667 100 0 c#39 inhier.SLE.P 176 656 368 656 efanouts.Fane.SLNK
w 656 1099 100 0 c#34 efanouts.Fand.LNK2 608 1088 752 1088 752 976 880 976 outhier.FLD2.p
w 720 1123 100 0 c#33 efanouts.Fand.LNK1 608 1120 880 1120 outhier.FLD1.p
w 248 1051 100 0 c#32 inhier.SLD.P 176 1040 368 1040 efanouts.Fand.SLNK
w 248 1435 100 0 c#31 inhier.SLC.P 176 1424 368 1424 efanouts.Fanc.SLNK
w 720 1507 100 0 c#30 efanouts.Fanc.LNK1 608 1504 880 1504 outhier.FLC1.p
w 656 1483 100 0 c#29 efanouts.Fanc.LNK2 608 1472 752 1472 752 1360 880 1360 outhier.FLC2.p
w 656 1867 100 0 c#20 efanouts.Fanb.LNK2 608 1856 752 1856 752 1744 880 1744 outhier.FLB2.p
w 720 1891 100 0 c#19 efanouts.Fanb.LNK1 608 1888 880 1888 outhier.FLB1.p
w 248 1819 100 0 c#18 inhier.SLB.P 176 1808 368 1808 efanouts.Fanb.SLNK
w 656 2251 100 0 n#17 efanouts.Fana.LNK2 608 2240 752 2240 752 2128 880 2128 outhier.FLA2.p
w 720 2275 100 0 n#16 efanouts.Fana.LNK1 608 2272 880 2272 outhier.FLA1.p
w 248 2203 100 0 n#15 inhier.SLA.P 176 2192 368 2192 efanouts.Fana.SLNK
s 1488 80 500 512 mfanout.sch
n -864 80 -384 432 100
LARGE NOTEBOX
This schematic may be used in places
where multiple fanouts are required.
There are six input links, and each
input link is fanned out to two
outputs, making 12 outputs in total.
.
A typical use of this multiple fanout
is in the schematic for a two axis
mechanism, to direct the processing
to both axes simultaneously.
.
.
.
_
[cell use]
use inhier 160 231 100 0 SLF
xform 0 176 272
use outhier 848 311 100 0 FLF1
xform 0 864 352
use outhier 848 167 100 0 FLF2
xform 0 864 208
use efanouts 368 135 100 0 Fanf
xform 0 488 288
p 416 64 100 0 1 PV:$(top)$(mech):
p 416 96 100 0 1 SELM:All
use efanouts 368 519 100 0 Fane
xform 0 488 672
p 416 464 100 0 1 PV:$(top)$(mech):
p 416 480 100 0 1 SELM:All
use outhier 848 551 100 0 FLE2
xform 0 864 592
use outhier 848 695 100 0 FLE1
xform 0 864 736
use inhier 160 615 100 0 SLE
xform 0 176 656
use efanouts 368 903 100 0 Fand
xform 0 488 1056
p 416 832 100 0 1 PV:$(top)$(mech):
p 416 864 100 0 1 SELM:All
use outhier 848 935 100 0 FLD2
xform 0 864 976
use outhier 848 1079 100 0 FLD1
xform 0 864 1120
use inhier 160 999 100 0 SLD
xform 0 176 1040
use inhier 160 1383 100 0 SLC
xform 0 176 1424
use outhier 848 1463 100 0 FLC1
xform 0 864 1504
use outhier 848 1319 100 0 FLC2
xform 0 864 1360
use efanouts 368 1287 100 0 Fanc
xform 0 488 1440
p 416 1232 100 0 1 PV:$(top)$(mech):
p 416 1264 100 0 1 SELM:All
use efanouts 368 1671 100 0 Fanb
xform 0 488 1824
p 416 1600 100 0 1 PV:$(top)$(mech):
p 416 1632 100 0 1 SELM:All
use outhier 848 1703 100 0 FLB2
xform 0 864 1744
use outhier 848 1847 100 0 FLB1
xform 0 864 1888
use inhier 160 1767 100 0 SLB
xform 0 176 1808
use efanouts 368 2055 100 0 Fana
xform 0 488 2208
p 416 2000 100 0 1 PV:$(top)$(mech):
p 416 2032 100 0 1 SELM:All
use outhier 848 2087 100 0 FLA2
xform 0 864 2128
use outhier 848 2231 100 0 FLA1
xform 0 864 2272
use inhier 160 2151 100 0 SLA
xform 0 176 2192
use bc200tr -1024 -104 -100 0 frame
xform 0 656 1200
p 1552 64 100 0 1 author:S.M.Beard
p 1776 48 100 0 -1 border:C
p 1552 32 100 0 1 checked:S.M.Beard
p 1808 48 100 0 -1 date:21 Jan 97
p 1792 176 100 0 -1 project:Core Instrument Control System
p 1792 112 100 0 -1 title:Multiple fanout schematic
[comments]
