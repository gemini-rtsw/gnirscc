
static struct {
	void *v;
	char *c;
} rcsid = {
	&rcsid,
	"$Id: epicsCAInt.c,v 1.1 2009/06/10 15:05:11 gemvx Exp $"
};

/* extern char sdsuErrorMessage[]; */ /* global error message string*/


/* all functions in this file use channel access to access EPICS records*/


#define NODBACCESS  
#include "epCommon.h"
#include "epicsNames.h"
#include "epicsCA.h"
#include "gnirsCC.h"


/*
 *	Routine:	getEpicsEnumCAEpics (char* name, dbr_int_t *val)
 * 	Author:		Peter Ruckle 
 *	Purpose:	Get an enumerated type from epics (eg. mbbi)
 *	Return:		status (OK, ERROR)
 *	Date:	        Oct 1996
 *			 		    
 *	 
 *	deficiencies:  This routine is not necessary, the getEpics or getEpicsCA routine
 *			should perform the same task using a short int as the return
 *			value type. 
 *				short val;		   
 *				getEpicsCA(name,count, &val)
 */
STATUS getEpicsEnumCA(char *name,dbr_int_t *val)
{
	STATUS ca_Status;
    chid chTmp;
    int chType;
    

    ca_Status = ca_search_and_connect(name, &chTmp, NULL, NULL);
    if (ca_Status != ECA_NORMAL)
    {
		fputs("get_Epics enum error:  search\n",stderr);	
		return ERROR;
    }
    ca_Status = ca_pend_io(1);
    if (ca_Status != ECA_NORMAL)
    {	
		fputs("get_Epics enum error:  pend 1\n",stderr);
		return ERROR;
    }
    chType = ca_field_type (chTmp);

    ca_Status = ca_array_get(chType, 1, chTmp, val);
    if (ca_Status != ECA_NORMAL)
    {
		fputs("get_Epics error:  get\n",stderr);
		return ERROR;
    }
    ca_Status =  ca_pend_io(1);
    if (ca_Status != ECA_NORMAL)
    {
		fputs("get_Epics enum error:  pend 2\n",stderr);
		return ERROR;
    }

	ca_flush_io(); 
 
    return OK;
    
}

/*
 *	Routine:	putEpicsEnumCA (char* name, dbr_int_t *val)
 * 	Author:		Peter Ruckle 
 *	Purpose:	put an enumerated type into an epics record
 *				(eg. mbbi)
 *	Return:		status (OK ERROR)
 *	Date:		       Oct 1996
 *			 
 *		    
 *	deficiencies:  This routine is not necessary, the putEpics or putEpicsCA routine
 *			should perform the same task using a short int as the return
 *			value type. 
 *				short val;		   
 *				putEpicsCA(name,count, &val)
 *			   
 */
/* status putEpicsEnum(char *name,dbr_int_t *val,void **tmp) */

STATUS putEpicsEnumCA(char *name, dbr_int_t *val)
{
	STATUS ca_Status = ECA_NORMAL;
    chid chTmp;
    int chType;

    ca_Status = ca_search_and_connect(name, &chTmp, NULL, NULL);
    if (ca_Status != ECA_NORMAL)
    {
		fputs ("put_Epics:  search and connect error",stderr);
		return ERROR;
    }

    ca_Status = ca_pend_io(1); 
    if (ca_Status != ECA_NORMAL)
	{
		fputs ("put_Epics:  search and connect pend error",stderr);
	    return ERROR;
	}
    chType = ca_field_type (chTmp);
    ca_Status = ca_array_put(DBR_INT, 1, chTmp, val);
    if (ca_Status != ECA_NORMAL)
    {
		fputs ("put_Epics:  put error ",stderr);
		fputs(name,stderr);
		return ERROR;
    }
    ca_Status = ca_pend_io(1);
    if (ca_Status != ECA_NORMAL)
    {
		fputs ("put_Epics:  put pend error",stderr);
		return ERROR;
    }

	ca_flush_io();  
   
    return OK;
}

/*
 *	Routine:	getEpicsCA (char* name, void *val)
 * 	Author:		Peter Ruckle 
 *	Purpose:	Get a value from an EPICS record (can be string
 *				long or double)
 *	Return:		status (OK ERROR)
 *	Date:		Oct 1999
 *	   
 *		    
 *			   
 *		    
 *			   
 */
STATUS getEpicsCA(char *name, int count,void *val) 
{ 
    STATUS status;
    void  *ch=NULL;
    int ret;
    ch = NULL;
    status = getEpics(name,val, count ,&ch);
    /*free channel resources*/
    if(status == OK)
    {
		ret = ca_clear_channel(ch);
		if(ret == ECA_NORMAL)
			ret = ca_pend_io(1);
		if(ret != ECA_NORMAL)
			status = ERROR;
    }
    return status;
}
/*
 *	Routine:	getEpics (char* name, void *val,int count ,void **tmp)
 * 	Author:		Peter Ruckle 
 *	Purpose:	Get a value from an EPICS record (can be string
 *				long or double)
 *	Invocation:	static void *ch; 	channel access id
 *			char name[80]; 		name of record to access
 *			[type] val;  		long double ...
 *			int count; 		number of elements if an array, otherwise 1
 *			putEpics( name,&val,count,&ch);

 *	Return:		status (OK ERROR)
 *	Date:	        Oct 1996
 *			Oct 1999 - Modified to allow id to be passed by caller
 *		    
 *			   
 *		    
 *			   
 */
STATUS getEpics (char* name, void *val,int count,void **tmp)
{
   
    STATUS ca_Status=ECA_NORMAL;
    chid *chTmp;
    int chType;
    char buf[140];
    
    chTmp = (chid *)tmp;
    if(*chTmp == NULL)
    {
	ca_Status = ca_search_and_connect(name, chTmp, NULL, NULL);
	if (ca_Status != ECA_NORMAL)
	{
	    sprintf(buf,"get_Epics error:  search %s\n",name);
	    fputs(buf,stderr);
	    if (ca_Status ==ECA_BADTYPE)
	    {
		fputs("ECA_BADTYPE\n",stderr);
	    }
	    else if (ca_Status ==ECA_STRTOBIG)
	    {
		fputs("ECA_STRTOBIG\n",stderr);
	    }
	    else if (ca_Status ==ECA_ALLOCMEM)
	    {
		fputs("ECA_ALLOCMEM\n",stderr);
	    }
	    else if (ca_Status ==ECA_GETFAIL)
	    {
		fputs("ECA_GETFAIL\n",stderr);
	    }
	    else
	    {
		sprintf (buf,"ca_Status = %d  %d  %d  %d  %d\n",
			 ca_Status,ECA_BADTYPE,ECA_STRTOBIG,ECA_ALLOCMEM,ECA_GETFAIL );

	    }
	    ca_clear_channel(*chTmp);
	    ca_pend_io(2);
	    
	    *chTmp = NULL;
	    return ERROR;
	}
	ca_Status = ca_pend_io(2);
	if (ca_Status != ECA_NORMAL)
	{	sprintf(buf,"get_Epics error:  pend 1 %s\n",name);
	    fputs(buf,stderr);
	    ca_clear_channel(*chTmp);
	    ca_pend_io(2);
	    *chTmp = NULL;
	    return ERROR;
	}
    }
    chType = ca_field_type (*chTmp);

    ca_Status = ca_array_get(chType, count, *chTmp, val);
    if (ca_Status != ECA_NORMAL)
    {
	sprintf(buf,"get_Epics error:  get %s\n",name);
	fputs(buf,stderr);

	ca_clear_channel(*chTmp);
	ca_pend_io(2);
	*chTmp = NULL;
	return ERROR;
    }
    ca_Status =  ca_pend_io(2);
    if (ca_Status != ECA_NORMAL)
    {
	sprintf(buf,"get_Epics error:  pend 2 %s\n",name);
	fputs(buf,stderr);

	ca_clear_channel(*chTmp);
	ca_pend_io(2);
	*chTmp = NULL;
	return ERROR;
    }
 
    ca_flush_io();  
 
    return OK;
    
}
/*
 *	Routine:	putEpicsCA (char* name, void *val)
 * 	Author:		Peter Ruckle 
 *	Purpose:	Put a value into an EPICS record (can be string
 *				long or double)
 *	Return:		status (OK ERROR)
 *	Date:	        Oct 1999
 *			 
 *		    
 *			   
 *		    
 *			   
 */

STATUS putEpicsCA(char *name, void *val) 
{
    STATUS status;
    long ret;
    void  *ch=NULL;


    status = putEpics(name,val,&ch);
    /*free channel resources*/
    if(status == OK)
    { 
	ret = ca_clear_channel(ch);
	if(ret == ECA_NORMAL)
	    ret = ca_pend_io(1);
	if(ret != ECA_NORMAL)
	    status = ERROR;
    } 
   
    return status;
}
/*
 *	Routine:	putEpics (char* name, void *val,void **chTmp)
 * 	Author:		Peter Ruckle 
 *	Purpose:	Put a value into an EPICS record (can be string
 *				long or double) use record pointer
 *				if it exists 
 *	Invocation:	static void *ch;  channel access id
 *			char name[80]; 		 name of record to access
 *			[type] val;  long double ...
 *			int count; number of elements if an array, otherwise 1
 *			getEpics( name,&val,count,&ch);	
 *	Return:		status (OK ERROR)	
 *	Date:	        Oct 1996
 *			Oct 1999 - Modified to allow id to be passed by caller
 *		    
 *			   
 *		    
 *			   
 */
STATUS putEpics (char* name, void *val,void **tmp)
{
    STATUS ca_Status = ECA_NORMAL;
    chid *chTmp;  
    int chType;
   

    chTmp = (chid *)tmp;
    if(*chTmp == NULL)
    {
	ca_Status = ca_search_and_connect(name, chTmp, NULL, NULL);
	if (ca_Status != ECA_NORMAL)
	{
	    fputs ("put_Epics:  search and connect error",stderr);
	    *chTmp = NULL;
	    return ERROR;
	}
	
	ca_Status = ca_pend_io(2); 
	if (ca_Status != ECA_NORMAL)
	{
	    fputs("search and connect pend error: ",stderr);
	    fputs (name,stderr);
	    ca_clear_channel(*chTmp);
	    ca_pend_io(2);
	  
	    *chTmp = NULL;
	    return ERROR;
	}
    }

    chType = ca_field_type (*chTmp);
    ca_Status = ca_array_put(chType, 1, *chTmp, val);
    if (ca_Status != ECA_NORMAL)
    {
	fputs ("put_Epics:  put error ",stderr);
	printf("status = %d ", ca_Status);
	fputs(name,stderr);
	ca_clear_channel(*chTmp);
	ca_pend_io(2);

	*chTmp = NULL;
	return ERROR;
    }

    ca_Status = ca_pend_io(2);
    if (ca_Status != ECA_NORMAL)
    {
	fputs ("put_Epics:  put pend error",stderr);

	ca_clear_channel(*chTmp);
	ca_pend_io(2);
	*chTmp = NULL;
	return ERROR;
    }

    /*   ca_flush_io();   */
  
   
    return OK;
}



/* THESE LAST FEW FUNCTIONS CAN BE USED IF THERE ARE MULTIPLE EPICS VARIABLES TO BE
   PLACED OR TAKEN FROM THE DATABASE*/
/*
 *	Routine:	getEpicsChanArrayCA (char **name, chid **chTmp, int n)
 * 	Author:		Peter Ruckle 
 *	Purpose:	connect to a list of EPICS records and return
 *				the connection pointers in chTmp
 *	Return:		status (OK ERROR)
 *	Date:	        Oct 1996
 *			 
 *		    
 *			   
 *		    
 *			   
 */
STATUS getEpicsChanArrayCA (char **name, chid **chTmp, int n)
{
    STATUS ca_Status;
    int i;
    for (i=0; i<n; i++)
	ca_search_and_connect(name[i], chTmp[i], NULL, NULL);
    ca_Status = ca_pend_io(1);
    if (ca_Status != ECA_NORMAL)
	return ERROR;
    return OK;
}

/*
 *	Routine:	getEpicsTypeArrayCA (chid *chTmp, int **chType, int n)
 * 	Author:		Peter Ruckle 
 *	Purpose:	get type of a list of epics records from their
 *				record pointers
 *	Return:		status (OK ERROR)
 *	Date:	        Oct 1996
 *			 
 *		    
 *			   
 *		    
 *			   
 */
STATUS getEpicsTypeArrayCA(chid *chTmp, int **chType, int n)
{
    STATUS ca_Status;
    int i;
    for (i=0; i<n; i++)
	*chType[i] = ca_field_type (chTmp[i]);
    ca_Status = ca_pend_io(1);
    if (ca_Status != ECA_NORMAL)
	return ERROR;
    return OK;
}

/*
 *	Routine:	getEpicsValArrayCA (int *chType, chid *chTmp, void **val, int n)
 * 	Author:		Peter Ruckle 
 *	Purpose:	Get the values from epics records using the chType from 
 *			'getEpicsTypeArrayCA' and the chTmp from
 * 			'getEpicsChanArrayCA'
 *	Return:		status (OK ERROR)
 *	Date:		Oct 1996
 *			 
 *		    
 *			   
 *		    
 *			   
 */
STATUS getEpicsValArrayCA(int *chType, chid *chTmp, void **val, int n)
{
    STATUS ca_Status;
    int i;
    for (i=0; i<n; i++)
	ca_array_get(chType[i],1,chTmp[i],val[i]);
    ca_Status = ca_pend_io(1);
    if (ca_Status != ECA_NORMAL)
	return ERROR;
    return OK;
}

/*
 *	Routine:	putEpicsValArrayCA (int *chType, chid *chTmp, void **val, int n)
 * 	Author:		Peter Ruckle 
 *	Purpose:	Putt values into epics records using the chType from 
 *			'getEpicsTypeArrayCA' and the chTmp from
 * 			'getEpicsChanArrayCA
 *	Return:		status (OK ERROR)
 *	Date:		Oct 1996
 *			 
 *		    
 *			   
 *		    
 *			   
 */
STATUS putEpicsValArrayCA(int *chType, chid *chTmp, void **val, int n)
{
    STATUS ca_Status;
    int i;
    for (i=0; i<n; i++)
	ca_array_put(chType[i], 1, chTmp[i], val[i] );
    ca_Status = ca_pend_io(1);
    return OK;
}
/*
 *	Routine:	clearEpicsChanArray ()
 * 	Author:		Peter Ruckle 
 *	Purpose:
 *	Return:		status (OK ERROR)
 *	Date:		Oct 1996
 *			 
 *		    
 *	Deficiencies:	this should do a ca_clear_channel on all the open chids
 *		    
 *			   
 */
STATUS clearEpicsChanArray ()
{
    return OK;
}
/*
 *	Routine:	updateHealth (char* health)
 * 	Author:		Peter Ruckle 
 *	Purpose:	update health variable in sad database
 *	Return:		status (OK ERROR)
 *	Date:		 Oct 1999
 *			 
 *		    
 *			   
 *		    
 *			   
 */
long updateHealth( char *health)
{
    long status = OK;
    char errMess[MAX_STRING_SIZE];
    char name[80];
    static void *ch=NULL;
    dbr_int_t h;

    sprintf(name,"%s%s",sadTop,HEALTH_INPUT);
 
    if (strcmp(health,"BAD")==0)
    {
	h = 2;
	/*        sdsuErrorMessage[39] = 0; */
	/*        cicsLogMessage(0,sdsuErrorMessage); */
    }
    else if(strcmp(health,"GOOD")==0)
	h = 0;
    else if(strcmp(health,"WARNING")==0)
    {
	/* 	sdsuErrorMessage[39] = 0; */
	/*        cicsLogMessage(0,sdsuErrorMessage); */
	h = 1;
    }
    else 
	return ERROR; 
  
    if((status = putEpics(name,  &h,&ch)) != OK)
    {
	printf("update health error\n");
	status = ERROR;
	cicsLogMessage(0, errMess);
    }
   
    return status;
}
/*
 *	Routine:	updateState (char* state)
 * 	Author:		Peter Ruckle 
 *	Purpose:	Update state variable in sad database
 *	Return:		status (OK ERROR)
 *	Date:		 Oct 1999
 *			 
 *		    
 *			   
 *		    
 *			   
 */
long updateState(char *state)
{
    char name[80];

    sprintf(name,"%s%s",sadTop,SAD_STATE);
    if(state != NULL) 
	if( putEpicsCA(name, state) != OK)
	{
	    printf("update state error\n");
	    cicsLogMessage(0, "updateState: Error setting state");
	    return ERROR;
	}

    return OK;
}
#if 0
static void *ch[NUM_TEMPS];
static char name[NUM_TEMPS][80];


int updateTemps(int a1,int a2,int a3,int a4,int a5,int a6,int a7,int a8,int a9,int a10)
{
    int i;
    /* 	static void *ch[NUM_TEMPS]; */
    STATUS stat;
    /* 	static char name[NUM_TEMPS][80]; */
    while (strcmp(tempNames[i],"END") != 0)
    {
	if(strlen(tempNames[i]) > 0)
	    sprintf(name[i],"%s%s",sadTop,tempNames[i]);
	else
	    name[i][0] = NULL;
    }
    while (1)
    {
	i = 0;
	while ( strcmp(tempNames[i],"END") != 0)
	{
	    if(name[i][0] != NULL)
	    {
		stat = putEpics(name[i],&temperatureCC[i],&ch[i]);
	    }
	    if(stat != OK)
		printf("error with %s %d\n",name[i],i);
	}
		
	sleep (10,0);
    }
    return OK;
}
#endif
