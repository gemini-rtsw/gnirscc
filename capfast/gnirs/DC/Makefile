# Makefile,v 1.3 1995/09/02 01:44:43 epics Exp
#
# Modified to exclude certain files from CC, WFS, ENG, LIB and GNAAC on
# "gmake release".
#
# Also modified to keep "colors.adl" and "template.adl".
#
# 27 May 1997: NIRS version.
#

include .applTop/config/CONFIG

APPLIC_DIR_TYPE = sys

include .applTop/config/RULES.Dirs

#PROD  = $(notdir $(shell pwd))
PROD = GNIRS2.0
TAR =	/usr/bin/tar
COMPRESS = gzip

release:
	$(RM) $(PROD).tar $(PROD).tar.gz .xfile
	find * \
	    \( -type l \
			-o -name config \
			-o -name data \
			-o -name temp \
			-o -name tar \
			-o -name db \
			-o -name core \
			-o -name epics \
	        -o -name .epics \
			-o -name .version \
			-o -name config.par \
			-o -name bin -o -name '*db' \
			-o -name 'data/*.dl' \
			-o -name 'data/*lut' \
			-o -name 'data/*pv' \
			-o -name 'UAE.dist*' \
			-o -name 'nirsLogin' \
			-o -name resource.def \
			-o -name local.vws \
			-o -name cad.rc \
			-o -name 'old*'\
	        -o -name 'O.*' \
			-o -name '*.dctsdr' \
			-o -name '#*' \
	        -o -name '*.sdrSum' \
			-o -name .applTop \
			-o -name '...*'\
	        -o -name '*%' \
			-o -name '*~' \
			-o -name '*.Z' \
			-o -name '*.gz' \
	    \) -prune -print > .xfile
#	echo 'capfast/nirsSadTop.sch' >> .xfile
#	echo 'capfast/nirsTop.sch' >> .xfile
#	echo 'CC/capfast/nirsCcSadTop.sch' >> .xfile
#	echo 'CC/capfast/nirsCcTop.sch' >> .xfile
#	find 'CC/capfast' -name '*.ps' -prune -print >> .xfile
#	echo 'WFS/capfast/nirsWfsSadTop.sch' >> .xfile
#	echo 'WFS/capfast/nirsWfsTop.sch' >> .xfile
#	find 'WFS/capfast' -name '*.ps' -prune -print >> .xfile
#	find 'data' -name '*.dl' -prune -print >> .xfile
#	find 'CC/data' -name '*.dl' -prune -print >> .xfile
#	find 'DC/data' -name '*.dl' -prune -print >> .xfile
#	find 'data' -name '*.dl' -prune -print >> .xfile
#	find 'CC/data' -name '*.dl' -prune -print >> .xfile
#	find 'DC/data' -name '*.dl' -prune -print >> .xfile
#	echo 'IS/capfast/nirsSadTop.sch' >> .xfile
#	echo 'IS/capfast/nirsTop.sch' >> .xfile
#	find 'IS/capfast' -name '*.ps' -prune -print >> .xfile
#	find 'WFS/ENG/capfast' -name '*.ps' -prune -print >> .xfile
#	echo 'WFS/ENG/capfast/atodTop.sch' >> .xfile
#	echo 'WFS/ENG/capfast/ccSimTop.sch' >> .xfile
#	echo 'WFS/ENG/capfast/ccTop.sch' >> .xfile
#	echo 'WFS/ENG/capfast/dioTop.sch' >> .xfile
#	echo 'WFS/ENG/capfast/engTop.sch' >> .xfile
#	echo 'WFS/ENG/capfast/tmpSimTop.sch' >> .xfile
#	echo 'WFS/ENG/capfast/tmpTop.sch' >> .xfile
#	echo 'WFS/ENG/capfast/wfsSimTop.sch' >> .xfile
#	echo 'WFS/ENG/capfast/wfsTop.sch' >> .xfile
	$(TAR) cvXf .xfile $(PROD).tar * .cvsignore
	$(COMPRESS) $(PROD).tar
	$(RM) .xfile

reset:
	find . -type d -name "O.*" | xargs $(RM) -r
	$(RM) -r bin data 

resetall: reset
	$(RM) config.par
	$(RM) capfast/cad.rc
	$(RM) startup/UAE.dist startup/local.vws startup/resource.def
	$(RM) -r config epics
	find . -type l | xargs $(RM) 
