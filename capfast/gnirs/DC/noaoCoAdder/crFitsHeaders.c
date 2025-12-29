static struct {
        void *v;
        char *c;
} rcsid = {
        &rcsid,
        "$Id: crFitsHeaders.c,v 1.2 2009/05/27 19:32:30 fkraemer Exp $"
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
	rdBanCom635Time - Read the current time and date

	AUTHOR
	Ken Ramey

	DATE
	14-Nov-97

	HISTORY
	14-Nov-97		Created		KJR
	21-Nov-97		Split		KJR
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

/*externs*/
extern int curYear,curDay,curMonth;
extern saverParams svrP;

/* function prototypes*/
long setHeader(int i,char *h[3],int timing,int type,int bufNum);


/* globals*/
char tmp[80];
static int cur ;

HeaderVals headerVals[MAXCAPTBUFS];

/* static globals*/

/* the first parameter is the header name, 
the second parameter is the EPICS name or NULL if not from epics
the third parameter is the EPICS type or NULL if not from epics   */
static char *hdrNamesDs[NUM_HDR][3] =
{
    {"INTEGRITY",NULL,NULL},
    {"ARRAYTYP",DET_TYPE,"DCASTRING"},
    {"ARRAYID",DET_ID,"DCASTRING"},
    {"HDRTIMING",NULL,NULL},
    {"COADDS",NULL,NULL},
    {"LNRS",NULL,NULL},
    {"MODE",NULL,NULL},
    {"EXPTIME",NULL,NULL},
    {"UCODETYP",NULL,NULL},
    {"NDAVGS",OBSSETUP_CAD ".VALC","DCALONG"},
    {"UCODENAM", ARSETUP_CAD ".VALB", "DCASTRING"},
    {"DATE",NULL,NULL },
    {"DATE-OBS",NULL,NULL},
    {"TIME_OBS",NULL,NULL},
    {"ENDUT",NULL,NULL},
    {"FRMSPCYCL",NULL,NULL},
    {NULL,NULL,NULL}
};
static char *hdrNamesFrame[NUM_HDR][3] =
{
    {"CTYPE1",WCS_CTYPE1,"DCASTRING"},
    {"CRPIX1",WCS_CRPIX1,"DCADOUBLE"},
    {"CRVAL1",WCS_CRVAL1,"DCADOUBLE"},
    {"CTYPE2",WCS_CTYPE2,"DCASTRING"},
    {"CRPIX2",WCS_CRPIX2,"DCADOUBLE"},
    {"CRVAL2",WCS_CRVAL2,"DCADOUBLE"},
    {"CD1_1",WCS_CD1_1,"DCADOUBLE"},
    {"CD1_2",WCS_CD1_2,"DCADOUBLE"},
    {"CD2_1",WCS_CD2_1,"DCADOUBLE"},
    {"CD2_2",WCS_CD2_2,"DCADOUBLE"},
    {"RADECSYS",WCS_RADECSYS,"DCASTRING"},
    {"EQUINOX",WCS_EQUINOX,"DCADOUBLE"},
    {"MJD_OBS",WCS_MJDOBS,"DCADOUBLE"},
    {"DROINUM",NULL,NULL},
    {"LOWROW",NULL,NULL},
    {"LOWCOL", NULL,NULL},
    {"HIROW", NULL,NULL},
    {"HICOL", NULL,NULL},
    {NULL,NULL,NULL}
};
/* this set are the ones that will change for before and after*/
static char *hdrNamesDsVAR[50][3] = 
{
    {"TDETABS",TEMP_DETABS,"DCADOUBLE"},
    {"TMOUNT",TEMP_MNTABS,"DCADOUBLE"},
    {"VSET",HK_VSET,"DCADOUBLE"},
    {"VDDCL1",HK_VDDCL1,"DCADOUBLE"},
    {"VDDCL2",HK_VDDCL2,"DCADOUBLE"},
    {"VGGCL1",HK_VGGCL1,"DCADOUBLE"},
    {"VGGCL2",HK_VGGCL2,"DCADOUBLE"},
    {"VDDUC", HK_VDDUC,"DCADOUBLE"},
    {"VDET",HK_VDET,"DCADOUBLE"},
    {NULL,NULL,NULL}
};

/*****************************************************************************
 * Function name:
 * 	getFrameHeader
 *
 * Invocation:
 * 	status = getFrameHeader( int roiNum,tRect *proi);
 *
 * PARAMETERS:
 *	int roiNum - roi number
 *	tRect *proi - structure of roi values
 *
 * FUNCTION VALUE:
 * 	int - Status value returned by function 
 *
 * PURPOSE:
 * 	Creates DHS frame header values by obtaining HK values and other 
 *	    information from EPICS and system variables.  
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
STATUS getFrameHeader(int roiNum,tRect *proi,int captBuf)
{     
#ifdef DEBUG
    int t1 = 0;
#endif
    int i;
    int status = OK;
    char buf[80];



	/* get headers from epics*/
	i = 0;
	while((hdrNamesFrame[i][0] != NULL)&&(status == OK))
	{ 
	    sprintf(buf,"%d ",i);
	    DPRINT(t1,buf);
	    if(hdrNamesFrame[i][1] != NULL)
		status = setHeader(i,hdrNamesFrame[i],BEFORE,HDR_FRAME,captBuf);
	    i++;
	}

	/* mark end of headers*/
	strncpy(headerVals[captBuf].frame[i][0],END_HEAD,
		MAX_STRING_SIZE);
	strncpy(headerVals[captBuf].frame[i][1] ,END_HEAD,
		MAX_STRING_SIZE);

    return status;
}
/*****************************************************************************
 * Function name:
 * 	getDsHeader
 *
 * Invocation:
 * 	status = getDsHeader( int timing );
 *
 * PARAMETERS:
 *	int timing - before or after taking data
 *
 * FUNCTION VALUE:
 * 	int - Status value returned by function 
 *
 * PURPOSE:
 * 	Creates dataset DHS header values by obtaining HK values and other 
 *	    information from EPICS and system variables. 
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
STATUS getDsHeader( int timing ,int captBuf)
{
    char buf[80];
#ifdef DEBUG
    int t1 = 0;
#endif
    double intTimeVal;
    double minInt,intTime;
    int i;
    long  status = OK;

	if ((timing == BEFORE)&&(status == OK))
	{
        i = 0;
        
        strncpy(headerVals[captBuf].ds[HDR_HDRTIMING][0], 
			    hdrNamesDs[HDR_HDRTIMING][0], MAX_STRING_SIZE);
        if(hdrTiming == BEFORE) 
          strncpy(headerVals[captBuf].ds[HDR_HDRTIMING][1], 
                  "BEFORE", MAX_STRING_SIZE);
        if(hdrTiming == AFTER) 
          strncpy(headerVals[captBuf].ds[HDR_HDRTIMING][1], 
                  "AFTER", MAX_STRING_SIZE);
        if(hdrTiming == BOTH) 
          strncpy(headerVals[captBuf].ds[HDR_HDRTIMING][1], 
                  "BOTH", MAX_STRING_SIZE);
          
	    /* get headers from epics*/
	    DPRINT(t1,"getheaderinfo while loop\n");
	    while((hdrNamesDs[i][0] != NULL)&&(status == OK))
	    {
		sprintf(buf,"%d ",i);
	  
		if(hdrNamesDs[i][1] != NULL)  
		    status = setHeader(i,hdrNamesDs[i],BEFORE,HDR_DS,captBuf);
		i++;
	    }	

	    /* INTTIME  this needs to be computed*/ 
	    DPRINT(t1,"\ngetheaderinfo int time\n");
	    if ((status = getEpics(dbTop, MININT ".VAL",DCADOUBLE, 
				      &minInt)) == OK)
		if ((status = getEpics(dbTop, OBSSETUP_CAD ".VALG",
					  DCADOUBLE, &intTime)) == OK)
		{
		    sprintf(tmp,"header minint = %f, imtTime = %f",
			    minInt,intTime);
		    /* 	cicsLogMessage(3,tmp); */
		    intTimeVal = minInt + intTime;
		    strncpy(headerVals[captBuf].ds[HDR_EXPTIME][0], 
			    hdrNamesDs[HDR_EXPTIME][0], MAX_STRING_SIZE);
		    sprintf(headerVals[captBuf].ds[HDR_EXPTIME][1], "%f", 
			    intTimeVal);

		}


	    DPRINT(t1,"getheaderinfo int time done\n");

	    /* COADDS, LNRS, MODE, UCODETYPE, FRAMES/CYCLE*/
	    strncpy(headerVals[captBuf].ds[HDR_COADDS][0], 
		    hdrNamesDs[HDR_COADDS][0], MAX_STRING_SIZE);
	    sprintf(headerVals[captBuf].ds[HDR_COADDS][1], "%ld", numCoAdds);
		strncpy(headerVals[captBuf].ds[HDR_LNRS][0], 
			hdrNamesDs[HDR_LNRS][0], MAX_STRING_SIZE);
	    sprintf(headerVals[captBuf].ds[HDR_LNRS][1], "%ld", numLNRs);
		strncpy(headerVals[captBuf].ds[HDR_MODE][0], 
			hdrNamesDs[HDR_MODE][0], MAX_STRING_SIZE);
	    sprintf(headerVals[captBuf].ds[HDR_MODE][1], "%s", 
		    procModeNames[procMode]);
		strncpy(headerVals[captBuf].ds[HDR_UCODETYP][0], 
			hdrNamesDs[HDR_UCODETYP][0], MAX_STRING_SIZE);
	    sprintf(headerVals[captBuf].ds[HDR_UCODETYP][1], "%s", 
		    uCodeTypeNames[uCodeType]);
		strncpy(headerVals[captBuf].ds[HDR_FRMSPCYCL][0], 
			hdrNamesDs[HDR_FRMSPCYCL][0], MAX_STRING_SIZE);
	    sprintf(headerVals[captBuf].ds[HDR_FRMSPCYCL][1], "%ld", 
		    framesPerCycle);  
	    cur = NUM_HDR_DS;
	    strncpy(headerVals[captBuf].ds[cur][0],END_HEAD,MAX_STRING_SIZE);
	    strncpy(headerVals[captBuf].ds[cur][1] ,END_HEAD,MAX_STRING_SIZE);

	}
  
	DPRINT(t1,"getheaderinfo before\n");
	if((status == OK)&&
	  (((hdrTiming == BOTH)||(hdrTiming == BEFORE))&&(timing == BEFORE)))
	{
	    i = 0;
	    /* get headers from epics*/
	    while((hdrNamesDsVAR[i][0] != NULL)&&(status == OK))
	    {
            if(hdrNamesDsVAR[i][1] != NULL)
              {
                status = setHeader(cur,hdrNamesDsVAR[i],BEFORE,HDR_DS,
                                   captBuf);
                cur++;
              }
            i++;
	    }
	    strncpy(headerVals[captBuf].ds[cur][0],END_HEAD,MAX_STRING_SIZE);
	    strncpy(headerVals[captBuf].ds[cur][1] ,END_HEAD,MAX_STRING_SIZE);

	}
	if ((status == OK)&&
        (((hdrTiming == BOTH)||(hdrTiming == AFTER))&&(timing == AFTER)))
      {
          i = 0;	
       
          /* get headers from epics*/
          while((hdrNamesDsVAR[i][0] != NULL)&&(status == OK))
          {
              strncpy(headerVals[captBuf].ds[cur][0],hdrNamesDsVAR[i][0],
                      MAX_STRING_SIZE);
             
              if(hdrNamesDsVAR[i][1] != NULL)
              {
                  status = setHeader(cur,hdrNamesDsVAR[i],AFTER,HDR_DS,
                                   captBuf);
                  cur++;
              }
              i++;
          }
          strncpy(headerVals[captBuf].ds[cur][0],END_HEAD,MAX_STRING_SIZE);
          strncpy(headerVals[captBuf].ds[cur][1] ,END_HEAD,MAX_STRING_SIZE);

      }
	else if (status != OK)
	{
	    printf("Error in getDsHeader\n");
	}
	    

	DPRINT(t1,"getheaderinfo after\n");
	if((timing == AFTER)&&(status == OK))
	{
	    strncpy(headerVals[captBuf].ds[HDR_INTEGRITY][0], 
		    hdrNamesDs[HDR_INTEGRITY][0], MAX_STRING_SIZE);
	    if(DCA_Abort)
	    	sprintf(headerVals[captBuf].ds[HDR_INTEGRITY][1], "%s",
			"ABORTED" );
	    else
		sprintf(headerVals[captBuf].ds[HDR_INTEGRITY][1], "%s","OK" );
	}
	DPRINT(t1,"getHeaderInfo done \n");


    return status;

}





/*****************************************************************************
 * Function name:
 * 	setHeader
 *
 * Invocation:
 * 	status = setHeader( int i,char *h[],int timing );
 *
 * PARAMETERS:
 *	int i - index into headerVals array
 *      char *h[] - header to store in
 *      int timing - before or after taking data
 *
 * FUNCTION VALUE:
 * 	int - Status value returned by function 
 *
 * PURPOSE:
 * 	copy header info from epics to headerVals array (the header that is 
 *                 sent with the image)
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







long setHeader(int i,char *h[],int timing,int type,int captBuf)
{
    double d;
    long status;
    long l;
#ifdef DEBUG
    int t1=0;
#endif
    char s[80];
    char *temp[2];
    
    /* frame or dataset*/
    if(type)
      {
        temp[0] = headerVals[captBuf].frame[i][0];
        temp[1] = headerVals[captBuf].frame[i][1];
      }
    else
      {
        temp[0] = headerVals[captBuf].ds[i][0];
        temp[1] = headerVals[captBuf].ds[i][1];
      }


    /* header value type*/
    if(strcmp(h[2], "DCASTRING") == 0)
    {

      DPRINT(t1,"string\n");
      if ((status = getEpics(dbTop, h[1],DCASTRING, s)) == OK)
        {
          strncpy(temp[1], s,MAX_STRING_SIZE); 

          if((timing == BEFORE))
            {

              strncpy(temp[0], h[0],MAX_STRING_SIZE); 
            }
          else if(timing == AFTER)
	  {
	  
	      sprintf(temp[0],"A_%s", h[0]); 
	  }
        }
      else
        {
          /* handle EPICS access error here */
          sprintf(tmp,"EPICS ERROR %s\n",h[1]);
          cicsLogMessage(0,tmp);
          return status;
        }
    }
    else if( strcmp(h[2] ,"DCADOUBLE") == 0)
      {
        DPRINT(t1,"long\n");
        if ((status = getEpics(dbTop, h[1],DCADOUBLE, &d)) == OK)
          {
            sprintf(temp[1], "%f",d); 


            if((timing == BEFORE))
              {
                strncpy(temp[0], h[0],MAX_STRING_SIZE); 
              }
            else if(timing == AFTER)
              sprintf(temp[0],"A_%s", h[0]); 
          }
        else
          {
            /* handle EPICS access error here */
            sprintf(tmp,"EPICS ERROR %s\n",h[1]);
            printf(tmp);
            cicsLogMessage(0,tmp);
	
          }
      }
    else if(strcmp(h[2],"DCALONG") == 0)
      {
        DPRINT(t1,"long\n");
        if ((status = getEpics(dbTop, h[1],DCALONG, &l)) == OK)
          {
            sprintf(temp[1],"%d", l); 


            if((timing == BEFORE))
              {
                strncpy(temp[0], h[0],MAX_STRING_SIZE); 
              }
            else if(timing == AFTER)
              sprintf(temp[0],"A_%s", h[0]); 
          }
        else
          {
            /* handle EPICS access error here */
            sprintf(tmp,"EPICS ERROR %s\n",h[1]);
            cicsLogMessage(0,tmp);
            return status;
          }
      }
    else
      {
        printf("bad type in setHeader\n");
        return ERROR;
      }
    return OK;
}

/*****************************************************************************
 * Function name:
 * 	setHeaderROI
 *
 * Invocation:
 * 	status =setHeaderROI (int roiNum  );
 *
 * PARAMETERS:
 *	roiNum - the roi number to use
 *
 * FUNCTION VALUE:
 * 	int - Status value returned by function 
 *
 * PURPOSE:
 * 	copy roi info into header
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
STATUS setHeaderROI(int roiNum,tRect *proi,int captBuf)
{
    int rowCount, colCount;
   	
    strncpy(headerVals[captBuf].frame[HDR_DROINUM][0], 
	    hdrNamesFrame[HDR_DROINUM][0], MAX_STRING_SIZE);
    sprintf(headerVals[captBuf].frame[HDR_DROINUM][1], "%ld", roiNum); 	
 
    rowCount = proi->rows;
    colCount = proi->cols;	
    strncpy(headerVals[captBuf].frame[HDR_LOWROW][0], 
	    hdrNamesFrame[HDR_LOWROW][0], MAX_STRING_SIZE);
    sprintf(headerVals[captBuf].frame[HDR_LOWROW][1], "%ld", proi->lowY); 
    strncpy(headerVals[captBuf].frame[HDR_LOWCOL][0], 
	    hdrNamesFrame[HDR_LOWCOL][0], MAX_STRING_SIZE);
    sprintf(headerVals[captBuf].frame[HDR_LOWCOL][1], "%ld", proi->lowX);
    strncpy(headerVals[captBuf].frame[HDR_HIROW][0], 
	    hdrNamesFrame[HDR_HIROW][0], MAX_STRING_SIZE);
    sprintf(headerVals[captBuf].frame[HDR_HIROW][1], "%ld", proi->hiY);
    strncpy(headerVals[captBuf].frame[HDR_HICOL][0], 
	    hdrNamesFrame[HDR_HICOL][0], MAX_STRING_SIZE); 
    sprintf(headerVals[captBuf].frame[HDR_HICOL][1], "%ld", proi->hiX);	  
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
#ifdef DEBUG
    int t1 = 1;
#endif
    int status ;
    wcsHeader wcs;
   

    status = getWCS(&wcs,roiNum);
    DPRINT(t1,"after getwcs\n");
  /*   sprintf(headerVals[captBuf].frame[HDR_CTYPE1][1], "%s", wcs.ctype1); */
/*     sprintf(headerVals[captBuf].frame[HDR_CRPIX1][1], "%f", wcs.crpix1); */
/*     sprintf(headerVals[captBuf].frame[HDR_CRVAL1][1], "%f", wcs.crval1); */
/*     sprintf(headerVals[captBuf].frame[HDR_CTYPE2][1], "%s", wcs.ctype2); */
/*     sprintf(headerVals[captBuf].frame[HDR_CRPIX2][1], "%f", wcs.crpix2); */
/*     sprintf(headerVals[captBuf].frame[HDR_CRVAL2][1], "%f", wcs.crval2); */
     sprintf(headerVals[captBuf].frame[HDR_CD1_1][1], "%f", wcs.cd1_1); 
/*     sprintf(headerVals[captBuf].frame[HDR_CD1_2][1], "%f", wcs.cd1_2); */
/*     sprintf(headerVals[captBuf].frame[HDR_CD2_1][1], "%f", wcs.cd2_1); */
/*     sprintf(headerVals[captBuf].frame[HDR_CD2_2][1], "%f", wcs.cd2_2); */
/*     sprintf(headerVals[captBuf].frame[HDR_RADECSYS][1], "%s", wcs.radecsys); */
/*     sprintf(headerVals[captBuf].frame[HDR_EQUINOX][1], "%f", wcs.equinox); */
/*     sprintf(headerVals[captBuf].frame[HDR_MJDOBS][1], "%f", wcs.mjdobs); */
  
    return status;

}

#include <bc350Time.h>
/*****************************************************************************
 * Function name:
 * 	rdBanCom635Time
 *
 * Invocation:
 * 	status = rdBanCom635Time( int timimg );
 *
 * PARAMETERS:
 *	timing - before or after taking data
 *
 * FUNCTION VALUE:
 * 	int - Status value returned by function 
 *
 * PURPOSE:
 * 	 Read the time from the time lan board and copy into the header
 *
 * DESCRIPTION:
 * 	
 *
 * EXTERNAL VARIABLES:
 * 	Lots - see gnDCAVars.h
 *
 * PRIOR REQUIREMENTS:
 *	 ban com board must be initialized with the correct time
 *
 * DEFICIENCIES:
 * 	None known
 *
 * HISTORY:
 * 	
 *
 *****************************************************************************/
int rdBanCom635Time(int timing,int captBuf)
{
    static struct time_struct uT;  
#if 1
  
    if (timing == BEFORE)
      {
   
        if(getDsHeader(BEFORE,captBuf) == ERROR)  {
          printf("error getting header info\n"); 
        }
        getTime(&uT); 
        strncpy(headerVals[captBuf].ds[HDR_TIME_OBS][0], 
                hdrNamesDs[HDR_TIME_OBS][0], MAX_STRING_SIZE);
       	sprintf(headerVals[captBuf].ds[HDR_TIME_OBS][1], "%02d:%02d:%02d.%04d",
                uT.hour,uT.min,uT.sec,uT.msec/1000); 
        sprintf(tmp," time before = %d min %d.%04d sec buffer = %d\n",
                uT.min,uT.sec, uT.msec/1000,captBuf);
        printf(tmp);
        strncpy(headerVals[captBuf].ds[HDR_DATE][0], 
                hdrNamesDs[HDR_DATE][0], MAX_STRING_SIZE);
       	sprintf(headerVals[captBuf].ds[HDR_DATE][1],"%2d/%02d/%2d",	
            curYear ,curMonth,curDay ); 
        strncpy(headerVals[captBuf].ds[HDR_DATEOBS][0], 
                hdrNamesDs[HDR_DATEOBS][0], MAX_STRING_SIZE);
        sprintf(headerVals[captBuf].ds[HDR_DATEOBS][1], "%2d/%02d/%2d",
                curDay,curMonth,curYear); 
      }
    else if(timing==AFTER)
      {

     
       
        if(getDsHeader(AFTER,captBuf) == ERROR)  {
          printf("error getting header info\n"); 
        }
      
        getTime(&uT); 
	strncpy(headerVals[captBuf].ds[HDR_ENDUT][0], 
		hdrNamesDs[HDR_ENDUT][0], MAX_STRING_SIZE);
      	sprintf(headerVals[captBuf].ds[HDR_ENDUT][1],"%02d:%02d:%02d.%04d",
                uT.hour,uT.min,uT.sec, uT.msec/1000); 
        sprintf(tmp," time after = %d min %d.%04d sec buffer = %d\n",
                uT.min,uT.sec, uT.msec/1000,captBuf);
        printf(tmp);
      }
    else
      {
        getTime(&uT); 
      
        sprintf(tmp," time  = %d min %d.%04d sec buffer = %d\n",
                uT.min,uT.sec, uT.msec/1000,captBuf);
        printf(tmp);
      }
#endif
    return OK;
}
    
void printHdr(char hdrarray[255][2][MAX_STRING_SIZE] )
{
  int n=0;
  printf("\n\nprint header \n");
  while (strcmp(hdrarray[n][0],END_HEAD) != 0)
     {
       printf("name = %s, val = %s\n",hdrarray[n][0],hdrarray[n][1]);
       n++;

     }
}
