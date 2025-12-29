static struct {
    void *v;
    char *c;
} rcsid = {
    &rcsid,
    "$Id: arSetupChk.c,v 1.4 2010/08/17 02:00:15 mrippa Exp $"
};
/*
 * Copyright 1997 Association of Universities for Research in Astronomy, Inc. 
 * See the file COPYRIGHT for more details.
 *
 * FILENAME
 * arSetupChk.c
 *
 * DESCRIPTION
 * This file contains the functions for the user subroutine
 * of the CAD record arSetup.
 * 
 * FUNCTION NAME(S)
 * parseCmd - parses the uCode cmd file and sets record values
 * arSetupChk - validates the input parameters to the CAD record
 * 
 * DEPENDENCIES
 * Changes to the CAD record properties must be reflected
 * in this software.  
 *
 *INDENT-OFF*
 * $Log: arSetupChk.c,v $
 * Revision 1.4  2010/08/17 02:00:15  mrippa
 * *** empty log message ***
 *
 * Revision 1.3  2010/08/16 20:27:08  mrippa
 * More debug level statements for array setup
 *
 * Revision 1.2  2009/05/27 19:32:19  fkraemer
 * fkraemer - copied my complete working dir over trunk
 *
 * Revision 1.2  1998/11/20 17:13:36  pruckle
 * speed up saver, log messages
 *
 * Revision 1.1.1.1  1998/09/30 16:40:27  pruckle
 * Initial Release
 *
 *INDENT-ON* 
 */
#include "debug.h"
#include <epCommon.h>
#include <naacTasks.h>
#include <car.h>
extern char *dbTop,*dbSadTop;
extern int epdebug ;
/*
 *+
 * FUNCTION NAME:
 * parseCmd
 *
 * INVOCATION:
 * char *filename
 * struct cadRecord *pCad;
 * long status;
 *
 * status = parseCmd(filename, pCad)
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * > filename (char *)             pointer to filename
 * ! pCad   (struct cadRecord *)   pointer to CAD data structure
 *
 * FUNCTION VALUE:
 * long  Status value. 0 indicates success
 *
 * PURPOSE:
 * Parses the ucode command file 
 *
 * DESCRIPTION:
 * This function searches the ucode command file specified by filename for
 * for certain record/field names.  When each is found the value is extracted
 * and put into the corresponding EPICS record in the database.  Any error messages
 * are put into the MESS field of the specified CAD reocrd and a non-zero status
 * value is returned. 
 *
 * EXTERNAL VARIABLES:
 * None
 *
 * PRIOR REQUIREMENTS:
 * The ucode file must exist as well as the corresponding EPICS records.
 * The ucode command file has the same format as that required by PVLOAD.
 *
 * DEFICIENCIES:
 * None known.
 *
 * HISTORY (optional):
 * 17-Jan-1997  Original version adapted from CICS alpha 1.0   J.E. Tvedt
 *
 *-
 */
long parseCmd(char *filename, struct cadRecord *pCad)
{
   long status = CAD_ACCEPT;
   FILE *fp;
   char str[80];
   char recName[MAX_NAME_SIZE];
   char fieldName[MAX_NAME_SIZE];
   char *ptr;
   int len;
   short numFound;
   short errFlag;
   double dVal;
   long iVal,lVal;

   /* Initialize counter and status */
   numFound = 0;
   status = CAD_ACCEPT;
   errFlag = 0;

   cicsLogMessage(DBG_MIN,"get observe car\n");
   /* Attempt to open the file */  
   getDbInfoT(dbTop, OBSERVE_CAR ".IVAL", str,DBF_LONG,&lVal);
   cicsLogMessage(DBG_MIN,"got observe car\n");
   printf("filename = %s\n",filename);
   if (lVal != CAR_IDLE)
      status =  CAD_REJECT;
   else if((fp = fopen(filename,"r")) == NULL)
   {
      printf("arSetupChk: fopen rtn NULL, errno=%d\n",errno);
      strcpy(pCad->mess,"Error opening uCode file. errno at console");
      cicsLogString(1, pCad->mess, filename);
      status = CAD_REJECT;
   }


   /* Parse the uCode command file */
   if(status == CAD_ACCEPT)
   {
      cicsLogMessage(DBG_MIN,"file opened\n");
      /* Read all the lines in the file until done or error occurs */
      while( (fgets(str, 80, fp) != NULL) &&  (errFlag == 0))
      {

	 cicsLogMessage(DBG_MIN,"got line\n");
	 /* Ignore lines of comments */
	 if(str[0] == '#')
	    continue;

	 /* Ignore lines containing "group" */
	 if(strstr(str,"group") != NULL)
	    continue;

	 /* Ignore blank lines.  A length of 10 accounts for
	    a few spaces on a line
	    */
	 if(strlen(str) < 10)
	    continue;

	 /* Extract record name */
	 recName[0] = '\0';
	 if((ptr = strstr(str,"$(top)")) != NULL)
	 {
	    len = strcspn(ptr+6,".");
	    strncpy(recName, ptr+6, len );
	    recName[len] = '\0';

	    /* Check to see if this is a record we want the value for.
	       If so, get the value, then put it into the EPICS reocrd.
	       */
	    if(strcmp(recName,PUC_MININT) == 0)
	    {
	       numFound++;
	       if(sscanf(str+strcspn(str,"=")+1,"%lf",&dVal) == 1)
	       {
		  sprintf(fieldName,"%s%s.VAL",dbTop,PUC_MININT);
		  if(putDbInfo(fieldName, pCad->mess, DBF_DOUBLE, 
			   &dVal) != 0)
		     errFlag = 3;
	       }
	       else
	       {
		  errFlag = 2;
	       }
	    }
	    else if(strcmp(recName,PUC_MINREAD) == 0)
	    {
	       numFound++;
	       if(sscanf(str+strcspn(str,"=")+1,"%lf",&dVal) == 1)
	       {
		  sprintf(fieldName,"%s%s.VAL",dbTop,PUC_MINREAD);
		  if(putDbInfo(fieldName, pCad->mess, DBF_DOUBLE,
			   &dVal) != 0)
		     errFlag = 3;
	       }
	       else
	       {
		  errFlag = 2;
	       }
	    }
	    else if(strcmp(recName,PUC_MINDLY) == 0)
	    {
	       numFound++;
	       if(sscanf(str+strcspn(str,"=")+1,"%lf",&dVal) == 1)
	       {
		  sprintf(fieldName,"%s%s.VAL",dbTop,PUC_MINDLY);
		  if(putDbInfo(fieldName, pCad->mess, DBF_DOUBLE, 
			   &dVal) != 0)
		     errFlag = 3;
	       }
	       else
	       {
		  errFlag = 2;
	       }
	    }
	    else if(strcmp(recName,PUC_DAVGDLY) == 0)
	    {
	       numFound++;
	       if(sscanf(str+strcspn(str,"=")+1,"%lf",&dVal) == 1)
	       {
		  sprintf(fieldName,"%s%s.VAL",dbTop,PUC_DAVGDLY);
		  if(putDbInfo(fieldName, pCad->mess, DBF_DOUBLE, 
			   &dVal) != 0)
		     errFlag = 3;
	       }
	       else
	       {
		  errFlag = 2;
	       }
	    }
	    else if(strcmp(recName,PUC_FRMSPCYCLE) == 0)
	    {
	       numFound++;
	       if(sscanf(str+strcspn(str,"=")+1,"%ld",&iVal) == 1)
	       {
		  sprintf(fieldName,"%s%s.VAL",dbTop,PUC_FRMSPCYCLE);
		  if(putDbInfo(fieldName, pCad->mess, DBF_LONG,
			   &iVal) != 0)
		     errFlag = 3;
	       }
	       else
	       {
		  errFlag = 2;
	       }
	    }
	    else if(strcmp(recName,PUC_CODETYPE) == 0)
	    {
	       numFound++;
	       if(sscanf(str+strcspn(str,"=")+1,"%ld",&iVal) == 1)
	       {
		  sprintf(fieldName,"%s%s.VAL",dbTop,PUC_CODETYPE);
		  if(putDbInfo(fieldName, pCad->mess, DBF_LONG,
			   &iVal) != 0)
		     errFlag = 3;
	       }
	       else
	       {
		  errFlag = 2;
	       }
	    }
	 }

	 /* Unrecognized format in this line - should not happen ... */
	 else
	 {
	    errFlag = 1;
	 }

      }

      /* Set error flan if not all 6 items were found */
      if((errFlag == 0) && numFound != 6)
	 errFlag = 4;

      /* Output error messages */
      if(errFlag != 0)
      {
	 status = CAD_REJECT;
	 strcpy(pCad->mess,"Error parsing uCode command file ");
	 cicsLogString(1, pCad->mess, filename);
	 switch(errFlag)
	 {
	    case 1:
	       cicsLogMessage(DBG_NOLOG, "Unrecognized format in line:");
	       cicsLogMessage(DBG_NOLOG, str);
	       break;
	    case 2:
	       cicsLogMessage(DBG_NOLOG, "Error obtaining attribute value for record:");
	       cicsLogMessage(DBG_NOLOG, recName);
	       break;
	    case 3:
	       cicsLogMessage(DBG_NOLOG, "Error putting value into database");
	       cicsLogString(DBG_NOLOG, "for record = ", fieldName);
	       break;
	    case 4:
	       cicsLogMessage(DBG_NOLOG, "Parser did not find all attributes");
	       cicsLogLong(DBG_NOLOG, "number found = ", (long) numFound);
	       break;
	 }

      }
      cicsLogMessage(DBG_MIN,"file closed\n");
      fclose(fp);
   }

   return status;
}


/*
 *+
 * FUNCTION NAME:
 * arSetupChk
 *
 * INVOCATION:
 * struct cadRecord *pCad;
 * status = arSetupChk( pCad );
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * ! pCad   (struct cadRecord *)   pointer to CAD data structure
 *
 * FUNCTION VALUE:
 * long  Status value written to CAD VAL field
 *
 * PURPOSE:
 * User defined function for "arSetup" CAD record
 *
 * DESCRIPTION:
 * This routine is called whenever the arSetup CAD record is processed.
 * arSetup is the command for setting up the array.  The command has 
 * has the following arguments as CAD inputs.  All CAD inputs are
 * restricted to strings.
 * (a) uCodePath: the directory path on the host computer containing the uCode
 * (b) uCodeName: the name of the uCode
 * (c) Vset: voltage setting for the array
 * (d) VddCl1: DAC voltage setting
 * (e) VddCl2: DAC voltage setting
 * (f) VggCl1: DAC voltage setting
 * (g) VggCl2: DAC voltage setting
 * (h) dBias: desired bias voltage for the array
 *
 * This routine will produce the following CAD outputs.
 * (vala) uCodePath: the directory path on the host computer containing the uCode
 * (valb) uCodeName: the name of the uCode
 * (valc) Vset: voltage setting for the array
 * (vald) VddCl1: DAC voltage setting
 * (vale) VddCl2: DAC voltage setting
 * (valf) VggCl1: DAC voltage setting
 * (valg) VggCl2: DAC voltage setting
 * (valh) dBias: desired bias voltage for the array
 * (vali) VDet: calculated detector voltage
 *
 * EXTERNAL VARIABLES:
 * tldFile: name of file containing uCode
 * cmdFile: name of file containing oommands associated with the uCode
 *
 * PRIOR REQUIREMENTS:
 * It is assumed the CAD record has already been initialized and the
 * directive and any arguments have already been assembled into the cadRecord
 * data structure.
 *
 * DEFICIENCIES:
 * CAD inputs are restricted to the maximum string size within EPICS.
 *
 * HISTORY (optional):
 * 19-Mar-1997  Original version adapted from CICS alpha 1.0   Janet Tvedt
 *
 *-
 */
long arSetupChk(struct cadRecord *pCad)
{
   long status = CAD_ACCEPT;         /* return status */
   int ret;
   unsigned short usVal;
   char buf1[80],buf[80]; 
   char buf2[80];
   char name[80];
   static double vDet= 0,vddUc= 0,dBias= 0,vSet= 0;
   static double VddCl1= 0,VddCl2= 0,VggCl1= 0,VggCl2= 0;
   static double vdetmin= 0,dbiasmin= 0,vsetmin= 0;
   static double vddcl1min= 0,vddcl2min= 0,vggcl1min= 0,vggcl2min= 0;
   static double vdetmax= 0,dbiasmax= 0,vsetmax= 0;
   static double vddcl1max= 0,vddcl2max= 0,vggcl1max= 0,vggcl2max= 0;
   static long  frcDwnLd, arSizeVal;
   char dummy[MAX_STRING_SIZE];
   char state[80];

   /* Initialise CAD status */
   status = CAD_ACCEPT;

   cicsLogMessage(DBG_MIN,"Arsetupchk \n");
   /* Switch according to the CAD directive in DIR field */
   switch (pCad->dir)
   {
      /* CAD MARK directive detected. Nothing needs to be done.*/
      case CAD_MARK:
	 cicsLogMessage(DBG_MIN,"arsetupchk mark\n");
	 cicsLogMessage( 2, "arSetupChk - MARK directive.");
	 break;

	 /* CAD PRESET directive detected.  Check the input argument. 
	    If it is acceptable then copy it to the output.       */
      case CAD_PRESET: 
	 cicsLogMessage(DBG_MIN," arsetupchk preset\n");
	 cicsLogMessage( 2, "arSetupChk - PRESET directive.");
	 getDbInfoT(dbSadTop, STATE ".VAL", dummy, DBF_STRING, state);
	 if(strcmp(state,"RUNNING") != 0)
	 {
	    status = CAD_REJECT;
	    strcpy(pCad->mess,"arSetupChk: State must be running first");
	 }

	 /* Construct filenames for uCode .tld and .cmd file */
	 cicsLogString(DBG_MIN, "uCodePath = ",pCad->a);
	 cicsLogString(DBG_MIN, "uCodeName = ",pCad->b);

	 /* 	printf("preset new = %s %s, old = %s %s",pCad->a,pCad->b,pCad->olda,pCad->oldb); */
	 pCad->a[MAX_STRING_SIZE-1] = NULL;
	 pCad->b[MAX_STRING_SIZE-1] = NULL;
	 strcpy (buf1,pCad->a);
	 if(buf1[strlen(buf1) - 1] != '/') 
	    strcat(buf1,"/");
	 strcpy (name,pCad->b);
	 strncat (buf1,name,strcspn (name,"."));
	 strcpy(buf2,buf1);
	 cicsLogMessage(DBG_MIN,"parseCmd\n");
	 if(status == CAD_ACCEPT)
	 {
	    if (strlen(buf1) < 75)
	    {
	       strcat (buf1,".tld");
	       strcat (buf2,".cmd");
	       /* Parse 	the .cmd file to determine uCode specific parameters */
	       status = parseCmd(buf2,pCad);
	    }
	    else 
	    {
	       cicsLogMessage( 2, "arSetupChk - uCode name exceeds 79 characters.");
	       status = CAD_REJECT;
	    }
	 }
	 cicsLogMessage(DBG_MIN,"parseCmd done\n");
	 if(status == CAD_ACCEPT)
	 {
	    cicsLogString (3,"Requested forceDownLd = ", pCad->i);
	    if ((strcmp( pCad->i, "FALSE") == 0) || 
		  (strcmp( pCad->i, "0") == 0))
	       frcDwnLd = (long) 0;
	    else 
	    {
	       frcDwnLd = (long) 1;
	    }
	    status = CAD_ACCEPT;
	 }
	 if ((strcmp(pCad->a, pCad->olda) != 0) ||
	       (strcmp(pCad->b, pCad->oldb) != 0))
	    frcDwnLd = 1;

	 cicsLogMessage(DBG_MIN,"check voltages\n");
	 /* Check voltage VSet */
	 if(status == CAD_ACCEPT)
	 {
	    cicsLogMessage(DBG_MIN,"checking\n");
	    cicsLogString (3,"Requested VSet = ", pCad->c);
	    /*find min and max from database*/

	    status = getDbInfoT(dbTop, VSET ".LOLO", 
		  pCad->mess, DBF_DOUBLE, &vsetmin);
	    if( status == CAD_ACCEPT)
	       status = getDbInfoT(dbTop, VSET ".HIHI", pCad->mess,
		     DBF_DOUBLE, &vsetmax);


	    if ((status == CAD_ACCEPT) &&
		  (ret = check_input(pCad->ftvc, pCad->c, &vsetmin,
				     &vsetmax, &vSet))!=OK)
	    {   
	       if (ret == ERROR_TYPE)
	       {	  
		  sprintf( pCad->mess, "ERROR: VSet should be a double" );
		  cicsLogMessage(DBG_MIN,"Error from check_input");
		  cicsLogMessage(DBG_MIN, pCad->mess);
	       }
	       else 
	       {	  
		  sprintf(pCad->mess, "ERROR: VSet range %5.2f - %5.2f",
			vsetmin, vsetmax );
		  cicsLogMessage(DBG_MIN, pCad->mess);
	       }
	       status =  CAD_REJECT;
	    }
	 }
	 sprintf(buf,"vset status = %d\n",status);
	 cicsLogMessage(DBG_MIN,buf);
	 /* Check VddCl1 */  
	 if(status == CAD_ACCEPT)
	 {
	    cicsLogString (3,"Requested VddCl1 = ",pCad->d);

	    /*find min and max from database*/

	    status = getDbInfoT(dbTop, VDDCL1 ".LOLO", pCad->mess,
		  DBF_DOUBLE, &vddcl1min);
	    if( status == CAD_ACCEPT)
	       status = getDbInfoT(dbTop, VDDCL1 ".HIHI", pCad->mess,
		     DBF_DOUBLE, &vddcl1max);

	    if ((status == CAD_ACCEPT) &&
		  (ret = check_input (pCad->ftvd, pCad->d, &vddcl1min,
				      &vddcl1max,  &VddCl1)) != OK)
	    {   
	       if (ret == ERROR_TYPE)
	       {	  
		  sprintf( pCad->mess, "ERROR: VddCl1 should be a double");
		  cicsLogMessage(DBG_NOLOG, pCad->mess);
	       }
	       else 
	       {    
		  sprintf( pCad->mess, "ERROR: VddCl1 range %5.2f - %5.2f",
			vddcl1min,vddcl1max );
		  cicsLogMessage(DBG_NOLOG, pCad->mess);
	       }
	       status =  CAD_REJECT;
	    }
	 }

	 sprintf(buf,"vddcl1 status = %d\n",status);
	 cicsLogMessage(DBG_MIN,buf);

	 /* Check VddCl2 */  
	 if(status == CAD_ACCEPT)
	 {
	    cicsLogString (3,"Requested VddCl2 = ",pCad->e);

	    /*find min and max from database*/

	    status = getDbInfoT(dbTop, VDDCL2 ".LOLO", pCad->mess, 
		  DBF_DOUBLE, &vddcl2min);
	    if( status == CAD_ACCEPT)
	       status = getDbInfoT(dbTop, VDDCL2 ".HIHI", pCad->mess,
		     DBF_DOUBLE, &vddcl2max);

	    if ((status == CAD_ACCEPT) &&
		  (ret = check_input (pCad->ftve,pCad->e,
				      &vddcl2min,&vddcl2max,
				      &VddCl2))!=OK)
	    {   
	       if (ret == ERROR_TYPE)
	       {	  
		  sprintf( pCad->mess, "ERROR: VddCl2 should be a double");
		  cicsLogMessage(DBG_NOLOG, pCad->mess);
	       }
	       else 
	       {	  
		  sprintf( pCad->mess, "ERROR: VddCl2 range %5.2f - %5.2f",
			vddcl2min,vddcl2max );
		  cicsLogMessage(DBG_NOLOG, pCad->mess);
	       }
	       status =  CAD_REJECT;
	    }
	 }


	 sprintf(buf,"vddcl2 status = %d\n",status);
	 cicsLogMessage(DBG_MIN,buf);
	 /* Check VggCl1 */
	 if(status == CAD_ACCEPT)
	 {
	    cicsLogString (3,"Requested VggCl1 = ",pCad->f);

	    /*find min and max from database*/

	    status = getDbInfoT(dbTop, VGGCL1 ".LOLO", pCad->mess, 
		  DBF_DOUBLE, &vggcl1min);
	    if( status == CAD_ACCEPT)
	       status = getDbInfoT(dbTop, VGGCL1 ".HIHI", pCad->mess, 
		     DBF_DOUBLE, &vggcl1max);

	    if ((status == CAD_ACCEPT) &&
		  (ret = check_input (pCad->ftvf, pCad->f, &vggcl1min,
				      &vggcl1max, &VggCl1)) != OK)
	    {   
	       if (ret == ERROR_TYPE)
	       {	  
		  sprintf( pCad->mess, "ERROR: VggCl1 should be a double");
		  cicsLogMessage(DBG_NOLOG, pCad->mess);
	       }
	       else 
	       {	  
		  sprintf( pCad->mess, "ERROR: VggCl1 range %5.2f - %5.2f",
			vggcl1min,vggcl1max );
		  cicsLogMessage(DBG_NOLOG, pCad->mess);
	       }
	       status =  CAD_REJECT;
	    }
	 }

	 sprintf(buf,"vggcl1 status = %d\n",status);
	 cicsLogMessage(DBG_MIN,buf);
	 /* Check VggCl2 */  
	 if(status == CAD_ACCEPT)
	 {
	    cicsLogString (3,"Requested VggCl2 = ",pCad->g);

	    /*find min and max from database*/

	    status = getDbInfoT(dbTop, VGGCL2 ".LOLO", pCad->mess,
		  DBF_DOUBLE, &vggcl2min);
	    if( status == CAD_ACCEPT)
	       status = getDbInfoT(dbTop, VGGCL2 ".HIHI", pCad->mess,
		     DBF_DOUBLE, &vggcl2max);

	    if ((status == CAD_ACCEPT) &&
		  (ret = check_input (pCad->ftvg,pCad->g,&vggcl2min,
				      &vggcl2max, &VggCl2))!=OK)
	    {   
	       if (ret == ERROR_TYPE)
	       {	  
		  sprintf( pCad->mess, "ERROR: VggCl2 should be a double");
		  cicsLogMessage(DBG_NOLOG, pCad->mess);
	       }
	       else 
	       {	  
		  sprintf( pCad->mess, "ERROR: VggCl2 range %5.2f - %5.2f",
			vggcl2min,vggcl2max);
		  cicsLogMessage(DBG_NOLOG, pCad->mess);
	       }
	       status =  CAD_REJECT;
	    }
	 }

	 sprintf(buf," vggcl2 status = %d\n",status);
	 cicsLogMessage(DBG_MIN,buf);
	 /* Calculate and check vDet from the dBias */
	 if(status == CAD_ACCEPT)
	 {
	    cicsLogString(3, "Requested bias = ", pCad->h);

	    /*find min and max from database*/

	    status = getDbInfoT(dbTop, DBIAS ".LOLO", pCad->mess,
		  DBF_DOUBLE, &dbiasmin);
	    if( status == CAD_ACCEPT)
	       status = getDbInfoT(dbTop, DBIAS ".HIHI", pCad->mess,
		     DBF_DOUBLE, &dbiasmax);

	    if ((status == CAD_ACCEPT) &&
		  (ret = check_input (pCad->ftvh, pCad->h, 
				      &dbiasmin, &dbiasmax, 
				      &dBias))!=OK)
	    {   
	       if (ret == ERROR_TYPE)
	       {	  
		  sprintf( pCad->mess, "ERROR: dBias should be a double" );
		  cicsLogMessage(DBG_NOLOG, pCad->mess);
	       }
	       else 
	       {	  
		  sprintf( pCad->mess, "ERROR: dBias range %5.2f - %5.2f",
			dbiasmin,dbiasmax);
		  cicsLogMessage(DBG_NOLOG, pCad->mess);
	       }
	       status =  CAD_REJECT;
	    }
	 }

	 sprintf(buf,"vdet status = %d\n",status);
	 cicsLogMessage(DBG_MIN,buf);
	 if (status == CAD_ACCEPT)
	 {
	    /* Obtain vddUc from database */
	    status = getDbInfoT(dbTop, VDDUC_CHAN ".VAL", pCad->mess,
		  DBF_DOUBLE, &vddUc);  
	    sprintf(buf,"vdduc_chan status = %d\n",status);
	    cicsLogMessage(DBG_MIN,buf);
	    if(status == CAD_ACCEPT)	 /* Now calculate vDet required to achieve bias */
	    {
	       vDet = fabs(vddUc + dBias);
	       cicsLogDouble(3, "vddUc from db = ", vddUc);
	       cicsLogDouble(3, "calculated vDet = ", vDet);

	       /*Find min and max from database*/

	       status = getDbInfoT(dbTop, VDET ".LOLO", pCad->mess,
		     DBF_DOUBLE, &vdetmin); 
	       sprintf(buf,"vdet lolo status = %d\n",status);
	       cicsLogMessage(DBG_MIN,buf);
	       status = getDbInfoT(dbTop, VDET ".HIHI", pCad->mess,
		     DBF_DOUBLE, &vdetmax);  
	       sprintf(buf,"vdet hihi status = %d\n",status);
	       cicsLogMessage(DBG_MIN,buf);



	       /* Check vDet against allowable range */
	       if (status == CAD_ACCEPT)
	       {  
		  sprintf (name, "%f", vDet);
		  if((ret = check_input( pCad->ftvi, name, &vdetmin, 
			      &vdetmax, &vDet)) != OK) 
		  {   
		     sprintf(buf,"ret = %d\n",status);
		     cicsLogMessage(DBG_MIN,buf);
		     if (ret == ERROR_TYPE)
		     {   
			sprintf( pCad->mess, "ERROR: vDet should be a double" );
			cicsLogMessage(DBG_NOLOG, pCad->mess);
		     }
		     else 
		     {	
			sprintf( pCad->mess, "ERROR: vDet range %5.2f - %5.2f", vdetmin, vdetmax );
			cicsLogMessage(DBG_NOLOG, pCad->mess);
		     }
		     status =  CAD_REJECT;
		  }
	       }
	    }
	 }
	 /* end of dBias tests */

	 /* If arguments are OK, set CAD outputs, global filenames and done record */
	 sprintf(buf,"status = %d\n",status);
	 cicsLogMessage(DBG_MIN,buf);

	 cicsLogMessage( 2, "arSetupChk - PRESET directive. done");
	 break;

	 /* CAD CLEAR directive detected. Nothing needs to be done. */
      case CAD_CLEAR:
	 cicsLogMessage( 2, "arSetupChk - CLEAR directive.");
	 break;

	 /* CAD START directive detected. */
      case CAD_START:
	 cicsLogMessage( 2, "arSetupChk - START directive.");

	 /* 	printf("start new = %s %s, old = %s %s",pCad->vala,pCad->valb,pCad->olda,pCad->oldb); */
	 strcpy(pCad->vala, pCad->a);
	 strcpy(pCad->valb, pCad->b);
	 status = assignVal(DBF_DOUBLE, &vSet, pCad->valc, pCad->mess);
	 if(status == CAD_ACCEPT) status = assignVal(DBF_DOUBLE, &VddCl1, 
	       pCad->vald, pCad->mess);
	 if(status == CAD_ACCEPT) status = assignVal(DBF_DOUBLE, &VddCl2, 
	       pCad->vale, pCad->mess);
	 if(status == CAD_ACCEPT) status = assignVal(DBF_DOUBLE, &VggCl1, 
	       pCad->valf, pCad->mess);
	 if(status == CAD_ACCEPT) status = assignVal(DBF_DOUBLE, &VggCl2,
	       pCad->valg, pCad->mess);
	 if(status == CAD_ACCEPT) status = assignVal(DBF_DOUBLE, &dBias, 
	       pCad->valh, pCad->mess);
	 if(status == CAD_ACCEPT) status = assignVal(DBF_DOUBLE, &vDet, 
	       pCad->vali, pCad->mess);
	 if(status == CAD_ACCEPT) status = assignVal(DBF_LONG, &frcDwnLd,
	       pCad->valj, pCad->mess);
	 status = getDbInfoT(dbTop, ROW_HI ".VAL", buf2,DBF_LONG, &arSizeVal);
	 /*   calcMinInt(arSizeVal, 1, 1); */

	 if(status == CAD_ACCEPT)
	 {
	    strcpy(tldFile,buf1);
	    strcpy(cmdFile,buf2);
	    usVal = 1;
	    status = putDbInfoT(dbTop, PUC_DONE ".VAL", pCad->mess, 
		  DBF_ENUM, &usVal); 
	 }


	 status = getDbInfoT(dbTop, ARSETUP_CAR ".VAL", pCad->mess, 
	       DBF_ENUM, &usVal);
	 if ((status != OK) || (usVal != CAR_IDLE))
	 {
	    status = CAD_REJECT;
	    strcpy(pCad->mess,"arSetup Busy");
	    cicsLogMessage(DBG_NOLOG, pCad->mess);
	 }

	 /* Check that the preset was done */
	 sprintf(name,"%s%s.VAL",dbTop, PUC_DONE);
	 status = getDbInfo(name, pCad->mess, DBF_ENUM, &usVal);
	 if((status == CAD_ACCEPT) && (usVal == 0))
	 {
	    status = CAD_REJECT;
	    strcpy(pCad->mess,"Preset on arSetup must be done");
	    cicsLogMessage(DBG_NOLOG, pCad->mess);
	 }

	 /* If it was, then set the CAR and DONE records to busy
	    and start this task
	    */
	 if(status == CAD_ACCEPT)
	 {

	    status = setCar(ARSETUP_CAR,CAR_BUSY,OK,"",dummy);
	    if(status == OK)
	    {
          cicsLogMessage(DBG_MIN, "****Setting ARSETUP BUSY***");
	       sprintf(name,"%s%s.VAL",dbTop,ARSETUP_DONE);
	       usVal = NAAC_BUSY;
	       status = putDbInfo(name, pCad->mess, DBF_ENUM, &usVal);
	    }

	    if(status == OK) {
	       semGive(semArSetup);
	    } 
       else {
	       status = CAD_REJECT;
	       printf("arSetupChk CAD_REJECT, semaphore not given.\n");
	    }
	 }
	 break;

	 /* CAD STOP directive detected.      */
      case CAD_STOP:
	 cicsLogMessage( 1,"arSetupChk - STOP directive. Cannot be stopped.");
	 strcpy(pCad->mess,"Cannot be stopped");
	 status = CAD_REJECT;
	 break;

	 /* Unrecognised CAD directive detected. This is regarded as an error. */
      default:
	 sprintf( pCad->mess, "Unrecognized CAD directive" );
	 status = CAD_REJECT;
	 break;
   }

   return status;
}
