static struct
  {
      void *v;
      char *c;
  }
sccsid =
{
    &sccsid,
        "@(#)motors.c	1.10 09/24/03"
};

#include <vxWorks.h>
/* #include <stdio.h> */
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
#include "gnirsCC.h"
#include "timeLib.h"
#include "epicsTypes.h"
#include "status.h"

int sendStatus(long index,long type,void *p);

/* Motor Controllers */
int sendToMotorDriver(int, char *, char *);
int chargeLimit(int, switchType *);

/* 0: disabled  1: messages  2: and timeouts  3: and responses */
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

void mErrMsg(motorVars *m, const char *fmt, ...) {
    char buffer[200];
    va_list arglist;

    /* build message from input */
    va_start(arglist, fmt);
    vsprintf(buffer, fmt, arglist);
    va_end(arglist);

    /* Store for EPICS*/
    if (m) 
	{
        strncpy(m->errorMsg, buffer, ERR_MSG_LEN);
        m->errorMsg[ERR_MSG_LEN - 1] = '\0';   /* safety */
        /* Tell whoever is listening */
        gnirsLogMessage(CICS_DB_ERROR, m->errorMsg);
    } else {
        gnirsLogMessage(CICS_DB_ERROR, buffer);
    }
}
int setFocus (int i)
{
	focusMode = i;
	return OK;
		
}
int sendToMotorDriver(int controller, char *string, char *response) {
    GNIRS_ST_MD *md;
    mdRegister *mdr;
    char c;
    int rc, n;
    unsigned char control;
    
    /* Print out every command, but skip the report ones unless
     * sendToMotorDebug is 3 or more
     */
    if (sendToMotorDebug) 
	{
        c = *(string + 3);
        if (((c == 'R') && sendToMotorDebug > 2) || (c != 'R'))
            printf("send%d: %s\n", controller, string);
    }

    if (gnirsG.simulation)
	{
        return VME_OK;
    }

    md = &motorDriver[controller];

    if(strlen(string) > rngFreeBytes(md->outRing))
	{
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
    /* XXX
    sysIntDisable(GNIRS_MOTOR_INTERRUPT_LEVEL);
    */
    control = mdr->control;
    mdr->control = control | MD_TBE_E;
    /* XXX
    sysIntEnable(GNIRS_MOTOR_INTERRUPT_LEVEL);
    */

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

void getMdStatus(int motor) 
{
	long l;
    int i, rc, controller;
    char mstat;
    char buf[MOTOR_MSG_LEN], c;
    motorVars *m;
    GNIRS_ST_MD *md;
    char cmd[MOTOR_CMD_LEN];

    m = motors[motor];
    if ((motor >= NUM_MOTORS) || !m) {
        mErrMsg(m, "status req: motor %d not allocated", motor);
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
            "Illegal motor cmd in C%d:\n\t<%s>", controller, md->cmdErr);
    }

    for (i = 0; i < MH_STAT_SIZE; i++ ) 
	{
        c = buf[i];
        switch (i) {
		  case 0:
			mstat = 0;
			if (c == 'M')
				mstat |= MH_MINUS;	/* Moving in negative direction	*/
			break;
		  case 1:
			if (c == 'D')
				mstat |= MH_DONE;	/* Done (ID, II, or IN command has been executed)	*/
			break;
		  case 2:
			if (c == 'L')
				mstat |= MH_LIMIT;	/* Axis in overtravel	*/
			break;
		  case 3:
			if (c == 'H')
				mstat |= MH_HOME;	/* Home switch is active	*/
			m->status = (m->status & ~MH_STAT_MASK) | mstat;
			break;
        }
    }
    /* If enable is on, isEnabled will be on if no overtravel */
    if (isSet(m->enable)) 
	{
		if (isSet(m->isEnabled))
		{
			m->status &= ~MS_OTRAVEL;
		}
		else if (mstat & MH_LIMIT)
		{
			printf("We've hit a limit: overtravel !!!\n");
			m->status |= MS_OTRAVEL;
			m->datumed = FALSE; 
		}
		else
		{
			/* Not enabled, but we didn't hit a limit. We can't trust it */
			printf("Spurious overtravel\n");
		}
    }
/* 	printf("datumed = %d\n",m->datumed); */   
	l = m->datumed;
	sendStatus(MECHDATUMED + motor, EPLONG, &l);

	/* Fault is true low */
	if (isClear(m->fault))
		m->status |= MS_FAULT;
	else
		m->status &= ~MS_FAULT;
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
        if (sendToMotorDebug > 1)
	    printf("timeout is %d for %d %d %d\n", time, dist,
            ms->velocity, ms->acceleration);
    return time;
}

int setupMotor(int motor) {
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

    /* filled in correctly via config file and finshMotorConfig() */
    m->controller = 0;
    m->intAxis = 2;
    m->charAxis = axisLetter[m->intAxis];

    /* fill in default values */

    m->homeLevel = 'L';
    if (first) {
        m->status = NULL;
        m->health = GOOD;
    }
    m->type = LINEAR;
    m->fullTravel = 4530;
    m->backlash = 200;
    
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
     * This is a bit silly as the type is defined above as LINEAR
     * and home.type is HOME_SW.
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

    m->aborted = FALSE;
    if (first) {
        m->datumed = FALSE;
        m->parked = FALSE;
    }
    m->reqOp = 0;
    m->reqArg = 0;
    /* Configuration will be finished when config file is read */
    
    return VME_OK;
}
void setbit (int x, int y)
{
	ioBit a;
	
	a.port = x;
	a.bit = y;
	setBit (a);
}

void clearbit (int x, int y)
{
	ioBit a;
	
	a.port = x;
	a.bit = y;
	clearBit (a);
}
void readbit(int x,int y)
{
	ioBit a;
	
	a.port = x;
	a.bit = y;
	printf("bit state %d\n\n",isSet(a));
}
void finishMotorConfig(int motor) {
    motorVars *m;
    int n;
    char *p;

    m = motors[motor];

    if (m->home.useAlt) 
	{
	/* 	printf("Alt home in use %d %d %d\n\n",motor,m->home.control.port,m->home.control.bit); */
        m->home.position = m->home.offset;
        setBit(m->home.control);
    } 
	else 
	{
	/* 	printf("standard home in use %d %d %d\n\n",motor,m->home.control.port,m->home.control.bit); */
        m->home.position = 0;
        clearBit(m->home.control);
    }

    m->charAxis = axisLetter[m->intAxis];

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
        /* semMoveMotor controls running of motor task spawned
         * just below.  These are used for command line control,
         * not by the EPICS interface.
         */
        if(m->semMoveMotor)
            semDelete(m->semMoveMotor);
        sem = semBCreate(SEM_Q_FIFO, SEM_EMPTY);
        if (sem == NULL) {
            mErrMsg(m, "Unable to create semMoveMotor for motor %d.", motor);
            return VME_MEMORY_ERROR;
        }
        m->semMoveMotor = sem;

        if (m->taskID)
            taskDelete(m->taskID);
        sprintf(name, "tMotor%02d", motor);
        m->taskID = taskSpawn(name, 50, VX_FP_TASK, 4000, motorTask, motor, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        if(m->taskID == ERROR) {
            m->taskID = 0;
            mErrMsg(m, "Failed to spawn motor%02d task", motor);
            return VME_ERROR;
        }

        /* Reset Phytron
         * turn off enable
         *   ... These should all be off/disabled when system boots ...
         */
        clearBit(m->reset);
        clearBit(m->enable);    /* use this as delay for reset as well */
        setBit(m->reset);
 

        /* Now tell the motor to do the init string */

        rc = noWait(motor, m->initString);
        if (rc != VME_OK) {
            return rc;
        }
        /* There's nothing to check after the init string in the returned
         * status since it's ok to be at HOME or at a LIMIT,
         * and the init string is not allowed to initiate any motion
         * (though there's no check for that here).
         */
        
        /* Get current position */
        rc = tellPV(motor);
        if (rc != VME_OK)
            return rc;
        if (gnirsG.simulation)
            m->currPos = 0;     /* hard to know what (else) to do */
    }
   return VME_OK;
}

int motorTask(int motor, int a1, int a2, int a3, int a4, int a5, int a6,
        int a7, int a8, int a9) {
    motorVars *m;

    m = motors[motor];
    if ((motor >= NUM_MOTORS) || !m) {
        mErrMsg(m, "status req: motor %d not allocated", motor);
        return VME_ILLEGAL_MOTOR;
    }
    for(;;) {
        /* wait until there's a command pending.*/
        semTake(m->semMoveMotor, WAIT_FOREVER);
		printf("motor task running\n");
        /* Now do it */
        moveOneMotor(motor);
        if (m->returnValue != VME_OK) {
            /* tell world of failure */
            gnirsLogMessage(CICS_DB_ERROR, m->errorMsg);
        }
    }
}

int moveOneMotor(int motor) 
{
   long l;
   char s[EPICS_LEN];
   int rc, status;
   int i, pos;
   int (*func)();             /* function to call to perform MoveMotor op */
   motorVars *m;
   char posRpt[EPICS_LEN * 2];
   /* FOCUS is a special case as the needed position may depend on all
    * the other mechanisms' final positions.  reqOp won't be NULL
    * if valid() had been called for the FOCUS mechanism.
    */

   /*modified to for focus to respond to position command instead of NULL PR*/
   m = motors[motor];
   if ((motor == FOCUS) && (m->reqOp == position)&&(focusMode == AUTOFOCUS)) {/* and auto*/
      for (pos = 0, i = 0; i < NUM_MECH; i++) {
	 pos += mechanism[i].fShift();
      }
      /*   m->reqOp = position; */
      m->reqArg = (int) pos;
   }

   /* Don't move if current position is the same as the requested
    * one.  This test is here to avoid even just turning on the motor
    * when we don't have to.  The test only works for the Rotary and
    * Linear motors since they use 'position'; the Binary ones use
    * 'findTheLimit'.  But for those, it doesn't matter if they
    * move a little since there is no precise positioning requirement
    * that is not enforced by the mechanism itself.
    */

   if ((m->reqOp == position) && 
	 ((m->currPos == m->reqArg)||
	  ((m->type==ROTARY)&&
	   ((m->fullTravel+m->currPos == m->reqArg)||
	    (m->currPos == m->fullTravel+m->reqArg))))) 
   {
      m->errorMsg[0] = '\0';
      m->reqOp = NULL;
      rc = VME_OK;
   } else if (m->reqOp) {

      printf("cur = %d, %d\n",m->currPos, m->reqArg);
      m->errorMsg[0] = '\0';
      func = m->reqOp;
      m->reqOp = NULL;        /* no accidental repeat , but if another
			       * valid call while this running, it's ok
			       * to have this become non-null again.
			       */

      healthString(s, m->health, EPICS_LEN); 
      sendStatus(MECHHEALTH + motor, EPSTRING, s);

      gnirsG.newConfiguration = TRUE;
      if ( (motor == GRATING) &&
	    ((strncmp("**", mechanism[motor].reqPosition, 2) == 0))) {
	 gratingWavelength = 0.0;
	 gratingAngle = 0.0;
	 gratingOrder = 0;
	 gratingStep = 0;
      }
      /*         strncpy(&mechPos[motor][0], "Moving", 7); */
      /*set mechPos in epics*/
      strcpy(s, "Moving");
      sendStatus(MECHPOS + motor, EPSTRING, s);
      /*   strncpy(&mechState[motor][0], "Moving", 7);	 */
      /*set mechState in epics*/
      strcpy(s, "Moving");
      sendStatus(MECHSTATE + motor, EPSTRING, s);

      /* BUSY flag set in motorOn routine */
      rc = (*(func))(motor, m->reqArg);
      /* and turned off in motorOff */
      gnirsLogMessage(CICS_DB_FULL,
	    "---DONE--- motor %d op %x arg %x",
	    motor, func, m->reqArg);
   } else
      rc = VME_OK;
   if (rc != VME_OK && !m->aborted)
      m->status |= MS_ERROR;     /* be sure it is set */
   m->returnValue = rc;
   /* Update various SAD structures */
   /*  mechDatumed[motor] = m->datumed; */
   /*set datumed in epics*/
   l = m->datumed;
   printf("motor = %d, datumed = %d \n",motor,m->datumed);
   sendStatus(MECHDATUMED + motor, EPLONG, &l);

   /*   mechParked[motor] = m->parked; */
   /*set parked in epics*/
   sendStatus(MECHPARKED + motor, EPLONG, &(m->parked));


   /*     mechEng[motor] = m->currPos; */	
   sendStatus(MECHENG + motor, EPLONG, &(m->currPos));



   /*     healthString(&mechHealth[motor][0], m->health, EPICS_LEN); */
   healthString(s, m->health, EPICS_LEN);
   sendStatus(MECHHEALTH + motor, EPSTRING,  s);


   /*  mechPos[motor][0] = '\0'; */  /* Make it blank unless we know where we are */	
   /*set mechPos in epics*/
   strcpy(s, "");
   sendStatus(MECHPOS + motor, EPSTRING,  s);

   if (rc == VME_OK) 
   {
      /* 	strncpy(&mechPos[motor][0], mechanism[motor].positionName, EPICS_LEN); */
      /*set mechPos in epics*/
      printf("sadmotor POS >>>%s<<<", mechanism[motor].positionName);
      sendStatus(MECHPOS + motor, EPSTRING, mechanism[motor].positionName );

      /*    strncpy(&mechState[motor][0], "OK", 3);	 */
      /*set mechState in epics*/
      strcpy(s, "OK");
      sendStatus(MECHSTATE + motor, EPSTRING,  s);

   } 
   else 
   {
      /*         strncpy(&mechPos[motor][0], "Invalid", 8); */
      /*set mechPos in epics*/
      strcpy(s, "Invalid");
      sendStatus(MECHPOS + motor, EPSTRING,  s);

      /*fault string */
      switch (rc) {
	 case VME_TIMEOUT:
	    sprintf(posRpt, "Timed Out");
	    break;
	 case VME_DRIVE_FAULT:
	    sprintf(posRpt, "Drive Fault");
	    break;
	 case VME_CABLE_HARDW:
	    sprintf(posRpt, "Hardware Error");
	    break;
	 case VME_ABORTED:

	    sprintf(posRpt, "Motion Aborted");
	    /* 	rc = VME_OK; */
	    break;
	 case VME_ILLEGAL_MOTOR:
	    sprintf(posRpt, "Motor Unknown");
	    break;
	 default:
	    sprintf(posRpt, "See error msg");
	    break;
      }
      status = m->status;
      if (status & MS_OTRAVEL)
	 strcat(posRpt, ": OT Limit");
      else if (status & MS_STUCK)
	 strcat(posRpt, ": stuck?");
      else if (status & MS_STALL)
	 strcat(posRpt, ": stalled?");
      if (status & MH_LIMIT)
	 if ((status & MH_DIR) == MH_MINUS)
	    strcat(posRpt, ": - Limit");
	 else
	    strcat(posRpt, ": + Limit");
      /*  strncpy(&mechState[motor][0], posRpt, EPICS_LEN); */
      /*set mechState in epics*/
      sendStatus(MECHSTATE + motor,EPSTRING, posRpt);
   }
   return rc;  /* not of much use except for scripting */
}

int noWait(int motor, char *command) {
    return waitFor(motor, command, 0);
}

int waitFor(int motor, char * cmdString, int wait) {
    int rc, n;
    GNIRS_ST_MD *md;
    motorVars *m;
    SEM_ID sem;
    int waitTime;
    char stopCmd[8];

    m = motors[motor];
    md = &motorDriver[m->controller];

    if (m->aborted && wait)  /* ok to proceed for non-motion commands */
	{
		printf("aborted\n");
        return VME_ABORTED;
	}
    n = strlen(cmdString);
    n++;        /* account for space to be added */
    if (wait)
        n += 3;     /* space to add the " ID" string */
    if (n >= MOTOR_CMD_LEN) {
        mErrMsg(m, "cmd too long: <%.60s>", cmdString);
        return VME_ERROR;
    }
    strcpy(m->command, cmdString);
    if (wait)
        strcat(m->command, " ID");
    strcat(m->command, " ");

    if (wait && !gnirsG.simulation) {
        md->waiting[m->intAxis] = sem = md->semDone[m->intAxis];
    }
/* 	printf("moving motor\n"); */
    rc = sendToMotorDriver(m->controller, m->command, NULL);
    if (rc != VME_OK) {
        mErrMsg(m, " failed in waitFor");
        md->waiting[m->intAxis] = NULL;
        return rc;
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
     * the distance to a switch is a bit more than we thought it would be.
     */
    waitTime = (1.2 * (float) wait);

    rc = semTake(sem, waitTime);
    if (rc != OK) {
        if (m->aborted) 
		{
            rc = VME_ABORTED;
        } 
		else if (errno == S_objLib_OBJ_TIMEOUT) 
		{
            /* Timeout: motor stuck */
            md->waiting[m->intAxis] = NULL;
            /* Worried here about race condition ... I wrote: (rjw 1)*/
            /* It could be that there was a semGive just after the
             * return from the semTake and before the previous line.
             * That would be the case if the motor was moving
             * unusually slowly. (rjw)
             */
            /* But the motor can not move other than at the rate specified
             * as the motor controller is what sets the rate and it is
             * oblivious, as far as I know, to the actual motor motion. (rjw 1)
             */
            /* Be sure the semaphore is clear.  The only way I know
             * to do this is to take the semaphore with NO_WAIT. (rjw)
             */
            semTake(sem, NO_WAIT);
            mErrMsg(m, "Timeout in waitFor for C%d%d",
                        m->controller, m->intAxis);
            rc = VME_TIMEOUT;
        } 
		else 
		{ /* not aborted, not timeout */
            mErrMsg(m, "C%d%d: Invalid semaphore in waitFor-1",
                        m->controller, m->intAxis);
            rc = VME_ERROR;
        }
    } 
	else if (m->aborted)
        rc = VME_ABORTED;
    else
        rc = VME_OK;

    /* Stop the controller if timeout; abort will have issued the 'ST' cmd */
    if (rc == VME_TIMEOUT) {
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

int motorPos(int motor, int where, int timeout) {
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
    if ((rc != VME_OK) && !(m->aborted)) {
	printf("error in waitFor\n");
        return rc;
    }
    /* Get here if rc == VME_OK or was aborted; in either case want to
     * get rotary position correct.
     */
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
	if(m->aborted) 
	    mErrMsg(m, "ABORTED %s motion\n", m->name);
	else 
	    mErrMsg(m, "destination %d, got %d", destination, m->currPos); return VME_ERROR;
    }

    /* If rotary motion has crossed the half way position, reset position
     * counter to correct value.
     */
    if ((m->type == ROTARY) && (abs(m->currPos) > (microRound(m->fullTravel/2)))) {
        if (m->currPos > 0)
            curr = m->currPos - m->fullTravel;
        else
            curr = m->currPos + m->fullTravel;
        sprintf(cmd, "A%c LP%d", m->charAxis, curr);
        rc = noWait(motor, cmd);
        if (rc != VME_OK) {
            mErrMsg(m, "Failed to reload position counter");
            return rc;
        }
        rc = tellPV(motor);       
        if (rc != VME_OK)
            return rc;
    }
    if (m->aborted)
        return VME_ABORTED;
    return VME_OK;
}

int position(int motor, int where) {
    motorVars *m;
    int rc, rc2;
    int initPosition;
    int count;
    int orig;

    m = motors[motor];
    if ((motor >= NUM_MOTORS) || !m) {
        mErrMsg(m, "status req: motor %d not allocated", motor);
        return VME_ILLEGAL_MOTOR;
    }
    /* check for no motion */
    (void) tellPV(motor);
    initPosition = microRound(m->currPos);
    if (microRound(where) == initPosition)
        return VME_OK;

    /* Shouldn't happen as higher code will check but just in case!  Also
     * let anything happen when not datumed (helpful in lab work)
     */
    if (m->datumed && m->type != ROTARY) {
        if (((where > 0) && (where > m->posLimit.position)) ||
                (where < m->negLimit.position)) {
            mErrMsg(m, "Position %d: too large", where); 
            return VME_ERROR;
        }
    }
    if (m->type == ROTARY) {
        orig = where;
        count = 0;
        while (abs(where) > microRound(m->fullTravel/2)) {
            count++;
            if (count > 10) {
                mErrMsg(m, "Rotary Position error %d", orig);
                return VME_ERROR;
            }
            if (where > 0)
                where -= m->fullTravel;
            else
                where += m->fullTravel;
        }
    }

    /* Power up */
    rc = motorOn(motor);
    if (rc != VME_OK) {
        m->health = BAD;
        return rc;
    }
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
            if (!(m->status & MH_LIMIT))
                m->status |= MS_STALL; /* timeout without hitting limit */
        } else if (rc != VME_ABORTED)
            m->health = BAD;
    }
    rc2 = motorOff(motor);
    if (rc2 != VME_OK) {
        m->health = BAD;
    }
    if ((rc != VME_ABORTED) && (microRound(m->currPos) == initPosition)) {
        /* failed to move at all */
        m->status |= MS_STUCK;
        if (m->health == GOOD)
            m->health = WARNING;
        rc = VME_ERROR;
    }
    if (gnirsG.simulation) {
        m->status = MH_DONE;
        if (where == m->home.position) 
            m->status |= MH_HOME;
    }
    if (m->status & MS_OTRAVEL)
        m->health = BAD;

    return rc;
}

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
        mErrMsg(m, "Cannot set seek accel/vel"); 
        return rc;
    }
    /* If distance to go is in positive direction and is less than
     * the backlash (which for positive motion isn't really a factor,
     * but gives us the distance along which we'll move slowly), then
     * don't bother with the fast motion.
     */
    if (m->datumed) {
        dist = where - m->currPos;
        if ((dist > 0) && (backlash >0) && (dist <= backlash) && removeBL)
		{
			printf ("dist < backlash\n");
            dist = 0;   /* Skip fast motion */
		}
		/* GNFR-75080: backlash is always positive, so the original test
		 * (backlash < 0) could never be true and small negative moves
		 * fell through to fast positioning, causing timeouts. Compare
		 * |dist| against the positive backlash instead.
		 */
		else if ((dist < 0) && (backlash > 0) && (abs(dist) <= backlash) && removeBL)
		{
			printf("GNFR-75080: skip fast-move, dist=%d backlash=%d\n", dist, backlash);
			dist = 0;
		}
        else
            dist -= backlash;
/* 		printf(" gotopos dist = %d\n",dist); */
    } else
        dist = m->fullTravel;
    if (dist) {
        timeout = motorTimeout(dist, &m->seek);
/* 		printf("fast positioning %d\n",where-backlash); */
        rc = motorPos(motor, where - backlash, timeout);
/* 		printf("returned from fast pos\n"); */
        /* Check for limit switch overtravel first */
        if (lim = checkLimit(motor, EITHER)) {
            mErrMsg(m, "\"%s\" %s limit hit (seek)",
                    m->name, (lim > 0) ? "positive" : "negative");
            return VME_ERROR;
        }
        if (rc != VME_OK) {
            if (! m->aborted)
                mErrMsg(m, "Failed on fast positioning");
            return rc;
        }
    }
    if (!removeBL)
        return VME_OK;

  /*   sprintf(cmd, "A%c AC%d VL%d", m->charAxis, */
/*             m->probe.acceleration, m->probe.velocity); */
    sprintf(cmd, "A%c AC%d VL%d", m->charAxis,
            m->backOff.acceleration, m->backOff.velocity);
    rc = noWait(motor, cmd);
    if (rc != VME_OK) {
        mErrMsg(m, "Cannot set probe accel/vel");
        return rc;
    }
    timeout = motorTimeout(backlash, &m->probe);
    if (m->datumed)
        rc = motorPos(motor, where, timeout);
    else
        rc = motorPos(motor, backlash, timeout); /* a relative move (MR) */
    if (lim = checkLimit(motor, EITHER)) {
        mErrMsg(m, "\"%s\" %s limit hit (slow)", m->name,
                (lim > 0) ? "positive" : "negative");
        return VME_ERROR;
    }
    if (rc != VME_OK) {
        if (! m->aborted)
            mErrMsg(m, "Failed on slow positioning");
        return rc;
    }
    return VME_OK;
}


int checkLimit(int motor, int flag) {
    motorVars *m;
    char cmd[MOTOR_CMD_LEN];

    m = motors[motor];
    if (flag == NEGATIVE) {
        sprintf(cmd, "A%c MM", m->charAxis);
        noWait(motor, cmd);
        getMdStatus(motor);
        if (m->status & MH_LIMIT)
            return -1;
        return 0;
    } else if (flag == POSITIVE) {
        sprintf(cmd, "A%c MP", m->charAxis);
        noWait(motor, cmd);
        getMdStatus(motor);
        if (m->status & MH_LIMIT)
            return 1;
        return 0;
    }
    /* EITHER / default */
    if (m->status & MH_LIMIT)
        if((m->status & MH_DIR) == MH_MINUS)
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

int motorOnOff(int motor, BOOL on) {
    motorVars *m;
    int limitCount;

    m = motors[motor];
    if (!m) {
        mErrMsg(m, "Can't control inactive motor %d", motor);
        return VME_ERROR;
    }

    if (gnirsG.simulation ) {
        m->status = 0;
    } else if (on) {
	/* Clear all software status bits */
        m->status &= ~MS_STAT_MASK;
      
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
        if (checkLimit(motor, NEGATIVE))
            limitCount++;
        if (checkLimit(motor, POSITIVE))
            limitCount++;
        if (limitCount > 1) {
            m->status |= MS_HARDW;
            mErrMsg(m, "Motor %d: Check cables, hardware", motor);
            return VME_CABLE_HARDW;
        }
    }

    /* Check motor driver fault indicator, set in getMdStatus */
    if(m->status & MS_FAULT) {
	mErrMsg(m, "Motor %d: Drive fault", motor);
	return VME_DRIVE_FAULT;
    }

    /*
     * OTRAVEL can only be determined when the enable bit is on; so it's
     * set or cleared in getMdStatus rather than here when we're
     * about to turn the motor off (enable off).
     */

    if (on) {
        /* Set all the status bits to 'ok' */ 
		/* and set BUSY.  The assumption here is that all motions must
         * go through this routine, and there are no other operations
         * besides motion commands.
         */
        m->status |= MS_BUSY;
        m->aborted = FALSE;
        m->health = GOOD;
        setBit(m->enable);  /* Turn on the motor */
    } else {
        clearBit(m->enable);
        m->status &= ~MS_BUSY;     /* Tell world we're done */
    }

    return VME_OK;
}

int motorCreep(int motor, int distance, int test) {
    int i, step, a, v;
    char cmd[MOTOR_CMD_LEN];
    motorVars *m;
    int rc, rc2;
/* 	printf("got here\n"); */
    m = motors[motor];

    if (distance < 0) {
        step = -2;
        distance = -distance;
    } else
        step = 2;
/* 	if(m->backlash < 0) */
/* 	{ */
/* 		step = -step; */
/* 	 	distance = -distance; */
/* 	} */
    if (test == MH_HOME) {
        a = m->probeH.acceleration;
        v = m->probeH.velocity;
    } else {
        a = m->probe.acceleration;
        v = m->probe.velocity;
    }
	a = 50;
	v = 50;
    rc = motorOn(motor);
    if (rc != VME_OK) {
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

    if (rc == VME_OK)
    {
	sprintf(cmd, "A%c AC%d VL%d MR%d GO", m->charAxis, a, v, step);
	for (i = 0; i < distance; i+=abs(step) ) {
            rc = waitFor(motor, cmd, TIMEOUT_MIN);
	    if(m->backlash < 0)
	    {
		if (!(m->status & test))
		    break;
	    }
	    else
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

    rc2 = motorOff(motor);
    if (rc2 != VME_OK) {
        m->health = BAD;
        return rc2;
    }

    return rc;
}

int motorSteps(int motor, int steps, int flag) {
    int rc, rc2;
    int a, v, timeout;
    motion *mo;
    motorVars *m;
    char cmd[MOTOR_CMD_LEN];

    m = motors[motor];

    rc = motorOn(motor);
    if (rc != VME_OK) {
        m->health = BAD;
        return rc;
    }

    mo = &m->probe;  /* default to slowest speed */
    if (flag == 1)
        mo = &m->backOff;
    else if (flag > 1)
        mo = &m->seek;
    
    a = mo->acceleration;
    v = mo->velocity;
    timeout = motorTimeout(steps, mo);
    /* Don't worry about backlash */

    sprintf(cmd, "A%c AC%d VL%d MR%d GO", m->charAxis, a, v, steps);
    rc = waitFor(motor, cmd, timeout);

    rc2 = motorOff(motor);
    if (rc2 != VME_OK) {
        m->health = BAD;
        return rc2;
    }

    return rc;
}

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
    if (motor == COVER)
        travelAllowed = 2000;   /* experimentally adjusted! */

    /* How far to go, and how to test?  Here, we're looking for either
     * the switch connected to the motor controller, or for the
     * HARD limit control circuitry, when we can ask for a much larger
     * travel than we need.
     * The controller will stop at the soft limit first.
     */
    mask = MH_LIMIT | MH_DIR;
    switch (condition) {
        case HARD_POS_LIM:
        case POS_LIM:
            test = MH_LIMIT | MH_PLUS ;
            break;
        case HARD_NEG_LIM:
        case NEG_LIM:
            travelAllowed *= -1;
            test = MH_LIMIT | MH_MINUS;
            break;
    }
    /* test works only if previous motion, which sets status, is in
     * the direction of the limit
     */
    if ((m->status & mask) == test) {
        /* This should never happen, so tell operator/programmer */
        mErrMsg(m, "Already at limit when probe called");
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
        mErrMsg(m, "Can't find limit in probe");
	printf("status = %d, mask = %d, test = %d, status & mask = %d\n",m->status,mask,test,m->status&mask);
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
    if (isSet(m->isEnabled) && !gnirsG.simulation) {
        mErrMsg(m, "isEnabled not false at hard limit");   
        return VME_ERROR;
    }
    return VME_OK;
}

int findTheLimit(int motor, int which) {
    motorVars *m;
    int rc;
    int roughPos;
    switchType *s;
    int negLimitWanted;

    m = motors[motor];

    if ((m->type == ROTARY) && (which != HOME_SW)) {
        mErrMsg(m, "Rotary mechanism does not have limits");
        return VME_ERROR;
    }

    /* The motorOn tests corrupt the limits, so we have to test explicitly
     * here.
     */
    negLimitWanted = FALSE;
    switch (which) {
        case HARD_POS_LIM:
            /* If looking for hard limit and have overtravel and
             * are at the positive limit as well, then we're at
             * the correct hard limit and can just return.
             */
            if ((checkLimit(motor, POSITIVE)) && (m->status & MS_OTRAVEL))
                return VME_OK;
            /* fall through */
        case POS_LIM:
            s = &(m->posLimit);
            break;
        case HARD_NEG_LIM:
            if ((checkLimit(motor, NEGATIVE)) && (m->status & MS_OTRAVEL))
                return VME_OK;
            /* fall through */
        case NEG_LIM:
            s = &(m->negLimit);
            negLimitWanted = TRUE;
            break;
        default:
            mErrMsg(m, "Bad limit %d for findTheLimit", which);
            return VME_ERROR;
            break;
    }


    if (gnirsG.simulation)
        m->status &= ~(MH_DIR | MH_LIMIT);
    m->parked = FALSE;
    /* If in hard limit, it's just like we're not datumed because the
     * position counter in the controller will no longer correspond to
     * the position.
     */
    if ((!(m->status & MS_OTRAVEL)) && m->datumed) {
        /* go just shy of position of soft limit position */
        roughPos = s->position - (negLimitWanted ? -m->backlash : m->backlash);
        rc = goToPos(motor, roughPos, FALSE);
        if (rc != VME_OK) {
            /* If limit, don't have positions quite right; just back out more.
             * This should not happen in normal operation with correct
             * positions; but if we're between the soft and hard limits
             * or someone's been testing hard limits (where pulses get
             * lost) or the mechanism is sloppy like the cover ...
             */
            if ((rc != VME_ABORTED) && checkLimit(motor, EITHER)) {
                (void) tellPV(motor);
                roughPos = m->currPos -
                   BKOFF_FUDGE * (negLimitWanted ? -m->backlash : m->backlash);
                rc = goToPos(motor, roughPos, FALSE);
            }
        }
    } else
      {
	printf("not datumed\n");
        rc = chargeLimit(motor, s);      /* don't know where we are */
      }

    /* Now go find limit slowly */
    if (rc == VME_OK)
        rc = probeLimSwitch(motor, which);
    if (rc == VME_OK)
        checkParked(motor);
    return rc;
}

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
        check = MH_LIMIT | MH_PLUS;
        limName = "positive";
        backoff = -backoff;
    } else {
        check = MH_LIMIT | MH_MINUS;
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
    if ((m->status & (MH_LIMIT | MH_DIR)) != check ) {
        timeout = motorTimeout(travel, &m->seek);
        sprintf(cmd, "A%c AC%d VL%d", m->charAxis,
                m->seek.acceleration, m->seek.velocity);
        rc = noWait(motor, cmd);
        if (rc != VME_OK)
            return rc;
        rc = motorPos(motor, travel, timeout);
        if (rc != VME_OK)
            return rc;
        if(gnirsG.simulation)
            m->status |= check;
        if ((m->status & (MH_LIMIT | MH_DIR)) != check ) {
            mErrMsg(m, "Can't find %s limit", limName);
            return VME_ERROR;
        }
    }
    /* We might have run into a hard limit, in which case, the backoff
     * value must be larger.
     */
    if (!isSet(m->isEnabled))
        backoff -= s->offset;

    timeout = motorTimeout(backoff, &m->backOff);
    sprintf(cmd, "A%c AC%d VL%d", m->charAxis,
            m->backOff.acceleration, m->backOff.velocity);
    rc = noWait(motor, cmd);
    if (rc != VME_OK)
        return rc;
    rc = motorPos(motor, backoff, timeout);
    if (rc != VME_OK)
        return rc;
    if(gnirsG.simulation)
        m->status &= ~check;
    sprintf(cmd, "A%c M%c", m->charAxis, s->type == POS_LIM ? 'P' : 'M');
    noWait(motor, cmd);
    getMdStatus(motor);
    if ((m->status & (MH_LIMIT | MH_DIR)) == check ) {
        /* We could be near the hard limit, but more than a backlash
         * distance away from the soft limit, so if we're looking for the
         * latter, we need a larger backoff distance.
         */
        backoff = -(s->offset + backoff);
        timeout = motorTimeout(backoff, &m->backOff);
        sprintf(cmd, "A%c AC%d VL%d", m->charAxis,
                m->backOff.acceleration, m->backOff.velocity);
        rc = noWait(motor, cmd);
        if (rc != VME_OK)
            return rc;
        rc = motorPos(motor, backoff, timeout);
        if (rc != VME_OK)
            return rc;
        if ((m->status & (MH_LIMIT | MH_DIR)) == check ) {
            mErrMsg(m, "Can't get off of %s limit position", limName);
            return VME_ERROR;
        }
    }
    return rc;
}

int gotoLimit(int motor, int limit) {
    motorVars *m;
    m = motors[motor];
    if ((motor >= NUM_MOTORS) || !m) {
        mErrMsg(m, "status req: motor %d not allocated", motor);
        return VME_ILLEGAL_MOTOR;
    }

    if ((limit == POS_LIM) && (checkLimit(motor, POSITIVE)))
        /* Already at positive limit */
        return VME_OK;
    if ((limit == NEG_LIM) && (checkLimit(motor, NEGATIVE)))
        /* Already at negative limit */
        return VME_OK;

    return findLimit(motor, limit);
}

/* Find limit.  Enables motor power, finds the limit, turns off
 * motor power.
 */
int findLimit(int motor, int limit) {
    int rc, rc2;
    motorVars *m;

    m = motors[motor];
    if (!m)
        return VME_OK;
    rc = motorOn(motor);
    if (rc != VME_OK) {
        m->health = BAD;
        return rc;
    }
    rc = findTheLimit(motor, limit);
    if (rc != VME_OK)
        m->health = WARNING;
    rc2 = motorOff(motor);
    if (rc2 != VME_OK) {
        m->health = BAD;
    }
    if (rc != VME_OK)
        return rc;
    return rc2;
}


/* Since code below temporarily sets the 'wrong' home level,
* make sure that, if aborted, home level is correct
*/

static int checkRetVal(int rc, int motor, char *msg) {
    char cmd[MOTOR_CMD_LEN];
    motorVars *m; 
    m = motors[motor];

    if (rc != VME_OK) {
        if (rc != VME_ABORTED) {
            mErrMsg(m, msg);
            abortMotor(motor);          /* issues stop command, sets abort flag */
            m->aborted = FALSE;         /* but not a real abort */
            sprintf(cmd, "A%c H%c", m->charAxis, m->homeLevel);
            (void) noWait(motor, cmd);
            printf("setHome aborted\n");
        }
    }

    return rc;
}


int setHome(int motor) {
    motorVars *m;
    int setRotaryHome(int motor);
    int setLinearHome(int motor);

    m = motors[motor];
    if (m->type == ROTARY) return setRotaryHome(motor);
    return setLinearHome(motor);
}


int setRotaryHome(int motor) {
    int rc;
    int pos;
    int timeout, dist;
    char cmd[MOTOR_CMD_LEN];
    motorVars *m;

    m = motors[motor];

    printf("Datuming rotary mechanism %d\n", motor);

    if (!gnirsG.simulation)
        getMdStatus(motor);




    /* Fast move to get us near the home switch */

    sprintf(cmd, "A%c AC%d VL%d H%c HR%d H%c HR%d", m->charAxis,
        m->seek.acceleration, m->seek.velocity, m->homeLevel, m->home.position, (m->homeLevel == 'L') ? 'H' : 'L', m->home.position);

    timeout = motorTimeout(m->fullTravel * 2, &m->seek);

    rc = waitFor(motor, cmd, timeout);

    if (checkRetVal(rc, motor, "Failed to move off home switch, or to find the home switch.") != VME_OK) return rc;





    (void) tellPV(motor);

    pos = -BKOFF_FUDGE * m->backlash; 				/* Direction test pr	*/
    dist = abs(BKOFF_FUDGE * m->backlash) + abs(m->currPos);	/* Direction test pr	*/




    /* Move further onto the home switch */

    sprintf(cmd, "A%c AC%d VL%d MA%d GO", m->charAxis,
        m->backOff.acceleration, m->backOff.velocity, pos);

    timeout = motorTimeout(dist, &m->backOff);

    rc = waitFor(motor, cmd, timeout);

    if (checkRetVal(rc, motor, "Failed to move further onto home switch.") != VME_OK) return rc;




    dist = BKOFF_FUDGE * m->backlash;




    /* Move off home switch positive */

    sprintf(cmd, "A%c AC%d VL%d H%c HM%d GO", m->charAxis,
         m->probe.acceleration, m->probe.velocity, m->homeLevel, m->home.position);

    timeout = motorTimeout(2 * dist, &m->probe);

    rc = waitFor(motor, cmd, timeout);

    if (checkRetVal(rc, motor, "Failed to slow approach home switch") != VME_OK) return rc;




    /* Return home level to default */

    sprintf(cmd, "A%c H%c", m->charAxis, m->homeLevel);	/* Direction test pr*/

    (void) noWait(motor, cmd);/*direction test pr*/




    checkParked(motor); /* Calls tellPV() and sets m->parked */

    if (gnirsG.simulation)
        m->status |= MH_HOME;

    if (!(m->status & MH_HOME)) {
        mErrMsg(m, "No home during probe motion");
        return VME_ERROR;
    }

    return VME_OK;
}


int setLinearHome(int motor) {
    int rc;
    int pos;
    int timeout, dist, offset;
    char cmd[MOTOR_CMD_LEN];
    char homeLevel;
    motorVars *m;

    m = motors[motor];

    printf("Datuming linear mechanism %d\n", motor);

    if (!gnirsG.simulation)
        getMdStatus(motor);

    homeLevel = m->homeLevel;    /* default home switch */
    offset = m->home.position;

    if (m->status & MH_HOME)
        homeLevel = (homeLevel == 'L') ? 'H' : 'L';

    /* In the following command, use 'HM' or 'HR' to avoid rapid stop;
     * using 'H' instead of 'K' is ok since we don't know where we
     * are anyway.
     */

    sprintf(cmd, "A%c AC%d VL%d H%c H%c%d", m->charAxis,
            m->seek.acceleration, m->seek.velocity, homeLevel,
            (m->status & MH_HOME) ? 'R' : 'M' , offset);
    homeLevel = m->homeLevel;

    timeout = motorTimeout(m->fullTravel, &m->seek);
    rc = waitFor(motor, cmd, timeout);

    if (checkRetVal(rc, motor, "Failed to seek to home") != VME_OK) return rc;

    /* We need to be at least the backlash distance on the negative
     * side of the home position.  Current position will be negative
     * if we were moving that direction.
     */

    (void) tellPV(motor);

    pos = -BKOFF_FUDGE * m->backlash; /*direction test pr*/
    dist = abs(BKOFF_FUDGE * m->backlash) + abs(m->currPos); /*direction test pr*/
    printf("dist = %d, pos = %d\n",dist,pos);

    /* even if dist is zero, send the command which will make sure the
     * home switch is sensed correctly.
     */

    sprintf(cmd, "A%c AC%d VL%d H%c MA%d GO", m->charAxis,
        m->backOff.acceleration, m->backOff.velocity, homeLevel, pos); /*direction test pr*/

    timeout = motorTimeout(dist, &m->backOff);

    rc = waitFor(motor, cmd, timeout);

    if (checkRetVal(rc, motor, "Failed to back off to home") != VME_OK) return rc;

    if ((m->status & MH_HOME) && (m->backlash > 0)) {
        mErrMsg(m, "Can't get off home switch");
        return VME_ERROR;
    }
    else {
        if (!(m->status & MH_HOME) && (m->backlash < 0)) {
            mErrMsg(m, "Can't get off home switch"); 
            return VME_ERROR;
        }
    }

    dist = BKOFF_FUDGE * m->backlash;

	/*might need to set to hr depending on direction*/

    if(m->backlash < 0) {
	if (m->status & MH_HOME) 
            homeLevel = (homeLevel == 'L') ? 'H' : 'L';
	sprintf(cmd, "A%c AC%d VL%d H%c HR%d", m->charAxis,
            m->probe.acceleration, m->probe.velocity, homeLevel ,offset);
    }
    else {
	sprintf(cmd, "A%c AC%d VL%d H%c HM%d", m->charAxis,
	    m->probe.acceleration, m->probe.velocity, homeLevel, offset);
    }

    timeout = motorTimeout(2 * dist, &m->probe);

    rc = waitFor(motor, cmd, timeout);

    if (checkRetVal(rc, motor, "Failed to probe home") != VME_OK) return rc;

    homeLevel = m->homeLevel;
    sprintf(cmd, "A%c H%c", m->charAxis, m->homeLevel);

    (void) noWait(motor, cmd);

    checkParked(motor); /* Calls tellPV() and sets m->parked */

    if (gnirsG.simulation)
        m->status |= MH_HOME;

    if (!(m->status & MH_HOME)) {
        mErrMsg(m, "No home during probe motion");
        return VME_ERROR;
    }

    return VME_OK;
}




int fakeDatum ()
{
    motorVars *m;
    int i;
    for (i=0;i<NUM_MOTORS;i++) {
	m = motors[i];
	m->datumed = TRUE;
    }
    return OK;
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
        mErrMsg(m, "status req: motor %d not allocated", motor);
        return VME_ILLEGAL_MOTOR;
    }
    if (m->home.type == NEG_LIM) {
        mErrMsg(m, "M%d home request at negative limit", motor);
        return VME_ERROR;
    }

    if (gnirsG.simulation)
        m->status = 0;
    else {
        m->status &= ~MS_STAT_MASK;
    }
    rc = motorOn(motor);
    if (rc != VME_OK) {
        m->health = BAD;
        return rc;
    }
    m->datumed = FALSE;
    rc3 = VME_OK;
    switch (m->home.type) {
        case HOME_SW:
            s = &(m->home);
            rc = setHome(motor);
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
                if (rc3 != VME_OK)
                    mErrMsg(m, "Failed to load position counter");
            }
            if (gnirsG.simulation)
                m->status |= MH_LIMIT | MH_PLUS;
            break;
    }
    /* Test return from routine that does the work */
    if (rc != VME_OK) {
        if (rc == VME_TIMEOUT) {
            m->health = WARNING;
            m->status |= MS_STALL;
        } else if (rc != VME_ABORTED)
            m->health = BAD;
    } else {
        /* motor is now datumed!  checkParked() was called by 
         * either setHome or position.
         */
        m->datumed = TRUE;
    }
    rc2 = motorOff(motor);
    if (rc2 != VME_OK) {
        m->health = BAD;
        return rc2;
    } else if (rc3 != VME_OK)
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
    /* lock out tasks */
    taskLock();
    /* XXX BUT, do we know that the motor is still moving?...there's a slim
     * chance we issue the abort just as it's about to stop.  What does
     * that do?
     */
    if (m->status & MS_BUSY)
		m->aborted = TRUE;  /* cleared at top level by motor control task */
    sem = md->waiting[m->intAxis];
    md->waiting[m->intAxis] = NULL;
    /* Allow other tasks */
    taskUnlock();
    /* Allow interrupts */
    sysIntEnable(GNIRS_MOTOR_INTERRUPT_LEVEL);
    /* Issue the stop command; if the motor is already stopped, that's
     * ok.
     */
    rc = noWait(motor, cmd);
    if (rc != VME_OK) {
        mErrMsg(m, "Failed to command stop for motor %d", motor);
    }
    /* Tell task that was waiting that it's over; when waitFor gets the
     * semaphore, it will see that it's been aborted, and will return
     * after getting status.  The status that it gets, however, will be
     * slightly in error as it is likely the motor won't have fully stopped
     * yet.  That is, we aren't waiting on an interrupt from the finish of
     * the (STOP) command, unlike the usual mode of operation.
     */
    if (sem)
        semGive(sem);
}

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
        /* oh kludge kludge...*/
        response[12] = 'x';
        response[13] = 'x';
        rc = strncmp(response, md->aliveID, strlen(md->aliveID));
        if (rc) {
            gnirsLogMessage(CICS_DB_ERROR,
                    "C%d ID error -- got <%s>", i, response);
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
    return (motors[motor]->status & MS_BUSY) ? TRUE : FALSE ;
}

BOOL isInError(int motor) {
    return (motors[motor]->status & MS_ERROR) ? TRUE : FALSE ;
}

BOOL isParked(int motor) {
    motorVars *m;

    m = motors[motor];
    if (!m)
        return FALSE;
    checkParked(motor);
    return m->parked;
}

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
     * position and at a limit switch, in which case one could be parked but
     * not exactly at the position called for (all that is desired in
     * this case is that the motor be at the (limit) switch.
     */
    if ((m->parkPosition == m->home.position) && (m->home.type != HOME_SW))
        if (m->status & MH_LIMIT) {
            negLimit = ((m->status & MH_DIR) == MH_MINUS);
            if ((negLimit  && (m->home.type == NEG_LIM)) ||
                (!negLimit && (m->home.type == POS_LIM)) )
                    m->parked = TRUE;
        }

    return;
}

/* Park the given mechanism */
int park(int motor) {
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

void resetFault() {
    motorVars *m;
    int i;

    for (i = 0; i < NUM_MOTORS; i++) {
        m = motors[i];
        if (!m)
            continue;
        if (m->status & MS_FAULT) {
            clearBit(m->reset);
            taskDelay(1);
            setBit(m->reset);
        }
    }
}


