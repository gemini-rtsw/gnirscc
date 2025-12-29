static struct {void *v; char *c;} rcsid = {&rcsid,
										   "$Id: nirsSeqCadLib.c,v 1.3 2010/05/25 01:55:40 mrippa Exp $"};

/* *INDENT-OFF* */
/*
 *   FILENAME
 *   gmSeqCadLib.c
 *
 *   PURPOSE:
 *   Routines used by NIRS Sequencer CAD records
 *
 *   FUNCTION NAME(S):
 *    nirsLutRead -       Initialise the filter CAD database
 *    gmSeqCadInitFilter -       Initialise the filter CAD
 *    gmSeqCadFilter -           Implements the fltPos CAD command
 *    gratingLutRead -      Initialise the grating database
 *    gmSeqCadInitGrating -      Initialise the grating CAD
 *    seqCadGrating -          Implements the grSelect CAD command
 *    gmSeqCadReboot  -          Null routine called by reboot CAD record
 *    gmSeqReboot  -             Reboot the NIRS Instrument Sequencer
 *    gmSeqCadInit  -            Implements the Sequencer init command
 *    gmSeqCadSetup  -           For various CAD routines involved in setting
 *                                       up NIRS
 *    gmSeqCadDebug -            Implements the Instrument Sequencer debug 
 *                                      command
 *    gmSeqNullInit  -           Null CAD initialisation
 *    gmSeqConfigBegin  -        Implements the start of configuration apply 
 *                                     handling.
 *    gmSeqConfigEnd  -          Implements the end of configuration apply 
 *                                       handling.
 *    gmSeqCadTrivial   -        Routine for trivial NIRS sequence CAD record
 *    gmSeqCadObserve -          Implements the CAD for the 'observe' 
 *                                      sequence command
 *    gmSeqCadStopObserve  -     For sequence CADs: pause/stop an observation
 *    gmSeqCadEndObserve  -      For NIRS sequence CAD endObserve
 *    gmSeqCadContinue  -        For NIRS sequence CAD continue

 *
 *   Copyright Observatory Sciences Ltd. 1999-2000. All rights reserved.
 *   Under contract to the UK Astronomy Technology Centre, who modified the 
 *    code in 2001.
 *
 */
/*
 * $Log: nirsSeqCadLib.c,v $
 * Revision 1.3  2010/05/25 01:55:40  mrippa
 * Mechanism names ported to output.
 *
 * Revision 1.2  2009/05/27 19:33:46  fkraemer
 * fkraemer - copied my complete working dir over trunk
 *
 * Revision 1.50  2001/03/01 14:09:19  nirs
 * Fixed mistake in gmSeqCadDebug where pcad->b was being treated as a long 
 * rather than string. Use (char *) explicitly for CAD string fields.
 *
 * Revision 1.49  2001/02/28 10:36:39  nirs
 * Some printfs reworded. Additional checking of lambdaFocus lookup table.
 *
 * Revision 1.48  2001/02/23 13:14:08  nirs
 * Added functions to translate mask names into barcode IDs, based on names 
 * read from masks.lut. Renamed global variables so they begin gmSeq. Made 
 * the filter and mask LUT reading functions more robust.
 *
 * Revision 1.47  2001/02/21 13:34:49  nirs
 * Copyright statement modified.
 *
 * Revision 1.46  2001/02/12 17:59:23  nirs
 * Use effective wavelength specified in grating SEL mode to predict focus 
 * offset.
 *
 * Revision 1.45  2001/02/12 16:56:47  nirs
 * Zero wavelength focus offset when mirror selected. Treat a grating used 
 * at zero order like a mirror.
 *
 * Revision 1.44  2001/02/05 13:07:29  nirs
 * Grating effective wavelength added to SAD. Code prologue tidied up.
 *
 * Revision 1.43  2001/02/01 11:04:16  nirs
 * Major change to the way that filter and grating offsets are handled. 
 * Filters now have surface power and effective wavelength offsets. Gratings 
 * now have a surface power offset in addition to wavelength offset. Debug 
 * command now has option to set only Instrument Sequencer mode.
 *
 * Revision 1.42  2001/01/30 12:00:50  nirs
 * Substantial amount of tidying up. Subtract 1 from strncpy lengths and 
 * format lengths to allow for trailing NULL. Illegal STOP directive now 
 * generates a 
 * message.
 *
 * Revision 1.41  2001/01/29 18:10:06  nirs
 * Format size deficiency commented.
 *
 * Revision 1.40  2001/01/29 18:02:40  nirs
 * LUT_TAG_SZ is 40 characters, not 20 - adjusted formats accordingly. Use
 *  symbolic constants for LUT_TAG_SZ and STRING_BUF_SZ wherever possible.
 *
 * Revision 1.39  2001/01/29 15:02:19  nirs
 * Reading lookup tables could result in buffer overflow. Fixed.
 *
 * Revision 1.38  2001/01/26 10:25:11  nirs
 * Added effective wavelength parameter to grSelect CAD. Tidyied up code
 *  layout in gmSeqGrating function considerably.
 *
 * Revision 1.37  2001/01/24 15:10:42  nirs
 * Added reading of lambdaFocus.lut, after IS lookup table changes.
 *
 * Revision 1.36  2001/01/24 10:41:29  nirs
 * Rework of IS lookup table reading (in work)
 *
 * Revision 1.35  2000/12/19 13:32:44  nirs
 * Allow the DEBUG command to accept the QUIET string. Also do not send MAX 
 * to detector controller.
 *
 * Revision 1.34  2000/12/15 11:35:25  nirs
 * MASTER_ENABLE test condition added. Test for MASTER_ENABLE in all commands
 *  except INIT and TEST.
 *
 * Revision 1.33  2000/10/25 16:18:01  pbt
 * Initial filter/grating names now "unknown"
 *
 * Revision 1.32  2000/10/25 09:24:10  nirs
 * Load the lookup table file names from gensub database fields rather than 
 * hard-wired names. Cad records decide the names from the database prefix.
 *
 * Revision 1.31  2000/10/24 11:34:43  nirs
 * Modified to reject OBSERVE, PAUSE, CONTINUE, STOP and ABORT commands when 
 * detector controller disconnected.
 *
 * Revision 1.30  2000/10/17 13:43:40  nirs
 * Lookup tables moved from data to /gemini/nirs directory.
 *
 * Revision 1.29  2000/09/26 13:55:14  nirs
 * Supressed superflous printf statements.
 *
 * Revision 1.28  2000/09/22 08:21:48  nirs
 * Parameters used to define a universal wavelength range of 300 to 1000 
 * nanometres.
 *
 * Revision 1.27  2000/09/20 14:05:45  nirs
 * Fixed mistake in CVS variables Log and Id
 *
 */
/* *INDENT-ON* */

#include  <vxWorks.h>
#include  <stdlib.h>
#include  <types.h>
#include  <stdioLib.h>
#include  <lstLib.h>
#include  <strLib.h>
#include  <taskLib.h>
#include  <rebootLib.h>
#include  <sysLib.h>

#include  <stdio.h>
#include  <string.h>
#include  <math.h>

#include  <dbDefs.h>
#include  <dbEvent.h>
#include  <genSubRecord.h>
#include  <cadRecord.h>
#include  <subRecord.h>
#include  <dbCommon.h>
#include  <recSup.h>
#include  <cad.h>
#include  <car.h>
#include  <subCadRecord.h>

#include "nirsSeq.h"
#include "nirsLutLib.h"
#include "mechNames.h"

#define DIRECTIVE pcad->dir
/* #define VERBOSE */    /* Define to enable extra printf statements */

/*
 * NOTE: The global pointers declared below are shared with 
 * gmSeqDisplayMenus.c
 */

/* void *gmSeqGratLutPtr = NULL;  */  /* Global grating lookup table pointer */
static int gratingInitOK;       /* If grating LUT table read OK */

void *fw1LutPtr = NULL;         /* Global filter lookup table pointer  */
void *fw2LutPtr = NULL;         /* Global filter lookup table pointer  */
/* void *focusLutPtr = NULL;  */      /* Global filter lookup table pointer  */
/* void *acqLutPtr = NULL;  */        /* Global filter lookup table pointer  */
/* void *deckerLutPtr = NULL;  */     /* Global filter lookup table pointer  */
/* void *cameraLutPtr = NULL;  */     /* Global filter lookup table pointer  */
/* void *coverLutPtr = NULL;  */      /* Global filter lookup table pointer  */
/* void *gratingLutPtr = NULL;  */    /* Global filter lookup table pointer  */
/* void *xdispLutPtr = NULL;    */    /* Global filter lookup table pointer  */
/* void *slitLutPtr = NULL;  */       /* Global filter lookup table pointer  */
static int filterInitOK;        /* If filter LUT table read OK */

void *gmSeqMaskLutPtr = NULL;   /* Global mask lookup table pointer  */
/* static int maskInitOK;  */   /* If mask LUT table read OK */

/* static nirsLookupTable lambdaFocusLut;*//* Wavelength vs. focus offset LUT data      */

int gmSeqDbgLevel = DBG_NONE;    /* Global debug level flag, initialised to DBG_NONE */

/* Function definitions */

long gmSeqMaskLUTread( char * lutfilename );
long nirsLutRead( char * lutfilename, void **lutPtr );
/* long gratingLutRead( char * lutfilename ); */

/**************************************************************************/


/*+
 *   Function name:
 *   nirsLutRead
 *
 *   Purpose:
 *   Initialise the filter CAD database
 *
 *   Description:
 *   Read the filter LUT data file 
 *   and set up an internal list of filter names with their 
 *   attributes (barcode and focus offset)
 *
 *   Invocation:
 *   nirsLutRead( char * lutfilename , void *lutPtr)
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *   (>)  lutfilename (string) name of filters lut file
 *
 *   Function value:
 *   (<)  status  (long) Return status, 0 = OK
 *                       Error if file not opened or missing data values
 * 
 *   External functions:
 *   (VxWorks lstLib routines) lstInit, lstAdd
 *
 *-
 */

long nirsLutRead( char * lutfilename, void **lutPtr)
{
	int num;
    FILE *fp;                /* LUT data file */
    char buf[STRING_BUF_SZ]; /* Input buffer */
    char tag[LUT_TAG_SZ];    /* Tag (= filter) name */
    NIRSLUT *p;              /* Look-up table data structure */
    long n;                  /* Status return for fscanf */
    long lcount;             /* Input line count */
    long status;             /* Return status */

    status = CAD_ACCEPT;
    filterInitOK = FALSE;

	/* Free up old list if this is not the first entry */

    if (*lutPtr != NULL) 
    {
		lstFree((LIST *) *lutPtr);
		free(*lutPtr);
		*lutPtr = NULL;
    }    
 
	/* read data file from remote disk */
	printf("open %s\n",lutfilename);
    if ((fp = fopen (lutfilename, "r")) == NULL)
    {
		printf("nirsLutRead: Failed to open file %s\n", lutfilename);
		return CAD_REJECT;
    }

    DBGMSGSTRING(DBG_MIN,"nirsLutRead: opened filter lut file: ", lutfilename);
    
    *lutPtr = (void *) malloc (sizeof (LIST));
    if (*lutPtr == NULL)    /* malloc failed */
    {
        printf("nirsLutRead: LUT malloc failed\n");
        fclose(fp);
        return CAD_REJECT;
    }

    lstInit ((LIST *) *lutPtr);
    
    lcount = 0;
    while (status == CAD_ACCEPT)
    {

		/* skip blank lines and comments */

		n = fscanf (fp, "%39s", tag);/* Format size should be LUT_TAG_SZ-1 */
		lcount++;
		if (n == 0 || tag[0] == '#')
		{
			/* Throw away the rest of a comment line */
			(void) fgets (buf, sizeof (buf)-1, fp);
		}
		else if (n == EOF)
		{
			/* End of File. Break out of loop. */
			break;
		}
		else
		{

/* 			printf("tag = %s\t",tag); */
			/* create new node, read tag and number of values */

			p = (NIRSLUT *) malloc (sizeof (NIRSLUT));

			if (p == NULL)    /* malloc failed */
			{
				printf("nirsLutRead: LUT node malloc failed\n");
				fclose(fp);
				return CAD_REJECT;
			}

			lstAdd ((LIST *) *lutPtr, (NODE *) p);
			strncpy (p->tag, tag, LUT_TAG_SZ-1);
			if ((num = fscanf(fp,"%d%lf%lf", &(p->barcodeId), 
							  &(p->focusOffset), &(p->effWavelength))) != 3 )
			{
				printf("nirsLutRead: Missing value(s) in %s line %d count = %d\n", 
					   lutfilename, lcount,num);
				status = CAD_REJECT;
			}
			else
			{
			/* 	printf("%d, %f, %f,    OK \n",p->barcodeId,p->focusOffset,p->effWavelength); */
				DBGMSGINT(DBG_FULL,"nirsLutRead: data line read OK: ",lcount); 
			}
		} 
    }
    fclose (fp);
    if (status == CAD_ACCEPT) filterInitOK = TRUE;
    
    return status;
}

/*+
 *   Function name:
 *   gmSeqCadInitFilter 
 *
 *   Purpose:
 *   Initialise the filter CAD 
 *
 *   Description:
 *   Called at EPICS iocInit. Calls nirsLutRead
 *
 *   Invocation:
 *   gmSeqCadInitFilter(pcad)
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *      (!)   pcad   (struct cadRecord*)  Pointer to CAD record structure
 *
 *   Function value:
 *   (<)  status  (long) Return status, 0 = OK
 *                       Error if file not opened or missing data values
 * 
 *   External functions:
 *   (VxWorks lstLib routines) lstInit, lstAdd
 *
 *-
 */

long gmSeqCadInitFw1(struct cadRecord *pcad)
{ 
    long status = CAD_ACCEPT;

    return status;
}

/*+
 *   Function name:
 *   gmSeqCadFw1 
 *
 *   Purpose:
 *   Implements the fltPos command
 *
 *   Description:
 *   This routine is the process subroutine for the fltPos CAD.
 *   It translates the input filter name by looking up the name in
 *   the filter look-up table, The resulting barcode ID is written to the
 *   NIRS Components Controller fltAssembly record.
 *
 *   Invocation:
 *   gmSeqCadFilter (pcad)
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *      (!)   pcad   (struct cadRecord*)  Pointer to CAD record structure
 *
 *   EPICS input parameters:
 *      a => Filter name 1 as a text string
 *
 *   EPICS output parameters:
 *      valg => translated Barcode ID for filter 1 (string)
 *      valc => Focus offset due to filter 1 surface curvature (double)
 *      vale => Focus offset due to filter 1 effective wavelength (double)
 *      vala => Filter name 1 (string)
 *
 *   Function value:
 *   (<)  status  (long) Return status, 0 = OK
 * 
 *   External functions:
 *
 *-
 */
long gmSeqCadFw1(struct cadRecord *pcad)
{ 
 
    long status;           /* Return status */
    NIRSLUT *p;
    char buff[STRING_BUF_SZ];
    static char fname[MAX_STRING_SIZE] = "unknown";
    static char barcodeId[MAX_STRING_SIZE];
    static double focusOffset;
    static double lambdaOffset;
    double lambda;

    status = CAD_REJECT ;
     
    DBGMSGINT(DBG_MIN,"CAD fltPos. Directive = ", DIRECTIVE);
    
    switch (DIRECTIVE)
    {

	  case CAD_PRESET:

		DBGMSG(DBG_FULL,"gmSeqCadFw1 PRESET ");
		if (!filterInitOK) 
		{
			strncpy(CADMESS, "Filter name translation uninitialized",
					MAX_STRING_SIZE-1);
			return CAD_REJECT;
		}

		/* Set the appropriate global test conditions for this CAD */
		gmSeqSetCADTest(OBSERVING+MASTER_ENABLE);
    		strncpy(fname, (char *)pcad->b, MAX_STRING_SIZE-1);
	/* 	printf("fname = %s\n",fname); */
		if (fname[0])
		{

			/*
			 * Try to translate the 1st input filter name. Read through all 
			 * filter lookup table entries, searching for specified name.
			 * The search will stop immediately if fw1LutPtr is NULL.
			 */

			for (p = (NIRSLUT *) lstFirst ((LIST *)fw1LutPtr); p != NULL;
				 p = (NIRSLUT *) lstNext ((NODE *) p))
			{
			/* 	printf("name = %s, %s\n",fname,p->tag); */
                if (strncmp (p->tag, fname, LUT_TAG_SZ) == 0)
                {
					sprintf(barcodeId, "%10d%1c", p->barcodeId,'\0');
					focusOffset = p->focusOffset;
					lambda = p->effWavelength;
					status = CAD_ACCEPT;
					break;  /* Breaks out of "for" statement, not "case". */
                }
			}
		}

		if (status == CAD_REJECT)
		{
			strncpy(buff, "Unknown fw1 name (A): ", MAX_STRING_SIZE-1);
			strncat(buff, fname, MAX_STRING_SIZE - strlen(buff));
			strncpy(CADMESS, buff, MAX_STRING_SIZE-1);
			return(status);
		}

	

	

		/*
		 * If the filters have a non-zero effective wavelength determine 
		 * the focus offset corresponding to that wavelength. This offset 
		 * will be used when NIRS is used for imaging. (If the call to 
		 * nirsLutApply fails, don't exit, just set offset focus value 
		 * to zero.)
		 */

		if ( lambda > 0.0 )
		{
#if 0
			ret = nirsLutApply(lambda, &lambdaFocusLut, &lambdaOffset);
			if (ret != NIRSLUT_S_OK)
			{
                nirsLutMessage(ret, STRING_BUF_SZ, buff);
                lambdaOffset = 0.0;
			}
#endif
		}
		else
		{
			lambdaOffset = 0.0;
		}

	

		/* Output the name for further checking by CC */

		strncpy((char *)pcad->vala, fname, MAX_STRING_SIZE-1);
		break ;

	  case CAD_START:

		DBGMSG(DBG_FULL,"gmSeqCadFw1 START ");
		/* Filters are OK, so output focus offsets and barcode*/

		*(double *) pcad->valc = focusOffset;
		*(double *) pcad->vale = lambdaOffset;
		strncpy(pcad->valg, barcodeId, MAX_STRING_SIZE-1);
		status = CAD_ACCEPT;
   
		DBGMSG(DBG_FULL,"CAD fltPos. Output values A..H:");
		DBGMSGSTRING(DBG_FULL,"A: ", (char *)pcad->vala);
		DBGMSGREAL(DBG_FULL,"C: ",*(double *)pcad->valc);
		DBGMSGREAL(DBG_FULL,"E: ",*(double *)pcad->vale);
		DBGMSGSTRING(DBG_FULL,"G: ",pcad->valg);
   
		break;

	  case CAD_MARK :

		status = CAD_ACCEPT;
		break ;

	  case CAD_STOP :

		status = CAD_REJECT;
		strncpy(CADMESS, "STOP directive not implemented", MAX_STRING_SIZE-1);
		break ;

	  case CAD_CLEAR :

		status = CAD_ACCEPT;
		break ;
	}

	return(status);
}

/*+
 *   Function name:
 *   gmSeqCadInitFw2
 *
 *   Purpose:
 *   Initialise the filter CAD 
 *
 *   Description:
 *   Called at EPICS iocInit. Calls nirsLutRead
 *
 *   Invocation:
 *   gmSeqCadInitFilter(pcad)
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *      (!)   pcad   (struct cadRecord*)  Pointer to CAD record structure
 *
 *   Function value:
 *   (<)  status  (long) Return status, 0 = OK
 *                       Error if file not opened or missing data values
 * 
 *   External functions:
 *   (VxWorks lstLib routines) lstInit, lstAdd
 *
 *-
 */

long gmSeqCadInitFw2(struct cadRecord *pcad)
{ 
    long status = CAD_ACCEPT;

    return status;
}

/*+
 *   Function name:
 *   gmSeqCadFilter 
 *
 *   Purpose:
 *   Implements the fltPos command
 *
 *   Description:
 *   This routine is the process subroutine for the fltPos CAD.
 *   It translates the input filter name by looking up the name in
 *   the filter look-up table, The resulting barcode ID is written to the
 *   NIRS Components Controller fltAssembly record.
 *
 *   Invocation:
 *   gmSeqCadFilter (pcad)
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *      (!)   pcad   (struct cadRecord*)  Pointer to CAD record structure
 *
 *   EPICS input parameters:
 *      a => Filter name 1 as a text string
 *
 *   EPICS output parameters:
 *      valg => translated Barcode ID for filter 1 (string)
 *      valc => Focus offset due to filter 1 surface curvature (double)
 *      vale => Focus offset due to filter 1 effective wavelength (double)
 *      vala => Filter name 1 (string)
 *
 *   Function value:
 *   (<)  status  (long) Return status, 0 = OK
 * 
 *   External functions:
 *
 *-
 */
long gmSeqCadFw2(struct cadRecord *pcad)
{ 
 
    long status;           /* Return status */
    NIRSLUT *p;
    char buff[STRING_BUF_SZ];
    static char fname[MAX_STRING_SIZE] = "unknown";
    static char barcodeId[MAX_STRING_SIZE];
    static double focusOffset;
    static double lambdaOffset;
    double lambda;

    status = CAD_REJECT ;
     
    DBGMSGINT(DBG_MIN,"CAD fltPos. Directive = ", DIRECTIVE);
    
    switch (DIRECTIVE)
    {

	  case CAD_PRESET:

		DBGMSG(DBG_FULL,"gmSeqCadFw2 PRESET ");
		if (!filterInitOK) 
		{
			strncpy(CADMESS, "Filter name translation uninitialized",
					MAX_STRING_SIZE-1);
			return CAD_REJECT;
		}

		/* Set the appropriate global test conditions for this CAD */

		gmSeqSetCADTest(OBSERVING+MASTER_ENABLE);
    
		strncpy(fname, (char *)pcad->b, MAX_STRING_SIZE-1);
		if (fname[0])
		{

			/*
			 * Try to translate the  input filter name. Read through all
			 * filter lookup table entries, searching for specified name. 
			 * The search will stop immediately if fw2LutPtr is NULL.
			 */

			for (p = (NIRSLUT *) lstFirst ((LIST *)fw2LutPtr); p != NULL;
				 p = (NIRSLUT *) lstNext ((NODE *) p))
			{
                if (strncmp (p->tag, fname, LUT_TAG_SZ) == 0)
                {
					sprintf(barcodeId, "%10d%1c", p->barcodeId,'\0');
					focusOffset = p->focusOffset;
					lambda = p->effWavelength;
					status = CAD_ACCEPT;
					break;     /* Breaks out of "for" statement, not "case".*/
                }
			}
		}

		if (status == CAD_REJECT)
		{
			strncpy(buff, "Unknown fw2 name (A): ", MAX_STRING_SIZE-1);
			strncat(buff, fname, MAX_STRING_SIZE - strlen(buff));
			strncpy(CADMESS, buff, MAX_STRING_SIZE-1);
			return(status);
		}

	
		/*
		 * If the filters have a non-zero effective wavelength determine the focus offset
		 * corresponding to that wavelength. This offset will be used when NIRS is
		 * used for imaging. (If the call to nirsLutApply fails, don't exit, just
		 * set offset focus value to zero.)
		 */

		if ( lambda > 0.0 )
		{
#if 0
			ret = nirsLutApply(lambda, &lambdaFocusLut, &lambdaOffset);
			if (ret != NIRSLUT_S_OK)
			{
                nirsLutMessage(ret, STRING_BUF_SZ, buff);
                lambdaOffset = 0.0;
			}
#endif
		}
		else
		{
			lambdaOffset = 0.0;
		}

	

		/* Output the barcode values for further checking by CC */

		strncpy((char *)pcad->vala, fname, MAX_STRING_SIZE-1);
		strncpy((char *)pcad->valh, fname, MAX_STRING_SIZE-1);
		break ;

	  case CAD_START:

		DBGMSG(DBG_FULL,"gmSeqCadFw1 START ");
		/* Filters are OK, so output focus offsets and original filter names */

		*(double *) pcad->valc = focusOffset;
		*(double *) pcad->vale = lambdaOffset;
		strncpy(pcad->valg, barcodeId, MAX_STRING_SIZE-1);
		status = CAD_ACCEPT;
   
		DBGMSG(DBG_FULL,"CAD fltPos. Output values A..H:");
		DBGMSGSTRING(DBG_FULL,"A: ", (char *)pcad->vala);
		DBGMSGREAL(DBG_FULL,"C: ",*(double *)pcad->valc);
		DBGMSGREAL(DBG_FULL,"E: ",*(double *)pcad->vale);
		DBGMSGSTRING(DBG_FULL,"G: ",pcad->valg);
		DBGMSGSTRING(DBG_FULL,"H: ", (char *)pcad->valh);
   
		break;

	  case CAD_MARK :

		status = CAD_ACCEPT;
		break ;

	  case CAD_STOP :

		status = CAD_REJECT;
		strncpy(CADMESS, "STOP directive not implemented", MAX_STRING_SIZE-1);
		break ;

	  case CAD_CLEAR :

		status = CAD_ACCEPT;
		break ;
	}

	return(status);
}
/******************************************************************************/


/******************************************************************************/
#if 0
/*+
 *   Function name:
 *   gratingLutRead 
 *
 *   Purpose:
 *   Initialise the grating database
 *
 *   Description:
 *   Read the grating LUT data 
 *   file and set up an internal list of grating names with their 
 *   attributes (barcode, ruling density and blaze orientation)
 *
 *   Invocation:
 *   gratingLutRead(  char * lutfilename )
 *
 *   Parameters: (">" input, "!" modified, "<" output) 
 *   (>) lutfilename (string) Name of lut file 
 *
 *   Function value:
 *   (<)  status  (long) Return status, 0 = OK
 *                       Error if file not opened or missing data values
 * 
 *   External functions:
 *   (VxWorks lstLib routines) lstInit, lstAdd
 *
 *-
 */

long gratingLutRead( char * lutfilename )
{
    FILE *fp;                /* LUT data file */
    char buf[STRING_BUF_SZ]; /* Input buffer */
    char tag[LUT_TAG_SZ];    /* Tag (= grating) name */
    NIRSLUT *p;              /* Look-up table data structure */
    long n;
    long lcount;             /* Input line count */
    long status;             /* Return status */

    status = CAD_ACCEPT;
    gratingInitOK = FALSE;

	/* Free up old list if this is not the first entry */

    if (gratingLutPtr != NULL) 
    {
		lstFree((LIST *) gratingLutPtr);
		free(gratingLutPtr);
		gratingLutPtr = NULL;
    }
    
	/* Read data file from remote disk */

    if ((fp = fopen (lutfilename, "r")) == NULL)
    {
        printf("gratingLutRead: Failed to open file %s\n", lutfilename);
        return CAD_REJECT;
    }

    DBGMSGSTRING(DBG_MIN,"gratingLutRead: Opened grating data file: ", lutfilename);
    
    gratingLutPtr = (void *) malloc (sizeof (LIST));
    if (gratingLutPtr == NULL)    /* malloc failed */
    {
        printf("gratingLutRead: LUT malloc failed\n");
        fclose(fp);
        return CAD_REJECT;
    }

    lstInit ((LIST *) gratingLutPtr);
    
    lcount = 0;
    while (status == 0)
    {

		/* skip blank lines and comments */

		n = fscanf (fp, "%39s", tag);     /* Format size should be LUT_TAG_SZ-1 */
		lcount++;
		if (n == 0 || tag[0] == '#')
		{
			(void) fgets (buf, sizeof (buf)-1, fp);
		}
		else if (n == EOF)
		{
			/* End of File. Break out of loop. */
			break;
		}
		else
		{
			/* create new node, read tag and number of values */

			p = (NIRSLUT *) malloc (sizeof (NIRSLUT));
			if (p == NULL)    /* malloc failed */
			{
				printf("gratingLutRead: LUT node malloc failed\n");
				fclose(fp);
				return CAD_REJECT;
			}

			lstAdd ((LIST *) gratingLutPtr, (NODE *) p);
			strncpy (p->tag, tag, LUT_TAG_SZ-1);
			if (fscanf(fp,"%d%d%d%lf", &(p->barcodeId), 
					   &(p->linesPerMm), &(p->blazeDir), &(p->focusOffset)) != 4 )
			{
				printf("gratingLutRead: Missing value(s) in %s line %d\n",
					   lutfilename, lcount);
				status = CAD_REJECT;
			}
			else
			{
				DBGMSGINT(DBG_FULL,"gratingLutRead: Data line read OK: ",lcount);
			}
        
		}
    }

    fclose (fp);
    if (status == CAD_ACCEPT) gratingInitOK = TRUE;

    return status;
}
#endif

/*+
 *   Function name:
 *   gmSeqCadInitGrating 
 *
 *   Purpose:
 *   Initialise the grating CAD 
 *
 *   Description:
 *   Called at EPICS iocInit. Calls gratingLutRead
 *
 *   Invocation:
 *   gmSeqCadInitGrating(pcad)
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *      (!)   pcad   (struct cadRecord*)  Pointer to CAD record structure
 *
 *   Function value:
 *   (<)  status  (long) Return status, 0 = OK
 *                       Error if file not opened or missing data values
 *
 *   External functions:
 *   (VxWorks lstLib routines) lstInit, lstAdd
 *
 *-
 */

long seqCadInitGrating(struct cadRecord *pcad)
{ 
    long status = CAD_ACCEPT;
	gratingInitOK = 1;
#ifdef VERBOSE    
    printf("Called seqCadInitGrating. DIR = %d\n", DIRECTIVE);
#endif

    return status;
}

/*+
 *   Function name:
 *   seqCadGrating 
 *
 *   Purpose:
 *   Implements the grSelect command
 *
 *   Description:
 *   This routine is the process subroutine for the grSelect CAD.
 *   It translates the input grating name by looking up the name in
 *   the grating look-up table. The resuting barcode ID is written to the
 *   NIRS Components Controller grAssembly record. The specified 
 *   central wavelength and order is converted into a grating tilt angle.
 *
 *   Invocation:
 *   seqCadGrating (pcad)
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *      (!)   pcad   (struct cadRecord*)  Pointer to CAD record structure
 *
 *   EPICS input parameters:
 *      a => Grating positioning mode as a string :
 *             tilt
 *             wavelength/order
 *             wavelength
 *      b => Grating name as a string
 *      c => Central wavelength (nanometres) (double)
 *      d => Grating order (integer)
 *      e => Effective wavelength for focus offset (nanometres) (double)
 *
 *   EPICS output parameters:
 *      vala => Selection mode (long) :
 *               0 = Select grating and set angle
 *               1 = Select grating only
 *      valb => Name of grating (string
 *      valc => Desired central wavelength in nanometres (double)
 *      vald => Requested grating order (long)
 *      vale => Grating tilt angle in rads.  (double)
 *      
 *      valg => Grating ruling density (long)
 *      valh => Grating blaze orientation (long)
 *      vali => Focus offset corresponding to wavelength in microns (double)
 *      valj => Focus offset due to grating surface curvature in microns (double)
 *      valk => Desired effective wavelength in nanometres (double)
 *
 *
 *   Function value:
 *   (<)  status  (long) Return status, 0 = OK
 * 
 *   External functions:
 *
 *   Deficiencies:
 *   The "select only" mode will not necessarily get the grating focus offset right,
 *   since the wavelength that the grating was originally selected with is not known
 *   to the instrument sequencer. Instead the user can supply an effective wavelength.
 *-
 */
#define MODE a
#define NAME b
#define FOCOUTNAME n
#define WAVELENGTH c
#define ORDER d
#define ANGLE e

long seqCadGrating(struct cadRecord *pcad)
{ 
 
    long status;                   /* Return status */
   /*  NIRSLUT *p; */
	char err[160];
    char buff[STRING_BUF_SZ];
    static char selectModeIn[MAX_STRING_SIZE];
    static char gratingName[MAX_STRING_SIZE] = "unknown";
  /*   static double cenWavelength; */  /* Central wavelength for grating tilt   */
    static double effWavelength;  /* Effective wavelength for focus offset */
    static double lambdaOffset;   /* Focus offset interpolated from LUT    */
    static double focusOffset;    /* Focus offset for this grating         */
    static long gratingOrder;     /* Grating order required                */
  /*   char wavelengthString[MAX_STRING_SIZE]; */
 /*    char gratingOrderString[MAX_STRING_SIZE];     */
    static long selectModeOut;
 /*    static char barcodeId[ MAX_STRING_SIZE]; */
 /*    static double tiltAngle; */
    static long linesPerMm;
    static long blazeDir;
  /*   static long isMirror = FALSE; */

    status = CAD_ACCEPT;
        
    DBGMSGINT(DBG_MIN,"CAD grSelect. Directive = ", DIRECTIVE);

    switch (DIRECTIVE) 
    {

	  case CAD_PRESET:

		DBGMSG(DBG_FULL,"gmSeqCadGrating PRESET ");
		if (!gratingInitOK) 
		{
			strncpy(CADMESS, "Grating name translation not initialized",
					MAX_STRING_SIZE-1);
			return CAD_REJECT;
		}

		/* Set the appropriate global test conditions for this CAD */

		gmSeqSetCADTest(OBSERVING+MASTER_ENABLE);

		/* Read in the grating selection mode, upper case then convert to integer */

		strncpy(selectModeIn,cadInput(MODE), MAX_STRING_SIZE-1);
		gmSeqUc(selectModeIn, strlen(selectModeIn) + 1, selectModeIn);
        
		if (strcmp(selectModeIn, "TILT") == 0)
		{
			selectModeOut = 0;
		}
		else if (strcmp(selectModeIn, "WAVELENGTH/ORDER") == 0)
		{
			selectModeOut = 1;
		}
		else if (strcmp(selectModeIn, "WAVELENGTH") == 0)
		{
			selectModeOut = 2;
		}
		else
		{
			strncpy(CADMESS,"grSelect mode not tilt, wavelength/order, or wavelength", MAX_STRING_SIZE-1);
			return CAD_REJECT;
		} 
    
		strncpy(gratingName, cadInput(NAME), MAX_STRING_SIZE-1);
	
		if (gratingName[0])
		{
			if(strcmp(gratingName,GRATING1) != 0)
				if(strcmp(gratingName,GRATING2) != 0)
					if(strcmp(gratingName,GRATING3) != 0)
						if(strcmp(gratingName,GRATING4) != 0)
						{
							status = CAD_REJECT;
							sprintf(err,"Grating name must be %s, %s, %s or %s\n",GRATING1,GRATING2,GRATING3,GRATING4);
							printf(err);
							strncpy(err,CADMESS,MAX_STRING_SIZE-1);
							
						}
		}
		else
		{
            strncpy(CADMESS,"Grating name not specified", MAX_STRING_SIZE-1);
            return CAD_REJECT;
		}
    
		if (status == CAD_REJECT)
		{
			strncpy(buff, "Unknown grating name: ", MAX_STRING_SIZE-1);
			strncat(buff, gratingName, MAX_STRING_SIZE - strlen(buff));
			strncpy(CADMESS, buff, MAX_STRING_SIZE-1);
			return(status);
		}

		/*
		 * The named grating is assumed to be a mirror if it is named "MIRROR",
		 * "mirror" or its ruling density is zero.
		 */

		gmSeqUc(gratingName, strlen(gratingName) + 1, gratingName);
	


		if (status == CAD_ACCEPT)
		{

			/*   Output the mode, barcode & tilt angle for checking by CC */
			
			*(long *)cadOutput(ORDER)= 	atoi(cadInput(ORDER));
			*(double *)cadOutput(WAVELENGTH) =atof(cadInput(WAVELENGTH)) ;
			*(long *)cadOutput(MODE) = selectModeOut;
			strncpy((char *)cadOutput(NAME), gratingName, MAX_STRING_SIZE-1);
			strncpy((char *)cadOutput(FOCOUTNAME), gratingName, MAX_STRING_SIZE-1);
			*(double *) cadOutput(ANGLE) =atof(cadInput(ANGLE)) ;
		
		}
		break ;

	  case CAD_START:

		DBGMSG(DBG_FULL,"gmSeqCadGrating START ");
		/* Copy out the input parameters for storing in the SAD etc.*/

		*(long *)pcad->valf = gratingOrder;
		*(long *)pcad->valg = linesPerMm;
		*(long *)pcad->valh = blazeDir;
		*(double *)pcad->vali = lambdaOffset;
		*(double *)pcad->valj = focusOffset;
		*(double *) pcad->valk = effWavelength;

		DBGMSG(DBG_FULL,"CAD grSelect. Output values A..K: ");
		DBGMSGINT(DBG_FULL,"A: ",*(long *)pcad->vala);
		DBGMSGREAL(DBG_FULL,"C: ",*(double *)pcad->valc);
		DBGMSGSTRING(DBG_FULL,"D: ", (char *)pcad->vald); 
		DBGMSGREAL(DBG_FULL,"E: ",*(double *)pcad->vale);
		DBGMSGINT(DBG_FULL,"F: ",*(long *)pcad->valf);
		DBGMSGINT(DBG_FULL,"G: ",*(long *)pcad->valg);
		DBGMSGINT(DBG_FULL,"H: ",*(long *)pcad->valh);
		DBGMSGREAL(DBG_FULL,"I: ",*(double *)pcad->vali);
		DBGMSGREAL(DBG_FULL,"J: ",*(double *)pcad->valj);
		DBGMSGREAL(DBG_FULL,"K: ",*(double *)pcad->valk);

		status = CAD_ACCEPT;
		break;

	  case CAD_MARK :
		status = CAD_ACCEPT;
		break ;

	  case CAD_STOP :
		status = CAD_REJECT;
		strncpy(CADMESS, "STOP directive not implemented", MAX_STRING_SIZE-1);
		break ;

	  case CAD_CLEAR :
		status = CAD_ACCEPT;
		break ;
    }
    return(status);
}
long gmSeqCadReboot (struct cadRecord *pcad)
/*
 * Null routine called by reboot CAD record -
 * actual reboot is done by a subroutine record 
 * calling gmSeqReboot
 */
{

	
	if ((DIRECTIVE == CAD_START)&&(strcmp(pcad->d,"disabled") == 0))
	{	printf("rebootCad motionDisable = %s\n",pcad->d);
		printf ("reboot rejected\n\n");
		sprintf(pcad->mess,"is:reboot:  rejected during observe");
		return CAD_REJECT;
	}
	return CAD_ACCEPT;
}

/*+
 *   Function name:
 *   gmSeqReboot
 *
 *   Purpose:
 *   Reboot the NIRS Instrument Sequencer
 *
 *   Description:
 *   Called by subroutine record to reboot Instrument Sequencer IOC
 *
 *   Invocation:
 *   gmSeqReboot(psub)
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *      (!)   psub   (struct subRecord*)  Pointer to subroutine record structure
 *
 *   Function value:
 *   (<)  status  (long) Return status, 0 = OK
 * 
 *   External functions:
 *   (VxWorks rebootLib routine) reboot
 *
 *-
 */
long gmSeqReboot (struct subRecord *psub)
{ 
	printf ("*** REBOOTING ***\n");
	taskDelay(60);

	reboot(BOOT_QUICK_AUTOBOOT);
	return CAD_ACCEPT;
}


/*+
 *   Function name:
 *   gmSeqCadInit
 *
 *   Purpose:
 *   Implements the Sequencer init command
 *
 *   Description:
 *   This routine is the process subroutine for the init sequence
 *   command  CAD. It calls the initialisation routines for both the
 *   filter and grating CADs.
 *
 *   Invocation:
 *   gmSeqCadinit (pcad)
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *      (!)   pcad   (struct cadRecord*)  Pointer to CAD record structure
 *
 *   EPICS input parameters:

 *
 *   Function value:
 *   (<)  status  (long) Return status, 0 = OK
 *                 non-zero = either or both initialisation 
 *                 calls returned an error status
 *
 *   External functions:
 *
 *-
 */
long gmSeqCadInit (struct cadRecord *pcad)
{
	long status = CAD_ACCEPT;
/* 	char lutName[STRING_BUF_SZ]; */
      
	switch (DIRECTIVE) 
	{
		
	  case CAD_PRESET :   
		/* add simulation mode !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

		/* Set the appropriate global test conditions for this CAD */

		gmSeqSetCADTest(CONFIGURING+READING_OUT+OBSERVING);
		break;
    
	  case CAD_START :


		/* Initialise from the Mask, Filter and Grating databases */

	

	    


		break;
     
	  case CAD_MARK :
		status = CAD_ACCEPT;
		break ;

	  case CAD_STOP :
		status = CAD_REJECT;
		strncpy(CADMESS, "STOP directive not implemented", MAX_STRING_SIZE-1);
		break ;

	  case CAD_CLEAR :
		status = CAD_ACCEPT;
		break ;
	}
  
	return status;
}


long gmSeqCadSetup (struct cadRecord *pcad)
{
	/*
	 * For any CAD routines involved in setting up the instrument
	 * not already covered by other functions.
	 * Currently does nothing. Just set the appropriate 
	 * global test conditions for this CAD
	 */

    gmSeqSetCADTest(CONFIGURING+READING_OUT+OBSERVING);
    return CAD_ACCEPT;
}

long gmSeqCadTest (struct cadRecord *pcad)
{
	/* Set global test conditions for TEST */
    gmSeqSetCADTest(CONFIGURING+READING_OUT+OBSERVING);
    return CAD_ACCEPT;
}

long gmSeqCadDatum (struct cadRecord *pcad)
{
	/* Set global test conditions for DATUM */
    gmSeqSetCADTest(CONFIGURING+READING_OUT+OBSERVING+MASTER_ENABLE);
    return CAD_ACCEPT;
}

long gmSeqCadPark (struct cadRecord *pcad)
{
	/* Set global test conditions for PARK */
    gmSeqSetCADTest(CONFIGURING+READING_OUT+OBSERVING+MASTER_ENABLE);
    return CAD_ACCEPT;
}


/*+
 *   Function name:
 *   gmSeqCadDebug
 *
 *   Purpose:
 *   Implements the Instrument Sequencer debug command
 *
 *   Description:
 *   This routine is the process subroutine for the debug
 *   command CAD. It sets the internal gmSeqDbgLevel flag to 
 *   the specified debug level and forwards the debug level
 *   to the components controller and detector controller.
 *   A flag may be used to prevent the debug level being forwarded,
 *   in order to debug the Instrument Sequencer alone.
 *
 *   Invocation:
 *   gmSeqCadDebug (pcad)
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *      (!)   pcad   (struct cadRecord*)  Pointer to CAD record structure
 *
 *   EPICS input parameters:
 *      a => Requested debug level : NONE, MIN or FULL (string)
 *      b => Instrument sequencer only flag            (long)
 *
 *   EPICS output parameters:
 *      vala => Componnets controller copy of the input debug level (string)
 *      valb => Detector controller copy of the input debug level   (string)
 *
 *   Function value:
 *   (<)  status  (long) Return status, 0 = OK
 *
 *   External functions:
 *
 *-
 */
long gmSeqCadDebugInit (struct cadRecord *pcad)
{

	strncpy((char *)pcad->vala, "NONE", MAX_STRING_SIZE-1);
	strcpy((char *)pcad->valb, "NOLOG");
	return OK;
}
long gmSeqCadDebug (struct cadRecord *pcad)
{
    static char debugModeIn[MAX_STRING_SIZE];
    static long debugMode = DBG_NONE;
    static long isOnlyFlag = 0;
    long status;

    status = CAD_ACCEPT;

    switch (DIRECTIVE) 
    {

	  case CAD_PRESET:

		/* Read in the debug mode, upper case, then check validity */

		strncpy(debugModeIn, (char *) pcad->a, MAX_STRING_SIZE-1);
		gmSeqUc(debugModeIn, strlen(debugModeIn) + 1, debugModeIn);
         
		if (strncmp(debugModeIn, "QUIET", 5) == 0)
		{
			debugMode = DBG_QUIET;
		}
		else if (strncmp(debugModeIn, "NONE", 4) == 0)
		{
			debugMode = DBG_QUIET | DBG_NONE;
		}
		else if (strncmp(debugModeIn, "MIN", 3) == 0)
		{
			debugMode = DBG_QUIET | DBG_NONE | DBG_MIN;
		}
		else if (strncmp(debugModeIn, "FULL", 4) == 0)
		{
			debugMode = DBG_QUIET | DBG_NONE | DBG_MIN | DBG_FULL;
		}
		else if (strncmp(debugModeIn, "MAX", 3) == 0)
		{
			debugMode = DBG_QUIET | DBG_NONE | DBG_MIN | DBG_FULL | DBG_MAX;
		}
		else
		{
			strncpy(CADMESS,"Unrecognised debug mode", MAX_STRING_SIZE-1);
			return CAD_REJECT;
		} 

		/*
		 * Read in the "Instrument Sequencer only" flag, if specified.
		 */

		if ( pcad->b != NULL )
		{
			if ( sscanf ((char *)pcad->b, "%ld", &isOnlyFlag ) != 1)
			{
                isOnlyFlag = 0;
			}
		}
		else
		{
			isOnlyFlag = 0;
		}
		/*
		 * Copy the debugging mode to the components controller.
		 */

		if ( !isOnlyFlag )
		{
			printf("set cc debug to %s",debugModeIn);
			strncpy((char *)pcad->vala, debugModeIn, MAX_STRING_SIZE-1);
		}

		/*
		 * The detector controller does not recognise the same debugging levels
		 * as NIRS. Convert QUIET mode into NOLOG mode and MAX mode into FULL.
		 */
		if ( !isOnlyFlag )
		{
			if (strncmp(debugModeIn, "QUIET", 5) == 0)
			{
                strcpy((char *)pcad->valb, "NOLOG");
			printf("set cc debug to nolog\n");
			}
			else if (strncmp(debugModeIn, "MAX", 3) == 0)
			{
                strcpy((char *)pcad->valb, "FULL");
			printf("set dc debug to full\n");
			}
			else
			{
			printf("set dc debug to %s\n",debugModeIn);
                strncpy((char *)pcad->valb, debugModeIn, MAX_STRING_SIZE-1);
			}
		}
		break;
    
	  case CAD_START:

		/* Set the debug level global variable */

		gmSeqDbgLevel = debugMode;
		DBGMSGINT(DBG_MIN,"Instrument Sequencer DEBUG level set to : ", gmSeqDbgLevel);

		/* Reset the "Instrument Sequencer only" flag. */

		isOnlyFlag = 0;
		break;

	  case CAD_MARK :
		status = CAD_ACCEPT;
		break ;

	  case CAD_STOP :
		status = CAD_REJECT;
		strncpy(CADMESS,"STOP directive not implemented", MAX_STRING_SIZE-1);
		break ;

	  case CAD_CLEAR :
		status = CAD_ACCEPT;
		break ;
	}
    
	return status;
}


long gmSeqNullInit (struct subRecord *psub)
/*
 * Null CAD initialisation, required by subroutine records.
 */

{
	return 0;
}

/*+
 *   Function name:
 *   gmSeqConfigBegin
 *
 *   Purpose:
 *   Implements the start of configuration apply handling.
 *
 *   Description:
 *   This routine is the subroutine invoked by the SubCAD
 *   record triggered as the first "CAD" from the top-level 
 *   apply record. It decides whether the apply command triggering
 *   can progress by checking the interlock status. It also clears
 *   the test condition mask, which will be set by subsequent CADs 
 *   triggered by the apply record.
 *
 *   Invocation:
 *   gmSeqConfigBegin (subcad)
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *      (!)   subcad   (struct subCadRecord*)  Pointer to subCad record structure
 *
 *   EPICS input parameters:
 *      a => Gemini system interlock status (1 = TRUE)
 *
 *   EPICS output parameters:
 *
 *   Function value:
 *   (<)  status  (long) Return status, 0 = OK
 *                            -1 = Interlock set
 * 
 *   External functions:
 *
 *-
 */
long gmSeqConfigBegin (struct subCadRecord *subcad)
{
    char interlock[MAX_STRING_SIZE];
    
    switch (subcad->dir) 
    {

	  case CAD_CLEAR:
	  case CAD_PRESET:

		/* Clear the CAD condition test mask for PRESET and CLEAR */

		gmSeqClearCADTest();
    
		/* Directive START : check for interlock */
	  case CAD_START:

		strncpy(interlock, subcad->a, MAX_STRING_SIZE-1);
    
		DBGMSGSTRING(DBG_FULL,"ConfigBegin: interlock status = ", interlock);
    
		if (strncmp(interlock, "OK", 2))
		{
			strncpy(subcad->mess, "NIRS is interlocked", MAX_STRING_SIZE-1);
			return CAD_REJECT;
		}

    }
    return CAD_ACCEPT;
}


/*+
 *   Function name:
 *   gmSeqConfigEnd
 *
 *   Purpose:
 *   Implements the end of configuration apply handling.
 *
 *   Description:
 *   This routine is the subroutine invoked by the SubCAD
 *   record triggered as the last "CAD" from the top-level 
 *   apply record. It decides whether the apply command triggering
 *   can progress by checking the specified test conditions as indicated
 *   by the CADtest mask value. These may include configuration, detector
 *   readout and observing in progress status checks. If any of these 
 *   are preventing the apply being processed then an error status 
 *   is returned. The input CAD directive is output, to be forwarded 
 *   to the subsystem apply record.
 *
 *   Invocation:
 *   gmSeqConfigEnd (subcad)
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *      (!)   subcad   (struct subCadRecord*)  Pointer to subCAD record structure
 *
 *   EPICS input parameters:
 *      dir => Directive to be forwarded to the subsystem apply record
 *      a => NIRS applyC CAR status value.
 *      b => Whether observing is in progress (observeC CAR value)
 *      c => Whether Detector controller is disconnected
 *      d => Whether NIRS detector controller readout 
 *              is currently in progress (gm:dc:rdout 1=TRUE)
 *      e => Whether NIRS "master enable" has been switched on
 *      
 *
 *   EPICS output parameters:
 *      vala => A copy of the input directive field (long)
 *
 *   Function value:
 *   (<)  status  (long) Return status, 0 = OK
 * 
 *   External functions:
 *
 *-
 */
long gmSeqConfigEnd (struct subCadRecord *subcad)
{
    long dir;
    char applyC_str[MAX_STRING_SIZE];
    char observeC_str[MAX_STRING_SIZE];
    char readout_str[MAX_STRING_SIZE];
    char menable_str[MAX_STRING_SIZE];
    long readout;
    int testMask, n;
    int DCdisconnected = FALSE;  /* Is detector controller disconnected? */
    char DCdisconnected_str[MAX_STRING_SIZE];
    
	/* Condition flags */
    int reading_out   = FALSE;
    int configuring   = FALSE;
    int observing     = FALSE;
    int master_enable = FALSE;

	/* Always get the current CAD test condition mask */
    testMask = gmSeqGetCADTest();
    DBGMSGINT(DBG_FULL, "configEnd: testMask = ", testMask);

	/* Read in the applyC values from the subsystems .
	   Items at A and B are strings representing CAR state  */
    strncpy(applyC_str, subcad->a, MAX_STRING_SIZE-1);
    DBGMSGSTRING(DBG_FULL, "A = ", applyC_str);
    
	/* Read the OBSERVING condition value */    
    strncpy(observeC_str, subcad->b, MAX_STRING_SIZE-1);
    DBGMSGSTRING(DBG_FULL, "B = ", observeC_str);
     
	/* Read the DC DISCONNECTED condition value : an integer */    
    strncpy(DCdisconnected_str, subcad->c, MAX_STRING_SIZE-1);
    DBGMSGSTRING(DBG_FULL, "C = ", DCdisconnected_str);
    n = sscanf(DCdisconnected_str, "%ld", &DCdisconnected);
    if (n != 1)  DCdisconnected = 0;    /* No valid integer found? */           
    DBGMSGINT(DBG_FULL," DCdisconnected = ", DCdisconnected);   
        
	/* Read in the READING OUT condition value; an integer */
    strncpy(readout_str, subcad->d, MAX_STRING_SIZE-1);
    DBGMSGSTRING(DBG_FULL, "D = ", readout_str);
    n = sscanf(readout_str, "%ld", &readout);
    if (n != 1) readout = 0;    /* No valid integer found? */    
    DBGMSGINT(DBG_FULL,"readout = ", readout);

	/* Read in the MASTER ENABLE condition value; an integer */
    strncpy(menable_str, subcad->e, MAX_STRING_SIZE-1);
    DBGMSGSTRING(DBG_FULL, "E = ", menable_str);
    n = sscanf(menable_str, "%ld", &master_enable);
    if (n != 1) master_enable = 0;    /* No valid integer found? */    
    DBGMSGINT(DBG_FULL,"master enable = ", master_enable);


	/* Determine the current condition of the system. */
    if (readout != 0 && !DCdisconnected)  /* Ignore reading out status if disconnected */
    {
		reading_out= TRUE;
		DBGMSG(DBG_FULL,"NIRS is READING OUT");
    }
      
    else if (strncmp(observeC_str,"BUSY",4) == 0)
    {
		observing = TRUE;
		DBGMSG(DBG_FULL,"NIRS is OBSERVING");
    }

    else if (strncmp(applyC_str,"BUSY",4) == 0)
    {
		configuring = TRUE;
		DBGMSG(DBG_FULL,"NIRS is CONFIGURING");
    }
             
    switch (subcad->dir) 
    {

		/* PRESET only: Check whether configuring and  whether master enabled, if specified
		   by CONFIGURING or MASTER_ENABLE in the test mask */

	  case CAD_PRESET:

		if ( ((testMask & CONFIGURING) != 0) && (configuring == TRUE) ) 
		{
			DBGMSG(DBG_FULL,"NIRS PRESET fail: configuring interlock");
			strncpy(subcad->mess, "NIRS currently reconfiguring", MAX_STRING_SIZE-1);
			return CAD_REJECT;
		}

		if ( ((testMask & MASTER_ENABLE) != 0) && (master_enable == FALSE) ) 
		{
			DBGMSG(DBG_FULL,"NIRS PRESET fail: no master enable interlock");
			strncpy(subcad->mess, "No NIRS master enable - still booting?", MAX_STRING_SIZE-1);
			return CAD_REJECT;
		}


		/* START: Perform any other checks (reading out, observing)
		   as specified in the test mask (Values READING_OUT and OBSERVING) */

	  case CAD_START: 
    
		if ( ((testMask & READING_OUT) != 0) && (reading_out == TRUE) )
		{
			DBGMSG(DBG_FULL,"NIRS START fail: readout interlock");
			strncpy(subcad->mess, "NIRS detector currently reading out", MAX_STRING_SIZE-1);
			return CAD_REJECT;
		}
      
		if ( ((testMask & OBSERVING) != 0) && (observing == TRUE) )
		{
			DBGMSG(DBG_FULL,"NIRS START fail: observing interlock");
			strncpy(subcad->mess, "NIRS currently observing", MAX_STRING_SIZE-1);
			return CAD_REJECT;
		}    
     
	  default:

		/* Always copy out the directive to VALA */

		dir = subcad->dir;
		*(long *) subcad->vala = dir;
    }
   
    return CAD_ACCEPT;
}


long gmSeqCadTrivial (struct cadRecord *pcad)

/*
 * Simple routine for NIRS sequence CAD record 'type A',
 * a trivial command which is not forwarded to CC or DC and 
 * there are no specific checks for accept/reject except that
 * the STOP directive is unimplemented.
 */
   
{   
    DBGMSGSTRING(DBG_MIN,"CAD ", pcad->name);
    DBGMSGINT(DBG_MIN,"Directive = ", DIRECTIVE);

    switch (DIRECTIVE) 
    {

	  case CAD_STOP:
		strncpy(CADMESS,"STOP directive not implemented", MAX_STRING_SIZE-1);
		return CAD_REJECT;
    
	  default:
		return CAD_ACCEPT;
    }
}

/*+
 *   Function name:
 *   gmSeqCadObserve
 *
 *   Purpose:
 *   Implements the CAD for the 'observe' sequence command
 *
 *   Description:
 *   This routine is the process subroutine for the observe CAD.
 *   Simply passes on the dataset ID string for the detector controller
 *
 *   Invocation:
 *   gmSeqCadObserve (pcad)
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *      (!)   pcad   (struct cadRecord*)  Pointer to cad record structure
 *
 *   EPICS input parameters:
 *      a => dataset ID (string)
 *      d => whether detector controller enabled (long)
 *
 *   EPICS output parameters:
 *      vala => dataset ID to DC (string)
 * 
 *
 *   Function value:
 *   (<)  status  (long) Return status, 0 = OK
 * 
 *   External functions:
 *
 *-
 */
long gmSeqCadObserve(struct cadRecord *pcad)
{ 
 
    long status=CAD_ACCEPT;           /* Return status */
    char datasetID[MAX_STRING_SIZE];
    long dcDisabled;

    DBGMSGINT(DBG_MIN,"CAD observe. Directive = ", DIRECTIVE);

    switch (DIRECTIVE) 
    {

	  case CAD_PRESET :

		/* Reject the command if the detector controller is disabled */

		sscanf( (char *)pcad->d, "%ld", &dcDisabled);
		DBGMSGINT(DBG_FULL,"CAD observe. DC disabled = ", dcDisabled);
		if ( dcDisabled )
		{
			strncpy(CADMESS, "Detector controller not available", MAX_STRING_SIZE-1);
			status = CAD_REJECT;
		}
		else
		{
			/*check state of CC*/

			/* Set the appropriate global test conditions for this CAD */
			gmSeqSetCADTest(CONFIGURING+READING_OUT+OBSERVING);

			/* Read in the dataset ID */
			strncpy(datasetID, (char *)pcad->a, MAX_STRING_SIZE-1);

			/* Output the dataset ID string */
			strncpy((char *)pcad->vala, datasetID, MAX_STRING_SIZE-1);
   
			status = CAD_ACCEPT;
		}

		break ;

	  case CAD_START :

		DBGMSGSTRING(DBG_FULL,"A: ",(char *)pcad->vala);

		status = CAD_ACCEPT;
		break;

	  case CAD_MARK :
		status = CAD_ACCEPT;
		break ;

	  case CAD_STOP :
		status = CAD_REJECT;
		strncpy(CADMESS,"STOP directive not implemented", MAX_STRING_SIZE-1);
		break ;

	  case CAD_CLEAR :
		status = CAD_ACCEPT;
		break ;
	}
   
	return(status);
}


long gmSeqCadStopObserve (struct cadRecord *pcad)

/*
 * Simple routine for NIRS sequence CAD records which pauses or
 * stops an observation.
 */
   
{
    long status = CAD_ACCEPT;           /* Return status */
    long dcDisabled;
	long observing;

    switch (DIRECTIVE)
    {
/* 	  case CAD_MARK: */
/* 		DBGMSG(DBG_FULL,"CAD stop observe. mark "); */
/* 		printf("marked\n"); */
	
	  case CAD_PRESET:

		/*      Reject the command if the detector controller is disabled */

		sscanf( (char *)pcad->b, "%ld", &dcDisabled);
		sscanf( (char *)pcad->c, "%ld", &observing);
		DBGMSGINT(DBG_FULL,"CAD stop observe. DC disabled = ", dcDisabled);
		if ( dcDisabled )
		{
			strncpy(CADMESS, "Detector controller not available", MAX_STRING_SIZE-1);
			status = CAD_REJECT;
		}
		else if (!observing)
		{
			
			strncpy(CADMESS, "Detector controller is not observing", MAX_STRING_SIZE-1);
			status = CAD_REJECT;
		}
				 
		else
		{

			/*         Set the appropriate global test condition for this CAD */

			gmSeqSetCADTest( 0 );
			status = CAD_ACCEPT;
		}
    }
    return (status);
}

long gmSeqCadEndObserve (struct cadRecord *pcad)
/*
 * Simple routine for NIRS sequence CAD endObserve
 */
   
{
    switch (DIRECTIVE)
    {
	  case CAD_PRESET:

		/*       Set the appropriate global test condition for this CAD */ 
		gmSeqSetCADTest(READING_OUT+OBSERVING);
    }
    
    return CAD_ACCEPT;
}

long gmSeqCadContinue (struct cadRecord *pcad)

/*
 * Simple routine for NIRS continue sequence CAD record to
 * continue an observation. The command should be rejected if 
 * a reconfiguration is in progress or the detector controller
 * is not available.
 */
   
{
    long status=CAD_ACCEPT;           /* Return status */
    long dcDisabled;

    switch (DIRECTIVE)
    {
	  case CAD_PRESET:

		/*       Reject the command if the detector controller is disabled */

		sscanf( (char *)pcad->b, "%ld", &dcDisabled);
		DBGMSGINT(DBG_FULL,"CAD continue. DC disabled = ", dcDisabled);
		if ( dcDisabled )
		{
			strncpy(CADMESS, "Detector controller not available", MAX_STRING_SIZE-1);
			status = CAD_REJECT;
		}
		else
		{

			/*         Set the appropriate global test condition for this CAD */

			gmSeqSetCADTest(CONFIGURING);
			status = CAD_ACCEPT;
		}
    }
    
    return (status);
}


/*+
 *   Function name:
 *   gmSeqCadInitCover 
 *
 *   Purpose:
 *   Initialise the filter CAD 
 *
 *   Description:
 *   Called at EPICS iocInit. Calls gmSeqCoverLUTread
 *
 *   Invocation:
 *   gmSeqCadInitCover(pcad)
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *      (!)   pcad   (struct cadRecord*)  Pointer to CAD record structure
 *
 *   Function value:
 *   (<)  status  (long) Return status, 0 = OK
 *                       Error if file not opened or missing data values
 * 
 *   External functions:
 *   (VxWorks lstLib routines) lstInit, lstAdd
 *
 *-
 */

long gmSeqCadInitCover(struct cadRecord *pcad)
{ 
    long status = CAD_ACCEPT;

    return status;
}

/*+
 *   Function name:
 *   gmSeqCadCover 
 *
 *   Purpose:
 *   Implements the fltPos command
 *
 *   Description:
 *   This routine is the process subroutine for the fltPos CAD.
 *   It translates the input filter name by looking up the name in
 *   the filter look-up table, The resulting barcode ID is written to the
 *   NIRS Components Controller fltAssembly record.
 *
 *   Invocation:
 *   gmSeqCadCover (pcad)
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *      (!)   pcad   (struct cadRecord*)  Pointer to CAD record structure
 *
 *   EPICS input parameters:
 *      a => Cover name 1 as a text string
 *
 *   EPICS output parameters:
 *      vala => translated Barcode ID for filter 1 (string)
 *      valc => Focus offset due to filter 1 surface curvature (double)
 *      vale => Focus offset due to filter 1 effective wavelength (double)
 *      valg => Cover name 1 (string)
 *
 *   Function value:
 *   (<)  status  (long) Return status, 0 = OK
 * 
 *   External functions:
 *
 *-
 */
long gmSeqCadCover(struct cadRecord *pcad)
{ 
	char err[160];
    long status;           /* Return status */
  /*   NIRSLUT *p; */
    char buff[STRING_BUF_SZ];
    static char fname[MAX_STRING_SIZE] = "unknown";
    static char barcodeId[MAX_STRING_SIZE];
    static double focusOffset;
    static double lambdaOffset;
   /*  double lambda; */

    status = CAD_ACCEPT ;
     
    DBGMSGINT(DBG_MIN,"CAD cover. Directive = ", DIRECTIVE);
    
    switch (DIRECTIVE)
    {

	  case CAD_PRESET:

	
		DBGMSG(DBG_FULL,"gmSeqCadCover PRESET");

		/* Set the appropriate global test conditions for this CAD */

		gmSeqSetCADTest(OBSERVING+MASTER_ENABLE);
    
		strncpy(fname, (char *)pcad->a, MAX_STRING_SIZE-1);
		DBGMSGSTRING(DBG_FULL,"***************",fname);
		if (fname[0])
		{
			if(strcmp(fname,COVER1) != 0)
				if(strcmp(fname,COVER2) != 0)
				{
					sprintf(err,"name must be %s,  or %s\n",COVER1, COVER2);
					strncpy(CADMESS,err,MAX_STRING_SIZE-1);
					printf(err);
					status = CAD_REJECT;
				}
		}

		if (status == CAD_REJECT)
		{
			strncpy(buff, "Unknown cover name (A): ", MAX_STRING_SIZE-1);
			strncat(buff, fname, MAX_STRING_SIZE - strlen(buff));
			strncpy(CADMESS, buff, MAX_STRING_SIZE-1);
			return(status);
		}



		/* Output the barcode values for further checking by CC */

		strncpy((char *)pcad->vala, fname, MAX_STRING_SIZE-1);
		break ;

	  case CAD_START:

		DBGMSG(DBG_FULL,"gmSeqCadCover START");
		/* Covers are OK, so output focus offsets and original filter names */

		*(double *) pcad->valc = focusOffset;
		*(double *) pcad->vale = lambdaOffset;
		strncpy(pcad->valg, barcodeId, MAX_STRING_SIZE-1);
		status = CAD_ACCEPT;
   
		DBGMSG(DBG_FULL,"CAD fltPos. Output values A..H:");
		DBGMSGSTRING(DBG_FULL,"A: ", (char *)pcad->vala);
		DBGMSGREAL(DBG_FULL,"C: ",*(double *)pcad->valc);
		DBGMSGREAL(DBG_FULL,"E: ",*(double *)pcad->vale);
		DBGMSGSTRING(DBG_FULL,"G: ",pcad->valg);
   
		break;

	  case CAD_MARK :

		status = CAD_ACCEPT;
		break ;

	  case CAD_STOP :

		status = CAD_REJECT;
		strncpy(CADMESS, "STOP directive not implemented", MAX_STRING_SIZE-1);
		break ;

	  case CAD_CLEAR :

		status = CAD_ACCEPT;
		break ;
	}

	return(status);
}/*+
  *   Function name:
  *   gmSeqCadInitDecker 
  *
  *   Purpose:
  *   Initialise the filter CAD 
  *
  *   Description:
  *   Called at EPICS iocInit. Calls gmSeqDeckerLUTread
  *
  *   Invocation:
  *   gmSeqCadInitDecker(pcad)
  *
  *   Parameters: (">" input, "!" modified, "<" output)  
  *      (!)   pcad   (struct cadRecord*)  Pointer to CAD record structure
  *
  *   Function value:
  *   (<)  status  (long) Return status, 0 = OK
  *                       Error if file not opened or missing data values
  * 
  *   External functions:
  *   (VxWorks lstLib routines) lstInit, lstAdd
  *
 *-
 */

long gmSeqCadInitDecker(struct cadRecord *pcad)
{ 
    long status = CAD_ACCEPT;

    return status;
}

/*+
 *   Function name:
 *   gmSeqCadDecker 
 *
 *   Purpose:
 *   Implements the fltPos command
 *
 *   Description:
 *   This routine is the process subroutine for the fltPos CAD.
 *   It translates the input filter name by looking up the name in
 *   the filter look-up table, The resulting barcode ID is written to the
 *   NIRS Components Controller fltAssembly record.
 *
 *   Invocation:
 *   gmSeqCadDecker (pcad)
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *      (!)   pcad   (struct cadRecord*)  Pointer to CAD record structure
 *
 *   EPICS input parameters:
 *      a => Decker name 1 as a text string
 *
 *   EPICS output parameters:
 *      vala => translated Barcode ID for filter 1 (string)
 *      valc => Focus offset due to filter 1 surface curvature (double)
 *      vale => Focus offset due to filter 1 effective wavelength (double)
 *      valg => Decker name 1 (string)
 *
 *   Function value:
 *   (<)  status  (long) Return status, 0 = OK
 * 
 *   External functions:
 *
 *-
 */
long gmSeqCadDecker(struct cadRecord *pcad)
{ 
	char err[160];
    long status;           /* Return status */
  /*   NIRSLUT *p; */
    char buff[STRING_BUF_SZ];
    static char fname[MAX_STRING_SIZE] = "unknown";
    static char barcodeId[MAX_STRING_SIZE];
    static double focusOffset;
    static double lambdaOffset;
   /*  double lambda; */
    status = CAD_ACCEPT ;
     
    DBGMSGINT(DBG_MIN,"CAD fltPos. Directive = ", DIRECTIVE);
    
    switch (DIRECTIVE)
    {

	  case CAD_PRESET:

	
		DBGMSG(DBG_FULL,"gmSeqCadDecker PRESET ");

		/* Set the appropriate global test conditions for this CAD */

		gmSeqSetCADTest(OBSERVING+MASTER_ENABLE);
    
		strncpy(fname, (char *)pcad->a, MAX_STRING_SIZE-1);
		if (fname[0])
		{
			if(strcmp(fname,DECKER1) != 0)
				if(strcmp(fname,DECKER2) != 0)
					if(strcmp(fname,DECKER3) != 0)
						if(strcmp(fname,DECKER4) != 0)
							if(strcmp(fname,DECKER5) != 0)
								if(strcmp(fname,DECKER6) != 0)
								{
									sprintf(err,"name must be %s, %s,%s, %s,%s or %s\n",DECKER1,DECKER2,DECKER3,DECKER4,DECKER5,DECKER6);
									strncpy(CADMESS,err,MAX_STRING_SIZE-1);
									printf(err);
									status = CAD_REJECT;
								}
		}

		if (status == CAD_REJECT)
		{
			strncpy(buff, "Unknown decker position (A): ", MAX_STRING_SIZE-1);
			strncat(buff, fname, MAX_STRING_SIZE - strlen(buff));
			strncpy(CADMESS, buff, MAX_STRING_SIZE-1);
			return(status);
		}

	

	

		/* Output the barcode values for further checking by CC */

		strncpy(pcad->vala, fname, MAX_STRING_SIZE-1);
		break ;

	  case CAD_START:

		DBGMSG(DBG_FULL,"gmSeqCadDecker START ");
		/* Deckers are OK, so output focus offsets and original filter names */

		*(double *) pcad->valc = focusOffset;
		*(double *) pcad->vale = lambdaOffset;
		status = CAD_ACCEPT;
		strncpy((char *)pcad->valg, barcodeId, MAX_STRING_SIZE-1);
   
		DBGMSG(DBG_FULL,"CAD fltPos. Output values A..H:");
		DBGMSGSTRING(DBG_FULL,"A: ", (char *)pcad->vala);
		DBGMSGREAL(DBG_FULL,"C: ",*(double *)pcad->valc);
		DBGMSGREAL(DBG_FULL,"E: ",*(double *)pcad->vale);
		DBGMSGSTRING(DBG_FULL,"G: ",pcad->valg);
   
		break;

	  case CAD_MARK :

		status = CAD_ACCEPT;
		break ;

	  case CAD_STOP :

		status = CAD_REJECT;
		strncpy(CADMESS, "STOP directive not implemented", MAX_STRING_SIZE-1);
		break ;

	  case CAD_CLEAR :

		status = CAD_ACCEPT;
		break ;
	}

	return(status);
}/*+
  *   Function name:
  *   gmSeqCadInitFocus 
  *
  *   Purpose:
  *   Initialise the filter CAD 
  *
  *   Description:
  *   Called at EPICS iocInit. Calls gmSeqFocusLUTread
  *
  *   Invocation:
  *   gmSeqCadInitFocus(pcad)
  *
  *   Parameters: (">" input, "!" modified, "<" output)  
  *      (!)   pcad   (struct cadRecord*)  Pointer to CAD record structure
  *
  *   Function value:
  *   (<)  status  (long) Return status, 0 = OK
  *                       Error if file not opened or missing data values
  * 
  *   External functions:
  *   (VxWorks lstLib routines) lstInit, lstAdd
  *
 *-
 */

long gmSeqCadInitFocus(struct cadRecord *pcad)
{ 
    long status = CAD_ACCEPT;

    return status;
}

/*+
 *   Function name:
 *   gmSeqCadFocus 
 *
 *   Purpose:
 *   Implements the fltPos command
 *
 *   Description:
 *   This routine is the process subroutine for the fltPos CAD.
 *   It translates the input filter name by looking up the name in
 *   the filter look-up table, The resulting barcode ID is written to the
 *   NIRS Components Controller fltAssembly record.
 *
 *   Invocation:
 *   gmSeqCadFocus (pcad)
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *      (!)   pcad   (struct cadRecord*)  Pointer to CAD record structure
 *
 *   EPICS input parameters:
 *      a => Focus name 1 as a text string
 *
 *   EPICS output parameters:
 *      vala => translated Barcode ID for filter 1 (string)
 *      valc => Focus offset due to filter 1 surface curvature (double)
 *      vale => Focus offset due to filter 1 effective wavelength (double)
 *      valg => Focus name 1 (string)
 *
 *   Function value:
 *   (<)  status  (long) Return status, 0 = OK
 * 
 *   External functions:
 *
 *-
 */
long gmSeqCadFocus(struct cadRecord *pcad)
{ 
	char err[160];
    long status;           /* Return status */
  /*   NIRSLUT *p; */
    char buff[STRING_BUF_SZ];
    static char fname[MAX_STRING_SIZE] = "unknown";
    static char barcodeId[MAX_STRING_SIZE];
    static double focusOffset;
    static double lambdaOffset;
 /*    double lambda; */

    status = CAD_ACCEPT ;
     
    DBGMSGINT(DBG_MIN,"CAD fltPos. Directive = ", DIRECTIVE);
    
    switch (DIRECTIVE)
    {

	  case CAD_PRESET:

	
		DBGMSG(DBG_FULL,"gmSeqCadFOCUS PRESET ");

		/* Set the appropriate global test conditions for this CAD */

		gmSeqSetCADTest(OBSERVING+MASTER_ENABLE);
    
		strncpy(fname, (char *)pcad->a, MAX_STRING_SIZE-1);
		if (fname[0])
		{

			if(strcmp(fname,FOCUS1) != 0)
				if(strcmp(fname,FOCUS2) != 0)
				{
					sprintf(err,"Focus name must be %s or %s\n",FOCUS1,FOCUS2);
					strncpy(CADMESS,err,MAX_STRING_SIZE-1);
					printf(err);
					status = CAD_REJECT;
				}
		}

		if (status == CAD_REJECT)
		{
			strncpy(buff, "Unknown  name (A): ", MAX_STRING_SIZE-1);
			strncat(buff, fname, MAX_STRING_SIZE - strlen(buff));
			strncpy(CADMESS, buff, MAX_STRING_SIZE-1);
			return(status);
		}

	

	
		/* Output the barcode values for further checking by CC */

		strncpy((char *)pcad->vala, fname, MAX_STRING_SIZE-1);
		break ;

	  case CAD_START:

		DBGMSG(DBG_FULL,"gmSeqCadFocus START");
		/* Focuss are OK, so output focus offsets and original filter names */

		*(double *) pcad->valc = focusOffset;
		*(double *) pcad->vale = lambdaOffset;
		strncpy(pcad->valg, barcodeId, MAX_STRING_SIZE-1);
		status = CAD_ACCEPT;
   
		DBGMSG(DBG_FULL,"CAD fltPos. Output values A..H:");
		DBGMSGSTRING(DBG_FULL,"A: ", (char *)pcad->vala);
		DBGMSGREAL(DBG_FULL,"C: ",*(double *)pcad->valc);
		DBGMSGREAL(DBG_FULL,"E: ",*(double *)pcad->vale);
		DBGMSGSTRING(DBG_FULL,"G: ",pcad->valg);
   
		break;

	  case CAD_MARK :

		status = CAD_ACCEPT;
		break ;

	  case CAD_STOP :

		status = CAD_REJECT;
		strncpy(CADMESS, "STOP directive not implemented", MAX_STRING_SIZE-1);
		break ;

	  case CAD_CLEAR :

		status = CAD_ACCEPT;
		break ;
	}

	return(status);
}

/*+
 *   Function name:
 *   gmSeqCadInitAcq 
 *
 *   Purpose:
 *   Initialise the filter CAD 
 *
 *   Description:
 *   Called at EPICS iocInit. Calls gmSeqAcqLUTread
 *
 *   Invocation:
 *   gmSeqCadInitAcq(pcad)
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *      (!)   pcad   (struct cadRecord*)  Pointer to CAD record structure
 *
 *   Function value:
 *   (<)  status  (long) Return status, 0 = OK
 *                       Error if file not opened or missing data values
 * 
 *   External functions:
 *   (VxWorks lstLib routines) lstInit, lstAdd
 *
 *-
 */

long gmSeqCadInitAcq(struct cadRecord *pcad)
{ 
    long status = CAD_ACCEPT;

    return status;
}

/*+
 *   Function name:
 *   gmSeqCadAcq 
 *
 *   Purpose:
 *   Implements the fltPos command
 *
 *   Description:
 *   This routine is the process subroutine for the fltPos CAD.
 *   It translates the input filter name by looking up the name in
 *   the filter look-up table, The resulting barcode ID is written to the
 *   NIRS Components Controller fltAssembly record.
 *
 *   Invocation:
 *   gmSeqCadAcq (pcad)
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *      (!)   pcad   (struct cadRecord*)  Pointer to CAD record structure
 *
 *   EPICS input parameters:
 *      a => Acq name 1 as a text string
 *
 *   EPICS output parameters:
 *      vala => translated Barcode ID for filter 1 (string)
 *      valc => Focus offset due to filter 1 surface curvature (double)
 *      vale => Focus offset due to filter 1 effective wavelength (double)
 *      valg => Acq name 1 (string)
 *
 *   Function value:
 *   (<)  status  (long) Return status, 0 = OK
 * 
 *   External functions:
 *
 *-
 */
long gmSeqCadAcq(struct cadRecord *pcad)
{ 
	char err[160];
    long status;           /* Return status */
  /*   NIRSLUT *p; */
    char buff[STRING_BUF_SZ];
    static char fname[MAX_STRING_SIZE] = "unknown";
    static char barcodeId[MAX_STRING_SIZE];
    static double focusOffset;
    static double lambdaOffset;
   /*  double lambda; */
    status = CAD_ACCEPT ;
     
    DBGMSGINT(DBG_MIN,"CAD fltPos. Directive = ", DIRECTIVE);
    
    switch (DIRECTIVE)
    {

	  case CAD_PRESET:

	
		DBGMSG(DBG_FULL,"gmSeqCadAcq PRESET");
		/* Set the appropriate global test conditions for this CAD */

		gmSeqSetCADTest(OBSERVING+MASTER_ENABLE);
    
		strncpy(fname, (char *)pcad->a, MAX_STRING_SIZE-1);
		DBGMSGSTRING(DBG_FULL,"seq fname =",fname);
		if (fname[0])
		{

			if(strcmp(fname,ACQ1) != 0)
				if(strcmp(fname,ACQ2) != 0)
				{
					sprintf(err,"name must be %s or %s\n",ACQ1,ACQ2);
					strncpy(CADMESS,err,MAX_STRING_SIZE-1);
					printf(err);
					status = CAD_REJECT;
				}
		}

		if (status == CAD_REJECT)
		{
			strncpy(buff, "Unknown acq name (A): ", MAX_STRING_SIZE-1);
			strncat(buff, fname, MAX_STRING_SIZE - strlen(buff));
			strncpy(CADMESS, buff, MAX_STRING_SIZE-1);
			return(status);
		}

	

	
		/* Output the  value for further checking by CC */

		strncpy(pcad->vala, fname, MAX_STRING_SIZE-1);
		strncpy(pcad->valh, fname, MAX_STRING_SIZE-1);
		break ;

	  case CAD_START:
		DBGMSG(DBG_FULL,"gmSeqCadAcq Start");
		/* Acqs are OK, so output focus offsets and original filter names */

		*(double *) pcad->valc = focusOffset;
		*(double *) pcad->vale = lambdaOffset;
		strncpy((char *)pcad->valg, barcodeId, MAX_STRING_SIZE-1);
		status = CAD_ACCEPT;
   
		DBGMSG(DBG_FULL,"CAD acqPos. Output values A..H:");
		DBGMSGSTRING(DBG_FULL,"A: ", (char *)pcad->vala);
		DBGMSGREAL(DBG_FULL,"C: ",*(double *)pcad->valc);
		DBGMSGREAL(DBG_FULL,"E: ",*(double *)pcad->vale);
		DBGMSGSTRING(DBG_FULL,"G: ",pcad->valg);
		DBGMSGSTRING(DBG_FULL,"H: ", (char *)pcad->valh);
   
		break;

	  case CAD_MARK :

		DBGMSG(DBG_FULL,"gmSeqCadAcq MARK");
		status = CAD_ACCEPT;
		break ;

	  case CAD_STOP :

		status = CAD_REJECT;
		strncpy(CADMESS, "STOP directive not implemented", MAX_STRING_SIZE-1);
		break ;

	  case CAD_CLEAR :

		DBGMSG(DBG_FULL,"gmSeqCadAcq CLEAR");
		status = CAD_ACCEPT;
		break ;
	}

	return(status);
}

/*+
 *   Function name:
 *   gmSeqCadInitCamera 
 *
 *   Purpose:
 *   Initialise the filter CAD 
 *
 *   Description:
 *   Called at EPICS iocInit. Calls gmSeqCameraLUTread
 *
 *   Invocation:
 *   gmSeqCadInitCamera(pcad)
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *      (!)   pcad   (struct cadRecord*)  Pointer to CAD record structure
 *
 *   Function value:
 *   (<)  status  (long) Return status, 0 = OK
 *                       Error if file not opened or missing data values
 * 
 *   External functions:
 *   (VxWorks lstLib routines) lstInit, lstAdd
 *
 *-
 */

long gmSeqCadInitCamera(struct cadRecord *pcad)
{ 
    long status = CAD_ACCEPT;

    return status;
}

/*+
 *   Function name:
 *   gmSeqCadCamera 
 *
 *   Purpose:
 *   Implements the fltPos command
 *
 *   Description:
 *   This routine is the process subroutine for the fltPos CAD.
 *   It translates the input filter name by looking up the name in
 *   the filter look-up table, The resulting barcode ID is written to the
 *   NIRS Components Controller fltAssembly record.
 *
 *   Invocation:
 *   gmSeqCadCamera (pcad)
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *      (!)   pcad   (struct cadRecord*)  Pointer to CAD record structure
 *
 *   EPICS input parameters:
 *      a => Camera name 1 as a text string
 *
 *   EPICS output parameters:
 *      vala => translated Barcode ID for filter 1 (string)
 *      valc => Focus offset due to filter 1 surface curvature (double)
 *      vale => Focus offset due to filter 1 effective wavelength (double)
 *      valg => Camera name 1 (string)
 *
 *   Function value:
 *   (<)  status  (long) Return status, 0 = OK
 * 
 *   External functions:
 *
 *-
 */
long gmSeqCadCamera(struct cadRecord *pcad)
{ 
	char err[160];
    long status;           /* Return status */
/*     NIRSLUT *p; */
    char buff[STRING_BUF_SZ];
    static char fname[MAX_STRING_SIZE] = "unknown";
    static char barcodeId[MAX_STRING_SIZE];
    static double focusOffset;
    static double lambdaOffset;
 /*    double lambda; */
  

    status = CAD_ACCEPT ;
     
    DBGMSGINT(DBG_MIN,"CAD fltPos. Directive = ", DIRECTIVE);
    
    switch (DIRECTIVE)
    {

	  case CAD_PRESET:

	

		/* Set the appropriate global test conditions for this CAD */

		gmSeqSetCADTest(OBSERVING+MASTER_ENABLE);
    
		strncpy(fname, (char *)pcad->a, MAX_STRING_SIZE-1);
		if (fname[0])
		{

			if(strcmp(fname,CAMERA1) != 0)
				if(strcmp(fname,CAMERA2) != 0)
					if(strcmp(fname,CAMERA3) != 0)
						if(strcmp(fname,CAMERA4) != 0)
						{
							sprintf(err,"Camera name must be %s, %s, or %s %s\n",CAMERA1,CAMERA2,CAMERA3,CAMERA4);
							strncpy(CADMESS,err,MAX_STRING_SIZE-1);
							printf(err);
							status = CAD_REJECT;
						}
		}

		if (status == CAD_REJECT)
		{
			strncpy(buff, "Unknown camera name (A): ", MAX_STRING_SIZE-1);
			strncat(buff, fname, MAX_STRING_SIZE - strlen(buff));
			strncpy(CADMESS, buff, MAX_STRING_SIZE-1);
			return(status);
		}

	

	

		/* Output the barcode values for further checking by CC */

		strncpy(pcad->vala, fname, MAX_STRING_SIZE-1);
		strncpy(pcad->valh, fname, MAX_STRING_SIZE-1);
		break ;

	  case CAD_START:

		/* Cameras are OK, so output focus offsets and original filter names */

		*(double *) pcad->valc = focusOffset;
		*(double *) pcad->vale = lambdaOffset;
		strncpy((char *)pcad->valg, barcodeId, MAX_STRING_SIZE-1);
		status = CAD_ACCEPT;
   
		DBGMSG(DBG_FULL,"CAD fltPos. Output values A..H:");
		DBGMSGSTRING(DBG_FULL,"A: ", (char *)pcad->vala);
		DBGMSGREAL(DBG_FULL,"C: ",*(double *)pcad->valc);
		DBGMSGREAL(DBG_FULL,"E: ",*(double *)pcad->vale);
		DBGMSGSTRING(DBG_FULL,"G: ",pcad->valg);
		DBGMSGSTRING(DBG_FULL,"H: ", (char *)pcad->valh);
   
		break;

	  case CAD_MARK :

		status = CAD_ACCEPT;
		break ;

	  case CAD_STOP :

		status = CAD_REJECT;
		strncpy(CADMESS, "STOP directive not implemented", MAX_STRING_SIZE-1);
		break ;

	  case CAD_CLEAR :

		status = CAD_ACCEPT;
		break ;
	}

	return(status);
}

/*+
 *   Function name:
 *   gmSeqCadInitSlit 
 *
 *   Purpose:
 *   Initialise the filter CAD 
 *
 *   Description:
 *   Called at EPICS iocInit. Calls gmSeqSlitLUTread
 *
 *   Invocation:
 *   gmSeqCadInitSlit(pcad)
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *      (!)   pcad   (struct cadRecord*)  Pointer to CAD record structure
 *
 *   Function value:
 *   (<)  status  (long) Return status, 0 = OK
 *                       Error if file not opened or missing data values
 * 
 *   External functions:
 *   (VxWorks lstLib routines) lstInit, lstAdd
 *
 *-
 */

long seqCadInitSlit(struct cadRecord *pcad)
{ 
    long status = CAD_ACCEPT;

    return status;
}

/*+
 *   Function name:
 *   gmSeqCadSlit 
 *
 *   Purpose:
 *   Implements the fltPos command
 *
 *   Description:
 *   This routine is the process subroutine for the fltPos CAD.
 *   It translates the input filter name by looking up the name in
 *   the filter look-up table, The resulting barcode ID is written to the
 *   NIRS Components Controller fltAssembly record.
 *
 *   Invocation:
 *   gmSeqCadSlit (pcad)
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *      (!)   pcad   (struct cadRecord*)  Pointer to CAD record structure
 *
 *   EPICS input parameters:
 *      a => Slit name 1 as a text string
 *
 *   EPICS output parameters:
 *      vala => translated Barcode ID for filter 1 (string)
 *      valc => Focus offset due to filter 1 surface curvature (double)
 *      vale => Focus offset due to filter 1 effective wavelength (double)
 *      valg => Slit name 1 (string)
 *
 *   Function value:
 *   (<)  status  (long) Return status, 0 = OK
 * 
 *   External functions:
 *
 *-
 */
long seqCadSlit(struct cadRecord *pcad)
{ 
	char err[160];
    long status;           /* Return status */
  /*   NIRSLUT *p; */
    char buff[STRING_BUF_SZ];
    static char fname[MAX_STRING_SIZE] = "unknown";
    static char barcodeId[MAX_STRING_SIZE];
    static double focusOffset;
    static double lambdaOffset;
  /*   double lambda; */

    status = CAD_ACCEPT ;
     
    DBGMSGINT(DBG_MIN,"CAD fltPos. Directive = ", DIRECTIVE);
    
    switch (DIRECTIVE)
    {

	  case CAD_PRESET:

	

		/* Set the appropriate global test conditions for this CAD */

		gmSeqSetCADTest(OBSERVING+MASTER_ENABLE);
    
		strncpy(fname, (char *)pcad->a, MAX_STRING_SIZE-1);
		if (fname[0])
		{

			if(strcmp(fname,SLIT1) != 0)
				if(strcmp(fname,SLIT2) != 0)
					if(strcmp(fname,SLIT3) != 0)
					{
						sprintf(err,"Slit name must be %s, %s, or %s\n",SLIT1,SLIT2,SLIT3);
						strncpy(CADMESS,err,MAX_STRING_SIZE-1);
						printf(err);
						status = CAD_REJECT;
					}
		}

		if (status == CAD_REJECT)
		{
			strncpy(buff, "Unknown slit name (A): ", MAX_STRING_SIZE-1);
			strncat(buff, fname, MAX_STRING_SIZE - strlen(buff));
			strncpy(CADMESS, buff, MAX_STRING_SIZE-1);
			return(status);
		}


	

		/* Output the barcode values for further checking by CC */

		strncpy(pcad->vala, fname, MAX_STRING_SIZE-1);
		DBGMSGSTRING(DBG_FULL,"seqCadSlit name = ######################", (char *)pcad->vala);
		break ;

	  case CAD_START:

		/* Slits are OK, so output focus offsets and original filter names */

		*(double *) pcad->valc = focusOffset;
		*(double *) pcad->vale = lambdaOffset;
		strncpy((char *)pcad->valg, barcodeId, MAX_STRING_SIZE-1);
		status = CAD_ACCEPT;
   
		DBGMSG(DBG_FULL,"CAD fltPos. Output values A..H:");
		DBGMSGSTRING(DBG_FULL,"A: ", (char *)pcad->vala);
		DBGMSGREAL(DBG_FULL,"C: ",*(double *)pcad->valc);
		DBGMSGREAL(DBG_FULL,"E: ",*(double *)pcad->vale);
		DBGMSGSTRING(DBG_FULL,"G: ",pcad->valg);
   
		break;

	  case CAD_MARK :

		status = CAD_ACCEPT;
		break ;

	  case CAD_STOP :

		status = CAD_REJECT;
		strncpy(CADMESS, "STOP directive not implemented", MAX_STRING_SIZE-1);
		break ;

	  case CAD_CLEAR :

		status = CAD_ACCEPT;
		break ;
	}

	return(status);
}

/*+
 *   Function name:
 *   gmSeqCadInitXDisp 
 *
 *   Purpose:
 *   Initialise the filter CAD 
 *
 *   Description:
 *   Called at EPICS iocInit. Calls gmSeqxdispLUTread
 *
 *   Invocation:
 *   gmSeqCadInitxdisp(pcad)
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *      (!)   pcad   (struct cadRecord*)  Pointer to CAD record structure
 *
 *   Function value:
 *   (<)  status  (long) Return status, 0 = OK
 *                       Error if file not opened or missing data values
 * 
 *   External functions:
 *   (VxWorks lstLib routines) lstInit, lstAdd
 *
 *-
 */

long gmSeqCadInitXDisp(struct cadRecord *pcad)
{ 
    long status = CAD_ACCEPT;

    return status;
}

/*+
 *   Function name:
 *   gmSeqCadXDisp 
 *
 *   Purpose:
 *   Implements the fltPos command
 *
 *   Description:
 *   This routine is the process subroutine for the fltPos CAD.
 *   It translates the input filter name by looking up the name in
 *   the filter look-up table, The resulting barcode ID is written to the
 *   NIRS Components Controller fltAssembly record.
 *
 *   Invocation:
 *   gmSeqCadxdisp (pcad)
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *      (!)   pcad   (struct cadRecord*)  Pointer to CAD record structure
 *
 *   EPICS input parameters:
 *      a => xdisp name 1 as a text string
 *      b => xdisp name 2 as a text string
 *
 *   EPICS output parameters:
 *      vala => translated Barcode ID for filter 1 (string)
 *      valc => Focus offset due to filter 1 surface curvature (double)
 *      vale => Focus offset due to filter 1 effective wavelength (double)
 *      valg => xdisp name 1 (string)
 *
 *   Function value:
 *   (<)  status  (long) Return status, 0 = OK
 * 
 *   External functions:
 *
 *-
 */
long gmSeqCadXDisp(struct cadRecord *pcad)
{ 
	char err[160];
    long status;           /* Return status */
  /*   NIRSLUT *p; */
    char buff[STRING_BUF_SZ];
    static char fname[MAX_STRING_SIZE] = "unknown";
    static char barcodeId[MAX_STRING_SIZE];
    static double focusOffset;
    static double lambdaOffset;
 /*    double lambda; */

    status = CAD_ACCEPT ;
     
    DBGMSGINT(DBG_MIN,"CAD fltPos. Directive = ", DIRECTIVE);
    
    switch (DIRECTIVE)
    {

	  case CAD_PRESET:

	

		/* Set the appropriate global test conditions for this CAD */

		gmSeqSetCADTest(OBSERVING+MASTER_ENABLE);
    
		strncpy(fname, (char *)pcad->a, MAX_STRING_SIZE-1);
		if (fname[0])
		{

			if(strcmp(fname,XDISP1) != 0)
				if(strcmp(fname,XDISP2) != 0)
					if(strcmp(fname,XDISP3) != 0)
						if(strcmp(fname,XDISP4) != 0)
							if(strcmp(fname,XDISP5) != 0)
								if(strcmp(fname,XDISP6) != 0)
								{
									sprintf(err,"Xdisp name must be %s, %s, %s, %s,%s or %s\n",XDISP1,XDISP2,XDISP3,XDISP4,XDISP5,XDISP6);
									strncpy(CADMESS,err,MAX_STRING_SIZE-1);
									printf(err);
									status = CAD_REJECT;
								}
		}

		if (status == CAD_REJECT)
		{
			strncpy(buff, "Unknown xdisp name (A): ", MAX_STRING_SIZE-1);
			strncat(buff, fname, MAX_STRING_SIZE - strlen(buff));
			strncpy(CADMESS, buff, MAX_STRING_SIZE-1);
			return(status);
		}

	
	

		/* Output the barcode values for further checking by CC */

		strncpy((char *)pcad->vala, fname, MAX_STRING_SIZE-1);
		strncpy((char *)pcad->valh, fname, MAX_STRING_SIZE-1);
		break ;

	  case CAD_START:

		/* xdisps are OK, so output focus offsets and original filter names */

		*(double *) pcad->valc = focusOffset;
		*(double *) pcad->vale = lambdaOffset;
		strncpy(pcad->valg, barcodeId, MAX_STRING_SIZE-1);
		status = CAD_ACCEPT;
   
		DBGMSG(DBG_FULL,"CAD fltPos. Output values A..H:");
		DBGMSGSTRING(DBG_FULL,"A: ", (char *)pcad->vala);
		DBGMSGREAL(DBG_FULL,"C: ",*(double *)pcad->valc);
		DBGMSGREAL(DBG_FULL,"E: ",*(double *)pcad->vale);
		DBGMSGSTRING(DBG_FULL,"G: ",pcad->valg);
		DBGMSGSTRING(DBG_FULL,"H: ", (char *)pcad->valh);
   
		break;

	  case CAD_MARK :

		status = CAD_ACCEPT;
		break ;

	  case CAD_STOP :

		status = CAD_REJECT;
		strncpy(CADMESS, "STOP directive not implemented", MAX_STRING_SIZE-1);
		break ;

	  case CAD_CLEAR :

		status = CAD_ACCEPT;
		break ;
	}

	return(status);
}
