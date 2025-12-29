/* Include file to declare wfire variables used by the message routines */

#if !defined(WFDEVSUP)
/* This is for the vxWorks code.  It knows nothing about EPICS. */
#include	<vxWorks.h>
#include	<types.h>
#include	<stdlib.h>
#include	<sysLib.h>
#include	<taskLib.h>
#include	<string.h>
#include	<msgQLib.h>
#include	<semLib.h>
#include	<dbAccess.h>


#define WFDEVSUP

#endif

/* this string defines the format of the EPICS string to be used in the set 
 * and read var message records. I spells out which node, variable group and 
 * var index are to be used. The group value is used as an index for setting 
 * DAC's in some systems.  It should be set to -1 is setting an ordianary
 * variable
 */
#define	WFVarFormat	"Node=%d,Var=%d,Grp=%d,Idx=%d"
#define	WFCmdFormat	"Node=%d,Cmd=%d,Grp=%d,Idx=%d"
/* define an enum for all the record types and message types used by the
 * system
 */ 
enum WFMsgId { WFMsgAo=0,
	       WFMsgAi,
	       WFMsgLo,
	       WFMsgLi,
	       WFMsgSo,
	       WFMsgSi,
	       WFCmdMsg
};

/* Both of the following sizes are used to determine space requirements for
 * the message queues In fact neither queue should ever have more than one
 * message in it.
 */
#define	MAXWFMsgMsgs	10	/* size of the outgoing message Queue */
#define	MAXWFMsgRetMsgs	10	/* size of the return message Queue */

/* Timeouts for various purposes:
 * For return messages this needs to be long enough that all the message types
 * complete in less than this time
 */
#define	WFMsgRetTimeout	(sysClkRateGet()*4)
#define	WFSemTakeTimeout (sysClkRateGet()*10)

/* This structure defines the things we need to know about the wfire message
 */
struct WFMsgInfo {
    void *precord ;		/* pointer to the record structure of the
				 * record sending the message */
    char string[80];/* Place to hold the format string for the
				  * Message it must Must be at least
				  * INSTIO_FLD_SZ characters. */
    int	 node, var, idx, grp ;	/* values from the record string from sscanf */
    enum WFMsgId recId;		/* Type of associated record and message. */
};

/* union to describe the values to be stored in a message or record. must
 * handle all of the types used by wfire message records
 */
union val { 
    double dval ;
    long   lval ;
    char   sval[80] ;
};
#define MSG_SIZE 80
/* This is the structure which will be queued when any output message is
 * required When the record is processed this structure is queued to the
 * outgoing message queue.
 */ 
struct WFMsgMsg	{
    struct 	WFMsgInfo info ;
    unsigned	long	sequence ;
    union	val	val ;
};

/* This is the structure which will be queued when any input message is
 * recieved. When the record is processed this structure is queued to the
 * incoming message queue.
 */ 
/* struct WFMsgRetMsg { */
/*     enum  WFMsgId recId ; */	/* Type of associated record. */
/*     union val	  val ; */
/* }; */

/* This structure defines fields used to communicate between the various tasks
 * in the WF message system it is a purely VxWorks set of objects and needs
 * to know nothing about EPICS
 */

struct	WFMsgTaskParameters {
    int	initialized ;
    SEM_ID EpicsInQSem;
    int	stop ;		/* Bits used to stop tasks. */
#define	STOP_TO_EPICS_TASK	0x1
#define	STOP_TO_TP_TASK	0x2
#define	STOP_FROM_TP_TASK	0x4
#define	STOP_DM_TASK	0x8
    int	verbose ;
    int	timeout ;
    int	linkFd ;
    SEM_ID readLinkSem;
    int	nRead, nWritten ;
    /* The queues for messages read from the transputer.
     * I.E. incoming messages.
     */
    MSG_Q_ID	WFMsg_To_EPICS_Q_ID ;
    MSG_Q_ID	WFMsg_DEBUG_MSG_Q_ID ;
    MSG_Q_ID	WFMsg_SET_VAR_Q_ID ; 
/*     MSG_Q_ID	WFMsg_THE_REST_Q_ID ; */
};

 


