[schematic2]
uniq 11
[tools]
[detail]
w 670 1419 100 0 n#8 inhier.{procMode}.P 320 1408 1056 1408 1056 1216 1024 1216 elongouts.procMode.VAL
w 670 1739 100 0 n#7 inhier.{HKstate}.P 320 1728 1056 1728 1056 1536 1024 1536 elongouts.HKState.VAL
[cell use]
use inhier 304 1367 100 0 {procMode}
xform 0 320 1408
use inhier 304 1687 100 0 {HKstate}
xform 0 320 1728
use elongouts 840 1288 100 0 procMode
xform 0 896 1216
p 848 1168 65 1536 1 PV:$(top)
p 854 1213 100 0 -1 Type:stringout
use elongouts 840 1616 100 0 HKState
xform 0 896 1536
p 832 1678 100 0 0 HHSV:MINOR
p 832 1742 100 0 0 HIGH:1
p 832 1806 100 0 0 HIHI:1
p 832 1646 100 0 0 HSV:MINOR
p 840 1488 65 1536 1 PV:$(top)
use CBorder -576 -192 -100 0 frame
xform 0 1104 1112
p 1992 -56 100 1536 1 Date:23 Apr 97
p 2096 40 300 1792 -1 Dnumber: 
p 2204 20 200 1536 -1 Title: setObsState.sch
p 1996 -24 100 1536 1 revision:0.9
[comments]
