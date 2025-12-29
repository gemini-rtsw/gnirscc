[schematic2]
uniq 189
[tools]
[detail]
w 1090 1835 -100 0 FLNK outhier.FLNK.p 1152 1824 1088 1824 lockCar.lockC.FLNK
w 1082 1867 -100 0 c#185 lockCar.lockC.OCID 1088 1856 1136 1856 bihier.OCID.p
w 1082 1899 -100 0 c#184 lockCar.lockC.OVAL 1088 1888 1136 1888 bihier.OVAL.p
w -534 1771 -100 0 n#183 bihier.MESS.p -528 1760 -480 1760 lockCad.lock.MESS
w -534 1803 -100 0 n#182 bihier.VAL.p -528 1792 -480 1792 lockCad.lock.VAL
w -534 1835 -100 0 n#181 bihier.CLID.p -528 1824 -480 1824 lockCad.lock.CLID
w -534 1867 -100 0 DIR bihier.DIR.p -528 1856 -480 1856 lockCad.lock.DIR
s 1488 112 400 1280 lock
[cell use]
use outhier 1168 1824 100 1536 FLNK
xform 0 1136 1824
use bihier -544 1856 100 2048 DIR
xform 0 -528 1856
use bihier -544 1824 100 2048 CLID
xform 0 -528 1824
use bihier -544 1792 100 2048 VAL
xform 0 -528 1792
use bihier -544 1760 100 2048 MESS
xform 0 -528 1760
use bihier 1168 1888 100 1536 OVAL
xform 0 1136 1888
use bihier 1168 1856 100 1536 OCID
xform 0 1136 1856
use lockCar 464 1888 100 1024 lockC
xform 0 704 1824
use lockCad -336 1888 100 1024 lock
xform 0 -96 1824
use bc200tr -1056 -24 -100 0 frame
xform 0 624 1280
p 1520 144 100 0 1 author:S.M.Beard
p 1744 128 100 0 -1 border:C
p 1520 112 100 0 1 checked:S.M.Beard
p 1776 128 100 0 -1 date:20 May 97
p 1760 256 100 0 -1 project:Gemini Near Infrared Imager
p 1760 192 100 0 -1 title:Under top level NIRI CC schematic
[comments]
