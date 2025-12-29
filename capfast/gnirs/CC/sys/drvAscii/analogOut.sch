[schematic2]
uniq 546
[tools]
[detail]
w 1604 1499 100 2 n#545 eaos.ao13.OUT 1600 1504 1600 1504 hwout.hwout#543.outp
w 1604 1723 100 2 n#544 eaos.ao12.OUT 1600 1728 1600 1728 hwout.hwout#538.outp
w 868 1723 100 2 n#539 eaos.ao3.OUT 864 1728 864 1728 hwout.hwout#280.outp
w 1604 2171 100 2 n#534 eaos.ao11.OUT 1600 2176 1600 2176 hwout.hwout#526.outp
w 1604 2379 100 2 n#533 hwout.hwout#523.outp 1600 2384 1600 2384 eaos.ao10.OUT
w 868 2171 100 2 n#532 eaos.ao1.OUT 864 2176 864 2176 hwout.hwout#284.outp
w 868 2395 100 2 n#531 eaos.ao.OUT 864 2400 864 2400 hwout.hwout#83.outp
w 868 1947 100 2 n#521 eaos.ao2.OUT 864 1952 864 1952 hwout.hwout#164.outp
w 868 379 100 2 n#520 eaos.ao9.OUT 864 384 864 384 hwout.hwout#324.outp
w 868 603 100 2 n#518 eaos.ao8.OUT 864 608 864 608 hwout.hwout#310.outp
w 868 827 100 2 n#516 eaos.ao7.OUT 864 832 864 832 hwout.hwout#309.outp
w 868 1051 100 2 n#514 eaos.ao6.OUT 864 1056 864 1056 hwout.hwout#304.outp
w 868 1275 100 2 n#512 eaos.ao5.OUT 864 1280 864 1280 hwout.hwout#303.outp
w 868 1499 100 2 n#510 eaos.ao4.OUT 864 1504 864 1504 hwout.hwout#281.outp
w 132 2299 100 2 n#471 hwout.hwout#168.outp 128 2304 128 2304 elongouts.timeout.OUT
w 132 1291 100 2 n#470 hwout.hwout#173.outp 128 1296 128 1296 estringouts.readCmt.OUT
w 132 1515 100 2 n#469 hwout.hwout#151.outp 128 1520 128 1520 estringouts.writeCmt.OUT
w 132 2043 100 2 n#468 hwout.hwout#211.outp 128 2048 128 2048 elongouts.debug.OUT
w 132 1755 100 2 n#467 eaos.slope.OUT 128 1760 128 1760 hwout.hwout#171.outp
s -64 2496 100 0 RECORDS WHICH CONTROL
s -64 2464 100 0 LINK ASPECTS
s -64 2423 100 0 timeout control max time to wait for a response
s -64 1927 100 0 slope controls ao and ai conversions
s -64 1895 100 0 rval->%f, %f->rval
s -64 1863 100 0 refer to header of devAscii.c
s -64 1607 100 0 writeCmt = write command terminator
s -64 1383 100 0 readCmt = read command terminator
s 672 1184 100 0 non-standary field delimiters
[cell use]
use hwout 1624 1464 100 0 hwout#543
xform 0 1696 1504
p 1696 1454 100 0 -1 val(outp):@/pty/tserv.M <AI%4b><%*d>
use hwout 1624 2136 100 0 hwout#526
xform 0 1696 2176
p 1696 2126 100 0 -1 val(outp):@/pty/tserv.M REAL <%f><%*f>
use hwout 1624 2344 100 0 hwout#523
xform 0 1696 2384
p 1696 2334 100 0 -1 val(outp):@/pty/tserv.M REAL <%f>
use hwout 888 2360 100 0 hwout#83
xform 0 960 2400
p 960 2350 100 0 -1 val(outp):@/pty/tserv.M <%f>
use hwout 152 1480 100 0 hwout#151
xform 0 224 1520
p 224 1470 100 0 -1 val(outp):@/pty/tserv.M <writeCmt %s>
use hwout 152 2264 100 0 hwout#168
xform 0 224 2304
p 224 2254 100 0 -1 val(outp):@/pty/tserv.M <timeout>
use hwout 152 1720 100 0 hwout#171
xform 0 224 1760
p 224 1710 100 0 -1 val(outp):@/pty/tserv.M <slope>
use hwout 152 1256 100 0 hwout#173
xform 0 224 1296
p 224 1246 100 0 -1 val(outp):@/pty/tserv.M <readCmt %s>
use hwout 152 2008 100 0 hwout#211
xform 0 224 2048
p 224 1998 100 0 -1 val(outp):@/pty/tserv.M <debug>
use hwout 888 1688 100 0 hwout#280
xform 0 960 1728
p 960 1678 100 0 -1 val(outp):@/pty/tserv.M <%b><%*d>
use hwout 888 1464 100 0 hwout#281
xform 0 960 1504
p 960 1454 100 0 -1 val(outp):@/pty/tserv.M <%x><%*x>
use hwout 888 2136 100 0 hwout#284
xform 0 960 2176
p 960 2126 100 0 -1 val(outp):@/pty/tserv.M <%f><%*f>
use hwout 888 1912 100 0 hwout#164
xform 0 960 1952
p 974 1880 100 0 0 typ(outp):val
p 960 1902 100 0 -1 val(outp):@/pty/tserv.M <%d><%*d>
use hwout 888 1240 100 0 hwout#303
xform 0 960 1280
p 960 1230 100 0 -1 val(outp):@/pty/tserv.M <%4c><%*d>
use hwout 888 1016 100 0 hwout#304
xform 0 960 1056
p 960 1006 100 0 -1 val(outp):@/pty/tserv.M {1AO%f}{$1%*f}
use hwout 888 792 100 0 hwout#309
xform 0 960 832
p 960 782 100 0 -1 val(outp):@/pty/tserv.M <$1AO%dX><$1AO%*fX>
use hwout 888 568 100 0 hwout#310
xform 0 960 608
p 960 558 100 0 -1 val(outp):@/pty/tserv.M <%f><%*k>
use hwout 888 344 100 0 hwout#324
xform 0 960 384
p 960 334 100 0 -1 val(outp):@/pty/tserv.M <%d><%*3c%*d>
use hwout 1624 1688 100 0 hwout#538
xform 0 1696 1728
p 1696 1678 100 0 -1 val(outp):@/pty/tserv.M <AI%b><%*d>
use eaos 1344 1447 200 0 ao13
xform 0 1472 1536
p 1696 1536 100 0 -1 DTYP:Ascii SIO
p 1088 1358 100 0 0 EGUF:1.0000000e+00
p 1408 1600 100 768 1 PV:
use eaos 1344 2119 200 0 ao11
xform 0 1472 2208
p 1696 2208 100 0 -1 DTYP:Ascii SIO
p 1088 2030 100 0 0 EGUF:1.0000000e+00
p 1408 2272 100 768 1 PV:
use eaos 1344 2327 200 0 ao10
xform 0 1472 2416
p 1696 2416 100 0 -1 DTYP:Ascii SIO
p 1088 2238 100 0 0 EGUF:1.0000000e+00
p 1408 2480 100 768 1 PV:
use eaos 608 327 200 0 ao9
xform 0 736 416
p 960 416 100 0 -1 DTYP:Ascii SIO
p 352 238 100 0 0 EGUF:1.0000000e+00
p 672 480 100 768 1 PV:
use eaos 608 551 200 0 ao8
xform 0 736 640
p 960 640 100 0 -1 DTYP:Ascii SIO
p 352 462 100 0 0 EGUF:1.0000000e+00
p 672 704 100 768 1 PV:
use eaos 608 775 200 0 ao7
xform 0 736 864
p 960 864 100 0 -1 DTYP:Ascii SIO
p 352 686 100 0 0 EGUF:1.0000000e+00
p 672 928 100 768 1 PV:
use eaos 608 999 200 0 ao6
xform 0 736 1088
p 960 1088 100 0 -1 DTYP:Ascii SIO
p 352 910 100 0 0 EGUF:1.0000000e+00
p 672 1152 100 768 1 PV:
use eaos 608 1223 200 0 ao5
xform 0 736 1312
p 960 1312 100 0 -1 DTYP:Ascii SIO
p 352 1134 100 0 0 EGUF:1.0000000e+00
p 672 1376 100 768 1 PV:
use eaos 608 1447 200 0 ao4
xform 0 736 1536
p 960 1536 100 0 -1 DTYP:Ascii SIO
p 352 1358 100 0 0 EGUF:1.0000000e+00
p 672 1600 100 768 1 PV:
use eaos 608 1671 200 0 ao3
xform 0 736 1760
p 960 1760 100 0 -1 DTYP:Ascii SIO
p 352 1582 100 0 0 EGUF:1.0000000e+00
p 672 1824 100 768 1 PV:
use eaos 608 1895 200 0 ao2
xform 0 736 1984
p 960 1984 100 0 -1 DTYP:Ascii SIO
p 352 1806 100 0 0 EGUF:1.0000000e+00
p 672 2048 100 768 1 PV:
use eaos 608 2119 200 0 ao1
xform 0 736 2208
p 960 2208 100 0 -1 DTYP:Ascii SIO
p 352 2030 100 0 0 EGUF:1.0000000e+00
p 672 2272 100 768 1 PV:
use eaos 608 2343 200 0 ao
xform 0 736 2432
p 960 2432 100 0 -1 DTYP:Ascii SIO
p 352 2254 100 0 0 EGUF:1.0000000e+00
p 672 2496 100 768 1 PV:
use eaos -112 1680 200 0 slope
xform 0 0 1792
p 224 1790 100 0 -1 DTYP:Ascii SIO
p -384 1774 100 0 0 OMSL:supervisory
p -64 1856 100 768 1 PV:
p -48 1758 100 0 -1 SCAN:Passive
p -384 1404 100 0 0 typ(OUT):path
use eaos 1344 1671 200 0 ao12
xform 0 1472 1760
p 1696 1760 100 0 -1 DTYP:Ascii SIO
p 1088 1582 100 0 0 EGUF:1.0000000e+00
p 1408 1824 100 768 1 PV:
use elongouts -104 2248 200 0 timeout
xform 0 0 2336
p 224 2334 100 0 -1 DTYP:Ascii SIO
p -64 2400 100 768 1 PV:
p -48 2302 100 0 -1 SCAN:Passive
p -288 2108 100 0 0 typ(DOL):path
use elongouts -104 1992 200 0 debug
xform 0 0 2080
p 224 2078 100 0 -1 DTYP:Ascii SIO
p -64 2144 100 768 1 PV:
p -48 2046 100 0 -1 SCAN:Passive
p -288 1852 100 0 0 typ(DOL):path
use estringouts -112 1440 200 0 writeCmt
xform 0 0 1536
p 224 1550 100 0 -1 DTYP:Ascii SIO
p -192 1342 100 0 0 OMSL:closed_loop
p -192 1374 100 0 0 PINI:NO
p -64 1584 100 768 1 PV:
p -48 1566 100 0 -1 SCAN:Passive
p -160 971 100 0 0 typ(OUT):path
use estringouts -112 1216 200 0 readCmt
xform 0 0 1312
p 224 1326 100 0 -1 DTYP:Ascii SIO
p -192 1118 100 0 0 OMSL:closed_loop
p -192 1150 100 0 0 PINI:NO
p -64 1360 100 768 1 PV:
p -48 1342 100 0 -1 SCAN:Passive
p -160 747 100 0 0 typ(OUT):path
[comments]
