/*
 * Copyright 1997 Association of Universities for Research in Astronomy, Inc.
 * See the file COPYRIGHT for more details.
 *
 *    FILENAME:
 *    epCommon.h
 *
 *    PURPOSE:
 *     This include file contains definitions and prototypes used in the
 *     EPICS support routines in the GNAAC software system.
 * 
 *INDENT-OFF*
 * $Log: epCommon.h,v $
 * Revision 1.3  2013/06/06 01:54:26  gemvx
 * Checking in for fixes related to REL-1149.
 *
 * REL-1149 requires moving FW1 into a blocking position when the aquisition
 * mirror is moving out. This will inhibit, "squiggles" on the detector caused
 * by a bright star reflecting off of the aquisition mirror onto the detector.
 *
 * See: http://swgserv01.cl.gemini.edu:8080/browse/REL-1149
 *
 * tom.c
 *
 * Revision 1.2  2009/05/27 19:31:50  fkraemer
 * fkraemer - copied my complete working dir over trunk
 *
 *INDENT-ON* 
 */

#ifndef EPCOMMON
#define EPCOMMON

#define DEBUG

/*	Language include files		*/
#include  <vxWorks.h>
#include  <types.h>
#include  <math.h>
#include  <time.h>
#include  <stdlib.h>
#include  <stdioLib.h>
#include  <string.h>

#if !defined(NODBACCESS)
/*	EPICS include files			*/
#include  <dbDefs.h>
#include  <genSubRecord.h>
#include  <dbCommon.h>
#include  <recSup.h>
#include  <dbAccess.h>


#endif

#include  <cicsConst.h>
#include  <cicsLib.h>
#include  <cadRecord.h>
#include  <cad.h>
#include "epicsDefines.h"

/* MACROS*/
#ifdef DEBUG 
extern int DPdebug; 
#define Dprint(a) (fputs (a,stderr), fflush(stderr))
#define DPRINT(a,b,c) if(b<=a)(fputs (c,stderr), fflush(stderr))
	 
#else
#define DPRINT(a,b,c)
#define Dprint(a)
#endif 

#define cadInput(x) INM(x)
#define INM(x) pCad-> ## x
#define cadOutput(x) OUTM(x)
#define OUTM(x) pCad->val ## x
#define type(x) TYPEM(x)
#define TYPEM(x)pCad->ftv ## x

#define LOG_MSG(a,b)	cicsLogMessage(a,b)


/* debug level defines*/
#define DBG_QUIET 0
#define DBG_NONE  1
#define DBG_MIN   2
#define DBG_FULL  3
#define DBG_MAX  4



/* error numbers*/
#define ERROR_MEMORY 	  	-99
#define ERROR_ABORT       	-18
#define ERROR_DHS_CONNECT 	-19
#define ERROR_DIRECTIVE 	-6
#define ERROR_DATUM	 	-23
#define ERROR_DIAGNOSE	 	-24
#define ERROR_EPICS 		-3
#define ERROR_MICROCODE 	-10
#define ERROR_NOSTOP 		-5
#define ERROR_NOTPAUSED 	-7
#define ERROR_PARK		-20
#define ERROR_PAUSE		-15
#define ERROR_POWER		-17
#define ERROR_RANGE 		-2
#define ERROR_REBOOT		-21
#define ERROR_RESUME		-16
#define ERROR_ROI 		-14
#define ERROR_SEQUENCE 		-4
#define ERROR_SIM 		-9
#define ERROR_STOP  		-13
#define ERROR_TYPE 		-1
#define ERROR_VALUE 		-8
#define ERROR_TEST 		-11
#define ERROR_VMEINIT 		-12
#define ERROR_WCS		   -22

/* cad/car status defines*/
#define SCCD_ERROR 3
#define SCCD_START_DONE 0
#define SCCD_DONE 0
#define SCCD_NOT_DONE 1
#define SCCD_PRESET_DONE 10
#define SCCD_UNKNOWN 1
#define SCCD_BUSY 2


#define SCCD_PAUSED 1
#define SCCD_NOT_PAUSED 0

#define OBSERVATION_IN_PROGRESS     0
#define OBSERVATION_NOT_IN_PROGRESS 1



enum HEADER_LEVEL { NORMAL_HDR, PARTIAL_HDR, FULL_HDR };
enum HK_STATE { HK_NONE, HK_BEFORE, HK_AFTER, HK_BOTH };
enum IDLE_STATE { IDLE_YES, IDLE_NO };
enum SHUTTER_STATE { SHUTTER_OPEN, SHUTTER_CLOSED };
enum SIMULATION_MODE { SIM_NONE, SIM_VSM, SIM_FAST, SIM_FULL };
enum GAIN_SETTINGS { GAIN_1, GAIN_2, GAIN_5, GAIN_10 };
enum READOUT_RATES { FAST_READOUT, SLOW_READOUT };

int check_input (unsigned short type, char *in, void *min, void *max,void *out);
int cvt(unsigned short type, char *in, void *out);
long assignVal( unsigned short type, void *inVal, void *outVal, char *errMess);
long getDbInfo(char *fieldName, char *errMess, unsigned short type, 
	       void *outVal);
long getDbInfoT(char * Top, char *fieldName, char *errMess, unsigned short type, 
	       void *outVal);
long carStatus(char *input);
long processRec(char *prefix, char *record);
long putDbInfo(char *fieldName, char *errMess, unsigned short type, 
	       void *outVal);
long putDbInfoT(char *Top, char *fieldName, char *errMess, unsigned short type, 
	       void *outVal);
void printCadVals( long debugLvl, struct cadRecord *pCad, short numVals );
long setCad(struct cadRecord* pCad,char *line);
long updateStateHealth(char *, char *);

long setCar (char *name,long ival,long ierr,char *message,char *error);
void sleep( int a, int b);
#endif
