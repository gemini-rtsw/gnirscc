static struct {
        void *v;
        char *c;
} rcsid = {
        &rcsid,
        "$Id: crFitsHeaders.c,v 1.2 2009/05/27 19:32:43 fkraemer Exp $"
};
/* #define DEBUG */
/*
	Copyright 1997 Association of Universities for Research in Astronomy, Inc.
	See the file; COPYRIGHT for more details.

	FILENAME
	crFitsHeaders.c

	DESCRIPTION
	This file creates the FITS headers for GNAAC data.
	
	FUNCTIONS
	getHeaderInfo - create the image header
	mallocHeaders - allocate the local areas for the data
	setHeader - reads database and copies value into header

	setHeaderROI - set ROI related header values
	getWCSInfo - set WCS related header values

	AUTHORS
	Ken Ramey
	Matthieu Bec

	DATE
	14-Nov-97

	HISTORY
	14-Nov-97		Created		KJR
	21-Nov-97		Split		KJR
	31-Jan-01		Reorganise	MBEC
	20-Feb-01		Yahoo		MBEC
*/ 

#include <stdio.h>
#include <vxWorks.h>
#include <time.h>
#include <string.h>
#include <taskLib.h>
#include <sysLib.h>
#include <stdlib.h>
#include "saveDefines.h"

#include <cadRecord.h>
#include <cadef.h> 
#include "gnDCADefs.h"
#undef	MAIN 
#define EPICS
#include "gnDCAVars.h" 
#include "gnDQSocket.h"
#include "saver.h"
#include "localWcs.h"

#define DUM_GBLSOURCE
#include "imageHdr.h"

/*externs*/
extern int curYear,curDay,curMonth;
extern saverParams svrP;

extern STATUS gnGetTime (char *val);
extern STATUS gnGetDate (char *val);


/* function prototypes*/
long setHeader(dhsHeadersDef *hdrdef,HeaderVals *hdrval);
STATUS getEpics (char* name, void *val, int count ,int id, unsigned short type);


/* globals*/
char tmp[80];

/*****************************************************************************
 * initHeader
 *****************************************************************************/
void initHeader(int captBuf)
{
	int i;
	
#ifdef TRACE
	printf ("initializing header for captBuf %d\n",captBuf);
#endif
	
	for (i=0;i<DHSDSHEADER;i++)
		dsHdr[captBuf][i].status=ERROR;
	for (i=0;i<DHSFRAMEHEADER;i++)
		frameHdr[captBuf][i].status=ERROR;
	for (i=0;i<DHSFRAMEROIHEADER;i++)
		frameRoiHdr[captBuf][i].status=ERROR;
}

/*****************************************************************************
 * getFrameHeader
 *****************************************************************************/
STATUS getFrameHeader(int timing ,int captBuf)
{     
    int i;

  if ( !(timing & NOW) && !(timing & hdrTiming))  {
#ifdef TRACE
  	  printf ("   not done -> getFrameHeader for captBuf %d [%s]\n",captBuf,(timing==BOTH)?"BOTH":(timing==BEFORE)?"BEFORE":(timing==AFTER)?"AFTER":(timing==NOW)?"NOW":"don'know timing");
#endif
  	  return OK;
  	  }
	
#ifdef TRACE
	printf ("getFrameHeader for captBuf %d [%s]\n",captBuf,(timing==BOTH)?"BOTH":(timing==BEFORE)?"BEFORE":(timing==AFTER)?"AFTER":(timing==NOW)?"NOW":"don'know timing");
#endif

/* stops on 1st failure */
	for (i=0;i<DHSFRAMEHEADER && dhsFrameHeader[i].keyword;i++)
	{
		if (timing == dhsFrameHeader[i].timing)
		{
			if (setHeader(&dhsFrameHeader[i],&frameHdr[captBuf][i]) != OK )
			{
				printf("error in getFrameHeader\n");
		printf("getFrameHeader timing = %d,%d  i = %d, name = %s\n",timing,dhsFrameHeader[i].timing,i,dhsFrameHeader[i].pvname);

				return ERROR;
			}
/* 		printf("getFrameHeader timing = %d,%d  i = %d, name = %s\n",timing,dhsFrameHeader[i].timing,i,dhsFrameHeader[i].pvname); */
		}
	}

    return OK;
}
/*****************************************************************************
 * 	getDsHeader
 *****************************************************************************/
STATUS getDsHeader( int timing ,int captBuf)
{
    int i;

	if ( !(timing & NOW) && !(timing & hdrTiming)) 
	{
#ifdef TRACE
		printf ("   not done -> getDsHeader for captBuf %d [%s]\n",captBuf,(timing==BOTH)?"BOTH":(timing==BEFORE)?"BEFORE":(timing==AFTER)?"AFTER":(timing==NOW)?"NOW":"don'know timing");
#endif
		return OK;
	}
	
	
	for (i=0;i<DHSDSHEADER && dhsDsHeader[i].keyword;i++)
	{
		if (timing == dhsDsHeader[i].timing)
			if (setHeader(&dhsDsHeader[i],&dsHdr[captBuf][i]) != OK )
			{
				printf("error in getDsHeader\n"); 
				printf("getdsheader timing = %d, i = %d, name = %s\n",timing,i,dhsDsHeader[i].pvname);
				
				return ERROR;    
			}
	}
	return OK;
}
 

void debugHeader() {
	int i;
	for (i=0;i<DHSDSHEADER;i++)
		printf ("%s\n",dhsDsHeader[i].pvname);
	for (i=0;i<DHSFRAMEHEADER;i++)
		printf ("%s\n",dhsFrameHeader[i].pvname);
	for (i=0;i<DHSFRAMEROIHEADER;i++)
		printf ("%s\n",dhsFrameRoiHeader[i].pvname);
	}


/*****************************************************************************
 * 	setHeader
 *****************************************************************************/
long setHeader(dhsHeadersDef *hdrdef,HeaderVals *hdrval) {
    double d;
    long l;
    char s[MAX_STRING_SIZE];
	
	 hdrval->status = OK;
	/*
	 * get EPICS value
	 */
/* 	printf("setHeader %s\n",hdrdef->pvname); */
	 if (hdrdef->pvname != NULL) 
	 {
		if(hdrdef->type == DCASTRING ) 
		{
			if (( hdrval->status = getEpics(hdrdef->pvname,s,1,hdrdef->id,DCASTRING)) == OK)
				strncpy(hdrval->value.sval, s,MAX_STRING_SIZE); 
		}
		else if( hdrdef->type== DCADOUBLE ) {
			if ((hdrval->status = getEpics(hdrdef->pvname,&d,1,hdrdef->id,DCADOUBLE)) == OK)
				hdrval->value.dval=d; 
		}
    	else if( hdrdef->type == DCALONG ) {
			if ((hdrval->status = getEpics(hdrdef->pvname,&l,1,hdrdef->id,DCALONG)) == OK)
				hdrval->value.lval = l; 
		}
		else 
		{
			printf("bad type in setHeader %s, %d\n",hdrdef->pvname,hdrdef->type);
			return ERROR;
		}
		
		if ( hdrval->status != OK) 
		{
			sprintf(tmp,"EPICS ERROR %s %d\n",hdrdef->pvname,strlen(hdrdef->pvname));
			cicsLogMessage(0,tmp);
			return ERROR;
		}
	 }
	
	/*
	 * user defined processing
	 */
	if (hdrdef->proc != NULL) 
	{
		hdrdef->proc(hdrval);
	}
	
	if ( hdrval->status != OK) {
		printf ("     setHeader %s [%d]\n",hdrdef->keyword,hdrval->status);
		}
		
	return hdrval->status;
	}

/*****************************************************************************
 * 	setHeaderROI
 *****************************************************************************/
STATUS setHeaderROI(int roiNum,tRect *proi,int captBuf)
{
    frameRoiHdr[captBuf][HDR_DROINUM].value.lval=roiNum;
	frameRoiHdr[captBuf][HDR_DROINUM].status = OK;
	frameRoiHdr[captBuf][HDR_LOWROW].value.lval=proi->lowY; 
    frameRoiHdr[captBuf][HDR_LOWROW].status = OK;
	frameRoiHdr[captBuf][HDR_LOWCOL].value.lval=proi->lowX;
    frameRoiHdr[captBuf][HDR_LOWCOL].status = OK;
	frameRoiHdr[captBuf][HDR_HIROW].value.lval=proi->hiY;
    frameRoiHdr[captBuf][HDR_HIROW].status = OK;
	frameRoiHdr[captBuf][HDR_HICOL].value.lval=proi->hiX;
    frameRoiHdr[captBuf][HDR_HICOL].status = OK;
	
	return OK;
}

/*****************************************************************************
 * 	setHeaderDEBUG
 *****************************************************************************/
STATUS setHeaderDEBUG(int captBuf,unsigned int xstart1,unsigned int xstart2 ,unsigned int vme)
{
    /*sprintf(dsHdr[captBuf][HDR_DEBUG].value.sval,"0x%x 0x%x 0x%x",xstart1,xstart2,vme);
	 dsHdr[captBuf][HDR_DEBUG].status = OK; */
	return OK;
}


/*****************************************************************************
 * Function name:
 * 	 getWCSInfo
 *
 * Invocation:
 * 	status =  getWCSInfo(  );
 *
 * PARAMETERS:
 *	none
 *
 * FUNCTION VALUE:
 * 	int - Status value returned by function 
 *
 * PURPOSE:
 * 	copy the wcs info into the header
 *
 * DESCRIPTION:
 * 	
 *
 * EXTERNAL VARIABLES:
 * 	Lots - see gnDCAVars.h
 *
 * PRIOR REQUIREMENTS:
 *	 
 *
 * DEFICIENCIES:
 * 	None known
 *
 * HISTORY:
 * 	
 *
 *****************************************************************************/

STATUS getWCSInfo(int roiNum,int captBuf)
{
    int status ;
    wcsHeader wcs;
	
#ifdef TRACE
	printf ("getWCSInfo for captBuf %d\n",captBuf);
#endif

	status = getWCS(&wcs,roiNum);
    
	sprintf(dsHdr[captBuf][HDR_CTYPE1].value.sval,"%s",wcs.ctype1);
	dsHdr[captBuf][HDR_CRPIX1].value.dval=wcs.crpix1;
	dsHdr[captBuf][HDR_CRVAL1].value.dval=wcs.crval1	;
	sprintf(dsHdr[captBuf][HDR_CTYPE2].value.sval,"%s",wcs.ctype2);
	dsHdr[captBuf][HDR_CRPIX2].value.dval=wcs.crpix2	;
	dsHdr[captBuf][HDR_CRVAL2].value.dval=wcs.crval2	;
	dsHdr[captBuf][HDR_CD1_1].value.dval=wcs.cd1_1	;
	dsHdr[captBuf][HDR_CD1_2].value.dval=wcs.cd1_2	;
	dsHdr[captBuf][HDR_CD2_1].value.dval=wcs.cd2_1	;
	dsHdr[captBuf][HDR_CD2_2].value.dval=wcs.cd2_2	;
	dsHdr[captBuf][HDR_MJD_OBS].value.dval=wcs.mjdobs	;
	
dsHdr[captBuf][HDR_CTYPE1].status= OK;
dsHdr[captBuf][HDR_CRPIX1].status= OK;
dsHdr[captBuf][HDR_CRVAL1].status= OK;
dsHdr[captBuf][HDR_CTYPE2].status= OK;
dsHdr[captBuf][HDR_CRPIX1].status= OK;
dsHdr[captBuf][HDR_CRPIX2].status= OK;
dsHdr[captBuf][HDR_CRVAL2].status= OK;
dsHdr[captBuf][HDR_CD1_1].status= OK;
dsHdr[captBuf][HDR_CD1_2].status= OK;
dsHdr[captBuf][HDR_CD2_1].status= OK;
dsHdr[captBuf][HDR_CD2_2].status= OK;
dsHdr[captBuf][HDR_MJD_OBS].status= OK;
	

/* 	sprintf(frameHdr[captBuf][FRHDR_CTYPE1].value.sval,"%s",wcs.ctype1); */
/* 	frameHdr[captBuf][FRHDR_CRPIX1].value.dval=wcs.crpix1; */
/* 	frameHdr[captBuf][FRHDR_CRVAL1].value.dval=wcs.crval1	; */
/* 	sprintf(frameHdr[captBuf][FRHDR_CTYPE2].value.sval,"%s",wcs.ctype2); */
/* 	frameHdr[captBuf][FRHDR_CRPIX2].value.dval=wcs.crpix2	; */
/* 	frameHdr[captBuf][FRHDR_CRVAL2].value.dval=wcs.crval2	; */
/* 	frameHdr[captBuf][FRHDR_CD1_1].value.dval=wcs.cd1_1	; */
/* 	frameHdr[captBuf][FRHDR_CD1_2].value.dval=wcs.cd1_2	; */
/* 	frameHdr[captBuf][FRHDR_CD2_1].value.dval=wcs.cd2_1	; */
/* 	frameHdr[captBuf][FRHDR_CD2_2].value.dval=wcs.cd2_2	; */
/* 	frameHdr[captBuf][FRHDR_MJD_OBS].value.dval=wcs.mjdobs	; */
	
/* frameHdr[captBuf][FRHDR_CTYPE1].status= OK; */
/* frameHdr[captBuf][FRHDR_CRPIX1].status= OK; */
/* frameHdr[captBuf][FRHDR_CRVAL1].status= OK; */
/* frameHdr[captBuf][FRHDR_CTYPE2].status= OK; */
/* frameHdr[captBuf][FRHDR_CRPIX1].status= OK; */
/* frameHdr[captBuf][FRHDR_CRPIX2].status= OK; */
/* frameHdr[captBuf][FRHDR_CRVAL2].status= OK; */
/* frameHdr[captBuf][FRHDR_CD1_1].status= OK; */
/* frameHdr[captBuf][FRHDR_CD1_2].status= OK; */
/* frameHdr[captBuf][FRHDR_CD2_1].status= OK; */
/* frameHdr[captBuf][FRHDR_CD2_2].status= OK; */
/* frameHdr[captBuf][FRHDR_MJD_OBS].status= OK; */






	return status;

}






#include <bc350Time.h>

/*****************************************************************************
 * user defined processing
 *****************************************************************************/

void sethdrINTEGRITY(HeaderVals *hdrval) {
	if(DCA_Abort)
		sprintf(hdrval->value.sval, "%s", "ABORTED" );
	else
		sprintf(hdrval->value.sval, "%s","OK" );
	hdrval->status = OK;
	}
	
	
void sethdrPrecision3Digits(HeaderVals *hdrval) {
	if ( hdrval->status == OK) {
		hdrval->value.dval = ((long) ((double) (hdrval->value.dval * 1000.0))) / 1000.0;
		}
	}
	
void sethdrPrecision4Digits(HeaderVals *hdrval) {
	if ( hdrval->status == OK) {
		hdrval->value.dval = ((long) ((double) (hdrval->value.dval * 10000.0))) / 10000.0;
		}
	}

