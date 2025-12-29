static char rcsid[] = "$Id: drvHoms.c,v 1.2 2009/05/27 19:34:43 fkraemer Exp $";

/*========================stepper hmotor driver ========================

 function:
	Allow users to queue messages to axis on a HOMS stepper
	hmotor controller board.  Each axis of every board available can
	be accessed independantly.

 public functions:
	hmotor_init() -	Initialize the driver task and structures for all
			boards available for the system.

	hmotor_send() -	Queue a message to the HOMS task.
	hmotor_task() -	A task that processes messages queued by users.

 private functions:
	query_axis() -	Get position information for axis in motion.
	process_messages() - Process all messages on the queue.
	get_head_node() - Get a message off the queue.
	send_mess() -	Send a message to the HOMS board.
	recv_mess() -	Receive a message from the HOMS board.


========================stepper hmotor driver ========================*/

#include	<vxWorks.h>
#include	<vme.h>
#include	<types.h>
#include	<ctype.h>
#include	<stdioLib.h>
#include	<sysLib.h>
#include	<tickLib.h>
#include	<string.h>
#include	<iv.h>
#include	<taskLib.h>
#include	<wdLib.h>
#include        <rngLib.h>
#include        <rebootLib.h>

#include	<alarm.h>
#include	<dbRecType.h>
#include	<dbDefs.h>
#include	<dbAccess.h>
#include	<dbCommon.h>
#include	<fast_lock.h>
#include	<recSup.h>
#include	<devSup.h>
#include	<drvSup.h>
#include	<dbScan.h>
#include        <devLib.h>
#include        <errMdef.h>
#include	<special.h>
#include	<module_types.h>

#include	"recHmotor.h"
#include	"drvHoms.h"

#define PRIVATE_FUNCTIONS 1	/* normal:1, debug:0 */
#define STATIC static

/* Define for return test on locationProbe() */
#define PROBE_SUCCESS(STATUS) ((STATUS)==S_dev_addressOverlap)

#define CMD_CLEAR       '\030'	/* Control-X, clears command errors only */

#define ALL_INFO        "A? QA RP RE EA\n"	/* jps: move QA to top. */
#define AXIS_INFO       "A? QA RP\n"	/* jps: move QA to top. */
#define GET_IDENT       "WY\n"
#define ERROR_CLEAR     "IC\n"
#define AXIS_STOP       "A? ST\n"
#define STOP_ALL        "AA SA\n"
#define KILL_ALL        "AA KL\n"
#define AXIS_POS        "A? RP\n"
#define ALL_POS         "AA RP\n"
#define ENCODER_POS     "A? RE\n"
#define DONE_QUERY      "A? RA\n"
#define AXIS_QUERY      "A? QA\n"
#define ENCODER_QUERY   "A? EA\n"
#define SET_AXIS(c,a)   c[1]=a;
#define SET_MM_ON(v,a)  v|=(1<<a)
#define SET_MM_OFF(v,a) v&=~(1<<a)

/* status register */
#define STAT_IRQ                0x80
#define STAT_TRANS_BUF_EMPTY    0x40
#define STAT_INPUT_BUF_FULL     0x20
#define STAT_DONE               0x10
#define STAT_OVERTRAVEL         0x08
#define STAT_ENCODER_REQ        0x04
#define STAT_UNUSED             0x02
#define STAT_ERROR              0x01

/* done flag register */
#define DONE_X                  0x01
#define DONE_Y                  0x02
#define DONE_Z                  0x04
#define DONE_T                  0x08
#define DONE_U                  0x10
#define DONE_V                  0x20
#define DONE_R                  0x40
#define DONE_S                  0x80

/* interrupt control register */
#define IRQ_ENABLE              0x80
#define IRQ_TRANS_BUF           0x40
#define IRQ_INPUT_BUF           0x20
#define IRQ_DONE                0x10

/*
 * There was a very serious bug in the motor record.  The input string 
 * buffer was not long enough, so that memory was getting overwritten
 * whenever the string returning the axis position was too large (50
 * characters.)  This has been increased to IBUFSZ (originally 128
 * characters, now increased to HMOTOR_MESS_SIZE [300]),
 * but recv_mesg should really be rewritten to check the input
 * string size, so that it won't clobber memory.
 *                                           -- hty
 */

/* input buffer size */
#define IBUFSZ (HMOTOR_MESS_SIZE)

#define IRQ_ENABLE_ALL         (IRQ_ENABLE|IRQ_DONE|IRQ_INPUT_BUF)

struct vmex_motor
{
    uint8_t unused0;
    uint8_t data;
    uint8_t unused1;
    uint8_t done;
    uint8_t unused2;
    uint8_t control;
    uint8_t unused3;
    uint8_t status;
    uint8_t unused4;
    uint8_t vector;
    uint8_t unused5[6];
};

struct axis_status
{
    char direction;
    char done;
    char overtravel;
    char home;
};

struct encoder_status
{
    char slip_enable;
    char pos_enable;
    char slip_detect;
    char pos_dead;
    char axis_home;
    char unused;
};

struct mess_queue
{
    struct homs_mess_node *head;
    struct homs_mess_node *tail;
};

/*----------------debugging-----------------*/

#ifdef NODEBUG
#define Debug(l,f,v) ;
#else
#define Debug(l,f,v) { if(l<=drvHOMSdebug) printf(f,v); }
#endif

/* added to remove warnings at compile time - lcl */
int  intLock( );
int  intUnlock( );
int  logMsg( );
long locationProbe( );

volatile int drvHOMSdebug = 0;
volatile int homsSpyClkRate = 0;
STATIC volatile int hmotor_scan_rate = HMOTOR_SCAN_RATE;
STATIC volatile unsigned homsInterruptVector = 0;
STATIC volatile uint8_t homsInterruptLevel = HOMS_INT_LEVEL;
STATIC volatile int max_io_tries = HMOTOR_MAX_COUNT;

/* Delay before acknowledging limit switch state - allows test for moving away from
 * a made switch */
STATIC volatile int overtravelTO = 2;  
STATIC volatile int motionTO = 10;  

/*----------------hmotor state info-----------------*/

struct mot_state
{
    int motor_in_motion;	/* count of hmotors in motion */
    char ident[IBUFSZ];		/* identification string for this card */
    int total_axis;		/* total axis on this card */
    char *localaddr;		/* address of this card */

    /* Interrupt Handling control elements */
    int irqErrno;		/* Error indicator from isr */

    uint8_t irqEnable;

    /* message receiving control */
    RING_ID recv_rng;
    SEM_ID recv_sem;

    /* message transmitting control */
    RING_ID send_rng;
    SEM_ID send_sem;

    struct mess_info
    {
	struct homs_mess_node *hmotor_motion;	/* in motion, NULL/node */
	int encoder_present;	/* one YES/NO for each axis */
	int position;		/* one pos for each axis */
	int encoder_position;	/* one pos for each axis */
	int velocity;		/* Raw velocity readback(not implemented) */
	int no_motion_count;
	unsigned long status;	/* one pos for each axis */
    } hmotor_info[HMOTOR_MAX_AXIS];
};
volatile static struct mot_state **hmotor_state;

STATIC int homs_num_cards = 0;
STATIC int homs_num_channels = 0;
STATIC void *homs_addrs = 0x0;

STATIC int total_cards;
STATIC char homs_trans_axis[] = {'X', 'Y', 'Z', 'T', 'U', 'V', 'R', 'S'};
STATIC int any_motor_in_motion;
STATIC struct mess_queue mess_queue;	/* in message queue head */
STATIC struct mess_queue free_list;

STATIC FAST_LOCK hmotor_freelist;
STATIC FAST_LOCK hmotor_queue;
STATIC SEM_ID hmotor_sem;


/*----------------functions-----------------*/
STATIC int recv_mess(int card, char *com, int amount);
STATIC int send_mess(int card, char *com, char c);
STATIC int homsGet(int card, char *pcom, int timeout);
STATIC int homsPut(int card, char *pcom);
STATIC int homsError(int card);
STATIC int hmotorIsrEnable(int card);
STATIC int process_messages();
STATIC int query_axis(int card);
STATIC struct homs_mess_node *get_head_node();
STATIC char *my_strtok(char *p, char *l, char **tok_save);
STATIC struct homs_mess_node *hmotor_malloc();
STATIC int set_status(int card, int signal);
STATIC int hmotor_free(struct homs_mess_node * node);
STATIC int hmotor_send(HOMS_MOTOR_CALL * u_msg);
STATIC int hmotor_card_info(int card, HOMS_MOTOR_CARD_QUERY * cq);
STATIC int hmotor_axis_info(int card, int signal, HOMS_MOTOR_AXIS_QUERY * aq);
STATIC void homs_reset();
static long report(int level);
static long init();
STATIC int hmotor_init();

#if 0
STATIC void hmotorIsrDisable(int card);
#endif

/*----------------functions-----------------*/

struct homs_support homs_access = {
    hmotor_send,
    hmotor_free,
    hmotor_card_info,
    hmotor_axis_info
};

struct
{
    long number;
    DRVSUPFUN report;
    DRVSUPFUN init;
} drvHoms /* drvHOMS */ =
{
    2,
    report,
    init
};

static long report(int level)
{
    printf("no report yet \n");
    return (0);
}

static long init()
{
    (void) hmotor_init();
    return ((long) 0);
}

/*****************************************************/
/* send/receive messages from the stepper controller */
/*		hmotor_task()			     */
/*****************************************************/

/*STATIC hmotor_task(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10)*/
STATUS hmotor_task(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10)
    int a1, a2, a3, a4, a5, a6, a7, a8, a9, a10;
{
    ULONG tick_count, tick_used, tick_curr;
    STATUS sem_ret;
    int wait_time, i;

    tick_used = tickGet();

    while (1)
    {
	tick_curr = tickGet() - tick_used;
	tick_count = (tick_curr > hmotor_scan_rate)
	    ? NO_WAIT : (hmotor_scan_rate - tick_curr);
	wait_time = any_motor_in_motion ? (int) tick_count : WAIT_FOREVER;

	sem_ret = semTake(hmotor_sem, wait_time);

	if (sem_ret == ERROR)
	    tick_used = tickGet();

	if (any_motor_in_motion)
	{

	    /* get 2 pieces of info from axis */
	    for (i = 0; i < homs_num_cards; i++)
	    {
		if (hmotor_state[i] &&
		    hmotor_state[i]->motor_in_motion)
		    query_axis(i);
	    }
	}

	if (sem_ret != ERROR)
	    process_messages();
    }
    return sem_ret;                      /* added - lcl */
}



/*****************************************************/
/* send query to get all axis in motions position */
/*		query_axis()     */
/*****************************************************/
STATIC int query_axis(int card)
{
    volatile struct vmex_motor *pmotor;
    HOMS_MOTOR_RETURN *mess_ret;
    register struct homs_mess_node *hmotor_motion;
    register struct mess_info *hmotor_info;
/*    unsigned char i_done;*/
    int i/*, done_flag*/;
    char q_buf[IBUFSZ];
/*    char o_trav;*/


    /* get the encoder positions for axis in motion */
    pmotor = (struct vmex_motor *) hmotor_state[card]->localaddr;

    for (i = 0; i < hmotor_state[card]->total_axis; i++)
    {
	hmotor_info = &(hmotor_state[card]->hmotor_info[i]);
	hmotor_motion = hmotor_info->hmotor_motion;


	if (hmotor_motion && set_status(card, i))
	{
	    hmotor_motion->position = hmotor_info->position;
	    hmotor_motion->encoder_position = hmotor_info->encoder_position;
	    hmotor_motion->status = hmotor_info->status;
	    hmotor_motion->velocity = hmotor_info->velocity;

	    mess_ret = (HOMS_MOTOR_RETURN *) hmotor_malloc();
	    mess_ret->callback = hmotor_motion->callback;
	    mess_ret->precord = hmotor_motion->precord;
	    mess_ret->position = hmotor_motion->position;
	    mess_ret->encoder_position = hmotor_motion->encoder_position;
	    mess_ret->velocity = hmotor_motion->velocity;
	    mess_ret->status = hmotor_motion->status;
	    mess_ret->type = hmotor_motion->type;

	    if (hmotor_motion->status & HMOTOR_RA_OVERTRAVEL ||
		hmotor_motion->status & HMOTOR_RA_DONE ||
		hmotor_motion->status & HMOTOR_RA_PROBLEM)
	    {
		send_mess(card, DONE_QUERY, homs_trans_axis[i]);
		recv_mess(card, q_buf, 1);

		if (hmotor_motion->status & HMOTOR_RA_PROBLEM)
		  {
		    send_mess(card, AXIS_STOP, homs_trans_axis[i]);		  
		    Debug(1, "motion timeout: status (%s)\n", q_buf);		    
		  }

		hmotor_state[card]->motor_in_motion--;

		hmotor_free(hmotor_motion);
		hmotor_motion = (struct homs_mess_node *) NULL;
		hmotor_info->hmotor_motion = (struct homs_mess_node *) NULL;
	    }

	    callbackRequest((CALLBACK *) mess_ret);

	    if (hmotor_state[card]->motor_in_motion == 0)
	    {
		SET_MM_OFF(any_motor_in_motion, card);
	    }
	}
    }
    return (0);
}


STATIC int set_status(int card, int signal)
{
    /* register struct homs_mess_node *hmotor_motion; */
    register struct mess_info *hmotor_info;
    char *p, *tok_save;
    struct axis_status *ax_stat;
    struct encoder_status *en_stat;
    char q_buf[IBUFSZ];
    int i, pos/*, vel*/;
    int rtn_state;

    hmotor_info = &(hmotor_state[card]->hmotor_info[signal]);

    if (hmotor_state[card]->hmotor_info[signal].encoder_present == YES)
    {
	/* get 4 peices of info from axis */
	send_mess(card, ALL_INFO, homs_trans_axis[signal]);
	recv_mess(card, q_buf, 4);
    }
    else
    {
	send_mess(card, AXIS_INFO, homs_trans_axis[signal]);
	recv_mess(card, q_buf, 2);
    }

    Debug(5, "info = (%s)\n", q_buf);

    for (i = 0, p = my_strtok(q_buf, ",", &tok_save); p;
	 p = my_strtok(NULL, ",", &tok_save), i++)
    {
	switch (i)
	{
	case 0:		/* axis status */
	    ax_stat = (struct axis_status *) p;

	    if (ax_stat->direction == 'P')
		hmotor_info->status |= HMOTOR_RA_DIRECTION;
	    else
		hmotor_info->status &= ~HMOTOR_RA_DIRECTION;

	    if (ax_stat->done == 'D')
	        hmotor_info->status |= HMOTOR_RA_DONE;
	    else
	        hmotor_info->status &= ~HMOTOR_RA_DONE;

	    if (ax_stat->overtravel == 'L')
		hmotor_info->status |= HMOTOR_RA_OVERTRAVEL;
	    else
		hmotor_info->status &= ~HMOTOR_RA_OVERTRAVEL;

	    if (ax_stat->home == 'H')
		hmotor_info->status |= HMOTOR_RA_HOME;
	    else
		hmotor_info->status &= ~HMOTOR_RA_HOME;

	    break;
	case 1:		/* hmotor pulse count (position) */
	    sscanf(p, "%i", &pos);

	    if (pos == hmotor_info->position)
		hmotor_info->no_motion_count++;
	    else
		hmotor_info->no_motion_count = 0;


	    if (hmotor_info->no_motion_count > motionTO)
	    {
		hmotor_info->status |= HMOTOR_RA_PROBLEM;
		hmotor_info->no_motion_count = 0;
	    }
	    else
		hmotor_info->status &= ~HMOTOR_RA_PROBLEM;

	    hmotor_info->position = pos;
	    break;
	case 2:		/* encoder pulse count (position) */
	    sscanf(p, "%i", &hmotor_info->encoder_position);
	    break;
	case 3:		/* encoder status */
	    en_stat = (struct encoder_status *) p;

	    if (en_stat->slip_enable == 'E')
		hmotor_info->status |= HMOTOR_EA_SLIP;
	    else
		hmotor_info->status &= ~HMOTOR_EA_SLIP;

	    if (en_stat->pos_enable == 'E')
		hmotor_info->status |= HMOTOR_EA_POSITION;
	    else
		hmotor_info->status &= ~HMOTOR_EA_POSITION;

	    if (en_stat->slip_detect == 'S')
		hmotor_info->status |= HMOTOR_EA_SLIP_STALL;
	    else
		hmotor_info->status &= ~HMOTOR_EA_SLIP_STALL;

	    if (en_stat->axis_home == 'H')
		hmotor_info->status |= HMOTOR_EA_HOME;
	    else
		hmotor_info->status &= ~HMOTOR_EA_HOME;

	    break;
	default:
	    break;
	}
    }

    /*
     * jps: Velocity should be set based on the actual velocity returned from
     * the 'RV' command (See drvHoms58.c). But the polling task does not have
     * time to request additional information so the velocity is set to
     * indicate moving or not-moving.
     */
    if (hmotor_info->status & HMOTOR_RA_DONE)
	hmotor_info->velocity = 0;
    else
	hmotor_info->velocity = 1;

    /* Check status for old information - usually the first status request 
     * after a motion command reflects the last hmotor state. */
    if (hmotor_info->status & HMOTOR_RA_OVERTRAVEL)
      {
	if (hmotor_info->no_motion_count < overtravelTO &&
	    !(hmotor_info->status & HMOTOR_RA_DONE))
	  {
	    /* Traveling away from limit - turn off indicator */
	    hmotor_info->status &= ~HMOTOR_RA_OVERTRAVEL;

	  }
      }

    if (!(hmotor_info->status & HMOTOR_RA_DIRECTION))
    hmotor_info->velocity *= -1;


    rtn_state = (!hmotor_info->no_motion_count ||
	      (hmotor_info->status & (HMOTOR_RA_OVERTRAVEL | HMOTOR_RA_DONE | HMOTOR_RA_PROBLEM))) ? 1 : 0;

    return (rtn_state);

}


/*****************************************************/
/* Process messages */
/*		process_messages()     */
/*****************************************************/
STATIC int process_messages()
{
    register struct homs_mess_node *hmotor_motion;
    struct homs_mess_node *node;
/*    char qa_buf[15];*/

    while (node = get_head_node())
    {

	Debug(6, "Got message (%s)\n", node->message);

	/* Check that card and signal exists */
	if (hmotor_state[node->card] &&
	    node->signal < hmotor_state[node->card]->total_axis)
	{
	    switch (node->type)
	    {
	    case HOMS_QUERY:
		/* send a message to HOMS */
		strcat(node->message, "\n");
		send_mess(node->card, node->message, (char) NULL);
		recv_mess(node->card, node->message, 1);
		callbackRequest((CALLBACK *) node);
		break;
	    case HOMS_VELOCITY:
		/* send a message to HOMS */
		hmotor_motion =
		    hmotor_state[node->card]->hmotor_info[node->signal].hmotor_motion;

		strcat(node->message, "\n");
		send_mess(node->card, node->message, (char) NULL);

		/*
		 * this is tricky - another motion is here there is a very
		 * large assumption being made here: that the person who sent
		 * the previous motion is the same one that is sending this
		 * one, if he weren't, the guy that sent the original would
		 * never get notified of finish motion.  This makes sense in
		 * record processing since only one record can be assigned to
		 * an axis and sent commands to it. An improvement would be
		 * to check and see if the record pointers were the same, if
		 * they were not, then send a finish message to the previous
		 * registered motion guy.
		 */

		if (!hmotor_motion)	/* if NULL */
		    hmotor_state[node->card]->motor_in_motion++;
		else
		    hmotor_free(hmotor_motion);

		SET_MM_ON(any_motor_in_motion, node->card);
		hmotor_state[node->card]->hmotor_info[node->signal].hmotor_motion = node;
		break;
	    case HOMS_MOTION:
		/* send a message to HOMS */
		hmotor_motion =
		    hmotor_state[node->card]->hmotor_info[node->signal].hmotor_motion;

		strcat(node->message, "\n");
		send_mess(node->card, node->message, (char) NULL);

		/* this is tricky - see velocity comment */
		if (!hmotor_motion)	/* if NULL */
		    hmotor_state[node->card]->motor_in_motion++;
		else
		    hmotor_free(hmotor_motion);

		SET_MM_ON(any_motor_in_motion, node->card);
		hmotor_state[node->card]->hmotor_info[node->signal].no_motion_count = 0;
		hmotor_state[node->card]->hmotor_info[node->signal].hmotor_motion = node;
		break;
	    case HOMS_INFO:
		set_status(node->card, node->signal);
		node->position =
		    hmotor_state[node->card]->hmotor_info[node->signal].position;
		node->encoder_position =
		    hmotor_state[node->card]->hmotor_info[node->signal].encoder_position;
		node->status =
		    hmotor_state[node->card]->hmotor_info[node->signal].status;

/*=============================================================================
* node->status & HMOTOR_RA_DONE is not a reliable indicator of anything, in this case,
* since set_status() didn't ask the HOMS controller to set the "Done" flag on
* command completion.
* Nevertheless, recMotor:process() needs to know whether the hmotor has stopped,
* and this we can tell by looking for a struct hmotor_motion.
==============================================================================*/
		hmotor_motion =
		    hmotor_state[node->card]->hmotor_info[node->signal].hmotor_motion;
		if (hmotor_motion)
		    node->status &= ~HMOTOR_RA_DONE;
		else
		    node->status |= HMOTOR_RA_DONE;

		callbackRequest((CALLBACK *) node);
		break;

	    case HOMS_MOVE_TERM:
		hmotor_motion =
		    hmotor_state[node->card]->hmotor_info[node->signal].hmotor_motion;
		strcat(node->message, hmotor_motion ? " ID\n" : "\n");
		send_mess(node->card, node->message, (char) NULL);
		hmotor_free(node);	/* free message buffer */
		break;
	    default:
		/* send a message to HOMS */
		strcat(node->message, "\n");
		send_mess(node->card, node->message, (char) NULL);
		hmotor_free(node);	/* free message buffer */
		break;
	    }
	}
	else
	{
	    Debug(1, "hmotorTask:process_message() - invalid card #%d\n", node->card);
	    node->position = 0;
	    node->encoder_position = 0;
	    node->velocity = 0;
	    node->status = HMOTOR_RA_PROBLEM;
	    callbackRequest((CALLBACK *) node);
	}

    }
    return (0);
}


/*****************************************************/
/* Get a message off the queue */
/*		get_head_node()			     */
/*****************************************************/
STATIC struct homs_mess_node *get_head_node()
{
    struct homs_mess_node *node, *save_node;

    Debug(7, "get_head_node: entry%c\n", ' ');
    /* get a message from queue */
    FASTLOCK(&hmotor_queue);

    node = mess_queue.head;
    save_node = (struct homs_mess_node *) NULL;
/*    Debug(7, "get_head_node: node = %x\n", node);*/
    Debug(7, "get_head_node: node = %p\n", node);

    /* delete node from list */
    if (node)
    {				/* tmm added */
	if (node == mess_queue.head)
	{
	    Debug(7, "get_head_node: setting mess_queue.head=%p\n", node->next);
	    mess_queue.head = node->next;
	}
	if (node == mess_queue.tail)
	{
	    Debug(7, "get_head_node: setting mess_queue.tail=%p\n", save_node);
	    mess_queue.tail = save_node;
	}
	if (save_node)
	{
	    Debug(7, "get_head_node: setting save_node->next=%p\n", node->next);
	    save_node->next = node->next;
	}
    }
    FASTUNLOCK(&hmotor_queue);

    Debug(7, "get_head_node: returning %p\n", node);
    return (node);
}


/*****************************************************/
/* send a message to the HOMS board		     */
/*		send_mess()			     */
/*****************************************************/
STATIC int send_mess(int card, char *com, char c)
{
/*    char *p;*/
    int return_code/*, i, trys*/;

/*    char buffer[33];*/

    Debug(9, "send_mess: entry.  card %d\n", card);
    /* Check that card exists */
    if (!hmotor_state[card])
    {
	Debug(1, "send_mess - invalid card #%d\n", card);
	return (-1);
    }

    /* Check/Clear command errors */
    homsError(card);

    /* Flush receive buffer */
    recv_mess(card, (char *) NULL, -1);

    if (c != (char) NULL)
	com[1] = c;		/* put in axis */

    Debug(9, "send_mess: ready to send message.%c\n", ' ');

    return_code = homsPut(card, com);

    if (return_code == OK)
    {
	Debug(4, "sent message (%s)\n", com);
    }
    else
    {
	Debug(4, "unable to send message (%s)\n", com);
    }

    return (return_code);
}




/*****************************************************/
/* receive a message to the HOMS board		     */
/*		recv_mess()			     */
/*****************************************************/
STATIC int recv_mess(int card, char *com, int amount)
{
    int i, trys;
    char junk;
/*    char *p;*/
    unsigned char c;
    int piece, head_size, tail_size;
    int return_code;

    return_code = 0;
    c = '\0';

    /* Check that card exists */
    if (card >= total_cards)
    {
	Debug(1, "recv_mess - invalid card #%d\n", card);
	return (-1);
    }

    if (amount == -1)
    {
	/* Process request to flush receive queue */
	Debug(7, "recv flush -------------%c", '-');
	while (homsGet(card, &junk, 0))
	{
	    Debug(7, "%c", junk);
	}
	Debug(7, "         -------------%c", '-');
	return (0);
    }

    for (i = 0; amount > 0; amount--)
    {
	Debug(7, "-------------%c", '-');
	head_size = 0;
	tail_size = 0;

	for (piece = 0, trys = 0; piece < 3 && trys < 3; trys++)
	{
	    if (homsGet(card, (char *)&c, max_io_tries))
	    {
		Debug(7, "%02x", c);

		switch (piece)
		{
		case 0:	/* header */
		    if (c == '\n' || c == '\r')
			head_size++;
		    else
		    {
			piece++;
			com[i++] = c;
		    }
		    break;
		case 1:	/* body */
		    if (c == '\n' || c == '\r')
		    {
			piece++;
			tail_size++;
		    }
		    else
			com[i++] = c;
		    break;

		case 2:	/* trailer */
		    tail_size++;
		    if (tail_size >= head_size)
			piece++;
		    break;

		}

		trys = 0;
	    }
	    else if (homsError(card))
		/* Command error detected - abort recv */
		return (-1);
	}
	Debug(7, "-------------%c\n", '-');
	if (trys >= 3)
	{
	    Debug(1, "Timeout occurred in recv_mess%c\n", ' ');
	    com[i] = '\0';
	    return (-1);
	}
	com[i++] = ',';
    }

    if (i > 0)
	com[i - 1] = '\0';
    else
	com[i] = '\0';

    Debug(4, "recv_mess: card %d", card);
    Debug(4, " com %s\n", com);

    return (0);
}


/*****************************************************/
/* Get next character from HOMS input buffer          */
/*		homsGet()			     */
/*****************************************************/
STATIC int homsGet(int card, char *pchar, int timeout)
{
    volatile struct mot_state *pmotorState;
    volatile struct vmex_motor *pmotor;
    int getCnt = 0;
    int retry = 0;

    pmotorState = hmotor_state[card];

    if (pmotorState->irqEnable)
    {
	/* Get character from isr - if available */
	while ((getCnt = rngBufGet(pmotorState->recv_rng, pchar, 1)) == 0 &&
	       retry < timeout)
	{
	    semTake(pmotorState->recv_sem, 1);	/* Wait for character */
	    retry += 5000;	/* Compensate for semaphore timeout */
	}
    }
    else
    {
	/* Direct read from card */
	pmotor = (struct vmex_motor *) pmotorState->localaddr;

	while (getCnt == 0 && retry++ < timeout)
	{
	    if (pmotor->status & STAT_INPUT_BUF_FULL)
	    {
		getCnt++;
		*pchar = pmotor->data;
	    }
	}
    }

    return (getCnt);
}

/*****************************************************/
/* Send Message to HOMS                               */
/*		homsPut()			     */
/*****************************************************/
static int homsPut(int card, char *pmess)
{
    volatile struct mot_state *pmotorState;
    volatile struct vmex_motor *pmotor;
/*    uint8_t status;*/
    int key;
    char *p;
    int putCnt;
    int trys;

    pmotorState = hmotor_state[card];
    pmotor = (struct vmex_motor *) pmotorState->localaddr;

    /*
     * This section enables the transmitt interrupt - it is not used because
     * of driver-failure during worst-case testing.
     */
    if (FALSE)
    {
	/* Put string into isr transmitt buffer */
	putCnt = strlen(pmess);
	if (rngBufPut(pmotorState->send_rng, pmess, putCnt) != putCnt)
	{
	    Debug(1, "homsPut: Put ring full.%c\n", ' ');
	    return (ERROR);
	}

	/* Turn-on transmit buffer interrupt */
	key = intLock();
	pmotor->control |= IRQ_TRANS_BUF;
	intUnlock(key);

	return (semTake(pmotorState->send_sem, HMOTOR_MAX_COUNT / 5000));
    }
    else
    {
/*	int i;*/

	/* Send next message */
	for (p = pmess; *p != '\0'; p++)
	{
	    trys = 0;
	    while (!(pmotor->status & STAT_TRANS_BUF_EMPTY))
	    {
		if (trys > max_io_tries)
		{
		    Debug(1, "homsPut: Time_out occurred in send%c\n", ' ');
		    return (ERROR);
		}
		if (pmotor->status & STAT_ERROR)
		{
		    Debug(1, "homsPut: error occurred in send%c\n", ' ');
		}
		trys++;
	    }
	    pmotor->data = *p;
	}
    }

    return (OK);
}


/*****************************************************/
/* Clear HOMS errors                                  */
/*		homsClearErrors()	             */
/*****************************************************/
STATIC int homsError(int card)
{
    volatile struct mot_state *pmotorState;
    volatile struct vmex_motor *pmotor;
    int rtnStat = FALSE;

    pmotorState = hmotor_state[card];
    pmotor = (struct vmex_motor *) pmotorState->localaddr;

    if (pmotorState->irqEnable)
    {
	/* Check status of last message */
	if (pmotorState->irqErrno & STAT_ERROR)
	{
	    /* Error on the card is cleared by the ISR */
	    pmotorState->irqErrno &= ~STAT_ERROR;
	    rtnStat = TRUE;
	}
    }
    else
    {
	int i;
	char *p;

	/* Check/Clear command error from last message */
	if ((pmotor->status) & STAT_ERROR)
	{
	    Debug(1, "homsPut: Error detected! 0x%02x\n", pmotor->status);
	    for (p = ERROR_CLEAR; *p != '\0'; p++)
	    {
		while (!(pmotor->status & STAT_TRANS_BUF_EMPTY));
		pmotor->data = *p;
	    }
	    for (i = 0; i < 20000; i++);
	    rtnStat = TRUE;
	}
    }

    return (rtnStat);
}


/*****************************************************/
/* Interrupt service routine.                        */
/* hmotorIsr()		                     */
/*****************************************************/
void hmotorIsr(unsigned card)
{
    volatile struct mot_state *pmotorState;
    volatile struct vmex_motor *pmotor;
    uint8_t control;
    uint8_t status;
    uint8_t doneFlags;
    uint8_t dataChar;


    if (card >= total_cards || (pmotorState = hmotor_state[card]) == NULL)
    {
	logMsg("Invalid entry-card #%d\n", card, 0, 0, 0, 0, 0);
	return;
    }

    pmotor = (struct vmex_motor *) (pmotorState->localaddr);


    /* Save interrupt state */
    control = pmotor->control;

    /* Status register - clear irqs on read. */
    status = pmotor->status;

    /* Done register - clears on read */
    doneFlags = pmotor->done;

    /* Determine cause of entry */

    if (drvHOMSdebug >= 10)
	logMsg("entry card #%d,status=0x%X,done=0x%X\n", card, status, doneFlags, 0, 0, 0);


    /* Motion done handling */
    if (status & STAT_DONE)
	/* Wake up polling task 'hmotor_task()' to issue callbacks */
	semGive(hmotor_sem);


    /* If command error is present - clear it */
    if (status & STAT_ERROR)
    {
	pmotor->data = (uint8_t) CMD_CLEAR;

	/* Send null character to indicate error */
	if (drvHOMSdebug >= 1)
	    logMsg("command error detected on card %d\n", card, 0, 0, 0, 0, 0);

	pmotorState->irqErrno |= STAT_ERROR;

    }

    /* Send message */
/*    if (status & STAT_TRANS_BUF_EMPTY) */
    if (FALSE)
    {
	if (rngBufGet(pmotorState->send_rng, (char *)&dataChar, 1))
	    pmotor->data = dataChar;
	else
	{
	    /* Transmit done - disable irq */
	    semGive(pmotorState->send_sem);
	    control &= ~IRQ_TRANS_BUF;
	}
    }

    /* Read Response */
    if (status & STAT_INPUT_BUF_FULL)
    {

	dataChar = pmotor->data;

	if (!rngBufPut(pmotorState->recv_rng, (char *)&dataChar, 1))
	{
	    logMsg("card %d recv ring full, lost '%c'\n", card, dataChar, 0, 0, 0, 0);
	    pmotorState->irqErrno |= STAT_INPUT_BUF_FULL;
	}

	semGive(pmotorState->recv_sem);

    }

    /* Update-interrupt state */
    pmotor->control = control;
}

STATIC int hmotorIsrEnable(int card)
{
    volatile struct mot_state *pmotorState;
    volatile struct vmex_motor *pmotor;
    long status;
    uint8_t cardStatus;

    Debug(5, "hmotorIsrEnable: Entry card#%d\n", card);

    pmotorState = hmotor_state[card];
    pmotor = (struct vmex_motor *) (pmotorState->localaddr);

    status = devConnectInterrupt(HOMS_INTERRUPT_TYPE,
				 homsInterruptVector + card,
				 &hmotorIsr,
				 (void *)card);
/*
				 (void *) &hmotorIsr,
*/
    if (!RTN_SUCCESS(status))
    {
	errPrintf(status, __FILE__, __LINE__, "Can't connect to vector %d\n",
		  homsInterruptVector + card);
	pmotorState->irqEnable = FALSE;	/* Interrupts disable on card */
	pmotor->control = 0;
	return (ERROR);
    }
    printf( "drvHoms::hmotorIsrEnable: interrupt connected\n" );

    status = devEnableInterruptLevel(HOMS_INTERRUPT_TYPE,
				     homsInterruptLevel);
    if (!RTN_SUCCESS(status))
    {
	errPrintf(status, __FILE__, __LINE__,
		  "Can't enable enterrupt level %d\n",
		  homsInterruptLevel);
	pmotorState->irqEnable = FALSE;	/* Interrupts disable on card */
	pmotor->control = 0;
	return (ERROR);
    }
    printf( "drvHoms::hmotorIsrEnable: interrupt level set\n" );

    /* Setup card for interrupt-on-done */
    pmotor->vector = homsInterruptVector + card;

    pmotorState->recv_rng = rngCreate(HOMS_RESP_Q_SZ);
    pmotorState->recv_sem = semBCreate(SEM_Q_PRIORITY, SEM_EMPTY);

    pmotorState->send_rng = rngCreate(HMOTOR_MESS_SIZE * 2);
    pmotorState->send_sem = semBCreate(SEM_Q_PRIORITY, SEM_EMPTY);

    pmotorState->irqEnable = TRUE;
    pmotorState->irqErrno = 0;

    /* Clear board status */
    cardStatus = pmotor->status;

    /* enable interrupt-when-done and input-buffer-full interrupts */
    pmotor->control = IRQ_ENABLE_ALL;

    return (OK);
}

#if 0
STATIC void hmotorIsrDisable(int card)
{
    volatile struct mot_state *pmotorState;
    volatile struct vmex_motor *pmotor;
    long status;

    Debug(5, "hmotorIsrDisable: Entry card#%d\n", card);

    pmotorState = hmotor_state[card];
    pmotor = (struct vmex_motor *) (pmotorState->localaddr);

    /* Disable interrupts */
    pmotor->control = 0;

    status = devDisconnectInterrupt(HOMS_INTERRUPT_TYPE,
				    homsInterruptVector + card,
				    &hmotorIsr);
/*				    (void *) &hmotorIsr);*/
    if (!RTN_SUCCESS(status))
	errPrintf(status, __FILE__, __LINE__, "Can't disconnect vector %d\n",
		  homsInterruptVector + card);


    /* Remove interrupt control functions */
    pmotorState->irqEnable = FALSE;
    pmotorState->irqErrno = 0;
    rngDelete(pmotorState->recv_rng);
    rngDelete(pmotorState->send_rng);

    semDelete(pmotorState->recv_sem);
    semDelete(pmotorState->send_sem);

}
#endif


/*****************************************************/
/* Configuration function for  module_types data     */
/* areas. homsSetup()                                */
/*****************************************************/
void homsSetup(int num_cards,		/* maximum number of cards in rack */
	 int num_channels,	/* Channels per card (4 or 8) */
	 void *addrs,		/* Base Address(0x0-0xb000 on 4K boundary) */
	 unsigned vector,	/* noninterrupting(0), valid vectors(64-255) */
	 int int_level,		/* interrupt level (1-6) */
	 int scan_rate)		/* polling rate - 1/60 sec units */
{

    if (num_cards < 1 || num_cards > HOMS_NUM_CARDS)
	homs_num_cards = HOMS_NUM_CARDS;
    else
	homs_num_cards = num_cards;

    if (num_channels < 1 || num_channels > HOMS_NUM_CHANNELS)
	homs_num_channels = HOMS_NUM_CHANNELS;
    else
	homs_num_channels = num_channels;

    /* Check boundary(16byte) on base address */
    if ((uint32_t) addrs & 0xF)
    {
	Debug(1, "homsSetup: invalid base address 0x%p\n", addrs);
	homs_addrs = (void *) HOMS_NUM_ADDRS;
    }
    else
	homs_addrs = addrs;


    homsInterruptVector = vector;
    if (vector < 64 || vector > 255)
    {
	if (vector != 0)
	{
	    Debug(1, "homsSetup: invalid interrupt vector %d\n", vector);
	    homsInterruptVector = (unsigned) HOMS_INT_VECTOR;
	}
    }

    if (int_level < 1 || int_level > 6)
    {
	Debug(1, "homsSetup: invalid interrupt level %d\n", int_level);
	homsInterruptLevel = HOMS_INT_LEVEL;
    }
    else
	homsInterruptLevel = int_level;

    /* Set hmotor polling task rate */
    if (scan_rate >= 1 && scan_rate <= sysClkRateGet())
	hmotor_scan_rate = sysClkRateGet() / scan_rate;
    else
	hmotor_scan_rate = HMOTOR_SCAN_RATE;
}

/*****************************************************/
/* initialize all software and hardware		     */
/*		hmotor_init()			     */
/*****************************************************/

/* Modified 18 Aug 1997 (lcl@roe.ac.uk)
   Added recv_mess calls to intercept replies to send_mess.
   It is not clear why anything should be received after some of these
   messages, but the insertions get the system working.  Without them the
   expected messages come back at the wrong times.
*/

STATIC int hmotor_init()
{
    volatile struct vmex_motor *pmotor;

    long status;
    int i, j;
    char axis_pos[IBUFSZ];
    char encoder_pos[IBUFSZ];
    char *tok_save;
    char *pos_ptr;
    int total_encoders = 0;
    int total_axis = 0;
/*
    unsigned long * localaddr;
    unsigned long * probeAddr;
*/
    void * localaddr;
    void * probeAddr;

    char buffer[IBUFSZ];

    tok_save = NULL;

    /* Check for setup */
    if (homs_num_cards <= 0)
      {
	Debug(1, "hmotor_init: *HOMS driver disabled* \n homsSetup() is missing from startup script.%c\n",' ');
	return(ERROR);
      }
  
    /* allocate space for total number of hmotors */
    hmotor_state = (volatile struct mot_state **)malloc(homs_num_cards *
				  sizeof(struct mot_state *));

    /* allocate structure space for each hmotor present */

    total_cards = 0;

    if (rebootHookAdd((FUNCPTR) homs_reset) == ERROR)
      Debug(1,"vme8/44 hmotor_init: homs_reset disabled%c\n",' ');

    for (i = 0; i < homs_num_cards; i++)
    {
	Debug(2, "hmotor_init: card %d\n", i);

	probeAddr = (char *)homs_addrs + (i * HOMS_BRD_SIZE);
/*	probeAddr = (unsigned long *)&homs_addrs + (i * HOMS_BRD_SIZE);*/
	Debug(9, "hmotor_init: locationProbe() on addr 0x%p\n", probeAddr);
	status = locationProbe(HOMS_ADDRS_TYPE, probeAddr);
	if (PROBE_SUCCESS(status))
	{
	    status = devRegisterAddress(__FILE__,
					HOMS_ADDRS_TYPE,
					probeAddr,
					HOMS_BRD_SIZE,
					&localaddr);
	    Debug(9, "hmotor_init: devRegisterAddress() status = %d\n", status);
	    if (!RTN_SUCCESS(status))
	    {
		errPrintf(status, __FILE__, __LINE__,
			  "Can't register address 0x%x\n", probeAddr);
		return (ERROR);
	    }

	    Debug(9, "hmotor_init: localaddr = %p\n", localaddr);
	    pmotor = (struct vmex_motor *) localaddr;

	    total_cards++;

	    Debug(9, "hmotor_init: malloc'ing hmotor_state%c\n", ' ');
	    hmotor_state[i] = (struct mot_state *) malloc(
						  sizeof(struct mot_state));
	    hmotor_state[i]->localaddr = localaddr;
	    hmotor_state[i]->motor_in_motion = 0;

	    /* Disable Interrupts */
	    hmotor_state[i]->irqEnable = FALSE;
	    pmotor->control = 0;

	    recv_mess(i, buffer, -1);            /* added - unnecessary? */
#if 0 /* DISABLED -- hty */
	    send_mess(i, "EF\n", (char) NULL);
	    recv_mess(i, buffer, 1);             /* added - lcl */
#endif

            printf( "drvHoms::hmotor_init: (echo off) message returned = %s\n", buffer );

            buffer[0] = 0;                       /* added - lcl */
	    send_mess(i, ERROR_CLEAR, (char) NULL);
	    recv_mess(i, buffer, 1);             /* added - lcl */
/*
            printf( "drvHoms::hmotor_init: (error clear) message returned = %s\n", buffer );
*/
            buffer[0] = 0;                       /* added - lcl */
	    recv_mess(i, buffer, -1);            /* redundant? */   
	    send_mess(i, STOP_ALL, (char) NULL);
	    recv_mess(i, buffer, 1);             /* added - lcl */
            buffer[0] = 0;                       /* added - lcl */

	    recv_mess(i, buffer, -1);            /* added - lcl */
	    send_mess(i, GET_IDENT, (char) NULL);
	    recv_mess(i, hmotor_state[i]->ident, 1);
	    Debug(3, "Identification = %s\n", hmotor_state[i]->ident);
/*
            printf( "drvHoms::hmotor_init: (get id) message returned = %s\n",
                    hmotor_state[i]->ident );
*/
	    send_mess(i, ALL_POS, (char) NULL);
	    recv_mess(i, axis_pos, 1);
/*
            printf( "drvHoms::hmotor_init: (all pos) message returned = %s\n", axis_pos );
*/
	    for (total_axis = 0, pos_ptr = my_strtok(axis_pos, ",", &tok_save);
		 pos_ptr;
		 pos_ptr = my_strtok(NULL, ",", &tok_save), total_axis++)
	    {
		hmotor_state[i]->hmotor_info[total_axis].hmotor_motion = NULL;
		hmotor_state[i]->hmotor_info[total_axis].status = 0;
	    }

	    Debug(3, "Total axis = %d\n", total_axis);
	    hmotor_state[i]->total_axis = total_axis;

	    /* Enable interrupt-when-done if selected - driver depends on 
	     * hmotor_state->total_axis  being set. */
	    if (homsInterruptVector)
	    {
		if (hmotorIsrEnable(i) == ERROR)
		    errPrintf(0, __FILE__, __LINE__, "Interrupts Disabled!\n");
	    }


	    for (total_encoders = 0, j = 0; j < total_axis; j++)
	    {
		send_mess(i, ENCODER_QUERY, homs_trans_axis[j]);
		if (recv_mess(i, encoder_pos, 1) == -1)
		{
		    /* Command error - no encoder */
		    Debug(2, "No encoder on %d\n", j);
		    hmotor_state[i]->hmotor_info[j].encoder_present = NO;
		}
		else
		{
		    total_encoders++;
		    hmotor_state[i]->hmotor_info[j].encoder_present = YES;
		}
	    }

	    for (j = 0; j < total_axis; j++)
	    {
		hmotor_state[i]->hmotor_info[j].status = 0;
		hmotor_state[i]->hmotor_info[j].no_motion_count = 0;
		hmotor_state[i]->hmotor_info[j].encoder_position = 0;
		hmotor_state[i]->hmotor_info[j].position = 0;

		if (hmotor_state[i]->hmotor_info[j].encoder_present == YES)
		    hmotor_state[i]->hmotor_info[j].status |= HMOTOR_EA_PRESENT;
		set_status(i, j);
	    }

	    Debug(2, "Init Address=0x%p\n", localaddr);
	    Debug(3, "Total encoders = %d\n\n", total_encoders);
	}
	else
	{
	    Debug(3, "hmotor_init: Card NOT found!%c\n", ' ');
	    hmotor_state[i] = (struct mot_state *) NULL;
	}
    }

    hmotor_sem = semBCreate(SEM_Q_PRIORITY, SEM_EMPTY);
    any_motor_in_motion = 0;

    FASTLOCKINIT(&hmotor_queue);
    FASTLOCK(&hmotor_queue);
    mess_queue.head = (struct homs_mess_node *) NULL;
    mess_queue.tail = (struct homs_mess_node *) NULL;
    FASTUNLOCK(&hmotor_queue);

    FASTLOCKINIT(&hmotor_freelist);
    FASTLOCK(&hmotor_freelist);
    free_list.head = (struct homs_mess_node *) NULL;
    free_list.tail = (struct homs_mess_node *) NULL;
    FASTUNLOCK(&hmotor_freelist);

    Debug(3, "Motors initialized%c\n", ' ');

    taskSpawn("tmotor", 64, VX_FP_TASK | VX_STDIO, 5000, hmotor_task,
	      0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

    Debug(3, "Started hmotor_task%c\n", ' ');

    return (0);
}





/*****************************************************/
/* send a command to the stepper hmotor task queue    */
/*		hmotor_send()			     */
/*****************************************************/
STATIC int hmotor_send(HOMS_MOTOR_CALL * u_msg)
{
    struct homs_mess_node *new_message;

    Debug(9, "hmotor_send invoked, message = (%s)\n", u_msg->message);
/*    printf( "drvHoms::hmotor_send: message = (%s)\n", u_msg->message );*/

    new_message = hmotor_malloc();
    new_message->callback = u_msg->callback;
    new_message->next = (struct homs_mess_node *) NULL;
    new_message->type = u_msg->type;
    new_message->signal = u_msg->signal;
    new_message->card = u_msg->card;
    new_message->precord = u_msg->precord;
    new_message->status = 0;
    strcpy(new_message->message, u_msg->message);

    switch (new_message->type)
    {
    case HOMS_MOTION:
	strcat(new_message->message, " ID");
	break;
    case HOMS_VELOCITY:
	break;
    case HOMS_MOVE_TERM:
	break;
    case HOMS_IMMEDIATE:
	break;
    case HOMS_QUERY:
    case HOMS_INFO:
	break;
    default:
	return (-1);
	break;
    }

    FASTLOCK(&hmotor_queue);

    if (mess_queue.tail)
    {
	mess_queue.tail->next = new_message;
	mess_queue.tail = new_message;
    }
    else
    {
	mess_queue.tail = new_message;
	mess_queue.head = new_message;
    }

    FASTUNLOCK(&hmotor_queue);

    semGive(hmotor_sem);

    return (0);
}

STATIC char *my_strtok(char *p, char *l, char **tok_save)
{
    char *tok_return;

    if (p)
	*tok_save = p;
    else
	p = *tok_save;

    if (!p)
	return (NULL);

    while (*p && !(index(l, *p)))
	p++;

    if (!(*p))
    {
	tok_return = *tok_save;
	*tok_save = NULL;
	return (tok_return);
    }
    else
	*p = '\0';

    tok_return = *tok_save;
    *tok_save = p + 1;
    return (tok_return);
}

STATIC struct homs_mess_node *hmotor_malloc()
{
    struct homs_mess_node *node;

    FASTLOCK(&hmotor_freelist);

    if (!free_list.head)
	node = (struct homs_mess_node *) malloc(sizeof(struct homs_mess_node));
    else
    {
	node = free_list.head;
	free_list.head = node->next;
	if (!free_list.head)
	    free_list.tail = (struct homs_mess_node *) NULL;
    }

    FASTUNLOCK(&hmotor_freelist);
    return (node);
}

STATIC int hmotor_free(struct homs_mess_node * node)
{
    FASTLOCK(&hmotor_freelist);

    node->next = (struct homs_mess_node *) NULL;

    if (free_list.tail)
    {
	free_list.tail->next = node;
	free_list.tail = node;
    }
    else
    {
	free_list.head = node;
	free_list.tail = node;
    }

    FASTUNLOCK(&hmotor_freelist);
    return (0);
}

/*---------------------------------------------------------------------*/
/* both of these routines are only to be used at initialization time -
   before queuing transactions starts for an axis.  These routines do no
   locking, since transactions will not enter the queue for an axis
   until after they have run. */

/* return the card information to caller */
STATIC int hmotor_card_info(int card, HOMS_MOTOR_CARD_QUERY * cq)
{
    if (card < homs_num_cards && hmotor_state[card])
    {
	cq->total_axis = hmotor_state[card]->total_axis;
	cq->card_name = hmotor_state[card]->ident;
	cq->axis_names = homs_trans_axis;
    }
    else
	cq->total_axis = 0;

    return (0);
}

/* return information for an axis to the caller */
STATIC int hmotor_axis_info(int card, int signal, HOMS_MOTOR_AXIS_QUERY * aq)
{
    if (card < homs_num_cards && hmotor_state[card] &&
	signal < hmotor_state[card]->total_axis)
    {
	aq->position = hmotor_state[card]->hmotor_info[signal].position;
	aq->encoder_position =
	    hmotor_state[card]->hmotor_info[signal].encoder_position;
	aq->status = hmotor_state[card]->hmotor_info[signal].status;
    }
    else
    {
	aq->position = aq->encoder_position = 0;
	aq->status = HMOTOR_RA_PROBLEM;
    }

    return (0);
}

/*
 *
 *  Disables interrupts. Called on CTL X reboot.
 *
 */
 
STATIC void homs_reset()
{
	short 		      card;        
	struct vmex_motor     *pmotor;
	short                   status;

 	for (card = 0; card < total_cards; card++,pmotor++)
	  {
	    pmotor = (struct vmex_motor *)hmotor_state[card]->localaddr;
	    if(vxMemProbe((char *)pmotor,READ,sizeof(short),(char *)&status) == OK)
	      pmotor->control &= 0x5f;
	  } 
}


/*---------------------------------------------------------------------*/
