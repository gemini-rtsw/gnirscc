[schematic2]
uniq 26
[tools]
[detail]
w 1464 3659 100 0 n#14 inhier.{hiCol}.P 1088 3648 1888 3648 1888 3552 1824 3552 elongouts.elongouts#7.VAL
w 1464 3883 100 0 n#13 inhier.{hiRow}.P 1088 3872 1888 3872 1888 3776 1824 3776 elongouts.elongouts#6.VAL
w 1464 4107 100 0 n#12 inhier.{lowCol}.P 1088 4096 1888 4096 1888 4000 1824 4000 elongouts.elongouts#5.VAL
w 1464 4331 100 0 n#11 inhier.{lowRow}.P 1088 4320 1888 4320 1888 4224 1824 4224 elongouts.elongouts#0.VAL
[cell use]
use inhier 1072 3607 100 0 {hiCol}
xform 0 1088 3648
use inhier 1072 3831 100 0 {hiRow}
xform 0 1088 3872
use inhier 1072 4055 100 0 {lowCol}
xform 0 1088 4096
use inhier 1072 4279 100 0 {lowRow}
xform 0 1088 4320
use elongouts 1568 3463 100 0 elongouts#7
xform 0 1696 3552
p 1540 3624 100 0 1 name:$(top)hiCol$(ROInum)
use elongouts 1568 3687 100 0 elongouts#6
xform 0 1696 3776
p 1580 3852 100 0 1 name:$(top)hiRow$(ROInum)
use elongouts 1568 3911 100 0 elongouts#5
xform 0 1696 4000
p 1552 4072 100 0 1 name:$(top)lowCol$(ROInum)
use elongouts 1568 4135 100 0 elongouts#0
xform 0 1696 4224
p 1564 4296 100 0 1 name:$(top)lowRow$(ROInum)
use CBorder 224 2184 -100 0 frame
xform 0 1904 3488
p 2792 2320 100 1536 1 Date:23 Apr 97
p 2896 2416 300 1792 -1 Dnumber:
p 3004 2396 150 1536 -1 Title: setDrRoi.sch
p 2796 2352 100 1536 1 revision: 1.1
[comments]
