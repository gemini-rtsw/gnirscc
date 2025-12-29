[schematic2]
uniq 267
[tools]
[detail]
w 178 867 100 0 n#261 junction 416 864 0 864 0 1024 -224 1024 inhier.{numPics}.P
w 530 867 100 0 n#261 junction 416 864 704 864 elongouts.numPics.DOL
w 178 1443 100 0 n#235 inhier.{numLNRs}.P -224 1600 0 1600 0 1440 416 1440 junction
w 594 1475 100 0 n#235 junction 416 1440 544 1440 544 1472 704 1472 elongouts.numLNRs.DOL
w 130 1155 100 0 c#265 inhier.{numDAgs}.P -224 1152 544 1152 544 1056 704 1056 elongouts.numDAvgs.DOL
w 594 1923 100 0 n#230 junction 416 1888 544 1888 544 1920 704 1920 elongouts.numCoAdds.DOL
w 190 1891 100 0 n#230 inhier.{numCoAdds}.P -208 2048 24 2048 24 1888 416 1888 junction
w 980 803 100 0 n#198 elongouts.numPics.OUT 960 800 1060 800 hwout.hwout#174.outp
w 978 995 100 0 n#196 elongouts.numDAvgs.OUT 960 992 1056 992 hwout.hwout#173.outp
w 978 1411 100 0 n#197 elongouts.numLNRs.OUT 960 1408 1056 1408 hwout.hwout#172.outp
w 978 1859 100 0 n#195 elongouts.numCoAdds.OUT 960 1856 1056 1856 hwout.hwout#171.outp
[cell use]
use CBorder -748 -28 -100 0 frame
xform 0 932 1276
p 1820 108 100 1536 1 Date:23 Apr 97
p 1924 204 300 1792 -1 Dnumber:
p 2052 188 150 1536 -1 Title:setSeqVars.sch
use inhier -224 2007 100 0 {numCoAdds}
xform 0 -208 2048
use inhier -240 1559 100 0 {numLNRs}
xform 0 -224 1600
use inhier -240 1111 100 0 {numDAgs}
xform 0 -224 1152
use inhier -240 983 100 0 {numPics}
xform 0 -224 1024
use hwout 1056 1815 100 0 hwout#171
xform 0 1152 1856
p 1144 1856 65 1536 -1 val(outp):@Node=10,Var=6,Grp=-1,Idx=0
use hwout 1056 1367 100 0 hwout#172
xform 0 1152 1408
p 1166 1336 100 0 0 typ(outp):val
p 1140 1408 65 1536 -1 val(outp):@Node=10,Var=5,Grp=-1,Idx=0
use hwout 1056 951 100 0 hwout#173
xform 0 1152 992
p 1144 988 65 0 -1 val(outp):@Node=10,Var=12,Grp=-1,Idx=0
use hwout 1060 759 100 0 hwout#174
xform 0 1156 800
p 1140 800 65 1536 -1 val(outp):@Node=10,Var=2,Grp=-1,Idx=0
use elongouts 768 1960 100 0 numCoAdds
xform 0 832 1888
p 660 2280 100 0 0 DTYP:wFireVarMsg
p 792 1864 65 1536 1 PV:$(top)
use elongouts 768 1096 100 0 numDAvgs
xform 0 832 1024
p 660 1416 100 0 0 DTYP:wFireVarMsg
p 776 984 65 0 1 HIGH:16
p 776 968 65 0 1 HIHI:16
p 776 1008 65 1536 1 PV:$(top)
use elongouts 768 1512 100 0 numLNRs
xform 0 832 1440
p 848 1056 100 0 0 DTYP:wFireVarMsg
p 776 1404 65 0 1 HIGH:32
p 776 1384 65 0 1 HIHI:32
p 784 1424 65 1536 1 PV:$(top)
use elongouts 776 900 100 0 numPics
xform 0 832 832
p 660 1224 100 0 0 DTYP:wFireVarMsg
p 772 752 65 1536 1 PV:$(top)
use notes 1664 1927 100 0 notes#160
xform 0 1920 2112
p 1692 2238 100 0 -1 COMMENT1:This implements the setSeqVars command.
[comments]
