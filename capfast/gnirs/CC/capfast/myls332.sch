[schematic2]
uniq 252
[tools]
[detail]
w -398 -3029 100 0 n#251 eais.cdat.FLNK -224 -2640 64 -2640 64 -3040 -800 -3040 -800 -3168 -480 -3168 eais.sdat.SLNK
w -1230 -2581 100 0 n#249 elongouts.elongouts#245.OUT -1248 -2592 -1152 -2592 hwout.hwout#248.outp
w -1566 -2517 100 0 n#247 hwin.hwin#246.in -1568 -2528 -1504 -2528 elongouts.elongouts#245.DOL
w -1118 -3429 100 0 n#244 eais.setp.FLNK -1248 -3440 -928 -3440 outhier.FLNK.p
w -142 -2917 100 0 n#239 eais.rang.VAL -224 -2928 0 -2928 outhier.RNG.p
w -142 -3173 100 0 n#236 eais.sdat.VAL -224 -3184 0 -3184 outhier.STMP.p
w -142 -2661 100 0 n#235 eais.cdat.VAL -224 -2672 0 -2672 outhier.CTMP.p
w -1166 -3685 100 0 n#234 eais.heat.VAL -1248 -3696 -1024 -3696 outhier.HEATV.p
w -1166 -3461 100 0 n#233 eais.setp.VAL -1248 -3472 -1024 -3472 outhier.SETPT.p
w -702 -2901 100 0 n#231 inhier.RANG.P -864 -2912 -480 -2912 eais.rang.SLNK
w -1726 -3669 100 0 n#229 inhier.HEAT.P -1888 -3680 -1504 -3680 eais.heat.SLNK
w -1726 -3445 100 0 n#228 inhier.RDSP.P -1888 -3456 -1504 -3456 eais.setp.SLNK
w -1726 -3221 100 0 n#227 inhier.RST.P -1888 -3232 -1504 -3232 estringouts.rst.SLNK
w -1726 -2997 100 0 n#226 inhier.SETP.P -1888 -3008 -1504 -3008 eaos.setsetp.SLNK
w -172 -3445 100 0 n#225 estringins.setp_stringin.SLNK -480 -3456 -576 -3456 -576 -3552 -224 -3552 -224 -3472 -176 -3472 -176 -3408 -128 -3408 eais.setp_ai.INP
w -206 -3453 100 0 n#223 estringins.setp_stringin.FLNK -224 -3456 -128 -3456 -128 -3440 eais.setp_ai.SLNK
w -510 -3413 100 0 n#188 hwin.hwin#187.in -480 -3424 -480 -3424 estringins.setp_stringin.INP
w -1278 -3029 100 0 n#119 eaos.setsetp.OUT -1248 -3040 -1248 -3040 hwout.hwout#116.outp
w -1534 -2965 100 0 n#118 eaos.setsetp.DOL -1504 -2976 -1504 -2976 hwin.setpt.in
w -1528 -3413 100 0 n#113 hwin.hwin#111.in -1504 -3424 -1504 -3424 eais.setp.INP
w -1528 -3637 100 0 n#108 eais.heat.INP -1504 -3648 -1504 -3648 hwin.hwin#110.in
w -504 -2613 100 0 n#107 hwin.hwin#105.in -480 -2624 -480 -2624 eais.cdat.INP
w -504 -2869 100 0 n#102 eais.rang.INP -480 -2880 -480 -2880 hwin.hwin#104.in
w -1272 -3237 100 0 n#93 hwout.hwout#92.outp -1248 -3248 -1248 -3248 estringouts.rst.OUT
w -1522 -2741 100 0 n#84 eaos.slope.DOL -1504 -2752 -1504 -2752 hwin.hwin#87.in
w -1266 -2805 100 0 n#83 eaos.slope.OUT -1248 -2816 -1248 -2816 hwout.hwout#86.outp
w -504 -3125 100 0 n#61 eais.sdat.INP -480 -3136 -480 -3136 hwin.hwin#63.in
s -864 -3776 400 0 ls330.sch
[cell use]
use hwout -1248 -3081 100 0 hwout#116
xform 0 -1152 -3040
p -1152 -3008 100 0 -1 val(outp):@$(PORT) <SETP%f>
use hwout -1248 -2857 100 0 hwout#86
xform 0 -1152 -2816
p -1152 -2784 100 0 -1 val(outp):@$(PORT) <slope>
use hwout -1248 -3289 100 0 hwout#92
xform 0 -1152 -3248
p -1152 -3216 100 0 -1 val(outp):@$(PORT) <*RST>
use hwout -1152 -2633 100 0 hwout#248
xform 0 -1056 -2592
p -1056 -2560 100 0 -1 val(outp):@$(PORT) <timeout>
use hwin -672 -3465 100 0 hwin#187
xform 0 -576 -3424
p -768 -3392 100 0 -1 val(in):@$(PORT) <SETP?><%*s>
use hwin -1696 -3017 100 0 setpt
xform 0 -1600 -2976
p -1693 -2984 100 0 -1 val(in):$(SETP)
use hwin -1696 -2793 100 0 hwin#87
xform 0 -1600 -2752
p -1693 -2760 100 0 -1 val(in):100.0
use hwin -672 -3177 100 0 hwin#63
xform 0 -576 -3136
p -768 -3104 100 0 -1 val(in):@$(PORT) <SDAT?><%f>
use hwin -672 -2921 100 0 hwin#104
xform 0 -576 -2880
p -768 -2848 100 0 -1 val(in):@$(PORT) <RANG?><%f>
use hwin -672 -2665 100 0 hwin#105
xform 0 -576 -2624
p -768 -2592 100 0 -1 val(in):@$(PORT) <CDAT?><%f>
use hwin -1696 -3689 100 0 hwin#110
xform 0 -1600 -3648
p -1792 -3616 100 0 -1 val(in):@$(PORT) <HEAT?><%f>
use hwin -1696 -3465 100 0 hwin#111
xform 0 -1600 -3424
p -1792 -3392 100 0 -1 val(in):@$(PORT) <SETP?><%f>
use hwin -1760 -2569 100 0 hwin#246
xform 0 -1664 -2528
p -1757 -2536 100 0 -1 val(in):6
use elongouts -1504 -2649 100 0 elongouts#245
xform 0 -1376 -2560
p -1548 -2168 100 0 0 DTYP:Ascii SIO
p -1664 -2642 100 0 0 OMSL:closed_loop
use outhier -1056 -3513 100 0 SETPT
xform 0 -1040 -3472
use outhier -1056 -3737 100 0 HEATV
xform 0 -1040 -3696
use outhier -32 -2713 100 0 CTMP
xform 0 -16 -2672
use outhier -32 -3225 100 0 STMP
xform 0 -16 -3184
use outhier -960 -3481 100 0 FLNK
xform 0 -944 -3440
use outhier -32 -2969 100 0 RNG
xform 0 -16 -2928
use inhier -880 -2953 100 0 RANG
xform 0 -864 -2912
use inhier -1904 -3721 100 0 HEAT
xform 0 -1888 -3680
use inhier -1904 -3497 100 0 RDSP
xform 0 -1888 -3456
use inhier -1904 -3273 100 0 RST
xform 0 -1888 -3232
use inhier -1904 -3049 100 0 SETP
xform 0 -1888 -3008
use bb200tr -2128 -3944 -100 0 frame
xform 0 -848 -3120
use eais -128 -3513 100 0 setp_ai
xform 0 0 -3440
p -384 -3826 100 0 0 DISV:2
p -384 -3602 100 0 0 PREC:2
p -364 -4056 100 0 0 PV:$(top)$(dev)$(c)
p -160 -3408 75 1280 -1 pproc(INP):PP
use eais -480 -3241 100 0 sdat
xform 0 -352 -3168
p -736 -3618 100 0 0 ADEL:-1.0
p -736 -3522 100 0 0 ASLO:0.01
p -416 -3120 100 768 -1 DTYP:Ascii SIO
p -736 -3426 100 0 0 EGU:Kelvin
p -736 -3298 100 0 0 LINR:NO CONVERSION
p -736 -3330 100 0 0 PREC:2
p -716 -3784 100 0 0 PV:$(top)$(dev)$(c)
p -416 -3200 100 768 -1 SCAN:Passive
use eais -480 -2985 100 0 rang
xform 0 -352 -2912
p -736 -3362 100 0 0 ADEL:-1.0
p -736 -3266 100 0 0 ASLO:0.01
p -416 -2864 100 768 -1 DTYP:Ascii SIO
p -736 -3042 100 0 0 LINR:NO CONVERSION
p -736 -3074 100 0 0 PREC:0
p -716 -3528 100 0 0 PV:$(top)$(dev)$(c)
p -416 -2944 100 768 -1 SCAN:Passive
use eais -480 -2729 100 0 cdat
xform 0 -352 -2656
p -736 -3106 100 0 0 ADEL:-1.0
p -736 -3010 100 0 0 ASLO:0.01
p -416 -2608 100 768 -1 DTYP:Ascii SIO
p -736 -2914 100 0 0 EGU:Kelvin
p -512 -2786 100 0 0 HHSV:MAJOR
p -512 -2722 100 0 0 HIGH:300.0
p -512 -2658 100 0 0 HIHI:301.0
p -512 -2818 100 0 0 HSV:MINOR
p -736 -3074 100 0 0 HYST:0.00
p -736 -2786 100 0 0 LINR:NO CONVERSION
p -736 -2818 100 0 0 PREC:2
p -716 -3272 100 0 0 PV:$(top)$(dev)$(c)
p -416 -2688 100 768 -1 SCAN:10 second
use eais -1504 -3753 100 0 heat
xform 0 -1376 -3680
p -1760 -4130 100 0 0 ADEL:-1.0
p -1760 -4034 100 0 0 ASLO:0.01
p -1440 -3632 100 768 -1 DTYP:Ascii SIO
p -1760 -3810 100 0 0 LINR:NO CONVERSION
p -1760 -3842 100 0 0 PREC:0
p -1740 -4296 100 0 1 PV:$(top)$(dev)$(c)
p -1440 -3712 100 768 -1 SCAN:Passive
use eais -1504 -3529 100 0 setp
xform 0 -1376 -3456
p -1760 -3906 100 0 0 ADEL:-1.0
p -1760 -3810 100 0 0 ASLO:0.01
p -1760 -3842 100 0 0 DISV:1
p -1440 -3408 100 768 -1 DTYP:Ascii SIO
p -1760 -3714 100 0 0 EGU:K
p -1760 -3586 100 0 0 LINR:NO CONVERSION
p -1760 -3618 100 0 0 PREC:2
p -1740 -4072 100 0 0 PV:$(top)$(dev)$(c)
p -1440 -3488 100 768 -1 SCAN:Passive
use estringins -480 -3529 100 0 setp_stringin
xform 0 -352 -3456
p -388 -3242 100 0 0 DTYP:Ascii SIO
p -524 -3988 100 0 0 PV:$(top)$(dev)$(c)
p -480 -3394 100 0 0 SCAN:10 second
p -480 -3618 100 0 0 SIOL:
use eaos -1504 -3097 100 0 setsetp
xform 0 -1376 -3008
p -1740 -3590 100 0 0 ASLO:0.01
p -1440 -2928 100 0 -1 DTYP:Ascii SIO
p -1536 -3282 100 0 0 EGU:K
p -1760 -3026 100 0 0 OMSL:closed_loop
p -1760 -2994 100 0 0 PINI:NO
p -1760 -3154 100 0 0 PREC:1
p -1720 -3610 100 0 0 PV:$(top)$(dev)$(c)
p -1424 -3056 100 0 -1 SCAN:Passive
p -1536 -2976 75 1280 -1 pproc(DOL):PP
p -1248 -3040 75 768 -1 pproc(OUT):PP
use eaos -1504 -2873 100 0 slope
xform 0 -1376 -2784
p -1440 -2720 100 768 -1 DTYP:Ascii SIO
p -1760 -2802 100 0 0 OMSL:closed_loop
p -1760 -2770 100 0 0 PINI:YES
p -1740 -3366 100 0 0 PV:$(top)$(dev)$(c)
p -1424 -2832 100 768 -1 SCAN:Passive
use estringouts -1504 -3305 100 0 rst
xform 0 -1376 -3232
p -1440 -3168 100 0 -1 DTYP:Ascii SIO
p -1516 -3892 100 0 0 PV:$(top)$(dev)$(c)
p -1424 -3280 100 768 -1 SCAN:Passive
p -1248 -3248 75 768 -1 pproc(OUT):PP
[comments]
