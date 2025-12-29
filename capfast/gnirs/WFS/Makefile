# Makefile,v 1.3 1995/09/02 01:44:43 epics Exp
#
# Modified to exclude certain files from CC, WFS, ENG, LIB and GNAAC on
# "gmake release".
#
# Also modified to keep "colors.adl" and "template.adl".
#
# 27 May 1997: NIRI version.
#

include .applTop/config/CONFIG

APPLIC_DIR_TYPE = sys

include .applTop/config/RULES.Dirs

PROD  = $(notdir $(shell pwd))
TAR =	/usr/bin/tar
COMPRESS = gzip

release:
	$(RM) $(PROD).tar $(PROD).tar.gz .xfile
	find * \
	    \( -type l -o -name config -o -name db -o -name epics \
	        -o -name .epics -o -name .version -o -name config.par \
			-o -name bin -o -name data -o -name include \
			-o -name 'UAE.dist*' -o -name resource.def -o -name local.vws \
			-o -name cad.rc -o -name niriLogin \
	        -o -name 'O.*' -o -name '*.dctsdr' \
	        -o -name '*.sdrSum' -o -name .applTop \
	        -o -name '*%' -o -name '*~' -o -name '*.Z' -o -name '*.gz' \
	    \) -prune -print > .xfile
	echo 'capfast/niriSadTop.sch' >> .xfile
	echo 'capfast/niriTop.sch' >> .xfile
	echo 'CC/capfast/niriCcSadTop.sch' >> .xfile
	echo 'CC/capfast/niriCcTop.sch' >> .xfile
	find 'CC/capfast' -name '*.ps' -prune -print >> .xfile
	echo 'WFS/capfast/niriWfsSadTop.sch' >> .xfile
	echo 'WFS/capfast/niriWfsTop.sch' >> .xfile
	find 'WFS/capfast' -name '*.ps' -prune -print >> .xfile
	echo 'IS/capfast/niriSadTop.sch' >> .xfile
	echo 'IS/capfast/niriTop.sch' >> .xfile
	find 'IS/capfast' -name '*.ps' -prune -print >> .xfile
	find 'ENG/capfast' -name '*.ps' -prune -print >> .xfile
	echo 'ENG/capfast/atodTop.sch' >> .xfile
	echo 'ENG/capfast/ccSimTop.sch' >> .xfile
	echo 'ENG/capfast/ccTop.sch' >> .xfile
	echo 'ENG/capfast/dioTop.sch' >> .xfile
	echo 'ENG/capfast/engTop.sch' >> .xfile
	echo 'ENG/capfast/tmpSimTop.sch' >> .xfile
	echo 'ENG/capfast/tmpTop.sch' >> .xfile
	echo 'ENG/capfast/wfsSimTop.sch' >> .xfile
	echo 'ENG/capfast/wfsTop.sch' >> .xfile
	$(TAR) cvXf .xfile $(PROD).tar * .cvsignore
	$(COMPRESS) $(PROD).tar
	$(RM) .xfile

reset:
	cd ENG; $(MAKE) reset
	cd LIB; $(MAKE) reset
	cd CC; $(MAKE) reset
	cd WFS; $(MAKE) reset
	cd DC; $(MAKE) reset
	cd IS; $(MAKE) reset
	find . -type d -name "O.*" | xargs $(RM) -r
	$(RM) -r bin data 
#include

resetall: reset
	cd ENG; $(MAKE) resetall
	cd LIB; $(MAKE) resetall
	cd CC; $(MAKE) resetall
	cd WFS; $(MAKE) resetall
	cd DC; $(MAKE) resetall
	cd IS; $(MAKE) resetall
	$(RM) config.par
	$(RM) capfast/cad.rc
	$(RM) startup/UAE.dist startup/local.vws startup/resource.def
	$(RM) -r config epics
	find . -type l | xargs $(RM) 
