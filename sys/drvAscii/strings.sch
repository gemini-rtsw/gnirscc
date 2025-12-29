[schematic2]
uniq 415
[tools]
[detail]
w 996 2043 100 2 n#324 hwout.hwout#307.outp 992 2048 992 2048 estringouts.so1.OUT
w 996 1787 100 2 n#325 hwout.hwout#308.outp 992 1792 992 1792 estringouts.so2.OUT
w 1796 1259 100 2 n#326 hwin.hwin#317.in 1792 1264 1792 1264 estringins.si5.INP
w 1796 1035 100 2 n#333 hwin.hwin#316.in 1792 1040 1792 1040 estringins.si6.INP
w 2500 2219 100 2 n#338 hwin.hwin#331.in 2496 2224 2496 2224 estringins.si7.INP
w 2500 1995 100 2 n#343 hwin.hwin#336.in 2496 2000 2496 2000 estringins.si8.INP
w 996 1467 100 2 n#348 hwout.hwout#305.outp 992 1472 992 1472 estringouts.so3.OUT
w 2500 1803 100 2 n#353 hwin.hwin#341.in 2496 1808 2496 1808 estringins.si9.INP
w 996 1243 100 2 n#358 hwout.hwout#346.outp 992 1248 992 1248 estringouts.so4.OUT
w 996 1051 100 2 n#359 hwout.hwout#356.outp 992 1056 992 1056 estringouts.so5.OUT
w 2500 1579 100 2 n#374 hwin.hwin#371.in 2496 1584 2496 1584 estringins.si10.INP
w 1796 2027 100 2 n#384 hwin.hwin#376.in 1792 2032 1792 2032 estringins.si1.INP
w 1796 1835 100 2 n#385 estringins.si2.INP 1792 1840 1792 1840 hwin.hwin#379.in
w 1796 1643 100 2 n#391 hwin.hwin#382.in 1792 1648 1792 1648 estringins.si3.INP
w 996 2267 100 2 n#399 hwout.hwout#306.outp 992 2272 992 2272 estringouts.so.OUT
w 1796 2219 100 2 n#405 hwin.hwin#318.in 1792 2224 1792 2224 estringins.si.INP
w 68 379 100 2 n#414 hwin.hwin#409.in 64 384 64 384 ewaves.wf2.INP
w 68 603 100 2 n#413 ewaves.wf1.INP 64 608 64 608 hwin.hwin#408.in
w 68 811 100 2 n#412 hwin.hwin#401.in 64 816 64 816 ewaves.wf.INP
w 1796 1451 100 2 n#392 hwin.hwin#389.in 1792 1456 1792 1456 estringins.si4.INP
w 164 1179 100 2 n#246 hwout.hwout#242.outp 160 1184 160 1184 elongouts.debug.OUT
w 164 1435 100 2 n#244 hwout.hwout#173.outp 160 1440 160 1440 estringouts.readCmt.OUT
w 130 2242 100 0 n#206 elongouts.timeout.OUT 160 2240 160 2240 hwout.hwout#168.outp
w 136 1906 100 0 n#169 eaos.slope.OUT 160 1904 160 1904 hwout.hwout#171.outp
w 136 1666 100 0 n#153 hwout.hwout#151.outp 160 1664 160 1664 estringouts.writeCmt.OUT
s 1808 2272 100 0 entire echo is the string in val
s 1600 2352 100 0 then the current value of VAL is embedded in the prompt
s 1600 2384 100 0 If a prompt contains "%s" or is simply <%s>
s 1856 1296 100 0 only part of echo is string in val
s 1856 1095 100 0 val is used for output and input
s 1857 1071 100 0 this provides for modifiable output strings
s 800 2359 100 0 has a prompt and response
s 800 2135 100 0 no prompt and ignores response
s 800 1911 100 0 field delimiters are '!'
s 800 1648 100 0 no prompt and no response
s 800 1616 100 0 try: caput so3 "\x58hello\x55"
s 800 1584 100 0 a poor mans protocol can be implemented
s 1856 2080 100 0 default prompt is the CMT
s 1856 1888 100 0 prompt default to <%s>
s 1856 1696 100 0 response default to <%s>
s 1600 2416 100 0 then it defaults to <%s>.
s 1856 1312 100 0 non-standard field delimiter
s -32 1527 100 0 readCmt = read command terminator
s -32 1751 100 0 writeCmt = write command terminator
s -32 2007 100 0 refer to header of devAscii.c
s -32 2039 100 0 rval->%f, %f->rval
s -32 2071 100 0 slope controls ao and ai conversions
s -32 2359 100 0 timeout control max time to wait for a response
s -32 2400 100 0 LINK ASPECTS
s -32 2432 100 0 RECORDS WHICH CONTROL
s 1600 2448 100 0 If the response is not specified 
s 1600 2480 100 0 then only the write command terminator is output.
s 800 2439 100 0 STRING OUTPUT RECORDS
s 1600 2512 100 0 If a prompt is not specified and it is not <%s>
[cell use]
use estringouts 752 1168 200 0 so4
xform 0 864 1264
p 1088 1278 100 0 -1 DTYP:Ascii SIO
p 672 1070 100 0 0 OMSL:closed_loop
p 672 1102 100 0 0 PINI:NO
p 800 1312 100 768 1 PV:
p 816 1294 100 0 -1 SCAN:Passive
p 704 699 100 0 0 typ(OUT):path
use estringouts 752 1712 200 0 so2
xform 0 864 1808
p 1088 1822 100 0 -1 DTYP:Ascii SIO
p 672 1614 100 0 0 OMSL:closed_loop
p 672 1646 100 0 0 PINI:NO
p 800 1856 100 768 1 PV:
p 816 1838 100 0 -1 SCAN:Passive
p 704 1243 100 0 0 typ(OUT):path
use estringouts 752 1968 200 0 so1
xform 0 864 2064
p 1088 2078 100 0 -1 DTYP:Ascii SIO
p 672 1870 100 0 0 OMSL:closed_loop
p 672 1902 100 0 0 PINI:NO
p 800 2112 100 768 1 PV:
p 816 2094 100 0 -1 SCAN:Passive
p 704 1499 100 0 0 typ(OUT):path
use estringouts 752 2192 200 0 so
xform 0 864 2288
p 1088 2302 100 0 -1 DTYP:Ascii SIO
p 672 2094 100 0 0 OMSL:closed_loop
p 672 2126 100 0 0 PINI:NO
p 800 2336 100 768 1 PV:
p 816 2318 100 0 -1 SCAN:Passive
p 704 1723 100 0 0 typ(OUT):path
use estringouts 752 1392 200 0 so3
xform 0 864 1488
p 1088 1502 100 0 -1 DTYP:Ascii SIO
p 672 1294 100 0 0 OMSL:closed_loop
p 672 1326 100 0 0 PINI:NO
p 800 1536 100 768 1 PV:
p 816 1518 100 0 -1 SCAN:Passive
p 704 923 100 0 0 typ(OUT):path
use estringouts 752 976 200 0 so5
xform 0 864 1072
p 1088 1086 100 0 -1 DTYP:Ascii SIO
p 672 878 100 0 0 OMSL:closed_loop
p 672 910 100 0 0 PINI:NO
p 800 1120 100 768 1 PV:
p 816 1102 100 0 -1 SCAN:Passive
p 704 507 100 0 0 typ(OUT):path
use estringouts -80 1360 200 0 readCmt
xform 0 32 1456
p 256 1470 100 0 -1 DTYP:Ascii SIO
p -160 1262 100 0 0 OMSL:closed_loop
p -160 1294 100 0 0 PINI:NO
p -32 1504 100 768 1 PV:
p -16 1486 100 0 -1 SCAN:Passive
p -128 891 100 0 0 typ(OUT):path
use estringouts -80 1584 200 0 writeCmt
xform 0 32 1680
p 256 1694 100 0 -1 DTYP:Ascii SIO
p -160 1486 100 0 0 OMSL:closed_loop
p -160 1518 100 0 0 PINI:NO
p -32 1728 100 768 1 PV:
p -16 1710 100 0 -1 SCAN:Passive
p -128 1115 100 0 0 typ(OUT):path
use estringins 1816 1544 200 0 si3
xform 0 1920 1616
p 1600 1678 100 0 -1 DTYP:Ascii SIO
p 1856 1664 100 768 1 PV:
p 1872 1646 100 0 -1 SCAN:Passive
use estringins 1816 1736 200 0 si2
xform 0 1920 1808
p 1600 1870 100 0 -1 DTYP:Ascii SIO
p 1856 1856 100 768 1 PV:
p 1872 1838 100 0 -1 SCAN:Passive
use estringins 1816 1928 200 0 si1
xform 0 1920 2000
p 1600 2062 100 0 -1 DTYP:Ascii SIO
p 1856 2048 100 768 1 PV:
p 1872 2030 100 0 -1 SCAN:Passive
use estringins 2520 1704 200 0 si9
xform 0 2624 1776
p 2304 1808 100 0 -1 DTYP:Ascii SIO
p 2560 1824 100 768 1 PV:
p 2576 1806 100 0 -1 SCAN:Passive
use estringins 2520 1896 200 0 si8
xform 0 2624 1968
p 2304 2000 100 0 -1 DTYP:Ascii SIO
p 2560 2016 100 768 1 PV:
p 2576 1998 100 0 -1 SCAN:Passive
use estringins 1816 2120 200 0 si
xform 0 1920 2192
p 1600 2254 100 0 -1 DTYP:Ascii SIO
p 1856 2240 100 768 1 PV:
p 1872 2222 100 0 -1 SCAN:Passive
use estringins 1816 1160 200 0 si5
xform 0 1920 1232
p 1600 1294 100 0 -1 DTYP:Ascii SIO
p 1856 1280 100 768 1 PV:
p 1872 1262 100 0 -1 SCAN:Passive
use estringins 1816 936 200 0 si6
xform 0 1920 1008
p 1600 1070 100 0 -1 DTYP:Ascii SIO
p 1856 1056 100 768 1 PV:
p 1872 1038 100 0 -1 SCAN:Passive
use estringins 2520 2120 200 0 si7
xform 0 2624 2192
p 2304 2224 100 0 -1 DTYP:Ascii SIO
p 2560 2240 100 768 1 PV:
p 2576 2222 100 0 -1 SCAN:Passive
use estringins 2520 1480 200 0 si10
xform 0 2624 1552
p 2304 1584 100 0 -1 DTYP:Ascii SIO
p 2560 1600 100 768 1 PV:
p 2576 1582 100 0 -1 SCAN:Passive
use estringins 1816 1352 200 0 si4
xform 0 1920 1424
p 1600 1486 100 0 -1 DTYP:Ascii SIO
p 1856 1472 100 768 1 PV:
p 1872 1454 100 0 -1 SCAN:Passive
use hwout 1016 1208 100 0 hwout#346
xform 0 1088 1248
p 1088 1198 100 0 -1 val(outp):@/pty/tserv.M {X%sX}{%*s}
use hwout 1016 1752 100 0 hwout#308
xform 0 1088 1792
p 1088 1742 100 0 -1 val(outp):@/pty/tserv.M !\x58!!<%*s!
use hwout 1016 2008 100 0 hwout#307
xform 0 1088 2048
p 1088 1998 100 0 -1 val(outp):@/pty/tserv.M <><%*k> 
use hwout 1016 2232 100 0 hwout#306
xform 0 1088 2272
p 1088 2222 100 0 -1 val(outp):@/pty/tserv.M <sts?><sts?%*s>
use hwout 1016 1432 100 0 hwout#305
xform 0 1088 1472
p 1088 1422 100 0 -1 val(outp):@/pty/tserv.M
use hwout 1016 1016 100 0 hwout#356
xform 0 1088 1056
p 1088 1006 100 0 -1 val(outp):@/pty/tserv.M [X%sX][X%*s X]
use hwout 184 1144 100 0 hwout#242
xform 0 256 1184
p 256 1134 100 0 -1 val(outp):@/pty/tserv.M <debug>
use hwout 184 1400 100 0 hwout#173
xform 0 256 1440
p 256 1390 100 0 -1 val(outp):@/pty/tserv.M <readCmt %s>
use hwout 184 1864 100 0 hwout#171
xform 0 256 1904
p 256 1854 100 0 -1 val(outp):@/pty/tserv.M <slope>
use hwout 184 2200 100 0 hwout#168
xform 0 256 2240
p 256 2190 100 0 -1 val(outp):@/pty/tserv.M <timeout>
use hwout 184 1624 100 0 hwout#151
xform 0 256 1664
p 256 1614 100 0 -1 val(outp):@/pty/tserv.M <writeCmt %s>
use hwin 1624 1608 100 0 hwin#382
xform 0 1696 1648
p 1424 1600 100 0 -1 val(in):@/pty/tserv.M <%s>
use hwin 1624 1800 100 0 hwin#379
xform 0 1696 1840
p 1424 1792 100 0 -1 val(in):@/pty/tserv.M <><%s>
use hwin 1624 1992 100 0 hwin#376
xform 0 1696 2032
p 1424 1984 100 0 -1 val(in):@/pty/tserv.M 
use hwin 2328 1768 100 0 hwin#341
xform 0 2400 1808
p 2304 1856 100 0 -1 val(in):@/pty/tserv.M <%s><XYZ:%s :ZYX>
use hwin 2328 1960 100 0 hwin#336
xform 0 2400 2000
p 2304 2048 100 0 -1 val(in):@/pty/tserv.M <%s><XYZ:%s>
use hwin 1624 2184 100 0 hwin#318
xform 0 1696 2224
p 1424 2176 100 0 -1 val(in):@/pty/tserv.M <?><%s>
use hwin 1624 1224 100 0 hwin#317
xform 0 1696 1264
p 1424 1216 100 0 -1 val(in):@/pty/tserv.M #status?##<st%s#
use hwin 1624 1000 100 0 hwin#316
xform 0 1696 1040
p 1424 992 100 0 -1 val(in):@/pty/tserv.M <query %s><%s>
use hwin 2328 2184 100 0 hwin#331
xform 0 2400 2224
p 2304 2272 100 0 -1 val(in):@/pty/tserv.M <\x58\x59\x5a:%s><XYZ:%s>
use hwin 2328 1544 100 0 hwin#371
xform 0 2400 1584
p 2304 1632 100 0 -1 val(in):@/pty/tserv.M <?><sts:%*d,%*d,%s%*d,%*f,OK>
use hwin -104 344 100 0 hwin#409
xform 0 -32 384
p -304 336 100 0 -1 val(in):@/pty/tserv.M <?><%2t%4k:%s>
use hwin -104 568 100 0 hwin#408
xform 0 -32 608
p -304 560 100 0 -1 val(in):@/pty/tserv.M <?><%2t%s>
use hwin 1624 1416 100 0 hwin#389
xform 0 1696 1456
p 1424 1408 100 0 -1 val(in):@/pty/tserv.M (%s)(%s)
use hwin -104 776 100 0 hwin#401
xform 0 -32 816
p -304 768 100 0 -1 val(in):@/pty/tserv.M <?><%s>
use ewaves 64 263 200 0 wf2
xform 0 192 352
p 85 489 100 0 0 DTYP:Ascii SIO
p 192 238 100 0 0 FTVL:CHAR
p 192 270 100 0 0 NELM:256
p 128 416 100 768 1 PV:
use ewaves 64 487 200 0 wf1
xform 0 192 576
p 85 713 100 0 0 DTYP:Ascii SIO
p 192 462 100 0 0 FTVL:CHAR
p 192 494 100 0 0 NELM:256
p 128 640 100 768 1 PV:
use ewaves 64 695 200 0 wf
xform 0 192 784
p 85 921 100 0 0 DTYP:Ascii SIO
p 192 670 100 0 0 FTVL:CHAR
p 192 702 100 0 0 NELM:256
p 128 848 100 768 1 PV:
use elongouts -96 1127 200 0 debug
xform 0 32 1216
p -211 1561 100 0 0 DESC:long output record
p 256 1216 100 0 -1 DTYP:Ascii SIO
p -32 1280 100 768 1 PV:
use elongouts -72 2184 200 0 timeout
xform 0 32 2272
p 256 2270 100 0 -1 DTYP:Ascii SIO
p -32 2336 100 768 1 PV:
p -16 2238 100 0 -1 SCAN:Passive
p -256 2044 100 0 0 typ(DOL):path
use eaos -80 1824 200 0 slope
xform 0 32 1936
p 256 1934 100 0 -1 DTYP:Ascii SIO
p -352 1918 100 0 0 OMSL:supervisory
p -32 2000 100 768 1 PV:
p -16 1902 100 0 -1 SCAN:Passive
p -352 1548 100 0 0 typ(OUT):path
[comments]
