static char rcsid[] = "$Id: devHmotorHoms.c,v 1.2 2009/05/27 19:34:41 fkraemer Exp $";

/* Device Support Routines for motor */
/*
 *      Original Author: Jim Kowalkowski
 *      Date: 01/18/93
 *
 *      Experimental Physics and Industrial Control System (EPICS)
 *
 *      Copyright 1991, the Regents of the University of California,
 *      and the University of Chicago Board of Governors.
 *
 *      This software was produced under  U.S. Government contracts:
 *      (W-7405-ENG-36) at the Los Alamos National Laboratory,
 *      and (W-31-109-ENG-38) at Argonne National Laboratory.
 *
 *      Initial development by:
 *	      The Controls and Automation Group (AT-8)
 *	      Ground Test Accelerator
 *	      Accelerator Technology Division
 *	      Los Alamos National Laboratory
 *
 *      Co-developed with
 *	      The Controls and Computing Group
 *	      Accelerator Systems Division
 *	      Advanced Photon Source
 *	      Argonne National Laboratory
 *
 * Modification Log:
 * -----------------
 * .01  01-18-93	jbk     initialized
 *      ...
 */

#include	<vxWorks.h>
#include	<vme.h>
#include	<types.h>
#include	<stdioLib.h>
#include	<stdlib.h>
#include	<string.h>
#include	<math.h>
#include        <semLib.h>	/* jps: include for init_record wait */
/* #include	<68k/iv.h> tmm: move to EPICS 3.11*/
#include	<memLib.h>
#include	<iv.h>

#include	<alarm.h>
#include	<callback.h>
#include	<dbRecType.h>
#include	<dbDefs.h>
#include	<dbAccess.h>
#include	<dbCommon.h>
#include	<fast_lock.h>
#include	<recSup.h>
#include	<devSup.h>
#include	<drvSup.h>
#include	<dbScan.h>
#include	<special.h>
#include	<module_types.h>
#include	<eventRecord.h>
#include	<hmotorRecord.h>
#include	<choiceHmotor.h>

#include	"recHmotor.h"
#include	"drvHoms.h"

#define PRIVATE_FUNCTIONS 0	/* normal:1, debug:0 */
#define STATIC static

#ifdef NODEBUG
#define Debug(L,FMT,V) ;
#else
#define Debug(L,FMT,V) {  if(L <= devHOMSdebug) \
			{ printf("%s(%d):",__FILE__,__LINE__); \
			  printf(FMT,V); } }
#endif

#define NINT(f)	(long)((f)>0 ? (f)+0.5 : (f)-0.5)	/* tmm */

/* ----------------Create the dsets for devHOMS----------------- */
/*static long report();*/
STATIC long homs_init(int after);
STATIC long homs_init_record(struct hmotorRecord * mr);
/*STATIC long get_ioint_info();*/
STATIC long homs_start_trans();
STATIC long homs_update_values();
STATIC long homs_build_trans();
STATIC long homs_end_trans();
STATIC void homs_callback();
STATIC void homs_init_callback();


typedef struct
{
    long number;
    DEVSUPFUN report;
    DEVSUPFUN init;
    DEVSUPFUN init_record;
    DEVSUPFUN get_ioint_info;
    DEVSUPFUN update_values;
    DEVSUPFUN start_trans;
    DEVSUPFUN build_trans;
    DEVSUPFUN end_trans;
} HOMSDSET;

HOMSDSET devHmotorHoms = {8,
    NULL,
    homs_init,
    homs_init_record,
    NULL,
    homs_update_values,
    homs_start_trans,
    homs_build_trans,
    homs_end_trans
};

/* xxDSET devXxxx={ 5,report,init,init_rec,get_ioint_info,read_write}; */

/* --------------------------- program data --------------------- */

volatile int devHOMSdebug = 0;

static struct hmotor_table homs_table[] = {
/*	  type      cmd  num_parms */
    {HOMS_MOTION, " MA", 1},		/* MOVE_ABS */
    {HOMS_MOTION, " MR", 1},		/* MOVE_REL */
    {HOMS_MOTION, " HM", 1},		/* HOME_FOR */
    {HOMS_MOTION, " HR", 1},		/* HOME_REV */
    {HOMS_MOTION, " HE", 1},		/* HOME_ENC */
    {HOMS_IMMEDIATE, " LP", 1},	/* LOAD_POS */
    {HOMS_IMMEDIATE, " VB", 1},	/* SET_VELO_BASE */
    {HOMS_IMMEDIATE, " VL", 1},	/* SET_VELO */
    {HOMS_IMMEDIATE, " AC", 1},	/* SET_ACCEL */
    {HOMS_IMMEDIATE, " GD", 0},	/* GO */
    {HOMS_IMMEDIATE, " ER", 2},	/* SET_ENC_RATIO */
    {HOMS_INFO, " ", 0},		/* GET_MOTOR_POS */
    {HOMS_INFO, " ", 0},		/* GET_ENCODER_POS */
    {HOMS_INFO, " ", 0},		/* GET_INFO */
    {HOMS_MOVE_TERM, " ST", 0},	/* STOP_AXIS */
    {HOMS_VELOCITY, " JG", 1},	/* JOG */
    {HOMS_IMMEDIATE, " AF", 0},	/* POWER_ON */
    {HOMS_IMMEDIATE, " AN", 0},	/* POWER_OFF */
};

/* status of an axis of a board, one present for each axis of each board*/
struct a_axis
{
    char name;
    int encoder_present;
    int in_use;
};

/* the device private data structure of the record */
struct homs_trans
{
    int state;
    FAST_LOCK lock;
    HOMS_MOTOR_CALL motor_call;
    int callback_changed;
    int motor_pos;
    int encoder_pos;
    int vel;
    unsigned long status;
    SEM_ID initSem;
};

/* the board status structure */
struct homs_stat
{
    int exists;
    char *name;
    int total_axis;
    struct a_axis *oms_axis;
};

STATIC struct homs_stat *homs_cards;
extern struct homs_support homs_access;

#define GO_COMM " GD"

/* (tmm) Old steppermotor record sent "AF" before moving and "AN" after moving
#define AUX_ON " AN"
#define AUX_OFF " AF"
*/
#define AUX_ON " AF"
#define AUX_OFF " AN"

#define IDLE_STATE 0
#define BUILD_STATE 1
#define YES 1
#define NO 0

#define SEM_TIMEOUT  50
#define ADDRESS "A?"		/* tmm */
/* jps: get axis name info from driver access routine */

/* --------------------------- program data --------------------- */

/* initialize device support for HOMS stepper motor */
STATIC long homs_init(int after)
{
    HOMS_MOTOR_CARD_QUERY card_query;
    int i, j;

    Debug(1, "homs_init: entry..%c\n", '.');
    if (after)
	return (0);

    /* allocate space for total cards in system */
    Debug(5, "homs_init: calling malloc(HOMS_NUM_CARDS=%d)\n", HOMS_NUM_CARDS);
    homs_cards = (struct homs_stat *) malloc(HOMS_NUM_CARDS * sizeof(struct homs_stat));

    /* for each card ask driver for num of axis supported and name */
    for (i = 0; i < HOMS_NUM_CARDS; i++)
    {
	(*homs_access.get_card_info) (i, &card_query);
	if (card_query.total_axis == 0)
	    homs_cards[i].exists = NO;
	else
	{
	    homs_cards[i].exists = YES;
	    homs_cards[i].name = card_query.card_name;
	    homs_cards[i].total_axis = card_query.total_axis;
	    Debug(5, "homs_init: calling malloc(card_query...)\n total axis =%d\n",
		  homs_cards[i].total_axis);
	    homs_cards[i].oms_axis = (struct a_axis *) malloc(
			     card_query.total_axis * sizeof(struct a_axis));

	    for (j = 0; j < card_query.total_axis; j++)
	    {
		homs_cards[i].oms_axis[j].in_use = 0;
		homs_cards[i].oms_axis[j].name = card_query.axis_names[j];
	    }

	}
    }

    Debug(1, "homs_init: exit..%c\n", '.');
    return (0);
}

/* initialize a record instance */
STATIC long homs_init_record(struct hmotorRecord * mr)
{
    int card, signal;
    struct homs_trans *ptrans;
    HOMS_MOTOR_AXIS_QUERY axis_query;
    HOMS_MOTOR_CALL *motor_call;
    double ep_mp[2];		/* encoder pulses, motor pulses */
    int rtnStat;

    Debug(1, "homs_init_record: entry..%c\n", '.');

    /* allocate space for private field - an homs_trans structure */
    Debug(5, "homs_init_record: calling malloc(...homs_trans...)%c\n", ' ');

    mr->dpvt = (struct homs_trans *) malloc(sizeof(struct homs_trans));
    ptrans = mr->dpvt;
    ptrans->state = IDLE_STATE;
    ptrans->callback_changed = NO;
    FASTLOCKINIT(&ptrans->lock);
    motor_call = &(ptrans->motor_call);

    callbackSetCallback(homs_callback, &(motor_call->callback));
    callbackSetPriority(priorityMedium, &(motor_call->callback));

    /* out must be an VME_IO */
    switch (mr->out.type)
    {
    case (VME_IO):
	break;
    default:
	recGblRecordError(S_dev_badBus, (void *) mr,
			  "devHOMS (init_record) Illegal OUT Bus Type");
	return (S_dev_badBus);
    }

    card = mr->out.value.vmeio.card;
    Debug(5, "homs_init_record: card %d\n", card);
    signal = mr->out.value.vmeio.signal;
    Debug(5, "homs_init_record: signal %d\n", signal);

    rtnStat = 0;

    if (card >= HOMS_NUM_CARDS || homs_cards[card].exists == NO)
    {
	recGblRecordError(S_db_badField, (void *) mr,
			  "devHOMS (init_record) card does not exist!");
	rtnStat = S_db_badField;
    }
    else if (signal >= homs_cards[card].total_axis)
    {
	recGblRecordError(S_db_badField, (void *) mr,
			  "devHOMS (init_record) signal does not exist!");
	rtnStat = S_db_badField;
    }
    else if (homs_cards[card].oms_axis[signal].in_use == YES)
    {
	recGblRecordError(S_db_badField, (void *) mr,
			  "devHOMS (init_record) motor already in use!");
	rtnStat = S_db_badField;
    }

    if (rtnStat)
    {
	mr->msta = HMOTOR_RA_PROBLEM;
	mr->res = mr->mres;
	mr->rmp = 0;		/* raw motor pulse count */
	mr->rep = 0;		/* raw encoder pulse count */
	return (rtnStat);
    }
    else
	/* query motor for all info to fill into record */
	(*homs_access.get_axis_info) (card, signal, &axis_query);

    mr->msta = axis_query.status;	/* status info */

    homs_cards[card].oms_axis[signal].in_use = YES;


/*jps* setting the encoder ratio was moved from recMotor.c/init_record() to
       stop callbacks during iocInit */
    /*
     * Set the encoder ratio.  Note this is blatantly device dependent.
     * Determine the number of encoder pulses and the number of motor pulses
     * per engineering unit (EGU).  Send an array containing this information
     * to device support.
     */
    if ((mr->msta & HMOTOR_EA_PRESENT) && mr->ueip)
    {
	if (fabs(mr->mres) < 1.e-9)
	    mr->mres = 1.;
	if (fabs(mr->eres) < 1.e-9)
	    mr->eres = mr->mres;
	ep_mp[0] = 1. / mr->eres;	/* encoder pulses per EGU */
	ep_mp[1] = 1. / mr->mres;	/* motor pulses per EGU */
	mr->res = mr->eres;
    }
    else
    {
	ep_mp[0] = 1.;
	ep_mp[1] = 1.;
	mr->res = mr->mres;
    }


    /* Program the device if an encoder is present */
    if (mr->msta & HMOTOR_EA_PRESENT)
    {

	Debug(7, "homs_init_record: encoder pulses per EGU = %f\n", ep_mp[0]);
	Debug(7, "homs_init_record: motor pulses per EGU = %f\n", ep_mp[1]);

	/*
	 * Semaphore used to hold initialization until device is programmed -
	 * cleared by callback
	 */
	ptrans->initSem = semBCreate(SEM_Q_FIFO, SEM_EMPTY);

	/*
	 * Switch to special init callback so that record will not be
	 * processed during iocInit
	 */
	callbackSetCallback(homs_init_callback, &(motor_call->callback));

	homs_start_trans(mr);
	homs_build_trans(HMOTOR_SET_ENC_RATIO, ep_mp, mr);
	homs_end_trans(mr, 0);

	/* Changing encoder ratio may have changed the readback value. */
	homs_start_trans(mr);
	homs_build_trans(HMOTOR_GET_INFO, NULL, mr);
	homs_end_trans(mr, 0);

	/* Wait for callback w/timeout */
	if ((rtnStat = semTake(ptrans->initSem, SEM_TIMEOUT)) == ERROR)
	{
	    recGblRecordError(S_dev_NoInit, (void *) mr,
			      "dev_NoInit (init_record: callback2 timeout");
	}
	semDelete(ptrans->initSem);

	/* Restore regular record callback */
	callbackSetCallback(homs_callback, &(motor_call->callback));

    }


    /* query motor for all info to fill into record */
    (*homs_access.get_axis_info) (card, signal, &axis_query);

    mr->rmp = axis_query.position;	/* raw motor pulse count */
    mr->rep = axis_query.encoder_position;	/* raw encoder pulse count */
    mr->msta = axis_query.status;	/* status info */
    Debug(7, "homs_init_record: rmp = %d\n", mr->rmp);
    Debug(7, "homs_init_record: rep = %d\n", mr->rep);
    Debug(7, "homs_init_record: msta = %d\n", mr->msta);

    Debug(1, "homs_init_record: exit..%c\n", '.');
    return (0);
}


#if PRIVATE_FUNCTIONS
STATIC long homs_update_values(struct hmotorRecord * mr)
#else
long homs_update_values(struct hmotorRecord * mr)
#endif
{
    struct homs_trans *ptrans;
    long rc;

    rc = HMOTOR_NOTHING_DONE;
    ptrans = (struct homs_trans *) mr->dpvt;

    FASTLOCK(&ptrans->lock);

    /* raw motor pulse count */
    if (ptrans->callback_changed == YES)
    {
	mr->rmp = ptrans->motor_pos;
	mr->rep = ptrans->encoder_pos;
	mr->rvel = ptrans->vel;
	mr->msta = ptrans->status;
	ptrans->callback_changed = NO;
	rc = HMOTOR_CALLBACK_DATA;
    }

    FASTUNLOCK(&ptrans->lock);

    return (rc);
}


/* start building a transaction */
#if PRIVATE_FUNCTIONS
STATIC long homs_start_trans(struct hmotorRecord * mr)
#else
long homs_start_trans(struct hmotorRecord * mr)
#endif
{
    int card = mr->out.value.vmeio.card;
    int axis = mr->out.value.vmeio.signal;
    struct homs_trans *trans = (struct homs_trans *) mr->dpvt;
    HOMS_MOTOR_CALL *motor_call;

    if (!mr->dpvt)
	return (S_dev_NoInit);

    motor_call = &(trans->motor_call);

    /*
     * initialize area to device private field to store command that is to be
     * built and mark as command build in progress
     */

    trans->state = BUILD_STATE;

    motor_call->card = card;
    motor_call->signal = axis;
    motor_call->type = HOMS_UNDEFINED;
    motor_call->precord = (struct dbCommon *) mr;
    motor_call->message[0] = (char) NULL;

    /* motor address in command */
    strcat(motor_call->message, ADDRESS);

    /* Check for card/signal before building command - allow callback 
     * to continue if no card/signal for simulation mode */
    if (homs_cards[card].exists == YES &&
	axis < homs_cards[card].total_axis)
      motor_call->message[1] = homs_cards[card].oms_axis[axis].name;
    else
      motor_call->message[1] = '*';

    /* put aux power-on in command */
	if (mr->pwr == HMOTOR_PWR_AUTO)
    	strcat(motor_call->message, AUX_ON);

    return (0);
}

/* end building a transaction */
#if PRIVATE_FUNCTIONS
STATIC long homs_end_trans(struct hmotorRecord * mr, int term_method)
#else
long homs_end_trans(struct hmotorRecord * mr, int term_method)
#endif
{
    struct homs_trans *trans = (struct homs_trans *) mr->dpvt;
    HOMS_MOTOR_CALL *motor_call;
    long rc;
/*
    int card = mr->out.value.vmeio.card;
    int axis = mr->out.value.vmeio.signal;
*/

    rc = 0;
    motor_call = &(trans->motor_call);

    switch (trans->state)
    {
    case BUILD_STATE:
	/* shut off command build in process thing */
	trans->state = IDLE_STATE;

	/* (tmm added) put aux power-off in command */
	if (mr->pwr == HMOTOR_PWR_AUTO)
	    strcat(motor_call->message, AUX_OFF);

	if (term_method)
	    strcat(motor_call->message, GO_COMM);

	rc = (*homs_access.send) (motor_call);
	break;

    case IDLE_STATE: break;

    }

    return (rc);
}

/* add a part to the transaction */
#if PRIVATE_FUNCTIONS
STATIC long homs_build_trans(int command, double *parms, struct hmotorRecord * mr)
#else
long homs_build_trans(int command, double *parms, struct hmotorRecord * mr)
#endif
{
    struct homs_trans *trans = (struct homs_trans *) mr->dpvt;
    HOMS_MOTOR_CALL *motor_call;
    char buffer[20];
    int first_one, i;

    motor_call = &(trans->motor_call);

    if (homs_table[command].type > motor_call->type)
	motor_call->type = homs_table[command].type;

    /* concatenate onto the dpvt message field */
    switch (trans->state)
    {
    case BUILD_STATE:
	/* put in command */
	strcat(motor_call->message, homs_table[command].command);

	/* put in parameters */
	for (first_one = YES, i = 0; i < homs_table[command].num_parms; i++)
	{
	    if (first_one == YES)
		first_one = NO;
	    else
		strcat(motor_call->message, ",");

	    sprintf(buffer, "%ld", NINT(parms[i]));
	    strcat(motor_call->message, buffer);
	}
	break;

    case IDLE_STATE:
    default:
	return ERROR;
    }

    return (0);
}

/* callback from HOMS driver for motor in motion */
#if PRIVATE_FUNCTIONS
STATIC void homs_callback(HOMS_MOTOR_RETURN * motor_return)
#else
void homs_callback(HOMS_MOTOR_RETURN * motor_return)
#endif
{
    struct hmotorRecord *mr = (struct hmotorRecord *) motor_return->precord;
    struct rset *prset = (struct rset *) (motor_return->precord->rset);
    struct homs_trans *ptrans;

    Debug(2, "homs_callback: entry%c\n", ' ');
    ptrans = (struct homs_trans *) mr->dpvt;

    Debug(6, "homs_callback: FASTLOCK%c\n", ' ');
    FASTLOCK(&ptrans->lock);

    /* raw motor pulse count */
    Debug(6, "homs_callback: update ptrans%c\n", ' ');
    ptrans->callback_changed = YES;
    ptrans->motor_pos = motor_return->position;
    /* raw encoder pulse count */
    ptrans->encoder_pos = motor_return->encoder_position;

    /* raw encoder pulse count */
    ptrans->vel = motor_return->velocity;

    ptrans->status = motor_return->status;	/* status */

    Debug(6, "homs_callback: FASTUNLOCK%c\n", ' ');
    FASTUNLOCK(&ptrans->lock);

    /* free the return data buffer */
    Debug(6, "homs_callback: free HMOTOR_RETURN%c\n", ' ');
    (*homs_access.free) (motor_return);

    Debug(6, "homs_callback: dbScanLock%c\n", ' ');
    dbScanLock((struct dbCommon *) mr);
    Debug(6, "homs_callback: calling process%c\n", ' ');
    (*prset->process) ((struct dbCommon *) mr);
    Debug(6, "homs_callback: dbScanUnlock%c\n", ' ');
    dbScanUnlock((struct dbCommon *) mr);
    Debug(2, "homs_callback: exit..%c\n", '.');
    return;
}


/* callback from HOMS driver for init_record  - duplicate of
 * the homs_callback without the call to process the record */
#if PRIVATE_FUNCTIONS
STATIC void homs_init_callback(HOMS_MOTOR_RETURN * motor_return)
#else
void homs_init_callback(HOMS_MOTOR_RETURN * motor_return)
#endif
{
    struct hmotorRecord *mr = (struct hmotorRecord *) motor_return->precord;
/*    struct rset *prset = (struct rset *) (motor_return->precord->rset);*/
    struct homs_trans *ptrans;

    Debug(2, "homs_init_callback: entry%c\n", ' ');
    ptrans = (struct homs_trans *) mr->dpvt;

    Debug(6, "homs_init_callback: FASTLOCK%c\n", ' ');
    FASTLOCK(&ptrans->lock);

    /* raw motor pulse count */
    Debug(6, "homs_init_callback: update ptrans%c\n", ' ');
    ptrans->callback_changed = YES;
    ptrans->motor_pos = motor_return->position;
    /* raw encoder pulse count */
    ptrans->encoder_pos = motor_return->encoder_position;
    /* raw encoder pulse count */
    ptrans->vel = motor_return->velocity;

    ptrans->status = motor_return->status;	/* status */

    /* free the return data buffer */
    Debug(6, "homs_init_callback: free HMOTOR_RETURN%c\n", ' ');
    (*homs_access.free) (motor_return);

    /* Continue record init. */
    Debug(6, "homs_init_callback: Continue sig.%c\n", ' ');
    semGive(ptrans->initSem);

    Debug(6, "homs_init_callback: FASTUNLOCK%c\n", ' ');
    FASTUNLOCK(&ptrans->lock);

    Debug(2, "homs_init_callback: exit%c\n", ' ');
    return;
}






















