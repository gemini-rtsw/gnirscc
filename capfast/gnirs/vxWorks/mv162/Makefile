# Makefile
# CREATED BY mpp
#       FROM MakeSkel
#         ON Mon May  1 19:29:51 PDT 1995
#


# MakeSkel - makefile skeleton for bsp/config/mv162
#
# Copyright 1984-1993 Wind River Systems, Inc.
#
# modification history
# --------------------
# 01c,17mar94,vin  changed RAM_LOW_ADRS to 0x20000 from 0x1000
# 01b,25jan93,ccc  changed ROM_TEXT_ADRS for on-board EPROMs.
# 01a,04jan93,ccc  written.
#
# DESCRIPTION
# This file contains rules for building VxWorks for the
# Motorola MVME162.
#
# INCLUDES
#     makeTarget
#*/

CPU              = MC68040
TOOL             = gnu

TARGET_DIR       = mv162
VENDOR           = Motorola
BOARD            = MVME162

#
# The constants ROM_TEXT_ADRS, ROM_SIZE, and RAM_HIGH_ADRS are defined
# in config.h, MakeSkel, Makefile, and Makefile.*
# All definitions for these constants must be identical.
#

ROM_TEXT_ADRS    = ff800008 # ROM entry address
ROM_SIZE         = 00040000 # number of bytes of ROM space

RAM_LOW_ADRS     = 00020000 # RAM text/data address
RAM_HIGH_ADRS    = 00100000 # RAM text/data address

HEX_FLAGS        = -v -p $(ROM_TEXT_ADRS) -a 8

MACH_EXTRA       =

# makeTarget - meta makeTarget stub for building BSPs
#
# modification history
# --------------------
# 01a,22sep92,yp    added modification history
#
# DESCRIPTION
# This file contains the makeTarget and intermediate makeTarget for VxWorks.
#*/

# makeTarget1 - makefile target skeleton for BSP Targets "pass 1"
#
# modification history
# --------------------
# 01j,15mar95,caf  added rules for mips resident roms (SPR #3856).
# 01i,30sep94,kdl  changed "make" to "$(MAKE)" to use environment variable;
#		   changed Makefile.$(CPU)$(TOOL) rule to use temporary file.
# 01h,15oct92,gae  added -I flag for preprocessing docs.
# 01g,21sep92,yp   removed -P flag from pre-processor pass for docs
# 01f,16sep92,gae  moved doc rules here from pass 2.
# 01e,11sep92,gae  made doc rules do something.
# 01d,04sep92,yp   fixed doc rule
# 01c,02sep92,yp   added documentation rule
# 01b,21jul92,rrr  added rules for save, restore, release, and clean
# 01a,17jul92,wmd  added mod history, targets vxWorks.res_rom.hex,
#		   bootrom_res, and bootrom_res.hex.
#
# DESCRIPTION
# This file contains the targets and intermediate targets for VxWorks.
#

MAN1	 = ../../man/man1/
MAN2	 = ../../man/man1/
MAKESKEL = MakeSkel
RM       = rm -f
CP       = cp -p

default:	vxWorks

vxWorks:	     Makefile.$(CPU)$(TOOL) .neverbuilt
	$(MAKE) -f Makefile.$(CPU)$(TOOL) CPU=$(CPU) TOOL=$(TOOL) vxWorks

vxWorks_rom:	     Makefile.$(CPU)$(TOOL) .neverbuilt
	$(MAKE) -f Makefile.$(CPU)$(TOOL) CPU=$(CPU) TOOL=$(TOOL) vxWorks_rom

vxWorks_rom.hex:     Makefile.$(CPU)$(TOOL) .neverbuilt
	$(MAKE) -f Makefile.$(CPU)$(TOOL) CPU=$(CPU) TOOL=$(TOOL) vxWorks_rom.hex

vxWorks.st:	     Makefile.$(CPU)$(TOOL) .neverbuilt
	$(MAKE) -f Makefile.$(CPU)$(TOOL) CPU=$(CPU) TOOL=$(TOOL) vxWorks.st

vxWorks.st_rom:	     Makefile.$(CPU)$(TOOL) .neverbuilt
	$(MAKE) -f Makefile.$(CPU)$(TOOL) CPU=$(CPU) TOOL=$(TOOL) vxWorks.st_rom

vxWorks.st_rom.hex:  Makefile.$(CPU)$(TOOL) .neverbuilt
	$(MAKE) -f Makefile.$(CPU)$(TOOL) CPU=$(CPU) TOOL=$(TOOL) vxWorks.st_rom.hex

vxWorks.res_rom:     Makefile.$(CPU)$(TOOL) .neverbuilt
	$(MAKE) -f Makefile.$(CPU)$(TOOL) CPU=$(CPU) TOOL=$(TOOL) vxWorks.res_rom

vxWorks.res_rom.hex: Makefile.$(CPU)$(TOOL) .neverbuilt
	$(MAKE) -f Makefile.$(CPU)$(TOOL) CPU=$(CPU) TOOL=$(TOOL) vxWorks.res_rom.hex

vxWorks.res_rom_mips:     Makefile.$(CPU)$(TOOL) .neverbuilt
	$(MAKE) -f Makefile.$(CPU)$(TOOL) CPU=$(CPU) TOOL=$(TOOL) vxWorks.res_rom_mips

vxWorks.res_rom_mips.hex: Makefile.$(CPU)$(TOOL) .neverbuilt
	$(MAKE) -f Makefile.$(CPU)$(TOOL) CPU=$(CPU) TOOL=$(TOOL) vxWorks.res_rom_mips.hex

bootrom:	     Makefile.$(CPU)$(TOOL) .neverbuilt
	$(MAKE) -f Makefile.$(CPU)$(TOOL) CPU=$(CPU) TOOL=$(TOOL) bootrom

bootrom.hex:	     Makefile.$(CPU)$(TOOL) .neverbuilt
	$(MAKE) -f Makefile.$(CPU)$(TOOL) CPU=$(CPU) TOOL=$(TOOL) bootrom.hex

bootrom_res:         Makefile.$(CPU)$(TOOL) .neverbuilt
	$(MAKE) -f Makefile.$(CPU)$(TOOL) CPU=$(CPU) TOOL=$(TOOL) bootrom_res

bootrom_res.hex:     Makefile.$(CPU)$(TOOL) .neverbuilt
	$(MAKE) -f Makefile.$(CPU)$(TOOL) CPU=$(CPU) TOOL=$(TOOL) bootrom_res.hex

bootrom_res_mips:         Makefile.$(CPU)$(TOOL) .neverbuilt
	$(MAKE) -f Makefile.$(CPU)$(TOOL) CPU=$(CPU) TOOL=$(TOOL) bootrom_res_mips

bootrom_res_mips.hex:     Makefile.$(CPU)$(TOOL) .neverbuilt
	$(MAKE) -f Makefile.$(CPU)$(TOOL) CPU=$(CPU) TOOL=$(TOOL) bootrom_res_mips.hex

bootrom_uncmp:	     Makefile.$(CPU)$(TOOL) .neverbuilt
	$(MAKE) -f Makefile.$(CPU)$(TOOL) CPU=$(CPU) TOOL=$(TOOL) bootrom_uncmp

bootrom_uncmp.hex:   Makefile.$(CPU)$(TOOL) .neverbuilt
	$(MAKE) -f Makefile.$(CPU)$(TOOL) CPU=$(CPU) TOOL=$(TOOL) bootrom_uncmp.hex

release:	Makefile.$(CPU)$(TOOL) .neverbuilt
	$(MAKE) -f Makefile.$(CPU)$(TOOL) CPU=$(CPU) TOOL=$(TOOL) release

clean:	Makefile.$(CPU)$(TOOL) .neverbuilt
	$(MAKE) -f Makefile.$(CPU)$(TOOL) CPU=$(CPU) TOOL=$(TOOL) clean

restore:	Makefile.$(CPU)$(TOOL) .neverbuilt
	$(MAKE) -f Makefile.$(CPU)$(TOOL) CPU=$(CPU) TOOL=$(TOOL) restore

save:	Makefile.$(CPU)$(TOOL) .neverbuilt
	$(MAKE) -f Makefile.$(CPU)$(TOOL) CPU=$(CPU) TOOL=$(TOOL) save


Makefile.$(CPU)$(TOOL):	MakeSkel $(VX_VW_BASE)/h/make/make.defaults \
    $(VX_VW_BASE)/h/make/make.$(CPU)$(TOOL) $(VX_VW_BASE)/h/make/makeTarget2 
	$(RM) $@
	$(VX_HSP_BASE)/bin/$(VX_HOST_TYPE)/mpp $(CPU)$(TOOL) > $@.tmp
	$(MAKE) -f $@.tmp CPU=$(CPU) TOOL=$(TOOL) depend.$(CPU)$(TOOL)
	$(RM) $@.tmp

.neverbuilt:

########## documentation #######################################################

man: $(MAN1)/$(TARGET_DIR).1 docs

$(MAN1)/$(TARGET_DIR).1: target.nr
	$(CP) $? $@

DOC_FILES = sysLib tyCoDrv

ENV_BIN = $(TOOLENV) $(VX_HSP_BASE)/bin/$(VX_HOST_TYPE)/
CPP=$(ENV_BIN)cpp
MG=$(ENV_BIN)mg

docs:
	@- (doc_files="$(DOC_FILES)" ; \
	echo -n "Processing " ; \
	for file in $$doc_files ; do \
		echo -n "$$file " ; \
		$(CPP) -E -C -DDOC -I../../src/drv/ $$file.c 1> $$file.i 2> /dev/null ; \
		$(MG) -p $(TARGET_DIR) $$file.i ; \
		$(RM) $$file.i ; \
	done ; \
	echo "complete." )

################################################################################

# end of makeTarget1


# end of makeTarget

