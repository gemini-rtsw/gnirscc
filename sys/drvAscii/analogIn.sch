[schematic2]
uniq 301
[tools]
[detail]
w 1796 1707 100 2 n#300 hwin.hwin#297.in 1792 1712 1792 1712 eais.ai13.INP
w 900 939 100 2 n#299 eais.ai6.INP 896 944 896 944 hwin.hwin#232.in
w 1796 1323 100 2 n#295 hwin.hwin#289.in 1792 1328 1792 1328 eais.ai11.INP
w 1796 1515 100 2 n#287 hwin.hwin#293.in 1792 1520 1792 1520 eais.ai12.INP
w 1796 1899 100 2 n#291 eais.ai7.INP 1792 1904 1792 1904 hwin.hwin#256.in
w 1796 2091 100 2 n#278 hwin.hwin#285.in 1792 2096 1792 2096 eais.ai8.INP
w 900 1515 100 2 n#283 hwin.hwin#223.in 896 1520 896 1520 eais.ai3.INP
w 1796 939 100 2 n#282 hwin.hwin#279.in 1792 944 1792 944 eais.ai10.INP
w 1812 1131 100 2 n#281 hwin.hwin#252.in 1808 1136 1808 1136 eais.ai9.INP
w 900 1131 100 2 n#271 eais.ai5.INP 896 1136 896 1136 hwin.hwin#229.in
w 900 1323 100 2 n#269 eais.ai4.INP 896 1328 896 1328 hwin.hwin#226.in
w 900 1707 100 2 n#266 eais.ai2.INP 896 1712 896 1712 hwin.hwin#220.in
w 900 1899 100 2 n#264 eais.ai1.INP 896 1904 896 1904 hwin.hwin#217.in
w 900 2091 100 2 n#262 eais.ai.INP 896 2096 896 2096 hwin.hwin#213.in
w 164 1179 100 2 n#246 hwout.hwout#242.outp 160 1184 160 1184 elongouts.debug.OUT
w 164 1435 100 2 n#244 hwout.hwout#173.outp 160 1440 160 1440 estringouts.readCmt.OUT
w 130 2242 100 0 n#206 elongouts.timeout.OUT 160 2240 160 2240 hwout.hwout#168.outp
w 136 1906 100 0 n#169 eaos.slope.OUT 160 1904 160 1904 hwout.hwout#171.outp
w 136 1666 100 0 n#153 hwout.hwout#151.outp 160 1664 160 1664 estringouts.writeCmt.OUT
s -32 1527 100 0 readCmt = read command terminator
s -32 1751 100 0 writeCmt = write command terminator
s -32 2007 100 0 refer to header of devAscii.c
s -32 2039 100 0 rval->%f, %f->rval
s -32 2071 100 0 slope controls ao and ai conversions
s -32 2359 100 0 timeout control max time to wait for a response
s -32 2400 100 0 LINK ASPECTS
s -32 2432 100 0 RECORDS WHICH CONTROL
s 864 992 100 0 non-standard field delimiters
[cell use]
use eais 1792 1223 200 0 ai11
xform 0 1920 1296
p 1600 1360 100 0 -1 DTYP:Ascii SIO
p 1536 1102 100 0 0 EGUF:1.0
p 1856 1344 100 768 1 PV:
use eais 1792 1799 200 0 ai7
xform 0 1920 1872
p 1600 1936 100 0 -1 DTYP:Ascii SIO
p 1536 1678 100 0 0 EGUF:1.0
p 1856 1920 100 768 1 PV:
use eais 896 839 200 0 ai6
xform 0 1024 912
p 704 976 100 0 -1 DTYP:Ascii SIO
p 640 718 100 0 0 EGUF:1.0
p 960 960 100 768 1 PV:
use eais 1808 1031 200 0 ai9
xform 0 1936 1104
p 1616 1168 100 0 -1 DTYP:Ascii SIO
p 1552 910 100 0 0 EGUF:1.0
p 1872 1152 100 768 1 PV:
use eais 896 1031 200 0 ai5
xform 0 1024 1104
p 704 1168 100 0 -1 DTYP:Ascii SIO
p 640 910 100 0 0 EGUF:1.0
p 960 1152 100 768 1 PV:
use eais 896 1223 200 0 ai4
xform 0 1024 1296
p 704 1360 100 0 -1 DTYP:Ascii SIO
p 640 1102 100 0 0 EGUF:1.0
p 960 1344 100 768 1 PV:
use eais 896 1415 200 0 ai3
xform 0 1024 1488
p 704 1552 100 0 -1 DTYP:Ascii SIO
p 640 1294 100 0 0 EGUF:1.0
p 960 1536 100 768 1 PV:
use eais 896 1607 200 0 ai2
xform 0 1024 1680
p 704 1744 100 0 -1 DTYP:Ascii SIO
p 640 1486 100 0 0 EGUF:1.0
p 960 1728 100 768 1 PV:
use eais 896 1799 200 0 ai1
xform 0 1024 1872
p 704 1936 100 0 -1 DTYP:Ascii SIO
p 640 1678 100 0 0 EGUF:1.0
p 960 1920 100 768 1 PV:
use eais 896 1991 200 0 ai
xform 0 1024 2064
p 704 2128 100 0 -1 DTYP:Ascii SIO
p 640 1870 100 0 0 EGUF:1.0
p 960 2112 100 768 1 PV:
use eais 1792 839 200 0 ai10
xform 0 1920 912
p 1600 976 100 0 -1 DTYP:Ascii SIO
p 1536 718 100 0 0 EGUF:1.0
p 1856 960 100 768 1 PV:
use eais 1792 1991 200 0 ai8
xform 0 1920 2064
p 1600 2128 100 0 -1 DTYP:Ascii SIO
p 1856 2112 100 768 1 PV:
use eais 1792 1415 200 0 ai12
xform 0 1920 1488
p 1600 1552 100 0 -1 DTYP:Ascii SIO
p 1856 1536 100 768 1 PV:
use eais 1792 1607 200 0 ai13
xform 0 1920 1680
p 1600 1744 100 0 -1 DTYP:Ascii SIO
p 1536 1486 100 0 0 EGUF:1.0
p 1856 1728 100 768 1 PV:
use hwin 1624 1288 100 0 hwin#289
xform 0 1696 1328
p 1472 1278 100 0 -1 val(in):@/pty/tserv.M <(4 char)?><%1k%3c>
use hwin 1640 1096 100 0 hwin#252
xform 0 1712 1136
p 1488 1086 100 0 -1 val(in):@/pty/tserv.M <?><*AI1%fX>
use hwin 728 2056 100 0 hwin#213
xform 0 800 2096
p 576 2046 100 0 -1 val(in):@/pty/tserv.M <?><%f>
use hwin 728 1864 100 0 hwin#217
xform 0 800 1904
p 576 1854 100 0 -1 val(in):@/pty/tserv.M <AI(int)?><%d>
use hwin 728 1672 100 0 hwin#220
xform 0 800 1712
p 576 1662 100 0 -1 val(in):@/pty/tserv.M <AI(scientific)?><%e>
use hwin 728 1480 100 0 hwin#223
xform 0 800 1520
p 576 1470 100 0 -1 val(in):@/pty/tserv.M <(char)?><%c>
use hwin 728 1288 100 0 hwin#226
xform 0 800 1328
p 576 1278 100 0 -1 val(in):@/pty/tserv.M <(4 char)?><%4c>
use hwin 728 1096 100 0 hwin#229
xform 0 800 1136
p 576 1086 100 0 -1 val(in):@/pty/tserv.M <(bin)?><%b>
use hwin 728 904 100 0 hwin#232
xform 0 800 944
p 576 894 100 0 -1 val(in):@/pty/tserv.M X?XX%4k%eX
use hwin 1624 1864 100 0 hwin#256
xform 0 1696 1904
p 1472 1854 100 0 -1 val(in):@/pty/tserv.M <?><%1kI%f>
use hwin 1624 904 100 0 hwin#279
xform 0 1696 944
p 1472 894 100 0 -1 val(in):@/pty/tserv.M <D><D%*d;%*fs;%*d;%*3c;%*d;%f;%*d;%*d>
use hwin 1624 2056 100 0 hwin#285
xform 0 1696 2096
p 1472 2046 100 0 -1 val(in):@/pty/tserv.M REAL <(float)?><%f>
use hwin 1624 1480 100 0 hwin#293
xform 0 1696 1520
p 1472 1470 100 0 -1 val(in):@/pty/tserv.M REAL <(4 char)?><%4c>
use hwin 1624 1672 100 0 hwin#297
xform 0 1696 1712
p 1472 1662 100 0 -1 val(in):@/pty/tserv.M <(hex)?><%x>
use elongouts -96 1127 100 0 debug
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
use eaos -80 1824 200 0 slope
xform 0 32 1936
p 256 1934 100 0 -1 DTYP:Ascii SIO
p -352 1918 100 0 0 OMSL:supervisory
p -32 2000 100 768 1 PV:
p -16 1902 100 0 -1 SCAN:Passive
p -352 1548 100 0 0 typ(OUT):path
[comments]
