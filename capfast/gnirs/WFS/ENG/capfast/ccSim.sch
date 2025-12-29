[schematic2]
uniq 246
[tools]
[detail]
w -254 259 100 0 n#205 mech.puvw.hall1B -224 256 -224 256 hwin.hwin#208.in
w -254 291 100 0 n#204 mech.puvw.hall1P -224 288 -224 288 hwin.hwin#209.in
w 354 291 100 0 n#203 mech.puvw.maddr 384 288 384 288 hwout.hwout#207.outp
w -254 707 100 0 n#198 mech.cov.hall1B -224 704 -224 704 hwin.hwin#201.in
w -254 739 100 0 n#197 mech.cov.hall1P -224 736 -224 736 hwin.hwin#202.in
w 354 739 100 0 n#196 mech.cov.maddr 384 736 384 736 hwout.hwout#200.outp
w -1342 259 100 0 n#195 mech.splt.hall1B -1312 256 -1312 256 hwin.hwin#99.in
w -1342 291 100 0 n#194 mech.splt.hall1P -1312 288 -1312 288 hwin.hwin#100.in
w -1342 1155 100 0 n#193 mech.filt3.hall1B -1312 1152 -1312 1152 hwin.hwin#92.in
w -1342 1187 100 0 n#192 mech.filt3.hall1P -1312 1184 -1312 1184 hwin.hwin#93.in
w -1342 1603 100 0 n#191 mech.filt2.hall1B -1312 1600 -1312 1600 hwin.hwin#85.in
w -1342 1635 100 0 n#190 mech.filt2.hall1P -1312 1632 -1312 1632 hwin.hwin#83.in
w -1342 611 100 0 n#189 mech.foc.hall2B -1312 608 -1312 608 hwin.hwin#47.in
w -1342 643 100 0 n#188 mech.foc.hall2P -1312 640 -1312 640 hwin.hwin#45.in
w -1342 739 100 0 n#186 mech.foc.hall1P -1312 736 -1312 736 hwin.hwin#13.in
w -1342 707 100 0 n#185 mech.foc.hall1B -1312 704 -1312 704 hwin.hwin#14.in
w -1342 2083 100 0 n#183 mech.filt1.hall1P -1312 2080 -1312 2080 hwin.hwin#9.in
w -1342 2051 100 0 n#182 mech.filt1.hall1B -1312 2048 -1312 2048 hwin.hwin#11.in
w -734 2083 100 0 n#180 hwout.hwout#3.outp -704 2080 -704 2080 mech.filt1.maddr
w -734 739 100 0 n#179 hwout.hwout#6.outp -704 736 -704 736 mech.foc.maddr
w -734 1635 100 0 n#177 hwout.hwout#81.outp -704 1632 -704 1632 mech.filt2.maddr
w -734 1187 100 0 n#176 hwout.hwout#91.outp -704 1184 -704 1184 mech.filt3.maddr
w -734 291 100 0 n#175 hwout.hwout#98.outp -704 288 -704 288 mech.splt.maddr
w -254 1155 100 0 n#174 hwin.hwin#118.in -224 1152 -224 1152 mech.fopl.hall1B
w -254 1187 100 0 n#173 hwin.hwin#119.in -224 1184 -224 1184 mech.fopl.hall1P
w -254 1603 100 0 n#172 hwin.hwin#110.in -224 1600 -224 1600 mech.ster2.hall1B
w -254 1635 100 0 n#171 hwin.hwin#111.in -224 1632 -224 1632 mech.ster2.hall1P
w -254 2051 100 0 n#170 hwin.hwin#103.in -224 2048 -224 2048 mech.ster1.hall1B
w -254 2083 100 0 n#169 hwin.hwin#104.in -224 2080 -224 2080 mech.ster1.hall1P
w 354 1187 100 0 n#168 hwout.hwout#117.outp 384 1184 384 1184 mech.fopl.maddr
w 354 2083 100 0 n#167 hwout.hwout#102.outp 384 2080 384 2080 mech.ster1.maddr
w 354 1635 100 0 n#166 hwout.hwout#109.outp 384 1632 384 1632 mech.ster2.maddr
[cell use]
use binio 1152 1792 100 768 binio
xform 0 1360 1568
use common 1248 2096 100 0 common
xform 0 1408 1968
use hwin -1504 1559 100 0 hwin#85
xform 0 -1408 1600
p -1501 1592 100 0 -1 val(in):@cc:filt2 1b
use hwin -1504 1591 100 0 hwin#83
xform 0 -1408 1632
p -1501 1624 100 0 -1 val(in):@cc:filt2 1p
use hwin -1504 567 100 0 hwin#47
xform 0 -1408 608
p -1501 600 100 0 -1 val(in):@cc:foc 2b
use hwin -1504 599 100 0 hwin#45
xform 0 -1408 640
p -1501 632 100 0 -1 val(in):@cc:foc 2p
use hwin -1504 663 100 0 hwin#14
xform 0 -1408 704
p -1501 696 100 0 -1 val(in):@cc:foc 1b
use hwin -1504 695 100 0 hwin#13
xform 0 -1408 736
p -1501 728 100 0 -1 val(in):@cc:foc 1p
use hwin -1504 2007 100 0 hwin#11
xform 0 -1408 2048
p -1501 2040 100 0 -1 val(in):@cc:filt1 1b
use hwin -1504 2039 100 0 hwin#9
xform 0 -1408 2080
p -1501 2072 100 0 -1 val(in):@cc:filt1 1p
use hwin -1504 1111 100 0 hwin#92
xform 0 -1408 1152
p -1501 1144 100 0 -1 val(in):@cc:filt3 1b
use hwin -1504 1143 100 0 hwin#93
xform 0 -1408 1184
p -1501 1176 100 0 -1 val(in):@cc:filt3 1p
use hwin -1504 215 100 0 hwin#99
xform 0 -1408 256
p -1501 248 100 0 -1 val(in):@cc:splt 1b
use hwin -1504 247 100 0 hwin#100
xform 0 -1408 288
p -1501 280 100 0 -1 val(in):@cc:splt 1p
use hwin -416 2007 100 0 hwin#103
xform 0 -320 2048
p -413 2040 100 0 -1 val(in):@cc:ster1 1b
use hwin -416 2039 100 0 hwin#104
xform 0 -320 2080
p -413 2072 100 0 -1 val(in):@cc:ster1 1p
use hwin -416 1559 100 0 hwin#110
xform 0 -320 1600
p -413 1592 100 0 -1 val(in):@cc:ster2 1b
use hwin -416 1591 100 0 hwin#111
xform 0 -320 1632
p -413 1624 100 0 -1 val(in):@cc:ster2 1p
use hwin -416 1111 100 0 hwin#118
xform 0 -320 1152
p -413 1144 100 0 -1 val(in):@cc:fopl 1b
use hwin -416 1143 100 0 hwin#119
xform 0 -320 1184
p -413 1176 100 0 -1 val(in):@cc:fopl 1p
use hwin -416 663 100 0 hwin#201
xform 0 -320 704
p -413 696 100 0 -1 val(in):@cc:cov 1b
use hwin -416 695 100 0 hwin#202
xform 0 -320 736
p -413 728 100 0 -1 val(in):@cc:cov 1p
use hwin -416 215 100 0 hwin#208
xform 0 -320 256
p -413 248 100 0 -1 val(in):@cc:puvw 1b
use hwin -416 247 100 0 hwin#209
xform 0 -320 288
p -413 280 100 0 -1 val(in):@cc:puvw 1p
use hwout -704 2039 100 0 hwout#3
xform 0 -608 2080
p -608 2071 100 0 -1 val(outp):@cc:filt1
use hwout -704 695 100 0 hwout#6
xform 0 -608 736
p -608 727 100 0 -1 val(outp):@cc:foc
use hwout -704 1591 100 0 hwout#81
xform 0 -608 1632
p -608 1623 100 0 -1 val(outp):@cc:filt2
use hwout -704 1143 100 0 hwout#91
xform 0 -608 1184
p -608 1175 100 0 -1 val(outp):@cc:filt3
use hwout -704 247 100 0 hwout#98
xform 0 -608 288
p -608 279 100 0 -1 val(outp):@cc:splt
use hwout 384 2039 100 0 hwout#102
xform 0 480 2080
p 480 2071 100 0 -1 val(outp):@cc:ster1
use hwout 384 1591 100 0 hwout#109
xform 0 480 1632
p 480 1623 100 0 -1 val(outp):@cc:ster2
use hwout 384 1143 100 0 hwout#117
xform 0 480 1184
p 480 1175 100 0 -1 val(outp):@cc:fopl
use hwout 384 695 100 0 hwout#200
xform 0 480 736
p 480 727 100 0 -1 val(outp):@cc:cov
use hwout 384 247 100 0 hwout#207
xform 0 480 288
p 480 279 100 0 -1 val(outp):@cc:puvw
use mech -1216 1664 100 768 filt2
xform 0 -1008 1472
p -1200 1568 100 768 -1 set1:name $(name) Filter 2
p -1200 1536 100 768 -1 set2:top $(top)filt2
p -1200 1504 100 768 -1 set3:mtype Wheel
p -1200 1472 100 768 -1 set4:ai1 Soft Channel HS
p -1200 1408 100 768 -1 set6:motor Soft Channel HS
p -1200 1344 100 768 -1 set8:ident $(ident)Filt2
use mech -1216 2112 100 768 filt1
xform 0 -1008 1920
p -1200 2016 100 768 -1 set1:name $(name) Filter 1
p -1200 1984 100 768 -1 set2:top $(top)filt1
p -1200 1952 100 768 -1 set3:mtype Wheel
p -1200 1920 100 768 -1 set4:ai1 Soft Channel HS
p -1200 1888 100 768 -1 set5:ai2 Soft Channel
p -1200 1856 100 768 -1 set6:motor Soft Channel HS
p -1200 1792 100 768 -1 set8:ident $(ident)Filt1
use mech -1216 768 100 768 foc
xform 0 -1008 576
p -1200 672 100 768 -1 set1:name $(name) Focus
p -1200 640 100 768 -1 set2:top $(top)foc
p -1200 608 100 768 -1 set3:mtype Stage
p -1200 576 100 768 -1 set4:ai1 Soft Channel HS
p -1200 544 100 768 -1 set5:ai2 Soft Channel HS
p -1200 512 100 768 -1 set6:motor Soft Channel HS
p -1200 448 100 768 -1 set8:ident $(ident)Foc
use mech -1216 1216 100 768 filt3
xform 0 -1008 1024
p -1200 1120 100 768 -1 set1:name $(name) Filter 3
p -1200 1088 100 768 -1 set2:top $(top)filt3
p -1200 1056 100 768 -1 set3:mtype Wheel
p -1200 1024 100 768 -1 set4:ai1 Soft Channel HS
p -1200 960 100 768 -1 set6:motor Soft Channel HS
p -1200 896 100 768 -1 set8:ident $(ident)Filt3
use mech -1216 320 100 768 splt
xform 0 -1008 128
p -1200 224 100 768 -1 set1:name $(name) Beam Splt
p -1200 192 100 768 -1 set2:top $(top)splt
p -1200 160 100 768 -1 set3:mtype Wheel
p -1200 128 100 768 -1 set4:ai1 Soft Channel HS
p -1200 64 100 768 -1 set6:motor Soft Channel HS
p -1200 0 100 768 -1 set8:ident $(ident)Splt
use mech -128 2112 100 768 ster1
xform 0 80 1920
p -112 2016 100 768 -1 set1:name $(name) Beam Str 1
p -112 1984 100 768 -1 set2:top $(top)ster1
p -112 1952 100 768 -1 set3:mtype Wheel
p -112 1920 100 768 -1 set4:ai1 Soft Channel HS
p -112 1856 100 768 -1 set6:motor Soft Channel HS
p -112 1824 100 768 -1 set7:mostat Soft Channel
p -112 1792 100 768 -1 set8:ident $(ident)Ster1
use mech -128 1664 100 768 ster2
xform 0 80 1472
p -112 1568 100 768 -1 set1:name $(name) Beam Str 2
p -112 1536 100 768 -1 set2:top $(top)ster2
p -112 1504 100 768 -1 set3:mtype Wheel
p -112 1472 100 768 -1 set4:ai1 Soft Channel HS
p -112 1408 100 768 -1 set6:motor Soft Channel HS
p -112 1344 100 768 -1 set8:ident $(ident)Ster2
use mech -128 1216 100 768 fopl
xform 0 80 1024
p -112 1120 100 768 -1 set1:name $(name) Foc Plane Mask
p -112 1088 100 768 -1 set2:top $(top)fopl
p -112 1056 100 768 -1 set3:mtype Wheel
p -112 1024 100 768 -1 set4:ai1 Soft Channel HS
p -112 960 100 768 -1 set6:motor Soft Channel HS
p -112 896 100 768 -1 set8:ident $(ident)Fopl
use mech -128 768 100 768 cov
xform 0 80 576
p -112 672 100 768 -1 set1:name $(name) Cover
p -112 640 100 768 -1 set2:top $(top)cov
p -112 608 100 768 -1 set3:mtype Binary
p -112 576 100 768 -1 set4:ai1 Soft Channel HS
p -112 512 100 768 -1 set6:motor Soft Channel HS
p -112 448 100 768 -1 set8:ident $(ident)Cov
use mech -128 320 100 768 puvw
xform 0 80 128
p -112 224 100 768 -1 set1:name $(name) Pupil Viewer
p -112 192 100 768 -1 set2:top $(top)puvw
p -112 160 100 768 -1 set3:mtype Binary
p -112 128 100 768 -1 set4:ai1 Soft Channel HS
p -112 64 100 768 -1 set6:motor Soft Channel HS
p -112 32 100 768 -1 set7:mostat Soft Channel
p -112 0 100 768 -1 set8:ident $(ident)Puvw
use eborderC 1600 -160 100 1280 $Id:
xform 0 16 1024
p 1136 -64 200 1536 -1 file:ccSim.sch
p 1600 -160 100 1280 -1 id:$Id: ccSim.sch,v 1.2 2009/05/27 19:34:01 fkraemer Exp $
[comments]
