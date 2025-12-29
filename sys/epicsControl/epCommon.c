static struct {
	void *v;
	char *c;
} rcsid = {
	&rcsid,
	"$Id: epCommon.c,v 1.2 2013/06/06 01:54:28 gemvx Exp $"
};
/*
 * Copyright 1997 Association of Universities for Research in Astronomy, Inc.
 * See the file COPYRIGHT for more details.
 *
 * FILENAME
 * epCommon.c
 *
 * DESCRIPTION (optional)
 * This file contains general purpose functions for the EPICS support 
 * software of the GNAAC system. 
 *
 * FUNCTION NAME(S)
 * check_input - checks input against a specified range and converts to
 *               specified data type
 * cvt         - converts string to specified data type
 * getDbInfo   - gets the value of the specified field of an EPICS 
 *			record
 * putDbInfo   - puts a value into the specified field of an EPICS 
 *			record
 * carStatus   - gets value from a CAR record and converts to one of 
 *			the CAR status values
 * processRec  - processes the specified EPICS record
 * printCadVals- prints the specified number of CAD VALx outputs using
 *		       the specified debug level
 * assignVal   - performs an assignment of values of arbitrary data 
 *			type and checks for possible memory allocation 
 *                       problem

long setCad(struct cadRecord* pCad,char *line)
 * updateStateHealth - updates status and health records
 * setCar - sets IERR, IVAL and IMSS fields of car record
 *
 *INDENT-OFF*
 * $Log: epCommon.c,v $
 * Revision 1.2  2013/06/06 01:54:28  gemvx
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
 * Revision 1.1  2009/06/10 15:05:11  gemvx
 * Added Files:
 * epicsControl diectory populated.
 *
 *INDENT-ON* 
 */


#include <epCommon.h>
#include <car.h>

/* long motorCadInit( struct cadRecord* pCad ) */
/* { */
/*     return CAD_ACCEPT; */
/* } */



/*
 *+
 * FUNCTION NAME:
 * check_input
 *
 * INVOCATION:
 * unsigned short type;
 * char *in;
 * long min, max, out, status;
 *
 * status = check_input (type, in, &min, &max, &out)
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * > type      	(unsiged short)	 data type to convert input into
 * > in		(char *)	 pointer to string containing input value
 * > min	(void *)         pointer to minimum value allowed
 * > max	(void *)	 pointer to maximum value allowed
 * < out	(void *)         pointer to output value
 *
 * FUNCTION VALUE:
 * (int)
 * OK            if successful
 * ERROR_TYPE    if data type is incorrect or not recognized
 * ERROR_RANGE   if input is outside the specified range
 * ERROR_MEMORY  if location of output is NULL
 *
 * PURPOSE:
 * General purpose routine for validating a parameter of a specified data type.
 *
 * DESCRIPTION:
 * This function checks that memory was successfully allocated since the CAD
 * record does not do this.  The function then converts the input to the
 * specified data type if it falls within the specified range of values.
 *
 * EXTERNAL VARIABLES:
 * cicsLogMessage - CICS logging function
 *
 * PRIOR REQUIREMENTS:
 * Sufficient memory must be allocated to store the output value before
 * this routine is called.
 *
 * DEFICIENCIES:
 * None known.  
 *
 * HISTORY (optional):
 * 01-Jan-1997  Original version. 			Peter Ruckle
 * 13-Mar-1997  Added check for NULL pointer.           Janet Tvedt
 *
 *-
 */
int check_input (unsigned short type, char *in, void *min, void *max,
		 						void *out)
{
    char inval[8];  /* input converted to temp value of specified data type */
    int ret;        /* return value */

    /* Check for a NULL pointer signifying error in memory allocation */
    if(out == NULL)
    {
	cicsLogMessage(0, "Fatal error - NULL pointer found");
	return ERROR_MEMORY; /**** this should never happen ****/
    }

    /* Convert the input to the specified data type and copy to the output
     * if the converted value is within the specified range.  
    */
    switch (type) 
    { 
      case DBF_STRING: 
	  return ERROR_TYPE;
      case DBF_CHAR: 
	  return ERROR_TYPE;
      case DBF_UCHAR: 
	  return ERROR_TYPE;
      case DBF_SHORT: 
	  if (ret = cvt (type,in,inval)!=OK)
	      return ret;
	  if ((*(short *)inval < *(short *)min) ||
	      			(*(short *)inval > *(short *)max) )
	      return ERROR_RANGE;
	  else
	  {
	      *(short *)out = *(short *)inval;
	      return OK;
	  }
      case DBF_USHORT: 
	  if (ret = cvt (type,in,inval)!=OK)
	      return ret;
	  if ((*(unsigned short *)inval < *(unsigned short *)min) ||
	       		  (*(unsigned short *)inval > *(unsigned short *)max))
	      return ERROR_RANGE;
	  else
	  {
	      *(unsigned short *)out = *(unsigned short *)inval;
	      return OK;
	  }
      case DBF_LONG: 
	  if (ret = cvt (type,in,inval)!=OK)
	      return ret;
	  if ((*(long *)inval < *(long *)min) ||
	      				(*(long *)inval > *(long *)max))
	      return ERROR_RANGE;
	  else
	  {
	      *(long *)out = *(long *)inval;
	      return OK;
	  }
      case DBF_ULONG: 
	  if (ret = cvt (type,in,inval)!=OK)
	      return ret;
	  if (( *(unsigned long *)inval < *(unsigned long *)min) ||
	      		     (*(unsigned long *)inval > *(unsigned long *)max))
	      return ERROR_RANGE;
	  else
	  {
	      *(unsigned long *)out = *(unsigned long *)inval;
	      return OK;
	  }
      case DBF_FLOAT: 
	  	  if (ret = cvt (type,in,inval)!=OK)
	      return ret;
	  if ((*(float *)inval < *(float *)min) ||
	      			(*(float *)inval > *(float *)max))
	      return ERROR_RANGE;
	  else
	  {
	      *(float *)out = *(float *)inval;
	      return OK;
	  }
      case DBF_DOUBLE:
 	  if (ret = cvt (type,in,inval)!=OK)
	      return ret;
	  if ((*(double *)inval < *(double *)min) ||
	      				(*(double *)inval > *(double *)max))
	      return ERROR_RANGE;
	  else
	  {
	      *(double *)out = *(double *)inval;
	      return OK;
	  }
      case DBF_ENUM: 
	  return ERROR_TYPE;
      case DBF_GBLCHOICE:
	  return ERROR_TYPE;
      case DBF_CVTCHOICE: 
	  return ERROR_TYPE;
      case DBF_RECCHOICE: 
	  return ERROR_TYPE;
      case DBF_DEVCHOICE: 
	  return ERROR_TYPE;
      case DBF_INLINK:
	  return ERROR_TYPE;
      case DBF_OUTLINK:  
	  return ERROR_TYPE;
      case DBF_FWDLINK: 
	  return ERROR_TYPE;
      case DBF_NOACCESS: 
	  return ERROR_TYPE;
      default: 
	  return ERROR_TYPE;
    } 
 
    return ERROR_TYPE;
}

/*
 *+
 * FUNCTION NAME:
 * cvt
 *
 * INVOCATION:
 * unsigned short type;
 * char *in;
 * double *out;
 * long status;
 *
 * status = cvt(type, in, out)
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * > type   (unsigned short)    data type to convert to
 * > in     (char *)		pointer to string containing input value
 * < out    (void *)		pointer to converted output value
 *
 * FUNCTION VALUE:
 * (int)
 * OK           if successful
 * ERROR_TYPE   if unrecognized data type
 *
 * PURPOSE:
 * convert the char in in to the type in type and place it in out
 *
 * DESCRIPTION:
 * This function checks that the output pointer is not NULL.  This check
 * is needed here as well as in the check_input function because this
 * function may be called by functions other than check input.  If the
 * output pointer is not NULL the function coverts the input string to 
 * the specified data type.
 *
 * EXTERNAL VARIABLES:
 * cicsLogMessage  -  CICS logging function
 *
 * PRIOR REQUIREMENTS:
 * Sufficient memory must be allocated to hold the output value before 
 * this routine is called.
 *
 * DEFICIENCIES:
 * None
 *
 * HISTORY (optional):
 * 01-Jan-1997  Original version.				Peter Ruckle
 * 13-Mar-1997  Added NULL pointer check.			Janet Tvedt
 *
 *-
 */
int cvt(unsigned short type, char *in, void *out)
{
 /*    DPRINT(10,0,"routine cvt\n"); */
    /* Check for a NULL pointer signifying error in memory allocation */
    if(out == NULL)
    {
	cicsLogMessage(0, "Fatal error - NULL pointer found");
    printf("type = %d out error\n",type);
	return(ERROR_MEMORY); /**** this should never happen ****/
    }
  /*   printf("type = %d\n",type); */
    switch (type) 
    { 
      case DBF_STRING: 
	  return ERROR_TYPE;
      case DBF_CHAR: 
	  return ERROR_TYPE;
      case DBF_UCHAR: 
	  return ERROR_TYPE;
      case DBF_SHORT:
	  *(short *)out = (short)atoi(in);
	  return OK;
      case DBF_USHORT:
	  *(unsigned short *)out = (unsigned short)atoi(in);
	  return OK;
      case DBF_LONG: 
	  *(long *)out = (long)atol(in);
/* 	printf("cvt long string %s val %d\n",in,*(long*)out); */
	  return OK;
      case DBF_ULONG: 
	  *(unsigned long *)out = (unsigned long)atoi(in);
	  return OK;
      case DBF_FLOAT: 
	  *(float *)out = (float)atof(in);
	  return OK;
      case DBF_DOUBLE: 
	  *(double *)out = (double)atof(in);
	  return OK;
      case DBF_ENUM: 
	  return ERROR_TYPE;
      case DBF_GBLCHOICE:
	  return ERROR_TYPE;
      case DBF_CVTCHOICE: 
	  return ERROR_TYPE;
      case DBF_RECCHOICE: 
	  return ERROR_TYPE;
      case DBF_DEVCHOICE: 
	  return ERROR_TYPE;
      case DBF_INLINK:
	  return ERROR_TYPE;
      case DBF_OUTLINK:  
	  return ERROR_TYPE;
      case DBF_FWDLINK: 
	  return ERROR_TYPE;
      case DBF_NOACCESS: 
	  return ERROR_TYPE;
      default: 
	DPRINT(DPdebug,0,"cvt bad type\n");
	  return ERROR_TYPE;
    } 
 
    return ERROR_TYPE;
}


/*
 *+
 * FUNCTION NAME:
 * getDbInfo
 *
 * INVOCATION:
 * long status;
 * char *fieldName;  
 * char *errMess;
 * unsigned short type;
 * double *outVal;
 *
 * status = getDbInfo(fieldName, errMess, type, &outVal);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * > fieldName (char *)               pointer to reocord_name.field_name
 * ! errMess   (char *)               pointer to string
 * > type      (unsigned short)       data type to convert output to 
 *                                    (DBF_DOUBLE, DBF_LONG, etc.)
 * < outval    (void *)               pointer to output value
 *
 * FUNCTION VALUE:
 * long  - status value returned to calling routine, a non-zero value indicates an error
 *
 * PURPOSE:
 * Function to get a value from an EPICS database record.
 *
 * DESCRIPTION:
 * This routine is called to obtain the value of the specified field 
 * within an EPICS database record.  It handles potential errors by
 * sending messages to the CICS logging functions and returning an
 * error emssage and status value to the calling routine.
 *
 * EXTERNAL VARIABLES:
 * dbNameToAddr    - EPICS database access routine for finding record address
 * dbGet           - EPICS database access routine for retrieving the data
 * cicsLogMessage  - CICS logging function for a message
 * cicsLogString   - CICS logging function for a message + a string
 *
 * PRIOR REQUIREMENTS:
 * None.
 * 
 * DEFICIENCIES:
 * I have been informed that the EPICS community are now encouraging the
 * use of "recGblGetLinkValue" instead of "dbGetField", since
 * "recGblGetLinkValue" decides whether to use Channel Access or Database
 * Access according to the circumstances. However, "recGblGetLinkValue"
 * is very much more difficult to use, and I don't understand the description
 * of this function in the "EPICS IOC Application Developers Guide".
 *                                                                 Steven Beard.
 *
 * HISTORY (optional):
 * 13-Mar-1997  Original version				   Janet Tvedt
 *
 *-
 */
long getDbInfoT(char *top,char *fieldName, char *errMess, unsigned short type, void *outVal)
{
  char n[80];


  strcpy(n,top);
  strcat(n,fieldName);
  return getDbInfo(n,errMess,type,outVal);

}

long getDbInfo(char *fieldName, char *errMess, unsigned short type, void *outVal)
{
    struct dbAddr addr;
    long ret, options=0L, nRq = 1L;
    long status;
    char err[80];
    status = OK;

   /*  printf("get:%s\n", fieldName); */

    /* Get the address of the data structure  and handle any errors */
    if( (ret = dbNameToAddr (fieldName,&addr)) != 0L)
    {
	status = ERROR_EPICS;
	sprintf(err, "ERR dNTA >%s< %ld", fieldName, ret);
	strncpy (errMess,err,MAX_STRING_SIZE);
	cicsLogMessage(2, errMess);
	cicsLogString(2, "dbName = ", fieldName);
    }

    /* If address found, get the data.  Handle any errors. */
    if( status == OK )
    {
       if( (ret = dbGetField(&addr, type, outVal, &options, &nRq, NULL)) != 0L)
       {
	   status = ERROR_EPICS;
           sprintf(err, "dbGet error = %ld", ret);
	   strncpy (errMess,err,MAX_STRING_SIZE);
	   cicsLogMessage(2, errMess);
       }
    }

    /* Return error status */
    return status;
}

/*
 *+
 * FUNCTION NAME:
 * putDbInfo
 *
 * INVOCATION:
 * char *fieldName;
 * char *errMess;
 * unsigned short type;
 * double outVal;
 * long status;
 *
 * status = putDbInfo(fieldName, errMess, DBF_DOUBLE, &outVal)
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * > fieldName (char *)           pointer to record+field name
 * ! errMess   (char *)           pointer to string
 * > type      (unsigned short)   data type of value to put
 * > outVal    (void *)           pointer of data to put
 *
 * FUNCTION VALUE:
 * long - Status - a non-zero value indicates an error
 *
 * PURPOSE:
 * Function to put a value into the specified field of an EPICS database record
 *
 * DESCRIPTION:
 * This routine may be called by a user subroutine to put a value
 * into an EPICS database record field.  The complete field name must
 * be specified (for example: sytem:subsystem:recordx.FIELDY). This function
 * will also handle errors by logging them through the CICS logging functions
 * and by returning an error message and status value to the calling routine.
 *
 * EXTERNAL VARIABLES:
 * None
 *
 * PRIOR REQUIREMENTS:
 * None
 *
 * DEFICIENCIES:
 * I have been informed that the EPICS community are now encouraging the
 * use of "recGblPutLinkValue" instead of "dbPutField", since
 * "recGblPutLinkValue" decides whether to use Channel Access or Database
 * Access according to the circumstances. However, "recGblPutLinkValue"
 * is very much more difficult to use, and I don't understand the description
 * of this function in the "EPICS IOC Application Developers Guide".
 *                                                                 Steven Beard.
 * HISTORY (optional):
 * 19-Mar-1997  Original version.				Janet Tvedt
 *
 *-
 */
long putDbInfoT(char *top,char *fieldName, char *errMess, unsigned short type, void *outVal)
{
  char n[80];


  strcpy(n,top);
  strcat(n,fieldName);
  return putDbInfo(n,errMess,type,outVal);

}

long putDbInfo(char *fieldName, char *errMess,
	       			unsigned short type, void *outVal)
{
    struct dbAddr addr;
    long ret, nRq = 1L;
    long status;

    char errNote[80];
    
    status = OK;

    /* Get the address of the data structure  and handle any errors */
    if( (ret = dbNameToAddr (fieldName,&addr)) != 0L)
    {
	sprintf(errNote,"dbNameToAddr failed for %s.\n", fieldName);
	DPRINT(DPdebug,0,errNote);
	status = ERROR_EPICS;
	sprintf(errNote, "ERR dNTA >%s< %ld", fieldName, ret);
	strncpy (errMess,errNote,MAX_STRING_SIZE);
	cicsLogMessage(2, errMess);
	cicsLogString(2, "dbName = ", fieldName);
    }
    /* If address found, write the data.  Handle any errors. */
    if( status == OK )
    {
       if( (ret = dbPutField(&addr, type, outVal, nRq)) != 0L)
       {
	   DPRINT(DPdebug,0,"dbPutField failed\n");
	   status = ERROR_EPICS;
           sprintf(errNote, "dbPutField error = %ld", ret);
	   strncpy (errMess,errNote,MAX_STRING_SIZE);
	   cicsLogMessage(2, errMess);
       }
    }
    /* Return error status */
    return status;
}

/*
 *+
 * FUNCTION NAME:
 * carStatus
 *
 * INVOCATION:
 * char *input;
 * long status;
 *
 * status = carStatus(input)
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * > input   (char *)   pointer to string containing the CAR status
 *
 * FUNCTION VALUE:
 * long  CAR value (idle, busy, paused, error)
 *
 * PURPOSE:
 * Convert string CAR value to corresponding long integer.
 *
 * DESCRIPTION:
 * This routine is called by functions needing to convert the strings
 * IDLE, BUSY, PAUSED or ERROR to a long integer CAR_IDLE, CAR_BUSY,
 * CAR_PAUSED, CAR_ERROR. 
 *
 * EXTERNAL VARIABLES:
 * None
 *
 * PRIOR REQUIREMENTS:
 * None
 *
 * DEFICIENCIES:
 * None known
 *
 * HISTORY (optional):
 * 21-Apr-1997  Original version.		 J.E. Tvedt
 *
 *-
 */

long carStatus(char *input)
{
    long status;

    if(strcmp(input,"IDLE") == 0)
	status = CAR_IDLE;
    else if(strcmp(input,"PAUSED") == 0)
	status = CAR_PAUSED;
    else if(strcmp(input,"BUSY") == 0)
	status = CAR_BUSY;
    else if(strcmp(input,"ERROR") == 0)
	status = CAR_ERROR;
    else
	status = -1;
    return status;
}


/*
 *+
 * FUNCTION NAME:
 * processRec
 *
 * INVOCATION:
 * char *prefix;
 * char *record;
 * long status;
 *
 * status = processRec(prefix, record);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * > prefix   (char *)   pointer to string containing db prefix
 * > reocrd   (char *)   pointer to string containing db reocrd name
 *
 * FUNCTION VALUE:
 * long  Status value, 0 indicates success
 *
 * PURPOSE:
 * Process the specified EPICS record
 *
 * DESCRIPTION:
 * This routine constructs the complete record name from the input
 * parameters and calls the appropriate db routines to cause that
 * record to be processed.
 *
 * EXTERNAL VARIABLES:
 * None
 *
 * PRIOR REQUIREMENTS:
 * The specified reocrd must exist and able to be processed.
 *
 * DEFICIENCIES:
 * None known
 *
 * HISTORY (optional):
 * 23-Apr-1997  Original version.		   J.E. Tvedt
 *
 *-
 */

/* Process the specified record */
long processRec(char *prefix, char *record)
{
    char fieldName[MAX_STRING_SIZE];
    struct dbAddr addr;
    long status;
    long ret;
    char errMess[80];
    
    /* Initialize the return value */
    status = OK;

    /* Form the fieldName of the record */
    sprintf(fieldName,"%s%s.VAL",prefix,record);

    /* Get the address of the data structure  and handle any errors */
    if( (ret = dbNameToAddr (fieldName,&addr)) != 0L)
    {
	status = ERROR_EPICS;
	sprintf(errMess, "ERR dNTA >%s< %ld", fieldName, ret);
	cicsLogMessage(2, errMess);
	cicsLogString(2, "dbName = ", fieldName);
    }

    /* If successful, process the record. */
    if(status == OK)
    {
	if( (ret = dbProcess(addr.precord)) != 0L)
	{
	    status = ERROR_EPICS;
	    cicsLogLong(2, "dbPrccess error =", ret);
	}
    }
    
    return status;
}

/*
 *+
 * FUNCTION NAME:
 * printCadVals
 *
 * INVOCATION:
 * long debugLvl;
 * struct cadRecord *pCad;
 * short numVals;
 *
 * printCadVals( debugLvl, pCad, numVals )
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * > debugLvl (long)                 debug level for logging purposes
 * ! pCad     (struct cadRecord *)   pointer to CAD data structure
 * > numVals  (short)                number of VALx fields to print
 *
 * FUNCTION VALUE:
 * None
 *
 * PURPOSE:
 * General purpose function for printing the VAL fields of a CAD
 * record.  Intended use for debugging.
 *
 * DESCRIPTION:
 * This function prints out the specified number of CAD outputs.  
 * It determines the datatype automatically.  It prints in order starting at
 * VALA and continues through the numVals specified, ignoring any data type 
 * not recognized.
 *
 * EXTERNAL VARIABLES:
 * None
 *
 * PRIOR REQUIREMENTS:
 * It is assumed the CAD record has already been initialized and the
 * directive and any arguments have already been assembled into the cadRecord
 * data structure.  This function is for use by CAD record user subroutines
 * only.
 *
 * DEFICIENCIES:
 * None known
 *
 * HISTORY (optional):
 * 13-Mar-1997  Original version.				Janet Tvedt
 *
 *-
 */

void printCadVals( long debugLvl, struct cadRecord *pCad, short numVals )
{
    void **valPtr;
    unsigned short *typePtr;
    short i, stopVal;
    char name[80];

    /* Make sure the number of values to print is reasonable */
    if(numVals > pCad->ctyp)
	stopVal = pCad->ctyp;
    else
	stopVal = numVals;

    /* Print out the number of values specified starting at VALA */
    valPtr = &pCad->vala;
    typePtr = &pCad->ftva;
    for(i=0; i<stopVal; i++, valPtr++, typePtr++)
    {
	/* Print the value according to its data type */
	switch (*typePtr)
	{
	  case DBF_STRING:
	      sprintf(name, "VAL%c = %s", ('A' + i), (char *) *valPtr);
	      cicsLogMessage(debugLvl, name);
	      break;
	  case DBF_LONG:
	      sprintf(name, "VAL%c = %ld", ('A' + i), *(long *) *valPtr);
	      cicsLogMessage(debugLvl, name);
	      break;
	  case DBF_DOUBLE:
	      sprintf(name, "VAL%c = %f", ('A' + i), *(double *) *valPtr);
	      cicsLogMessage(debugLvl, name);
	      break;
	}

    }
}

/*
 *+
 * FUNCTION NAME:
 * assignVal
 *
 * INVOCATION:
 * unsigned short type;
 * double inVal, outVal;
 * char *errMess;
 * long status;
 * 
 * status = assignVal( type, &inVal, &outVal, errMess)
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * > type    (unsigned short)       data type of input & output values
 * > *inVal  (void *)               pointer to input value
 * < *outVal (void *)               pointer to output value
 * ! errMess (char *)		    pointer to a string
 *
 * FUNCTION VALUE:
 * long  status returned to calling routine, a non-zero value indicates
 *       that an error occured
 *
 * PURPOSE:
 * to make assignments with memory allocation checks
 *
 * DESCRIPTION:
 * This routine is called by user subroutines of EPICS records when there
 * is a need to make assignments with memory checks.  The CAD record
 * support routines allocate memory for CAD outputs (VALA, VALB, etc.)
 * but do not check that the memory was actually allocated.  This routine
 * checks to see if the pointer to the output value is NULL and copies
 * the input value to the output value if possible.  Errors are logged
 * using the CICS logging functions and the error message and the status
 * value returned to the calling routine.
 *
 * EXTERNAL VARIABLES:
 * None
 *
 * PRIOR REQUIREMENTS:
 * None
 *
 * DEFICIENCIES:
 * None known
 *
 * HISTORY (optional):
 * 18-Mar-1997  Original version.                        Janet Tvedt
 *
 *-
 */
long assignVal(unsigned short type, void *inVal, void *outVal, char *errMess)
{
    long status = OK;

    /* If outVal is not a NULL assume it's a valid pointer, so make the assignment */
    if(outVal != NULL)
    {
	switch (type) 
	{ 
	  case DBF_SHORT: 
	      *(short *)outVal = *(short *)inVal;
	      break;
	  case DBF_USHORT: 
	      *(unsigned short *)outVal = *(unsigned short *)inVal;
	      break;
	  case DBF_LONG: 
	      *(long *)outVal = *(long *)inVal;
	      break;
	  case DBF_ULONG: 
	      *(unsigned long *)outVal = *(unsigned long *)inVal;
	      break;
	  case DBF_FLOAT: 
	      *(float *)outVal = *(float *)inVal;
	      break;
	  case DBF_DOUBLE:
	      *(double *)outVal = *(double *)inVal;
	      break;
	  case DBF_STRING:
	      strcpy(outVal, inVal);
	      break;
	  default: 
	      strcpy(errMess,"assignVal: Unrecognized data type");
	      cicsLogLong(0,"assignVal: Unrecognized data type = ",(long) type );
	      status = ERROR_TYPE;
	}
    } 
    else /* handle error messages and set return value to an error */
    {
	status = ERROR_MEMORY;
        strcpy(errMess,"Fatal error - NULL pointer found");
	cicsLogMessage(0,"Fatal error - NULL pointer found" );
    }
    
    return status;

}


/*
 *+
 * FUNCTION NAME:
 * updateStateHealth
 *
 * INVOCATION:
 * char *state, char *health;
 * updateStateHealth(state,health);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * > state  (char *)  pointer to a string with current state
 * > health (char * pointer to a string with current health
 *
 * FUNCTION VALUE:
 * long  Status value OK or some type of error
 *
 * PURPOSE:
 * Support function to update status and health records
 *
 * DESCRIPTION:
 * This routine is called whenever the init CAD record is processed.
 * init is the command for reinitializing the subsystem.  This function
 * will update the state and health records to the specified values.
 * The changes will be reflected automatically in the status/alarm
 * database.
 *
 * EXTERNAL VARIABLES:
 * None
 *
 * PRIOR REQUIREMENTS:
 * It is assumed that the database records exist and the links have been
 * set up properly between the local control database and the status/alarm
 * database.
 *
 * DEFICIENCIES:
 * None
 *
 * HISTORY (optional):
 * 25-Jan-1999   Original version.   Janet Tvedt
 *
 *-
 */


long updateStateHealth(char *state, char *health)
{
   long status = OK;
   char errMess[MAX_STRING_SIZE];

 
   if(state != NULL) 
      if((status = putDbInfoT(dbTop, "state.VAL", errMess, DBF_STRING, state)) != OK)
         cicsLogMessage(0, errMess);
   if((status == OK) && (health != NULL))
      if((status = putDbInfoT(dbTop, "health.VAL", errMess, DBF_STRING, health)) != OK)
         cicsLogMessage(0, errMess);

   return status;
}

/*
 *+
 * FUNCTION NAME:
 * setCar
 *
 * INVOCATION:
 * long ierr,long ival, char *message
 * setCar(ierr,ival,message);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * > ierr  (long)  error field value
 * > ival (long)   val field value
 * > message (char *) message field value
 *
 * FUNCTION VALUE:
 * long  Status value OK or some type of error
 *
 * PURPOSE:
 * Support function to set car fields
 *
 * DESCRIPTION:
 * This routine is called to set the car to busy, idle or error states.
 *
 * EXTERNAL VARIABLES:
 * None
 *
 * PRIOR REQUIREMENTS:
 * It is assumed that the database records exist and the links have
 * been set up properly between the local control database and the 
 * status/alarm database.
 *
 * DEFICIENCIES:
 * None
 *
 * HISTORY (optional):
 * 28-July-1999   Original version.   Peter Ruckle
 *
 *-
 */
long setCar (char *name, long ival, long ierr, char *message, char *error)
{
    long status=OK;
    char dummy[200];
    char rec[200];
    sprintf(rec,"%s%s.IERR",dbTop,name);

    if(status == OK) {
	sprintf(rec,"%s%s%s",dbTop,name,".IMSS");
	status = putDbInfo(rec,dummy,DBF_STRING, message);
    }
    else {
	sprintf(error,"Error setting car record %s\n",rec);
    } 
  
    if(status == OK) {
       sprintf(rec,"%s%s.IERR",dbTop,name);
       status = putDbInfo(rec,dummy,DBF_LONG,&ierr);
    }
    else
       sprintf(error,"Error setting car record %s\n",rec);
    if(status == OK) {
       sprintf(rec,"%s%s%s",dbTop,name,".IVAL");
       status = putDbInfo(rec,dummy,DBF_LONG,
			  &ival);
    } 
    else {
       sprintf(error,"Error setting car record %s\n",rec);
    }

    if (status != OK)
	sprintf(error,"Error setting car record %s\n",rec);

    return status;
}

long sc(char *name,long err,long val,char *str)
{
    long status;
    char rec[80];
    char dummy[80];
    sprintf(rec,"nirs:cc:%s",name);
    status = setCar(name,val,err,dummy,str);
    return status;
   
}
/*
 *+
 * FUNCTION NAME:
 * sleep
 *
 * INVOCATION:
 * int a, b;
 *
 * sleep( a, b);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * > a    (int)         number of seconds to sleep
 * > b    (int)         number of nanoseconds
 *
 * FUNCTION VALUE:
 * None
 *
 * PURPOSE:
 * to create a delay of the specified length of time
 *
 * DESCRIPTION:
 * This routine is called whenever a function wants to impose a time
 * delay.
 *
 * EXTERNAL VARIABLES:
 * None
 *
 * PRIOR REQUIREMENTS:
 * None
 *
 * DEFICIENCIES:
 * None known.
 *
 * HISTORY (optional):
 * 27-Mar-1997  Original version.                       Peter Ruckle
 *
 *-
 */

void sleep (int a, int b) 
{ 
	struct timespec to; 
	struct timespec rm; 
	to.tv_sec = a; 
	to.tv_nsec = b; 
	nanosleep(&to,&rm); 
} 
