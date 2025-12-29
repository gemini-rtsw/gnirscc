# Makefile,v 1.3 1995/09/02 01:45:07 epics Exp
#

include .applTop/config/CONFIG

APPLIC_DIR_TYPE = sys

include .applTop/config/RULES.Dirs

tags:	FORCE
	ctags -t /repos2/NAACsrc/tp/common/*.[c] /repos2/NAACsrc/tp/icon/*.[c]
	sed -e "/^SEND/d" $@ >temp
	mv temp $@
	xtags /repos2/NAACsrc/tp/icon/config_st.h >>$@
	cat tags.send >>tags
	sort -o tags tags
FORCE:
