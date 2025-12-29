#
# These aliases are required before running this set-up file
#

alias addenv 'if (:${\!:1}\: !~ *:\!{:2}\:*) setenv \!:1 ${\!:1}\:\!:2'

# Which OS are we using?

setenv HOST_ARCH solaris
setenv TARGET_ARCH 68k

#
# Important paths to:
# VxWorks 5.2
# Capfast
# EPICS epics3.12.2GEM5
#
setenv VX_DIR          /mpgv/vxworks/v5.2
setenv CAPDIR          /home/p3/wcs/bin
setenv EPICS           /home/gemini/epics/epics3.12.2GEM5

#
# Setup for VxWorks Release 5.2
#
addenv PATH            ${VX_DIR}/bin/${HOST_ARCH}
addenv PATH            ${VX_DIR}/gnu/${HOST_ARCH}.${TARGET_ARCH}/bin
addenv MANPATH         ${VX_DIR}/man
addenv MANPATH         ${VX_DIR}/gnu/${HOST_ARCH}.${TARGET_ARCH}/man
setenv VX_HOST_TYPE    ${HOST_ARCH}
setenv VX_HSP_BASE     ${VX_DIR}
setenv VX_BSP_BASE     ${VX_DIR}
setenv VX_VW_BASE      ${VX_DIR}
setenv VX_CPU_FAMILY   ${TARGET_ARCH}
setenv GCC_EXEC_PREFIX ${VX_DIR}/gnu/${HOST_ARCH}.${TARGET_ARCH}/lib/gcc-lib/

#
# Setup for Capfast
#
if (-d ${CAPDIR}) then
    addenv PATH            ${CAPDIR}
endif

addenv PATH            ${EPICS}/extensions/src/edif/lib/sym
#
# Setup for epics3.12.2GEM5
#
addenv PATH            ${EPICS}/extensions/bin/${HOST_ARCH}
addenv PATH            ${EPICS}/base/bin/${HOST_ARCH}
addenv PATH            ${EPICS}/base/tools

#
# Setup for using Tcl/Tk Interface to Channel Access
#
setenv TCL_LIBRARY     ${EPICS}/extensions/src/tcllib/lib/tcl
setenv TK_LIBRARY      ${EPICS}/extensions/src/tcllib/lib/tk
setenv DP_LIBRARY      ${EPICS}/extensions/src/tcllib/lib/dp

addenv PATH /home/afcom/afcon-1.0
