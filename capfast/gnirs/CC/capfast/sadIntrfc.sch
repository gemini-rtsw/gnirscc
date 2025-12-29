[schematic2]
uniq 148
[tools]
[detail]
w 2994 1995 100 0 n#147 hwout.hwout#30.outp 3024 1984 3024 1984 elongins.det1MaxRow.FLNK
w 2738 2011 100 0 n#146 elongins.det1MaxRow.INP 2768 2000 2768 2000 hwin.hwin#26.in
w 2760 1179 100 0 n#144 hwin.hwin#141.in 2784 1168 2784 1168 elongins.det3MaxRow.INP
w 3016 1163 100 0 n#143 hwout.hwout#140.outp 3040 1152 3040 1152 elongins.det3MaxRow.FLNK
w 3016 1579 100 0 n#136 elongins.det2MaxRow.FLNK 3040 1568 3040 1568 hwout.hwout#139.outp
w 2760 1595 100 0 n#135 elongins.det2MaxRow.INP 2784 1584 2784 1584 hwin.hwin#138.in
w 2760 1371 100 0 n#134 hwin.hwin#127.in 2784 1360 2784 1360 elongins.det3MaxCol.INP
w 3016 1355 100 0 n#133 hwout.hwout#125.outp 3040 1344 3040 1344 elongins.det3MaxCol.FLNK
w 3016 1771 100 0 n#116 elongins.det2MaxCol.FLNK 3040 1760 3040 1760 hwout.hwout#124.outp
w 2760 1787 100 0 n#115 elongins.det2MaxCol.INP 2784 1776 2784 1776 hwin.hwin#122.in
w 2638 2635 100 0 n#76 estringouts.errorTemp.FLNK 2656 2624 2656 2624 hwout.hwout#78.outp
w 3000 2235 100 0 n#29 hwout.hwout#28.outp 3024 2224 3024 2224 elongins.det1MaxCol.FLNK
w 2744 2251 100 0 n#25 hwin.hwin#24.in 2768 2240 2768 2240 elongins.det1MaxCol.INP
w 1144 2419 100 0 n#18 hwout.hwout#17.outp 1168 2416 1168 2416 estringouts.detID.FLNK
w 1144 2651 100 0 n#16 hwout.hwout#15.outp 1168 2640 1168 2640 estringouts.detType.FLNK
w 1144 2827 100 0 n#11 hwout.hwout#10.outp 1168 2816 1168 2816 estringouts.health.FLNK
w 1150 3003 100 0 n#9 hwout.hwout#8.outp 1168 2992 1168 2992 estringouts.state.FLNK
[cell use]
use elongins 2832 2272 100 0 det1MaxCol
xform 0 2896 2208
p 2880 2112 100 0 1 PINI:YES
use elongins 2848 1808 100 0 det2MaxCol
xform 0 2912 1744
p 2528 1774 100 0 0 PINI:YES
use elongins 2848 1392 100 0 det3MaxCol
xform 0 2912 1328
p 2528 1358 100 0 0 PINI:YES
use elongins 2848 1616 100 0 det2MaxRow
xform 0 2912 1552
p 2528 1582 100 0 0 PINI:YES
use elongins 2848 1200 100 0 det3MaxRow
xform 0 2912 1136
p 2528 1166 100 0 0 PINI:YES
use elongins 2832 2032 100 0 det1MaxRow
xform 0 2896 1968
p 2512 1998 100 0 0 PINI:YES
use hwin 2576 1959 100 0 hwin#26
xform 0 2672 2000
p 2579 1992 100 0 -1 val(in):2053
use hwin 2576 2199 100 0 hwin#24
xform 0 2672 2240
p 2579 2232 100 0 -1 val(in):9000
use hwin 2592 1735 100 0 hwin#122
xform 0 2688 1776
p 2595 1768 100 0 -1 val(in):9000
use hwin 2592 1319 100 0 hwin#127
xform 0 2688 1360
p 2595 1352 100 0 -1 val(in):9000
use hwin 2592 1543 100 0 hwin#138
xform 0 2688 1584
p 2595 1576 100 0 -1 val(in):2053
use hwin 2592 1127 100 0 hwin#141
xform 0 2688 1168
p 2595 1160 100 0 -1 val(in):2053
use hwout 2656 2583 100 0 hwout#78
xform 0 2752 2624
p 2752 2615 100 0 -1 val(outp):$(sadtop)errorTemp.VAL
use hwout 3024 1943 100 0 hwout#30
xform 0 3120 1984
p 3120 1975 100 0 -1 val(outp):$(sadtop)det1MaxRow.VAL
use hwout 3024 2183 100 0 hwout#28
xform 0 3120 2224
p 3120 2215 100 0 -1 val(outp):$(sadtop)det1MaxCol.VAL
use hwout 1168 2375 100 0 hwout#17
xform 0 1264 2416
p 1264 2407 100 0 -1 val(outp):$(sadtop)detID.VAL
use hwout 1168 2599 100 0 hwout#15
xform 0 1264 2640
p 1264 2631 100 0 -1 val(outp):$(sadtop)detType.VAL
use hwout 1168 2951 100 0 hwout#8
xform 0 1264 2992
p 1264 2983 100 0 -1 val(outp):$(sadtop)state.VAL
use hwout 1168 2775 100 0 hwout#10
xform 0 1264 2816
p 1264 2807 100 0 -1 val(outp):$(sadtop)health.VAL
use hwout 3040 1719 100 0 hwout#124
xform 0 3136 1760
p 3136 1751 100 0 -1 val(outp):$(sadtop)det2MaxCol.VAL
use hwout 3040 1303 100 0 hwout#125
xform 0 3136 1344
p 3136 1335 100 0 -1 val(outp):$(sadtop)det3MaxCol.VAL
use hwout 3040 1527 100 0 hwout#139
xform 0 3136 1568
p 3136 1568 100 0 -1 val(outp):$(sadtop)det2MaxRow.VAL
use hwout 3040 1111 100 0 hwout#140
xform 0 3136 1152
p 3136 1143 100 0 -1 val(outp):$(sadtop)det3MaxRow.VAL
use estringouts 2464 2672 100 0 errorTemp
xform 0 2528 2608
use estringouts 976 2688 100 0 detType
xform 0 1040 2624
p 1024 2512 100 0 1 PINI:YES
p 1024 2544 100 0 1 VAL:DETECTOR TYPE?
use estringouts 992 2464 100 0 detID
xform 0 1040 2400
p 1008 2288 100 0 1 PINI:YES
p 1008 2320 100 0 1 VAL:DETECTOR ID?
use estringouts 1008 3040 100 0 state
xform 0 1040 2976
use estringouts 992 2864 100 0 health
xform 0 1040 2800
use esirs 1936 3088 100 0 heartBeat
xform 0 1984 2944
p 1888 2720 100 0 1 FTVL:LONG
p 1888 2688 100 0 1 SCAN:Passive
p 1888 2752 100 0 1 SNAM:
use notes 3072 2663 100 0 notes#1
xform 0 3328 2848
p 3100 2974 100 0 -1 COMMENT1:This schematic contains many of the
p 3100 2942 100 0 -1 COMMENT2:records which provide input to the status
p 3100 2912 100 0 -1 COMMENT3:alarm database.  The SAD will most likely
p 3100 2880 100 0 -1 COMMENT4:reside on a separate IOC so there needs
p 3100 2848 100 0 -1 COMMENT5:to be some information retained in the
p 3100 2816 100 0 -1 COMMENT6:local database.  Most of these local
p 3100 2784 100 0 -1 COMMENT7:records are updated from the VxWorks
p 3100 2752 100 0 -1 COMMENT8:control tasks.  Data is transferred
p 3100 2720 100 0 -1 COMMENT9:between this database and the SAD using
use notes 3072 2295 100 0 notes#2
xform 0 3328 2480
p 3100 2606 100 0 -1 COMMENT1:links withing the databases.
p 3100 2544 100 0 -1 COMMENT3:.
use eborderC 528 711 100 0 eborderC#0
xform 0 2208 2016
p 3104 864 100 768 -1 author:Peter Ruckle
p 3088 832 100 768 -1 date:1-18-01
p 3328 912 200 768 -1 file:sadIntrfc.sch
p 3600 864 100 0 -1 page:1
p 3712 864 100 0 -1 pages:1
p 3376 864 100 0 -1 revision:0
p 3328 976 150 768 -1 system:Gnirs Components Controller
[comments]
