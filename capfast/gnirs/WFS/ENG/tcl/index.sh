#! /bin/sh

src='tmp.tcl scan.tcl cycle.tcl features.tcl ca.tcl util.tcl help.tcl pos.tcl'
echo "auto_mkindex . $src; exit" | et_wish
