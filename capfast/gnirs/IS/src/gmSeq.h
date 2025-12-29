/* *INDENT-OFF* */
/*
*   FILENAME
*   gmSeq.h
* 
*   PURPOSE:
*   Symbolic constants, macro and structure definitions for the GMOS Sequencer
*
*   Copyright Observatory Sciences Ltd. 1999-2000. All rights reserved.
*   Under contract to the UK Astronomy Technology Centre, who modified the code
*   in 2001.
*
*/
/*
 * $Log: gmSeq.h,v $
 * Revision 1.1  2010/04/19 17:10:02  mrippa
 * Found missing from cvs
 *
 * Revision 1.1  2008/04/24 16:44:17  gemvx
 *
 * - ----------------------------------------------------------------------
 * -
 * - Committing in V1-4, gnirs version in use.
 * -
 * - Added Files:
 * - 	gnirs/.cvsignore gnirs/GNIRS.SftwrMntnMan2.doc gnirs/Makefile
 * - 	gnirs/Makefile.subdirs gnirs/README gnirs/epics.csh
 * - 	gnirs/iocBoot.flex gnirs/ls gnirs/nirs.env gnirs/nirsSetup
 * - 	gnirs/software.man gnirs/wfsBoot gnirs/CC/.cvsignore
 * - 	gnirs/CC/.make_subdirs gnirs/CC/Makefile gnirs/CC/README
 * - 	gnirs/CC/epicsBoot gnirs/CC/nirs.env gnirs/CC/nirsSetup
 * - 	gnirs/CC/alh/Makefile gnirs/CC/ar/Makefile
 * - 	gnirs/CC/ar/arChan/.cvsignore gnirs/CC/ar/arReq/.cvsignore
 * - 	gnirs/CC/ar/arSet/.cvsignore gnirs/CC/ascii/Makefile
 * - 	gnirs/CC/ascii/Makefile.Unix
 * - 	gnirs/CC/ascii/cat_ascii/.cvsignore
 * - 	gnirs/CC/ascii/cat_ascii/devSup.ascii
 * - 	gnirs/CC/ascii/replace_ascii/.cvsignore
 * - 	gnirs/CC/capfast/CBorder.sym gnirs/CC/capfast/CCcapfast.txt
 * - 	gnirs/CC/capfast/Makefile gnirs/CC/capfast/Makefile.Unix
 * - 	gnirs/CC/capfast/cad.rc gnirs/CC/capfast/cad2.sch
 * - 	gnirs/CC/capfast/cad2.sym gnirs/CC/capfast/cad4.sch
 * - 	gnirs/CC/capfast/cad4.sym gnirs/CC/capfast/cadCar.sch
 * - 	gnirs/CC/capfast/cadCar.sym gnirs/CC/capfast/cadFan.sch
 * - 	gnirs/CC/capfast/cadFan.sym gnirs/CC/capfast/cadFanout.sch
 * - 	gnirs/CC/capfast/cadFanout.sym gnirs/CC/capfast/ccApply.sch
 * - 	gnirs/CC/capfast/ccApply.sym gnirs/CC/capfast/ccSysCar.sch
 * - 	gnirs/CC/capfast/ccSysCar.sym gnirs/CC/capfast/ccTop.sch
 * - 	gnirs/CC/capfast/ccTop.sym gnirs/CC/capfast/combCar.sch
 * - 	gnirs/CC/capfast/combCar.sym gnirs/CC/capfast/coverApply.sym
 * - 	gnirs/CC/capfast/cryoCad.sch gnirs/CC/capfast/cryoCad.sym
 * - 	gnirs/CC/capfast/cryoSad.sch gnirs/CC/capfast/cryoSad.sym
 * - 	gnirs/CC/capfast/dataSad.sch gnirs/CC/capfast/dataSad.sym
 * - 	gnirs/CC/capfast/datumCad.sch gnirs/CC/capfast/datumCad.sym
 * - 	gnirs/CC/capfast/diagnosticTempSad.sch
 * - 	gnirs/CC/capfast/diagnosticTempSad.sym
 * - 	gnirs/CC/capfast/eapplyx.sym gnirs/CC/capfast/eborderC.sym
 * - 	gnirs/CC/capfast/edb.def gnirs/CC/capfast/edb.def.bak
 * - 	gnirs/CC/capfast/esirs.sch.bak gnirs/CC/capfast/fanTest.sch
 * - 	gnirs/CC/capfast/getNames.sch gnirs/CC/capfast/getNames.sym
 * - 	gnirs/CC/capfast/gmSeqLookupTables.sch
 * - 	gnirs/CC/capfast/gmosCadCar.sym
 * - 	gnirs/CC/capfast/gratingSad.sch
 * - 	gnirs/CC/capfast/gratingSad.sym gnirs/CC/capfast/hdwrSad.sch
 * - 	gnirs/CC/capfast/hdwrSad.sym gnirs/CC/capfast/healthSad.sch
 * - 	gnirs/CC/capfast/healthSad.sym gnirs/CC/capfast/heartBeat.sch
 * - 	gnirs/CC/capfast/heartBeat.sym gnirs/CC/capfast/init.sch
 * - 	gnirs/CC/capfast/init.sym gnirs/CC/capfast/link2Dir.sch
 * - 	gnirs/CC/capfast/link2Dir.sym gnirs/CC/capfast/link2DirVal.sch
 * - 	gnirs/CC/capfast/link2DirVal.sym
 * - 	gnirs/CC/capfast/lookupTables.sch
 * - 	gnirs/CC/capfast/lookupTables.sym
 * - 	gnirs/CC/capfast/mech1Cad.sch gnirs/CC/capfast/mech1Cad.sym
 * - 	gnirs/CC/capfast/mech2Cad.sch gnirs/CC/capfast/mech2Cad.sym
 * - 	gnirs/CC/capfast/mechApply.sch gnirs/CC/capfast/mechApply.sym
 * - 	gnirs/CC/capfast/mechNames.sch gnirs/CC/capfast/mechNames.sym
 * - 	gnirs/CC/capfast/motor1Apply.sch
 * - 	gnirs/CC/capfast/motor1Apply.sym
 * - 	gnirs/CC/capfast/motor2Apply.sch
 * - 	gnirs/CC/capfast/motor2Apply.sym
 * - 	gnirs/CC/capfast/motorApply.sch
 * - 	gnirs/CC/capfast/motorApply.sym gnirs/CC/capfast/motorSad.sch
 * - 	gnirs/CC/capfast/motorSad.sym
 * - 	gnirs/CC/capfast/motorSadIntrfc.sch
 * - 	gnirs/CC/capfast/motorSadIntrfc.sym
 * - 	gnirs/CC/capfast/multiCad.sym gnirs/CC/capfast/niriCCTop.sch
 * - 	gnirs/CC/capfast/nirsCCSad.sch gnirs/CC/capfast/nirsCCSad.sym
 * - 	gnirs/CC/capfast/nirsCCSadTop.sch
 * - 	gnirs/CC/capfast/nirsCCTop.sch gnirs/CC/capfast/nirsCadCar.sch
 * - 	gnirs/CC/capfast/noOp1Apply.sch
 * - 	gnirs/CC/capfast/noOp1Apply.sym
 * - 	gnirs/CC/capfast/noOp2Apply.sch
 * - 	gnirs/CC/capfast/noOp2Apply.sym gnirs/CC/capfast/noOpApply.sch
 * - 	gnirs/CC/capfast/noOpApply.sym gnirs/CC/capfast/noOpCmd.sch
 * - 	gnirs/CC/capfast/noOpCmd.sym gnirs/CC/capfast/notes.sch
 * - 	gnirs/CC/capfast/notes.sym gnirs/CC/capfast/oslBorderC.sym
 * - 	gnirs/CC/capfast/oslBorderD.sym gnirs/CC/capfast/parkCad.sch
 * - 	gnirs/CC/capfast/parkCad.sym gnirs/CC/capfast/prSccd
 * - 	gnirs/CC/capfast/prSccdSad gnirs/CC/capfast/pressureSad.sch
 * - 	gnirs/CC/capfast/pressureSad.sym
 * - 	gnirs/CC/capfast/primaryTempSad.sch
 * - 	gnirs/CC/capfast/primaryTempSad.sym
 * - 	gnirs/CC/capfast/primaryTemps.sch gnirs/CC/capfast/records.txt
 * - 	gnirs/CC/capfast/results gnirs/CC/capfast/roiSad.sch
 * - 	gnirs/CC/capfast/roiSad.sym gnirs/CC/capfast/sadInterface.sch
 * - 	gnirs/CC/capfast/sadInterface.sym
 * - 	gnirs/CC/capfast/sadIntrfc.sch gnirs/CC/capfast/sadIntrfc.sym
 * - 	gnirs/CC/capfast/sadRecords.txt gnirs/CC/capfast/schPrintFlex
 * - 	gnirs/CC/capfast/secondaryTemps.sch
 * - 	gnirs/CC/capfast/secondaryTemps.sym
 * - 	gnirs/CC/capfast/secondaryTempsSad.sch
 * - 	gnirs/CC/capfast/secondaryTempsSad.sym
 * - 	gnirs/CC/capfast/slitApply.sym gnirs/CC/capfast/sysApply.sch
 * - 	gnirs/CC/capfast/sysApply.sym gnirs/CC/capfast/sysCar.sch
 * - 	gnirs/CC/capfast/sysCar.sym gnirs/CC/capfast/sysSad.sch
 * - 	gnirs/CC/capfast/sysSad.sym gnirs/CC/capfast/task.sch
 * - 	gnirs/CC/capfast/task.sym gnirs/CC/capfast/tempDiag.sch
 * - 	gnirs/CC/capfast/tempDiag.sym gnirs/CC/capfast/tempSad.sch
 * - 	gnirs/CC/capfast/tempSad.sym gnirs/CC/capfast/test.sch
 * - 	gnirs/CC/capfast/test.sym gnirs/CC/capfast/xdispApply.sym
 * - 	gnirs/CC/capfast/CC/cad.rc gnirs/CC/capfast/CC/capfast/cad.rc
 * - 	gnirs/CC/capfast/CC/capfast/mech1Cad.sym
 * - 	gnirs/CC/capfast/CC/capfast/mech2Cad.sym
 * - 	gnirs/CC/dl/CCcars.adl gnirs/CC/dl/CCcars.dl
 * - 	gnirs/CC/dl/CCdl.txt gnirs/CC/dl/Makefile
 * - 	gnirs/CC/dl/Makefile.Unix gnirs/CC/dl/cars.adl
 * - 	gnirs/CC/dl/cars.dl gnirs/CC/dl/ccTop.adl gnirs/CC/dl/ccTop.dl
 * - 	gnirs/CC/dl/colors.adl gnirs/CC/dl/colors.dl
 * - 	gnirs/CC/dl/cryoCad.adl gnirs/CC/dl/cryoCad.dl
 * - 	gnirs/CC/dl/datumCad.adl gnirs/CC/dl/datumCad.dl
 * - 	gnirs/CC/dl/debugCad.adl gnirs/CC/dl/debugCad.dl
 * - 	gnirs/CC/dl/gmColors.adl gnirs/CC/dl/gmColors.dl
 * - 	gnirs/CC/dl/gmSeq.dl gnirs/CC/dl/gratingControl.adl
 * - 	gnirs/CC/dl/gratingControl.dl gnirs/CC/dl/gratingDatmCad.adl
 * - 	gnirs/CC/dl/gratingDatmCad.dl gnirs/CC/dl/gratingParkCad.adl
 * - 	gnirs/CC/dl/gratingParkCad.dl gnirs/CC/dl/gratingPosCad.adl
 * - 	gnirs/CC/dl/gratingPosCad.dl gnirs/CC/dl/gratingSad.adl
 * - 	gnirs/CC/dl/gratingSad.dl gnirs/CC/dl/gratingStepsCad.adl
 * - 	gnirs/CC/dl/gratingStepsCad.dl gnirs/CC/dl/initCad.adl
 * - 	gnirs/CC/dl/initCad.dl gnirs/CC/dl/list
 * - 	gnirs/CC/dl/motorCad.adl gnirs/CC/dl/motorCars.adl
 * - 	gnirs/CC/dl/motorCars.dl gnirs/CC/dl/motorControl.adl
 * - 	gnirs/CC/dl/motorControl.dl gnirs/CC/dl/motorDatmCad.adl
 * - 	gnirs/CC/dl/motorDatmCad.dl gnirs/CC/dl/motorParkCad.adl
 * - 	gnirs/CC/dl/motorParkCad.dl gnirs/CC/dl/motorPosCad.adl
 * - 	gnirs/CC/dl/motorPosCad.dl gnirs/CC/dl/motorSad.adl
 * - 	gnirs/CC/dl/motorSad.dl gnirs/CC/dl/motorStepsCad.adl
 * - 	gnirs/CC/dl/motorStepsCad.dl gnirs/CC/dl/motors.adl
 * - 	gnirs/CC/dl/motors.dl gnirs/CC/dl/noOpCad.adl
 * - 	gnirs/CC/dl/noOpCad.dl gnirs/CC/dl/parkCad.adl
 * - 	gnirs/CC/dl/parkCad.dl gnirs/CC/dl/pressure.adl
 * - 	gnirs/CC/dl/pressure.dl gnirs/CC/dl/rebootCad.adl
 * - 	gnirs/CC/dl/rebootCad.dl gnirs/CC/dl/sad.adl
 * - 	gnirs/CC/dl/sad.dl gnirs/CC/dl/tempDiags.adl
 * - 	gnirs/CC/dl/tempDiags.dl gnirs/CC/dl/template.adl
 * - 	gnirs/CC/dl/template.dl gnirs/CC/dl/temps.adl
 * - 	gnirs/CC/dl/temps.dl gnirs/CC/dl/test.adl gnirs/CC/dl/test.dl
 * - 	gnirs/CC/dl/CC/dl/motorPosCad.adl
 * - 	gnirs/CC/dl/CC/sys/hdwrControl/tandp.c
 * - 	gnirs/CC/dl/converttmp/colors.adl
 * - 	gnirs/CC/dl/converttmp/template.adl
 * - 	gnirs/CC/include/NaacIpsEpics.h gnirs/CC/include/bc350Time.h
 * - 	gnirs/CC/include/ccDefines.h gnirs/CC/include/cicsConst.h
 * - 	gnirs/CC/include/cicsLib.h gnirs/CC/include/epCommon.h
 * - 	gnirs/CC/include/epicsCA.h gnirs/CC/include/epicsCAint.h
 * - 	gnirs/CC/include/epicsDefines.h gnirs/CC/include/epicsNames.h
 * - 	gnirs/CC/include/gnirs.h.not gnirs/CC/include/gnirsCC.h
 * - 	gnirs/CC/include/gnirsCC.peter.h
 * - 	gnirs/CC/include/gnirsCCDefines.h
 * - 	gnirs/CC/include/gnirsCcDefs.h gnirs/CC/include/gnirsTasks.h
 * - 	gnirs/CC/include/obsSetupCad.h gnirs/CC/include/roiDefines.h
 * - 	gnirs/CC/include/saverCommon.h gnirs/CC/include/sdsuDiags.h
 * - 	gnirs/CC/include/sockutil.h gnirs/CC/include/status.h
 * - 	gnirs/CC/include/vxSockUtil.h gnirs/CC/include/wcs.h
 * - 	gnirs/CC/pv/Makefile gnirs/CC/pv/Makefile.Unix
 * - 	gnirs/CC/pv/Makefile.Vx gnirs/CC/pv/Mechanisms.fake
 * - 	gnirs/CC/pv/acq.cad gnirs/CC/pv/acq.lut gnirs/CC/pv/cadVals.pv
 * - 	gnirs/CC/pv/camera.lut gnirs/CC/pv/cover.lut
 * - 	gnirs/CC/pv/coverConfig gnirs/CC/pv/decker.lut
 * - 	gnirs/CC/pv/focus.lut gnirs/CC/pv/fw1.lut gnirs/CC/pv/fw2.lut
 * - 	gnirs/CC/pv/gnirsConfig gnirs/CC/pv/gnirsConfig.bak
 * - 	gnirs/CC/pv/gnirsConfig.test gnirs/CC/pv/gnirsConfigReal
 * - 	gnirs/CC/pv/gnirsFilters gnirs/CC/pv/gnirsMechanisms
 * - 	gnirs/CC/pv/gnirsMechanismsReal gnirs/CC/pv/grating.lut
 * - 	gnirs/CC/pv/mechanisms.pv gnirs/CC/pv/slit.lut
 * - 	gnirs/CC/pv/startupCC.pv gnirs/CC/pv/startupCCSim.pv
 * - 	gnirs/CC/pv/testC gnirs/CC/pv/xdisp.lut
 * - 	gnirs/CC/pv/keep/gnirsConfig gnirs/CC/pv/keep/gnirsFilters
 * - 	gnirs/CC/pv/keep/gnirsMechanisms gnirs/CC/pv/seed/gnirsConfig
 * - 	gnirs/CC/pv/seed/gnirsConfigReal gnirs/CC/pv/seed/gnirsFilters
 * - 	gnirs/CC/pv/seed/gnirsMechanisms
 * - 	gnirs/CC/pv/seed/gnirsMechanismsReal gnirs/CC/python/Makefile
 * - 	gnirs/CC/python/Makefile.in gnirs/CC/python/VxWorks.py
 * - 	gnirs/CC/python/VxWorks.pyc gnirs/CC/python/acconfig.h
 * - 	gnirs/CC/python/buildno gnirs/CC/python/config.h
 * - 	gnirs/CC/python/getbuildinfo.o gnirs/CC/python/getopt.c
 * - 	gnirs/CC/python/getopt.o gnirs/CC/python/install-sh
 * - 	gnirs/CC/python/libpython1.5.a gnirs/CC/python/motors.py
 * - 	gnirs/CC/python/motors.pyc gnirs/CC/python/python.o
 * - 	gnirs/CC/python/vxpython gnirs/CC/python/Grammar/Grammar
 * - 	gnirs/CC/python/Grammar/Makefile
 * - 	gnirs/CC/python/Include/Makefile
 * - 	gnirs/CC/python/Include/Python.h
 * - 	gnirs/CC/python/Include/abstract.h
 * - 	gnirs/CC/python/Include/bitset.h
 * - 	gnirs/CC/python/Include/bufferobject.h
 * - 	gnirs/CC/python/Include/cStringIO.h
 * - 	gnirs/CC/python/Include/ceval.h
 * - 	gnirs/CC/python/Include/classobject.h
 * - 	gnirs/CC/python/Include/cobject.h
 * - 	gnirs/CC/python/Include/compile.h
 * - 	gnirs/CC/python/Include/complexobject.h
 * - 	gnirs/CC/python/Include/dictobject.h
 * - 	gnirs/CC/python/Include/errcode.h
 * - 	gnirs/CC/python/Include/eval.h
 * - 	gnirs/CC/python/Include/fileobject.h
 * - 	gnirs/CC/python/Include/floatobject.h
 * - 	gnirs/CC/python/Include/frameobject.h
 * - 	gnirs/CC/python/Include/funcobject.h
 * - 	gnirs/CC/python/Include/getopt.h
 * - 	gnirs/CC/python/Include/graminit.h
 * - 	gnirs/CC/python/Include/grammar.h
 * - 	gnirs/CC/python/Include/import.h
 * - 	gnirs/CC/python/Include/intobject.h
 * - 	gnirs/CC/python/Include/intrcheck.h
 * - 	gnirs/CC/python/Include/listobject.h
 * - 	gnirs/CC/python/Include/longintrepr.h
 * - 	gnirs/CC/python/Include/longobject.h
 * - 	gnirs/CC/python/Include/marshal.h
 * - 	gnirs/CC/python/Include/metagrammar.h
 * - 	gnirs/CC/python/Include/methodobject.h
 * - 	gnirs/CC/python/Include/modsupport.h
 * - 	gnirs/CC/python/Include/moduleobject.h
 * - 	gnirs/CC/python/Include/mymalloc.h
 * - 	gnirs/CC/python/Include/mymath.h
 * - 	gnirs/CC/python/Include/myproto.h
 * - 	gnirs/CC/python/Include/myselect.h
 * - 	gnirs/CC/python/Include/mytime.h
 * - 	gnirs/CC/python/Include/node.h
 * - 	gnirs/CC/python/Include/object.h
 * - 	gnirs/CC/python/Include/objimpl.h
 * - 	gnirs/CC/python/Include/opcode.h
 * - 	gnirs/CC/python/Include/osdefs.h
 * - 	gnirs/CC/python/Include/parsetok.h
 * - 	gnirs/CC/python/Include/patchlevel.h
 * - 	gnirs/CC/python/Include/pgenheaders.h
 * - 	gnirs/CC/python/Include/pydebug.h
 * - 	gnirs/CC/python/Include/pyerrors.h
 * - 	gnirs/CC/python/Include/pyfpe.h
 * - 	gnirs/CC/python/Include/pystate.h
 * - 	gnirs/CC/python/Include/pythonrun.h
 * - 	gnirs/CC/python/Include/pythread.h
 * - 	gnirs/CC/python/Include/rangeobject.h
 * - 	gnirs/CC/python/Include/rename2.h
 * - 	gnirs/CC/python/Include/sliceobject.h
 * - 	gnirs/CC/python/Include/stringobject.h
 * - 	gnirs/CC/python/Include/structmember.h
 * - 	gnirs/CC/python/Include/sysmodule.h
 * - 	gnirs/CC/python/Include/thread.h
 * - 	gnirs/CC/python/Include/token.h
 * - 	gnirs/CC/python/Include/traceback.h
 * - 	gnirs/CC/python/Include/tupleobject.h
 * - 	gnirs/CC/python/Lib/BaseHTTPServer.py
 * - 	gnirs/CC/python/Lib/Bastion.py
 * - 	gnirs/CC/python/Lib/CGIHTTPServer.py
 * - 	gnirs/CC/python/Lib/ConfigParser.py
 * - 	gnirs/CC/python/Lib/Makefile gnirs/CC/python/Lib/MimeWriter.py
 * - 	gnirs/CC/python/Lib/MimeWriter.pyc
 * - 	gnirs/CC/python/Lib/Queue.py
 * - 	gnirs/CC/python/Lib/SimpleHTTPServer.py
 * - 	gnirs/CC/python/Lib/SocketServer.py
 * - 	gnirs/CC/python/Lib/StringIO.py
 * - 	gnirs/CC/python/Lib/StringIO.pyc
 * - 	gnirs/CC/python/Lib/UserDict.py
 * - 	gnirs/CC/python/Lib/UserDict.pyc
 * - 	gnirs/CC/python/Lib/UserList.py
 * - 	gnirs/CC/python/Lib/UserList.pyc gnirs/CC/python/Lib/aifc.py
 * - 	gnirs/CC/python/Lib/anydbm.py gnirs/CC/python/Lib/asynchat.py
 * - 	gnirs/CC/python/Lib/asyncore.py
 * - 	gnirs/CC/python/Lib/audiodev.py gnirs/CC/python/Lib/base64.py
 * - 	gnirs/CC/python/Lib/bdb.py gnirs/CC/python/Lib/binhex.py
 * - 	gnirs/CC/python/Lib/binhex.pyc gnirs/CC/python/Lib/bisect.py
 * - 	gnirs/CC/python/Lib/calendar.py
 * - 	gnirs/CC/python/Lib/calendar.pyc gnirs/CC/python/Lib/cgi.py
 * - 	gnirs/CC/python/Lib/cmd.py gnirs/CC/python/Lib/cmp.py
 * - 	gnirs/CC/python/Lib/cmpcache.py gnirs/CC/python/Lib/code.py
 * - 	gnirs/CC/python/Lib/codeop.py gnirs/CC/python/Lib/colorsys.py
 * - 	gnirs/CC/python/Lib/commands.py
 * - 	gnirs/CC/python/Lib/compileall.py gnirs/CC/python/Lib/copy.py
 * - 	gnirs/CC/python/Lib/copy.pyc gnirs/CC/python/Lib/copy_reg.py
 * - 	gnirs/CC/python/Lib/copy_reg.pyc gnirs/CC/python/Lib/dbhash.py
 * - 	gnirs/CC/python/Lib/dircache.py gnirs/CC/python/Lib/dircmp.py
 * - 	gnirs/CC/python/Lib/dis.py gnirs/CC/python/Lib/dospath.py
 * - 	gnirs/CC/python/Lib/dumbdbm.py gnirs/CC/python/Lib/dump.py
 * - 	gnirs/CC/python/Lib/exceptions.py
 * - 	gnirs/CC/python/Lib/exceptions.pyc
 * - 	gnirs/CC/python/Lib/fileinput.py gnirs/CC/python/Lib/find.py
 * - 	gnirs/CC/python/Lib/fnmatch.py
 * - 	gnirs/CC/python/Lib/formatter.py
 * - 	gnirs/CC/python/Lib/fpformat.py gnirs/CC/python/Lib/ftplib.py
 * - 	gnirs/CC/python/Lib/getopt.py gnirs/CC/python/Lib/getopt.pyc
 * - 	gnirs/CC/python/Lib/getpass.py gnirs/CC/python/Lib/glob.py
 * - 	gnirs/CC/python/Lib/gopherlib.py gnirs/CC/python/Lib/grep.py
 * - 	gnirs/CC/python/Lib/gzip.py gnirs/CC/python/Lib/gzip.pyc
 * - 	gnirs/CC/python/Lib/htmlentitydefs.py
 * - 	gnirs/CC/python/Lib/htmllib.py gnirs/CC/python/Lib/httplib.py
 * - 	gnirs/CC/python/Lib/ihooks.py gnirs/CC/python/Lib/imaplib.py
 * - 	gnirs/CC/python/Lib/imghdr.py gnirs/CC/python/Lib/keyword.py
 * - 	gnirs/CC/python/Lib/knee.py gnirs/CC/python/Lib/linecache.py
 * - 	gnirs/CC/python/Lib/linecache.pyc
 * - 	gnirs/CC/python/Lib/locale.py gnirs/CC/python/Lib/macpath.py
 * - 	gnirs/CC/python/Lib/macurl2path.py
 * - 	gnirs/CC/python/Lib/mailbox.py gnirs/CC/python/Lib/mailcap.py
 * - 	gnirs/CC/python/Lib/mhlib.py gnirs/CC/python/Lib/mimetools.py
 * - 	gnirs/CC/python/Lib/mimetools.pyc
 * - 	gnirs/CC/python/Lib/mimetypes.py gnirs/CC/python/Lib/mimify.py
 * - 	gnirs/CC/python/Lib/multifile.py gnirs/CC/python/Lib/mutex.py
 * - 	gnirs/CC/python/Lib/netrc.py gnirs/CC/python/Lib/nntplib.py
 * - 	gnirs/CC/python/Lib/ntpath.py gnirs/CC/python/Lib/ntpath.pyc
 * - 	gnirs/CC/python/Lib/nturl2path.py gnirs/CC/python/Lib/os.py
 * - 	gnirs/CC/python/Lib/os.pyc gnirs/CC/python/Lib/os.pyc.ok
 * - 	gnirs/CC/python/Lib/packmail.py gnirs/CC/python/Lib/pdb.doc
 * - 	gnirs/CC/python/Lib/pdb.py gnirs/CC/python/Lib/pickle.py
 * - 	gnirs/CC/python/Lib/pickle.pyc gnirs/CC/python/Lib/pipes.py
 * - 	gnirs/CC/python/Lib/popen2.py gnirs/CC/python/Lib/popen2.pyc
 * - 	gnirs/CC/python/Lib/poplib.py gnirs/CC/python/Lib/posixfile.py
 * - 	gnirs/CC/python/Lib/posixpath.py
 * - 	gnirs/CC/python/Lib/posixpath.pyc
 * - 	gnirs/CC/python/Lib/pprint.py gnirs/CC/python/Lib/profile.doc
 * - 	gnirs/CC/python/Lib/profile.py gnirs/CC/python/Lib/pstats.py
 * - 	gnirs/CC/python/Lib/pty.py gnirs/CC/python/Lib/py_compile.py
 * - 	gnirs/CC/python/Lib/pyclbr.py gnirs/CC/python/Lib/quopri.py
 * - 	gnirs/CC/python/Lib/random.py gnirs/CC/python/Lib/random.pyc
 * - 	gnirs/CC/python/Lib/re.py gnirs/CC/python/Lib/re.pyc
 * - 	gnirs/CC/python/Lib/reconvert.py
 * - 	gnirs/CC/python/Lib/regex_syntax.py
 * - 	gnirs/CC/python/Lib/regex_syntax.pyc
 * - 	gnirs/CC/python/Lib/regsub.py gnirs/CC/python/Lib/repr.py
 * - 	gnirs/CC/python/Lib/rexec.py gnirs/CC/python/Lib/rfc822.py
 * - 	gnirs/CC/python/Lib/rfc822.pyc
 * - 	gnirs/CC/python/Lib/rlcompleter.py
 * - 	gnirs/CC/python/Lib/sched.py gnirs/CC/python/Lib/sgmllib.py
 * - 	gnirs/CC/python/Lib/shelve.py gnirs/CC/python/Lib/shlex.py
 * - 	gnirs/CC/python/Lib/shutil.py gnirs/CC/python/Lib/site.py
 * - 	gnirs/CC/python/Lib/site.pyc
 * - 	gnirs/CC/python/Lib/sitecustomize.py
 * - 	gnirs/CC/python/Lib/sitecustomize.pyc
 * - 	gnirs/CC/python/Lib/smtplib.py gnirs/CC/python/Lib/sndhdr.py
 * - 	gnirs/CC/python/Lib/stat.py gnirs/CC/python/Lib/stat.pyc
 * - 	gnirs/CC/python/Lib/statcache.py
 * - 	gnirs/CC/python/Lib/statvfs.py gnirs/CC/python/Lib/string.py
 * - 	gnirs/CC/python/Lib/string.pyc gnirs/CC/python/Lib/sunau.py
 * - 	gnirs/CC/python/Lib/sunaudio.py gnirs/CC/python/Lib/symbol.py
 * - 	gnirs/CC/python/Lib/telnetlib.py
 * - 	gnirs/CC/python/Lib/tempfile.py
 * - 	gnirs/CC/python/Lib/tempfile.pyc
 * - 	gnirs/CC/python/Lib/threading.py gnirs/CC/python/Lib/toaiff.py
 * - 	gnirs/CC/python/Lib/token.py gnirs/CC/python/Lib/token.pyc
 * - 	gnirs/CC/python/Lib/tokenize.py
 * - 	gnirs/CC/python/Lib/tokenize.pyc
 * - 	gnirs/CC/python/Lib/traceback.py
 * - 	gnirs/CC/python/Lib/traceback.pyc gnirs/CC/python/Lib/tty.py
 * - 	gnirs/CC/python/Lib/turtle.py gnirs/CC/python/Lib/types.py
 * - 	gnirs/CC/python/Lib/types.pyc gnirs/CC/python/Lib/tzparse.py
 * - 	gnirs/CC/python/Lib/urllib.py gnirs/CC/python/Lib/urlparse.py
 * - 	gnirs/CC/python/Lib/user.py gnirs/CC/python/Lib/util.py
 * - 	gnirs/CC/python/Lib/uu.py gnirs/CC/python/Lib/wave.py
 * - 	gnirs/CC/python/Lib/whichdb.py gnirs/CC/python/Lib/whrandom.py
 * - 	gnirs/CC/python/Lib/whrandom.pyc gnirs/CC/python/Lib/xdrlib.py
 * - 	gnirs/CC/python/Lib/xmllib.py gnirs/CC/python/Lib/xmllib.pyc
 * - 	gnirs/CC/python/Lib/dos-8x3/basehttp.py
 * - 	gnirs/CC/python/Lib/dos-8x3/bastion.py
 * - 	gnirs/CC/python/Lib/dos-8x3/cgihttps.py
 * - 	gnirs/CC/python/Lib/dos-8x3/compilea.py
 * - 	gnirs/CC/python/Lib/dos-8x3/configpa.py
 * - 	gnirs/CC/python/Lib/dos-8x3/exceptio.py
 * - 	gnirs/CC/python/Lib/dos-8x3/fileinpu.py
 * - 	gnirs/CC/python/Lib/dos-8x3/formatte.py
 * - 	gnirs/CC/python/Lib/dos-8x3/gopherli.py
 * - 	gnirs/CC/python/Lib/dos-8x3/htmlenti.py
 * - 	gnirs/CC/python/Lib/dos-8x3/linecach.py
 * - 	gnirs/CC/python/Lib/dos-8x3/macurl2p.py
 * - 	gnirs/CC/python/Lib/dos-8x3/mimetool.py
 * - 	gnirs/CC/python/Lib/dos-8x3/mimetype.py
 * - 	gnirs/CC/python/Lib/dos-8x3/mimewrit.py
 * - 	gnirs/CC/python/Lib/dos-8x3/multifil.py
 * - 	gnirs/CC/python/Lib/dos-8x3/nturl2pa.py
 * - 	gnirs/CC/python/Lib/dos-8x3/para.py
 * - 	gnirs/CC/python/Lib/dos-8x3/posixfil.py
 * - 	gnirs/CC/python/Lib/dos-8x3/posixpat.py
 * - 	gnirs/CC/python/Lib/dos-8x3/py_compi.py
 * - 	gnirs/CC/python/Lib/dos-8x3/queue.py
 * - 	gnirs/CC/python/Lib/dos-8x3/reconver.py
 * - 	gnirs/CC/python/Lib/dos-8x3/regex_sy.py
 * - 	gnirs/CC/python/Lib/dos-8x3/regex_te.py
 * - 	gnirs/CC/python/Lib/dos-8x3/rlcomple.py
 * - 	gnirs/CC/python/Lib/dos-8x3/simpleht.py
 * - 	gnirs/CC/python/Lib/dos-8x3/socketse.py
 * - 	gnirs/CC/python/Lib/dos-8x3/statcach.py
 * - 	gnirs/CC/python/Lib/dos-8x3/stringio.py
 * - 	gnirs/CC/python/Lib/dos-8x3/telnetli.py
 * - 	gnirs/CC/python/Lib/dos-8x3/test_arr.py
 * - 	gnirs/CC/python/Lib/dos-8x3/test_aud.py
 * - 	gnirs/CC/python/Lib/dos-8x3/test_bin.py
 * - 	gnirs/CC/python/Lib/dos-8x3/test_bsd.py
 * - 	gnirs/CC/python/Lib/dos-8x3/test_bui.py
 * - 	gnirs/CC/python/Lib/dos-8x3/test_cma.py
 * - 	gnirs/CC/python/Lib/dos-8x3/test_cpi.py
 * - 	gnirs/CC/python/Lib/dos-8x3/test_cry.py
 * - 	gnirs/CC/python/Lib/dos-8x3/test_err.py
 * - 	gnirs/CC/python/Lib/dos-8x3/test_exc.py
 * - 	gnirs/CC/python/Lib/dos-8x3/test_fcn.py
 * - 	gnirs/CC/python/Lib/dos-8x3/test_gdb.py
 * - 	gnirs/CC/python/Lib/dos-8x3/test_gra.py
 * - 	gnirs/CC/python/Lib/dos-8x3/test_gzi.py
 * - 	gnirs/CC/python/Lib/dos-8x3/test_ima.py
 * - 	gnirs/CC/python/Lib/dos-8x3/test_img.py
 * - 	gnirs/CC/python/Lib/dos-8x3/test_lon.py
 * - 	gnirs/CC/python/Lib/dos-8x3/test_mat.py
 * - 	gnirs/CC/python/Lib/dos-8x3/test_mim.py
 * - 	gnirs/CC/python/Lib/dos-8x3/test_ntp.py
 * - 	gnirs/CC/python/Lib/dos-8x3/test_opc.py
 * - 	gnirs/CC/python/Lib/dos-8x3/test_ope.py
 * - 	gnirs/CC/python/Lib/dos-8x3/test_pic.py
 * - 	gnirs/CC/python/Lib/dos-8x3/test_pop.py
 * - 	gnirs/CC/python/Lib/dos-8x3/test_reg.py
 * - 	gnirs/CC/python/Lib/dos-8x3/test_rfc.py
 * - 	gnirs/CC/python/Lib/dos-8x3/test_rgb.py
 * - 	gnirs/CC/python/Lib/dos-8x3/test_rot.py
 * - 	gnirs/CC/python/Lib/dos-8x3/test_sel.py
 * - 	gnirs/CC/python/Lib/dos-8x3/test_sig.py
 * - 	gnirs/CC/python/Lib/dos-8x3/test_soc.py
 * - 	gnirs/CC/python/Lib/dos-8x3/test_str.py
 * - 	gnirs/CC/python/Lib/dos-8x3/test_sun.py
 * - 	gnirs/CC/python/Lib/dos-8x3/test_sup.py
 * - 	gnirs/CC/python/Lib/dos-8x3/test_thr.py
 * - 	gnirs/CC/python/Lib/dos-8x3/test_tim.py
 * - 	gnirs/CC/python/Lib/dos-8x3/test_tok.py
 * - 	gnirs/CC/python/Lib/dos-8x3/test_typ.py
 * - 	gnirs/CC/python/Lib/dos-8x3/test_unp.py
 * - 	gnirs/CC/python/Lib/dos-8x3/test_use.py
 * - 	gnirs/CC/python/Lib/dos-8x3/test_xml.py
 * - 	gnirs/CC/python/Lib/dos-8x3/test_zli.py
 * - 	gnirs/CC/python/Lib/dos-8x3/testntpa.py
 * - 	gnirs/CC/python/Lib/dos-8x3/threadin.py
 * - 	gnirs/CC/python/Lib/dos-8x3/tokenize.py
 * - 	gnirs/CC/python/Lib/dos-8x3/tracebac.py
 * - 	gnirs/CC/python/Lib/dos-8x3/userdict.py
 * - 	gnirs/CC/python/Lib/dos-8x3/userlist.py
 * - 	gnirs/CC/python/Lib/dos-8x3/whatsoun.py
 * - 	gnirs/CC/python/Lib/lib-old/Para.py
 * - 	gnirs/CC/python/Lib/lib-old/addpack.py
 * - 	gnirs/CC/python/Lib/lib-old/codehack.py
 * - 	gnirs/CC/python/Lib/lib-old/fmt.py
 * - 	gnirs/CC/python/Lib/lib-old/lockfile.py
 * - 	gnirs/CC/python/Lib/lib-old/newdir.py
 * - 	gnirs/CC/python/Lib/lib-old/ni.py
 * - 	gnirs/CC/python/Lib/lib-old/poly.py
 * - 	gnirs/CC/python/Lib/lib-old/rand.py
 * - 	gnirs/CC/python/Lib/lib-old/tb.py
 * - 	gnirs/CC/python/Lib/lib-old/whatsound.py
 * - 	gnirs/CC/python/Lib/lib-old/zmod.py
 * - 	gnirs/CC/python/Lib/lib-stdwin/Abstract.py
 * - 	gnirs/CC/python/Lib/lib-stdwin/BoxParent.py
 * - 	gnirs/CC/python/Lib/lib-stdwin/Buttons.py
 * - 	gnirs/CC/python/Lib/lib-stdwin/CSplit.py
 * - 	gnirs/CC/python/Lib/lib-stdwin/DirList.py
 * - 	gnirs/CC/python/Lib/lib-stdwin/FormSplit.py
 * - 	gnirs/CC/python/Lib/lib-stdwin/HVSplit.py
 * - 	gnirs/CC/python/Lib/lib-stdwin/Histogram.py
 * - 	gnirs/CC/python/Lib/lib-stdwin/Sliders.py
 * - 	gnirs/CC/python/Lib/lib-stdwin/Soundogram.py
 * - 	gnirs/CC/python/Lib/lib-stdwin/Split.py
 * - 	gnirs/CC/python/Lib/lib-stdwin/StripChart.py
 * - 	gnirs/CC/python/Lib/lib-stdwin/TextEdit.py
 * - 	gnirs/CC/python/Lib/lib-stdwin/TransParent.py
 * - 	gnirs/CC/python/Lib/lib-stdwin/VUMeter.py
 * - 	gnirs/CC/python/Lib/lib-stdwin/WindowParent.py
 * - 	gnirs/CC/python/Lib/lib-stdwin/WindowSched.py
 * - 	gnirs/CC/python/Lib/lib-stdwin/anywin.py
 * - 	gnirs/CC/python/Lib/lib-stdwin/basewin.py
 * - 	gnirs/CC/python/Lib/lib-stdwin/dirwin.py
 * - 	gnirs/CC/python/Lib/lib-stdwin/filewin.py
 * - 	gnirs/CC/python/Lib/lib-stdwin/formatter.py
 * - 	gnirs/CC/python/Lib/lib-stdwin/gwin.py
 * - 	gnirs/CC/python/Lib/lib-stdwin/listwin.py
 * - 	gnirs/CC/python/Lib/lib-stdwin/mainloop.py
 * - 	gnirs/CC/python/Lib/lib-stdwin/rect.py
 * - 	gnirs/CC/python/Lib/lib-stdwin/srcwin.py
 * - 	gnirs/CC/python/Lib/lib-stdwin/stdwinevents.py
 * - 	gnirs/CC/python/Lib/lib-stdwin/stdwinq.py
 * - 	gnirs/CC/python/Lib/lib-stdwin/tablewin.py
 * - 	gnirs/CC/python/Lib/lib-stdwin/textwin.py
 * - 	gnirs/CC/python/Lib/lib-stdwin/wdb.py
 * - 	gnirs/CC/python/Lib/lib-stdwin/wdbframewin.py
 * - 	gnirs/CC/python/Lib/lib-stdwin/wdbsrcwin.py
 * - 	gnirs/CC/python/Lib/lib-tk/Canvas.py
 * - 	gnirs/CC/python/Lib/lib-tk/Dialog.py
 * - 	gnirs/CC/python/Lib/lib-tk/FileDialog.py
 * - 	gnirs/CC/python/Lib/lib-tk/FixTk.py
 * - 	gnirs/CC/python/Lib/lib-tk/ScrolledText.py
 * - 	gnirs/CC/python/Lib/lib-tk/SimpleDialog.py
 * - 	gnirs/CC/python/Lib/lib-tk/Tkconstants.py
 * - 	gnirs/CC/python/Lib/lib-tk/Tkdnd.py
 * - 	gnirs/CC/python/Lib/lib-tk/Tkinter.py
 * - 	gnirs/CC/python/Lib/lib-tk/tkColorChooser.py
 * - 	gnirs/CC/python/Lib/lib-tk/tkCommonDialog.py
 * - 	gnirs/CC/python/Lib/lib-tk/tkFileDialog.py
 * - 	gnirs/CC/python/Lib/lib-tk/tkFont.py
 * - 	gnirs/CC/python/Lib/lib-tk/tkMessageBox.py
 * - 	gnirs/CC/python/Lib/lib-tk/tkSimpleDialog.py
 * - 	gnirs/CC/python/Lib/plat-generic/regen
 * - 	gnirs/CC/python/Lib/plat-sunos5/CDIO.py
 * - 	gnirs/CC/python/Lib/plat-sunos5/FCNTL.py
 * - 	gnirs/CC/python/Lib/plat-sunos5/FCNTL.pyc
 * - 	gnirs/CC/python/Lib/plat-sunos5/IN.py
 * - 	gnirs/CC/python/Lib/plat-sunos5/SOCKET.py
 * - 	gnirs/CC/python/Lib/plat-sunos5/STROPTS.py
 * - 	gnirs/CC/python/Lib/plat-sunos5/SUNAUDIODEV.py
 * - 	gnirs/CC/python/Lib/plat-sunos5/TERMIOS.py
 * - 	gnirs/CC/python/Lib/plat-sunos5/regen
 * - 	gnirs/CC/python/Lib/test/__init__.py
 * - 	gnirs/CC/python/Lib/test/audiotest.au
 * - 	gnirs/CC/python/Lib/test/autotest.py
 * - 	gnirs/CC/python/Lib/test/greyrgb.uue
 * - 	gnirs/CC/python/Lib/test/pystone.py
 * - 	gnirs/CC/python/Lib/test/re_tests.py
 * - 	gnirs/CC/python/Lib/test/re_tests.pyc
 * - 	gnirs/CC/python/Lib/test/regex_tests.py
 * - 	gnirs/CC/python/Lib/test/regex_tests.pyc
 * - 	gnirs/CC/python/Lib/test/regrtest.py
 * - 	gnirs/CC/python/Lib/test/reperf.py
 * - 	gnirs/CC/python/Lib/test/sortperf.py
 * - 	gnirs/CC/python/Lib/test/test_MimeWriter.py
 * - 	gnirs/CC/python/Lib/test/test_MimeWriter.pyc
 * - 	gnirs/CC/python/Lib/test/test_al.py
 * - 	gnirs/CC/python/Lib/test/test_al.pyc
 * - 	gnirs/CC/python/Lib/test/test_array.py
 * - 	gnirs/CC/python/Lib/test/test_array.pyc
 * - 	gnirs/CC/python/Lib/test/test_audioop.py
 * - 	gnirs/CC/python/Lib/test/test_audioop.pyc
 * - 	gnirs/CC/python/Lib/test/test_b1.py
 * - 	gnirs/CC/python/Lib/test/test_b1.pyc
 * - 	gnirs/CC/python/Lib/test/test_b2.py
 * - 	gnirs/CC/python/Lib/test/test_b2.pyc
 * - 	gnirs/CC/python/Lib/test/test_binascii.py
 * - 	gnirs/CC/python/Lib/test/test_binascii.pyc
 * - 	gnirs/CC/python/Lib/test/test_bsddb.py
 * - 	gnirs/CC/python/Lib/test/test_bsddb.pyc
 * - 	gnirs/CC/python/Lib/test/test_builtin.py
 * - 	gnirs/CC/python/Lib/test/test_builtin.pyc
 * - 	gnirs/CC/python/Lib/test/test_cd.py
 * - 	gnirs/CC/python/Lib/test/test_cd.pyc
 * - 	gnirs/CC/python/Lib/test/test_cl.py
 * - 	gnirs/CC/python/Lib/test/test_cl.pyc
 * - 	gnirs/CC/python/Lib/test/test_cmath.py
 * - 	gnirs/CC/python/Lib/test/test_cmath.pyc
 * - 	gnirs/CC/python/Lib/test/test_cpickle.py
 * - 	gnirs/CC/python/Lib/test/test_cpickle.pyc
 * - 	gnirs/CC/python/Lib/test/test_crypt.py
 * - 	gnirs/CC/python/Lib/test/test_crypt.pyc
 * - 	gnirs/CC/python/Lib/test/test_dbm.py
 * - 	gnirs/CC/python/Lib/test/test_dbm.pyc
 * - 	gnirs/CC/python/Lib/test/test_dl.py
 * - 	gnirs/CC/python/Lib/test/test_dl.pyc
 * - 	gnirs/CC/python/Lib/test/test_errno.py
 * - 	gnirs/CC/python/Lib/test/test_errno.pyc
 * - 	gnirs/CC/python/Lib/test/test_exceptions.py
 * - 	gnirs/CC/python/Lib/test/test_exceptions.pyc
 * - 	gnirs/CC/python/Lib/test/test_fcntl.py
 * - 	gnirs/CC/python/Lib/test/test_fcntl.pyc
 * - 	gnirs/CC/python/Lib/test/test_gdbm.py
 * - 	gnirs/CC/python/Lib/test/test_gdbm.pyc
 * - 	gnirs/CC/python/Lib/test/test_gl.py
 * - 	gnirs/CC/python/Lib/test/test_gl.pyc
 * - 	gnirs/CC/python/Lib/test/test_grammar.py
 * - 	gnirs/CC/python/Lib/test/test_grammar.pyc
 * - 	gnirs/CC/python/Lib/test/test_grp.py
 * - 	gnirs/CC/python/Lib/test/test_grp.pyc
 * - 	gnirs/CC/python/Lib/test/test_gzip.py
 * - 	gnirs/CC/python/Lib/test/test_gzip.pyc
 * - 	gnirs/CC/python/Lib/test/test_imageop.py
 * - 	gnirs/CC/python/Lib/test/test_imageop.pyc
 * - 	gnirs/CC/python/Lib/test/test_imgfile.py
 * - 	gnirs/CC/python/Lib/test/test_imgfile.pyc
 * - 	gnirs/CC/python/Lib/test/test_long.py
 * - 	gnirs/CC/python/Lib/test/test_long.pyc
 * - 	gnirs/CC/python/Lib/test/test_math.py
 * - 	gnirs/CC/python/Lib/test/test_math.pyc
 * - 	gnirs/CC/python/Lib/test/test_md5.py
 * - 	gnirs/CC/python/Lib/test/test_md5.pyc
 * - 	gnirs/CC/python/Lib/test/test_new.py
 * - 	gnirs/CC/python/Lib/test/test_new.pyc
 * - 	gnirs/CC/python/Lib/test/test_nis.py
 * - 	gnirs/CC/python/Lib/test/test_nis.pyc
 * - 	gnirs/CC/python/Lib/test/test_ntpath.py
 * - 	gnirs/CC/python/Lib/test/test_ntpath.pyc
 * - 	gnirs/CC/python/Lib/test/test_opcodes.py
 * - 	gnirs/CC/python/Lib/test/test_opcodes.pyc
 * - 	gnirs/CC/python/Lib/test/test_operations.py
 * - 	gnirs/CC/python/Lib/test/test_operations.pyc
 * - 	gnirs/CC/python/Lib/test/test_operator.py
 * - 	gnirs/CC/python/Lib/test/test_operator.pyc
 * - 	gnirs/CC/python/Lib/test/test_pickle.py
 * - 	gnirs/CC/python/Lib/test/test_pickle.pyc
 * - 	gnirs/CC/python/Lib/test/test_pkg.py
 * - 	gnirs/CC/python/Lib/test/test_pkg.pyc
 * - 	gnirs/CC/python/Lib/test/test_popen2.py
 * - 	gnirs/CC/python/Lib/test/test_popen2.pyc
 * - 	gnirs/CC/python/Lib/test/test_pow.py
 * - 	gnirs/CC/python/Lib/test/test_pow.pyc
 * - 	gnirs/CC/python/Lib/test/test_pwd.py
 * - 	gnirs/CC/python/Lib/test/test_pwd.pyc
 * - 	gnirs/CC/python/Lib/test/test_re.py
 * - 	gnirs/CC/python/Lib/test/test_re.pyc
 * - 	gnirs/CC/python/Lib/test/test_regex.py
 * - 	gnirs/CC/python/Lib/test/test_regex.pyc
 * - 	gnirs/CC/python/Lib/test/test_rfc822.py
 * - 	gnirs/CC/python/Lib/test/test_rfc822.pyc
 * - 	gnirs/CC/python/Lib/test/test_rgbimg.py
 * - 	gnirs/CC/python/Lib/test/test_rgbimg.pyc
 * - 	gnirs/CC/python/Lib/test/test_rotor.py
 * - 	gnirs/CC/python/Lib/test/test_rotor.pyc
 * - 	gnirs/CC/python/Lib/test/test_select.py
 * - 	gnirs/CC/python/Lib/test/test_select.pyc
 * - 	gnirs/CC/python/Lib/test/test_sha.py
 * - 	gnirs/CC/python/Lib/test/test_sha.pyc
 * - 	gnirs/CC/python/Lib/test/test_signal.py
 * - 	gnirs/CC/python/Lib/test/test_signal.pyc
 * - 	gnirs/CC/python/Lib/test/test_socket.py
 * - 	gnirs/CC/python/Lib/test/test_socket.pyc
 * - 	gnirs/CC/python/Lib/test/test_strftime.py
 * - 	gnirs/CC/python/Lib/test/test_strftime.pyc
 * - 	gnirs/CC/python/Lib/test/test_strop.py
 * - 	gnirs/CC/python/Lib/test/test_strop.pyc
 * - 	gnirs/CC/python/Lib/test/test_struct.py
 * - 	gnirs/CC/python/Lib/test/test_struct.pyc
 * - 	gnirs/CC/python/Lib/test/test_sunaudiodev.py
 * - 	gnirs/CC/python/Lib/test/test_sunaudiodev.pyc
 * - 	gnirs/CC/python/Lib/test/test_support.py
 * - 	gnirs/CC/python/Lib/test/test_support.pyc
 * - 	gnirs/CC/python/Lib/test/test_thread.py
 * - 	gnirs/CC/python/Lib/test/test_thread.pyc
 * - 	gnirs/CC/python/Lib/test/test_time.py
 * - 	gnirs/CC/python/Lib/test/test_time.pyc
 * - 	gnirs/CC/python/Lib/test/test_timing.py
 * - 	gnirs/CC/python/Lib/test/test_timing.pyc
 * - 	gnirs/CC/python/Lib/test/test_tokenize.py
 * - 	gnirs/CC/python/Lib/test/test_tokenize.pyc
 * - 	gnirs/CC/python/Lib/test/test_types.py
 * - 	gnirs/CC/python/Lib/test/test_types.pyc
 * - 	gnirs/CC/python/Lib/test/test_unpack.py
 * - 	gnirs/CC/python/Lib/test/test_unpack.pyc
 * - 	gnirs/CC/python/Lib/test/test_userdict.py
 * - 	gnirs/CC/python/Lib/test/test_userdict.pyc
 * - 	gnirs/CC/python/Lib/test/test_userlist.py
 * - 	gnirs/CC/python/Lib/test/test_userlist.pyc
 * - 	gnirs/CC/python/Lib/test/test_xmllib.py
 * - 	gnirs/CC/python/Lib/test/test_xmllib.pyc
 * - 	gnirs/CC/python/Lib/test/test_zlib.py
 * - 	gnirs/CC/python/Lib/test/test_zlib.pyc
 * - 	gnirs/CC/python/Lib/test/testall.py
 * - 	gnirs/CC/python/Lib/test/testimg.uue
 * - 	gnirs/CC/python/Lib/test/testimgr.uue
 * - 	gnirs/CC/python/Lib/test/testrgb.uue
 * - 	gnirs/CC/python/Lib/test/tokenize_tests.py
 * - 	gnirs/CC/python/Lib/test/output/test_MimeWriter
 * - 	gnirs/CC/python/Lib/test/output/test_al
 * - 	gnirs/CC/python/Lib/test/output/test_array
 * - 	gnirs/CC/python/Lib/test/output/test_audioop
 * - 	gnirs/CC/python/Lib/test/output/test_binascii
 * - 	gnirs/CC/python/Lib/test/output/test_builtin
 * - 	gnirs/CC/python/Lib/test/output/test_cd
 * - 	gnirs/CC/python/Lib/test/output/test_cl
 * - 	gnirs/CC/python/Lib/test/output/test_cmath
 * - 	gnirs/CC/python/Lib/test/output/test_cpickle
 * - 	gnirs/CC/python/Lib/test/output/test_crypt
 * - 	gnirs/CC/python/Lib/test/output/test_dbm
 * - 	gnirs/CC/python/Lib/test/output/test_dl
 * - 	gnirs/CC/python/Lib/test/output/test_errno
 * - 	gnirs/CC/python/Lib/test/output/test_exceptions
 * - 	gnirs/CC/python/Lib/test/output/test_fcntl
 * - 	gnirs/CC/python/Lib/test/output/test_gdbm
 * - 	gnirs/CC/python/Lib/test/output/test_gl
 * - 	gnirs/CC/python/Lib/test/output/test_grammar
 * - 	gnirs/CC/python/Lib/test/output/test_grp
 * - 	gnirs/CC/python/Lib/test/output/test_gzip
 * - 	gnirs/CC/python/Lib/test/output/test_imageop
 * - 	gnirs/CC/python/Lib/test/output/test_imgfile
 * - 	gnirs/CC/python/Lib/test/output/test_long
 * - 	gnirs/CC/python/Lib/test/output/test_math
 * - 	gnirs/CC/python/Lib/test/output/test_md5
 * - 	gnirs/CC/python/Lib/test/output/test_new
 * - 	gnirs/CC/python/Lib/test/output/test_nis
 * - 	gnirs/CC/python/Lib/test/output/test_ntpath
 * - 	gnirs/CC/python/Lib/test/output/test_opcodes
 * - 	gnirs/CC/python/Lib/test/output/test_operations
 * - 	gnirs/CC/python/Lib/test/output/test_operator
 * - 	gnirs/CC/python/Lib/test/output/test_pickle
 * - 	gnirs/CC/python/Lib/test/output/test_pkg
 * - 	gnirs/CC/python/Lib/test/output/test_popen2
 * - 	gnirs/CC/python/Lib/test/output/test_pow
 * - 	gnirs/CC/python/Lib/test/output/test_pwd
 * - 	gnirs/CC/python/Lib/test/output/test_re
 * - 	gnirs/CC/python/Lib/test/output/test_regex
 * - 	gnirs/CC/python/Lib/test/output/test_rfc822
 * - 	gnirs/CC/python/Lib/test/output/test_rgbimg
 * - 	gnirs/CC/python/Lib/test/output/test_rotor
 * - 	gnirs/CC/python/Lib/test/output/test_select
 * - 	gnirs/CC/python/Lib/test/output/test_sha
 * - 	gnirs/CC/python/Lib/test/output/test_signal
 * - 	gnirs/CC/python/Lib/test/output/test_socket
 * - 	gnirs/CC/python/Lib/test/output/test_strftime
 * - 	gnirs/CC/python/Lib/test/output/test_strop
 * - 	gnirs/CC/python/Lib/test/output/test_struct
 * - 	gnirs/CC/python/Lib/test/output/test_sunaudiodev
 * - 	gnirs/CC/python/Lib/test/output/test_thread
 * - 	gnirs/CC/python/Lib/test/output/test_time
 * - 	gnirs/CC/python/Lib/test/output/test_timing
 * - 	gnirs/CC/python/Lib/test/output/test_tokenize
 * - 	gnirs/CC/python/Lib/test/output/test_types
 * - 	gnirs/CC/python/Lib/test/output/test_unpack
 * - 	gnirs/CC/python/Lib/test/output/test_userdict
 * - 	gnirs/CC/python/Lib/test/output/test_userlist
 * - 	gnirs/CC/python/Lib/test/output/test_xmllib
 * - 	gnirs/CC/python/Lib/test/output/test_zlib
 * - 	gnirs/CC/python/Modules/Makefile
 * - 	gnirs/CC/python/Modules/Makefile.pre
 * - 	gnirs/CC/python/Modules/Makefile.pre.in
 * - 	gnirs/CC/python/Modules/Makefile.pre.ok
 * - 	gnirs/CC/python/Modules/Setup gnirs/CC/python/Modules/Setup.in
 * - 	gnirs/CC/python/Modules/Setup.local
 * - 	gnirs/CC/python/Modules/Setup.thread
 * - 	gnirs/CC/python/Modules/Setup.thread.in
 * - 	gnirs/CC/python/Modules/_localemodule.c
 * - 	gnirs/CC/python/Modules/_tkinter.c
 * - 	gnirs/CC/python/Modules/add2lib
 * - 	gnirs/CC/python/Modules/almodule.c
 * - 	gnirs/CC/python/Modules/arraymodule.c
 * - 	gnirs/CC/python/Modules/arraymodule.o
 * - 	gnirs/CC/python/Modules/audioop.c
 * - 	gnirs/CC/python/Modules/binascii.c
 * - 	gnirs/CC/python/Modules/bsddbmodule.c
 * - 	gnirs/CC/python/Modules/cPickle.c
 * - 	gnirs/CC/python/Modules/cStringIO.c
 * - 	gnirs/CC/python/Modules/cdmodule.c
 * - 	gnirs/CC/python/Modules/cgen.py
 * - 	gnirs/CC/python/Modules/cgensupport.c
 * - 	gnirs/CC/python/Modules/cgensupport.h
 * - 	gnirs/CC/python/Modules/clmodule.c
 * - 	gnirs/CC/python/Modules/cmathmodule.c
 * - 	gnirs/CC/python/Modules/cmathmodule.o
 * - 	gnirs/CC/python/Modules/config.c
 * - 	gnirs/CC/python/Modules/config.c.in
 * - 	gnirs/CC/python/Modules/config.o
 * - 	gnirs/CC/python/Modules/cryptmodule.c
 * - 	gnirs/CC/python/Modules/cstubs
 * - 	gnirs/CC/python/Modules/cursesmodule.c
 * - 	gnirs/CC/python/Modules/dbmmodule.c
 * - 	gnirs/CC/python/Modules/dlmodule.c
 * - 	gnirs/CC/python/Modules/errnomodule.c
 * - 	gnirs/CC/python/Modules/errnomodule.o
 * - 	gnirs/CC/python/Modules/fcntlmodule.c
 * - 	gnirs/CC/python/Modules/flmodule.c
 * - 	gnirs/CC/python/Modules/fmmodule.c
 * - 	gnirs/CC/python/Modules/fpectlmodule.c
 * - 	gnirs/CC/python/Modules/fpetestmodule.c
 * - 	gnirs/CC/python/Modules/gdbmmodule.c
 * - 	gnirs/CC/python/Modules/getbuildinfo.c
 * - 	gnirs/CC/python/Modules/getbuildinfo.o
 * - 	gnirs/CC/python/Modules/getpath.c
 * - 	gnirs/CC/python/Modules/getpath.o
 * - 	gnirs/CC/python/Modules/glmodule.c
 * - 	gnirs/CC/python/Modules/grpmodule.c
 * - 	gnirs/CC/python/Modules/hassignal
 * - 	gnirs/CC/python/Modules/imageop.c
 * - 	gnirs/CC/python/Modules/imgfile.c
 * - 	gnirs/CC/python/Modules/ld_so_aix
 * - 	gnirs/CC/python/Modules/license.terms
 * - 	gnirs/CC/python/Modules/main.c gnirs/CC/python/Modules/main.o
 * - 	gnirs/CC/python/Modules/makesetup
 * - 	gnirs/CC/python/Modules/makexp_aix
 * - 	gnirs/CC/python/Modules/mathmodule.c
 * - 	gnirs/CC/python/Modules/mathmodule.o
 * - 	gnirs/CC/python/Modules/md5.h gnirs/CC/python/Modules/md5c.c
 * - 	gnirs/CC/python/Modules/md5module.c gnirs/CC/python/Modules/mo
 * - 	gnirs/CC/python/Modules/mpzmodule.c
 * - 	gnirs/CC/python/Modules/newmodule.c
 * - 	gnirs/CC/python/Modules/newmodule.o
 * - 	gnirs/CC/python/Modules/nismodule.c
 * - 	gnirs/CC/python/Modules/operator.c
 * - 	gnirs/CC/python/Modules/operator.o
 * - 	gnirs/CC/python/Modules/parsermodule.c
 * - 	gnirs/CC/python/Modules/pcre-int.h
 * - 	gnirs/CC/python/Modules/pcre.h
 * - 	gnirs/CC/python/Modules/pcremodule.c
 * - 	gnirs/CC/python/Modules/pcremodule.o
 * - 	gnirs/CC/python/Modules/posixmodule.c
 * - 	gnirs/CC/python/Modules/posixmodule.o
 * - 	gnirs/CC/python/Modules/puremodule.c
 * - 	gnirs/CC/python/Modules/pwdmodule.c
 * - 	gnirs/CC/python/Modules/pypcre.c
 * - 	gnirs/CC/python/Modules/pypcre.o
 * - 	gnirs/CC/python/Modules/python.c
 * - 	gnirs/CC/python/Modules/python.o
 * - 	gnirs/CC/python/Modules/readline.c
 * - 	gnirs/CC/python/Modules/regexmodule.c
 * - 	gnirs/CC/python/Modules/regexmodule.o
 * - 	gnirs/CC/python/Modules/regexpr.c
 * - 	gnirs/CC/python/Modules/regexpr.h
 * - 	gnirs/CC/python/Modules/regexpr.o
 * - 	gnirs/CC/python/Modules/resource.c
 * - 	gnirs/CC/python/Modules/rgbimgmodule.c
 * - 	gnirs/CC/python/Modules/rotormodule.c
 * - 	gnirs/CC/python/Modules/selectmodule.c
 * - 	gnirs/CC/python/Modules/sgimodule.c
 * - 	gnirs/CC/python/Modules/shamodule.c
 * - 	gnirs/CC/python/Modules/signalmodule.c
 * - 	gnirs/CC/python/Modules/signalmodule.o
 * - 	gnirs/CC/python/Modules/socketmodule.c
 * - 	gnirs/CC/python/Modules/soundex.c
 * - 	gnirs/CC/python/Modules/stdwinmodule.c
 * - 	gnirs/CC/python/Modules/stropmodule.c
 * - 	gnirs/CC/python/Modules/stropmodule.o
 * - 	gnirs/CC/python/Modules/structmodule.c
 * - 	gnirs/CC/python/Modules/structmodule.o
 * - 	gnirs/CC/python/Modules/sunaudiodev.c
 * - 	gnirs/CC/python/Modules/svmodule.c
 * - 	gnirs/CC/python/Modules/syslogmodule.c
 * - 	gnirs/CC/python/Modules/tclNotify.c
 * - 	gnirs/CC/python/Modules/termios.c
 * - 	gnirs/CC/python/Modules/threadmodule.c
 * - 	gnirs/CC/python/Modules/timemodule.c
 * - 	gnirs/CC/python/Modules/timemodule.o
 * - 	gnirs/CC/python/Modules/timing.h
 * - 	gnirs/CC/python/Modules/timingmodule.c
 * - 	gnirs/CC/python/Modules/tkImaging.c
 * - 	gnirs/CC/python/Modules/tkappinit.c
 * - 	gnirs/CC/python/Modules/vxmodule.c
 * - 	gnirs/CC/python/Modules/vxmodule.o
 * - 	gnirs/CC/python/Modules/xxmodule.c
 * - 	gnirs/CC/python/Modules/yuv.h
 * - 	gnirs/CC/python/Modules/yuvconvert.c
 * - 	gnirs/CC/python/Modules/zlibmodule.c
 * - 	gnirs/CC/python/Modules/sunos/Makefile.sunos
 * - 	gnirs/CC/python/Modules/sunos/Setup.local.sunos
 * - 	gnirs/CC/python/Modules/sunos/Setup.sunos
 * - 	gnirs/CC/python/Modules/sunos/Setup.thread.sunos
 * - 	gnirs/CC/python/Modules/sunos/posixmodule.c
 * - 	gnirs/CC/python/Objects/Makefile
 * - 	gnirs/CC/python/Objects/Makefile.in
 * - 	gnirs/CC/python/Objects/Makefile.sunos
 * - 	gnirs/CC/python/Objects/abstract.c
 * - 	gnirs/CC/python/Objects/abstract.o
 * - 	gnirs/CC/python/Objects/add2lib
 * - 	gnirs/CC/python/Objects/bufferobject.c
 * - 	gnirs/CC/python/Objects/bufferobject.o
 * - 	gnirs/CC/python/Objects/classobject.c
 * - 	gnirs/CC/python/Objects/classobject.o
 * - 	gnirs/CC/python/Objects/cobject.c
 * - 	gnirs/CC/python/Objects/cobject.o
 * - 	gnirs/CC/python/Objects/complexobject.c
 * - 	gnirs/CC/python/Objects/complexobject.o
 * - 	gnirs/CC/python/Objects/dictobject.c
 * - 	gnirs/CC/python/Objects/dictobject.o
 * - 	gnirs/CC/python/Objects/fileobject.c
 * - 	gnirs/CC/python/Objects/fileobject.o
 * - 	gnirs/CC/python/Objects/floatobject.c
 * - 	gnirs/CC/python/Objects/floatobject.o
 * - 	gnirs/CC/python/Objects/frameobject.c
 * - 	gnirs/CC/python/Objects/frameobject.o
 * - 	gnirs/CC/python/Objects/funcobject.c
 * - 	gnirs/CC/python/Objects/funcobject.o
 * - 	gnirs/CC/python/Objects/intobject.c
 * - 	gnirs/CC/python/Objects/intobject.o
 * - 	gnirs/CC/python/Objects/listobject.c
 * - 	gnirs/CC/python/Objects/listobject.o
 * - 	gnirs/CC/python/Objects/longobject.c
 * - 	gnirs/CC/python/Objects/longobject.o
 * - 	gnirs/CC/python/Objects/methodobject.c
 * - 	gnirs/CC/python/Objects/methodobject.o
 * - 	gnirs/CC/python/Objects/moduleobject.c
 * - 	gnirs/CC/python/Objects/moduleobject.o
 * - 	gnirs/CC/python/Objects/object.c
 * - 	gnirs/CC/python/Objects/object.o
 * - 	gnirs/CC/python/Objects/rangeobject.c
 * - 	gnirs/CC/python/Objects/rangeobject.o
 * - 	gnirs/CC/python/Objects/sliceobject.c
 * - 	gnirs/CC/python/Objects/sliceobject.o
 * - 	gnirs/CC/python/Objects/stringobject.c
 * - 	gnirs/CC/python/Objects/stringobject.o
 * - 	gnirs/CC/python/Objects/tupleobject.c
 * - 	gnirs/CC/python/Objects/tupleobject.o
 * - 	gnirs/CC/python/Objects/typeobject.c
 * - 	gnirs/CC/python/Objects/typeobject.o
 * - 	gnirs/CC/python/Objects/xxobject.c
 * - 	gnirs/CC/python/Parser/Makefile
 * - 	gnirs/CC/python/Parser/Makefile.in
 * - 	gnirs/CC/python/Parser/Makefile.sunos
 * - 	gnirs/CC/python/Parser/acceler.c
 * - 	gnirs/CC/python/Parser/acceler.o
 * - 	gnirs/CC/python/Parser/add2lib gnirs/CC/python/Parser/assert.h
 * - 	gnirs/CC/python/Parser/bitset.c
 * - 	gnirs/CC/python/Parser/bitset.o
 * - 	gnirs/CC/python/Parser/firstsets.c
 * - 	gnirs/CC/python/Parser/grammar.c
 * - 	gnirs/CC/python/Parser/grammar1.c
 * - 	gnirs/CC/python/Parser/grammar1.o
 * - 	gnirs/CC/python/Parser/intrcheck.c
 * - 	gnirs/CC/python/Parser/intrcheck.o
 * - 	gnirs/CC/python/Parser/listnode.c
 * - 	gnirs/CC/python/Parser/listnode.o
 * - 	gnirs/CC/python/Parser/metagrammar.c
 * - 	gnirs/CC/python/Parser/metagrammar.o
 * - 	gnirs/CC/python/Parser/myreadline.c
 * - 	gnirs/CC/python/Parser/myreadline.o
 * - 	gnirs/CC/python/Parser/node.c gnirs/CC/python/Parser/node.o
 * - 	gnirs/CC/python/Parser/parser.c
 * - 	gnirs/CC/python/Parser/parser.h
 * - 	gnirs/CC/python/Parser/parser.o
 * - 	gnirs/CC/python/Parser/parsetok.c
 * - 	gnirs/CC/python/Parser/parsetok.o gnirs/CC/python/Parser/pgen
 * - 	gnirs/CC/python/Parser/pgen.c gnirs/CC/python/Parser/pgen.h
 * - 	gnirs/CC/python/Parser/pgenmain.c
 * - 	gnirs/CC/python/Parser/printgrammar.c
 * - 	gnirs/CC/python/Parser/tokenizer.c
 * - 	gnirs/CC/python/Parser/tokenizer.h
 * - 	gnirs/CC/python/Parser/tokenizer.o
 * - 	gnirs/CC/python/Python/Makefile
 * - 	gnirs/CC/python/Python/Makefile.in
 * - 	gnirs/CC/python/Python/Makefile.sunos
 * - 	gnirs/CC/python/Python/add2lib gnirs/CC/python/Python/atof.c
 * - 	gnirs/CC/python/Python/bltinmodule.c
 * - 	gnirs/CC/python/Python/bltinmodule.o
 * - 	gnirs/CC/python/Python/ceval.c gnirs/CC/python/Python/ceval.o
 * - 	gnirs/CC/python/Python/compile.c
 * - 	gnirs/CC/python/Python/compile.o gnirs/CC/python/Python/dup2.c
 * - 	gnirs/CC/python/Python/errors.c
 * - 	gnirs/CC/python/Python/errors.o gnirs/CC/python/Python/fmod.c
 * - 	gnirs/CC/python/Python/frozen.c
 * - 	gnirs/CC/python/Python/frozen.o
 * - 	gnirs/CC/python/Python/frozenmain.c
 * - 	gnirs/CC/python/Python/frozenmain.o
 * - 	gnirs/CC/python/Python/getargs.c
 * - 	gnirs/CC/python/Python/getargs.o
 * - 	gnirs/CC/python/Python/getcompiler.c
 * - 	gnirs/CC/python/Python/getcompiler.o
 * - 	gnirs/CC/python/Python/getcopyright.c
 * - 	gnirs/CC/python/Python/getcopyright.o
 * - 	gnirs/CC/python/Python/getcwd.c
 * - 	gnirs/CC/python/Python/getmtime.c
 * - 	gnirs/CC/python/Python/getmtime.o
 * - 	gnirs/CC/python/Python/getopt.c
 * - 	gnirs/CC/python/Python/getplatform.c
 * - 	gnirs/CC/python/Python/getplatform.o
 * - 	gnirs/CC/python/Python/getversion.c
 * - 	gnirs/CC/python/Python/getversion.o
 * - 	gnirs/CC/python/Python/graminit.c
 * - 	gnirs/CC/python/Python/graminit.o
 * - 	gnirs/CC/python/Python/hypot.c gnirs/CC/python/Python/import.c
 * - 	gnirs/CC/python/Python/import.o
 * - 	gnirs/CC/python/Python/importdl.c
 * - 	gnirs/CC/python/Python/importdl.h
 * - 	gnirs/CC/python/Python/importdl.o
 * - 	gnirs/CC/python/Python/marshal.c
 * - 	gnirs/CC/python/Python/marshal.o
 * - 	gnirs/CC/python/Python/memmove.c
 * - 	gnirs/CC/python/Python/modsupport.c
 * - 	gnirs/CC/python/Python/modsupport.o
 * - 	gnirs/CC/python/Python/mystrtoul.c
 * - 	gnirs/CC/python/Python/mystrtoul.o
 * - 	gnirs/CC/python/Python/pyfpe.c gnirs/CC/python/Python/pyfpe.o
 * - 	gnirs/CC/python/Python/pystate.c
 * - 	gnirs/CC/python/Python/pystate.o
 * - 	gnirs/CC/python/Python/pythonrun.c
 * - 	gnirs/CC/python/Python/pythonrun.o
 * - 	gnirs/CC/python/Python/sigcheck.c
 * - 	gnirs/CC/python/Python/sigcheck.o
 * - 	gnirs/CC/python/Python/strdup.c
 * - 	gnirs/CC/python/Python/strerror.c
 * - 	gnirs/CC/python/Python/strtod.c
 * - 	gnirs/CC/python/Python/structmember.c
 * - 	gnirs/CC/python/Python/structmember.o
 * - 	gnirs/CC/python/Python/sysmodule.c
 * - 	gnirs/CC/python/Python/sysmodule.o
 * - 	gnirs/CC/python/Python/thread.c
 * - 	gnirs/CC/python/Python/thread_beos.h
 * - 	gnirs/CC/python/Python/thread_cthread.h
 * - 	gnirs/CC/python/Python/thread_foobar.h
 * - 	gnirs/CC/python/Python/thread_lwp.h
 * - 	gnirs/CC/python/Python/thread_nt.h
 * - 	gnirs/CC/python/Python/thread_os2.h
 * - 	gnirs/CC/python/Python/thread_pthread.h
 * - 	gnirs/CC/python/Python/thread_sgi.h
 * - 	gnirs/CC/python/Python/thread_solaris.h
 * - 	gnirs/CC/python/Python/thread_wince.h
 * - 	gnirs/CC/python/Python/traceback.c
 * - 	gnirs/CC/python/Python/traceback.o
 * - 	gnirs/CC/python/sunos/Makefile
 * - 	gnirs/CC/python/sunos/config.cache
 * - 	gnirs/CC/python/sunos/config.h
 * - 	gnirs/CC/python/sunos/config.h.in
 * - 	gnirs/CC/python/sunos/config.log
 * - 	gnirs/CC/python/sunos/config.status
 * - 	gnirs/CC/python/sunos/configure
 * - 	gnirs/CC/python/sunos/configure.in gnirs/CC/src/Makefile
 * - 	gnirs/CC/src/Makefile.Unix gnirs/CC/src/Makefile.Vx
 * - 	gnirs/CC/startup/Makefile gnirs/CC/startup/Makefile.Unix
 * - 	gnirs/CC/startup/Makefile.Vx gnirs/CC/startup/UAE.dist
 * - 	gnirs/CC/startup/local.23.vws gnirs/CC/startup/local.seed.vws
 * - 	gnirs/CC/startup/local.vws gnirs/CC/startup/resource.def
 * - 	gnirs/CC/startup/resource.def.flex
 * - 	gnirs/CC/startup/resource.def.old
 * - 	gnirs/CC/startup/resource.seed.def
 * - 	gnirs/CC/startup/startup.CC.epics.vws
 * - 	gnirs/CC/startup/startup.python.vws
 * - 	gnirs/CC/startup/startup.seed.vws gnirs/CC/startup/startup.vws
 * - 	gnirs/CC/startup/O.mv167/.DEPENDS
 * - 	gnirs/CC/startup/O.mv167/Target.include
 * - 	gnirs/CC/startup/O.solaris/.DEPENDS
 * - 	gnirs/CC/startup/O.solaris/Target.include
 * - 	gnirs/CC/sys/Makefile gnirs/CC/sys/Makefile.Unix
 * - 	gnirs/CC/sys/Makefile.Vx gnirs/CC/sys/global/Makefile
 * - 	gnirs/CC/sys/global/Makefile.Unix
 * - 	gnirs/CC/sys/global/Makefile.Vx gnirs/CC/sys/global/ccGlobal.c
 * - 	gnirs/CC/sys/global/epicsCAInt.c
 * - 	gnirs/CC/sys/hdwrControl/Makefile
 * - 	gnirs/CC/sys/hdwrControl/Makefile.Unix
 * - 	gnirs/CC/sys/hdwrControl/Makefile.Vx
 * - 	gnirs/CC/sys/hdwrControl/commands.c
 * - 	gnirs/CC/sys/hdwrControl/currPos
 * - 	gnirs/CC/sys/hdwrControl/diags.c
 * - 	gnirs/CC/sys/hdwrControl/diags.txt
 * - 	gnirs/CC/sys/hdwrControl/digital.c
 * - 	gnirs/CC/sys/hdwrControl/globals.c
 * - 	gnirs/CC/sys/hdwrControl/intfc.c
 * - 	gnirs/CC/sys/hdwrControl/intfc.wfd
 * - 	gnirs/CC/sys/hdwrControl/lv.c gnirs/CC/sys/hdwrControl/mech.c
 * - 	gnirs/CC/sys/hdwrControl/motors.c
 * - 	gnirs/CC/sys/hdwrControl/senTorr.h
 * - 	gnirs/CC/sys/hdwrControl/senTorr.xc
 * - 	gnirs/CC/sys/hdwrControl/sockutil.c
 * - 	gnirs/CC/sys/hdwrControl/status gnirs/CC/sys/hdwrControl/tags
 * - 	gnirs/CC/sys/hdwrControl/tandp.c gnirs/CC/sys/hdwrControl/type
 * - 	gnirs/CC/sys/hdwrControl/util.c
 * - 	gnirs/CC/sys/hdwrControl/vxworks.py
 * - 	gnirs/CC/sys/hdwrControl/wfd.py
 * - 	gnirs/CC/sys/hdwrControl/wfdew.h
 * - 	gnirs/CC/sys/hdwrControl/CC/sys/hdwrControl/motors.c
 * - 	gnirs/CC/sys/hdwrControl/Former/commands.c
 * - 	gnirs/CC/sys/hdwrControl/Former/diags.c
 * - 	gnirs/CC/sys/hdwrControl/Former/digital.c
 * - 	gnirs/CC/sys/hdwrControl/Former/globals.c
 * - 	gnirs/CC/sys/hdwrControl/Former/gnirsCC.h
 * - 	gnirs/CC/sys/hdwrControl/Former/gnirsCommands.c
 * - 	gnirs/CC/sys/hdwrControl/Former/gnirsDiags.c
 * - 	gnirs/CC/sys/hdwrControl/Former/gnirsDigital.c
 * - 	gnirs/CC/sys/hdwrControl/Former/gnirsGlobals.c
 * - 	gnirs/CC/sys/hdwrControl/Former/gnirsIntfc.c
 * - 	gnirs/CC/sys/hdwrControl/Former/gnirsLv.c
 * - 	gnirs/CC/sys/hdwrControl/Former/gnirsMech.c
 * - 	gnirs/CC/sys/hdwrControl/Former/gnirsMotors.c
 * - 	gnirs/CC/sys/hdwrControl/Former/gnirsTandP.c
 * - 	gnirs/CC/sys/hdwrControl/Former/gnirsUtil.c
 * - 	gnirs/CC/sys/hdwrControl/Former/intfc.c
 * - 	gnirs/CC/sys/hdwrControl/Former/lv.c
 * - 	gnirs/CC/sys/hdwrControl/Former/mech.c
 * - 	gnirs/CC/sys/hdwrControl/Former/motors.c
 * - 	gnirs/CC/sys/hdwrControl/Former/senTorr.h
 * - 	gnirs/CC/sys/hdwrControl/Former/sockutil.c
 * - 	gnirs/CC/sys/hdwrControl/Former/tandp.c
 * - 	gnirs/CC/sys/hdwrControl/Former/util.c
 * - 	gnirs/CC/sys/hdwrControl/Former/wfdew.h
 * - 	gnirs/CC/sys/hdwrControl/Former/xc
 * - 	gnirs/CC/sys/hdwrControl/O.mv167/Makefile
 * - 	gnirs/CC/sys/hdwrControl/O.mv167/Target.include
 * - 	gnirs/CC/sys/hdwrControl/O.solaris/Makefile
 * - 	gnirs/CC/sys/hdwrControl/O.solaris/Target.include
 * - 	gnirs/CC/sys/hdwrControl/Roddier/V2T.c
 * - 	gnirs/CC/sys/hdwrControl/Roddier/devSupAiTmpMon.c
 * - 	gnirs/CC/sys/hdwrControl/Roddier/wfdew.c
 * - 	gnirs/CC/sys/hdwrControl/epicsDrv/drvXy240.c
 * - 	gnirs/CC/sys/hdwrControl/epicsDrv/drvXy240.epics
 * - 	gnirs/CC/sys/hdwrControl/keep/commands.c
 * - 	gnirs/CC/sys/hdwrControl/keep/diags.c
 * - 	gnirs/CC/sys/hdwrControl/keep/digital.c
 * - 	gnirs/CC/sys/hdwrControl/keep/globals.c
 * - 	gnirs/CC/sys/hdwrControl/keep/gnirsCC.h
 * - 	gnirs/CC/sys/hdwrControl/keep/intfc.c
 * - 	gnirs/CC/sys/hdwrControl/keep/lv.c
 * - 	gnirs/CC/sys/hdwrControl/keep/mech.c
 * - 	gnirs/CC/sys/hdwrControl/keep/motors.c
 * - 	gnirs/CC/sys/hdwrControl/keep/sockutil.c
 * - 	gnirs/CC/sys/hdwrControl/keep/tandp.c
 * - 	gnirs/CC/sys/hdwrControl/keep/util.c
 * - 	gnirs/CC/sys/hdwrControl/wfd/commands.wfd
 * - 	gnirs/CC/sys/hdwrControl/wfd/diags.wfd
 * - 	gnirs/CC/sys/hdwrControl/wfd/digital.wfd
 * - 	gnirs/CC/sys/hdwrControl/wfd/globals.wfd
 * - 	gnirs/CC/sys/hdwrControl/wfd/intfc.c
 * - 	gnirs/CC/sys/hdwrControl/wfd/intfc.wfd
 * - 	gnirs/CC/sys/hdwrControl/wfd/mech.wfd
 * - 	gnirs/CC/sys/hdwrControl/wfd/motors.wfd
 * - 	gnirs/CC/sys/hdwrControl/wfd/tandp.wfd
 * - 	gnirs/CC/sys/hdwrControl/wfd/util.wfd gnirs/DC/.cvsignore
 * - 	gnirs/DC/0README.txt gnirs/DC/IMP_Startup.guinevere
 * - 	gnirs/DC/IMP_Startup.juliet gnirs/DC/Makefile
 * - 	gnirs/DC/Makefile.subdirs gnirs/DC/README gnirs/DC/TODO_LIST
 * - 	gnirs/DC/globalsFiles.txt gnirs/DC/gnaacLogin
 * - 	gnirs/DC/gnaacLogin_IGPO gnirs/DC/gnaacLogin_NOAO
 * - 	gnirs/DC/gnaacSetup gnirs/DC/nirsSetup gnirs/DC/setup.gnaac
 * - 	gnirs/DC/test1 gnirs/DC/test2 gnirs/DC/annex/Makefile
 * - 	gnirs/DC/annex/Makefile.Unix gnirs/DC/annex/Makefile.Vx
 * - 	gnirs/DC/annex/gnReset.c gnirs/DC/annex/reset.h
 * - 	gnirs/DC/annex/reset.txt gnirs/DC/ascii/Makefile
 * - 	gnirs/DC/ascii/Makefile.Unix
 * - 	gnirs/DC/ascii/cat_ascii/choiceRec.ascii
 * - 	gnirs/DC/ascii/cat_ascii/choiceTcon.h
 * - 	gnirs/DC/ascii/cat_ascii/dbRecType.ascii
 * - 	gnirs/DC/ascii/cat_ascii/devSup.ascii
 * - 	gnirs/DC/ascii/cat_ascii/drvSup.ascii
 * - 	gnirs/DC/ascii/cat_ascii/tconRecord.ascii
 * - 	gnirs/DC/capfast/CBorder.sym gnirs/DC/capfast/Makefile
 * - 	gnirs/DC/capfast/Makefile.Unix
 * - 	gnirs/DC/capfast/SCRegCntrlTst.sch
 * - 	gnirs/DC/capfast/abortCad.sch gnirs/DC/capfast/abortCad.sym
 * - 	gnirs/DC/capfast/activChk.sch gnirs/DC/capfast/activChk.sym
 * - 	gnirs/DC/capfast/arSetupCad.sch
 * - 	gnirs/DC/capfast/arSetupCad.sym gnirs/DC/capfast/border.sch
 * - 	gnirs/DC/capfast/cad.rc gnirs/DC/capfast/cadCar.sch
 * - 	gnirs/DC/capfast/cadCar.sym gnirs/DC/capfast/common.sch
 * - 	gnirs/DC/capfast/common.sym gnirs/DC/capfast/continueCad.sch
 * - 	gnirs/DC/capfast/continueCad.sym gnirs/DC/capfast/dataSad.sch
 * - 	gnirs/DC/capfast/dataSad.sym gnirs/DC/capfast/dataSimul.sch
 * - 	gnirs/DC/capfast/dataSimul.sym
 * - 	gnirs/DC/capfast/dc32t+32vHlth.sch
 * - 	gnirs/DC/capfast/dc32t+32vHlth.sym
 * - 	gnirs/DC/capfast/dc64ChanHlth.sch
 * - 	gnirs/DC/capfast/dc64ChanHlth.sym
 * - 	gnirs/DC/capfast/debugCad.sch gnirs/DC/capfast/debugCad.sym
 * - 	gnirs/DC/capfast/diagFuncs.sch gnirs/DC/capfast/diagFuncs.sym
 * - 	gnirs/DC/capfast/dqDummy.sch gnirs/DC/capfast/drRoiCad.sch
 * - 	gnirs/DC/capfast/drRoiCad.sym gnirs/DC/capfast/edb.def
 * - 	gnirs/DC/capfast/esirs.sch gnirs/DC/capfast/etcons.sym
 * - 	gnirs/DC/capfast/f.sym gnirs/DC/capfast/hdwrNEngFuncs.sch
 * - 	gnirs/DC/capfast/hdwrNEngFuncs.sym
 * - 	gnirs/DC/capfast/hdwrSad.sch gnirs/DC/capfast/hdwrSad.sym
 * - 	gnirs/DC/capfast/healthCheck.sch
 * - 	gnirs/DC/capfast/healthCheck.sym
 * - 	gnirs/DC/capfast/heartBeat.sch gnirs/DC/capfast/heartBeat.sym
 * - 	gnirs/DC/capfast/hk16ChansHlth.sch
 * - 	gnirs/DC/capfast/hk16ChansHlth.sym
 * - 	gnirs/DC/capfast/hk16TempsHlth.sch
 * - 	gnirs/DC/capfast/hk16TempsHlth.sym
 * - 	gnirs/DC/capfast/iconRegs.sch gnirs/DC/capfast/iconRegs.sym
 * - 	gnirs/DC/capfast/initCad.sch gnirs/DC/capfast/initCad.sym
 * - 	gnirs/DC/capfast/makePostscript gnirs/DC/capfast/naacDc.sch
 * - 	gnirs/DC/capfast/naacDc.sym gnirs/DC/capfast/naacSad.sch
 * - 	gnirs/DC/capfast/naacSad.sym gnirs/DC/capfast/naacSadTop.sch
 * - 	gnirs/DC/capfast/naacTop.sch gnirs/DC/capfast/naacTop.sym
 * - 	gnirs/DC/capfast/newTempCntrl.sch
 * - 	gnirs/DC/capfast/newTempCntrl.sym
 * - 	gnirs/DC/capfast/niriSadTop.sch gnirs/DC/capfast/niriTop.sch
 * - 	gnirs/DC/capfast/nirsSadTop.sch gnirs/DC/capfast/nirsTop.sch
 * - 	gnirs/DC/capfast/noOpApply.sch gnirs/DC/capfast/noOpApply.sym
 * - 	gnirs/DC/capfast/noOpCar.sch gnirs/DC/capfast/noOpCar.sym
 * - 	gnirs/DC/capfast/noOpCmd.sch gnirs/DC/capfast/noOpCmd.sym
 * - 	gnirs/DC/capfast/noaoSadTop.sch gnirs/DC/capfast/noaoTop.sch
 * - 	gnirs/DC/capfast/notask.sch gnirs/DC/capfast/notask.sym
 * - 	gnirs/DC/capfast/notes.sch gnirs/DC/capfast/notes.sym
 * - 	gnirs/DC/capfast/obsSetupCad.sch
 * - 	gnirs/DC/capfast/obsSetupCad.sym
 * - 	gnirs/DC/capfast/observeCad.sch
 * - 	gnirs/DC/capfast/observeCad.sym gnirs/DC/capfast/parkCad.sch
 * - 	gnirs/DC/capfast/parkCad.sym gnirs/DC/capfast/pauseCad.sch
 * - 	gnirs/DC/capfast/pauseCad.sym gnirs/DC/capfast/rebootCad.sch
 * - 	gnirs/DC/capfast/rebootCad.sym gnirs/DC/capfast/seqVars.sch
 * - 	gnirs/DC/capfast/seqVars.sym gnirs/DC/capfast/setBias.sch
 * - 	gnirs/DC/capfast/setBias.sym
 * - 	gnirs/DC/capfast/setDhsInfoCad.sch
 * - 	gnirs/DC/capfast/setDhsInfoCad.sch.old
 * - 	gnirs/DC/capfast/setDhsInfoCad.sym
 * - 	gnirs/DC/capfast/setDrRoi.sch gnirs/DC/capfast/setDrRoi.sym
 * - 	gnirs/DC/capfast/setHdrVars.sch
 * - 	gnirs/DC/capfast/setHdrVars.sym
 * - 	gnirs/DC/capfast/setIntTime.sch
 * - 	gnirs/DC/capfast/setIntTime.sym
 * - 	gnirs/DC/capfast/setObsState.sch
 * - 	gnirs/DC/capfast/setObsState.sym
 * - 	gnirs/DC/capfast/setSeqROI.sch gnirs/DC/capfast/setSeqROI.sym
 * - 	gnirs/DC/capfast/setSeqVars.sch
 * - 	gnirs/DC/capfast/setSeqVars.sym
 * - 	gnirs/DC/capfast/setVoltages.sch
 * - 	gnirs/DC/capfast/setVoltages.sym
 * - 	gnirs/DC/capfast/setWcsCad.sch gnirs/DC/capfast/setWcsCad.sym
 * - 	gnirs/DC/capfast/simulMode.sch gnirs/DC/capfast/simulMode.sym
 * - 	gnirs/DC/capfast/stateApply.sch
 * - 	gnirs/DC/capfast/stateApply.sym gnirs/DC/capfast/stopCad.sch
 * - 	gnirs/DC/capfast/stopCad.sym gnirs/DC/capfast/sysApply.sch
 * - 	gnirs/DC/capfast/sysApply.sym gnirs/DC/capfast/sysCar.sch
 * - 	gnirs/DC/capfast/sysCar.sym gnirs/DC/capfast/sysSad.sch
 * - 	gnirs/DC/capfast/sysSad.sym gnirs/DC/capfast/task.sch
 * - 	gnirs/DC/capfast/task.sym gnirs/DC/capfast/tempBdCntrl.sch
 * - 	gnirs/DC/capfast/tempBdCntrl.sym
 * - 	gnirs/DC/capfast/tempBdDacRd.sch
 * - 	gnirs/DC/capfast/tempBdDacRd.sym
 * - 	gnirs/DC/capfast/tempBdExtra.sch
 * - 	gnirs/DC/capfast/tempBdExtra.sym
 * - 	gnirs/DC/capfast/tempBdReqrd.sch
 * - 	gnirs/DC/capfast/tempBdReqrd.sym
 * - 	gnirs/DC/capfast/tempBdSet.sch gnirs/DC/capfast/tempBdSet.sym
 * - 	gnirs/DC/capfast/testCad.sch gnirs/DC/capfast/testCad.sym
 * - 	gnirs/DC/capfast/tmpCar.sch gnirs/DC/capfast/tmpMech.sch
 * - 	gnirs/DC/capfast/tmpMech.sch.orig gnirs/DC/capfast/tmpMech.sym
 * - 	gnirs/DC/capfast/uCodeDwnLd.sch
 * - 	gnirs/DC/capfast/uCodeDwnLd.sym gnirs/DC/capfast/DC/cad.rc
 * - 	gnirs/DC/capfast/DC/capfast/cad.rc
 * - 	gnirs/DC/capfast/DC/capfast/hdwrNEngFuncs.sch
 * - 	gnirs/DC/cfitsio/Makefile gnirs/DC/cfitsio/Makefile.in
 * - 	gnirs/DC/cfitsio/ORIG.Makefile gnirs/DC/cfitsio/README
 * - 	gnirs/DC/cfitsio/cfileio.c gnirs/DC/cfitsio/cfitsio.doc
 * - 	gnirs/DC/cfitsio/cfitsio.ps gnirs/DC/cfitsio/cfitsio.tex
 * - 	gnirs/DC/cfitsio/cfitsio.toc gnirs/DC/cfitsio/cfitsio.txt
 * - 	gnirs/DC/cfitsio/changes.doc gnirs/DC/cfitsio/config.cache
 * - 	gnirs/DC/cfitsio/config.log gnirs/DC/cfitsio/config.status
 * - 	gnirs/DC/cfitsio/configure gnirs/DC/cfitsio/configure.in
 * - 	gnirs/DC/cfitsio/convert.c gnirs/DC/cfitsio/cookbook.c
 * - 	gnirs/DC/cfitsio/fitscore.c gnirs/DC/cfitsio/fitsio.h
 * - 	gnirs/DC/cfitsio/fitsio2.h gnirs/DC/cfitsio/getcol.c
 * - 	gnirs/DC/cfitsio/getkey.c gnirs/DC/cfitsio/libcfitsio.a
 * - 	gnirs/DC/cfitsio/longnam.h gnirs/DC/cfitsio/makealphavms.com
 * - 	gnirs/DC/cfitsio/makepc.bat gnirs/DC/cfitsio/makevaxvms.com
 * - 	gnirs/DC/cfitsio/modkey.c gnirs/DC/cfitsio/psplit
 * - 	gnirs/DC/cfitsio/psplit.c gnirs/DC/cfitsio/putcol.c
 * - 	gnirs/DC/cfitsio/putkey.c gnirs/DC/cfitsio/testprog.c
 * - 	gnirs/DC/cfitsio/testprog.out gnirs/DC/cfitsio/testprog.std
 * - 	gnirs/DC/cfitsio/utilproc.c gnirs/DC/cfitsio/vmsieeed.mar
 * - 	gnirs/DC/cfitsio/vmsieeer.mar gnirs/DC/dl/ErrorDisp.adl
 * - 	gnirs/DC/dl/GFCI.adl gnirs/DC/dl/Makefile
 * - 	gnirs/DC/dl/Makefile.Unix gnirs/DC/dl/ROICad.adl
 * - 	gnirs/DC/dl/ROICad.dl gnirs/DC/dl/adcHK.adl
 * - 	gnirs/DC/dl/adcHK.dl gnirs/DC/dl/adcHealth.adl
 * - 	gnirs/DC/dl/arSetupCad.adl gnirs/DC/dl/arrayCntrl.adl
 * - 	gnirs/DC/dl/cad0.adl gnirs/DC/dl/cad1.adl gnirs/DC/dl/cad2.adl
 * - 	gnirs/DC/dl/cad3.adl gnirs/DC/dl/cad9.adl
 * - 	gnirs/DC/dl/cadSad.adl gnirs/DC/dl/cadSad.dl
 * - 	gnirs/DC/dl/carVals.adl gnirs/DC/dl/carVals.dl
 * - 	gnirs/DC/dl/choice1Cad.adl gnirs/DC/dl/colors.dl
 * - 	gnirs/DC/dl/dataCube.adl gnirs/DC/dl/debugging.adl
 * - 	gnirs/DC/dl/defineROICad.adl gnirs/DC/dl/dhsCntrl.adl
 * - 	gnirs/DC/dl/dhsCntrl.adl.old gnirs/DC/dl/dhsSad.adl
 * - 	gnirs/DC/dl/diagCntrl.adl gnirs/DC/dl/dqHealth.adl
 * - 	gnirs/DC/dl/drRoiSetCad.adl gnirs/DC/dl/exposureSad.adl
 * - 	gnirs/DC/dl/gmColors.adl gnirs/DC/dl/gmColors.dl
 * - 	gnirs/DC/dl/gnaacEngDisp.adl gnirs/DC/dl/gnaacEngDisp.dl
 * - 	gnirs/DC/dl/gnaacStart gnirs/DC/dl/health.adl
 * - 	gnirs/DC/dl/healthSad.adl gnirs/DC/dl/list
 * - 	gnirs/DC/dl/mark.adl gnirs/DC/dl/mark.dl
 * - 	gnirs/DC/dl/motorSad.adl gnirs/DC/dl/motorSad.dl
 * - 	gnirs/DC/dl/motors.adl gnirs/DC/dl/motors.dl
 * - 	gnirs/DC/dl/obsSetupCad.adl gnirs/DC/dl/obsrvCntrl.adl
 * - 	gnirs/DC/dl/omega.adl gnirs/DC/dl/omega.dl
 * - 	gnirs/DC/dl/preAmpHK.adl gnirs/DC/dl/preAmpHK.dl
 * - 	gnirs/DC/dl/preAmpHealth.adl gnirs/DC/dl/preAmpHealth.dl
 * - 	gnirs/DC/dl/pwrSupHealth.adl gnirs/DC/dl/pwrSupHealth.dl
 * - 	gnirs/DC/dl/sad1.adl gnirs/DC/dl/sad2.adl gnirs/DC/dl/sad3.adl
 * - 	gnirs/DC/dl/sad4.adl gnirs/DC/dl/seqVarsCad.adl
 * - 	gnirs/DC/dl/setDebug.adl gnirs/DC/dl/setSim.adl
 * - 	gnirs/DC/dl/tempCntrl.adl gnirs/DC/dl/tempCntrl.dl
 * - 	gnirs/DC/dl/tempHealth.adl gnirs/DC/dl/template.adl
 * - 	gnirs/DC/dl/voltagesCad.adl gnirs/DC/dl/voltsHealth.adl
 * - 	gnirs/DC/dl/voltsHealth.dl gnirs/DC/dl/wFireCntrl.adl
 * - 	gnirs/DC/dl/wFireHealth.adl gnirs/DC/dl/wcsCad.adl
 * - 	gnirs/DC/dl/wcsCad.dl gnirs/DC/dl/DC/dl/gnaacEngDisp.adl
 * - 	gnirs/DC/dqSocket/Makefile gnirs/DC/dqSocket/Makefile.Unix
 * - 	gnirs/DC/dqSocket/Makefile.Vx
 * - 	gnirs/DC/dqSocket/dqSocketFiles.txt
 * - 	gnirs/DC/dqSocket/gnDQSocket.c
 * - 	gnirs/DC/dqSocket/AIII/gnSocketIntrfc.c
 * - 	gnirs/DC/dqSocket/tmp/gnDQSocket.c
 * - 	gnirs/DC/epicsCntrlSrc/0CHANGES.txt
 * - 	gnirs/DC/epicsCntrlSrc/0README.txt
 * - 	gnirs/DC/epicsCntrlSrc/Diagnose.c
 * - 	gnirs/DC/epicsCntrlSrc/Makefile
 * - 	gnirs/DC/epicsCntrlSrc/Makefile.Unix
 * - 	gnirs/DC/epicsCntrlSrc/Makefile.Vx
 * - 	gnirs/DC/epicsCntrlSrc/arSetupChk.c
 * - 	gnirs/DC/epicsCntrlSrc/clrError.c
 * - 	gnirs/DC/epicsCntrlSrc/combCars.c
 * - 	gnirs/DC/epicsCntrlSrc/dataSimCtrl.c
 * - 	gnirs/DC/epicsCntrlSrc/drRoiChk.c
 * - 	gnirs/DC/epicsCntrlSrc/epCommon.c
 * - 	gnirs/DC/epicsCntrlSrc/epicsCAInt.c
 * - 	gnirs/DC/epicsCntrlSrc/initTasks.c
 * - 	gnirs/DC/epicsCntrlSrc/initWcsCad.c
 * - 	gnirs/DC/epicsCntrlSrc/initWcsCtrl.c
 * - 	gnirs/DC/epicsCntrlSrc/monvars
 * - 	gnirs/DC/epicsCntrlSrc/naacGlobals.c
 * - 	gnirs/DC/epicsCntrlSrc/noopCad.c
 * - 	gnirs/DC/epicsCntrlSrc/obsSetupChk.c
 * - 	gnirs/DC/epicsCntrlSrc/recordList
 * - 	gnirs/DC/epicsCntrlSrc/sdsuWcs.c
 * - 	gnirs/DC/epicsCntrlSrc/simMode.c
 * - 	gnirs/DC/epicsCntrlSrc/stateCad.c
 * - 	gnirs/DC/epicsCntrlSrc/stateTasks.c
 * - 	gnirs/DC/epicsCntrlSrc/sysCad.c
 * - 	gnirs/DC/epicsCntrlSrc/sysReboot.c
 * - 	gnirs/DC/epicsCntrlSrc/sysTasks.c
 * - 	gnirs/DC/epicsCntrlSrc/testwcs.c
 * - 	gnirs/DC/epicsCntrlSrc/ucode1.cmd
 * - 	gnirs/DC/epicsCntrlSrc/ucode1.tld gnirs/DC/globals/Makefile
 * - 	gnirs/DC/globals/Makefile.Unix gnirs/DC/globals/Makefile.Vx
 * - 	gnirs/DC/globals/cicsLib.c gnirs/DC/globals/cicsLib2.c
 * - 	gnirs/DC/globals/coadGlobals.c gnirs/DC/globals/epicsGlobals.c
 * - 	gnirs/DC/globals/globals.c gnirs/DC/globals/globals.txt
 * - 	gnirs/DC/gnRpcSrc/Makefile gnirs/DC/gnRpcSrc/Makefile.Unix
 * - 	gnirs/DC/gnRpcSrc/Makefile.Vx gnirs/DC/gnRpcSrc/README
 * - 	gnirs/DC/gnRpcSrc/epdq.txt gnirs/DC/gnRpcSrc/epdq_clnt.c
 * - 	gnirs/DC/gnRpcSrc/epdq_xdr.c gnirs/DC/gnRpcSrc/gnRpcClient.c
 * - 	gnirs/DC/gnRpcSrc/gnRpcDHServer.c gnirs/DC/gnRpcSrc/rpc.txt
 * - 	gnirs/DC/gnRpcSrc/rpcFiles.txt gnirs/DC/include/DCA.h
 * - 	gnirs/DC/include/Link.h gnirs/DC/include/WFireMessage.h
 * - 	gnirs/DC/include/ansi.h gnirs/DC/include/b016.h
 * - 	gnirs/DC/include/b016reg.h gnirs/DC/include/bc350Time.h
 * - 	gnirs/DC/include/call_main.h gnirs/DC/include/choiceTcon.h
 * - 	gnirs/DC/include/cicsConst.h gnirs/DC/include/cicsLib.h
 * - 	gnirs/DC/include/client2.h gnirs/DC/include/coaddTest.h
 * - 	gnirs/DC/include/common.h gnirs/DC/include/dataHandling.h
 * - 	gnirs/DC/include/dcvx.h gnirs/DC/include/debug.h
 * - 	gnirs/DC/include/dpInt.h gnirs/DC/include/epCommon.h
 * - 	gnirs/DC/include/epTypedefs.h gnirs/DC/include/epdq.h
 * - 	gnirs/DC/include/epicsCA.h gnirs/DC/include/epicsCAint.h
 * - 	gnirs/DC/include/epicsDefines.h gnirs/DC/include/epicsDev.h
 * - 	gnirs/DC/include/fcio_b011.h gnirs/DC/include/fcio_defs.h
 * - 	gnirs/DC/include/fcio_link.h gnirs/DC/include/fcio_vars.h
 * - 	gnirs/DC/include/file gnirs/DC/include/fitsio.h
 * - 	gnirs/DC/include/gnDCADefs.h gnirs/DC/include/gnDCAVars.h
 * - 	gnirs/DC/include/gnDQSocket.h gnirs/DC/include/gnDQVars.h
 * - 	gnirs/DC/include/gnIpsDefs.h gnirs/DC/include/gnerrno.h
 * - 	gnirs/DC/include/header.h gnirs/DC/include/ierrorno.h
 * - 	gnirs/DC/include/ifaErrors.h gnirs/DC/include/imageHdr.h
 * - 	gnirs/DC/include/imhutils.h gnirs/DC/include/ims_bcmd.h
 * - 	gnirs/DC/include/inmos.h gnirs/DC/include/ipOctal.h
 * - 	gnirs/DC/include/irstd.h gnirs/DC/include/iserver.h
 * - 	gnirs/DC/include/link_sock.h gnirs/DC/include/localWcs.h
 * - 	gnirs/DC/include/longnam.h gnirs/DC/include/naacTasks.h
 * - 	gnirs/DC/include/noaoDCA.h gnirs/DC/include/nprotocol.h
 * - 	gnirs/DC/include/picture.h gnirs/DC/include/protdefs.h
 * - 	gnirs/DC/include/reboot.h gnirs/DC/include/regexp.h
 * - 	gnirs/DC/include/saveDefines.h gnirs/DC/include/saver.h
 * - 	gnirs/DC/include/saverCommon.h gnirs/DC/include/sbs915.h
 * - 	gnirs/DC/include/seqhdw.h gnirs/DC/include/shared.h
 * - 	gnirs/DC/include/sockutil.h gnirs/DC/include/svcontrol.h
 * - 	gnirs/DC/include/svserver.h gnirs/DC/include/taldef.h
 * - 	gnirs/DC/include/tcf.h gnirs/DC/include/tcl.h
 * - 	gnirs/DC/include/tcl.hsav gnirs/DC/include/tclHash.h
 * - 	gnirs/DC/include/tclUnix.h gnirs/DC/include/tcpExample.h
 * - 	gnirs/DC/include/telescope.h gnirs/DC/include/tkConfig.h
 * - 	gnirs/DC/include/util.h gnirs/DC/include/varlist
 * - 	gnirs/DC/include/vxSockUtil.h gnirs/DC/include/vximhutils.h
 * - 	gnirs/DC/include/wFireMsgDefs.h gnirs/DC/include/wcs.h
 * - 	gnirs/DC/include/AIII/epCommon.h
 * - 	gnirs/DC/include/AIII/gnDCAVars.h
 * - 	gnirs/DC/include/AIII/gnDQSocket.h
 * - 	gnirs/DC/include/rec/aaiRecord.h
 * - 	gnirs/DC/include/rec/aaoRecord.h
 * - 	gnirs/DC/include/rec/aiRecord.h
 * - 	gnirs/DC/include/rec/aoRecord.h
 * - 	gnirs/DC/include/rec/applyRecord.h
 * - 	gnirs/DC/include/rec/biRecord.h
 * - 	gnirs/DC/include/rec/boRecord.h
 * - 	gnirs/DC/include/rec/cadRecord.h
 * - 	gnirs/DC/include/rec/calcRecord.h
 * - 	gnirs/DC/include/rec/carRecord.h
 * - 	gnirs/DC/include/rec/compressRecord.h
 * - 	gnirs/DC/include/rec/dbCommon.h
 * - 	gnirs/DC/include/rec/dfanoutRecord.h
 * - 	gnirs/DC/include/rec/egRecord.h
 * - 	gnirs/DC/include/rec/egeventRecord.h
 * - 	gnirs/DC/include/rec/erRecord.h
 * - 	gnirs/DC/include/rec/ereventRecord.h
 * - 	gnirs/DC/include/rec/eventRecord.h
 * - 	gnirs/DC/include/rec/fanoutRecord.h
 * - 	gnirs/DC/include/rec/genSubRecord.h
 * - 	gnirs/DC/include/rec/histogramRecord.h
 * - 	gnirs/DC/include/rec/loadRecord.h
 * - 	gnirs/DC/include/rec/longinRecord.h
 * - 	gnirs/DC/include/rec/longoutRecord.h
 * - 	gnirs/DC/include/rec/lutinRecord.h
 * - 	gnirs/DC/include/rec/lutoutRecord.h
 * - 	gnirs/DC/include/rec/mbbiDirectRecord.h
 * - 	gnirs/DC/include/rec/mbbiRecord.h
 * - 	gnirs/DC/include/rec/mbboDirectRecord.h
 * - 	gnirs/DC/include/rec/mbboRecord.h
 * - 	gnirs/DC/include/rec/mosubRecord.h
 * - 	gnirs/DC/include/rec/motorRecord.h
 * - 	gnirs/DC/include/rec/permissiveRecord.h
 * - 	gnirs/DC/include/rec/pidRecord.h
 * - 	gnirs/DC/include/rec/pulseCounterRecord.h
 * - 	gnirs/DC/include/rec/pulseDelayRecord.h
 * - 	gnirs/DC/include/rec/pulseTrainRecord.h
 * - 	gnirs/DC/include/rec/scanRecord.h
 * - 	gnirs/DC/include/rec/selRecord.h
 * - 	gnirs/DC/include/rec/seqRecord.h
 * - 	gnirs/DC/include/rec/sirRecord.h
 * - 	gnirs/DC/include/rec/stateRecord.h
 * - 	gnirs/DC/include/rec/statusRecord.h
 * - 	gnirs/DC/include/rec/steppermotorRecord.h
 * - 	gnirs/DC/include/rec/stringinRecord.h
 * - 	gnirs/DC/include/rec/stringoutRecord.h
 * - 	gnirs/DC/include/rec/subArrayRecord.h
 * - 	gnirs/DC/include/rec/subCadRecord.h
 * - 	gnirs/DC/include/rec/subRecord.h
 * - 	gnirs/DC/include/rec/tconRecord.h
 * - 	gnirs/DC/include/rec/timerRecord.h
 * - 	gnirs/DC/include/rec/waitRecord.h
 * - 	gnirs/DC/include/rec/waveformRecord.h
 * - 	gnirs/DC/noaoCoAdder/coaddTest.c
 * - 	gnirs/DC/noaoCoAdder/crFitsHeaders.c
 * - 	gnirs/DC/noaoCoAdder/gnCaptBufs.c
 * - 	gnirs/DC/noaoCoAdder/gnTakeData.c
 * - 	gnirs/DC/noaoCoAdder/gnTestProcs.c
 * - 	gnirs/DC/noaoCoAdder/gnUtilFuncs.c
 * - 	gnirs/DC/noaoCoAdder/intRoutine.c gnirs/DC/omega/Makefile
 * - 	gnirs/DC/omega/Makefile.Unix gnirs/DC/omega/Makefile.Vx
 * - 	gnirs/DC/omega/devAiOcyc.c gnirs/DC/omega/devTconOcyc.c
 * - 	gnirs/DC/omega/drvOcyc.c gnirs/DC/omega/drvOcyc.h
 * - 	gnirs/DC/omega/recTcon.c gnirs/DC/omega/recTcon.h
 * - 	gnirs/DC/omega/DC/Makefile.subdirs
 * - 	gnirs/DC/omega/DC/ascii/cat_ascii/choiceRec.ascii
 * - 	gnirs/DC/omega/DC/ascii/cat_ascii/choiceTcon.h
 * - 	gnirs/DC/omega/DC/ascii/cat_ascii/dbRecType.ascii
 * - 	gnirs/DC/omega/DC/ascii/cat_ascii/devSup.ascii
 * - 	gnirs/DC/omega/DC/ascii/cat_ascii/drvSup.ascii
 * - 	gnirs/DC/omega/DC/ascii/cat_ascii/tconRecord.ascii
 * - 	gnirs/DC/omega/DC/capfast/tmpMech.sch
 * - 	gnirs/DC/omega/DC/capfast/tmpMech.sym
 * - 	gnirs/DC/omega/DC/dl/omega.adl
 * - 	gnirs/DC/omega/DC/dl/tempCntrl.adl
 * - 	gnirs/DC/omega/DC/dl/tempHealth.adl
 * - 	gnirs/DC/omega/DC/omega/Makefile
 * - 	gnirs/DC/omega/DC/omega/Makefile.Unix
 * - 	gnirs/DC/omega/DC/omega/Makefile.Vx
 * - 	gnirs/DC/omega/DC/omega/devAiOcyc.c
 * - 	gnirs/DC/omega/DC/omega/drvOcyc.c
 * - 	gnirs/DC/omega/DC/omega/drvOcyc.h
 * - 	gnirs/DC/omega/DC/omega/drvOcyd.c
 * - 	gnirs/DC/omega/DC/omega/drvOcyd.h
 * - 	gnirs/DC/omega/DC/omega/recTcon.c
 * - 	gnirs/DC/omega/DC/omega/recTcon.h
 * - 	gnirs/DC/omega/DC/pv/cadInitVals.pv
 * - 	gnirs/DC/omega/DC/startup/startup.romeo.flex.vws
 * - 	gnirs/DC/omega/DC/startup/startup.romeo.vws
 * - 	gnirs/DC/omega/DC/utilSource/healthChk/hk.c
 * - 	gnirs/DC/ppc/.cvsignore gnirs/DC/ppc/IMP_Startup.juliet
 * - 	gnirs/DC/ppc/IMP_Startup.juliet2 gnirs/DC/ppc/Makefile
 * - 	gnirs/DC/ppc/epicsGEM7.cshrc gnirs/DC/ppc/nirsPPCInstall
 * - 	gnirs/DC/ppc/dc-data/Makefile
 * - 	gnirs/DC/ppc/dc-data/Makefile.Dirs
 * - 	gnirs/DC/ppc/dc-data/bootChange.prev
 * - 	gnirs/DC/ppc/dc-data/checkup gnirs/DC/ppc/dc-data/ppcInstall
 * - 	gnirs/DC/ppc/dc-data/annex/Makefile
 * - 	gnirs/DC/ppc/dc-data/annex/Makefile.Host
 * - 	gnirs/DC/ppc/dc-data/annex/Makefile.Vx
 * - 	gnirs/DC/ppc/dc-data/annex/gnReset.c
 * - 	gnirs/DC/ppc/dc-data/annex/reset.h
 * - 	gnirs/DC/ppc/dc-data/dhs_temp/Makefile
 * - 	gnirs/DC/ppc/dc-data/dhs_temp/Makefile.Host
 * - 	gnirs/DC/ppc/dc-data/dhs_temp/Makefile.Vx
 * - 	gnirs/DC/ppc/dc-data/dhs_temp/Makefile.subdirs
 * - 	gnirs/DC/ppc/dc-data/dhs_temp/dcapi/Makefile
 * - 	gnirs/DC/ppc/dc-data/dhs_temp/dcapi/Makefile.Host
 * - 	gnirs/DC/ppc/dc-data/dhs_temp/dcapi/Makefile.Vx
 * - 	gnirs/DC/ppc/dc-data/dhs_temp/dcapi/dcsaver.c
 * - 	gnirs/DC/ppc/dc-data/dqSocket/Makefile
 * - 	gnirs/DC/ppc/dc-data/dqSocket/Makefile.Host
 * - 	gnirs/DC/ppc/dc-data/dqSocket/Makefile.Vx
 * - 	gnirs/DC/ppc/dc-data/dqSocket/dqSocketFiles.txt
 * - 	gnirs/DC/ppc/dc-data/dqSocket/gnSocketIntrfc.c
 * - 	gnirs/DC/ppc/dc-data/dqSocket/bak/gnSocketIntrfc.c
 * - 	gnirs/DC/ppc/dc-data/globals/Makefile
 * - 	gnirs/DC/ppc/dc-data/globals/Makefile.Host
 * - 	gnirs/DC/ppc/dc-data/globals/Makefile.Vx
 * - 	gnirs/DC/ppc/dc-data/globals/coadGlobals.c
 * - 	gnirs/DC/ppc/dc-data/globals/globals.c
 * - 	gnirs/DC/ppc/dc-data/globals/globals.txt
 * - 	gnirs/DC/ppc/dc-data/gnRpcSrc/Makefile
 * - 	gnirs/DC/ppc/dc-data/gnRpcSrc/Makefile.Host
 * - 	gnirs/DC/ppc/dc-data/gnRpcSrc/Makefile.Vx
 * - 	gnirs/DC/ppc/dc-data/gnRpcSrc/README
 * - 	gnirs/DC/ppc/dc-data/gnRpcSrc/epdq.txt
 * - 	gnirs/DC/ppc/dc-data/gnRpcSrc/epdq_svc.c
 * - 	gnirs/DC/ppc/dc-data/gnRpcSrc/gnRpcServer.c
 * - 	gnirs/DC/ppc/dc-data/gnRpcSrc/hi
 * - 	gnirs/DC/ppc/dc-data/gnRpcSrc/rpc.txt
 * - 	gnirs/DC/ppc/dc-data/gnRpcSrc/rpcFiles.txt
 * - 	gnirs/DC/ppc/dc-data/include/DCA.h
 * - 	gnirs/DC/ppc/dc-data/include/bc350Time.h
 * - 	gnirs/DC/ppc/dc-data/include/cicsConst.h
 * - 	gnirs/DC/ppc/dc-data/include/cicsLib.h
 * - 	gnirs/DC/ppc/dc-data/include/coaddTest.h
 * - 	gnirs/DC/ppc/dc-data/include/dataHandling.h
 * - 	gnirs/DC/ppc/dc-data/include/dcvx.h
 * - 	gnirs/DC/ppc/dc-data/include/debug.h
 * - 	gnirs/DC/ppc/dc-data/include/epCommon.h
 * - 	gnirs/DC/ppc/dc-data/include/epdq.h
 * - 	gnirs/DC/ppc/dc-data/include/fitsio.h
 * - 	gnirs/DC/ppc/dc-data/include/gnDCADefs.h
 * - 	gnirs/DC/ppc/dc-data/include/gnDCAVars.h
 * - 	gnirs/DC/ppc/dc-data/include/gnDQSocket.h
 * - 	gnirs/DC/ppc/dc-data/include/gnerrno.h
 * - 	gnirs/DC/ppc/dc-data/include/imageHdr.h
 * - 	gnirs/DC/ppc/dc-data/include/imhutils.h
 * - 	gnirs/DC/ppc/dc-data/include/irstd.h
 * - 	gnirs/DC/ppc/dc-data/include/localWcs.h
 * - 	gnirs/DC/ppc/dc-data/include/longnam.h
 * - 	gnirs/DC/ppc/dc-data/include/noaoDCA.h
 * - 	gnirs/DC/ppc/dc-data/include/saveDefines.h
 * - 	gnirs/DC/ppc/dc-data/include/saver.h
 * - 	gnirs/DC/ppc/dc-data/include/saverCommon.h
 * - 	gnirs/DC/ppc/dc-data/include/sockutil.h
 * - 	gnirs/DC/ppc/dc-data/include/vxSockUtil.h
 * - 	gnirs/DC/ppc/dc-data/include/bak/DCA.h
 * - 	gnirs/DC/ppc/dc-data/include/bak/bc350Time.h
 * - 	gnirs/DC/ppc/dc-data/include/bak/cicsConst.h
 * - 	gnirs/DC/ppc/dc-data/include/bak/cicsLib.h
 * - 	gnirs/DC/ppc/dc-data/include/bak/coaddTest.h
 * - 	gnirs/DC/ppc/dc-data/include/bak/dataHandling.h
 * - 	gnirs/DC/ppc/dc-data/include/bak/dcvx.h
 * - 	gnirs/DC/ppc/dc-data/include/bak/debug.h
 * - 	gnirs/DC/ppc/dc-data/include/bak/epCommon.h
 * - 	gnirs/DC/ppc/dc-data/include/bak/epdq.h
 * - 	gnirs/DC/ppc/dc-data/include/bak/fitsio.h
 * - 	gnirs/DC/ppc/dc-data/include/bak/gnDCADefs.h
 * - 	gnirs/DC/ppc/dc-data/include/bak/gnDCAVars.h
 * - 	gnirs/DC/ppc/dc-data/include/bak/gnDQSocket.h
 * - 	gnirs/DC/ppc/dc-data/include/bak/gnerrno.h
 * - 	gnirs/DC/ppc/dc-data/include/bak/imageHdr.h
 * - 	gnirs/DC/ppc/dc-data/include/bak/imhutils.h
 * - 	gnirs/DC/ppc/dc-data/include/bak/irstd.h
 * - 	gnirs/DC/ppc/dc-data/include/bak/localWcs.h
 * - 	gnirs/DC/ppc/dc-data/include/bak/longnam.h
 * - 	gnirs/DC/ppc/dc-data/include/bak/noaoDCA.h
 * - 	gnirs/DC/ppc/dc-data/include/bak/saveDefines.h
 * - 	gnirs/DC/ppc/dc-data/include/bak/saver.h
 * - 	gnirs/DC/ppc/dc-data/include/bak/saverCommon.h
 * - 	gnirs/DC/ppc/dc-data/include/bak/sockutil.h
 * - 	gnirs/DC/ppc/dc-data/include/bak/vxSockUtil.h
 * - 	gnirs/DC/ppc/dc-data/noaoCoAdder/Makefile
 * - 	gnirs/DC/ppc/dc-data/noaoCoAdder/Makefile.Host
 * - 	gnirs/DC/ppc/dc-data/noaoCoAdder/Makefile.Vx
 * - 	gnirs/DC/ppc/dc-data/noaoCoAdder/coaddTest.c
 * - 	gnirs/DC/ppc/dc-data/noaoCoAdder/crFitsHeaders.c
 * - 	gnirs/DC/ppc/dc-data/noaoCoAdder/dhs.c
 * - 	gnirs/DC/ppc/dc-data/noaoCoAdder/gnCaptBufs.c
 * - 	gnirs/DC/ppc/dc-data/noaoCoAdder/gnDCASetup.c
 * - 	gnirs/DC/ppc/dc-data/noaoCoAdder/gnDebugFuncs.c
 * - 	gnirs/DC/ppc/dc-data/noaoCoAdder/gnStartDCA.c
 * - 	gnirs/DC/ppc/dc-data/noaoCoAdder/gnStdSysInit.c
 * - 	gnirs/DC/ppc/dc-data/noaoCoAdder/gnTakeData.c
 * - 	gnirs/DC/ppc/dc-data/noaoCoAdder/gnTestProcs.c
 * - 	gnirs/DC/ppc/dc-data/noaoCoAdder/gnUtilFuncs.c
 * - 	gnirs/DC/ppc/dc-data/noaoCoAdder/intRoutine.c
 * - 	gnirs/DC/ppc/dc-data/noaoCoAdder/logMessage.c
 * - 	gnirs/DC/ppc/dc-data/noaoCoAdder/outline.c
 * - 	gnirs/DC/ppc/dc-data/noaoCoAdder/outline.c.movie.nowait.emptychunk
 * - 	gnirs/DC/ppc/dc-data/noaoCoAdder/outline.c.org
 * - 	gnirs/DC/ppc/dc-data/noaoCoAdder/bak/coaddTest.c
 * - 	gnirs/DC/ppc/dc-data/noaoCoAdder/bak/crFitsHeaders.c
 * - 	gnirs/DC/ppc/dc-data/noaoCoAdder/bak/dhs.c
 * - 	gnirs/DC/ppc/dc-data/noaoCoAdder/bak/gnCaptBufs.c
 * - 	gnirs/DC/ppc/dc-data/noaoCoAdder/bak/gnDCASetup.c
 * - 	gnirs/DC/ppc/dc-data/noaoCoAdder/bak/gnDebugFuncs.c
 * - 	gnirs/DC/ppc/dc-data/noaoCoAdder/bak/gnStartDCA.c
 * - 	gnirs/DC/ppc/dc-data/noaoCoAdder/bak/gnStdSysInit.c
 * - 	gnirs/DC/ppc/dc-data/noaoCoAdder/bak/gnTakeData.c
 * - 	gnirs/DC/ppc/dc-data/noaoCoAdder/bak/gnTestProcs.c
 * - 	gnirs/DC/ppc/dc-data/noaoCoAdder/bak/gnUtilFuncs.c
 * - 	gnirs/DC/ppc/dc-data/noaoCoAdder/bak/intRoutine.c
 * - 	gnirs/DC/ppc/dc-data/noaoCoAdder/bak/logMessage.c
 * - 	gnirs/DC/ppc/dc-data/noaoCoAdder/bak/outline.c
 * - 	gnirs/DC/ppc/dc-data/startup/IMP_Startup.juliet
 * - 	gnirs/DC/ppc/dc-data/startup/IMP_Startup.mko-dc-data-niri
 * - 	gnirs/DC/ppc/dc-data/startup/Makefile
 * - 	gnirs/DC/ppc/dc-data/startup/Makefile.Host
 * - 	gnirs/DC/ppc/dc-data/startup/Makefile.Vx
 * - 	gnirs/DC/ppc/dc-data/startup/calist
 * - 	gnirs/DC/ppc/dc-data/startup/local.flex.vws
 * - 	gnirs/DC/ppc/dc-data/startup/local.vws.HI
 * - 	gnirs/DC/ppc/dc-data/startup/startup.flex.vws
 * - 	gnirs/DC/ppc/dc-data/startup/startup.minnie-MK-dhst2.0.vws
 * - 	gnirs/DC/ppc/dc-data/startup/startup.minnie-MK.vws
 * - 	gnirs/DC/ppc/dc-data/startup/startup.minnie-dhs-test.vws
 * - 	gnirs/DC/ppc/dc-data/startup/startup.minnie-dhs.vws
 * - 	gnirs/DC/ppc/dc-data/startup/startup.minnie-dhsArchie.vws
 * - 	gnirs/DC/ppc/dc-data/startup/startup.minnie-dhsBetty-v17.vws
 * - 	gnirs/DC/ppc/dc-data/startup/startup.minnie-dhsBetty.vws
 * - 	gnirs/DC/ppc/dc-data/startup/startup.minnie-hbf.vws
 * - 	gnirs/DC/ppc/dc-data/startup/startup.minnie-mk.vws
 * - 	gnirs/DC/ppc/dc-data/startup/startup.minnie-mk2-hostlist.vws
 * - 	gnirs/DC/ppc/dc-data/startup/startup.minnie-mk2-netmask.vws
 * - 	gnirs/DC/ppc/dc-data/startup/startup.minnie-mk2.vws
 * - 	gnirs/DC/ppc/dc-data/startup/startup.minnie.vws
 * - 	gnirs/DC/ppc/dc-data/startup/startup.minnieMinimal-MK.vws
 * - 	gnirs/DC/ppc/dc-data/startup/startup.socket
 * - 	gnirs/DC/ppc/dc-data/startup/startup.socket.vws
 * - 	gnirs/DC/ppc/dc-data/startup/startup.vws
 * - 	gnirs/DC/ppc/dc-data/startup/bak/startup.minnie-MK-dhst2.0.vws
 * - 	gnirs/DC/ppc/dc-data/startup/bak/startup.minnie-MK.vws
 * - 	gnirs/DC/ppc/dc-data/startup/bak/startup.minnie-dhs-test.vws
 * - 	gnirs/DC/ppc/dc-data/startup/bak/startup.minnie-dhs.vws
 * - 	gnirs/DC/ppc/dc-data/startup/bak/startup.minnie-dhsArchie.vws
 * - 	gnirs/DC/ppc/dc-data/startup/bak/startup.minnie-dhsBetty-v17.vws
 * - 	gnirs/DC/ppc/dc-data/startup/bak/startup.minnie-dhsBetty.vws
 * - 	gnirs/DC/ppc/dc-data/startup/bak/startup.minnie-hbf.vws
 * - 	gnirs/DC/ppc/dc-data/startup/bak/startup.minnie-mk.vws
 * - 	gnirs/DC/ppc/dc-data/startup/bak/startup.minnie-mk2-hostlist.vws
 * - 	gnirs/DC/ppc/dc-data/startup/bak/startup.minnie-mk2-netmask.vws
 * - 	gnirs/DC/ppc/dc-data/startup/bak/startup.minnie-mk2.vws
 * - 	gnirs/DC/ppc/dc-data/startup/bak/startup.minnie.vws
 * - 	gnirs/DC/ppc/dc-data/startup/bak/startup.minnieMinimal-MK.vws
 * - 	gnirs/DC/ppc/dc-data/startup/bak/startup.vws
 * - 	gnirs/DC/ppc/dc-data/utilSource/Makefile
 * - 	gnirs/DC/ppc/dc-data/utilSource/Makefile.Dirs
 * - 	gnirs/DC/ppc/dc-data/utilSource/socket/Makefile
 * - 	gnirs/DC/ppc/dc-data/utilSource/socket/Makefile.Host
 * - 	gnirs/DC/ppc/dc-data/utilSource/socket/Makefile.Vx
 * - 	gnirs/DC/ppc/dc-data/utilSource/socket/sockutil.c
 * - 	gnirs/DC/ppc/dc-data/utilSource/time/Makefile
 * - 	gnirs/DC/ppc/dc-data/utilSource/time/Makefile.Host
 * - 	gnirs/DC/ppc/dc-data/utilSource/time/Makefile.Vx
 * - 	gnirs/DC/ppc/dc-data/utilSource/time/bc350time.ORG
 * - 	gnirs/DC/ppc/dc-data/utilSource/time/bc350time.c
 * - 	gnirs/DC/ppc/dc-data/utilSource/time/bc350time.c_orig
 * - 	gnirs/DC/ppc/dc-data/utilSource/time/getTime.c
 * - 	gnirs/DC/ppc/dc-data/utilSource/time/time.txt
 * - 	gnirs/DC/ppc/dc-data/utilSource/time/timeFiles.txt
 * - 	gnirs/DC/ppc/dc-data/vxworks/vxWorks
 * - 	gnirs/DC/ppc/dc-data/vxworks/vxWorks.sym
 * - 	gnirs/DC/ppc/solaris/lib/libbfd.a
 * - 	gnirs/DC/ppc/solaris/lib/libbfd.la
 * - 	gnirs/DC/ppc/solaris/lib/libhistory.a
 * - 	gnirs/DC/ppc/solaris/lib/libiberty.a
 * - 	gnirs/DC/ppc/solaris/lib/libmmalloc.a
 * - 	gnirs/DC/ppc/solaris/lib/libopcodes.a
 * - 	gnirs/DC/ppc/solaris/lib/libopcodes.la
 * - 	gnirs/DC/ppc/solaris/lib/libreadline.a
 * - 	gnirs/DC/ppc/solaris/lib/libstdc++.a.2.10.0
 * - 	gnirs/DC/ppc/solaris/lib/libstdc++.so.2.10.0
 * - 	gnirs/DC/ppc/solaris/lib/gcc-lib/sparc-sun-solaris2.8/2.95.3/SYSCALLS.c.X
 * - 	gnirs/DC/ppc/solaris/lib/gcc-lib/sparc-sun-solaris2.8/2.95.3/cc1
 * - 	gnirs/DC/ppc/solaris/lib/gcc-lib/sparc-sun-solaris2.8/2.95.3/cc1chill
 * - 	gnirs/DC/ppc/solaris/lib/gcc-lib/sparc-sun-solaris2.8/2.95.3/cc1obj
 * - 	gnirs/DC/ppc/solaris/lib/gcc-lib/sparc-sun-solaris2.8/2.95.3/cc1plus
 * - 	gnirs/DC/ppc/solaris/lib/gcc-lib/sparc-sun-solaris2.8/2.95.3/chillrt0.o
 * - 	gnirs/DC/ppc/solaris/lib/gcc-lib/sparc-sun-solaris2.8/2.95.3/collect2
 * - 	gnirs/DC/ppc/solaris/lib/gcc-lib/sparc-sun-solaris2.8/2.95.3/cpp0
 * - 	gnirs/DC/ppc/solaris/lib/gcc-lib/sparc-sun-solaris2.8/2.95.3/crt1.o
 * - 	gnirs/DC/ppc/solaris/lib/gcc-lib/sparc-sun-solaris2.8/2.95.3/crtbegin.o
 * - 	gnirs/DC/ppc/solaris/lib/gcc-lib/sparc-sun-solaris2.8/2.95.3/crtend.o
 * - 	gnirs/DC/ppc/solaris/lib/gcc-lib/sparc-sun-solaris2.8/2.95.3/crti.o
 * - 	gnirs/DC/ppc/solaris/lib/gcc-lib/sparc-sun-solaris2.8/2.95.3/crtn.o
 * - 	gnirs/DC/ppc/solaris/lib/gcc-lib/sparc-sun-solaris2.8/2.95.3/f771
 * - 	gnirs/DC/ppc/solaris/lib/gcc-lib/sparc-sun-solaris2.8/2.95.3/gcrt1.o
 * - 	gnirs/DC/ppc/solaris/lib/gcc-lib/sparc-sun-solaris2.8/2.95.3/gmon.o
 * - 	gnirs/DC/ppc/solaris/lib/gcc-lib/sparc-sun-solaris2.8/2.95.3/jc1
 * - 	gnirs/DC/ppc/solaris/lib/gcc-lib/sparc-sun-solaris2.8/2.95.3/jvgenmain
 * - 	gnirs/DC/ppc/solaris/lib/gcc-lib/sparc-sun-solaris2.8/2.95.3/libchill.a
 * - 	gnirs/DC/ppc/solaris/lib/gcc-lib/sparc-sun-solaris2.8/2.95.3/libg2c.a
 * - 	gnirs/DC/ppc/solaris/lib/gcc-lib/sparc-sun-solaris2.8/2.95.3/libgcc.a
 * - 	gnirs/DC/ppc/solaris/lib/gcc-lib/sparc-sun-solaris2.8/2.95.3/libobjc.a
 * - 	gnirs/DC/ppc/solaris/lib/gcc-lib/sparc-sun-solaris2.8/2.95.3/specs
 * - 	gnirs/DC/ppc/solaris/lib/gcc-lib/sparc-sun-solaris2.8/2.95.3/include/README
 * - 	gnirs/DC/ppc/solaris/lib/gcc-lib/sparc-sun-solaris2.8/2.95.3/include/assert.h
 * - 	gnirs/DC/ppc/solaris/lib/gcc-lib/sparc-sun-solaris2.8/2.95.3/include/curses.h
 * - 	gnirs/DC/ppc/solaris/lib/gcc-lib/sparc-sun-solaris2.8/2.95.3/include/exception
 * - 	gnirs/DC/ppc/solaris/lib/gcc-lib/sparc-sun-solaris2.8/2.95.3/include/g2c.h
 * - 	gnirs/DC/ppc/solaris/lib/gcc-lib/sparc-sun-solaris2.8/2.95.3/include/iso646.h
 * - 	gnirs/DC/ppc/solaris/lib/gcc-lib/sparc-sun-solaris2.8/2.95.3/include/limits.h
 * - 	gnirs/DC/ppc/solaris/lib/gcc-lib/sparc-sun-solaris2.8/2.95.3/include/math.h
 * - 	gnirs/DC/ppc/solaris/lib/gcc-lib/sparc-sun-solaris2.8/2.95.3/include/new
 * - 	gnirs/DC/ppc/solaris/lib/gcc-lib/sparc-sun-solaris2.8/2.95.3/include/new.h
 * - 	gnirs/DC/ppc/solaris/lib/gcc-lib/sparc-sun-solaris2.8/2.95.3/include/proto.h
 * - 	gnirs/DC/ppc/solaris/lib/gcc-lib/sparc-sun-solaris2.8/2.95.3/include/stdarg.h
 * - 	gnirs/DC/ppc/solaris/lib/gcc-lib/sparc-sun-solaris2.8/2.95.3/include/stdbool.h
 * - 	gnirs/DC/ppc/solaris/lib/gcc-lib/sparc-sun-solaris2.8/2.95.3/include/stddef.h
 * - 	gnirs/DC/ppc/solaris/lib/gcc-lib/sparc-sun-solaris2.8/2.95.3/include/syslimits.h
 * - 	gnirs/DC/ppc/solaris/lib/gcc-lib/sparc-sun-solaris2.8/2.95.3/include/typeinfo
 * - 	gnirs/DC/ppc/solaris/lib/gcc-lib/sparc-sun-solaris2.8/2.95.3/include/va-alpha.h
 * - 	gnirs/DC/ppc/solaris/lib/gcc-lib/sparc-sun-solaris2.8/2.95.3/include/va-arc.h
 * - 	gnirs/DC/ppc/solaris/lib/gcc-lib/sparc-sun-solaris2.8/2.95.3/include/va-c4x.h
 * - 	gnirs/DC/ppc/solaris/lib/gcc-lib/sparc-sun-solaris2.8/2.95.3/include/va-clipper.h
 * - 	gnirs/DC/ppc/solaris/lib/gcc-lib/sparc-sun-solaris2.8/2.95.3/include/va-h8300.h
 * - 	gnirs/DC/ppc/solaris/lib/gcc-lib/sparc-sun-solaris2.8/2.95.3/include/va-i860.h
 * - 	gnirs/DC/ppc/solaris/lib/gcc-lib/sparc-sun-solaris2.8/2.95.3/include/va-i960.h
 * - 	gnirs/DC/ppc/solaris/lib/gcc-lib/sparc-sun-solaris2.8/2.95.3/include/va-m32r.h
 * - 	gnirs/DC/ppc/solaris/lib/gcc-lib/sparc-sun-solaris2.8/2.95.3/include/va-m88k.h
 * - 	gnirs/DC/ppc/solaris/lib/gcc-lib/sparc-sun-solaris2.8/2.95.3/include/va-mips.h
 * - 	gnirs/DC/ppc/solaris/lib/gcc-lib/sparc-sun-solaris2.8/2.95.3/include/va-mn10200.h
 * - 	gnirs/DC/ppc/solaris/lib/gcc-lib/sparc-sun-solaris2.8/2.95.3/include/va-mn10300.h
 * - 	gnirs/DC/ppc/solaris/lib/gcc-lib/sparc-sun-solaris2.8/2.95.3/include/va-pa.h
 * - 	gnirs/DC/ppc/solaris/lib/gcc-lib/sparc-sun-solaris2.8/2.95.3/include/va-ppc.h
 * - 	gnirs/DC/ppc/solaris/lib/gcc-lib/sparc-sun-solaris2.8/2.95.3/include/va-pyr.h
 * - 	gnirs/DC/ppc/solaris/lib/gcc-lib/sparc-sun-solaris2.8/2.95.3/include/va-sh.h
 * - 	gnirs/DC/ppc/solaris/lib/gcc-lib/sparc-sun-solaris2.8/2.95.3/include/va-sparc.h
 * - 	gnirs/DC/ppc/solaris/lib/gcc-lib/sparc-sun-solaris2.8/2.95.3/include/va-spur.h
 * - 	gnirs/DC/ppc/solaris/lib/gcc-lib/sparc-sun-solaris2.8/2.95.3/include/va-v850.h
 * - 	gnirs/DC/ppc/solaris/lib/gcc-lib/sparc-sun-solaris2.8/2.95.3/include/varargs.h
 * - 	gnirs/DC/ppc/solaris/lib/gcc-lib/sparc-sun-solaris2.8/2.95.3/include/objc/NXConstStr.h
 * - 	gnirs/DC/ppc/solaris/lib/gcc-lib/sparc-sun-solaris2.8/2.95.3/include/objc/Object.h
 * - 	gnirs/DC/ppc/solaris/lib/gcc-lib/sparc-sun-solaris2.8/2.95.3/include/objc/Protocol.h
 * - 	gnirs/DC/ppc/solaris/lib/gcc-lib/sparc-sun-solaris2.8/2.95.3/include/objc/encoding.h
 * - 	gnirs/DC/ppc/solaris/lib/gcc-lib/sparc-sun-solaris2.8/2.95.3/include/objc/hash.h
 * - 	gnirs/DC/ppc/solaris/lib/gcc-lib/sparc-sun-solaris2.8/2.95.3/include/objc/objc-api.h
 * - 	gnirs/DC/ppc/solaris/lib/gcc-lib/sparc-sun-solaris2.8/2.95.3/include/objc/objc-list.h
 * - 	gnirs/DC/ppc/solaris/lib/gcc-lib/sparc-sun-solaris2.8/2.95.3/include/objc/objc.h
 * - 	gnirs/DC/ppc/solaris/lib/gcc-lib/sparc-sun-solaris2.8/2.95.3/include/objc/sarray.h
 * - 	gnirs/DC/ppc/solaris/lib/gcc-lib/sparc-sun-solaris2.8/2.95.3/include/objc/thr.h
 * - 	gnirs/DC/ppc/solaris/lib/gcc-lib/sparc-sun-solaris2.8/2.95.3/include/objc/typedstream.h
 * - 	gnirs/DC/ppc/solaris/lib/gcc-lib/sparc-sun-solaris2.8/2.95.3/include/sys/stream.h
 * - 	gnirs/DC/ppc/solaris/lib/locale/cs/LC_MESSAGES/tar.mo
 * - 	gnirs/DC/ppc/solaris/lib/locale/de/LC_MESSAGES/tar.mo
 * - 	gnirs/DC/ppc/solaris/lib/locale/es/LC_MESSAGES/tar.mo
 * - 	gnirs/DC/ppc/solaris/lib/locale/fr/LC_MESSAGES/tar.mo
 * - 	gnirs/DC/ppc/solaris/lib/locale/it/LC_MESSAGES/tar.mo
 * - 	gnirs/DC/ppc/solaris/lib/locale/ko/LC_MESSAGES/tar.mo
 * - 	gnirs/DC/ppc/solaris/lib/locale/nl/LC_MESSAGES/tar.mo
 * - 	gnirs/DC/ppc/solaris/lib/locale/no/LC_MESSAGES/tar.mo
 * - 	gnirs/DC/ppc/solaris/lib/locale/pl/LC_MESSAGES/tar.mo
 * - 	gnirs/DC/ppc/solaris/lib/locale/pt/LC_MESSAGES/tar.mo
 * - 	gnirs/DC/ppc/solaris/lib/locale/ru/LC_MESSAGES/tar.mo
 * - 	gnirs/DC/ppc/solaris/lib/locale/sl/LC_MESSAGES/tar.mo
 * - 	gnirs/DC/ppc/solaris/lib/locale/sv/LC_MESSAGES/tar.mo
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/AnyDBM_File.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/AutoLoader.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/AutoSplit.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/Benchmark.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/CGI.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/CPAN.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/Carp.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/Cwd.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/DB.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/DirHandle.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/Dumpvalue.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/English.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/Env.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/Exporter.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/Fatal.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/FileCache.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/FileHandle.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/FindBin.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/SelectSaver.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/SelfLoader.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/Shell.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/Symbol.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/Test.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/UNIVERSAL.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/Win32.pod
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/abbrev.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/assert.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/attributes.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/autouse.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/base.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/bigfloat.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/bigint.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/bigrat.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/blib.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/bytes.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/bytes_heavy.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/cacheout.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/charnames.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/chat2.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/complete.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/constant.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/ctime.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/diagnostics.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/dotsh.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/dumpvar.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/exceptions.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/fastcwd.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/fields.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/filetest.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/find.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/finddepth.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/flush.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/ftp.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/getcwd.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/getopt.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/getopts.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/hostname.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/importenv.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/integer.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/less.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/lib.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/locale.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/look.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/newgetopt.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/open.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/open2.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/open3.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/overload.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/perl5db.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/pwd.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/shellwords.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sigtrap.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/stat.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/strict.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/subs.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/syslog.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/tainted.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/termcap.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/timelocal.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/utf8.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/utf8_heavy.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/validate.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/vars.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/warnings.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/B/assemble
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/B/cc_harness
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/B/disassemble
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/B/makeliblinks
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/CGI/Apache.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/CGI/Carp.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/CGI/Cookie.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/CGI/Fast.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/CGI/Pretty.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/CGI/Push.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/CGI/Switch.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/CGI/Util.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/CPAN/FirstTime.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/CPAN/Nox.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/Carp/Heavy.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/Class/Struct.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/Devel/SelfStubber.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/Exporter/Heavy.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/ExtUtils/Command.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/ExtUtils/Embed.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/ExtUtils/Install.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/ExtUtils/Installed.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/ExtUtils/Liblist.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/ExtUtils/MM_Cygwin.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/ExtUtils/MM_OS2.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/ExtUtils/MM_Unix.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/ExtUtils/MM_VMS.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/ExtUtils/MM_Win32.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/ExtUtils/MakeMaker.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/ExtUtils/Manifest.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/ExtUtils/Miniperl.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/ExtUtils/Mkbootstrap.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/ExtUtils/Mksymlists.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/ExtUtils/Packlist.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/ExtUtils/inst
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/ExtUtils/testlib.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/ExtUtils/typemap
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/ExtUtils/xsubpp
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/File/Basename.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/File/CheckTree.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/File/Compare.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/File/Copy.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/File/DosGlob.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/File/Find.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/File/Path.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/File/Spec.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/File/Temp.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/File/stat.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/File/Spec/Epoc.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/File/Spec/Functions.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/File/Spec/Mac.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/File/Spec/OS2.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/File/Spec/Unix.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/File/Spec/VMS.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/File/Spec/Win32.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/Getopt/Long.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/Getopt/Std.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/I18N/Collate.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/IO/Socket/INET.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/IO/Socket/UNIX.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/IPC/Open2.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/IPC/Open3.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/Math/BigFloat.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/Math/BigInt.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/Math/Complex.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/Math/Trig.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/Net/Ping.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/Net/hostent.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/Net/netent.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/Net/protoent.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/Net/servent.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/Pod/Checker.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/Pod/Find.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/Pod/Functions.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/Pod/Html.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/Pod/InputObjects.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/Pod/LaTeX.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/Pod/Man.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/Pod/ParseUtils.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/Pod/Parser.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/Pod/Plainer.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/Pod/Select.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/Pod/Text.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/Pod/Usage.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/Pod/Text/Color.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/Pod/Text/Overstrike.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/Pod/Text/Termcap.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/Search/Dict.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/Term/ANSIColor.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/Term/Cap.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/Term/Complete.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/Term/ReadLine.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/Test/Harness.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/Text/Abbrev.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/Text/ParseWords.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/Text/Soundex.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/Text/Tabs.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/Text/Wrap.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/Tie/Array.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/Tie/Handle.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/Tie/Hash.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/Tie/RefHash.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/Tie/Scalar.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/Tie/SubstrHash.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/Time/Local.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/Time/gmtime.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/Time/localtime.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/Time/tm.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/User/grent.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/User/pwent.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/auto/Getopt/Long/Configure.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/auto/Getopt/Long/Croak.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/auto/Getopt/Long/FindOption.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/auto/Getopt/Long/GetOptions.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/auto/Getopt/Long/autosplit.ix
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/auto/Getopt/Long/config.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/pod/perl.pod
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/pod/perl5004delta.pod
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/pod/perl5005delta.pod
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/pod/perlaix.pod
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/pod/perlamiga.pod
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/pod/perlapi.pod
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/pod/perlapio.pod
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/pod/perlbook.pod
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/pod/perlboot.pod
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/pod/perlbot.pod
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/pod/perlbs2000.pod
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/pod/perlcall.pod
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/pod/perlclib.pod
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/pod/perlcompile.pod
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/pod/perlcygwin.pod
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/pod/perldata.pod
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/pod/perldbmfilter.pod
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/pod/perldebguts.pod
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/pod/perldebtut.pod
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/pod/perldebug.pod
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/pod/perldelta.pod
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/pod/perldiag.pod
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/pod/perldos.pod
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/pod/perldsc.pod
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/pod/perlebcdic.pod
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/pod/perlembed.pod
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/pod/perlepoc.pod
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/pod/perlfaq.pod
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/pod/perlfaq1.pod
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/pod/perlfaq2.pod
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/pod/perlfaq3.pod
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/pod/perlfaq4.pod
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/pod/perlfaq5.pod
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/pod/perlfaq6.pod
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/pod/perlfaq7.pod
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/pod/perlfaq8.pod
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/pod/perlfaq9.pod
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/pod/perlfilter.pod
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/pod/perlfork.pod
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/pod/perlform.pod
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/pod/perlfunc.pod
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/pod/perlguts.pod
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/pod/perlhack.pod
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/pod/perlhist.pod
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/pod/perlhpux.pod
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/pod/perlintern.pod
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/pod/perlipc.pod
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/pod/perllexwarn.pod
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/pod/perllocale.pod
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/pod/perllol.pod
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/pod/perlmachten.pod
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/pod/perlmacos.pod
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/pod/perlmod.pod
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/pod/perlmodinstall.pod
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/pod/perlmodlib.pod
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/pod/perlmpeix.pod
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/pod/perlnewmod.pod
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/pod/perlnumber.pod
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/pod/perlobj.pod
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/pod/perlop.pod
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/pod/perlopentut.pod
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/pod/perlos2.pod
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/pod/perlos390.pod
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/pod/perlpod.pod
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/pod/perlport.pod
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/pod/perlre.pod
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/pod/perlref.pod
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/pod/perlreftut.pod
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/pod/perlrequick.pod
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/pod/perlretut.pod
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/pod/perlrun.pod
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/pod/perlsec.pod
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/pod/perlsolaris.pod
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/pod/perlstyle.pod
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/pod/perlsub.pod
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/pod/perlsyn.pod
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/pod/perlthrtut.pod
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/pod/perltie.pod
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/pod/perltoc.pod
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/pod/perltodo.pod
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/pod/perltoot.pod
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/pod/perltootc.pod
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/pod/perltrap.pod
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/pod/perlunicode.pod
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/pod/perlutil.pod
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/pod/perlvar.pod
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/pod/perlvmesa.pod
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/pod/perlvms.pod
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/pod/perlvos.pod
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/pod/perlwin32.pod
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/pod/perlxs.pod
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/pod/perlxstut.pod
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/B.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/ByteLoader.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/Config.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/DynaLoader.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/Errno.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/Fcntl.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/GDBM_File.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/IO.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/NDBM_File.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/ODBM_File.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/Opcode.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/POSIX.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/POSIX.pod
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/SDBM_File.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/Safe.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/Socket.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/XSLoader.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/attrs.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/ops.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/perllocal.pod
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/re.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/B/Asmdata.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/B/Assembler.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/B/Bblock.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/B/Bytecode.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/B/C.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/B/CC.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/B/Concise.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/B/Debug.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/B/Deparse.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/B/Disassembler.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/B/Lint.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/B/Showlex.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/B/Stackobj.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/B/Stash.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/B/Terse.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/B/Xref.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/CORE/EXTERN.h
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/CORE/INTERN.h
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/CORE/XSUB.h
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/CORE/av.h
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/CORE/cc_runtime.h
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/CORE/config.h
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/CORE/cop.h
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/CORE/cv.h
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/CORE/dosish.h
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/CORE/embed.h
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/CORE/embedvar.h
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/CORE/fakethr.h
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/CORE/form.h
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/CORE/gv.h
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/CORE/handy.h
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/CORE/hv.h
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/CORE/intrpvar.h
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/CORE/iperlsys.h
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/CORE/keywords.h
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/CORE/libperl.a
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/CORE/mg.h
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/CORE/nostdio.h
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/CORE/objXSUB.h
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/CORE/op.h
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/CORE/opcode.h
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/CORE/opnames.h
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/CORE/patchlevel.h
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/CORE/perl.h
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/CORE/perlapi.h
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/CORE/perlio.h
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/CORE/perlsdio.h
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/CORE/perlsfio.h
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/CORE/perlvars.h
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/CORE/perly.h
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/CORE/pp.h
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/CORE/pp_proto.h
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/CORE/proto.h
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/CORE/regcomp.h
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/CORE/regexp.h
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/CORE/regnodes.h
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/CORE/scope.h
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/CORE/sv.h
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/CORE/thrdvar.h
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/CORE/thread.h
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/CORE/unixish.h
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/CORE/utf8.h
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/CORE/util.h
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/CORE/warnings.h
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/Data/Dumper.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/Devel/DProf.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/Devel/Peek.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/File/Glob.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/IO/Dir.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/IO/File.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/IO/Handle.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/IO/Pipe.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/IO/Poll.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/IO/Seekable.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/IO/Select.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/IO/Socket.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/IPC/Msg.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/IPC/Semaphore.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/IPC/SysV.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/Sys/Hostname.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/Sys/Syslog.pm
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/B/B.bs
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/ByteLoader/ByteLoader.bs
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/Data/Dumper/Dumper.bs
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/Devel/DProf/DProf.bs
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/Devel/Peek/Peek.bs
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/DynaLoader/DynaLoader.a
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/DynaLoader/autosplit.ix
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/DynaLoader/dl_expandspec.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/DynaLoader/dl_find_symbol_anywhere.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/DynaLoader/dl_findfile.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/DynaLoader/extralibs.ld
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/Fcntl/Fcntl.bs
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/IO/IO.bs
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/IPC/SysV/SysV.bs
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/NDBM_File/NDBM_File.bs
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/ODBM_File/ODBM_File.bs
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/Opcode/Opcode.bs
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/POSIX.bs
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/POSIX.so
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/abs.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/alarm.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/assert.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/atan2.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/atexit.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/atof.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/atoi.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/atol.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/autosplit.ix
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/bsearch.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/calloc.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/chdir.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/chmod.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/chown.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/clearerr.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/closedir.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/cos.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/creat.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/div.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/errno.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/execl.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/execle.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/execlp.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/execv.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/execve.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/execvp.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/exit.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/exp.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/fabs.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/fclose.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/fcntl.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/fdopen.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/feof.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/ferror.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/fflush.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/fgetc.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/fgetpos.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/fgets.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/fileno.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/fopen.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/fork.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/fprintf.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/fputc.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/fputs.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/fread.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/free.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/freopen.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/fscanf.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/fseek.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/fsetpos.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/fstat.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/ftell.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/fwrite.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/getc.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/getchar.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/getcwd.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/getegid.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/getenv.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/geteuid.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/getgid.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/getgrgid.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/getgrnam.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/getgroups.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/getlogin.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/getpgrp.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/getpid.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/getppid.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/getpwnam.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/getpwuid.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/gets.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/getuid.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/gmtime.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/isatty.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/kill.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/labs.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/ldiv.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/link.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/load_imports.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/localtime.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/log.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/longjmp.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/malloc.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/memchr.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/memcmp.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/memcpy.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/memmove.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/memset.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/mkdir.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/offsetof.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/opendir.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/perror.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/pow.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/printf.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/putc.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/putchar.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/puts.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/qsort.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/raise.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/rand.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/readdir.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/realloc.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/remove.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/rename.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/rewind.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/rewinddir.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/rmdir.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/scanf.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/setbuf.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/setgid.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/setjmp.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/setuid.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/setvbuf.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/siglongjmp.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/sigsetjmp.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/sin.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/sleep.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/sprintf.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/sqrt.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/srand.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/sscanf.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/stat.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/strcat.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/strchr.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/strcmp.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/strcpy.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/strcspn.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/strerror.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/strlen.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/strncat.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/strncmp.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/strncpy.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/strpbrk.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/strrchr.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/strspn.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/strstr.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/strtok.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/system.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/time.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/tmpfile.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/tolower.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/toupper.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/umask.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/ungetc.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/unlink.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/utime.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/vfprintf.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/vprintf.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/vsprintf.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/wait.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/POSIX/waitpid.al
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/SDBM_File/SDBM_File.bs
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/Socket/Socket.bs
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/Sys/Hostname/Hostname.bs
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/Sys/Hostname/autosplit.ix
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/Sys/Syslog/Syslog.bs
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/attrs/attrs.bs
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/re/re.bs
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/sun4-solaris/auto/sdbm/extralibs.ld
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/ArabLink.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/ArabLnkGrp.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/ArabShap.txt
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/BidiMirr.txt
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Bidirectional.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Block.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Blocks.txt
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/CaseFold.txt
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Category.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/CombiningClass.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/CompExcl.txt
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Decomposition.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/EAWidth.txt
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Index.txt
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Jamo.txt
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/JamoShort.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/LineBrk.txt
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Makefile
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Name.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Names.txt
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/NamesList.html
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Number.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/PropList.txt
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/README.Ethiopic
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/README.perl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/ReadMe.txt
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/SpecCase.txt
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/UCD301.html
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/UCDFF301.html
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Unicode.301
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/mktables.PL
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/syllables.txt
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/In/AlphabeticPresentationForms.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/In/Arabic.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/In/ArabicPresentationForms-A.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/In/ArabicPresentationForms-B.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/In/Armenian.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/In/Arrows.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/In/BasicLatin.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/In/Bengali.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/In/BlockElements.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/In/Bopomofo.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/In/BopomofoExtended.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/In/BoxDrawing.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/In/BraillePatterns.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/In/CJKCompatibility.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/In/CJKCompatibilityForms.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/In/CJKCompatibilityIdeographs.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/In/CJKRadicalsSupplement.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/In/CJKSymbolsandPunctuation.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/In/CJKUnifiedIdeographs.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/In/CJKUnifiedIdeographsExtensionA.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/In/Cherokee.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/In/CombiningDiacriticalMarks.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/In/CombiningHalfMarks.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/In/CombiningMarksforSymbols.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/In/ControlPictures.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/In/CurrencySymbols.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/In/Cyrillic.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/In/Devanagari.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/In/Dingbats.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/In/EnclosedAlphanumerics.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/In/EnclosedCJKLettersandMonths.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/In/Ethiopic.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/In/GeneralPunctuation.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/In/GeometricShapes.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/In/Georgian.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/In/Greek.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/In/GreekExtended.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/In/Gujarati.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/In/Gurmukhi.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/In/HalfwidthandFullwidthForms.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/In/HangulCompatibilityJamo.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/In/HangulJamo.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/In/HangulSyllables.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/In/Hebrew.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/In/HighPrivateUseSurrogates.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/In/HighSurrogates.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/In/Hiragana.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/In/IPAExtensions.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/In/IdeographicDescriptionCharacters.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/In/Kanbun.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/In/KangxiRadicals.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/In/Kannada.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/In/Katakana.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/In/Khmer.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/In/Lao.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/In/Latin-1Supplement.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/In/LatinExtended-A.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/In/LatinExtended-B.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/In/LatinExtendedAdditional.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/In/LetterlikeSymbols.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/In/LowSurrogates.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/In/Malayalam.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/In/MathematicalOperators.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/In/MiscellaneousSymbols.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/In/MiscellaneousTechnical.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/In/Mongolian.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/In/Myanmar.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/In/NumberForms.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/In/Ogham.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/In/OpticalCharacterRecognition.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/In/Oriya.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/In/PrivateUse.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/In/Runic.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/In/Sinhala.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/In/SmallFormVariants.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/In/SpacingModifierLetters.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/In/Specials.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/In/SuperscriptsandSubscripts.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/In/Syriac.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/In/Tamil.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/In/Telugu.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/In/Thaana.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/In/Thai.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/In/Tibetan.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/In/UnifiedCanadianAboriginalSyllabics.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/In/YiRadicals.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/In/YiSyllables.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/ASCII.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/Alnum.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/Alpha.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/BidiAL.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/BidiAN.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/BidiB.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/BidiBN.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/BidiCS.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/BidiEN.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/BidiES.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/BidiET.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/BidiL.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/BidiLRE.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/BidiLRO.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/BidiNSM.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/BidiON.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/BidiPDF.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/BidiR.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/BidiRLE.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/BidiRLO.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/BidiS.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/BidiWS.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/Blank.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/C.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/Cc.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/Cf.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/Cn.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/Cntrl.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/Co.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/Cs.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/DCcircle.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/DCcompat.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/DCfinal.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/DCfont.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/DCfraction.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/DCinitial.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/DCisolated.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/DCmedial.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/DCnarrow.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/DCnoBreak.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/DCsmall.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/DCsquare.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/DCsub.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/DCsuper.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/DCvertical.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/DCwide.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/DecoCanon.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/DecoCompat.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/Digit.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/Graph.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/L.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/LbrkAI.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/LbrkAL.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/LbrkB2.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/LbrkBA.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/LbrkBB.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/LbrkBK.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/LbrkCB.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/LbrkCL.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/LbrkCM.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/LbrkCR.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/LbrkEX.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/LbrkGL.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/LbrkHY.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/LbrkID.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/LbrkIN.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/LbrkIS.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/LbrkLF.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/LbrkNS.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/LbrkNU.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/LbrkOP.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/LbrkPO.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/LbrkPR.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/LbrkQU.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/LbrkSA.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/LbrkSG.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/LbrkSP.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/LbrkSY.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/LbrkXX.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/LbrkZW.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/Ll.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/Lm.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/Lo.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/Lower.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/Lt.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/Lu.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/M.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/Mc.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/Me.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/Mirrored.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/Mn.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/N.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/Nd.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/Nl.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/No.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/P.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/Pc.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/Pd.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/Pe.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/Pf.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/Pi.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/Po.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/Print.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/Ps.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/Punct.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/S.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/Sc.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/Sk.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/Sm.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/So.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/Space.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/SpacePerl.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/SylA.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/SylAA.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/SylAAI.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/SylAI.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/SylC.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/SylE.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/SylEE.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/SylI.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/SylII.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/SylN.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/SylO.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/SylOO.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/SylU.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/SylV.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/SylWA.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/SylWAA.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/SylWC.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/SylWE.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/SylWEE.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/SylWI.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/SylWII.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/SylWO.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/SylWOO.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/SylWU.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/SylWV.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/Syllable.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/Upper.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/Word.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/XDigit.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/Z.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/Zl.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/Zp.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/Is/Zs.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/To/Digit.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/To/Lower.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/To/Title.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/unicode/To/Upper.pl
 * - 	gnirs/DC/ppc/solaris/lib/perl5/5.6.1/warnings/register.pm
 * - 	gnirs/DC/pv/Makefile gnirs/DC/pv/Makefile.Unix
 * - 	gnirs/DC/pv/Makefile.Vx gnirs/DC/pv/cadInitVals.pv
 * - 	gnirs/DC/pv/carInitVals.pv gnirs/DC/pv/chip
 * - 	gnirs/DC/pv/clearCads.pv gnirs/DC/pv/clearCars.pv
 * - 	gnirs/DC/pv/doInit.pv gnirs/DC/pv/hkChanInfo.pv
 * - 	gnirs/DC/pv/initVals.pv gnirs/DC/pv/supportInitVals.pv
 * - 	gnirs/DC/pv/testVals.pv gnirs/DC/pv/tmp.lut
 * - 	gnirs/DC/src/Makefile gnirs/DC/src/Makefile.Unix
 * - 	gnirs/DC/src/Makefile.Vx gnirs/DC/startup/Makefile
 * - 	gnirs/DC/startup/Makefile.Unix gnirs/DC/startup/Makefile.Vx
 * - 	gnirs/DC/startup/UAE.dist gnirs/DC/startup/local.flex.vws
 * - 	gnirs/DC/startup/local.vws gnirs/DC/startup/resource.def
 * - 	gnirs/DC/startup/resource.flex.def
 * - 	gnirs/DC/startup/startup.EPICS.vws
 * - 	gnirs/DC/startup/startup.EPICS.vws.pr
 * - 	gnirs/DC/startup/startup.arthur.coadd.vws
 * - 	gnirs/DC/startup/startup.arthur.test.vws
 * - 	gnirs/DC/startup/startup.arthur.vws
 * - 	gnirs/DC/startup/startup.epics.template
 * - 	gnirs/DC/startup/startup.gerbil.vws
 * - 	gnirs/DC/startup/startup.merlin.vws
 * - 	gnirs/DC/startup/startup.pr.vws
 * - 	gnirs/DC/startup/startup.python
 * - 	gnirs/DC/startup/startup.python.vws
 * - 	gnirs/DC/startup/startup.romeo.coadd.vws
 * - 	gnirs/DC/startup/startup.romeo.flex.vws
 * - 	gnirs/DC/startup/startup.romeo.vws
 * - 	gnirs/DC/startup/startup.servoMonitor.vws
 * - 	gnirs/DC/startup/startup.tristan.vws
 * - 	gnirs/DC/startup/startup.vws gnirs/DC/startup/startupTEST
 * - 	gnirs/DC/startup/startupX.vws gnirs/DC/sys/Makefile
 * - 	gnirs/DC/sys/Makefile.Unix gnirs/DC/sys/Makefile.Vx
 * - 	gnirs/DC/tempCard/Makefile gnirs/DC/tempCard/Makefile.Unix
 * - 	gnirs/DC/tempCard/Makefile.Vx gnirs/DC/tempCard/TempLog.py
 * - 	gnirs/DC/tempCard/epicsDev.c gnirs/DC/tempCard/openprt2.c
 * - 	gnirs/DC/tempCard/temp.txt gnirs/DC/tempCard/tempCard.txt
 * - 	gnirs/DC/tempCard/tempCardFiles.txt
 * - 	gnirs/DC/tempCard/tempCardVars
 * - 	gnirs/DC/tempCard/ds5ksource/PROT/avc51.cfg
 * - 	gnirs/DC/tempCard/ds5ksource/PROT/prot.asm
 * - 	gnirs/DC/tempCard/ds5ksource/PROT/prot.c
 * - 	gnirs/DC/tempCard/ds5ksource/PROT/prot.h
 * - 	gnirs/DC/tempCard/ds5ksource/PROT/prot.hex
 * - 	gnirs/DC/tempCard/ds5ksource/PROT/prot.map
 * - 	gnirs/DC/tempCard/ds5ksource/PROT/prot.obj
 * - 	gnirs/DC/tempCard/ds5ksource/PROT/protw.asm
 * - 	gnirs/DC/tempCard/ds5ksource/TEMPCTRL/avc.bat
 * - 	gnirs/DC/tempCard/ds5ksource/TEMPCTRL/avc51.cfg
 * - 	gnirs/DC/tempCard/ds5ksource/TEMPCTRL/avl.bat
 * - 	gnirs/DC/tempCard/ds5ksource/TEMPCTRL/cmd.c
 * - 	gnirs/DC/tempCard/ds5ksource/TEMPCTRL/commands.txt
 * - 	gnirs/DC/tempCard/ds5ksource/TEMPCTRL/hard.c
 * - 	gnirs/DC/tempCard/ds5ksource/TEMPCTRL/init.c
 * - 	gnirs/DC/tpSource/ICON gnirs/DC/tpSource/Makefile
 * - 	gnirs/DC/tpSource/com.make gnirs/DC/tpSource/readme
 * - 	gnirs/DC/tpSource/util.make
 * - 	gnirs/DC/tpSource/common/COMMON.dvi
 * - 	gnirs/DC/tpSource/common/COMMON.log
 * - 	gnirs/DC/tpSource/common/COMMON.tex
 * - 	gnirs/DC/tpSource/common/Makefile
 * - 	gnirs/DC/tpSource/common/Makefile.bak
 * - 	gnirs/DC/tpSource/common/README
 * - 	gnirs/DC/tpSource/common/README.nodes
 * - 	gnirs/DC/tpSource/common/channels.draw
 * - 	gnirs/DC/tpSource/common/common.dvi
 * - 	gnirs/DC/tpSource/common/common.h
 * - 	gnirs/DC/tpSource/common/common.log
 * - 	gnirs/DC/tpSource/common/common.tex
 * - 	gnirs/DC/tpSource/common/control.c
 * - 	gnirs/DC/tpSource/common/control.trl
 * - 	gnirs/DC/tpSource/common/debug.c
 * - 	gnirs/DC/tpSource/common/debug.trl
 * - 	gnirs/DC/tpSource/common/mem.c
 * - 	gnirs/DC/tpSource/common/mem.trl
 * - 	gnirs/DC/tpSource/common/message.c
 * - 	gnirs/DC/tpSource/common/message.trl
 * - 	gnirs/DC/tpSource/common/monitor.c
 * - 	gnirs/DC/tpSource/common/proc.c
 * - 	gnirs/DC/tpSource/common/proc.trl
 * - 	gnirs/DC/tpSource/common/queue.c
 * - 	gnirs/DC/tpSource/common/queue.trl
 * - 	gnirs/DC/tpSource/common/send_debug
 * - 	gnirs/DC/tpSource/common/talk.c
 * - 	gnirs/DC/tpSource/common/talk.trl
 * - 	gnirs/DC/tpSource/common/var.c
 * - 	gnirs/DC/tpSource/common/var.trl
 * - 	gnirs/DC/tpSource/common/var_sr.c
 * - 	gnirs/DC/tpSource/common/var_sr.trl
 * - 	gnirs/DC/tpSource/iconNew/DACmethods.h
 * - 	gnirs/DC/tpSource/iconNew/Makefile
 * - 	gnirs/DC/tpSource/iconNew/Makefile.bak
 * - 	gnirs/DC/tpSource/iconNew/arr_ctrl.c
 * - 	gnirs/DC/tpSource/iconNew/arr_ctrl.trl
 * - 	gnirs/DC/tpSource/iconNew/cmnd_funcs.c
 * - 	gnirs/DC/tpSource/iconNew/cmnd_funcs.trl
 * - 	gnirs/DC/tpSource/iconNew/cnfg_inst.c
 * - 	gnirs/DC/tpSource/iconNew/config.h
 * - 	gnirs/DC/tpSource/iconNew/config_st.h
 * - 	gnirs/DC/tpSource/iconNew/ctio_devs.c
 * - 	gnirs/DC/tpSource/iconNew/dev_funcs.c
 * - 	gnirs/DC/tpSource/iconNew/dev_funcs.trl
 * - 	gnirs/DC/tpSource/iconNew/devdefs.h
 * - 	gnirs/DC/tpSource/iconNew/file
 * - 	gnirs/DC/tpSource/iconNew/inst_cmnds.c
 * - 	gnirs/DC/tpSource/iconNew/inst_cmnds.trl
 * - 	gnirs/DC/tpSource/iconNew/inst_ctrl.c
 * - 	gnirs/DC/tpSource/iconNew/inst_ctrl.trl
 * - 	gnirs/DC/tpSource/iconNew/inst_hdw.c
 * - 	gnirs/DC/tpSource/iconNew/inst_hdw.trl
 * - 	gnirs/DC/tpSource/iconNew/inst_hka2d.c
 * - 	gnirs/DC/tpSource/iconNew/inst_hka2d.trl
 * - 	gnirs/DC/tpSource/iconNew/inst_main.c
 * - 	gnirs/DC/tpSource/iconNew/inst_main.tld
 * - 	gnirs/DC/tpSource/iconNew/inst_main.trl
 * - 	gnirs/DC/tpSource/iconNew/inst_rdvars.c
 * - 	gnirs/DC/tpSource/iconNew/inst_rdvars.trl
 * - 	gnirs/DC/tpSource/iconNew/inst_setvars.c
 * - 	gnirs/DC/tpSource/iconNew/inst_setvars.trl
 * - 	gnirs/DC/tpSource/iconNew/instdefs.h
 * - 	gnirs/DC/tpSource/iconNew/instprocs.h
 * - 	gnirs/DC/tpSource/iconNew/instvars.h
 * - 	gnirs/DC/tpSource/iconNew/map.map
 * - 	gnirs/DC/tpSource/iconNew/NAAC/GCBits.ALL
 * - 	gnirs/DC/tpSource/iconNew/NAAC/arr_ctrl_NAAC.c
 * - 	gnirs/DC/tpSource/iconNew/NAAC/cnfg_NAAC.c
 * - 	gnirs/DC/tpSource/iconNew/NAAC/config_NAAC.h
 * - 	gnirs/DC/tpSource/iconNew/NAAC/icon.tld
 * - 	gnirs/DC/tpSource/iconNew/NAAC/inst.tld
 * - 	gnirs/DC/tpSource/include/protocol.h
 * - 	gnirs/DC/tpSource/include/prototypes.h
 * - 	gnirs/DC/tpSource/seq/Makefile
 * - 	gnirs/DC/tpSource/seq/Makefile.bak
 * - 	gnirs/DC/tpSource/seq/commands.c
 * - 	gnirs/DC/tpSource/seq/commands.trl
 * - 	gnirs/DC/tpSource/seq/main.c gnirs/DC/tpSource/seq/main.trl
 * - 	gnirs/DC/tpSource/seq/map.map gnirs/DC/tpSource/seq/seq.h
 * - 	gnirs/DC/tpSource/seq/seq.tld
 * - 	gnirs/DC/tpSource/seq/seq.tld.old
 * - 	gnirs/DC/tpSource/seq/seq_defs.h
 * - 	gnirs/DC/tpSource/seq/seq_vars.h
 * - 	gnirs/DC/tpSource/seq/seqhdw.h gnirs/DC/tpSource/seq/vars.c
 * - 	gnirs/DC/tpSource/seq/vars.trl
 * - 	gnirs/DC/tpSource/tld/NAAC/icon.tld
 * - 	gnirs/DC/tpSource/tld/NAAC/icon.tld.new
 * - 	gnirs/DC/tpSource/tld/NAAC/icon.tld.old
 * - 	gnirs/DC/tpSource/tld/NAAC/seq.tld
 * - 	gnirs/DC/tpSource/tld/NAAC/seq.tld.new
 * - 	gnirs/DC/tpSource/tld/NAAC/seq.tld.old
 * - 	gnirs/DC/tpSource/tp/nifs/NAACtp.nif
 * - 	gnirs/DC/tpSource/tp/nifs/NAACtp3.nif
 * - 	gnirs/DC/tpSource/tp/nifs/NAACtptest.nif
 * - 	gnirs/DC/tpSource/tp/nifs/NAACtptest.nif.pr
 * - 	gnirs/DC/tpSource/tp/tld/icon.tld
 * - 	gnirs/DC/tpSource/tp/tld/icon.working
 * - 	gnirs/DC/tpSource/tp/tld/seq.tld
 * - 	gnirs/DC/tpSource/tp/tld/seq.tld.working
 * - 	gnirs/DC/tpSource/ucode/NAAC/ALAD2_20_02.base
 * - 	gnirs/DC/tpSource/ucode/NAAC/Makefile
 * - 	gnirs/DC/tpSource/ucode/NAAC/common.h
 * - 	gnirs/DC/tpSource/ucode/NAAC/con
 * - 	gnirs/DC/tpSource/ucode/NAAC/gnAIII_1026xSU01_4u.cmd
 * - 	gnirs/DC/tpSource/ucode/NAAC/gnAII_1024xAA01_4u.c
 * - 	gnirs/DC/tpSource/ucode/NAAC/gnAII_1024xFFT_RR.c
 * - 	gnirs/DC/tpSource/ucode/NAAC/gnAII_1024xFFT_RR.cmd
 * - 	gnirs/DC/tpSource/ucode/NAAC/gnAII_1024xFFT_RR.tld
 * - 	gnirs/DC/tpSource/ucode/NAAC/gnAII_1024xSU01_2u.c
 * - 	gnirs/DC/tpSource/ucode/NAAC/gnAII_1024xSU01_2u.cmd
 * - 	gnirs/DC/tpSource/ucode/NAAC/gnAII_1024xSU01_2u.tld
 * - 	gnirs/DC/tpSource/ucode/NAAC/gnAII_1024xSU01_4u.c
 * - 	gnirs/DC/tpSource/ucode/NAAC/gnAII_1024xSU01_4u.cmd
 * - 	gnirs/DC/tpSource/ucode/NAAC/gnAII_1024xSU01_4u.tld
 * - 	gnirs/DC/tpSource/ucode/NAAC/gnAII_1024xSU01_4u.trl
 * - 	gnirs/DC/tpSource/ucode/NAAC/gnAII_1024xSU01_4uEX.c
 * - 	gnirs/DC/tpSource/ucode/NAAC/gnAII_1024xSU01_6RR.c
 * - 	gnirs/DC/tpSource/ucode/NAAC/gnAII_1024xSU01_6RR.cmd
 * - 	gnirs/DC/tpSource/ucode/NAAC/gnAII_1024xSU01_6RR.tld
 * - 	gnirs/DC/tpSource/ucode/NAAC/gnAII_1024xSU01_6u.c
 * - 	gnirs/DC/tpSource/ucode/NAAC/gnAII_1024xSU01_6u.cmd
 * - 	gnirs/DC/tpSource/ucode/NAAC/gnAII_1024xSU01_6u.tld
 * - 	gnirs/DC/tpSource/ucode/NAAC/gnaacBits.h
 * - 	gnirs/DC/tpSource/ucode/NAAC/gnaacWFire.h
 * - 	gnirs/DC/tpSource/ucode/NAAC/map.map
 * - 	gnirs/DC/tpSource/ucode/NAAC/seqhdw.h
 * - 	gnirs/DC/tpSource/ucode/NAAC/t8lib.tll
 * - 	gnirs/DC/tpSource/ucode/NAAC/tcTest.c
 * - 	gnirs/DC/tpSource/ucode/NAAC/tcTest.cmd
 * - 	gnirs/DC/tpSource/ucode/NAAC/tcTest.tld
 * - 	gnirs/DC/utilSource/Makefile
 * - 	gnirs/DC/utilSource/b014Drv/Makefile
 * - 	gnirs/DC/utilSource/b014Drv/Makefile.Unix
 * - 	gnirs/DC/utilSource/b014Drv/Makefile.Vx
 * - 	gnirs/DC/utilSource/b014Drv/b014.c
 * - 	gnirs/DC/utilSource/b014Drv/b014Drv.c
 * - 	gnirs/DC/utilSource/b014Drv/b014cmds.h
 * - 	gnirs/DC/utilSource/b014Drv/b014includes.h
 * - 	gnirs/DC/utilSource/b014Drv/b014link.c
 * - 	gnirs/DC/utilSource/b014Drv/b014regs.h
 * - 	gnirs/DC/utilSource/b014Drv/call_main.c
 * - 	gnirs/DC/utilSource/b014Drv/call_main.h
 * - 	gnirs/DC/utilSource/b014Drv/regex.c
 * - 	gnirs/DC/utilSource/b014Drv/vwUtil.c
 * - 	gnirs/DC/utilSource/epicstp/Makefile
 * - 	gnirs/DC/utilSource/epicstp/Makefile.Unix
 * - 	gnirs/DC/utilSource/epicstp/Makefile.Vx
 * - 	gnirs/DC/utilSource/epicstp/devAoWFireMessage.c
 * - 	gnirs/DC/utilSource/epicstp/devWFireMessage.C
 * - 	gnirs/DC/utilSource/epicstp/epicstpFiles.txt
 * - 	gnirs/DC/utilSource/healthChk/HKVars.c
 * - 	gnirs/DC/utilSource/healthChk/Makefile
 * - 	gnirs/DC/utilSource/healthChk/Makefile.Unix
 * - 	gnirs/DC/utilSource/healthChk/Makefile.Vx
 * - 	gnirs/DC/utilSource/healthChk/chanInfo
 * - 	gnirs/DC/utilSource/healthChk/epTypedefs.h
 * - 	gnirs/DC/utilSource/healthChk/healthChk.txt
 * - 	gnirs/DC/utilSource/healthChk/healthChkFiles.txt
 * - 	gnirs/DC/utilSource/healthChk/hk.c
 * - 	gnirs/DC/utilSource/healthChk/hkutil.c
 * - 	gnirs/DC/utilSource/socket/Makefile
 * - 	gnirs/DC/utilSource/socket/Makefile.Unix
 * - 	gnirs/DC/utilSource/socket/Makefile.Vx
 * - 	gnirs/DC/utilSource/socket/sockutil.c
 * - 	gnirs/DC/utilSource/time/Makefile
 * - 	gnirs/DC/utilSource/time/Makefile.Unix
 * - 	gnirs/DC/utilSource/time/Makefile.Vx
 * - 	gnirs/DC/utilSource/time/bc350time.c
 * - 	gnirs/DC/utilSource/time/getTime.c
 * - 	gnirs/DC/utilSource/time/time.txt
 * - 	gnirs/DC/utilSource/time/timeFiles.txt
 * - 	gnirs/DC/utilSource/ucdl/Makefile
 * - 	gnirs/DC/utilSource/ucdl/Makefile.Unix
 * - 	gnirs/DC/utilSource/ucdl/Makefile.Vx
 * - 	gnirs/DC/utilSource/ucdl/array.c
 * - 	gnirs/DC/utilSource/ucdl/b016.h gnirs/DC/utilSource/ucdl/hd.c
 * - 	gnirs/DC/utilSource/ucdl/hd.h gnirs/DC/utilSource/ucdl/util.c
 * - 	gnirs/DC/utilSource/ucdl/util.h
 * - 	gnirs/DC/utilSource/vxDrvrApplic/Makefile
 * - 	gnirs/DC/utilSource/vxDrvrApplic/Makefile.Unix
 * - 	gnirs/DC/utilSource/vxDrvrApplic/Makefile.Vx
 * - 	gnirs/DC/utilSource/vxDrvrApplic/bioIsr.c
 * - 	gnirs/DC/utilSource/vxDrvrApplic/bioIsr.h
 * - 	gnirs/DC/utilSource/vxDrvrApplic/config.h
 * - 	gnirs/DC/utilSource/vxDrvrApplic/control.c
 * - 	gnirs/DC/utilSource/vxDrvrApplic/drvr_b014.h
 * - 	gnirs/DC/utilSource/vxDrvrApplic/drvr_defs.h
 * - 	gnirs/DC/utilSource/vxDrvrApplic/drvr_vars.h
 * - 	gnirs/DC/utilSource/vxDrvrApplic/drvrlink.c
 * - 	gnirs/DC/utilSource/vxDrvrApplic/drvrlink.h
 * - 	gnirs/DC/utilSource/vxDrvrApplic/fchanio.h
 * - 	gnirs/DC/utilSource/vxDrvrApplic/irstd.h
 * - 	gnirs/DC/utilSource/vxDrvrApplic/mv162.h
 * - 	gnirs/DC/utilSource/vxDrvrApplic/naacdrvr.c
 * - 	gnirs/DC/utilSource/vxDrvrApplic/naacsrvr.c
 * - 	gnirs/DC/utilSource/vxDrvrApplic/netConsts.h
 * - 	gnirs/DC/utilSource/vxDrvrApplic/reader.c
 * - 	gnirs/DC/utilSource/vxDrvrApplic/writer.c
 * - 	gnirs/DC/utilSource/vxddldnet/Makefile
 * - 	gnirs/DC/utilSource/vxddldnet/Makefile.Unix
 * - 	gnirs/DC/utilSource/vxddldnet/Makefile.Vx
 * - 	gnirs/DC/utilSource/vxddldnet/chanio.c
 * - 	gnirs/DC/utilSource/vxddldnet/chanio.h
 * - 	gnirs/DC/utilSource/vxddldnet/ldnet.txt
 * - 	gnirs/DC/utilSource/vxddldnet/tload.h
 * - 	gnirs/DC/utilSource/vxddldnet/tplink.h
 * - 	gnirs/DC/utilSource/vxddldnet/vxboot.c
 * - 	gnirs/DC/utilSource/vxddldnet/vxldnet.c
 * - 	gnirs/DC/utilSource/wfire/Makefile
 * - 	gnirs/DC/utilSource/wfire/Makefile.Unix
 * - 	gnirs/DC/utilSource/wfire/Makefile.Vx
 * - 	gnirs/DC/utilSource/wfire/WFireMessage.h
 * - 	gnirs/DC/utilSource/wfire/WFireMessageUtil.c
 * - 	gnirs/DC/utilSource/wfire/hkData.txt
 * - 	gnirs/DC/utilSource/wfire/putWFMsg.c
 * - 	gnirs/DC/utilSource/wfire/wfire.txt
 * - 	gnirs/DC/utilSource/wfire/wfireFiles.txt
 * - 	gnirs/DOC/ATP_summary_3.xls gnirs/DOC/CC.txt
 * - 	gnirs/DOC/Fprd97.doc
 * - 	gnirs/DOC/T-ReCS_AcceptanceTestPlan_neat1.doc
 * - 	gnirs/DOC/gnirsSoftwareAT.bak.doc
 * - 	gnirs/DOC/gnirsSoftwareAT.doc gnirs/DOC/icd19b_31.doc
 * - 	gnirs/DOC/icd19b_31jhe.doc gnirs/DOC/~$d19b_31.doc
 * - 	gnirs/DOC/~$d19b_31jhe.doc gnirs/DOC/capfast/cccads
 * - 	gnirs/DOC/capfast/cccars gnirs/DOC/capfast/dccads
 * - 	gnirs/DOC/capfast/dccars gnirs/DOC/capfast/iscads
 * - 	gnirs/DOC/capfast/iscars gnirs/IS/Makefile
 * - 	gnirs/IS/Makefile.subdirs gnirs/IS/config.par
 * - 	gnirs/IS/nirs.env gnirs/IS/nirsSetup gnirs/IS/uaeLogin
 * - 	gnirs/IS/uaeSetup gnirs/IS/uaeTidy gnirs/IS/uaeUnsetup
 * - 	gnirs/IS/alh/Makefile gnirs/IS/alh/Makefile.Unix
 * - 	gnirs/IS/alh/gmosDc.alh gnirs/IS/alh/O.solaris/.DEPENDS
 * - 	gnirs/IS/alh/O.solaris/Target.include gnirs/IS/ascii/Makefile
 * - 	gnirs/IS/ascii/Makefile.Unix gnirs/IS/ascii/O.solaris/Makefile
 * - 	gnirs/IS/ascii/O.solaris/Target.include
 * - 	gnirs/IS/ascii/cat_ascii/devSup.ascii
 * - 	gnirs/IS/ascii/replace_ascii/.cvsignore
 * - 	gnirs/IS/capfast/Makefile gnirs/IS/capfast/Makefile.Unix
 * - 	gnirs/IS/capfast/cad.rc gnirs/IS/capfast/cadFan.sch
 * - 	gnirs/IS/capfast/cadFan.sym gnirs/IS/capfast/dcObsSetup.sch
 * - 	gnirs/IS/capfast/dcObsSetup.sym gnirs/IS/capfast/dcSetup.sch
 * - 	gnirs/IS/capfast/dcSetup.sym gnirs/IS/capfast/eapply.sym
 * - 	gnirs/IS/capfast/edb.def gnirs/IS/capfast/gmSeq.sch
 * - 	gnirs/IS/capfast/gmSeq.sym gnirs/IS/capfast/gmSeqApply.sch
 * - 	gnirs/IS/capfast/gmSeqApply.sym
 * - 	gnirs/IS/capfast/gmSeqApplycCombine.sch
 * - 	gnirs/IS/capfast/gmSeqApplycCombine.sym
 * - 	gnirs/IS/capfast/gmSeqCadCarB.sch
 * - 	gnirs/IS/capfast/gmSeqCadCarB.sym
 * - 	gnirs/IS/capfast/gmSeqCadCarC.sch
 * - 	gnirs/IS/capfast/gmSeqCadCarC.sym
 * - 	gnirs/IS/capfast/gmSeqCadCarD.sch
 * - 	gnirs/IS/capfast/gmSeqCadCarD.sym
 * - 	gnirs/IS/capfast/gmSeqCadCarE.sch
 * - 	gnirs/IS/capfast/gmSeqCadCarE.sym
 * - 	gnirs/IS/capfast/gmSeqCadObserve.sch
 * - 	gnirs/IS/capfast/gmSeqCadObserve.sym
 * - 	gnirs/IS/capfast/gmSeqCadPark.sch
 * - 	gnirs/IS/capfast/gmSeqCadPark.sym
 * - 	gnirs/IS/capfast/gmSeqCarCombine.sym
 * - 	gnirs/IS/capfast/gmSeqCarMonitor.sch
 * - 	gnirs/IS/capfast/gmSeqCarMonitor.sym
 * - 	gnirs/IS/capfast/gmSeqCarSubsys.sch
 * - 	gnirs/IS/capfast/gmSeqCarSubsys.sym
 * - 	gnirs/IS/capfast/gmSeqCommands.sch
 * - 	gnirs/IS/capfast/gmSeqCommands.sym
 * - 	gnirs/IS/capfast/gmSeqDebug.sch
 * - 	gnirs/IS/capfast/gmSeqDebug.sym
 * - 	gnirs/IS/capfast/gmSeqDriveSubApply.sch
 * - 	gnirs/IS/capfast/gmSeqDriveSubApply.sym
 * - 	gnirs/IS/capfast/gmSeqDtaPos.sch
 * - 	gnirs/IS/capfast/gmSeqDtaPos.sym
 * - 	gnirs/IS/capfast/gmSeqEndObserve.sch
 * - 	gnirs/IS/capfast/gmSeqEndObserve.sym
 * - 	gnirs/IS/capfast/gmSeqHealth.sch
 * - 	gnirs/IS/capfast/gmSeqHealth.sym
 * - 	gnirs/IS/capfast/gmSeqHeartBeat.sch
 * - 	gnirs/IS/capfast/gmSeqHeartBeat.sym
 * - 	gnirs/IS/capfast/gmSeqImCommands.sym
 * - 	gnirs/IS/capfast/gmSeqInterlock.sch
 * - 	gnirs/IS/capfast/gmSeqInterlock.sym
 * - 	gnirs/IS/capfast/gmSeqLocalCars.sch
 * - 	gnirs/IS/capfast/gmSeqLocalCars.sym
 * - 	gnirs/IS/capfast/gmSeqLookupTables.sch
 * - 	gnirs/IS/capfast/gmSeqLookupTables.sym
 * - 	gnirs/IS/capfast/gmSeqMotionDisable.sch
 * - 	gnirs/IS/capfast/gmSeqMotionDisable.sym
 * - 	gnirs/IS/capfast/gmSeqObsMean.sch
 * - 	gnirs/IS/capfast/gmSeqObsMean.sym
 * - 	gnirs/IS/capfast/gmSeqObsMeanCalc.sch
 * - 	gnirs/IS/capfast/gmSeqObsMeanCalc.sym
 * - 	gnirs/IS/capfast/gmSeqObserveCar.sch
 * - 	gnirs/IS/capfast/gmSeqObserveCar.sym
 * - 	gnirs/IS/capfast/gmSeqReboot.sch
 * - 	gnirs/IS/capfast/gmSeqReboot.sym gnirs/IS/capfast/gmSeqSad.sch
 * - 	gnirs/IS/capfast/gmSeqSad.sym gnirs/IS/capfast/gmSeqSadTop.sch
 * - 	gnirs/IS/capfast/gmSeqSadTopCP.sch
 * - 	gnirs/IS/capfast/gmSeqSeqCommandCar.sch
 * - 	gnirs/IS/capfast/gmSeqSeqCommandCar.sym
 * - 	gnirs/IS/capfast/gmSeqSeqCommands.sch
 * - 	gnirs/IS/capfast/gmSeqSeqCommands.sym
 * - 	gnirs/IS/capfast/gmSeqSeqCommands1.sch
 * - 	gnirs/IS/capfast/gmSeqSeqCommands1.sym
 * - 	gnirs/IS/capfast/gmSeqSeqCommands2.sch
 * - 	gnirs/IS/capfast/gmSeqSeqCommands2.sym
 * - 	gnirs/IS/capfast/gmSeqSeqCommands3.sch
 * - 	gnirs/IS/capfast/gmSeqSeqCommands3.sym
 * - 	gnirs/IS/capfast/gmSeqState.sch
 * - 	gnirs/IS/capfast/gmSeqState.sym
 * - 	gnirs/IS/capfast/gmSeqStatus.sch
 * - 	gnirs/IS/capfast/gmSeqStatus.sym
 * - 	gnirs/IS/capfast/gmSeqSubApply.sch
 * - 	gnirs/IS/capfast/gmSeqSubApply.sym
 * - 	gnirs/IS/capfast/gmSeqSubsysHealth.sch
 * - 	gnirs/IS/capfast/gmSeqSubsysHealth.sym
 * - 	gnirs/IS/capfast/gmSeqSubsysPresent.sch
 * - 	gnirs/IS/capfast/gmSeqSubsysPresent.sym
 * - 	gnirs/IS/capfast/gmSeqTimeOut.sch
 * - 	gnirs/IS/capfast/gmSeqTimeOut.sym
 * - 	gnirs/IS/capfast/gmSeqToggleApply.sch
 * - 	gnirs/IS/capfast/gmSeqToggleApply.sym
 * - 	gnirs/IS/capfast/gmSeqUpdate.sch
 * - 	gnirs/IS/capfast/gmSeqUpdate.sym
 * - 	gnirs/IS/capfast/gmSeqWaitChange.sch
 * - 	gnirs/IS/capfast/gmSeqWaitChange.sym
 * - 	gnirs/IS/capfast/gmosApply.sch gnirs/IS/capfast/link2Dir.sch
 * - 	gnirs/IS/capfast/link2Dir.sym gnirs/IS/capfast/lookupTable.sch
 * - 	gnirs/IS/capfast/lookupTables.sch
 * - 	gnirs/IS/capfast/makePostscript
 * - 	gnirs/IS/capfast/niriSeqSadTop.sch
 * - 	gnirs/IS/capfast/niriSeqTop.sch gnirs/IS/capfast/nirsMech.sch
 * - 	gnirs/IS/capfast/nirsMech.sym gnirs/IS/capfast/nirsMech2.sch
 * - 	gnirs/IS/capfast/nirsMech2.sym
 * - 	gnirs/IS/capfast/nirsSeqSadTop.sch
 * - 	gnirs/IS/capfast/nirsSeqTop.sch
 * - 	gnirs/IS/capfast/oslBorderC.sym
 * - 	gnirs/IS/capfast/oslBorderD.sym gnirs/IS/capfast/seqAcq.sch
 * - 	gnirs/IS/capfast/seqAcq.sym gnirs/IS/capfast/seqCamera.sch
 * - 	gnirs/IS/capfast/seqCamera.sym gnirs/IS/capfast/seqCover.sch
 * - 	gnirs/IS/capfast/seqCover.sym gnirs/IS/capfast/seqDecker.sch
 * - 	gnirs/IS/capfast/seqDecker.sym gnirs/IS/capfast/seqFocus.sch
 * - 	gnirs/IS/capfast/seqFocus.sym gnirs/IS/capfast/seqFw1.sch
 * - 	gnirs/IS/capfast/seqFw1.sym gnirs/IS/capfast/seqFw2.sch
 * - 	gnirs/IS/capfast/seqFw2.sym gnirs/IS/capfast/seqGrating.sch
 * - 	gnirs/IS/capfast/seqGrating.sym
 * - 	gnirs/IS/capfast/seqMechNames.sch
 * - 	gnirs/IS/capfast/seqMechNames.sym gnirs/IS/capfast/seqSlit.sch
 * - 	gnirs/IS/capfast/seqSlit.sch.bak gnirs/IS/capfast/seqSlit.sym
 * - 	gnirs/IS/capfast/seqXDisp.sch gnirs/IS/capfast/seqXDisp.sym
 * - 	gnirs/IS/capfast/sortedDiff gnirs/IS/capfast/ukatcBorderC.sym
 * - 	gnirs/IS/capfast/ukatcBorderD.sym
 * - 	gnirs/IS/capfast/O.solaris/Makefile
 * - 	gnirs/IS/capfast/O.solaris/Target.include
 * - 	gnirs/IS/capfast/txt/cad.rc gnirs/IS/capfast/txt/iscads
 * - 	gnirs/IS/dl/IScars.adl gnirs/IS/dl/Makefile
 * - 	gnirs/IS/dl/Makefile.Unix gnirs/IS/dl/cadTest.adl
 * - 	gnirs/IS/dl/cadTest.dl gnirs/IS/dl/card.dl gnirs/IS/dl/cars.dl
 * - 	gnirs/IS/dl/ccTop.dl gnirs/IS/dl/gmColors.adl
 * - 	gnirs/IS/dl/gmColors.dl gnirs/IS/dl/gmEditFile
 * - 	gnirs/IS/dl/gmEditFileField gnirs/IS/dl/gmSeq.adl
 * - 	gnirs/IS/dl/gmSeq.dl gnirs/IS/dl/gmSeqADCControl.adl
 * - 	gnirs/IS/dl/gmSeqADCPos.adl gnirs/IS/dl/gmSeqCcCommand
 * - 	gnirs/IS/dl/gmSeqCommands1.adl gnirs/IS/dl/gmSeqCommands2.adl
 * - 	gnirs/IS/dl/gmSeqDebug.adl gnirs/IS/dl/gmSeqDebug.dl
 * - 	gnirs/IS/dl/gmSeqDoCommand gnirs/IS/dl/gmSeqDtaCoeffs.adl
 * - 	gnirs/IS/dl/gmSeqDtaControl.adl gnirs/IS/dl/gmSeqDtaModels.adl
 * - 	gnirs/IS/dl/gmSeqDtaPos.adl gnirs/IS/dl/gmSeqLookupTables.adl
 * - 	gnirs/IS/dl/gmSeqLookupTables.dl
 * - 	gnirs/IS/dl/gmSeqMechCommands.adl
 * - 	gnirs/IS/dl/gmSeqMechCommands.dl gnirs/IS/dl/gmSeqMechHelp.adl
 * - 	gnirs/IS/dl/gmSeqMessages.adl gnirs/IS/dl/gmSeqMessages.dl
 * - 	gnirs/IS/dl/gmSeqObsCommands.adl
 * - 	gnirs/IS/dl/gmSeqObsCommands.dl
 * - 	gnirs/IS/dl/gmSeqObsCommandsOCS.adl
 * - 	gnirs/IS/dl/gmSeqObsCommandsOCS.dl
 * - 	gnirs/IS/dl/gmSeqOtherSeqCommands.adl
 * - 	gnirs/IS/dl/gmSeqOtherSeqCommands.dl
 * - 	gnirs/IS/dl/gmSeqReboot.adl gnirs/IS/dl/gmSeqSad.adl
 * - 	gnirs/IS/dl/gmSeqSeqCommand.adl gnirs/IS/dl/gmSeqStart
 * - 	gnirs/IS/dl/gmSeqStartMKRR gnirs/IS/dl/gmSeqTcsSimulate.adl
 * - 	gnirs/IS/dl/gmSeqTemplate.adl gnirs/IS/dl/gmSeqatmTrack.test
 * - 	gnirs/IS/dl/gmSeqdtaTrack.test
 * - 	gnirs/IS/dl/gmosCommandsHelp.adl
 * - 	gnirs/IS/dl/gmosCommandsHelp.dl gnirs/IS/dl/gmosStartCP
 * - 	gnirs/IS/dl/gmosStartMKRR gnirs/IS/dl/gmosStartMKRS
 * - 	gnirs/IS/dl/gmosStartMKSR gnirs/IS/dl/gmosStartMKSS
 * - 	gnirs/IS/dl/gmosStartMKSS2 gnirs/IS/dl/nirsCommandsHelp.adl
 * - 	gnirs/IS/dl/nirsSad.adl gnirs/IS/dl/nirsSad.dl
 * - 	gnirs/IS/dl/nirsSeqTopHelp.adl gnirs/IS/dl/nirsTop.adl
 * - 	gnirs/IS/dl/nirsTop.dl gnirs/IS/dl/nirsTopHelp.adl
 * - 	gnirs/IS/dl/stopTest.adl gnirs/IS/dl/stopTest.dl
 * - 	gnirs/IS/dl/tempCtrl.adl gnirs/IS/dl/tempCtrl.dl
 * - 	gnirs/IS/dl/template.adl gnirs/IS/dl/IS/dl/tempCtrl.adl
 * - 	gnirs/IS/include/devAssControl.h
 * - 	gnirs/IS/include/devDeviceControl.h
 * - 	gnirs/IS/include/drvOmsVme.h gnirs/IS/include/nirsLutLib.h
 * - 	gnirs/IS/include/nirsSeq.h gnirs/IS/include/recAssControl.h
 * - 	gnirs/IS/include/recDeviceControl.h gnirs/IS/pv/Makefile
 * - 	gnirs/IS/pv/Makefile.Unix gnirs/IS/pv/Makefile.Vx
 * - 	gnirs/IS/pv/acqIS.lut gnirs/IS/pv/cameraIS.lut
 * - 	gnirs/IS/pv/coverIS.lut gnirs/IS/pv/deckerIS.lut
 * - 	gnirs/IS/pv/filters_TEST.lut gnirs/IS/pv/focusIS.lut
 * - 	gnirs/IS/pv/fw1IS.lut gnirs/IS/pv/fw2IS.lut
 * - 	gnirs/IS/pv/fwIS.lut gnirs/IS/pv/gmSeq.pv
 * - 	gnirs/IS/pv/gmSeqSim.pv gnirs/IS/pv/gmSeqTracking.pv
 * - 	gnirs/IS/pv/gratingIS.lut gnirs/IS/pv/gratings_TEST.lut
 * - 	gnirs/IS/pv/lambdaFocus.lut gnirs/IS/pv/slitIS.lut
 * - 	gnirs/IS/pv/startup.pv gnirs/IS/pv/xdispIS.lut
 * - 	gnirs/IS/src/Makefile gnirs/IS/src/Makefile.Unix
 * - 	gnirs/IS/src/Makefile.Vx gnirs/IS/src/README
 * - 	gnirs/IS/src/dbMatchField.c gnirs/IS/src/doublePrint
 * - 	gnirs/IS/src/fw1cad gnirs/IS/src/grating gnirs/IS/src/iscars
 * - 	gnirs/IS/src/n gnirs/IS/src/nirsGenSubLib.c
 * - 	gnirs/IS/src/nirsSeqCadLib.c
 * - 	gnirs/IS/src/nirsSeqDisplayMenus.c gnirs/IS/src/nirsSeqLib.c
 * - 	gnirs/IS/src/removedroutines gnirs/IS/src/widePrint
 * - 	gnirs/IS/src/IS/src/nirsSeqCadLib.c gnirs/IS/startup/Makefile
 * - 	gnirs/IS/startup/Makefile.Unix gnirs/IS/startup/Makefile.Vx
 * - 	gnirs/IS/startup/UAE.dist gnirs/IS/startup/gmStartupCP.vws
 * - 	gnirs/IS/startup/gmStartupMKRR.vws
 * - 	gnirs/IS/startup/gmStartupMKRS.vws
 * - 	gnirs/IS/startup/gmStartupMKSR.vws
 * - 	gnirs/IS/startup/gmStartupMKSS.vws
 * - 	gnirs/IS/startup/gmStartupMKSS2.vws
 * - 	gnirs/IS/startup/isStartup.vws
 * - 	gnirs/IS/startup/isStartupMK.vws
 * - 	gnirs/IS/startup/isStartupUKATC.vws
 * - 	gnirs/IS/startup/isStartupUKATC_NODC.vws
 * - 	gnirs/IS/startup/local.seed.vws gnirs/IS/startup/local.vws
 * - 	gnirs/IS/startup/resource.def
 * - 	gnirs/IS/startup/resource.def.seed
 * - 	gnirs/IS/startup/startCP.vws gnirs/IS/startup/startMK.vws
 * - 	gnirs/IS/startup/startPost.vws gnirs/IS/startup/startPre.vws
 * - 	gnirs/IS/startup/startSim.vws gnirs/IS/startup/startSim2.vws
 * - 	gnirs/IS/startup/startup.vws.gmos
 * - 	gnirs/IS/startup/startupSim.vws
 * - 	gnirs/IS/startup/startupSim2.vws
 * - 	gnirs/IS/startup/startup_MINIMAL.vws gnirs/IS/test/cauSort
 * - 	gnirs/IS/test/checkVal gnirs/IS/test/doCad
 * - 	gnirs/IS/test/doCadWait gnirs/IS/test/gmSeqCadCheck1.test
 * - 	gnirs/IS/test/gmSeqConfInterlock gnirs/IS/test/gmSeqInit.cau
 * - 	gnirs/IS/test/gmSeqObsInterlock
 * - 	gnirs/IS/test/gmSeqRdoutInterlock gnirs/IS/test/gmSeqTestSetup
 * - 	gnirs/IS/test/gmSeqatmPos.test
 * - 	gnirs/IS/test/gmSeqatmTrack.test
 * - 	gnirs/IS/test/gmSeqdtaPos.test
 * - 	gnirs/IS/test/gmSeqdtaTrack.test
 * - 	gnirs/IS/test/gmSeqfltPos.test
 * - 	gnirs/IS/test/gmSeqgrSelect.test
 * - 	gnirs/IS/test/gmSeqmskPos.test gnirs/IS/test/monitor_apply.cau
 * - 	gnirs/IS/test/monitor_datum.cau
 * - 	gnirs/IS/test/monitor_debug.cau
 * - 	gnirs/IS/test/monitor_guide.cau gnirs/IS/test/monitor_init.cau
 * - 	gnirs/IS/test/monitor_observe.cau
 * - 	gnirs/IS/test/monitor_park.cau
 * - 	gnirs/IS/test/monitor_reboot.cau
 * - 	gnirs/IS/test/monitor_simulate.cau
 * - 	gnirs/IS/test/monitor_statehealth.cau
 * - 	gnirs/IS/test/monitor_test.cau
 * - 	gnirs/IS/test/monitor_update.cau
 * - 	gnirs/IS/test/monitor_verify.cau gnirs/IS/test/startReadout
 * - 	gnirs/IS/test/tcsTests.bit gnirs/IS/test/tcsTests.sh
 * - 	gnirs/IS/test/tcsTests.tk gnirs/IS/test/waitApply
 * - 	gnirs/SEQ/V2-5p2/Makefile gnirs/SEQ/V2-5p2/REALEASE.NOTES
 * - 	gnirs/SEQ/V2-5p2/cutpaste gnirs/SEQ/V2-5p2/seqexec
 * - 	gnirs/SEQ/V2-5p2/seqexec.ca gnirs/SEQ/V2-5p2/seqexec.ca.TC1
 * - 	gnirs/SEQ/V2-5p2/seqexec.ca.TCS
 * - 	gnirs/SEQ/V2-5p2/ca_config/acqcam.ca
 * - 	gnirs/SEQ/V2-5p2/ca_config/dhshdr.ca
 * - 	gnirs/SEQ/V2-5p2/ca_config/gcal.ca
 * - 	gnirs/SEQ/V2-5p2/ca_config/gmos.ca
 * - 	gnirs/SEQ/V2-5p2/ca_config/gmoshdr.ca
 * - 	gnirs/SEQ/V2-5p2/ca_config/gpol.ca
 * - 	gnirs/SEQ/V2-5p2/ca_config/michellehdr.ca
 * - 	gnirs/SEQ/V2-5p2/ca_config/niri.ca
 * - 	gnirs/SEQ/V2-5p2/ca_config/nirihdr.ca
 * - 	gnirs/SEQ/V2-5p2/ca_config/trecs.ca
 * - 	gnirs/SEQ/V2-5p2/doc/sequencer.mif
 * - 	gnirs/SEQ/V2-5p2/doc/sequencer.pdf
 * - 	gnirs/SEQ/V2-5p2/lib/AcqCamInst.itcl
 * - 	gnirs/SEQ/V2-5p2/lib/GenericInst.itcl
 * - 	gnirs/SEQ/V2-5p2/lib/GmosInst.itcl
 * - 	gnirs/SEQ/V2-5p2/lib/HeaderInfo.itk
 * - 	gnirs/SEQ/V2-5p2/lib/Makefile
 * - 	gnirs/SEQ/V2-5p2/lib/NiriInst.itcl
 * - 	gnirs/SEQ/V2-5p2/lib/NirsInst.itcl
 * - 	gnirs/SEQ/V2-5p2/lib/ObsDialog.itk
 * - 	gnirs/SEQ/V2-5p2/lib/OcsSession.itcl
 * - 	gnirs/SEQ/V2-5p2/lib/ProgressList.itk
 * - 	gnirs/SEQ/V2-5p2/lib/ProgressPopup.itk
 * - 	gnirs/SEQ/V2-5p2/lib/PubVarClassWidget.itk
 * - 	gnirs/SEQ/V2-5p2/lib/RealGcal.itcl
 * - 	gnirs/SEQ/V2-5p2/lib/RealGpol.itcl
 * - 	gnirs/SEQ/V2-5p2/lib/RealTcs.itcl
 * - 	gnirs/SEQ/V2-5p2/lib/SeqExec.itk
 * - 	gnirs/SEQ/V2-5p2/lib/Sequence.itcl
 * - 	gnirs/SEQ/V2-5p2/lib/SequenceFactory.itcl
 * - 	gnirs/SEQ/V2-5p2/lib/SequenceNS.itcl
 * - 	gnirs/SEQ/V2-5p2/lib/SequenceNSloop.itcl
 * - 	gnirs/SEQ/V2-5p2/lib/SequenceTrecs.itcl
 * - 	gnirs/SEQ/V2-5p2/lib/SequenceWidget.itk
 * - 	gnirs/SEQ/V2-5p2/lib/SimGcal.itcl
 * - 	gnirs/SEQ/V2-5p2/lib/SimGpol.itcl
 * - 	gnirs/SEQ/V2-5p2/lib/SimHdrInfo.itcl
 * - 	gnirs/SEQ/V2-5p2/lib/SimInst.itcl
 * - 	gnirs/SEQ/V2-5p2/lib/SimTcs.itcl
 * - 	gnirs/SEQ/V2-5p2/lib/Step.itcl
 * - 	gnirs/SEQ/V2-5p2/lib/SystemWidget.itk
 * - 	gnirs/SEQ/V2-5p2/lib/TrecsInst.itcl
 * - 	gnirs/SEQ/V2-5p2/lib/UserPrefs.itk
 * - 	gnirs/SEQ/V2-5p2/lib/closedIcon.gif
 * - 	gnirs/SEQ/V2-5p2/lib/nodeIcon.gif
 * - 	gnirs/SEQ/V2-5p2/lib/openedIcon.gif
 * - 	gnirs/SEQ/V2-5p2/lib/seqparse.itcl
 * - 	gnirs/SEQ/V2-5p2/lib/tclIndex
 * - 	gnirs/SEQ/V2-5p2/lib/gmos/Makefile
 * - 	gnirs/SEQ/V2-5p2/lib/gmos/gmos.tcl
 * - 	gnirs/SEQ/V2-5p2/lib/gmos/pkgIndex.tcl
 * - 	gnirs/SEQ/V2-5p2/lib/gmos/scripts/CC.itcl
 * - 	gnirs/SEQ/V2-5p2/lib/gmos/scripts/DC.itcl
 * - 	gnirs/SEQ/V2-5p2/lib/gmos/scripts/Gmos.itcl
 * - 	gnirs/SEQ/V2-5p2/lib/gmos/scripts/Makefile
 * - 	gnirs/SEQ/V2-5p2/lib/gmos/scripts/Observe.itcl
 * - 	gnirs/SEQ/V2-5p2/lib/gmos/scripts/tclIndex
 * - 	gnirs/SEQ/V2-5p2/lib/michelle/Makefile
 * - 	gnirs/SEQ/V2-5p2/lib/michelle/michelle.tcl
 * - 	gnirs/SEQ/V2-5p2/lib/michelle/pkgIndex.tcl
 * - 	gnirs/SEQ/V2-5p2/lib/michelle/scripts/CC.itcl
 * - 	gnirs/SEQ/V2-5p2/lib/michelle/scripts/DC.itcl
 * - 	gnirs/SEQ/V2-5p2/lib/michelle/scripts/Makefile
 * - 	gnirs/SEQ/V2-5p2/lib/michelle/scripts/Michelle.itcl
 * - 	gnirs/SEQ/V2-5p2/lib/michelle/scripts/tclIndex
 * - 	gnirs/SEQ/V2-5p2/lib/niri/Makefile
 * - 	gnirs/SEQ/V2-5p2/lib/niri/niri.tcl
 * - 	gnirs/SEQ/V2-5p2/lib/niri/pkgIndex.tcl
 * - 	gnirs/SEQ/V2-5p2/lib/niri/scripts/CC.itcl
 * - 	gnirs/SEQ/V2-5p2/lib/niri/scripts/DC.itcl
 * - 	gnirs/SEQ/V2-5p2/lib/niri/scripts/Makefile
 * - 	gnirs/SEQ/V2-5p2/lib/niri/scripts/Niri.itcl
 * - 	gnirs/SEQ/V2-5p2/lib/niri/scripts/Observe.itcl
 * - 	gnirs/SEQ/V2-5p2/lib/niri/scripts/tclIndex
 * - 	gnirs/SEQ/V2-5p2/lib/nirs/Makefile
 * - 	gnirs/SEQ/V2-5p2/lib/nirs/nirs.tcl
 * - 	gnirs/SEQ/V2-5p2/lib/nirs/pkgIndex.tcl
 * - 	gnirs/SEQ/V2-5p2/lib/nirs/scripts/CC.itcl
 * - 	gnirs/SEQ/V2-5p2/lib/nirs/scripts/DC.itcl
 * - 	gnirs/SEQ/V2-5p2/lib/nirs/scripts/Makefile
 * - 	gnirs/SEQ/V2-5p2/lib/nirs/scripts/Nirs.tcl
 * - 	gnirs/SEQ/V2-5p2/lib/nirs/scripts/Observe.itcl
 * - 	gnirs/SEQ/V2-5p2/lib/nirs/scripts/tclIndex
 * - 	gnirs/SEQ/V2-5p2/lib/ocsdhshdr/Makefile
 * - 	gnirs/SEQ/V2-5p2/lib/ocsdhshdr/dhshdr.tcl
 * - 	gnirs/SEQ/V2-5p2/lib/ocsdhshdr/gmosdhshdr.tcl
 * - 	gnirs/SEQ/V2-5p2/lib/ocsdhshdr/michelledhshdr.tcl
 * - 	gnirs/SEQ/V2-5p2/lib/ocsdhshdr/niridhshdr.tcl
 * - 	gnirs/SEQ/V2-5p2/lib/ocsdhshdr/pkgIndex.tcl
 * - 	gnirs/SEQ/V2-5p2/lib/ocsdhshdr/scripts/HdrGmos.itcl
 * - 	gnirs/SEQ/V2-5p2/lib/ocsdhshdr/scripts/HdrInfo.itcl
 * - 	gnirs/SEQ/V2-5p2/lib/ocsdhshdr/scripts/HdrMichelle.itcl
 * - 	gnirs/SEQ/V2-5p2/lib/ocsdhshdr/scripts/HdrNiri.itcl
 * - 	gnirs/SEQ/V2-5p2/lib/ocsdhshdr/scripts/Makefile
 * - 	gnirs/SEQ/V2-5p2/lib/ocsdhshdr/scripts/dictload.tcl
 * - 	gnirs/SEQ/V2-5p2/lib/ocsdhshdr/scripts/tclIndex
 * - 	gnirs/SEQ/V2-5p2/lib/trecs/Makefile
 * - 	gnirs/SEQ/V2-5p2/lib/trecs/pkgIndex.tcl
 * - 	gnirs/SEQ/V2-5p2/lib/trecs/trecs.tcl
 * - 	gnirs/SEQ/V2-5p2/lib/trecs/scripts/CC.itcl
 * - 	gnirs/SEQ/V2-5p2/lib/trecs/scripts/DC.itcl
 * - 	gnirs/SEQ/V2-5p2/lib/trecs/scripts/Makefile
 * - 	gnirs/SEQ/V2-5p2/lib/trecs/scripts/Trecs.itcl
 * - 	gnirs/SEQ/V2-5p2/lib/trecs/scripts/tclIndex
 * - 	gnirs/SEQ/V2-5p2/tools/fitsspec.pl
 * - 	gnirs/SEQ/V2-5p2/tools/genericHeaders.txt
 * - 	gnirs/SEQ/V2-5p2/tools/libdd.config.michelle
 * - 	gnirs/SEQ/V2-5p2/tools/michelleAlias.dat
 * - 	gnirs/SEQ/V2-5p2/tools/tempo
 * - 	gnirs/SEQ/V2-5p2/xml-sample/CO_Ori.xml
 * - 	gnirs/SEQ/V2-5p2/xml-sample/NS-TEMPLATE.xml
 * - 	gnirs/SEQ/V2-5p2/xml-sample/OTdemoSequence.xml
 * - 	gnirs/SEQ/V2-5p2/xml-sample/README
 * - 	gnirs/SEQ/V2-5p2/xml-sample/_NodShuffleSequence.xml
 * - 	gnirs/SEQ/V2-5p2/xml-sample/_TrecsNodSequence.xml
 * - 	gnirs/SEQ/V2-5p2/xml-sample/actest1.xml
 * - 	gnirs/SEQ/V2-5p2/xml-sample/actest10.xml
 * - 	gnirs/SEQ/V2-5p2/xml-sample/actest11.xml
 * - 	gnirs/SEQ/V2-5p2/xml-sample/actest12.xml
 * - 	gnirs/SEQ/V2-5p2/xml-sample/actest2.xml
 * - 	gnirs/SEQ/V2-5p2/xml-sample/actest3.xml
 * - 	gnirs/SEQ/V2-5p2/xml-sample/actest4.xml
 * - 	gnirs/SEQ/V2-5p2/xml-sample/actest5.xml
 * - 	gnirs/SEQ/V2-5p2/xml-sample/actest6.xml
 * - 	gnirs/SEQ/V2-5p2/xml-sample/actest7.xml
 * - 	gnirs/SEQ/V2-5p2/xml-sample/actest8.xml
 * - 	gnirs/SEQ/V2-5p2/xml-sample/actest9.xml
 * - 	gnirs/SEQ/V2-5p2/xml-sample/actestCUR.xml
 * - 	gnirs/SEQ/V2-5p2/xml-sample/actestCUR2.xml
 * - 	gnirs/SEQ/V2-5p2/xml-sample/b21108_a.xml
 * - 	gnirs/SEQ/V2-5p2/xml-sample/dor3774-1a.xml
 * - 	gnirs/SEQ/V2-5p2/xml-sample/dor4302-1a.xml
 * - 	gnirs/SEQ/V2-5p2/xml-sample/eso161_a.xml
 * - 	gnirs/SEQ/V2-5p2/xml-sample/gcaltest.xml
 * - 	gnirs/SEQ/V2-5p2/xml-sample/gmosobserve.xml
 * - 	gnirs/SEQ/V2-5p2/xml-sample/gmostest1.xml
 * - 	gnirs/SEQ/V2-5p2/xml-sample/gpol-test.xml
 * - 	gnirs/SEQ/V2-5p2/xml-sample/gpol.xml
 * - 	gnirs/SEQ/V2-5p2/xml-sample/limberlost_10arcsec_edited.xml
 * - 	gnirs/SEQ/V2-5p2/xml-sample/mrc1029_a.xml
 * - 	gnirs/SEQ/V2-5p2/xml-sample/mrc959_a.xml
 * - 	gnirs/SEQ/V2-5p2/xml-sample/mrcb0543_a.xml
 * - 	gnirs/SEQ/V2-5p2/xml-sample/n2865_a.xml
 * - 	gnirs/SEQ/V2-5p2/xml-sample/n3115_a.xml
 * - 	gnirs/SEQ/V2-5p2/xml-sample/n3268_a.xml
 * - 	gnirs/SEQ/V2-5p2/xml-sample/n3311_a.xml
 * - 	gnirs/SEQ/V2-5p2/xml-sample/n3489_a.xml
 * - 	gnirs/SEQ/V2-5p2/xml-sample/n4105_a.xml
 * - 	gnirs/SEQ/V2-5p2/xml-sample/n4472_a.xml
 * - 	gnirs/SEQ/V2-5p2/xml-sample/niritest1.xml
 * - 	gnirs/SEQ/V2-5p2/xml-sample/ns1-exp10.xml
 * - 	gnirs/SEQ/V2-5p2/xml-sample/ns15-exp15.xml
 * - 	gnirs/SEQ/V2-5p2/xml-sample/ns4-exp30.xml
 * - 	gnirs/SEQ/V2-5p2/xml-sample/ns_slitlet_im_seq.xml
 * - 	gnirs/SEQ/V2-5p2/xml-sample/ns_slitlet_seq.xml
 * - 	gnirs/SEQ/V2-5p2/xml-sample/nsp0q3-exp15-iter4.xml
 * - 	gnirs/SEQ/V2-5p2/xml-sample/nsp3q0-exp15-iter1.xml
 * - 	gnirs/SEQ/V2-5p2/xml-sample/sftest.xml
 * - 	gnirs/SEQ/V2-5p2/xml-sample/tcstest1.xml
 * - 	gnirs/SEQ/V2-5p2/xml-sample/tcstest10.xml
 * - 	gnirs/SEQ/V2-5p2/xml-sample/tcstest11.xml
 * - 	gnirs/SEQ/V2-5p2/xml-sample/tcstest12.xml
 * - 	gnirs/SEQ/V2-5p2/xml-sample/tcstest13.xml
 * - 	gnirs/SEQ/V2-5p2/xml-sample/tcstest14.xml
 * - 	gnirs/SEQ/V2-5p2/xml-sample/tcstest15.xml
 * - 	gnirs/SEQ/V2-5p2/xml-sample/tcstest2.xml
 * - 	gnirs/SEQ/V2-5p2/xml-sample/tcstest3.xml
 * - 	gnirs/SEQ/V2-5p2/xml-sample/tcstest4.xml
 * - 	gnirs/SEQ/V2-5p2/xml-sample/tcstest5.xml
 * - 	gnirs/SEQ/V2-5p2/xml-sample/tcstest6.xml
 * - 	gnirs/SEQ/V2-5p2/xml-sample/tcstest7.xml
 * - 	gnirs/SEQ/V2-5p2/xml-sample/tcstest8.xml
 * - 	gnirs/SEQ/V2-5p2/xml-sample/tcstest9.xml
 * - 	gnirs/SEQ/V2-5p2/xml-sample/test1.xml
 * - 	gnirs/SEQ/V2-5p2/xml-sample/testCUR2.xml
 * - 	gnirs/SEQ/dhshdr1.5/Makefile gnirs/SEQ/dhshdr1.5/dhshdr.tcl
 * - 	gnirs/SEQ/dhshdr1.5/gmosdhshdr.tcl
 * - 	gnirs/SEQ/dhshdr1.5/niridhshdr.tcl
 * - 	gnirs/SEQ/dhshdr1.5/pkgIndex.tcl
 * - 	gnirs/SEQ/dhshdr1.5/scripts/HdrGmos.itcl
 * - 	gnirs/SEQ/dhshdr1.5/scripts/HdrInfo.itcl
 * - 	gnirs/SEQ/dhshdr1.5/scripts/HdrInfo.itcl.orig
 * - 	gnirs/SEQ/dhshdr1.5/scripts/HdrNiri.itcl
 * - 	gnirs/SEQ/dhshdr1.5/scripts/Makefile
 * - 	gnirs/SEQ/dhshdr1.5/scripts/dictload.tcl
 * - 	gnirs/SEQ/dhshdr1.5/scripts/tclIndex gnirs/SEQ/seq2.1/Makefile
 * - 	gnirs/SEQ/seq2.1/pkgIndex.tcl gnirs/SEQ/seq2.1/seq.tcl
 * - 	gnirs/SEQ/seq2.1/scripts/Action.itcl
 * - 	gnirs/SEQ/seq2.1/scripts/Makefile
 * - 	gnirs/SEQ/seq2.1/scripts/PrincipalSystem.itcl
 * - 	gnirs/SEQ/seq2.1/scripts/tclIndex gnirs/WFS/0README.txt
 * - 	gnirs/WFS/Makefile gnirs/WFS/Makefile.subdirs
 * - 	gnirs/WFS/RELEASE gnirs/WFS/listCad gnirs/WFS/niriCcStart
 * - 	gnirs/WFS/niriDcStart gnirs/WFS/niriEngStart
 * - 	gnirs/WFS/niriIsStart gnirs/WFS/niriLogin_IFA
 * - 	gnirs/WFS/niriLogin_SAMPLE gnirs/WFS/niriSetup
 * - 	gnirs/WFS/niriStart gnirs/WFS/niriTidy gnirs/WFS/niriUnpack
 * - 	gnirs/WFS/niriWfsStart gnirs/WFS/nirsSetup
 * - 	gnirs/WFS/ENG/.make_subdirs gnirs/WFS/ENG/Makefile
 * - 	gnirs/WFS/ENG/README gnirs/WFS/ENG/Version
 * - 	gnirs/WFS/ENG/niriEngStart gnirs/WFS/ENG/niriLogin_IFA
 * - 	gnirs/WFS/ENG/niriLogin_SAMPLE gnirs/WFS/ENG/niriSetup
 * - 	gnirs/WFS/ENG/nirsSetup gnirs/WFS/ENG/ascii/Makefile
 * - 	gnirs/WFS/ENG/ascii/Makefile.Unix
 * - 	gnirs/WFS/ENG/ascii/cat_ascii/choiceCool.h
 * - 	gnirs/WFS/ENG/ascii/cat_ascii/choiceHallStep.h
 * - 	gnirs/WFS/ENG/ascii/cat_ascii/choiceHmotor.h
 * - 	gnirs/WFS/ENG/ascii/cat_ascii/choiceRec.ascii
 * - 	gnirs/WFS/ENG/ascii/cat_ascii/choiceTcon.h
 * - 	gnirs/WFS/ENG/ascii/cat_ascii/choiceTsen.h
 * - 	gnirs/WFS/ENG/ascii/cat_ascii/coolRecord.ascii
 * - 	gnirs/WFS/ENG/ascii/cat_ascii/dbRecType.ascii
 * - 	gnirs/WFS/ENG/ascii/cat_ascii/devSup.ascii
 * - 	gnirs/WFS/ENG/ascii/cat_ascii/drvSup.ascii
 * - 	gnirs/WFS/ENG/ascii/cat_ascii/hallStepRecord.ascii
 * - 	gnirs/WFS/ENG/ascii/cat_ascii/hmotorRecord.ascii
 * - 	gnirs/WFS/ENG/ascii/cat_ascii/tconRecord.ascii
 * - 	gnirs/WFS/ENG/ascii/cat_ascii/tsenRecord.ascii
 * - 	gnirs/WFS/ENG/capfast/Makefile
 * - 	gnirs/WFS/ENG/capfast/Makefile.Unix
 * - 	gnirs/WFS/ENG/capfast/ain.sch gnirs/WFS/ENG/capfast/ain.sym
 * - 	gnirs/WFS/ENG/capfast/altwfs.sch
 * - 	gnirs/WFS/ENG/capfast/altwfs.sym
 * - 	gnirs/WFS/ENG/capfast/altwfsSet.sch
 * - 	gnirs/WFS/ENG/capfast/altwfsSet.sym
 * - 	gnirs/WFS/ENG/capfast/altwfsSim.sch
 * - 	gnirs/WFS/ENG/capfast/altwfsSimSet.sch
 * - 	gnirs/WFS/ENG/capfast/altwfsSimSet.sym
 * - 	gnirs/WFS/ENG/capfast/altwfsSimTop.sch
 * - 	gnirs/WFS/ENG/capfast/altwfsTop.sch
 * - 	gnirs/WFS/ENG/capfast/atod.sch gnirs/WFS/ENG/capfast/atod.sym
 * - 	gnirs/WFS/ENG/capfast/atodSet.sch
 * - 	gnirs/WFS/ENG/capfast/atodSet.sym
 * - 	gnirs/WFS/ENG/capfast/atodTop.sch
 * - 	gnirs/WFS/ENG/capfast/binio.sch
 * - 	gnirs/WFS/ENG/capfast/binio.sym gnirs/WFS/ENG/capfast/cad.rc
 * - 	gnirs/WFS/ENG/capfast/cc.sch gnirs/WFS/ENG/capfast/cc.sym
 * - 	gnirs/WFS/ENG/capfast/ccSet.sch
 * - 	gnirs/WFS/ENG/capfast/ccSet.sym
 * - 	gnirs/WFS/ENG/capfast/ccSim.sch
 * - 	gnirs/WFS/ENG/capfast/ccSim.sym
 * - 	gnirs/WFS/ENG/capfast/ccSimSet.sch
 * - 	gnirs/WFS/ENG/capfast/ccSimSet.sym
 * - 	gnirs/WFS/ENG/capfast/ccSimTop.sch
 * - 	gnirs/WFS/ENG/capfast/ccTop.sch
 * - 	gnirs/WFS/ENG/capfast/common.sch
 * - 	gnirs/WFS/ENG/capfast/common.sym gnirs/WFS/ENG/capfast/din.sch
 * - 	gnirs/WFS/ENG/capfast/din.sym gnirs/WFS/ENG/capfast/dio.sch
 * - 	gnirs/WFS/ENG/capfast/dio.sym gnirs/WFS/ENG/capfast/dioSet.sch
 * - 	gnirs/WFS/ENG/capfast/dioSet.sym
 * - 	gnirs/WFS/ENG/capfast/dioTop.sch
 * - 	gnirs/WFS/ENG/capfast/dout.sch gnirs/WFS/ENG/capfast/dout.sym
 * - 	gnirs/WFS/ENG/capfast/dummy.sym
 * - 	gnirs/WFS/ENG/capfast/eHallStep.sym
 * - 	gnirs/WFS/ENG/capfast/eborderC.sym
 * - 	gnirs/WFS/ENG/capfast/ecools.sym gnirs/WFS/ENG/capfast/edb.def
 * - 	gnirs/WFS/ENG/capfast/ehmotor.sym
 * - 	gnirs/WFS/ENG/capfast/ehmotors.sym
 * - 	gnirs/WFS/ENG/capfast/ehmotorx.sym
 * - 	gnirs/WFS/ENG/capfast/elutoutx.sym
 * - 	gnirs/WFS/ENG/capfast/eng.sch gnirs/WFS/ENG/capfast/eng.sym
 * - 	gnirs/WFS/ENG/capfast/engSet.sch
 * - 	gnirs/WFS/ENG/capfast/engSet.sym
 * - 	gnirs/WFS/ENG/capfast/engTop.sch
 * - 	gnirs/WFS/ENG/capfast/estepx.sym
 * - 	gnirs/WFS/ENG/capfast/etcons.sym
 * - 	gnirs/WFS/ENG/capfast/etsens.sym gnirs/WFS/ENG/capfast/genTop
 * - 	gnirs/WFS/ENG/capfast/global.sch
 * - 	gnirs/WFS/ENG/capfast/global.sym
 * - 	gnirs/WFS/ENG/capfast/lock.sch gnirs/WFS/ENG/capfast/lock.sym
 * - 	gnirs/WFS/ENG/capfast/makePostscript
 * - 	gnirs/WFS/ENG/capfast/mech.sch gnirs/WFS/ENG/capfast/mech.sym
 * - 	gnirs/WFS/ENG/capfast/rmTop gnirs/WFS/ENG/capfast/schPrintFlex
 * - 	gnirs/WFS/ENG/capfast/sim.sch gnirs/WFS/ENG/capfast/sim.sym
 * - 	gnirs/WFS/ENG/capfast/templateTop.sch
 * - 	gnirs/WFS/ENG/capfast/tmp.sch gnirs/WFS/ENG/capfast/tmp.sym
 * - 	gnirs/WFS/ENG/capfast/tmpMech.sch
 * - 	gnirs/WFS/ENG/capfast/tmpMech.sym
 * - 	gnirs/WFS/ENG/capfast/tmpSet.sch
 * - 	gnirs/WFS/ENG/capfast/tmpSet.sym
 * - 	gnirs/WFS/ENG/capfast/tmpSim.sch
 * - 	gnirs/WFS/ENG/capfast/tmpSim.sym
 * - 	gnirs/WFS/ENG/capfast/tmpSimSet.sch
 * - 	gnirs/WFS/ENG/capfast/tmpSimSet.sym
 * - 	gnirs/WFS/ENG/capfast/tmpSimTop.sch
 * - 	gnirs/WFS/ENG/capfast/tmpTop.sch
 * - 	gnirs/WFS/ENG/capfast/tmpin.sch
 * - 	gnirs/WFS/ENG/capfast/tmpin.sym gnirs/WFS/ENG/capfast/wfs.sch
 * - 	gnirs/WFS/ENG/capfast/wfs.sym gnirs/WFS/ENG/capfast/wfsSet.sch
 * - 	gnirs/WFS/ENG/capfast/wfsSet.sym
 * - 	gnirs/WFS/ENG/capfast/wfsSim.sch
 * - 	gnirs/WFS/ENG/capfast/wfsSim.sym
 * - 	gnirs/WFS/ENG/capfast/wfsSimSet.sch
 * - 	gnirs/WFS/ENG/capfast/wfsSimSet.sym
 * - 	gnirs/WFS/ENG/capfast/wfsSimTop.sch
 * - 	gnirs/WFS/ENG/capfast/wfsTop.sch
 * - 	gnirs/WFS/ENG/capfast/WFS/cad.rc
 * - 	gnirs/WFS/ENG/capfast/WFS/ENG/cad.rc
 * - 	gnirs/WFS/ENG/capfast/WFS/ENG/capfast/cad.rc
 * - 	gnirs/WFS/ENG/capfast/WFS/ENG/capfast/tmpSet.sch
 * - 	gnirs/WFS/ENG/capfast/top/altwfsSimTop.sch
 * - 	gnirs/WFS/ENG/capfast/top/altwfsTop.sch
 * - 	gnirs/WFS/ENG/capfast/top/atodTop.sch
 * - 	gnirs/WFS/ENG/capfast/top/cad.rc
 * - 	gnirs/WFS/ENG/capfast/top/ccSimTop.sch
 * - 	gnirs/WFS/ENG/capfast/top/ccTop.sch
 * - 	gnirs/WFS/ENG/capfast/top/dioTop.sch
 * - 	gnirs/WFS/ENG/capfast/top/engTop.sch
 * - 	gnirs/WFS/ENG/capfast/top/templateTop.sch
 * - 	gnirs/WFS/ENG/capfast/top/tmpSimTop.sch
 * - 	gnirs/WFS/ENG/capfast/top/tmpTop.sch
 * - 	gnirs/WFS/ENG/capfast/top/wfsSimTop.sch
 * - 	gnirs/WFS/ENG/capfast/top/wfsTop.sch gnirs/WFS/ENG/dl/Makefile
 * - 	gnirs/WFS/ENG/dl/Makefile.Unix gnirs/WFS/ENG/dl/engAtod.adl
 * - 	gnirs/WFS/ENG/dl/engCc.adl gnirs/WFS/ENG/dl/engDio.adl
 * - 	gnirs/WFS/ENG/dl/engGbl.adl gnirs/WFS/ENG/dl/engLock.adl
 * - 	gnirs/WFS/ENG/dl/engMech.adl gnirs/WFS/ENG/dl/engParams.adl
 * - 	gnirs/WFS/ENG/dl/engShs.adl gnirs/WFS/ENG/dl/engStripChart.adl
 * - 	gnirs/WFS/ENG/dl/engTmp.adl gnirs/WFS/ENG/dl/engTmp.dl
 * - 	gnirs/WFS/ENG/dl/engWfs.adl gnirs/WFS/ENG/dl/gmColors.adl
 * - 	gnirs/WFS/ENG/dl/gmColors.dl gnirs/WFS/ENG/dl/niriEng.adl
 * - 	gnirs/WFS/ENG/dl/template.adl
 * - 	gnirs/WFS/ENG/docs/motor1/INSTALL
 * - 	gnirs/WFS/ENG/docs/motor1/Readme gnirs/WFS/ENG/docs/motor1/len
 * - 	gnirs/WFS/ENG/docs/motor1/len2 gnirs/WFS/ENG/docs/motor1/len3
 * - 	gnirs/WFS/ENG/docs/motor1/motorRecord.doc
 * - 	gnirs/WFS/ENG/pv/Makefile gnirs/WFS/ENG/pv/Makefile.Unix
 * - 	gnirs/WFS/ENG/pv/Makefile.Vx gnirs/WFS/ENG/pv/busy.ste
 * - 	gnirs/WFS/ENG/pv/cc.pv gnirs/WFS/ENG/pv/cc.ste
 * - 	gnirs/WFS/ENG/pv/ccSim.pv gnirs/WFS/ENG/pv/cfg.tcl
 * - 	gnirs/WFS/ENG/pv/follow.lut gnirs/WFS/ENG/pv/idle.ste
 * - 	gnirs/WFS/ENG/pv/lock.pv gnirs/WFS/ENG/pv/lockCfg.lut
 * - 	gnirs/WFS/ENG/pv/lockGen.lut gnirs/WFS/ENG/pv/lockInit.lut
 * - 	gnirs/WFS/ENG/pv/lockObs.lut gnirs/WFS/ENG/pv/lockTmp.lut
 * - 	gnirs/WFS/ENG/pv/sim.pv gnirs/WFS/ENG/pv/tmp.lut
 * - 	gnirs/WFS/ENG/pv/tmp.pv gnirs/WFS/ENG/pv/tmpSim.pv
 * - 	gnirs/WFS/ENG/pv/wfs.pv gnirs/WFS/ENG/pv/wfs.ste
 * - 	gnirs/WFS/ENG/pv/wfsDir.lut gnirs/WFS/ENG/pv/wfsSim.pv
 * - 	gnirs/WFS/ENG/pv/cold/busy.pv gnirs/WFS/ENG/pv/cold/cc.pv
 * - 	gnirs/WFS/ENG/pv/cold/ccCov.lut gnirs/WFS/ENG/pv/cold/ccCov.pv
 * - 	gnirs/WFS/ENG/pv/cold/ccFilt1.lut
 * - 	gnirs/WFS/ENG/pv/cold/ccFilt1.pv
 * - 	gnirs/WFS/ENG/pv/cold/ccFilt2.lut
 * - 	gnirs/WFS/ENG/pv/cold/ccFilt2.pv
 * - 	gnirs/WFS/ENG/pv/cold/ccFilt3.lut
 * - 	gnirs/WFS/ENG/pv/cold/ccFilt3.pv
 * - 	gnirs/WFS/ENG/pv/cold/ccFoc.lut gnirs/WFS/ENG/pv/cold/ccFoc.pv
 * - 	gnirs/WFS/ENG/pv/cold/ccFopl.lut
 * - 	gnirs/WFS/ENG/pv/cold/ccFopl.pv
 * - 	gnirs/WFS/ENG/pv/cold/ccPuvw.lut
 * - 	gnirs/WFS/ENG/pv/cold/ccPuvw.pv
 * - 	gnirs/WFS/ENG/pv/cold/ccSplt.lut
 * - 	gnirs/WFS/ENG/pv/cold/ccSplt.pv
 * - 	gnirs/WFS/ENG/pv/cold/ccSter1.lut
 * - 	gnirs/WFS/ENG/pv/cold/ccSter1.pv
 * - 	gnirs/WFS/ENG/pv/cold/ccSter2.lut
 * - 	gnirs/WFS/ENG/pv/cold/ccSter2.pv gnirs/WFS/ENG/pv/cold/idle.pv
 * - 	gnirs/WFS/ENG/pv/cold/wfs.pv gnirs/WFS/ENG/pv/cold/wfsFilt.lut
 * - 	gnirs/WFS/ENG/pv/cold/wfsFilt.pv
 * - 	gnirs/WFS/ENG/pv/cold/wfsFoc.lut
 * - 	gnirs/WFS/ENG/pv/cold/wfsFoc.pv
 * - 	gnirs/WFS/ENG/pv/cold/wfsPrbx.lut
 * - 	gnirs/WFS/ENG/pv/cold/wfsPrbx.pv
 * - 	gnirs/WFS/ENG/pv/cold/wfsPrby.lut
 * - 	gnirs/WFS/ENG/pv/cold/wfsPrby.pv
 * - 	gnirs/WFS/ENG/pv/cold/bak/busy.pv
 * - 	gnirs/WFS/ENG/pv/cold/bak/cc.pv
 * - 	gnirs/WFS/ENG/pv/cold/bak/ccCov.lut
 * - 	gnirs/WFS/ENG/pv/cold/bak/ccCov.pv
 * - 	gnirs/WFS/ENG/pv/cold/bak/ccFilt1.lut
 * - 	gnirs/WFS/ENG/pv/cold/bak/ccFilt1.pv
 * - 	gnirs/WFS/ENG/pv/cold/bak/ccFilt2.lut
 * - 	gnirs/WFS/ENG/pv/cold/bak/ccFilt2.pv
 * - 	gnirs/WFS/ENG/pv/cold/bak/ccFilt3.lut
 * - 	gnirs/WFS/ENG/pv/cold/bak/ccFilt3.pv
 * - 	gnirs/WFS/ENG/pv/cold/bak/ccFoc.lut
 * - 	gnirs/WFS/ENG/pv/cold/bak/ccFoc.pv
 * - 	gnirs/WFS/ENG/pv/cold/bak/ccFopl.lut
 * - 	gnirs/WFS/ENG/pv/cold/bak/ccFopl.pv
 * - 	gnirs/WFS/ENG/pv/cold/bak/ccPuvw.lut
 * - 	gnirs/WFS/ENG/pv/cold/bak/ccPuvw.pv
 * - 	gnirs/WFS/ENG/pv/cold/bak/ccSplt.lut
 * - 	gnirs/WFS/ENG/pv/cold/bak/ccSplt.pv
 * - 	gnirs/WFS/ENG/pv/cold/bak/ccSter1.lut
 * - 	gnirs/WFS/ENG/pv/cold/bak/ccSter1.pv
 * - 	gnirs/WFS/ENG/pv/cold/bak/ccSter2.lut
 * - 	gnirs/WFS/ENG/pv/cold/bak/ccSter2.pv
 * - 	gnirs/WFS/ENG/pv/cold/bak/idle.pv
 * - 	gnirs/WFS/ENG/pv/cold/bak/wfs.pv
 * - 	gnirs/WFS/ENG/pv/cold/bak/wfsFilt.lut
 * - 	gnirs/WFS/ENG/pv/cold/bak/wfsFilt.pv
 * - 	gnirs/WFS/ENG/pv/cold/bak/wfsFoc.lut
 * - 	gnirs/WFS/ENG/pv/cold/bak/wfsFoc.pv
 * - 	gnirs/WFS/ENG/pv/cold/bak/wfsPrbx.lut
 * - 	gnirs/WFS/ENG/pv/cold/bak/wfsPrbx.pv
 * - 	gnirs/WFS/ENG/pv/cold/bak/wfsPrby.lut
 * - 	gnirs/WFS/ENG/pv/cold/bak/wfsPrby.pv
 * - 	gnirs/WFS/ENG/pv/null/busy.pv gnirs/WFS/ENG/pv/null/cc.pv
 * - 	gnirs/WFS/ENG/pv/null/ccCov.lut gnirs/WFS/ENG/pv/null/ccCov.pv
 * - 	gnirs/WFS/ENG/pv/null/ccFilt1.lut
 * - 	gnirs/WFS/ENG/pv/null/ccFilt1.pv
 * - 	gnirs/WFS/ENG/pv/null/ccFilt2.lut
 * - 	gnirs/WFS/ENG/pv/null/ccFilt2.pv
 * - 	gnirs/WFS/ENG/pv/null/ccFilt3.lut
 * - 	gnirs/WFS/ENG/pv/null/ccFilt3.pv
 * - 	gnirs/WFS/ENG/pv/null/ccFoc.lut gnirs/WFS/ENG/pv/null/ccFoc.pv
 * - 	gnirs/WFS/ENG/pv/null/ccFopl.lut
 * - 	gnirs/WFS/ENG/pv/null/ccFopl.pv
 * - 	gnirs/WFS/ENG/pv/null/ccPuvw.lut
 * - 	gnirs/WFS/ENG/pv/null/ccPuvw.pv
 * - 	gnirs/WFS/ENG/pv/null/ccSplt.lut
 * - 	gnirs/WFS/ENG/pv/null/ccSplt.pv
 * - 	gnirs/WFS/ENG/pv/null/ccSter1.lut
 * - 	gnirs/WFS/ENG/pv/null/ccSter1.pv
 * - 	gnirs/WFS/ENG/pv/null/ccSter2.lut
 * - 	gnirs/WFS/ENG/pv/null/ccSter2.pv gnirs/WFS/ENG/pv/null/idle.pv
 * - 	gnirs/WFS/ENG/pv/null/wfs.pv gnirs/WFS/ENG/pv/null/wfsFilt.lut
 * - 	gnirs/WFS/ENG/pv/null/wfsFilt.pv
 * - 	gnirs/WFS/ENG/pv/null/wfsFoc.lut
 * - 	gnirs/WFS/ENG/pv/null/wfsFoc.pv
 * - 	gnirs/WFS/ENG/pv/null/wfsPrbx.lut
 * - 	gnirs/WFS/ENG/pv/null/wfsPrbx.pv
 * - 	gnirs/WFS/ENG/pv/null/wfsPrby.lut
 * - 	gnirs/WFS/ENG/pv/null/wfsPrby.pv gnirs/WFS/ENG/pv/sim/busy.pv
 * - 	gnirs/WFS/ENG/pv/sim/cc.pv gnirs/WFS/ENG/pv/sim/ccCov.lut
 * - 	gnirs/WFS/ENG/pv/sim/ccCov.pv gnirs/WFS/ENG/pv/sim/ccFilt1.lut
 * - 	gnirs/WFS/ENG/pv/sim/ccFilt1.pv
 * - 	gnirs/WFS/ENG/pv/sim/ccFilt2.lut
 * - 	gnirs/WFS/ENG/pv/sim/ccFilt2.pv
 * - 	gnirs/WFS/ENG/pv/sim/ccFilt3.lut
 * - 	gnirs/WFS/ENG/pv/sim/ccFilt3.pv gnirs/WFS/ENG/pv/sim/ccFoc.lut
 * - 	gnirs/WFS/ENG/pv/sim/ccFoc.pv gnirs/WFS/ENG/pv/sim/ccFopl.lut
 * - 	gnirs/WFS/ENG/pv/sim/ccFopl.pv gnirs/WFS/ENG/pv/sim/ccPuvw.lut
 * - 	gnirs/WFS/ENG/pv/sim/ccPuvw.pv gnirs/WFS/ENG/pv/sim/ccSplt.lut
 * - 	gnirs/WFS/ENG/pv/sim/ccSplt.pv
 * - 	gnirs/WFS/ENG/pv/sim/ccSter1.lut
 * - 	gnirs/WFS/ENG/pv/sim/ccSter1.pv
 * - 	gnirs/WFS/ENG/pv/sim/ccSter2.lut
 * - 	gnirs/WFS/ENG/pv/sim/ccSter2.pv gnirs/WFS/ENG/pv/sim/idle.pv
 * - 	gnirs/WFS/ENG/pv/sim/wfs.pv gnirs/WFS/ENG/pv/sim/wfsFilt.lut
 * - 	gnirs/WFS/ENG/pv/sim/wfsFilt.pv
 * - 	gnirs/WFS/ENG/pv/sim/wfsFoc.lut gnirs/WFS/ENG/pv/sim/wfsFoc.pv
 * - 	gnirs/WFS/ENG/pv/sim/wfsPrbx.lut
 * - 	gnirs/WFS/ENG/pv/sim/wfsPrbx.pv
 * - 	gnirs/WFS/ENG/pv/sim/wfsPrby.lut
 * - 	gnirs/WFS/ENG/pv/sim/wfsPrby.pv gnirs/WFS/ENG/pv/warm/busy.pv
 * - 	gnirs/WFS/ENG/pv/warm/cc.pv gnirs/WFS/ENG/pv/warm/ccCov.lut
 * - 	gnirs/WFS/ENG/pv/warm/ccCov.pv
 * - 	gnirs/WFS/ENG/pv/warm/ccFilt1.lut
 * - 	gnirs/WFS/ENG/pv/warm/ccFilt1.pv
 * - 	gnirs/WFS/ENG/pv/warm/ccFilt2.lut
 * - 	gnirs/WFS/ENG/pv/warm/ccFilt2.pv
 * - 	gnirs/WFS/ENG/pv/warm/ccFilt3.lut
 * - 	gnirs/WFS/ENG/pv/warm/ccFilt3.pv
 * - 	gnirs/WFS/ENG/pv/warm/ccFoc.lut gnirs/WFS/ENG/pv/warm/ccFoc.pv
 * - 	gnirs/WFS/ENG/pv/warm/ccFopl.lut
 * - 	gnirs/WFS/ENG/pv/warm/ccFopl.pv
 * - 	gnirs/WFS/ENG/pv/warm/ccPuvw.lut
 * - 	gnirs/WFS/ENG/pv/warm/ccPuvw.pv
 * - 	gnirs/WFS/ENG/pv/warm/ccSplt.lut
 * - 	gnirs/WFS/ENG/pv/warm/ccSplt.pv
 * - 	gnirs/WFS/ENG/pv/warm/ccSter1.lut
 * - 	gnirs/WFS/ENG/pv/warm/ccSter1.pv
 * - 	gnirs/WFS/ENG/pv/warm/ccSter2.lut
 * - 	gnirs/WFS/ENG/pv/warm/ccSter2.pv gnirs/WFS/ENG/pv/warm/idle.pv
 * - 	gnirs/WFS/ENG/pv/warm/wfs.pv gnirs/WFS/ENG/pv/warm/wfsFilt.lut
 * - 	gnirs/WFS/ENG/pv/warm/wfsFilt.pv
 * - 	gnirs/WFS/ENG/pv/warm/wfsFilt.pv.orig
 * - 	gnirs/WFS/ENG/pv/warm/wfsFilttest.pv
 * - 	gnirs/WFS/ENG/pv/warm/wfsFoc.lut
 * - 	gnirs/WFS/ENG/pv/warm/wfsFoc.pv
 * - 	gnirs/WFS/ENG/pv/warm/wfsPrbx.lut
 * - 	gnirs/WFS/ENG/pv/warm/wfsPrbx.lut.pos
 * - 	gnirs/WFS/ENG/pv/warm/wfsPrbx.pv
 * - 	gnirs/WFS/ENG/pv/warm/wfsPrbx.pv.cycle
 * - 	gnirs/WFS/ENG/pv/warm/wfsPrby.lut
 * - 	gnirs/WFS/ENG/pv/warm/wfsPrby.lut.pos
 * - 	gnirs/WFS/ENG/pv/warm/wfsPrby.pv
 * - 	gnirs/WFS/ENG/pv/warm/wfsPrby.pv.cycle
 * - 	gnirs/WFS/ENG/pv/warm/wfsPrby1.pv
 * - 	gnirs/WFS/ENG/pv/warm/original/busy.pv
 * - 	gnirs/WFS/ENG/pv/warm/original/cc.pv
 * - 	gnirs/WFS/ENG/pv/warm/original/ccCov.lut
 * - 	gnirs/WFS/ENG/pv/warm/original/ccCov.pv
 * - 	gnirs/WFS/ENG/pv/warm/original/ccFilt1.lut
 * - 	gnirs/WFS/ENG/pv/warm/original/ccFilt1.pv
 * - 	gnirs/WFS/ENG/pv/warm/original/ccFilt2.lut
 * - 	gnirs/WFS/ENG/pv/warm/original/ccFilt2.pv
 * - 	gnirs/WFS/ENG/pv/warm/original/ccFilt3.lut
 * - 	gnirs/WFS/ENG/pv/warm/original/ccFilt3.pv
 * - 	gnirs/WFS/ENG/pv/warm/original/ccFoc.lut
 * - 	gnirs/WFS/ENG/pv/warm/original/ccFoc.pv
 * - 	gnirs/WFS/ENG/pv/warm/original/ccFopl.lut
 * - 	gnirs/WFS/ENG/pv/warm/original/ccFopl.pv
 * - 	gnirs/WFS/ENG/pv/warm/original/ccPuvw.lut
 * - 	gnirs/WFS/ENG/pv/warm/original/ccPuvw.pv
 * - 	gnirs/WFS/ENG/pv/warm/original/ccSplt.lut
 * - 	gnirs/WFS/ENG/pv/warm/original/ccSplt.pv
 * - 	gnirs/WFS/ENG/pv/warm/original/ccSter1.lut
 * - 	gnirs/WFS/ENG/pv/warm/original/ccSter1.pv
 * - 	gnirs/WFS/ENG/pv/warm/original/ccSter2.lut
 * - 	gnirs/WFS/ENG/pv/warm/original/ccSter2.pv
 * - 	gnirs/WFS/ENG/pv/warm/original/idle.pv
 * - 	gnirs/WFS/ENG/pv/warm/original/wfs.pv
 * - 	gnirs/WFS/ENG/pv/warm/original/wfsFilt.lut
 * - 	gnirs/WFS/ENG/pv/warm/original/wfsFilt.pv
 * - 	gnirs/WFS/ENG/pv/warm/original/wfsFilt.pv.orig
 * - 	gnirs/WFS/ENG/pv/warm/original/wfsFilttest.pv
 * - 	gnirs/WFS/ENG/pv/warm/original/wfsFoc.lut
 * - 	gnirs/WFS/ENG/pv/warm/original/wfsFoc.pv
 * - 	gnirs/WFS/ENG/pv/warm/original/wfsPrbx.lut
 * - 	gnirs/WFS/ENG/pv/warm/original/wfsPrbx.lut.pos
 * - 	gnirs/WFS/ENG/pv/warm/original/wfsPrbx.pv
 * - 	gnirs/WFS/ENG/pv/warm/original/wfsPrbx.pv.cycle
 * - 	gnirs/WFS/ENG/pv/warm/original/wfsPrby.lut
 * - 	gnirs/WFS/ENG/pv/warm/original/wfsPrby.lut.pos
 * - 	gnirs/WFS/ENG/pv/warm/original/wfsPrby.pv
 * - 	gnirs/WFS/ENG/pv/warm/original/wfsPrby.pv.cycle
 * - 	gnirs/WFS/ENG/scripts/Makefile
 * - 	gnirs/WFS/ENG/scripts/Makefile.Unix
 * - 	gnirs/WFS/ENG/scripts/niriEngAtod
 * - 	gnirs/WFS/ENG/scripts/niriEngDio
 * - 	gnirs/WFS/ENG/scripts/niriEngStart
 * - 	gnirs/WFS/ENG/scripts/niriEtWish
 * - 	gnirs/WFS/ENG/scripts/niriScanPlot
 * - 	gnirs/WFS/ENG/scripts/niriScanSplit gnirs/WFS/ENG/src/Makefile
 * - 	gnirs/WFS/ENG/src/Makefile.Unix gnirs/WFS/ENG/src/Makefile.Vx
 * - 	gnirs/WFS/ENG/src/README.motor gnirs/WFS/ENG/src/addLoad.c
 * - 	gnirs/WFS/ENG/src/devAiOcyc.c gnirs/WFS/ENG/src/devAiOcyd.c
 * - 	gnirs/WFS/ENG/src/devAiSoftHS.c gnirs/WFS/ENG/src/devAiTsim.c
 * - 	gnirs/WFS/ENG/src/devCoolSoft.c
 * - 	gnirs/WFS/ENG/src/devHmotorHoms.c
 * - 	gnirs/WFS/ENG/src/devHmotorSoftHS.c
 * - 	gnirs/WFS/ENG/src/devHmotorTsim.c
 * - 	gnirs/WFS/ENG/src/devHsBinary.c
 * - 	gnirs/WFS/ENG/src/devHsGimbal.c gnirs/WFS/ENG/src/devHsSlide.c
 * - 	gnirs/WFS/ENG/src/devHsSoft.c gnirs/WFS/ENG/src/devHsStage.c
 * - 	gnirs/WFS/ENG/src/devHsWheel.c gnirs/WFS/ENG/src/devTconOcyc.c
 * - 	gnirs/WFS/ENG/src/devTconTsim.c
 * - 	gnirs/WFS/ENG/src/devTsenOcyd.c
 * - 	gnirs/WFS/ENG/src/devTsenTsim.c gnirs/WFS/ENG/src/drvHoms.c
 * - 	gnirs/WFS/ENG/src/drvHoms.h gnirs/WFS/ENG/src/drvOcyc.c
 * - 	gnirs/WFS/ENG/src/drvOcyc.h gnirs/WFS/ENG/src/drvOcyd.c
 * - 	gnirs/WFS/ENG/src/drvOcyd.h gnirs/WFS/ENG/src/drvSoftHS.c
 * - 	gnirs/WFS/ENG/src/drvSoftHS.h gnirs/WFS/ENG/src/drvTsim.c
 * - 	gnirs/WFS/ENG/src/drvTsim.h gnirs/WFS/ENG/src/ifaErrors.h
 * - 	gnirs/WFS/ENG/src/ints.c gnirs/WFS/ENG/src/recCool.c
 * - 	gnirs/WFS/ENG/src/recCool.h gnirs/WFS/ENG/src/recHallStep.c
 * - 	gnirs/WFS/ENG/src/recHallStep.h gnirs/WFS/ENG/src/recHmotor.c
 * - 	gnirs/WFS/ENG/src/recHmotor.h gnirs/WFS/ENG/src/recTcon.c
 * - 	gnirs/WFS/ENG/src/recTcon.h gnirs/WFS/ENG/src/recTsen.c
 * - 	gnirs/WFS/ENG/src/state.c gnirs/WFS/ENG/src/state.h
 * - 	gnirs/WFS/ENG/src/stateSt.stem gnirs/WFS/ENG/src/tm.c
 * - 	gnirs/WFS/ENG/src/xy490.c gnirs/WFS/ENG/src/xy490.h
 * - 	gnirs/WFS/ENG/src/serial/Makefile
 * - 	gnirs/WFS/ENG/src/serial/ints.c gnirs/WFS/ENG/src/serial/local
 * - 	gnirs/WFS/ENG/src/serial/omegaTest.c
 * - 	gnirs/WFS/ENG/src/serial/otest.c
 * - 	gnirs/WFS/ENG/src/serial/serialTest.c
 * - 	gnirs/WFS/ENG/src/serial/startup
 * - 	gnirs/WFS/ENG/src/serial/stest.c
 * - 	gnirs/WFS/ENG/src/serial/tests.h
 * - 	gnirs/WFS/ENG/src/serial/xy490.c
 * - 	gnirs/WFS/ENG/src/serial/xy490.h
 * - 	gnirs/WFS/ENG/startup/Makefile
 * - 	gnirs/WFS/ENG/startup/Makefile.Unix
 * - 	gnirs/WFS/ENG/startup/Makefile.Vx gnirs/WFS/ENG/startup/README
 * - 	gnirs/WFS/ENG/startup/UAE.dist
 * - 	gnirs/WFS/ENG/startup/ccParm.VWS
 * - 	gnirs/WFS/ENG/startup/ccSeq.VWS
 * - 	gnirs/WFS/ENG/startup/ccSimParm.VWS
 * - 	gnirs/WFS/ENG/startup/done.VWS
 * - 	gnirs/WFS/ENG/startup/drvAltConf.VWS
 * - 	gnirs/WFS/ENG/startup/drvConf.VWS
 * - 	gnirs/WFS/ENG/startup/drvSimConf.VWS
 * - 	gnirs/WFS/ENG/startup/engParm.VWS
 * - 	gnirs/WFS/ENG/startup/engSeq.VWS
 * - 	gnirs/WFS/ENG/startup/engSimParm.VWS
 * - 	gnirs/WFS/ENG/startup/local.vws
 * - 	gnirs/WFS/ENG/startup/lockBusy.VWS
 * - 	gnirs/WFS/ENG/startup/lockCc.VWS
 * - 	gnirs/WFS/ENG/startup/lockIdle.VWS
 * - 	gnirs/WFS/ENG/startup/lockWfs.VWS
 * - 	gnirs/WFS/ENG/startup/resource.def
 * - 	gnirs/WFS/ENG/startup/startup.VWS
 * - 	gnirs/WFS/ENG/startup/startupAltWfs.VWS
 * - 	gnirs/WFS/ENG/startup/startupSim.VWS
 * - 	gnirs/WFS/ENG/startup/startupWfs.VWS
 * - 	gnirs/WFS/ENG/startup/startupWfsSim.VWS
 * - 	gnirs/WFS/ENG/startup/subst gnirs/WFS/ENG/startup/tmpParm.VWS
 * - 	gnirs/WFS/ENG/startup/tmpSimParm.VWS
 * - 	gnirs/WFS/ENG/startup/wfsParm.VWS
 * - 	gnirs/WFS/ENG/startup/wfsSeq.VWS
 * - 	gnirs/WFS/ENG/startup/wfsSimParm.VWS
 * - 	gnirs/WFS/ENG/tcl/Makefile gnirs/WFS/ENG/tcl/Makefile.Unix
 * - 	gnirs/WFS/ENG/tcl/README gnirs/WFS/ENG/tcl/ca.tcl
 * - 	gnirs/WFS/ENG/tcl/cycle.tcl gnirs/WFS/ENG/tcl/features.tcl
 * - 	gnirs/WFS/ENG/tcl/help.tcl gnirs/WFS/ENG/tcl/index.sh
 * - 	gnirs/WFS/ENG/tcl/init.tcl gnirs/WFS/ENG/tcl/pos.tcl
 * - 	gnirs/WFS/ENG/tcl/scan.tcl gnirs/WFS/ENG/tcl/tclIndex
 * - 	gnirs/WFS/ENG/tcl/tmp.tcl gnirs/WFS/ENG/tcl/util.tcl
 * - 	gnirs/WFS/LIB/0README.txt gnirs/WFS/LIB/Makefile
 * - 	gnirs/WFS/LIB/Makefile.subdirs gnirs/WFS/LIB/niriSetup
 * - 	gnirs/WFS/LIB/nirsSetup gnirs/WFS/LIB/capfast/Makefile
 * - 	gnirs/WFS/LIB/capfast/Makefile.Unix
 * - 	gnirs/WFS/LIB/capfast/cad.rc gnirs/WFS/LIB/capfast/cadCar.sch
 * - 	gnirs/WFS/LIB/capfast/cadCar.sym
 * - 	gnirs/WFS/LIB/capfast/cadFanout.sch
 * - 	gnirs/WFS/LIB/capfast/cadFanout.sym
 * - 	gnirs/WFS/LIB/capfast/cadPlus.sch
 * - 	gnirs/WFS/LIB/capfast/cadPlus.sym
 * - 	gnirs/WFS/LIB/capfast/carPlus.sch
 * - 	gnirs/WFS/LIB/capfast/carPlus.sym
 * - 	gnirs/WFS/LIB/capfast/comb2mSad.sch
 * - 	gnirs/WFS/LIB/capfast/comb2mSad.sym
 * - 	gnirs/WFS/LIB/capfast/combCar.sch
 * - 	gnirs/WFS/LIB/capfast/combCar.sym
 * - 	gnirs/WFS/LIB/capfast/combSadHealth.sch
 * - 	gnirs/WFS/LIB/capfast/combSadHealth.sym
 * - 	gnirs/WFS/LIB/capfast/combVal.sch
 * - 	gnirs/WFS/LIB/capfast/combVal.sym
 * - 	gnirs/WFS/LIB/capfast/comp1CadSel.sch
 * - 	gnirs/WFS/LIB/capfast/comp1CadSel.sym
 * - 	gnirs/WFS/LIB/capfast/comp1Car.sch
 * - 	gnirs/WFS/LIB/capfast/comp1Car.sym
 * - 	gnirs/WFS/LIB/capfast/comp1m.sch
 * - 	gnirs/WFS/LIB/capfast/comp1m.sym
 * - 	gnirs/WFS/LIB/capfast/comp1mCad.sch
 * - 	gnirs/WFS/LIB/capfast/comp1mCad.sym
 * - 	gnirs/WFS/LIB/capfast/comp1mCadCmd.sch
 * - 	gnirs/WFS/LIB/capfast/comp1mCadCmd.sym
 * - 	gnirs/WFS/LIB/capfast/comp1mSad.sch
 * - 	gnirs/WFS/LIB/capfast/comp1mSad.sym
 * - 	gnirs/WFS/LIB/capfast/comp1mSadEng.sch
 * - 	gnirs/WFS/LIB/capfast/comp1mSadEng.sym
 * - 	gnirs/WFS/LIB/capfast/comp1mSadHealth.sch
 * - 	gnirs/WFS/LIB/capfast/comp1mSadHealth.sym
 * - 	gnirs/WFS/LIB/capfast/comp1mSadRec.sch
 * - 	gnirs/WFS/LIB/capfast/comp1mSadRec.sym
 * - 	gnirs/WFS/LIB/capfast/comp1pCadCmd.sch
 * - 	gnirs/WFS/LIB/capfast/comp1pCadCmd.sym
 * - 	gnirs/WFS/LIB/capfast/comp2CadSel.sch
 * - 	gnirs/WFS/LIB/capfast/comp2CadSel.sym
 * - 	gnirs/WFS/LIB/capfast/comp2Car.sch
 * - 	gnirs/WFS/LIB/capfast/comp2Car.sym
 * - 	gnirs/WFS/LIB/capfast/comp2m.sch
 * - 	gnirs/WFS/LIB/capfast/comp2m.sym
 * - 	gnirs/WFS/LIB/capfast/comp2mCad.sch
 * - 	gnirs/WFS/LIB/capfast/comp2mCad.sym
 * - 	gnirs/WFS/LIB/capfast/comp2mCadAlt.sch
 * - 	gnirs/WFS/LIB/capfast/comp2mCadAlt.sym
 * - 	gnirs/WFS/LIB/capfast/comp2mCadCmd.sch
 * - 	gnirs/WFS/LIB/capfast/comp2mCadCmd.sym
 * - 	gnirs/WFS/LIB/capfast/comp2mSad.sch
 * - 	gnirs/WFS/LIB/capfast/comp2mSad.sym
 * - 	gnirs/WFS/LIB/capfast/comp2mSadAlt.sch
 * - 	gnirs/WFS/LIB/capfast/comp2mSadAlt.sym
 * - 	gnirs/WFS/LIB/capfast/comp3mCadCmd.sch
 * - 	gnirs/WFS/LIB/capfast/comp3mCadCmd.sym
 * - 	gnirs/WFS/LIB/capfast/compCad.sch
 * - 	gnirs/WFS/LIB/capfast/compCad.sym
 * - 	gnirs/WFS/LIB/capfast/compSnlArg.sch
 * - 	gnirs/WFS/LIB/capfast/compSnlArg.sym
 * - 	gnirs/WFS/LIB/capfast/compSnlCmd.sch
 * - 	gnirs/WFS/LIB/capfast/compSnlCmd.sym
 * - 	gnirs/WFS/LIB/capfast/dummy.sym
 * - 	gnirs/WFS/LIB/capfast/eapplyx.sym
 * - 	gnirs/WFS/LIB/capfast/ecad2.sym
 * - 	gnirs/WFS/LIB/capfast/ecad4.sym gnirs/WFS/LIB/capfast/edb.def
 * - 	gnirs/WFS/LIB/capfast/egenSub.sym
 * - 	gnirs/WFS/LIB/capfast/elutins.sym
 * - 	gnirs/WFS/LIB/capfast/elutouts.sym
 * - 	gnirs/WFS/LIB/capfast/escan.sym
 * - 	gnirs/WFS/LIB/capfast/escans.sym
 * - 	gnirs/WFS/LIB/capfast/estringin.sym
 * - 	gnirs/WFS/LIB/capfast/estringins.sym
 * - 	gnirs/WFS/LIB/capfast/ewait.sym
 * - 	gnirs/WFS/LIB/capfast/link2Dir.sch
 * - 	gnirs/WFS/LIB/capfast/link2Dir.sym
 * - 	gnirs/WFS/LIB/capfast/lock.sch gnirs/WFS/LIB/capfast/lock.sym
 * - 	gnirs/WFS/LIB/capfast/lockCad.sch
 * - 	gnirs/WFS/LIB/capfast/lockCad.sym
 * - 	gnirs/WFS/LIB/capfast/lockCar.sch
 * - 	gnirs/WFS/LIB/capfast/lockCar.sym
 * - 	gnirs/WFS/LIB/capfast/lockSad.sch
 * - 	gnirs/WFS/LIB/capfast/lockSad.sym
 * - 	gnirs/WFS/LIB/capfast/lockSadHealth.sch
 * - 	gnirs/WFS/LIB/capfast/lockSadHealth.sym
 * - 	gnirs/WFS/LIB/capfast/makeLinks
 * - 	gnirs/WFS/LIB/capfast/makePostscript
 * - 	gnirs/WFS/LIB/capfast/mfanout.sch
 * - 	gnirs/WFS/LIB/capfast/mfanout.sym
 * - 	gnirs/WFS/LIB/capfast/notes.sch
 * - 	gnirs/WFS/LIB/capfast/notes.sym gnirs/WFS/LIB/capfast/snl.sch
 * - 	gnirs/WFS/LIB/capfast/snl.sym
 * - 	gnirs/WFS/LIB/capfast/templateC.sch
 * - 	gnirs/WFS/LIB/capfast/templateD.sch
 * - 	gnirs/WFS/LIB/capfast/templateTop.sch
 * - 	gnirs/WFS/LIB/capfast/tmpSad.sch
 * - 	gnirs/WFS/LIB/capfast/tmpSad.sym
 * - 	gnirs/WFS/LIB/capfast/tmpSadCool.sch
 * - 	gnirs/WFS/LIB/capfast/tmpSadCool.sym
 * - 	gnirs/WFS/LIB/capfast/tmpSadCtrl.sch
 * - 	gnirs/WFS/LIB/capfast/tmpSadCtrl.sym
 * - 	gnirs/WFS/LIB/capfast/tmpSadSens.sch
 * - 	gnirs/WFS/LIB/capfast/tmpSadSens.sym gnirs/WFS/LIB/dl/Makefile
 * - 	gnirs/WFS/LIB/dl/Makefile.Unix gnirs/WFS/LIB/dl/comp1m.adl
 * - 	gnirs/WFS/LIB/dl/comp1m.dl gnirs/WFS/LIB/dl/comp1mCad.adl
 * - 	gnirs/WFS/LIB/dl/comp1mSad.adl gnirs/WFS/LIB/dl/comp2m.adl
 * - 	gnirs/WFS/LIB/dl/comp2mAlt.adl gnirs/WFS/LIB/dl/comp2mCad.adl
 * - 	gnirs/WFS/LIB/dl/comp2mEngAlt.adl
 * - 	gnirs/WFS/LIB/dl/comp2mSad.adl gnirs/WFS/LIB/dl/gmColors.adl
 * - 	gnirs/WFS/LIB/dl/gmColors.dl gnirs/WFS/LIB/dl/lock.adl
 * - 	gnirs/WFS/LIB/dl/lockCad.adl gnirs/WFS/LIB/dl/lockSad.adl
 * - 	gnirs/WFS/LIB/dl/niriMotors.adl gnirs/WFS/LIB/dl/sysDebug.adl
 * - 	gnirs/WFS/LIB/dl/template.adl gnirs/WFS/LIB/dl/tmp.adl
 * - 	gnirs/WFS/LIB/dl/tmpSad.adl gnirs/WFS/LIB/src/Makefile
 * - 	gnirs/WFS/LIB/src/Makefile.Unix gnirs/WFS/LIB/src/Makefile.Vx
 * - 	gnirs/WFS/LIB/src/cicsCarHealth.c
 * - 	gnirs/WFS/LIB/src/cicsConst.h gnirs/WFS/LIB/src/cicsLib.c
 * - 	gnirs/WFS/LIB/src/cicsLib.h gnirs/WFS/LIB/src/cicsLib2.c
 * - 	gnirs/WFS/LIB/src/cicsMiscSub.c
 * - 	gnirs/WFS/LIB/src/compEngMove.c gnirs/WFS/LIB/src/compLib.c
 * - 	gnirs/WFS/LIB/src/compLib.h gnirs/WFS/LIB/src/compMove.c
 * - 	gnirs/WFS/LIB/src/compPseudoSt.stpp
 * - 	gnirs/WFS/LIB/src/compSel.c gnirs/WFS/LIB/src/compType.c
 * - 	gnirs/WFS/LIB/src/engLib.c gnirs/WFS/LIB/src/engModeSt.stpp
 * - 	gnirs/WFS/LIB/src/engSt.stpp gnirs/WFS/LIB/src/sysCad.c
 * - 	gnirs/WFS/LIB/src/sysSt.stpp gnirs/WFS/WFS/0README.txt
 * - 	gnirs/WFS/WFS/Makefile gnirs/WFS/WFS/Makefile.subdirs
 * - 	gnirs/WFS/WFS/niriSetup gnirs/WFS/WFS/niriWfsStart
 * - 	gnirs/WFS/WFS/nirsSetup gnirs/WFS/WFS/capfast/Makefile
 * - 	gnirs/WFS/WFS/capfast/Makefile.Unix
 * - 	gnirs/WFS/WFS/capfast/cad.rc gnirs/WFS/WFS/capfast/cadCar.sch
 * - 	gnirs/WFS/WFS/capfast/cadCar.sym
 * - 	gnirs/WFS/WFS/capfast/cadPlus.sch
 * - 	gnirs/WFS/WFS/capfast/cadPlus.sym
 * - 	gnirs/WFS/WFS/capfast/carPlus.sch
 * - 	gnirs/WFS/WFS/capfast/carPlus.sym
 * - 	gnirs/WFS/WFS/capfast/comb2mSad.sch
 * - 	gnirs/WFS/WFS/capfast/comb2mSad.sym
 * - 	gnirs/WFS/WFS/capfast/combCar.sch
 * - 	gnirs/WFS/WFS/capfast/combCar.sym
 * - 	gnirs/WFS/WFS/capfast/combSadHealth.sch
 * - 	gnirs/WFS/WFS/capfast/combSadHealth.sym
 * - 	gnirs/WFS/WFS/capfast/combVal.sch
 * - 	gnirs/WFS/WFS/capfast/combVal.sym
 * - 	gnirs/WFS/WFS/capfast/comp1CadSel.sch
 * - 	gnirs/WFS/WFS/capfast/comp1CadSel.sym
 * - 	gnirs/WFS/WFS/capfast/comp1Car.sch
 * - 	gnirs/WFS/WFS/capfast/comp1Car.sym
 * - 	gnirs/WFS/WFS/capfast/comp1m.sch
 * - 	gnirs/WFS/WFS/capfast/comp1m.sym
 * - 	gnirs/WFS/WFS/capfast/comp1mCad.sch
 * - 	gnirs/WFS/WFS/capfast/comp1mCad.sym
 * - 	gnirs/WFS/WFS/capfast/comp1mCadCmd.sch
 * - 	gnirs/WFS/WFS/capfast/comp1mCadCmd.sym
 * - 	gnirs/WFS/WFS/capfast/comp1mSad.sch
 * - 	gnirs/WFS/WFS/capfast/comp1mSad.sym
 * - 	gnirs/WFS/WFS/capfast/comp1mSadEng.sch
 * - 	gnirs/WFS/WFS/capfast/comp1mSadEng.sym
 * - 	gnirs/WFS/WFS/capfast/comp1mSadHealth.sch
 * - 	gnirs/WFS/WFS/capfast/comp1mSadHealth.sym
 * - 	gnirs/WFS/WFS/capfast/comp1mSadRec.sch
 * - 	gnirs/WFS/WFS/capfast/comp1mSadRec.sym
 * - 	gnirs/WFS/WFS/capfast/comp1pCadCmd.sch
 * - 	gnirs/WFS/WFS/capfast/comp1pCadCmd.sym
 * - 	gnirs/WFS/WFS/capfast/comp2CadSel.sch
 * - 	gnirs/WFS/WFS/capfast/comp2CadSel.sym
 * - 	gnirs/WFS/WFS/capfast/comp2Car.sch
 * - 	gnirs/WFS/WFS/capfast/comp2Car.sym
 * - 	gnirs/WFS/WFS/capfast/comp2m.sch
 * - 	gnirs/WFS/WFS/capfast/comp2m.sym
 * - 	gnirs/WFS/WFS/capfast/comp2mCad.sch
 * - 	gnirs/WFS/WFS/capfast/comp2mCad.sym
 * - 	gnirs/WFS/WFS/capfast/comp2mCadAlt.sch
 * - 	gnirs/WFS/WFS/capfast/comp2mCadAlt.sym
 * - 	gnirs/WFS/WFS/capfast/comp2mCadCmd.sch
 * - 	gnirs/WFS/WFS/capfast/comp2mCadCmd.sym
 * - 	gnirs/WFS/WFS/capfast/comp2mSad.sch
 * - 	gnirs/WFS/WFS/capfast/comp2mSad.sym
 * - 	gnirs/WFS/WFS/capfast/comp2mSadAlt.sch
 * - 	gnirs/WFS/WFS/capfast/comp2mSadAlt.sym
 * - 	gnirs/WFS/WFS/capfast/comp3mCadCmd.sch
 * - 	gnirs/WFS/WFS/capfast/comp3mCadCmd.sym
 * - 	gnirs/WFS/WFS/capfast/compCad.sch
 * - 	gnirs/WFS/WFS/capfast/compCad.sym
 * - 	gnirs/WFS/WFS/capfast/compSnlArg.sch
 * - 	gnirs/WFS/WFS/capfast/compSnlArg.sym
 * - 	gnirs/WFS/WFS/capfast/compSnlCmd.sch
 * - 	gnirs/WFS/WFS/capfast/compSnlCmd.sym
 * - 	gnirs/WFS/WFS/capfast/dummy.sym
 * - 	gnirs/WFS/WFS/capfast/eapplyx.sym
 * - 	gnirs/WFS/WFS/capfast/ecad2.sym
 * - 	gnirs/WFS/WFS/capfast/ecad4.sym gnirs/WFS/WFS/capfast/edb.def
 * - 	gnirs/WFS/WFS/capfast/egenSub.sym
 * - 	gnirs/WFS/WFS/capfast/escan.sym
 * - 	gnirs/WFS/WFS/capfast/escans.sym
 * - 	gnirs/WFS/WFS/capfast/esiread.cnf
 * - 	gnirs/WFS/WFS/capfast/estringin.sym
 * - 	gnirs/WFS/WFS/capfast/estringins.sym
 * - 	gnirs/WFS/WFS/capfast/ewait.sym gnirs/WFS/WFS/capfast/fol.sch
 * - 	gnirs/WFS/WFS/capfast/fol.sym gnirs/WFS/WFS/capfast/folCad.sch
 * - 	gnirs/WFS/WFS/capfast/folCad.sym
 * - 	gnirs/WFS/WFS/capfast/folCar.sch
 * - 	gnirs/WFS/WFS/capfast/folCar.sym
 * - 	gnirs/WFS/WFS/capfast/folRecords.sch
 * - 	gnirs/WFS/WFS/capfast/folRecords.sym
 * - 	gnirs/WFS/WFS/capfast/folSad.sch
 * - 	gnirs/WFS/WFS/capfast/folSad.sym
 * - 	gnirs/WFS/WFS/capfast/folSetCad.sch
 * - 	gnirs/WFS/WFS/capfast/folSetCad.sym
 * - 	gnirs/WFS/WFS/capfast/genTop
 * - 	gnirs/WFS/WFS/capfast/link2Dir.sch
 * - 	gnirs/WFS/WFS/capfast/link2Dir.sym
 * - 	gnirs/WFS/WFS/capfast/lock.sch gnirs/WFS/WFS/capfast/lock.sym
 * - 	gnirs/WFS/WFS/capfast/lockCad.sch
 * - 	gnirs/WFS/WFS/capfast/lockCad.sym
 * - 	gnirs/WFS/WFS/capfast/lockCar.sch
 * - 	gnirs/WFS/WFS/capfast/lockCar.sym
 * - 	gnirs/WFS/WFS/capfast/lockSad.sch
 * - 	gnirs/WFS/WFS/capfast/lockSad.sym
 * - 	gnirs/WFS/WFS/capfast/lockSadHealth.sch
 * - 	gnirs/WFS/WFS/capfast/lockSadHealth.sym
 * - 	gnirs/WFS/WFS/capfast/makeLinks
 * - 	gnirs/WFS/WFS/capfast/makePostscript
 * - 	gnirs/WFS/WFS/capfast/mfanout.sch
 * - 	gnirs/WFS/WFS/capfast/mfanout.sym
 * - 	gnirs/WFS/WFS/capfast/niriWfs.sch
 * - 	gnirs/WFS/WFS/capfast/niriWfs.sym
 * - 	gnirs/WFS/WFS/capfast/niriWfsSad.sch
 * - 	gnirs/WFS/WFS/capfast/niriWfsSad.sym
 * - 	gnirs/WFS/WFS/capfast/niriWfsSadSet.sch
 * - 	gnirs/WFS/WFS/capfast/niriWfsSadSet.sym
 * - 	gnirs/WFS/WFS/capfast/niriWfsSadTop.sch
 * - 	gnirs/WFS/WFS/capfast/niriWfsSet.sch
 * - 	gnirs/WFS/WFS/capfast/niriWfsSet.sym
 * - 	gnirs/WFS/WFS/capfast/niriWfsTop.sch
 * - 	gnirs/WFS/WFS/capfast/notes.sch
 * - 	gnirs/WFS/WFS/capfast/notes.sym
 * - 	gnirs/WFS/WFS/capfast/prbCad.sch
 * - 	gnirs/WFS/WFS/capfast/prbCad.sym
 * - 	gnirs/WFS/WFS/capfast/prbSad.sch
 * - 	gnirs/WFS/WFS/capfast/prbSad.sym gnirs/WFS/WFS/capfast/rmTop
 * - 	gnirs/WFS/WFS/capfast/snl.sch gnirs/WFS/WFS/capfast/snl.sym
 * - 	gnirs/WFS/WFS/capfast/templateC.sch
 * - 	gnirs/WFS/WFS/capfast/templateD.sch
 * - 	gnirs/WFS/WFS/capfast/templateTop.sch
 * - 	gnirs/WFS/WFS/capfast/tmpSad.sch
 * - 	gnirs/WFS/WFS/capfast/tmpSad.sym
 * - 	gnirs/WFS/WFS/capfast/tmpSadCool.sch
 * - 	gnirs/WFS/WFS/capfast/tmpSadCool.sym
 * - 	gnirs/WFS/WFS/capfast/tmpSadCtrl.sch
 * - 	gnirs/WFS/WFS/capfast/tmpSadCtrl.sym
 * - 	gnirs/WFS/WFS/capfast/tmpSadSens.sch
 * - 	gnirs/WFS/WFS/capfast/tmpSadSens.sym
 * - 	gnirs/WFS/WFS/capfast/wfsBeamSad.sch
 * - 	gnirs/WFS/WFS/capfast/wfsBeamSad.sym
 * - 	gnirs/WFS/WFS/capfast/wfsSysCad1.sch
 * - 	gnirs/WFS/WFS/capfast/wfsSysCad1.sym
 * - 	gnirs/WFS/WFS/capfast/wfsSysCad2.sch
 * - 	gnirs/WFS/WFS/capfast/wfsSysCad2.sym
 * - 	gnirs/WFS/WFS/capfast/wfsSysCad3.sch
 * - 	gnirs/WFS/WFS/capfast/wfsSysCad3.sym
 * - 	gnirs/WFS/WFS/capfast/wfsSysCad4.sch
 * - 	gnirs/WFS/WFS/capfast/wfsSysCad4.sym
 * - 	gnirs/WFS/WFS/capfast/wfsSysCar.sch
 * - 	gnirs/WFS/WFS/capfast/wfsSysCar.sym
 * - 	gnirs/WFS/WFS/capfast/wfsSysSad.sch
 * - 	gnirs/WFS/WFS/capfast/wfsSysSad.sym
 * - 	gnirs/WFS/WFS/capfast/wfsSystem.sch
 * - 	gnirs/WFS/WFS/capfast/wfsSystem.sym gnirs/WFS/WFS/dl/Makefile
 * - 	gnirs/WFS/WFS/dl/Makefile.Unix gnirs/WFS/WFS/dl/fol.adl
 * - 	gnirs/WFS/WFS/dl/folEng.adl gnirs/WFS/WFS/dl/folSad.adl
 * - 	gnirs/WFS/WFS/dl/gmColors.adl gnirs/WFS/WFS/dl/niriWfs.adl
 * - 	gnirs/WFS/WFS/dl/niriWfsConsole.adl
 * - 	gnirs/WFS/WFS/dl/niriWfsOld.adl gnirs/WFS/WFS/dl/template.adl
 * - 	gnirs/WFS/WFS/dl/wfsReboot.adl gnirs/WFS/WFS/pv/Makefile
 * - 	gnirs/WFS/WFS/pv/Makefile.Unix gnirs/WFS/WFS/pv/Makefile.Vx
 * - 	gnirs/WFS/WFS/pv/debug.pv gnirs/WFS/WFS/pv/filt.lut
 * - 	gnirs/WFS/WFS/pv/filt.pv gnirs/WFS/WFS/pv/filtBeam.lut
 * - 	gnirs/WFS/WFS/pv/foc.lut gnirs/WFS/WFS/pv/foc.pv
 * - 	gnirs/WFS/WFS/pv/fol.lut gnirs/WFS/WFS/pv/fol.pv
 * - 	gnirs/WFS/WFS/pv/lockCfg.lut gnirs/WFS/WFS/pv/lockCfg.pv
 * - 	gnirs/WFS/WFS/pv/lockGen.lut gnirs/WFS/WFS/pv/lockGen.pv
 * - 	gnirs/WFS/WFS/pv/lockObs.lut gnirs/WFS/WFS/pv/lockObs.pv
 * - 	gnirs/WFS/WFS/pv/lockTmp.lut gnirs/WFS/WFS/pv/lockTmp.pv
 * - 	gnirs/WFS/WFS/pv/prb.lut gnirs/WFS/WFS/pv/prb.pv
 * - 	gnirs/WFS/WFS/pv/prbx.lut gnirs/WFS/WFS/pv/prby.lut
 * - 	gnirs/WFS/WFS/pv/sys1.pv gnirs/WFS/WFS/pv/sys2.pv
 * - 	gnirs/WFS/WFS/src/Makefile gnirs/WFS/WFS/src/Makefile.Unix
 * - 	gnirs/WFS/WFS/src/Makefile.Vx gnirs/WFS/WFS/src/cad.c
 * - 	gnirs/WFS/WFS/src/cicsConst.h gnirs/WFS/WFS/src/cicsLib.h
 * - 	gnirs/WFS/WFS/src/compLib.h gnirs/WFS/WFS/src/fol.c
 * - 	gnirs/WFS/WFS/startup/Makefile
 * - 	gnirs/WFS/WFS/startup/Makefile.Unix
 * - 	gnirs/WFS/WFS/startup/Makefile.Vx gnirs/WFS/WFS/startup/README
 * - 	gnirs/WFS/WFS/startup/UAE.dist
 * - 	gnirs/WFS/WFS/startup/local.flex.vws
 * - 	gnirs/WFS/WFS/startup/local.vws
 * - 	gnirs/WFS/WFS/startup/resource.def
 * - 	gnirs/WFS/WFS/startup/resource.def.flex
 * - 	gnirs/WFS/WFS/startup/startup.VWS
 * - 	gnirs/WFS/WFS/startup/startup.gnirs.VWS
 * - 	gnirs/WFS/WFS/startup/startup.gnirs.flex.VWS
 * - 	gnirs/WFS/WFS/startup/startupAlt.VWS
 * - 	gnirs/WFS/WFS/startup/startupSim.VWS
 * - 	gnirs/WFS/WFS/startup/subst gnirs/WFS/WFS/startup/wfsParm.VWS
 * - 	gnirs/WFS/WFS/startup/wfsSeq.VWS
 * - 	gnirs/WFS/WFS/startup/WFS/WFS/startup/Makefile
 * - 	gnirs/WFS/WFS/startup/WFS/WFS/startup/Makefile.Unix
 * - 	gnirs/WFS/WFS/startup/WFS/WFS/startup/Makefile.Vx
 * - 	gnirs/WFS/WFS/startup/WFS/WFS/startup/resource.def.flex
 * - 	gnirs/WFS/dl/Makefile gnirs/WFS/dl/Makefile.Unix
 * - 	gnirs/WFS/dl/gmColors.adl gnirs/WFS/dl/gmColors.dl
 * - 	gnirs/WFS/dl/niri.adl gnirs/WFS/dl/observe.adl
 * - 	gnirs/WFS/dl/template.adl gnirs/WFS/dl/testCar
 * - 	gnirs/WFS/dl/testCar.adl gnirs/WFS/dl/testCarMenu.adl
 * - 	gnirs/WFS/docs/0Contents gnirs/WFS/docs/Makefile
 * - 	gnirs/WFS/docs/fmdictionary gnirs/WFS/docs/niri_hty.book
 * - 	gnirs/WFS/docs/niri_hty.pdf gnirs/WFS/docs/niri_hty.ps
 * - 	gnirs/WFS/docs/niri_hty_000.fm gnirs/WFS/docs/niri_hty_001.fm
 * - 	gnirs/WFS/docs/niri_hty_001.pdf gnirs/WFS/docs/niri_hty_001.ps
 * - 	gnirs/WFS/docs/niri_hty_002.fm gnirs/WFS/docs/niri_hty_002.pdf
 * - 	gnirs/WFS/docs/niri_hty_002.ps gnirs/WFS/docs/niri_hty_003.fm
 * - 	gnirs/WFS/docs/niri_hty_003.fm.backup
 * - 	gnirs/WFS/docs/niri_hty_003.pdf gnirs/WFS/docs/niri_hty_003.ps
 * - 	gnirs/WFS/docs/niri_hty_004.fm gnirs/WFS/docs/niri_hty_004.pdf
 * - 	gnirs/WFS/docs/niri_hty_004.ps gnirs/WFS/docs/niri_hty_005.fm
 * - 	gnirs/WFS/docs/niri_hty_005.pdf gnirs/WFS/docs/niri_hty_005.ps
 * - 	gnirs/WFS/docs/niri_hty_006.fm gnirs/WFS/docs/niri_hty_006.pdf
 * - 	gnirs/WFS/docs/niri_hty_006.ps gnirs/WFS/docs/niri_hty_007.fm
 * - 	gnirs/WFS/docs/niri_hty_007.pdf gnirs/WFS/docs/niri_hty_007.ps
 * - 	gnirs/WFS/docs/niri_hty_008.fm gnirs/WFS/docs/niri_hty_008.pdf
 * - 	gnirs/WFS/docs/niri_hty_008.ps gnirs/WFS/docs/niri_hty_009.fm
 * - 	gnirs/WFS/docs/niri_hty_009.pdf gnirs/WFS/docs/niri_hty_009.ps
 * - 	gnirs/WFS/docs/niri_hty_010.fm gnirs/WFS/docs/niri_hty_010.pdf
 * - 	gnirs/WFS/docs/niri_hty_010.ps gnirs/WFS/docs/niri_hty_011.fm
 * - 	gnirs/WFS/docs/niri_hty_011.pdf gnirs/WFS/docs/niri_hty_011.ps
 * - 	gnirs/WFS/docs/niri_hty_012.fm gnirs/WFS/docs/niri_hty_012.pdf
 * - 	gnirs/WFS/docs/niri_hty_012.ps gnirs/WFS/docs/niri_hty_013.fm
 * - 	gnirs/WFS/docs/niri_hty_014.fm gnirs/WFS/docs/niri_hty_015.fm
 * - 	gnirs/WFS/docs/niri_hty_015.pdf gnirs/WFS/docs/niri_hty_015.ps
 * - 	gnirs/WFS/docs/niri_hty_016.fm gnirs/WFS/docs/niri_hty_016.pdf
 * - 	gnirs/WFS/docs/niri_hty_016.ps gnirs/WFS/docs/niri_hty_TOC.doc
 * - 	gnirs/WFS/docs/niri_hty_TOC.pdf gnirs/WFS/docs/niri_hty_TOC.ps
 * - 	gnirs/WFS/docs/oiwfs_hty.book gnirs/WFS/docs/oiwfs_hty.pdf
 * - 	gnirs/WFS/docs/oiwfs_hty.ps gnirs/WFS/docs/oiwfs_hty_TOC.doc
 * - 	gnirs/WFS/docs/oiwfs_hty_TOC.pdf
 * - 	gnirs/WFS/docs/oiwfs_hty_TOC.ps
 * - 	gnirs/WFS/docs/figs/CycleTool.gif
 * - 	gnirs/WFS/docs/figs/ScanTool.gif gnirs/WFS/docs/figs/a0a.dat
 * - 	gnirs/WFS/docs/figs/filt1.dat
 * - 	gnirs/WFS/docs/figs/gnirs-x-linear.gif
 * - 	gnirs/WFS/docs/figs/gnirs-y-linear.gif
 * - 	gnirs/WFS/docs/figs/hallstep.gif
 * - 	gnirs/WFS/docs/figs/home12a.dat
 * - 	gnirs/WFS/docs/figs/hsCycle.gif
 * - 	gnirs/WFS/docs/figs/hsDebug.gif gnirs/WFS/docs/figs/hsfilt.gif
 * - 	gnirs/WFS/docs/figs/hsfilt.plt gnirs/WFS/docs/figs/hshome.gif
 * - 	gnirs/WFS/docs/figs/hshome.plt gnirs/WFS/docs/figs/hslin.gif
 * - 	gnirs/WFS/docs/figs/hslin.plt gnirs/WFS/docs/figs/mech.sym.gif
 * - 	gnirs/WFS/docs/figs/niriDiag.gif
 * - 	gnirs/WFS/docs/figs/niriEng.gif
 * - 	gnirs/WFS/docs/figs/engui/comp1m.gif
 * - 	gnirs/WFS/docs/figs/engui/comp1mSad.gif
 * - 	gnirs/WFS/docs/figs/engui/comp2m.gif
 * - 	gnirs/WFS/docs/figs/engui/comp2mSad.gif
 * - 	gnirs/WFS/docs/figs/engui/cycle_tool.gif
 * - 	gnirs/WFS/docs/figs/engui/engCc.gif
 * - 	gnirs/WFS/docs/figs/engui/engGbl.gif
 * - 	gnirs/WFS/docs/figs/engui/engLock.gif
 * - 	gnirs/WFS/docs/figs/engui/engMech.gif
 * - 	gnirs/WFS/docs/figs/engui/engParams.gif
 * - 	gnirs/WFS/docs/figs/engui/engShs.gif
 * - 	gnirs/WFS/docs/figs/engui/engTmp.gif
 * - 	gnirs/WFS/docs/figs/engui/engWfs.gif
 * - 	gnirs/WFS/docs/figs/engui/fol.gif
 * - 	gnirs/WFS/docs/figs/engui/folEng.gif
 * - 	gnirs/WFS/docs/figs/engui/folSad.gif
 * - 	gnirs/WFS/docs/figs/engui/lock.gif
 * - 	gnirs/WFS/docs/figs/engui/lockSad.gif
 * - 	gnirs/WFS/docs/figs/engui/niri.gif
 * - 	gnirs/WFS/docs/figs/engui/niriCc.gif
 * - 	gnirs/WFS/docs/figs/engui/niriCcConsole.gif
 * - 	gnirs/WFS/docs/figs/engui/niriCcShared.gif
 * - 	gnirs/WFS/docs/figs/engui/niriEng.gif
 * - 	gnirs/WFS/docs/figs/engui/niriIs.gif
 * - 	gnirs/WFS/docs/figs/engui/niriWfs.gif
 * - 	gnirs/WFS/docs/figs/engui/niriWfsConsole.gif
 * - 	gnirs/WFS/docs/figs/engui/observe.gif
 * - 	gnirs/WFS/docs/figs/engui/position_tool.gif
 * - 	gnirs/WFS/docs/figs/engui/sysDebug.gif
 * - 	gnirs/WFS/docs/figs/engui/tmp.gif
 * - 	gnirs/WFS/docs/figs/engui/tmpSad.gif
 * - 	gnirs/WFS/docs/templates/icd_form.fm
 * - 	gnirs/WFS/docs/templates/intro.fm
 * - 	gnirs/WFS/docs/templates/subsys.fm
 * - 	gnirs/WFS/docs/tools/sirCc.txt gnirs/WFS/docs/tools/sirIs.txt
 * - 	gnirs/WFS/docs/tools/sirList gnirs/WFS/docs/tools/sirWfs.txt
 * - 	gnirs/WFS/scripts/Makefile gnirs/WFS/scripts/Makefile.Unix
 * - 	gnirs/WFS/scripts/README gnirs/WFS/scripts/ccSnapshot.SH
 * - 	gnirs/WFS/scripts/compClear.SH
 * - 	gnirs/WFS/scripts/compEngMove.SH gnirs/WFS/scripts/compMove.SH
 * - 	gnirs/WFS/scripts/compPreset.SH gnirs/WFS/scripts/compShow.SH
 * - 	gnirs/WFS/scripts/compShowEng.SH
 * - 	gnirs/WFS/scripts/compStart.SH gnirs/WFS/scripts/compWait.SH
 * - 	gnirs/WFS/scripts/coolRate.SH gnirs/WFS/scripts/dcGo.SH
 * - 	gnirs/WFS/scripts/dcSetTime.SH gnirs/WFS/scripts/sample
 * - 	gnirs/WFS/scripts/scriptRun.SH gnirs/WFS/scripts/scriptTest.SH
 * - 	gnirs/WFS/scripts/subst gnirs/WFS/scripts/util.sh
 * - 	gnirs/WFS/scripts/wfsSnapshot.SH gnirs/WFS/startup/Makefile
 * - 	gnirs/WFS/startup/Makefile.Unix gnirs/WFS/startup/Makefile.Vx
 * - 	gnirs/WFS/startup/README gnirs/WFS/startup/UAE.dist
 * - 	gnirs/WFS/startup/local.vws gnirs/WFS/startup/resource.def
 * - 	gnirs/WFS/startup/startup.VWS gnirs/WFS/startup/startupSim.VWS
 * - 	gnirs/WFS/startup/subst gnirs/WFS/wfsfiles/0README.txt
 * - 	gnirs/WFS/wfsfiles/Makefile
 * - 	gnirs/WFS/wfsfiles/Makefile.subdirs
 * - 	gnirs/WFS/wfsfiles/niriSetup gnirs/WFS/wfsfiles/niriWfsStart
 * - 	gnirs/WFS/wfsfiles/nirs.env gnirs/WFS/wfsfiles/nirsSetup
 * - 	gnirs/WFS/wfsfiles/nirsWfsStart
 * - 	gnirs/WFS/wfsfiles/startupParams
 * - 	gnirs/WFS/wfsfiles/ENG/Makefile
 * - 	gnirs/WFS/wfsfiles/ENG/nirsSetup
 * - 	gnirs/WFS/wfsfiles/LIB/Makefile
 * - 	gnirs/WFS/wfsfiles/LIB/Makefile.subdirs
 * - 	gnirs/WFS/wfsfiles/LIB/nirsSetup gnirs/alh/Makefile
 * - 	gnirs/ascii/Makefile gnirs/ascii/Makefile.Unix
 * - 	gnirs/capfast/Makefile gnirs/capfast/Makefile.Unix
 * - 	gnirs/capfast/cad.rc gnirs/dl/Makefile gnirs/dl/Makefile.Unix
 * - 	gnirs/dl/colors.adl gnirs/dl/flexStart gnirs/dl/flexStart.old
 * - 	gnirs/dl/gmColors.adl gnirs/dl/gmColors.dl
 * - 	gnirs/dl/nirsSeqStart gnirs/dl/nirsSeqTop.adl
 * - 	gnirs/dl/nirsSeqTop.dl gnirs/dl/nirsStart
 * - 	gnirs/dl/nirsWfsEng.adl gnirs/dl/sad.adl gnirs/dl/sad.dl
 * - 	gnirs/dl/template.adl gnirs/dl/template.dl gnirs/dl/wfs.adl
 * - 	gnirs/dl/wfs.dl gnirs/dl/wfsCC.adl gnirs/dl/wfsCC.dl
 * - 	gnirs/dl/converttmp/colors.adl
 * - 	gnirs/dl/converttmp/template.adl gnirs/include/debug.h
 * - 	gnirs/include/gmSeq.h gnirs/include/mechNames.h
 * - 	gnirs/include/saverCommon.h gnirs/include/sockutil.h
 * - 	gnirs/include/include/mechNames.h
 * - 	gnirs/include/rec/aaiRecord.h gnirs/include/rec/aaoRecord.h
 * - 	gnirs/include/rec/aiRecord.h gnirs/include/rec/aoRecord.h
 * - 	gnirs/include/rec/applyRecord.h gnirs/include/rec/biRecord.h
 * - 	gnirs/include/rec/boRecord.h gnirs/include/rec/cadRecord.h
 * - 	gnirs/include/rec/calcRecord.h gnirs/include/rec/carRecord.h
 * - 	gnirs/include/rec/compressRecord.h
 * - 	gnirs/include/rec/dbCommon.h gnirs/include/rec/dfanoutRecord.h
 * - 	gnirs/include/rec/egRecord.h gnirs/include/rec/egeventRecord.h
 * - 	gnirs/include/rec/erRecord.h gnirs/include/rec/ereventRecord.h
 * - 	gnirs/include/rec/eventRecord.h
 * - 	gnirs/include/rec/fanoutRecord.h
 * - 	gnirs/include/rec/genSubRecord.h
 * - 	gnirs/include/rec/histogramRecord.h
 * - 	gnirs/include/rec/loadRecord.h
 * - 	gnirs/include/rec/longinRecord.h
 * - 	gnirs/include/rec/longoutRecord.h
 * - 	gnirs/include/rec/lutinRecord.h
 * - 	gnirs/include/rec/lutoutRecord.h
 * - 	gnirs/include/rec/mbbiDirectRecord.h
 * - 	gnirs/include/rec/mbbiRecord.h
 * - 	gnirs/include/rec/mbboDirectRecord.h
 * - 	gnirs/include/rec/mbboRecord.h gnirs/include/rec/mosubRecord.h
 * - 	gnirs/include/rec/motorRecord.h
 * - 	gnirs/include/rec/permissiveRecord.h
 * - 	gnirs/include/rec/pidRecord.h
 * - 	gnirs/include/rec/pulseCounterRecord.h
 * - 	gnirs/include/rec/pulseDelayRecord.h
 * - 	gnirs/include/rec/pulseTrainRecord.h
 * - 	gnirs/include/rec/scanRecord.h gnirs/include/rec/selRecord.h
 * - 	gnirs/include/rec/seqRecord.h gnirs/include/rec/sirRecord.h
 * - 	gnirs/include/rec/stateRecord.h
 * - 	gnirs/include/rec/statusRecord.h
 * - 	gnirs/include/rec/steppermotorRecord.h
 * - 	gnirs/include/rec/stringinRecord.h
 * - 	gnirs/include/rec/stringoutRecord.h
 * - 	gnirs/include/rec/subArrayRecord.h
 * - 	gnirs/include/rec/subCadRecord.h gnirs/include/rec/subRecord.h
 * - 	gnirs/include/rec/timerRecord.h gnirs/include/rec/waitRecord.h
 * - 	gnirs/include/rec/waveformRecord.h gnirs/pv/Makefile
 * - 	gnirs/pv/Makefile.Unix gnirs/pv/Makefile.Vx gnirs/src/Makefile
 * - 	gnirs/src/Makefile.Unix gnirs/src/Makefile.Vx
 * - 	gnirs/startup/Makefile gnirs/startup/Makefile.Unix
 * - 	gnirs/startup/Makefile.Vx gnirs/startup/UAE.dist
 * - 	gnirs/startup/local.flex.vws gnirs/startup/local.vws
 * - 	gnirs/startup/resource.def gnirs/startup/resource.def.flex
 * - 	gnirs/startup/startup.IS.flex.vws
 * - 	gnirs/startup/startup.IS.seed.vws gnirs/startup/startup.IS.vws
 * - 	gnirs/startup/startup.flex.vws
 * - 	gnirs/startup/startup.is.vws.old
 * - 	gnirs/startup/startup.python.vws gnirs/startup/startup.sim.vws
 * - 	gnirs/startup/startup.vws gnirs/testing/Makefile
 * - 	gnirs/testing/Makefile.Unix gnirs/testing/Makefile.Vx
 * - 	gnirs/testing/headers.c gnirs/testing/logger.c
 * - 	gnirs/testing/logger.py gnirs/testing/logger.py.bak
 * - 	gnirs/testing/loggerError gnirs/testing/pTest.c
 * - 	gnirs/testing/pTest.c.bak gnirs/testing/pTest.py
 * - 	gnirs/testing/testing/logger.c gnirs/vxWorks/mv162/,config.h
 * - 	gnirs/vxWorks/mv162/,sysLib.c gnirs/vxWorks/mv162/MakeSkel
 * - 	gnirs/vxWorks/mv162/Makefile
 * - 	gnirs/vxWorks/mv162/Makefile.MC68040gnu
 * - 	gnirs/vxWorks/mv162/README gnirs/vxWorks/mv162/config.h
 * - 	gnirs/vxWorks/mv162/dataSegPad.o gnirs/vxWorks/mv162/mv162.h
 * - 	gnirs/vxWorks/mv162/romInit.s gnirs/vxWorks/mv162/symTbl.c
 * - 	gnirs/vxWorks/mv162/sysALib.o gnirs/vxWorks/mv162/sysALib.s
 * - 	gnirs/vxWorks/mv162/sysLib.c gnirs/vxWorks/mv162/sysLib.c.v1
 * - 	gnirs/vxWorks/mv162/sysLib.o gnirs/vxWorks/mv162/target.nr
 * - 	gnirs/vxWorks/mv162/tyCoDrv.c gnirs/vxWorks/mv162/tyCoDrv.o
 * - 	gnirs/vxWorks/mv162/usrConfig.o gnirs/vxWorks/mv162/vxWorks
 * - 	gnirs/vxWorks/mv162/vxWorks.sym gnirs/vxWorks/mv162/wrs.an
 * - 	gnirs/vxWorks/mv167/Makefile.MC68040gnu
 * - 	gnirs/vxWorks/mv167/config.h gnirs/vxWorks/mv167/dataSegPad.o
 * - 	gnirs/vxWorks/mv167/sysALib.o gnirs/vxWorks/mv167/sysLib.o
 * - 	gnirs/vxWorks/mv167/tyCoDrv.o gnirs/vxWorks/mv167/usrConfig.o
 * - 	gnirs/vxWorks/mv167/vxWorks gnirs/vxWorks/mv167/vxWorks.sym
 * - 	gnirs/vxWorks/mv167/wrs.an
 * - ----------------------------------------------------------------------
 *
 * Revision 1.28  2001/02/28 12:46:39  gmos
 * gmSeqDisplayCADTest function added.
 *
 * Revision 1.27  2001/02/28 10:34:47  gmos
 * Maximum wavelength changed from 1000nm to 1100nm
 *
 * Revision 1.26  2001/02/23 13:12:50  gmos
 * Renamed global variables so they begin gmSeq. Added more comments.
 *
 * Revision 1.25  2001/02/21 18:14:16  gmos
 * New code to separate mask data into three separate menus - one for each cassette.
 *
 * Revision 1.24  2001/02/01 11:02:52  gmos
 * Min. and Max wavelengths should be floating point constants.
 *
 * Revision 1.23  2001/01/29 18:00:59  gmos
 * Added filter effective wavelength and grating focus offset to database. Define string buffer size here.
 *
 * Revision 1.22  2001/01/29 15:02:19  gmos
 * Reading lookup tables could result in buffer overflow. Fixed.
 *
 * Revision 1.21  2001/01/23 11:58:34  gmos
 * Translation stage limits now include a border around the edge.
 *
 * Revision 1.20  2000/12/19 13:32:16  gmos
 * Define DBG_QUIET.
 *
 * Revision 1.19  2000/12/15 11:35:25  gmos
 * MASTER_ENABLE test condition added. Test for MASTER_ENABLE in all commands except INIT and TEST.
 *
 * Revision 1.18  2000/09/22 08:21:48  gmos
 * Parameters used to define a universal wavelength range of 300 to 1000 nanometres.
 *
 * Revision 1.17  2000/09/20 14:05:45  gmos
 * Fixed mistake in CVS variables Log and Id
 *
 */
/* *INDENT-ON* */

#ifndef INCgmseqh
#define INCgmseqh

#include <lstLib.h>	/* Contains definition of NODE */
#include <dbDefs.h>     /* Contains definition of MAX_STRING_SIZE */

/* Bit masks for CAD command test conditions */
#define CONFIGURING    0x0001     /* If set, reject when instrument configuring.    */
#define READING_OUT    0x0002     /* If set, reject when detector reading out.      */
#define OBSERVING      0x0004     /* If set, reject when detector acquiring data.   */
#define MASTER_ENABLE  0x0008     /* If set, reject when master enable not enabled. */

/*
 * Limits (microns) for the demand positions of the detector translation stage,
 * before flexure offsets are applied. These limits are set inside the actual
 * range of the translation assembly by the defined X, Y and Z borders to allow
 * for flexure offsets.
 */

#define DTAXBORDER    30.0  /* Size of X border (microns) */
#define DTAYBORDER    30.0  /* Size of Y border (microns) */
#define DTAZBORDER    30.0  /* Size of Z border (microns) */

#define DTAABSXMAX   230.0  /* Maximum X of detector translation stage (microns) */
#define DTAABSXMIN   -25.0  /* Minimum X of detector translation stage (microns) */
#define DTAABSYMAX   230.0  /* Maximum Y of detector translation stage (microns) */
#define DTAABSYMIN   -15.0  /* Minimum Y of detector translation stage (microns) */
#define DTAABSZMAX  4735.0  /* Maximum Z of detector translation stage (microns) */
#define DTAABSZMIN     0.0  /* Minimum Z of detector translation stage (microns) */

#define DTAXMAX   (DTAABSXMAX - DTAXBORDER)
#define DTAXMIN   (DTAABSXMIN + DTAXBORDER)
#define DTAYMAX   (DTAABSYMAX - DTAYBORDER)
#define DTAYMIN   (DTAABSYMIN + DTAYBORDER)
#define DTAZMAX   (DTAABSZMAX - DTAZBORDER)
#define DTAZMIN   (DTAABSZMIN + DTAZBORDER)

/* Limits (degrees) for movement of atmDC entrance and exit prism angles */

#define ATMENPMIN -360.0  /* atmDC entrance prism minimum angle (degrees) */
#define ATMENPMAX  360.0  /* atmDC entrance prism maximum angle (degrees) */
#define ATMEXPMIN -360.0  /* atmDC exit prism minimum angle (degrees)     */
#define ATMEXPMAX  360.0  /* atmDC exit prism maximum angle (degrees)     */

/* Min. and Max acceptable wavelengths (nanometres) */
#define MIN_WAVELENGTH  300.0
#define MAX_WAVELENGTH 1100.0

/*
 * Tracking modes for atmospheric dispersion compensator and detector
 * translation.
 */
#define TRACKSTOP     0
#define TRACKMOVE     1
#define TRACKFOLLOW   2
#define TRACKFOLLOWXY 3

/*
 * Define the debug levels recognised by the GMOS instrument sequencer.
 */

#define DBG_QUIET 0x0000
#define DBG_NONE  0x0001
#define DBG_MIN   0x0002
#define DBG_FULL  0x0004
#define DBG_MAX   0x0008

/*
 * Define the debugging macros used by the intrument sequencer.
 * Each macro will display the message associated with it if the
 * bit corresponding to the debugging level is defined in the dbglevel
 * bit mask.
 */

extern int epicsPrintf(char *, ...);   /* printf function provided by EPICS logging */

#define DBGMSG( _dbgLevel, _dbgMsg) \
  do { \
     if (gmSeqDbgLevel & (_dbgLevel) ) epicsPrintf ("gmSeq: %s\n", _dbgMsg) ; \
  } while(0)

#define DBGMSGSTRING( _dbgLevel, _dbgMsg, _dbgString) \
  do { \
     if (gmSeqDbgLevel & (_dbgLevel) ) epicsPrintf ("gmseq: %s %s\n", _dbgMsg, _dbgString) ; \
  } while(0)

#define DBGMSGREAL( _dbgLevel, _dbgMsg, _dbgFloat) \
  do { \
     if (gmSeqDbgLevel & (_dbgLevel) ) epicsPrintf ("gmseq: %s %f\n", _dbgMsg, _dbgFloat) ; \
  } while(0)


#define DBGMSGINT( _dbgLevel, _dbgMsg, _dbgInt) \
  do { \
     if (gmSeqDbgLevel & (_dbgLevel) ) epicsPrintf ("gmSeq: %s %d\n", _dbgMsg, _dbgInt) ; \
  } while(0)

/*
 * GMOS limits, array and buffer size definitions
 */

#define FILTPERWHEEL    12      /* Max filters per wheel                                */
#define MASKPERCASS      9      /* Max masks per cassette                               */
#define MAXGRATINGS      4      /* Max loaded gratings                                  */

#define MAXMENU         16      /* Max number of strings allowed in mbbi menu           */
#define LUT_TAG_SZ	MAX_STRING_SIZE
                                /* Make LUT name size same as maximum EPICS string size */
#define STRING_BUF_SZ   256     /* Size of large string buffers.                        */
#define HALF_BUF_SZ     128     /* Half size of large string buffers.                   */


/*
 * Structure for data from filters.lut data file describing all the filters
 * known to the software.
 */ 

typedef struct FILTLUT
{
    NODE node;                     /* Next node in linked list.                  */
    char tag[LUT_TAG_SZ];          /* Filter name.                               */
    long barcodeId;                /* Barcode ID of filter.                      */
    double focusOffset;            /* Focus offset of filter (microns)           */
    double effWavelength;          /* Effective wavelength of filter (nm)        */
} FILTLUT;

/*
 * Structure for data from flt.lut data file describing the filters
 * installed on a particular filter wheel.
 */

typedef struct WHEELFILTLUT
{
    NODE node;                     /* Next node in linked list.                  */
    long barcode;                  /* Barcode ID of filter.                      */
    long wheelnum;                 /* Wheel number on which filter installed.    */
    char position[LUT_TAG_SZ];     /* Position name on wheel.                    */
} WHEELFILTLUT;

/*
 * Structure for data from gratings.lut data file describing all the gratings
 * known to the software. There are two entries for each grating representing
 * the two possible orientations it may be installed in.
 */ 

typedef struct GRATLUT
{
    NODE node;                     /* Next node in linked list.                  */
    char tag[LUT_TAG_SZ];          /* Grating/orientation combination name.      */
    long barcodeId;                /* Barcode ID of grating at this orientation. */
    long linesPerMm;               /* Ruling density (lines per mm).             */
    long blazeDir;                 /* Blaze direction at this orientation.       */
    double focusOffset;            /* Focus offset of grating (microns).         */
} GRATLUT;

/*
 * Structure for data from gr.lut data file describing the gratings
 * installed on the turret, and recording their tilt angles.
 */

typedef struct LOADEDGRATLUT
{
    NODE node;                     /* Next node in linked list.                  */
    long barcode;                  /* Barcode ID of grating.                     */
    long turretPos;                /* Turret position at which grating installed.*/
    double tilt;                   /* Current grating tilt angle (degrees).      */
} LOADEDGRATLUT;

/*
 * Structure for data from masks.lut data file describing all the special named
 * masks known to the software. NOTE: Only the special masks have names. Most of
 * the science masks are referred to only by their barcodes.
 */ 

typedef struct MASKLUT
{
    NODE node;                     /* Next node in linked list.                  */
    char tag[LUT_TAG_SZ];          /* Mask name.                                 */
    long barcodeId;                /* Barcode ID of mask.                        */
} MASKLUT;

/*
 * Structure for data from msk.lut data file describing the masks
 * installed in the loaded cassettes.
 */

typedef struct LOADEDMASKLUT
{
    NODE node;                     /* Next node in linked list.                  */
    long barcode;                  /* Barcode ID of mask.                        */
    long cassette;                 /* Cassette in which mask is installed.       */
    long slot;                     /* Slot number of mask within cassette.       */
} LOADEDMASKLUT;


/* Global and external functions */

/* 
 * Convert a string to upper case.
 */
char * gmSeqUc(char *in, int lout, char *out);

/*
 * Return a grating tilt angle for a given wavelength, order and ruling density 
 */
int gratingSci2Tilt(
            const double wavelength,    /* Central wavelength in nm              */
            const int order,            /* Grating order                         */
            const double rulingDensity, /* Grating ruling density per mm         */
            double *tiltAngle);         /* Pointer grating tilt angle in degrees */

/*
 * Functions for managing the bit mask of CAD command test conditions.
 */
void gmSeqSetCADTest(const int testMask);
void gmSeqClearCADTest();
int  gmSeqGetCADTest();
void gmSeqDisplayCADTest();

/*
 * Decode a string by matching an array element.
 */
int  gmSeqDcString(char **match, char *string);

#endif	/* !INCgmseqh */
