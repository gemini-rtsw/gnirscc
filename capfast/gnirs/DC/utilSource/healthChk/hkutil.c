static struct {
        void *v;
        char *c;
} rcsid = {
        &rcsid,
        "$Id: hkutil.c,v 1.2 2009/05/27 19:33:33 fkraemer Exp $"
};
extern int healthdebug;
/*
 * Copyright 1997 Association of Universities for Research in Astronomy, Inc.
 * See the file COPYRIGHT for more details.
 *
 * FILENAME
 * hkutil.c
 *
 * DESCRIPTION
 * This file contains the source for all the utility functions used by the
 * NAAC system Housekeeping CAD records. These functions are used to  
 * validate the arguments given to the CAD record for the housekeeping 
 * variables and to initalize related data structures
 * 
 * 
 * FUNCTION NAME(S)
 * initHkArray - initalize the array holding expected voltages and voltage limits
 * procInput   - process input - assign values, check voltages
 * setVal      - set the values of input on output channels
 * checkStatus - check status of voltages
 *   
 * DEPENDENCIES
 * The ordering of the arguments within each CAD record (A, B, C...) is
 * defined in the description of the interface between the CAD database
 * and its clients. Changes to that interface should be reflected in this
 * file.
 *
 * The input arguments for each CAD record (A, B, C...) are all strings
 * and must be converted to their appropriate data types before validation.
 * However, the data type of each output argument (VALA, VALB, VALC...)
 * is determined by the (FTVA, FTVB, FTVC....) fields in the CAD record.
 * The data types assumed here must match those declared in the CAD
 * record properties.
 *
 *INDENT-OFF*
 * $Log: hkutil.c,v $
 * Revision 1.2  2009/05/27 19:33:33  fkraemer
 * fkraemer - copied my complete working dir over trunk
 *
 * Revision 1.1.1.1  1998/12/15 16:18:47  buchholz
 * Imported gnaacSrc into CVS
 *
 * Revision 1.2  1998/11/20 17:16:27  pruckle
 * speed up saver, log messages
 *
 * Revision 1.1.1.1  1998/09/30 16:40:23  pruckle
 * Initial Release
 *
 *INDENT-ON* 
 */
#include <ctype.h>

#include <epCommon.h>
#include "epTypedefs.h"
extern char *dbTop;

/* global variable to contain HK channel limits info */
hkLimits_t hkChanDesc[MAX_HK_CHANNELS][NUM_HK_INPUTS];

/*****************************************************************************
 *FUNCTION NAME:
 *	initHkArray
 *
 *INVOCATION:
 *	status = initHkArray( parfile );
 *
 *PARAMETERS: 
 *	parfile - char * - full path name of default values parameter file.
 *
 *FUNCTION VALUE:
 *	int - Success or failure of initalization (PASS | ERROR)
 *
 *PURPOSE:
 *	Initalize the array holding expected voltages and voltage limits
 *
 *DESCRIPTION:
 *	This routine must be called to initalize the housekeeping array that
 *	holds the ranges of valid voltages. These values are obtained from the
 *	file named by the "parfile" parameter.
 *
 *EXTERNAL VARIABLES:
 *	hkChanDesc - static global to this file.
 *
 *PRIOR REQUIREMENTS:
 *	None
 *
 *DEFICIENCIES:
 *	None
 *
 *HISTORY:
 * 17-Jan-1997  Original template adapted from CICS alpha 1.0   J.E. Tvedt
 * 03-Feb-1997  Original function   T. Morgan
 *
 */
int initHkArray(char *parfile)
{
    FILE *hkpars;
    long status,val,oldVal;
    char inbuffer[256], tmpstr[256],errMess[80];
    int i, j, chan, input, lines, retval, group, method, cnt, ccnt;
    double base, warn_max, warn_min, bad_max, bad_min;

  /* open housekeeping parameters file */
    hkpars = fopen(parfile, "r" );
    if (hkpars == NULL) 
    {
	cicsLogString(1, "initHkArray - ERROR - unable to open housekeeping parameter file", parfile);
	return ERROR;
    }

    /* initialize variables, place array in state indicating invalid data 
     (channel = -1) */
    input = ccnt = 0;
    retval = PASS;
  
    for (i = 0; i < MAX_HK_CHANNELS; i++)
	for (j = 0; j < NUM_HK_INPUTS; j++)
	{
	
	    hkChanDesc[i][j].group  = i;
	    hkChanDesc[i][j].channel  = j;
	    hkChanDesc[i][j].method  = 0;
	    hkChanDesc[i][j].base     = 0;
	    hkChanDesc[i][j].warn_min = 0;
	    hkChanDesc[i][j].warn_max = 0;
	    hkChanDesc[i][j].bad_min  = 0;
	    hkChanDesc[i][j].bad_max  = 0;
	}

    /* read housekeeping parameters & place in array */
    lines = 0;
    val = 0;
    status = getDbInfoT(dbTop, OVERALLHLTH ".SCAN",errMess,DBF_LONG,&oldVal);  
   
  
    while (!feof(hkpars))
    {
	if (fgets(inbuffer, 256, hkpars) != NULL)
	{
	    lines++;
	    if (inbuffer[0] == '#' || isspace(inbuffer[0]))
		/* ignore comments (lines begin with a # */
		continue;
	    else
	    {
		ccnt++;
		cnt = sscanf(inbuffer, "%d %d %d %lf %lf %lf %lf %lf",
			     &group, &chan, &method, &base ,&warn_max,
			     &warn_min, &bad_max, &bad_min);
		if (cnt == 8)
		{
		    /* set alarm values for epics records*/
		    hkChanDesc[group][chan].group    = group;
		    hkChanDesc[group][chan].channel  = chan;
		    hkChanDesc[group][chan].method  = method; 
		    if (method >0)
		    {  
			status = setAlarm(group,chan, base+warn_min, base+warn_max, base+bad_min, base+bad_max);  
			hkChanDesc[group][chan].base     = base;
			hkChanDesc[group][chan].warn_min = warn_min;
			hkChanDesc[group][chan].warn_max = warn_max;
			hkChanDesc[group][chan].bad_min  = bad_min;
			hkChanDesc[group][chan].bad_max  = bad_max;
		    }
		}
		else if (cnt != 3)
		{
		    sprintf(tmpstr, "initHkArray - ERROR - format error on "
			    "line %d of housekeeping parameter file %s", lines, parfile);
		    cicsLogMessage(0,tmpstr);
		    retval = ERROR;
		    break;
		}
	    }
	}
	else
	{
	    if (ccnt == 256)
	    {
		retval = PASS;
		break;
	    }
	    else
	    {
		sprintf(tmpstr, "initHkArray - ERROR - error on line %d "
			"of housekeeping parameter file %s", lines, parfile);
		cicsLogMessage(0,tmpstr);
		retval = ERROR;
		break;
	    }
	}
    }
   
    fclose(hkpars);
    return retval;
}
	

/*****************************************************************************
 *FUNCTION NAME:
 *	procInput
 *
 *INVOCATION:
 * 	status = procInput(a, vala, ftva, channel, input);
 *
 *PARAMETERS:
 * 	a - char * - pointer to character string holding value
 * 	vala - void * - pointer to value after convertion
 * 	ftva - unsigned short - one of an enumeration of EPICS DB types
 * 	channel - int - which of 256 sets of input ranges to use
 * 	input - int - which of the i inputs for channel to process
 *
 *FUNCTION VALUE:
 *	long - status (STATUS_GOOD || STATUS_WARNING ||
 *				STATUS_BAD || STATUS_ERROR)
 *
 *PURPOSE:
 *	process input from file assign values, default voltages etc.
 *
 *DESCRIPTION:
 *	This routine converts the character string passed to it to the 
 *	type specified by ftva and returns the value via the void pointer. 
 *	The channel & input numbers are used to retrieve the valid ranges
 * 	from the static global hkChanDesc array.
 *
 *EXTERNAL VARIABLES:
 *	hkChanDesc - static global to this file.
 *
 *PRIOR REQUIREMENTS:
 *	Initialization of hkChanDesc with initHkArray.
 *
 *DEFICIENCIES:
 *	None
 *
 *HISTORY:
 * 17-Jan-1997  Original template adapted from CICS alpha 1.0   J.E. Tvedt
 * 03-Feb-1997  Original function   T. Morgan
 *
 */
int procInput(int channel, int input)
{
    long tmpStat;
  
#if 0
    tmp = malloc(dbr_sizeof(ftva));

    setVal(a, tmp, ftva);
    /* if channel is -1 it is not used*/
    if (hkChanDesc[channel][input].method != 0) 
	tmpStat = checkStatus(tmp, ftva, hkChanDesc[channel][input]);
    else
	tmpStat = STATUS_ERROR;

    switch (tmpStat) 
    {
      case STATUS_GOOD:
	  sprintf(tmpstr, "checkHk - GOOD - Channel %d Input %d Value %f",
		  channel, input, *(float *) tmp);
	  cicsLogMessage(3,tmpstr);
	  memcpy(vala, tmp, dbr_sizeof(ftva));
	  break;

      case STATUS_WARNING:
	  sprintf(tmpstr, "checkHk - WARNING - Channel %d Input %d Value %f",
		  channel, input, *(float *) tmp);
	  cicsLogMessage(0,tmpstr);
	  break;
      
      case STATUS_BAD:
	  sprintf(tmpstr, "checkHk - BAD - Channel %d Input %d Value %f",
		  channel, input, *(float *) tmp);
	  cicsLogMessage(0,tmpstr);
	  break;

      case STATUS_ERROR:
      default:
	  sprintf(tmpstr, "checkHk - ERROR - Channel %d Input %d Value %f",
		  channel, input, *(float *) tmp);
	  cicsLogMessage(0,tmpstr);
	  break;
    }

    free(tmp);
#endif
  return tmpStat;
}


/*****************************************************************************
 *FUNCTION NAME:
 *	setVal
 *
 *INVOCATION:
 *	status = setVal(a, vala, ftva);
 *
 *PARAMETERS:
 * 	a - char * - pointer to character string holding value
 * 	vala - void * - pointer to value after convertion
 * 	ftva - unsigned short - one of an enumeration of EPICS DB types
 *
 *FUNCTION VALUE:
 * 	None
 *
 *PURPOSE:
 * 	copy value into allocated memory space
 *
 *DESCRIPTION:
 *	This routine converts the character string passed to it to the type
 *	specified by ftva and returns the converted value via the void
 * 	pointer. 
 *
 *EXTERNAL VARIABLES:
 *	None
 *
 *PRIOR REQUIREMENTS:
 *	None
 *
 *DEFICIENCIES:
 * 	does not work with  DBR_UCHAR, DBR_SHORT, DBR_USHORT, DBR_ULONG
 * 
 *
 * HISTORY:
 * 17-Jan-1997  Original template adapted from CICS alpha 1.0   J.E. Tvedt
 * 03-Feb-1997  Original function   T. Morgan
 *
 */
void setVal(char *a, void *vala, unsigned short ftva)
{

    switch (ftva) 
    {
      case DBR_STRING:
	  strncpy(vala, a, MAX_STRING_SIZE);
	  break;

      case DBF_CHAR:
	  *(dbr_char_t *)vala = (dbr_char_t) a[0];
	  break;

#if 0
      case DBR_INT:
	  *(dbr_int_t *)vala = (dbr_int_t) atoi(a);
	  break;

      case DBF_UCHAR: 
	  *(dbr_char_t *)vala = (dbr_char_t) abs(a[0]); 
	  break; 

      case DBR_SHORT: 
	  *(dbr_short_t *)vala = (dbr_short_t) atoi(a); 
	  break; 

      case DBR_USHORT: 
	  *(dbr_ushort_t *) vala = (dbr_ushort_t) abs(atoi(a)); 
	  break; 

      case DBR_ULONG: 
	  *(dbr_long_t *) vala = (dbr_long_t) abs(atol(a)); 
	  break; 
#endif

      case DBR_ENUM:
	  *(dbr_enum_t *) vala = (dbr_enum_t) abs(atoi(a));
	  break;

      case DBR_LONG:
	  *(dbr_long_t *) vala = (dbr_long_t) atol(a);
	  break;

      case DBR_FLOAT:
	  *(dbr_float_t *) vala = (dbr_float_t) atof(a);
	  break;

      case DBR_DOUBLE:
	  *(dbr_double_t *) vala = (dbr_double_t) atof(a);
	  break;
    }

}


/*****************************************************************************
 *FUNCTION NAME:
 *	dbr_sizeof
 *
 *INVOCATION:
 *	size = dbr_sizeof(ftva);
 *
 *PARAMETERS:
 * 	ftva - unsigned short - one of an enumeration of EPICS DB types
 *
 *FUNCTION VALUE:
 * 	size in bytes of the EPICS type
 *
 *PURPOSE:
 *	 to determine how much storage space is used by the EPICS type
 *
 *DESCRIPTION:
 * 	This routine takes the type value (as defined in <epicsType.h>) and
 *	returns the size of this type in bytes.
 *
 *EXTERNAL VARIABLES:
 * None
 *
 *PRIOR REQUIREMENTS:
 * None
 *
 *DEFICIENCIES:
 *
 *HISTORY:
 * 17-Jan-1997  Original template adapted from CICS alpha 1.0   J.E. Tvedt
 * 03-Feb-1997  Original function   T. Morgan
 * 25-Oct-1997	revised comments and cleaned up - ncb
 *
 ****************************************************************************/
int dbr_sizeof(unsigned short ftva)
{
    int retval;

    switch (ftva) 
    {
      case DBR_STRING:
	  retval = sizeof(dbr_char_t) * MAX_STRING_SIZE;
	  break;

      case DBF_CHAR:
#if 0
      case DBR_UCHAR: 
#endif
	  retval = sizeof(dbr_char_t);
	  break;

#if 0
      case DBR_INT:
	  retval = sizeof(dbr_int_t);
	  break;

      case DBR_SHORT: 
	  retval = sizeof(dbr_short_t); 
	  break; 
      case DBR_USHORT: 
	  retval = sizeof(dbr_ushort_t); 
	  break; 
#endif

      case DBR_ENUM:
	  retval = sizeof(dbr_enum_t);
	  break;

      case DBR_LONG:
#if 0
      case DBR_ULONG: 
#endif
	  retval = sizeof(dbr_long_t);
	  break;

      case DBR_FLOAT:
	  retval = sizeof(dbr_float_t);
	  break;

      case DBR_DOUBLE:
	  retval = sizeof(dbr_double_t);
	  break;

      default:
	  retval = 0;
    }

    return retval;
}


/*****************************************************************************
 *FUNCTION NAME: 
 *	checkStatus
 *
 *INVOCATION: 
 *	status = checkStatus(val, ftv, vs);
 *
 * PARAMETERS: 
 * 	val - void * - value to compare to the structure values
 * 	ftv - unsigned short - one of an enumeration of EPICS DB types
 * 	vs - struct valStruct - housekeeping channel limits structure
 *
 * FUNCTION VALUE:
 * 	long - status - (STATUS_GOOD | STATUS_WARNING |
 *				STATUS_BAD | STATUS_ERROR)
 *
 * PURPOSE:
 * 	check status of housekeeping channels 
 *
 * DESCRIPTION:
 * 	This routines compares the value of val with the central value & limits
 * 	for it that are stored in the valStruct. Currently implemented to work
 *	with doubles & longs, the types being used that this form of
 *	comparison are valid for. Checks to see if it is within the Warning or
 *	Bad ranges.
 *
 * EXTERNAL VARIABLES:
 * None
 *
 * PRIOR REQUIREMENTS:
 * None
 *
 * DEFICIENCIES:
 * None
 *
 * HISTORY:
 * 17-Jan-1997  Original template adapted from CICS alpha 1.0   J.E. Tvedt
 * 03-Feb-1997  Original function   T. Morgan
 * 25-Oct-1997	revised comments and cleaned up - ncb
 *
 ****************************************************************************/

int checkStatus(double val,  hkLimits_t vs)
{
    long retval = STATUS_GOOD;
#if 0
    long lLoBad, lHiBad, lLoWarn, lHiWarn;
    double dLoBad, dHiBad, dLoWarn, dHiWarn;

    switch (ftv) 
    {
      case DBR_STRING:
	  retval = STATUS_ERROR;
	  break;

      case DBR_LONG:
	  lLoBad = (dbr_long_t)vs.base + (dbr_long_t)vs.bad_min;
	  lHiBad = (dbr_long_t)vs.base + (dbr_long_t)vs.bad_max; 
	  lLoWarn = (dbr_long_t)vs.base + (dbr_long_t)vs.warn_min;
	  lHiWarn = (dbr_long_t)vs.base + (dbr_long_t)vs.warn_max;
	  if (*(dbr_long_t*)val <= lLoBad || *(dbr_long_t*)val >= lHiBad)
	  {
	      retval = STATUS_BAD;
	  }
	  else if (*(dbr_long_t*)val <= lLoWarn || *(dbr_long_t*)val >= lHiWarn)
	  {
	      retval = STATUS_WARNING;
	  }  
	  break;

      case DBR_DOUBLE:
	  dLoBad = (dbr_double_t)vs.base + (dbr_double_t)vs.bad_min;
	  dHiBad = (dbr_double_t)vs.base + (dbr_double_t)vs.bad_max; 
	  dLoWarn = (dbr_double_t)vs.base + (dbr_double_t)vs.warn_min;
	  dHiWarn = (dbr_double_t)vs.base + (dbr_double_t)vs.warn_max;
	  if (*(dbr_double_t*)val <= dLoBad || *(dbr_double_t*)val >= dHiBad)
	  {
	      retval = STATUS_BAD;
	  }
	  else if (*(dbr_double_t*)val <= dLoWarn || 
		   *(dbr_double_t*)val >= dHiWarn)
	  {
	      retval = STATUS_WARNING;
	  }
	  break;

      default:
	  retval = STATUS_ERROR;
    }
#endif
    return retval;
}
void printdef()
{
    int i,j;
    printf ("  group  Chan  method     base  warn_min  warn_max  bad_min  bad_max\n\n");
    for (i=0;i<MAX_HK_CHANNELS ;i++)
	for (j=0;j<NUM_HK_INPUTS ;j++)
	{

	    printf ("%5d   %5d",hkChanDesc[i][j].group ,hkChanDesc[i][j].channel);
	    printf ("  %5d  ", hkChanDesc[i][j].method);
	    printf ("  %7.3f  %7.3f  %7.3f", hkChanDesc[i][j].base, hkChanDesc[i][j].warn_min, hkChanDesc[i][j].warn_max);
	    printf ("  %7.3f  %7.3f\n", hkChanDesc[i][j].bad_min, hkChanDesc[i][j].bad_max);
	}

}
