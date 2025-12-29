[schematic2]
uniq 589
[tools]
[detail]
w -1342 1099 100 0 n#588 hwin.hwin#585.in -1312 1088 -1312 1088 mech.prbx.hall2B
w -1342 1131 100 0 n#587 hwin.hwin#586.in -1312 1120 -1312 1120 mech.prbx.hall2P
w -1342 651 100 0 n#584 hwin.hwin#581.in -1312 640 -1312 640 mech.prby.hall2B
w -1342 683 100 0 n#583 hwin.hwin#582.in -1312 672 -1312 672 mech.prby.hall2P
w -1342 1675 100 0 n#580 hwin.hwin#13.in -1312 1664 -1312 1664 mech.filt.hall1P
w -1342 1643 100 0 n#579 hwin.hwin#14.in -1312 1632 -1312 1632 mech.filt.hall1B
w -1342 2123 100 0 n#578 hwin.hwin#96.in -1312 2112 -1312 2112 mech.foc.hall1P
w -1342 2091 100 0 n#577 hwin.hwin#95.in -1312 2080 -1312 2080 mech.foc.hall1B
w -1342 1419 100 0 n#575 mech.filt.mmstat -1312 1408 -1312 1408 hwin.hwin#562.in
w -734 1419 100 0 n#573 hwout.hwout#561.outp -704 1408 -704 1408 mech.filt.mmreset
w -1342 971 100 0 n#572 hwin.hwin#569.in -1312 960 -1312 960 mech.prbx.mmstat
w -734 971 100 0 n#571 hwout.hwout#570.outp -704 960 -704 960 mech.prbx.mmreset
w -734 523 100 0 n#568 hwout.hwout#565.outp -704 512 -704 512 mech.prby.mmreset
w -1342 523 100 0 n#567 hwin.hwin#566.in -1312 512 -1312 512 mech.prby.mmstat
w -734 1867 100 0 n#560 hwout.hwout#557.outp -704 1856 -704 1856 mech.foc.mmreset
w -1342 1867 100 0 n#559 hwin.hwin#558.in -1312 1856 -1312 1856 mech.foc.mmstat
w -734 764 100 768 n#550 hwout.hwout#549.outp -704 768 -704 768 mech.prby.maddr
w -1342 1212 100 768 n#547 hwin.hwin#459.in -1312 1216 -1312 1216 mech.prbx.hall1P
w -1342 1180 100 768 n#546 hwin.hwin#458.in -1312 1184 -1312 1184 mech.prbx.hall1B
w -734 1219 100 0 n#477 hwout.hwout#499.outp -704 1216 -704 1216 mech.prbx.maddr
w -1342 771 100 0 n#449 hwin.hwin#456.in -1312 768 -1312 768 mech.prby.hall1P
w -1342 739 100 0 n#448 hwin.hwin#455.in -1312 736 -1312 736 mech.prby.hall1B
w -1342 1987 100 0 n#139 mech.foc.hall2B -1312 1984 -1312 1984 hwin.hwin#47.in
w -1342 2019 100 0 n#138 mech.foc.hall2P -1312 2016 -1312 2016 hwin.hwin#45.in
w -734 2115 100 0 n#128 hwout.hwout#6.outp -704 2112 -704 2112 mech.foc.maddr
w -734 1667 100 0 n#127 hwout.hwout#94.outp -704 1664 -704 1664 mech.filt.maddr
[cell use]
use hwin -1504 1079 100 0 hwin#586
xform 0 -1408 1120
p -1501 1112 100 0 -1 val(in):#C1 S21
use hwin -1504 1047 100 0 hwin#585
xform 0 -1408 1088
p -1501 1080 100 0 -1 val(in):#C1 S29
use hwin -1504 631 100 0 hwin#582
xform 0 -1408 672
p -1501 664 100 0 -1 val(in):#C1 S31
use hwin -1504 599 100 0 hwin#581
xform 0 -1408 640
p -1501 632 100 0 -1 val(in):#C1 S23
use hwin -1504 919 100 0 hwin#569
xform 0 -1408 960
p -1501 952 100 0 -1 val(in):#C0 S6
use hwin -1504 471 100 0 hwin#566
xform 0 -1408 512
p -1501 504 100 0 -1 val(in):#C0 S1
use hwin -1504 1175 100 0 hwin#459
xform 0 -1408 1216
p -1501 1208 100 0 -1 val(in):#C1 S20
use hwin -1504 1143 100 0 hwin#458
xform 0 -1408 1184
p -1501 1176 100 0 -1 val(in):#C1 S19
use hwin -1504 727 100 0 hwin#456
xform 0 -1408 768
p -1501 760 100 0 -1 val(in):#C1 S30
use hwin -1504 695 100 0 hwin#455
xform 0 -1408 736
p -1501 728 100 0 -1 val(in):#C1 S22
use hwin -1504 1623 100 0 hwin#13
xform 0 -1408 1664
p -1501 1656 100 0 -1 val(in):#C0 S5
use hwin -1504 1591 100 0 hwin#14
xform 0 -1408 1632
p -1501 1624 100 0 -1 val(in):#C0 S13
use hwin -1504 1975 100 0 hwin#45
xform 0 -1408 2016
p -1501 2008 100 0 -1 val(in):#C0 S4
use hwin -1504 1943 100 0 hwin#47
xform 0 -1408 1984
p -1501 1976 100 0 -1 val(in):#C0 S3
use hwin -1504 2039 100 0 hwin#95
xform 0 -1408 2080
p -1501 2072 100 0 -1 val(in):#C0 S10
use hwin -1504 2071 100 0 hwin#96
xform 0 -1408 2112
p -1501 2104 100 0 -1 val(in):#C0 S11
use hwin -1504 1815 100 0 hwin#558
xform 0 -1408 1856
p -1501 1848 100 0 -1 val(in):#C0 S5
use hwin -1504 1367 100 0 hwin#562
xform 0 -1408 1408
p -1501 1400 100 0 -1 val(in):#C0 S2
use hwout -704 919 100 768 hwout#570
xform 0 -608 960
p -608 951 100 0 -1 val(outp):#C0 S6
use hwout -704 471 100 768 hwout#565
xform 0 -608 512
p -608 503 100 0 -1 val(outp):#C0 S1
use hwout -704 727 100 768 hwout#549
xform 0 -608 768
p -608 759 100 0 -1 val(outp):#C1 S4
use hwout -704 2071 100 0 hwout#6
xform 0 -608 2112
p -608 2103 100 0 -1 val(outp):#C1 S2
use hwout -704 1623 100 0 hwout#94
xform 0 -608 1664
p -608 1655 100 0 -1 val(outp):#C1 S3
use hwout -704 1175 100 0 hwout#499
xform 0 -608 1216
p -608 1207 100 0 -1 val(outp):#C1 S0
use hwout -704 1815 100 0 hwout#557
xform 0 -608 1856
p -608 1847 100 0 -1 val(outp):#C0 S5
use hwout -704 1367 100 0 hwout#561
xform 0 -608 1408
p -608 1399 100 0 -1 val(outp):#C0 S2
use mech -1216 1248 100 768 prbx
xform 0 -1008 1056
p -1200 1152 100 768 -1 set1:name $(name) Probe X
p -1200 1120 100 768 -1 set2:top $(top)prbx
p -1200 1088 100 768 -1 set3:mtype Gimbal
p -1200 1056 100 768 -1 set4:ai1 XYCOM-566 SE Scanned
p -1200 1024 100 768 -1 set5:ai2 XYCOM-566 SE Scanned
p -1200 992 100 768 -1 set6:motor OMS VME8, VME44
p -1200 960 100 768 -1 set7:mostat XYCOM-240
p -1200 928 100 768 -1 set8:ident $(ident)Prbx
use mech -1216 2144 100 768 foc
xform 0 -1008 1952
p -1200 2048 100 768 -1 set1:name $(name) Focus
p -1200 2016 100 768 -1 set2:top $(top)foc
p -1200 1984 100 768 -1 set3:mtype Stage
p -1200 1952 100 768 -1 set4:ai1 XYCOM-566 SE Scanned
p -1200 1920 100 768 -1 set5:ai2 XYCOM-566 SE Scanned
p -1200 1888 100 768 -1 set6:motor OMS VME8, VME44
p -1200 1856 100 768 -1 set7:mostat XYCOM-240
p -1200 1824 100 768 -1 set8:ident $(ident)Foc
use mech -1216 1696 100 768 filt
xform 0 -1008 1504
p -1200 1600 100 768 -1 set1:name $(name) Filter
p -1200 1568 100 768 -1 set2:top $(top)filt
p -1200 1536 100 768 -1 set3:mtype Wheel
p -1200 1504 100 768 -1 set4:ai1 XYCOM-566 SE Scanned
p -1200 1440 100 768 -1 set6:motor OMS VME8, VME44
p -1200 1408 100 768 -1 set7:mostat XYCOM-240
p -1200 1376 100 768 -1 set8:ident $(ident)Filt
use mech -1216 800 100 768 prby
xform 0 -1008 608
p -1200 704 100 768 -1 set1:name $(name) Probe Y
p -1200 672 100 768 -1 set2:top $(top)prby
p -1200 640 100 768 -1 set3:mtype Gimbal
p -1200 608 100 768 -1 set4:ai1 XYCOM-566 SE Scanned
p -1200 576 100 768 -1 set5:ai2 XYCOM-566 SE Scanned
p -1200 544 100 768 -1 set6:motor OMS VME8, VME44
p -1200 512 100 768 -1 set7:mostat XYCOM-240
p -1200 480 100 768 -1 set8:ident $(ident)Prby
use common 1248 1863 100 0 common#399
xform 0 1408 2000
use eborderC 1600 -160 100 1280 $Id:
xform 0 16 1024
p 1136 -64 200 1536 -1 file:wfs.sch
p 1600 -160 100 1280 -1 id:$Id: wfs.sch,v 1.2 2009/05/27 19:34:05 fkraemer Exp $
[comments]
