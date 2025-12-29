static struct {
	void *v;
	char *c;
} rcsid = {
	&rcsid,
	"$Id: initWcsCad.c,v 1.9 2011/08/18 20:49:27 gemvx Exp $"
};
/*
 * Copyright 1997 Association of Universities for Research in Astronomy, Inc.
 * See the file COPYRIGHT for more details.
 *
 * FILENAME
 * initWcsCad.c
 *
 * DESCRIPTION 
 * Process any inputs to "setWcs" CAD record.
 *
 * 
 * FUNCTION NAME(S)
 *	initWcsCad
 *
 *   
 * DEPENDENCIES
 * EPICS support library.
 *
 *
 *INDENT-OFF*
 * $Log: initWcsCad.c,v $
 * Revision 1.9  2011/08/18 20:49:27  gemvx
 * read the agPort by channel access if the db link isn't working
 *
 * Revision 1.8  2010/11/27 03:29:16  mrippa
 * Prevent disabling the wcs input file processing when
 * the wcsInfo array is undefined.
 *
 * Revision 1.7  2010/11/17 00:51:52  mrippa
 * Update checks for valid AG port and AO position
 * as required by WCS ICD.
 *
 * Revision 1.6  2010/09/01 23:51:14  mrippa
 * Formatting only
 *
 * Revision 1.5  2010/08/19 04:05:40  mrippa
 * Added aoName to selectWcs routine
 *
 * Revision 1.4  2010/08/16 20:33:52  mrippa
 * Print statement added to CAD directive state
 *
 * Revision 1.3  2010/08/02 05:16:31  mrippa
 * New WCS routines look at Camera and AG port
 * for WCS file definition.
 *
 * Revision 1.2  2009/05/27 19:32:21  fkraemer
 * fkraemer - copied my complete working dir over trunk
 *
 *INDENT-ON* 
 */



/* VxWorks include files */
#include <vxWorks.h>
#include <taskLib.h>
#include <semLib.h>

/* EPICS specific include files */
#include "epCommon.h"
#include "alarm.h"


#include "slalib.h"
#include "astLib.h"


/* Include file needed for control tasks */
#include <sysLib.h>
#include <car.h>

extern SEM_ID semInitWcs;
extern long TCS;


/* Forward declarations of the functions in this file */

/*
 *+
 * FUNCTION NAME:
 *	initWcsCad
 *
 * INVOCATION:
 * struct cadRecord *pCad;
 * status = initWcsCad( pCad );
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 *
 *
 * FUNCTION VALUE:
 * > pCad   (struct cadRecord *)   pointer to CAD data structure
 *
 * PURPOSE:
 * User defined function for "setWcs" CAD record
 *
 * DESCRIPTION:
 *	Gives a semaphore to control task which does the calculations for the wcs.
 *
 *
 * EXTERNAL VARIABLES:
 *	TCS true if TCS is available
 *
 * PRIOR REQUIREMENTS:
 *
 *
 * DEFICIENCIES:
 *
 *
 * HISTORY (optional):
 *
 *
 *
 *-
 */
int initWcsCad(struct cadRecord *pCad)
{
    char errMess[80];
    long status = CAD_ACCEPT;
#ifdef DEBUG
    int t1 = 0;
#endif
  
    /* Action to be taken depends on directive received		*/
    switch (DIRECTIVE)
    {
      case CAD_MARK:	       	/* No action required		*/
	DPRINT(t1, "initWcsCad - MARK directive");

	break;

      case CAD_PRESET:
	DPRINT(t1, "initWcsCad - PRESET directive");

	
	break;
      case CAD_CLEAR:		/* No action required		*/
	DPRINT(t1, "initWcsCad - CLEAR directive");

	break;

      case CAD_START:
	/* copy filenames to output */


	status = assignVal(pCad->ftva, pCad->a, pCad->vala, MSG);
	status += assignVal(pCad->ftvb, pCad->b, pCad->valb, MSG);
	status += assignVal(pCad->ftvc, pCad->c, pCad->valc, MSG);

	if(status == OK)
	{
	    DPRINT(t1, "initWcsCad - START directive");
	    if(setCar(WCS_CAR,CAR_BUSY,OK,"",errMess)!= OK)
	    {
		status = CAD_REJECT;
		DPRINT(1,errMess);
	    }
	    else
	    {
		semGive(semInitWcs);
	    }
	}


	break;

      case CAD_STOP:		   /* Really can't stop	       */
	DPRINT(t1, "initWcsCad - STOP directive");
	strncpy(MSG, "initWcsCad: Cannot stop",MAX_STRING_SIZE - 1);
	status = CAD_REJECT;
	break;

      default:		   /* Unknown directive		*/
	strncpy(MSG, "initWcsCad: Unrecongized directive",MAX_STRING_SIZE - 1);
	status = CAD_REJECT;
	break;
    } 
  
  
    return status;
 }



/*
 *+
 * FUNCTION NAME:
 *      updateAstCtx
 *
 * INVOCATION:
 *      called when "astCtx"  gensub is processed
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 *      none
 * FUNCTION VALUE:
 *      status
 *
 * PURPOSE:
 *      Get context from TCS.
 *
 * DESCRIPTION:
 *      Tries to get ast context from TCS.  If it fails, it just sets a 
/
...skipping
 */
long updateAstCtx(struct genSubRecord *pgsub)
{
   printf ("wfsUpdateAstCtx: pgsub->sevr = %d\n", pgsub->sevr); 

   if(pgsub->sevr != INVALID_ALARM)
   { 
      printf("TCS = 1 !!!!!, to astSetctx\n");
      TCS = 1;

      strcpy(pgsub->valb,pgsub->b);                  /* tcs:sad:sourceATrackFrame */
      strcpy(pgsub->valc,pgsub->c);                  /* tcs:sad:sourceATrackEq */
      *(double*)pgsub->vald = *(double*)pgsub->d;    /* tcs:sad:sourceAWavelength */

      astSetctx(pgsub->a); 
      printf("From astSetctx\n"); 
   } 
   else 
   { 
      printf("TCS = 0 ??????\n");
      TCS = 0 ;
      return ERROR;
   }
   return OK;
}

/*
 *+
 * FUNCTION NAME:
 *      selectWcs
 *
 * INVOCATION:
 *      called when "astCtx"  gensub is processed
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 *      none
 * FUNCTION VALUE:
 *      status
 *
 * PURPOSE:
 *      Get context from TCS.
 *
 * DESCRIPTION:
 *      Tries to get ast context from TCS.  If it fails, it just sets a 
/
...skipping
 */
int overridePort = 0;

long selectWcs(struct cadRecord *pCad)
{
   char camera[MAX_STRING_SIZE] = "unknown";
   char aoInstr[MAX_STRING_SIZE] = "unknown";
   char selectedPort[MAX_STRING_SIZE] = "unknown";
   char wcscamera[MAX_STRING_SIZE] = "unknown";
   char selectedAoname[MAX_STRING_SIZE] = "unknown";
   char errMess[80];
   long agport = 1; /*default agport*/
   long status = CAD_ACCEPT; 
   long aoIn = 0;
   long  mywcsPoints = 0;
    
   *(long*) pCad->vala = 1;  /* Disable wfs switch by default*/ 

   switch (DIRECTIVE)
    {
      case CAD_MARK:	       	/* No action required		*/
	DPRINT(t1, "initWcsCad - MARK directive");
	break;

      case CAD_PRESET:
	DPRINT(t1, "initWcsCad - PRESET directive");	
	break;

      case CAD_CLEAR:		/* No action required		*/
	DPRINT(t1, "initWcsCad - CLEAR directive");
        status = CAD_ACCEPT;
	break;

      case CAD_START:            /* No action required           */
   	  printf ("selectWcs: pCad->sevr = %d\n", pCad->sevr); 
	  /*printf("WCS file based on AGmanualPort=%s\n camera=%s\n AGport=%s\n setWcs.VALA=%s\n aoName=%s\n aomanualSelect=%s\n", pCad->a, pCad->b, pCad->c, pCad->d, pCad->e, pCad->f); */

	  if(pCad->sevr != INVALID_ALARM)
	    { 
	      agport = atoi(pCad->c);                                               /* Input C */
	    } else {
	      printf("attempt to read agport by CA\n");
	      if(getDbInfo("ag:port:nirs",errMess,DBR_LONG,&agport) != OK) {
		printf("error reading agport from ag via CA\n");
	      }
	    }
	  strncpy(selectedPort, (char *)pCad->a, MAX_STRING_SIZE-1);            /* Input A */
	  strncpy(camera, (char *)pCad->b, MAX_STRING_SIZE-1);                  /* Input B */
	  /* Input D */	
	  strncpy(aoInstr, (char *)pCad->e, MAX_STRING_SIZE-1);                 /* Input E */
	  strncpy(selectedAoname, (char *)pCad->f, MAX_STRING_SIZE-1);          /* Input F */
	  mywcsPoints = atoi(pCad->g);                                          /* Input G */
	  /* Input H RESERVED: It's accessed externally.*/
	  printf("selectedPort = %s\n",selectedPort); 
	  printf("camera = %s\n",camera); 
	  printf("agport = %d\n",agport); 
	  printf("wcspoints = %d\n",mywcsPoints); 
	  printf("aoName = %s\n",aoInstr); 
	  printf("matchFile = >>%s<<\n",pCad->d); 
	  printf("selectedAoname = %s\n",selectedAoname); 
	  
 
	  /*Camera selection*/
	  if ((strstr(camera, "Long") != NULL) || 
	      (strstr(camera, "Open") != NULL) )
	  {
	  	strcpy(wcscamera, "long");
          }
	  else if (strstr(camera, "Short") != NULL) {
	  	strcpy(wcscamera, "short");
	  }
	  else {
	    strncpy(pCad->mess, "selectWcsCad: Invalid camera",MAX_STRING_SIZE - 1);
		printf("ERR: selectWcsCad: Invalid camera\n");
	    status = CAD_REJECT;
		break;
	  }
	  
          /* AOname and PORT selection*/ 
          if (overridePort) {
             if (!strcmp(selectedAoname, "IN")) {
                aoIn = 1;
             }

             if (!strcmp(selectedPort, "")) 
             { 
              strcat(selectedPort,"1");
             }
             agport = atoi(selectedPort);

          } else {
             if (!strcmp(aoInstr, "IN")) {
                aoIn = 1;
             }
          }

          /* Validate AGport: test for NOT 1,3, or 5*/
          if ( !((agport == 1) || (agport == 3) || (agport == 5)) ) {
             strncpy(pCad->mess, "gnirsWcs:agport!=1,3,5",MAX_STRING_SIZE - 1);
             printf("selectWcsCad: Manual agport only 1,3,or 5\n");
             status = CAD_REJECT;
             break;
          }

	  /* set file name*/
          if(aoIn){ 
	    sprintf(pCad->valb, "data/aognirs_%ld_%s.wcs", agport, wcscamera);
	  } else {
	    sprintf(pCad->valb, "data/gnirs_%ld_%s.wcs", agport, wcscamera); 
	  }


          /*If wcsInfo array is defined && current filename matches new filename: DISABLE*/
	  if ((mywcsPoints > 0) && !strcmp(pCad->valb,pCad->d)) {
	     *(long *) pCad->vala=1;
	  }
	  else {
	     *(long *) pCad->vala=0;
	  }
	  break;

      default:		   /* Unknown directive		*/
	strncpy(pCad->mess, "selectWcsCad: Unrecongized directive",MAX_STRING_SIZE - 1);
	status = CAD_REJECT;
	break;

     }

    return status;
}

long copyTCSvalues(struct genSubRecord *pgsub)
{
printf ("in copyTCSvalues\n"); 
printf ("tcsValues.a = %s\n", (char *)pgsub->a); 
printf ("tcsValues.b = %s\n", (char *)pgsub->b); 
printf ("tcsValues.c = %s\n", (char *)pgsub->c); 
 
        pgsub->vala = pgsub->a; 
        pgsub->valb = pgsub->b; 
        pgsub->valc = pgsub->c; 

    return OK;
}
