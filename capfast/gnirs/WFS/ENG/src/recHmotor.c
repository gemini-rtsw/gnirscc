static char rcsid[] = "$Id: recHmotor.c,v 1.2 2009/05/27 19:34:45 fkraemer Exp $";

/* recMotor.c - Record Support Routines for Motor records */

/*
 *		Original Author: Jim Kowalkowski
 *		Current Author:	Tim Mooney
 *		Date: 06/14/95
 *
 *	Experimental Physics and Industrial Control System (EPICS)
 *
 *	Copyright 1995, the University of Chicago Board of Governors.
 *
 *	This software was produced under U.S. Government contract:
 *	(W-31-109-ENG-38) at Argonne National Laboratory.
 *
 *	Developed by
 *		The Beamline Controls and Data Acquisition Group
 *		Experimental Facilities Division
 *		Advanced Photon Source
 *		Argonne National Laboratory
 *
 *	Co-developed with
 *		The Controls and Computing Group
 *		Accelerator Systems Division
 *		Advanced Photon Source
 *		Argonne National Laboratory
 *
 *
 * Modification Log:
 * -----------------
 * .01  01-18-93 jbk  initial development
 * .02  01-25-93 tmm  initial development continued
 * .03  09-13-93 tmm  disabled HOPR & LOPR with #define USE_HOPR_LOPR 0
 * .04  08-01-94 tmm  move to 3.11.6 (tsLocalTime -> recGblGetTimeStamp)
 * .05  09-07-94 tmm  make .diff zero when we set drive fields to readback
 *                    fields
 * .06  09-07-94 tmm  test for stop and spmg every time record processes
 * .07  11-15-94 jps  create .rvel to indicate motor actually moving
 * .08  06-12-95 tmm  added VERSION and .vers field, fixed stuck .dmov flag
 * .09  06-14-95 tmm  propagate dial-value calibration to user value;
 *                    fix precision and units assoc. with .vers field
 * .10  07-20-95 tmm  added record name to debug output
 * .11  07-20-95 tmm  check that we've actually moved before worrying whether
 *                    we're going in the wrong direction
 * .12  08-23-95 tmm  version 1.5  removed HOPR/LOPR code, added dial dir
 *                    (sign of .mres).  Fixed coding error in jog*.  No
 *                    longer propagate dial-value calib. to user value
 * .13  08-25-95 tmm  version 1.51  limit switches now reported w.r.t user
 *                    coordinates.  Added raw limit switches .rhls, .rlls.
 * .14  08-28-95 tmm  version 1.6  Added .card field, so users can know which
 *                    card in crate is associated with this motor.
 * .15  08-30-95 tmm  version 1.7  Added .nmap field to supplement .mmap, and
 *                    rewrote MARK() macros.  
 * .16  08-31-95 tmm  version 1.8  Freeze-offset switch (.foff) replaces earlier
 *                    "propagate dial-value calibration to user value"
 * .17  09-01-95 tmm  limit-violation changes.
 * .18  09-07-95 tmm  version 1.9  Added .fof / .vof fields to put .foff (freeze
 *                    offset) into frozen / variable mode.
 *                    Added .urev, .srev, .s, .sbas, .sbak fields to meet user
 *                    demands for more intuitive setting of speeds and
 *                    resolution.
 *                    Modified offset handling.  Changing .off should never
 *                    cause the motor to move.
 * .19  09-08-95 tmm  version 1.91  Work around 8-character limit in handling
 *                    by dbGet() of the string returned by get_units().
 * .20  09-13-95 tmm  version 1.92  Allow .rdbl (readback inlink) and .rlnk
 *                    (readout outlink) to be channel-access links.
 * .21  09-26-95 tmm  version 1.93  Post process (as if STOP had been hit)
 *                    when a limit switch is hit.  When post process sets,
 *                    e.g., VAL=RBV, then also set LVAL=RBV.
 * .22  11-21-95 tmm  Version 1.94  Fix post_MARKed_fields.  Fix pmr->pp bug
 *                    that caused post processing every time.
 * .23  12-27-95 tmm  Version 1.95  Implemented MISS field.  More sanity checks
 *                    on resolution, speed, and acceleration.
 * .24  02-09-96 tmm  Version 1.96  Don't db_post dmov==0 if special() already
 *                    posted it.
 * .25  03-19-96 tmm  v1.97  Much more precise encoder-ratio calculation.
 *                    Made actual home speed independent of UEIP flag.
 *                    (Thanks to Vicky Austin, Royal Greenwich Observatory.)
 * .26  06-07-96 tmm  v1.98 Added more debugging output.
 * .27  06-10-96 tmm  v2.00 Fixed the way changes to resolutions and speeds are
 *                    handled in init_record() and in special().  Now, when
 *                    motor resolution is changed, speeds in revs/sec are held
 *                    constant, which requires speeds in EGUs/sec to change.
 */
#define VERSION 2.0

#include	<vxWorks.h>
#include	<types.h>
#include	<stdioLib.h>
#include	<lstLib.h>
#include	<string.h>

#include	<alarm.h>
#include	<cvtTable.h>
#include	<dbDefs.h>
#include	<dbAccess.h>
#include	<dbScan.h>
#include	<dbFldTypes.h>
#include	<devSup.h>
#include	<errMdef.h>
#include	<recSup.h>
#include	<special.h>
#include	<dbEvent.h>

#include	<math.h>
#include	<hmotorRecord.h>
#include	<choiceHmotor.h>
#include	"recHmotor.h"


/* nearest integer */
#define NINT(f)	(long)((f)>0 ? (f)+0.5 : (f)-0.5)

/*** Local options within this code ***/

/*
 * If motor fails to get to target position, record tries again, incrementing
 * .rcnt.  If .rcnt get incremented to .rtry (max allowed retries), then
 * record either quits or puts motor into the Pause state, depending on the
 * following compile-time option.
 */
#define PAUSE_ON_MAX_RETRY 0

/*** Debugging variables, macros ***/

#ifdef NODEBUG
#define Debug(l,FMT,V) ;
#else
#define Debug(l,FMT,V) {  if(l <= recHmotordebug) \
			{ printf("%s:%s(%d):",pmr->name,__FILE__,__LINE__); \
			  printf(FMT,V); } }
#endif

volatile int    recHmotordebug = 0;


/*** Forward references ***/

static long     do_work( struct hmotorRecord * pmr );
static void     alarm(struct hmotorRecord * pmr);
static void     monitor(struct hmotorRecord * pmr);
static void     post_MARKed_fields(struct hmotorRecord * pmr, unsigned short mask);
static void     process_motor_info(struct hmotorRecord * pmr);
static void     load_pos(struct hmotorRecord * pmr);
static void		check_speed_and_resolution(struct hmotorRecord * pmr);

/*** Record Support Entry Table (RSET) functions. ***/

#define report NULL
#define initialize NULL
static long     init_record(struct hmotorRecord * pmr, int pass);
static long     process(struct hmotorRecord * pmr);
static long     special(struct dbAddr * paddr, int after);
static long     get_value(struct hmotorRecord * pmr, struct valueDes * pvdes);
#define cvt_dbaddr NULL
#define get_array_info NULL
#define put_array_info NULL
static long     get_units(struct dbAddr * paddr, char *units);
#define get_enum_str NULL
#define get_enum_strs NULL
#define put_enum_str NULL
static long     get_precision(struct dbAddr * paddr, long *precision);
static long     get_graphic_double(struct dbAddr * paddr, struct dbr_grDouble * pgd);
static long     get_control_double(struct dbAddr * p, struct dbr_ctrlDouble * pcd);
static long     get_alarm_double(struct dbAddr * paddr, struct dbr_alDouble * pad);

/* record support entry table */
struct rset     hmotorRSET = {
	RSETNUMBER,
	report,
	initialize,
	init_record,
	process,
	special,
	get_value,
	cvt_dbaddr,
	get_array_info,
	put_array_info,
	get_units,
	get_precision,
	get_enum_str,
	get_enum_strs,
	put_enum_str,
	get_graphic_double,
	get_control_double,
	get_alarm_double
};

/* device support entry table */
struct hmotor_dset {
	long            number;
	DEVSUPFUN       dev_report;
	DEVSUPFUN       init;
	DEVSUPFUN       init_record;
	DEVSUPFUN       get_ioint_info;
	DEVSUPFUN       update_values;
	DEVSUPFUN       start_trans;
	DEVSUPFUN       build_trans;
	DEVSUPFUN       end_trans;
};


/*******************************************************************************
Support for tracking the progress of motor from one invocation of 'process()'
to the next.  The field 'pmr->mip' stores the motion in progress using these
fields.  ('pmr' is a pointer to struct hmotorRecord.)
*******************************************************************************/
#define MIP_DONE	0x0000	/* No motion is in progress.                 */
#define MIP_JOGF	0x0001	/* A jog-forward command is in progress.     */
#define MIP_JOGR	0x0002	/* A jog-reverse command is in progress.     */
#define MIP_JOG_BL	0x0004	/* Done jogging; now take out backlash.      */
#define MIP_JOG		(MIP_JOGF | MIP_JOGR | MIP_JOG_BL)
#define MIP_HOMF	0x0008	/* A home-forward command is in progress.    */
#define MIP_HOMR	0x0010	/* A home-forward command is in progress.    */
#define MIP_HOME	(MIP_HOMF | MIP_HOMR)
#define MIP_MOVE	0x0020	/* A move not resulting from Jog* or Hom*.   */
#define MIP_RETRY	0x0040	/* A retry is in progress.                   */
#define MIP_LOAD_P	0x0080	/* A load-position command is in progress.   */
#define MIP_LOAD_ER	0x0100	/* A load-encoder-ratio command in progress. */
#define MIP_STOP	0x0200	/* We're trying to stop.  When combined with */
/*                                 MIP_JOG* or MIP_HOM*, the jog or home     */
/*                                 command is performed after motor stops    */

/*******************************************************************************
Support for keeping track of which record fields have been changed, so we can
eliminate redundant db_post_events() without having to think, and without having
to keep lots of "last value of field xxx" fields in the record.  The idea is
to say...

	MARK(M_XXXX);

when you mean...

	db_post_events(pmr, &pmr->xxxx, monitor_mask);

Before leaving, you have to call post_MARKed_fields() to actually post the
field to all listeners.  monitor() does this.

	--- NOTE WELL ---
	The macros below assume that the variable "pmr" exists and points to a
	motor record, like so:
		struct hmotorRecord *pmr;
	No check is made in this code to ensure that this really is true.
*******************************************************************************/
#define M_VAL		0x00000001
#define M_DVAL		0x00000002
#define M_HLM		0x00000004
#define M_LLM		0x00000008
#define	M_DMOV		0x00000010
#define	M_SPMG		0x00000020
#define	M_RCNT		0x00000040
#define	M_MRES		0x00000080
#define	M_ERES		0x00000100
#define	M_UEIP		0x00000200
#define M_URIP		0x00000400
#define M_LVIO		0x00000800
#define M_RVAL		0x00001000
#define	M_RLV		0x00002000
#define	M_OFF		0x00004000
#define	M_RBV		0x00008000
#define	M_DHLM		0x00010000
#define	M_DLLM		0x00020000
#define	M_DRBV		0x00040000
#define	M_RDBD		0x00080000
#define	M_MOVN		0x00100000
#define	M_HLS		0x00200000
#define	M_LLS		0x00400000
#define	M_RRBV		0x00800000
#define	M_RMP		0x01000000
#define	M_REP		0x02000000
#define	M_MSTA		0x04000000
#define	M_ATHM		0x08000000
#define	M_TDIR		0x10000000
#define	M_MIP		0x20000000
#define	M_DIFF		0x40000000
#define M_SHIFT		0x80000000
#define M_RDIF		0x80000001
#define M_S		0x80000002
#define M_SBAS		0x80000004
#define M_SBAK		0x80000008
#define M_SREV		0x80000010
#define M_UREV		0x80000020
#define M_VELO		0x80000040
#define M_VBAS		0x80000080
#define M_BVEL		0x80000100
#define M_MISS		0x80000200
#define M_ACCL		0x80000400
#define M_BACC		0x80000800
#define M_LPWR      0x80001000

#define M_FBITS		0x7FFFFFFF

/* #define MARK(a)		pmr->mmap |= (a) */
#define MARK(a)		if ((a)&M_SHIFT) pmr->nmap |= (a); else pmr->mmap |= (a);
#define MARKED(a)	(((a)&M_SHIFT) ? pmr->nmap&M_FBITS&(a) : pmr->mmap&(a))
#define UNMARK(a)	if ((a)&M_SHIFT) pmr->nmap &= ~(a); else pmr->mmap &= ~(a);
#define UNMARK_ALL	pmr->mmap = pmr->nmap = 0

/*******************************************************************************
Device support allows us to string several motor commands into a single
"transaction", using the calls prototyped below:

	int start_trans(dbCommon *mr)
	int build_trans(int command, double *parms, dbCommon *mr)
	int end_trans(struct dbCommon *mr, int go)

For clarity and to avoid typo's, the macros defined below provide simplified
calls.

		--- NOTE WELL ---
	The following macros assume that the variable "pmr" points to a motor
	record, and that the variable "pdset" points to that motor record's device
	support entry table:
		struct hmotorRecord *pmr;
		struct hmotor_dset *pdset = (struct hmotor_dset *)(pmr->dset);

	No checks are made in this code to ensure that these conditions are met.
*******************************************************************************/
/* To begin a transaction... */
#define INIT_MSG()				(*pdset->start_trans)(pmr)

/* To send a single command... */
#define WRITE_MSG(cmd,parms)	(*pdset->build_trans)((cmd), (parms), pmr)

/* To end a transaction and send accumulated commands to the motor... */
#define SEND_MSG()				(*pdset->end_trans)(pmr, 0)


/******************************************************************************
	init_record()

Called twice after an EPICS database has been loaded, and then never called
again.
*******************************************************************************/
static long
init_record(struct hmotorRecord * pmr, int pass)
{
	struct hmotor_dset *pdset = (struct hmotor_dset *) (pmr->dset);
	long            status;

	Debug(4, "init_record: pass = %d\n", pass);
	if (pass == 0) {
		pmr->vers = VERSION;
		return (0);
	}
	/* Check that we have a device-support entry table. */
	if (!(pdset = (struct hmotor_dset *) (pmr->dset))) {
		recGblRecordError(S_dev_noDSET, (void *) pmr,
				  "motor: init_record");
		return (S_dev_noDSET);
	}
	/* Check that DSET has pointers to functions we need. */
	if ((pdset->number < 8) ||
	    (pdset->update_values == NULL) ||
	    (pdset->start_trans == NULL) ||
	    (pdset->build_trans == NULL) ||
	    (pdset->end_trans == NULL)) {
		recGblRecordError(S_dev_missingSup, (void *) pmr,
				  "motor: init_record");
		return (S_dev_missingSup);
	}

	/*
	 * Reconcile two different ways of specifying speed and resolution;
	 * make sure things are sane.
	 */
	check_speed_and_resolution(pmr);

	/* Call device support to initialize itself and the driver */
	Debug(3, "init_record: calling dset->init_record%c\n", ' ');
	if (pdset->init_record) {
		status = (*pdset->init_record) (pmr);
		Debug(4, "init_record: dset->init_record returns %d\n", status);
		if (status) {
			pmr->card = -1;
			return (status);
		}
		pmr->card = pmr->out.value.vmeio.card;
	}
	/*
	 * .dol (Desired Output Location) is a struct containing either a
	 * link to some other field in this database, or a constant intended
	 * to initialize the .val field.  If the latter, get that initial
	 * value and apply it.
	 */
	if (pmr->dol.type == CONSTANT) {
		pmr->udf = FALSE;
		pmr->val = pmr->dol.value.value;
	}
	if (pmr->dol.type == PV_LINK) {
		status = dbCaAddInlink(&(pmr->dol), (void *) pmr, "VAL");
		if (status)
			return (status);
	}

	/* initialize any other channel-access links */
	if (pmr->rdbl.type == PV_LINK) {
		status = dbCaAddInlink(&(pmr->rdbl), (void *) pmr, "DRBV");
		if (status)
			return (status);
	}
	if (pmr->rlnk.type == PV_LINK) {
		status = dbCaAddOutlink(&(pmr->rlnk), (void *) pmr, "RBV");
		if (status)
			return (status);
	}

	/*
	 * Get motor position, encoder position, status, and readback-link
	 * value by calling process_motor_info().
	 */
	process_motor_info(pmr);
	Debug(3, "init_record: rmp = %d\n", pmr->rmp);
	Debug(3, "init_record: drbv = %f\n", pmr->drbv);

	if (pmr->rdbd < pmr->res * .51)
		pmr->rdbd = pmr->res * .51;
	MARK(M_RDBD);

	/*
	 * If we're in closed-loop mode, initializing the user- and
	 * dial-coordinate motor positions (.val and .dval) is someone else's
	 * job. Otherwise, initialize them to the readback values (.rbv and
	 * .drbv) set by our recent call to process_motor_info().
	 */
	if (pmr->omsl != CLOSED_LOOP) {
		pmr->val = pmr->rbv;
		MARK(M_VAL);
		pmr->dval = pmr->drbv;
		MARK(M_DVAL);
		pmr->rval = pmr->rrbv;
		MARK(M_RVAL);
	}
	/* Translate dial-coordinate limits to user-coordinate limits. */
	if (pmr->dir == HMOTOR_DIR_POS) {
		pmr->hlm = pmr->dhlm + pmr->off;
		pmr->llm = pmr->dllm + pmr->off;
	} else {
		pmr->hlm = -(pmr->dllm) + pmr->off;
		pmr->llm = -(pmr->dhlm) + pmr->off;
	}

	/* Initialize miscellaneous control fields. */
	pmr->dmov = TRUE;
	MARK(M_DMOV);
	pmr->movn = FALSE;
	MARK(M_MOVN);
	pmr->cvel = FALSE;
	pmr->lspg = pmr->spmg = HMOTOR_SPMG_GO;
	MARK(M_SPMG);
	pmr->pp = TRUE;
	pmr->diff = pmr->dval - pmr->drbv;
	MARK(M_DIFF);
	pmr->rdif = NINT(pmr->diff / pmr->mres);
	MARK(M_RDIF);
	pmr->lval = pmr->val;
	pmr->ldvl = pmr->dval;
	pmr->lrvl = pmr->rval;
	/* init limit-violation field */
	pmr->lvio = 0;
	if ((pmr->drbv > pmr->dhlm + pmr->res) ||
		(pmr->drbv < pmr->dllm - pmr->res)) {
		pmr->lvio = 1;
		MARK(M_LVIO);
	}
	post_MARKed_fields(pmr, DBE_VALUE);

	return (0);
}


/******************************************************************************
	process()

Called under many different circumstances for many different reasons.

1) Someone poked our .proc field, or some other field that is marked
'process-passive' in the hmotorRecord.ascii file.  In this case, we
determine which fields have changed since the last time we were invoked
and attempt to act accordingly.

2) Device support will call us periodically while a motor is moving, and
once after it stops.  In these cases, we infer that device support has
called us by looking at the flag it set, report the motor's state, and
fire off readback links.  If the motor has stopped, we fire off forward links
as well.

Note that this routine handles all motor records, and that several 'copies'
of this routine may execute 'simultaneously' (in the multitasking sense), as
long as they operate on different records.  This much is normal for an EPICS
record, and the normal mechanism for ensuring that a record does not get
processed by more than one 'simultaneous' copy of this routine (the .pact field)
works here as well.

However, it is normal for an EPICS record to be either 'synchronous' (runs
to completion at every invocation of process()) or 'asynchronous' (begins
processing at one invocation and forbids all further invocations except the
callback invocation from device support that completes processing).  This
record is worse than asynchronous because we can't forbid invocations while
a motor is moving (else a motor could not be stopped), nor can we complete
processing until a motor stops.

Backlash correction would complicate this picture further, since a motor
must stop before backlash correction starts and stops it again, but device
support and the Oregon Microsystems controller allow us to string two move
commands together--even with different velocities and accelerations.

Backlash-corrected jogs (move while user holds 'jog' button down) do
complicate the picture:  we can't string the jog command together with a
backlash correction because we don't know when the user is going to release
the jog button.  Worst of all, it is possible for the user to give us a
'jog' command while the motor is moving.  Then we have to do the following
in separate invocations of process():
	tell the motor to stop
	handle motor-in-motion callbacks while the motor slows down
	recognize the stopped-motor callback and begin jogging
	handle motor-in-motion callbacks while the motor jogs
	recognize when the user releases the jog button and tell the motor to stop
	handle motor-in-motion callbacks while the motor slows down
	recognize the stopped-motor callback and begin a backlash correction
	handle motor-in-motion callbacks while the motor is moving
	recognize the stopped-motor callback and fire off forward links
For this reason, a fair amount of code is devoted to keeping track of
where the motor is in a sequence of movements that comprise a single motion.

*******************************************************************************/
static long
process(struct hmotorRecord * pmr)
{
	struct hmotor_dset *pdset = (struct hmotor_dset *) (pmr->dset);
	long            status, process_reason, nRequest = 1;
	int             old_lvio = pmr->lvio;
	int             dir = (pmr->dir == HMOTOR_DIR_POS) ? 1 : -1;

	if (pmr->pact)
		return (0);

	Debug(4, "process:---------------------- begin; motor \"%s\"\n", pmr->name);
	pmr->pact = 1;

	/*** Who called us? ***/
	/*
	 * Call device support to get raw motor position/status and to see
	 * whether this is a callback.
	 */
	process_reason = (*pdset->update_values) (pmr);
/*        printf( "recMotor::process: reason = %d\n", process_reason );*/

	if (process_reason == HMOTOR_CALLBACK_DATA) {
		/*
		 * This is, effectively, a callback from device support:
		 * either a motor-in-motion update, or some asynchronous
		 * acknowledgement of a command we sent in a previous life.
		 */
		Debug(3, "process: callback%c\n", ' ');

		/*
		 * Get position and status from motor controller. Get
		 * readback-link value if link exists.
		 */
		process_motor_info(pmr);

		if (pmr->movn) {
			/*
			 * This is a motor-in-motion update from device
			 * support.
			 */
			Debug(3, "process: motion update%c\n", ' ');

			/*
			 * Are we going in the wrong direction?  (Don't be
			 * fooled by a backlash correction into saying "yes".
			 * 'Jog*' and 'Hom*' motions don't get midcourse
			 * corrections since we don't know their
			 * destinations.)
			 */
			Debug(10, "process: rvel = %d\n", pmr->rvel);
			if ((fabs(pmr->diff) > 2 * (fabs(pmr->bdst) + pmr->rdbd)) &&
			    ((pmr->rval > pmr->rrbv) != (pmr->tdir == 1)) &&
			    (pmr->rvel != 0) && (MARKED(M_RRBV)) &&
			    ((pmr->mip == MIP_MOVE) || (pmr->mip == MIP_RETRY))) {
				/*
				 * We're going in the wrong direction.
				 * Readback problem?
				 */
				Debug(2, "process: wrong direction, diff = %f", pmr->diff);
/*				printf("%s:tdir = %d\n", pmr->name, pmr->tdir);*/
				INIT_MSG();
				WRITE_MSG(HMOTOR_STOP_AXIS, NULL);
				SEND_MSG();
				pmr->mip |= MIP_STOP;
				MARK(M_MIP);
			}
			status = 0;
		} else {
			/* Motor has stopped. */
			Debug(3, "process: motor has stopped.%c\n", ' ');

			/*
			 * Assume we're done moving until we find out
			 * otherwise.
			 */
			pmr->dmov = TRUE;
			MARK(M_DMOV);

			if (pmr->hls || pmr->lls) {pmr->pp = TRUE;}

			if (pmr->pp) {
				/***************************************
				 * Post process a command or motion when
				 * motor has stopped. We do this for one
				 * of several reasons:
				 * 1) This is the first call to process()
				 * 2) User hit a "Stop" button, and motor
				 *    has stopped.
				 * 3) User released a "Jog*" button and
				 *    motor has stopped.
				 * 4) Hom* command has completed.
				 * 5) User hit Hom* or Jog* while motor
				 *    was moving, causing a 'stop' to be
				 *    sent to the motor, and the motor
				 *    has stopped.
				 * 6) User caused a new value to be
				 *    written to the motor hardware's
				 *    position register.
				 * 7) We hit a limit switch.
				 */

				Debug(3, "process: post process%c\n", ' ');
				pmr->pp = 0;

				if (pmr->omsl != CLOSED_LOOP) {
					/*
					 * Make drive values agree with
					 * readback value.
					 */
					pmr->val = pmr->rbv;
					MARK(M_VAL);
					pmr->dval = pmr->drbv;
					MARK(M_DVAL);
#if 0
pmr->lval = pmr->val;
pmr->ldvl = pmr->dval;
pmr->lrvl = pmr->rval;
#endif
					pmr->rval = pmr->rrbv;
					MARK(M_RVAL);
					pmr->diff = 0.;
					MARK(M_DIFF);
					pmr->rdif = 0.;
					MARK(M_RDIF);
				}
				if (pmr->mip & (MIP_LOAD_P | MIP_LOAD_ER)) {
					/*
					 * We sent HMOTOR_LOAD_POS or HMOTOR_SET_ENC_RATIO,
					 * followed by HMOTOR_GET_INFO.
					 */
					pmr->mip = 0;
					MARK(M_MIP);
				} else if (pmr->mip & (MIP_HOMF | MIP_HOMR)) {
					/* Home command */
					if (pmr->mip & MIP_STOP) {
						/*
						 * Stopped and Hom* button
						 * still down.  Now do Hom*.
						 */
						double          vbase = pmr->vbas / fabs(pmr->res);
						double          hvel = 1000 * fabs(pmr->mres/pmr->eres);
						double          hpos = 0;

						if (hvel <= vbase)
							hvel = vbase + 1;
						pmr->mip &= ~MIP_STOP;
						pmr->dmov = FALSE;
						MARK(M_DMOV);
						pmr->rcnt = 0;
						MARK(M_RCNT);
						INIT_MSG();
						WRITE_MSG(HMOTOR_SET_VEL_BASE, &vbase);
						WRITE_MSG(HMOTOR_SET_VELOCITY, &hvel);
						WRITE_MSG((pmr->mip & MIP_HOMF) ? HMOTOR_HOME_FOR : HMOTOR_HOME_REV, &hpos);
#if 0
						WRITE_MSG(HMOTOR_SET_VELOCITY, &hvel);
						WRITE_MSG(HMOTOR_MOVE_ABS, &hpos);
#endif
						WRITE_MSG(HMOTOR_GO, NULL);
						SEND_MSG();
					} else {
						pmr->mip = 0;	/* All done. */
					}
					MARK(M_MIP);
				} else if (pmr->mip & (MIP_JOGF | MIP_JOGR)) {
					/* Jog command */
					pmr->dmov = FALSE;
					MARK(M_DMOV);
					if ((pmr->mip & MIP_STOP) && (pmr->jogf || pmr->jogr)) {
						/*
						 * Stopped and Jog* button
						 * still down.  Now do Jog*.
						 */
						double          jogv = (pmr->velo * dir) / pmr->res;
						double          jacc = fabs(jogv) / pmr->accl;

						Debug(3, "process: jog start.%c\n", ' ');
						pmr->mip &= ~MIP_STOP;
						MARK(M_MIP);
						pmr->pp = TRUE;
						if (pmr->jogr)
							jogv = -jogv;
						INIT_MSG();
						WRITE_MSG(HMOTOR_SET_ACCEL, &jacc);
						WRITE_MSG(HMOTOR_JOG, &jogv);
						SEND_MSG();
					} else {
						/*
						 * First part of jog done. Do
						 * backlash correction.
						 */
						double          bvel = pmr->bvel / fabs(pmr->res);
						double          bacc = bvel / pmr->bacc;
						double          vbase = pmr->vbas / fabs(pmr->res);
						double          vel = pmr->velo / fabs(pmr->res);
						double          acc = vel / pmr->accl;
						double          bpos = (pmr->dval - pmr->bdst) / pmr->res;
						double          currpos = pmr->dval / pmr->res;
						double          newpos;

						Debug(3, "process: jog BL start. dmov=%d\n", pmr->dmov);
						pmr->mip = MIP_JOG_BL;
						MARK(M_MIP);
						pmr->pp = TRUE;

						INIT_MSG();

						WRITE_MSG(HMOTOR_SET_VEL_BASE, &vbase);
						if (vel <= vbase)
							vel = vbase + 1;
						WRITE_MSG(HMOTOR_SET_VELOCITY, &vel);
						WRITE_MSG(HMOTOR_SET_ACCEL, &acc);
						WRITE_MSG(HMOTOR_MOVE_ABS, &bpos);
						WRITE_MSG(HMOTOR_GO, NULL);

						if (bvel <= vbase)
							bvel = vbase + 1;
						WRITE_MSG(HMOTOR_SET_VELOCITY, &bvel);
						WRITE_MSG(HMOTOR_SET_ACCEL, &bacc);
						newpos = bpos + pmr->frac * (currpos - bpos);
						pmr->rval = NINT(newpos);
						WRITE_MSG(HMOTOR_MOVE_ABS, &newpos);
						WRITE_MSG(HMOTOR_GO, NULL);

						SEND_MSG();
					}
				} else if (pmr->mip & MIP_JOG_BL) {
					/*
					 * Completed backlash part of jog
					 * command.
					 */
					Debug(3, "process: jog backlash done.%c\n", ' ');
					pmr->mip = 0;
					MARK(M_MIP);
				}
				/*
				 * MARK() values for posting, save old values
				 * for next call.
				 */
				MARK(M_HLM);
				MARK(M_LLM);
				pmr->lval = pmr->val;
				pmr->ldvl = pmr->dval;
				pmr->lrvl = pmr->rval;
				pmr->mip &= ~MIP_STOP;
				MARK(M_MIP);
				status = 0;
			}	/* if (pmr->pp) */
			/* Are we "close enough" to desired position? */
			if (pmr->dmov) {
				if ((fabs(pmr->diff) > pmr->rdbd) && !pmr->hls && !pmr->lls) {
					/*
					 * No, we're not close enough.  Try
					 * again...
					 */
					Debug(1, "process: not close enough; diff = %f\n", pmr->diff);
					/*
					 * If max retry count is zero, retry
					 * is disabled
					 */
					if (pmr->rtry) {
						if (++(pmr->rcnt) > pmr->rtry) {
							/* Too many retries. */
#if PAUSE_ON_MAX_RETRY
							pmr->spmg = HMOTOR_SPMG_PAUSE;
							MARK(M_SPMG);
#endif
							pmr->rcnt = 0;
							MARK(M_RCNT);
							pmr->mip = 0;
							MARK(M_MIP);

							/*
							 * We should probably
							 * be triggering
							 * alarms here.
							 */
							pmr->miss = 1;
							MARK(M_MISS);
						} else {
							pmr->dmov = FALSE;
							MARK(M_DMOV);
							pmr->mip = MIP_RETRY;
							MARK(M_MIP);
						}
						MARK(M_RCNT);
					}
				} else {
					/*
					 * Yes, we're close enough to the
					 * desired value.
					 */
					pmr->mip = 0;
					MARK(M_MIP);
					pmr->rcnt = 0;
					MARK(M_RCNT);
					if (pmr->miss) {
						pmr->miss = 0;
						MARK(M_MISS);
					}

					/*
					 * If user initiated this motion by
					 * hitting the "Move" button, pause.
					 */
					if (pmr->spmg == HMOTOR_SPMG_MOVE) {
						pmr->spmg = HMOTOR_SPMG_PAUSE;
						MARK(M_SPMG);
					}
				}
			}
		} /* if (pmr->movn) {} else */
	} /* if (process_reason == CALLBACK_DATA) */

	/* check for soft-limit violation */
	if (pmr->mip & (MIP_JOG | MIP_HOME)) {
		pmr->lvio = (pmr->drbv > pmr->dhlm - pmr->velo) ||
			(pmr->drbv < pmr->dllm + pmr->velo);
	} else {
		pmr->lvio = (pmr->drbv > pmr->dhlm + fabs(pmr->res)) ||
			(pmr->drbv < pmr->dllm - fabs(pmr->res));
	}
	if (pmr->lvio != old_lvio) {
		MARK(M_LVIO);
		if (pmr->lvio && !pmr->set) {
			pmr->stop = 1;
			pmr->jogf = pmr->jogr = 0;
		}
	}
	/*
	 * Do we need to examine the record to figure out what work to
	 * perform?
	 */
	if (pmr->stop || (pmr->spmg == HMOTOR_SPMG_STOP) ||
	    (pmr->spmg == HMOTOR_SPMG_PAUSE) ||
	    (process_reason != HMOTOR_CALLBACK_DATA) || pmr->dmov ||
	    pmr->mip & MIP_RETRY ||
	    pmr->pwr != pmr->lpwr) {
		status = do_work(pmr);
	}
	/* Fire off readback link */
/* !!!!!! */
     	Debug(5, "process: putting ReadbackLink value%c\n", ' ');
/*     	printf( "recMotor::process: putting ReadbackLink value\n" );*/
	status = recGblPutLinkValue(&(pmr->rlnk), (void *) pmr, DBR_DOUBLE,
				    &(pmr->rbv), &nRequest);
/*   !!!!!!
*/
	if (pmr->dmov) {
		recGblFwdLink(pmr);	/* Process the forward-scan-link
					 * record. */
	}
	/*** We're done.  Report the current state of the motor. ***/
	recGblGetTimeStamp(pmr);
	alarm(pmr);		/* If we've violated alarm limits, yell. */
	monitor(pmr);		/* If values have changed, broadcast them. */
	pmr->pact = 0;
	return (status);
}


/******************************************************************************
	do_work()
Here, we do the real work of processing the motor record.

The equations that transform between user and dial coordinates follow.
Note: if user and dial coordinates differ in sign, we have to reverse the
sense of the limits in going between user and dial.

Dial to User:
userVAL	= DialVAL * DIR + OFFset
userHLM	= (DIR==+) ? DialHLM + OFFset : -DialLLM + OFFset
userLLM = (DIR==+) ? DialLLM + OFFset : -DialHLM + OFFset

User to Dial:
DialVAL	= (userVAL - OFFset) / DIR
DialHLM	= (DIR==+) ? userHLM - OFFset : -userLLM + OFFset
DialLLM = (DIR==+) ? userLLM - OFFset : -userHLM + OFFset

Offset:
OFFset	= userVAL - DialVAL * DIR
*******************************************************************************/
static long
do_work(struct hmotorRecord * pmr)
{
	struct hmotor_dset *pdset = (struct hmotor_dset *) (pmr->dset);
	int             dir_positive = (pmr->dir == HMOTOR_DIR_POS);
	int             dir = dir_positive ? 1 : -1;
	int             set = pmr->set;
	int             stopped = (pmr->spmg == HMOTOR_SPMG_STOP || pmr->spmg == HMOTOR_SPMG_PAUSE);
	int             old_lvio = pmr->lvio;
/*	int             jog_limit;*/

	Debug(3, "do_work: begin%c\n", ' ');
        
	/*** Process Stop button. ***/
	if (pmr->stop && (pmr->mip != MIP_STOP)) {
		/* Stop motor. */
		Debug(3, "do_work: Stop button%c\n", ' ');
		pmr->mip = MIP_STOP;
		MARK(M_MIP);
		pmr->pp = TRUE;
		pmr->jogf = pmr->jogr = 0;
		pmr->stop = 0;
		INIT_MSG();
		WRITE_MSG(HMOTOR_STOP_AXIS, NULL);
		SEND_MSG();
		return (0);
	}

	/*** Process Poff command ***/
	if (pmr->pwr != pmr->lpwr) {
		if (pmr->pwr == HMOTOR_PWR_ON) {
			INIT_MSG();
			WRITE_MSG(HMOTOR_POWER_ON, NULL);
			SEND_MSG();
		} else if (pmr->pwr == HMOTOR_PWR_OFF) {
			INIT_MSG();
			WRITE_MSG(HMOTOR_POWER_OFF, NULL);
			SEND_MSG();
		}
		pmr->lpwr = pmr->pwr;
		MARK(M_LPWR);
		return (0);
	}

	/*** Process Stop/Pause/Go_Pause/Go switch. ***
	*
	* STOP	means make the motor stop and, when it does, make the drive
	*       fields (e.g., .val) agree with the readback fields (e.g., .rbv)
	*       so the motor stays stopped until somebody gives it a new place
	*       to go and sets the switch to MOVE or HMOTOR_GO.
	*
 	* PAUSE	means stop the motor like the old steppermotorRecord stops
	*       a motor:  At the next call to process() the motor will continue
	*       moving to .val.
	*
	* MOVE	means Go to .val, but then wait for another explicit Go or
	*       Go_Pause before moving the motor, even if the .dval field
	*       changes.
	*
	* HMOTOR_GO	means Go, and then respond to any field whose change causes
	*       .dval to change as if .dval had received a dbPut().
	*       (Implicit Go, as implemented in the old steppermotorRecord.)
	*       Note that a great many fields (.val, .rvl, .off, .twf, .homf,
	*       .jogf, etc.) can make .dval change.
	*/
	if (pmr->spmg != pmr->lspg) {
		Debug(3, "do_work: SPMG button%c\n", ' ');
		pmr->lspg = pmr->spmg;
		if (pmr->spmg == HMOTOR_SPMG_STOP || pmr->spmg == HMOTOR_SPMG_PAUSE) {
			/* Stop motor. */
			pmr->mip = MIP_STOP;
			MARK(M_MIP);
			INIT_MSG();
			WRITE_MSG(HMOTOR_STOP_AXIS, NULL);
			SEND_MSG();
			/*
			 * If STOP, make drive values agree with readback
			 * values (when the motor actually stops).
			 */
			if (pmr->spmg == HMOTOR_SPMG_STOP) {
				if (pmr->movn) {
					pmr->pp = TRUE;	/* Do when motor stops. */
				} else {
					pmr->val = pmr->rbv;
					MARK(M_VAL);
					pmr->dval = pmr->drbv;
					MARK(M_DVAL);
					pmr->rval = pmr->rrbv;
					MARK(M_RVAL);
				}
			}
			return (0);
		} else {
			pmr->mip = 0;
			MARK(M_MIP);
			pmr->rcnt = 0;
			MARK(M_RCNT);
		}
	}
	/*** Handle changes in motor/encoder resolution, and in .ueip. ***/
	if (MARKED(M_MRES) || MARKED(M_ERES) || MARKED(M_UEIP)) {
		/* encoder pulses, motor pulses */
		double          ep_mp[2];
		long m;

		Debug(3, "do_work: New resolution or ueip.%c\n", ' ');
		/*
		 * Set the encoder ratio.  Note this is blatantly device
		 * dependent.
		 */
		if ((pmr->msta & HMOTOR_EA_PRESENT) && pmr->ueip) {
			/* defend against divide by zero */
			if (fabs(pmr->mres) < 1.e-9) {
				pmr->mres = 1.;
				MARK(M_MRES);
			}
			if (fabs(pmr->eres) < 1.e-9) {
				pmr->eres = pmr->mres;
				MARK(M_ERES);
			}
			/*
			 * OMS hardware can't handle negative motor or encoder
			 * resolution in the SET_ENCODER_RATIO command.
			 * For now, we simply don't allow motor and encoder
			 * resolutions to differ in sign.
			 */
			if ((pmr->mres < 0.) != (pmr->eres < 0.)) {
				pmr->eres *= -1.;
				MARK(M_ERES);
			}
			/* Calculate encoder ratio. */
			for (m=10000000; (m > 1) &&
				(fabs(m/pmr->eres) > 1.e6 || fabs(m/pmr->mres) > 1.e6);
				m /= 10)
				;
			Debug(5, "do_work: ER mult = %d\n", m);
			ep_mp[0] = fabs(m / pmr->eres);
			ep_mp[1] = fabs(m / pmr->mres);
			/* Select encoder resolution for use in later calculations. */
			pmr->res = pmr->eres;
		} else {
			ep_mp[0] = 1.;
			ep_mp[1] = 1.;
			pmr->res = pmr->mres;
		}
		Debug(5, "do_work: Encoder ratio numerator (encoder) = %f\n", ep_mp[0]);
		Debug(5, "do_work: Encoder ratio denominator (motor) = %f\n", ep_mp[1]);
		Debug(3, "do_work: eres = %f.\n", pmr->eres);
		Debug(3, "do_work: mres = %f.\n", pmr->mres);
		Debug(3, "do_work: res = %f.\n", pmr->res);

		/* Make sure retry deadband is achievable */
		if (pmr->rdbd < fabs(pmr->res) * 0.51) {
			pmr->rdbd = fabs(pmr->res) * 0.51;
			MARK(M_RDBD);
		}
		if (pmr->msta & HMOTOR_EA_PRESENT) {
			INIT_MSG();
			WRITE_MSG(HMOTOR_SET_ENC_RATIO, ep_mp);
			SEND_MSG();
		}
		Debug(3, "do_work: Encoder ratio set.%c\n", ' ');
		load_pos(pmr);

		return (0);
	}
	/*** Collect .val (User value) changes from all sources. ***/
	if (pmr->omsl == CLOSED_LOOP && pmr->dol.type == DB_LINK) {
		/** If we're in CLOSED_LOOP mode, get value from input link. **/
		long            status, options = 0, nRequest = 1;

		Debug(3, "do_work: GetLink .dol%c\n", ' ');
		status = recGblGetLinkValue(&(pmr->dol), (void *) pmr, DBR_DOUBLE,
					  &(pmr->val), &options, &nRequest);
		if (!RTN_SUCCESS(status)) {
			pmr->udf = TRUE;
			return (1);
		}
		pmr->udf = FALSE;
		/* Later, we'll act on this new value of .val. */
	} else {
		/** Check out all the buttons and other sources of motion **/

		/* Send motor to home switch in forward direction. */
		if (!stopped && !pmr->lvio && (
		       (pmr->homf && !(pmr->mip & MIP_HOMF) && !pmr->hls) ||
			  (pmr->homr && !(pmr->mip & MIP_HOMR) && !pmr->lls)
					       )) {
			Debug(3, "do_work: Hom* button%c\n", ' ');
/*
			printf( "recMotor::do_work: HOMF %d HOMR %d using %s\n",
                                pmr->homf, pmr->homr,
                                (pmr->homf?"HOMF":"HOMR") );
*/

			/* check for limit violation */
			if ((pmr->dval > pmr->dhlm - pmr->velo) ||
			    (pmr->dval < pmr->dllm + pmr->velo)) {
				pmr->lvio = 1;
				MARK(M_LVIO);
/*                            printf( "recMotor::do_work: limit violation\n" );*/
				return (0);
			}
			pmr->mip = pmr->homf ? MIP_HOMF : MIP_HOMR;
			MARK(M_MIP);
			pmr->pp = TRUE;
			if (pmr->movn) {
				pmr->mip |= MIP_STOP;
				MARK(M_MIP);
				INIT_MSG();
				WRITE_MSG(HMOTOR_STOP_AXIS, NULL);
				SEND_MSG();
			} else {
				double          vbase = pmr->vbas / fabs(pmr->res);
#if 0
				double          hvel = 1000 * fabs(pmr->mres/pmr->eres); 
#endif
				/*
				 * The built-in velocity for finding home is really slow
				 * (for better accuracy?).  Since I have to do a peak
				 * search anyway, I set the speed to the normal
				 * speed.
				 */
				double			hvel = pmr->velo / fabs(pmr->res);
				double          acc = hvel / pmr->accl;

				double          hpos = 0;

				INIT_MSG();
				WRITE_MSG(HMOTOR_SET_VEL_BASE, &vbase);
				if (hvel <= vbase)
					hvel = vbase + 1;
/*                                printf( "recMotor::do_work: MRES %f ERES %f\n",
                                        pmr->mres, pmr->eres );*/
/*                                printf( "recMotor::do_work: hvel %f\n", hvel );*/
				WRITE_MSG(HMOTOR_SET_VELOCITY, &hvel);
				WRITE_MSG(HMOTOR_SET_ACCEL, &acc);
				WRITE_MSG((pmr->mip & MIP_HOMF) ? HMOTOR_HOME_FOR : HMOTOR_HOME_REV, &hpos);
				/*
				 * WRITE_MSG(HMOTOR_SET_VELOCITY, &hvel);
				 * WRITE_MSG(HMOTOR_MOVE_ABS, &hpos);
				 */
				WRITE_MSG(HMOTOR_GO, NULL);
				SEND_MSG();
				pmr->dmov = FALSE;
				MARK(M_DMOV);
				pmr->rcnt = 0;
				MARK(M_RCNT);
			}
			pmr->homf = pmr->homr = 0;
			return (0);
		}
		/*
		 * Jog motor.  Move continuously until we hit a software
		 * limit or a limit switch, or until user releases button.
		 */
		if (!stopped && !pmr->lvio && (
		       (pmr->jogf && !(pmr->mip & MIP_JOGF) && !pmr->hls) ||
			  (pmr->jogr && !(pmr->mip & MIP_JOGR) && !pmr->lls)
					       )) {
			Debug(3, "do_work: Jog* start.%c\n", ' ');

			/* check for limit violation */
			if ((pmr->dval > pmr->dhlm - pmr->velo) ||
			    (pmr->dval < pmr->dllm + pmr->velo)) {
				pmr->lvio = 1;
				MARK(M_LVIO);
				return (0);
			}
			pmr->mip = pmr->jogf ? MIP_JOGF : MIP_JOGR;
			MARK(M_MIP);
			if (pmr->movn) {
				pmr->mip |= MIP_STOP;
				MARK(M_MIP);
				pmr->pp = TRUE;
				INIT_MSG();
				WRITE_MSG(HMOTOR_STOP_AXIS, NULL);
				SEND_MSG();
			} else {
				double          jogv = (pmr->velo * dir) / pmr->res;
				double          jacc = fabs(jogv) / pmr->accl;

				pmr->dmov = FALSE;
				MARK(M_DMOV);
				pmr->pp = TRUE;
				if (pmr->jogr)
					jogv = -jogv;
				INIT_MSG();
				WRITE_MSG(HMOTOR_SET_ACCEL, &jacc);
				WRITE_MSG(HMOTOR_JOG, &jogv);
				SEND_MSG();
			}
			return (0);
		}
		/* Stop jogging. */
		if ((!pmr->jogf && (pmr->mip & MIP_JOGF)) ||
		    (!pmr->jogr && (pmr->mip & MIP_JOGR))) {
			/*
			 * Stop motor.  When stopped, process() will correct
			 * backlash.
			 */
			Debug(3, "do_work: Jog* stop.%c\n", ' ');
			pmr->pp = TRUE;
			INIT_MSG();
			WRITE_MSG(HMOTOR_STOP_AXIS, NULL);
			SEND_MSG();
			return (0);
		}
		/*
		 * Tweak motor forward (reverse).  Increment motor's position
		 * by a value stored in pmr->twv.
		 */
		if (pmr->twf || pmr->twr) {
			Debug(3, "do_work: Tweak%c\n", ' ');
			pmr->val += pmr->twv * (pmr->twf ? 1 : -1);
			/* Later, we'll act on this. */
			if (pmr->twf)
				pmr->twf = 0;
			if (pmr->twr)
				pmr->twr = 0;
		}
		/*
		 * New relative value.  Someone has poked a value into the
		 * "move relative" field (just like the .val field, but
		 * relative instead of absolute.)
		 */
		if (pmr->rlv != pmr->lrlv) {
			Debug(3, "do_work: RelVal%c\n", ' ');
			pmr->val += pmr->rlv;
			/* Later, we'll act on this. */
			pmr->rlv = 0.;
			MARK(M_RLV);
			pmr->lrlv = pmr->rlv;
		}
		/* New raw value.  Propagate to .dval and act later. */
		if (pmr->rval != pmr->lrvl) {
			Debug(3, "do_work: RawVal%c\n", ' ');
			pmr->dval = pmr->rval * pmr->res;
			/* Later, we'll act on this. */
		}
	}

	/*** Collect .dval (Dial value) changes from all sources. ***
	* Now we either act directly on the .val change and return, or we
	* propagate it into a .dval change.
	*/
	if (pmr->val != pmr->lval) {
		Debug(3, "do_work: New .val%c\n", ' ');
		MARK(M_VAL);
		if (set && !pmr->foff) {
			/*
			 * Act directly on .val. and return. User wants to
			 * redefine .val without moving the motor and
			 * without making a change to .dval.  Adjust the
			 * offset and recalc user limits back into agreement
			 * with dial limits.
			 */
			Debug(3, "do_work: New .val (set==TRUE)%c\n", ' ');
			pmr->off = pmr->val - pmr->dval * dir;
			pmr->rbv = pmr->drbv * dir + pmr->off;
			if (dir_positive) {
				pmr->hlm = pmr->dhlm + pmr->off;
				pmr->llm = pmr->dllm + pmr->off;
			} else {
				pmr->hlm = -(pmr->dllm) + pmr->off;
				pmr->llm = -(pmr->dhlm) + pmr->off;
			}
			MARK(M_VAL);
			pmr->lval = pmr->val;
			MARK(M_OFF);
			MARK(M_RBV);
			MARK(M_HLM);
			MARK(M_LLM);
			pmr->mip = 0;
			MARK(M_MIP);
			pmr->dmov = TRUE;
			MARK(M_DMOV);
			return (0);
		} else {
			/*
			 * User wants to move the motor, or to recalibrate both
			 * user and dial.  Propagate .val to .dval.
			 */
			Debug(3, "do_work: New .val ==> .dval%c\n", ' ');
			pmr->dval = (pmr->val - pmr->off) / dir;
			/* Later we'll act on this. */
		}
	}
	/* Record limit violation */
	pmr->lvio = (pmr->dval > pmr->dhlm) || (pmr->dval > pmr->dhlm + pmr->bdst)
		|| (pmr->dval < pmr->dllm) || (pmr->dval < pmr->dllm + pmr->bdst);
	if (pmr->lvio != old_lvio)
		MARK(M_LVIO);
	if (pmr->lvio) {
		Debug(4, "do_work: limit violation holding motor stopped%c\n", ' ');
		pmr->val = pmr->lval;
		MARK(M_VAL);
		pmr->dval = pmr->ldvl;
		MARK(M_DVAL);
		pmr->rval = pmr->lrvl;
		MARK(M_RVAL);
		pmr->dmov = TRUE;
		MARK(M_DMOV);
		return (0);
	}
	Debug(4, "do_work: dval=%f\n", pmr->dval);
/****
	printf( "recMotor::do_work: dval=%f drbv=%f\n", pmr->dval, pmr->drbv );
****/
	if (pmr->spmg == HMOTOR_SPMG_STOP || pmr->spmg == HMOTOR_SPMG_PAUSE) {
		Debug(4, "do_work: SPMG holding motor stopped%c\n", ' ');
		return (0);
	}
	/*** New dial value. ***/
	if (pmr->dval != pmr->ldvl || !pmr->dmov) {
		Debug(3, "do_work: New .dval or !dmov (dmov=%d)\n", pmr->dmov);
		MARK(M_DVAL);
		pmr->diff = pmr->dval - pmr->drbv;
		MARK(M_DIFF);
		pmr->rdif = NINT(pmr->diff / pmr->mres);
		MARK(M_RDIF);
		if (set) {
			load_pos(pmr);
			/*
			 * device support will call us back when load is
			 * done.
			 */
			return (0);
		} else {
			/** Calc new raw position, and do a (backlash-corrected?) move. **/
			double          rbvpos = pmr->drbv / pmr->res;	/* where motor is  */
			double          currpos = pmr->ldvl / pmr->res;	/* where we are    */
			double          newpos = pmr->dval / pmr->res;	/* where to go     */
			double          vbase = pmr->vbas / fabs(pmr->res);	/* base speed      */
			double          vel = pmr->velo / fabs(pmr->res);	/* normal speed    */
			double          acc = vel / pmr->accl;	/* normal accel.   */
			/*
			 * 'bpos' is one backlash distance away from
			 * 'newpos'.
			 */
			double          bpos = (pmr->dval - pmr->bdst) / pmr->res;
			double          bvel = pmr->bvel / fabs(pmr->res);	/* backlash speed  */
			double          bacc = bvel / pmr->bacc;	/* backlash accel. */
/*			double          trypos;*/
			double          slop = 0.95 * pmr->rdbd;
			/* Use if encoder or ReadbackLink is in use. */
			int             use_rel = ((pmr->msta & HMOTOR_EA_PRESENT) && pmr->ueip) || pmr->urip;
			double          relpos = pmr->diff / pmr->res;
			double          relbpos = ((pmr->dval - pmr->bdst) - pmr->drbv) / pmr->res;
			long            rpos, npos;

			rpos = NINT(rbvpos);
			npos = NINT(newpos);
			if (npos == rpos) {
				Debug(3, "do_work: MOVE zero steps.%c\n", ' ');
				pmr->dmov = TRUE;
				MARK(M_DMOV);
				return (0);
			}
			/* enforce lower limit on speeds */
			if (vel <= vbase)
				vel = vbase + 1;
			if (bvel <= vbase)
				bvel = vbase + 1;
/***
                        printf( "recMotor::do_work: vel %f bvel %f\n",
                                vel, bvel );
***/
			/*
			 * Post new values, recalc .val to reflect the change
			 * in .dval. (We no longer know the origin of the
			 * .dval change.  If user changed .val, we're ok as
			 * we are, but if .dval was changed directly, we must
			 * make .val agree.)
			 */
			Debug(3, "do_work: MOVE to .dval.%c\n", ' ');
			pmr->val = pmr->dval * dir + pmr->off;
			pmr->rval = NINT(pmr->dval / pmr->res);
			MARK(M_DVAL);
			MARK(M_VAL);
			MARK(M_RVAL);

			/*
			 * If we're within retry deadband, move only in
			 * preferred dir.
			 */
			if (use_rel) {
				if ((fabs(pmr->diff) < slop) &&
				    ((relpos > 0) != (pmr->bdst > 0))) {
					Debug(3, "do_work: (rel) In .rbdb and not pref dir.%c\n", ' ');
					pmr->dmov = TRUE;
					MARK(M_DMOV);
					return (0);
				}
			} else {
				if ((fabs(pmr->diff) < slop) &&
				  ((newpos > currpos) != (pmr->bdst > 0))) {
					Debug(3, "do_work: (abs) In .rbdb and not pref dir.%c\n", ' ');
					pmr->dmov = TRUE;
					MARK(M_DMOV);
					return (0);
				}
			}

			if (!pmr->movn) {
				pmr->mip = MIP_MOVE;
				MARK(M_MIP);
				/* v1.96 Don't post dmov if special already did. */
				if (pmr->dmov) {
					pmr->dmov = FALSE;
					MARK(M_DMOV);
				}
				pmr->ldvl = pmr->dval;
				pmr->lval = pmr->val;
				pmr->lrvl = pmr->rval;

				INIT_MSG();

				/* Is backlash correction disabled? */
				if (fabs(pmr->bdst) < pmr->res) {
					/*
					 * Yes, just move to newpos at
					 * (vel,acc)
					 */
					WRITE_MSG(HMOTOR_SET_VEL_BASE, &vbase);
					WRITE_MSG(HMOTOR_SET_VELOCITY, &vel);
					WRITE_MSG(HMOTOR_SET_ACCEL, &acc);
					if (use_rel) {
						relpos *= pmr->frac;
						Debug(3, "do_work: HMOTOR_MOVE_REL to %f\n", relpos);
						WRITE_MSG(HMOTOR_MOVE_REL, &relpos);
					} else {
						newpos = currpos + pmr->frac * (newpos - currpos);
						Debug(3, "do_work: HMOTOR_MOVE_ABS to %f\n", newpos);
						WRITE_MSG(HMOTOR_MOVE_ABS, &newpos);
					}
					WRITE_MSG(HMOTOR_GO, NULL);
				}
				/*
				 * If current position is already within
				 * backlash range, or if we already within
				 * retry deadband of desired position, then
				 * we don't have to take out backlash.
				 */
				else if (
					 (fabs(pmr->diff) < slop) ||
					 (use_rel && ((relbpos < 0) == (relpos > 0))) ||
					 (!use_rel && (((currpos + slop) > bpos) ==
						       (newpos > currpos)))
					) {
					/**
					 * Yes, assume backlash has already been taken out.
					 * Move to newpos at (bvel, bacc).
					 */
					WRITE_MSG(HMOTOR_SET_VEL_BASE, &vbase);
					WRITE_MSG(HMOTOR_SET_VELOCITY, &bvel);
					WRITE_MSG(HMOTOR_SET_ACCEL, &bacc);
					/********************************************************
					 * Backlash correction imposes a much larger penalty on
					 * overshoot than on undershoot. Here, we allow user to
					 * specify (by .frac) the fraction of the backlash distance
					 * to move as a first approximation. When the motor stops
					 * and we're not yet at 'newpos', the callback will give
					 * us another chance, and we'll go .frac of the remaining
					 * distance, and so on. This algorithm is essential when
					 * the drive creeps after a move (e.g., piezo inchworm),
					 * and helpful when the readback device has a latency
					 * problem (e.g., interpolated encoder), or is a little
					 * nonlinear. (Blatantly nonlinear readback is not handled
					 * by the motor record.)
					 */
					if (use_rel) {
						relpos *= pmr->frac;
						Debug(3, "do_work: HMOTOR_MOVE_REL to %f\n", relpos);
						WRITE_MSG(HMOTOR_MOVE_REL, &relpos);
					} else {
						newpos = currpos + pmr->frac * (newpos - currpos);
						Debug(3, "do_work: HMOTOR_MOVE_ABS to %f\n", newpos);
						WRITE_MSG(HMOTOR_MOVE_ABS, &newpos);
					}
					WRITE_MSG(HMOTOR_GO, NULL);
				} else {

					/*
					 * We need to take out backlash.  Go
					 * to bpos at (vel,acc)
					 */
					WRITE_MSG(HMOTOR_SET_VEL_BASE, &vbase);
					WRITE_MSG(HMOTOR_SET_VELOCITY, &vel);
					WRITE_MSG(HMOTOR_SET_ACCEL, &acc);
					if (use_rel) {
						Debug(3, "do_work: HMOTOR_MOVE_REL (BL) to %f\n", relbpos);
						WRITE_MSG(HMOTOR_MOVE_REL, &relbpos);
					} else {
						Debug(3, "do_work: HMOTOR_MOVE_ABS (BL) to %f\n", bpos);
						WRITE_MSG(HMOTOR_MOVE_ABS, &bpos);
					}
					WRITE_MSG(HMOTOR_GO, NULL);

					/* Move to newpos at (bvel, bacc). */
					WRITE_MSG(HMOTOR_SET_VELOCITY, &bvel);
					WRITE_MSG(HMOTOR_SET_ACCEL, &bacc);
					/*
					 * See note regarding backlash and
					 * overshoot above.
					 */
					if (use_rel) {
						relpos = (relpos - relbpos) * pmr->frac;
						Debug(3, "do_work: HMOTOR_MOVE_REL to %f\n", relpos);
						WRITE_MSG(HMOTOR_MOVE_REL, &relpos);
					} else {
						newpos = bpos + pmr->frac * (newpos - bpos);
						Debug(3, "do_work: HMOTOR_MOVE_ABS to %f\n", newpos);
						WRITE_MSG(HMOTOR_MOVE_ABS, &newpos);
					}
					WRITE_MSG(HMOTOR_GO, NULL);
				}
				SEND_MSG();
			}
		}
	}
	return (0);
}


/******************************************************************************
	special()
*******************************************************************************/
static long
special(paddr, after)
	struct dbAddr  *paddr;
	int             after;
{
	struct hmotorRecord *pmr = (struct hmotorRecord *) paddr->precord;
	unsigned short  monitor_mask = DBE_VALUE;
	int             dir_positive = (pmr->dir == HMOTOR_DIR_POS);
	int             dir = dir_positive ? 1 : -1;
	int             changed = 0;

	Debug(3, "special: after = %d\n", after);
	/*
	 * Someone wrote to drive field.  Blink .dmov unless record is
	 * disabled.
	 */
	if (!after &&
	    ((paddr->pfield == &pmr->val) ||
             (paddr->pfield == &pmr->dval) ||
             (paddr->pfield == &pmr->rval) ||
             (paddr->pfield == &pmr->rlv)) )
        {
/*
	    (paddr->pfield == (void *) &pmr->val) ||
	    (paddr->pfield == (void *) &pmr->dval) ||
	    (paddr->pfield == (void *) &pmr->rval) ||
	    (paddr->pfield == (void *) &pmr->rlv) {
*/
		if (pmr->disa == pmr->disv) {
			Debug(5, "special: drive field hit with record disabled%c\n", ' ');
			return (0);
		}
		if (pmr->disp) {
			Debug(5, "special: drive field hit with puts disabled%c\n", ' ');
			return (0);
		}
		pmr->dmov = FALSE;
		db_post_events(pmr, &pmr->dmov, monitor_mask);
		Debug(5, "special: dmov set to 0.%c\n", ' ');
		return (0);
	}
	if (!after)
		return (0);

	/* new velo: make s agree */
	if (paddr->pfield == (void *) &pmr->velo) {
		if (pmr->velo < 0.0) {
			pmr->velo = 0.0;
			changed = 1;
		}
		if (pmr->velo < pmr->vbas) {
			pmr->velo = pmr->vbas;
			changed = 1;
		}
		if (changed)
			db_post_events(pmr, &pmr->velo, monitor_mask);
		if ((pmr->urev != 0.0) && (pmr->s != pmr->velo / fabs(pmr->urev))) {
			pmr->s = pmr->velo / fabs(pmr->urev);
			db_post_events(pmr, &pmr->s, monitor_mask);
		}
		return (0);
	}

	/* new bvel: make sbak agree */
	if (paddr->pfield == (void *) &pmr->bvel) {
		if (pmr->bvel < 0) {
			pmr->bvel = 0;
			changed = 1;
		}
		if (pmr->bvel < pmr->vbas) {
			pmr->bvel = pmr->vbas;
			changed = 1;
		}
		if (changed)
			db_post_events(pmr, &pmr->bvel, monitor_mask);
		if ((pmr->urev != 0.0) && (pmr->sbak != pmr->bvel / fabs(pmr->urev))) {
			pmr->sbak = pmr->bvel / fabs(pmr->urev);
			db_post_events(pmr, &pmr->sbak, monitor_mask);
		}
		return (0);
	}

	/* new vbas: make sbas agree */
	if (paddr->pfield == (void *) &pmr->vbas) {
		if (pmr->vbas < 0) {
			pmr->vbas = 0;
			db_post_events(pmr, &pmr->vbas, monitor_mask);
		}
		if ((pmr->urev != 0.0) && (pmr->sbas != pmr->vbas / fabs(pmr->urev))) {
			pmr->sbas = pmr->vbas / fabs(pmr->urev);
			db_post_events(pmr, &pmr->sbas, monitor_mask);
		}
		return (0);
	}

	/* new s: make velo agree */
	if (paddr->pfield == (void *) &pmr->s) {
		if (pmr->s < 0) {
			pmr->s = 0;
			changed = 1;
		}
		if (pmr->s < pmr->sbas) {
			pmr->s = pmr->sbas;
			changed = 1;
		}
		if (changed)
			db_post_events(pmr, &pmr->s, monitor_mask);
		if (pmr->velo != fabs(pmr->urev) * pmr->s) {
			pmr->velo = fabs(pmr->urev) * pmr->s;
			db_post_events(pmr, &pmr->velo, monitor_mask);
		}
		return (0);
	}

	/* new sbak: make bvel agree */
	if (paddr->pfield == (void *) &pmr->sbak) {
		if (pmr->sbak < 0) {
			pmr->sbak = 0;
			changed = 1;
		}
		if (pmr->sbak < pmr->sbas) {
			pmr->sbak = pmr->sbas;
			changed = 1;
		}
		if (changed)
			db_post_events(pmr, &pmr->sbak, monitor_mask);
		if (pmr->bvel != fabs(pmr->urev) * pmr->sbak) {
			pmr->bvel = fabs(pmr->urev) * pmr->sbak;
			db_post_events(pmr, &pmr->bvel, monitor_mask);
		}
		return (0);
	}

	/* new sbas: make vbas agree */
	if (paddr->pfield == (void *) &pmr->sbas) {
		if (pmr->sbas < 0) {
			pmr->sbas = 0;
			db_post_events(pmr, &pmr->sbas, monitor_mask);
		}
		if (pmr->vbas != fabs(pmr->urev) * pmr->sbas) {
			pmr->vbas = fabs(pmr->urev) * pmr->sbas;
			db_post_events(pmr, &pmr->vbas, monitor_mask);
		}
		return (0);
	}

	/* new accl */
	if (paddr->pfield == (void *) &pmr->accl) {
		if (pmr->accl <= 0.0) {
			pmr->accl = 0.1;
			db_post_events(pmr, &pmr->accl, monitor_mask);
		}
		return (0);
	}

	/* new bacc */
	if (paddr->pfield == (void *) &pmr->bacc) {
		if (pmr->bacc <= 0.0) {
			pmr->bacc = 0.1;
			db_post_events(pmr, &pmr->bacc, monitor_mask);
		}
		return (0);
	}

	/* new rdbd */
	if (paddr->pfield == (void *) &pmr->rdbd) {
		if (pmr->rdbd < fabs(pmr->res) * .51) {
			pmr->rdbd = fabs(pmr->res) * .51;
			db_post_events(pmr, &pmr->rdbd, monitor_mask);
		}
		return (0);
	}

	/* new dir */
	if (paddr->pfield == (void *) &pmr->dir) {
		if (pmr->foff) {
			pmr->val = pmr->dval * dir + pmr->off;
			MARK(M_VAL);
		} else {
			pmr->off = pmr->val - pmr->dval * dir;
			MARK(M_OFF);
		}
		pmr->rbv = pmr->drbv * dir + pmr->off;
		if (dir_positive) {
			pmr->hlm = pmr->dhlm + pmr->off;
			pmr->llm = pmr->dllm + pmr->off;
		} else {
			pmr->hlm = -(pmr->dllm) + pmr->off;
			pmr->llm = -(pmr->dhlm) + pmr->off;
		}
		MARK(M_RBV);
		MARK(M_HLM);
		MARK(M_LLM);
		return (0);
	}

	/* new offset */
	if (paddr->pfield == (void *) &pmr->off) {
		pmr->val = pmr->dval * dir + pmr->off;
		pmr->lval = pmr->ldvl * dir + pmr->off;
		pmr->rbv = pmr->drbv * dir + pmr->off;
		if (dir_positive) {
			pmr->hlm = pmr->dhlm + pmr->off;
			pmr->llm = pmr->dllm + pmr->off;
		} else {
			pmr->hlm = -(pmr->dllm) + pmr->off;
			pmr->llm = -(pmr->dhlm) + pmr->off;
		}
		MARK(M_VAL);
		MARK(M_RBV);
		MARK(M_HLM);
		MARK(M_LLM);
		return (0);
	}

	/* new user high limit */
	if (paddr->pfield == (void *) &pmr->hlm) {
		if (dir_positive) {
			pmr->dhlm = pmr->hlm - pmr->off;
			MARK(M_DHLM);
		} else {
			pmr->dllm = -(pmr->hlm - pmr->off);
			MARK(M_DLLM);
		}
		MARK(M_HLM);
		return (0);
	}

	/* new user low limit */
	if (paddr->pfield == (void *) &pmr->llm) {
		if (dir_positive) {
			pmr->dllm = pmr->llm - pmr->off;
			MARK(M_DLLM);
		} else {
			pmr->dhlm = -(pmr->llm - pmr->off);
			MARK(M_DHLM);
		}
		MARK(M_LLM);
		return (0);
	}

	/* new dial high limit */
	if (paddr->pfield == (void *) &pmr->dhlm) {
		/* recalc user limit */
		if (dir_positive) {
			pmr->hlm = pmr->dhlm + pmr->off;
			MARK(M_HLM);
		} else {
			pmr->llm = -pmr->dhlm + pmr->off;
			MARK(M_LLM);
		}
		MARK(M_DHLM);
		return (0);
	}

	/* new dial low limit */
	if (paddr->pfield == (void *) &pmr->dllm) {
		/* recalc user limit */
		if (dir_positive) {
			pmr->llm = pmr->dllm + pmr->off;
			MARK(M_LLM);
		} else {
			pmr->hlm = -pmr->dllm + pmr->off;
			MARK(M_HLM);
		}
		MARK(M_DLLM);
		return (0);
	}

	/* new frac (move fraction) */
	if (paddr->pfield == (void *) &pmr->frac) {
		/* enforce limit */
		if (pmr->frac < 0.1) {
			pmr->frac = 0.1;
			changed = 1;
		}
		if (pmr->frac > 1.5) {
			pmr->frac = 1.5;
			changed = 1;
		}
		if (changed)
			db_post_events(pmr, &pmr->frac, monitor_mask);
		return (0);
	}

	/* new mres: make urev agree, and change (velo,bvel,vbas) to leave */
	/* (s,sbak,sbas) constant */
	if (paddr->pfield == (void *) &pmr->mres) {
		MARK(M_MRES); /* MARK it so we'll remember to tell device support */
		if (pmr->urev != pmr->mres * pmr->srev) {
			pmr->urev = pmr->mres * pmr->srev;
			MARK(M_UREV);
		}
		if (pmr->velo != fabs(pmr->urev) * pmr->s) {
			pmr->velo = fabs(pmr->urev) * pmr->s;
			MARK(M_VELO);
		}
		if (pmr->vbas != fabs(pmr->urev) * pmr->sbas) {
			pmr->vbas = fabs(pmr->urev) * pmr->sbas;
			MARK(M_VBAS);
		}
		if (pmr->bvel != fabs(pmr->urev) * pmr->sbak) {
			pmr->bvel = fabs(pmr->urev) * pmr->sbak;
			MARK(M_BVEL);
		}
		return (0);
	}

	/* new urev: make mres agree, and change (velo,bvel,vbas) to leave */
	/* (s,sbak,sbas) constant */

	if (paddr->pfield == (void *) &pmr->urev) {
		if (pmr->mres != pmr->urev / pmr->srev) {
			pmr->mres = pmr->urev / pmr->srev;
			MARK(M_MRES);
		}
		if (pmr->velo != fabs(pmr->urev) * pmr->s) {
			pmr->velo = fabs(pmr->urev) * pmr->s;
			MARK(M_VELO);
		}
		if (pmr->vbas != fabs(pmr->urev) * pmr->sbas) {
			pmr->vbas = fabs(pmr->urev) * pmr->sbas;
			MARK(M_VBAS);
		}
		if (pmr->bvel != fabs(pmr->urev) * pmr->sbak) {
			pmr->bvel = fabs(pmr->urev) * pmr->sbak;
			MARK(M_BVEL);
		}
		return (0);
	}

	/* new srev: make mres agree */
	if (paddr->pfield == (void *) &pmr->srev) {
		if (pmr->srev <= 0) {
			pmr->srev = 200;
			MARK(M_SREV);
		}
		if (pmr->mres != pmr->urev / pmr->srev) {
			pmr->mres = pmr->urev / pmr->srev;
			MARK(M_MRES);
		}
		return (0);
	}

	/* new eres (encoder resolution) */
	if (paddr->pfield == (void *) &pmr->eres) {
		MARK(M_ERES); /* MARK it so we'll remember to tell device support */
		return (0);
	}

	/* new ueip flag */
	if (paddr->pfield == (void *) &pmr->ueip) {
		MARK(M_UEIP); /* MARK it so we'll remember to tell device support */
		/* Ideally, we should be recalculating speeds, but at the moment */
		/* we don't know whether hardware even has an encoder. */
		return (0);
	}

	/* new urip flag */
	if (paddr->pfield == (void *) &pmr->urip) {
		return (0);
	}

	/* Set to SET mode  */
	if (paddr->pfield == (void *) &pmr->sset) {
		pmr->set = 1;
		db_post_events(pmr, &pmr->set, DBE_VALUE);
		return (0);
	}

	/* Set to USE mode  */
	if (paddr->pfield == (void *) &pmr->suse) {
		pmr->set = 0;
		db_post_events(pmr, &pmr->set, DBE_VALUE);
		return (0);
	}

	/* Set freeze-offset to freeze mode */
	if (paddr->pfield == (void *) &pmr->fof) {
		pmr->foff = 1;
		db_post_events(pmr, &pmr->foff, DBE_VALUE);
		return (0);
	}

	/* Set freeze-offset to variable mode */
	if (paddr->pfield == (void *) &pmr->vof) {
		pmr->foff = 0;
		db_post_events(pmr, &pmr->foff, DBE_VALUE);
		return (0);
	}
	return (0);
}


/******************************************************************************
	get_value()
*******************************************************************************/
static long
get_value(struct hmotorRecord * pmr, struct valueDes * pvdes)
{
	Debug(4, "get_value: entry%c\n", ' ');
	pvdes->field_type = DBF_DOUBLE;
	pvdes->no_elements = 1;
/*	(double *) (pvdes->pvalue) = &pmr->val;*/
	pvdes->pvalue = &pmr->val;
	return (0);
}

/******************************************************************************
	get_units()
*******************************************************************************/
static long
get_units(struct dbAddr * paddr, char *units)
{
	struct hmotorRecord *pmr = (struct hmotorRecord *) paddr->precord;
	int siz = dbr_units_size-1; /* "dbr_units_size" from dbAccess.h */
	char s[30];

	Debug(4, "get_units: entry%c\n", ' ');
	if ((paddr->pfield == (void *) &pmr->velo) ||
	    (paddr->pfield == (void *) &pmr->bvel) ||
	    (paddr->pfield == (void *) &pmr->vbas)) {
		strcpy(s, pmr->egu);
		strcat(s, "/sec");
	} else if ((paddr->pfield == (void *) &pmr->accl) ||
		   (paddr->pfield == (void *) &pmr->bacc)) {
		strcpy(s, "sec");
	} else if ((paddr->pfield == (void *) &pmr->s) ||
	    (paddr->pfield == (void *) &pmr->sbas) ||
	    (paddr->pfield == (void *) &pmr->sbak)) {
		strcpy(s, "rev/sec");
	} else if (paddr->pfield == (void *) &pmr->srev) {
		strcpy(s, "steps/rev");
	} else if (paddr->pfield == (void *) &pmr->urev) {
		strcpy(s, pmr->egu);
		strcat(s, "/rev");
	} else {
		strcpy(s, pmr->egu);
	}
	s[siz] = '\0';
	strncpy(units,s,siz+1);
	return (0);
}

/******************************************************************************
	get_graphic_double()
*******************************************************************************/
static long
get_graphic_double(struct dbAddr * paddr, struct dbr_grDouble * pgd)
{
	struct hmotorRecord *pmr = (struct hmotorRecord *) paddr->precord;

	Debug(4, "get_graphic_double: entry%c\n", ' ');
	if ((paddr->pfield == (void *) &pmr->val) ||
	    (paddr->pfield == (void *) &pmr->rbv)) {
		pgd->upper_disp_limit = pmr->hlm;
		pgd->lower_disp_limit = pmr->llm;
	} else if ((paddr->pfield == (void *) &pmr->dval) ||
		   (paddr->pfield == (void *) &pmr->drbv)) {
		pgd->upper_disp_limit = pmr->dhlm;
		pgd->lower_disp_limit = pmr->dllm;
	} else if ((paddr->pfield == (void *) &pmr->rval) ||
		   (paddr->pfield == (void *) &pmr->rrbv)) {
		if (pmr->res >= 0) {
			pgd->upper_disp_limit = pmr->dhlm / pmr->res;
			pgd->lower_disp_limit = pmr->dllm / pmr->res;
		} else {
			pgd->upper_disp_limit = pmr->dllm / pmr->res;
			pgd->lower_disp_limit = pmr->dhlm / pmr->res;
		}
	} else {
		recGblGetGraphicDouble(paddr, pgd);
	}

	return (0);
}

/******************************************************************************
	get_control_double()
*******************************************************************************/
static long
get_control_double(struct dbAddr * paddr, struct dbr_ctrlDouble * pcd)
{
	struct hmotorRecord *pmr = (struct hmotorRecord *) paddr->precord;

	Debug(4, "get_control_double: entry%c\n", ' ');
	if ((paddr->pfield == (void *) &pmr->val) ||
	    (paddr->pfield == (void *) &pmr->rbv)) {
		pcd->upper_ctrl_limit = pmr->hlm;
		pcd->lower_ctrl_limit = pmr->llm;
	} else if ((paddr->pfield == (void *) &pmr->dval) ||
		   (paddr->pfield == (void *) &pmr->drbv)) {
		pcd->upper_ctrl_limit = pmr->dhlm;
		pcd->lower_ctrl_limit = pmr->dllm;
	} else if ((paddr->pfield == (void *) &pmr->rval) ||
		   (paddr->pfield == (void *) &pmr->rrbv)) {
		if (pmr->res >= 0) {
			pcd->upper_ctrl_limit = pmr->dhlm / pmr->res;
			pcd->lower_ctrl_limit = pmr->dllm / pmr->res;
		} else {
			pcd->upper_ctrl_limit = pmr->dllm / pmr->res;
			pcd->lower_ctrl_limit = pmr->dhlm / pmr->res;
		}
	} else {
		recGblGetControlDouble(paddr, pcd);
	}

	return (0);
}

/******************************************************************************
	get_precision()
*******************************************************************************/
static long
get_precision(struct dbAddr * paddr, long *precision)
{
	struct hmotorRecord *pmr = (struct hmotorRecord *) paddr->precord;

	Debug(4, "get_precision: entry%c\n", ' ');
	if ((paddr->pfield == (void *) &pmr->rrbv) ||
	    (paddr->pfield == (void *) &pmr->rmp) ||
	    (paddr->pfield == (void *) &pmr->rep)) {
		*precision = 0;
	} else if (paddr->pfield == (void *) &pmr->vers) {
		*precision = 2;
	} else {
		*precision = pmr->prec;
	}
	/* recGblGetPrec(paddr,precision); */
	return (0);
}



/******************************************************************************
	get_alarm_double()
*******************************************************************************/
static long
get_alarm_double(struct dbAddr * paddr, struct dbr_alDouble * pad)
{
	struct hmotorRecord *pmr = (struct hmotorRecord *) paddr->precord;

	Debug(4, "get_alarm_double: entry%c\n", ' ');
	if (paddr->pfield == (void *) &pmr->val ||
	    paddr->pfield == (void *) &pmr->dval) {
		pad->upper_alarm_limit = pmr->hihi;
		pad->upper_warning_limit = pmr->high;
		pad->lower_warning_limit = pmr->low;
		pad->lower_alarm_limit = pmr->lolo;
	} else {
		recGblGetAlarmDouble(paddr, pad);
	}

	return (0);
}


/******************************************************************************
	alarm()
*******************************************************************************/
static void
alarm(struct hmotorRecord * pmr)
{

	Debug(4, "alarm: entry%c\n", ' ');
	if (pmr->udf == TRUE) {
		recGblSetSevr(pmr, UDF_ALARM, INVALID_ALARM);
		return;
	}
	/* limit-switch and soft-limit violations */
	if (pmr->hlsv && (pmr->hls || (pmr->dval > pmr->dhlm))) {
		recGblSetSevr(pmr, HIGH_ALARM, pmr->hlsv);
		return;
	}
	if (pmr->hlsv && (pmr->lls || (pmr->dval < pmr->dllm))) {
		recGblSetSevr(pmr, LOW_ALARM, pmr->hlsv);
		return;
	}
	return;
}


/******************************************************************************
	monitor()
*******************************************************************************/
static void
monitor(struct hmotorRecord * pmr)
{
	unsigned short  monitor_mask;

	Debug(4, "monitor: entry\n%c", ' ');
	monitor_mask = recGblResetAlarms(pmr);

	monitor_mask |= (DBE_VALUE | DBE_LOG);

	/*
	 * Mark .val, .dval changes, and save old values for backlash
	 * correction.
	 */
	if (pmr->val != pmr->lval)
		MARK(M_VAL);
	if (pmr->dval != pmr->ldvl)
		MARK(M_DVAL);
	if (pmr->rval != pmr->lrvl)
		MARK(M_RVAL);

	/* Catch all previous 'calls' to MARK(). */
	post_MARKed_fields(pmr, monitor_mask);
	return;
}


/******************************************************************************
	post_MARKed_fields()
*******************************************************************************/
static void
post_MARKed_fields(struct hmotorRecord * pmr, unsigned short mask)
{
	if (MARKED(M_VAL)) {
		db_post_events(pmr, &pmr->val, mask);
		UNMARK(M_VAL);
	}
	if (MARKED(M_DVAL)) {
		db_post_events(pmr, &pmr->dval, mask);
		UNMARK(M_DVAL);
	}
	if (MARKED(M_RVAL)) {
		db_post_events(pmr, &pmr->rval, mask);
		UNMARK(M_RVAL);
	}
	if (MARKED(M_DMOV)) {
		db_post_events(pmr, &pmr->dmov, mask);
		UNMARK(M_DMOV);
	}
	if (MARKED(M_TDIR)) {
		db_post_events(pmr, &pmr->tdir, mask);
		UNMARK(M_TDIR);
	}
	if (MARKED(M_MOVN)) {
		db_post_events(pmr, &pmr->movn, mask);
		UNMARK(M_MOVN);
	}
	if (MARKED(M_RRBV)) {
		db_post_events(pmr, &pmr->rrbv, mask);
		UNMARK(M_RRBV);
	}
	if (MARKED(M_RMP)) {
		db_post_events(pmr, &pmr->rmp, mask);
		UNMARK(M_RMP);
	}
	if (MARKED(M_REP)) {
		db_post_events(pmr, &pmr->rep, mask);
		UNMARK(M_REP);
	}
	if (MARKED(M_MSTA)) {
		db_post_events(pmr, &pmr->msta, mask);
		UNMARK(M_MSTA);
	}
	if (MARKED(M_MIP)) {
		db_post_events(pmr, &pmr->mip, mask);
		UNMARK(M_MIP);
	}
	if (MARKED(M_DIFF)) {
		db_post_events(pmr, &pmr->diff, mask);
		UNMARK(M_DIFF);
	}
	if (MARKED(M_RDIF)) {
		db_post_events(pmr, &pmr->rdif, mask);
		UNMARK(M_RDIF);
	}

	/* short circuit: less frequently posted PV's go below this line. */

	if ((pmr->mmap == 0) && (pmr->nmap == 0)) return;

	if (MARKED(M_HLM))
		db_post_events(pmr, &pmr->hlm, mask);
	if (MARKED(M_LLM))
		db_post_events(pmr, &pmr->llm, mask);
	if (MARKED(M_SPMG))
		db_post_events(pmr, &pmr->spmg, mask);
	if (MARKED(M_RCNT))
		db_post_events(pmr, &pmr->rcnt, mask);
	if (MARKED(M_RLV))
		db_post_events(pmr, &pmr->rlv, mask);
	if (MARKED(M_OFF))
		db_post_events(pmr, &pmr->off, mask);
	if (MARKED(M_RBV))
		db_post_events(pmr, &pmr->rbv, mask);
	if (MARKED(M_DHLM))
		db_post_events(pmr, &pmr->dhlm, mask);
	if (MARKED(M_DLLM))
		db_post_events(pmr, &pmr->dllm, mask);
	if (MARKED(M_DRBV))
		db_post_events(pmr, &pmr->drbv, mask);
	if (MARKED(M_HLS)) {
		db_post_events(pmr, &pmr->hls, mask);
		if ((pmr->dir == HMOTOR_DIR_POS) == (pmr->res >= 0)) {
			db_post_events(pmr, &pmr->rhls, mask);
		} else {
			db_post_events(pmr, &pmr->rlls, mask);
		}
	}
	if (MARKED(M_LLS)) {
		db_post_events(pmr, &pmr->lls, mask);
		if ((pmr->dir == HMOTOR_DIR_POS) == (pmr->res >= 0)) {
			db_post_events(pmr, &pmr->rlls, mask);
		} else {
			db_post_events(pmr, &pmr->rhls, mask);
		}
	}
	if (MARKED(M_ATHM))
		db_post_events(pmr, &pmr->athm, mask);
	if (MARKED(M_MRES))
		db_post_events(pmr, &pmr->mres, mask);
	if (MARKED(M_ERES))
		db_post_events(pmr, &pmr->eres, mask);
	if (MARKED(M_UEIP))
		db_post_events(pmr, &pmr->ueip, mask);
	if (MARKED(M_URIP))
		db_post_events(pmr, &pmr->urip, mask);
	if (MARKED(M_LVIO))
		db_post_events(pmr, &pmr->lvio, mask);
	if (MARKED(M_RDBD))
		db_post_events(pmr, &pmr->rdbd, mask);

	if (MARKED(M_S))
		db_post_events(pmr, &pmr->s, mask);
	if (MARKED(M_SBAS))
		db_post_events(pmr, &pmr->sbas, mask);
	if (MARKED(M_SBAK))
		db_post_events(pmr, &pmr->sbak, mask);
	if (MARKED(M_SREV))
		db_post_events(pmr, &pmr->srev, mask);
	if (MARKED(M_UREV))
		db_post_events(pmr, &pmr->urev, mask);
	if (MARKED(M_VELO))
		db_post_events(pmr, &pmr->velo, mask);
	if (MARKED(M_VBAS))
		db_post_events(pmr, &pmr->vbas, mask);
	if (MARKED(M_BVEL))
		db_post_events(pmr, &pmr->bvel, mask);
	if (MARKED(M_MISS))
		db_post_events(pmr, &pmr->miss, mask);
	if (MARKED(M_ACCL))
		db_post_events(pmr, &pmr->accl, mask);
	if (MARKED(M_BACC))
		db_post_events(pmr, &pmr->bacc, mask);
	if (MARKED(M_LPWR))
		db_post_events(pmr, &pmr->lpwr, mask);
	UNMARK_ALL;
}


/******************************************************************************
	process_motor_info()
*******************************************************************************/
static void
process_motor_info(struct hmotorRecord * pmr)
{
	unsigned long   status = pmr->msta;
	double          old_drbv = pmr->drbv;
	double          old_rbv = pmr->rbv;
	long            old_rrbv = pmr->rrbv;
	short           old_tdir = pmr->tdir;
	short           old_movn = pmr->movn;
	short           old_hls = pmr->hls;
	short           old_lls = pmr->lls;
	short           old_athm = pmr->athm;
	int             dir = (pmr->dir == HMOTOR_DIR_POS) ? 1 : -1;

	/*** Process record fields. ***/

	Debug(4, "process_motor_info: entry%c\n", ' ');
	MARK(M_MSTA);

	/* Calculate raw and dial readback values. */
	if ((status & HMOTOR_EA_PRESENT) && pmr->ueip) {
		/* An encoder is present and the user wants us to use it. */
		pmr->rrbv = pmr->rep;
	} else {
		pmr->rrbv = pmr->rmp;
	}
	pmr->drbv = pmr->rrbv * pmr->res;
	MARK(M_RMP);
	MARK(M_REP);
	if (pmr->rrbv != old_rrbv)
		MARK(M_RRBV);
	if (pmr->drbv != old_drbv)
		MARK(M_DRBV);

	/* Calculate user readback value. */
	pmr->rbv = dir * pmr->drbv + pmr->off;
	if (pmr->rbv != old_rbv)
		MARK(M_RBV);

	/* Get current or most recent direction. */
	pmr->tdir = (status & HMOTOR_RA_DIRECTION) ? 1 : 0;
	if (pmr->tdir != old_tdir)
		MARK(M_TDIR);

	/* Get motor-now-moving indicator. */
	pmr->movn = (status & (HMOTOR_RA_DONE | HMOTOR_RA_PROBLEM | HMOTOR_RA_OVERTRAVEL)) ? 0 : 1;
	if (pmr->movn != old_movn)
		MARK(M_MOVN);
	if (status & HMOTOR_RA_PROBLEM)
		Debug(1, "process_motor_info: HMOTOR_RA_PROBLEM%c\n", ' ');

	/* Get velocity indicator */
/*     pmr->cvel = (status & HMOTOR_RA_MOVING) ? TRUE : FALSE; */

	/* Get states of high, low limit switches. */
	pmr->rhls = (status & HMOTOR_RA_OVERTRAVEL) && pmr->tdir;
	pmr->rlls = (status & HMOTOR_RA_OVERTRAVEL) && !pmr->tdir;
	pmr->hls = ((pmr->dir == HMOTOR_DIR_POS) == (pmr->res >= 0)) ? pmr->rhls : pmr->rlls;
	pmr->lls = ((pmr->dir == HMOTOR_DIR_POS) == (pmr->res >= 0)) ? pmr->rlls : pmr->rhls;
	if (pmr->hls != old_hls)
		MARK(M_HLS);
	if (pmr->lls != old_lls)
		MARK(M_LLS);

	/* Get state of motor's or encoder's home switch. */
	if ((status & HMOTOR_EA_PRESENT) && pmr->ueip) {
		pmr->athm = (status & HMOTOR_EA_HOME) ? 1 : 0;
	} else {
		pmr->athm = (status & HMOTOR_RA_HOME) ? 1 : 0;
	}
	if (pmr->athm != old_athm)
		MARK(M_ATHM);


	/*
	 * If we've got an external readback device, get Dial readback from
	 * it, and propagate to User readback. We do this after motor and
	 * encoder readbacks have been read and propagated to .rbv in case
	 * .rdbl is a link involving that field.
	 */
	if (pmr->urip && pmr->rdbl.type == DB_LINK) {
		long            status, options = 0, nRequest = 1;

		Debug(5, "process_motor_info: getting ReadbackLink value%c\n", ' ');
/*		printf( "process_motor_info: getting ReadbackLink value\n" );*/
		old_drbv = pmr->drbv;
		status = recGblGetLinkValue(&(pmr->rdbl), (void *) pmr, DBR_DOUBLE,
					 &(pmr->drbv), &options, &nRequest);
		if (!RTN_SUCCESS(status)) {
			Debug(5, "process_motor_info: get ReadbackLink failed%c\n", ' ');
			pmr->drbv = old_drbv;
		} else {
			Debug(5, "process_motor_info: ReadbackLink value = %f\n", pmr->drbv);
			pmr->drbv *= pmr->rres;
			pmr->rbv = pmr->drbv * dir + pmr->off;
			if (pmr->drbv != old_drbv) {
				MARK(M_DRBV);
				MARK(M_RBV);
			}
		}
	}
	pmr->diff = pmr->dval - pmr->drbv;
	MARK(M_DIFF);
	pmr->rdif = NINT(pmr->diff / pmr->res);
	MARK(M_RDIF);
}

/* Calc and load new raw position into motor w/out moving it. */
static void
load_pos(struct hmotorRecord * pmr)
{
	struct hmotor_dset *pdset = (struct hmotor_dset *) (pmr->dset);
	double          newpos = pmr->dval / pmr->res;

	Debug(3, "load_pos: entry%c\n", ' ');
	pmr->ldvl = pmr->dval;
	pmr->lval = pmr->val;
	pmr->lrvl = pmr->rval = newpos;

	if (pmr->foff) {
		/* Translate dial value to user value. */
		if (pmr->dir == HMOTOR_DIR_POS) {
			pmr->val = pmr->off + pmr->dval;
		} else {
			pmr->val = pmr->off - pmr->dval;
		}
		MARK(M_VAL);
	} else {
		/* Translate dial limits to user limits. */
		if (pmr->dir == HMOTOR_DIR_POS) {
			pmr->off = pmr->val - pmr->dval;
			pmr->hlm = pmr->dhlm + pmr->off;
			pmr->llm = pmr->dllm + pmr->off;
		} else {
			pmr->off = pmr->val + pmr->dval;
			pmr->hlm = -(pmr->dllm) + pmr->off;
			pmr->llm = -(pmr->dhlm) + pmr->off;
		}
		MARK(M_OFF);
		MARK(M_HLM);
		MARK(M_LLM);
	}
	pmr->mip = MIP_LOAD_P;
	MARK(M_MIP);
	pmr->pp = TRUE;

	/* Load pos. into motor controller.  Get new readback vals. */
	INIT_MSG();
	WRITE_MSG(HMOTOR_LOAD_POS, &newpos);
	SEND_MSG();
	INIT_MSG();
	WRITE_MSG(HMOTOR_GET_INFO, NULL);
	SEND_MSG();
}

static void check_speed_and_resolution(struct hmotorRecord * pmr)
{
	/*
	 * Reconcile two different ways of specifying speed, resolution,
	 * and make sure things are sane.
	 */

	/* SREV (steps/revolution) must be sane. */
	if (pmr->srev <= 0) {
		pmr->srev = 200;
		MARK(M_SREV);
	}

	/* UREV (EGU/revolution) <--> MRES (EGU/step) */
	if (pmr->urev != 0.0) {
		pmr->mres = pmr->urev / pmr->srev;
		MARK(M_MRES);
	}
	if (pmr->mres == 0.0) {
		pmr->mres = 1.0;
		MARK(M_MRES);
	}
	if (pmr->urev != pmr->mres * pmr->srev) {
		pmr->urev = pmr->mres * pmr->srev;
		MARK(M_UREV);
	}

	/* S (revolutions/sec) <--> VELO (EGU/sec) */
	if (pmr->velo < 0.0) {
		pmr->velo = fabs(pmr->velo);
		MARK(M_VELO);
	}
	if (pmr->s > 0.0) {
		pmr->velo = fabs(pmr->urev) * pmr->s;
		MARK(M_VELO);
	}
	if ((pmr->urev != 0.0) && (pmr->s != pmr->velo / fabs(pmr->urev))) {
		pmr->s = pmr->velo / fabs(pmr->urev);
		MARK(M_S);
	}

	/* SBAS (revolutions/sec) <--> VBAS (EGU/sec) */
	if (pmr->vbas < 0.0) {
		pmr->vbas = fabs(pmr->vbas);
		MARK(M_VBAS);
	}
	if (pmr->sbas > 0.0) {
		pmr->vbas = fabs(pmr->urev) * pmr->sbas;
		MARK(M_VBAS);
	}
	if ((pmr->urev != 0.0) && (pmr->sbas != pmr->vbas / fabs(pmr->urev))) {
		pmr->sbas = pmr->vbas / fabs(pmr->urev);
		MARK(M_SBAS);
	}

	/* SBAK (revolutions/sec) <--> BVEL (EGU/sec) */
	if (pmr->bvel < 0.0) {
		pmr->bvel = fabs(pmr->bvel);
		MARK(M_BVEL);
	}
	if (pmr->sbak > 0.0) {
		pmr->bvel = fabs(pmr->urev) * pmr->sbak;
		MARK(M_BVEL);
	}
	if ((pmr->urev != 0.0) && (pmr->sbak != pmr->bvel / fabs(pmr->urev))) {
		pmr->sbak = pmr->bvel / fabs(pmr->urev);
		MARK(M_SBAK);
	}

	/* Sanity check on acceleration time. */
	if (pmr->accl == 0.0) {
		pmr->accl = 0.1;
		MARK(M_ACCL);
	}
	if (pmr->bacc == 0.0) {
		pmr->bacc = 0.1;
		MARK(M_BACC);
	}
}
