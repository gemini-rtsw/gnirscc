static struct {
        void *v;
        char *c;
} rcsid = {
        &rcsid,
        "$Id: devAoWFireMessage.c,v 1.2 2009/05/27 19:33:33 fkraemer Exp $"
};
int wfiredebug = 0;
/* #define DEBUG */
#include <debug.h>
char buf[256];
int hkDelay = 3;
/*devAoWFireMessage.c */

/* Initial attempt, based on $EPICS/base/src/dev/dev*Ao*.c. */


#include	<string.h>

#include	<alarm.h>
#include	<dbDefs.h>
#include	<dbAccess.h>
#include        <recSup.h>
#include	<devSup.h>
#include	<module_types.h>
#include	<aoRecord.h>
#include	<aiRecord.h>
#include	<longoutRecord.h>
#include	<longinRecord.h>
#include	<stringinRecord.h>
#include	<stringoutRecord.h>

#include	<wFireMsgDefs.h>
#include <cicsLib.h>
#include	".applTop/tpSource/include/protocol.h"
struct WFMsgTaskParameters wfParams= { FALSE };
extern int intDisable;

#if	defined(STUBS)
#define	STATIC
#else
#define	STATIC	static
/* Create the dset for devWFireCmdMessage */

/* this is a new longOut device support type to handle certain wfire command 
 * messages - ncb - 21Aug97
 */

STATIC long init_WFireCmdMsg();
STATIC long write_WFireCmdMsg();
struct {
	long		number;
	DEVSUPFUN	report;
	DEVSUPFUN	init;
	DEVSUPFUN	init_record;
	DEVSUPFUN	get_ioint_info;
	/* The elements above this line must conform to dset and the elements
	 * below must conform to aodset.
	 */
	DEVSUPFUN	write_lo;

}devWFireCmdMsg=
 {
	6,
	NULL,
	NULL,
	init_WFireCmdMsg,
	NULL,
	write_WFireCmdMsg,
 };

/* Create the dset for devAoWFireMessage */

/* Note that this device set is implicitly required to conform to the
 * dset structure as defined in $EPICS/base/include/devSup.h and also to the
 * aodset structure as defined in $EPICS/base/src/rec/recAo.c.
 */
STATIC long init_aoWFireVarMsg();
STATIC long write_aoWFireVarMsg();
struct {
	long		number;
	DEVSUPFUN	report;
	DEVSUPFUN	init;
	DEVSUPFUN	init_record;
	DEVSUPFUN	get_ioint_info;
	/* The elements above this line must conform to dset and the elements
	 * below must conform to aodset.
	 */
	DEVSUPFUN	write_ao;
	DEVSUPFUN	special_linconv;
}devAoWFireVarMsg={
	6,
	NULL,
	NULL,
	init_aoWFireVarMsg,
	NULL,
	write_aoWFireVarMsg,
	NULL};

/* Create the dset for devAiWFireVarMsg */

/* Note that this device set is implicitly required to conform to the
 * dset structure as defined in $EPICS/base/include/devSup.h and also to the
 * aidset structure as defined in $EPICS/base/src/rec/recAi.c.
 */
STATIC long init_aiWFireVarMsg();
STATIC long read_aiWFireVarMsg();

struct {
	long		number;
	DEVSUPFUN	report;
	DEVSUPFUN	init;
	DEVSUPFUN	init_record;
	DEVSUPFUN	get_ioint_info;
	/* The elements above this line must conform to dset and the elements
	 * below must conform to aidset.
	 */
	DEVSUPFUN	read_ai;/*(0,2)=> success and convert,don't convert)*/
			/* if convert then raw value stored in rval */
	DEVSUPFUN	special_linconv;
}devAiWFireVarMsg={
	6,
	NULL,
	NULL,
	init_aiWFireVarMsg,
	NULL,
	read_aiWFireVarMsg,
	NULL};

/* Create the dset for devLoWFireVarMsg */

/* Note that this device set is implicitly required to conform to the
 * dset structure as defined in $EPICS/base/include/devSup.h and also to the
 * longoutdset structure as defined in $EPICS/base/src/rec/recLongout.c.
 */
STATIC long init_loWFireVarMsg();
STATIC long write_loWFireVarMsg();

struct {
	long		number;
	DEVSUPFUN	report;
	DEVSUPFUN	init;
	DEVSUPFUN	init_record;
	DEVSUPFUN	get_ioint_info;
	/* The elements above this line must conform to dset and the elements
	 * below must conform to longoutdset.
	 */
	DEVSUPFUN	write_longout;/*(-1,0)=>(failure,success*/
}devLoWFireVarMsg={
	5,
	NULL,
	NULL,
	init_loWFireVarMsg,
	NULL,
	write_loWFireVarMsg,
	};

/* Create the dset for devLiWFireVarMsg */

/* Note that this device set is implicitly required to conform to the
 * dset structure as defined in $EPICS/base/include/devSup.h and also to the
 * longindset structure as defined in $EPICS/base/src/rec/recLongin.c.
 */
STATIC long init_liWFireVarMsg();
STATIC long read_liWFireVarMsg();

struct {
	long		number;
	DEVSUPFUN	report;
	DEVSUPFUN	init;
	DEVSUPFUN	init_record;
	DEVSUPFUN	get_ioint_info;
	/* The elements above this line must conform to dset and the elements
	 * below must conform to longindset.
	 */
	DEVSUPFUN	read_longin; /*returns: (-1,0)=>(failure,success)*/
}devLiWFireVarMsg={
	5,
	NULL,
	NULL,
	init_liWFireVarMsg,
	NULL,
	read_liWFireVarMsg,
	};

/* Create the dset for devSoWFireVarMsg */

/* Note that this device set is implicitly required to conform to the
 * dset structure as defined in $EPICS/base/include/devSup.h and also to the
 * stringoutdset structure as defined in $EPICS/base/src/rec/recStringout.c.
 */
STATIC long init_soWFireVarMsg();
STATIC long write_soWFireVarMsg();

struct {
	long		number;
	DEVSUPFUN	report;
	DEVSUPFUN	init;
	DEVSUPFUN	init_record;
	DEVSUPFUN	get_ioint_info;
	/* The elements above this line must conform to dset and the elements
	 * below must conform to stringoutdset.
	 */
	DEVSUPFUN	write_stringout;/*(-1,0)=>(failure,success)*/
}devSoWFireVarMsg={
	5,
	NULL,
	NULL,
	init_soWFireVarMsg,
	NULL,
	write_soWFireVarMsg,
	};

/* Create the dset for devSiWFireVarMsg */

/* Note that this device set is implicitly required to conform to the
 * dset structure as defined in $EPICS/base/include/devSup.h and also to the
 * stringindset structure as defined in $EPICS/base/src/rec/recStringin.c.
 */
STATIC long init_siWFireVarMsg();
STATIC long read_siWFireVarMsg();

struct {
	long		number;
	DEVSUPFUN	report;
	DEVSUPFUN	init;
	DEVSUPFUN	init_record;
	DEVSUPFUN	get_ioint_info;
	/* The elements above this line must conform to dset and the elements
	 * below must conform to longindset.
	 */
	DEVSUPFUN	read_stringin; /*returns: (-1,0)=>(failure,success)*/
}devSiWFireVarMsg={
	5,
	NULL,
	NULL,
	init_siWFireVarMsg,
	NULL,
	read_siWFireVarMsg,
	};

#endif

/* The outgoing WFireVarMsg queue.
 * used for outgoing ai, li, lo, si, so and command messages also.
 */
MSG_Q_ID	WFMsg_Q_ID_out = NULL ;
/* For incoming WFireVarMsg messages.
 * this queue is used for all incoming wfire messages 
 */
MSG_Q_ID	WFMsg_Q_ID_in = NULL ;
int createQueues()
{


   /* Create the outgoing wfire message queue if it hasen't been done
     * This queue is also used for other wfire messages so it may have
     * been created elsewhere.
     */
   if( WFMsg_Q_ID_out == NULL )
    {
	WFMsg_Q_ID_out = msgQCreate( MAXWFMsgMsgs,
				     sizeof(struct WFMsgMsg),
				     MSG_Q_FIFO ) ; 
	/* Check for error.  I don't think this is necessary on the grounds
	 * that "it's not going to happen". I agree with Jan if this fails we
	 * are really screwed with a malfunctioning vxWorks implementation
	 */
	if( WFMsg_Q_ID_out == (MSG_Q_ID)NULL )
	{
	   
	    return( errno );
	}
    }
  /* Create a message queue for incoming aiWFireVarMsg's. */
    if( WFMsg_Q_ID_in == NULL ) {
	WFMsg_Q_ID_in = msgQCreate( MAXWFMsgMsgs,
	    sizeof( struct WFMsgMsg ), MSG_Q_FIFO ) ; 
	/* Check for error.  I don't think this is necessary on the grounds
	 * that "it's not going to happen".
	 */
	if( WFMsg_Q_ID_in == (MSG_Q_ID)NULL ) {
	 
	    return( errno );
	}
    }
    return OK;
}




/*
 *+
 * FUNCTION NAME:
 * init_WFireCmdMsg
 *
 * INVOCATION:
 * status = init_WFireCmdMsg(plo);
 * 
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * > longoutRecord *plo  
 *
 * FUNCTION VALUE:
 *   status - long
 *
 * PURPOSE:
 * initializes the wfire Message queques for wFirecmdMsg messages
 *
 * DESCRIPTION:
 * 
 *
 * EXTERNAL VARIABLES:
 * None
 *
 * PRIOR REQUIREMENTS:
 * none?
 *
 * DEFICIENCIES:
 * None
 *
 * HISTORY:
 * June-1997  Original version adapted from CICS alpha 1.0  Jan Schwitters
 * Aug-21-1997 - added support for returning messages from transputers
 *-
 */
/* Sequence numbers for things going into the WFMsg_Q_ID_out. */
static unsigned long sequence = 1 ;

STATIC long init_WFireCmdMsg(plo)
struct longoutRecord *plo;
{
  struct WFMsgMsg msg;
    struct WFMsgInfo	*pWFMsgInfo ;

    if( plo->out.type != INST_IO ) {
        recGblRecordError(S_db_badField,(void *)plo,
            "devWFireVarMsg (init_record) Illegal out.type");
        return(S_db_badField);
    }
#if 0
    if (createQueues()!= OK)
       recGblRecordError(errno,(void *)plo,
			      "devWFireCmdMsg: WFMsg_Q_ID: msgQCreate() failed");
#endif
 
    
    /* Fill in and save the WFMsgInfo structure. */
    pWFMsgInfo = (struct WFMsgInfo *)
	malloc( sizeof( struct WFMsgInfo ) ) ;
    bzero( (char *)pWFMsgInfo, sizeof( struct WFMsgInfo ) ) ;
    /* Save the associated record structure just in case someone wants it
     * later.
     */
    pWFMsgInfo->precord = (void *)plo ;
    /* Identify the type of record which caused the creation of this
     * structure.
     */
    pWFMsgInfo->recId = WFCmdMsg ; 
    

    /* Save the command string. */
    (void)strncpy( pWFMsgInfo->string, plo->out.value.instio.string,
	INSTIO_FLD_SZ );
    if( sscanf( plo->out.value.instio.string, WFCmdFormat,
	&pWFMsgInfo->node,
	&pWFMsgInfo->var,
	&pWFMsgInfo->grp,
	&pWFMsgInfo->idx) != 4 ) {
	    recGblRecordError(S_db_badField,(void *)plo,
		"devloWFireVarMsg (init_record) Illegal out.value");
	    return(S_db_badField);
    }
    plo->dpvt = (char *)pWFMsgInfo ;
 msg.info = *(struct WFMsgInfo *)plo->dpvt ;
    /* Don't see anything else that needs to be done here. */
    return( OK ) ;
}
/*
 *+
 * FUNCTION NAME:
 * write_WFireCmdMsg
 *
 * INVOCATION:
 * 
 * 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 *  
 *
 * FUNCTION VALUE:
 *   none
 *
 * PURPOSE:
 *   Write a message from EPICS to transputers from EPICS record
 *        with DTYP = wFireCmdMsg
 *
 * DESCRIPTION:
 * 
 *
 * EXTERNAL VARIABLES:
 * None
 *
 * PRIOR REQUIREMENTS:
 *    init_WFireCmdMsg must be run before this
 *
 * DEFICIENCIES:
 * None
 *
 * HISTORY:
 * June-1997  Original version adapted from CICS alpha 1.0  Jan Schwitters
 *
 *-
 */
STATIC long write_WFireCmdMsg(plo)
    struct longoutRecord	*plo;
{
  int cont;
  
  struct WFMsgInfo	*pWFMsgInfo ;
    struct WFMsgMsg	msg;
    struct WFMsgMsg  incomingMsg;
	static long numReadHK = 0;
 

    
    /* Fill in the message and send it off to the queue. */
    msg.info = *(struct WFMsgInfo *)plo->dpvt ;
    msg.sequence = sequence++ ;
    msg.val.lval = plo->val ;
    pWFMsgInfo = plo->dpvt;
  
    /* get the receive Message Queue semaphore here */

	cont = 1;
	
 	if(msg.info.var == READ_HK) 
 	  { 
 	    numReadHK++;			 
 	    if((numReadHK %= hkDelay) != 2) 
 		cont = 0;  
 	  } 
	
	if (cont)
	  {    
	    sprintf (buf, "\n\n***********write_WFireCmdMsg(): %s\n",plo->name ) ; 
	    cicsLogMessage(3,buf);
	    
	    if (semTake( wfParams.EpicsInQSem, WFSemTakeTimeout) == OK)
	      {
		if( msgQSend( WFMsg_Q_ID_out, 
			      (char *)&msg, 
			      sizeof( msg ),
			      NO_WAIT, 
			      MSG_PRI_NORMAL ) != OK )
		  {	
		    
		    (void)semGive( wfParams.EpicsInQSem);
		    recGblRecordError( errno,(void *)plo,
				       "devWFireCmdMsg (write_record) msgQSend() failed");
		    return(	 errno );
		  }
		
		/* Wait for the reply or timeout. */
		if( msgQReceive(WFMsg_Q_ID_in, 
				(char *)&incomingMsg,
				sizeof(incomingMsg),  
				WFMsgRetTimeout) != ERROR ) 
		  {		
				/* Give the receive Message Queue semaphore here */
		    (void)semGive( wfParams.EpicsInQSem);
				
		  }
		else
		  {
				/* Give the receive Message Queue semaphore here */
		    (void)semGive( wfParams.EpicsInQSem);
		    cicsLogMessage(0,  "###########\n write_WFireCmdMsg msgQReceive TIMEOUT\n##########");
		    recGblRecordError( errno,(void *)plo,
				       "devWFireCmdMsg: msgQReceive() failed");
		
		    return( errno );
		  }
		strncpy(msg.info.string,incomingMsg.info.string,INSTIO_FLD_SZ);
		msg.info.string[INSTIO_FLD_SZ] = 0;
		/* check if transputer had an error*/
		if (strstr(msg.info.string,"ERROR"))
		  {
		    
		    return ERROR;
		  }
		/* Don't see anything else that needs to be done here. */
	      }
	    else	
	      {
		cicsLogMessage(0,"Couldn't take semaphore\n");
		return ERROR;
	      }
	  }	
    return( OK ) ;
}





/*
 *+
 * FUNCTION NAME:
 * init_aoWFireVarMsg
 *
 * INVOCATION:
 * 
 * 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 *  
 *
 * FUNCTION VALUE:
 *   none
 *
 * PURPOSE:
 *   initializes the wfire Message queques for wFireVarMsg messages and 
 *       sets up parameters for ao records
 *
 *
 * DESCRIPTION:
 * 
 *
 * EXTERNAL VARIABLES:
 * None
 *
 * PRIOR REQUIREMENTS:
 * none?
 *
 * DEFICIENCIES:
 * None
 *
 * HISTORY:
 * June-1997  Original version adapted from CICS alpha 1.0  Jan Schwitters
 *
 *-
 */

STATIC long init_aoWFireVarMsg(pao)
struct aoRecord *pao;
{
     struct WFMsgInfo	*pWFMsgInfo ;

     
/* 	cicsLogMessage(3, "***********init_aoWFireVarMsg():\n" ) ; */
    
    


    if( pao->out.type != INST_IO ) {
        recGblRecordError(S_db_badField,(void *)pao,
            "devaoWFireVarMsg (init_record) Illegal out.type");

        return(S_db_badField);
    }
#if 0
     if (createQueues()!= OK)
       recGblRecordError(errno,(void *)pao,
			      "devWFireVarMsg: WFMsg_Q_ID: msgQCreate() failed");
#endif
    /* Fill in and save the WFMsgInfo structure. */
    pWFMsgInfo = (struct WFMsgInfo *)
	malloc( sizeof( struct WFMsgInfo ) ) ;
    bzero( (char *)pWFMsgInfo, sizeof( struct WFMsgInfo ) ) ;
    /* Save the associated record structure just in case someone wants it
     * later.
     */
    pWFMsgInfo->precord = (void *)pao ;
    /* Identify the type of record which caused the creation of this
     * structure.
     */
    pWFMsgInfo->recId = WFMsgAo ;
    /* Save the command string. */
    (void)strncpy( pWFMsgInfo->string, pao->out.value.instio.string,
	INSTIO_FLD_SZ );
  
    if( sscanf( pao->out.value.instio.string, WFVarFormat,
	&pWFMsgInfo->node,
	&pWFMsgInfo->var,
	&pWFMsgInfo->grp,
	&pWFMsgInfo->idx) != 4 ) {
	    recGblRecordError(S_db_badField,(void *)pao,
		"devaoWFireVarMsg (init_record) Illegal out.value");
	  

	    return(S_db_badField);
    }
    pao->dpvt = (char *)pWFMsgInfo ;
 

      
    return( OK ) ;
}
/*
 *+
 * FUNCTION NAME:
 * write_aoWFireVarMsg
 *
 * INVOCATION:
 * 
 * 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 *  
 *
 * FUNCTION VALUE:
 *   none
 *
 * PURPOSE:
 *    Write a value from EPICS ao record (from EPICS record
 *        with DTYP = wFireVarMsg) to transputers
 *
 * DESCRIPTION:
 * 
 *
 * EXTERNAL VARIABLES:
 * None
 *
 * PRIOR REQUIREMENTS:
 * none?
 *
 * DEFICIENCIES:
 * None
 *
 * HISTORY:
 * June-1997  Original version adapted from CICS alpha 1.0  Jan Schwitters
 *
 *-
 */
STATIC long write_aoWFireVarMsg(pao)
    struct aoRecord	*pao;
{
    struct WFMsgMsg	msg ;
    struct WFMsgMsg  incomingMsg;
  /*   cicsLogMessage(3, "***********write_aoWFireVarMsg():\n" ) ; */
  
   
    /* Fill in the message and send it off to the queue. */
    msg.info = *(struct WFMsgInfo *)pao->dpvt ;
    msg.sequence = sequence++ ;
    msg.val.dval = pao->val ;

   
    /* get the receive Message Queue semaphore here */
    if (semTake( wfParams.EpicsInQSem, WFSemTakeTimeout) == OK)
    {
	if( msgQSend( WFMsg_Q_ID_out, 
		      (char *)&msg, 
		      sizeof( msg ),
		      NO_WAIT, 
		      MSG_PRI_NORMAL ) != OK )
	{
	    /* Give the receive Message Queue semaphore here */
	    (void)semGive( wfParams.EpicsInQSem);
	    recGblRecordError( errno,(void *)pao,
			       "devaoWFireVarMsg (write_ao) msgQSend() failed");
	    return( errno );
	}
/*  cicsLogMessage(3, "***********write_aoWFireVarMsg(): waiting for reply\n" ) ; */
	
	/* Wait for the reply or timeout. */
	if( msgQReceive(WFMsg_Q_ID_in, 
			(char *)&incomingMsg,
			sizeof(incomingMsg),  
			WFMsgRetTimeout) != ERROR ) 
	{	
	    /* Give the receive Message Queue semaphore here */
	    (void)semGive( wfParams.EpicsInQSem);

	    /* We got the a message. */
	    /* 	pao->val = incomingMsg.val.dval ; */
	   sprintf (buf," %s\n",incomingMsg.info.string);  
	   cicsLogMessage(3,buf);
	   
	}
	else
	{
	    /* Give the receive Message Queue semaphore here */
	    (void)semGive( wfParams.EpicsInQSem);
	    cicsLogMessage(0,"###########\n write_aoWFireVarMsg TIMEOUT\n##########");
	    recGblRecordError( errno,(void *)pao,
			   "devaoWFireVarMsg: msgQReceive() failed");

	    return( errno );
	}

	strncpy(msg.info.string,incomingMsg.info.string,INSTIO_FLD_SZ);
	msg.info.string[INSTIO_FLD_SZ] = 0;
	/* check if transputer had an error*/
	if (strstr(msg.info.string,"ERROR"))
	{  	
	  sprintf (buf," %s\n",incomingMsg.info.string);  
	  cicsLogMessage(3,buf);
	    return ERROR;
	}
    }
    else	
    {
	cicsLogMessage(0,"Couldn't take semaphore\n");
	return ERROR;
    }

   /*  cicsLogMessage(3, "***********write_aoWFireVarMsg(): done\n" ) ; */
    return( OK ) ;
}
/*
 *+
 * FUNCTION NAME:
 * 
 *
 * INVOCATION:
 * 
 * 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 *  
 *
 * FUNCTION VALUE:
 *   none
 *
 * PURPOSE:
 *   initializes the wfire Message queques for wFireVarMsg messages and 
 *       sets up parameters for ai records
 *
 * DESCRIPTION:
 * 
 *
 * EXTERNAL VARIABLES:
 * None
 *
 * PRIOR REQUIREMENTS:
 * none?
 *
 * DEFICIENCIES:
 * None
 *
 * HISTORY:
 * June-1997  Original version adapted from CICS alpha 1.0  Jan Schwitters
 *
 *-
 */
/* Initial attempt, based on $EPICS/base/src/dev/dev*Ai*.c. */

STATIC long init_aiWFireVarMsg(pai)
struct aiRecord *pai;
{
    struct WFMsgInfo	*pWFMsgInfo ;
    int ii;
 
/* 	cicsLogMessage(3, "***********init_aiWFireVarMsg():\n" ) ; */
    
  

    if( pai->inp.type != INST_IO ) {
        recGblRecordError(S_db_badField,(void *)pai,
            "devaiWFireVarMsg (init_record) Illegal inp.type");
        return(S_db_badField);
    }
#if 0
  if (createQueues()!= OK) 
        recGblRecordError(errno,(void *)pai, 
 			      "devWFireVarMsg: WFMsg_Q_ID: msgQCreate() failed"); 
   
#endif
    /* Fill in and save the WFMsgInfo structure. */
    pWFMsgInfo = (struct WFMsgInfo *)
	malloc( sizeof( struct WFMsgInfo ) ) ;
    bzero( (char *)pWFMsgInfo, sizeof( struct WFMsgInfo ) ) ;
    /* Save the associated record structure just in case someone wants it
     * later.
     */
    pWFMsgInfo->precord = (void *)pai ;
    /* Identify the type of record which caused the creation of this
     * structure.
     */
    pWFMsgInfo->recId = WFMsgAi ;
    /* Save the command string. */
    (void)strncpy( pWFMsgInfo->string, pai->inp.value.instio.string,
	INSTIO_FLD_SZ );

 

    if( ii = (sscanf( pai->inp.value.instio.string, WFVarFormat,
		      &pWFMsgInfo->node, &pWFMsgInfo->var,
		      &pWFMsgInfo->grp, &pWFMsgInfo->idx)) != 4 )
    {
	recGblRecordError(S_db_badField,(void *)pai,
			  "devaiWFireVarMsg (init_record) Illegal out.value");
	sprintf (buf,"fields %d, %d, %d, %d\n",ii,pWFMsgInfo->node,
		pWFMsgInfo->var, pWFMsgInfo->grp);
	cicsLogMessage(3,buf);
	
	return(S_db_badField);
    }

    pai->dpvt = (char *)pWFMsgInfo ;

    /* Don't see anything else that needs to be done here. */

    return( OK ) ;

}
/*
 *+
 * FUNCTION NAME:
 * read_aiWFireVarmMsg
 *
 * INVOCATION:
 * 
 * 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 *  
 *
 * FUNCTION VALUE:
 *   none
 *
 * PURPOSE:
 *    Read a value from transputers and return to EPICS ai record 
 *         (from EPICS record with DTYP = wFifeVarMsg)
 *
 * DESCRIPTION:
 * 
 *
 * EXTERNAL VARIABLES:
 * None
 *
 * PRIOR REQUIREMENTS:
 * none?
 *
 * DEFICIENCIES:
 * None
 *
 * HISTORY:
 * June-1997  Original version adapted from CICS alpha 1.0  Jan Schwitters
 *
 *-
 */
STATIC long read_aiWFireVarMsg(pai)
    struct aiRecord	*pai;
{
    double d;
    struct WFMsgMsg	msg ;
    struct WFMsgMsg	incomingMsg ;
    int	i ;
/* 	cicsLogMessage(3, "***********read_aiWFireVarMsg():\n" ) ; */
    
    /* The WFMsg_Q_ID_in queue should probably be empty.
     * I don't know what to do if not.
     */
    if( ( i = msgQNumMsgs( WFMsg_Q_ID_in ) ) != 0 ) {
	cicsLogMessage(3, "devaiWFireVarMsg (read_record)" ) ;
	sprintf(buf, " msgQNumMsgs( %s ) = %d.\n", "WFMsg_Q_ID_in", i ) ;
	cicsLogMessage(3,buf);
	
    }

    /* Fill in the outgoing message and send it off to the queue. */
    msg.info = *(struct WFMsgInfo *)pai->dpvt ;
    msg.sequence = sequence++ ;
    msg.val.dval = pai->val ;
    /* get the receive Message Queue semaphore here */
    if(semTake( wfParams.EpicsInQSem, WFSemTakeTimeout) == OK)
    {
	if( msgQSend( WFMsg_Q_ID_out, 
		      (char *)&msg, 
		      sizeof( msg ),
		      NO_WAIT, 
		      MSG_PRI_NORMAL ) != OK ) 
	{
	    /* Give the receive Message Queue semaphore here */
	    (void)semGive( wfParams.EpicsInQSem); 
	    recGblRecordError( errno,(void *)pai,
			   "devaiWFireVarMsg (read_record) msgQSend() failed");
	    return( errno );
	}

	/* Wait for the reply or timeout. */
	if( msgQReceive( WFMsg_Q_ID_in, 
		     (char *)&incomingMsg,
		     sizeof( incomingMsg ), 
		     WFMsgRetTimeout) != ERROR ) 
	{
      
	    /* Give the receive Message Queue semaphore here */
	    (void)semGive( wfParams.EpicsInQSem);

	    /* We got a message. */
	    pai->val = incomingMsg.val.dval ;
	    d = incomingMsg.val.dval;
	    strncpy(msg.info.string,incomingMsg.info.string,INSTIO_FLD_SZ);
	    msg.info.string[INSTIO_FLD_SZ] = 0;
	    /* check if transputer had an error*/
	    if (strstr(msg.info.string,"ERROR"))
	    {
		return ERROR;
	    }
	    /* Don't see anything else that needs to be done here. */
	return( 2) ;
	    
	} 
	else 
	{
	    /* Give the receive Message Queue semaphore here */
	    (void)semGive( wfParams.EpicsInQSem);
	    cicsLogMessage(0,"###########\n read_aiWFireVarMsg TIMEOUT\n##########");
	    recGblRecordError( errno,(void *)pai,
			   "devaiWFireVarMsg (read_record) msgQReceive() failed");
	    return( errno );
	}
    }
    else 	
    {
	cicsLogMessage(0,"Couldn't take semaphore\n");
	return ERROR;
    }
    return OK;

}


/*
 *+
 * FUNCTION NAME:
 * init_loWFireVarMsg
 *
 * INVOCATION:
 * 
 * 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 *  
 *
 * FUNCTION VALUE:
 *   none
 *
 * PURPOSE:
 *   initializes the wfire Message queques for wFireVarMsg messages and 
 *       sets up parameters for lo records
 *
 *
 * DESCRIPTION:
 * 
 *
 * EXTERNAL VARIABLES:
 * None
 *
 * PRIOR REQUIREMENTS:
 * none?
 *
 * DEFICIENCIES:
 * None
 *
 * HISTORY:
 * June-1997  Original version adapted from CICS alpha 1.0  Jan Schwitters
 *
 *-
 */
STATIC long init_loWFireVarMsg(plo)
struct longoutRecord *plo;
{
    struct WFMsgInfo	*pWFMsgInfo ;

/* 	cicsLogMessage(3, "***********init_loWFireVarMsg():\n" ) ; */
    
    if( plo->out.type != INST_IO ) {
        recGblRecordError(S_db_badField,(void *)plo,
            "devloWFireVarMsg (init_record) Illegal out.type");
        return(S_db_badField);
    }
#if 0
  if (createQueues()!= OK)
       recGblRecordError(errno,(void *)plo,

			      "devWFireVarMsg: WFMsg_Q_ID: msgQCreate() failed");
#endif
    /* Fill in and save the WFMsgInfo structure. */
    pWFMsgInfo = (struct WFMsgInfo *)
	malloc( sizeof( struct WFMsgInfo ) ) ;
    bzero( (char *)pWFMsgInfo, sizeof( struct WFMsgInfo ) ) ;
    /* Save the associated record structure just in case someone wants it
     * later.
     */
    pWFMsgInfo->precord = (void *)plo ;
    /* Identify the type of record which caused the creation of this
     * structure.
     */
    pWFMsgInfo->recId = WFMsgLo ;
    /* Save the command string. */
    (void)strncpy( pWFMsgInfo->string, plo->out.value.instio.string,
	INSTIO_FLD_SZ );
    if( sscanf( plo->out.value.instio.string, WFVarFormat,
	&pWFMsgInfo->node,
	&pWFMsgInfo->var,
	&pWFMsgInfo->grp,
	&pWFMsgInfo->idx) != 4 ) {
	    recGblRecordError(S_db_badField,(void *)plo,
		"devloWFireVarMsg (init_record) Illegal out.value");
	    return(S_db_badField);
    }
    plo->dpvt = (char *)pWFMsgInfo ;

    /* Don't see anything else that needs to be done here. */
    return( OK ) ;
}
/*
 *+
 * FUNCTION NAME:
 * write_loWFireVarMsg
 *
 * INVOCATION:
 * 
 * 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 *  
 *
 * FUNCTION VALUE:
 *   none
 *
 * PURPOSE:
 *    Write a value from EPICS lo record (from EPICS record
 *        with DTYP = wFireVarMsg) to transputers
 *
 * DESCRIPTION:
 * 
 *
 * EXTERNAL VARIABLES:
 * None
 *
 * PRIOR REQUIREMENTS:
 * none?
 *
 * DEFICIENCIES:
 * None
 *
 * HISTORY:
 * June-1997  Original version adapted from CICS alpha 1.0  Jan Schwitters
 *
 *-
 */
STATIC long write_loWFireVarMsg(plo)
    struct longoutRecord	*plo;
{
    struct WFMsgMsg	msg;
    struct WFMsgMsg  incomingMsg;

     cicsLogMessage(3, "\n\n***********write_loWFireVarMsg():\n" ) ; 
    
    /* Fill in the message and send it off to the queue. */
    msg.info = *(struct WFMsgInfo *)plo->dpvt ;
    msg.sequence = sequence++ ;
    msg.val.lval = plo->val ;
   
    /* get the receive Message Queue semaphore here */
    if (semTake( wfParams.EpicsInQSem, WFSemTakeTimeout) == OK)
    {
	if( msgQSend( WFMsg_Q_ID_out, 
		  (char *)&msg, 
		  sizeof( msg ),
		  NO_WAIT, 
		  MSG_PRI_NORMAL ) != OK )
	{
	    /* Give the receive Message Queue semaphore here */
	    (void)semGive( wfParams.EpicsInQSem); 
	    recGblRecordError( errno,(void *)plo,
			   "devloWFireVarMsg (write_record) msgQSend() failed");
	    return( errno );
	}
	
	/* Wait for the reply or timeout. */
	if( msgQReceive(WFMsg_Q_ID_in, 
		    (char *)&incomingMsg,
		    sizeof(incomingMsg),  
		    WFMsgRetTimeout*2) != ERROR ) 
	{	
	    /* Give the receive Message Queue semaphore here */
	    (void)semGive( wfParams.EpicsInQSem);
	    sprintf (buf," %s\n",incomingMsg.info.string);  
	   cicsLogMessage(3,buf);
	}
	else
	{
	    /* Give the receive Message Queue semaphore here */
	    (void)semGive( wfParams.EpicsInQSem);
	    cicsLogMessage(0,"###########\n write_loWFireVarMsg msgQReceive TIMEOUT name = ");
	    
	    recGblRecordError( errno,(void *)plo,
			   "devloWFireVarMsg: msgQReceive() failed");

	    return( errno );
	}

	strncpy(msg.info.string,incomingMsg.info.string,INSTIO_FLD_SZ);
	msg.info.string[INSTIO_FLD_SZ] = 0;
	/* check if transputer had an error*/
	if (strstr(msg.info.string,"ERROR"))
	{  sprintf (buf," %s\n",incomingMsg.info.string);  
	   cicsLogMessage(3,buf);
	    return ERROR;
	}
    }
    else 	
    {
	cicsLogMessage(0,"Couldn't take semaphore\n");
	return ERROR;
    }
    /* Don't see anything else that needs to be done here. */
    return( OK ) ;
}
/*
 *+
 * FUNCTION NAME:
 * init_liWFireVarMsg
 *
 * INVOCATION:
 * 
 * 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 *  
 *
 * FUNCTION VALUE:
 *   none
 *
 * PURPOSE:
 *   initializes the wfire Message queques for wFireVarMsg messages and 
 *       sets up parameters for li records
 *
 *
 * DESCRIPTION:
 * 
 *
 * EXTERNAL VARIABLES:
 * None
 *
 * PRIOR REQUIREMENTS:
 * none?
 *
 * DEFICIENCIES:
 * None
 *
 * HISTORY:
 * June-1997  Original version adapted from CICS alpha 1.0  Jan Schwitters
 *
 *-
 */
/* Initial attempt, based on $EPICS/base/src/dev/dev*Ai*.c. */

STATIC long init_liWFireVarMsg(pli)
struct longinRecord *pli;
{
    struct WFMsgInfo	*pWFMsgInfo ;

/* 	cicsLogMessage(3, "***********init_liWFireVarMsg():\n" ) ; */
    
 

    if( pli->inp.type != INST_IO ) {
        recGblRecordError(S_db_badField,(void *)pli,
            "devliWFireVarMsg (init_record) Illegal inp.type");
        return(S_db_badField);
    }
#if 0
 if (createQueues()!= OK)
       recGblRecordError(errno,(void *)pli,
			      "devWFireVarMsg: WFMsg_Q_ID: msgQCreate() failed");
#endif
    /* Fill in and save the WFMsgInfo structure. */
    pWFMsgInfo = (struct WFMsgInfo *)
	malloc( sizeof( struct WFMsgInfo ) ) ;
    bzero( (char *)pWFMsgInfo, sizeof( struct WFMsgInfo ) ) ;
    /* Save the associated record structure just in case someone wants it
     * later.
     */
    pWFMsgInfo->precord = (void *)pli ;
    /* Identify the type of record which caused the creation of this
     * structure.
     */
    pWFMsgInfo->recId = WFMsgLi ;
    /* Save the command string. */
    (void)strncpy( pWFMsgInfo->string, pli->inp.value.instio.string,
	INSTIO_FLD_SZ );
    if( sscanf( pli->inp.value.instio.string, WFVarFormat,
	&pWFMsgInfo->node,
	&pWFMsgInfo->var,
	&pWFMsgInfo->grp,
	&pWFMsgInfo->idx) != 4 ) {
	    recGblRecordError(S_db_badField,(void *)pli,
		"devliWFireVarMsg (init_record) Illegal out.value");
	    return(S_db_badField);
    }
    pli->dpvt = (char *)pWFMsgInfo ;

    /* Don't see anything else that needs to be done here. */
    return( OK ) ;

}
/*
 *+
 * FUNCTION NAME:
 * read_liWFireVarMsg
 *
 * INVOCATION:
 * 
 * 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 *  
 *
 * FUNCTION VALUE:
 *   none
 *
 * PURPOSE:
 *    Read a value from transputers and return to EPICS li record 
 *         (from EPICS record with DTYP = wFifeVarMsg)
 *
 * DESCRIPTION:
 * 
 *
 * EXTERNAL VARIABLES:
 * None
 *
 * PRIOR REQUIREMENTS:
 * none?
 *
 * DEFICIENCIES:
 * None
 *
 * HISTORY:
 * June-1997  Original version adapted from CICS alpha 1.0  Jan Schwitters
 *
 *-
 */
STATIC long read_liWFireVarMsg(pli)
    struct longinRecord	*pli;
{
    struct WFMsgMsg	msg ;
    struct WFMsgMsg	incomingMsg ;

     cicsLogMessage(3, "\n\n*********read_liWFireVarMsg().\n" ) ; 

    /* Fill in the outgoing message and send it off to the queue. */
    msg.info = *(struct WFMsgInfo *)pli->dpvt ;
    msg.sequence = sequence++ ;
    msg.val.lval = pli->val ;

    /* get the receive Message Queue semaphore here */
    if (semTake( wfParams.EpicsInQSem, WFSemTakeTimeout) == OK)
    {
	if( msgQSend( WFMsg_Q_ID_out, 
		      (char *)&msg, 
		  sizeof( msg ),
		  NO_WAIT, 
		  MSG_PRI_NORMAL ) != OK )
	{
	    /* Give the receive Message Queue semaphore here */
	    (void)semGive( wfParams.EpicsInQSem); 
	    recGblRecordError( errno, (void *)pli,
			   "devliWFireVarMsg (read_record) msgQSend() failed");
	    return( errno );
	}

	/* Wait for the reply or timeout. */
	if( msgQReceive( WFMsg_Q_ID_in, 
		     (char *)&incomingMsg,
		     sizeof( incomingMsg ), 
		     WFMsgRetTimeout ) != ERROR )
	{
	    /* We got a message. */
	    /* Give the receive Message Queue semaphore here */
	    (void)semGive( wfParams.EpicsInQSem);

	    pli->val = incomingMsg.val.lval ;
	    
	    /* Don't see anything else that needs to be done here. */

	    strncpy(msg.info.string,incomingMsg.info.string,INSTIO_FLD_SZ);
	    msg.info.string[INSTIO_FLD_SZ] = 0;

	    /* check if transputer had an error*/
	    if (strstr(msg.info.string,"ERROR"))
	    {
		return ERROR;
	    }

	    return( OK ) ;
	}
	else
	{
	    /* Give the receive Message Queue semaphore here */
	    (void)semGive( wfParams.EpicsInQSem);
	    recGblRecordError( errno,(void *)pli,
			   "devliWFireVarMsg (read_record) msgQReceive() failed");
    
	    return( errno );
	}
    }
    else	
    {
	cicsLogMessage(0,"Couldn't take semaphore\n");
	return ERROR;
    }

}
/*
 *+
 * FUNCTION NAME:
 * init_soWFireVarMsg
 *
 * INVOCATION:
 * 
 * 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 *  
 *
 * FUNCTION VALUE:
 *   none
 *
 * PURPOSE:
 *   initializes the wfire Message queques for wFireVarMsg messages and 
 *       sets up parameters for so records
 *
 *
 * DESCRIPTION:
 * 
 *
 * EXTERNAL VARIABLES:
 * None
 *
 * PRIOR REQUIREMENTS:
 * none?
 *
 * DEFICIENCIES:
 * None
 *
 * HISTORY:
 * June-1997  Original version adapted from CICS alpha 1.0  Jan Schwitters
 *
 *-
 */
STATIC long init_soWFireVarMsg(pso)
struct stringoutRecord *pso;
{
    struct WFMsgInfo	*pWFMsgInfo ;

/* 	cicsLogMessage(3, "***********init_soWFireVarMsg():\n" ) ; */
    
   
    if( pso->out.type != INST_IO ) {
        recGblRecordError(S_db_badField,(void *)pso,
            "devsoWFireVarMsg (init_record) Illegal out.type");
        return(S_db_badField);
    }

#if 0
 if (createQueues()!= OK)
       recGblRecordError(errno,(void *)pso,
			      "devWFireVarMsg: WFMsg_Q_ID: msgQCreate() failed");
#endif
    /* Fill in and save the WFMsgInfo structure. */
    pWFMsgInfo = (struct WFMsgInfo *)
	malloc( sizeof( struct WFMsgInfo ) ) ;
    bzero( (char *)pWFMsgInfo, sizeof( struct WFMsgInfo ) ) ;
    /* Save the associated record structure just in case someone wants it
     * later.
     */
    pWFMsgInfo->precord = (void *)pso ;
    /* Identify the type of record which caused the creation of this
     * structure.
     */
    pWFMsgInfo->recId = WFMsgSo ;
    /* Save the command string. */
    (void)strncpy( pWFMsgInfo->string, pso->out.value.instio.string,
	INSTIO_FLD_SZ );
    if( sscanf( pso->out.value.instio.string, WFVarFormat,
	&pWFMsgInfo->node,
	&pWFMsgInfo->var,
	&pWFMsgInfo->grp,
	&pWFMsgInfo->idx) != 4 ) {
	    recGblRecordError(S_db_badField,(void *)pso,
		"devsoWFireVarMsg (init_record) Illegal out.value");
	    return(S_db_badField);
    }
    pso->dpvt = (char *)pWFMsgInfo ;

    /* Don't see anything else that needs to be done here. */
    return( OK ) ;
}
/*
 *+
 * FUNCTION NAME:
 * write_soWFireVarMsg
 *
 * INVOCATION:
 * 
 * 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 *  
 *
 * FUNCTION VALUE:
 *   none
 *
 * PURPOSE:
 *    Write a value from EPICS so record (from EPICS record
 *        with DTYP = wFireVarMsg) to transputers
 *
 * DESCRIPTION:
 * 
 *
 * EXTERNAL VARIABLES:
 * None
 *
 * PRIOR REQUIREMENTS:
 * none?
 *
 * DEFICIENCIES:
 * None
 *
 * HISTORY:
 * June-1997  Original version adapted from CICS alpha 1.0  Jan Schwitters
 *
 *-
 */
STATIC long write_soWFireVarMsg(pso)
    struct stringoutRecord	*pso;
{
    struct WFMsgMsg	msg ;
    struct WFMsgMsg	incomingMsg ;
 
	cicsLogMessage(3, "***********write_soWFireVarMsg():\n" ) ;
    
    /* Fill in the message and send it off to the queue. */
    msg.info = *(struct WFMsgInfo *)pso->dpvt ;
    msg.sequence = sequence++ ;
    (void)strncpy( msg.val.sval, pso->val, 40 ) ;
    /* get the receive Message Queue semaphore here */
    if (semTake( wfParams.EpicsInQSem, WFSemTakeTimeout) == OK)
    {
	if( msgQSend( WFMsg_Q_ID_out, 
		  (char *)&msg, 
		  sizeof( msg ),
		  NO_WAIT, 
		  MSG_PRI_NORMAL ) != OK )
	{
	    /* Give the receive Message Queue semaphore here */
	    (void)semGive( wfParams.EpicsInQSem); 
	    recGblRecordError( errno,(void *)pso,
			   "devsoWFireVarMsg (write_so) msgQSend() failed");
	    return( errno );
	}

	/* Wait for the reply or timeout. */
	if( msgQReceive(WFMsg_Q_ID_in, 
		    (char *)&incomingMsg,
		    sizeof(incomingMsg),  
		    WFMsgRetTimeout) != ERROR ) 
	{	
	    /* Give the receive Message Queue semaphore here */
	    (void)semGive( wfParams.EpicsInQSem);
	     sprintf (buf," %s\n",incomingMsg.val.sval);  
	   cicsLogMessage(3,buf);
	    /* We got the a message. */
	    strncpy(pso->val, incomingMsg.val.sval, 39);

	}
	else
	{
	    /* Give the receive Message Queue semaphore here */
	    (void)semGive( wfParams.EpicsInQSem);
	    cicsLogMessage(0,"###########\n write_soWFireMsg msgQReceive TIMEOUT\n##########");
	    recGblRecordError( errno,(void *)pso,
			   "devaoWFireVarMsg: msgQReceive() failed");

	    return( errno );
	}

	strncpy(msg.info.string,incomingMsg.info.string,INSTIO_FLD_SZ);
	msg.info.string[INSTIO_FLD_SZ] = 0;
	/* check if transputer had an error*/
	if (strstr(msg.info.string,"ERROR"))
	{
	    return ERROR;
	}
    }
    else 	
    {
	cicsLogMessage(0,"Couldn't take semaphore\n");
	return ERROR;
    }
    /* Don't see anything else that needs to be done here. */

    return( OK ) ;
}
/*
 *+
 * FUNCTION NAME:
 * init_siWFireVarMsg
 *
 * INVOCATION:
 * 
 * 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 *  
 *
 * FUNCTION VALUE:
 *   none
 *
 * PURPOSE:
 *   initializes the wfire Message queques for wFireVarMsg messages and 
 *       sets up parameters for si records
 *
 * DESCRIPTION:
 * 
 *
 * EXTERNAL VARIABLES:
 * None
 *
 * PRIOR REQUIREMENTS:
 * none?
 *
 * DEFICIENCIES:
 * None
 *
 * HISTORY:
 * June-1997  Original version adapted from CICS alpha 1.0  Jan Schwitters
 *
 *-
 */
/* Initial attempt, based on $EPICS/base/src/dev/dev*Ai*.c. */

STATIC long init_siWFireVarMsg(psi)
struct stringinRecord *psi;
{
    struct WFMsgInfo	*pWFMsgInfo ;
 
/* 	cicsLogMessage(3, "***********init_siWFireVarMsg():\n" ) ; */
    
   

    if( psi->inp.type != INST_IO ) {
        recGblRecordError(S_db_badField,(void *)psi,
            "devsiWFireVarMsg (init_record) Illegal inp.type");
        return(S_db_badField);
    }

    /* Fill in and save the WFMsgInfo structure. */
    pWFMsgInfo = (struct WFMsgInfo *)
	malloc( sizeof( struct WFMsgInfo ) ) ;
    bzero( (char *)pWFMsgInfo, sizeof( struct WFMsgInfo ) ) ;
    /* Save the associated record structure just in case someone wants it
     * later.
     */
    pWFMsgInfo->precord = (void *)psi ;
    /* Identify the type of record which caused the creation of this
     * structure.
     */
    pWFMsgInfo->recId = WFMsgSi ;
    /* Save the command string. */
    (void)strncpy( pWFMsgInfo->string, psi->inp.value.instio.string,
	INSTIO_FLD_SZ );
    if( sscanf( psi->inp.value.instio.string, WFVarFormat,
	&pWFMsgInfo->node,
	&pWFMsgInfo->var,
	&pWFMsgInfo->grp,
	&pWFMsgInfo->idx) != 4 ) {
	    recGblRecordError(S_db_badField,(void *)psi,
		"devsiWFireVarMsg (init_record) Illegal out.value");
	    return(S_db_badField);
    }
    psi->dpvt = (char *)pWFMsgInfo ;

    /* Don't see anything else that needs to be done here. */
    return( OK ) ;

}
/*
 *+
 * FUNCTION NAME:
 * read_siWFireVarMsg
 *
 * INVOCATION:
 * 
 * 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 *  
 *
 * FUNCTION VALUE:
 *   none
 *
 * PURPOSE:
 *    Read a value from transputers and return to EPICS si record 
 *         (from EPICS record with DTYP = wFifeVarMsg)
 *
 * DESCRIPTION:
 * 
 *
 * EXTERNAL VARIABLES:
 * None
 *
 * PRIOR REQUIREMENTS:
 * none?
 *
 * DEFICIENCIES:
 * None
 *
 * HISTORY:
 * June-1997  Original version adapted from CICS alpha 1.0  Jan Schwitters
 *
 *-
 */
STATIC long read_siWFireVarMsg(psi)
    struct stringinRecord	*psi;
{
    struct WFMsgMsg	msg ;
    struct WFMsgMsg	incomingMsg ;
 
	cicsLogMessage(3, "***********read_siWFireVarMsg():\n" ) ;
    
    /* Fill in the outgoing message and send it off to the queue. */
    msg.info = *(struct WFMsgInfo *)psi->dpvt ;
    msg.sequence = sequence++ ;
    (void)strncpy( msg.val.sval, psi->val, 40 ) ;
    /* get the receive Message Queue semaphore here */
    if (semTake( wfParams.EpicsInQSem, WFSemTakeTimeout) == OK)
    {
	if( msgQSend( WFMsg_Q_ID_out, 
		  (char *)&msg, 
		  sizeof( msg ),
		  NO_WAIT, 
		  MSG_PRI_NORMAL ) != OK )
	{
	    /* Got an error on msgQsend */
	    /* Give the receive Message Queue semaphore here */
	    (void)semGive( wfParams.EpicsInQSem); 
	    recGblRecordError( errno,(void *)psi,
			   "devsiWFireVarMsg (read_record) msgQSend() failed");
	    return( errno );
	}

	/* Wait for the reply or timeout. */
	if( msgQReceive( WFMsg_Q_ID_in, 
		     (char *)&incomingMsg,
		     sizeof( incomingMsg ), 
		     WFMsgRetTimeout) != ERROR )
	{
	    /* We got a message. */
	    /* Give the receive Message Queue semaphore here */
	    (void)semGive( wfParams.EpicsInQSem);
	    (void)strncpy( psi->val, incomingMsg.val.sval, 40 ) ;
	
	    strncpy(msg.info.string,incomingMsg.info.string,INSTIO_FLD_SZ);
	    msg.info.string[INSTIO_FLD_SZ] = 0;
	    /* check if transputer had an error*/
	    if (strstr(msg.info.string,"ERROR"))
	    {  sprintf (buf," %s\n",incomingMsg.info.string);  
	   cicsLogMessage(3,buf);
		return ERROR;
	    }
	    /* Don't see anything else that needs to be done here. */
	    return( OK ) ;

	}
	else 
	{
	    /* Give the receive Message Queue semaphore here */
	    (void)semGive( wfParams.EpicsInQSem);
	    recGblRecordError( errno,(void *)psi,
			   "devsiWFireVarMsg (read_record) msgQReceive() failed");
	    return( errno );
	}
    }
    else
    {
	cicsLogMessage(0,"Couldn't take semaphore\n");
	return ERROR;
    }
    return OK;

}
