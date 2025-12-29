[schematic2]
uniq 224
[tools]
[detail]
w -1630 1819 100 0 n#220 hwin.hwin#219.in -1600 1808 -1600 1808 ebis.in.INP
[cell use]
use hwin -1792 1767 100 0 hwin#219
xform 0 -1696 1808
p -1789 1800 100 0 -1 val(in):$(hwin)
use bc200tr -2032 744 -100 0 frame
xform 0 -352 2048
use ebis -1536 1840 100 0 in
xform 0 -1472 1776
p -1536 1696 100 768 1 DTYP:XYCOM-240
p -1536 1664 100 768 1 SCAN:.1 second
[comments]
