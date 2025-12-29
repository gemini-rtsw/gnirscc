[schematic2]
uniq 54
[tools]
[detail]
w 2952 2027 100 0 n#53 hwout.hwout#52.outp 2976 2016 2976 2016 cadFan.cadFan#27.OUTJ
w 2952 2059 100 0 n#51 hwout.hwout#50.outp 2976 2048 2976 2048 cadFan.cadFan#27.OUTI
w 2952 2091 100 0 n#49 hwout.hwout#48.outp 2976 2080 2976 2080 cadFan.cadFan#27.OUTH
w 2952 2123 100 0 n#47 hwout.hwout#46.outp 2976 2112 2976 2112 cadFan.cadFan#27.OUTG
w 2952 2155 100 0 n#45 hwout.hwout#44.outp 2976 2144 2976 2144 cadFan.cadFan#27.OUTF
w 2952 2187 100 0 n#43 hwout.hwout#42.outp 2976 2176 2976 2176 cadFan.cadFan#27.OUTE
w 2952 2219 100 0 n#41 hwout.hwout#40.outp 2976 2208 2976 2208 cadFan.cadFan#27.OUTD
w 2952 2251 100 0 n#39 hwout.hwout#38.outp 2976 2240 2976 2240 cadFan.cadFan#27.OUTC
w 2952 2283 100 0 n#37 hwout.hwout#36.outp 2976 2272 2976 2272 cadFan.cadFan#27.OUTB
w 2952 2315 100 0 n#35 hwout.hwout#34.outp 2976 2304 2976 2304 cadFan.cadFan#27.OUTA
w 2680 2187 100 0 n#33 ecad2.getNames.SPLK 2624 2176 2784 2176 cadFan.cadFan#27.SPLK
w 2680 2219 100 0 n#32 ecad2.getNames.STLK 2624 2208 2784 2208 cadFan.cadFan#27.STLK
w 2680 2251 100 0 n#31 ecad2.getNames.PLNK 2624 2240 2784 2240 cadFan.cadFan#27.PLNK
w 2680 2283 100 0 n#30 ecad2.getNames.CLNK 2624 2272 2784 2272 cadFan.cadFan#27.CLNK
w 2680 2315 100 0 n#29 ecad2.getNames.MLNK 2624 2304 2784 2304 cadFan.cadFan#27.MLNK
[cell use]
use hwout 2976 1975 100 0 hwout#52
xform 0 3072 2016
p 3072 2007 100 0 -1 val(outp):$(top)acqstart.DIR PP NMS
use hwout 2976 2007 100 0 hwout#50
xform 0 3072 2048
p 3072 2039 100 0 -1 val(outp):$(top)focusstart.DIR PP NMS
use hwout 2976 2039 100 0 hwout#48
xform 0 3072 2080
p 3072 2071 100 0 -1 val(outp):$(top)fw1start.DIR PP NMS
use hwout 2976 2071 100 0 hwout#46
xform 0 3072 2112
p 3072 2103 100 0 -1 val(outp):$(top)xdispstart.DIR PP NMS
use hwout 2976 2103 100 0 hwout#44
xform 0 3072 2144
p 3072 2135 100 0 -1 val(outp):$(top)slitstart.DIR PP NMS
use hwout 2976 2135 100 0 hwout#42
xform 0 3072 2176
p 3072 2167 100 0 -1 val(outp):$(top)gratingstart.DIR PP NMS
use hwout 2976 2167 100 0 hwout#40
xform 0 3072 2208
p 3072 2199 100 0 -1 val(outp):$(top)deckerstart.DIR PP NMS
use hwout 2976 2199 100 0 hwout#38
xform 0 3072 2240
p 3072 2231 100 0 -1 val(outp):$(top)camerastart.DIR PP NMS
use hwout 2976 2231 100 0 hwout#36
xform 0 3072 2272
p 3072 2263 100 0 -1 val(outp):$(top)coverstart.DIR PP NMS
use hwout 2976 2263 100 0 hwout#34
xform 0 3072 2304
p 3072 2295 100 0 -1 val(outp):$(top)fw2start.DIR PP NMS
use ecad2 2304 2119 100 0 getNames
xform 0 2464 2432
use cadFan 2784 1863 100 0 cadFan#27
xform 0 2880 2112
p 2804 1836 100 0 1 set0:cad getNames
use estringins 4176 2295 100 0 cameraLut
xform 0 4304 2368
p 4176 2256 100 0 1 VAL:camera.lut
use estringins 3744 2295 100 0 cameraSeqLut
xform 0 3872 2368
p 3744 2256 100 0 1 VAL:cameraIS.lut
use estringins 3744 2519 100 0 coverSeqLut
xform 0 3872 2592
p 3744 2480 100 0 1 VAL:coverIS.lut
use estringins 4176 2519 100 0 coverLut
xform 0 4304 2592
p 4176 2480 100 0 1 VAL:cover.lut
use estringins 4176 2743 100 0 deckerLut
xform 0 4304 2816
p 4176 2704 100 0 1 VAL:decker.lut
use estringins 3744 2743 100 0 deckerSeqLut
xform 0 3872 2816
p 3744 2704 100 0 1 VAL:deckerIS.lut
use estringins 3744 2967 100 0 xdispSeqLut
xform 0 3872 3040
p 3744 2928 100 0 1 VAL:xdispIS.lut
use estringins 4176 2967 100 0 xdispLut
xform 0 4304 3040
p 4176 2928 100 0 1 VAL:xdisp.lut
use estringins 4144 3191 100 0 focusLut
xform 0 4272 3264
p 4144 3152 100 0 1 VAL:focus.lut
use estringins 3712 3191 100 0 focusSeqLut
xform 0 3840 3264
p 3712 3152 100 0 1 VAL:focusIS.lut
use estringins 3712 3415 100 0 acqSeqLut
xform 0 3840 3488
p 3712 3376 100 0 1 VAL:acqIS.lut
use estringins 4144 3415 100 0 acqLut
xform 0 4272 3488
p 4144 3376 100 0 1 VAL:acq.lut
use estringins 4144 3607 100 0 fw2Lut
xform 0 4272 3680
p 4144 3568 100 0 1 VAL:fw2.lut
use estringins 3712 3607 100 0 fw2SeqLut
xform 0 3840 3680
p 3712 3568 100 0 1 VAL:fw2IS.lut
use estringins 2960 2791 100 0 slitSeqLut
xform 0 3088 2864
p 2960 2752 100 0 1 VAL:slitIS.lut
use estringins 3392 3015 100 0 gratingLut
xform 0 3520 3088
p 3392 2976 100 0 1 VAL:grating.lut
use estringins 3392 3239 100 0 fw1Lut
xform 0 3520 3312
p 3392 3200 100 0 1 VAL:fw1.lut
use estringins 2960 2567 100 0 lambdaFocusLUT
xform 0 3088 2640
p 2960 2528 100 0 1 VAL:lambdaFocus.lut
use estringins 2960 3463 100 0 dirLut
xform 0 3088 3536
p 2960 3424 100 0 1 VAL:/home/gemini/gnirs/control/data
use estringins 3392 2791 100 0 slitLut
xform 0 3520 2864
p 3392 2752 100 0 1 VAL:slit.lut
use estringins 2960 3015 100 0 gratingSeqLut
xform 0 3088 3088
p 2960 2976 100 0 1 VAL:gratingIS.lut
use estringins 2960 3239 100 0 fw1SeqLut
xform 0 3088 3312
p 2960 3200 100 0 1 VAL:fw1IS.lut
[comments]
