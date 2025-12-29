


/* devAoWFireMessage.c */

/* Initial attempt, based on $EPICS/base/src/dev/dev*Ao*.c. */

#include	<vxWorks.h>
#include	<types.h>
#include	<stdlib.h>
#include	<sysLib.h>
#include	<msgQLib.h>
#include	<taskLib.h>
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

#include	"WFireMessage.h"

#include	"../../../include/protocol.h"

#if	defined(STUBS)
#define	STATIC
#else
#define	STATIC	static
/* Create the dset for devAoWFireMessage */

/* Note that this device set is implicitly required to conform to the
 * dset structure as defined in $EPICS/base/include/devSup.h and also to the
 * aodset structure as defined in $EPICS/base/src/rec/recAo.c.
 */
STATIC long init_WFireCmdMsg();
STATIC long write_WFireCmdMsg();
struct 
{
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

/* The aoWFireVarMsg queue.
 * It seems that this queue can be used for outgoing ai messages also.
 * Probably li, lo, si, so also.
 */
MSG_Q_ID	WFMsg_Q_ID_out = NULL ;
/* For incoming ai messages.
 * It seems that this queue can be used for incoming li messages also.
 * Probably si also.
 */
MSG_Q_ID	WFMsg_Q_ID_in = NULL ;
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
/* Sequence numbers for things going into the WFMsg_Q_ID_out. */
static unsigned long sequence = 1 ;

STATIC long init_WFireCmdMsg(plo)
struct longoutRecord *plo;
{
    struct WFMsgInfo	*pWFMsgInfo ;

	printf( "***********init_WFireCmdMsg():\n" ) ;
    
   

    if( plo->out.type != INST_IO ) {
        recGblRecordError(S_db_badField,(void *)plo,
            "devWFireVarMsg (init_record) Illegal out.type");
        return(S_db_badField);
    }

    /* Create a message queue for loWFireVarMsg's.
     * This queue is also used for ai (and other) messages so it may have
     * been created elsewhere.
     */
    if( WFMsg_Q_ID_out == NULL ) {
	WFMsg_Q_ID_out = msgQCreate( MAXWFMsgMsgs,
	    sizeof( struct WFMsgMsg ), MSG_Q_FIFO ) ; 
	/* Check for error.  I don't think this is necessary on the grounds
	 * that "it's not going to happen".
	 */
	if( WFMsg_Q_ID_out == (MSG_Q_ID)NULL ) {
	    recGblRecordError( errno,(void *)plo,
	    "devWFireVarMsg: WFMsg_Q_ID_out = msgQCreate() failed");
	    return( errno );
	}
    }
    
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
    
	printf ("$$$$$$$$$$record type %d\n",pWFMsgInfo->recId);
    /* Save the command string. */
    (void)strncpy( pWFMsgInfo->string, plo->out.value.instio.string,
	INSTIO_FLD_SZ );
    if( sscanf( plo->out.value.instio.string, WFStringFormat,
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
STATIC long write_WFireCmdMsg(plo)
    struct longoutRecord	*plo;
{
    struct WFMsgMsg	msg;
	printf( "***********write_WFireVarMsg():\n" ) ;
    
    /* Fill in the message and send it off to the queue. */
    msg.info = *(struct WFMsgInfo *)plo->dpvt ;
    msg.sequence = sequence++ ;
    msg.val.lval = plo->val ;
    if( msgQSend( WFMsg_Q_ID_out, (char *)&msg, sizeof( msg ),
	NO_WAIT, MSG_PRI_NORMAL ) != OK ) {
	    recGblRecordError( errno,(void *)plo,
		"devWFireVarMsg (write_record) msgQSend() failed");
	    return( errno );
    }

    /* get response off of queue ??????????*/

    /* Don't see anything else that needs to be done here. */
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

     
	printf( "***********init_aoWFireVarMsg():\n" ) ;
    
    


    if( pao->out.type != INST_IO ) {
        recGblRecordError(S_db_badField,(void *)pao,
            "devaoWFireVarMsg (init_record) Illegal out.type");

        return(S_db_badField);
    }

    /* Create a message queue for aoWFireVarMsg's.
     * This queue is also used for ai (and other) messages so it may have
     * been created elsewhere.
     */
    if( WFMsg_Q_ID_out == NULL ) {
	WFMsg_Q_ID_out = msgQCreate( MAXWFMsgMsgs,
	    sizeof( struct WFMsgMsg ), MSG_Q_FIFO ) ; 
	/* Check for error.  I don't think this is necessary on the grounds
	 * that "it's not going to happen".
	 */
	if( WFMsg_Q_ID_out == (MSG_Q_ID)NULL ) {
	    recGblRecordError( errno,(void *)pao,
	    "devaoWFireVarMsg: WFMsg_Q_ID_out = msgQCreate() failed");

	    return( errno );
	}
    }
    
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
  
    if( sscanf( pao->out.value.instio.string, WFStringFormat,
	&pWFMsgInfo->node,
	&pWFMsgInfo->var,
	&pWFMsgInfo->grp,
	&pWFMsgInfo->idx) != 4 ) {
	    recGblRecordError(S_db_badField,(void *)pao,
		"devaoWFireVarMsg (init_record) Illegal out.value");
	  

	    return(S_db_badField);
    }
    pao->dpvt = (char *)pWFMsgInfo ;
 
    /* Don't see anything else that needs to be done here. */

      
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
STATIC long write_aoWFireVarMsg(pao)
    struct aoRecord	*pao;
{
    struct WFMsgMsg	msg ;
  
	printf( "***********write_aoWFireVarMsg():\n" ) ;
   
    
    /* Fill in the message and send it off to the queue. */
    msg.info = *(struct WFMsgInfo *)pao->dpvt ;
    msg.sequence = sequence++ ;
    msg.val.dval = pao->val ;
    if( msgQSend( WFMsg_Q_ID_out, (char *)&msg, sizeof( msg ),
	NO_WAIT, MSG_PRI_NORMAL ) != OK ) {
	    recGblRecordError( errno,(void *)pao,
		"devaoWFireVarMsg (write_record) msgQSend() failed");
	    return( errno );
    }

    /* Don't see anything else that needs to be done here. */
    return( OK ) ;
}

/* Initial attempt, based on $EPICS/base/src/dev/dev*Ai*.c. */

STATIC long init_aiWFireVarMsg(pai)
struct aiRecord *pai;
{
    struct WFMsgInfo	*pWFMsgInfo ;
    int ii;
 
	printf( "***********init_aiWFireVarMsg():\n" ) ;
    
  

    if( pai->inp.type != INST_IO ) {
        recGblRecordError(S_db_badField,(void *)pai,
            "devaiWFireVarMsg (init_record) Illegal inp.type");
        return(S_db_badField);
    }

    /* Create a message queue for aoWFireVarMsg's. */
    if( WFMsg_Q_ID_out == NULL ) {
	WFMsg_Q_ID_out = msgQCreate( MAXWFMsgMsgs,
	    sizeof( struct WFMsgMsg ), MSG_Q_FIFO ) ; 
	/* Check for error.  I don't think this is necessary on the grounds
	 * that "it's not going to happen".
	 */
	if( WFMsg_Q_ID_out == (MSG_Q_ID)NULL ) {
	    recGblRecordError( errno,(void *)pai,
	    "devaiWFireVarMsg: WFMsg_Q_ID_out = msgQCreate() failed");
	    return( errno );
	}
    }
    
    /* Create a message queue for incoming aiWFireVarMsg's. */
    if( WFMsg_Q_ID_in == NULL ) {
	WFMsg_Q_ID_in = msgQCreate( MAXWFMsgRetMsgs,
	    sizeof( struct WFMsgRetMsg ), MSG_Q_FIFO ) ; 
	/* Check for error.  I don't think this is necessary on the grounds
	 * that "it's not going to happen".
	 */
	if( WFMsg_Q_ID_in == (MSG_Q_ID)NULL ) {
	    recGblRecordError( errno,(void *)pai,
	    "devaiWFireVarMsg: WFMsg_Q_ID_out = msgQCreate() failed");
	    return( errno );
	}
    }
    
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

 

    if( ii = (sscanf( pai->inp.value.instio.string, WFStringFormat,
		      &pWFMsgInfo->node,
		      &pWFMsgInfo->var,
		      &pWFMsgInfo->grp,
		      &pWFMsgInfo->idx)) != 4 ) {
	    recGblRecordError(S_db_badField,(void *)pai,
		"devaiWFireVarMsg (init_record) Illegal out.value");
	    printf ("fields %d, %d, %d, %d\n",ii,pWFMsgInfo->node,	pWFMsgInfo->var,	pWFMsgInfo->grp);
	    return(S_db_badField);
    }
    pai->dpvt = (char *)pWFMsgInfo ;

    /* Don't see anything else that needs to be done here. */

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
STATIC long read_aiWFireVarMsg(pai)
    struct aiRecord	*pai;
{
    struct WFMsgMsg	msg ;
    struct WFMsgRetMsg	incomingMsg ;
    int	i ;
	printf( "***********read_aiWFireVarMsg():\n" ) ;
    
    /* The WFMsg_Q_ID_in queue should probably be empty.
     * I don't know what to do if not.
     */
    if( ( i = msgQNumMsgs( WFMsg_Q_ID_in ) ) != 0 ) {
	printf( "devaiWFireVarMsg (read_record)" ) ;
	printf( " msgQNumMsgs( %s ) = %d.\n", "WFMsg_Q_ID_in", i ) ;
    }

    /* Fill in the outgoing message and send it off to the queue. */
    msg.info = *(struct WFMsgInfo *)pai->dpvt ;
    msg.sequence = sequence++ ;
    msg.val.dval = pai->val ;
    if( msgQSend( WFMsg_Q_ID_out, (char *)&msg, sizeof( msg ),
	NO_WAIT, MSG_PRI_NORMAL ) != OK ) {

	    recGblRecordError( errno,(void *)pai,
		"devaiWFireVarMsg (read_record) msgQSend() failed");
	    return( errno );
    }

    /* Wait for the reply or timeout. */
    if( msgQReceive( WFMsg_Q_ID_in, (char *)&incomingMsg, sizeof( incomingMsg ), 
		     WFMsgRetTimeout ) != ERROR ) 
      {
	printf ("size of WFMsgId %d",sizeof (incomingMsg.recId));
	printf ("###########\n pbr read_aiWFireVarMsg  val = %f\n##########",incomingMsg.val.dval);
	/* We got a message. */
	pai->val = incomingMsg.val.dval ;

	printf ("in = %f, out = %f",incomingMsg.val.dval,pai->val);
	
	/* Don't see anything else that needs to be done here. */
	return( 2) ;

      } 
    else 
      {
	printf ("###########\n read_aiWFireVarMsg TIMEOUT\n##########");
	recGblRecordError( errno,(void *)pai,
			   "devaiWFireVarMsg (read_record) msgQReceive() failed");
	return( errno );
      }

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

	printf( "***********init_loWFireVarMsg():\n" ) ;
    
   

    if( plo->out.type != INST_IO ) {
        recGblRecordError(S_db_badField,(void *)plo,
            "devloWFireVarMsg (init_record) Illegal out.type");
        return(S_db_badField);
    }

    /* Create a message queue for loWFireVarMsg's.
     * This queue is also used for ai (and other) messages so it may have
     * been created elsewhere.
     */
    if( WFMsg_Q_ID_out == NULL ) {
	WFMsg_Q_ID_out = msgQCreate( MAXWFMsgMsgs,
	    sizeof( struct WFMsgMsg ), MSG_Q_FIFO ) ; 
	/* Check for error.  I don't think this is necessary on the grounds
	 * that "it's not going to happen".
	 */
	if( WFMsg_Q_ID_out == (MSG_Q_ID)NULL ) {
	    recGblRecordError( errno,(void *)plo,
	    "devloWFireVarMsg: WFMsg_Q_ID_out = msgQCreate() failed");
	    return( errno );
	}
    }
    
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
    if( sscanf( plo->out.value.instio.string, WFStringFormat,
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
STATIC long write_loWFireVarMsg(plo)
    struct longoutRecord	*plo;
{
    struct WFMsgMsg	msg;
	printf( "***********write_loWFireVarMsg():\n" ) ;
    
    /* Fill in the message and send it off to the queue. */
    msg.info = *(struct WFMsgInfo *)plo->dpvt ;
    msg.sequence = sequence++ ;
    msg.val.lval = plo->val ;
    if( msgQSend( WFMsg_Q_ID_out, (char *)&msg, sizeof( msg ),
	NO_WAIT, MSG_PRI_NORMAL ) != OK ) {
	    recGblRecordError( errno,(void *)plo,
		"devloWFireVarMsg (write_record) msgQSend() failed");
	    return( errno );
    }

    /* Don't see anything else that needs to be done here. */
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

	printf( "***********init_liWFireVarMsg():\n" ) ;
    
 

    if( pli->inp.type != INST_IO ) {
        recGblRecordError(S_db_badField,(void *)pli,
            "devliWFireVarMsg (init_record) Illegal inp.type");
        return(S_db_badField);
    }

    /* Create a message queue for aoWFireVarMsg's. */
    if( WFMsg_Q_ID_out == NULL ) {
	WFMsg_Q_ID_out = msgQCreate( MAXWFMsgMsgs,
	    sizeof( struct WFMsgMsg ), MSG_Q_FIFO ) ; 
	/* Check for error.  I don't think this is necessary on the grounds
	 * that "it's not going to happen".
	 */
	if( WFMsg_Q_ID_out == (MSG_Q_ID)NULL ) {
	    recGblRecordError( errno,(void *)pli,
	    "devliWFireVarMsg: WFMsg_Q_ID_out = msgQCreate() failed");
	    return( errno );
	}
    }
    
    /* Create a message queue for incoming aiWFireVarMsg's. */
    if( WFMsg_Q_ID_in == NULL ) {
	WFMsg_Q_ID_in = msgQCreate( MAXWFMsgRetMsgs,
	    sizeof( struct WFMsgRetMsg ), MSG_Q_FIFO ) ; 
	/* Check for error.  I don't think this is necessary on the grounds
	 * that "it's not going to happen".
	 */
	if( WFMsg_Q_ID_in == (MSG_Q_ID)NULL ) {
	    recGblRecordError( errno,(void *)pli,
	    "devliWFireVarMsg: WFMsg_Q_ID_out = msgQCreate() failed");
	    return( errno );
	}
    }
    
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
    if( sscanf( pli->inp.value.instio.string, WFStringFormat,
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
STATIC long read_liWFireVarMsg(pli)
    struct longinRecord	*pli;
{
    struct WFMsgMsg	msg ;
    struct WFMsgRetMsg	incomingMsg ;

	printf( "*********read_liWFireVarMsg().\n" ) ;
    

    /* Fill in the outgoing message and send it off to the queue. */
    msg.info = *(struct WFMsgInfo *)pli->dpvt ;
    msg.sequence = sequence++ ;
    msg.val.lval = pli->val ;
    if( msgQSend( WFMsg_Q_ID_out, (char *)&msg, sizeof( msg ),
	NO_WAIT, MSG_PRI_NORMAL ) != OK ) {
	    recGblRecordError( errno,(void *)pli,
		"devliWFireVarMsg (read_record) msgQSend() failed");
	    return( errno );
    }

    /* Wait for the reply or timeout. */
    if( msgQReceive( WFMsg_Q_ID_in, (char *)&incomingMsg, sizeof( incomingMsg ),
	WFMsgRetTimeout ) != ERROR ) {

	    /* We got a message. */
	printf ("###########\n read_liWFireVarMsg  val = %d\n##########",incomingMsg.val.lval);
	    pli->val = incomingMsg.val.lval ;

	    /* Don't see anything else that needs to be done here. */

	printf( "*********leaving read_liWFireVarMsg().\n" ) ;
    
	    return( OK ) ;

    } else {

	    recGblRecordError( errno,(void *)pli,
		"devliWFireVarMsg (read_record) msgQReceive() failed");

	printf( "#########\nleaving (error) read_liWFireVarMsg().\n############\n" ) ;
    
	    return( errno );
    }

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

	printf( "***********init_soWFireVarMsg():\n" ) ;
    
   
    if( pso->out.type != INST_IO ) {
        recGblRecordError(S_db_badField,(void *)pso,
            "devsoWFireVarMsg (init_record) Illegal out.type");
        return(S_db_badField);
    }

    /* Create a message queue for aoWFireVarMsg's.
     * This queue is also used for ai (and other) messages so it may have
     * been created elsewhere.
     */
    if( WFMsg_Q_ID_out == NULL ) {
	WFMsg_Q_ID_out = msgQCreate( MAXWFMsgMsgs,
	    sizeof( struct WFMsgMsg ), MSG_Q_FIFO ) ; 
	/* Check for error.  I don't think this is necessary on the grounds
	 * that "it's not going to happen".
	 */
	if( WFMsg_Q_ID_out == (MSG_Q_ID)NULL ) {
	    recGblRecordError( errno,(void *)pso,
	    "devsoWFireVarMsg: WFMsg_Q_ID_out = msgQCreate() failed");
	    return( errno );
	}
    }
    
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
    if( sscanf( pso->out.value.instio.string, WFStringFormat,
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
STATIC long write_soWFireVarMsg(pso)
    struct stringoutRecord	*pso;
{
    struct WFMsgMsg	msg ;
 
	printf( "***********write_soWFireVarMsg():\n" ) ;
    
    /* Fill in the message and send it off to the queue. */
    msg.info = *(struct WFMsgInfo *)pso->dpvt ;
    msg.sequence = sequence++ ;
    (void)strncpy( msg.val.sval, pso->val, 40 ) ;
    if( msgQSend( WFMsg_Q_ID_out, (char *)&msg, sizeof( msg ),
	NO_WAIT, MSG_PRI_NORMAL ) != OK ) {
	    recGblRecordError( errno,(void *)pso,
		"devsoWFireVarMsg (write_record) msgQSend() failed");
	    return( errno );
    }

    /* Don't see anything else that needs to be done here. */
    return( OK ) ;
}

/* Initial attempt, based on $EPICS/base/src/dev/dev*Ai*.c. */

STATIC long init_siWFireVarMsg(psi)
struct stringinRecord *psi;
{
    struct WFMsgInfo	*pWFMsgInfo ;
 
	printf( "***********init_siWFireVarMsg():\n" ) ;
    
   

    if( psi->inp.type != INST_IO ) {
        recGblRecordError(S_db_badField,(void *)psi,
            "devsiWFireVarMsg (init_record) Illegal inp.type");
        return(S_db_badField);
    }

    /* Create a message queue for aoWFireVarMsg's. */
    if( WFMsg_Q_ID_out == NULL ) {
	WFMsg_Q_ID_out = msgQCreate( MAXWFMsgMsgs,
	    sizeof( struct WFMsgMsg ), MSG_Q_FIFO ) ; 
	/* Check for error.  I don't think this is necessary on the grounds
	 * that "it's not going to happen".
	 */
	if( WFMsg_Q_ID_out == (MSG_Q_ID)NULL ) {
	    recGblRecordError( errno,(void *)psi,
	    "devsiWFireVarMsg: WFMsg_Q_ID_out = msgQCreate() failed");
	    return( errno );
	}
    }
    
    /* Create a message queue for incoming aiWFireVarMsg's. */
    if( WFMsg_Q_ID_in == NULL ) {
	WFMsg_Q_ID_in = msgQCreate( MAXWFMsgRetMsgs,
	    sizeof( struct WFMsgRetMsg ), MSG_Q_FIFO ) ; 
	/* Check for error.  I don't think this is necessary on the grounds
	 * that "it's not going to happen".
	 */
	if( WFMsg_Q_ID_in == (MSG_Q_ID)NULL ) {
	    recGblRecordError( errno,(void *)psi,
	    "devsiWFireVarMsg: WFMsg_Q_ID_out = msgQCreate() failed");
	    return( errno );
	}
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
    if( sscanf( psi->inp.value.instio.string, WFStringFormat,
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
STATIC long read_siWFireVarMsg(psi)
    struct stringinRecord	*psi;
{
    struct WFMsgMsg	msg ;
    struct WFMsgRetMsg	incomingMsg ;
 
	printf( "***********read_siWFireVarMsg():\n" ) ;
    
    /* Fill in the outgoing message and send it off to the queue. */
    msg.info = *(struct WFMsgInfo *)psi->dpvt ;
    msg.sequence = sequence++ ;
    (void)strncpy( msg.val.sval, psi->val, 40 ) ;
    if( msgQSend( WFMsg_Q_ID_out, (char *)&msg, sizeof( msg ),
	NO_WAIT, MSG_PRI_NORMAL ) != OK ) {
	    recGblRecordError( errno,(void *)psi,
		"devsiWFireVarMsg (read_record) msgQSend() failed");
	    return( errno );
    }

    /* Wait for the reply or timeout. */
    if( msgQReceive( WFMsg_Q_ID_in, (char *)&incomingMsg, sizeof( incomingMsg ),
	WFMsgRetTimeout ) != ERROR ) {

	    /* We got a message. */
	    (void)strncpy( psi->val, incomingMsg.val.sval, 40 ) ;

	    /* Don't see anything else that needs to be done here. */
	    return( OK ) ;

    } else {

	    recGblRecordError( errno,(void *)psi,
		"devsiWFireVarMsg (read_record) msgQReceive() failed");
	    return( errno );
    }

}
