[schematic2]
uniq 114
[tools]
[detail]
w -2268 -2325 100 2 n#113 hwin.hwin#112.in -2272 -2320 -2272 -2320 estringouts.readCmt.DOL
w -2268 -2133 100 2 n#111 hwin.hwin#110.in -2272 -2128 -2272 -2128 estringouts.writeCmt.DOL
w -2012 -2181 100 2 n#109 hwout.hwout#100.outp -2016 -2176 -2016 -2176 estringouts.writeCmt.OUT
w -2268 -3221 100 2 n#107 hwin.hwin#106.in -2272 -3216 -2272 -3216 ebis.connectSts.INP
w -2012 -3077 100 2 n#105 hwout.hwout#104.outp -2016 -3072 -2016 -3072 ebos.connect.OUT
w -2268 -3013 100 2 n#103 hwin.hwin#102.in -2272 -3008 -2272 -3008 ebos.connect.DOL
w -2012 -2373 100 2 n#91 hwout.hwout#90.outp -2016 -2368 -2016 -2368 estringouts.readCmt.OUT
w -2040 -2821 100 0 n#21 elongouts.timeout.OUT -2016 -2832 -2016 -2832 hwout.hwout#27.outp
w -2296 -2757 100 0 n#20 elongouts.timeout.DOL -2272 -2768 -2272 -2768 hwin.hwin#25.in
w -2290 -2533 100 0 n#19 eaos.slope.DOL -2272 -2544 -2272 -2544 hwin.hwin#24.in
w -2034 -2597 100 0 n#18 eaos.slope.OUT -2016 -2608 -2016 -2608 hwout.hwout#26.outp
s -1248 -3328 100 0 drvAscii serial line control records
[cell use]
use hwin -2464 -2361 100 0 hwin#112
xform 0 -2368 -2320
p -2461 -2328 100 0 -1 val(in):$(RD_TERM)
use hwin -2464 -3257 100 0 hwin#106
xform 0 -2368 -3216
p -2624 -3200 100 768 -1 val(in):@$(DEVICE) <connectSts>
use hwin -2464 -3049 100 0 hwin#102
xform 0 -2368 -3008
p -2461 -3016 100 0 -1 val(in):$(CONNECT)
use hwin -2464 -2809 100 0 hwin#25
xform 0 -2368 -2768
p -2461 -2776 100 0 -1 val(in):$(TIMEOUT)
use hwin -2464 -2585 100 0 hwin#24
xform 0 -2368 -2544
p -2461 -2552 100 0 -1 val(in):$(SLOPE)
use hwin -2464 -2169 100 0 hwin#110
xform 0 -2368 -2128
p -2461 -2136 100 0 -1 val(in):$(WR_TERM)
use ebis -2272 -3321 100 0 connectSts
xform 0 -2144 -3248
p -2208 -3200 100 768 -1 DTYP:Ascii SIO
p -2496 -3410 100 0 0 ONAM:Connected
p -2476 -3876 100 0 0 PV:$(MY_PV)
p -2496 -3378 100 0 0 ZNAM:Disconnected
use bb200tr -3264 -3544 -100 0 frame
xform 0 -1984 -2720
use hwout -2016 -3113 100 0 hwout#104
xform 0 -1920 -3072
p -1920 -3056 100 768 -1 val(outp):@$(DEVICE) <connect>
use hwout -2016 -2217 100 0 hwout#100
xform 0 -1920 -2176
p -1920 -2160 100 768 -1 val(outp):@$(DEVICE) <writeCmt %s>
use hwout -2016 -2409 100 0 hwout#90
xform 0 -1920 -2368
p -1920 -2352 100 768 -1 val(outp):@$(DEVICE) <readCmt %s>
use hwout -2016 -2873 100 0 hwout#27
xform 0 -1920 -2832
p -1920 -2816 100 768 -1 val(outp):@$(DEVICE) <timeout>
use hwout -2016 -2649 100 0 hwout#26
xform 0 -1920 -2608
p -1920 -2592 100 768 -1 val(outp):@$(DEVICE) <slope>
use ebos -2272 -3129 100 0 connect
xform 0 -2144 -3040
p -2208 -2976 100 768 -1 DTYP:Ascii SIO
p -2592 -3090 100 0 0 OMSL:closed_loop
p -2592 -3186 100 0 0 ONAM:Connected
p -2592 -3058 100 0 0 PINI:$(CONNECT_PINI)
p -2572 -3640 100 0 0 PV:$(MY_PV)
p -2592 -3154 100 0 0 ZNAM:Disconnected
use estringouts -2272 -2233 100 0 writeCmt
xform 0 -2144 -2160
p -2208 -2112 100 768 -1 DTYP:Ascii SIO
p -2336 -2354 100 0 0 OMSL:closed_loop
p -2336 -2322 100 0 0 PINI:$(WR_TERM_PINI)
p -2284 -2820 100 0 1 PV:$(MY_PV)
p -2336 -2194 100 0 0 VAL:$(WR_TERM)
use estringouts -2272 -2425 100 0 readCmt
xform 0 -2144 -2352
p -2208 -2304 100 768 -1 DTYP:Ascii SIO
p -2336 -2546 100 0 0 OMSL:closed_loop
p -2336 -2514 100 0 0 PINI:$(RD_TERM_PINI)
p -2284 -3012 100 0 1 PV:$(MY_PV)
p -2336 -2386 100 0 0 VAL:$(RD_TERM)
use eaos -2272 -2665 100 0 slope
xform 0 -2144 -2576
p -2208 -2512 100 768 -1 DTYP:Ascii SIO
p -2528 -2594 100 0 0 OMSL:closed_loop
p -2528 -2562 100 0 0 PINI:$(SLOPE_PINI)
p -2508 -3158 100 0 0 PV:$(MY_PV)
use elongouts -2272 -2889 100 0 timeout
xform 0 -2144 -2800
p -2208 -2736 100 768 -1 DTYP:Ascii SIO
p -2432 -2882 100 0 0 OMSL:closed_loop
p -2432 -2626 100 0 0 PINI:$(TIMEOUT_PINI)
p -2412 -3208 100 0 0 PV:$(MY_PV)
[comments]
