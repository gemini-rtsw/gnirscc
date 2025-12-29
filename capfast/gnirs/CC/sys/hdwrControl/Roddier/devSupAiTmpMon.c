/* VxWorks include files */
#include <vxWorks.h>
#include <remLib.h>
#include <ioLib.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* EPICS include files */
#include <dbDefs.h>
#include <dbAccess.h>
#include <recSup.h>
#include <devSup.h>
#include <aiRecord.h>

/* NIRS include files */
#include <nirsConst.h>
#include <nirsGlobals.h>
#include <wfdew.h>

/* Forward declarations of device support functions */
static long init();
static long init_ai(struct aiRecord *);
static long read_ai(struct aiRecord *);
extern double CnvT(double);

typedef struct
{
    short channel;
    float voltage;
} AITMPMON_INFO;

/* Create device support structure with names of functions
   to be executed by the record processing routines.
*/
struct
{
    long number;
    DEVSUPFUN report;
    DEVSUPFUN init;
    DEVSUPFUN init_record;
    DEVSUPFUN get_ioint_info;
    DEVSUPFUN read_ai;
    DEVSUPFUN special_linconv;
} devAiTmpMon = { 6, NULL, init, init_ai, NULL, read_ai, NULL};


/* Call function to initialize the module */
static long init()
{
    if(debugLevel == NIRS_DBG_FULL)
	wfdewdbgon();
    dewinit();
    wfdewdbgoff();
    return OK;
}


static long init_ai(struct aiRecord *pAi)
{
    struct link *pLink = &pAi->inp;
    AITMPMON_INFO *pInfo;

    /* Check that the link type is valid */
    if(pLink->type != INST_IO)
    {
	recGblRecordError(S_db_badField,(void *) pAi,
			  "devSupAiTmpMon illegal INP value");
	return S_db_badField;
    }

    /* Obtain the channel number and save the address in
       the DPVT field of the record.
    */
    pInfo = (AITMPMON_INFO *) malloc(sizeof (AITMPMON_INFO));
    if(sscanf(pLink->value.instio.string, "%hd", &(pInfo->channel)) != 1)
    {
	recGblRecordError(S_db_badField,(void *) pAi,
			  "devSupAiTmpMon bad INP value");
	return S_db_badField;	
    }
    pAi->dpvt = (char *) pInfo;

    /* Check units: valid ones are volts, degC and degK */
    if((strcmp(pAi->egu,"volts") != 0) && (strcmp(pAi->egu,"degC") != 0)
       && (strcmp(pAi->egu,"degK") != 0))
    {
	recGblRecordError(S_db_badField,(void *) pAi,
			  "devSupAiTmpMon bad EGU value");
	return S_db_badField;	
    }

    return OK;
}

/* Read and convert the analog input value */
static long read_ai(struct aiRecord *pAi)
{
    AITMPMON_INFO *pInfo;

    if(debugLevel == NIRS_DBG_FULL)
	wfdewdbgon();
    else
	wfdewdbgoff();

    /* Obtain dewar voltage for the specified channel */
    pInfo = (AITMPMON_INFO *) pAi->dpvt;
    pInfo->voltage = rddewvolt(pInfo->channel);

    /* Convert to degrees Kelvin if EGU = "degK" */
    if(strcmp(pAi->egu,"degK") == 0)
	pAi->val = CnvT(pInfo->voltage);

    /* Convert to degrees Celsius if EGU = "degC" */
    else if(strcmp(pAi->egu,"degC") == 0)
	pAi->val = CnvT(pInfo->voltage) - 273;

    /* Leave as a voltage otherwise */
    else
	pAi->val = pInfo->voltage;

    return(2);  /* 2 = success, value converted to EGU */
}



