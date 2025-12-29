static struct {void *v; char *c;} rcsid = {&rcsid,
	"$Id: nirsSeqDisplayMenus.c,v 1.2 2009/05/27 19:33:46 fkraemer Exp $"};

/* *INDENT-OFF* */
/*
 *   FILENAME
 *   -------- 
 *   gmSeqDisplayMenus.c
 *
 *   DESCRIPTION
 *   -----------
 *   This file contains the source for functions used by the GNIRS
 *   Sequencer to get the names of the filters mounted in the two 
 *   filter wheels, the grating names and the mask names. All to be used
 *   to provide selection menu records of these items for display use. 
 *
 *   FUNCTION NAME(S)
 *   ----------------
 *
 *   gmSeqAllFilterLUTread    - Read database file of all filters + barcodes.
 *   seqMechLutRead  - Read database file of all barcodes with for mechanism
 *   seqNameMatch     - Search through the two filter lookup tables to
 *                              obtain the names of filters loaded on a
 *                              particular wheel.
 *   seqfw1Names    - gensub subroutine to get filter names and
 *                              write the names as record outputs.

 *
 *   AUTHOR
 *   ------
 *   Philip Taylor  (pbt@observatorysciences.co.uk)
 *
 *   Copyright Observatory Sciences Ltd. 2000. All rights reserved.
 *   Under contract to the UK Astronomy Technology Centre, who modified the code
 *   in 2000/2001.
 *
 *   HISTORY
 *   -------
 *   10-May-2000: Original version.                       (pbt)
 *   24-Oct-2000: Removed hard-wired lookup table names.  (smb)
 *   05-Feb-2001: Added CVS log below.                    (smb)
 */
/*
 * $Log: nirsSeqDisplayMenus.c,v $
 * Revision 1.2  2009/05/27 19:33:46  fkraemer
 * fkraemer - copied my complete working dir over trunk
 *
 * Revision 1.22  2001/02/23 13:14:08  gmos
 * Added functions to translate mask names into barcode IDs, based on names read from masks.lut. Renamed global variables so they begin gmSeq. Made the filter and mask LUT reading functions more robust.
 *
 * Revision 1.21  2001/02/22 17:16:09  gmos
 * Comments updated
 *
 * Revision 1.20  2001/02/21 18:14:16  gmos
 * New code to separate mask data into three separate menus - one for each cassette.
 *
 * Revision 1.19  2001/02/21 14:41:29  gmos
 * in 2001 --> in 2000/2001
 *
 * Revision 1.18  2001/02/21 13:34:49  gmos
 * Copyright statement modified.
 *
 * Revision 1.17  2001/02/05 13:07:41  gmos
 * Code prologue tidied up.
 *
 *
 */
/* *INDENT-ON* */

#include  <vxWorks.h>
#include  <stdlib.h>
#include  <types.h>
#include  <stdioLib.h>
#include  <lstLib.h>
#include  <strLib.h>
#include  <rebootLib.h>
#include  <sysLib.h>

#include  <stdio.h>
#include  <string.h>
#include  <math.h>

#include  <genSubRecord.h>
#include  <cad.h>

#include "nirsSeq.h"

/* #define VERBOSE */    /* Define to enable extra printf statements */

/*
 * Define pointers to local and external data structures :-
 */

/*
 * Pointers to the local and global lookup table containing the list of all known
 * filters, together with their barcodes and focus offsets. The local pointer is
 * redundant, being available to test this module stand-alone. The global pointer
 * is initialized by function gmSeqFilterLUTRead() in gmSeqCadLib.c.
 */


extern void *fw1LutPtr;             /* Global pointer            */
extern void *fw2LutPtr;             /* Global pointer            */
#if 0
extern void *focusLutPtr;             /* Global pointer            */
extern void *cameraLutPtr;             /* Global pointer            */
extern void *coverLutPtr;             /* Global pointer            */
extern void *xdispLutPtr;             /* Global pointer            */
extern void *slitLutPtr;             /* Global pointer            */
extern void *deckerLutPtr;             /* Global pointer            */
extern void *acqLutPtr;             /* Global pointer            */
extern void *gratingLutPtr;             /* Global pointer            */
#endif
/*
 * Pointer to the local lookup table of loaded filter barcodes,  and position.
 */

static void *fw1Tbl = NULL;
static void *fw2Tbl = NULL;
#if 0
static void *focusTbl = NULL;
static void *cameraTbl = NULL;
static void *coverTbl = NULL;
static void *xdispTbl = NULL;
static void *gratingTbl = NULL;
static void *deckerTbl = NULL;
static void *slitTbl = NULL;
static void *acqTbl = NULL;
#endif    

/*
 * Global variable containing the debugging level flag declared in gmSeqCadLib.
 */

extern int gmSeqDbgLevel;

/*
 * Local function definitions.
 */
long nirsLutRead( char * lutfilename, void **lutPtr);
void seqLoadNames(struct genSubRecord *pgsub,char mechNames[][LUT_TAG_SZ],int nfound);
long seqNameMatch( char filtnames[][LUT_TAG_SZ], int *nfound,void **tbl,void **lutPtr);
/* long gmSeqMaskNameMatch(long cassettenum, char masknames[][LUT_TAG_SZ], int *nfound); */


/******************************************************************************/

/*+
 *   Function name:
 *   seqMechLutRead
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
 *   seqMechLutRead( char * lutfilename )
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
 *   Derived from the routine gmSeqFilterLUTread but
 *   to be used to read the barcode list file for
 *   setting up the filter menus 
 * 
 *-
 */


long seqMechLutRead( char *lutfilename, void **tbl )
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
 printf("\n\nfilename = %s\n",lutfilename);
/* Free up old list if this is not the first entry */
    if (*tbl != NULL) 
    {
		printf("free list\n");
       lstFree((LIST *) *tbl);
      free(*tbl);
      *tbl = NULL;
    }    
 
/* read wheel contents data file from remote disk */

    if ((fp = fopen (lutfilename, "r")) == NULL)
    {
        printf("seqMechLutRead error: failed to open file %s\n", lutfilename);
        return CAD_REJECT;
    }

    DBGMSGSTRING(DBG_MIN,"seqMechLutRead: opened filter lut file:", lutfilename);
    
    *tbl = (void *) malloc (sizeof (LIST));
    if (*tbl == NULL)    /* malloc failed */
    {
        printf("seqMechLutRead: LUT malloc failed\n");
        fclose(fp);
        return CAD_REJECT;
    }
    lstInit ((LIST *) *tbl);
    
    lcount = 0;
    while (status == 0)
	{
		/*      skip blank lines and comments */

		buf = fgets (tag, 255, fp);    
/* 		printf("\ntag = %s",tag); */
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
					printf("seqMechLutRead: LUT node malloc failed\n");
					fclose(fp);
					return CAD_REJECT;
				}
				lstAdd ((LIST *) *tbl, (NODE *) p);
				p->barcode = barcode;
				strncpy(p->name,name,39);
				strncpy(p->pos,pos,POS_LEN-1);
				p->pos[POS_LEN-1] = NULL;
				p->tilt = tilt;
			 /* 	printf("barcode %d,  name = %s, pos = %s, tilt = %f\n",barcode,name,pos,tilt); */
				DBGMSGSTRING(DBG_FULL,"seqMechLutRead data line read OK:",tag);
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
    
    
    return status;
}

/******************************************************************************/

/*+
 *   Function name:
 *   seqNameMatch
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
 *   seqNameMatch( filtnames, &nfound)
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

long seqNameMatch( char filtnames[][LUT_TAG_SZ], int *nfound,void **tbl,void **lutPtr)
{
    MECHLUT *pw;  /* Pointer to barcode/wheel lookup table item */
    NIRSLUT   *pa;     /* Pointer to filter name/barcode lookup table item */
    long barcode;
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

		barcode = pw->barcode;  /* The barcode to be searched for */
            
		/*
		 * Now search through the list of all filters for the name of the filter 
		 * with this barcode. The search will stop immediately if lutPtr
		 * is NULL.
		 */

		for (pa = (NIRSLUT *) lstFirst ((LIST *)lutPtr); pa != NULL;
			 pa = (NIRSLUT *) lstNext ((NODE *) pa))
		{
			if (barcode == pa->barcodeId)   /* Found the barcode we wanted */
			{
                if (nfilt >= FILTPERWHEEL)
                {
					printf("WARNING: More than %d filter names found for wheel\n",
						   FILTPERWHEEL);
					break;
                }
                else
                {

					/* Copy the filter name for this barcode into the returned name array */
#define VERBOSE
					strncpy(filtnames[nfilt], pa->tag, LUT_TAG_SZ-1);
#ifdef VERBOSE
					printf("%d. Barcode %d, filter name %s\n", nfilt, barcode,
						   filtnames[nfilt]);
#endif
					nfilt++;
					break;
                }
              
            }
            
            if (pa == NULL)
            {

				/* No match found for barcode: use the barcode string instead of the name */

				DBGMSGINT(DBG_MIN, "Check filters.lut. No name found for filter with barcode:",
						  barcode);
				sprintf(filtnames[nfilt], "%d%c", barcode, '\0');
				nfilt++;
				nfailed++;
            }
		}
	}

	if (nWheelFilt == 0)
	{
		DBGMSG(DBG_NONE,"WARNING: No filters listed for wheel:");
		status = CAD_REJECT;   /* No filter barcodes found for this wheel */
	}
                 
	if (nfilt == 0)
	{
		DBGMSG(DBG_NONE,"WARNING: No  barcodes could be translated into names:"   );
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

void seqLoadNames(struct genSubRecord *pgsub, char mechNames[][LUT_TAG_SZ],int nfound)
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
 *   seqfw1Names
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
 *   seqfw1Names(wheelnum, pgsub)
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *      (!)   pgsub  (struct genSubRecord *pgsub)  Pointer to gensub structure
 *      
 *
 *   EPICS input parameters:
 *      j => lut file path
 *      l => all filters lut file name (string)
 *      n => filter wheel lut file name (string)
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
void printNirs(void *lutPtr)
{
	NIRSLUT   *pa;
	for (pa = (NIRSLUT *) lstFirst ((LIST *)lutPtr); pa != NULL;
			 pa = (NIRSLUT *) lstNext ((NODE *) pa))
		printf("barcode = %d, name = %s\n",pa->barcodeId,pa->tag);

}

long seqfw1Names(struct genSubRecord *pgsub)
{

     char mechNames[MAXMENU][LUT_TAG_SZ] = {""}; /* Filter name list */
     long status = CAD_ACCEPT;
     char lutName[STRING_BUF_SZ];
     int  nfound = 0;

	 
	 strncpy (lutName, (char *)pgsub->j, HALF_BUF_SZ);
	 strcat  (lutName, "/");
	 strncat (lutName, (char *)pgsub->n, HALF_BUF_SZ-2);
	/*  printf("%s, %s\n",pgsub->j,pgsub->n); */

	 status = nirsLutRead(lutName,&fw1LutPtr);

     /* Read the file of filter names with barcodes */
     strncpy (lutName, (char *)pgsub->j, HALF_BUF_SZ);
     strcat  (lutName, "/");
     strncat (lutName, (char *)pgsub->l, HALF_BUF_SZ-2);
	/*read lut file and fill fw1Tbl */

     status = seqMechLutRead(lutName,&fw1Tbl);

/* 	printf("seqfw1Names: number of nodes = %d\n",lstCount(fw1Tbl)); */
/* 	printMech(fw1Tbl); */
/* 	printNirs(fw1LutPtr); */
     if (status == CAD_ACCEPT)
		 status = seqNameMatch( mechNames, &nfound,&fw1Tbl,&fw1LutPtr);


     /* Copy out the array of names to the output fields VALA..VALL */
	 seqLoadNames(pgsub,mechNames,nfound);
    
     return status;
}

#if 0
/******************************************************************************/

/*+
 *   Function name:
 *   seqacqNames
 *
 *   Purpose:
 *   Read the  definition files and output the  names
 *    on the gensub record output links
 *
 *   Description:
 *   Call the routines to read the files and define the two lookup tables.
 *   Output 12 strings on gensub output links A..L. These are intended
 *   to be used to set 12 MBBI record state strings (ZRST,..,ELST)
 *
 *   Invocation:
 *   seqacqNames(wheelnum, pgsub)
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *      (!)   pgsub  (struct genSubRecord *pgsub)  Pointer to gensub structure
 *      
 *
 *   EPICS input parameters:
 *     
 *      j => lut file path
 *      l => all positions lut file name (string)
 *      n => lut file name (string)
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

long seqacqNames(struct genSubRecord *pgsub)
{

     char mechNames[MAXMENU][LUT_TAG_SZ] = {""}; /* Filter name list */
     long status = CAD_ACCEPT;
     char lutName[STRING_BUF_SZ];
     int  nfound = 0;
	/* acq lut*/	
		strncpy (lutName, (char *)pgsub->j, HALF_BUF_SZ);
		strcat  (lutName, "/");
		strncat (lutName,(char *)pgsub->n , HALF_BUF_SZ-2);
		
		status = nirsLutRead(lutName,&acqLutPtr);
		if (status != CAD_ACCEPT)
		{
		/* 	strncpy(CADMESS, "Error initializing Mask database", */
/* 					MAX_STRING_SIZE-1); */
			DBGMSG(DBG_MIN,"gmSeqCadInit: Error initialising Mask database");
		}
		else
		{
			DBGMSG(DBG_MIN,"Initialized mask database OK");
		}
     /* Read the file of filter names with barcodes */
     strncpy (lutName, (char *)pgsub->j, HALF_BUF_SZ);
     strcat  (lutName, "/");
     strncat (lutName, (char *)pgsub->l, HALF_BUF_SZ-2);

	 /*read lut file and fill acqTbl */

     status = seqMechLutRead(lutName,&acqTbl);

     if (status == CAD_ACCEPT)
		 status = seqNameMatch( mechNames, &nfound,&acqTbl,&acqLutPtr);


     /* Copy out the array of names to the output fields VALA..VALL */
	 seqLoadNames(pgsub,mechNames,nfound);
    
     return status;
}

/******************************************************************************/

/*+
 *   Function name:
 *   seqcameraNames
 *
 *   Purpose:
 *   Read the  definition files and output the  names
 *    on the gensub record output links
 *
 *   Description:
 *   Call the routines to read the files and define the two lookup tables.
 *   Output 12 strings on gensub output links A..L. These are intended
 *   to be used to set 12 MBBI record state strings (ZRST,..,ELST)
 *
 *   Invocation:
 *   seqcameraNames(wheelnum, pgsub)
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *      (!)   pgsub  (struct genSubRecord *pgsub)  Pointer to gensub structure
 *      
 *
 *   EPICS input parameters:
 *     
 *      j => lut file path
 *      l => all positions lut file name (string)
 *      n =>  lut file name (string)
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

long seqcameraNames(struct genSubRecord *pgsub)
{

     char mechNames[MAXMENU][LUT_TAG_SZ] = {""}; /* Filter name list */
     long status = CAD_ACCEPT;
     char lutName[STRING_BUF_SZ];
     int  nfound = 0;
	/* camera lut*/

		strncpy (lutName, (char *)pgsub->j, HALF_BUF_SZ);
		strcat  (lutName, "/");
		strncat (lutName,(char *)pgsub->n , HALF_BUF_SZ-2);
		
		status = nirsLutRead(lutName,&cameraLutPtr);
		if (status != CAD_ACCEPT)
		{
		/* 	strncpy(CADMESS, "Error initializing Mask database", */
/* 					MAX_STRING_SIZE-1); */
			DBGMSG(DBG_MIN,"gmSeqCadInit: Error initialising Mask database");
		}
		else
		{
			DBGMSG(DBG_MIN,"Initialized mask database OK");
		}
     /* Read the file of filter names with barcodes */
     strncpy (lutName, (char *)pgsub->j, HALF_BUF_SZ);
     strcat  (lutName, "/");
     strncat (lutName, (char *)pgsub->l, HALF_BUF_SZ-2);

	 /*read lut file and fill cameraTbl */

     status = seqMechLutRead(lutName,&cameraTbl);

     if (status == CAD_ACCEPT)
		 status = seqNameMatch( mechNames, &nfound,&cameraTbl,&cameraLutPtr);


     /* Copy out the array of names to the output fields VALA..VALL */
	 seqLoadNames(pgsub,mechNames,nfound);
    
     return status;
}

/******************************************************************************/

/*+
 *   Function name:
 *   seqcoverNames
 *
 *   Purpose:
 *   Read the  definition files and output the  names
 *    on the gensub record output links
 *
 *   Description:
 *   Call the routines to read the files and define the two lookup tables.
 *   Output 12 strings on gensub output links A..L. These are intended
 *   to be used to set 12 MBBI record state strings (ZRST,..,ELST)
 *
 *   Invocation:
 *   seqcoverNames(wheelnum, pgsub)
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *      (!)   pgsub  (struct genSubRecord *pgsub)  Pointer to gensub structure
 *      
 *
 *   EPICS input parameters:
 *     
 *      j => lut file path
 *      l => all positions lut file name (string)
 *      n => lut file name (string)
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

long seqcoverNames(struct genSubRecord *pgsub)
{

     char mechNames[MAXMENU][LUT_TAG_SZ] = {""}; /* Filter name list */
     long status = CAD_ACCEPT;
     char lutName[STRING_BUF_SZ];
     int  nfound = 0;
	/* cover lut*/	
		strncpy (lutName, (char *)pgsub->j, HALF_BUF_SZ);
		strcat  (lutName, "/");
		strncat (lutName, (char *)pgsub->n, HALF_BUF_SZ-2);
		
		status = nirsLutRead(lutName,&coverLutPtr);
		if (status != CAD_ACCEPT)
		{
		/* 	strncpy(CADMESS, "Error initializing Mask database", */
/* 					MAX_STRING_SIZE-1); */
			DBGMSG(DBG_MIN,"gmSeqCadInit: Error initialising Mask database");
		}
		else
		{
			DBGMSG(DBG_MIN,"Initialized mask database OK");
		}
     /* Read the file of filter names with barcodes */
     strncpy (lutName, (char *)pgsub->j, HALF_BUF_SZ);
     strcat  (lutName, "/");
     strncat (lutName, (char *)pgsub->l, HALF_BUF_SZ-2);

	 /*read lut file and fill coverTbl */

     status = seqMechLutRead(lutName,&coverTbl);

     if (status == CAD_ACCEPT)
		 status = seqNameMatch(mechNames, &nfound,&coverTbl,&coverLutPtr);


     /* Copy out the array of names to the output fields VALA..VALL */
	 seqLoadNames(pgsub,mechNames,nfound);
    
     return status;
}

/******************************************************************************/

/*+
 *   Function name:
 *   seqdeckerNames
 *
 *   Purpose:
 *   Read the  definition files and output the  names
 *    on the gensub record output links
 *
 *   Description:
 *   Call the routines to read the files and define the two lookup tables.
 *   Output 12 strings on gensub output links A..L. These are intended
 *   to be used to set 12 MBBI record state strings (ZRST,..,ELST)
 *
 *   Invocation:
 *   seqdeckerNames(wheelnum, pgsub)
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *      (!)   pgsub  (struct genSubRecord *pgsub)  Pointer to gensub structure
 *      
 *
 *   EPICS input parameters:
 *     
 *      j => lut file path
 *      l => all positions lut file name (string)
 *      n =>  lut file name (string)
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

long seqdeckerNames(struct genSubRecord *pgsub)
{

     char mechNames[MAXMENU][LUT_TAG_SZ] = {""}; /* Filter name list */
     long status = CAD_ACCEPT;
     char lutName[STRING_BUF_SZ];
     int  nfound = 0;
/*decker lut*/
		strncpy (lutName, (char *)pgsub->j, HALF_BUF_SZ);
		strcat  (lutName, "/");
		strncat (lutName,(char *)pgsub->n , HALF_BUF_SZ-2);
		
		status = nirsLutRead(lutName,&deckerLutPtr);
	/* 	printNirs(deckerLutPtr); */
		if (status != CAD_ACCEPT)
		{
		/* 	strncpy(CADMESS, "Error initializing Mask database", */
/* 					MAX_STRING_SIZE-1); */
			DBGMSG(DBG_MIN,"gmSeqCadInit: Error initialising Mask database");
		}
		else
		{
			DBGMSG(DBG_MIN,"Initialized mask database OK");
		}
     /* Read the file of filter names with barcodes */
     strncpy (lutName, (char *)pgsub->j, HALF_BUF_SZ);
     strcat  (lutName, "/");
     strncat (lutName, (char *)pgsub->l, HALF_BUF_SZ-2);

	 /*read lut file and fill deckerTbl */

     status = seqMechLutRead(lutName,&deckerTbl);
	/*  printMech(deckerTbl); */
     if (status == CAD_ACCEPT)
		 status = seqNameMatch( mechNames, &nfound,&deckerTbl,&deckerLutPtr);


     /* Copy out the array of names to the output fields VALA..VALL */
	 seqLoadNames(pgsub,mechNames,nfound);
    
     return status;
}

/******************************************************************************/

/*+
 *   Function name:
 *   seqfocusNames
 *
 *   Purpose:
 *   Read the  definition files and output the  names
 *    on the gensub record output links
 *
 *   Description:
 *   Call the routines to read the files and define the two lookup tables.
 *   Output 12 strings on gensub output links A..L. These are intended
 *   to be used to set 12 MBBI record state strings (ZRST,..,ELST)
 *
 *   Invocation:
 *   seqfocusNames(wheelnum, pgsub)
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *      (!)   pgsub  (struct genSubRecord *pgsub)  Pointer to gensub structure
 *      
 *
 *   EPICS input parameters:
 *     
 *      j => lut file path
 *      l => all positions lut file name (string)
 *      n => lut file name (string)
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

long seqfocusNames(struct genSubRecord *pgsub)
{

     char mechNames[MAXMENU][LUT_TAG_SZ] = {""}; /* Filter name list */
     long status = CAD_ACCEPT;
     char lutName[STRING_BUF_SZ];
     int  nfound = 0;
	/* focus lut*/	
		strncpy (lutName, (char *)pgsub->j, HALF_BUF_SZ);
		strcat  (lutName, "/");
		strncat (lutName, (char *)pgsub->n, HALF_BUF_SZ-2);
		
		status = nirsLutRead(lutName,&focusLutPtr);
		if (status != CAD_ACCEPT)
		{
		/* 	strncpy(CADMESS, "Error initializing Mask database", */
/* 					MAX_STRING_SIZE-1); */
			DBGMSG(DBG_MIN,"gmSeqCadInit: Error initialising Mask database");
		}
		else
		{
			DBGMSG(DBG_MIN,"Initialized mask database OK");
		}
     /* Read the file of filter names with barcodes */
     strncpy (lutName, (char *)pgsub->j, HALF_BUF_SZ);
     strcat  (lutName, "/");
     strncat (lutName, (char *)pgsub->l, HALF_BUF_SZ-2);

	 /*read lut file and fill focusTbl */

     status = seqMechLutRead(lutName,&focusTbl);

     if (status == CAD_ACCEPT)
		 status = seqNameMatch( mechNames, &nfound,&focusTbl,&focusLutPtr);


     /* Copy out the array of names to the output fields VALA..VALL */
	 seqLoadNames(pgsub,mechNames,nfound);
    
     return status;
}
#endif
/******************************************************************************/

/*+
 *   Function name:
 *   seqfw2Names
 *
 *   Purpose:
 *   Read the  definition files and output ther names
 *    on the gensub record output links
 *
 *   Description:
 *   Call the routines to read the files and define the two lookup tables.
 *   Output 12 strings on gensub output links A..L. These are intended
 *   to be used to set 12 MBBI record state strings (ZRST,..,ELST)
 *
 *   Invocation:
 *   seqfw2Names(wheelnum, pgsub)
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *      (!)   pgsub  (struct genSubRecord *pgsub)  Pointer to gensub structure
 *      
 *
 *   EPICS input parameters:
 *      j => lut file path
 *      l => all filters lut file name (string)
 *      n => filter wheel lut file name (string)
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

long seqfw2Names(struct genSubRecord *pgsub)
{

     char mechNames[MAXMENU][LUT_TAG_SZ] = {""}; /* Filter name list */
     long status = CAD_ACCEPT;
     char lutName[STRING_BUF_SZ];
     int  nfound = 0;
	/*fw2 lut*/
		strncpy (lutName, (char *)pgsub->j, HALF_BUF_SZ);
		strcat  (lutName, "/");
		strncat (lutName, (char *)pgsub->n, HALF_BUF_SZ-2);

		status = nirsLutRead(lutName,&fw2LutPtr);
		if (status != CAD_ACCEPT)
		{
		/* 	strncpy(CADMESS, "Error initializing Filter database", */
/* 					MAX_STRING_SIZE-1); */
			DBGMSG(DBG_MIN,"gmSeqCadInit: Error initialising Filter database");
		}
		else
		{
			DBGMSG(DBG_MIN,"Initialized filter database OK");
		}
     /* Read the file of filter names with barcodes */
     strncpy (lutName, (char *)pgsub->j, HALF_BUF_SZ);
     strcat  (lutName, "/");
     strncat (lutName, (char *)pgsub->l, HALF_BUF_SZ-2);

	 /*read lut file and fill fw2Tbl */

     status = seqMechLutRead(lutName,&fw2Tbl);
/* 	 printMech(fw2Tbl); */
     if (status == CAD_ACCEPT)
		 status = seqNameMatch( mechNames, &nfound,&fw2Tbl,&fw2LutPtr);


     /* Copy out the array of names to the output fields VALA..VALL */
	 seqLoadNames(pgsub,mechNames,nfound);
    
     return status;
}
#if 0
/******************************************************************************/

/*+
 *   Function name:
 *   seqgratingNames
 *
 *   Purpose:
 *   Read the  definition files and output the  names
 *    on the gensub record output links
 *
 *   Description:
 *   Call the routines to read the files and define the two lookup tables.
 *   Output 12 strings on gensub output links A..L. These are intended
 *   to be used to set 12 MBBI record state strings (ZRST,..,ELST)
 *
 *   Invocation:
 *   seqgratingNames(wheelnum, pgsub)
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *      (!)   pgsub  (struct genSubRecord *pgsub)  Pointer to gensub structure
 *      
 *
 *   EPICS input parameters:
 *      j => lut file path
 *      l => all positions lut file name (string)
 *      n => lut file name (string)
 *      
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

long seqgratingNames(struct genSubRecord *pgsub)
{

     char mechNames[MAXMENU][LUT_TAG_SZ] = {""}; /* Filter name list */
     long status = CAD_ACCEPT;
     char lutName[STRING_BUF_SZ];
     int  nfound = 0;
	/*grating lut*/
		strncpy (lutName, (char *)pgsub->j, HALF_BUF_SZ);
		strcat  (lutName, "/");
		strncat (lutName, (char *)pgsub->n, HALF_BUF_SZ-2);

		status = nirsLutRead(lutName,&gratingLutPtr);
		if (status != CAD_ACCEPT)
		{
		/* 	strncpy(CADMESS, "Error initializing Filter database", */
/* 					MAX_STRING_SIZE-1); */
			DBGMSG(DBG_MIN,"gmSeqCadInit: Error initialising grating database");
		}
		else
		{
			DBGMSG(DBG_MIN,"Initialized grating database OK");
		}
     /* Read the file of filter names with barcodes */
     strncpy (lutName, (char *)pgsub->j, HALF_BUF_SZ);
     strcat  (lutName, "/");
     strncat (lutName, (char *)pgsub->l, HALF_BUF_SZ-2);

	 /*read lut file and fill gratingTbl */

     status = seqMechLutRead(lutName,&gratingTbl);

     if (status == CAD_ACCEPT)
		 status = seqNameMatch( mechNames, &nfound,&gratingTbl,&gratingLutPtr);
	 printNirs(gratingLutPtr);
	 printMech(gratingTbl);

     /* Copy out the array of names to the output fields VALA..VALL */
	 seqLoadNames(pgsub,mechNames,nfound);
    
     return status;
}

/******************************************************************************/

/*+
 *   Function name:
 *   seqslitNames
 *
 *   Purpose:
 *   Read the  definition files and output the  names
 *    on the gensub record output links
 *
 *   Description:
 *   Call the routines to read the files and define the two lookup tables.
 *   Output 12 strings on gensub output links A..L. These are intended
 *   to be used to set 12 MBBI record state strings (ZRST,..,ELST)
 *
 *   Invocation:
 *   seqslitNames(wheelnum, pgsub)
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *      (!)   pgsub  (struct genSubRecord *pgsub)  Pointer to gensub structure
 *      
 *
 *   EPICS input parameters:
 *     
 *      j => lut file path
 *      l => all positions lut file name (string)
 *      n => lut file name (string)
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

long seqslitNames(struct genSubRecord *pgsub)
{

     char mechNames[MAXMENU][LUT_TAG_SZ] = {""}; /* Filter name list */
     long status = CAD_ACCEPT;
     char lutName[STRING_BUF_SZ];
     int  nfound = 0;
	/*slit lut*/
		strncpy (lutName, (char *)pgsub->j, HALF_BUF_SZ);
		strcat  (lutName, "/");
		strncat (lutName, (char *)pgsub->n, HALF_BUF_SZ-2);

		status = nirsLutRead(lutName,&slitLutPtr);
		if (status != CAD_ACCEPT)
		{
		/* 	strncpy(CADMESS, "Error initializing Grating database", */
/* 					MAX_STRING_SIZE-1); */
			DBGMSG(DBG_MIN,"gmSeqCadInit: Error initialising Grating database");
		}
		else
		{
			DBGMSG(DBG_MIN,"Initialized grating database OK");
		}
     /* Read the file of filter names with barcodes */
     strncpy (lutName, (char *)pgsub->j, HALF_BUF_SZ);
     strcat  (lutName, "/");
     strncat (lutName, (char *)pgsub->l, HALF_BUF_SZ-2);

	 /*read lut file and fill slitTbl */

     status = seqMechLutRead(lutName,&slitTbl);

     if (status == CAD_ACCEPT)
		 status = seqNameMatch( mechNames, &nfound,&slitTbl,&slitLutPtr);


     /* Copy out the array of names to the output fields VALA..VALL */
	 seqLoadNames(pgsub,mechNames,nfound);
    
     return status;
}

/******************************************************************************/

/*+
 *   Function name:
 *   seqxdispNames
 *
 *   Purpose:
 *   Read the  definition files and output the  names
 *    on the gensub record output links
 *
 *   Description:
 *   Call the routines to read the files and define the two lookup tables.
 *   Output 12 strings on gensub output links A..L. These are intended
 *   to be used to set 12 MBBI record state strings (ZRST,..,ELST)
 *
 *   Invocation:
 *   seqxdispNames(wheelnum, pgsub)
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *      (!)   pgsub  (struct genSubRecord *pgsub)  Pointer to gensub structure
 *      
 *
 *   EPICS input parameters:
 *      j => lut file path
 *      l => all positions lut file name (string)
 *      n => lut file name (string)
 *     
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

long seqxdispNames(struct genSubRecord *pgsub)
{

     char mechNames[MAXMENU][LUT_TAG_SZ] = {""}; /* Filter name list */
     long status = CAD_ACCEPT;
     char lutName[STRING_BUF_SZ];
     int  nfound = 0;
	/* xdisp*/	
		strncpy (lutName, (char *)pgsub->j, HALF_BUF_SZ);
		strcat  (lutName, "/");
		strncat (lutName,(char *)pgsub->n , HALF_BUF_SZ-2);
		
		status = nirsLutRead(lutName,&xdispLutPtr);
		if (status != CAD_ACCEPT)
		{
		/* 	strncpy(CADMESS, "Error initializing Mask database", */
/* 					MAX_STRING_SIZE-1); */
			DBGMSG(DBG_MIN,"gmSeqCadInit: Error initialising Mask database");
		}
		else
		{
			DBGMSG(DBG_MIN,"Initialized mask database OK");
		}
     /* Read the file of filter names with barcodes */
     strncpy (lutName, (char *)pgsub->j, HALF_BUF_SZ);
     strcat  (lutName, "/");
     strncat (lutName, (char *)pgsub->l, HALF_BUF_SZ-2);

	 /*read lut file and fill xdispTbl */

     status = seqMechLutRead(lutName,&xdispTbl);

     if (status == CAD_ACCEPT)
		 status = seqNameMatch( mechNames, &nfound,&xdispTbl,&xdispLutPtr);


     /* Copy out the array of names to the output fields VALA..VALL */
	 seqLoadNames(pgsub,mechNames,nfound);
    
     return status;
}
#endif
