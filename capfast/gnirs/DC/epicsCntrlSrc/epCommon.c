static struct {
        void *v;
        char *c;
} rcsid = {
        &rcsid,
        "$Id: epCommon.c,v 1.3 2010/11/17 00:47:03 mrippa Exp $"
};

/*
 * Copyright 1997 Association of Universities for Research in Astronomy, Inc.
 * See the file COPYRIGHT for more details.
 *
 * FILENAME
 * epCommon.c
 *
 * DESCRIPTION (optional)
 * This file contains general purpose functions for the EPICS support software
 * of the GNAAC system. 
 *
 * FUNCTION NAME(S)
 * getDbInfo   - gets the value of the specified field of an EPICS record
 * putDbInfo   - puts a value into the specified field of an EPICS record
 * putDaqFlags - puts values into the standard data acquisition SAD flags
 * naacStatus  - gets value from mbbi status record and converts to one of the
 *               naac status values
 * carStatus   - gets value from a CAR record and converts to one of the
 *               CAR status values
 * setAlarm    - set alarm status of record
 * processRec  - processes the specified EPICS record
 * printCadVals- prints the specified number of CAD VALx outputs using the
 *               specified debug level
 * check_input - checks input against a specified range and converts to
 *               specified data type
 * cvt         - converts string to specified data type
 * assignVal   - performs an assignment of values of arbitrary data type
 *               and checks for possible memory allocation problem
 * pow2        - determines if a number is a power of 2
 * sleep       - function which delays the specified amount of time
 *   
 *
 *INDENT-OFF*
 * $Log: epCommon.c,v $
 * Revision 1.3  2010/11/17 00:47:03  mrippa
 * Modify getDbInfo to use getDbCaInfo routine if needed.
 *
 * Revision 1.2  2009/05/27 19:32:20  fkraemer
 * fkraemer - copied my complete working dir over trunk
 *
 * Revision 1.1.1.1  1998/12/15 16:18:50  buchholz
 * Imported gnaacSrc into CVS
 *
 * Revision 1.2  1998/11/20 17:13:45  pruckle
 * speed up saver, log messages
 *
 * Revision 1.1.1.1  1998/09/30 16:40:27  pruckle
 * Initial Release
 *
 *INDENT-ON* 
 */
/* externed global variables*/
/*extern int epdebug ;*/

/* include files*/
#include <epCommon.h>
#include <naacTasks.h>
#include <car.h>

/*extern globals*/
extern char *dbTop;
extern char *dbSadTop;
extern long getDbCaInfo (char *pvname, char *errMess, unsigned short type, void *outVal);

/*globals*/
char tmp[80];

/* set the database prefixes for the 2 databases*/
void setTop(char *top,char *sadtop)
{
  if(dbTop != NULL)
    free (dbTop);
  if(dbSadTop != NULL)
    free (dbSadTop);
  dbTop = malloc(strlen(top));
  strcpy(dbTop,top);
  dbSadTop = malloc(strlen(sadtop));
  strcpy(dbSadTop,sadtop);
  printf("dbsadTop = %s, top = %s\n",dbSadTop,dbTop);
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

   status = OK;


   /* Get the address of the data structure  and handle any errors */
   if( (ret = dbNameToAddr (fieldName,&addr)) != 0L)
   {
      /* try channel access (needed for FRAME tcs:sad:sourceAInputFrame" */
      if( (ret = getDbCaInfo(fieldName,errMess,type,outVal)) != 0) {
         status = ERROR_EPICS;
         sprintf(errMess, "ERR ca_search >%s< %ld", fieldName, ret);
         cicsLogMessage(2, errMess);
         cicsLogString(2, "dbName = ", fieldName);

         printf("read %s failed internally and from channel access\n",fieldName);

         return status;
      }
      else {
         status = OK;

         return status;
      }

      status = ERROR_EPICS;
      printf("read %s from channel access\n",fieldName);
      return status;
   }

   /* If address found, get the data.  Handle any errors. */
   if( status == OK )
   {
      if( (ret = dbGetField(&addr, type, outVal, &options, &nRq, NULL)) != 0L)
      {
         status = ERROR_EPICS;
         sprintf(errMess, "dbGet error = %ld", ret);
         cicsLogMessage(2, errMess);
         printf("read %s failed internally\n",fieldName);
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
 *                                                                Steven Beard.
 * HISTORY (optional):
 * 19-Mar-1997  Original version.				Janet Tvedt
 *
 *-
 */
long putDbInfoT(char *top,char *fieldName, char *errMess, unsigned short type,
		void *outVal)
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
    
    status = OK;
#if 1
    /* Get the address of the data structure  and handle any errors */
    if( (ret = dbNameToAddr (fieldName,&addr)) != 0L)
    {
		status = ERROR_EPICS;
		sprintf(errMess, "\nputDbInfo >%s< %ld", fieldName, ret);
	/* 	cicsLogMessage(2, errMess); */
/* 		cicsLogString(2, "\nputDbInfo dbName = ", fieldName); */
    }

    /* If address found, write the data.  Handle any errors. */
    if( status == OK )
    {
		if( (ret = dbPutField(&addr, type, outVal, nRq)) != 0L)
		{
			status = ERROR_EPICS;
			sprintf(errMess, "putDbInfo dbPutField error = %ld", ret);
			cicsLogMessage(2, errMess);
		}
    }
#endif
    /* Return error status */
    return status;
}

/*
 *+
 * FUNCTION NAME:
 * putDaqFlags
 *
 * INVOCATION:
 * char *fieldName;
 * char *errMess;
 * unsigned short type;
 * double outVal;
 * long status;
 *
 * status = putDaqFlags(prep, acq, rdout)
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * > prep      (long)             value for "prep" flag (0/1)
 * > acq       (long)             value for "acq" flag (0/1)
 * > rdout     (long)             value for "rdout" flag (0/1)
 *
 * FUNCTION VALUE:
 * long  Status value returned to calling routine, a non-zero value indicates an error
 *
 * PURPOSE:
 * Function to update the data acquisition status flags in the SAD.
 *
 * DESCRIPTION:
 * This routine may be called by a user subroutine to update the data
 * acquisition status flags in the Status Alarm Database. Gemini requires
 * that three flags be maintained:
 *
 *    prep - This is set to 0 when the detector is preparing to start an
 *           exposure, otherwise it is set to 1.
 *
 *    acq  - This is set to 0 when the detector is acquiring data,
 *           otherwise it is set to 1.
 *
 *    rdout - This is set to 0 when the detector is reading out,
 *            otherwise it is set to 1.
 *
 * This function will also handle errors by logging them through the CICS
 * logging functions and by returning an error message and status value to
 * the calling routine.
 *
 * EXTERNAL VARIABLES:
 * None
 *
 * PRIOR REQUIREMENTS:
 * None
 *
 * DEFICIENCIES:
 * The flags should really be updated using Channel Access rather than
 * database access. The calls to putDbInfo should be replaced by calls
 * to putCaInfo when this function exists.
 *
 * It would be cleaner if all three flags could be written simultaneously
 * rather than one at a time, but I don't know how to do this.
 *
 * HISTORY (optional):
 * 16-Jun-1997:  Original version.				Steven Beard
 *
 *-
 */

long putDaqFlags( const long prep, const long acq, const long rdout)
{
    long prepFlag, acqFlag, rdoutFlag;
    char fieldName[MAX_NAME_SIZE];    /* EPICS field name */
    char errMess[MAX_STRING_SIZE];    /* Error message */
    long status;
    
    status = OK;

    /* Copy the supplied values to intermediate variables. This allows
     * constants to be supplied as arguments but a pointer to each variable
     * to be passed to the "putDbInfo" functions.
     */

    prepFlag  = prep;
    acqFlag   = acq;
    rdoutFlag = rdout;

    /* Write the three flags one at a time, checking the return status
     * each time.
     */

    sprintf( fieldName, "%sprep.VAL", dbSadTop );
    status = putDbInfo( fieldName, errMess, DBF_LONG, &prepFlag );

    if ( status == OK )
    {

       sprintf( fieldName, "%sacq.VAL", dbSadTop );
       status = putDbInfo( fieldName, errMess, DBF_LONG, &acqFlag );

       if ( status == OK )
       {

          sprintf( fieldName, "%srdout.VAL", dbSadTop );
          status = putDbInfo( fieldName, errMess, DBF_LONG, &rdoutFlag );
       }
    }

    /* Return error status */
    return status;
}



/*
 *+
 * FUNCTION NAME:
 * naacStatus
 *
 * INVOCATION:
 * char *top;
 * char *statusRec;
 * short status;
 *
 * status = naacStatus( top, statusRec);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * > top        (char *)    pointer to string containing the db prefix
 * > statusRec  (char *)    pointer to string containing the record name
 *
 * FUNCTION VALUE:
 * short    NAAC status value (unknown, busy, done, error)
 *
 * PURPOSE:
 * Gets the  value from mbbi status record and converts to one of the
 * naac status values.
 *
 * DESCRIPTION:
 * This function is called by routines that need to obtain the value
 * from one of the mbbi reoords which are used for command sequencing.
 * The full name of the record is constructed.  The value is obtained
 * and is converted to one of the NAAC status values.
 *
 * EXTERNAL VARIABLES:
 * None
 *
 * PRIOR REQUIREMENTS:
 * It is assumed the specified record exists in the database and
 * is set to one of the defined NAAC status values.
 *
 * DEFICIENCIES:
 * None known
 *
 * HISTORY (optional):
 * 17-Apr-1997  Original version.		   J.E. Tvedt
 *
 *-
 */


short naacStatus( char *top, char *statusRec)
{
    char fieldName[MAX_STRING_SIZE];
    char errMess[MAX_STRING_SIZE];
    unsigned short outVal;

    sprintf(fieldName,"%s%s.VAL",top,statusRec);
    if(getDbInfo(fieldName,errMess,DBF_ENUM,&outVal) == OK)
	return outVal;
    else
	return -1;
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
/* #define DEBUG */
/*
 *+
 * FUNCTION NAME:
 * setAlarm
 *
 * INVOCATION:
 * char *name;
 * long alarm;

 *
 * status = processRec(prefix, record);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * > name  (char *)   pointer to string containing db name
 * > alarm   (char *)   alarm status
 *
 * FUNCTION VALUE:
 * long  Status value, 0 indicates success
 *
 * PURPOSE:
 * Set the alarm status of an EPICS record
 *
 * DESCRIPTION:
 * this routine calls the appropriate db routines to change the alarm status.
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
 * 5-Jan-1998  Original version.		   P. Ruckle
 *
 *-
 */
long setAlarm(int i,int j, double low, double high, double lolo, double hihi)
{
    char name[80];
    long status = OK;
    switch(i)
    {
      case 0:
	  sprintf(name,"%s%s:a%d",dbTop,HK0,j);
	  break;
      
      case 1:
	  sprintf(name,"%s%s:a%d",dbTop,HK1,j);
	  break;
      case 2:
	  sprintf(name,"%s%s:a%d",dbTop,HK2,j);
	  break;
      case 3:
	  sprintf(name,"%s%s:a%d",dbTop,HK3,j);
	  break;
      case 4:
	  sprintf(name,"%s%s:a%d",dbTop,HK4,j);
	  break;
      case 5:
	  sprintf(name,"%s%s:a%d",dbTop,HK5,j);
	  break;
      case 6:
	  sprintf(name,"%s%s:a%d",dbTop,HK6,j);
	  break;
      case 7:
	  sprintf(name,"%s%s:a%d",dbTop,HK7,j);
	  break;
      case 8:
	  sprintf(name,"%s%s:a%d",dbTop,HK8,j);
	  break;
      case 9:
	  sprintf(name,"%s%s:a%d",dbTop,HK9,j);
	  break;
      case 10:
	  sprintf(name,"%s%s:a%d",dbTop,HK10,j);
	  break;
      case 11:
	  sprintf(name,"%s%s:a%d",dbTop,HK11,j);
	  break;
      case 12:
	  sprintf(name,"%s%s:a%d",dbTop,HK12,j);
	  break;
      case 13:
	  sprintf(name,"%s%s:a%d",dbTop,HK13,j);
	  break;
      case 14:
	  sprintf(name,"%s%s:a%d",dbTop,HK14,j);
	  break;
      case 15:
	  sprintf(name,"%s%s:a%d",dbTop,HK15,j);
	  break;
      default:
	  status = ERROR;
	  printf("setAlarm:epCommon.c bad value");
    }
   status = setEpicsAlarmStatus(name);  
    if (status != OK)	
    { 
  	sprintf(tmp,"ERROR in setEpicsAlarmStatus %s low = %f, high = %f, lolo = %f, hihi = %f\n",name,low, high,lolo,hihi); 
	cicsLogMessage(0,tmp);
  	return status; 
    }
    status = setEpicsAlarm(name,low,high, lolo, hihi);   
    if (status != OK)		  
    {    
	return status;  
    }  
  
   
    return status;
}
long setEpicsAlarmT(char *top,char *name, double low, double high, double lolo, double hihi)
{
  char n[80];
  strcpy(n,top);
  strcat(n,name);
  return setEpicsAlarm(n,  low,  high, lolo,  hihi);
}
long setEpicsAlarm(char *name, double low, double high, double lolo, double hihi)
{
    char field[128],errMess[80];
    long status = 0,retval = 0;
   

    sprintf(field,"%s.HIHI",name);
    status = putDbInfo(field,errMess,DBF_DOUBLE,&hihi);   
    if(status != 0)    
      {
	retval = status;
	sprintf(tmp,"setEpicsAlarm error setting hihi for %s\n",name);
	cicsLogMessage(0,tmp);
      }
    sprintf(field,"%s.HIGH",name); 
    status = putDbInfo(field,errMess,DBF_DOUBLE,&high); 
    if(status != 0)    
      {
	retval = status;  
	sprintf(tmp,"setEpicsAlarm error setting high val for %s\n",name);
	cicsLogMessage(0,tmp);
      }  
    
    sprintf(field,"%s.LOW",name); 
    status = putDbInfo(field,errMess,DBF_DOUBLE,&low); 
    if(status != 0)
      {
	retval = status;
	sprintf(tmp,"setEpicsAlarm error setting low val for %s\n",name);
	cicsLogMessage(0,tmp);
      }
    sprintf(field,"%s.LOLO",name); 
    status = putDbInfo(field,errMess,DBF_DOUBLE,&lolo);  
    if(status != 0)    
      {
	retval = status;
	sprintf(tmp,"setEpicsAlarm error setting high lolo for %s\n",name);
	cicsLogMessage(0,tmp);
      } 

#if 0
    sprintf(field,"%s.HIHI",name);
    status = getDbInfo(field,errMess,DBF_DOUBLE,&oldHiHi); 
    if (hihi > oldHiHi )
      {
	status = putDbInfo(field,errMess,DBF_DOUBLE,&hihi);   
	if(status != 0)    
	  {
	    retval = status;
	    sprintf(tmp,"setEpicsAlarm error setting high lolo for %s\n",name);
	cicsLogMessage(0,tmp);
	  }

	sprintf(field,"%s.HIGH",name); 	
	status = getDbInfo(field,errMess,DBF_DOUBLE,&oldHigh); 
	if(low > oldHigh)
	  {
	    status = putDbInfo(field,errMess,DBF_DOUBLE,&high); 
	    if(status != 0)    
	      {
		retval = status;  
		sprintf(tmp,"setEpicsAlarm error setting high val for %s\n",
			name);
	cicsLogMessage(0,tmp);
	      }  

	    sprintf(field,"%s.LOW",name); 
	    status = putDbInfo(field,errMess,DBF_DOUBLE,&low); 
	    if(status != 0)
	      {
		retval = status;
		sprintf(tmp,"setEpicsAlarm error setting low val for %s\n",
			name);
		cicsLogMessage(0,tmp);
	      }
	    sprintf(field,"%s.LOLO",name); 
	    status = putDbInfo(field,errMess,DBF_DOUBLE,&lolo);  
	    if(status != 0)    
	      {
		retval = status;
		sprintf(tmp,"setEpicsAlarm error setting high lolo for %s\n",
			name);
	cicsLogMessage(0,tmp);
	      } 
	  }
	else /*if(low > oldHigh) */
	  {
	   
	    sprintf(field,"%s.LOLO",name); 
	    status = getDbInfo(field,errMess,DBF_DOUBLE,&oldLoLo);
	    if(low < oldLoLo)
	      {
		status = putDbInfo(field,errMess,DBF_DOUBLE,&lolo);  
		if(status != 0)    
		{
		    retval = status;
		    sprintf(tmp,"setEpicsAlarm error setting high lolo for %s\n", name);
		    cicsLogMessage(0,tmp);
		  } 
		sprintf(field,"%s.LOW",name); 
		status = putDbInfo(field,errMess,DBF_DOUBLE,&low); 
		if(status != 0)
		  {
		    retval = status;
		    sprintf(tmp,"setEpicsAlarm error setting low val for %s\n",
			    name);
		    cicsLogMessage(0,tmp);
		  } 
		sprintf(field,"%s.HIGH",name); 	
		status = putDbInfo(field,errMess,DBF_DOUBLE,&high); 
		if(status != 0)    
		  {
		    retval = status;  
		    sprintf(tmp,"setEpicsAlarm error setting high val for %s\n",name);
		    cicsLogMessage(0,tmp);
		  } 
	      }
	  
	    else/*  if(low < oldLoLo) */
	      {
		status = putDbInfo(field,errMess,DBF_DOUBLE,&low); 
		if(status != 0)
		  {
		    retval = status;
		   sprintf(tmp,"setEpicsAlarm error setting low val for %s\n",
			   name);
		   cicsLogMessage(0,tmp);
		  } 
		sprintf(field,"%s.HIGH",name); 	
		status = putDbInfo(field,errMess,DBF_DOUBLE,&high); 
		if(status != 0)    
		  {
		    retval = status;  
		    sprintf(tmp,"setEpicsAlarm error setting high val for %s\n",name);
		    cicsLogMessage(0,tmp);
		  } 
		sprintf(field,"%s.LOLO",name); 
		status = putDbInfo(field,errMess,DBF_DOUBLE,&lolo);  
		if(status != 0)    
		  {
		    retval = status;
		    sprintf(tmp,"setEpicsAlarm error setting high lolo for %s\n",name);
		    cicsLogMessage(0,tmp);
		  } 
	      }
	  }
	    
      }


    else  /* if (oldHiHi > hihi) */
      { 
	sprintf(field,"%s.HIGH",name); 
	status = putDbInfo(field,errMess,DBF_DOUBLE,&high); 
	if(status != 0)    
	  {
	    retval = status;  
	   sprintf(tmp,"setEpicsAlarm error setting high val for %s\n",name);
	cicsLogMessage(0,tmp);
	  }  
	sprintf(field,"%s.HIHI",name);
	status = putDbInfo(field,errMess,DBF_DOUBLE,&hihi);   
	if(status != 0)    
	  {
	    retval = status;
	sprintf(tmp,"setEpicsAlarm error setting high lolo for %s\n",name);
	cicsLogMessage(0,tmp);
	  }
      }
 
  
#endif


    return retval;
}


long setEpicsAlarmStatus(char *name)
{
   char field[80],errMess[80];
   long status,alarm;

   sprintf(field,"%s.HSV",name);
   alarm = MINOR_ALARM;
   status = putDbInfo(field,errMess,DBF_LONG,&alarm);

   sprintf(field,"%s.HHSV",name);
   alarm = MAJOR_ALARM;
   status = putDbInfo(field,errMess,DBF_LONG,&alarm);

   sprintf(field,"%s.LSV",name);
   alarm = MINOR_ALARM;
   status = putDbInfo(field,errMess,DBF_LONG,&alarm);

   sprintf(field,"%s.LLSV",name);
   alarm = MAJOR_ALARM;
   status = putDbInfo(field,errMess,DBF_LONG,&alarm);
   return status;
}

/*
 *+
 * FUNCTION NAME:
 * resetAlarm
 *
 * INVOCATION:
 * char *name;
 *
 * status = reset(name);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * > prefix   (char *)   pointer to string containing db name
 *
 * FUNCTION VALUE:
 * long  Status value, 0 indicates success
 *
 * PURPOSE:
 * Reset the alarm status of an EPICS record
 *
 * DESCRIPTION
 * this routine calls the appropriate db routines to change the alarm status.
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
 * 5-Jan-1998  Original version.		   P. Ruckle
 *
 *-
 */
long resetAlarm(char * name)
{
    long status=(OK);

    char fieldName[MAX_STRING_SIZE];
    struct dbAddr addr;
    long ret;
    char errMess[80];
    
    /* Initialize the return value */
    status = OK;

    /* Get the address of the data structure  and handle any errors */
    if( (ret = dbNameToAddr (name,&addr)) != 0L)
    {
	status = ERROR_EPICS;
	sprintf(errMess, "ERR dNTA >%s< %ld", fieldName, ret);
	cicsLogMessage(2, errMess);
	cicsLogString(2, "dbName = ", fieldName);
    }

    /* If successful, process the record. */
    if(status == OK)
    {
	if( (ret = recGblResetAlarms(addr.precord)) != 0L)
	{
	    status = ERROR_EPICS;
	    cicsLogLong(2, "recGblSetSevr error =", ret);
	}
    }

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
long setCar (char *name, long ival,long ierr,char  *message,char *error)
{
    long status;
    char dummy[80];
    char rec[80];
    sprintf(rec,"%s%s%s",dbTop,name,".IERR");
    status = putDbInfo(rec,dummy,DBF_LONG,&ierr);
    if(status == OK)
    {
        sprintf(rec,"%s%s%s",dbTop,name,".IMSS");
        status = putDbInfo(rec,dummy,DBF_STRING,
                      message);
    }
    else
        sprintf(error,"Error setting car record %s\n",rec);
   
    if(status == OK)
    {
        sprintf(rec,"%s%s%s",dbTop,name,".IVAL");
        status = putDbInfo(rec,dummy,DBF_LONG,
                           &ival);
    } 
    else
        sprintf(error,"Error setting car record %s\n",rec);
    
    if (status != OK)
        sprintf(error,"Error setting car record %s\n",rec);

    return status;

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
    /* Check for a NULL pointer signifying error in memory allocation */
    if(out == NULL)
    {
	cicsLogMessage(0, "Fatal error - NULL pointer found");
	return(ERROR_MEMORY); /**** this should never happen ****/
    }

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
	  return ERROR_TYPE;
    } 
 
    return ERROR_TYPE;
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
	      strcpy(errMess,"Unrecognized data type");
	      cicsLogLong(0,"Unrecognized data type = ",(long) type );
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
 * pow2
 *
 * INVOCATION:
 * int x, status;
 *
 * status = pow2(x)
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * > x	(int)	the number to check
 *
 * FUNCTION VALUE:
 * (int)
 *  1     if the number is a power of 2
 *  0	  if the number is NOT a power of 2
 *
 * PURPOSE:
 * to determine if a number is a power of 2
 *
 * DESCRIPTION:
 * Determine the largest value of i such that x <= 2**i.
 * If x = 2**i, then x is a power of 2.
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
 * 01-Mar-1997  Original version.		Peter Ruckle
 *
 *-
 */
int pow2(int x, int *i)
{
    int y;

    y = x;
    *i = 0;
    for (;y>1; *i = *i + 1)
	y>>=1;

    if(x==(y<< *i))
	return 1;
	return 0;

}

