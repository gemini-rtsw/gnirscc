[schematic2]
uniq 25
[tools]
[detail]
w 1368 3115 100 0 n#24 inhier.{comment}.P 1216 3104 1568 3104 estringout.comment.U2
w 1512 3595 100 0 n#23 inhier.{seqNum}.P 1216 3584 1856 3584 1856 3488 1824 3488 elongouts.seqNum.VAL
w 1368 3947 100 0 n#22 inhier.{title}.P 1216 3936 1568 3936 estringout.title.U2
w 1512 4459 100 0 n#21 inhier.{hdrDetail}.P 1216 4448 1856 4448 1856 4320 1824 4320 elongouts.hdrDetail.VAL
w 2088 3459 100 0 n#10 elongouts.seqNum.OUT 1824 3456 2400 3456 free
[cell use]
use estringout 1632 3368 100 0 comment
xform 0 1872 3088
p 1672 2832 65 1536 1 PV:$(top)
p 1600 3104 100 1024 -1 username(U2):VAL
use estringout 1632 4200 100 0 title
xform 0 1872 3920
p 1664 3672 65 1536 1 PV:$(top)
p 1600 3936 100 1024 -1 username(U2):VAL
use inhier 1200 3063 100 0 {comment}
xform 0 1216 3104
use inhier 1200 3543 100 0 {seqNum}
xform 0 1216 3584
use inhier 1200 3895 100 0 {title}
xform 0 1216 3936
use inhier 1200 4407 100 0 {hdrDetail}
xform 0 1216 4448
use elongouts 1640 4392 100 0 hdrDetail
xform 0 1696 4320
p 1648 4280 65 1536 1 PV:$(top)
use elongouts 1632 3560 100 0 seqNum
xform 0 1696 3488
p 1648 3440 65 1536 1 PV:$(top)
use CBorder 544 2216 -100 0 frame
xform 0 2224 3520
p 3112 2352 100 1536 1 Date:23 Apr 97
p 3216 2448 300 1792 -1 Dnumber:
p 3344 2432 150 1536 -1 Title:setHdrVars.sch
[comments]
