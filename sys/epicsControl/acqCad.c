static struct {
    void *v;
    char *c;
} rcsid = {
    &rcsid,
    "$Id: acqCad.c,v 1.6 2017/09/07 02:33:09 gemvx Exp $"
};


/*
 * Copyright 1997 Association of Universities for Research in Astronomy, Inc.
 * See the file COPYRIGHT for more details.
 *
 * FILENAME
 * 	acqCad.c
 *
 * DESCRIPTION 
 * 	acqCad  support routines
 *
 * 
 * FUNCTION NAME(S)
 *	acqDatmCad
 *	acqParkCad
 *	acqStepsCad
 *	acqPosCad
 *	acqCtrl
 *   
 * DEPENDENCIES
 * 	EPICS support libraries
 *
 *
 *INDENT-OFF*
 * $Log: acqCad.c,v $
 * Revision 1.6  2017/09/07 02:33:09  gemvx
 * Revert to using FW1 for anti-squiggle code (V1-13).
 *
 * Revision 1.4  2016/05/04 21:26:48  gemvx
 * Fix typo
 *
 * Revision 1.3  2013/06/07 00:13:52  gemvx
 * More minor tweaks for Jira-1149 before shipping it.
 *
 * Revision 1.2  2013/06/06 01:54:27  gemvx
 * Checking in for fixes related to REL-1149.
 *
 * REL-1149 requires moving FW1 into a blocking position when the aquisition
 * mirror is moving out. This will inhibit, "squiggles" on the detector caused
 * by a bright star reflecting off of the aquisition mirror onto the detector.
 *
 * See: http://swgserv01.cl.gemini.edu:8080/browse/REL-1149
 *
 * tom.c
 *
 * Revision 1.1  2009/06/10 15:05:08  gemvx
 * Added Files:
 * epicsControl diectory populated.
 *
 * Code added to support http://swgserv01.cl.gemini.edu:8080/browse/REL-1149.
 * "Modify GNIRS acquisition mirror movement sequence"
 * These changes moves fw1 into a, "safe" position when moving the acq mirror
 * from "in" to "out" so the star doesn't streak the detector.
 *
 * INDENT-ON 
 */




/* VxWorks include files */
#include <vxWorks.h>
#include <taskLib.h>
#include <unistd.h>

/* EPICS specific include files */
/* #include "gnirsCcDefs.h" */
#include "gnirsTasks.h"
#include "epCommon.h"
#include "epicsNames.h"
#include "gnirsCC.h"

/* Include file needed for control tasks */
#include <sysLib.h>
#include <car.h>


/*
 * List of up to four things for acqCtrl to do. If there's less than four things, arg[n][0] s/b 0.
 */

#define MAX_MOVE	4		/* Max movements in sequence	*/
#define MAX_THING	16		/* Max chars in think to do	*/

struct acqMsg {
    int mech;		/* FW1, ACQ, etc...		*/
    int op;		/* DATUM, PARK, STEPS, POS	*/
    char arg[MAX_THING];/* "Open", "123456", etc.	*/
};




/*
 * Some shortcuts to make access to pCad arguments more readable.
 */

#define acqCurPos ((char *)pCad->valb)
#define acqNewPos (pCad->b)
#define fw1CurPos (pCad->d)
#define fw1NewPos (pCad->c)
#define fw1Mark   (pCad->e)


/* Global variables */

static MSG_Q_ID acqMotorQ; 

/*
 * If the fw1 contains a filter in, "safety" we can move the acquisition
 * mirror without worries. Otherwise we have to move one of these in,
 * move the mirror, them move fw1 back.
 */

#define MAX_SAFETY      8
#define MAX_FILTER_NAME 32

static char safety[MAX_SAFETY][MAX_FILTER_NAME];

/* Forward declarations of the functions in this file */

static const char *whats[] = {"Datum", "Park", "Steps", "Pos"};
static int acqCtrl(int n1, int n2, int n3, int n4, int n5, int n6, int n7, int n8, int n9, int n10);
static long checkNamedPos(int mech, char *name);
static long acqGrokVMEStatus(long status, const char *who);
static long DoWeMoveFw1(struct cadRecord *pCad);

long acqDatmCad (struct cadRecord* pCad); 
long acqParkCad (struct cadRecord* pCad); 
long acqStepsCad(struct cadRecord* pCad); 
long acqPosCad  (struct cadRecord* pCad);

long acqDatmCadInit (struct cadRecord* pCad);
long acqParkCadInit (struct cadRecord* pCad);
long acqStepsCadInit(struct cadRecord* pCad);
long acqPosCadInit  (struct cadRecord* pCad);
long sendAcqPark();
long sendAcqDatum();
long readSafetyFilters(char *file);

long carVal = CAR_IDLE;


int findMechName(int mech, char *name, char *pos);

/*
 *+
 * FUNCTION NAME:    acq????CadInit
 *
 * INVOCATION:
 *      cad init routine
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 *	   ! struct cadRecord *pCad
 *
 * FUNCTION VALUE:
 *    status val
 *
 * PURPOSE:
 *    create message queue and spawn task to deal with acq cad records
 *
 * DESCRIPTION: 
 *
 * EXTERNAL VARIABLES:
 *	None
 *
 * PRIOR REQUIREMENTS:
 *	None
 *
 * DEFICIENCIES:
 *	
 *
 * HISTORY (optional):
 *
 *-
 */

long acqDatmCadInit( struct cadRecord* pCad )
{
    static int done = 0;
    static int acqId;

    gnirsLogMessage(CICS_DB_MIN, "acqDatmCadInit");

    /* park, datum, pos, and steps cads will call this, only do once*/

    if (done) return (CAD_ACCEPT);
    done = 1;

    gnirsLogMessage(CICS_DB_FULL, "create Msg Queue");

    acqMotorQ = msgQCreate(2, sizeof(struct acqMsg) * MAX_MOVE, MSG_Q_FIFO);

    if(acqMotorQ == NULL) {
	strncpy(MESSAGE, "Init: Error creating messageQ", MAX_STRING_SIZE - 1);
	return(CAD_REJECT);
    }
	
    gnirsLogMessage(CICS_DB_FULL, "spawn task");
	
    if ((acqId = taskSpawn("tAcqCtrl", 50, VX_FP_TASK, 4000, acqCtrl, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)) == ERROR) {
        gnirsLogMessage(CICS_DB_ERROR, "Init: Error Spawning task");
        return(CAD_REJECT);
    }

    return (CAD_ACCEPT);
}




long acqParkCadInit( struct cadRecord* pCad )
{
    
    gnirsLogMessage(CICS_DB_MIN, "acqParkCadInit"); 

    return CAD_ACCEPT;
}




long acqPosCadInit( struct cadRecord* pCad )
{
    gnirsLogMessage(CICS_DB_MIN, "acqPosCadInit");

    return CAD_ACCEPT;
}




long acqStepsCadInit( struct cadRecord* pCad )
{
   
    gnirsLogMessage(CICS_DB_MIN, "acqStepsCadInit");

    return CAD_ACCEPT;
}




/*
 *+
 * FUNCTION NAME:
 *	acqCtrl
 *
 * INVOCATION:
 *	Started as seperate task in initTasks.
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 *	Set of 10 integer variables, interpreted as variety of 
 *	parameters.
 *
 * FUNCTION VALUE:
 *	never returns
 *
 * PURPOSE:
 *
 * DESCRIPTION:
 *	Cad record releases semaphore which allows acqCtrl to run.  
 *
 * EXTERNAL VARIABLES:
 *	None
 *
 * PRIOR REQUIREMENTS:
 *	None
 *
 * DEFICIENCIES:
 *	
 *
 * HISTORY (optional):
 *
 *-
 */





static int acqCtrl(int n1, int n2, int n3, int n4, int n5, int n6, int n7, int n8, int n9, int n10 )
{
    long status = VME_OK;
    char dummy[MAX_STRING_SIZE];
    struct acqMsg msg[MAX_MOVE];
    char flags;
    int i;
 

    while(1) {

	gnirsLogMessage(CICS_DB_FULL, "Task tacqCtrl sleeping... ");
	
	if( msgQReceive(acqMotorQ, (char *)msg, sizeof(msg), WAIT_FOREVER ) == ERROR ) {
            gnirsLogMessage(CICS_DB_ERROR, "Error receiving messages from acqMotorQ");
	    if(setCar(ACQ_CAR, CAR_ERROR, ERROR, "setCar error in acqCtrl", dummy) != OK) {
		gnirsLogMessage(CICS_DB_ERROR, dummy);
		continue;
            }
	}

	gnirsLogMessage(CICS_DB_MIN, "Task tacqCtrl awoken...");

	/* Determine what mechanisms will be moving, and set their busy Car state */

        for (flags = i = 0; i < MAX_MOVE; ++i) {
            if (! msg[i].arg[0]) break;
            flags |= (1 << msg[i].mech);
        }

        if (flags & (1 << ACQ)) {
            if(setCar(ACQ_CAR, CAR_BUSY, OK, "", dummy) != OK) {
	        gnirsLogMessage(CICS_DB_ERROR, dummy);
                continue;		/* Blow this movement sequence off */
            }
	    gnirsLogMessage(CICS_DB_FULL, "Setting ACQ CAR BUSY");
        }

        if (flags & (1 << FW1)) {
            if(setCar(FW1_CAR, CAR_BUSY, OK, "", dummy) != OK) {
	        gnirsLogMessage(CICS_DB_ERROR, dummy);
                continue;		/* Blow this movement sequence off */
            }
	    gnirsLogMessage(CICS_DB_FULL, "Setting FW1 CAR BUSY");
        }

        /* Move the mechanisms */

        for (i = 0; i < MAX_MOVE; ++i) {
            if (! msg[i].arg[0]) break;

            switch(msg[i].op) {
		case STEPS:  setEngPos(ACQ, atoi(msg[i].arg));       break;
		case POS:    checkNamedPos(msg[i].mech, msg[i].arg); break;
            }

	    if ((status = moveMech(msg[i].mech, dummy)) != VME_OK) {
                char dum2[MAX_STRING_SIZE];
                if (setCar(ACQ_CAR, CAR_ERROR, status, dummy, dum2) != OK) {
		    acqGrokVMEStatus(status, whats[msg[i].op]);
                    break;
                }
            }

            if (msg[i].mech == FW1)
                putDbInfoT(dbTop, "fw1PosCad.VALB", dummy, DBF_STRING, msg[i].arg); /* Informational only, so who cares if it fails. */

        }

        if (status != VME_OK) continue;

        carVal = CAR_IDLE;    

        if (flags & (1 << ACQ)) {
            if(setCar(ACQ_CAR, CAR_IDLE, OK, "", dummy) != OK) {
                gnirsLogMessage(CICS_DB_ERROR, dummy);
            }
	    gnirsLogMessage(CICS_DB_FULL, "Setting ACQ CAR IDLE");
        }

        if (flags & (1 << FW1)) {
            if(setCar(FW1_CAR, CAR_IDLE, OK, "", dummy) != OK) {
                gnirsLogMessage(CICS_DB_ERROR, dummy);
            }
	    gnirsLogMessage(CICS_DB_FULL, "Setting ACQ CAR IDLE");
        }
    }
    
    return 0;	/* Should never return	*/
}





/*
 * Collapsed acq<function>Cad routines into one routine that handles all functions.
 */

static long acqCommonCad(struct cadRecord *, int);

/*
 * These are referenced from the capfast Cad symbol 
 */

long acqDatmCad (struct cadRecord* pCad ) { return(acqCommonCad(pCad, DATUM)); }
long acqParkCad (struct cadRecord* pCad ) { return(acqCommonCad(pCad, PARK )); }
long acqStepsCad(struct cadRecord* pCad ) { return(acqCommonCad(pCad, STEPS)); }
long acqPosCad  (struct cadRecord* pCad ) { return(acqCommonCad(pCad, POS  )); }

static long acqGrokVMEStatus(long status, const char *who) {

    switch(status) {
        case VME_ILLEGAL_MOTOR: gnirsLogMessage(CICS_DB_ERROR, "acq%sCad: Illegal motor",  who); break;
        case VME_ABORTED:       gnirsLogMessage(CICS_DB_ERROR, "acq%sCad: Aborted",        who); break;
        case VME_TIMEOUT:       gnirsLogMessage(CICS_DB_ERROR, "acq%sCad: Timed out",      who); break;
        case VME_DRIVE_FAULT:
        case VME_SENTORR:
        case VME_CABLE_HARDW:   gnirsLogMessage(CICS_DB_ERROR, "acq%sCad: Hardware error", who); break;
        case VME_MEMORY_ERROR:  gnirsLogMessage(CICS_DB_ERROR, "acq%sCad: Out of memory",  who); break;
        default:
            if (status != VME_OK) {
                gnirsLogMessage(CICS_DB_ERROR, "acq%sCad: General error", who);
            }
    }
    return(status);
}

static long acqPosCadMsgSetup(struct cadRecord *pCad, struct acqMsg *msg);
static long acqPosCadPRESET(struct cadRecord *pCad);




/*
 * Common routine for acqDatmCad, acqParkCad, acqStepsCad, and acqPosCad.
 */

static long acqCommonCad(struct cadRecord *pCad, int what) {
    struct acqMsg msg[MAX_MOVE];
    const char *who = whats[what];
    long status;

    switch( DIRECTIVE ) {
        case CAD_MARK:

  	    gnirsLogMessage(CICS_DB_FULL, "acq%sCad - MARK directive", who);
	    strcpy(MESSAGE, "");
	    return CAD_ACCEPT;

      case CAD_PRESET:	

  	    gnirsLogMessage(CICS_DB_FULL, "acq%sCad - PRESET directive", who);

            if ((what != DATUM) && (! atoi(pCad->f))) {
		gnirsLogMessage(CICS_DB_ERROR, "acq%sCad - Acq not Datumed", who);
                return CAD_REJECT;
            } 

            switch(what) {
		case DATUM: status = datumMech(ACQ);                           break;
		case PARK:  status = parkMech(ACQ);                            break;
		case STEPS: status = setEngPos(ACQ, atoi(cadInput(STEPSCAD))); break;
                case POS:   status = acqPosCadPRESET(pCad);                    break;
		default:
                    gnirsLogMessage(CICS_DB_FULL, "acq%sCad - Don't know what %d is.", who, what);
                    return CAD_REJECT;
            }

            if (acqGrokVMEStatus(status, who) != VME_OK) return CAD_REJECT;
         
	    return CAD_ACCEPT;

      case CAD_CLEAR:	

            gnirsLogMessage(CICS_DB_FULL, "acq%sCad - CLEAR directive", who);
	    strcpy(MESSAGE, "");
	    return CAD_ACCEPT;

      case CAD_START:	

            gnirsLogMessage(CICS_DB_FULL, "acq%sCad - START directive", who);

	    if (! strcmp(pCad->h, DISABLED)) {	 /* Cad disabled ? */
		gnirsLogMessage(CICS_DB_ERROR, "acq%sCad - Acq Observe in progress", who);
 		return CAD_REJECT;
	    }

	    if (carVal == CAR_BUSY) {
		gnirsLogMessage(CICS_DB_ERROR, "acq%sCad: Acq is busy", who);
		return CAD_REJECT;
            }

  	    carVal = CAR_BUSY;	

            memset(msg, 0, sizeof(msg));
	    msg[0].mech   = ACQ;
	    msg[0].op     = what;
	    msg[0].arg[0] = 1;	/* Make it non-zero	*/

            switch(what) {
		case DATUM:
		case PARK:                                                      break;
		case STEPS: strncpy(msg[0].arg, cadInput(STEPSCAD), MAX_THING); break;
                case POS:   acqPosCadMsgSetup(pCad, msg);                       break;
		default:
		    gnirsLogMessage(CICS_DB_ERROR, "acq%sCad: Don't know WHAT(%d) to start.", who, what);
		    return CAD_REJECT;
            }

            assignVal(type(POSCAD), cadInput(POSCAD), cadOutput(POSCAD), MESSAGE);

            if (msgQSend(acqMotorQ, (char *)msg, sizeof(msg), NO_WAIT, MSG_PRI_NORMAL) != OK) {
                gnirsLogMessage(CICS_DB_ERROR, "msgQSend error in acq%sCad", who);
                return CAD_REJECT;
	    }

            gnirsLogMessage(CICS_DB_FULL, "acq%sCad - START directive CAD_ACCEPT", who);
	    return CAD_ACCEPT;

      case CAD_STOP:   
		gnirsLogMessage(CICS_DB_FULL, "acq%sCad - STOP directive", who);

		if (carVal != CAR_BUSY) {
		    gnirsLogMessage(CICS_DB_ERROR, "acq%sCad: Acq is not busy", who);
		    return CAD_REJECT;	
		}
	
	        abortMotor(ACQ);

                carVal = CAR_IDLE;    

		return CAD_ACCEPT;
    }

    gnirsLogMessage(CICS_DB_ERROR, "acq%sCad: Unrecognized directive", who);
    return CAD_REJECT;
}





/*
 * Check pCad parameters to see if we need move FW1. If so, set pCad->valc so that
 * Fw1 will not get START'd so we can move the FW1. Do any other tasks that are needed
 * by, "PRESET".
 */

static long acqPosCadPRESET(struct cadRecord *pCad) {
    long status;

    gnirsLogMessage(CICS_DB_ERROR, "acqPosCad: Checking acq position \"%s\"", acqNewPos);

    if ((status = checkNamedPos(ACQ, acqNewPos)) != VME_OK) return status;

    /* If we have to move Fw1 ... */

    *(long *)pCad->valc = 0;
    if (DoWeMoveFw1(pCad)) {
        if ((status = checkNamedPos(FW1, fw1NewPos)) != VME_OK) return status;
        *(long *)pCad->valc = 1;	/* Clear Fw1Cad's MARK */
    }

    return VME_OK;
}






/*
 * Check if the named position to position to is valid. This also means checking
 * if there exists an encoder position for the named position.
 *
 * Returns one of the VME_* errors (hopefully, VME_OK!).
 */

static long checkNamedPos(int mech, char *name) {

    #define POS_LEN 16
    char pos[POS_LEN];

    if (! findMechName(mech, name, pos)) {
	gnirsLogMessage(CICS_DB_ERROR, "\"%s\" not found in database.", name);
	return VME_ILLEGAL_MOTOR;
    }

    gnirsLogMessage(CICS_DB_FULL, "\"%s\" position in database, \"%d\".", name, pos);

    /*
     * Gawd this is ugly. valid(ACQ, name) takes the name as the second argument,
     * valid(FW1, pos) takes the position returned from findMechName(..) as it's
     * second argument.
     */

    if (mech == ACQ) return valid(mech, name);
    if (mech == FW1) return valid(mech, pos);
    gnirsLogMessage(CICS_DB_FULL, "Mech %d is neither ACQ or FW1", mech);
    return VME_ILLEGAL_MOTOR;
}






/*
 * This sets up a sequence of moves for acqPosCad(). This fixes a problem where the acq
 * mirror leaves a squiggle on the detector caused by the reflection of a star as the
 * mirror is moving out. This code will detect the condition where it happens, and move
 * a blocking filter on FW1 into position before moving the acq mirror.
 *
 * This is accomplished by building up a sequence of steps to be performed by both
 * ACQ and FW1. E.g. (worse case) Move FW1 -> Dark, ACQ -> Out, FW1 -> Open.
 *
 *  pCad->valb = ACQ current position
 *  pCad->b    = ACQ demanded position
 *  pCad->c    = FW1 current position
 *  pCad->d    = FW1 demanded position
 *  pCad->e    = FW1 MARK'ed
 *
 */

static int IsItSafe(char *filter) ;

static long acqPosCadMsgSetup(struct cadRecord *pCad, struct acqMsg *msg) {
    int msgP = 0;


    gnirsLogMessage(CICS_DB_FULL, "ACQ curPos: \"%s\", ACQ newPos: \"%s\", FW1 curPos: \"%s\", FW1 newPos: \"%s\", FW1 Mark: \"%s\"",  acqCurPos, acqNewPos, fw1CurPos, fw1NewPos, fw1Mark);

    if (! DoWeMoveFw1(pCad)) {
        msg[msgP].mech = ACQ;
        msg[msgP].op   = POS;
        strncpy(msg[msgP].arg, acqNewPos, MAX_THING);
        ++msgP;
        msg[msgP].arg[0] = 0;
        gnirsLogMessage(CICS_DB_MIN, "Fw1 not being moved.");
        return CAD_ACCEPT;
    }

    if (IsItSafe(fw1CurPos)) {
        msg[msgP].mech = ACQ;
        msg[msgP].op   = POS;
        strncpy(msg[msgP].arg, acqNewPos, MAX_THING);
        ++msgP;
        msg[msgP].mech = FW1;
        msg[msgP].op   = POS;
        strncpy(msg[msgP].arg, fw1NewPos, MAX_THING);
        ++msgP;
        gnirsLogMessage(CICS_DB_MIN, "Acq mirror will be moved OUT first, then FW1 because the current FW1 is safe.");
    }
    else {
        if (IsItSafe(fw1NewPos)) {
            msg[msgP].mech = FW1;
            msg[msgP].op   = POS;
            strncpy(msg[msgP].arg, fw1NewPos, MAX_THING);
            ++msgP;
            msg[msgP].mech = ACQ;
            msg[msgP].op   = POS;
            strncpy(msg[msgP].arg, acqNewPos, MAX_THING);
            ++msgP;
            gnirsLogMessage(CICS_DB_MIN, "FW1 Will be moved first because the new position is safe, then the Acq mirror will be moved OUT.");
        }
        else {
            if (! safety[0][0]) {
                gnirsLogMessage(CICS_DB_ERROR, "acqPosCad - No safe filter to move to.");
                return CAD_REJECT;
            }
            msg[msgP].mech = FW1;
            msg[msgP].op   = POS;
            strncpy(msg[msgP].arg, safety[0], MAX_THING);
            ++msgP;
            msg[msgP].mech = ACQ;
            msg[msgP].op   = POS;
            strncpy(msg[msgP].arg, acqNewPos, MAX_THING);
            ++msgP;
            msg[msgP].mech = FW1;
            msg[msgP].op   = POS;
            strncpy(msg[msgP].arg, fw1NewPos, MAX_THING);
            ++msgP;
            gnirsLogMessage(CICS_DB_FULL, "Neither the current or new FW1 positions are safe.");
            gnirsLogMessage(CICS_DB_MIN, "First moving FW1 to \"%s\", then ACQ mirror to \"%s\", then FW1 to final position at \"%s\"",
                safety[0], acqNewPos, fw1NewPos);
        }
    }

    if (msgP < MAX_MOVE) msg[msgP].arg[0] = 0;

    return CAD_ACCEPT;
}





/*
 * Return true if we also have to move Fw1 to avoid squiggles.
 */

static long DoWeMoveFw1(struct cadRecord *pCad) {

    gnirsLogMessage(CICS_DB_FULL, "DoWeMoveFw1 fw1Mark:\"%s\" acqNewPos:\"%s\" acqCurPos:\"%s\".", fw1Mark, acqNewPos, acqCurPos);


    /* If we're moving the acq mirror in, we don't have the squiggles problem.		*/
    if (! strcmp(acqNewPos, "In")) return 0;

    /* If we're not moving the acq mirror, then we don't have a squiggles problem.	*/
    if (! strcmp(acqNewPos, acqCurPos)) return 0;

    /* If CurFw1 is unsafe, we have to move it.						*/
    if (! IsItSafe(fw1CurPos)) return 1;

    /* If Fw1 isn't moving (and we know it's safe) we don't have to move it.		*/
    if (! strcmp(fw1NewPos, fw1CurPos)) return 0;

    return 1;
}






/*
 * Check if filter is a safe filter to have in when we move the acquisition
 * mirror in. Return TRUE if it's found.
 */

static int IsItSafe(char *filter) {
    int i;

    for (i = 0; i < MAX_SAFETY; ++i) {
        if (! strcmp(filter, safety[i]))
            return 1;
    }
    return(0);
}




/*
 * Read a configuration file containing a list of filter wheel 1's filters that
 * are safe to have in position when moving the acquisition mirror out.
 */

long readSafetyFilters(char *file) {
    #define MAX_SAFETY_FILTERLINE 128
    char buffer[MAX_SAFETY_FILTERLINE + 1];
    int count;
    FILE *fd;

    memset(safety, 0, sizeof(safety));

    if ((fd = fopen(file, "r")) == NULL) {
        gnirsLogMessage(CICS_DB_ERROR, "Cannot open '%s'.", file);
        return VME_ERROR;
    }

    for (count = 0; fgets(buffer, MAX_SAFETY_FILTERLINE, fd) != NULL; ) {
        char *p, *t;

        if (count == MAX_SAFETY) {
            gnirsLogMessage(CICS_DB_ERROR, "Too many safety filter entries '%s'.", file);
            return VME_ERROR;
        }

        /* Clean up input; get rid of comments, null lines, spaces, etc. */

        *(strchr(buffer, '\n' )) = 0;
        if (strchr(buffer, '#' ) != NULL) continue;
        for (p = buffer; *p && ((*p == ' ') || (*p == '\t')); ++p) ; if (! *p) continue;
        for (t = p     ; *p &&  (*p != ' ') && (*p != '\t');  ++p) ; *p = 0;

        /* Got something valid, ship it */

        gnirsLogMessage(CICS_DB_MIN, "Read safe filter \"%s\".", t);

        if (checkNamedPos(FW1, t) == VME_OK)
            strcpy(safety[count++], t);

    }

    return count;
}


long DumpSafetyFilters() {
    int i;

    for (i = 0; i < MAX_SAFETY; ++i) {
        if (! safety[i][0]) break;
        gnirsLogMessage(CICS_DB_NONE, "Safety at %d \"%s\".", i, safety[i]);
    }
    return i;
}









/*
 * The following routines are called by DatumCad and ParkCad to kick off
 * Datum's or Park's in our context.
 */




/*
 * Called by both DatumCad and ParkCad to set our busy flag.
 */

long setacqCarBusy()
{
	carVal = CAR_BUSY;
	if(setCar(ACQ_CAR, carVal, OK, "", dummy) != OK) {
            gnirsLogMessage(CICS_DB_ERROR, dummy);
	    return ERROR;
	}
	return OK;
}




/*
 * Called by both DatumCad and ParkCad to get our busy state.
 */

long getacqCar()
{
	return carVal;
}






/*
 * Called from ParkCad to park the acq mirror.
 */

long sendAcqPark() {
    struct acqMsg msg[MAX_MOVE];

    memset(msg, 0, sizeof(msg));
    msg[0].mech   = ACQ;
    msg[0].op     = PARK;
    msg[0].arg[0] = 1;  /* Make it non-zero     */

    if (msgQSend(acqMotorQ, (char *)msg, sizeof(msg), NO_WAIT, MSG_PRI_NORMAL) != OK) {
        gnirsLogMessage(CICS_DB_ERROR, "msgQSend error in acqSendPark");
        return CAD_REJECT;
    }
    return CAD_ACCEPT;
}






/*
 * Called from DatumCad to datum the acq mirror.
 */

long sendAcqDatum() {
    struct acqMsg msg[MAX_MOVE];

    memset(msg, 0, sizeof(msg));
    msg[0].mech   = ACQ;
    msg[0].op     = DATUM;
    msg[0].arg[0] = 1;  /* Make it non-zero     */

    if (msgQSend(acqMotorQ, (char *)msg, sizeof(msg), NO_WAIT, MSG_PRI_NORMAL) != OK) {
        gnirsLogMessage(CICS_DB_ERROR, "msgQSend error in acqSendDatum");
        return CAD_REJECT;
    }
    return CAD_ACCEPT;
}
