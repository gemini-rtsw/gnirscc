[schematic2]
uniq 68
[tools]
[detail]
w 3192 731 100 0 n#67 hwin.hwin#51.in 3216 720 3216 720 eais.pressureIG.INP
w 3192 1035 100 0 n#66 hwin.hwin#46.in 3216 1024 3216 1024 eais.pressureTC2.INP
w 3016 1371 100 0 n#63 hwin.hwin#45.in 3040 1360 3040 1360 eais.pressureTC1.INP
w 2696 2459 100 0 n#61 hwin.hwin#1.in 2720 2448 2720 2448 elongins.health.INP
w 2664 2139 100 0 n#60 hwin.hwin#10.in 2688 2128 2688 2128 elongins.parked.INP
w 2664 1819 100 0 n#59 hwin.hwin#29.in 2688 1808 2688 1808 eais.gratingWvlength.INP
w 3336 2475 100 0 n#55 hwin.hwin#18.in 3360 2464 3360 2464 elongins.state.INP
w 3384 2203 100 0 n#54 hwin.hwin#13.in 3408 2192 3408 2192 elongins.datumed.INP
w 3368 1851 100 0 n#53 hwin.hwin#31.in 3392 1840 3392 1840 elongins.gratingOrder.INP
w 3112 1563 100 0 n#52 hwin.hwin#35.in 3136 1552 3136 1552 eais.gratingTilt.INP
[cell use]
use eais 3216 615 100 0 pressureIG
xform 0 3344 688
p 3102 808 100 0 0 DTYP:vxWorks Variable (INST_IO)
p 2960 430 100 0 0 EGU:torr
p 2960 686 100 0 0 SCAN:1 second
use eais 3216 919 100 0 pressureTC2
xform 0 3344 992
p 3102 1112 100 0 0 DTYP:vxWorks Variable (INST_IO)
p 2960 734 100 0 0 EGU:torr
p 2960 990 100 0 0 SCAN:1 second
use eais 3136 1447 100 0 gratingTilt
xform 0 3264 1520
p 3152 1600 100 0 1 DTYP:vxWorks Variable (INST_IO)
p 2880 1262 100 0 0 EGU:degrees
p 3168 1648 100 0 1 SCAN:1 second
use eais 2688 1703 100 0 gratingWvlength
xform 0 2816 1776
p 2704 1856 100 0 1 DTYP:vxWorks Variable (INST_IO)
p 2432 1518 100 0 0 EGU:microns
p 2720 1904 100 0 1 SCAN:1 second
use eais 3040 1255 100 0 pressureTC1
xform 0 3168 1328
p 2926 1448 100 0 0 DTYP:vxWorks Variable (INST_IO)
p 2784 1070 100 0 0 EGU:torr
p 2784 1326 100 0 0 SCAN:1 second
use hwin 3024 679 100 0 hwin#51
xform 0 3120 720
p 3027 712 100 0 -1 val(in):@senTorrIg
use hwin 3024 983 100 0 hwin#46
xform 0 3120 1024
p 3027 1016 100 0 -1 val(in):@senTorrTc2
use hwin 2848 1319 100 0 hwin#45
xform 0 2944 1360
p 2851 1352 100 0 -1 val(in):@senTorrTc1
use hwin 2528 2407 100 0 hwin#1
xform 0 2624 2448
p 2531 2440 100 0 -1 val(in):@healthCC
use hwin 2496 2087 100 0 hwin#10
xform 0 2592 2128
p 2499 2120 100 0 -1 val(in):@parkedCC
use hwin 3216 2151 100 0 hwin#13
xform 0 3312 2192
p 3219 2184 100 0 -1 val(in):@datumedCC
use hwin 3168 2423 100 0 hwin#18
xform 0 3264 2464
p 3171 2456 100 0 -1 val(in):@initCCStatus
use hwin 2496 1767 100 0 hwin#29
xform 0 2592 1808
p 2499 1800 100 0 -1 val(in):@gratingWavelength
use hwin 3200 1799 100 0 hwin#31
xform 0 3296 1840
p 3203 1832 100 0 -1 val(in):@gratingOrder
use hwin 2944 1511 100 0 hwin#35
xform 0 3040 1552
p 2947 1544 100 0 -1 val(in):@gratingAngle
use elongins 2688 2023 100 0 parked
xform 0 2816 2096
p 2704 2224 100 0 1 DTYP:vxWorks Variable (INST_IO)
p 2704 2176 100 0 1 SCAN:1 second
use elongins 3408 2087 100 0 datumed
xform 0 3536 2160
p 3424 2288 100 0 1 DTYP:vxWorks Variable (INST_IO)
p 3424 2240 100 0 1 SCAN:1 second
use elongins 2720 2343 100 0 health
xform 0 2848 2416
p 2736 2544 100 0 1 DTYP:vxWorks Variable (INST_IO)
p 2736 2496 100 0 1 SCAN:1 second
use elongins 3360 2359 100 0 state
xform 0 3488 2432
p 3376 2560 100 0 1 DTYP:vxWorks Variable (INST_IO)
p 3376 2512 100 0 1 SCAN:1 second
use elongins 3392 1735 100 0 gratingOrder
xform 0 3520 1808
p 3408 1936 100 0 1 DTYP:vxWorks Variable (INST_IO)
p 3408 1888 100 0 1 SCAN:1 second
use bc200tr 1984 264 -100 0 frame
xform 0 3664 1568
p 4816 480 160 0 -1 File:sadInterface.sch
p 1984 312 100 0 0 revision:2.2
[comments]
