[schematic2]
uniq 259
[tools]
[detail]
w 298 867 100 0 n#226 elongouts.curCols.OUT 296 864 360 864 hwout.hwout#225.outp
w 294 1059 100 0 n#223 elongouts.curRows.OUT 296 1056 352 1056 hwout.hwout#224.outp
w 298 1443 100 0 n#216 elongouts.rowHigh.OUT 296 1440 360 1440 hwout.hwout#215.outp
w 294 1251 100 0 n#213 elongouts.colHigh.OUT 296 1248 352 1248 hwout.hwout#214.outp
w 298 1635 100 0 n#193 elongouts.colLow.OUT 296 1632 360 1632 hwout.hwout#206.outp
w 298 1827 100 0 n#192 elongouts.rowLow.OUT 296 1824 360 1824 hwout.hwout#207.outp
[cell use]
use CBorder -892 -96 -100 0 frame
xform 0 788 1208
p 1676 40 100 1536 1 Date:23 Apr 97
p 1780 136 300 1792 -1 Dnumber:
p 1908 120 150 1536 -1 Title:setSeqROI.sch
use notes 1632 1899 100 0 notes#160
xform 0 1888 2084
p 1660 2210 100 0 -1 COMMENT1:This implements the setSeqROI command
p 1660 2178 100 0 -1 COMMENT2:which defines the data reduction region
p 1660 2148 100 0 -1 COMMENT3:of interest in the DataCube software.
p 1660 2116 100 0 -1 COMMENT4:.
use notes -704 1575 100 0 notes#254
xform 0 -448 1760
p -676 1886 100 0 -1 COMMENT1:Routine: setSeqROICmd
p -672 1872 65 1536 -1 COMMENT2:Purpose: setSeqROI command handles all of the variables and
p -680 1856 65 1536 -1 COMMENT3:     parameters which change as a result of changing the size
p -680 1840 65 1536 -1 COMMENT4:     of the central portion of the array which is read out.  
p -680 1824 65 1536 -1 COMMENT5:    It also handles the display of the appropriate array ROI.
p -680 1808 65 1536 -1 COMMENT6:     for the user.
p -672 1784 65 1536 -1 COMMENT7:Called By: - the obsSetupCad genSub record routine returns to
p -680 1768 65 1536 -1 COMMENT8:     that routine after completion.
p -672 1736 65 1536 -1 COMMENT9:Parameters:
p -680 1720 65 1536 -1 COMMENTA:     seqRoiSize - long - size of the central ROI to readout.
p -568 1704 65 1536 -1 COMMENTB:    The allowed size is restricted to 1024, 768,
p -568 1688 65 1536 -1 COMMENTC:    512, 384, 256 or 128 square 
use elongins 1636 1196 100 0 curMaxRow
xform 0 1700 1140
p 1652 1116 65 1536 1 PV:$(top)
use elongins 1632 1376 100 0 curMaxCol
xform 0 1696 1320
p 1640 1296 65 1536 1 PV:$(top)
use elongins 1632 1568 100 0 maxCol
xform 0 1696 1512
p 1312 1542 100 0 0 PINI:YES
p 1648 1488 65 1536 1 PV:$(top)
p 1636 1436 65 1536 1 VAL:1024
use elongins 1632 1760 100 0 maxRow
xform 0 1696 1704
p 1312 1734 100 0 0 PINI:YES
p 1640 1680 65 1536 1 PV:$(top)
p 1636 1628 65 1536 1 VAL:1024
use ebis 1128 936 100 0 is128
xform 0 1188 880
p 836 718 100 0 0 ONAM:TRUE
p 1128 808 100 0 1 PV:$(top)
p 836 750 100 0 0 ZNAM:FALSE
use ebis 1128 1128 100 0 is256
xform 0 1188 1072
p 836 910 100 0 0 ONAM:TRUE
p 1128 1000 100 0 1 PV:$(top)
p 836 942 100 0 0 ZNAM:FALSE
use ebis 1128 1320 100 0 is384
xform 0 1188 1264
p 836 1102 100 0 0 ONAM:TRUE
p 1128 1192 100 0 1 PV:$(top)
p 836 1134 100 0 0 ZNAM:FALSE
use ebis 1128 1512 100 0 is512
xform 0 1188 1456
p 836 1294 100 0 0 ONAM:TRUE
p 1128 1384 100 0 1 PV:$(top)
p 836 1326 100 0 0 ZNAM:FALSE
use ebis 1128 1704 100 0 is768
xform 0 1188 1648
p 836 1486 100 0 0 ONAM:TRUE
p 1128 1576 100 0 1 PV:$(top)
p 836 1518 100 0 0 ZNAM:FALSE
use ebis 1128 1896 100 0 is1024
xform 0 1188 1840
p 836 1678 100 0 0 ONAM:1
p 1132 1800 65 1536 1 PV:$(top)
p 1136 1776 65 1536 1 VAL:1
p 836 1710 100 0 0 ZNAM:0
use hwout 360 1591 100 0 hwout#206
xform 0 456 1632
p 444 1632 65 1536 -1 val(outp):@HKc15[11]
use hwout 360 1783 100 0 hwout#207
xform 0 456 1824
p 440 1824 65 1536 -1 val(outp):@HKc15[10]
use hwout 352 1207 100 0 hwout#214
xform 0 448 1248
p 436 1248 65 1536 -1 val(outp):@HKc15[13]
use hwout 360 1399 100 0 hwout#215
xform 0 456 1440
p 440 1440 65 1536 -1 val(outp):@HKc15[12]
use hwout 352 1015 100 0 hwout#224
xform 0 448 1056
p 432 1056 65 1536 -1 val(outp):@HKc15[14]
use hwout 360 823 100 0 hwout#225
xform 0 456 864
p 444 864 65 1536 -1 val(outp):@HKc15[15]
use elongouts 104 1736 100 0 colLow
xform 0 168 1664
p -4 2056 100 0 0 DTYP:vxWorks Variable (INST_IO)
p 112 1584 65 1536 1 PV:$(top)
use elongouts 104 1924 100 0 rowLow
xform 0 168 1856
p -4 2248 100 0 0 DTYP:vxWorks Variable (INST_IO)
p 108 1780 65 1536 1 PV:$(top)
use elongouts 104 1352 100 0 colHigh
xform 0 168 1280
p -4 1672 100 0 0 DTYP:vxWorks Variable (INST_IO)
p 112 1200 65 1536 1 PV:$(top)
use elongouts 104 1540 100 0 rowHigh
xform 0 168 1472
p -4 1864 100 0 0 DTYP:vxWorks Variable (INST_IO)
p 108 1396 65 1536 1 PV:$(top)
use elongouts 104 1156 100 0 curRows
xform 0 168 1088
p -4 1480 100 0 0 DTYP:vxWorks Variable (INST_IO)
p 108 1012 65 1536 1 PV:$(top)
use elongouts 104 968 100 0 curCols
xform 0 168 896
p -4 1288 100 0 0 DTYP:vxWorks Variable (INST_IO)
p 116 816 65 1536 1 PV:$(top)
[comments]
