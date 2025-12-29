[schematic2]
uniq 93
[tools]
[detail]
w 168 323 100 0 n#91 inhier.ICID.P -288 320 672 320 672 -352 864 -352 ecars.C.ICID
w -88 -1077 100 0 n#90 inhier.BSLK.P -288 -1088 160 -1088 elongouts.BS.SLNK
w -88 -789 100 0 n#88 inhier.ERLK.P -288 -800 160 -800 elongouts.ER.SLNK
w -88 -501 100 0 n#87 inhier.PSLK.P -288 -512 160 -512 elongouts.PS.SLNK
w -88 -213 100 0 n#86 inhier.IDLK.P -288 -224 160 -224 elongouts.ID.SLNK
w -88 75 100 0 n#85 inhier.UALK.P -288 64 160 64 elongouts.UA.SLNK
w 540 -981 100 0 n#92 elongouts.BS.OUT 416 -1120 544 -1120 544 -832 junction
w 540 -693 100 0 n#92 elongouts.ER.OUT 416 -832 544 -832 544 -544 junction
w 540 -437 100 0 n#92 elongouts.PS.OUT 416 -544 544 -544 544 -320 junction
w 456 -245 100 0 n#92 elongouts.ID.OUT 416 -256 544 -256 junction
w 540 -149 100 0 n#92 elongouts.UA.OUT 416 32 544 32 544 -320 864 -320 ecars.C.IVAL
w 104 -1045 100 0 n#77 hwin.hwin#79.in 96 -1056 160 -1056 elongouts.BS.DOL
w 104 -757 100 0 n#76 hwin.hwin#74.in 96 -768 160 -768 elongouts.ER.DOL
w 104 -469 100 0 n#71 hwin.hwin#73.in 96 -480 160 -480 elongouts.PS.DOL
w 104 -181 100 0 n#70 hwin.hwin#68.in 96 -192 160 -192 elongouts.ID.DOL
w 104 107 100 0 n#66 hwin.hwin#53.in 96 96 160 96 elongouts.UA.DOL
s -192 560 200 0 (TBD)
s -96 -1008 100 0 CAR_BUSY = 2
s -96 -720 100 0 CAR_ERROR = 3
s -96 -432 100 0 CAR_PAUSED = 1
s -96 -144 100 0 CAR_IDLE = 0
s 608 -1472 500 0 carPlus.sch
s -96 144 100 0 CAR_UNAVAILABLE = 4
s -96 192 100 0 CAR_UNAVAILABLE no longer exists!!
n -1008 -1408 -656 -1056 100
This represents the contents
of a hierarchical symbol
"carPlus", which is used to
change the state of a CAR
record by activating one
of the input links.
_
[cell use]
use hwin -96 -1097 100 0 hwin#79
xform 0 0 -1056
p -93 -1064 100 0 -1 val(in):2
use hwin -96 -809 100 0 hwin#74
xform 0 0 -768
p -93 -776 100 0 -1 val(in):3
use hwin -96 -521 100 0 hwin#73
xform 0 0 -480
p -93 -488 100 0 -1 val(in):1
use hwin -96 -233 100 0 hwin#68
xform 0 0 -192
p -93 -200 100 0 -1 val(in):0
use hwin -96 55 100 0 hwin#53
xform 0 0 96
p -93 88 100 0 -1 val(in):4
use elongouts 160 -1177 100 0 BS
xform 0 288 -1088
p 224 -1216 100 0 1 EGU:CAR event
p 224 -1184 100 0 1 OMSL:supervisory
p 224 -1248 100 0 1 PV:$(top)$(car)
p 416 -1120 75 768 -1 pproc(OUT):PP
use elongouts 160 -889 100 0 ER
xform 0 288 -800
p 224 -928 100 0 1 EGU:CAR event
p 224 -896 100 0 1 OMSL:supervisory
p 224 -960 100 0 1 PV:$(top)$(car)
p 416 -832 75 768 -1 pproc(OUT):PP
use elongouts 160 -601 100 0 PS
xform 0 288 -512
p 224 -640 100 0 1 EGU:CAR event
p 224 -608 100 0 1 OMSL:supervisory
p 224 -672 100 0 1 PV:$(top)$(car)
p 416 -544 75 768 -1 pproc(OUT):PP
use elongouts 160 -313 100 0 ID
xform 0 288 -224
p 224 -352 100 0 1 EGU:CAR event
p 224 -320 100 0 1 OMSL:supervisory
p 224 -384 100 0 1 PV:$(top)$(car)
p 416 -256 75 768 -1 pproc(OUT):PP
use elongouts 160 -25 100 0 UA
xform 0 288 64
p 224 -64 100 0 1 EGU:CAR event
p 224 -32 100 0 1 OMSL:supervisory
p 224 -96 100 0 1 PV:$(top)$(car)
p 416 32 75 768 -1 pproc(OUT):PP
use inhier -304 -1129 100 0 BSLK
xform 0 -288 -1088
use inhier -304 23 100 0 UALK
xform 0 -288 64
use inhier -304 -265 100 0 IDLK
xform 0 -288 -224
use inhier -304 -553 100 0 PSLK
xform 0 -288 -512
use inhier -304 535 100 0 STAT
xform 0 -288 576
use inhier -304 -841 100 0 ERLK
xform 0 -288 -800
use inhier -304 279 100 0 ICID
xform 0 -288 320
use bc200tr -1248 -1656 -100 0 frame
xform 0 432 -352
p 1328 -1488 100 0 1 author:S.M.Beard
p 1552 -1488 100 0 -1 border:C
p 1312 -1520 100 0 1 checked:S.M.Beard
p 1584 -1488 100 0 -1 date:10 Nov 96
p 1552 -1376 100 0 -1 project:Core Instrument Control System
p 1552 -1440 100 0 -1 title:Wrap up for CAR triggered by link
use ecars 864 -601 100 0 C
xform 0 1024 -432
p 928 -608 100 0 1 DESC:$(car) CAR record
p 928 -640 100 0 1 PV:$(top)$(car)
[comments]
