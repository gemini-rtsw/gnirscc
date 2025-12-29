[schematic2]
uniq 3
[tools]
[detail]
w 174 4667 100 0 n#2 hwin.hwin#1.in 192 4656 192 4656 esirs.esirs#0.INP
[cell use]
use hwin 0 4615 100 0 hwin#1
xform 0 96 4656
p 3 4648 100 0 -1 val(in):#C0 B0
use esirs 192 4407 100 0 esirs#0
xform 0 400 4560
p 128 4208 100 0 0 EGU:kelvin
p 128 4112 100 0 0 FTVL:DOUBLE
p 384 4272 100 0 0 SCAN:1 second
use be200tr -208 -376 -100 0 frame
xform 0 3232 2288
[comments]
