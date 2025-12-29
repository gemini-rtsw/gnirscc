[schematic2]
uniq 39
[tools]
[detail]
[cell use]
use esirs 1088 1735 100 0 sysHealth
xform 0 1296 1888
p 1200 1728 100 1024 0 name:$(sadtop)$(I)
use esirs 1088 903 100 0 motorHealth
xform 0 1296 1056
p 1200 896 100 1024 0 name:$(sadtop)$(I)
use esirs 512 1319 100 0 pressureHealth
xform 0 720 1472
p 624 1312 100 1024 0 name:$(sadtop)$(I)
use esirs 512 887 100 0 tempHealth
xform 0 720 1040
p 624 880 100 1024 0 name:$(sadtop)$(I)
use esirs 480 1751 100 0 ccTopHealth
xform 0 688 1904
p 592 1744 100 1024 0 name:$(sadtop)$(I)
use esirs 1088 1319 100 0 sadHealth
xform 0 1296 1472
p 1200 1312 100 1024 0 name:$(sadtop)$(I)
use bc200tr 256 -344 -100 0 frame
xform 0 1936 960
p 3072 -64 100 0 -1 filename:healthSad.sch
[comments]
