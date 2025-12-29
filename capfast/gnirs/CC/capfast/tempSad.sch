[schematic2]
uniq 14
[tools]
[detail]
w 174 4683 100 0 n#2 hwin.hwin#1.in 192 4672 192 4672 esirs.esirs#0.INP
[cell use]
use esirs 192 4423 100 0 esirs#0
xform 0 400 4576
p 128 4224 100 0 0 EGU:kelvin
p 128 4128 100 0 0 FTVL:DOUBLE
p 384 4288 100 0 0 SCAN:1 second
use esirs 192 4039 100 0 sendReading1
xform 0 400 4192
p 128 3840 100 0 0 EGU:kelvin
p 128 3744 100 0 0 FTVL:DOUBLE
p 384 3904 100 0 0 SCAN:1 second
p 304 4032 100 1024 0 name:$(sadtop)$(I)
use esirs 192 3655 100 0 sendReading2
xform 0 400 3808
p 128 3456 100 0 0 EGU:kelvin
p 128 3360 100 0 0 FTVL:DOUBLE
p 384 3520 100 0 0 SCAN:1 second
p 304 3648 100 1024 0 name:$(sadtop)$(I)
use esirs 192 3271 100 0 sendPeakReading
xform 0 400 3424
p 128 3072 100 0 0 EGU:kelvin
p 128 2976 100 0 0 FTVL:DOUBLE
p 384 3136 100 0 0 SCAN:1 second
p 304 3264 100 1024 0 name:$(sadtop)$(I)
use esirs 192 2887 100 0 PB1
xform 0 400 3040
p 128 2688 100 0 0 EGU:kelvin
p 128 2592 100 0 0 FTVL:DOUBLE
p 384 2752 100 0 0 SCAN:1 second
p 304 2880 100 1024 0 name:$(sadtop)$(I)
use esirs 192 2503 100 0 setPoint1
xform 0 400 2656
p 128 2304 100 0 0 EGU:kelvin
p 128 2208 100 0 0 FTVL:DOUBLE
p 384 2368 100 0 0 SCAN:1 second
p 304 2496 100 1024 0 name:$(sadtop)$(I)
use esirs 896 4039 100 0 setPoint2
xform 0 1104 4192
p 832 3840 100 0 0 EGU:kelvin
p 832 3744 100 0 0 FTVL:DOUBLE
p 1088 3904 100 0 0 SCAN:1 second
p 1008 4032 100 1024 0 name:$(sadtop)$(I)
use esirs 896 3655 100 0 input
xform 0 1104 3808
p 832 3456 100 0 0 EGU:kelvin
p 832 3360 100 0 0 FTVL:DOUBLE
p 1088 3520 100 0 0 SCAN:1 second
p 1008 3648 100 1024 0 name:$(sadtop)$(I)
use esirs 896 3271 100 0 rate1
xform 0 1104 3424
p 832 3072 100 0 0 EGU:kelvin
p 832 2976 100 0 0 FTVL:DOUBLE
p 1088 3136 100 0 0 SCAN:1 second
p 1008 3264 100 1024 0 name:$(sadtop)$(I)
use esirs 896 2887 100 0 reset1
xform 0 1104 3040
p 832 2688 100 0 0 EGU:kelvin
p 832 2592 100 0 0 FTVL:DOUBLE
p 1088 2752 100 0 0 SCAN:1 second
p 1008 2880 100 1024 0 name:$(sadtop)$(I)
use esirs 896 2503 100 0 sendMainReading
xform 0 1104 2656
p 832 2304 100 0 0 EGU:kelvin
p 832 2208 100 0 0 FTVL:DOUBLE
p 1088 2368 100 0 0 SCAN:1 second
p 1008 2496 100 1024 0 name:$(sadtop)$(I)
use esirs 896 4423 100 0 sendValley
xform 0 1104 4576
p 832 4224 100 0 0 EGU:kelvin
p 832 4128 100 0 0 FTVL:DOUBLE
p 1088 4288 100 0 0 SCAN:1 second
p 1008 4416 100 1024 0 name:$(sadtop)$(I)
use bc200tr -208 2296 -100 0 frame
xform 0 1472 3600
use hwin 0 4631 100 0 hwin#1
xform 0 96 4672
p 3 4664 100 0 -1 val(in):#C0 B0
[comments]
