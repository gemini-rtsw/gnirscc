[schematic2]
uniq 417
[tools]
[detail]
w -2014 740 100 768 n#409 ecalcs.SnlExec.VAL -1824 480 -1792 480 -1792 736 -2176 736 -2176 672 -2112 672 ecalcs.SnlExec.INPA
s -1504 416 100 768 The records are normally triggered by compSnlCmd.
s -16 -1264 400 1280 compSnlArg
s -1504 672 100 768 This schematic is used to trigger the SNL code which
s -1504 640 100 768 carries out most mechanism actions.
s -1504 576 100 768 An operation code is put in SnlOp, a client ID is put in SnlID
s -1504 544 100 768 and an optional (operation dependant) argument is placed
s -1504 512 100 768 in SnlArg.  When SnlExec is triggerd, it VAL field will increment
s -1504 480 100 768 which triggers the SNL code, which reads the SnlOp, SnlId, and SnlArg fields.
[cell use]
use estringins -2048 -384 100 768 SnlArg
xform 0 -1984 -432
p -2048 -544 100 768 1 DESC:$(desc) SNL Argument
p -2048 -512 100 768 1 PV:$(top)$(mech)
use elongins -2048 64 100 768 SnlOp
xform 0 -1984 16
p -2048 -96 100 768 1 DESC:$(desc) SNL operation code
p -2048 -64 100 768 1 PV:$(top)$(mech)
use elongins -2048 -160 100 768 SnlID
xform 0 -1984 -208
p -2048 -320 100 768 1 DESC:$(desc) SNL operation code
p -2048 -288 100 768 1 PV:$(top)$(mech)
use ecalcs -2048 704 100 768 SnlExec
xform 0 -1968 464
p -2048 160 100 768 1 CALC:A+1
p -2048 128 100 768 1 DESC:$(desc) SNL start
p -2048 192 100 768 1 PV:$(top)$(mech)
use bc200tr -2560 -1400 -100 0 frame
xform 0 -880 -96
p 0 -1232 100 0 1 author:H.T.Yamada
p 240 -1248 100 0 -1 border:C
p 0 -1264 100 0 1 checked:H.T.Yamada
p 272 -1248 100 0 -1 date:1999-10-04
p 524 -1244 100 1792 -1 page:1
p 240 -1120 100 0 -1 project:Near Infra-Red Imager
p -2560 -1352 100 0 0 revision:1.0
p 240 -1184 100 0 -1 title:SNL arguments
[comments]
