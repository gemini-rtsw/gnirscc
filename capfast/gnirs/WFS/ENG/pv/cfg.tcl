# $Id: cfg.tcl,v 1.3 2010/04/14 01:52:45 mrippa Exp $

#
# This file provides a map from position names into position coordinates,
# which is used to generate the lookup tables (using the position tool
# in the NIRI engineering user interface).  The columns are:
#
# 1)  Name of position (any combination of alphanumeric characters)
# 2)  The absolute position (in motor microsteps)
# 3)  The position tolerance (output to generated table)
# 4)  The voltage tolerance (output to generated table) 
#
# Note:  The voltage tolerance for the home position for the wheels is
# larger than the voltage tolerance other positions, because the home
# position has no detente.
#
# Note: If this file is changed, it may be necessary to change
# ${NIRI}/CC/pv/*.pv; see the notes in that directory.
#

puts stdout "cfg.tcl"

set const(defs,ccSplt) {
	{pos00    4580 25 0.150}
	{pos01  161315 25 0.150}
	{pos02  318049 25 0.150}
	{pos03  474784 25 0.150}
	{home        0  0 0.300}
	{park     4580  0 0.150}
}

set const(defs,ccSimSplt) $const(defs,ccSplt)

#
# The range of travel is deliberately set a little large, so that the
# mechanism will be forced into a stop.
#

set const(defs,ccPuvw) {
	{pos00       0  0 0.500}
	{pos01   -3150  0 0.500}
	{home        0  0 0.500}
	{park        0  0 0.500}
}

set const(defs,ccSimPuvw) $const(defs,ccPuvw)

set const(defs,ccFopl) {
	{pos00    2300 25 0.150}
	{pos01   40014 25 0.150}
	{pos02   77729 25 0.150}
	{pos03  115443 25 0.150}
	{pos04  153157 25 0.150}
	{pos05  190871 25 0.150}
	{pos06  228586 25 0.150}
	{pos07  266300 25 0.150}
	{pos08  304014 25 0.150}
	{pos09  341728 25 0.150}
	{pos10  379443 25 0.150}
	{pos11  417157 25 0.150}
	{home        0  0 0.300}
	{park     2300 25 0.150}
}

set const(defs,ccSimFopl) $const(defs,ccFopl)

set const(defs,ccFilt1) {
	{pos00   13250 25 0.250}
	{pos01   37250 25 0.250}
	{pos02   61250 25 0.250}
	{pos03   85250 25 0.250}
	{pos04  109250 25 0.250}
	{pos05  133250 25 0.250}
	{pos06  157250 25 0.250}
	{pos07  181250 25 0.250}
	{pos08  205250 25 0.250}
	{pos09  229250 25 0.250}
	{pos10  253250 25 0.250}
	{pos11  277250 25 0.250}
	{home        0  0 0.500}
	{park   205250  0 0.250}
}

set const(defs,ccSimFilt1) $const(defs,ccFilt1)

set const(defs,ccFilt2) {
	{pos00   13250 25 0.250}
	{pos01   37250 25 0.250}
	{pos02   61250 25 0.250}
	{pos03   85250 25 0.250}
	{pos04  109250 25 0.250}
	{pos05  133250 25 0.250}
	{pos06  157250 25 0.250}
	{pos07  181250 25 0.250}
	{pos08  205250 25 0.250}
	{pos09  229250 25 0.250}
	{pos10  253250 25 0.250}
	{pos11  277250 25 0.250}
	{home        0  0 0.500}
	{park    13250  0 0.250}
}

set const(defs,ccSimFilt2) $const(defs,ccFilt2)

set const(defs,ccFilt3) {
	{pos00   13250 25 0.250}
	{pos01   37250 25 0.250}
	{pos02   61250 25 0.250}
	{pos03   85250 25 0.250}
	{pos04  109250 25 0.250}
	{pos05  133250 25 0.250}
	{pos06  157250 25 0.250}
	{pos07  181250 25 0.250}
	{pos08  205250 25 0.250}
	{pos09  229250 25 0.250}
	{pos10  253250 25 0.250}
	{pos11  277250 25 0.250}
	{home        0  0 0.500}
	{park   205250  0 0.250}
}

set const(defs,ccSimFilt3) $const(defs,ccFilt3)

set const(defs,ccSter1) {
	{pos00    6250 25 0.250}
	{pos01   16536 25 0.250}
	{pos02   26822 25 0.250}
	{pos03   37107 25 0.250}
	{home        0  0 0.500}
	{park    37107  0 0.250}
}

set const(defs,ccSimSter1) $const(defs,ccSter1)

set const(defs,ccSter2) {
	{pos00    6250 25 0.250}
	{pos01   16536 25 0.250}
	{pos02   26822 25 0.250}
	{pos03   37107 25 0.250}
	{home        0  0 0.500}
	{park    37107  0 0.250}
}

set const(defs,ccSimSter2) $const(defs,ccSter2)

# pos01 and pos02 must match LOPR and HOPR in cc.pv
# pos00 = center
# pos01 = minimum
# pos02 = maximum

set const(defs,ccFoc) {
	{pos00       0  0 0.150}
	{pos01  -30000 25 0.150}
	{pos02    2500 25 0.150}
	{home        0  0 0.150}
	{park        0  0 0.150}
}

set const(defs,ccSimFoc) $const(defs,ccFoc)

#
# The window cover
#
# The "sensor" value is actually a +2 if the mechanism is in the fully
# closed position, and a +1 if the mechanism is in the fully open
# position.  The value is exact, and should include no error.
#

set const(defs,ccCov) {
	{pos00       0  500 0.001}
	{pos01    2100  0   0.001}
	{home        0  500 0.001}
	{park        0  500 0.001}
}

set const(defs,ccSimCov) $const(defs,ccCov)

set const(defs,wfsFilt) {
	{pos00  110357 25 0.300}
	{pos01  100929 25 0.300}
	{pos02	 91500 25 0.300}
	{pos03	 82072 25 0.300}
	{pos04	 72643 25 0.300}
	{pos05	 63215 25 0.300}
	{pos06	 53786 25 0.300}
	{pos07	 44357 25 0.300}
	{pos08	 34929 25 0.300}
	{pos09	 25500 25 0.300}
	{pos10	 16072 25 0.300}
	{pos11	  6643 25 0.300}
	{home        0  0 0.300}
	{park 	110357  0 0.300}
}

set const(defs,wfsSimFilt) $const(defs,wfsFilt)

# pos01 and pos02 must match LOPR and HOPR in wfs.pv
# pos00 = center
# pos01 = minimum
# pos02 = maximum

set const(defs,wfsFoc) {
	{pos00    1500  0 0.300}
	{pos01   -4100 25 0.300}
	{pos02    3300 25 0.300}
	{home        0  0 0.300}
	{park     1500  0 0.300}
}

set const(defs,wfsSimFoc) $const(defs,wfsFoc)

# pos01 and pos02 must match LOPR and HOPR in wfs.pv
# pos00 = center
# pos01 = minimum
# pos02 = maximum

set const(defs,wfsPrbx) {
	{pos00       0  0 0.300}
	{pos01 -150000 25 0.300}
	{pos02  150000 25 0.300}
	{home        0  0 0.300}
	{park        0  0 0.300}
}

set const(defs,wfsSimPrbx) $const(defs,wfsPrbx)

# pos01 and pos02 must match LOPR and HOPR in wfs.pv
# pos00 = center
# pos01 = minimum
# pos02 = maximum

set const(defs,wfsPrby) {
	{pos00       0  0 0.300}
	{pos01 -150000 25 0.300}
	{pos02  150000 25 0.300}
	{home        0  0 0.300}
	{park        0  0 0.300}
}

set const(defs,wfsSimPrby) $const(defs,wfsPrby)
