[schematic2]
uniq 58
[tools]
[detail]
[cell use]
use bc200tr -48 472 -100 0 frame
xform 0 1632 1776
use esirs 448 1079 100 0 Position
xform 0 656 1232
p 576 1200 100 0 1 FTVL:STRING
p 576 1152 100 0 1 SCAN:Passive
p 560 1072 100 1024 0 name:$(sadtop)$(name)$(I)
use esirs 1120 1015 100 0 State
xform 0 1328 1168
p 1248 1104 100 0 1 FTVL:STRING
p 1248 1056 100 0 1 SCAN:Passive
p 1232 1008 100 1024 0 name:$(sadtop)$(name)$(I)
use esirs 1120 1687 100 0 Health
xform 0 1328 1840
p 1280 1728 100 0 1 SCAN:Passive
p 1232 1680 100 1024 0 name:$(sadtop)$(name)$(I)
p 1120 1936 75 1280 -1 palrm(INP):MS
use esirs 448 1719 100 0 Datumed
xform 0 656 1872
p 624 1760 100 0 1 FTVL:LONG
p 576 1792 100 0 1 SCAN:Passive
p 560 1712 100 1024 0 name:$(sadtop)$(name)$(I)
use esirs 448 1399 100 0 Parked
xform 0 656 1552
p 608 1440 100 0 1 FTVL:LONG
p 576 1472 100 0 1 SCAN:Passive
p 560 1392 100 1024 0 name:$(sadtop)$(name)$(I)
use esirs 1120 1367 100 0 ParkPos
xform 0 1328 1520
p 1056 1072 100 0 0 FTVL:LONG
p 1232 1408 100 0 1 SCAN:Passive
p 1232 1360 100 1024 0 name:$(sadtop)$(name)$(I)
use esirs 1120 2039 100 0 Eng
xform 0 1328 2192
p 1056 1744 100 0 0 FTVL:LONG
p 1248 2080 100 0 1 SCAN:Passive
p 1232 2032 100 1024 0 name:$(sadtop)$(name)$(I)
use esirs 448 2039 100 0 Name
xform 0 656 2192
p 544 2080 100 0 1 SCAN:Passive
p 560 2032 100 1024 0 name:$(sadtop)$(name)$(I)
use esirs 1824 1943 100 0 nLim
xform 0 2032 2096
p 1952 2032 100 0 1 FTVL:LONG
p 1952 1984 100 0 1 SCAN:Passive
p 1936 1936 100 1024 0 name:$(sadtop)$(name)$(I)
use esirs 1856 1623 100 0 pLim
xform 0 2064 1776
p 1984 1712 100 0 1 FTVL:LONG
p 1984 1664 100 0 1 SCAN:Passive
p 1968 1616 100 1024 0 name:$(sadtop)$(name)$(I)
use esirs 1856 1271 100 0 Home
xform 0 2064 1424
p 1984 1360 100 0 1 FTVL:LONG
p 1984 1312 100 0 1 SCAN:Passive
p 1968 1264 100 1024 0 name:$(sadtop)$(name)$(I)
use esirs 2480 1735 100 0 OT
xform 0 2688 1888
p 2608 1824 100 0 1 FTVL:LONG
p 2608 1776 100 0 1 SCAN:1 second
p 2592 1728 100 1024 0 name:$(sadtop)$(name)$(I)
use esirs 2512 1351 100 0 Fault
xform 0 2720 1504
p 2640 1440 100 0 1 FTVL:LONG
p 2640 1392 100 0 1 SCAN:1 second
p 2624 1344 100 1024 0 name:$(sadtop)$(name)$(I)
[comments]
