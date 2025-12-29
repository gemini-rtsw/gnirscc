static struct
  {
      void *v;
      char *c;
  }
rcsid =
{
    &rcsid,
        "$Id: gnirsMotors.c,v 1.2 2009/05/27 19:32:07 fkraemer Exp $"
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
#include "stdarg.h"
#include "gnirs.h"
#include "timeLib.h"
#include "epicsTypes.h"


/* Motor Controllers */
int sendToMotorDriver(int, char *, char *);
int chargeLimit(int, switchType *);

int sendToMotorDebug = 0;

/* The function microRound is not needed; but leave the calls in place
 * for now in case we change our minds.
 */
#define microRound(x)  (x)


/* Round a given distance to nearest multiple of MICRO_STEPS */
/* NOT IN USE
 * int microRound(int);

 * int microRound(int d) {
 *    int rem;
 *    int dist;

 *    dist = abs(d);
 *    rem = dist % MICRO_STEPS;
 *    dist -= rem;
 *    if (rem > (MICRO_STEPS/2))
 *        dist += MICRO_STEPS;
 *    if ( d < 0)
 *        return -dist;
 *    else
 *        return dist;
 *}
 * END of NOT IN USE
 */

/* Routine to send a string to a motor controller.
 * If the response string pointer is null, don't wait for
 * the response.  Command string should end in ' ' just for
 * safety's sake.  For commands which don't require a response,
 * use 'noWait' instead of 'waitFor'.
 */
int sendToMotorDriver(int controller, char *string, char *response) {
    GNIRS_ST_MD *md;
    mdRegister *mdr;
    char c;
    int rc, n;
    unsigned char control;

    if (gnirsG.simulation) {
        /* This should not happen */
        gnirsLogMessage(CICS_DB_ERROR,
                "sendToMotorDriver called during simulation");
        return VME_ERROR;
    }

    md = &motorDriver[controller];
    /* Print out every command, but skip the report ones unless
     * sendToMotorDebug is 2 or more
     */
    if (sendToMotorDebug) {
        c = *(string + 3);
        if (((c == 'R') && sendToMotorDebug > 1) || (c != 'R'))
            printf("send%d: %s\n", controller, string);
    }

    if(strlen(string) > rngFreeBytes(md->outRing)) {
        gnirsLogMessage(CICS_DB_ERROR, "Ring buffer for C%d too small",
                controller);
        return VME_ERROR;
    }

    /* Get the semaphore that allows us to talk to the controller */
    rc = semTake(md->semOut, gnirsG.clockRate);  /* wait one second */
    if (rc != OK) {
        gnirsLogMessage(CICS_DB_ERROR, "Can't get controller %d's semaphore",
                controller);
        return VME_ERROR;
    }

    mdr = md->registers;

    /* Setting the null is a precaution and may help with debugging */
    md->messageVars.message[0] = '\0';

    /* Send the message.  It's ok to do this with interrupts enabled
     * since even if the queue drains before we enable the interrupt,
     * the extra interrupt will be ignored by the handler.
     */
    n = rngBufPut(md->outRing, string, strlen(string));
    if (n != strlen(string)) {
        gnirsLogMessage(CICS_DB_ERROR, "C%d: no room for command string",
                controller);
        semGive(md->semOut);
        return VME_ERROR;
    }

    /* set the interrupt enable for the transmit buffer */
    sysIntDisable(GNIRS_MOTOR_INTERRUPT_LEVEL);
    control = mdr->control;
    mdr->control = control | MD_TBE_E;
    sysIntEnable(GNIRS_MOTOR_INTERRUPT_LEVEL);

    /* If a response required, wait on the semaphore */
    if (response) {
        rc = semTake(md->semResponse, gnirsG.clockRate); /* timeout time XXX */
        if (rc != OK) {
            if (errno == S_objLib_OBJ_TIMEOUT) {
                    gnirsLogMessage(CICS_DB_ERROR,
                        "Timeout in sendToMotor for C%d", controller);
                    rc = VME_TIMEOUT;
            } else {
                gnirsLogMessage(CICS_DB_ERROR,
                        "Invalid semaphore in sendToMotor for C%d", controller);
                rc = VME_ERROR;
            }
            semGive(md->semOut);
            return rc;
        }
        strcpy(response, md->messageVars.message);
    }

    semGive(md->semOut);
    return VME_OK;
}

/* Get status information for a motor controller */
void getMdStatus(int motor) {
    int i, rc, controller;
    char mstat;
    char buf[MOTOR_MSG_LEN], c;
    motorVars *m;
    GNIRS_ST_MD *md;
    char cmd[MOTOR_CMD_LEN];

    m = motors[motor];
    if ((motor >= NUM_MOTORS) || !m) {
        gnirsLogMessage(CICS_DB_ERROR, "status req: motor %d not allocated",
                motor);
        return;
    }
    controller = m->controller;
    md = &motorDriver[controller];
    
    sprintf(cmd, "A%c RA", m->charAxis);
    rc = sendToMotorDriver(controller, cmd, buf);
    if (rc != VME_OK)
        return;

    if (md->status & MD_CMD_S) {
        md->status &= ~MD_CMD_S;
        gnirsLogMessage(CICS_DB_ERROR,
            "Illegal motor command in C%d:\n\t<%s>", controller, md->cmdErr);
        /* XXX change in health ? */
    }

    for (i = 0; i < MD_STAT_SIZE; i++ ) {
        c = buf[i];
        switch (i) {
            case 0:
                mstat = 0;
                if (c == 'M')
                    mstat |= MD_MINUS;
                break;
            case 1:
                if (c == 'D')
                    mstat |= MD_DONE;
                break;
            case 2:
                if (c == 'L')
                    mstat |= MD_LIMIT;
                break;
            case 3:
                if (c == 'H')
                    mstat |= MD_HOME;
                m->status = (m->status & ~MD_STAT_MASK) | mstat;
                break;
        }
    }
    /* If enable is on, isEnabled will be on if no overtravel */
    if (isSet(m->enable)) {
        if (isSet(m->isEnabled))
            m->status &= ~MDS_OTRAVEL;
        else
            m->status |= MDS_OTRAVEL;
    }
    /* Fault is true low */
    if (isClear(m->fault))
        m->status |= MDS_FAULT;
    else
        m->status &= ~MDS_FAULT;
}

/* The motor "Class" */

/* Helper function.  Determines number of ticks to wait for the
 * motor to go 'dist' at velocity 'vel'.
 * If time is less than a minimum, return that.
 */
int motorTimeout(int dist, motion *ms) {
    int time;
    double vel, acc;
    double d, D;

    vel = ms->velocity;
    acc = ms->acceleration;
    D = (double)abs(dist);

    /* distance covered with constant acceleration to specified velocity
     * and then deceleration to rest.
     */
    d = vel * vel / acc;
    /* If desired distance is greater than this, the remainder of the
     * distance is covered at constant velocity.
     */
    if (d < D)
        time = ((D - d) / vel) + 2.*vel/acc + 0.5;
    else
        time = 2. * sqrt(D/acc) + 0.5;

    time *= gnirsG.clockRate;
    if (!gnirsG.simulation && (time < TIMEOUT_MIN))
        time = TIMEOUT_MIN;
        if (sendToMotorDebug)
	    printf("timeout is %d for %d %d %d\n", time, dist,
            ms->velocity, ms->acceleration);
    return time;
}

/* Set up a motor with default parameters.  Called by the routine
 * that reads the configuration file.
 */
int setupMotor(int motor, char *name) {
    motorVars *m;
    BOOL first;

    if (motor >= NUM_MOTORS) {
        gnirsLogMessage(CICS_DB_ERROR, "Illegal motor number: %d", motor);
        return VME_ERROR;
    }

    if (motors[motor] == NULL)
        first = TRUE;
    else
        first = FALSE;
    m = motors[motor] = &motorV[motor];

    /* fill in default values */

    if (name)
        strncpy(m->name, name, MOTOR_ID_LEN);
    else
        m->name[0] = '\0';
    m->controller = motor / NUM_AXES;
    m->intAxis = motor % NUM_AXES;
    m->charAxis = axisLetter[m->intAxis];
    m->homeLevel = 'L';
    if (first) {
        m->status = NULL;
        m->health = GOOD;
    }
    m->type = LINEAR;
    m->fullTravel = 4530;
    
    m->seek.acceleration = 50;
    m->seek.velocity = 200;

    m->backOff.acceleration = 25;
    m->backOff.velocity = 100;

    /* fast deceleration if will probe for linear limits as 'SF'
     * kills the done interrupt at overtravel !!
     */
    m->probe.acceleration = 10;
    m->probe.velocity = 20;
    m->probeH.acceleration = 1;
    m->probeH.velocity = 5;

    m->home.type = HOME_SW;     /* can be POS_LIM or NEG_LIM */
    m->home.useAlt = FALSE;
    m->home.offset = 130;
    m->home.control.port = 0;
    m->home.control.bit = 0;

    m->parkPosition = 0;        /* default: park at home */

    m->posLimit.type = POS_LIM; /* redundant information */
    m->posLimit.offset = 100;   /* must be positive */
    /* port, bit not used */

    m->negLimit.type = NEG_LIM; /* redundant information */
    m->negLimit.offset = -150;  /* must be negative */

    /* set up somewhat plausible limit positions;
     * home is always at zero if not using the alternative switch.
     */
    if (m->type != ROTARY) {
        if (m->home.type == HOME_SW) {
            m->posLimit.position = microRound(m->fullTravel/2);
            m->negLimit.position = -(m->posLimit.position);
        } else if (m->home.type == NEG_LIM) {
            m->posLimit.position = m->fullTravel;
            m->negLimit.position = 0;
        } else {
            m->posLimit.position = 0;
            m->negLimit.position = -(m->fullTravel);
        }
    }

    m->stopDistance = 50;
    m->backlash = 200;
    m->probeStep = 5;   /* XXX not used */

    m->aborted = FALSE;
    if (first) {
        m->datumed = FALSE;
        m->parked = FALSE;
    }
    m->boostTime = 0;
    m->reqOp = 0;
    m->reqArg = 0;
    /* Configuration will be finished when config file is read */
    
    return VME_OK;
}

/* Some parameters depend on others.  Since there's no control in
 * the configuration file on the order of parameters, this function is
 * used, after the configuration information for a given motor is read,
 * to finish by setting the dependent parameters.
 */
void finishMotorConfig(int motor) {
    motorVars *m;
    int n;
    char *p;

    m = motors[motor];

    if (m->home.useAlt) {
        m->home.position = m->home.offset;
        setBit(m->home.control);
    } else {
        m->home.position = 0;
        clearBit(m->home.control);
    }

    /* When using limit as a home position, the limit must be the
     * positive one because we assume backlash is to be taken out
     * by going to the negative side of the end position and then
     * moving in a positive direction; there is no available 'space'
     * to do so if we use the negative limit.
     */
    if (m->type == BINARY)
        m->home.type = POS_LIM;

    /* Prefix raw initString with axis specifier */
    sprintf(m->initString, "A%c ", m->charAxis);
    strcat(m->initString, motorInitString);
    n = MOTOR_CMD_LEN - strlen(m->initString);
    strncat(m->initString, m->cfgInitString, n);
    
    /* Set home level command */
    for (p = m->initString; *p; p++) {
        if (*p == 'H') {
            p++;
            if ((*p == 'L') || (*p == 'H'))
                break;
            else
                p--;
        }
    }
    if (*p)
        *p = m->homeLevel;
}

int initMotors() {
    motorVars *m;
    int motor, rc;
    char name[16];
    SEM_ID sem;

    for (motor = 0; motor < NUM_MOTORS; motor++ ) {
        m = motors[motor];
        if (!m)
            continue;
        if(m->semMoveMotor)
            semDelete(m->semMoveMotor);
        sem = semBCreate(SEM_Q_FIFO, SEM_EMPTY);
        if (sem == NULL) {
            gnirsLogMessage(CICS_DB_ERROR,
                       "Unable to create semMoveMotor for motor %d.", motor);
            return VME_MEMORY_ERROR;
        }
        m->semMoveMotor = sem;

        if (m->taskID)
            taskDelete(m->taskID);
        sprintf(name, "tMotor%02d", motor);
        m->taskID = taskSpawn(name, 50, VX_FP_TASK, 4000,
                motorTask, motor, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        if(m->taskID == ERROR) {
            m->taskID = 0;
            gnirsLogMessage(CICS_DB_ERROR,
                    "Failed to spawn motor%02d task", motor);
            return VME_ERROR;
        }

        /* Reset Phytron
         * turn off enable
         *   ... These should all be off/disabled when system boots ...
         */
        clearBit(m->reset);
	/* setBit(m->boost); */
        clearBit(m->enable);
        /* XXX is this long enough? */
        setBit(m->reset);
 

        /* Now tell the motor to do the init string */

        rc = noWait(motor, m->initString);
        if (rc != VME_OK) {
            return rc;
        }
        /* There's nothing to check in the returned status since it's ok
         * to be at HOME or at a LIMIT, and the init string is not allowed
         * to initiate any motion (though there's no check for that here).
         */
        
        /* Get current position */
        rc = tellPV(motor);
        if (rc != VME_OK)
            return rc;
        if (gnirsG.simulation)
            m->currPos = 0;     /* hard to know what (else) to do */
        m->lastPos = m->currPos;
    }
   return VME_OK;
}

int motorTask(int motor, int a1, int a2, int a3, int a4, int a5, int a6,
        int a7, int a8, int a9) {

    int rc;
    motorVars *m;

    m = motors[motor];
    for(;;) {
        /* wait until there's a command pending */
        semTake(m->semMoveMotor, WAIT_FOREVER);
        if (m->reqOp) {
            m->aborted = FALSE;
            rc = (*(m->reqOp))(motor, m->reqArg);
            gnirsLogMessage(CICS_DB_FULL, "motor %d op %x arg %x -- %d",
                    motor, m->reqOp, m->reqArg, gnirsG.movesInProgress);
            m->reqOp = NULL;        /* no accidental repeat */
        } else
            rc = VME_OK;
        m->status &= ~MDS_BUSY;     /* be sure not set */
        if (rc == VME_ERROR)
            m->status |= MDS_ERROR;     /* be sure it is set */
        /* We're done */
        if(gnirsG.movesInProgress > 0) {
            gnirsG.movesInProgress--;
            if (gnirsG.movesInProgress == 0)
                gnirsLogMessage(CICS_DB_FULL, "All motor tasks done");
        } else {
            gnirsLogMessage(CICS_DB_ERROR, "Moves counter not positive %d",
                gnirsG.movesInProgress);
            gnirsG.movesInProgress = 0;
        }
    }
}

int noWait(int motor, char *command) {
    return waitFor(motor, command, 0);
}

/* Send a command string to the motor and wait the given number of
 * clock ticks for interrupt (caused by the 'ID' command).  Then fetch
 * status before returning.
 *
 * If in simulation mode, delay the return if the mode requires an
 * approximation to realistic timing.
 */
int waitFor(int motor, char * cmdString, int wait) {
    int rc, n;
    GNIRS_ST_MD *md;
    motorVars *m;
    SEM_ID sem;
    int boosting, waitTime, localErrno;
    static char waitForBuffer[MOTOR_MSG_LEN];
    char stopCmd[8];

    m = motors[motor];
    md = &motorDriver[m->controller];
    boosting = m->boostTime;

    if (m->aborted)
        return VME_ABORTED;
    n = strlen(cmdString);
    n++;        /* account for space to be added */
    if (wait)
        n += 3;     /* space to add the " ID" string */
    if (n >= MOTOR_CMD_LEN) {
        gnirsLogMessage(CICS_DB_ERROR, "cmd too long: <%s>", cmdString);
        return VME_ERROR;
    }
    strcpy(m->command, cmdString);
    if (wait)
        strcat(m->command, " ID");
    strcat(m->command, " ");
    if (gnirsG.simulation == NOSIM) {
        if (wait) {
            if (boosting) {
                clearBit(m->boost);
            }
            md->waiting[m->intAxis] = sem = md->semDone[m->intAxis];
        }
        rc = sendToMotorDriver(m->controller, m->command, NULL);
        if (rc != VME_OK) {
            gnirsLogMessage(CICS_DB_ERROR, " failed in waitFor");
            md->waiting[m->intAxis] = NULL;
            return rc;
        }
    } else {
        /* copy message so if m->command gets overwritten, there's a
         * chance the logging system will still get original.
         */ 
        strcpy(waitForBuffer, m->command);
        gnirsLogMessage(CICS_DB_FULL, "command: %s", waitForBuffer);
    }
    if (! wait)
        return VME_OK;

    if (gnirsG.simulation == FULLSIM) {
        taskDelay(wait);
    }
    if (gnirsG.simulation)
        return VME_OK;

    /* wait time requested plus another 20%.
     * wait is an approximate time for motor motion, but we want the timeout
     * to be on the generous side so we don't say there's an error when
     * the motor is just a bit slower than we thought it would be.
     */
    waitTime = (1.2 * (float) wait);

    if (boosting) {
        waitTime -= boosting;
        if (waitTime < 1)
            waitTime = 1;
        rc = semTake(sem, boosting);
        setBit(m->boost);
    } else {
        rc = semTake(sem, waitTime);
        waitTime = 0;
    }
    if (rc != OK) {
        if (m->aborted) {
            rc = VME_ABORTED;
            goto statReturn;
        }
        if (errno == S_objLib_OBJ_TIMEOUT) {
                if (boosting) { /* timeout is expected */
                    rc = semTake(sem, waitTime);
                    if (rc == OK)
                        rc = VME_OK;
                    if (m->aborted) {
                        rc = VME_ABORTED;
                    }
                    if ((rc == VME_OK) || (rc == VME_ABORTED))
                        goto statReturn;
                }
                /* Timeout: motor stuck */
                md->waiting[m->intAxis] = NULL;
                /* It could be that there was a semGive just after the
                 * return from the semTake and before the previous line.
                 * That would be the case if the motor was moving
                 * unusually slowly.
                 * Be sure the semaphore is clear.  The only way I know
                 * to do this is to take the semaphore with NO_WAIT.
                 */
                localErrno = errno;
                semTake(sem, NO_WAIT);
                if (localErrno == S_objLib_OBJ_TIMEOUT) {
                    gnirsLogMessage(CICS_DB_ERROR,
                        "Timeout in waitFor for C%d%d",
                                m->controller, m->intAxis);
                    rc = VME_TIMEOUT;
                } else {
                    gnirsLogMessage(CICS_DB_ERROR,
                        "C%d%d: Invalid semaphore in waitFor-1",
                                m->controller, m->intAxis);
                    rc = VME_ERROR;
                }
        } else {
            gnirsLogMessage(CICS_DB_ERROR,
                    "C%d%d: Invalid semaphore in waitFor-2",
                            m->controller, m->intAxis);
            rc = VME_ERROR;
        }
    } else
        rc = VME_OK;
statReturn:
    /* Stop the controller if timeout or abort */
    if ((rc == VME_TIMEOUT) || (rc == VME_ABORTED)) {
        sprintf(stopCmd, "A%c ST ", m->charAxis);
        sendToMotorDriver(m->controller, stopCmd, NULL);
    }
    getMdStatus(motor);

    return rc;
}

/* Reports position and velocity of a motor */
int tellPV(int motor) {
    motorVars *m;
    char response[MOTOR_MSG_LEN];
    char cmd[12];
    int rc;

    /* It's possible to do things like set the position counter, via
     * an LP command, and then call RP, and get back the value of the
     * counter before(!) the LP command.  So as a not very nice "fix",
     * delay 32ms
     */
    taskDelay(2);

    m = motors[motor];
    if (gnirsG.simulation) {
        m->currVel = 0;
        return VME_OK;
    }
    sprintf(cmd, "A%c RP ", m->charAxis);
    rc = sendToMotorDriver(m->controller, cmd, response);
    if (rc != VME_OK)
        return rc;
    m->currPos = atoi(response);
    sprintf(cmd, "A%c RV ", m->charAxis);
    rc = sendToMotorDriver(m->controller, cmd, response);
    if (rc != VME_OK)
        return rc;
    m->currVel = atoi(response);
    return VME_OK;
}

/* Go to a given position.  Does not reset the position counter except
 * to compensate for rotary motions where the motion crosses the point
 * half way around from the home position.
 * The timeout value (in ticks) tells how long to wait for the motion
 * to complete.
 * If the motor has not been datumed, then consider this a relative
 * move (since we don't know where the motor is).
 * Do not change lastPos here.  That's done in higher level code so it
 * reflects the last reached position. XXX
 */
int motorPos(int motor, int where, int stopD, int timeout) {
    motorVars *m;
    int rc, delta, destination;
    int curr;
    char cmd[MOTOR_MSG_LEN];

    m = motors[motor];
    destination = where;
    if (where % MICRO_STEPS)  /* XXX not needed with MICRO_STEP == 1 */
        gnirsLogMessage(CICS_DB_FULL,
                "Motion %d not a multiple of microstepping factor", where);

    /* If linear, we just go to that absolute position.  If rotary, we need
     * to optimize to avoid more than half a rotation.
     * The 'RM' command would do the required modulo arithemtic on the
     * internal position counter, but it seems to make all distances positive
     * from home.
     */
    if (m->datumed) {
        if (where == m->currPos)
            return VME_OK;
      
        delta = destination - m->currPos;
        if (m->type == ROTARY) {
            if (abs(delta) == m->fullTravel)    /* we're there already */
                return VME_OK;
            /* if would go more than half way, pick a "better" destination */
            if (abs(delta) > microRound(m->fullTravel/2)) {
                if (delta < 0 ) {     /* move further in positive direction */
                    destination += m->fullTravel;
                } else {
                    destination -= m->fullTravel;
                }
            }
        }
        /* Go to that position */
        sprintf(cmd, "A%c MA%d GO", m->charAxis, destination);
    } else {
        sprintf(cmd, "A%c MR%d GO", m->charAxis, destination);
    }
    rc = waitFor(motor, cmd, timeout);
    if (rc != VME_OK)
        return rc;
    if (!m->datumed)
        return rc;
    if (gnirsG.simulation) {
        m->currPos = destination;
        return VME_OK;
    }
    rc = tellPV(motor);
    if (rc != VME_OK)
        return rc;
    if (m->currPos != destination) {
        gnirsLogMessage(CICS_DB_ERROR, "destination %d, got %d",
                destination, m->currPos);  /* XXX as above */
    }

    /* If rotary motion has crossed the half way position, reset position
     * counter to correct value.
     */
    if ((m->type == ROTARY) &&
            (abs(m->currPos) > (microRound(m->fullTravel/2)))) {
        if (m->currPos > 0)
            curr = m->currPos - m->fullTravel;
        else
            curr = m->currPos + m->fullTravel;
        sprintf(cmd, "A%c LP%d", m->charAxis, curr);
        rc = noWait(motor, cmd);
        if (rc != VME_OK) {
            if (rc != VME_ABORTED)
                gnirsLogMessage(CICS_DB_ERROR,
                        "Failed to reload position counter");
            return rc;
        }
        rc = tellPV(motor);       
        if (rc != VME_OK)
            return rc;
    }
    return VME_OK;
}

/* Go to a given (standard) position.
 * Reset the lastPos indicator when successfully done from requested
 * position, otherwise, from current position..
 * Calls goToPos for most of the effort.
 */
int position(int motor, int where) {
    motorVars *m;
    int rc, rc2;
    int initPosition;

    m = motors[motor];
    if ((motor >= NUM_MOTORS) || !m) {
        gnirsLogMessage(CICS_DB_ERROR, "status req: motor %d not allocated",
                motor);
        return VME_ILLEGAL_MOTOR;
    }
    /* check for no motion */
    (void) tellPV(motor);
    initPosition = microRound(m->currPos);
    if (microRound(where) == initPosition)
        return VME_OK;

    /* Shouldn't happen as higher code will check but just in case <g>, but
     * let anything happen when not datumed (helpful in lab work)
     */
    if (m->datumed && m->type != ROTARY) {
        if (((where > 0) && (where > m->posLimit.position)) ||
                (where < m->negLimit.position)) {
            gnirsLogMessage(CICS_DB_ERROR, "Position %d: too large", where);
            return VME_ERROR;
        }
    }
    m->health = GOOD;
    if (m->type == ROTARY) {
        if (abs(where) > microRound(m->fullTravel/2))
            if (where > 0)
                where -= m->fullTravel;
            else
                where += m->fullTravel;
    }
    m->reqPos = where;

    /* Power up */
    rc = motorOn(motor);
    if (rc != VME_OK) {
        m->health = BAD;
        m->status &= ~MDS_BUSY;
        return rc;
    }
    m->lastPos = m->currPos;
    m->parked = FALSE;
    rc = goToPos(motor, where, TRUE);
    /* Check to see if at parked position, but only if move was successful;
     * if it wasn't, we don't know where we are.
     */
    if (rc == VME_OK)
        checkParked(motor);
    if (rc != VME_OK) {
        tellPV(motor);  /* Be sure we have some inkling of where we are */
        if (rc == VME_TIMEOUT) {
            m->health = WARNING;    /* maybe it can move */
            if (!(m->status & MD_LIMIT))
                m->status |= MDS_STALL; /* timeout without hitting limit */
        } else if (rc != VME_ABORTED)
            m->health = BAD;
    }
    rc2 = motorOff(motor);
    m->status &= ~MDS_BUSY;
    if (rc2 != VME_OK) {
        m->health = BAD;
    }
    if ((rc != VME_ABORTED) && (microRound(m->currPos) == initPosition)) {
        /* failed to move at all */
        m->status |= MDS_STUCK;
        if (m->health == GOOD)
            m->health = WARNING;
        rc = VME_ERROR;
    }
    if (gnirsG.simulation) {
        m->status = MD_DONE;
        if (where == m->home.position) 
            m->status |= MD_HOME;
    }
    if (m->status & MDS_OTRAVEL)
        m->health = BAD;

    return rc;
}

/* Does the main work of moving to a given "encoder" position.
 * Correct for backlash and stopping distance.
 * Reaching a limit switch is an error.
 * Not useful if motor is not datumed.
 * If removeBL is FALSE, act as if there is no backlash to remove.
 */
int goToPos(int motor, int where, int removeBL) {
    motorVars *m;
    int rc, timeout;
    int lim, backlash;
    char cmd[MOTOR_CMD_LEN];
    int dist;

    m = motors[motor];
    if (!m->datumed)
        gnirsLogMessage(CICS_DB_MIN, "Call goToPos() when not datumed");

    /* A two step process.
     * 1) Go to the desired position at high speed stopping short if going
     * in the positive direction, going further if negative.
     * 2) Approach at slow (probe) speed (in positive direction).
     * Calling this routine when the mechanism has not been datumed is
     * probably an error since it assumes absolute moves and motorPos,
     * which moves the motor, will use relative moves.
     */

    if (m->currPos == where)
        return VME_OK;
    
    if (removeBL)
        backlash = m->backlash;
    else
        backlash = 0;
    sprintf(cmd, "A%c AC%d VL%d", m->charAxis,
            m->seek.acceleration, m->seek.velocity);
    rc = noWait(motor, cmd);
    if (rc != VME_OK) {
        if (rc != VME_ABORTED)
            gnirsLogMessage(CICS_DB_ERROR, "Cannot set seek accel/vel");
        return rc;
    }
    /* If distance to go is in positive direction and is less than
     * the backlash (which for positive motion isn't really a factor,
     * but gives us the distance along which we'll move slowly), then
     * don't bother with the fast motion.
     */
    if (m->datumed) {
        dist = where - m->currPos;
        if ((dist > 0) && (dist <= backlash) && removeBL)
            dist = 0;   /* Skip fast motion */
        else
            dist -= backlash;
    } else
        dist = m->fullTravel;
    if (dist) {
        timeout = motorTimeout(dist, &m->seek);
        rc = motorPos(motor, where - backlash, m->stopDistance, timeout);
        /* Check for limit switch overtravel first */
        if (lim = checkLimit(motor)) {
            gnirsLogMessage(CICS_DB_ERROR, "\"%s\" %s limit hit (seek)",
                    m->name, (lim > 0) ? "positive" : "negative");
            return VME_ERROR;
        }
        if (rc != VME_OK) {
            gnirsLogMessage(CICS_DB_ERROR, "Failed on fast positioning");
            return rc;
        }
    }
    if (!removeBL)
        return VME_OK;

    sprintf(cmd, "A%c AC%d VL%d", m->charAxis,
            m->probe.acceleration, m->probe.velocity);
    rc = noWait(motor, cmd);
    if (rc != VME_OK) {
        if (rc != VME_ABORTED)
            gnirsLogMessage(CICS_DB_ERROR, "Cannot set probe accel/vel");
        return rc;
    }
    timeout = motorTimeout(backlash, &m->probe);
    if (m->datumed)
        rc = motorPos(motor, where, 0, timeout);
    else
        rc = motorPos(motor, backlash, 0, timeout); /* a relative move (MR) */
    if (lim = checkLimit(motor)) {
        gnirsLogMessage(CICS_DB_ERROR, "\"%s\" %s limit hit (slow)", m->name,
                (lim > 0) ? "positive" : "negative");
        return VME_ERROR;
    }
    if (rc != VME_OK) {
        gnirsLogMessage(CICS_DB_ERROR, "Failed on slow positioning");
        return rc;
    }
    return VME_OK;
}


/* Check for limit switch.  Return 0 if not in a limit,
 * 1 if at positive limit, -1 if at negative.
 */
int checkLimit(int motor) {
    motorVars *m;

    m = motors[motor];
    if (m->status & MD_LIMIT)
        if ((m->status & MD_DIR) == MD_MINUS)
            return -1;
        else
            return 1;
    else
        return 0;
}

int motorOn(int motor) {
    return motorOnOff(motor, TRUE);
}

int motorOff(int motor) {
    return motorOnOff(motor, FALSE);
}

/* Does some status setup and checking.  Turns on BUSY when motor is
 * turned on.  Does not turn BUSY off afterwards.
 */
int motorOnOff(int motor, BOOL on) {
    motorVars *m;
    int limitCount;
    int rc;
    char cmd[MOTOR_CMD_LEN];

    m = motors[motor];
    if (!m) {
        gnirsLogMessage(CICS_DB_MIN, "Can't control inactive motor %d", motor);
        return VME_ERROR;
    }

    if (gnirsG.simulation ) {
        m->status = 0;
    } else if (on) {
        m->status &= ~MDS_STAT_MASK;
        /* To see the hardware fault, we must force a given direction
         * and then check for a limit.  Do this for both limits and if
         * we find both, there's a cable or hardware problem.
         * 'noWait' does not get status because in general it is not
         * useful to do so; therefore, it must be done explicitly here.
         *
         * The OMS board will show a limit only if the
         * current movement direction is toward that limit and the
         * sequence MM MP will thus remove all indication of a negative
         * limit.  Therefore, the test is made only when the motor
         * is turned on so that the limit status is not disturbed
         * when a given motion is finished.
         */
        limitCount = 0;
        sprintf(cmd, "A%c MM", m->charAxis);
        noWait(motor, cmd);
        getMdStatus(motor);
        if (m->status & MD_LIMIT )
            limitCount++;
        sprintf(cmd, "A%c MP", m->charAxis);
        noWait(motor, cmd);
        getMdStatus(motor);
        if (m->status & MD_LIMIT )
            limitCount++;
        if (limitCount > 1) {
            m->status |= MDS_HARDW;
            gnirsLogMessage(CICS_DB_ERROR,
                    "Motor %d: Check cables, hardware", motor);
            rc = VME_CABLE_HARDW;
            goto errorOff;
        }
        /* Check motor driver fault indicator, set in getMdStatus */
        if(m->status & MDS_FAULT) {
            gnirsLogMessage(CICS_DB_ERROR,
                    "Motor %d: Drive fault", motor);
            rc = VME_DRIVE_FAULT;
            goto errorOff;
        }
    }
    /* As a side effect of the above calls to getMdStatus, the OTRAVEL
     * bit is set/cleared.  OTRAVEL can't be determined unless the
     * enable bit is set, which it is when we're about to turn the
     * motor off.
     */

    if (on) {
        m->status |= MDS_BUSY;
        setBit(m->enable);  /* Turn on the motor */
    } else
        clearBit(m->enable);

    /* BUSY is not turned off here since it is used to signal that the
     * status is available, and some other software status bits may need
     * to be set first.
     */
    return VME_OK;

errorOff:
    clearBit(m->enable);
    return rc;
}

/* Creep toward a switch position.
 * This makes the most sense if distance is positive; assuming the backlash
 * has been removed, the result should be consistent test to test.  If
 * distance is negative, the backlash will interfere!
 * test is either MD_HOME or MD_LIMIT.
 */
int motorCreep(int motor, int distance, int test) {
    int i, step, a, v;
    char cmd[MOTOR_CMD_LEN];
    int saveBoost;
    motorVars *m;
    int rc, rc2;

    m = motors[motor];
    /* If we don't need boosting, having this non-zero just complicates
     * things.  If we do need boosting, we probably should just turn it
     * on and leave it on until we reach the switch.
     */
    saveBoost = m->boostTime;
    m->boostTime = 0;

    if (distance < 0) {
        step = -1;
        distance = -distance;
    } else
        step = 1;
    if (test == MD_HOME) {
        a = m->probeH.acceleration;
        v = m->probeH.velocity;
    } else {
        a = m->probe.acceleration;
        v = m->probe.velocity;
    }

    rc = motorOn(motor);  /* motorOn turns on BUSY */
    if (rc != VME_OK) {
        m->status &= ~MDS_BUSY;
        m->health = BAD;
        return rc;
    }

    /* If we're going for the negative limit (step < 0), we need to make up
     * the backlash first.
     */
    /*  XXX   Unsure how to make this work __reliably__
    sprintf(cmd, "A%c AC%d VL%d MR%d GO", m->charAxis,
            m->backOff.acceleration, m->backOff.velocity, -2 * m->backlash);
    rc = waitFor(motor, cmd, TIMEOUT_MIN);
    */

    if (rc == VME_OK) {
        sprintf(cmd, "A%c AC%d VL%d MR%d GO", m->charAxis, a, v, step);
        for (i = 0; i < distance; i++ ) {
            rc = waitFor(motor, cmd, TIMEOUT_MIN);
            if (m->status & test)
                break;
            if (rc != VME_OK)
                break;
            if (gnirsG.simulation)
                if (i > distance/2)
                    break;
        }
    if (i == distance)
        rc = VME_ERROR;
    }
    m->boostTime = saveBoost;

    rc2 = motorOff(motor);
    m->status &= ~MDS_BUSY;
    if (rc2 != VME_OK) {
        m->health = BAD;
        return rc2;
    }

    return rc;
}

/* Probe routine.
 * Move toward a limit , and return when at that position.
 * When routine is called, motor should be within a backlash
 * distance, or so, from the desired (real) limit.
 */
int probeLimSwitch(int motor, int condition) {
    int rc, rc2, axis;
    int travelAllowed;
    int timeout;
    motorVars *m;
    int mask, test;
    char cmd[MOTOR_CMD_LEN];
    GNIRS_ST_MD *md;
    switchType *s;

    m = motors[motor];
    md = &motorDriver[m->controller];
    axis = m->intAxis;
    travelAllowed = 2 * BKOFF_FUDGE * m->backlash;

    /* How far to go, and how to test.  Here, we're looking for the switch
     * connected to the motor controller, or to the HARD limit control
     * circuitry, so we can ask for a much larger travel than we need.
     * The controller will stop at the limit.
     */
    mask = MD_LIMIT | MD_DIR;
    switch (condition) {
        case HARD_POS_LIM:
            travelAllowed += m->posLimit.offset;
            /* fall through */
        case POS_LIM:
            test = MD_LIMIT | MD_PLUS ;
            break;
        case HARD_NEG_LIM:
            /* Note that negLimit.offset is negative! */
            travelAllowed -= m->negLimit.offset;
            /* fall through */
        case NEG_LIM:
            travelAllowed *= -1;
            test = MD_LIMIT | MD_MINUS;
            break;
    }
    if ((m->status & mask) == test) {
        /* This should never happen, so tell operator/programmer */
        gnirsLogMessage(CICS_DB_ERROR, "Already at limit when probe called");
        return VME_ERROR;
    }

    sprintf(cmd, "A%c AC%d VL%d ", m->charAxis, m->probe.acceleration,
            m->probe.velocity);
    rc = noWait(motor, cmd);
    if (rc != VME_OK)
        return rc;

    sprintf(cmd, "A%c MR%d GO", m->charAxis, travelAllowed);
    timeout = motorTimeout(travelAllowed, &m->probe);

    /* Will stop when reach limit.  */
    rc = waitFor(motor, cmd, timeout);
    if (rc != VME_OK)
        return rc;

    /* we're there */
    if (gnirsG.simulation)
        m->status |= test;
    if ((m->status & mask) != test) {
        gnirsLogMessage(CICS_DB_ERROR, "Can't find limit in probe");
        return VME_ERROR;
    }
    switch (condition) {
        case POS_LIM:
        case NEG_LIM:
            return VME_OK;
            break;
        case HARD_POS_LIM:
            s = &m->posLimit;
            travelAllowed = m->posLimit.offset + LIM_FUDGE;
            break;
        case HARD_NEG_LIM:
            s = &m->negLimit;
            travelAllowed = m->negLimit.offset - LIM_FUDGE;
            break;
    }
    /* Now look for HARD limit position.  This means we have to turn
     * off the limits so we can get past the soft ones.
     */
    sprintf(cmd, "A%c LF", m->charAxis);
    rc = noWait(motor, cmd);
    if (rc != VME_OK)
        return rc;
    /* Move to hard limit switch */
    sprintf(cmd, "A%c MR%d GO", m->charAxis, travelAllowed);
    timeout = motorTimeout(travelAllowed, &m->probe);
    rc2 = waitFor(motor, cmd, timeout);
    /* Turn limits back on */
    sprintf(cmd, "A%c LN", m->charAxis);
    rc = noWait(motor, cmd);
    if (rc != VME_OK)
        return rc;
    if (rc2 != VME_OK)
        return rc2;
    /* Verify that have gotten to a hard limit: isEnabled should be false */
    if (isSet(m->isEnabled) && !gnirsG.simulation)
        return VME_ERROR;
    return VME_OK;
}

/* Find a particular switch.
 * Move to a position just shy of switch,
 * then use probe speed to move slowly into switch -- probeLimSwitch().
 */
int findTheLimit(int motor, int which) {
    motorVars *m;
    int rc;
    int roughPos;
    switchType *s;
    int negLimitWanted;
    char cmd[MOTOR_CMD_LEN];

    m = motors[motor];

    if (m->type == ROTARY) {
        gnirsLogMessage(CICS_DB_ERROR, "Rotary mechanism does not have limits");
        return VME_ERROR;
    }

    /* The motorOn tests corrupt the limits, so we have to test explicitly
     * here.
     */
    negLimitWanted = FALSE;
    switch (which) {
        case POS_LIM:
            sprintf(cmd, "A%c MP", m->charAxis);
            noWait(motor, cmd);
            getMdStatus(motor);
            if (checkLimit(motor) > 0)
                return VME_OK; /* already there */
            /* fall through */
        case HARD_POS_LIM:
            if(m->status & MDS_OTRAVEL)
                return VME_OK; /* at hard limit */
            s = &(m->posLimit);
            break;
        case NEG_LIM:
            sprintf(cmd, "A%c MM", m->charAxis);
            noWait(motor, cmd);
            getMdStatus(motor);
            if (checkLimit(motor) < 0)
                return VME_OK; /* already there */
            /* fall through */
        case HARD_NEG_LIM:
            if(m->status & MDS_OTRAVEL)
                return VME_OK; /* at hard limit */
            s = &(m->negLimit);
            negLimitWanted = TRUE;
            break;
        default:
            gnirsLogMessage(CICS_DB_ERROR, "Bad limit %d for findTheLimit", which);
            return VME_ERROR;
            break;
    }

    if (gnirsG.simulation)
        m->status &= ~(MD_DIR|MD_LIMIT);
    m->parked = FALSE;
    if (m->datumed) {
        /* go just shy of position.  Ignore HARD limits, as they just
         * mean there's a little bit more to go.
         */
        roughPos = s->position - (negLimitWanted ? -m->backlash : m->backlash);
        rc = goToPos(motor, roughPos, FALSE);
        if (rc != VME_OK) {
            /* If limit, don't have positions quite right; just back out.
             * This should not happen in normal operation with correct
             * positions; as the stop at the hard limit is sudden and counts
             * may be lost, we need to set the position correctly.
             */
            if ((rc != VME_ABORTED) && checkLimit(motor)) {
                (void) tellPV(motor);
                roughPos = m->currPos -
                   BKOFF_FUDGE * (negLimitWanted ? -m->backlash : m->backlash);
                rc = goToPos(motor, roughPos, FALSE);
            }
        }
    } else
        rc = chargeLimit(motor, s);      /* don't know where we are */

    /* Now go find limit slowly */
    if (rc == VME_OK)
        rc = probeLimSwitch(motor, which);
    if (rc == VME_OK)
        checkParked(motor);
    return rc;
}

/* Go rushing into the soft positive or negative limit,
 * then backoff a bit.
 */
int chargeLimit(int motor, switchType *s) {
    int rc, check;
    int travel, timeout, backoff;
    char * limName;
    motorVars *m;
    char cmd[MOTOR_CMD_LEN];

    m = motors[motor];
    backoff = m->backlash * BKOFF_FUDGE;
    travel = m->fullTravel * 1.05;  /* add a bit for safety's sake */
    if ( s->type == POS_LIM ) {
        check = MD_LIMIT | MD_PLUS;
        limName = "positive";
        backoff = -backoff;
    } else {
        check = MD_LIMIT | MD_MINUS;
        travel = -travel;
        limName = "negative";
    }
    if (!gnirsG.simulation)
        getMdStatus(motor);

    /* if are not at limit, go charging into it, and then back off.
     * if are there, just back off.  If looking for hard limit (just
     * to be sure it is still functional), we still find real limit
     * first; the hard limit will be beyond it.
     */
    if ((m->status & (MD_LIMIT | MD_DIR)) != check ) {
        timeout = motorTimeout(travel, &m->seek);
        sprintf(cmd, "A%c AC%d VL%d", m->charAxis,
                m->seek.acceleration, m->seek.velocity);
        rc = noWait(motor, cmd);
        if (rc != VME_OK)
            return rc;
        rc = motorPos(motor, travel, 0, timeout);
        if (rc != VME_OK)
            return rc;
        if(gnirsG.simulation)
            m->status |= check;
        if ((m->status & (MD_LIMIT | MD_DIR)) != check ) {
            gnirsLogMessage(CICS_DB_ERROR, "Can't find %s limit",
                    limName);
            return VME_ERROR;
        }
        /* We might have run into a hard limit, in which case, the backoff
         * value must be larger.
         */
        if (!isSet(m->isEnabled))
            backoff -= s->offset;
    }
    
    timeout = motorTimeout(backoff, &m->backOff);
    sprintf(cmd, "A%c AC%d VL%d", m->charAxis,
            m->backOff.acceleration, m->backOff.velocity);
    rc = noWait(motor, cmd);
    if (rc != VME_OK)
        return rc;
    rc = motorPos(motor, backoff, 0, timeout);
    if (rc != VME_OK)
        return rc;
    if(gnirsG.simulation)
        m->status &= ~check;
    if ((m->status & (MD_LIMIT | MD_DIR)) == check ) {
        gnirsLogMessage(CICS_DB_ERROR,
                "Can't get off of %s limit position", limName);
        return VME_ERROR;
    }
    return rc;
}

/* Find limit.  Enables motor power, finds the limit, turns off
 * motor power.
 */
int findLimit(int motor, int type) {
    int rc, rc2;
    motorVars *m;

    m = motors[motor];
    if (!m)
        return VME_OK;
    rc = motorOn(motor);
    if (rc != VME_OK) {
        m->status &= ~MDS_BUSY;
        m->health = BAD;
        return rc;
    }
    rc = findTheLimit(motor, type);
    rc2 = motorOff(motor);
    if (rc2 != VME_OK) {
        m->health = BAD;
    }
    m->status &= ~MDS_BUSY;
    if (rc != VME_OK)
        return rc;
    return rc2;
}


/* This routine finds the home position.  It is not called if the home
 * position is at a limit.  Use 'MA', or the 'KR' or 'KM', commands to get
 * close without changing the position counter.  Then backoff, and
 * approach slowly.  This routine is meant to be called only when the
 * motor has not been datumed, since it will reset the home position.
 */
int setHome(int motor) {
    int rc;
    int timeout, dist, offset;
    char cmd[MOTOR_CMD_LEN];
    char homeLevel;
    motorVars *m;

    m = motors[motor];

    if (!gnirsG.simulation)
        getMdStatus(motor);
    homeLevel = m->homeLevel;    /* default home switch */
    offset = m->home.position;
    if (m->status & MD_HOME)
        homeLevel = (homeLevel == 'L') ? 'H' : 'L';
    /* In the following command, use 'HM' or 'HR' to avoid rapid stop;
     * using 'H' instead of 'K' is ok since we don't know where we
     * are anyway.
     */
    sprintf(cmd, "A%c AC%d VL%d H%c H%c%d", m->charAxis,
            m->seek.acceleration, m->seek.velocity, homeLevel,
            (m->status & MD_HOME) ? 'R' : 'M' , offset);
    homeLevel = m->homeLevel;
    timeout = motorTimeout(m->fullTravel, &m->seek);
    rc = waitFor(motor, cmd, timeout);
    if (rc != VME_OK) {
        if (rc != VME_ABORTED) {
            gnirsLogMessage(CICS_DB_ERROR, "Failed to seek to home");
            abortMotor(motor); /* issues stop command, sets abort flag */
            m->aborted = FALSE; /* but not a real abort */
        }
        return rc;
    }
    /* We need to be at least the backlash distance on the negative
     * side of the home position.  Current position will be negative
     * if we were moving that direction.
     */
    (void) tellPV(motor);
    dist = BKOFF_FUDGE * m->backlash + m->currPos;
    /* even if dist is zero, send the command which will make sure the
     * home switch is sensed correctly.
     */
    sprintf(cmd, "A%c AC%d VL%d H%c MR%d GO", m->charAxis,
        m->backOff.acceleration, m->backOff.velocity, homeLevel, -dist);
    timeout = motorTimeout(dist, &m->backOff);
    rc = waitFor(motor, cmd, timeout);
    if (rc != VME_OK) {
        if (rc != VME_ABORTED)
            gnirsLogMessage(CICS_DB_ERROR, "Failed to back off home");
        return rc;
    }
    if (m->status & MD_HOME) {
        gnirsLogMessage(CICS_DB_ERROR, "Can't get off home switch");
        return VME_ERROR;
    }

    dist = BKOFF_FUDGE * m->backlash;
    sprintf(cmd, "A%c AC%d VL%d HM%d", m->charAxis,
        m->probe.acceleration, m->probe.velocity, offset);
    timeout = motorTimeout(2 * dist, &m->probe);
    rc = waitFor(motor, cmd, timeout);
    if (rc != VME_OK) {
        if (rc != VME_ABORTED)
            gnirsLogMessage(CICS_DB_ERROR, "Failed to probe home switch");
        return rc;
    }
    checkParked(motor); /* Calls tellPV() and sets m->parked */
    if (gnirsG.simulation)
        m->status |= MD_HOME;
    if (!(m->status & MD_HOME)) {
        gnirsLogMessage(CICS_DB_ERROR, "No home during probe motion");
        return VME_ERROR;
    }
    return VME_OK;
}

int datum(int motor) {
    int rc, rc2, rc3;
    motorVars *m;
    switchType *s;
    float a, v;
    int d;
    char cmd[MOTOR_CMD_LEN];
    
    m = motors[motor];
    if ((motor >= NUM_MOTORS) || !m) {
        gnirsLogMessage(CICS_DB_ERROR, "status req: motor %d not allocated",
                motor);
        return VME_ILLEGAL_MOTOR;
    }
    if (m->home.type == NEG_LIM) {
        gnirsLogMessage(CICS_DB_ERROR,
            "M%d home request at negative limit", motor);
        return VME_ERROR;
    }

    if (gnirsG.simulation)
        m->status = 0;
    else {
        m->status &= ~MDS_STAT_MASK;
    }
    m->health = GOOD;
    m->lastPos = m->currPos;
    rc = motorOn(motor);
    if (rc != VME_OK) {
        m->status &= ~MDS_BUSY;
        m->health = BAD;
        return rc;
    }
    m->datumed = FALSE;
    switch (m->home.type) {
        case HOME_SW:
            s = &(m->home);
            m->reqPos = s->position;
            rc = setHome(motor);
            rc3 = VME_OK;
            break;
        case POS_LIM:
            rc = findTheLimit(motor, POS_LIM);
            if (rc == VME_OK) {
                /* Set the zero point on the controller position counter
                 * Since we don't use the controller's home command when
                 * going to a limit, we have to set the position manually.
                 * The system will have decelerated to a stop at probe
                 * rates when the limit is reached, so determine what to
                 * load into counter from those parameters.
                 */
                v = (float)m->probe.velocity;
                a = (float)m->probe.acceleration;
                d = (v * v) / (2. * a) + 0.5;
                sprintf(cmd, "A%c LP%d", m->charAxis, d);
                rc3 = noWait(motor, cmd);
                if ((rc3 != VME_OK) && (rc3 != VME_ABORTED))
                    gnirsLogMessage(CICS_DB_ERROR,
                        "Failed to load position counter");
            }
            if (gnirsG.simulation)
                m->status |= MD_LIMIT | MD_PLUS;
            break;
    }
    /* Test return from routine that does the work */
    if (rc != VME_OK) {
        if (rc == VME_TIMEOUT) {
            m->health = WARNING;
            /* CHECK FOR OVERTRAVEL XXX */
            m->status |= MDS_STALL;
        } else if (rc != VME_ABORTED)
            m->health = BAD;
    } else {
        /* motor is now datumed!  checkParked() was called by 
         * either setHome or position.
         */
        m->datumed = TRUE;
        m->currPos = m->lastPos = m->home.position;
    }
    rc2 = motorOff(motor);
    if (rc2 != VME_OK) {
        m->health = BAD;
    }
    m->status &= ~MDS_BUSY;
    if (rc2 != VME_OK)  /* hardware failure */
        return rc2;
    else if (rc3 != VME_OK)
        return rc3;    /* failed to set position counter */
    else
        return rc;    /* failed to set home */
}

void abortMotor(int motor) {
    int rc;
    motorVars *m;
    SEM_ID sem;
    GNIRS_ST_MD *md;
    char cmd[MOTOR_CMD_LEN];

    m = motors[motor];
    md = &motorDriver[m->controller];
    sprintf(cmd, "A%c ST ", m->charAxis);

    /* lock out interrupts */
    sysIntDisable(GNIRS_MOTOR_INTERRUPT_LEVEL);
    taskLock();
    /* lock out tasks */
    /* XXX BUT, do we know that the motor is still moving...there's a slim
     * chance we issue the abort just as it's about to stop.  What does
     * that do ... will aborted be reset to FALSE by someone?
     */
    m->aborted = TRUE;  /* cleared at top level by motor control task */
    sem = md->waiting[m->intAxis];
    md->waiting[m->intAxis] = NULL;
    /* allow tasks and interrupts */
    taskUnlock();
    sysIntEnable(GNIRS_MOTOR_INTERRUPT_LEVEL);
    rc = sendToMotorDriver(m->controller, cmd, NULL);
    if (rc != VME_OK) {
        gnirsLogMessage(CICS_DB_ERROR, "Failed to command stop for motor %d",
                motor);
    }
    /* Tell task that was waiting that it's over; when waitFor gets the
     * semaphore, it will see that it's been aborted, and will return
     * after getting status.  The status that it gets, however, will be
     * slightly in error as it is likely the motor won't have fully stopped
     * yet.
     */
    if (sem)
        semGive(sem);
}

/* Test to see if OMS VME cards are alive */
int testOMSCards() {
    int rc, i;
    char response[MOTOR_MSG_LEN];
    GNIRS_ST_MD *md;

    if (gnirsG.simulation)
        return VME_OK;

    for ( i = 0; (i < NUM_CARDS) && ( i < mdMax) ; i++ ) {
        md = &motorDriver[i];
        /* Now ask card who it is: */
        rc = sendToMotorDriver(i, "WY", response);
        if (rc != VME_OK) {
            gnirsLogMessage(CICS_DB_ERROR, "Failed to send 'WY' to C%d", i);
            return rc;
        }
        rc = strncmp(response, md->aliveID, strlen(md->aliveID));
        if (rc) {
            gnirsLogMessage(CICS_DB_ERROR, "C%d ID error -- got <%s>", i,
                    md->aliveID);
            return VME_ERROR;
        }
    }
    return VME_OK;
}

/* Returns composite health of all the motors */
int motorHealthTree() {
    int health;
    motorVars *m;
    int i;

    health = GOOD;
    for (i = 0; i < NUM_MOTORS; i++ ) {
        m = motors[i];
        if (!m)
            continue;
        switch (m->health) {
            case GOOD:
                break;
            case WARNING:
                health = WARNING;
                break;
            case BAD:
                return BAD;
                break;
        }
    }
    return health;
}

BOOL isBusy(int motor) {
    return (motors[motor]->status & MDS_BUSY) ? TRUE : FALSE ;
}

BOOL isInError(int motor) {
    return (motors[motor]->status & MDS_ERROR) ? TRUE : FALSE ;
}

BOOL isParked(int motor) {
    motorVars *m;

    m = motors[motor];
    if (!m)
        return FALSE;
    checkParked(motor);
    return m->parked;
}

/* Check to see if at the parked postion.  This doesn't work when
 * in simulation mode.
 */
void checkParked(int motor) {
    motorVars *m;
    int negLimit;
    int below, above;

    m = motors[motor];
    if (!m)
        return;

    (void) tellPV(motor);
    m->parked = FALSE;
    /* allow a bit of margin, just in case (though I don't _know_ why I
     * should bother.
     */
    below = m->parkPosition - 3;
    above = m->parkPosition + 3;
    if ((m->currPos <= above) && (m->currPos >= below)) {
        m->parked = TRUE;
        return;
    }
    /* If the match fails, then perhaps the park position is the home
     * position and at a switch, in which case one could be parked but
     * not exactly at the position called for (all that is desired in
     * this case is that the motor be at the (limit) switch.
     */
    if ((m->parkPosition == m->home.position) && (m->home.type != HOME_SW))
        if (m->status & MD_LIMIT) {
            negLimit = ((m->status & MD_DIR) == MD_MINUS);
            if ((negLimit  && (m->home.type == NEG_LIM)) ||
                (!negLimit && (m->home.type == POS_LIM)) )
                    m->parked = TRUE;
        }

    return;
}

/* Park the given mechanism */
int parkOne(int motor) {
    motorVars *m;

    m = motors[motor];
    if (!m)
        return VME_ILLEGAL_MOTOR;

    if (gnirsG.simulation) {
        m->parked = TRUE;
        m->currPos = m->parkPosition;
        return VME_OK;
    }
    checkParked(motor);
    if (m->parked == TRUE)
        return VME_OK;

    /* If the home switch is not a limit, then the park position is
     * a position like all others, and nothing special needs to be done.
     * We just go there via the 'position' command.
     * If that's not true, then we have to find the appropriate limit.
     */
    if (m->home.type != HOME_SW)
        return findLimit(motor, m->home.type);
    else
        return position(motor, m->parkPosition);
    /* Note that both position() and findLimit() check for parked status
     * when they complete successfully.  Therefore, we don't have to set
     * m->parked here.
     */
}

/* parkMotor and datumMotor are for use by the doAll() routine */
void parkMotor(int motor) {
    motorVars *m;

    m = motors[motor];
    if (!m)
        return;
    m->reqOp = parkOne;
}

void datumMotor(int motor) {
    motorVars *m;

    m = motors[motor];
    if (!m)
        return;
    m->reqOp = datum;
}

void resetFault(int motor) {
    motorVars *m;

    m = motors[motor];
    if (!m)
        return;
    clearBit(m->reset);
    taskDelay(1);
    setBit(m->reset);
}
