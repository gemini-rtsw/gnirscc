#
#
# this set you up for epics3-13 and t2
# Matthieu Bec - 12/20/01
#
alias addenv 'if (:${\!:1}\: !~ *:\!{:2}\:*) setenv \!:1 ${\!:1}\:\!:2'
setenv EPICS /home/gemini/epics/epics3.13.4GEM7
setenv PATH /solaris/gnu/bin
addenv PATH /solaris/gnu/gcc-2.95.3/lib/gcc-lib/sparc-sun-solaris2.5.1/2.95.3
addenv PATH /solaris/local/bin
addenv PATH /solaris/gnu/gcc/bin
addenv PATH /opt/SUNWspro/bin
addenv PATH /usr/openwin/bin
addenv PATH /usr/openwin/bin/xview
addenv PATH /opt/X11R6/bin
addenv PATH /opt/bin
addenv PATH /daikon/gemini/ppc/solaris/bin
addenv PATH /usr/bin
addenv PATH /usr/local
addenv PATH /usr/ucb
addenv PATH /usr/sbin
addenv PATH /usr/ccs/bin
addenv PATH /usr/local/bin
addenv PATH /usr/local/sbin
addenv PATH /opt/local/bin
addenv PATH /usr/local/Adobe/frame5/bin
addenv PATH /opt/wsplus
addenv PATH /home/p3/wcs/bin
addenv PATH /home/gemini/epics/epics3.13.4GEM7/extensions/bin/solaris
addenv PATH /home/gemini/epics/epics3.13.4GEM7/base/bin/solaris
addenv PATH /home/gemini/epics/epics3.13.4GEM7/base/tools
addenv PATH /daikon/gemini/tornado2.0/host/sun4-solaris2/bin
addenv PATH /mpgf/opt/local/perl5.6/bin
setenv LD_LIBRARY_PATH /solaris/gnu/lib
addenv LD_LIBRARY_PATH /solaris/gnu/gcc-2.95.3/lib/gcc-lib/sparc-sun-solaris2.5.1/2.95.3
addenv LD_LIBRARY_PATH /solaris/local/lib
addenv LD_LIBRARY_PATH /solaris/gnu/gcc/lib
addenv LD_LIBRARY_PATH /daikon/gemini/tornado2.0/host/sun4-solaris2/lib
addenv LD_LIBRARY_PATH /daikon/gemini/ppc/solaris/lib
addenv LD_LIBRARY_PATH /usr/lib
addenv LD_LIBRARY_PATH /usr/openwin/lib
addenv LD_LIBRARY_PATH /usr/lib/X11
addenv LD_LIBRARY_PATH /home/gemini/epics/epics3.13.4GEM7/base/lib/solaris
addenv LD_LIBRARY_PATH /home/gemini/epics/epics3.13.4GEM7/extensions/lib/solaris
setenv CAPDIR /home/p3/wcs/bin
setenv DCDATA_TARGET ppc604
setenv HOST_ARCH solaris
alias gmake make

#echo $NIRS_DCINSTPATH

#./nirsPPCInstall

#echo Done!


