[schematic2]
uniq 489
[tools]
[detail]
w 1090 604 100 768 n#485 elongouts.StopOp.FLNK 896 608 1344 608 1344 992 junction
w 1154 988 100 768 n#485 elongouts.Id.SLNK 1472 992 896 992 elongouts.Op.FLNK
w 866 548 100 768 n#480 elongouts.StopOp.OUT 896 544 896 544 hwout.hwout#483.outp
w 610 604 100 768 n#479 elongouts.StopOp.DOL 640 608 640 608 hwin.hwin#482.in
w 466 956 -100 768 c#424 inhier.ISTL.P 352 960 640 960 elongouts.Op.SLNK
w 1698 1028 100 768 n#476 hwout.hwout#456.outp 1728 1024 1728 1024 elongouts.Id.FLNK
w 466 572 -100 768 c#468 elongouts.StopOp.SLNK 640 576 352 576 inhier.ISPL.P
w 1378 1027 -100 0 OCID inhier.ICID.P 1344 1024 1472 1024 elongouts.Id.DOL
w 866 932 100 768 n#451 hwout.hwout#450.outp 896 928 896 928 elongouts.Op.OUT
w 1698 964 100 768 n#445 hwout.hwout#444.outp 1728 960 1728 960 elongouts.Id.OUT
w 82 1276 -100 768 c#422 bihier.ARG.p 32 1280 192 1280 hwout.hwout#460.outp
w 610 988 100 768 n#406 hwin.hwin#393.in 640 992 640 992 elongouts.Op.DOL
s 1472 1200 100 0 The target records are normally located in compSnlArg.
s 928 1440 100 0 The client ID is passed on to make it easy to execute multiple commands.
s 256 1520 100 0 the axis to insure that record names are unique.
s 256 1552 100 0 that the command name is combined with
s 256 1584 100 0 Note: the command name is irrelevant except
s 256 576 100 2048 Stop Link
s 2032 96 400 1280 compSnlCmd
s 1472 672 100 0 It does no parameter checking.
s 1472 704 100 0 This record is not intended to be written to directly.
s 1472 736 100 0 The state notation code monitors the counter.
s 1376 1056 100 2048 Client ID
s 256 960 100 2048 Start Link
s 928 1408 100 0 The same is true for the start link and the stop link.
s 928 1376 100 0 The argument is NOT passed on, because it is often command specific.
n 2240 320 2720 672 100
This schematic contains the records
for passing a command to SNL code.
A client ID, an operation code, and
an optional (op-code specific) argument
are loaded into the appropriate record.
When the start link is triggered, a
counter is incremented, which causes
the SNL code to read the arguments and
to start processing.
.
Note that there is a possible race
condition here, which must be avoided
by never having two pending operations
being written simultaneously.
_
[cell use]
use hwout 192 1239 100 768 hwout#460
xform 0 288 1280
p 288 1271 100 0 -1 val(outp):$(top)$(mech)SnlArg PP NMS
use hwout 1728 983 100 768 hwout#456
xform 0 1824 1024
p 1824 1015 100 0 -1 val(outp):$(top)$(mech)SnlExec
use hwout 896 887 100 768 hwout#450
xform 0 992 928
p 992 919 100 0 -1 val(outp):$(top)$(mech)SnlOp PP NMS
use hwout 1728 919 100 768 hwout#444
xform 0 1824 960
p 1824 951 100 0 -1 val(outp):$(top)$(mech)SnlId PP NMS
use hwout 896 503 100 768 hwout#483
xform 0 992 544
p 992 535 100 0 -1 val(outp):$(top)$(mech)SnlOp PP NMS
use bihier 0 1280 100 2048 ARG
xform 0 32 1280
use hwin 448 951 100 768 hwin#393
xform 0 544 992
p 451 984 100 0 -1 val(in):$(op)
use hwin 448 567 100 768 hwin#482
xform 0 544 608
p 451 600 100 0 -1 val(in):$(opStop)
use elongouts 704 1024 100 768 Op
xform 0 768 960
p 704 832 100 768 1 DESC:$(mech) $(cmd) Op
p 704 864 100 768 1 PV:$(top)$(mech)$(cmd)
p 896 928 75 768 -1 pproc(OUT):PP
use elongouts 1536 1056 100 768 Id
xform 0 1600 992
p 1536 832 100 768 1 DESC:$(mech) $(cmd) ID
p 1536 864 100 768 1 OMSL:closed_loop
p 1536 896 100 768 1 PV:$(top)$(mech)$(cmd)
p 1728 960 75 768 -1 pproc(OUT):PP
use elongouts 704 640 100 768 StopOp
xform 0 768 576
p 704 448 100 768 1 DESC:$(mech) $(cmd) Op
p 704 480 100 768 1 PV:$(top)$(mech)$(cmd)
p 896 544 75 768 -1 pproc(OUT):PP
use inhier 320 576 100 2048 ISPL
xform 0 352 576
use inhier 1312 1024 100 2048 ICID
xform 0 1344 1024
use inhier 320 960 100 2048 ISTL
xform 0 352 960
use bc200tr -512 -24 -100 0 frame
xform 0 1168 1280
p 2064 144 100 0 1 author:H.T.Yamada
p 2288 128 100 0 -1 border:C
p 2048 112 100 0 1 checked:H.T.Yamada
p 2320 128 100 0 -1 date:1998-05-06
p -512 -24 100 0 0 id:frame
p 2288 256 100 0 -1 project:Core Instrument Control System
p 2288 192 100 0 -1 title:Move CAD records for discrete axis
[comments]
