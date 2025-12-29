[schematic2]
uniq 13
[tools]
[detail]
w 124 -533 100 0 n#12 ebos.abortFlag.VAL -160 -992 128 -992 128 -64 480 -64 ecalcs.ecalcs#6.INPF
w 60 -405 100 0 n#11 ebos.sdtFlag.VAL -160 -768 64 -768 64 -32 480 -32 ecalcs.ecalcs#6.INPE
w -4 -277 100 0 n#10 ebos.simFlag.VAL -160 -544 0 -544 0 0 480 0 ecalcs.ecalcs#6.INPD
w 190 35 100 0 n#9 ebos.readFlag.VAL -160 -320 -64 -320 -64 32 480 32 ecalcs.ecalcs#6.INPC
w 158 67 100 0 n#8 ebos.dieFlag.VAL -160 -96 -128 -96 -128 64 480 64 ecalcs.ecalcs#6.INPB
w 158 99 100 0 n#7 ebos.idleFlag.VAL -160 128 -128 128 -128 96 480 96 ecalcs.ecalcs#6.INPA
[cell use]
use ecalcs 480 -377 100 0 ecalcs#6
xform 0 624 -112
use ebos -344 -920 100 0 abortFlag
xform 0 -288 -992
p -736 -1138 100 0 0 ONAM:ON
p -736 -1106 100 0 0 ZNAM:OFF
use ebos -344 -696 100 0 sdtFlag
xform 0 -288 -768
p -736 -914 100 0 0 ONAM:ON
p -736 -882 100 0 0 ZNAM:OFF
use ebos -344 -472 100 0 simFlag
xform 0 -288 -544
p -736 -690 100 0 0 ONAM:ON
p -736 -658 100 0 0 ZNAM:OFF
use ebos -344 -248 100 0 readFlag
xform 0 -288 -320
p -736 -466 100 0 0 ONAM:ON
p -736 -434 100 0 0 ZNAM:OFF
use ebos -344 -24 100 0 dieFlag
xform 0 -288 -96
p -736 -242 100 0 0 ONAM:ON
p -736 -210 100 0 0 ZNAM:OFF
use ebos -352 200 100 0 idleFlag
xform 0 -288 128
p -736 -18 100 0 0 ONAM:ON
p -736 14 100 0 0 ZNAM:OFF
[comments]
