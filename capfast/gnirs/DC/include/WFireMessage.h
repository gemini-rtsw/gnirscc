/* Include file for WFMessage routines. */

enum	WFMsgId	{ WFMsgAo=0, WFMsgAi, WFMsgLo, WFMsgLi, WFMsgSo, WFMsgSi } ;

/* This structure will contain the fixed portion of the relevant part of the
 * EPICS record.
 * Some parsing will probably be done at init time.
 * 
 * Fri Feb 28 08:30:20 MST 1997
 * String in question looks like this: "Node=2,Var=0,Idx=0"
 * Thu May  1 10:06:24 MST 1997
 * String in question looks like this: "Node=2,Var=9,Grp=4"
 */
#if	defined(WFDevSup)
/* Try to simplify and centralize changes in WFDevSup code needed to accomodate
 * changes in the fixed string format.
 * The code looks like:
    if( sscanf( pao->out.value.instio.string, "Node=%d,Var=%d,Idx=%d",
	&paoWFMsgInfo->node,
	&paoWFMsgInfo->var,
	&paoWFMsgInfo->idx ) != 3 ) {
 * We will define WFStringFormat and WFVariable so that the above becomes:
    if( sscanf( pao->out.value.instio.string, WFStringFormat,
	&paoWFMsgInfo->node,
	&paoWFMsgInfo->var,
	&paoWFMsgInfo->WFVariable ) != 3 ) {
 */
#define	WFStringFormat	"Node=%d,Var=%d,Grp=%d"
#define	WFVariable	grp
#endif
struct aoWFMsgInfo	{
    void	*precord ;
    char	string[40];	/* Must be at least INSTIO_FLD_SZ characters. */
    int		node, var, idx, grp ;
    enum	WFMsgId	recId ;	/* Type of associated record. */
};

union	val	{
    double	dval ;
    long	lval ;
    char	sval[40] ;
} ;
/* This is the structure which will be queued when the analog output record
 * is processed.
 * This is the structure queued to the outgoing message queue.
 */ 
struct aoWFMsgMsg	{
    struct	aoWFMsgInfo	info ;
    unsigned	long	sequence ;
    union	val	val ;
};
#define	MAXaoWFMsgMsgs	10

/* This is the structure which will be placed on the analog input record
 * incoming message queue.
 */ 
struct aiWFMsgMsg	{
    enum	WFMsgId	recId ;	/* Type of associated record. */
    union	val	val ;
};
#define	MAXaiWFMsgMsgs	8
/* Wait interval for incoming ai reply. */
#define	aiWFMsgTimeout	(sysClkRateGet()/5)

#if	!defined(WFDevSup)

/* This is for the vxWorks code.  It knows nothing about EPICS. */
#include	<msgQLib.h>
#include	<semLib.h>

/* A structure which is used to communicate with aoMsgTask.
 * And possibly other tasks.
 */
struct	WFMsgTaskParameters {
    int	initialized ;
    int	stop ;		/* Bits used to stop tasks. */
#define	STOP_AI_TASK	0x1
#define	STOP_AO_TASK	0x2
#define	STOP_XX_TASK	0x4
#define	STOP_DM_TASK	0x8
    int	verbose ;
    int	timeout ;
    int	linkFd ;
    SEM_ID readLinkSem ;
    int	nRead, nWritten ;
    /* The queues for messages read from the transputer.
     * I.E. incoming messages.
     */
    MSG_Q_ID	WFMsg_VAR_READ_Q_ID ;
    MSG_Q_ID	WFMsg_DEBUG_MSG_Q_ID ;
    MSG_Q_ID	WFMsg_SET_VAR_Q_ID ;
    MSG_Q_ID	WFMsg_THE_REST_Q_ID ;
};

#endif
