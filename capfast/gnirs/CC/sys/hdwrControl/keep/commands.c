static struct
  {
      void *v;
      char *c;
  }
sccsid =
{
    &sccsid,
        "%W% %G%"
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

int moveMech(int mech, char *message) {
    motorVars *m;
    int motor;

    motor = mech;
    if ((motor < 0) || (motor >= NUM_MOTORS))
        return VME_ILLEGAL_MOTOR;
    m = motors[motor];
    if (!m) {
        mErrMsg(m, "moveMech: motor %d not allocated", motor);
        return VME_ILLEGAL_MOTOR;
    }

    /* Don't move if current position is the same as the requested
     * one.  This test is here to avoid even just turning on the motor
     * when we don't have to.  The test only works for the Rotary and
     * Linear motors since they use 'position'; the Binary ones use
     * 'findTheLimit'.  But for those, it doesn't matter if they
     * move a little since there is no precise positioning requirement
     * that is not enforced by the mechanism itself.
     */
    if ((m->reqOp == position) && (m->currPos == m->reqArg)) {
        m->errorMsg[0] = '\0';
        m->reqOp = NULL;
        return VME_OK;
    }

    moveOneMotor(motor);
    if (m->returnValue != VME_OK) {
        strncpy(message, m->errorMsg, EPICS_LEN);
        message[EPICS_LEN] = '\0';
    }
    return m->returnValue;
}

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
        if (m->status & MS_ERROR)
            rc = G_ERROR;
    }
    gnirsG.datumed = datumed;
    gnirsG.parked = parked;
    return rc;
}

int setPositionReq(int mech, int where ) {
    motorVars *m;
    int motor;
    int normal, lim;

    motor = mech;
    if ((motor < 0) || (motor >= NUM_MOTORS))
        return VME_ILLEGAL_MOTOR;
    m = motors[motor];
    if (!m)
        return VME_ILLEGAL_MOTOR;
    /* Make sure where is legal */
    switch (m->type) {
        case BINARY:
            normal = TRUE;
            if(where >= 0)    /* 0 defined as travel limit */
                lim = POS_LIM;
            else if (where < m->negLimit.position)
                lim = NEG_LIM;
            else
                normal = FALSE;
            if (normal) {
                m->reqOp = gotoLimit;
                m->reqArg = lim;
                return VME_OK;
            }
            break;
        case LINEAR:
            if ((where >= m->posLimit.position) ||
                    (where <= m->negLimit.position)) {
                mErrMsg(m, "Linear Pos %d too large", where); 
                return VME_ERROR;
            }
            break;
        case ROTARY:
            if (abs(where) > m->fullTravel){
                mErrMsg(m, "Rotary Pos %d too large", where); 
                return VME_ERROR;
            }
    }
    m->reqArg = where;
    m->reqOp = position;
    return VME_OK;
}

int parkMech(int mech) {
    motorVars *m;
    int motor;

    motor = mech;
    if ((motor < 0) || (motor >= NUM_MOTORS))
        return VME_ILLEGAL_MOTOR;
    m = motors[motor];
    if (!m)
        return VME_ILLEGAL_MOTOR;
    m->reqOp = park;
    return VME_OK;
}

int datumMech(int mech) {
    motorVars *m;
    int motor;

    motor = mech;
    if ((motor < 0) || (motor >= NUM_MOTORS))
        return VME_ILLEGAL_MOTOR;
    m = motors[motor];
    if (!m)
        return VME_ILLEGAL_MOTOR;
    m->reqOp = datum;
    return VME_OK;
}

int valid(int who, char *where) {
    mechDescriptor *pMech;
    mechNode *pNode;
    motorVars *m;
    filterNode *fNode;
    int lim, normal, rc;

    /* A bit of diagnostic help */
    if (who < 0) {
        printf("valid (who, where): who is encoded 0 - 9 as follows\n");
        printf("[0] cover, [1] fw1, [2] fw2, [3] slit, [4] decker\n[5] acq, [6] xdisp, [7] grating, [8] camera, [9] focus [10] filter\n");
        return VME_ERROR;
    }
    rc = VME_OK;
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
            pMech->focusShift = pNode->focusShift;
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
                    m = motors[who];
                    if (!m) {
                        gnirsLogMessage (CICS_DB_ERROR4,
                                "Illegal motor %d in 'valid()'", who);
                        return VME_ILLEGAL_MOTOR;
                    }
                    normal = TRUE;
                    if(pNode->position >= 0)    /* 0 defined as travel limit */
                        lim = POS_LIM;
                    else if (pNode->position < m->negLimit.position)
                        lim = NEG_LIM;
                    else
                        normal = FALSE;
                    if (normal) {
                        m->reqOp = gotoLimit;
                        m->reqArg = lim;
                    } else
                       rc = setPositionReq(who, pNode->position);
                    break;
                case GRATING:
                    /* Nothing else to do; the grating is specified by
                     * the integer stored in reqStepPosition.
                     */
                    break;
                /* Everyone else uses steps to define position */
                default:
                    rc = setPositionReq(who, pNode->position);
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
            pMech->focusShift = pNode->focusShift;
            strncpy(pMech->reqPosition, fNode->f1, ITEM_ID_LEN);
            rc = setPositionReq(who, pNode->position);
            if (rc != VME_OK)
                return rc;

            pMech = &mechanism[FW2];
            pNode = (mechNode *)nodeLookup(fNode->f2, pMech->pTable, FALSE);
            pMech->reqStepPosition = pNode->position;
            pMech->focusShift = pNode->focusShift;
            strncpy(pMech->reqPosition, fNode->f2, ITEM_ID_LEN);
            rc = setPositionReq(who, pNode->position);

            break;
        default:
            gnirsLogMessage (CICS_DB_ERROR4,
                    "Illegal device %d in 'valid()'", who);
            return VME_ERROR;
            break;
    }
    return rc;
}

int setGratingTilt(double angle) {

    gratingOrder = 1;
    return setGratingRot(angle);
}

int setGratingRot(double angle) {
    mechDescriptor *pMech;
    motorVars *m;
    int pos;
    double x, theta;
    gdata *gdp;

    pMech = &mechanism[GRATING];
    gdp = &gratingData[pMech->reqStepPosition];

    theta = angle;
    if (theta >= 360.)
        theta -= 360.;
    if (theta <= -360.)
        theta += 360.;

    m = motors[GRATING];
    pos = (theta / 360.) * m->fullTravel;
    gratingStep = pos + gdp->zeroPt;

    /* actual differs from requested due to truncation, so
     * invert to get the actual wavelength
     */
    theta = (360. * pos) / m->fullTravel;
    if ( gratingOrder == 0) {
	gratingAngle = 0.0;
	gratingWavelength = 0.0; /* undefined */
    } else {
	gratingAngle = theta;
	x = sin(gratingAngle * (3.14159/180.));
	if (x < 0.)
	    x *= -1.;
	gratingWavelength = x * gdp->A / gratingOrder;
    }

    return setPositionReq(GRATING, gratingStep);
}

int validGratingWavelength(double wavelength, int order) {
    mechDescriptor *pMech;
    double x;
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
    /* Record the header info */
    /* Angle in degrees */
    if (order == 0) {
	gratingAngle = 0.0;
	gratingWavelength = 0.0; /* undefined */
    } else {
	gratingAngle = asin(x) * (180. / 3.1415926);
	gratingWavelength = wavelength;
    }
    gratingOrder = order;
    return setGratingRot(gratingAngle);
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

/* scan rate for updating slow SAD information, in seconds */
int slowScanRate = 15;

int scanStatusTask(int a1, int a2, int a3, int a4, int a5, int a6,
        int a7, int a8, int a9, int a10) {

    int interval;

    /* Wait 5 seconds for everything to quiet down */
    taskDelay(5 * gnirsG.clockRate);
    interval = 0;       /* Count number of seconds have waited */
    for(;;) {
        /* Every second */
        /* Cryo head control switch.  This pretty much obviates
         * the need for interrupt handling for the computer/manual
         * switch (see tandp.c, digital.c), but looping tasks are
         * ugly so the interrupt code will stay.
         */
        cryoCpuControl = gnirsG.cryoCpuState;    /* 0 off, 1 on */
        cryoOnOffSw = isSet(cryoSwitches[ON_OFF_SWITCH]);
        cryoSelectSw = gnirsG.cryoSelect;   /* COMPUTER/MANUAL */

        /* Redo any once per reconfiguration calculations.  Since
         * the flag is set by moveOneMotor, it's possible that this
         * calculation could be done twice or so.  That's a waste
         * of cpu cycles but otherwise is not a problem.
         */
        if (gnirsG.newConfiguration) {
            /* calculate grating extents */
	    /* XXX fix me */
            gratingEnd[0] = 1.1;
            gratingEnd[1] = 2.2;
            /* and resolution at center */
            gratingResolution = .00056;
            gnirsG.newConfiguration = FALSE;
        }
        taskDelay(gnirsG.clockRate);    /* one second */
        interval++;
        if (( interval % slowScanRate) == 0) {
            /* do slow stuff */
            interval = 0;
        }
    }
    return VME_OK;      /* never returns */
}
