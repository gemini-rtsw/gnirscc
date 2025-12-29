[schematic2]
uniq 68
[tools]
[detail]
w -264 579 100 0 n#65 hwin.hwin#66.in -352 576 -128 576 egenSub.InvCalc.INPH
w -264 643 100 0 n#63 hwin.hwin#64.in -352 640 -128 640 egenSub.InvCalc.INPG
w -264 707 100 0 n#62 hwin.hwin#61.in -352 704 -128 704 egenSub.InvCalc.INPF
w -264 771 100 0 n#59 hwin.hwin#60.in -352 768 -128 768 egenSub.InvCalc.INPE
w -264 835 100 0 n#58 hwin.hwin#57.in -352 832 -128 832 egenSub.InvCalc.INPD
w -264 899 100 0 n#55 hwin.hwin#56.in -352 896 -128 896 egenSub.InvCalc.INPC
w -264 1027 100 0 n#20 hwin.hwin#19.in -352 1024 -128 1024 egenSub.InvCalc.INPA
w -264 963 100 0 n#21 hwin.hwin#22.in -352 960 -128 960 egenSub.InvCalc.INPB
w 312 1059 100 0 n#24 esirs.Pos1.INP 512 1056 160 1056 egenSub.InvCalc.VALA
w 728 771 100 0 n#30 egenSub.InvCalc.VALB 160 992 384 992 384 768 1120 768 esirs.Pos2.INP
w 412 603 100 0 n#31 egenSub.InvCalc.FLNK 160 320 416 320 416 896 512 896 esirs.Pos1.SLNK
w 956 827 100 0 n#28 esirs.Pos1.FLNK 928 1056 960 1056 960 608 1120 608 esirs.Pos2.SLNK
s 1520 16 400 1280 comp2mSadAlt
s 736 2224 500 1024 2-Axis $(alt) SAD records.
[cell use]
use esirs -736 2144 100 768 Label
xform 0 -592 2016
p -736 1856 100 768 1 DESC:The FDSC field describes the $(desc)
p -736 1728 100 768 1 EGU:
p -736 1696 100 768 1 FDSC:Motion using $(alt) coordinates
p -736 1824 100 768 1 FTVL:STRING
p -736 1792 100 768 1 PV:$(top)$(mech)$(prefix)
p -736 1760 100 768 1 SCAN:Passive
use esirs 576 1088 100 768 Pos1
xform 0 720 960
p 576 640 100 768 1 DESC:Position of $(axis1)-Axis ($(alt) units)
p 576 704 100 768 1 EGU:$(alt) units
p 576 672 100 768 1 FDSC:Position of $(axis1)-Axis ($(alt) units)
p 576 800 100 768 1 FTVL:DOUBLE
p 576 768 100 768 1 PREC:3
p 576 736 100 768 1 name:$(top)$(mech)$(prefix):$(axis1)Pos
use esirs 1184 800 100 768 Pos2
xform 0 1328 672
p 1184 352 100 768 1 DESC:Position of $(axis2)-Axis ($(alt) units)
p 1184 416 100 768 1 EGU:$(alt) units
p 1184 384 100 768 1 FDSC:Position of $(axis2)-Axis ($(alt) units)
p 1184 512 100 768 1 FTVL:DOUBLE
p 1184 480 100 768 1 PREC:3
p 1184 448 100 768 1 name:$(top)$(mech)$(prefix):$(axis2)Pos
use esirs -256 2144 100 768 Units
xform 0 -112 2016
p -256 1856 100 768 1 DESC:Definition of $(alt) units
p -256 1728 100 768 1 EGU:
p -256 1696 100 768 1 FDSC:Definition of $(alt) units
p -256 1824 100 768 1 FTVL:STRING
p -256 1792 100 768 1 PV:$(top)$(mech)$(prefix)
p -256 1760 100 768 1 SCAN:Passive
use hwin -544 983 100 0 hwin#19
xform 0 -448 1024
p -541 1016 100 0 -1 val(in):$(eng)$(mech)$(axis1)Motor.RBV
use hwin -544 919 100 0 hwin#22
xform 0 -448 960
p -541 952 100 0 -1 val(in):$(eng)$(mech)$(axis2)Motor.RBV
use hwin -544 855 100 0 hwin#56
xform 0 -448 896
p -541 888 100 0 -1 val(in):$(top)$(mech)$(prefix)I11
use hwin -544 791 100 0 hwin#57
xform 0 -448 832
p -541 824 100 0 -1 val(in):$(top)$(mech)$(prefix)I12
use hwin -544 727 100 0 hwin#60
xform 0 -448 768
p -541 760 100 0 -1 val(in):$(top)$(mech)$(prefix)I21
use hwin -544 663 100 0 hwin#61
xform 0 -448 704
p -541 696 100 0 -1 val(in):$(top)$(mech)$(prefix)I22
use hwin -544 599 100 0 hwin#64
xform 0 -448 640
p -541 632 100 0 -1 val(in):$(top)$(mech)$(prefix)C1
use hwin -544 535 100 0 hwin#66
xform 0 -448 576
p -541 568 100 0 -1 val(in):$(top)$(mech)$(prefix)C2
use egenSub -64 1088 100 768 InvCalc
xform 0 16 688
p -64 160 100 768 1 DESC:$(alt) Inverse Transformation
p -192 1040 100 1280 1 FTA:LONG
p -192 976 100 1280 1 FTB:LONG
p 208 1056 100 768 1 FTVA:DOUBLE
p 208 992 100 768 1 FTVB:DOUBLE
p -416 814 100 0 0 PREC:4
p -64 256 100 768 1 PV:$(top)$(mech)$(prefix)
p -64 224 100 768 1 SCAN:.5 second
p -64 192 100 768 1 SNAM:comp2mAltInvCalc
use bc200tr -1024 -120 -100 0 frame
xform 0 656 1184
p 1552 48 100 0 1 author:H.T.Yamada
p 1776 32 100 0 -1 border:C
p 1552 16 100 0 1 checked:H.T.Yamada
p 1808 32 100 768 -1 date:yyyy-mm-dd
p 1792 160 100 0 -1 project:Near Infra-Red Imager
p 1776 0 100 768 -1 revision:1.0
p 1792 96 100 0 -1 title:Diagram Title
[comments]
