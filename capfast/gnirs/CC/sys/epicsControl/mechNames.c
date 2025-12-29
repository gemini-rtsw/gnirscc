
#include  <vxWorks.h>
#include  <stdlib.h>
#include  <types.h>
#include  <stdioLib.h>
#include  <lstLib.h>
#include  <strLib.h>
#include  <stdio.h>
#include  <string.h>
#include  <math.h>
#include  <genSubRecord.h>
#include  <cad.h>
#include "gnirsCcDefs.h"
#include "gnirsCC.h"
#include "mechNames.h"
void printMech(void *tbl);

/*
 * Pointer to the local lookup table of loaded filter barcodes,  and position.
 */

static void *fw1Tbl = NULL;
static void *fw2Tbl = NULL;
static void *focusTbl = NULL;
static void *cameraTbl = NULL;
static void *coverTbl = NULL;
static void *xdispTbl = NULL;
static void *gratingTbl = NULL;
static void *deckerTbl = NULL;
static void *slitTbl = NULL;
static void *acqTbl = NULL;

static int wheelFileReadOK    = FALSE;
/******************************************************************************/

/*+
 *   Function name:
 *   mechLutRead
 *
 *   Purpose:
 *   Initialise  mechanism  database
 *
 *   Description:
 *   Read the filter data file containing a list of all loaded filter
 *   barcodes along with the corresponding wheel number and position name. 
 *   and set up an internal list of barcodes with wheel number.
 *
 *   Invocation:
 *   mechLutRead( char * lutfilename )
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *   (>)  lutfilename  (string)  Name of loaded filters lut file (flt.lut)
 *
 *   Function value:
 *   (<)  status  (long) Return status, 0 = OK
 *                       Error if file not opened or missing data values
 * 
 *   External functions:
 *   (VxWorks lstLib routines) lstInit, lstAdd
 *
 *   Derived from the routine gmFilterLUTread but
 *   to be used to read the barcode list file for
 *   setting up the filter menus 
 * 
 *-
 */


long mechLutRead( char *lutfilename, void **tbl )
{
    FILE *fp;                /* LUT data file */
    char *buf; /* Input buffer */
    char tag[256];    /* Tag (= barcode) as a string */
    long barcode;            /* barcodeID value */
	char name[40];
	char pos[40];
	double tilt;
    MECHLUT *p;         /* Look-up table data structure */
    long n;
    long nfilt = 0;
    long lcount;             /* Input line count */
    long status;             /* Return status */

    status = CAD_ACCEPT;
    wheelFileReadOK = FALSE;
	/* Free up old list if this is not the first entry */
    if (*tbl != NULL) 
    {
		lstFree((LIST *) *tbl);
		free(*tbl);
		*tbl = NULL;
    }    
	
	/* read wheel contents data file from remote disk */
	
    printf("mechLutRead : open file check: %s\n", lutfilename);

    if ((fp = fopen (lutfilename, "r")) == NULL)
    {
        printf("mechLutRead error: failed to open file %s\n", lutfilename);
        return CAD_REJECT;
    }
	
	/*     DBGMSGSTRING(DBG_MIN,"mechLutRead: opened filter lut file:", lutfilename); */
    
    *tbl = (void *) malloc (sizeof (LIST));
    if (*tbl == NULL)    /* malloc failed */
    {
        printf("mechLutRead: LUT malloc failed\n");
        fclose(fp);
        return CAD_REJECT;
    }
    lstInit ((LIST *) *tbl);
    
    lcount = 0;
    while (status == 0)
	{
		/*      skip blank lines and comments */
		
		buf = fgets (tag, 255, fp);   
		lcount++;
		if (buf == NULL)
			break;
		/* create new node, read barcode and other values */
		else if(tag[0] != '#')
		{
			n = sscanf(tag, "%ld%20s%9s%lf", &barcode,name,pos,&tilt);
			if (n > 2)
			{
				p = (MECHLUT *) malloc (sizeof (MECHLUT));
				if (p == NULL)    /* malloc failed */
				{
					printf("mechLutRead: LUT node malloc failed\n");
					fclose(fp);
					return CAD_REJECT;
				}
				lstAdd ((LIST *) *tbl, (NODE *) p);
				p->barcode = barcode;
				strncpy(p->name,name,39);
				strncpy(p->pos,pos,POS_LEN-1);
				p->pos[POS_LEN-1] = NULL;
				p->tilt = tilt;
				printf("barcode %d,  name = %s, pos = %s, tilt = %f\n",barcode,name,pos,tilt);
				/*DBGMSGINT(DBG_FULL,"mechLutRead data line read OK:",lcount); */
				nfilt++;
                    
			}
		}
	}
    fclose (fp);
    printf("number of nodes = %d\n",lstCount(*tbl));

/* If no valid filter lines found, then error */
    if (nfilt == 0)
    {  
        printf("ERROR: no valid filter definitions found in file %s\n", lutfilename);
        status = CAD_REJECT;
    }    
    
    if (status == CAD_ACCEPT) wheelFileReadOK = TRUE;
    
    return status;
}

int findMechName(int mech, char *name, char *pos)
{
  MECHLUT *pw;  /* Pointer to barcode/wheel lookup table item */
  int found = 0;
  LIST *tbl;

  switch (mech)
  {
    case FW1 :
      tbl = fw1Tbl;
      found = 0;
      for (pw = (MECHLUT *) lstFirst ((LIST *)tbl); pw != NULL;
	  pw = (MECHLUT *) lstNext ((NODE *) pw))
      {
	/* printf("name = %s, name = %s\n",name,pw->name); */
	if(strcmp(pw->name,name) == 0)
	{
	  found = 1;
	  strcpy (pos,pw->pos);
	  break;
	}
      }
      /* printMech(fw1Tbl); */
      break;
    case FW2 :
      tbl = fw2Tbl;
      found = 0;
      for (pw = (MECHLUT *) lstFirst ((LIST *)tbl); pw != NULL;
	  pw = (MECHLUT *) lstNext ((NODE *) pw))
      {
	/* printf("name = %s, name = %s\n",name,pw->name); */
	if(strcmp(pw->name,name) == 0)
	{
	  found = 1;
	  strcpy (pos,pw->pos);
	  break;
	}
      }
      /* printMech(fw2Tbl); */
      break;
    case COVER :	
      if (name[0])
      {
	found = 1;
	if(strcmp(name,COVER1) != 0)
	  if(strcmp(name,COVER2) != 0)
	    found = 0;
      }
      break;
    case SLIT :
      if (name[0])
      {
	found = 1;

	if(strcmp(name,SLIT1) != 0)
	  if(strcmp(name,SLIT2) != 0)
	    if(strcmp(name,SLIT3) != 0)
	      if(strcmp(name,SLIT4) != 0)
		if(strcmp(name,SLIT5) != 0)
		  if(strcmp(name,SLIT6) != 0)
		    if(strcmp(name,SLIT7) != 0)
		      if(strcmp(name,SLIT8) != 0)
			if(strcmp(name,SLIT9) != 0)
			  if(strcmp(name,SLIT10) != 0)
			    if(strcmp(name,SLIT11) != 0)
			      if(strcmp(name,SLIT12) != 0)
			        if(strcmp(name,SLIT13) != 0)
				found = 0;
      }
      /* printMech(fw1Tbl); */
      break;
    case DECKER :
      if (name[0])
      {
	found = 1;
	if(strcmp(name,DECKER1) != 0)
	  if(strcmp(name,DECKER2) != 0)
	    if(strcmp(name,DECKER3) != 0)
	      if(strcmp(name,DECKER4) != 0)
		if(strcmp(name,DECKER5) != 0)
		  if(strcmp(name,DECKER6) != 0)
		    if(strcmp(name,DECKER7) != 0)
		      if(strcmp(name,DECKER8) != 0)
			if(strcmp(name,DECKER9) != 0)
			  if(strcmp(name,DECKER10) != 0)
			    if(strcmp(name,DECKER11) != 0)
			  found = 0;
      }
      break;
    case ACQ :
      if (name[0])
      {
	found = 1;
	if(strcmp(name,ACQ1) != 0)
	  if(strcmp(name,ACQ2) != 0)
	    found = 0;
      }
      break;

    case GRATING :
      if (name[0])
      {
	found = 1;
	if(strcmp(name,GRATING1) != 0)
	  if(strcmp(name,GRATING2) != 0)
	    if(strcmp(name,GRATING3) != 0)
	      if(strcmp(name,GRATING4) != 0)
	       if(strcmp(name,GRATING5) != 0)
	        if(strcmp(name,GRATING6) != 0)
	         if(strcmp(name,GRATING7) != 0)
	          if(strcmp(name,GRATING8) != 0)
	           if(strcmp(name,GRATING9) != 0)
	            if(strcmp(name,GRATING10) != 0)
	             if(strcmp(name,GRATING11) != 0)
	              if(strcmp(name,GRATING12) != 0)
	               if(strcmp(name,GRATING13) != 0)
	                if(strcmp(name,GRATING14) != 0)
	                 if(strcmp(name,GRATING15) != 0)
	                  if(strcmp(name,GRATING16) != 0)
	                   if(strcmp(name,GRATING17) != 0)
	                    if(strcmp(name,GRATING18) != 0)
	                     if(strcmp(name,GRATING19) != 0)
		found = 0;
      }


      break; 
    case XDISP :	
      if (name[0])
      {
	found = 1;
	if(strcmp(name,XDISP1) != 0)
	  if(strcmp(name,XDISP2) != 0)
	    if(strcmp(name,XDISP3) != 0)
	      if(strcmp(name,XDISP4) != 0)
		if(strcmp(name,XDISP5) != 0)
		  if(strcmp(name,XDISP6) != 0)
	  	    if(strcmp(name,XDISP7) != 0)
		      if(strcmp(name,XDISP8) != 0)
		        if(strcmp(name,XDISP9) != 0)
		          if(strcmp(name,XDISP10) != 0)
	  	             if(strcmp(name,XDISP11) != 0)
		               if(strcmp(name,XDISP12) != 0)
		                 if(strcmp(name,XDISP13) != 0)
		                   if(strcmp(name,XDISP14) != 0)
                                     if(strcmp(name,XDISP15) != 0)
                                       if(strcmp(name,XDISP16) != 0)
                                         if(strcmp(name,XDISP17) != 0)
                                           if(strcmp(name,XDISP18) != 0)
                                             if(strcmp(name,XDISP19) != 0)
                                               if(strcmp(name,XDISP20) != 0)
	  	                                 if(strcmp(name,XDISP21) != 0)
		                                   if(strcmp(name,XDISP22) != 0)
		found = 0;
      }

      break;
    case CAMERA :	
      if (name[0])
      {
	found = 1;
	if(strcmp(name,CAMERA1) != 0)
	  if(strcmp(name,CAMERA2) != 0)
	    if(strcmp(name,CAMERA3) != 0)
	      if(strcmp(name,CAMERA4) != 0)
		if(strcmp(name,CAMERA5) != 0)
		      found = 0;
      }
      break;
    case FOCUS :	
      if (name[0])
      {
	found = 1;
	if(strcmp(name,FOCUS1) != 0)
	  if(strcmp(name,FOCUS2) != 0)
	    found = 0;
      }
      break;
  }
/*
  printMech(tbl);
  printf("findMechName: mech = %d, name = %s\n",mech,name);
*/
  return found;
}
#if 1
/******************************************************************************/

/*+
 *   Function name:
 *   nameMatch
 *
 *   Purpose:
 *   Find filter names for a given filter wheel : return the names
 *   as an array of strings
 *
 *   Description:
 *   Search through the barcode/wheel database list for each barcode
 *   for that wheel. Find the name of the filter corresponding to that
 *   barcode by searching through the filter name/barcode list. Only
 *   MAXMENU filter names per wheel are sought: any found beyond this are 
 *   ignored  
 *
 *   Invocation:
 *   nameMatch( filtnames, &nfound)
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *   (<)   filtnames   (char [][])  Array of names of filters on that wheel 
 *   (<)   nfound      (int)        Number of filters found on wheel
 *
 *   Function value:
 *   (<)  status  (long) Return status, 0 = OK
 *                       Error if no filter names located for this wheel
 * 
 *   External functions:
 *   (VxWorks lstLib routines) lstFirst, lstNext
 * 
 *-
 */

long nameMatch( char filtnames[][LUT_TAG_SZ], int *nfound,void **tbl)
{
    MECHLUT *pw;  /* Pointer to barcode/wheel lookup table item */
  
    long status;
    long nWheelFilt = 0; /* Number of filters found on this wheel */
    long nfilt = 0;      /* Number of filter names found */
    long nfailed = 0;    /* Number of filter names not found */

    status = CAD_ACCEPT;
    
	/*
	 * Search through the list of barcodes for each barcode corresponding
	 * to the specified wheel number. The search will stop immediately if
	 * tbl is NULL.
	 */
	for (pw = (MECHLUT *) lstFirst ((LIST *)*tbl); pw != NULL;
		 pw = (MECHLUT *) lstNext ((NODE *) pw))
	{
        
		/* Break out if there are already too many filters found on this wheel */
		nWheelFilt++;
		if ( nWheelFilt > FILTPERWHEEL )
		{
			printf("WARNING: More than %d filters defined for wheel\n",
				   FILTPERWHEEL);
			break;
		}

	
		/* Copy the filter name for this barcode into the returned name array*/
		
		strncpy(filtnames[nfilt], pw->name, LUT_TAG_SZ-1);
/* 		printf("pw->name = %s\n",pw->name); */
		
		nfilt++;
            
		
	}

	if (nWheelFilt == 0)
	{
	/* 	DBGMSG(DBG_NONE,"WARNING: No filters listed for wheel:"); */
		status = CAD_REJECT;   /* No filter barcodes found for this wheel */
	}
                 
	if (nfilt == 0)
	{
	/* 	DBGMSG(DBG_NONE,"WARNING: No filter barcodes could be translated into names on wheel:"   ); */
		return CAD_REJECT;   /* No filter names found at all */
	}

	if ( nfailed > 0 )
	{
		printf("WARNING: %d filter barcodes on wheel could not be translated into names\n",
			   nfailed);
	}

	*nfound = nfilt;
	return status;
}
#endif
void loadNames(struct genSubRecord *pgsub, char mechNames[][LUT_TAG_SZ],int nfound)
{
	if ( nfound > 0 )
	{
        strncpy((char *)pgsub->vala, mechNames[0], LUT_TAG_SZ-1) ;
        strncpy((char *)pgsub->valb, mechNames[1], LUT_TAG_SZ-1) ;
        strncpy((char *)pgsub->valc, mechNames[2], LUT_TAG_SZ-1) ;
        strncpy((char *)pgsub->vald, mechNames[3], LUT_TAG_SZ-1) ;
        strncpy((char *)pgsub->vale, mechNames[4], LUT_TAG_SZ-1) ;
        strncpy((char *)pgsub->valf, mechNames[5], LUT_TAG_SZ-1) ;
        strncpy((char *)pgsub->valg, mechNames[6], LUT_TAG_SZ-1) ;
        strncpy((char *)pgsub->valh, mechNames[7], LUT_TAG_SZ-1) ;
        strncpy((char *)pgsub->vali, mechNames[8], LUT_TAG_SZ-1) ;
        strncpy((char *)pgsub->valj, mechNames[9], LUT_TAG_SZ-1) ;
        strncpy((char *)pgsub->valk, mechNames[10], LUT_TAG_SZ-1) ;
        strncpy((char *)pgsub->vall, mechNames[11], LUT_TAG_SZ-1) ;
	}
	else
	{
        strncpy((char *)pgsub->vala, "(none)", LUT_TAG_SZ-1) ;
        strncpy((char *)pgsub->valb, "", LUT_TAG_SZ-1) ;
        strncpy((char *)pgsub->valc, "", LUT_TAG_SZ-1) ;
        strncpy((char *)pgsub->vald, "", LUT_TAG_SZ-1) ;
        strncpy((char *)pgsub->vale, "", LUT_TAG_SZ-1) ;
        strncpy((char *)pgsub->valf, "", LUT_TAG_SZ-1) ;
        strncpy((char *)pgsub->valg, "", LUT_TAG_SZ-1) ;
        strncpy((char *)pgsub->valh, "", LUT_TAG_SZ-1) ;
        strncpy((char *)pgsub->vali, "", LUT_TAG_SZ-1) ;
        strncpy((char *)pgsub->valj, "", LUT_TAG_SZ-1) ;
        strncpy((char *)pgsub->valk, "", LUT_TAG_SZ-1) ;
        strncpy((char *)pgsub->vall, "", LUT_TAG_SZ-1) ;
	}
}
/******************************************************************************/

/*+
 *   Function name:
 *   fw1Names
 *
 *   Purpose:
 *   Read the filter definition files and output the filter names
 *   for a specified wheel on the gensub record output links
 *
 *   Description:
 *   Call the routines to read the files and define the two lookup tables.
 *   Output 12 strings on gensub output links A..L. These are intended
 *   to be used to set 12 MBBI record state strings (ZRST,..,ELST)
 *
 *   Invocation:
 *   fw1Names(wheelnum, pgsub)
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *      (!)   pgsub  (struct genSubRecord *pgsub)  Pointer to gensub structure
 *      
 *
 *   EPICS input parameters:
 *      b => Wheel number (long)
 *      c => all filters lut file name (string)
 *      e => filter wheel lut file name (string)
 *
 *   EPICS output parameters:
 *      vala..vall => filter names (string). Some may be blank.
 *      valm       => error message (string)
 *
 *   Function value:
 *   (<)  status  (long) Return status, 0 = OK
 *                       Error if file reads fail or no filter names found.
 * 
 *-
 */
void printMech(void *tbl)
{
	MECHLUT *pw;
	printf("printmech: number of nodes = %d\n",lstCount(tbl));
	for (pw = (MECHLUT *) lstFirst ((LIST *)tbl); pw != NULL;
            pw = (MECHLUT *) lstNext ((NODE *) pw))
            printf("barcode = %d, name = %s, pos = %s\n",pw->barcode,pw->name,pw->pos);

}
long fw1Names(struct genSubRecord *pgsub)
{

     char mechNames[MAXMENU][LUT_TAG_SZ] = {""}; /* Filter name list */
     long status = CAD_ACCEPT;
     char lutName[STRING_BUF_SZ];
     int  nfound = 0;

     /* Read the file of filter names with barcodes */
     strncpy (lutName, (char *)pgsub->j, HALF_BUF_SZ);
     strcat  (lutName, "/");
     strncat (lutName, (char *)pgsub->l, HALF_BUF_SZ-2);
	/*read lut file and fill fw1Tbl */

     status = mechLutRead(lutName,&fw1Tbl);


      if (status == CAD_ACCEPT)
		 status = nameMatch( mechNames, &nfound,&fw1Tbl);
	  printf("updating menus\n");

     /* Copy out the array of names to the output fields VALA..VALL */
	 loadNames(pgsub,mechNames,nfound); 
    
     return status;
}

/******************************************************************************/

/*+
 *   Function name:
 *   acqNames
 *
 *   Purpose:
 *   Read the filter definition files and output the filter names
 *   for a specified wheel on the gensub record output links
 *
 *   Description:
 *   Call the routines to read the files and define the two lookup tables.
 *   Output 12 strings on gensub output links A..L. These are intended
 *   to be used to set 12 MBBI record state strings (ZRST,..,ELST)
 *
 *   Invocation:
 *   acqNames(wheelnum, pgsub)
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *      (!)   pgsub  (struct genSubRecord *pgsub)  Pointer to gensub structure
 *      
 *
 *   EPICS input parameters:
 *      b => Wheel number (long)
 *      c => all filters lut file name (string)
 *      e => filter wheel lut file name (string)
 *
 *   EPICS output parameters:
 *      vala..vall => filter names (string). Some may be blank.
 *      valm       => error message (string)
 *
 *   Function value:
 *   (<)  status  (long) Return status, 0 = OK
 *                       Error if file reads fail or no filter names found.
 * 
 *-
 */
long setMechNamesInit( struct genSubRecord* pGsub )
{
    long status = CAD_ACCEPT;
#if 0
      /* park, datum, pos, and steps cads will call this, only do once*/
    if (!done)
    {
		/*create msg q*/
		LOG_MSG(DEBUG2_MSG,"create Msg Queue\n");
		mechNameQ = msgQCreate(4,sizeof(mechNameMsg),MSG_Q_FIFO);
		if(mechNamesQ == NULL)
		{
			status = CAD_REJECT;
			strncpy(MESSAGE,"Init: Error creating messageQ",MAX_STRING_SIZE - 1);
		}
		/*spawn task*/
		LOG_MSG(DEBUG2_MSG,"spawn task\n");
		if(status == CAD_ACCEPT)
			fw1Id = taskSpawn("tmechNameCtrl",50,VX_FP_TASK,4000,setMechNames, 0,0,0,0,0,0,0,0,0,0);
		if(fw1Id == ERROR)
		{
			status = CAD_REJECT;
			strncpy(MESSAGE,"Init: Error Spawning task",MAX_STRING_SIZE - 1);
		}
		if(status = CAD_ACCEPT)
			done = 1;
    }
#endif
   
    return status;
}
long setMechNames(int n1, int n2, int n3, int n4, int n5, int n6, 
			 int n7, int n8, int n9, int n10)
{
	long status;
#if 0
	while (1)
	{	if( msgQReceive( mechNameQ, (char *)&msg, sizeof(mechNameMsg ), 
						 WAIT_FOREVER ) == ERROR ) 
		{
			LOG_MSG(ERROR_MSG,"error in fw1Ctrl msgReceive\n");
			continue;
		}
		/* Read the file of filter names with barcodes */
		strncpy (lutName, (char *)pgsub->j, HALF_BUF_SZ);
		strcat  (lutName, "/");
		strncat (lutName, (char *)pgsub->l, HALF_BUF_SZ-2);
		
		/*read lut file and fill acqTbl */

		status = mechLutRead(lutName,&acqTbl);

		if (status == CAD_ACCEPT)
			status = nameMatch( mechNames, &nfound,&acqTbl);


		/* Copy out the array of names to the output fields VALA..VALL */
		loadNames(pgsub,mechNames,nfound);
	}
#endif
	 return status;
}
long acqNames(struct genSubRecord *pgsub)
{

     char mechNames[MAXMENU][LUT_TAG_SZ] = {""}; /* Filter name list */
     long status = CAD_ACCEPT;
     char lutName[STRING_BUF_SZ];
     int  nfound = 0;

     /* Read the file of filter names with barcodes */
     strncpy (lutName, (char *)pgsub->j, HALF_BUF_SZ);
     strcat  (lutName, "/");
     strncat (lutName, (char *)pgsub->l, HALF_BUF_SZ-2);

	 /*read lut file and fill acqTbl */

     status = mechLutRead(lutName,&acqTbl);
     if (status == CAD_ACCEPT)
		 status = nameMatch( mechNames, &nfound,&acqTbl);

     /* Copy out the array of names to the output fields VALA..VALL */
	 loadNames(pgsub,mechNames,nfound);
    
     return status;
}

/******************************************************************************/

/*+
 *   Function name:
 *   cameraNames
 *
 *   Purpose:
 *   Read the filter definition files and output the filter names
 *   for a specified wheel on the gensub record output links
 *
 *   Description:
 *   Call the routines to read the files and define the two lookup tables.
 *   Output 12 strings on gensub output links A..L. These are intended
 *   to be used to set 12 MBBI record state strings (ZRST,..,ELST)
 *
 *   Invocation:
 *   cameraNames(wheelnum, pgsub)
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *      (!)   pgsub  (struct genSubRecord *pgsub)  Pointer to gensub structure
 *      
 *
 *   EPICS input parameters:
 *      b => Wheel number (long)
 *      c => all filters lut file name (string)
 *      e => filter wheel lut file name (string)
 *
 *   EPICS output parameters:
 *      vala..vall => filter names (string). Some may be blank.
 *      valm       => error message (string)
 *
 *   Function value:
 *   (<)  status  (long) Return status, 0 = OK
 *                       Error if file reads fail or no filter names found.
 * 
 *-
 */

long cameraNames(struct genSubRecord *pgsub)
{

     char mechNames[MAXMENU][LUT_TAG_SZ] = {""}; /* Filter name list */
     long status = CAD_ACCEPT;
     char lutName[STRING_BUF_SZ];
     int  nfound = 0;

     /* Read the file of filter names with barcodes */
     strncpy (lutName, (char *)pgsub->j, HALF_BUF_SZ);
     strcat  (lutName, "/");
     strncat (lutName, (char *)pgsub->l, HALF_BUF_SZ-2);

	 /*read lut file and fill cameraTbl */
     status = mechLutRead(lutName,&cameraTbl);
     if (status == CAD_ACCEPT)
		 status = nameMatch( mechNames, &nfound,&cameraTbl);

     /* Copy out the array of names to the output fields VALA..VALL */
	 loadNames(pgsub,mechNames,nfound);
    
     return status;
}


/**
 * Function name:
 * 	cameraCheck()
 *
 * Purpose: This simply checks that the camera is the only
 * mechanism to mark the DC side selectWcs.B record.
 *
 * Description: The output here sets the SDIS appropriately
 * for the wcsMark record. With SDIS set to 1 the wcsMark record
 * will not process.
 *
 */
long cameraCheck (struct genSubRecord *pgsub) {

   long status = CAD_ACCEPT;

   if (!strcmp((char *) pgsub->a, "camera")) {
     printf("wcsCheck: camera match\n");
     *(long *) pgsub->vala=0;      /* enable the wcsMark*/   
   }

   else {
     printf("wcsCheck: not a camera mech\n");
     *(long *) pgsub->vala=1;      /* disable the wcsMark*/
   }

   return status;
}

/******************************************************************************/

/*+
 *   Function name:
 *   coverNames
 *
 *   Purpose:
 *   Read the filter definition files and output the filter names
 *   for a specified wheel on the gensub record output links
 *
 *   Description:
 *   Call the routines to read the files and define the two lookup tables.
 *   Output 12 strings on gensub output links A..L. These are intended
 *   to be used to set 12 MBBI record state strings (ZRST,..,ELST)
 *
 *   Invocation:
 *   coverNames(wheelnum, pgsub)
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *      (!)   pgsub  (struct genSubRecord *pgsub)  Pointer to gensub structure
 *      
 *
 *   EPICS input parameters:
 *      b => Wheel number (long)
 *      c => all filters lut file name (string)
 *      e => filter wheel lut file name (string)
 *
 *   EPICS output parameters:
 *      vala..vall => filter names (string). Some may be blank.
 *      valm       => error message (string)
 *
 *   Function value:
 *   (<)  status  (long) Return status, 0 = OK
 *                       Error if file reads fail or no filter names found.
 * 
 *-
 */

long coverNames(struct genSubRecord *pgsub)
{

     char mechNames[MAXMENU][LUT_TAG_SZ] = {""}; /* Filter name list */
     long status = CAD_ACCEPT;
     char lutName[STRING_BUF_SZ];
     int  nfound = 0;

     /* Read the file of filter names with barcodes */
     strncpy (lutName, (char *)pgsub->j, HALF_BUF_SZ);
     strcat  (lutName, "/");
     strncat (lutName, (char *)pgsub->l, HALF_BUF_SZ-2);

	 /*read lut file and fill coverTbl */
     status = mechLutRead(lutName,&coverTbl);
     if (status == CAD_ACCEPT)
		 status = nameMatch(mechNames, &nfound,&coverTbl);

     /* Copy out the array of names to the output fields VALA..VALL */
	 loadNames(pgsub,mechNames,nfound);
    
     return status;
}

/******************************************************************************/

/*+
 *   Function name:
 *   deckerNames
 *
 *   Purpose:
 *   Read the filter definition files and output the filter names
 *   for a specified wheel on the gensub record output links
 *
 *   Description:
 *   Call the routines to read the files and define the two lookup tables.
 *   Output 12 strings on gensub output links A..L. These are intended
 *   to be used to set 12 MBBI record state strings (ZRST,..,ELST)
 *
 *   Invocation:
 *   deckerNames(wheelnum, pgsub)
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *      (!)   pgsub  (struct genSubRecord *pgsub)  Pointer to gensub structure
 *      
 *
 *   EPICS input parameters:
 *      b => Wheel number (long)
 *      c => all filters lut file name (string)
 *      e => filter wheel lut file name (string)
 *
 *   EPICS output parameters:
 *      vala..vall => filter names (string). Some may be blank.
 *      valm       => error message (string)
 *
 *   Function value:
 *   (<)  status  (long) Return status, 0 = OK
 *                       Error if file reads fail or no filter names found.
 * 
 *-
 */

long deckerNames(struct genSubRecord *pgsub)
{

     char mechNames[MAXMENU][LUT_TAG_SZ] = {""}; /* Filter name list */
     long status = CAD_ACCEPT;
     char lutName[STRING_BUF_SZ];
     int  nfound = 0;

     /* Read the file of filter names with barcodes */
     strncpy (lutName, (char *)pgsub->j, HALF_BUF_SZ);
     strcat  (lutName, "/");
     strncat (lutName, (char *)pgsub->l, HALF_BUF_SZ-2);

	 /*read lut file and fill deckerTbl */
     status = mechLutRead(lutName,&deckerTbl);
     if (status == CAD_ACCEPT)
		 status = nameMatch( mechNames, &nfound,&deckerTbl);

     /* Copy out the array of names to the output fields VALA..VALL */
	 loadNames(pgsub,mechNames,nfound);
    
     return status;
}

/******************************************************************************/

/*+
 *   Function name:
 *   focusNames
 *
 *   Purpose:
 *   Read the filter definition files and output the filter names
 *   for a specified wheel on the gensub record output links
 *
 *   Description:
 *   Call the routines to read the files and define the two lookup tables.
 *   Output 12 strings on gensub output links A..L. These are intended
 *   to be used to set 12 MBBI record state strings (ZRST,..,ELST)
 *
 *   Invocation:
 *   focusNames(wheelnum, pgsub)
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *      (!)   pgsub  (struct genSubRecord *pgsub)  Pointer to gensub structure
 *      
 *
 *   EPICS input parameters:
 *      b => Wheel number (long)
 *      c => all filters lut file name (string)
 *      e => filter wheel lut file name (string)
 *
 *   EPICS output parameters:
 *      vala..vall => filter names (string). Some may be blank.
 *      valm       => error message (string)
 *
 *   Function value:
 *   (<)  status  (long) Return status, 0 = OK
 *                       Error if file reads fail or no filter names found.
 * 
 *-
 */

long focusNames(struct genSubRecord *pgsub)
{

     char mechNames[MAXMENU][LUT_TAG_SZ] = {""}; /* Filter name list */
     long status = CAD_ACCEPT;
     char lutName[STRING_BUF_SZ];
     int  nfound = 0;

     /* Read the file of filter names with barcodes */
     strncpy (lutName, (char *)pgsub->j, HALF_BUF_SZ);
     strcat  (lutName, "/");
     strncat (lutName, (char *)pgsub->l, HALF_BUF_SZ-2);

	 /*read lut file and fill focusTbl */
     status = mechLutRead(lutName,&focusTbl);
     if (status == CAD_ACCEPT)
		 status = nameMatch( mechNames, &nfound,&focusTbl);

     /* Copy out the array of names to the output fields VALA..VALL */
	 loadNames(pgsub,mechNames,nfound);
    
     return status;
}

/******************************************************************************/

/*+
 *   Function name:
 *   fw2Names
 *
 *   Purpose:
 *   Read the filter definition files and output the filter names
 *   for a specified wheel on the gensub record output links
 *
 *   Description:
 *   Call the routines to read the files and define the two lookup tables.
 *   Output 12 strings on gensub output links A..L. These are intended
 *   to be used to set 12 MBBI record state strings (ZRST,..,ELST)
 *
 *   Invocation:
 *   fw2Names(wheelnum, pgsub)
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *      (!)   pgsub  (struct genSubRecord *pgsub)  Pointer to gensub structure
 *      
 *
 *   EPICS input parameters:
 *      b => Wheel number (long)
 *      c => all filters lut file name (string)
 *      e => filter wheel lut file name (string)
 *
 *   EPICS output parameters:
 *      vala..vall => filter names (string). Some may be blank.
 *      valm       => error message (string)
 *
 *   Function value:
 *   (<)  status  (long) Return status, 0 = OK
 *                       Error if file reads fail or no filter names found.
 * 
 *-
 */

long fw2Names(struct genSubRecord *pgsub)
{

     char mechNames[MAXMENU][LUT_TAG_SZ] = {""}; /* Filter name list */
     long status = CAD_ACCEPT;
     char lutName[STRING_BUF_SZ];
     int  nfound = 0;

     /* Read the file of filter names with barcodes */
     strncpy (lutName, (char *)pgsub->j, HALF_BUF_SZ);
     strcat  (lutName, "/");
     strncat (lutName, (char *)pgsub->l, HALF_BUF_SZ-2);

	 /*read lut file and fill fw2Tbl */
     status = mechLutRead(lutName,&fw2Tbl);
     if (status == CAD_ACCEPT)
		 status = nameMatch( mechNames, &nfound,&fw2Tbl);

     /* Copy out the array of names to the output fields VALA..VALL */
	 loadNames(pgsub,mechNames,nfound);
    
     return status;
}

/******************************************************************************/

/*+
 *   Function name:
 *   gratingNames
 *
 *   Purpose:
 *   Read the filter definition files and output the filter names
 *   for a specified wheel on the gensub record output links
 *
 *   Description:
 *   Call the routines to read the files and define the two lookup tables.
 *   Output 12 strings on gensub output links A..L. These are intended
 *   to be used to set 12 MBBI record state strings (ZRST,..,ELST)
 *
 *   Invocation:
 *   gratingNames(wheelnum, pgsub)
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *      (!)   pgsub  (struct genSubRecord *pgsub)  Pointer to gensub structure
 *      
 *
 *   EPICS input parameters:
 *      b => Wheel number (long)
 *      c => all filters lut file name (string)
 *      e => filter wheel lut file name (string)
 *
 *   EPICS output parameters:
 *      vala..vall => filter names (string). Some may be blank.
 *      valm       => error message (string)
 *
 *   Function value:
 *   (<)  status  (long) Return status, 0 = OK
 *                       Error if file reads fail or no filter names found.
 * 
 *-
 */

long gratingNames(struct genSubRecord *pgsub)
{

     char mechNames[MAXMENU][LUT_TAG_SZ] = {""}; /* Filter name list */
     long status = CAD_ACCEPT;
     char lutName[STRING_BUF_SZ];
     int  nfound = 0;

     /* Read the file of filter names with barcodes */
     strncpy (lutName, (char *)pgsub->j, HALF_BUF_SZ);
     strcat  (lutName, "/");
     strncat (lutName, (char *)pgsub->l, HALF_BUF_SZ-2);



     printf("--------------------> Generating Grating Names\n");
     printf("--------------------> Generating Grating LUT: %s\n", lutName);






	 /*read lut file and fill gratingTbl */
     status = mechLutRead(lutName,&gratingTbl);
     if (status == CAD_ACCEPT)
	 status = nameMatch( mechNames, &nfound,&gratingTbl);

     /* Copy out the array of names to the output fields VALA..VALL */
     loadNames(pgsub,mechNames,nfound);
    
     return status;
}

/******************************************************************************/

/*+
 *   Function name:
 *   slitNames
 *
 *   Purpose:
 *   Read the filter definition files and output the filter names
 *   for a specified wheel on the gensub record output links
 *
 *   Description:
 *   Call the routines to read the files and define the two lookup tables.
 *   Output 12 strings on gensub output links A..L. These are intended
 *   to be used to set 12 MBBI record state strings (ZRST,..,ELST)
 *
 *   Invocation:
 *   slitNames(wheelnum, pgsub)
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *      (!)   pgsub  (struct genSubRecord *pgsub)  Pointer to gensub structure
 *      
 *
 *   EPICS input parameters:
 *      b => Wheel number (long)
 *      c => all filters lut file name (string)
 *      e => filter wheel lut file name (string)
 *
 *   EPICS output parameters:
 *      vala..vall => filter names (string). Some may be blank.
 *      valm       => error message (string)
 *
 *   Function value:
 *   (<)  status  (long) Return status, 0 = OK
 *                       Error if file reads fail or no filter names found.
 * 
 *-
 */

long slitNames(struct genSubRecord *pgsub)
{

     char mechNames[MAXMENU][LUT_TAG_SZ] = {""}; /* Filter name list */
     long status = CAD_ACCEPT;
     char lutName[STRING_BUF_SZ];
     int  nfound = 0;

     /* Read the file of filter names with barcodes */
     strncpy (lutName, (char *)pgsub->j, HALF_BUF_SZ);
     strcat  (lutName, "/");
     strncat (lutName, (char *)pgsub->l, HALF_BUF_SZ-2);

	 /*read lut file and fill slitTbl */
     status = mechLutRead(lutName,&slitTbl);
     if (status == CAD_ACCEPT)
	 status = nameMatch( mechNames, &nfound,&slitTbl);

     /* Copy out the array of names to the output fields VALA..VALL */
     loadNames(pgsub,mechNames,nfound);
    
     return status;
}

/******************************************************************************/

/*+
 *   Function name:
 *   xdispNames
 *
 *   Purpose:
 *   Read the filter definition files and output the filter names
 *   for a specified wheel on the gensub record output links
 *
 *   Description:
 *   Call the routines to read the files and define the two lookup tables.
 *   Output 12 strings on gensub output links A..L. These are intended
 *   to be used to set 12 MBBI record state strings (ZRST,..,ELST)
 *
 *   Invocation:
 *   xdispNames(wheelnum, pgsub)
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *      (!)   pgsub  (struct genSubRecord *pgsub)  Pointer to gensub structure
 *      
 *
 *   EPICS input parameters:
 *      b => Wheel number (long)
 *      c => all filters lut file name (string)
 *      e => filter wheel lut file name (string)
 *
 *   EPICS output parameters:
 *      vala..vall => filter names (string). Some may be blank.
 *      valm       => error message (string)
 *
 *   Function value:
 *   (<)  status  (long) Return status, 0 = OK
 *                       Error if file reads fail or no filter names found.
 * 
 *-
 */

long xdispNames(struct genSubRecord *pgsub)
{

     char mechNames[MAXMENU][LUT_TAG_SZ] = {""}; /* Filter name list */
     long status = CAD_ACCEPT;
     char lutName[STRING_BUF_SZ];
     int  nfound = 0;

     /* Read the file of filter names with barcodes */
     strncpy (lutName, (char *)pgsub->j, HALF_BUF_SZ);
     strcat  (lutName, "/");
     strncat (lutName, (char *)pgsub->l, HALF_BUF_SZ-2);

	 /*read lut file and fill xdispTbl */
     status = mechLutRead(lutName,&xdispTbl);
     if (status == CAD_ACCEPT)
         status = nameMatch( mechNames, &nfound,&xdispTbl);

     /* Copy out the array of names to the output fields VALA..VALL */
	 loadNames(pgsub,mechNames,nfound);
    
     return status;
}
