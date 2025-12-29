[schematic2]
uniq 515
[tools]
[detail]
w -798 491 100 0 n#512 ecalcs.SafeCount.VAL -608 224 -576 224 -576 480 -960 480 -960 416 -896 416 ecalcs.SafeCount.INPA
w -990 43 100 0 n#511 elongins.Safe.FLNK -1024 32 -896 32 ecalcs.SafeCount.SLNK
w -798 1115 100 0 n#483 ecalcs.StopCount.VAL -608 848 -576 848 -576 1104 -960 1104 -960 1040 -896 1040 ecalcs.StopCount.INPA
w -990 667 100 0 n#482 elongins.Stop.FLNK -1024 656 -896 656 ecalcs.StopCount.SLNK
w -798 1691 100 0 n#478 ecalcs.OpCount.VAL -608 1424 -576 1424 -576 1680 -960 1680 -960 1616 -896 1616 ecalcs.OpCount.INPA
w -990 1243 100 0 n#477 elongins.Op.FLNK -1024 1232 -896 1232 ecalcs.OpCount.SLNK
s -1504 2048 100 768 performed on all mechanisms.
s -1504 2080 100 768 the counter changes, then some action (SNL determined) is
s -1504 2112 100 768 These records are monitored by SNL code.  When the value of
[cell use]
use elongins -1216 64 100 768 Safe
xform 0 -1152 16
use ecalcs -832 448 100 768 SafeCount
xform 0 -752 208
p -816 160 100 768 -1 CALC:A+1
use common 1248 1863 100 0 common#510
xform 0 1408 2000
use elongins -1216 1264 100 768 Op
xform 0 -1152 1216
use elongins -1216 688 100 768 Stop
xform 0 -1152 640
use ecalcs -832 1648 100 768 OpCount
xform 0 -752 1408
p -816 1360 100 768 -1 CALC:A+1
use ecalcs -832 1072 100 768 StopCount
xform 0 -752 832
p -816 784 100 768 -1 CALC:A+1
use eborderC 1600 -160 100 1280 $Id:
xform 0 16 1024
p 1136 -64 200 1536 -1 file:lock.sch
p 1600 -160 100 1280 -1 id:$Id: global.sch,v 1.2 2009/05/27 19:34:03 fkraemer Exp $
[comments]
