[schematic2]
uniq 37
[tools]
[detail]
[cell use]
use motorSad 320 1799 100 0 motorSad#36
xform 0 544 2048
p 340 1772 100 0 1 set0:name grating
use hwin 944 1911 100 0 hwin#25
xform 0 1040 1952
p 947 1944 100 0 -1 val(in):$(top)$(name)State
use hwin 1648 1911 100 0 hwin#27
xform 0 1744 1952
p 1651 1944 100 0 -1 val(in):$(top)$(name)State
use hwin 240 1591 100 0 hwin#30
xform 0 336 1632
p 243 1624 100 0 -1 val(in):$(top)$(name)State
use hwin 944 1591 100 0 hwin#34
xform 0 1040 1632
p 947 1624 100 0 -1 val(in):$(top)$(name)State
use esirs 1280 1639 100 0 GratingWavelength
xform 0 1488 1792
p 1216 1344 100 0 0 FTVL:DOUBLE
p 1472 1504 100 0 0 SCAN:1 second
p 1392 1632 100 1024 0 name:$(sadtop)$(name)$(I)
use esirs 1984 1639 100 0 GratingWavelengthReq
xform 0 2192 1792
p 1920 1344 100 0 0 FTVL:DOUBLE
p 2176 1504 100 0 0 SCAN:1 second
p 2096 1632 100 1024 0 name:$(sadtop)$(name)$(I)
use esirs 576 1319 100 0 GratingOrder
xform 0 784 1472
p 512 1024 100 0 0 FTVL:LONG
p 768 1184 100 0 0 SCAN:1 second
p 688 1312 100 1024 0 name:$(sadtop)$(name)$(I)
use esirs 1280 1319 100 0 GratingAngle
xform 0 1488 1472
p 1216 1024 100 0 0 FTVL:DOUBLE
p 1472 1184 100 0 0 SCAN:1 second
p 1392 1312 100 1024 0 name:$(sadtop)$(name)$(I)
use bb200tr 128 840 -100 0 frame
xform 0 1408 1664
p 2144 1056 200 0 -1 filename:gratingSad.sch
[comments]
