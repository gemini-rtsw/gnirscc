 /* #define DEBUG  */
static struct {
        void *v;
        char *c;
} rcsid = {
        &rcsid,
        "$Id: hk.c,v 1.2 2009/05/27 19:33:33 fkraemer Exp $"
};
int healthdebug;
extern long hkDelay;
/*
 * Copyright 1997 Association of Universities for Research in Astronomy, Inc.
 * See the file COPYRIGHT for more details.
 *
 * FILENAME
 * hk.c
 *
 * DESCRIPTION
 * This file contains the source for all the functions used by the
 * NAAC system Housekeeping CAD records. These functions are used to  
 * validate the arguments given to the CAD record for the housekeeping 
 * variables.  Each function checks that the command is acceptable and 
 * returns a status. A message is supplied for rejected commands or error 
 * conditions.  The status and message is subsequently written to the 
 * VAL and MESS fields of the CAD record by the record support routines. 
 * 
 * FUNCTION NAME(S)
 * checkHk - process the housekeeping CAD directives
 * updateHlthVars - calculate values for health status variables and place them
 *            in the EPICS DB
 *   
 * DEPENDENCIES
 * The names of the subroutines in this file should be identical to those
 * declared in the SNAM field of each CAD record. If a change is made to
 * the name of a subroutine, that change should be reflected in the SNAM
 * field, and vice versa.
 *
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
 * $Log: hk.c,v $
 * Revision 1.2  2009/05/27 19:33:33  fkraemer
 * fkraemer - copied my complete working dir over trunk
 *
 * Revision 1.1.1.1  1998/12/15 16:18:47  buchholz
 * Imported gnaacSrc into CVS
 *
 * Revision 1.2  1998/11/20 17:16:24  pruckle
 * speed up saver, log messages
 *
 * Revision 1.1.1.1  1998/09/30 16:40:22  pruckle
 * Initial Release
 *
 *INDENT-ON* 
 */


#include <epCommon.h>
#include "epTypedefs.h"
#include <genSubRecord.h>
#include <subCadRecord.h>
#include <naacTasks.h>

long assignVal(unsigned short type, void *inVal, void *outVal, char *errMess);
long checkHealth(char *name,char * errMess, double base, double warmMin, 
		 double warnMax, double badMin, double badMax) ;
long checkHealthT(char *top,char *name, char *errMess, double base, double warnMin,  double warnMax, double badMin, double badMax) ;
long checkVal(double val, double base, double warmMin, double warnMax, 
	      double badMin, double badMax,char * errMess) ;
/* global containing limits info */
extern hkLimits_t hkChanDesc[MAX_HK_CHANNELS][NUM_HK_INPUTS];
extern char *dbTop;
/*
 *+
 * FUNCTION NAME:
 * checkHk
 *
 * INVOCATION:
 * struct cadRecord *pCad;
 * status = checkHk( pCad );
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * ! pCad   (struct cadRecord *)   pointer to CAD data structure
 *
 * FUNCTION VALUE:
 * long  Status value written to CAD VAL field
 *
 * PURPOSE:
 * User defined function for "init" CAD record
 *
 * DESCRIPTION:
 * This routine is called whenever the init CAD record is processed.
 * init is the command for ????  
 * The command has ???? argument, 
 *
 * EXTERNAL VARIABLES:
 * None
 *
 * PRIOR REQUIREMENTS:
 * It is assumed the CAD record has already been initialized and the
 * directive and any arguments have already been assembled into the cadRecord
 * data structure.
 *
 * DEFICIENCIES:
 * None
 *
 * HISTORY:
 * 17-Jan-1997  Original template adapted from CICS alpha 1.0   J.E. Tvedt
 * 03-Feb-1997  Original function   T. Morgan
 *
 *-
 */

long checkHK( struct genSubRecord *pGenSub )
{
    long retStat, tmpStat;
    int group, channel, warnings, bads;
  
    retStat = STATUS_GOOD;
    warnings = 0;
    bads = 0;
    return OK;
    

    group = *(long *)pGenSub->a;
   
   
    for(channel=0; channel<16; channel++)
    {
		tmpStat = procInput(group, channel);
		if (tmpStat == STATUS_WARNING) warnings++; 
		if (tmpStat == STATUS_BAD) bads++;
		if (tmpStat > retStat) retStat = tmpStat;
    }

    /* vala is group for status value return, valb for message return */
    *(long *)pGenSub->vala = retStat;            
    sprintf(pGenSub->valb, "Warnings: %d, Bads: %d", warnings, bads);


    return retStat;

}

/*
 *+
 * FUNCTION NAME:
 * updateHlthVars
 *
 * INVOCATION:
 * struct cadRecord *pCad;
 * status = updateHlthVars( pCad );
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * ! pCad   (struct cadRecord *)   pointer to CAD data structure
 *
 * FUNCTION VALUE:
 * long  Status value written to CAD VAL field
 *
 * PURPOSE:
 * Calculate values for health status variables and place them in the EPICS DB
 *
 * DESCRIPTION:
 * This routine is called to update the values of the array controller health
 * values in the EPICS database.
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
 *
 *-
 */

long updateHlthVars(struct subCadRecord *pCad)
{
	char str[80];
	char buf[80];
    long retval,volt,pwr;
    long status =  0;
	static long numUpdate;
    struct dbAddr dbaVolt, dbaTemp, dbaADC, dbaWFire, dbaPreAmp, dbaDQ,
		dbaPwrSup, dbaGFCI, dbaOverall;
    char voltHlth[MAX_STRING_SIZE], tempHlth[MAX_STRING_SIZE], 
		wFireHlth[MAX_STRING_SIZE], adcHlth[MAX_STRING_SIZE], 
		dqHlth[MAX_STRING_SIZE],	pwrSupHlth[MAX_STRING_SIZE], 
		gfciHlth[MAX_STRING_SIZE];
 
    /*   long voltHlth, tempHlth, wFireHlth, adcHlth, preAmpHlth, dqHlth, */
    /* 	pwrSupHlth, gfciHlth,overallHlth; */

  
   
    char tmpstr[256];	
	cicsLogMessage(DBG_FULL,"updateHealthVars\n");
    if (++numUpdate % 2 != 0)
		return OK;	
	/*     if (++numUpdate % hkDelay != 0) */
	/*       return OK;	 */
  
    /* same for all directives to CAD and it only gets one directive anyway */
   
    
    /* setup direct access to EPICS DB variables */
	sprintf(buf,"%s%s.VAL",dbTop,VOLTHLTH);
    if ( dbNameToAddr(buf, &dbaVolt) != OK)
    {
		sprintf(tmpstr,"updateHlthVars - unable to find \"%s\" in EPICS database.",
				buf);
		cicsLogMessage(0,tmpstr);
		return ERROR;
    }	
#if 0
	sprintf(buf,"%s%s.VAL",dbTop,TEMPHLTH);
    if ( dbNameToAddr(buf, &dbaTemp)!= OK)
    {
		sprintf(tmpstr,"updateHlthVars - unable to find \"%s\" in EPICS database.",
				buf); 
		cicsLogMessage(0,tmpstr);
		return ERROR;
    }	
#endif
	sprintf(buf,"%s%s.VAL",dbTop,ADCHLTH);
	if ( dbNameToAddr(buf, &dbaADC)!= OK)
	{
		sprintf(tmpstr,"updateHlthVars - unable to find \"%s\" in EPICS database.",
				buf);
		cicsLogMessage(0,tmpstr); 
		return ERROR;
	}	
   
	sprintf(buf,"%s%s.VAL",dbTop,PREAMPHLTH);
    if (dbNameToAddr(buf, &dbaPreAmp) != OK)
    {
		sprintf(tmpstr,"updateHlthVars - unable to find \"%s\" in EPICS database.",
				buf);
		cicsLogMessage(0,tmpstr);
		return ERROR;
    }	

	sprintf(buf,"%s%s.VAL",dbTop,WFIREHLTH);
    if ( dbNameToAddr(buf, &dbaWFire)!= OK)
    {
		sprintf(tmpstr,"updateHlthVars - unable to find \"%s\" in EPICS database.",
				buf);
		cicsLogMessage(0,tmpstr); 
		return ERROR;
    }	

	sprintf(buf,"%s%s.VAL",dbTop,DQHLTH);
    if (dbNameToAddr(buf, &dbaDQ) != OK)
    {
		sprintf(tmpstr,"updateHlthVars - unable to find \"%s\" in EPICS database.",
				buf);
		cicsLogMessage(0,tmpstr); 
		return ERROR;
    }	

	sprintf(buf,"%s%s.VAL",dbTop,PWRSUPHLTH);
    if (dbNameToAddr(buf, &dbaPwrSup) != OK)
    {
		sprintf(tmpstr,"updateHlthVars - unable to find \"%s\" in EPICS database.",
				buf);
		cicsLogMessage(0,tmpstr); 
		return ERROR;
    }	

	sprintf(buf,"%s%s.VAL",dbTop,GFCIHLTH);
    if (dbNameToAddr(buf, &dbaGFCI) != OK)
    {
		sprintf(tmpstr,"updateHlthVars - unable to find \"%s\" in EPICS database.",
				buf);
		cicsLogMessage(0,tmpstr); 
		return ERROR;
    }	

	sprintf(buf,"%s%s.VAL",dbTop,OVERALLHLTH);
    if ( dbNameToAddr(buf, &dbaOverall) != OK)
    {
		sprintf(tmpstr,"updateHlthVars - unable to find \"%s\" in EPICS database.",
				buf);
		cicsLogMessage(0,tmpstr); 
		return ERROR;
    }	

    /* Calculate & set controller array voltage health */  
    /*    printf ("start Health Checks\n");*/ 
    sprintf (str,"\n\nvolthealth");  
    cicsLogMessage(DBG_FULL,str);
    volt = retval= calcVoltsHlth(voltHlth);
    sprintf (str,"volthealth %d\n",retval);  
    cicsLogMessage(DBG_FULL,str);
    assignVal(DBF_LONG, &retval, pCad->valb, tmpstr);
    if(retval > status)
		status = retval;

    /* Calculate & set controller temperature health */ 
    sprintf (str,"\n\ntemp health \n");
    cicsLogMessage(DBG_FULL,str);
#if 0
    retval = calcTempHlth(tempHlth);
#endif
    assignVal(DBF_LONG, &retval, pCad->valc, tmpstr);
    sprintf (str,"temp health = %d \n\n",retval);
    cicsLogMessage(DBG_FULL,str);
    if(retval > status)
		status = retval;

     /*  return retval; */
    /* Calculate & set controller analog-to-digitial module health */
    retval  = calcAdcHlth(adcHlth);
    assignVal(DBF_LONG, &retval, pCad->vald, tmpstr);	
    sprintf (str,"adchealth = %d\n\n",retval); 
    cicsLogMessage(DBG_FULL,str);
    if(retval > status)
		status = retval;

  

    /* Calculate & set controller WildFire board health */
	retval = calcwFireHlth(wFireHlth);
    assignVal(DBF_LONG, &retval, pCad->valf, tmpstr);
    sprintf (str,"wFire health = %d\n\n",retval);
    cicsLogMessage(DBG_FULL,str);
    if(retval > status)
		status = retval;

/*     return retval;  */

    /* Calculate & set DataCube health */
    retval = calcDqHlth(dqHlth); 
    assignVal(DBF_LONG, &retval, pCad->valg, tmpstr);
    sprintf (str,"dq health = %d\n\n",retval); 
    cicsLogMessage(DBG_FULL,str);
    if(retval > status)
		status = retval;
 
    /* Calculate & set Power Supply health */ 
    sprintf (str,"\n\npwrSup health \n"); 
    cicsLogMessage(DBG_FULL,str);
    pwr =  retval = calcPwrSupHlth(pwrSupHlth);
    assignVal(DBF_LONG, &retval, pCad->valh, tmpstr); 
    sprintf (str,"pwrSup health = %d\n\n",retval); 
    cicsLogMessage(DBG_FULL,str);
    if(retval > status)
		status = retval;

   /* Calculate & set controller analog preamp module health */
    if((pwr == STATUS_BAD)||(volt == STATUS_BAD))
		retval = STATUS_BAD;
    else if((pwr == STATUS_WARNING)||(volt == STATUS_WARNING))
		retval = STATUS_WARNING;
    else
		retval = STATUS_GOOD;
    assignVal(DBF_LONG, &retval, pCad->vale, tmpstr);
    sprintf (str,"preAmp health = %d\n\n",retval); 
    cicsLogMessage(DBG_FULL,str);
    if(retval > status)
		status = retval;

    /* Calculate & set GFCI health */
    retval  = calcGfciHlth(gfciHlth); 
    assignVal(DBF_LONG, &retval, pCad->vali, tmpstr);
    sprintf (str,"gfci health = %d\n\n",retval); 
    cicsLogMessage(DBG_FULL,str);
    if(retval > status)
		status = retval;
  
    
    /* Calculate & set controller overall health */
  /*   retval = calcOverallHlth(overallHlth,status);  */
    sprintf(buf,"calcOverallHlth retval = %d, status = %d\n",retval,status);
    cicsLogMessage(DBG_FULL,"calcOverallHlth\n");
    assignVal(DBF_LONG, &status, pCad->vala, tmpstr);
    sprintf (str,"overall health %d\n\n\n",status); 
    cicsLogMessage(DBG_FULL,str);
    

    return retval;
}


int calcVoltsHlth(char *str)
{
    long status=0,retval=0;
    double val;
    long arSetupDone;
   
    
    hkLimits_t hkChan;
    strcpy(str, HLTHGOOD);
 
    /*only check arSetup if arsetupdone is NAAC_DONE*/
    if (getDbInfoT(dbTop, ARSETUP_DONE,str,DBF_LONG, &arSetupDone) != OK)
		return STATUS_ERROR;

    if (arSetupDone == NAAC_DONE) 
    {
		if (getDbInfoT(dbTop, ARSETUP_CAD ".VALD",str,DBF_DOUBLE,&val) != OK)
			return STATUS_ERROR;
		setEpicsAlarmT(dbTop, VDDCL1_CHAN,-val-.75,-val+.75,-val-1.0,-val+1.0);   
		/* 	printf("calcVoltsHlth vddcl1, val = %f, %f - %f  %f - %f \n",-val,-val-.05,-val+.05,-val-.5,-val+.5);  */
		if ((status = checkHealthT(dbTop, VDDCL1_CHAN, str, -val, -.75, .75, 
								   -1.0, 1.0)) == STATUS_BAD )
			return status;
		else if(status == STATUS_WARNING )
			retval = status;
		else if(status == STATUS_ERROR)
			return STATUS_ERROR;
    
		if (getDbInfoT(dbTop, ARSETUP_CAD  ".VALE",str,DBF_DOUBLE,&val) != OK)
			return STATUS_ERROR;
		setEpicsAlarmT(dbTop, VDDCL2_CHAN,-val-.75,-val+.75,-val-1.0,-val+1.0);  
		if ((status = checkHealthT(dbTop, VDDCL2_CHAN, str, -val, -.75, .75, 
								   -1.0, 1.0)) == STATUS_BAD )
			return status;
		else if(status == STATUS_WARNING )
			retval = status;
		else if(status == STATUS_ERROR)
			return STATUS_ERROR;
    
		if (getDbInfoT(dbTop, ARSETUP_CAD  ".VALF",str,DBF_DOUBLE,&val) != OK)
			return STATUS_ERROR;
		setEpicsAlarmT(dbTop, VGGCL1_CHAN,-val-.75,-val+.75,-val-1.0,-val+1.0);  
		if ((status = checkHealthT(dbTop, VGGCL1_CHAN, str, -val, -.75, .75, 
								   -1.0, 1.0 )) == STATUS_BAD )
			return status;
		else if(status == STATUS_WARNING )
			retval = status;
		else if(status == STATUS_ERROR)
			return STATUS_ERROR;
    
		if (getDbInfoT(dbTop, ARSETUP_CAD  ".VALG",str,DBF_DOUBLE,&val) != OK)
			return STATUS_ERROR;
		setEpicsAlarmT(dbTop, VGGCL2_CHAN,-val-.75,-val+.75,-val-1.0,-val+1.0);  
		if ((status = checkHealthT(dbTop, VGGCL2_CHAN, str, -val, -.75, .75, 
								   -1.0, 1.0)) == STATUS_BAD )
			return status;
		else if(status == STATUS_WARNING )
			retval = status;
		else if(status == STATUS_ERROR)
			return STATUS_ERROR;
    
		if (getDbInfoT(dbTop, ARSETUP_CAD  ".VALC",str,DBF_DOUBLE,&val) != OK)
			return STATUS_ERROR;
		setEpicsAlarmT(dbTop, VSET_CHAN,-val-.75,-val+.75,-val-1.0,-val+1.0);  
		if ((status = checkHealthT(dbTop, VSET_CHAN, str, -val,-.75, .75, 
								   -1.0, 1.0)) == STATUS_BAD )
			return status;
		else if(status == STATUS_WARNING )
			retval = status;
		else if(status == STATUS_ERROR)
			return STATUS_ERROR;
    
		if (getDbInfoT(dbTop, ARSETUP_CAD  ".VALI",str,DBF_DOUBLE,&val) != OK)
			return STATUS_ERROR;
		setEpicsAlarmT(dbTop, VDET_CHAN,-val-.75,-val+.75,-val-1.0,-val+1.0);  
		if ((status = checkHealthT(dbTop, VDET_CHAN, str, -val, -.75, .75, 
								   -1.0, 1.0)) == STATUS_BAD )
			return status;
		else if(status == STATUS_WARNING )
			retval = status;
		else if(status == STATUS_ERROR)
			return STATUS_ERROR;
    }
   
   
     
    hkChan = hkChanDesc[4][5];
    if ((status = checkHealthT(dbTop, "c64_79:a5", str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
    {
		/* printf ("status %d\n",status); */
		return status;
    }
    else if(status == STATUS_WARNING )
    {
		/* printf ("status %d\n",status); */
		retval = status;
    }
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    
  
   
    hkChan = hkChanDesc[4][9];
    if ((status = checkHealthT(dbTop, "c64_79:a9", str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    
   
   
/*     hkChan = hkChanDesc[4][2]; */
/*     if ((status = checkHealthT(dbTop, "c64_79:a2", str,hkChan.base, */
/* 							   hkChan.warn_min, hkChan.warn_max,  */
/* 							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD ) */
/* 		return status; */
/*     else if(status == STATUS_WARNING ) */
/* 		retval = status; */
/*     else if(status == STATUS_ERROR) */
/* 		return STATUS_ERROR; */
    
   
    hkChan = hkChanDesc[4][8];
    if ((status = checkHealthT(dbTop, "c64_79:a8", str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    
#if 0
    /* this voltage will be VDet if array is activated VddUC if deactivated we need to
     * check activation status and compare to the appropriate voltage level
     */ 
    hkChan = hkChanDesc[4][3];
    if ((status = checkHealthT(dbTop, "c64_79:a3", str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
#endif   
  
   
    hkChan = hkChanDesc[3][7];
    if ((status = checkHealthT(dbTop, "c48_63:a7", str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    
    
    hkChan = hkChanDesc[3][0];
    if ((status = checkHealthT(dbTop, "c48_63:a0", str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    
   
    hkChan = hkChanDesc[3][10];
    if ((status = checkHealthT(dbTop, "c48_63:a10", str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    
    hkChan = hkChanDesc[5][10];
    if ((status = checkHealthT(dbTop, "c80_95:a10", str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    
   
   
    hkChan = hkChanDesc[5][2];
    if ((status = checkHealthT(dbTop, "c80_95:a2", str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    
   
   
    hkChan = hkChanDesc[5][7];
    if ((status = checkHealthT(dbTop, "c80_95:a7", str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    
   
  
    hkChan = hkChanDesc[5][13];
    if ((status = checkHealthT(dbTop, "c80_95:a13", str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    
    
  
    hkChan = hkChanDesc[0][0];
    if ((status = checkHealthT(dbTop, "c0_15:a0", str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    
   
  
  
    
 
  
    hkChan = hkChanDesc[0][15];
    if ((status = checkHealthT(dbTop, "c0_15:a15", str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
   
  
    hkChan = hkChanDesc[1][2];
    if ((status = checkHealthT(dbTop, "c16_31:a2", str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    
    
  
    hkChan = hkChanDesc[1][14];
    if ((status = checkHealthT(dbTop, "c16_31:a14", str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    
   
   
    hkChan = hkChanDesc[2][1];
    if ((status = checkHealthT(dbTop, "c32_47:a1", str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status; 
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    
   
   
    hkChan = hkChanDesc[2][13];
    if ((status = checkHealthT(dbTop, "c32_47:a13", str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = STATUS_WARNING;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;

    if (retval == STATUS_GOOD)
		strcpy(str, HLTHGOOD); 
    else if (retval == STATUS_WARNING)
		strcpy(str, HLTHWARN); 
  
    return (retval);
}

long checkHealthT(char *top,char *name, char *errMess, double base, double warnMin,  double warnMax, double badMin, double badMax) 
{
	char n[80];
	strcpy(n,top);
	strcat(n,name);
	return checkHealth(n, errMess, base, warnMin, 
					   warnMax,  badMin,  badMax);
}
long checkHealth(char *name, char *errMess, double base, double warnMin, 
				 double warnMax, double badMin, double badMax) 
{
    double val;
    long status;
    char str[256];  
    sprintf (str,"checkHealth %s   ", name );  
   
	/*       cicsLogMessage(DBG_FULL,str);  */ 
    if (getDbInfo(name, errMess, DBF_DOUBLE, &val) != OK)
		return STATUS_ERROR;

    
    status = checkVal(val, base, warnMin, warnMax, badMin, badMax,errMess);
	/*  sprintf (str,"val = %f base = %f warnMin = %f warnMax = %f badMin = %f badMax = %f\n",val, base, base+warnMin, base+warnMax, base+badMin, base+badMax );   */
   
    /*   cicsLogMessage(DBG_FULL,str);   */


    return status;
}

long checkVal(double val, double base, double warnMin, double warnMax, 
			  double badMin, double badMax,char *errMess) 
{
	char str[80];
	long status=STATUS_GOOD;
   
    if (val > (base+ badMax))
	{
		status = STATUS_BAD;
		sprintf (str,"status 1 BAD %f > %f\n",val,base + badMax);
		cicsLogMessage(DBG_FULL,str);  
	}
    
    else if (val < (base + badMin))
	{
		status = STATUS_BAD;
		sprintf (str,"status 2 BAD %f < %f\n",val,base + badMin);
		cicsLogMessage(DBG_FULL,str);  
	}
    else if (val > (base + warnMax))
	{
		status = STATUS_WARNING;
		sprintf (str,"status 3 WARNING %f > %f\n",val,base + warnMax);
		cicsLogMessage(DBG_FULL,str);  
	}
    else if(val < (base + warnMin))	
	{
		status = STATUS_WARNING;
		sprintf (str,"status 4 WARNING  %f  < %f\n",val,base + warnMin);
		cicsLogMessage(DBG_FULL,str);  
	}
    return status;
}

#if 0
int calcTempHlth(char *str)
{
    char name[80];
    double val;
    long status;
    
    sprintf(name,"%s%s",dbTop,TEMP_DET_ERROR);
    /* check temperature Error field for foot and integrating block ??????? */ 
    if (getDbInfo(name, str, DBF_DOUBLE, &val) != OK)
    { 
		sprintf(name,"\n\n ERROR TempHlth error = %f\n",val);
		cicsLogMessage(DBG_NOLOG,name);
		return STATUS_ERROR;
    }
  
    if ((val > 1000)||(val < -1000))
    {
		strcpy(str, HLTHBAD);
		status = STATUS_BAD;	
    }
    else if ((val > 50)||(val < -50))
    {
		strcpy(str, HLTHWARN);
		status = STATUS_WARNING;
    }
    else
    {
		status = STATUS_GOOD;
		strcpy(str, HLTHGOOD);
    }
    return (status);
}
#endif
int calcAdcHlth(char *str)
{
    long status=0,retval=0;
    hkLimits_t hkChan;
  
  
    hkChan = hkChanDesc[8][0];
    if ((status = checkHealthT(dbTop, "c128_143:a0", str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    
 

    hkChan = hkChanDesc[8][1];
    if ((status = checkHealthT(dbTop, "c128_143:a1", str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    
   

    hkChan = hkChanDesc[8][2];
    if ((status = checkHealthT(dbTop, "c128_143:a2", str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    
 

    hkChan = hkChanDesc[8][3];
    if ((status = checkHealthT(dbTop, "c128_143:a3", str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    

  
    hkChan = hkChanDesc[8][4];
    if ((status = checkHealthT(dbTop, "c128_143:a4", str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    
   

    hkChan = hkChanDesc[8][5];
    if ((status = checkHealthT(dbTop, "c128_143:a5", str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    

  
    hkChan = hkChanDesc[8][6];
    if ((status = checkHealthT(dbTop, "c128_143:a6", str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    
   

    hkChan = hkChanDesc[8][7];
    if ((status = checkHealthT(dbTop, "c128_143:a7", str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    
   

    hkChan = hkChanDesc[8][8];
    if ((status = checkHealthT(dbTop, "c128_143:a8", str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    

  
    hkChan = hkChanDesc[8][9];
    if ((status = checkHealthT(dbTop, "c128_143:a9", str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    
   

    hkChan = hkChanDesc[8][10];
    if ((status = checkHealthT(dbTop, "c128_143:a10", str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    
  

    hkChan = hkChanDesc[8][11];
    if ((status = checkHealthT(dbTop, "c128_143:a11", str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    
  

    hkChan = hkChanDesc[8][12];
    if ((status = checkHealthT(dbTop, "c128_143:a12", str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    
  

    hkChan = hkChanDesc[8][13];
    if ((status = checkHealthT(dbTop, "c128_143:a13", str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    

   
    hkChan = hkChanDesc[8][14];
    if ((status = checkHealthT(dbTop, "c128_143:a14", str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status; 
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    
   

    hkChan = hkChanDesc[9][15];
    if ((status = checkHealthT(dbTop, "c128_143:a15", str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    


  
    hkChan = hkChanDesc[9][0];
    if ((status = checkHealthT(dbTop, "c144_159:a0", str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    
  
    hkChan = hkChanDesc[9][1];
    if ((status = checkHealthT(dbTop, "c144_159:a1", str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    

  
    hkChan = hkChanDesc[9][2];
    if ((status = checkHealthT(dbTop, "c144_159:a2", str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    
  

    hkChan = hkChanDesc[9][3];
    if ((status = checkHealthT(dbTop, "c144_159:a3", str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    
   

    hkChan = hkChanDesc[9][4];
    if ((status = checkHealthT(dbTop, "c144_159:a4", str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    
  

    hkChan = hkChanDesc[9][5];
    if ((status = checkHealthT(dbTop, "c144_159:a5", str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    
  

    hkChan = hkChanDesc[9][6];
    if ((status = checkHealthT(dbTop, "c144_159:a6", str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    
  

    hkChan = hkChanDesc[9][7];
    if ((status = checkHealthT(dbTop, "c144_159:a7", str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    

   
    hkChan = hkChanDesc[9][8];
    if ((status = checkHealthT(dbTop, "c144_159:a8", str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    
   

    hkChan = hkChanDesc[9][9];
    if ((status = checkHealthT(dbTop, "c144_159:a9", str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    
   

    hkChan = hkChanDesc[9][10];
    if ((status = checkHealthT(dbTop, "c144_159:a10", str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    
  

    hkChan = hkChanDesc[9][11];
    if ((status = checkHealthT(dbTop, "c144_159:a11", str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    

 
    hkChan = hkChanDesc[9][12];
    if ((status = checkHealthT(dbTop, "c144_159:a12", str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    
  

    hkChan = hkChanDesc[9][13];
    if ((status = checkHealthT(dbTop, "c144_159:a13", str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    


    hkChan = hkChanDesc[9][14];
    if ((status = checkHealthT(dbTop, "c144_159:a14", str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    
  
    hkChan = hkChanDesc[9][15];
    if ((status = checkHealthT(dbTop, "c144_159:a15", str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    


  

    hkChan = hkChanDesc[12][0];
    if ((status = checkHealthT(dbTop, "c192_207:a0", str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    
   

    hkChan = hkChanDesc[12][1];
    if ((status = checkHealthT(dbTop, "c192_207:a1", str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    

   
    hkChan = hkChanDesc[12][2];
    if ((status = checkHealthT(dbTop, "c192_207:a2", str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    
   

    hkChan = hkChanDesc[12][3];
    if ((status = checkHealthT(dbTop, "c192_207:a3", str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    
   

    hkChan = hkChanDesc[12][4];
    if ((status = checkHealthT(dbTop, "c192_207:a4", str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    
  

    hkChan = hkChanDesc[12][5];
    if ((status = checkHealthT(dbTop, "c192_207:a5", str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    
  

    hkChan = hkChanDesc[12][6];
    if ((status = checkHealthT(dbTop, "c192_207:a6", str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    

 
    hkChan = hkChanDesc[12][7];
    if ((status = checkHealthT(dbTop, "c192_207:a7", str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    
   

    hkChan = hkChanDesc[12][8];
    if ((status = checkHealthT(dbTop, "c192_207:a8", str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    

  
    hkChan = hkChanDesc[12][9];
    if ((status = checkHealthT(dbTop, "c192_207:a9", str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    

   
    hkChan = hkChanDesc[12][10];
    if ((status = checkHealthT(dbTop, "c192_207:a10", str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    
   

    hkChan = hkChanDesc[12][11];
    if ((status = checkHealthT(dbTop, "c192_207:a11", str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    
  

    hkChan = hkChanDesc[12][12];
    if ((status = checkHealthT(dbTop, "c192_207:a12", str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    

  
    hkChan = hkChanDesc[12][13];
    if ((status = checkHealthT(dbTop, "c192_207:a13", str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    

  
    hkChan = hkChanDesc[12][14];
    if ((status = checkHealthT(dbTop, "c192_207:a14", str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    
   

    hkChan = hkChanDesc[12][15];
    if ((status = checkHealthT(dbTop, "c192_207:a15", str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    
  

    hkChan = hkChanDesc[13][0];
    if ((status = checkHealthT(dbTop, "c208_223:a0", str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    

  
    hkChan = hkChanDesc[13][1];
    if ((status = checkHealthT(dbTop, "c208_223:a1", str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    
  

    hkChan = hkChanDesc[14][4];
    if ((status = checkHealthT(dbTop, "c224_239:a4", str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    
  

    hkChan = hkChanDesc[14][5];
    if ((status = checkHealthT(dbTop, "c224_239:a5", str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    

  
    hkChan = hkChanDesc[14][6];
    if ((status = checkHealthT(dbTop, "c224_239:a6", str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    

 
    hkChan = hkChanDesc[14][7];
    if ((status = checkHealthT(dbTop, "c224_239:a7", str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    

  
    hkChan = hkChanDesc[14][8];
    if ((status = checkHealthT(dbTop, "c224_239:a8", str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    
  

    hkChan = hkChanDesc[14][9];
    if ((status = checkHealthT(dbTop, "c224_239:a9", str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    
  

    hkChan = hkChanDesc[14][10];
    if ((status = checkHealthT(dbTop, "c224_239:a10", str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    

  
    hkChan = hkChanDesc[14][11];
    if ((status = checkHealthT(dbTop, "c224_239:a11", str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    

   
    hkChan = hkChanDesc[14][14];
    if ((status = checkHealthT(dbTop, "c224_239:a14", str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    

  
    hkChan = hkChanDesc[14][15];
    if ((status = checkHealthT(dbTop, "c224_239:a15", str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    

  
    hkChan = hkChanDesc[15][0];
    if ((status = checkHealthT(dbTop, "c240_255:a0", str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    

  
    hkChan = hkChanDesc[15][1];
    if ((status = checkHealthT(dbTop, "c240_255:a1", str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    

  
    hkChan = hkChanDesc[15][2];
    if ((status = checkHealthT(dbTop, "c240_255:a2", str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    

  
    hkChan = hkChanDesc[15][3];
    if ((status = checkHealthT(dbTop, "c240_255:a3", str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    

   
    hkChan = hkChanDesc[15][4];
    if ((status = checkHealthT(dbTop, "c240_255:a4", str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    

   
    hkChan = hkChanDesc[15][5];
    if ((status = checkHealthT(dbTop, "c240_255:a5", str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;

   
    hkChan = hkChanDesc[15][6];
    if ((status = checkHealthT(dbTop, "c240_255:a6", str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    
   

    hkChan = hkChanDesc[15][7];
    if ((status = checkHealthT(dbTop, "c240_255:a7", str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    

  
    hkChan = hkChanDesc[15][8];
    if ((status = checkHealthT(dbTop, "c240_255:a8", str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    


  
    hkChan = hkChanDesc[15][10];
    if ((status = checkHealthT(dbTop, "c240_255:a10", str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    
  

    hkChan = hkChanDesc[15][11];
    if ((status = checkHealthT(dbTop, "c240_255:a11", str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    

   
    hkChan = hkChanDesc[15][12];
    if ((status = checkHealthT(dbTop, "c240_255:a12", str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status; 
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    


    hkChan = hkChanDesc[15][13];
    if ((status = checkHealthT(dbTop, "c240_255:a13", str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    
  
    if (retval == STATUS_GOOD)
		strcpy(str, HLTHGOOD); 
    else
		strcpy(str, HLTHWARN);
    
    return (retval);
}
#if 0
int calcPaHlth(char *str)
{
    strcpy(str, HLTHGOOD);
    return (STATUS_GOOD);
}
#endif
int calcwFireHlth(char *str)
{
    long val;
   
    /* check trace flag on each transputer*/
    if (getDbInfoT(dbTop,"seqTraceFlag",str,DBF_LONG,&val) != OK)
	{
		strcpy(str, HLTHBAD);
		return STATUS_BAD;
	}
    if (getDbInfoT(dbTop,"instTraceFlag",str,DBF_LONG,&val) != OK)
	{
	
		strcpy(str, HLTHBAD);
		return STATUS_BAD;
	}
    strcpy(str, HLTHGOOD);
    return (STATUS_GOOD);
}

int calcDqHlth(char *str)
{/* ??? */
    strcpy(str, HLTHGOOD);
    return (STATUS_GOOD);
}

int calcPwrSupHlth(char *str)
{ 
    long status,retval;
    hkLimits_t hkChan;
   
	retval = STATUS_GOOD;
    hkChan = hkChanDesc[0][6];
    if ((status = checkHealthT(dbTop, "c0_15:a6" , str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    

   /*  hkChan = hkChanDesc[0][9]; */
/*     if ((status = checkHealthT(dbTop, "c0_15:a9" , str,hkChan.base, */
/* 							   hkChan.warn_min, hkChan.warn_max,  */
/* 							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD ) */
/* 		return status; */
/*     else if(status == STATUS_WARNING ) */
/* 		retval = status; */
/*     else if(status == STATUS_ERROR) */
/* 		return STATUS_ERROR; */
    

    hkChan = hkChanDesc[0][12];
    if ((status = checkHealthT(dbTop, "c0_15:a12" , str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    

    hkChan = hkChanDesc[1][5];
    if ((status = checkHealthT(dbTop, "c16_31:a5" , str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;

    hkChan = hkChanDesc[1][11];
    if ((status = checkHealthT(dbTop, "c16_31:a11" , str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    

    hkChan = hkChanDesc[2][4];
    if ((status = checkHealthT(dbTop, "c32_47:a4" , str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    

   /*  hkChan = hkChanDesc[2][7]; */
/*     if ((status = checkHealthT(dbTop, "c32_47:a7" , str,hkChan.base, */
/* 							   hkChan.warn_min, hkChan.warn_max,  */
/* 							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD ) */
/* 		return status; */
/*     else if(status == STATUS_WARNING ) */
/* 		retval = status; */
/*     else if(status == STATUS_ERROR) */
/* 		return STATUS_ERROR; */
    

    hkChan = hkChanDesc[2][10];
    if ((status = checkHealthT(dbTop, "c32_47:a10" , str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    

    hkChan = hkChanDesc[3][3];
    if ((status = checkHealthT(dbTop, "c48_63:a3" , str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
   

   /*  hkChan = hkChanDesc[3][6]; */
/*     if ((status = checkHealthT(dbTop, "c48_63:a6" , str,hkChan.base, */
/* 							   hkChan.warn_min, hkChan.warn_max,  */
/* 							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD ) */
/* 		return status; */
/*     else if(status == STATUS_WARNING ) */
/* 		retval = status; */
/*     else if(status == STATUS_ERROR) */
/* 		return STATUS_ERROR; */
    

    hkChan = hkChanDesc[3][9];
    if ((status = checkHealthT(dbTop, "c48_63:a9" , str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    

    hkChan = hkChanDesc[3][12];
    if ((status = checkHealthT(dbTop, "c48_63:a12" , str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    

    hkChan = hkChanDesc[3][15];
    if ((status = checkHealthT(dbTop, "c48_63:a15" , str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    

    hkChan = hkChanDesc[4][6];
    if ((status = checkHealthT(dbTop, "c64_79:a6" , str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    

    hkChan = hkChanDesc[4][15];
    if ((status = checkHealthT(dbTop, "c64_79:a15" , str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    

    hkChan = hkChanDesc[5][3];
    if ((status = checkHealthT(dbTop, "c80_95:a3" , str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    

    hkChan = hkChanDesc[5][4];
    if ((status = checkHealthT(dbTop, "c80_95:a4" , str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    

    hkChan = hkChanDesc[5][5];
    if ((status = checkHealthT(dbTop, "c80_95:a5" , str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    

    hkChan = hkChanDesc[5][6];
    if ((status = checkHealthT(dbTop, "c80_95:a6" , str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    

    hkChan = hkChanDesc[5][8];
    if ((status = checkHealthT(dbTop, "c80_95:a8" , str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    

    hkChan = hkChanDesc[5][9];
    if ((status = checkHealthT(dbTop, "c80_95:a9" , str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    

    hkChan = hkChanDesc[5][11];
    if ((status = checkHealthT(dbTop, "c80_95:a11" , str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    

    hkChan = hkChanDesc[5][12];
    if ((status = checkHealthT(dbTop, "c80_95:a12" , str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    

    hkChan = hkChanDesc[5][14];
    if ((status = checkHealthT(dbTop, "c80_95:a14" , str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    

  
    

    hkChan = hkChanDesc[6][5];
    if ((status = checkHealthT(dbTop, "c96_111:a5" , str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    

    hkChan = hkChanDesc[6][6];
    if ((status = checkHealthT(dbTop, "c96_111:a6" , str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    

    hkChan = hkChanDesc[6][7];
    if ((status = checkHealthT(dbTop, "c96_111:a7" , str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    

    hkChan = hkChanDesc[6][8];
    if ((status = checkHealthT(dbTop, "c96_111:a8" , str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;

    hkChan = hkChanDesc[6][9];
    if ((status = checkHealthT(dbTop, "c96_111:a9" , str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    

    hkChan = hkChanDesc[6][10];
    if ((status = checkHealthT(dbTop, "c96_111:a10" , str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    

    hkChan = hkChanDesc[6][11];
    if ((status = checkHealthT(dbTop, "c96_111:a11" , str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    

    hkChan = hkChanDesc[6][12];
    if ((status = checkHealthT(dbTop, "c96_111:a12" , str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    

    if (retval == STATUS_GOOD)
		strcpy(str, HLTHGOOD); 
    else
		strcpy(str, HLTHWARN); 

    return (retval);
}

int calcGfciHlth(char *str)
{  
    hkLimits_t hkChan;
  
    long status,retval=0;

    strcpy(str, HLTHGOOD);
    return (STATUS_GOOD);

    hkChan = hkChanDesc[5][15];
    if ((status = checkHealthT(dbTop, PAGFCI , str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    

    hkChan = hkChanDesc[13][15];
    if ((status = checkHealthT(dbTop, ADCGFCI , str,hkChan.base,
							   hkChan.warn_min, hkChan.warn_max, 
							   hkChan.bad_min, hkChan.bad_max)) == STATUS_BAD )
		return status;
    else if(status == STATUS_WARNING )
		retval = status;
    else if(status == STATUS_ERROR)
		return STATUS_ERROR;
    
#if 0
    pawarnLo = hkChanDesc[5][15].base + hkChanDesc[5][15].warn_min;
    pawarnHi = hkChanDesc[5][15].base + hkChanDesc[5][15].warn_max;
    pabadLo = hkChanDesc[5][15].base + hkChanDesc[5][15].bad_min;
    pabadHi = hkChanDesc[5][15].base + hkChanDesc[5][15].bad_max;
    adcwarnLo = hkChanDesc[13][15].base + hkChanDesc[13][15].warn_min;
    adcwarnHi = hkChanDesc[13][15].base + hkChanDesc[13][15].warn_max;
    adcbadLo = hkChanDesc[13][15].base + hkChanDesc[13][15].bad_min;
    adcbadHi = hkChanDesc[13][15].base + hkChanDesc[13][15].bad_max;

    getDbInfoT(dbTop, PAGFCI ".VAL", dummy, DBF_DOUBLE, &paGFCI);
    getDbInfoT(dbTop, ADCGFCI ".VAL", dummy, DBF_DOUBLE, &adcGFCI);
  
    if(paGFCI <= pabadLo || paGFCI >= pabadHi ||  
       adcGFCI <= adcbadLo || adcGFCI >= adcbadHi)
    {
		strcpy(str, HLTHBAD);
		return (STATUS_BAD);
    }
  
    if(paGFCI <= pawarnLo || paGFCI >= pawarnHi ||  
       adcGFCI <= adcwarnLo || adcGFCI >= adcwarnHi)
    {
		strcpy(str, HLTHWARN);
		return (STATUS_WARNING);
    }
#endif 
    /*  strcpy(str, HLTHGOOD); */
    return (retval);
}

int calcOverallHlth(char *str)
{
	/*   char voltHlth[MAX_STRING_SIZE], tempHlth[MAX_STRING_SIZE], */
	/* 	wFireHlth[MAX_STRING_SIZE], adcHlth[MAX_STRING_SIZE], */
	/* 	preAmpHlth[MAX_STRING_SIZE], dqHlth[MAX_STRING_SIZE], */
	/* 	pwrSupHlth[MAX_STRING_SIZE], gfciHlth[MAX_STRING_SIZE]; */
    long voltHlth, tempHlth, wFireHlth, adcHlth, preAmpHlth, dqHlth,
		pwrSupHlth, gfciHlth;
    char dummy[MAX_STRING_SIZE];

    getDbInfoT(dbTop, VOLTHLTH ".VAL", dummy, DBF_LONG, &voltHlth);
    cicsLogMessage(DBG_FULL,"got volthlth\n");

#if 0
    getDbInfoT(dbTop, TEMPHLTH ".VAL", dummy, DBF_LONG, &tempHlth);
    cicsLogMessage(DBG_FULL,"got temphlth\n");
#endif
    getDbInfoT(dbTop, ADCHLTH ".VAL", dummy, DBF_LONG, &adcHlth);
    cicsLogMessage(DBG_FULL,"got adchlth\n");
    getDbInfoT(dbTop, PREAMPHLTH ".VAL", dummy, DBF_LONG, &preAmpHlth);
    cicsLogMessage(DBG_FULL,"got preamphlth\n");
    getDbInfoT(dbTop, WFIREHLTH ".VAL", dummy, DBF_LONG, &wFireHlth);
    cicsLogMessage(DBG_FULL,"got wfirehlth\n");
    getDbInfoT(dbTop, DQHLTH ".VAL", dummy, DBF_LONG, &dqHlth);
    cicsLogMessage(DBG_FULL,"got dqhlth\n");
    getDbInfoT(dbTop, PWRSUPHLTH ".VAL", dummy, DBF_LONG, &pwrSupHlth);
    cicsLogMessage(DBG_FULL,"got pwrsuphlth\n");
    getDbInfoT(dbTop, GFCIHLTH ".VAL", dummy, DBF_LONG, &gfciHlth); 
    cicsLogMessage(DBG_FULL,"got vals\n");
    if ((voltHlth== STATUS_BAD)  ||
		(tempHlth== STATUS_BAD)  ||
		(adcHlth== STATUS_BAD)  ||
		(preAmpHlth== STATUS_BAD)  ||
		(wFireHlth== STATUS_BAD)  ||
		(dqHlth== STATUS_BAD)  ||
		(pwrSupHlth== STATUS_BAD)  ||
		(gfciHlth== STATUS_BAD) )
    {
		strcpy(str, HLTHBAD);
		return STATUS_BAD;
    }

    if ((voltHlth== STATUS_WARNING)  ||
		(tempHlth== STATUS_WARNING)  ||
		(adcHlth== STATUS_WARNING)  ||
		(preAmpHlth== STATUS_WARNING)  ||
		(wFireHlth== STATUS_WARNING)  ||
		(dqHlth== STATUS_WARNING)  ||
		(pwrSupHlth== STATUS_WARNING)  ||
		(gfciHlth== STATUS_WARNING) )
    {
		strcpy(str, HLTHWARN);
		return STATUS_WARNING;
    }

    strcpy(str, HLTHGOOD);
    return (STATUS_GOOD);
}


