[schematic2]
uniq 72
[tools]
[detail]
[cell use]
use esirs 3216 1719 100 0 roi1YSize
xform 0 3424 1872
p 3328 1696 100 0 1 DESC:Y maximum of ROI 1 
p 3328 1664 100 0 1 EGU:pixels
p 3152 1424 100 0 0 FTVL:LONG
p 3456 1632 100 1024 1 name:$(top)$(I)
use esirs 1632 1719 100 0 roi1Y
xform 0 1840 1872
p 1744 1696 100 0 1 DESC:Y minimum of ROI 1
p 1744 1664 100 0 1 EGU:pixels
p 1568 1424 100 0 0 FTVL:LONG
p 1872 1632 100 1024 1 name:$(top)$(I)
use esirs 2400 1719 100 0 roi1XSize
xform 0 2608 1872
p 2512 1696 100 0 1 DESC:X maximum of ROI 1 
p 2512 1664 100 0 1 EGU:pixels
p 2336 1424 100 0 0 FTVL:LONG
p 2640 1632 100 1024 1 name:$(top)$(I)
use esirs 864 1719 100 0 roi1X
xform 0 1072 1872
p 976 1696 100 0 1 DESC:X minimum of ROI 1 
p 976 1664 100 0 1 EGU:pixels
p 800 1424 100 0 0 FTVL:LONG
p 1104 1632 100 1024 1 name:$(top)$(I)
use esirs 3216 1271 100 0 roi2YSize
xform 0 3424 1424
p 3328 1248 100 0 1 DESC:Y maximum of ROI 2 
p 3328 1216 100 0 1 EGU:pixels
p 3152 976 100 0 0 FTVL:LONG
p 3456 1184 100 1024 1 name:$(top)$(I)
use esirs 1632 1271 100 0 roi2Y
xform 0 1840 1424
p 1744 1248 100 0 1 DESC:Y minimum of ROI 2 
p 1744 1216 100 0 1 EGU:pixels
p 1568 976 100 0 0 FTVL:LONG
p 1872 1184 100 1024 1 name:$(top)$(I)
use esirs 2400 1271 100 0 roi2XSize
xform 0 2608 1424
p 2512 1248 100 0 1 DESC:X maximum of ROI 2 
p 2512 1216 100 0 1 EGU:pixels
p 2336 976 100 0 0 FTVL:LONG
p 2640 1184 100 1024 1 name:$(top)$(I)
use esirs 864 1271 100 0 roi2X
xform 0 1072 1424
p 976 1248 100 0 1 DESC:X minimum of ROI 2 
p 976 1216 100 0 1 EGU:pixels
p 800 976 100 0 0 FTVL:LONG
p 1104 1184 100 1024 1 name:$(top)$(I)
use esirs 864 839 100 0 roi3X
xform 0 1072 992
p 976 816 100 0 1 DESC:X minimum of ROI 3 
p 976 784 100 0 1 EGU:pixels
p 800 544 100 0 0 FTVL:LONG
p 1104 752 100 1024 1 name:$(top)$(I)
use esirs 2400 839 100 0 roi3XSize
xform 0 2608 992
p 2512 816 100 0 1 DESC:X maximum of ROI 3 
p 2512 784 100 0 1 EGU:pixels
p 2336 544 100 0 0 FTVL:LONG
p 2640 752 100 1024 1 name:$(top)$(I)
use esirs 1632 839 100 0 roi3Y
xform 0 1840 992
p 1744 816 100 0 1 DESC:Y minimum of ROI 3 
p 1744 784 100 0 1 EGU:pixels
p 1568 544 100 0 0 FTVL:LONG
p 1872 752 100 1024 1 name:$(top)$(I)
use esirs 3216 839 100 0 roi3YSize
xform 0 3424 992
p 3328 816 100 0 1 DESC:Y maximum of ROI 3 
p 3328 784 100 0 1 EGU:pixels
p 3152 544 100 0 0 FTVL:LONG
p 3456 752 100 1024 1 name:$(top)$(I)
use esirs 3216 375 100 0 roi4YSize
xform 0 3424 528
p 3328 352 100 0 1 DESC:Y maximum of ROI 4 
p 3328 320 100 0 1 EGU:pixels
p 3152 80 100 0 0 FTVL:LONG
p 3456 288 100 1024 1 name:$(top)$(I)
use esirs 1632 375 100 0 roi4Y
xform 0 1840 528
p 1744 352 100 0 1 DESC:Y minimum of ROI 4 
p 1744 320 100 0 1 EGU:pixels
p 1568 80 100 0 0 FTVL:LONG
p 1872 288 100 1024 1 name:$(top)$(I)
use esirs 2400 375 100 0 roi4XSize
xform 0 2608 528
p 2512 352 100 0 1 DESC:X maximum of ROI 4 
p 2512 320 100 0 1 EGU:pixels
p 2336 80 100 0 0 FTVL:LONG
p 2640 288 100 1024 1 name:$(top)$(I)
use esirs 864 375 100 0 roi4X
xform 0 1072 528
p 976 352 100 0 1 DESC:X minimum of ROI 4 
p 976 320 100 0 1 EGU:pixels
p 800 80 100 0 0 FTVL:LONG
p 1104 288 100 1024 1 name:$(top)$(I)
use hwin 656 2039 100 0 hwin#10
xform 0 752 2080
p 659 2072 100 0 -1 val(in):$(cctop)roi.VALB
use hwin 1424 2039 100 0 hwin#12
xform 0 1520 2080
p 1427 2072 100 0 -1 val(in):$(cctop)roi.VALC
use hwin 2192 2039 100 0 hwin#14
xform 0 2288 2080
p 2195 2072 100 0 -1 val(in):$(cctop)roi.VALD
use hwin 3008 2039 100 0 hwin#16
xform 0 3104 2080
p 3011 2072 100 0 -1 val(in):$(cctop)roi.VALE
use hwin 656 1591 100 0 hwin#22
xform 0 752 1632
p 659 1624 100 0 -1 val(in):$(cctop)roi.VALF
use hwin 1424 1591 100 0 hwin#24
xform 0 1520 1632
p 1427 1624 100 0 -1 val(in):$(cctop)roi.VALG
use hwin 2192 1591 100 0 hwin#26
xform 0 2288 1632
p 2195 1624 100 0 -1 val(in):$(cctop)roi.VALH
use hwin 3008 1591 100 0 hwin#28
xform 0 3104 1632
p 3011 1624 100 0 -1 val(in):$(cctop)roi.VALI
use hwin 3008 1159 100 0 hwin#31
xform 0 3104 1200
p 3011 1192 100 0 -1 val(in):$(cctop)roi.VALM
use hwin 2192 1159 100 0 hwin#33
xform 0 2288 1200
p 2195 1192 100 0 -1 val(in):$(cctop)roi.VALL
use hwin 1424 1159 100 0 hwin#35
xform 0 1520 1200
p 1427 1192 100 0 -1 val(in):$(cctop)roi.VALK
use hwin 656 1159 100 0 hwin#37
xform 0 752 1200
p 659 1192 100 0 -1 val(in):$(cctop)roi.VALJ
use hwin 656 695 100 0 hwin#46
xform 0 752 736
p 659 728 100 0 -1 val(in):$(cctop)roi.VALN
use hwin 1424 695 100 0 hwin#48
xform 0 1520 736
p 1427 728 100 0 -1 val(in):$(cctop)roi.VALO
use hwin 2192 695 100 0 hwin#50
xform 0 2288 736
p 2195 728 100 0 -1 val(in):$(cctop)roi.VALP
use hwin 3008 695 100 0 hwin#52
xform 0 3104 736
p 3011 728 100 0 -1 val(in):$(cctop)roi.VALQ
use eborderC 528 -329 100 0 eborderC#5
xform 0 2208 976
p 3104 -176 100 768 -1 author:Peter Ruckle
p 3088 -208 100 768 -1 date:1-18-01
p 3328 -128 200 768 -1 file:roiSad.sch
p 3600 -176 100 0 -1 page:1
p 3712 -176 100 0 -1 pages:1
p 3376 -176 100 0 -1 revision:0
p 3328 -64 150 768 -1 system:Gnirs Components Controller
use notes 816 -185 100 0 notes#0
xform 0 1072 0
p 844 126 100 0 -1 COMMENT1:This schematic contains the Status Alarm
p 844 94 100 0 -1 COMMENT2:database records for a group of regions
p 844 64 100 0 -1 COMMENT3:of interest (ROIs). A separate instance 
p 844 32 100 0 -1 COMMENT4:of this symbol can be used for each group
p 844 0 100 0 -1 COMMENT5:of upto four ROIs.  There is a status
p 844 -32 100 0 -1 COMMENT6:item one level up which has the number
p 844 -64 100 0 -1 COMMENT7:of regions of interest that are 
p 844 -96 100 0 -1 COMMENT8:currently defined.
[comments]
