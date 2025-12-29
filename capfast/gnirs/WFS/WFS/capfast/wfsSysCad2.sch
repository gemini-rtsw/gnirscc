[schematic2]
uniq 365
[tools]
[detail]
w -1182 -821 100 0 n#363 ecad2.verify.STLK -1216 -832 -1088 -832 cadCar.verifyCC.SLNK
w -398 -821 100 0 n#362 ecad2.endVerify.STLK -448 -832 -288 -832 cadCar.endVerifyCC.SLNK
w -356 -565 100 0 n#359 junction -352 -352 -352 -768 -288 -768 cadCar.endVerifyCC.STAT
w -1166 -1117 100 0 n#359 ecad2.endVerify.VAL -448 -352 -64 -352 -64 -1120 -2208 -1120 -2208 -96 -2112 -96 eapply.Apply2.INPB
w -388 -629 100 0 n#358 ecad2.endVerify.OCID -448 -448 -384 -448 -384 -800 -288 -800 cadCar.endVerifyCC.ICID
w -1124 -565 100 0 n#356 junction -1120 -352 -1120 -768 -1088 -768 cadCar.verifyCC.STAT
w -1598 -1181 100 0 n#356 ecad2.verify.VAL -1216 -352 -864 -352 -864 -1184 -2272 -1184 -2272 -32 -2112 -32 eapply.Apply2.INPA
w -1156 -629 100 0 n#355 ecad2.verify.OCID -1216 -448 -1152 -448 -1152 -800 -1088 -800 cadCar.verifyCC.ICID
w -1166 -1085 100 0 n#335 ecad2.endVerify.MESS -448 -384 -96 -384 -96 -1088 -2176 -1088 -2176 -128 -2112 -128 eapply.Apply2.INMB
w -1598 -1149 100 0 n#333 ecad2.verify.MESS -1216 -384 -896 -384 -896 -1152 -2240 -1152 -2240 -64 -2112 -64 eapply.Apply2.INMA
w -1310 -125 100 0 n#329 eapply.Apply2.OCLB -1728 -128 -832 -128 -832 -384 -768 -384 ecad2.endVerify.ICID
w -1294 -93 100 0 n#328 eapply.Apply2.OUTB -1728 -96 -800 -96 -800 -352 -768 -352 ecad2.endVerify.DIR
w -1604 -229 100 0 n#327 eapply.Apply2.OCLA -1728 -64 -1600 -64 -1600 -384 -1536 -384 ecad2.verify.ICID
w -1572 -197 100 0 n#326 eapply.Apply2.OUTA -1728 -32 -1568 -32 -1568 -352 -1536 -352 ecad2.verify.DIR
w -1678 35 100 0 n#270 eapply.Apply2.MESS -1728 32 -1568 32 -1568 0 -1504 0 outhier.MESS.p
w -1646 67 100 0 n#269 eapply.Apply2.VAL -1728 64 -1504 64 outhier.VAL.p
w -2270 35 100 0 n#246 inhier.CLID.P -2400 0 -2368 0 -2368 32 -2112 32 eapply.Apply2.CLID
w -2286 75 100 0 n#245 inhier.DIR.P -2400 64 -2112 64 eapply.Apply2.DIR
s -16 -944 100 0 associated CAR records BUSY then IDLE.
s -16 -912 100 0 NOTE: The verify commands are ignored and simply set their
s -624 -432 100 0 (B)
s -1392 -432 100 0 (A)
s -80 -1232 500 512 wfsSysCad2.sch
s -2112 288 100 0 This "apply" record sequences the
s -2112 240 100 0 CAD records in the order
s -2112 192 100 0 A, B, C, D
n 160 -288 640 64 100
This schematic contains the
following systemwide Command
Action Directive (CAD) records
for the NIRI:
.
verify     - Begin verifying.
.
endVerify  - End verifying.
.
.
.
.
.
.
_
[cell use]
use cadCar -288 -921 100 0 endVerifyCC
xform 0 -208 -816
p -268 -948 100 0 1 set:cad endVerify
use cadCar -1088 -921 100 0 verifyCC
xform 0 -1008 -816
p -1068 -948 100 0 1 set:cad verify
use ecad2 -752 -928 100 0 endVerify
xform 0 -608 -608
p -688 -960 100 0 1 DESC:End verifying
p -688 -1024 100 0 1 PV:$(top)
p -688 -992 100 0 1 SNAM:
use ecad2 -1520 -928 100 0 verify
xform 0 -1376 -608
p -1456 -960 100 0 1 DESC:Begin verifying
p -1456 -1024 100 0 1 PV:$(top)
p -1456 -992 100 0 1 SNAM:
use outhier -1536 23 100 0 VAL
xform 0 -1520 64
use outhier -1536 -41 100 0 MESS
xform 0 -1520 0
use inhier -2416 23 100 0 DIR
xform 0 -2400 64
use inhier -2416 -41 100 0 CLID
xform 0 -2400 0
use bc200tr -2560 -1400 -100 0 frame
xform 0 -880 -96
p 0 -1232 100 0 1 author:S.M.Beard
p 240 -1248 100 0 -1 border:C
p 0 -1264 100 0 1 checked:S.M.Beard
p 272 -1248 100 0 -1 date:30 May 97
p 524 -1244 100 1792 -1 page:1
p 1696 384 100 0 -1 project:Gemini Near Infra-Red OIWFS
p 240 -1184 100 0 -1 title:NIRI WFS System CAD Records - 2
use eapply -2112 -569 100 0 Apply2
xform 0 -1920 -208
p -2048 -608 100 0 1 DESC:$(name) wfsSysCad2 APPLY record
p -2048 -640 100 0 1 PV:$(top)$(mech)
[comments]
