static struct {
        void *v;
        char *c;
} rcsid = {
        &rcsid,
        "$Id: WFireMessageUtil.c,v 1.2 2009/05/27 19:33:38 fkraemer Exp $"
};
extern int wfiredebug;
/* #define DEBUG */
#include <debug.h>

/*
 * Copyright 1997 Association of Universities for Research in Astronomy, Inc.
 * See the file COPYRIGHT for more details.
 *
 * FILENAME
 * WFireMessageUtil.c
 *
 * DESCRIPTION
 * These routines receive information off a queue and send it off to the
 * transputers.  The information is placed on the queue by other routines
 * (normally EPICS driven).
 * 
 * FUNCTION NAME(S)
 * Proc - void WFMsgGo()
 * Proc - void WFMsgStop( int force )
 * Proc - int xmodify( int argc, char **argv )
 * Proc - void toTPMsgShow( struct WFMsgMsg *pmsg )
 * Proc - void toEPICSMsgShow( struct WFMsgMsg *pmsg )
 * Proc - void xxMsgShow( char *arg )
 * Proc - static void xxShow( char *comment, MSG_Q_INFO *pinfo, void *(*queueShow)() )
 * Proc - void toTPShow()
 * Proc - static int put_var( struct WFMsgMsg *in, struct WFMsg *out )
 * Proc - static int put_d2a_var( struct WFMsgMsg *in, struct WFMsg *out )
 							not used currently
 * Proc - void toTPMsgQTask()
 * Proc - void toEPICSMsgQTask()
 * Proc - void dmMsgQTask()
 * Proc - void fromTPMsgQTask()
 * Proc - int x( int count, int delay )
 *   
 * DEPENDENCIES
 * The queues referred to in this file should have been built before this is
 *	run.  This is done by the init record functions in the
 *	devWFireMessage.c.  
 *
 *INDENT-OFF*
 * $Log: WFireMessageUtil.c,v $
 * Revision 1.2  2009/05/27 19:33:38  fkraemer
 * fkraemer - copied my complete working dir over trunk
 *
 * Revision 1.1.1.1  1998/12/15 16:18:33  buchholz
 * Imported gnaacSrc into CVS
 *
 * Revision 1.2  1998/11/20 17:16:57  pruckle
 * speed up saver, log messages
 *
 * Revision 1.1.1.1  1998/09/30 16:40:00  pruckle
 * Initial Release
 *
 *INDENT-ON* 
 */

/* Utilities for WFMessage routines. */
void sleep (int a, int b);

#include	<stdio.h>
#include	<usrLib.h>
#include	<taskLib.h>
#include	<string.h>

#include	<wFireMsgDefs.h>
#include	<call_main.h>
#include	<.applTop/tpSource/include/protocol.h>
#include <cicsLib.h>
extern int LinkId;
/* A WFire message with the header and array in a structure. */
struct WFMsg	{
    int	head ;
    int	array[MAXBUFLEN] ;
    int	flags ;
};

/* The WFireMessage queues. */
extern	MSG_Q_ID	WFMsg_Q_ID_out ;
extern	MSG_Q_ID	WFMsg_Q_ID_in ;

extern	int	putWFMsg( int, int, int * ) ;
extern	int	getWFMsg( int, int *, int * ) ;
extern	void	lstWFMsg( FILE *, int, int * ) ;
extern	void	wfm_byte_swap( char *, int ) ;
/* Here is a global variable so other routines can put the putWFMsg code in 
 * verbose mode.
 */
extern	int	putWFMsgVerbose ;

extern	int	OpenLink( char * ) ;
extern	int	CloseLink( int ) ;

/* A structure used to communicate with toTPMsgTask. */
extern struct	WFMsgTaskParameters wfParams  ;

static int stop_to_TPTask() { wfParams.stop |= STOP_TO_TP_TASK; return( 2 ); }
static int stop_to_EPICS_Task() { wfParams.stop |= STOP_TO_EPICS_TASK; return( 2 ); }
static int stop_from_TP_Task() { wfParams.stop |= STOP_FROM_TP_TASK;  return( 2 ); }
static int stop_dmTask() { wfParams.stop |= STOP_DM_TASK; return( 2 ); }

/* We attempt to standardize as indicated below. */

static	struct	WFMsg_tasks_info	{
	int	auto_go ;	/* TRUE for tasks started in WFMsgGo() */
#define	BASE_PRIO	100
	int	prio ;
	char	*entry ;	/* Entry point name */
	char	*arg ;
	int	(*stop_me)() ;
	int	tid ;
	char	name[100] ;	/* Task name */
} WFMsg_tasks[] = {
    { 1, BASE_PRIO+10, "toTPMsgQTask", "", stop_to_TPTask, 0, "", },
    { 1, BASE_PRIO+12, "fromTPMsgQTask", "", stop_from_TP_Task, 0, "", },
    { 1, BASE_PRIO+14, "toEPICSMsgQTask", "", stop_to_EPICS_Task, 0, "", },
    { 1, BASE_PRIO+40, "dmMsgQTask", "", stop_dmTask, 0, "", },
    { 0, 0, (char *)NULL, }, /* To signal end. */
} ;
  int x(struct WFMsg msg ,  int count, int delay  );

char tmp[80];
int createQueues();
void debugon ()
{
      wfParams.verbose = 1;
}

void debugoff ()
{
      wfParams.verbose = 0;
}

/*
 *FUNCTION NAME:
 *	WFMsgGo()
 *
 *INVOCATION:
 *	WFMsgGo();
 *
 *PARAMETERS: (">" input, "!" modified, "<" output)
 *	none
 *
 *FUNCTION VALUE:
 *	no return value
 *
 *PURPOSE:
 *	starts the WFire message routines
 *
 *DESCRIPTION:
 *	This routine spawns the tasks that read the queues and communicate
 *		with the transputers.
 *
 *
 *EXTERNAL VARIABLES:
 *	None
 *
 *PRIOR REQUIREMENTS:
 *	The EPICS databases need to be loaded and iocinit needs to be run 
 *		before this is run.
 *
 *DEFICIENCIES:
 *	None
 *
 *HISTORY:
 *	June-1997  Original version adapted from CICS alpha 1.0  Jan Schwitters
 *
 */
void WFMsgGo()
{
    extern int spTaskOptions ;
    extern int spTaskStackSize ;
    int	tid ;
    struct	WFMsg_tasks_info	*ip ;
	
    /* We need to initialize wfParams.
     * It is essential to do the initialization here.
     * See code in WFMsgStop() below.
     *
     */
    createQueues();
    if( ! wfParams.initialized ) 
    {
	int	i ;
	/* We expect the out in queues to be empty.  If not, say so. */
	if( ( i = msgQNumMsgs( WFMsg_Q_ID_out ) ) != 0 ) 
	{
	    struct WFMsgMsg	msg ;

	    /* Make it empty. */
	    while( msgQReceive( WFMsg_Q_ID_out, 
				(char *)&msg, 
				sizeof( msg ),
				NO_WAIT ) != ERROR ) ;
	}
	if( ( i = msgQNumMsgs( WFMsg_Q_ID_in ) ) != 0 ) 
	{
	    struct WFMsgMsg	msg ;
	    while( msgQReceive( WFMsg_Q_ID_in, 
				(char *)&msg, 
				sizeof( msg ),
				NO_WAIT ) != ERROR ) ;
	}
	bzero( (char *)&wfParams, sizeof( wfParams ) ) ;

	/* Create message queues for WFire messages received.
	 * We note that things can be left in an indeterminate state if any of
	 * the initializations below fail.
	 */
	wfParams.WFMsg_To_EPICS_Q_ID = msgQCreate( MAXWFMsgMsgs,
						   sizeof( struct WFMsg ), MSG_Q_FIFO ) ; 
	if( wfParams.WFMsg_To_EPICS_Q_ID == (MSG_Q_ID)NULL ) 
	{
	    printErr("In WFMsgGo(), WFMsg_To_EPICS_Q_ID = msgQCreate() failed.\n" );
	    return ;
	}
	/* There seem to be lots of debug messages. */
	wfParams.WFMsg_DEBUG_MSG_Q_ID = msgQCreate( MAXWFMsgMsgs*4,
						    sizeof( struct WFMsg ), MSG_Q_FIFO ) ; 
	if( wfParams.WFMsg_DEBUG_MSG_Q_ID == (MSG_Q_ID)NULL ) 
	{
	    printErr("In WFMsgGo(), WFMsg_DEBUG_MSG_Q_ID = msgQCreate() failed.\n" );
	    return ;
	}

	wfParams.WFMsg_SET_VAR_Q_ID = msgQCreate( MAXWFMsgMsgs,
						  sizeof( struct WFMsg ), MSG_Q_FIFO ) ; 
	if( wfParams.WFMsg_SET_VAR_Q_ID == (MSG_Q_ID)NULL ) 
	{
	    printErr("In WFMsgGo(), WFMsg_SET_VAR_Q_ID = msgQCreate() failed.\n" );
	    return ;
	}
#if 0
	wfParams.WFMsg_THE_REST_Q_ID = msgQCreate( MAXWFMsgMsgs,
						   sizeof( struct WFMsg ), MSG_Q_FIFO ) ; 
	if( wfParams.WFMsg_THE_REST_Q_ID == (MSG_Q_ID)NULL ) 
	{
	    printErr("In WFMsgGo(), WFMsg_THE_REST_Q_ID = msgQCreate() failed.\n" );
	    return ;
	}
#endif
	/* We expect link to be closed here. */
	if( ( LinkId=wfParams.linkFd = OpenLink( NULL ) ) <= 0 ) 
	{
	    printErr( "In WFMsgGo(), cannot open link.\n" ) ;
	    return ;
	}
	wfParams.readLinkSem = semBCreate( SEM_Q_PRIORITY, SEM_EMPTY ) ;
	wfParams.EpicsInQSem = semBCreate( SEM_Q_PRIORITY, SEM_FULL  ) ;
	wfParams.timeout = 50 ;
	wfParams.initialized = TRUE ;
    }

    /* Start the necessary tasks. */
    ip = WFMsg_tasks ;
    while( ip->entry ) {
	int	value ;
	int	vwSymFind() ;
	char	*vwTaskName() ;
	/* Fill in the name field. */
	strcpy( ip->name, vwTaskName( ip->entry ) ) ;
	if( (ip->auto_go) && ((ip->tid==NULL)
			      || ((tid=taskNameToId(ip->name))==ERROR)
			      || (taskIdVerify(tid)==ERROR)) ) {
	    /* Find the command in the system symbol table. */
	    if( ( value = vwSymFind( ip->entry ) ) == ERROR ) 
	    {
		sprintf(tmp,"Task entry point \"%s\" not in symbol table.", 
			 ip->entry ) ;
		cicsLogMessage(0,tmp);
	    } 
	    else 
	    {
		/* Spawn the task with specified arguments and priority. */
		fprintf( stderr, "Spawn %s( %s ).\n", ip->name, ip->arg ) ;
		ip->tid = taskSpawn( ip->name,
				     ip->prio, spTaskOptions,
				     spTaskStackSize, (FUNCPTR)value, (int)ip->arg,
				     0, 0, 0, 0, 0, 0, 0, 0, 0);
	    }
	}
	ip++ ;
    }
}

/* For someone who has trouble typing with the shift key. */
void wfmsggo() { WFMsgGo() ; }

/* Here is a subroutine to stop WFMsg tasks.
 */

/*
 *+
 * FUNCTION NAME:
 * WFMsgStop
 *
 * INVOCATION:
 * int force
 * void WFMsgStop(force)
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 *  > force - value of 1 causes task to be deleted if it doesn't stop by itself 
 *
 * FUNCTION VALUE:
 *   none
 *
 * PURPOSE:
 *    Stops the WFire message routines
 *
 * DESCRIPTION:
 * Kills the tasks spawned in WFMsgGo
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
    void
WFMsgStop( int force )
{
    int	k ;
    int	tid ;
    struct	WFMsg_tasks_info	*ip ;
    char	*id = "WFMsgStop():" ;

    ip = WFMsg_tasks ;
    while( ip->entry ) {
	char	*vwTaskName() ;
	/* Fill in the name field. */
	strcpy( ip->name, vwTaskName( ip->entry ) ) ;
	/*
	 * Is there a task to stop?
	 * We assume so if taskIdVerify() returns OK.
	 */
	if( ((tid=taskNameToId(ip->name))!=ERROR)
	    && (taskIdVerify(tid)==OK) ) 
	  {
	    if( ((ip->tid!=NULL) && (ip->tid!=tid)) ) 
	      {
		sprintf(tmp,
			 "%s Task id mismatch for %s, 0X%x!=0X%x.\n",
			 id, ip->name, tid, ip->tid ) ;
		cicsLogMessage(0,tmp);
	      }
	    if( ip->stop_me ) 
	      {
		/*
		 * Try to stop the task, but not forever.
		 * (*ip->stop_me)() returns the time to wait in seconds.
		 */
		k = (*ip->stop_me)() *  sysClkRateGet() ;
		while( (taskIdVerify(tid)==OK) && (k>0) ) 
		  {
		    /* Task is still there. */
		    (void)(*ip->stop_me)() ;
		    taskDelay( sysClkRateGet() / 10 ) ;
		    k -= sysClkRateGet() / 10 ;
		  }
		if( k>0 )
		  tid = ip->tid = NULL ;
		else
		  fprintf( stderr,
			   "%s Could not stop %s.\n", id, ip->name ) ;
	      }
	    else 
	      {
		fprintf( stderr,
			 "%s don't know how to stop %s.\n",
			 id, ip->name ) ;
	      }
	    if( force && (tid!=NULL) ) 
	      {
		    if( taskDelete(tid) == OK ) 
		      {
			fprintf( stderr,
				 "%s task %s deleted.\n",
				 id, ip->name ) ;
			tid = ip->tid = NULL ;
		      } 
		    else 
		      {
			fprintf( stderr,
				 "%s could not delete %s.\n",
				 id, ip->name ) ;
		      }
	      }
	  }
	ip++ ;
    }

    /* Clean up wfParams.
     * See code in WFMsgGo() above.
     */
    if( wfParams.initialized ) 
      {
	(void)CloseLink( wfParams.linkFd ) ;
	semDelete( wfParams.readLinkSem ) ;
	msgQDelete( wfParams.WFMsg_To_EPICS_Q_ID ) ;
	msgQDelete( wfParams.WFMsg_DEBUG_MSG_Q_ID ) ;
	msgQDelete( wfParams.WFMsg_SET_VAR_Q_ID ) ;
#if 0
	msgQDelete( wfParams.WFMsg_THE_REST_Q_ID ) ;
#endif
	wfParams.initialized = FALSE ;
      }
}
void wfmsgstop( int a ) { WFMsgStop( a ) ; }
/*
 *+
 * FUNCTION NAME:
 * xmodify
 *
 * INVOCATION:
 * char x
 *  modify (x)
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * >x - this is a flag:  possibilities are "[-t timeout] [-s{diox}] [-vN]"
 *
 * FUNCTION VALUE:
 *  status = modify (x)
 *
 * PURPOSE:
 * change parameters that are used by WFireMsg tasks
 *
 * DESCRIPTION:
 *  This is used mostly for debugging.  It allows you to stop individual WFireMsg routines, 
 *       print debugging messages or set a timeout.
 *
 * EXTERNAL VARIABLES:
 * None
 *
 * PRIOR REQUIREMENTS:
 *
 *
 * DEFICIENCIES:
 * None
 *
 * HISTORY:
 * June-1997  Original version adapted from CICS alpha 1.0  Jan Schwitters
 *
 *-
 */
call_main( modify, xmodify ) 

    int
xmodify( int argc, char **argv )
{
  char	*usage = "[-t timeout] [-s{diox}] [-vN]";
  char	*pc ;
  int	i;
  
  /* Check for command line arguments. */
  if( argc <= 1 )
    goto BAD ;
  while (argc > 1 && *argv[1] == '-') 
    {
      switch (argv[1][1]) 
	{
	case 't':				/* -t timeout */
	  if (argv[1][2] != '\0')
	    pc = &argv[1][2];
	  else 
	    {
	      pc = argv[2];
	      argc--, argv++;
	    }
	  (void)sscanf( pc, "%d", &i ) ;
	  wfParams.timeout = i ;
	  /* timeout must be positive. */
	  if( wfParams.timeout <= 0 ) {
	    printErr( "Timeout=%d is illegal.\n",
		      wfParams.timeout ) ;
	    wfParams.timeout = 50 ;
	    printErr( "Use default timeout=%d.\n",
		      wfParams.timeout ) ;
	  }
	  
	  break;
	case 's':			      /* -sx */
	  switch (argv[1][2]) 
	    {
	    default:
	      break ;
	    case 'D':
	    case 'd':
	      stop_dmTask() ;
	      break ;
	    case 'O':
	    case 'o':
	      stop_to_TPTask() ;
	      break ;
	    case 'I':
	    case 'i':
	      stop_to_EPICS_Task() ;
	      break ;
	    case 'X':
	    case 'x':
	      stop_from_TP_Task() ;
	      break ;
	    case '\0':
	      stop_to_TPTask() ;
	      stop_to_EPICS_Task() ;
	      stop_from_TP_Task() ;
	      stop_dmTask() ;
	      break ;
	    }
	  break;
	case 'v':			      /* -vN */
	  i = 0;
	  if (argv[1][2] != '\0')
	    {
	      int	ii ;
	      ii = sscanf( &argv[1][2], "%d", &i ) ;
	    }
	  wfParams.verbose = i;
	  putWFMsgVerbose = wfParams.verbose ;
	  break;
	default:
	  printErr("%s: Don't know %s.\n",
		   "modify", argv[1]);
	BAD:
	  printErr("Usage: %s %s\n", "modify", usage);
	  return(2);
	  break;
	}
      argc--, argv++;
    }
  return OK ;
}

static	char	*assocRec[] = {
  "Analog output",
  "Analog input",
  "Long output",
  "Long input",
  "String output",
  "String input",
  "Wfire command",
} ;

/*
 *+
 * FUNCTION NAME:
 * toTPMsgShow
 *
 * INVOCATION:
 * struct WFMsgMsg pmsg;
 * toTPMsgShow (&pmsg)
 * 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * > struct WFMsgMsg *pmsg - 
 *
 * FUNCTION VALUE:
 * void 
 *
 * PURPOSE:
 * Routine to print things about an WFMsgMsg. 
 *
 * DESCRIPTION:

 *
 * EXTERNAL VARIABLES:
 * None
 *
 * PRIOR REQUIREMENTS:

 *
 * DEFICIENCIES:
 * None
 *
 * HISTORY:
 * June-1997  Original version adapted from CICS alpha 1.0  Jan Schwitters
 *
 *-
 */

    void
toTPMsgShow( struct WFMsgMsg *pmsg )
{
    printf( "%18s \"%s\"\n", "msg.info.string:", pmsg->info.string ) ;
  
    printf( "%18s %u\n", "msg.sequence:", pmsg->sequence ) ;
  
    printf( "%18s \"%s\"\n", "Associated rec:", assocRec[pmsg->info.recId] ) ;
 
    switch( pmsg->info.recId ) {
	case WFMsgAo:

	case WFMsgAi:
	    printf( "%18s %.6f\n\n", "msg.val.dval:", pmsg->val.dval ) ;
	    break ;
	case WFMsgLo:
	case WFMsgLi:
	    printf( "%18s %d\n\n", "msg.val.lval:", pmsg->val.lval ) ;
	    break ;
	case WFMsgSo:
	case WFMsgSi:
	    printf( "%18s \"%s\"\n\n", "msg.val.sval:", pmsg->val.sval ) ;
	    break ;
	default:
	    break ;
	    
    }
 
}
/*
 *+
 * FUNCTION NAME:
 * toEPICSMsgShow
 *
 * INVOCATION:
 * struct WFMsgMsg pmsg;
 * toEPICSMsgShow (&pmsg);
 *
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * > struct WFMsgMsg pmsg -
 *
 * FUNCTION VALUE:

 *
 * PURPOSE:
 * Routine to print things about an WFMsgMsg
 *
 * DESCRIPTION:

 *
 * EXTERNAL VARIABLES:
 * None
 *
 * PRIOR REQUIREMENTS:

 *
 * DEFICIENCIES:
 * None
 *
 * HISTORY:
 * June-1997  Original version adapted from CICS alpha 1.0  Jan Schwitters
 *
 *-
 */

    void
toEPICSMsgShow( struct WFMsgMsg *pmsg )
{
 if( wfParams.verbose ) 
   {
     printf( "***********toEPICSMsgShow():\n" ) ;
   }
    printf( "%18s \"%s\"\n", "Associated rec:", assocRec[pmsg->info.recId] ) ;
    switch( pmsg->info.recId ) {
	case WFMsgAi:
	    printf( "%18s %.1f\n", "msg.val.dval:", pmsg->val.dval ) ;
	    break ;
	case WFMsgLi:
	    printf( "%18s %d\n", "msg.val.lval:", pmsg->val.lval ) ;
	    break ;
	case WFMsgSi:
	    printf( "%18s \"%s\"\n", "msg.val.sval:", pmsg->val.sval ) ;
	    break ;
	default:
	    break ;
    }
}
/*
 *+
 * FUNCTION NAME:
 * fromTPMsgShow
 *
 * INVOCATION:
 * 
 * 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 
 *
 * FUNCTION VALUE:

 *
 * PURPOSE
 *  Routine to print things about an entry in the messages read queue.
 *
 * DESCRIPTION:

 *
 * EXTERNAL VARIABLES:
 * None
 *
 * PRIOR REQUIREMENTS:

 *
 * DEFICIENCIES:
 * None
 *
 * HISTORY:
 * June-1997  Original version adapted from CICS alpha 1.0  Jan Schwitters
 *
 *-
 */

    void
fromTPMsgShow( char *arg )
{
  
    struct WFMsg *pmsg ;
 if( wfParams.verbose ) 
   {
     printf( "***********fromTPMsgShow():\n" ) ;
   }
    pmsg = (struct WFMsg *)arg ;
    lstWFMsg( NULL, pmsg->head, pmsg->array ) ;
}
/*
 *+
 * FUNCTION NAME:
 * fromTPShow
 *
 * INVOCATION:
 * fromTPShow();
 * 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 
 *
 * FUNCTION VALUE:

 *
 * PURPOSE:
 * User defined function ?
 *
 * DESCRIPTION:

 *
 * EXTERNAL VARIABLES:
 * None
 *
 * PRIOR REQUIREMENTS:

 *
 * DEFICIENCIES:
 * None
 *
 * HISTORY:
 * June-1997  Original version adapted from CICS alpha 1.0  Jan Schwitters
 *
 *-
 */
    static void
fromTPShow( char *comment, MSG_Q_INFO *pinfo, void (*queueShow)() )
{
  int	i, n ;
  char	*taskName( int ) ;
  if( wfParams.verbose ) 
    {
      cicsLogMessage(3, "***********fromTPshow():\n" ) ;
    }
  printf( "%s\n", comment ) ;
  
  /* We have several possible cases.  See the code below.  */
  
  if( pinfo->numMsgs==0 && pinfo->numTasks==0 ) 
    {
      cicsLogMessage(3, "\tNo messages, no receiving tasks blocked.\n" ) ;
      return ;
    }
  
  if( pinfo->numMsgs==0 && (n=pinfo->numTasks)>0 ) 
    {
      printf( "\tNo messages, receiving task%s", n>1?"s":"" ) ;
      for( i=0 ; i<n ; i++ ) 
	{
	  printf( " %s%s", taskName( pinfo->taskIdList[i] ),
		  i<(n-1) ? "," : "" ) ;
	}
      cicsLogMessage(3, " blocked.\n" ) ;
      return ;
    }
  
  if( (i=pinfo->numMsgs)==pinfo->maxMsgs && (n=pinfo->numTasks)>0 ) 
    {
      printf( "\t%d messages, sending task%s", i, n>1?"s":"" ) ;
      for( i=0 ; i<n ; i++ ) 
	{
	  printf( " %s%s", taskName( pinfo->taskIdList[i] ),
		  i<(n-1) ? "," : "" ) ;
	}
      cicsLogMessage(3, " blocked.\n" ) ;
      return ;
    }
  
  if( (n=pinfo->numMsgs)>0 ) 
    {
      cicsLogMessage(3, "\tMessages queued:\n" ) ;
      for( i=0 ; i<n ; i++ ) 
	{
	  (*queueShow)( pinfo->msgPtrList[i] ) ;
	}
      return ;
    }
  
}
/*
 *+
 * FUNCTION NAME:
 * toTPShow
 *
 * INVOCATION:
 * toTPShow();
 *
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * none
 *
 * FUNCTION VALUE:

 *
 * PURPOSE:
 * What's in the queue?
 * We show all queues here.
 *
 * DESCRIPTION:

 *
 * EXTERNAL VARIABLES:
 * None
 *
 * PRIOR REQUIREMENTS:

 *
 * DEFICIENCIES:
 * None
 *
 * HISTORY:
 * June-1997  Original version adapted from CICS alpha 1.0  Jan Schwitters
 *
 *-
 */
/* What's in the queue?
 * We show all queues here.
 */
    void
toTPShow()
{
  MSG_Q_INFO	qinfo ;
  int	taskIds[MAXWFMsgMsgs] ;
  char	*msgPtrs[MAXWFMsgMsgs] ;
  if( wfParams.verbose ) 
    {
      cicsLogMessage(3, "***********toTPShow():\n" ) ;
    }
  qinfo.taskIdListMax = MAXWFMsgMsgs ;
  qinfo.taskIdList = taskIds ;
  qinfo.msgListMax = MAXWFMsgMsgs ;
  qinfo.msgPtrList = msgPtrs ;
  qinfo.msgLenList = NULL ;
  
  if( msgQInfoGet( WFMsg_Q_ID_out, &qinfo ) != OK ) 
    {
      printf( "msgQInfoGet( %s ) failed.\n", "WFMsg_Q_ID_out" ) ;
    } 
  else 
    {
      /* Print information. */
      fromTPShow( "outWFireMessage queue:", &qinfo, toTPMsgShow ) ;
    }
  
  /* Now the messages read queues. */
  
  printf( "%18s %d\n", "WFMsg's written:", wfParams.nWritten ) ;
  printf( "%18s %d\n", "WFMsg's read:", wfParams.nRead ) ;
  
  if( msgQInfoGet( wfParams.WFMsg_To_EPICS_Q_ID, &qinfo ) != OK ) 
    {
      printf( "msgQInfoGet( %s ) failed.\n", "WFMsg_To_EPICS_Q_ID" ) ;
    } 
  else 
    {
      fromTPShow( "WFMsg_VAR_READ queue:", &qinfo, fromTPMsgShow ) ;
    }
  if( msgQInfoGet( wfParams.WFMsg_DEBUG_MSG_Q_ID, &qinfo ) != OK ) 
    {
      printf( "msgQInfoGet( %s ) failed.\n", "WFMsg_DEBUG_MSG_Q_ID" ) ;
    } 
  else 
    {
      fromTPShow( "WFMsg_DEBUG_MSG queue:", &qinfo, fromTPMsgShow ) ;
    }
  if( msgQInfoGet( wfParams.WFMsg_SET_VAR_Q_ID, &qinfo ) != OK ) 
    {
      printf( "msgQInfoGet( %s ) failed.\n", "WFMsg_SET_VAR_Q_ID" ) ;
    } 
  else 
    {
      fromTPShow( "WFMsg_SET_VAR queue:", &qinfo, fromTPMsgShow ) ;
    }
#if 0
  if( msgQInfoGet( wfParams.WFMsg_THE_REST_Q_ID, &qinfo ) != OK ) 
    {
      printf( "msgQInfoGet( %s ) failed.\n", "WFMsg_THE_REST_Q_ID" ) ;
    } 
  else 
    {
      fromTPShow( "WFMsg_THE_REST queue:", &qinfo, fromTPMsgShow ) ;
    }
#endif
  /* Now the to EPICS  messages queue. */
  if( msgQInfoGet( WFMsg_Q_ID_in, &qinfo ) != OK ) 
    {
      printf( "msgQInfoGet( %s ) failed.\n", "WFMsg_Q_ID_in" ) ;
    } 
  else 
    {
      
      fromTPShow( "toEPICSWFireMessage queue:", &qinfo, toEPICSMsgShow ) ;
    }
  
}

/*
 *+
 * FUNCTION NAME:
 * put_var
 *
 * INVOCATION:
 * struct WFMsgMsg in;
 * struct WFMsg out;
 * status = put_var (&in,&out);
 *
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * >struct WFMsgMsg *in - 
 * <struct WFMsg *out     - 
 *
 * FUNCTION VALUE:
 * 
 *
 * PURPOSE:
 * Take information in an incoming struct WFMsgMsg and generate an outgoing
 * SET_VAR type struct WFMsg.
 *
 * DESCRIPTION:
 * Information from the EPICS record is placed into a wfire message to be send
 *   to the transputers.
 *
 * EXTERNAL VARIABLES:
 * None
 *
 * PRIOR REQUIREMENTS:

 *
 * DEFICIENCIES:
 * None
 *
 * HISTORY:
 * June-1997  Original version adapted from CICS alpha 1.0  Jan Schwitters
 *
 *-
 */



    static	int
put_var( struct WFMsgMsg *in, struct WFMsg *out )
{


  out->flags = 0 ;
  out->head  = MESSAGE(SET_VAR) | TO_NODE(in->info.node) ;
  out->array[VAR_NUM] = in->info.var ;
   out->array[VAR_NUM] |= in->info.idx << 16 ;   /* Set ARR_INDEX. */

  if (in->info.grp == -1)
    {   /* normal SET_VAR command*/
      out->head |= OF_LENGTH(2) ;
      *((float*)&out->array[VAR_VAL]) = (float)in->val.dval ;
    }
  else
    {   /* SET_VAR for ARRAYD2A*/
      out->head |= OF_LENGTH(3) ;
     /*  out->array[VAR_NUM] |= in->info.idx << 16 ; */   /* Set ARR_INDEX. */
   
      out->array[ISUBVNUM] = in->info.grp ;        /* Set D2A_GROUP. */
      out->array[ISUBVNUM] |= 0 << 16 ;            /* Set D2A_INDEX. */
   
      /* The value to be set. */
      *(float *)&out->array[ISUBVAR_VAL] = (float)in->val.dval;
  
    }
  
  return OK ;
}



    static	int
read_var( struct WFMsgMsg *in, struct WFMsg *out )
{

 
  out->flags = 0 ;
  out->head  = MESSAGE(READ_VAR) | TO_NODE(in->info.node)|FROM_NODE(0) ;
  out->array[VAR_NUM] = in->info.var ;
   out->array[VAR_NUM] |= in->info.idx << 16 ;   /* Set ARR_INDEX. */

  if (in->info.grp == -1)
    {   /* normal READ_VAR command*/
	out->array[FROM] = 0;  /* return node number*/
	out->array[LENGTH_TO_SEND] = 1; /* bytes expected*/
	out->head |= OF_LENGTH(2) ;/*change length to 2 and add fromnode to array*/
      /* *((float*)&out->array[VAR_VAL]) = (float)in->val.dval ; */
    }
/*idx for housekeeping*/
  else
    {   /* READ_VAR for ARRAYD2A*/
      out->head |= OF_LENGTH(3) ;
     /*  out->array[VAR_NUM] |= in->info.idx << 16 ; */   /* Set ARR_INDEX. */
      out->array[ISUBVNUM] = in->info.grp ;        /* Set D2A_GROUP. */
    /*   out->array[ISUBVNUM] |= 0 << 16 ;    */         /* Set D2A_INDEX. */
      out->array[2] = 0;    /* return node number*/
      /* The value to be set. */
     /*  *(float *)&out->array[ISUBVAR_VAL] = (float)in->val.dval; */

    }
  
  return OK ;
}
/*
 *+
 * FUNCTION NAME:
 * put_d2a_var not used
 *
 * INVOCATION:
 * struct cadRecord *pCad;
 * status = ? ( pCad );
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 
 *
 * FUNCTION VALUE:

 *
 * PURPOSE:
 * not used
 *
 * DESCRIPTION:

 *
 * EXTERNAL VARIABLES:
 * None
 *
 * PRIOR REQUIREMENTS:

 *
 * DEFICIENCIES:
 * None
 *
 * HISTORY:
 * June-1997  Original version adapted from CICS alpha 1.0  Jan Schwitters
 *
 *-
 */
#if 0
    static	int
put_d2a_var( struct WFMsgMsg *in, struct WFMsg *out )
{
  out->flags = 0 ;
  if( wfParams.verbose ) 
    {
      cicsLogMessage(3, "**********putd_2a_var():\n" ) ;
    }
  /* Generate header. */
  out->head  = MESSAGE(SET_VAR) | TO_NODE(in->info.node) ;
  out->head |= OF_LENGTH(3) ;
  
  /* Set VAR_NUMVAL. */
  out->array[VAR_NUM] = in->info.var ;
  
  /* Set ARR_INDEX. */
  /* Thu May  1 10:40:27 MST 1997
   * idx is always 0.
   */
  out->array[VAR_NUM] |= in->info.idx << 16 ;
  
  /* Set D2A_GROUP. */
  out->array[ISUBVNUM] = in->info.grp ;
  
  /* Set D2A_INDEX. */
  out->array[ISUBVNUM] |= 0 << 16 ;
  
  /* The value to be set. */
  *(float *)&out->array[ISUBVAR_VAL] = (float)in->val.dval;
  
  return OK ;
}
#endif
/*
 *+
 * FUNCTION NAME:
 * toTPMsgQTask
 *
 * INVOCATION:
 * toTPMsgQTask();
 * 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * none
 *
 * FUNCTION VALUE:
 * void
 *
 * PURPOSE:
 * A task for processing messages from the EPICS message queue.
 * Just something simple to get going.
 *
 * DESCRIPTION:
 * This routine waits for a message on the WFMsg_Q_ID_out queue.  When it gets
 *  one, it processes it and sends it to the transputer.  
 *
 * EXTERNAL VARIABLES:
 * None
 *
 * PRIOR REQUIREMENTS:

 *
 * DEFICIENCIES:
 * None
 *
 * HISTORY:
 * June-1997  Original version adapted from CICS alpha 1.0  Jan Schwitters
 *
 *-
 */
/* There seems to be no way to associate a VAR_READ message with a READ_VAR
 * message except by temporal relationship.  Thus we save a copy of the
 * most recently sent WFMsgMsg and, when we get a VAR_READ message we
 * assume it to be the reply.
 */
static    struct WFMsgMsg	lastWFMsg ;

/* A task for processing messages from the out message queue.
 * Just something simple to get going.
 */
    void
toTPMsgQTask()
{
  
  struct WFMsgMsg	msg ;
  if( wfParams.verbose ) 
    {
      cicsLogMessage(3, "***********toTPMsgQTask():\n" ) ;
    }
  
  
  /* Do some initialization. */
  wfParams.stop &= ~STOP_TO_TP_TASK ;
  
  while( ! ( wfParams.stop & STOP_TO_TP_TASK ) ) 
    {
      
      /* Is there a message on the queue? */
      if( msgQReceive( WFMsg_Q_ID_out, 
		       (char *)&msg, 
		       sizeof( msg ),
		       wfParams.timeout ) != ERROR )
	{
	  struct	WFMsg	outgoingMsg ;
	  
	  /* We got a message. */
	  
	    cicsLogMessage(3, "********toTPMsgQTask() reports message dequeued:\n" ) ;
	   /*  toTPMsgShow( &msg ) ; */
      
	  
	  /* Save the message for possible reference later. */
	  lastWFMsg = msg ;
	 
	  /* What kind of message is it? */
	  if( wfParams.verbose )
	  {
	      printf ("$$$$$$$$$$$$$$record type %d\n",msg.info.recId); 
	      printf ("wfutil %d\n",msg.info.recId); 
	  }
	  switch( msg.info.recId ) 
	    {
	      
	 
	    case WFCmdMsg:
	      {
		switch (msg.info.var)
		  {
		  case KILL_PROC:

		    if( wfParams.verbose ) 
		      DPRINT (wfiredebug,"WFireMessageUtil.c command = Kill_PROC\n"); 
		    outgoingMsg.flags = 0;
		    outgoingMsg.head = MESSAGE(KILL_PROC);
		    outgoingMsg.head |= TO_NODE(msg.info.node);
		    outgoingMsg.head |= OF_LENGTH(0);    
		    break;
		    
		  case EXECUTE_PROC:
		    if( wfParams.verbose ) 
		      DPRINT (wfiredebug,"WFireMessageUtil.c command = EXECUTE_PROC\n"); 
		    outgoingMsg.flags = 0;
		    outgoingMsg.head = MESSAGE(EXECUTE_PROC);
		    outgoingMsg.head |= TO_NODE(msg.info.node);
		    outgoingMsg.head |= OF_LENGTH(0);
		    break;
		  case READ_HK:
		   
		    outgoingMsg.flags = 0;
		    outgoingMsg.head = MESSAGE(READ_HK);
		    outgoingMsg.head |= TO_NODE(msg.info.node);
		    outgoingMsg.head |= OF_LENGTH(0);
		    break;
		    /*add other messages here*/
		    
		  default:
		    printErr( "toTPMsgQTask(): bad message.\n" ) ;
		    toTPMsgShow( &msg ) ;
		   /*  stop_to_TPTask() ; */
		    continue ;
		  }
		/*msg.info.var = WFireMessage number */
		/* outgoingMsg gets message*/
		break;
	      }	
	    case WFMsgAo:	
	      {
		/* Send a SET_VAR message.
		 * SET_VAR messages come in many flavors.  
		 * There is a jump table (indexed by variable number)
		 * down in transputer land which specifies
		 * how the messages are handled.  The tags file in
		 * this directory will send you to setv_cmnds[] so you
		 * can see what the transputer expects.
		 */
		
		/* Generate the WFire message, checking for errors as
		 * we go.
		 */
	
		if( msg.info.var < 0 || put_var( &msg, &outgoingMsg )) 
		  {  
		    printErr( "toTPMsgQTask(): 1 bad request.\n" ) ;
		    toTPMsgShow( &msg ) ;
		   /*  stop_to_TPTask() ; */
		    continue ;
		  }
		
	
		break ; 
	      }
	      
	    case WFMsgAi:
	      /* Send a READ_VAR message.  We expect to get a
	       * VAR_READ message back.
	       * To see how the transputer will respond, start at
	       * command[] which will eventually send you to
	       * readv_cmnds[].
	       * They all send back a VAR_READ with a single data word.
	       */
		read_var(&msg,&outgoingMsg);
	    /*   outgoingMsg.head  = MESSAGE(READ_VAR) ; */
/* 	      outgoingMsg.head |= TO_NODE(msg.info.node) ; */
/* 	      outgoingMsg.head |= OF_LENGTH(2) ; */
/* 	      outgoingMsg.array[VAR_NUM] = msg.info.var ; */
/* 	      outgoingMsg.array[REPLY_TO] = 0 ; */
	      break ;
	      
	    case WFMsgLo:
	      /* Send a SET_VAR message. */
	      outgoingMsg.head  = MESSAGE(SET_VAR) ;
	      outgoingMsg.head |= TO_NODE(msg.info.node) ;
	      outgoingMsg.head |= OF_LENGTH(2) ;
	      outgoingMsg.array[VAR_NUM] = msg.info.var ;
	      outgoingMsg.array[VAR_NUM] |= msg.info.idx << 16 ;
	      *(long *)&outgoingMsg.array[VAR_VAL] = msg.val.lval ;
	      break ;
	      
	    case WFMsgLi:
	      /* Send a READ_VAR message.  We expect to get a
	       * VAR_READ message back.
	       */
	      outgoingMsg.head  = MESSAGE(READ_VAR) ;
	      outgoingMsg.head |= TO_NODE(msg.info.node) ;
	      outgoingMsg.head |= OF_LENGTH(2) ;
	      outgoingMsg.array[VAR_NUM] = msg.info.var ;
	      outgoingMsg.array[VAR_NUM] |= msg.info.idx << 16 ;
	      outgoingMsg.array[REPLY_TO] = 0 ;
	      break ;
	      
	    case WFMsgSo:
	    case WFMsgSi:
	      printErr( "toTPMsgQTask(): cannot handle si/so yet.\n" ) ;
	      toTPMsgShow( &msg ) ;
	     /*  stop_to_TPTask() ; */
	      continue ;
	      
	    default:
	      printErr( "toTPMsgQTask(): 3 bad record id.\n" ) ;
	      toTPMsgShow( &msg ) ;
	     /*  stop_to_TPTask() ; */
	      continue ;
	    }
	  
	  /* Transmit the message. */
	 /*  if((msg.info.node == 10)&&(msg.info.var == 4)&&(msg.val.lval & 4)) */
/* 	      printf("send msg to sequencer %d\n",tickGet()); */
	  if(putWFMsg( wfParams.linkFd, outgoingMsg.head,  outgoingMsg.array ) != ERROR)
	    {
	      wfParams.nWritten++ ;
	      /* Chances are that a reply will be forthcoming. */
	    
	    }
	  else
	    cicsLogMessage(0,"to tp: putwfmsg failed");
	  
	   (void)semGive( wfParams.readLinkSem ) ;
	  
	} 
      else 
		{
		  if( errno != S_objLib_OBJ_TIMEOUT ) 
			{
			  printErr( "In toTPMsgQTask(), msgQReceive() errno= " ) ;
			  printErrno( errno ) ;
			  /* 	perror(); */
			  strerror(errno);
			 
			  cicsLogMessage(3, "toTPMsgQTask() will be stopped.\n" ) ;
		/* 	  stop_to_TPTask() ; */
			  continue ;
			}
		}
    }
  
  /* We get here if stop==TRUE; what shall we do?
   */
  if( wfParams.verbose ) 
    {
      cicsLogMessage(3, " *!*!*!*!*!*!*!*!*!*!*!*!*!*!*!!Exit toTPMsgQTask().\n" ) ;
    }
  
}







/*
 *+
 * FUNCTION NAME:
 * toEPICSMsgQTask
 *
 * INVOCATION:
 * toEPICSMsgQTask();
 * 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * none
 *
 * FUNCTION VALUE:
 * void
 *
 * PURPOSE:
 * A task for processing WFire messages and putting things on the "to EPICS"
 * message queue.
 * Just something simple to get going.
 *
 * DESCRIPTION:

 *
 * EXTERNAL VARIABLES:
 * None
 *
 * PRIOR REQUIREMENTS:

 *
 * DEFICIENCIES:
 * None
 *
 * HISTORY:
 * June-1997  Original version adapted from CICS alpha 1.0  Jan Schwitters
 *
 *-
 */


    void
toEPICSMsgQTask()
{
 
  float f;
  long l;
  struct	WFMsg	msg ;
  struct	WFMsgMsg	toEPICSMsg ;
  if( wfParams.verbose ) 
    {
      cicsLogMessage(3, "***********toEPICSMsgQTask():\n" ) ;
    }
   

  /* Do some initialization. */
  wfParams.stop &= ~STOP_TO_EPICS_TASK ;
  
  while( ! ( wfParams.stop & STOP_TO_EPICS_TASK ) ) 
    {
      
      /* Is there a message on the queue? */
      if( msgQReceive( wfParams.WFMsg_To_EPICS_Q_ID,
		       (char *)&msg, 
		       sizeof( msg ), 
		       wfParams.timeout ) != ERROR ) 
	{
#if 0
	  /* If the returned value is a float: */
	  float	val ;
	  val = *(float *)&msg.array[RETURN_VAL] ;
	  
	  /* If the returned value is a long: */
	  long	val ;
	  val = msg.array[RETURN_VAL] ;
#endif
	  
	  /* We got a message. */
	  if( wfParams.verbose ) 
	    {
	      cicsLogMessage(3, "****************toEPICSMsgQTask() reports message dequeued:\n" ) ;
	      fromTPMsgShow( (char *)&msg ) ;
	    }
	  
	  /* Who requested this message?
	   * Only allow ai and longin for now.
	   */
	  switch( lastWFMsg.info.recId ) 
	    {
	    case WFMsgAi:
	      /* The ai record support routine wants
	       * a double.
	       */
	      f =  *(float *)&msg.array[RETURN_VAL] ;
	     
	       /* 	if( wfParams.verbose )  */

	      toEPICSMsg.val.dval = (double)f ;
	      break ;
	      
	    case WFMsgLi:
	      /* The longin record support routine wants
	       * a long.
	       */
	      l = msg.array[RETURN_VAL] ;
	     
	      toEPICSMsg.val.lval = l ;
	      break ;
	    default:
	      printErr( "toEPICSMsgQTask(): bad request. %d\n",lastWFMsg.info.recId ) ;
	      toTPMsgShow( &lastWFMsg ) ;
	      /* stop_to_EPICS_Task() ; */
	      continue ;
	      
	    }
	  
	  toEPICSMsg.info.recId = lastWFMsg.info.recId ;
	  
	  /* Queue it up for the record support routine. */
	  
/*   DPRINT (wfiredebug,"\n\nsend message to epics 1\n\n"); */
	  if( msgQSend( WFMsg_Q_ID_in, 
			(char *)&toEPICSMsg, 
			sizeof( toEPICSMsg ),
			NO_WAIT, 
			MSG_PRI_NORMAL ) != OK ) 
	    {
	      /* Any error is fatal. */
	      printErr( "In toEPICSMsgQTask(), errno=" ) ;
	      /* printErrno() ends with '\n'. */
	      printErrno( errno ) ;
	      /* stop_to_EPICS_Task() ; */
	      continue ;
	    }
	  
	} 
      else 
	{
	  
	  /* ERROR
	   * How do we know that errno still has the value assigned above?
	   * The VxWorks documentation assures us that it will.
	   */
	  if( errno != S_objLib_OBJ_TIMEOUT ) 
	    {
	      printErr( "In toEPICSMsgQTask(), msgQReceive() errno=" ) ;
	      printErrno( errno ) ;
	      printErr( "toEPICSMsgQTask() will be stopped.\n" ) ;
	     /*  stop_to_EPICS_Task() ; */
	      continue ;
	    }
	}
    }
  
  /* We get here if stop==TRUE; what shall we do?
   */
  if( wfParams.verbose ) 
    {
      cicsLogMessage(3, "Exit toEPICSMsgQTask().\n" ) ;
    }
  
}
/*
 *+
 * FUNCTION NAME:
 * dmMsgQTask
 *
 * INVOCATION:
 * dmMsgQTask();
 * 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * none
 *
 * FUNCTION VALUE:
 * void
 *
 * PURPOSE:
 * A task for processing debug messages.
 * Just print them for now.
 *
 * DESCRIPTION:
 *  
 *
 * EXTERNAL VARIABLES:
 * None
 *
 * PRIOR REQUIREMENTS:
 *
 *
 * DEFICIENCIES:
 * None
 *
 * HISTORY:
 * June-1997  Original version adapted from CICS alpha 1.0  Jan Schwitters
 *
 *-
 */
/* A task for processing debug messages.
 * Just print them for now.
 */
void dmMsgQTask()
{
    /*  long l; */
    /*     double d; */
    char buf1[80];
    
    struct  WFMsgMsg toEPICSMsg;
    struct	WFMsg	msg ;
    if( wfParams.verbose ) 
    {
	cicsLogMessage(3, "***********dmMsgQTask():\n" ) ;
    }
  
    /* Do some initialization. */
    wfParams.stop &= ~STOP_DM_TASK ;
  
    while( ! ( wfParams.stop & STOP_DM_TASK ) ) 
    {
      
	/* Is there a message on the queue? */
	if( msgQReceive( wfParams.WFMsg_DEBUG_MSG_Q_ID, (char *)&msg, 
			 	sizeof( msg ), wfParams.timeout ) != ERROR ) 
	{
	  
	    /* We got a message.  What to do?
	     
	     */
	  
	    /* The input data array has been byte swapped.  It appears that
	     * character strings from the transputer are in "the usual order".
	     * Thus, I think the character data needs to be byte swapped
	     * back to the original order.
	     */
	    int	buf[MAXBUFLEN] ;
	    char *c;
	    int	len = LENGTH(msg.head)*sizeof(int) ;
	  
		cicsLogMessage(3, "********dmMsgQTask() reports message dequeued:\n" ) ;
	   
	    bcopy( (char *)msg.array, (char *)buf, MAXBUFLEN*sizeof(int) ) ;
	    wfm_byte_swap( (char *)buf, len );
	    c= (char *)buf;
	 
	    if (c[0] == '*')
	    {
	     
		sprintf(tmp, "\nDEBUG_MSG: \"%s\"\n", (char *)buf);
		cicsLogMessage(3,tmp);
		DPRINT(1,tmp);
	    }
	    else
	    {
		bcopy( (char *)buf,  (char *)toEPICSMsg.info.string,MSG_SIZE );
		toEPICSMsg.info.string[MSG_SIZE] = 0;
     
	
		toEPICSMsg.info.recId = lastWFMsg.info.recId ;
		sprintf(buf1,"return = %s\n",(char *)buf);
		
		cicsLogMessage(3,buf1);
		
		if(msgQSend(WFMsg_Q_ID_in,
			    (char *)&toEPICSMsg,
			    sizeof(toEPICSMsg),
			    NO_WAIT,MSG_PRI_NORMAL)!= OK)
		{
		    printErr( "msgQSend( WFMsg_Q_ID_in ... )" ) ;
		    printErr( ", errno=" ) ;
		    /* printErrno() ends with '\n'. */
		    printErrno( errno ) ;
		  /*   stop_from_TP_Task() ; */
		    continue ;
		}
	    }
	  
	} 
	else 
	{
	    if( errno != S_objLib_OBJ_TIMEOUT ) 
	    {
		printErr( "In dmMsgQTask(), msgQReceive() errno=" ) ;
		printErrno( errno ) ;
		printErr( "dmMsgQTask() will be stopped.\n" ) ;
	/* 	stop_dmTask() ; */
		continue ;
	    }
	}
    }
  
    /* We get here if stop==TRUE; what shall we do?
     */
   
	cicsLogMessage(3, "Exit toEPICSMsgQTask().\n" ) ;
   

}
/*
 *+
 * FUNCTION NAME:
 * fromTPMsgQTask
 *
 * INVOCATION:
 *  fromTPMsgQTask();
 * 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 *  none
 *
 * FUNCTION VALUE:
 * void
 *
 * PURPOSE:
 * A task for reading WFire messages and saving them in a queue.
 * Just something simple to get going.
 *
 * DESCRIPTION:

 *
 * EXTERNAL VARIABLES:
 * None
 *
 * PRIOR REQUIREMENTS:

 *
 * DEFICIENCIES:
 * None
 *
 * HISTORY:
 * June-1997  Original version adapted from CICS alpha 1.0  Jan Schwitters
 *
 *-
 */

    void
    fromTPMsgQTask()
{
    int count,delay;
    extern	int	ReadLink( int, char *, int, int ) ;
    extern	int	TestRead( int ) ;
    struct	WFMsg	msg ;
    struct  WFMsgMsg toEPICSMsg;
    char bf[100];
    if( wfParams.verbose ) 
    {
	cicsLogMessage(3, "***********fromTPMsgQTask():\n" ) ;
    }
    /*   if( wfParams.verbose ) { */
    /* 	cicsLogMessage(3, "Starting fromTPMsgQTask().\n" ) ; */
/*     } */

    /* Do some initialization. */
    wfParams.stop &= ~STOP_FROM_TP_TASK ;
  
    while( ! ( wfParams.stop & STOP_FROM_TP_TASK ) ) 
    {
    
	/* Wait for semaphore or timeout. */
	(void)semTake( wfParams.readLinkSem, sysClkRateGet()/10 ) ;
  /* 	cicsLogMessage(3,"fromTPMsgQTask: waiting for return message\n");   */
	
	/* Is there a message to read? */
	if( ! TestRead( wfParams.linkFd ) )
	    continue;
 
	/* Read it.??????? */
	getWFMsg( wfParams.linkFd, (int *)&msg.head,  msg.array ) ;
	msg.flags = 0 ;
	wfParams.nRead++ ;
	cicsLogMessage(3,"#############fromTPMsgQTask() got a message:\n" ) ;
	if( wfParams.verbose ) 
	{
	    printf( "#############fromTPMsgQTask() got a message type = %d:\n",MESSAGE(msg.head) ) ;
	    strcpy (bf,(char *)msg.array);
	    /*   wfm_byte_swap(bf,strlen(bf)); */

	    lstWFMsg( NULL, msg.head, msg.array ) ;
	  
	
	}

	/* What kind of message is it?  We have special queues for some
	 * messages, a catchall queue for what's left over.
	 */
      
	switch (MESSAGE(msg.head)) 
	{
	  case VAR_READ:
	      /* Put it on the VAR_READ queue. */
	    
		  cicsLogMessage(3,"FROMTP: VAR_READ\n");
	      if( msgQSend(wfParams.WFMsg_To_EPICS_Q_ID,
			   (char *)&msg, 
			   sizeof( msg ),
			   NO_WAIT, 
			   MSG_PRI_NORMAL ) != OK ) 
	      {
		  /* Any error is fatal. */
		  printErr( "msgQSend( WFMsg_To_EPICS_Q_ID ... )" ) ;
		QSendFailed:
		  printErr( ", errno=" ) ;
		  /* printErrno() ends with '\n'. */
		  printErrno( errno ) ;
		 /*  stop_from_TP_Task() ; */
		  continue ;
	      }
	      break ;
	  
	  case DEBUG_MSG:

	      /* Put it on the DEBUG_MSG queue. */
	  
	       	cicsLogMessage(3,"from TP:  debug Msg\n"); 
	      if( msgQSend( wfParams.
			    WFMsg_DEBUG_MSG_Q_ID,
			    (char *)&msg, 
			    sizeof( msg ),
			    NO_WAIT, 
			    MSG_PRI_NORMAL ) != OK ) 
	      {
		  printErr( "msgQSend( WFMsg_DEBUG_MSG_Q_ID ... )" ) ;
		  goto QSendFailed ;
	      }

	      break ;
	  
	  case SET_VAR:  /* return from read_hk*/
	      /* Put it on the SET_VAR queue. */
	    cicsLogMessage(3,"fromtp: read_hk\n");
	    
	      if (x(msg,count,delay) != OK)
	      {
		  strcpy(toEPICSMsg.info.string, "ERROR reading HK"); 
		  if( msgQSend( WFMsg_Q_ID_in, 
				(char *)&toEPICSMsg, 
				sizeof( toEPICSMsg),  
				NO_WAIT, 
				MSG_PRI_NORMAL)!=OK)
		  {
		      /* Any error is fatal. */
		      printErr( "msgQSend( WFMsg_To_EPICS_Q_ID ... )" ) ;
		      printErr( ", errno=" ) ;
		      /* printErrno() ends with '\n'. */
		      printErrno( errno ) ;
		     /*  stop_from_TP_Task() ; */
		      continue ;
		  }
	      
		  /*  return ERROR; */
	      }
	      strcpy(toEPICSMsg.info.string, "Read HK"); 
	 
	      if( msgQSend( WFMsg_Q_ID_in, 
			    (char *)&toEPICSMsg, 
			    sizeof( toEPICSMsg),  
			    NO_WAIT, 
			    MSG_PRI_NORMAL)!=OK)
	      {
		  /* Any error is fatal. */
		  printErr( "msgQSend( WFMsg_To_EPICS_Q_ID ... )" ) ;
		  printErr( ", errno=" ) ;
		  /* printErrno() ends with '\n'. */
		  printErrno( errno ) ;
		 /*  stop_from_TP_Task() ; */
		  continue ;
	      }
	      /* 	if( msgQSend( wfParams.WFMsg_To_EPICS_Q_ID, */
	      /*  		  (char *)&msg, sizeof( msg ),  */
	      /* 		   NO_WAIT, MSG_PRI_NORMAL ) != OK )  { */
	      /* 	 	    printErr( "msgQSend( WFMsg_SET_VAR_Q_ID ... )" ) ;  */
	      /*  		    goto QSendFailed ;  */
	      /*  		}  */
	      break ;
	  default:
	     
		  cicsLogMessage(3,"FROMTPMsgQTask:  default VAR_READ\n");
	      if( msgQSend( wfParams.WFMsg_To_EPICS_Q_ID,
			    (char *)&msg, 
			    sizeof( msg ),
			    NO_WAIT, 
			    MSG_PRI_NORMAL ) != OK ) 
	      {
		  /* Any error is fatal. */
		  printErr( "msgQSend( WFMsg_To_EPICS_Q_ID ... )" ) ;
		  printErr( ", errno=" ) ;
		  /* printErrno() ends with '\n'. */
		  printErrno( errno ) ;
		  /* stop_from_TP_Task() ; */
		  continue ;
	      }
	 
	      break ;
	}
      
    }
  
    /* We get here if stop==TRUE; what shall we do?
     */
   
	cicsLogMessage(3, "Exit fromTPMsgQTask().\n" ) ;
  
  
}

/*
 *+
 * FUNCTION NAME:
 * x
 *
 * INVOCATION:
 * int count,delay;
 * long status;
 * status = x(count,delay);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 *  > int count -
 *  > int delay - 
 *
 * FUNCTION VALUE:
 * int status = x(count,delay)
 *
 * PURPOSE:
 * Read Housekeeping information from transputers
 *
 * DESCRIPTION:
 * Send a wildfire message to the transputers that asks for the housekeeping 
 *    information.  Wait for the return message and copy the information to
 *    local arrays.
 *
 * EXTERNAL VARIABLES:
 *     extern	double	*HKc[]; 
 *
 * PRIOR REQUIREMENTS:
 *
 * DEFICIENCIES:
 * None
 *
 * HISTORY:
 * June-1997  Original version adapted from CICS alpha 1.0  Jan Schwitters
 *
 */

/* Here is the promised routine x().  delay in seconds. */
int x(struct WFMsg msg, int count, int delay  )
{
    int		i, len;
    int 	group, channel;
    extern	double	*HKc[16]; 
  
    if( wfParams.verbose )
	printf( "ReadHK: Got a SET_VAR message.\n" ) ;
    lstWFMsg( NULL, msg.head, msg.array ) ;
  
    /* Move the data into local arrays. len in words. */
    len = LENGTH(msg.head) ;
    if( len<1 || len>255 ) 
    {
       sprintf(tmp, "Reply to READ_HK request has length %d.\n", len ) ;
       cicsLogMessage(0,tmp);
	return ERROR ;
    }
  
    /* The data in question is actually floats. */
    i=1;
    group = channel = 0;
    while (i<len)
    {
	if (channel == 16)
	{
	    channel = 0;
	    group++;
	}
	HKc[group][channel++] =(double)(* (float*)&msg.array[i]);
	i++;
    }

    return OK ;
}

void y( float x[] )
{
    int	i ;
    for( i=0 ; i<16 ; i++ ) 
    {
	printf( "HK[%d] = %10.5f\n", i, x[i] ) ;
    }
}


void printhk()
{
    int i;
    extern	double *HKc[16]; 

    printf(" 0-15   16-31   32-47   48-63   64-79   80-95   96-111  112-127 128-143 144-159 160-175 176-191 192-207 208-223 224-239 240-255\n");
 
    for (i=0;i<16;i++)
	printf(" %4.1f  %5.1f   %5.1f   %5.1f   %5.1f   %5.1f   %5.1f  " 
	       " %5.1f  %5.1f   %5.1f   %5.1f   %5.1f   %5.1f   %5.1f  "
	       " %5.1f   %5.1f\n",
	       HKc[0][i], HKc[1][i], HKc[2][i], HKc[3][i], HKc[4][i],
	       HKc[5][i], HKc[6][i], HKc[7][i], HKc[8][i], HKc[9][i],
	       HKc[10][i], HKc[11][i], HKc[12][i], HKc[13][i], HKc[14][i],
	       HKc[15][i] );
}

