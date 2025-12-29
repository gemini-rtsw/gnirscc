[schematic2]
uniq 236
[tools]
[detail]
w -728 -2533 100 0 n#233 hwin.hwin#234.in -704 -2544 -704 -2544 eais.mytest.INP
w -1340 -2725 100 2 n#232 hwout.hwout#231.outp -1344 -2720 -1344 -2720 eaos.slope.OUT
w 36 -2725 100 2 n#230 hwout.hwout#229.outp 32 -2720 32 -2720 estringouts.readCmt.OUT
w -956 -3717 100 2 n#228 hwout.hwout#227.outp -960 -3712 -960 -3712 elongouts.debug.OUT
w -332 -3381 100 0 n#224 estringins.setp_stringin.VAL -384 -3408 -336 -3408 -336 -3344 -288 -3344 eais.setp_ai.INP
w -366 -3365 100 0 n#223 estringins.setp_stringin.FLNK -384 -3376 -288 -3376 eais.setp_ai.SLNK
w -670 -3349 100 0 n#188 hwin.hwin#187.in -640 -3360 -640 -3360 estringins.setp_stringin.INP
w -1406 -2997 100 0 n#119 eaos.setsetp.OUT -1376 -3008 -1376 -3008 hwout.hwout#116.outp
w -1662 -2933 100 0 n#118 eaos.setsetp.DOL -1632 -2944 -1632 -2944 hwin.setpt.in
w -1656 -3269 100 0 n#113 hwin.hwin#111.in -1632 -3280 -1632 -3280 eais.setp.INP
w -1656 -3429 100 0 n#108 eais.heat.INP -1632 -3440 -1632 -3440 hwin.hwin#110.in
w -664 -2853 100 0 n#107 hwin.hwin#105.in -640 -2864 -640 -2864 eais.cdat.INP
w -664 -3013 100 0 n#102 eais.rang.INP -640 -3024 -640 -3024 hwin.hwin#104.in
w -1400 -3157 100 0 n#93 hwout.hwout#92.outp -1376 -3168 -1376 -3168 estringouts.rst.OUT
w -664 -3173 100 0 n#61 eais.sdat.INP -640 -3184 -640 -3184 hwin.hwin#63.in
[cell use]
use eais -288 -3449 100 0 setp_ai
xform 0 -160 -3376
p -544 -3762 100 0 0 DISV:2
p -544 -3538 100 0 0 PREC:2
p -640 -3504 100 0 1 SCAN:Passive
p -320 -3344 75 1280 -1 pproc(INP):PP
use eais -640 -3289 100 0 sdat
xform 0 -512 -3216
p -896 -3666 100 0 0 ADEL:-1.0
p -896 -3570 100 0 0 ASLO:0.01
p -576 -3168 100 768 -1 DTYP:Ascii SIO
p -896 -3346 100 0 0 LINR:NO CONVERSION
p -896 -3378 100 0 0 PREC:1
p -576 -3248 100 768 -1 SCAN:Passive
use eais -640 -3129 100 0 rang
xform 0 -512 -3056
p -896 -3506 100 0 0 ADEL:-1.0
p -896 -3410 100 0 0 ASLO:0.01
p -576 -3008 100 768 -1 DTYP:Ascii SIO
p -896 -3186 100 0 0 LINR:NO CONVERSION
p -896 -3218 100 0 0 PREC:0
p -576 -3088 100 768 -1 SCAN:Passive
use eais -640 -2969 100 0 cdat
xform 0 -512 -2896
p -896 -3346 100 0 0 ADEL:-1.0
p -896 -3250 100 0 0 ASLO:0.01
p -576 -2848 100 768 -1 DTYP:Ascii SIO
p -672 -3026 100 0 0 HHSV:MAJOR
p -672 -2962 100 0 0 HIGH:$(HIGH)
p -672 -2898 100 0 0 HIHI:$(HIHI)
p -672 -3058 100 0 0 HSV:MINOR
p -896 -3314 100 0 0 HYST:$(HYST)
p -896 -3026 100 0 0 LINR:NO CONVERSION
p -896 -3058 100 0 0 PREC:1
p -576 -2928 100 768 -1 SCAN:Passive
use eais -1632 -3545 100 0 heat
xform 0 -1504 -3472
p -1888 -3922 100 0 0 ADEL:-1.0
p -1888 -3826 100 0 0 ASLO:0.01
p -1568 -3424 100 768 -1 DTYP:Ascii SIO
p -1888 -3602 100 0 0 LINR:NO CONVERSION
p -1888 -3634 100 0 0 PREC:0
p -1568 -3504 100 768 -1 SCAN:Passive
use eais -1632 -3385 100 0 setp
xform 0 -1504 -3312
p -1888 -3762 100 0 0 ADEL:-1.0
p -1888 -3666 100 0 0 ASLO:0.01
p -1888 -3698 100 0 0 DISV:1
p -1568 -3264 100 768 -1 DTYP:Ascii SIO
p -1888 -3442 100 0 0 LINR:NO CONVERSION
p -1888 -3474 100 0 0 PREC:2
p -1568 -3344 100 768 -1 SCAN:Passive
use eais -704 -2649 100 0 mytest
xform 0 -576 -2576
p -960 -3026 100 0 0 ADEL:-1.0
p -960 -2930 100 0 0 ASLO:0.01
p -640 -2528 100 768 -1 DTYP:Ascii SIO
p -960 -2706 100 0 0 LINR:NO CONVERSION
p -960 -2738 100 0 0 PREC:1
p -640 -2608 100 768 -1 SCAN:Passive
use hwin -832 -3401 100 0 hwin#187
xform 0 -736 -3360
p -928 -3328 100 0 -1 val(in):@$(DEVICE) <SETP?><%*s>
use hwin -1824 -2985 100 0 setpt
xform 0 -1728 -2944
p -1821 -2952 100 0 -1 val(in):$(SETP)
use hwin -832 -3225 100 0 hwin#63
xform 0 -736 -3184
p -928 -3152 100 0 -1 val(in):@$(DEVICE) <SDAT?><%f>
use hwin -832 -3065 100 0 hwin#104
xform 0 -736 -3024
p -928 -2992 100 0 -1 val(in):@$(DEVICE) <RANG?><%f>
use hwin -832 -2905 100 0 hwin#105
xform 0 -736 -2864
p -928 -2832 100 0 -1 val(in):@$(DEVICE) <CDAT?><%f >
use hwin -1824 -3481 100 0 hwin#110
xform 0 -1728 -3440
p -1920 -3408 100 0 -1 val(in):@$(DEVICE) <HEAT?><%f>
use hwin -1824 -3321 100 0 hwin#111
xform 0 -1728 -3280
p -1920 -3248 100 0 -1 val(in):@$(DEVICE) <SETP?><%f>
use hwin -896 -2585 100 0 hwin#234
xform 0 -800 -2544
p -992 -2512 100 0 -1 val(in):@$(DEVICE) <SDAT?><%f>
use hwout 32 -2761 100 0 hwout#229
xform 0 128 -2720
p 64 -2688 100 0 -1 val(outp):@$(DEVICE) <readCmt %s>
use hwout -960 -3753 100 0 hwout#227
xform 0 -864 -3712
p -864 -3721 100 0 -1 val(outp):@$(DEVICE) <debug>
use hwout -1376 -3049 100 0 hwout#116
xform 0 -1280 -3008
p -1280 -2976 100 0 -1 val(outp):@$(DEVICE) <SETP%f>
use hwout -1376 -3209 100 0 hwout#92
xform 0 -1280 -3168
p -1280 -3136 100 0 -1 val(outp):@$(DEVICE) <*RST>
use hwout -1344 -2761 100 0 hwout#231
xform 0 -1248 -2720
p -1248 -2729 100 0 -1 val(outp):@$(DEVICE) <slope>
use eaos -1632 -3065 100 0 setsetp
xform 0 -1504 -2976
p -1868 -3558 100 0 0 ASLO:0.01
p -1568 -2896 100 0 -1 DTYP:Ascii SIO
p -1888 -2994 100 0 0 OMSL:closed_loop
p -1888 -2962 100 0 0 PINI:YES
p -1888 -3122 100 0 0 PREC:1
p -1552 -3024 100 0 -1 SCAN:Passive
p -1664 -2944 75 1280 -1 pproc(DOL):PP
p -1376 -3008 75 768 -1 pproc(OUT):PP
use eaos -1600 -2777 100 0 slope
xform 0 -1472 -2688
p -1520 -2608 100 0 -1 DTYP:Ascii SIO
p -1520 -2736 100 0 -1 SCAN:Passive
use estringouts -224 -2777 100 0 readCmt
xform 0 -96 -2704
p -144 -2624 100 0 -1 DTYP:Ascii SIO
use estringouts -1632 -3225 100 0 rst
xform 0 -1504 -3152
p -1568 -3088 100 0 -1 DTYP:Ascii SIO
p -1552 -3200 100 768 -1 SCAN:Passive
p -1376 -3168 75 768 -1 pproc(OUT):PP
use elongouts -1216 -3769 100 0 debug
xform 0 -1088 -3680
p -928 -3616 100 0 -1 DTYP:Ascii SIO
use bb200tr -2128 -3944 -100 0 frame
xform 0 -848 -3120
use estringins -640 -3465 100 0 setp_stringin
xform 0 -512 -3392
p -548 -3178 100 0 0 DTYP:Ascii SIO
p -208 -3408 100 0 -1 SCAN:Passive
[comments]
