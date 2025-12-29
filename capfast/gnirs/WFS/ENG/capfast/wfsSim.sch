[schematic2]
uniq 405
[tools]
[detail]
w -1342 1091 100 0 n#143 mech.prbx.hall2B -1312 1088 -1312 1088 hwin.hwin#107.in
w -1342 1123 100 0 n#142 mech.prbx.hall2P -1312 1120 -1312 1120 hwin.hwin#108.in
w -1342 1187 100 0 n#141 mech.prbx.hall1B -1312 1184 -1312 1184 hwin.hwin#111.in
w -1342 1219 100 0 n#140 mech.prbx.hall1P -1312 1216 -1312 1216 hwin.hwin#112.in
w -1342 1987 100 0 n#139 mech.foc.hall2B -1312 1984 -1312 1984 hwin.hwin#47.in
w -1342 2019 100 0 n#138 mech.foc.hall2P -1312 2016 -1312 2016 hwin.hwin#45.in
w -1342 2083 100 0 n#137 mech.foc.hall1B -1312 2080 -1312 2080 hwin.hwin#14.in
w -1342 2115 100 0 n#136 mech.foc.hall1P -1312 2112 -1312 2112 hwin.hwin#13.in
w -1342 1635 100 0 n#135 mech.filt.hall1B -1312 1632 -1312 1632 hwin.hwin#95.in
w -1342 1667 100 0 n#134 mech.filt.hall1P -1312 1664 -1312 1664 hwin.hwin#96.in
w -1342 643 100 0 n#133 mech.prby.hall2B -1312 640 -1312 640 hwin.hwin#125.in
w -1342 675 100 0 n#132 mech.prby.hall2P -1312 672 -1312 672 hwin.hwin#124.in
w -1342 739 100 0 n#131 mech.prby.hall1B -1312 736 -1312 736 hwin.hwin#123.in
w -1342 771 100 0 n#130 mech.prby.hall1P -1312 768 -1312 768 hwin.hwin#122.in
w -734 1219 100 0 n#129 hwout.hwout#106.outp -704 1216 -704 1216 mech.prbx.maddr
w -734 2115 100 0 n#128 hwout.hwout#6.outp -704 2112 -704 2112 mech.foc.maddr
w -734 1667 100 0 n#127 hwout.hwout#94.outp -704 1664 -704 1664 mech.filt.maddr
w -734 771 100 0 n#126 hwout.hwout#121.outp -704 768 -704 768 mech.prby.maddr
[cell use]
use common 1248 1863 100 0 common#399
xform 0 1408 2000
use hwin -1504 599 100 0 hwin#125
xform 0 -1408 640
p -1501 632 100 0 -1 val(in):@wfs:prby 2b
use hwin -1504 631 100 0 hwin#124
xform 0 -1408 672
p -1501 664 100 0 -1 val(in):@wfs:prby 2p
use hwin -1504 695 100 0 hwin#123
xform 0 -1408 736
p -1501 728 100 0 -1 val(in):@wfs:prby 1b
use hwin -1504 727 100 0 hwin#122
xform 0 -1408 768
p -1501 760 100 0 -1 val(in):@wfs:prby 1p
use hwin -1504 1623 100 0 hwin#96
xform 0 -1408 1664
p -1501 1656 100 0 -1 val(in):@wfs:filt 1p
use hwin -1504 1591 100 0 hwin#95
xform 0 -1408 1632
p -1501 1624 100 0 -1 val(in):@wfs:filt 1b
use hwin -1504 1943 100 0 hwin#47
xform 0 -1408 1984
p -1501 1976 100 0 -1 val(in):@wfs:foc 2b
use hwin -1504 1975 100 0 hwin#45
xform 0 -1408 2016
p -1501 2008 100 0 -1 val(in):@wfs:foc 2p
use hwin -1504 2039 100 0 hwin#14
xform 0 -1408 2080
p -1501 2072 100 0 -1 val(in):@wfs:foc 1b
use hwin -1504 2071 100 0 hwin#13
xform 0 -1408 2112
p -1501 2104 100 0 -1 val(in):@wfs:foc 1p
use hwin -1504 1047 100 0 hwin#107
xform 0 -1408 1088
p -1501 1080 100 0 -1 val(in):@wfs:prbx 2b
use hwin -1504 1079 100 0 hwin#108
xform 0 -1408 1120
p -1501 1112 100 0 -1 val(in):@wfs:prbx 2p
use hwin -1504 1143 100 0 hwin#111
xform 0 -1408 1184
p -1501 1176 100 0 -1 val(in):@wfs:prbx 1b
use hwin -1504 1175 100 0 hwin#112
xform 0 -1408 1216
p -1501 1208 100 0 -1 val(in):@wfs:prbx 1p
use hwout -704 727 100 0 hwout#121
xform 0 -608 768
p -608 759 100 0 -1 val(outp):@wfs:prby
use hwout -704 1623 100 0 hwout#94
xform 0 -608 1664
p -608 1655 100 0 -1 val(outp):@wfs:filt
use hwout -704 2071 100 0 hwout#6
xform 0 -608 2112
p -608 2103 100 0 -1 val(outp):@wfs:foc
use hwout -704 1175 100 0 hwout#106
xform 0 -608 1216
p -608 1207 100 0 -1 val(outp):@wfs:prbx
use mech -1216 800 100 768 prby
xform 0 -1008 608
p -1200 704 100 768 -1 set1:name $(name) Probe Y Sim
p -1200 672 100 768 -1 set2:top $(top)prby
p -1200 640 100 768 -1 set3:mtype Gimbal
p -1200 608 100 768 -1 set4:ai1 Soft Channel HS
p -1200 576 100 768 -1 set5:ai2 Soft Channel HS
p -1200 544 100 768 -1 set6:motor Soft Channel HS
p -1200 480 100 768 -1 set8:ident $(ident)Prby
use mech -1216 1696 100 768 filt
xform 0 -1008 1504
p -1200 1600 100 768 -1 set1:name $(name) Filter Sim
p -1200 1568 100 768 -1 set2:top $(top)filt
p -1200 1536 100 768 -1 set3:mtype Wheel
p -1200 1504 100 768 -1 set4:ai1 Soft Channel HS
p -1200 1440 100 768 -1 set6:motor Soft Channel HS
p -1200 1376 100 768 -1 set8:ident $(ident)Filt
use mech -1216 2144 100 768 foc
xform 0 -1008 1952
p -1200 2048 100 768 -1 set1:name $(name) Focus Sim
p -1200 2016 100 768 -1 set2:top $(top)foc
p -1200 1984 100 768 -1 set3:mtype Stage
p -1200 1952 100 768 -1 set4:ai1 Soft Channel HS
p -1200 1920 100 768 -1 set5:ai2 Soft Channel HS
p -1200 1888 100 768 -1 set6:motor Soft Channel HS
p -1200 1824 100 768 -1 set8:ident $(ident)Foc
use mech -1216 1248 100 768 prbx
xform 0 -1008 1056
p -1200 1152 100 768 -1 set1:name $(name) Probe X Sim
p -1200 1120 100 768 -1 set2:top $(top)prbx
p -1200 1088 100 768 -1 set3:mtype Gimbal
p -1200 1056 100 768 -1 set4:ai1 Soft Channel HS
p -1200 1024 100 768 -1 set5:ai2 Soft Channel HS
p -1200 992 100 768 -1 set6:motor Soft Channel HS
p -1200 928 100 768 -1 set8:ident $(ident)Prbx
use eborderC 1600 -160 100 1280 $Id:
xform 0 16 1024
p 1136 -64 200 1536 -1 file:wfsSim.sch
p 1600 -160 100 1280 -1 id:$Id: wfsSim.sch,v 1.2 2009/05/27 19:34:05 fkraemer Exp $
[comments]
