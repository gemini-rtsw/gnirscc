[schematic2]
uniq 268
[tools]
[detail]
w 56 771 100 0 n#263 hwout.hwout#265.outp 80 768 80 768 eaos.setMntDGain.OUT
w -466 771 100 0 n#262 hwout.hwout#264.outp -448 768 -448 768 eaos.setFootDGain.OUT
w 698 1835 100 0 n#261 eaos.mntHtrDAC.VAL 664 1824 792 1824 esubs.mntHtrDACSub.INPA
w 700 1643 100 0 n#260 eaos.mntHtrDAC.FLNK 664 1856 696 1856 696 1440 792 1440 esubs.mntHtrDACSub.SLNK
w 1130 1603 100 0 n#259 esubs.mntHtrDACSub.FLNK 1080 1664 1112 1664 1112 1600 1208 1600 eaos.setMntHtrDAC.SLNK
w 1114 1635 100 0 n#258 esubs.mntHtrDACSub.VAL 1080 1632 1208 1632 eaos.setMntHtrDAC.DOL
w 18 1531 100 0 n#253 esubs.footHtrDACSub.VAL -16 1528 112 1528 eaos.setFootHtrDAC.DOL
w 34 1499 100 0 n#252 esubs.footHtrDACSub.FLNK -16 1560 16 1560 16 1496 112 1496 eaos.setFootHtrDAC.SLNK
w -396 1539 100 0 n#251 eaos.footHtrDAC.FLNK -432 1752 -400 1752 -400 1336 -304 1336 esubs.footHtrDACSub.SLNK
w -398 1731 100 0 n#250 eaos.footHtrDAC.VAL -432 1720 -304 1720 esubs.footHtrDACSub.INPA
w 130 2083 100 0 n#249 esubs.footAoDACSub.VAL 96 2080 224 2080 eaos.setFootDAC.DOL
w 1108 2179 100 0 n#244 esubs.mntAoDACSub.FLNK 1080 2216 1112 2216 1112 2152 1176 2152 eaos.setMntDAC.SLNK
w 1098 2187 100 0 n#243 esubs.mntAoDACSub.VAL 1080 2184 1176 2184 eaos.setMntDAC.DOL
w 692 2195 100 0 n#242 eaos.mntDesiredTemp.FLNK 608 2408 696 2408 696 1992 792 1992 esubs.mntAoDACSub.SLNK
w 670 2379 100 0 n#241 eaos.mntDesiredTemp.VAL 608 2376 792 2376 esubs.mntAoDACSub.INPA
w 146 2051 100 0 n#238 esubs.footAoDACSub.FLNK 96 2112 128 2112 128 2048 224 2048 eaos.setFootDAC.SLNK
w -284 2091 100 0 n#235 eaos.footDesiredTemp.FLNK -320 2304 -288 2304 -288 1888 -192 1888 esubs.footAoDACSub.SLNK
w -286 2283 100 0 n#234 eaos.footDesiredTemp.VAL -320 2272 -192 2272 esubs.footAoDACSub.INPA
w 1448 1571 100 0 n#53 eaos.setMntHtrDAC.OUT 1472 1568 1472 1568 hwout.hwout#33.outp
w 64 315 100 0 n#52 hwout.hwout#36.outp 88 312 88 312 eaos.setMntIGain.OUT
w 1408 2131 100 0 n#49 eaos.setMntDAC.OUT 1432 2120 1432 2120 hwout.hwout#41.outp
w 48 539 100 0 n#47 eaos.setMntPGain.OUT 72 536 72 536 hwout.hwout#45.outp
w 344 1467 100 0 n#29 eaos.setFootHtrDAC.OUT 368 1464 368 1464 hwout.hwout#25.outp
w -464 315 100 0 n#23 hwout.hwout#22.outp -440 312 -440 312 eaos.setFootIGain.OUT
w 440 2019 100 0 n#17 eaos.setFootDAC.OUT 480 2016 448 2016 hwout.hwout#13.outp
w -474 539 100 0 n#9 eaos.setFootPGain.OUT -456 536 -456 536 hwout.hwout#8.outp
[cell use]
use eaos 480 1904 100 0 mntHtrDAC
xform 0 536 1824
p 152 1806 100 0 0 OMSL:supervisory
p 152 1678 100 0 0 PREC:2
use eaos -616 1800 100 0 footHtrDAC
xform 0 -560 1720
p -944 1702 100 0 0 OMSL:supervisory
p -944 1574 100 0 0 PREC:2
use eaos -648 640 100 0 setFootPGain
xform 0 -584 568
p -825 802 100 0 0 DTYP:tempCard
p -968 422 100 0 0 PREC:3
p -948 -14 100 0 1 VAL:50.0
p -968 116 100 0 0 typ(DOL):path
use eaos 288 2120 100 0 setFootDAC
xform 0 352 2048
p 111 2282 100 0 0 DTYP:tempCard
p -32 2030 100 0 0 OMSL:closed_loop
p -32 1902 100 0 0 PREC:3
use eaos -632 416 100 0 setFootIGain
xform 0 -568 344
p -809 578 100 0 0 DTYP:tempCard
p -952 198 100 0 0 PREC:3
use eaos 176 1568 100 0 setFootHtrDAC
xform 0 240 1496
p -1 1730 100 0 0 DTYP:tempCard
p -144 1478 100 0 0 OMSL:closed_loop
p -144 1350 100 0 0 PREC:3
use eaos 1280 1672 100 0 setMntHtrDAC
xform 0 1344 1600
p 1103 1834 100 0 0 DTYP:tempCard
p 960 1582 100 0 0 OMSL:closed_loop
p 960 1454 100 0 0 PREC:3
use eaos -104 416 100 0 setMntIGain
xform 0 -40 344
p -281 578 100 0 0 DTYP:tempCard
p -424 198 100 0 0 PREC:3
use eaos 1240 2224 100 0 setMntDAC
xform 0 1304 2152
p 1063 2386 100 0 0 DTYP:tempCard
p 920 2134 100 0 0 OMSL:closed_loop
p 920 2006 100 0 0 PREC:3
use eaos -120 640 100 0 setMntPGain
xform 0 -56 568
p -297 802 100 0 0 DTYP:tempCard
p -440 422 100 0 0 PREC:3
use eaos -504 2352 100 0 footDesiredTemp
xform 0 -448 2272
p -832 2254 100 0 0 OMSL:supervisory
p -832 2126 100 0 0 PREC:2
use eaos 424 2456 100 0 mntDesiredTemp
xform 0 480 2376
p 96 2230 100 0 0 PREC:2
use eaos -640 872 100 0 setFootDGain
xform 0 -576 800
p -817 1034 100 0 0 DTYP:tempCard
p -960 654 100 0 0 PREC:3
p -940 218 100 0 1 VAL:50.0
p -960 348 100 0 0 typ(DOL):path
use eaos -112 872 100 0 setMntDGain
xform 0 -48 800
p -289 1034 100 0 0 DTYP:tempCard
p -432 654 100 0 0 PREC:3
use hwout -456 495 100 0 hwout#8
xform 0 -360 536
p -360 527 100 0 -1 val(outp):@p 0
use hwout 448 1975 100 0 hwout#13
xform 0 544 2016
p 544 2007 100 0 -1 val(outp):@t 0
use hwout -440 271 100 0 hwout#22
xform 0 -344 312
p -344 303 100 0 -1 val(outp):@i 0
use hwout 368 1423 100 0 hwout#25
xform 0 464 1464
p 464 1455 100 0 -1 val(outp):@h 0
use hwout 1472 1527 100 0 hwout#33
xform 0 1568 1568
p 1624 1568 100 0 -1 val(outp):@h 1
use hwout 88 271 100 0 hwout#36
xform 0 184 312
p 184 303 100 0 -1 val(outp):@i 1
use hwout 1432 2079 100 0 hwout#41
xform 0 1528 2120
p 1528 2111 100 0 -1 val(outp):@t 1
use hwout 72 495 100 0 hwout#45
xform 0 168 536
p 168 527 100 0 -1 val(outp):@p 1
use hwout -448 727 100 0 hwout#264
xform 0 -352 768
p -352 759 100 0 -1 val(outp):@d 0
use hwout 80 727 100 0 hwout#265
xform 0 176 768
p 176 759 100 0 -1 val(outp):@d 1
use esubs 864 1864 100 0 mntHtrDACSub
xform 0 936 1616
p 504 1470 100 0 0 INAM:initConvertTemp
p 504 1438 100 0 0 SNAM:cnvrtTempAo
p 488 1688 100 0 1 def(INPB):1.0
p 488 1656 100 0 1 def(INPC):199.0
p 488 1624 100 0 1 def(INPD):24.0
p 504 1566 100 0 1 def(INPE):1000.0
p 504 1534 100 0 1 def(INPF):2730.0
p 569 1254 100 0 0 typ(FLNK):path
use esubs -232 1760 100 0 footHtrDACSub
xform 0 -160 1512
p -592 1366 100 0 0 INAM:initConvertTemp
p -592 1398 100 0 0 PREC:3
p -592 1334 100 0 0 SNAM:cnvrtTempAo
p -608 1616 100 0 1 def(INPB):1
p -608 1592 100 0 1 def(INPC):1000.0
p -608 1568 100 0 1 def(INPD):120.0
p -608 1544 100 0 1 def(INPE):1000.0
p -608 1512 100 0 1 def(INPF):2730.0
p -527 1150 100 0 0 typ(FLNK):path
use esubs -120 2312 100 0 footAoDACSub
xform 0 -48 2064
p -480 1918 100 0 0 INAM:initConvertTemp
p -480 1886 100 0 0 SNAM:cnvrtTempAo
p -496 2136 100 0 1 def(INPB):0
p -496 2104 100 0 1 def(INPC):2462.0
p -415 1702 100 0 0 typ(FLNK):path
use esubs 864 2416 100 0 mntAoDACSub
xform 0 936 2168
p 504 2022 100 0 0 INAM:initConvertTemp
p 504 1990 100 0 0 SNAM:cnvrtTempAo
p 488 2248 100 0 1 def(INPB):0
p 488 2216 100 0 1 def(INPC):2460.0
use eborderC -896 95 100 0 eborderC#0
xform 0 784 1400
p 1904 296 200 768 -1 file:newTempCntrl
[comments]
