# $Id: Makefile,v 1.2 2009/05/27 19:33:56 fkraemer Exp $

include .applTop/config/CONFIG

APPLIC_DIR_TYPE = sys

include .applTop/config/RULES.Dirs

PROD =	$(notdir $(shell pwd))
TAR =	/usr/bin/tar
COMPRESS = gzip

release:
	$(RM) $(PROD).tar $(PROD).tar.Z .xfile
	find * \
	    \( -name bin -o -name config -o -name db -o -name epics \
		-o -name data -o -name include \
		-o -name 'O.*' -o -name '*.dctsdr' -o -name '*.sdrSum' \
		-o -name .applTop \
		-o -name .epics -o -name .version \
		-o -name '*%' -o -name '*~' -o -name '*.Z' -o -name '*.gz' \
	    \) -prune -print > .xfile
	echo '.xfile' >> .xfile
	echo 'capfast/cad.rc' >> .xfile
	echo 'startup/local.vws' >> .xfile
	echo 'startup/resource.def' >> .xfile
	$(TAR) cvXf .xfile $(PROD).tar * .??*
	$(COMPRESS) $(PROD).tar
	$(RM) .xfile

reset:
	find . -type d -name "O.*" | xargs $(RM) -r
	$(RM) -r bin data 
#include

resetall: reset
	capfast/rmTop capfast/*Set.sch
	$(RM) config.par
	$(RM) startup/UAE.dist* startup/local.vws startup/resource.def
	$(RM) -r config epics
	find . -type l | xargs $(RM)
