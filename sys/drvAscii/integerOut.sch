[schematic2]
uniq 501
[tools]
[detail]
w 2212 539 100 2 n#500 mbbodirects.mbbod5.OUT 2208 544 2208 544 hwout.hwout#498.outp
w 2404 891 100 2 n#499 mbbodirects.mbbod4.OUT 2400 896 2400 896 hwout.hwout#427.outp
w 2372 1915 100 2 n#495 elongouts.lo2.OUT 2368 1920 2368 1920 hwout.hwout#432.outp
w 2372 2139 100 2 n#494 elongouts.lo1.OUT 2368 2144 2368 2144 hwout.hwout#396.outp
w 2372 2363 100 2 n#493 elongouts.lo.OUT 2368 2368 2368 2368 hwout.hwout#395.outp
w 2372 1243 100 2 n#492 elongouts.lo5.OUT 2368 1248 2368 1248 hwout.hwout#438.outp
w 2372 1467 100 2 n#491 elongouts.lo4.OUT 2368 1472 2368 1472 hwout.hwout#434.outp
w 2372 1691 100 2 n#490 elongouts.lo3.OUT 2368 1696 2368 1696 hwout.hwout#433.outp
w 1668 411 100 2 n#479 hwout.hwout#424.outp 1664 416 1664 416 mbbodirects.mbbod3.OUT
w 1668 891 100 2 n#478 mbbodirects.mbbod2.OUT 1664 896 1664 896 hwout.hwout#350.outp
w 964 891 100 2 n#477 hwout.hwout#339.outp 960 896 960 896 mbbodirects.mbbod.OUT
w 964 411 100 2 n#476 mbbodirects.mbbod1.OUT 960 416 960 416 hwout.hwout#347.outp
w 132 315 100 2 n#475 hwout.hwout#324.outp 128 320 128 320 ebos.bo9.OUT
w 132 987 100 2 n#474 hwout.hwout#304.outp 128 992 128 992 ebos.bo6.OUT
w 132 763 100 2 n#473 hwout.hwout#309.outp 128 768 128 768 ebos.bo7.OUT
w 132 539 100 2 n#472 hwout.hwout#310.outp 128 544 128 544 ebos.bo8.OUT
w 132 2299 100 2 n#471 hwout.hwout#168.outp 128 2304 128 2304 elongouts.timeout.OUT
w 132 1291 100 2 n#470 hwout.hwout#173.outp 128 1296 128 1296 estringouts.readCmt.OUT
w 132 1515 100 2 n#469 hwout.hwout#151.outp 128 1520 128 1520 estringouts.writeCmt.OUT
w 132 2043 100 2 n#468 hwout.hwout#211.outp 128 2048 128 2048 elongouts.debug.OUT
w 132 1755 100 2 n#467 eaos.slope.OUT 128 1760 128 1760 hwout.hwout#171.outp
w 868 2395 100 2 n#466 hwout.hwout#83.outp 864 2400 864 2400 ebos.bo.OUT
w 868 1275 100 2 n#465 hwout.hwout#303.outp 864 1280 864 1280 ebos.bo5.OUT
w 868 1499 100 2 n#464 hwout.hwout#281.outp 864 1504 864 1504 ebos.bo4.OUT
w 868 1947 100 2 n#463 hwout.hwout#164.outp 864 1952 864 1952 ebos.bo2.OUT
w 868 1723 100 2 n#462 hwout.hwout#280.outp 864 1728 864 1728 ebos.bo3.OUT
w 868 2171 100 2 n#461 hwout.hwout#284.outp 864 2176 864 2176 ebos.bo1.OUT
w 1636 1275 100 2 n#460 hwout.hwout#419.outp 1632 1280 1632 1280 embbos.mbbo5.OUT
w 1636 1499 100 2 n#459 hwout.hwout#407.outp 1632 1504 1632 1504 embbos.mbbo4.OUT
w 1636 1723 100 2 n#458 embbos.mbbo3.OUT 1632 1728 1632 1728 hwout.hwout#400.outp
w 1636 1947 100 2 n#457 hwout.hwout#348.outp 1632 1952 1632 1952 embbos.mbbo2.OUT
w 1636 2395 100 0 n#456 hwout.hwout#337.outp 1632 2400 1632 2400 embbos.mbbo.OUT
w 1636 2171 100 2 n#455 hwout.hwout#345.outp 1632 2176 1632 2176 embbos.mbbo1.OUT
s -64 2496 100 0 RECORDS WHICH CONTROL
s -64 2464 100 0 LINK ASPECTS
s -64 2423 100 0 timeout control max time to wait for a response
s -64 1927 100 0 slope controls ao and ai conversions
s -64 1895 100 0 rval->%f, %f->rval
s -64 1863 100 0 refer to header of devAscii.c
s -64 1607 100 0 writeCmt = write command terminator
s -64 1383 100 0 readCmt = read command terminator
[cell use]
use hwout 888 2360 100 0 hwout#83
xform 0 960 2400
p 960 2350 100 0 -1 val(outp):@/pty/tserv.M <%d>
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
p 960 1678 100 0 -1 val(outp):@/pty/tserv.M <$1DO%d><$1DO%*d>
use hwout 888 1464 100 0 hwout#281
xform 0 960 1504
p 960 1454 100 0 -1 val(outp):@/pty/tserv.M (\x241DO%c)(\x241DO%*s)
use hwout 888 2136 100 0 hwout#284
xform 0 960 2176
p 960 2126 100 0 -1 val(outp):@/pty/tserv.M <%d><%*d>
use hwout 888 1912 100 0 hwout#164
xform 0 960 1952
p 974 1880 100 0 0 typ(outp):val
p 960 1902 100 0 -1 val(outp):@/pty/tserv.M <$1DO%d><%*k>
use hwout 888 1240 100 0 hwout#303
xform 0 960 1280
p 960 1230 100 0 -1 val(outp):@/pty/tserv.M <$1DO%b><$1DO%*d>
use hwout 152 952 100 0 hwout#304
xform 0 224 992
p 224 942 100 0 -1 val(outp):@/pty/tserv.M <$1DO%f><$1DO%*f>
use hwout 152 728 100 0 hwout#309
xform 0 224 768
p 224 718 100 0 -1 val(outp):@/pty/tserv.M <$1DO%dX><$1DO%*dX>
use hwout 152 504 100 0 hwout#310
xform 0 224 544
p 224 494 100 0 -1 val(outp):@/pty/tserv.M <$1DO%2x><$1DO%*x>
use hwout 152 280 100 0 hwout#324
xform 0 224 320
p 224 270 100 0 -1 val(outp):@/pty/tserv.M <%d><%*3c%*d>
use hwout 1656 2360 100 0 hwout#337
xform 0 1728 2400
p 1728 2350 100 0 -1 val(outp):@/pty/tserv.M <mb%d>
use hwout 984 856 100 0 hwout#339
xform 0 1056 896
p 1056 846 100 0 -1 val(outp):@/pty/tserv.M <mbd%d>
use hwout 1656 2136 100 0 hwout#345
xform 0 1728 2176
p 1728 2126 100 0 -1 val(outp):@/pty/tserv.M <mb%d><%*d>
use hwout 984 376 100 0 hwout#347
xform 0 1056 416
p 1056 366 100 0 -1 val(outp):@/pty/tserv.M <mbd%d><%*d>
use hwout 1656 1912 100 0 hwout#348
xform 0 1728 1952
p 1742 1880 100 0 0 typ(outp):val
p 1728 1902 100 0 -1 val(outp):@/pty/tserv.M <mb%f><%*f>
use hwout 1688 856 100 0 hwout#350
xform 0 1760 896
p 1774 824 100 0 0 typ(outp):val
p 1760 846 100 0 -1 val(outp):@/pty/tserv.M <mbd%x><%*x>
use hwout 2392 2328 100 0 hwout#395
xform 0 2464 2368
p 2464 2318 100 0 -1 val(outp):@/pty/tserv.M <lo%d>
use hwout 2392 2104 100 0 hwout#396
xform 0 2464 2144
p 2464 2094 100 0 -1 val(outp):@/pty/tserv.M <lo%d><%*d>
use hwout 1656 1688 100 0 hwout#400
xform 0 1728 1728
p 1742 1656 100 0 0 typ(outp):val
p 1728 1678 100 0 -1 val(outp):@/pty/tserv.M <mb%5b><%*k>
use hwout 1656 1464 100 0 hwout#407
xform 0 1728 1504
p 1742 1432 100 0 0 typ(outp):val
p 1728 1454 100 0 -1 val(outp):@/pty/tserv.M [mb%4c][%*k]
use hwout 1656 1240 100 0 hwout#419
xform 0 1728 1280
p 1742 1208 100 0 0 typ(outp):val
p 1728 1230 100 0 -1 val(outp):@/pty/tserv.M <mb%x><%*x>
use hwout 1688 376 100 0 hwout#424
xform 0 1760 416
p 1774 344 100 0 0 typ(outp):val
p 1760 366 100 0 -1 val(outp):@/pty/tserv.M <mbd%16b><%*d>
use hwout 2424 856 100 0 hwout#427
xform 0 2496 896
p 2510 824 100 0 0 typ(outp):val
p 2496 846 100 0 -1 val(outp):@/pty/tserv.M <mbd%4c><%*k>
use hwout 2392 1880 100 0 hwout#432
xform 0 2464 1920
p 2478 1848 100 0 0 typ(outp):val
p 2464 1870 100 0 -1 val(outp):@/pty/tserv.M <lo%f><%*f>
use hwout 2392 1656 100 0 hwout#433
xform 0 2464 1696
p 2478 1624 100 0 0 typ(outp):val
p 2464 1646 100 0 -1 val(outp):@/pty/tserv.M <lo%32b><%*d>
use hwout 2392 1432 100 0 hwout#434
xform 0 2464 1472
p 2478 1400 100 0 0 typ(outp):val
p 2464 1422 100 0 -1 val(outp):@/pty/tserv.M {lo%4c}{%*k}
use hwout 2392 1208 100 0 hwout#438
xform 0 2464 1248
p 2478 1176 100 0 0 typ(outp):val
p 2464 1198 100 0 -1 val(outp):@/pty/tserv.M <lo%x><%*x>
use hwout 2232 504 100 0 hwout#498
xform 0 2304 544
p 2318 472 100 0 0 typ(outp):val
p 2304 494 100 0 -1 val(outp):@/pty/tserv.M <mbd%f><%*f>
use mbbodirects 1376 743 200 0 mbbod2
xform 0 1520 944
p 1744 928 100 0 -1 DTYP:Ascii SIO
p 1728 510 100 0 0 OMSL:supervisory
p 1440 1136 100 0 1 PV: 
p 1568 832 100 0 1 pproc(OUT):PP
p 1728 318 100 0 0 typ(OUT):path
use mbbodirects 672 263 200 0 mbbod1
xform 0 816 464
p 1056 448 100 0 -1 DTYP:Ascii SIO
p 1024 30 100 0 0 OMSL:supervisory
p 736 656 100 0 1 PV: 
p 864 352 100 0 1 pproc(OUT):PP
p 1024 -162 100 0 0 typ(OUT):path
use mbbodirects 672 743 200 0 mbbod
xform 0 816 944
p 1056 928 100 0 -1 DTYP:Ascii SIO
p 1024 510 100 0 0 OMSL:supervisory
p 736 1136 100 0 1 PV: 
p 864 832 100 0 1 pproc(OUT):PP
p 1024 318 100 0 0 typ(OUT):path
use mbbodirects 1376 263 200 0 mbbod3
xform 0 1520 464
p 1744 448 100 0 -1 DTYP:Ascii SIO
p 1728 30 100 0 0 OMSL:supervisory
p 1440 656 100 0 1 PV: 
p 1568 352 100 0 1 pproc(OUT):PP
p 1728 -162 100 0 0 typ(OUT):path
use mbbodirects 2112 743 200 0 mbbod4
xform 0 2256 944
p 2480 928 100 0 -1 DTYP:Ascii SIO
p 2464 510 100 0 0 OMSL:supervisory
p 2176 1136 100 0 1 PV: 
p 2304 832 100 0 1 pproc(OUT):PP
p 2464 318 100 0 0 typ(OUT):path
use mbbodirects 1920 391 200 0 mbbod5
xform 0 2064 592
p 2288 576 100 0 -1 DTYP:Ascii SIO
p 2272 158 100 0 0 OMSL:supervisory
p 1984 784 100 0 1 PV: 
p 2112 480 100 0 1 pproc(OUT):PP
p 2272 -34 100 0 0 typ(OUT):path
use embbos 1376 2311 200 0 mbbo
xform 0 1504 2400
p 1728 2432 100 0 -1 DTYP:Ascii SIO
p 1536 2462 100 0 0 ONST:one
p 1344 2462 100 0 0 ONVL:1
p 1440 2464 100 768 1 PV:
p 1536 2398 100 0 0 THST:three
p 1344 2398 100 0 0 THVL:3
p 1536 2430 100 0 0 TWST:two
p 1344 2430 100 0 0 TWVL:2
p 1536 2494 100 0 0 ZRST:zero
use embbos 1376 2087 200 0 mbbo1
xform 0 1504 2176
p 1712 2208 100 0 -1 DTYP:Ascii SIO
p 1536 2238 100 0 0 ONST:one
p 1344 2238 100 0 0 ONVL:1
p 1440 2240 100 768 1 PV:
p 1536 2174 100 0 0 THST:three
p 1344 2174 100 0 0 THVL:3
p 1536 2206 100 0 0 TWST:two
p 1344 2206 100 0 0 TWVL:2
p 1536 2270 100 0 0 ZRST:zero
use embbos 1376 1863 200 0 mbbo2
xform 0 1504 1952
p 1712 1984 100 0 -1 DTYP:Ascii SIO
p 1536 2014 100 0 0 ONST:one
p 1344 2014 100 0 0 ONVL:1
p 1440 2016 100 768 1 PV:
p 1536 1950 100 0 0 THST:three
p 1344 1950 100 0 0 THVL:3
p 1536 1982 100 0 0 TWST:two
p 1344 1982 100 0 0 TWVL:2
p 1536 2046 100 0 0 ZRST:zero
use embbos 1376 1639 200 0 mbbo3
xform 0 1504 1728
p 1712 1760 100 0 -1 DTYP:Ascii SIO
p 1536 1790 100 0 0 ONST:one
p 1344 1790 100 0 0 ONVL:1
p 1440 1792 100 768 1 PV:
p 1536 1726 100 0 0 THST:three
p 1344 1726 100 0 0 THVL:3
p 1536 1758 100 0 0 TWST:two
p 1344 1758 100 0 0 TWVL:2
p 1536 1822 100 0 0 ZRST:zero
use embbos 1376 1415 200 0 mbbo4
xform 0 1504 1504
p 1728 1536 100 0 -1 DTYP:Ascii SIO
p 1536 1566 100 0 0 ONST:one
p 1344 1566 100 0 0 ONVL:1
p 1440 1568 100 768 1 PV:
p 1536 1502 100 0 0 THST:three
p 1344 1502 100 0 0 THVL:3
p 1536 1534 100 0 0 TWST:two
p 1344 1534 100 0 0 TWVL:2
p 1536 1598 100 0 0 ZRST:zero
use embbos 1376 1191 200 0 mbbo5
xform 0 1504 1280
p 1712 1312 100 0 -1 DTYP:Ascii SIO
p 1536 1342 100 0 0 ONST:one
p 1344 1342 100 0 0 ONVL:1
p 1440 1344 100 768 1 PV:
p 1536 1278 100 0 0 THST:three
p 1344 1278 100 0 0 THVL:3
p 1536 1310 100 0 0 TWST:two
p 1344 1310 100 0 0 TWVL:2
p 1536 1374 100 0 0 ZRST:zero
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
use elongouts 2112 2311 200 0 lo
xform 0 2240 2400
p 2448 2400 100 0 -1 DTYP:Ascii SIO
p 2176 2464 100 768 1 PV:
use elongouts 2112 2087 200 0 lo1
xform 0 2240 2176
p 2448 2176 100 0 -1 DTYP:Ascii SIO
p 2176 2240 100 768 1 PV:
use elongouts 2112 1863 200 0 lo2
xform 0 2240 1952
p 2464 1936 100 0 -1 DTYP:Ascii SIO
p 2176 2016 100 768 1 PV:
use elongouts 2112 1639 200 0 lo3
xform 0 2240 1728
p 2448 1728 100 0 -1 DTYP:Ascii SIO
p 2176 1792 100 768 1 PV:
use elongouts 2112 1415 200 0 lo4
xform 0 2240 1504
p 2464 1504 100 0 -1 DTYP:Ascii SIO
p 2176 1568 100 768 1 PV:
use elongouts 2112 1191 200 0 lo5
xform 0 2240 1280
p 2464 1280 100 0 -1 DTYP:Ascii SIO
p 2176 1344 100 768 1 PV:
use ebos 632 2344 200 0 bo
xform 0 736 2432
p 944 2430 100 0 -1 DTYP:Ascii SIO
p 672 2496 100 768 1 PV:
p 688 2398 100 0 -1 SCAN:Passive
p 288 2012 100 0 0 typ(OUT):path
use ebos 632 1672 200 0 bo3
xform 0 736 1760
p 944 1758 100 0 -1 DTYP:Ascii SIO
p 672 1824 100 768 1 PV:
p 688 1726 100 0 -1 SCAN:Passive
p 288 1340 100 0 0 typ(OUT):path
use ebos 632 1448 200 0 bo4
xform 0 736 1536
p 944 1534 100 0 -1 DTYP:Ascii SIO
p 672 1600 100 768 1 PV:
p 688 1502 100 0 -1 SCAN:Passive
p 288 1116 100 0 0 typ(OUT):path
use ebos 632 2120 200 0 bo1
xform 0 736 2208
p 944 2206 100 0 -1 DTYP:Ascii SIO
p 672 2272 100 768 1 PV:
p 688 2174 100 0 -1 SCAN:Passive
p 288 1788 100 0 0 typ(OUT):path
use ebos 632 1896 200 0 bo2
xform 0 736 1984
p 960 1982 100 0 -1 DTYP:Ascii SIO
p 672 2048 100 768 1 PV:
p 688 1950 100 0 -1 SCAN:Passive
p 288 1564 100 0 0 typ(OUT):path
use ebos 632 1224 200 0 bo5
xform 0 736 1312
p 944 1310 100 0 -1 DTYP:Ascii SIO
p 672 1376 100 768 1 PV:
p 688 1278 100 0 -1 SCAN:Passive
p 288 892 100 0 0 typ(OUT):path
use ebos -104 936 200 0 bo6
xform 0 0 1024
p 208 1022 100 0 -1 DTYP:Ascii SIO
p -64 1088 100 768 1 PV:
p -48 990 100 0 -1 SCAN:Passive
p -448 604 100 0 0 typ(OUT):path
use ebos -104 712 200 0 bo7
xform 0 0 800
p 208 798 100 0 -1 DTYP:Ascii SIO
p -64 864 100 768 1 PV:
p -48 766 100 0 -1 SCAN:Passive
p -448 380 100 0 0 typ(OUT):path
use ebos -104 488 200 0 bo8
xform 0 0 576
p 208 574 100 0 -1 DTYP:Ascii SIO
p -64 640 100 768 1 PV:
p -48 542 100 0 -1 SCAN:Passive
p -448 156 100 0 0 typ(OUT):path
use ebos -104 264 200 0 bo9
xform 0 0 352
p 208 350 100 0 -1 DTYP:Ascii SIO
p -64 416 100 768 1 PV:
p -48 318 100 0 -1 SCAN:Passive
p -448 -68 100 0 0 typ(OUT):path
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
use eaos -112 1680 200 0 slope
xform 0 0 1792
p 224 1790 100 0 -1 DTYP:Ascii SIO
p -384 1774 100 0 0 OMSL:supervisory
p -64 1856 100 768 1 PV:
p -48 1758 100 0 -1 SCAN:Passive
p -384 1404 100 0 0 typ(OUT):path
[comments]
