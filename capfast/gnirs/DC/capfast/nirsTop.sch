[schematic2]
uniq 160
[tools]
[detail]
[cell use]
use CBorder -712 -120 -100 0 frame
xform 0 968 1184
p 2360 12 75 1536 -1 Author:Janet E. Tvedt
p 1856 16 100 1536 1 Date:23 Apr 97
p 1960 112 300 1792 -1 Dnumber:
p 2088 96 150 1536 -1 Title:naacTop.sch
use hdwrNEngFuncs 384 535 100 0 hdwrNEngFuncs#158
xform 0 592 768
p 496 512 100 0 1 set1:top nirsg:dc:
p 496 496 100 0 1 set2:sadtop nirsg:sad:dc:
use naacDc 112 1123 100 0 naacDc#157
xform 0 592 1404
p 132 1096 100 0 1 set1:top nirsg:dc:
p 128 1056 100 0 1 set2:sadtop nirsg:sad:dc:
p 128 1024 100 0 1 set3:agtop ag:
p 128 1008 100 0 1 set4:cctop nirs:cc:
use notes 1608 1843 100 0 notes#155
xform 0 1864 2028
p 1636 2154 100 0 -1 COMMENT1:This is the top level schematic for the
p 1636 2122 100 0 -1 COMMENT2:Gemini NOAO Advanced Array Controller.
p 1636 2092 100 0 -1 COMMENT3:It contains no database records, but
p 1636 2060 100 0 -1 COMMENT4:defines the following macro variables
p 1636 2028 100 0 -1 COMMENT5:and includes the top level detector
p 1636 1996 100 0 -1 COMMENT6:schematic "naacDc.sch".
p 1636 1964 100 0 -1 COMMENT7:.
p 1636 1932 100 0 -1 COMMENT8:top = control db record name prefix
p 1636 1900 100 0 -1 COMMENT9:sadtop = status alarm db record name prefix
[comments]
