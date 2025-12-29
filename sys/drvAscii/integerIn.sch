[schematic2]
uniq 426
[tools]
[detail]
w 804 1899 100 2 n#422 hwin.hwin#256.in 800 1904 800 1904 ebis.bi9.INP
w 804 1707 100 2 n#421 hwin.hwin#252.in 800 1712 800 1712 ebis.bi8.INP
w 804 763 100 2 n#417 hwin.hwin#406.in 800 768 800 768 mbbidirects.mbbid1.INP
w 2340 1211 100 2 n#413 hwin.hwin#412.in 2336 1216 2336 1216 mbbidirects.mbbid4.INP
w 1604 763 100 2 n#411 hwin.hwin#410.in 1600 768 1600 768 mbbidirects.mbbid3.INP
w 1604 1211 100 2 n#409 hwin.hwin#408.in 1600 1216 1600 1216 mbbidirects.mbbid2.INP
w 68 427 100 2 n#395 hwin.hwin#223.in 64 432 64 432 ebis.bi4.INP
w 68 619 100 2 n#394 hwin.hwin#220.in 64 624 64 624 ebis.bi3.INP
w 68 811 100 2 n#393 ebis.bi2.INP 64 816 64 816 hwin.hwin#217.in
w 68 1003 100 2 n#392 hwin.hwin#213.in 64 1008 64 1008 ebis.bi1.INP
w 68 1195 100 2 n#391 hwin.hwin#72.in 64 1200 64 1200 ebis.bi.INP
w 804 2475 100 2 n#376 hwin.hwin#226.in 800 2480 800 2480 ebis.bi5.INP
w 804 2283 100 2 n#375 hwin.hwin#229.in 800 2288 800 2288 ebis.bi6.INP
w 804 2091 100 2 n#374 hwin.hwin#232.in 800 2096 800 2096 ebis.bi7.INP
w 1604 1707 100 2 n#370 hwin.hwin#288.in 1600 1712 1600 1712 embbis.mbbi4.INP
w 1604 1899 100 2 n#369 hwin.hwin#289.in 1600 1904 1600 1904 embbis.mbbi3.INP
w 1604 2091 100 2 n#368 hwin.hwin#290.in 1600 2096 1600 2096 embbis.mbbi2.INP
w 1604 2283 100 2 n#367 hwin.hwin#291.in 1600 2288 1600 2288 embbis.mbbi1.INP
w 2340 1707 100 2 n#365 elongins.li4.INP 2336 1712 2336 1712 hwin.hwin#356.in
w 2340 1899 100 2 n#363 hwin.hwin#350.in 2336 1904 2336 1904 elongins.li3.INP
w 2340 2091 100 2 n#362 hwin.hwin#351.in 2336 2096 2336 2096 elongins.li2.INP
w 2340 2283 100 2 n#361 elongins.li1.INP 2336 2288 2336 2288 hwin.hwin#342.in
w 68 1323 100 2 n#246 hwout.hwout#242.outp 64 1328 64 1328 elongouts.debug.OUT
w 68 1579 100 2 n#244 hwout.hwout#173.outp 64 1584 64 1584 estringouts.readCmt.OUT
w 114 2338 100 0 n#206 elongouts.timeout.OUT 144 2336 144 2336 hwout.hwout#168.outp
w 40 2050 100 0 n#169 eaos.slope.OUT 64 2048 64 2048 hwout.hwout#171.outp
w 40 1810 100 0 n#153 hwout.hwout#151.outp 64 1808 64 1808 estringouts.writeCmt.OUT
s -128 2528 100 0 RECORDS WHICH CONTROL
s -128 2496 100 0 LINK ASPECTS
s -128 2455 100 0 timeout control max time to wait for a response
s -128 2215 100 0 slope controls ao and ai conversions
s -128 2183 100 0 rval->%f, %f->rval
s -128 2151 100 0 refer to header of devAscii.c
s -128 1895 100 0 writeCmt = write command terminator
s -128 1671 100 0 readCmt = read command terminator
[cell use]
use hwin 2168 1176 100 0 hwin#412
xform 0 2240 1216
p 2016 1166 100 0 -1 val(in):@/pty/tserv.M <(bin)><%16b>
use hwin 1432 728 100 0 hwin#410
xform 0 1504 768
p 1280 718 100 0 -1 val(in):@/pty/tserv.M <(char)><%c>
use hwin 1432 1176 100 0 hwin#408
xform 0 1504 1216
p 1280 1166 100 0 -1 val(in):@/pty/tserv.M <(real)><%f>
use hwin 632 728 100 0 hwin#406
xform 0 704 768
p 480 718 100 0 -1 val(in):@/pty/tserv.M <(int)><%d>
use hwin 2168 1672 100 0 hwin#356
xform 0 2240 1712
p 2016 1662 100 0 -1 val(in):@/pty/tserv.M {(bin)}{%b}
use hwin 2168 2056 100 0 hwin#351
xform 0 2240 2096
p 2016 2046 100 0 -1 val(in):@/pty/tserv.M <(real)><%f>
use hwin 2168 1864 100 0 hwin#350
xform 0 2240 1904
p 2016 1854 100 0 -1 val(in):@/pty/tserv.M <(4char)><%4c>
use hwin 1432 2248 100 0 hwin#291
xform 0 1504 2288
p 1280 2238 100 0 -1 val(in):@/pty/tserv.M <(int)><%d>
use hwin 1432 2056 100 0 hwin#290
xform 0 1504 2096
p 1280 2046 100 0 -1 val(in):@/pty/tserv.M <(real)><%f>
use hwin 1432 1864 100 0 hwin#289
xform 0 1504 1904
p 1280 1854 100 0 -1 val(in):@/pty/tserv.M <(char)><%c>
use hwin 1432 1672 100 0 hwin#288
xform 0 1504 1712
p 1280 1662 100 0 -1 val(in):@/pty/tserv.M [(bin)][%b]
use hwin 632 1864 100 0 hwin#256
xform 0 704 1904
p 480 1854 100 0 -1 val(in):@/pty/tserv.M <*DI(3char)><*DI%3c>
use hwin 632 2056 100 0 hwin#232
xform 0 704 2096
p 480 2046 100 0 -1 val(in):@/pty/tserv.M <(4k)(real e)><%4k%e>
use hwin 632 2248 100 0 hwin#229
xform 0 704 2288
p 480 2238 100 0 -1 val(in):@/pty/tserv.M <*DI1(int)B><*D01%d\x42>
use hwin 632 2440 100 0 hwin#226
xform 0 704 2480
p 480 2430 100 0 -1 val(in):@/pty/tserv.M <*X01(bin)><*X01%b>
use hwin -104 392 100 0 hwin#223
xform 0 -32 432
p -256 382 100 0 -1 val(in):@/pty/tserv.M !*DI(char)!!*DI%c!
use hwin -104 584 100 0 hwin#220
xform 0 -32 624
p -256 574 100 0 -1 val(in):@/pty/tserv.M <*DI(real)><*DI%f>
use hwin -104 776 100 0 hwin#217
xform 0 -32 816
p -256 766 100 0 -1 val(in):@/pty/tserv.M <*DI(int)><*DI%d>
use hwin -104 968 100 0 hwin#213
xform 0 -32 1008
p -256 958 100 0 -1 val(in):@/pty/tserv.M <DI(int)><DI%d>
use hwin -104 1160 100 0 hwin#72
xform 0 -32 1200
p -256 1150 100 0 -1 val(in):@/pty/tserv.M <(int)><%d>
use hwin 632 1672 100 0 hwin#252
xform 0 704 1712
p 480 1662 100 0 -1 val(in):@/pty/tserv.M <*DI1(int)X><*DI1%dX>
use hwin 2168 2248 100 0 hwin#342
xform 0 2240 2288
p 2016 2238 100 0 -1 val(in):@/pty/tserv.M <(int)><%d>
use ebis 824 1800 200 0 bi9
xform 0 928 1872
p 608 1934 100 0 -1 DTYP:Ascii SIO
p 864 1936 100 1536 1 PV:
p 880 1838 100 0 -1 SCAN:Passive
p 576 1372 100 0 0 typ(INP):path
use ebis 824 1992 200 0 bi7
xform 0 928 2064
p 608 2126 100 0 -1 DTYP:Ascii SIO
p 864 2128 100 1536 1 PV:
p 880 2030 100 0 -1 SCAN:Passive
p 576 1564 100 0 0 typ(INP):path
use ebis 824 2184 200 0 bi6
xform 0 928 2256
p 608 2318 100 0 -1 DTYP:Ascii SIO
p 864 2320 100 1536 1 PV:
p 880 2222 100 0 -1 SCAN:Passive
p 576 1756 100 0 0 typ(INP):path
use ebis 824 2376 200 0 bi5
xform 0 928 2448
p 608 2510 100 0 -1 DTYP:Ascii SIO
p 864 2512 100 1536 1 PV:
p 880 2414 100 0 -1 SCAN:Passive
p 576 1948 100 0 0 typ(INP):path
use ebis 88 328 200 0 bi4
xform 0 192 400
p -128 462 100 0 -1 DTYP:Ascii SIO
p 128 464 100 1536 1 PV:
p 144 366 100 0 -1 SCAN:Passive
p -160 -100 100 0 0 typ(INP):path
use ebis 88 520 200 0 bi3
xform 0 192 592
p -128 654 100 0 -1 DTYP:Ascii SIO
p 128 656 100 1536 1 PV:
p 144 558 100 0 -1 SCAN:Passive
p -160 92 100 0 0 typ(INP):path
use ebis 88 712 200 0 bi2
xform 0 192 784
p -128 846 100 0 -1 DTYP:Ascii SIO
p 128 848 100 1536 1 PV:
p 144 750 100 0 -1 SCAN:Passive
p -160 284 100 0 0 typ(INP):path
use ebis 88 904 200 0 bi1
xform 0 192 976
p -48 1102 100 0 0 DTYP:Ascii SIO
p 128 1040 100 1536 1 PV:
use ebis 88 1096 200 0 bi
xform 0 192 1168
p -128 1230 100 0 -1 DTYP:Ascii SIO
p 128 1232 100 1536 1 PV:
p 144 1134 100 0 -1 SCAN:Passive
p -160 668 100 0 0 typ(INP):path
use ebis 824 1608 200 0 bi8
xform 0 928 1680
p 608 1742 100 0 -1 DTYP:Ascii SIO
p 864 1744 100 1536 1 PV:
p 880 1646 100 0 -1 SCAN:Passive
p 576 1180 100 0 0 typ(INP):path
use mbbidirects 2336 999 200 0 mbbid4
xform 0 2496 1168
p 2144 1248 100 0 -1 DTYP:Ascii SIO
p 2400 1312 100 0 1 PV:
p 2504 552 100 0 0 palrm(INP):NMS
p 2524 532 100 0 0 palrm(SDIS):NMS
p 2304 1232 100 0 -1 pproc(INP):NPP
p 2564 492 100 0 0 pproc(SDIS):NPP
use mbbidirects 1600 551 200 0 mbbid3
xform 0 1760 720
p 1408 800 100 0 -1 DTYP:Ascii SIO
p 1664 864 100 0 1 PV:
p 1768 104 100 0 0 palrm(INP):NMS
p 1788 84 100 0 0 palrm(SDIS):NMS
p 1568 784 100 0 -1 pproc(INP):NPP
p 1828 44 100 0 0 pproc(SDIS):NPP
use mbbidirects 1600 999 200 0 mbbid2
xform 0 1760 1168
p 1408 1248 100 0 -1 DTYP:Ascii SIO
p 1664 1312 100 0 1 PV:
p 1768 552 100 0 0 palrm(INP):NMS
p 1788 532 100 0 0 palrm(SDIS):NMS
p 1568 1232 100 0 -1 pproc(INP):NPP
p 1828 492 100 0 0 pproc(SDIS):NPP
use mbbidirects 800 551 200 0 mbbid1
xform 0 960 720
p 608 800 100 0 -1 DTYP:Ascii SIO
p 864 864 100 0 1 PV:
p 968 104 100 0 0 palrm(INP):NMS
p 988 84 100 0 0 palrm(SDIS):NMS
p 768 784 100 0 -1 pproc(INP):NPP
p 1028 44 100 0 0 pproc(SDIS):NPP
use elongins 2336 1607 200 0 li4
xform 0 2464 1680
p 2144 1744 100 0 -1 DTYP:Ascii SIO
p 2400 1728 100 768 1 PV:
use elongins 2336 1991 200 0 li2
xform 0 2464 2064
p 2144 2128 100 0 -1 DTYP:Ascii SIO
p 2400 2112 100 768 1 PV:
use elongins 2336 1799 200 0 li3
xform 0 2464 1872
p 2144 1936 100 0 -1 DTYP:Ascii SIO
p 2400 1920 100 768 1 PV:
use elongins 2336 2183 200 0 li1
xform 0 2464 2256
p 2144 2320 100 0 -1 DTYP:Ascii SIO
p 2400 2304 100 768 1 PV:
use embbis 1600 1607 200 0 mbbi4
xform 0 1728 1680
p 1408 1744 100 0 -1 DTYP:Ascii SIO
p 1664 1728 100 768 1 PV:
use embbis 1600 1799 200 0 mbbi3
xform 0 1728 1872
p 1408 1936 100 0 -1 DTYP:Ascii SIO
p 1664 1920 100 768 1 PV:
use embbis 1600 1991 200 0 mbbi2
xform 0 1728 2064
p 1408 2128 100 0 -1 DTYP:Ascii SIO
p 1664 2112 100 768 1 PV:
use embbis 1600 2183 200 0 mbbi1
xform 0 1728 2256
p 1408 2320 100 0 -1 DTYP:Ascii SIO
p 1664 2304 100 768 1 PV:
use elongouts -88 2280 200 0 timeout
xform 0 16 2368
p 240 2366 100 0 -1 DTYP:Ascii SIO
p -64 2304 100 1280 -1 OMSL:supervisory
p -48 2432 100 768 1 PV:
p -32 2334 100 0 -1 SCAN:Passive
p -272 2140 100 0 0 typ(DOL):path
use elongouts -192 1271 100 0 debug
xform 0 -64 1360
p -307 1705 100 0 0 DESC:long output record
p 160 1360 100 0 -1 DTYP:Ascii SIO
p -128 1424 100 768 1 PV:
use hwout 88 1768 100 0 hwout#151
xform 0 160 1808
p 160 1758 100 0 -1 val(outp):@/pty/tserv.M <writeCmt %s>
use hwout 168 2296 100 0 hwout#168
xform 0 240 2336
use hwout 88 2008 100 0 hwout#171
xform 0 160 2048
p 160 1998 100 0 -1 val(outp):@/pty/tserv.M <slope>
use hwout 88 1544 100 0 hwout#173
xform 0 160 1584
p 160 1534 100 0 -1 val(outp):@/pty/tserv.M <readCmt %s>
use hwout 88 1288 100 0 hwout#242
xform 0 160 1328
p 160 1278 100 0 -1 val(outp):@/pty/tserv.M <debug>
use estringouts -176 1728 200 0 writeCmt
xform 0 -64 1824
p 160 1838 100 0 -1 DTYP:Ascii SIO
p -256 1630 100 0 0 OMSL:closed_loop
p -256 1662 100 0 0 PINI:NO
p -128 1872 100 768 1 PV:
p -112 1854 100 0 -1 SCAN:Passive
p -224 1259 100 0 0 typ(OUT):path
use estringouts -176 1504 200 0 readCmt
xform 0 -64 1600
p 160 1614 100 0 -1 DTYP:Ascii SIO
p -256 1406 100 0 0 OMSL:closed_loop
p -256 1438 100 0 0 PINI:NO
p -128 1648 100 768 1 PV:
p -112 1630 100 0 -1 SCAN:Passive
p -224 1035 100 0 0 typ(OUT):path
use eaos -176 1968 200 0 slope
xform 0 -64 2080
p 160 2078 100 0 -1 DTYP:Ascii SIO
p -448 2062 100 0 0 OMSL:supervisory
p -128 2144 100 768 1 PV:
p -112 2046 100 0 -1 SCAN:Passive
p -448 1692 100 0 0 typ(OUT):path
[comments]
