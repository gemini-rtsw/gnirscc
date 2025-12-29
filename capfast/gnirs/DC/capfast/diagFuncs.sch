[schematic2]
uniq 15
[tools]
[detail]
w 24 2283 100 0 n#13 ebis.doDiag.FLNK -32 2272 128 2272 128 2144 288 2144 emosub.diagFuncs.SLNK
w 24 2419 100 0 n#10 embbis.runDiag.VAL -32 2416 128 2416 128 2560 288 2560 emosub.diagFuncs.INPA
[cell use]
use notes 1744 2303 100 0 notes#14
xform 0 2000 2488
p 1772 2614 100 0 -1 COMMENT1:NOTES: This schematic allows diagnostic
p 1772 2582 100 0 -1 COMMENT2:functions to be called through the
p 1772 2552 100 0 -1 COMMENT3:mosub record.
use CBorder -824 336 -100 0 frame
xform 0 856 1640
p 1744 472 100 1536 1 Date:24 Apr 97
p 1848 568 300 1792 -1 Dnumber:
p 1976 552 150 1536 -1 Title:diagFuncs.sch
use ebis -224 2312 100 0 doDiag
xform 0 -160 2256
p -512 2094 100 0 0 ONAM:Running
p -224 2192 65 1536 1 PV:$(top)
p -512 2126 100 0 0 ZNAM:Halted
p -320 2288 75 1280 -1 pproc(INP):PP
use embbis -224 2504 100 0 runDiag
xform 0 -160 2432
p -352 2651 100 0 0 DESC:multibit binary input record
p -128 2270 100 0 0 EIST:Diagnostic_Eight
p -320 2270 100 0 0 EIVL:8
p -128 2398 100 0 0 FRST:Diagnostic_Four
p -320 2398 100 0 0 FRVL:4
p -128 2366 100 0 0 FVST:Diagnostic_Five
p -320 2366 100 0 0 FVVL:5
p -128 2494 100 0 0 ONST:Diagnostic_One
p -320 2494 100 0 0 ONVL:1
p -224 2352 65 0 1 PV:$(top)
p -128 2302 100 0 0 SVST:Diagnostic_Seven
p -320 2302 100 0 0 SVVL:7
p -128 2334 100 0 0 SXST:Diagnostic_Six
p -320 2334 100 0 0 SXVL:6
p -128 2430 100 0 0 THST:Diagnostic_Three
p -320 2430 100 0 0 THVL:3
p -128 2462 100 0 0 TWST:Diagnostic_Two
p -320 2462 100 0 0 TWVL:2
p -185 2428 100 0 -1 Type:mbbi
p -160 2478 100 0 0 ZRST:Diagnostic_Zero
p 64 2526 100 0 0 ZRSV:NO_ALARM
use emosub 360 2608 100 0 diagFuncs
xform 0 432 2128
p 0 2206 100 0 0 INAM:iDiagnose
p 368 2088 65 1536 1 PV:$(top)
p 368 2072 65 1536 1 SNAM:Diagnose
p 584 2120 65 1536 1 STR1:Test string 1
p 584 2056 65 1536 1 STR2:Test string 2
p 584 1992 65 1536 1 STR3:Test string 3
p 584 1928 65 1536 1 STR4:Test string 4
p 592 1864 65 1536 1 STR5:Test string 5
p 584 1800 65 1536 1 STR6:Test string 6
[comments]
