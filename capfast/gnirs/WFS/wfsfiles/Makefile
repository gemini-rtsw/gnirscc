# Makefile,v 1.3 1995/09/02 01:44:43 epics Exp
#

include .applTop/config/CONFIG

APPLIC_DIR_TYPE = sys

include .applTop/config/RULES.Dirs

PROD  = $(notdir $(shell pwd))

release:
	$(RM) $(PROD).tar $(PROD).tar.Z .xfile
	find * \
	    \( -name bin -o -name config -o -name db -o -name epics \
	        -o -name .epics -o -name .version \
	        -o -name 'O.*' -o -name '*.dctsdr' \
		-o -name '*.sdrSum' -o -name .applTop -o -name .cvsignore \
	        -o -name '*%' -o -name '*~' -o -name '*.Z' -o -name '*.gz' \
	    \) -prune -print > .xfile
	find 'include/rec' -name '*.h' -prune -print >> .xfile
	find 'data' -name '*.db' -prune -print >> .xfile
	find 'data' -name '*.dl' -prune -print >> .xfile
	echo 'capfast/cad.rc' >> .xfile
	echo 'dl/template.adl' >> .xfile
	echo 'dl/colors.adl' >> .xfile
	echo 'startup/local.vws' >> .xfile
	echo 'startup/resource.def' >> .xfile
	/usr/bin/tar cvXf .xfile $(PROD).tar *
	/usr/local/bin/gzip $(PROD).tar
	$(RM) .xfile

reset:
	find . -type d -name "O.*" | xargs $(RM) -r
	$(RM) -r bin data include

resetall: reset
	capfast/rmTop capfast/*Set.sch
	$(RM) -r config epics
	$(RM) config.par
	$(RM) startup/UAE.dist* startup/local.vws startup/resource.def
	find . -type l | xargs $(RM)
