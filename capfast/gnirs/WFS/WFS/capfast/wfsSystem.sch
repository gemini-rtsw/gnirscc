[schematic2]
uniq 318
[tools]
[detail]
w -974 -989 100 0 n#311 wfsSysCad4.wfsSysCad4#307.MESS -416 -736 -160 -736 -160 -992 -1728 -992 -1728 -192 -1472 -192 eapply.Apply.INMC
w -974 -1021 100 0 n#310 wfsSysCad4.wfsSysCad4#307.VAL -416 -704 -128 -704 -128 -1024 -1760 -1024 -1760 -160 -1472 -160 eapply.Apply.INPC
w -900 -469 100 0 n#309 eapply.Apply.OCLC -1088 -192 -896 -192 -896 -736 -576 -736 wfsSysCad4.wfsSysCad4#307.CLID
w -868 -437 100 0 n#308 eapply.Apply.OUTC -1088 -160 -864 -160 -864 -704 -576 -704 wfsSysCad4.wfsSysCad4#307.DIR
w -996 411 100 0 MESS eapply.Apply.MESS -1088 32 -992 32 -992 800 -800 800 outhier.MESS.p
w -1028 491 100 0 n#305 eapply.Apply.VAL -1088 64 -1024 64 -1024 928 -800 928 outhier.VAL.p
w -974 387 100 0 n#300 wfsSysCad2.wfsSysCad2#289.MESS -416 -160 -128 -160 -128 384 -1760 384 -1760 -128 -1472 -128 eapply.Apply.INMB
w -974 355 100 0 n#299 wfsSysCad2.wfsSysCad2#289.VAL -416 -128 -160 -128 -160 352 -1728 352 -1728 -96 -1472 -96 eapply.Apply.INPB
w -1006 291 100 0 n#298 wfsSysCad1.wfsSysCad1#288.MESS -416 128 -256 128 -256 288 -1696 288 -1696 -64 -1472 -64 eapply.Apply.INMA
w -1006 259 100 0 n#297 wfsSysCad1.wfsSysCad1#288.VAL -416 160 -288 160 -288 256 -1664 256 -1664 -32 -1472 -32 eapply.Apply.INPA
w -926 -125 100 0 n#294 eapply.Apply.OCLB -1088 -128 -704 -128 -704 -160 -576 -160 wfsSysCad2.wfsSysCad2#289.CLID
w -910 -93 100 0 n#293 eapply.Apply.OUTB -1088 -96 -672 -96 -672 -128 -576 -128 wfsSysCad2.wfsSysCad2#289.DIR
w -910 -61 100 0 n#292 eapply.Apply.OCLA -1088 -64 -672 -64 -672 128 -576 128 wfsSysCad1.wfsSysCad1#288.CLID
w -926 -29 100 0 n#291 eapply.Apply.OUTA -1088 -32 -704 -32 -704 160 -576 160 wfsSysCad1.wfsSysCad1#288.DIR
w -1726 35 100 0 n#246 inhier.CLID.P -2016 -32 -1920 -32 -1920 32 -1472 32 eapply.Apply.CLID
w -1774 67 100 0 n#245 inhier.DIR.P -2016 64 -1472 64 eapply.Apply.DIR
s -736 -512 100 0 Instrument Sequencer, so it does not attempt to forward these commands.
s -736 -480 100 0 It is important that this fact is reflected in the
s -736 -448 100 0 They are not required and are therefore left out.
s -736 -416 100 0 NOTE: wfsSysCad3.sch contains commands which are all optional.
s -1472 -800 100 0 wfsSysCad schematics in the order A, B, C, D
s -1472 -752 100 0 "apply" records contained in the lower level
s -1472 -704 100 0 The "apply" record sequences the other
s -512 -784 100 0 (C)
s -512 -208 100 0 (B)
s -512 80 100 0 (A)
s -48 -1232 500 512 wfsSysCad.sch
n 64 -928 544 -576 100
This is the top level schematic
for the systemwide Command Action
Directive (CAD) records for the
NIRI. It is split into the
following subschematics:
.
wfsSysCad1 - test,init,datum,park.
.
wfsSysCad2 - guide,endGuide,verify,endVerify.
.
wfsSysCad3 - observe,pause,continue,stop,abort.
.
wfsSysCad4 - debug,reboot.
.
_
[cell use]
use notes 0 599 100 0 notes#317
xform 0 256 784
p 0 576 100 768 0 author:S.M. Beard
p 16 896 100 768 -1 comment0:To add a lot of new system commands, add
p 16 862 100 768 -1 comment1:a new lower level schematic to this one
p 16 830 100 768 -1 comment2:(e.g. wfsSysCad5) and attach it to the
p 16 800 100 768 -1 comment3:APPLY record.
p 256 928 100 1024 -1 title:ICD 14
use wfsSysCad4 -576 -857 100 0 wfsSysCad4#307
xform 0 -496 -752
use outhier -832 759 100 0 MESS
xform 0 -816 800
use outhier -832 887 100 0 VAL
xform 0 -816 928
use wfsSysCad2 -576 -281 100 0 wfsSysCad2#289
xform 0 -496 -176
use wfsSysCad1 -576 7 100 0 wfsSysCad1#288
xform 0 -496 112
use inhier -2032 23 100 0 DIR
xform 0 -2016 64
use inhier -2032 -73 100 0 CLID
xform 0 -2016 -32
use bc200tr -2560 -1400 -100 0 frame
xform 0 -880 -96
p 0 -1232 100 0 1 author:S.M.Beard
p 240 -1248 100 0 -1 border:C
p 0 -1264 100 0 1 checked:S.M.Beard
p 272 -1248 100 0 -1 date:18 Mar 97
p 1696 384 100 0 -1 project:Gemini Near Infra-Red OIWFS
p 240 -1184 100 0 -1 title:System Command Action Directive Records
use eapply -1472 -569 100 0 Apply
xform 0 -1280 -208
p -1408 -608 100 0 1 DESC:$(name) system APPLY record
p -1408 -640 100 0 1 PV:$(top)$(mech)
[comments]
