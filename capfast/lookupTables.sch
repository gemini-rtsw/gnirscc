[schematic2]
uniq 155
[tools]
[detail]
w 3442 2171 100 0 n#154 elongouts.testCad.VAL 3600 2016 3664 2016 3664 2160 3280 2160 3280 2304 3376 2304 3376 2288 3408 2288 hwout.hwout#87.outp
w 3220 2123 100 0 n#153 cadFan.cadFan#64.OUTA 3104 2208 3216 2208 3216 2048 3344 2048 elongouts.testCad.DOL
w 2826 2091 100 0 n#147 ecad2.getnames.SPLK 2800 2080 2912 2080 cadFan.cadFan#64.SPLK
w 2826 2123 100 0 n#146 ecad2.getnames.STLK 2800 2112 2912 2112 cadFan.cadFan#64.STLK
w 2826 2155 100 0 n#145 ecad2.getnames.PLNK 2800 2144 2912 2144 cadFan.cadFan#64.PLNK
w 2826 2187 100 0 n#144 ecad2.getnames.CLNK 2800 2176 2912 2176 cadFan.cadFan#64.CLNK
w 2826 2219 100 0 n#143 ecad2.getnames.MLNK 2800 2208 2912 2208 cadFan.cadFan#64.MLNK
[cell use]
use elongouts 3344 1927 100 0 testCad
xform 0 3472 2016
use hwout 3776 1815 100 0 hwout#75
xform 0 3872 1856
p 3872 1847 100 0 -1 val(outp):$(top)xdispstart.DIR PP NMS
use hwout 3776 1847 100 0 hwout#76
xform 0 3872 1888
p 3872 1879 100 0 -1 val(outp):$(top)slitstart.DIR PP NMS
use hwout 3776 1879 100 0 hwout#77
xform 0 3872 1920
p 3872 1911 100 0 -1 val(outp):$(top)focusstart.DIR PP NMS
use hwout 3776 1911 100 0 hwout#79
xform 0 3872 1952
p 3872 1943 100 0 -1 val(outp):$(top)gratingstart.DIR PP NMS
use hwout 3776 1943 100 0 hwout#80
xform 0 3872 1984
p 3872 1975 100 0 -1 val(outp):$(top)acqstart.DIR PP NMS
use hwout 3776 1975 100 0 hwout#82
xform 0 3872 2016
p 3872 2007 100 0 -1 val(outp):$(top)deckerstart.DIR PP NMS
use hwout 3776 2007 100 0 hwout#84
xform 0 3872 2048
p 3872 2039 100 0 -1 val(outp):$(top)camerastart.DIR PP NMS
use hwout 3776 2039 100 0 hwout#86
xform 0 3872 2080
p 3872 2071 100 0 -1 val(outp):$(top)coverstart.DIR PP NMS
use hwout 3408 2247 100 0 hwout#87
xform 0 3504 2288
p 3504 2279 100 0 -1 val(outp):$(top)fw2start.DIR 
use hwout 3408 2215 100 0 hwout#89
xform 0 3504 2256
p 3504 2240 100 0 -1 val(outp):$(top)fw1start.DIR
use ecad2 2480 2023 100 0 getnames
xform 0 2640 2336
p 2576 2048 100 0 0 SNAM:testGet
use cadFan 2912 1767 100 0 cadFan#64
xform 0 3008 2016
p 2932 1740 100 0 1 set0:cad get
use bc200tr 2240 1560 -100 0 frame
xform 0 3920 2864
p 5088 1776 150 0 -1 filename:lookupTables.sch
use estringins 2960 3239 100 0 fw1SeqLut
xform 0 3088 3312
p 2960 3200 100 0 1 VAL:fwIS.lut
use estringins 2960 3015 100 0 gratingSeqLut
xform 0 3088 3088
p 2960 2976 100 0 1 VAL:gratingIS.lut
use estringins 3392 2791 100 0 slitLut
xform 0 3520 2864
p 3392 2752 100 0 1 VAL:slit.lut
use estringins 2960 3463 100 0 dirLut
xform 0 3088 3536
p 2960 3424 100 0 1 VAL:dirLutUndefined
use estringins 2960 2567 100 0 lambdaFocusLUT
xform 0 3088 2640
p 2960 2528 100 0 1 VAL:lambdaFocus.lut
use estringins 3392 3239 100 0 fw1Lut
xform 0 3520 3312
p 3392 3200 100 0 1 VAL:fw1.lut
use estringins 3392 3015 100 0 gratingLut
xform 0 3520 3088
p 3392 2976 100 0 1 VAL:grating.lut
use estringins 2960 2791 100 0 slitSeqLut
xform 0 3088 2864
p 2960 2752 100 0 1 VAL:slitIS.lut
use estringins 3712 3607 100 0 fw2SeqLut
xform 0 3840 3680
p 3712 3568 100 0 1 VAL:fwIS.lut
use estringins 4144 3607 100 0 fw2Lut
xform 0 4272 3680
p 4144 3568 100 0 1 VAL:fw2.lut
use estringins 4144 3415 100 0 acqLut
xform 0 4272 3488
p 4144 3376 100 0 1 VAL:acq.lut
use estringins 3712 3415 100 0 acqSeqLut
xform 0 3840 3488
p 3712 3376 100 0 1 VAL:acqIS.lut
use estringins 3712 3191 100 0 focusSeqLut
xform 0 3840 3264
p 3712 3152 100 0 1 VAL:focusIS.lut
use estringins 4144 3191 100 0 focusLut
xform 0 4272 3264
p 4144 3152 100 0 1 VAL:focus.lut
use estringins 4176 2967 100 0 xdispLut
xform 0 4304 3040
p 4176 2928 100 0 1 VAL:xdisp.lut
use estringins 3744 2967 100 0 xdispSeqLut
xform 0 3872 3040
p 3744 2928 100 0 1 VAL:xdispIS.lut
use estringins 3744 2743 100 0 deckerSeqLut
xform 0 3872 2816
p 3744 2704 100 0 1 VAL:deckerIS.lut
use estringins 4176 2743 100 0 deckerLut
xform 0 4304 2816
p 4176 2704 100 0 1 VAL:decker.lut
use estringins 4176 2519 100 0 coverLut
xform 0 4304 2592
p 4176 2480 100 0 1 VAL:cover.lut
use estringins 3744 2519 100 0 coverSeqLut
xform 0 3872 2592
p 3744 2480 100 0 1 VAL:coverIS.lut
use estringins 3744 2295 100 0 cameraSeqLut
xform 0 3872 2368
p 3744 2256 100 0 1 VAL:cameraIS.lut
use estringins 4176 2295 100 0 cameraLut
xform 0 4304 2368
p 4176 2256 100 0 1 VAL:camera.lut
[comments]
