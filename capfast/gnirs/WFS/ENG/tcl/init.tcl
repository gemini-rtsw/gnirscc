#! /bin/echo init.tcl should not be run directly from the command line

# $Id: init.tcl,v 1.2 2009/05/27 19:34:52 fkraemer Exp $

bind Entry <Return> { }
bind Entry <Key-Delete> [bind Entry <Key-BackSpace>]

#
# Default directory for parameters 
#

set const(pvDir) $env(NIRI_ENGDIR)/pv
set const(lutDir) $env(NIRI_ENGDIR)/pv
set const(cfgFile) $env(NIRI_ENGDIR)/pv/cfg.tcl

#
# Bit flags for various scanning modes
#

set const(dir,forward) 0x1
set const(dir,reverse) 0x2
