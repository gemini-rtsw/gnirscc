[schematic2]
uniq 184
[tools]
[detail]
w 66 1635 100 0 c#182 inhier.{seqFDly}.P -32 1632 224 1632 224 1568 448 1568 eaos.fDly.DOL
w 756 1507 100 0 n#173 eaos.fDly.OUT 704 1504 868 1504 hwout.hwout#174.outp
w 754 1739 100 0 n#172 eaos.intTime.OUT 704 1728 864 1728 hwout.hwout#171.outp
w 24 1859 100 0 {seqIntTime} inhier.{seqIntTime}.P -32 1856 224 1856 224 1792 448 1792 eaos.intTime.DOL
s -8 1640 100 0 (fDelay)
s -8 1864 100 0 (seqTime)
[cell use]
use ebos 516 1360 100 0 errIntTime
xform 0 576 1280
p 520 1192 100 0 1 PV:$(top)
use CBorder -1020 -224 -100 0 frame
xform 0 660 1080
p 1548 -88 100 1536 1 Date:23 Apr 97
p 1652 8 300 1792 -1 Dnumber:
p 1780 -8 150 1536 -1 Title:setIntTime.sch
use inhier -48 1591 100 0 {seqFDly}
xform 0 -32 1632
use inhier -48 1815 100 0 {seqIntTime}
xform 0 -32 1856
use eaos 512 1608 100 0 fDly
xform 0 576 1536
p 335 1770 100 0 0 DTYP:wFireVarMsg
p 528 1512 65 1536 1 PV:$(top)
use eaos 512 1832 100 0 intTime
xform 0 576 1760
p 335 1994 100 0 0 DTYP:wFireVarMsg
p 528 1736 65 1536 1 PV:$(top)
use hwout 868 1463 100 0 hwout#174
xform 0 964 1504
p 956 1504 65 1536 -1 val(outp):@Node=10,Var=8,Grp=-1,Idx=0
use hwout 864 1687 100 0 hwout#171
xform 0 960 1728
p 974 1688 100 0 0 primitive:hwout
p 952 1728 65 1536 -1 val(outp):@Node=10,Var=7,Grp=-1,Idx=0
use notes -672 1535 100 0 notes#160
xform 0 -416 1720
p -644 1846 100 0 -1 COMMENT1:This implements the setIntTime command.
p -644 1814 100 0 -1 COMMENT2:errIntTime needs to be set by C function.
[comments]
