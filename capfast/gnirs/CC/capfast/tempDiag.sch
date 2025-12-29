[schematic2]
uniq 251
[tools]
[detail]
w -166 -509 100 0 n#250 eais.temp1v2.FLNK 352 -368 448 -368 448 -512 -720 -512 -720 -608 -464 -608 eais.tempGroundB1.SLNK
w -182 -293 100 0 n#249 eais.temp100K2.FLNK 352 -176 432 -176 432 -304 -736 -304 -736 -400 -480 -400 eais.temp1v1.SLNK
w -182 -101 100 0 n#248 eais.tempDiode2.FLNK 352 16 432 16 432 -112 -736 -112 -736 -208 -480 -208 eais.temp100K1.SLNK
w -182 91 100 0 n#247 eais.tempGroundA2.FLNK 352 208 432 208 432 80 -736 80 -736 -16 -480 -16 eais.tempDiode1.SLNK
w -78 -581 100 0 n#246 eais.tempGroundB1.FLNK -208 -592 112 -592 eais.tempGroundB2.SLNK
w -94 -373 100 0 n#245 eais.temp1v1.FLNK -224 -384 96 -384 eais.temp1v2.SLNK
w -94 -181 100 0 n#244 eais.temp100K1.FLNK -224 -192 96 -192 eais.temp100K2.SLNK
w -94 11 100 0 n#243 eais.tempDiode1.FLNK -224 0 96 0 eais.tempDiode2.SLNK
w -94 203 100 0 n#242 eais.tempGroundA1.FLNK -224 192 96 192 eais.tempGroundA2.SLNK
w 82 -549 100 0 n#241 hwin.hwin#61.in 112 -560 112 -560 eais.tempGroundB2.INP
w 66 -341 100 0 n#240 hwin.hwin#204.in 96 -352 96 -352 eais.temp1v2.INP
w 66 -149 100 0 n#239 hwin.hwin#206.in 96 -160 96 -160 eais.temp100K2.INP
w 66 43 100 0 n#238 hwin.hwin#210.in 96 32 96 32 eais.tempDiode2.INP
w 66 235 100 0 n#237 hwin.hwin#212.in 96 224 96 224 eais.tempGroundA2.INP
w -510 219 100 0 n#236 hwin.hwin#65.in -480 208 -480 208 eais.tempGroundA1.INP
w -510 27 100 0 n#235 hwin.hwin#1.in -480 16 -480 16 eais.tempDiode1.INP
w -510 -165 100 0 n#234 hwin.hwin#53.in -480 -176 -480 -176 eais.temp100K1.INP
w -510 -357 100 0 n#233 hwin.hwin#55.in -480 -368 -480 -368 eais.temp1v1.INP
w -494 -565 100 0 n#232 hwin.hwin#59.in -464 -576 -464 -576 eais.tempGroundB1.INP
[cell use]
use bb200tr -960 -1000 -100 0 frame
xform 0 320 -176
p 1040 -784 200 0 -1 File:tempDiag.sch
use hwin -672 -25 100 0 hwin#1
xform 0 -576 16
p -669 8 100 0 -1 val(in):@temperatureCC[28]
use hwin -672 -217 100 0 hwin#53
xform 0 -576 -176
p -669 -184 100 0 -1 val(in):@temperatureCC[29]
use hwin -672 -409 100 0 hwin#55
xform 0 -576 -368
p -669 -376 100 0 -1 val(in):@temperatureCC[30]
use hwin -656 -617 100 0 hwin#59
xform 0 -560 -576
p -653 -584 100 0 -1 val(in):@temperatureCC[31]
use hwin -80 -601 100 0 hwin#61
xform 0 16 -560
p -77 -568 100 0 -1 val(in):@temperatureCC[63]
use hwin -672 167 100 0 hwin#65
xform 0 -576 208
p -669 200 100 0 -1 val(in):@temperatureCC[15]
use hwin -96 -393 100 0 hwin#204
xform 0 0 -352
p -93 -360 100 0 -1 val(in):@temperatureCC[62]
use hwin -96 -201 100 0 hwin#206
xform 0 0 -160
p -93 -168 100 0 -1 val(in):@temperatureCC[61]
use hwin -96 -9 100 0 hwin#210
xform 0 0 32
p -93 24 100 0 -1 val(in):@temperatureCC[60]
use hwin -96 183 100 0 hwin#212
xform 0 0 224
p -93 216 100 0 -1 val(in):@temperatureCC[47]
use eais -480 -89 100 0 tempDiode1
xform 0 -352 -16
p -691 57 100 0 0 DESC:nirs temperature
p -594 104 100 0 0 DTYP:vxWorks Variable (INST_IO)
p -736 -274 100 0 0 EGU:degrees C
p -736 -18 100 0 0 SCAN:Passive
use eais -480 -281 100 0 temp100K1
xform 0 -352 -208
p -691 -135 100 0 0 DESC:nirs temperature
p -594 -88 100 0 0 DTYP:vxWorks Variable (INST_IO)
p -736 -466 100 0 0 EGU:degrees C
p -736 -210 100 0 0 SCAN:Passive
use eais -480 -473 100 0 temp1v1
xform 0 -352 -400
p -691 -327 100 0 0 DESC:nirs temperature
p -594 -280 100 0 0 DTYP:vxWorks Variable (INST_IO)
p -736 -658 100 0 0 EGU:degrees C
p -736 -402 100 0 0 SCAN:Passive
use eais -464 -681 100 0 tempGroundB1
xform 0 -336 -608
p -675 -535 100 0 0 DESC:nirs temperature
p -578 -488 100 0 0 DTYP:vxWorks Variable (INST_IO)
p -720 -866 100 0 0 EGU:degrees C
p -720 -610 100 0 0 SCAN:Passive
use eais 112 -665 100 0 tempGroundB2
xform 0 240 -592
p -99 -519 100 0 0 DESC:nirs temperature
p -2 -472 100 0 0 DTYP:vxWorks Variable (INST_IO)
p -144 -850 100 0 0 EGU:degrees C
p -144 -594 100 0 0 SCAN:Passive
use eais -480 103 100 0 tempGroundA1
xform 0 -352 176
p -691 249 100 0 0 DESC:nirs temperature
p -544 256 100 0 1 DTYP:vxWorks Variable (INST_IO)
p -736 -82 100 0 0 EGU:degrees C
p -736 174 100 0 0 SCAN:10 second
use eais 96 -457 100 0 temp1v2
xform 0 224 -384
p -115 -311 100 0 0 DESC:nirs temperature
p -18 -264 100 0 0 DTYP:vxWorks Variable (INST_IO)
p -160 -642 100 0 0 EGU:degrees C
p -160 -386 100 0 0 SCAN:Passive
use eais 96 -265 100 0 temp100K2
xform 0 224 -192
p -115 -119 100 0 0 DESC:nirs temperature
p -18 -72 100 0 0 DTYP:vxWorks Variable (INST_IO)
p -160 -450 100 0 0 EGU:degrees C
p -160 -194 100 0 0 SCAN:Passive
use eais 96 -73 100 0 tempDiode2
xform 0 224 0
p -115 73 100 0 0 DESC:nirs temperature
p -18 120 100 0 0 DTYP:vxWorks Variable (INST_IO)
p -160 -258 100 0 0 EGU:degrees C
p -160 -2 100 0 0 SCAN:Passive
use eais 96 119 100 0 tempGroundA2
xform 0 224 192
p -115 265 100 0 0 DESC:nirs temperature
p -18 312 100 0 0 DTYP:vxWorks Variable (INST_IO)
p -160 -66 100 0 0 EGU:degrees C
p -160 190 100 0 0 SCAN:Passive
[comments]
