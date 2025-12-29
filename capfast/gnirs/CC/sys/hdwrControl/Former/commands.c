static struct
  {
      void *v;
      char *c;
  }
rcsid =
{
    &rcsid,
        "$Id: commands.c,v 1.2 2009/05/27 19:32:06 fkraemer Exp $"
};

#include <vxWorks.h>
#include <stdio.h>
#include <fcntl.h>
#include <ioLib.h>
#include <vme.h>
#include <memLib.h>
#include <usrLib.h>             /* Debugging */
#include <cacheLib.h>
#include <taskLib.h>
#include <sysLib.h>
#include <intLib.h>
#include <logLib.h>
#include <iv.h>
#include <vxLib.h>
#include <ctype.h>
#include "gnirsCC.h"
#include "timeLib.h"
#include "math.h"


#define HW_REG32 volatile unsigned long

#define BIT_SET(p, d)      { __typeof__ (* (p)) __temp = (* (p));        \
                             * (p) = __temp | (d); }

#define BUS_RESET_REG_MV167   0xfff40060   /* Bus reset register for MVME167 */
#define BUS_RESET_BIT_MV167   0x01800000   /* Reset-Switch-Enable and
                                            * Bus-Reset bits
                                            */

void wfsBusReset(void)
{

    gnirsLogMessage(CICS_DB_ERROR,
            "wfsBusReset: BUS RESET - SYSTEM WILL REBOOT.\n");

    /* Brief pause to allow message to flush..   */
    taskDelay(4 * gnirsG.clockRate);

    /* ..then waggle the hardware bits            */
    BIT_SET((HW_REG32 *) BUS_RESET_REG_MV167, BUS_RESET_BIT_MV167);
}

int gnirsReboot()
{

    taskSpawn("suicide", 20, VX_NO_STACK_FILL, 2000, (FUNCPTR) wfsBusReset,
              0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

    return VME_OK;
}

int gnirsTest()
{
    return testHardware();
}

/* Move a motor by calling the motor task, and return success or
 * error flag.  If the latter, copy the error message into "message".
 * EPICS API uses this routine.
 */
int moveMech(int mech, char *message) {
    motorVars *m;
    int motor;

    motor = mechanism[mech].motor;
    if (motor < 0)
        return VME_ILLEGAL_MOTOR;
    m= motors[motor];
    if ((motor >= NUM_MOTORS) || !m) {
        mErrMsg(m, "status req: motor %d not allocated", motor);
        return VME_ILLEGAL_MOTOR;
    }

    moveOneMotor(motor);
    if (m->returnValue != VME_OK) {
        strncpy(message, m->errorMsg, EPICS_LEN);
        message[EPICS_LEN] = '\0';
    }
    return m->returnValue;
}

/* Call this to determine if the move(s) [parkAll, datumAll, doAll, etc.]
 * completed ok.  Set the global all datumed and all parked flags.  The
 * returned value is what the EPICS CAR record will see as the result of
 * the park/datum/apply/... call.
 */
int gnirsGlobalStatus() {
    int rc;
    int i;
    BOOL parked, datumed;
    motorVars *m;
    
    parked = datumed = TRUE;
    rc = G_DONE;    /* no error */
    for (i = 0; i < NUM_MOTORS; i++ ) {
        m = motors[i];
        if (!m)
            continue;
        if (!m->parked)
            parked = FALSE;
        if (!m->datumed)
            datumed = FALSE;
        if (m->status & MDS_ERROR)  /* XXX MDS_ERROR may not be used/useful */
            rc = G_ERROR;
    }
    gnirsG.datumed = datumed;
    gnirsG.parked = parked;
    return rc;
}

/* Set a position request where the position is just a number and not
 * something set in the configuration file.  As a subroutine, useful
 * for setting raw positions to be gone to via doAll, esp. during testing.
 * When a particular motor should be (immediately) driven to a give
 * position (during testing), use the 'position(motor, pos) command.
 * Note that there is no check for validity of the position.
 */
int setPositionReq(int mech, int where ) {
    motorVars *m;
    int motor;

    motor = mechanism[mech].motor;
    if (motor < 0)
        return VME_ILLEGAL_MOTOR;
    m = motors[motor];
    if (!m)
        return VME_ILLEGAL_MOTOR;
    m->reqArg = where;
    m->reqOp = position;
    return VME_OK;
}

/* parkMech and datumMech are for use by the doAll() or EPICS routines */
int parkMech(int mech) {
    motorVars *m;
    int motor;

    motor = mechanism[mech].motor;
    if (motor < 0)
        return VME_ILLEGAL_MOTOR;
    m = motors[motor];
    if (!m)
        return VME_ILLEGAL_MOTOR;
    m->reqOp = parkOne;
    return VME_OK;
}

int datumMech(int mech) {
    motorVars *m;
    int motor;

    motor = mechanism[mech].motor;
    if (motor < 0)
        return VME_ILLEGAL_MOTOR;
    m = motors[motor];
    if (!m)
        return VME_ILLEGAL_MOTOR;
    m->reqOp = datum;
    return VME_OK;
}

/* Translate from a mechanism to a motor number */
int mechMotor(int mech) {
    
    return mechanism[mech].motor;
}

/* Validate a request for positioning from the EPICS layer */
int valid(int who, char *where) {
    mechDescriptor *pMech;
    mechNode *pNode;
    motorVars *m;
    filterNode *fNode;
    int lim;

    /* A bit of diagnostic help */
    if (who < 0) {
        printf("valid (who, where): who is encoded 0 - 9 as follows\n");
        printf("[0] cover, [1] fw1, [2] fw2, [3] slit, [4] decker\n[5] acq, [6] xdisp, [7] grating, [8] camera, [9] focus\n");
        return VME_ERROR;
    }
    switch (who) {
        /* Positions that are defined in the config file.  For FOCUS, it's
         * not likely that such predefined positions will be used.
         */
        case COVER:
        case FW1:
        case FW2:
        case SLIT:
        case DECKER:
        case ACQ:
        case XDISP:
        case GRATING:
        case CAMERA:
        case FOCUS:
            pMech = &mechanism[who];
            pNode = (mechNode *)nodeLookup(where, pMech->pTable, FALSE);
            if (pNode == NULL) {
                gnirsLogMessage(CICS_DB_ERROR4, "Unknown position for %s",
                        pMech->name);
                return VME_ERROR;
            }
            /* Now have valid step position; record the position for
             * later generation of header information.
	     * reqStepPosition is the step count from zero, except for the
	     * grating where it is simply the grating number.
	     * reqPosition is a string containing the name of the position.
             */
            pMech->reqStepPosition = pNode->position;
            strncpy(pMech->reqPosition, where, ITEM_ID_LEN);
            /* Erase FILTER header info if we set either of FW1 or FW2 */
            switch (who) {
                case FW1:
                case FW2:
                    reqFilterPosition[0] = '\0';
                    break;
            }
            switch (who) {
                /* These two cases use limit switches to define position */
                case COVER:
                case ACQ:
                    if(pNode->position == 0)
                        lim = POS_LIM;
                    else
                        lim = NEG_LIM;
                    m = motors[pMech->motor];
                    if (m) {
                        m->reqOp = findLimit;
                        m->reqArg = lim;
                    }
                    break;
                case GRATING:
                    /* Nothing else to do; the grating is specified by
                     * the integer stored in reqStepPosition.
                     */
                    break;
                /* Everyone else uses steps to define position */
                default:
                    setPositionReq(pMech->motor, pNode->position);
                    break;
            }  
            break;
        /* Set two filter positions */
        case FILTER:
            fNode = (filterNode *) nodeLookup(where, &filterHead, FALSE);
            if (fNode == NULL) {
                gnirsLogMessage(CICS_DB_ERROR4, " 'filter' position unknown");
                return VME_ERROR;
            }
            /* Record settings for FILTER header */
            strncpy((char *)reqFilterPosition, where, ITEM_ID_LEN);

            /* We now know the combo, and since the configuration reading
             * routine verified that the combo points to defined filters,
             * we don't have to check for lookup errors.
             */
            pMech = &mechanism[FW1];
            pNode = (mechNode *)nodeLookup(fNode->f1, pMech->pTable, FALSE);
            pMech->reqStepPosition = pNode->position;
            strncpy(pMech->reqPosition, fNode->f1, ITEM_ID_LEN);
            setPositionReq(pMech->motor, pNode->position);

            pMech = &mechanism[FW2];
            pNode = (mechNode *)nodeLookup(fNode->f2, pMech->pTable, FALSE);
            pMech->reqStepPosition = pNode->position;
            strncpy(pMech->reqPosition, fNode->f2, ITEM_ID_LEN);
            setPositionReq(pMech->motor, pNode->position);

            break;
        default:
            gnirsLogMessage (CICS_DB_ERROR4,
                    "Illegal device %d in 'valid()'", who);
            return VME_ERROR;
            break;
    }
    return VME_OK;
}

/* Convert rotation in degrees to encoder position.  Not really needed
 * as a separate function, but perhaps useful this way for testing.
 * theta is in degrees, and is measured from the home position.
 */
int setGratingRot(double theta) {
    mechDescriptor *pMech;
    motorVars *m;
    int pos;

    pMech = &mechanism[GRATING];
    m = motors[pMech->motor];
    pos = (theta / 360.) * m->fullTravel;
    gratingStep = pos;      /* Set header info */
    setPositionReq(GRATING, pos);
    return VME_OK;
}

int validGratingWavelength(double wavelength, int order) {
    mechDescriptor *pMech;
    double theta, x;
    gdata *gdp;

    pMech = &mechanism[GRATING];
    gdp = &gratingData[pMech->reqStepPosition];

    /* Validate reasonable lambda */
    x = (wavelength * order) / gdp->A;
    if ( abs(x) >= 1.) {
        gnirsLogMessage(CICS_DB_ERROR4, "Invalid lambda/order (%f/%d)",
                wavelength, order);
        return VME_ERROR;
    }
    /* theta in degrees */
    theta = asin(x) * (180. / 3.1415926) + gdp->zeroPt;
    if (theta >= 360.)
        theta -= 360.;
    /* Not possible if zeroPoint is positive, but it's easy to check */
    if (theta <= -360.)
        theta += 360.;
    /* Record the header info */
    gratingWavelength = wavelength;
    gratingAngle = theta;
    gratingOrder = order;
    return setGratingRot(theta);
}

int validPreferredWavelength(double wavelength){
    mechDescriptor *pMech;
    gdata *gdp;
    double *dp, w;
    int order;

    w = wavelength;
    if (w < 0)
        w = -w;
    pMech = &mechanism[GRATING];
    gdp = &gratingData[pMech->reqStepPosition];
    dp = gdp->orderLimits;
    for (order = 0; order < NUM_GRAT_BOUNDS; order++) {
        if (w >= *dp)
            break;
        dp++;
    }
    if ((order == 0) || (order == NUM_GRAT_BOUNDS)) {
        gnirsLogMessage(CICS_DB_ERROR4, "wavelength %f outside bounds",
                    wavelength);
        return VME_ERROR;
    }
    return validGratingWavelength(wavelength, order);
}

/* Set simulation level */
int setSimulation(int level) {

    switch(level) {
        case NOSIM:
        case VSM:
        case FULLSIM:
        case FASTSIM:
            gnirsG.simulation = level;
            return VME_OK;
            break;
        default:
            break;
    }
    return VME_ERROR;
}

/* SAD interface routines */
SAD_STATIC getSadStatic() {
    static SAD_STATIC info;

    strncpy(info.name, "GNIRS CC", EPICS_LEN);
    info.name[EPICS_LEN - 1] = '\0';
    strncpy(info.swID, SWID, EPICS_LEN);
    info.swID[EPICS_LEN - 1] = '\0';

    return info;
}

SAD_DYNAMIC getSadDynamic() {
    static SAD_DYNAMIC info;
    static uint32 startTime = 0;
    char *p;

    switch (gnirsG.state) {
    case BOOTING:
        p = "BOOTING";
        break;
    case INITIALIZING:
        p = "INITIALIZING";
        break;
    case RUNNING:
        p = "RUNNING";
        break;
    case CONFIGURING:
        p = "CONFIGURING";
        break;
    }
    strcpy(info.state, p);

    healthString(info.health, gnirsG.health);

    /* just using a counter for heartbeat */
    info.heartBeat = startTime++;

    return info;
}
