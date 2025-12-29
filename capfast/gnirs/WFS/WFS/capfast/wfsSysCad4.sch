[schematic2]
uniq 374
[tools]
[detail]
w 162 -381 100 0 n#373 ecad2.reboot.PLNK 0 -512 96 -512 96 -384 288 -384 carPlus.carPlus#355.BSLK
w -590 931 100 0 n#371 ecad2.reboot.MESS 0 -96 672 -96 672 928 -1792 928 -1792 192 -1472 192 eapply.Apply4.INMB
w -590 899 100 0 n#370 junction 160 -64 640 -64 640 896 -1760 896 -1760 224 -1472 224 eapply.Apply4.INPB
w 50 -61 100 0 n#370 ecad2.reboot.VAL 0 -64 160 -64 160 -192 288 -192 carPlus.carPlus#355.STAT
w -830 203 100 0 n#369 eapply.Apply4.OCLB -1088 192 -512 192 -512 -96 -320 -96 ecad2.reboot.ICID
w -814 235 100 0 n#368 eapply.Apply4.OUTB -1088 224 -480 224 -480 -64 -320 -64 ecad2.reboot.DIR
w 92 -725 100 0 n#372 ecad2.reboot.STLK 0 -544 96 -544 96 -896 224 -896 esubs.rebootSub.SLNK
w 162 -221 100 0 n#358 ecad2.reboot.OCID 0 -160 96 -160 96 -224 288 -224 carPlus.carPlus#355.ICID
w 66 515 100 0 n#351 ecad2.debug.OUTA 0 512 192 512 hwout.hwout#349.outp
w -612 -29 100 0 n#366 estringouts.debugStr.OUT -912 -592 -608 -592 -608 544 -320 544 ecad2.debug.A
w -1302 -573 100 0 n#365 embbi.debugMenu.FLNK -1376 -576 -1168 -576 estringouts.debugStr.SLNK
w -1252 -661 100 0 n#346 embbi.debugMenu.VAL -1376 -768 -1248 -768 -1248 -544 -1168 -544 estringouts.debugStr.DOL
w -734 835 100 0 n#311 ecad2.debug.VAL 0 736 256 736 256 832 -1664 832 -1664 288 -1472 288 eapply.Apply4.INPA
w -740 651 100 0 n#270 eapply.Apply4.MESS -1088 352 -736 352 -736 960 -448 960 outhier.MESS.p
w -772 699 100 0 n#269 eapply.Apply4.VAL -1088 384 -768 384 -768 1024 -448 1024 outhier.VAL.p
w -1870 355 100 0 n#246 inhier.CLID.P -2336 -32 -2208 -32 -2208 352 -1472 352 eapply.Apply4.CLID
w -1886 387 100 0 n#245 inhier.DIR.P -2336 64 -2240 64 -2240 384 -1472 384 eapply.Apply4.DIR
w -734 867 100 0 n#363 ecad2.debug.MESS 0 704 288 704 288 864 -1696 864 -1696 256 -1472 256 eapply.Apply4.INMA
w -708 507 100 0 n#213 eapply.Apply4.OUTA -1088 288 -704 288 -704 736 -320 736 ecad2.debug.DIR
w -676 475 100 0 n#364 eapply.Apply4.OCLA -1088 256 -672 256 -672 704 -320 704 ecad2.debug.ICID
s -1280 -912 100 0 converted to a string.
s -80 -1232 500 512 wfsSysCad4.sch
s -176 528 100 0 (A)
s -2320 -272 100 0 This record is used to allow dm to present the user with a menu
s -2320 -320 100 0 It is used by the engineering interface only.
s 208 432 100 0 These outputs link to the debugmode
s 208 400 100 0 record in the Status/Alarm database
s 208 368 100 0 by Channel Access
s -2320 -192 200 0 Debug mode menu
s -1328 -832 100 0 This "stringout" record serves two purposes:
s -1328 -880 100 0 (1) To ensure the enumerated value stored in the "mbbi" record is
s -1328 -960 100 0 (2) To convert the value output from the "mbbi" record to a link output.
s -1280 -992 100 0 (Connecting debugMenu.VAL to debug.A would not have worked).
s 96 560 100 0 BUG WORK AROUND: PP NMS added to destination
n -2432 704 -1952 1056 100
This schematic contains the
following systemwide Command
Action Directive (CAD) records
for the NIRI:
.
debug     - Set debugging mode.
.
reboot    - Reboot the IOC
.
.
.
.
.
.
_
[cell use]
use esubs 224 -985 100 0 rebootSub
xform 0 368 -720
p 288 -1024 100 0 1 INAM:cicsNullInit
p 288 -1088 100 0 1 PV:$(top)
p 288 -1056 100 0 1 SNAM:cicsReboot
use carPlus 288 -457 100 0 carPlus#355
xform 0 368 -296
p 304 -464 100 0 1 set:car reboot
use ecad2 -304 -640 100 0 reboot
xform 0 -160 -320
p -240 -672 100 0 1 DESC:Reboot
p -240 -736 100 0 1 PV:$(top)
p -240 -704 100 0 1 SNAM:CADreboot
use ecad2 -304 160 100 0 debug
xform 0 -160 480
p -240 128 100 0 1 DESC:Set debugging mode
p -278 944 100 0 0 FTVA:STRING
p -240 96 100 0 1 INAM:CADdebugInit
p -240 32 100 0 1 PV:$(top)
p -240 64 100 0 1 SNAM:CADdebug
p 16 512 75 1024 -1 pproc(OUTA):PP
use hwout 192 471 100 0 hwout#349
xform 0 288 512
p 288 503 100 0 -1 val(outp):$(sadtop)debugMode.VAL PP NMS
use estringouts -1168 -649 100 0 debugStr
xform 0 -1040 -576
p -1104 -688 100 0 1 OMSL:closed_loop
p -1104 -720 100 0 1 PV:$(top)
use embbi -2400 -1209 100 0 debugMenu
xform 0 -1888 -784
p -1856 -738 100 0 1 FRST:bad
p -2272 -706 100 0 1 NOBT:4
p -1856 -642 100 0 1 ONST:NONE
p -2272 -1232 100 0 1 PV:$(top)
p -1856 -706 100 0 1 THST:FULL
p -1856 -674 100 0 1 TWST:MIN
p -1856 -610 100 0 1 ZRST:NOLOG
use outhier -480 919 100 0 MESS
xform 0 -464 960
use outhier -480 983 100 0 VAL
xform 0 -464 1024
use inhier -2352 -73 100 0 CLID
xform 0 -2336 -32
use inhier -2352 23 100 0 DIR
xform 0 -2336 64
use bc200tr -2560 -1400 -100 0 frame
xform 0 -880 -96
p 0 -1232 100 0 1 author:S.M.Beard
p 240 -1248 100 0 -1 border:C
p 0 -1264 100 0 1 checked:S.M.Beard
p 272 -1248 100 0 -1 date:17 Mar 97
p 1696 384 100 0 -1 project:Gemini Near Infra-Red OIWFS
p 240 -1184 100 0 -1 title:NIRI WFS System CAD Records - 4
use eapply -1472 -249 100 0 Apply4
xform 0 -1280 112
p -1408 -288 100 0 1 DESC:$(name) wfsSysCad2 APPLY record
p -1408 -320 100 0 1 PV:$(top)$(mech)
[comments]
