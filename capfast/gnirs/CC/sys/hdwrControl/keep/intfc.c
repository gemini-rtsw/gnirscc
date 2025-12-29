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

BOOL gnirsInterruptsAllowed = FALSE;
static int lvTaskId;
static int scanStatusTaskId;

int motorDriverInit();
void gnirsMotor0IntHndlr(int);
void gnirsMotor1IntHndlr(int);
void gnirsMotor2IntHndlr(int);
void motorIntHandler(int);

void wdTimeOut(int);

int vmeInit(void)
{
    int rc;

    gnirsG.health = GOOD;
    gnirsG.state = BOOTING;
    if (!gnirsG.initDone) {

        getClockSpeed();        /* Determine clock resolution    */

	/* spawn global tasks and set interrupt vectors, done once */
        lvTaskId = taskSpawn("tLvDump", 25, 0, 3000, lvDump,
                        0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        if (lvTaskId == ERROR)
            gnirsLogMessage(CICS_DB_ERROR,
                           "Unable to spawn lv dump task.");

        scanStatusTaskId = taskSpawn("tCCscan", 25, 0, 3000, scanStatusTask,
                        0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        if (scanStatusTaskId == ERROR) {
            gnirsLogMessage(CICS_DB_ERROR,
                           "Unable to spawn scan status task.");
            gnirsG.health = BAD;
            return VME_ERROR;
        }

        sysIntDisable(GNIRS_MOTOR_INTERRUPT_LEVEL);


        if (intConnect(INUM_TO_IVEC(GNIRS_MOTOR0_INT_NUM),
                       gnirsMotor0IntHndlr, 0) == ERROR) {
            gnirsLogMessage(CICS_DB_ERROR,
                           "Unable to install Motor0 interrupt handler.");
            return VME_ERROR;
        }
        if (intConnect(INUM_TO_IVEC(GNIRS_MOTOR1_INT_NUM),
                       gnirsMotor1IntHndlr, 0) == ERROR) {
            gnirsLogMessage(CICS_DB_ERROR,
                           "Unable to install Motor1 interrupt handler.");
            return VME_ERROR;
        }
        if (intConnect(INUM_TO_IVEC(GNIRS_MOTOR2_INT_NUM),
                       gnirsMotor2IntHndlr, 0) == ERROR) {
            gnirsLogMessage(CICS_DB_ERROR,
                           "Unable to install Motor2 interrupt handler.");
            return VME_ERROR;
        }
        if (intConnect(INUM_TO_IVEC(GNIRS_XYCOM0_INT_NUM),
                       xycomIntHndlr, 0) == ERROR) {
            gnirsLogMessage(CICS_DB_ERROR,
                           "Unable to install XYCOM interrupt handler.");
            return VME_ERROR;
        }


        gnirsLogMessage(CICS_DB_MIN, "Interrupt handlers installed.\n");

        gnirsG.initDone = TRUE;

    } else {
    }

    /*
     * Re-enable GNIRS interrupts
     */

    if (sysIntEnable(GNIRS_MOTOR_INTERRUPT_LEVEL) == ERROR) {
        gnirsLogMessage(CICS_DB_ERROR, "Unable to enable GNIRS_INTERRUPT.");
        return VME_ERROR;
    }

    gnirsG.state = INITIALIZING;

    rc = gnirsInit();
    if (rc != VME_OK) {
        gnirsG.health = BAD;
        return rc;
    }
    gnirsG.health = GOOD;
    gnirsG.state = RUNNING;

    return VME_OK;
}

/* Configuration directory must be set by startup script */
char configDirectory[CONFIG_DIR_LEN];

/* Most of the initialization is done here */
int gnirsInit() {
    motorVars *m;
    int motor, rc;
    char filename[CONFIG_DIR_LEN + 80];

    /* disable all the currently enabled motors */
    for (motor = 0; motor < NUM_MOTORS; motor++) {
        m = motors[motor];
        motors[motor] = 0;
        if (!m)
            continue;
        if(m->semMoveMotor) {
            semDelete(m->semMoveMotor);
            m->semMoveMotor = 0;
        }
        if (m->taskID) {
            taskDelete(m->taskID);
            m->taskID = 0;
        }
    }

    /* Bring up the system. */
    rc = vmeBoardInit();
    if (rc == VME_ERROR) {
        gnirsLogMessage(CICS_DB_ERROR, "\n***\nvmeBoard Init Failed\n***\n");
        return rc;
    }

    rc = testHardware();
    if (rc != VME_OK)
        return rc;

    /* Build software structures */
    rc = mechDescInit();
    if (rc != VME_OK)
        return rc;

    rc = filterDescInit();
    if (rc != VME_OK)
        return rc;

    gnirsG.state = CONFIGURING;

    /* (re-)read configuration files; set up motor/filter/etc configurations */
    strncpy(filename, configDirectory, CONFIG_DIR_LEN);
    strcat(filename, "gnirsConfig");
    rc = readConfig(filename);
    if (rc != VME_OK)
        return rc;

    strncpy(filename, configDirectory, CONFIG_DIR_LEN);
    strcat(filename, "gnirsMechanisms");
    rc = readConfig(filename);
    if (rc != VME_OK)
        return rc;
    rc = finishMechConfig();
    if (rc != VME_OK)
        return rc;

    strncpy(filename, configDirectory, CONFIG_DIR_LEN);
    strcat(filename, "gnirsFilters");
    rc = readConfig(filename);
    if (rc != VME_OK)
        return rc;
    rc = finishFiltersConfig();
    if (rc != VME_OK)
        return rc;

    /* Turn on/off the output ports on the digital IO board.
     * This will select the alternative home switch if so indicated in
     * the configuration file, as well as set reset/enable to their
     * default (disabled) settings.
     */
    rc = initIOBits();
    if (rc != VME_OK)
        return rc;
    /* Finish the configuration of the cryo control.  This is done
     * after initIOBits as those bits only set up the system as the
     * static config file requires and that may not conform to what the
     * external switches indicate.
     */
    finishCryoConfig();

    /* Everything seems ok, so set up tasks/semaphores for active motors */
    rc = initMotors();
    if (rc != VME_OK)
        return rc;

    return VME_OK;
}

int vmeBoardInit()
{
    int rc;

    gnirsInterruptsAllowed = FALSE;

    rc = motorDriverInit();
    if (rc != VME_OK)
        return rc;
    rc = initXycom();
    if (rc != VME_OK)
        return rc;
    rc = initTempBoards();
    if (rc != VME_OK)
        return rc;
    rc = initSenTorr();
    if (rc != VME_OK)
        return rc;
    /*
     * Tell GNIRS controller to give us interrupts after command
     * processing has completed (digital i/o will have been enabled,
     * "software wise" already).
     */
    gnirsLogMessage(CICS_DB_FULL, "Allowing interrupts");
    gnirsInterruptsAllowed = TRUE;

    return rc;
}

int mdMax = 2;  /* for debugging  XXX */

int motorDriverInit() {
    int i, j, status;
    SEM_ID sem;
    RING_ID r;
    GNIRS_ST_MD *md;
    mdRegister *mdr;

    /* set up each motor driver
     *   set addresses;
     *   create semaphores
     *   create ring buffers
     */
    for ( i = 0; (i < NUM_CARDS) && ( i < mdMax) ; i++ ) {
        md = &motorDriver[i];

	/* Elegant simulation possible if this address is changed
         * to correspond to an area which is read/written by a
         * simulation process.
         */
        md->registers = (mdRegister *)
                        (GNIRS_MD_ADDR_BASE + (i * GNIRS_MD_ADDR_INCR));
        mdr = md->registers;

        md->aliveID = motorID;

        /* create the semaphores:
         * Make insertion of message into output queue atomic
         */
        if(md->semOut)
            semDelete(md->semOut);
        sem = semBCreate(SEM_Q_FIFO, SEM_FULL);
        if (sem == NULL) {
            gnirsLogMessage(CICS_DB_ERROR,
                           "Unable to create semOut for md %d.", i);
            return VME_MEMORY_ERROR;
        }
        md->semOut = sem;

        /* response is available */
        if(md->semResponse)
            semDelete(md->semResponse);
        sem = semBCreate(SEM_Q_FIFO, SEM_EMPTY);
        if (sem == NULL) {
            gnirsLogMessage(CICS_DB_ERROR,
                       "Unable to create semResponse for md %d.", i);
            return VME_MEMORY_ERROR;
        }
        md->semResponse = sem;

        for (j = 0; j < NUM_AXES; j++) {
            sem = md->semDone[j];
            if(sem)
                semDelete(sem);
            sem = semBCreate(SEM_Q_FIFO, SEM_EMPTY);
            if (sem == NULL) {
                gnirsLogMessage(CICS_DB_ERROR,
                   "Unable to create semDone semaphore for md %d/%d.", i, j);
                return VME_MEMORY_ERROR;
            }
            md->semDone[j] = sem;
            md->waiting[j] = NULL;  /* no one waiting on this motor */
        }

        /* create the ring buffers */
        if (md->outRing)
            rngDelete(md->outRing);
        r = rngCreate(MD_RING_SIZE);
        if (r == NULL) {
            gnirsLogMessage(CICS_DB_ERROR,
               "Unable to create ring buffer for md %d.", i);
            return VME_MEMORY_ERROR;
        }
        md->outRing = r;

        if (md->sentRing)
            rngDelete(md->sentRing);
        r = rngCreate(SHORT_RING);
        if (r == NULL) {
            gnirsLogMessage(CICS_DB_ERROR,
               "Unable to create sent ring buffer for md %d.", i);
            return VME_MEMORY_ERROR;
        }
        md->sentRing = r;

        if (gnirsG.simulation)
            return VME_OK;

        /* Now verify that we don't have an error in the status register */
        if (mdr->status & MD_INIT) {
            taskDelay(10);  /* how long do we need to wait for board INIT??*/
            if (mdr->status & MD_INIT) {
                gnirsLogMessage(CICS_DB_ERROR,
                        "Controller %d has not passed INIT stage", i);
                return VME_ERROR;
            }
        }

        /* Disable interrupts
         * clear done status
         * set interrupt vector
         * verify no error condition
         */
        mdr->control = 0;
        mdr->data = CONTROL_Y;
        mdr->interruptVector = GNIRS_MOTOR0_INT_NUM + i;
        status = mdr->status;
        if (status & (MD_CMD_S | MD_ENC_S | MD_OVRT)) {
                gnirsLogMessage(CICS_DB_ERROR,
                        "Controller %d initial status fault 0x%x", i,
                        status & (MD_CMD_S | MD_ENC_S | MD_OVRT));
                return VME_ERROR;
        }

        if (status & MD_IBF_S) {
            j = (int) mdr->data;
            gnirsLogMessage(CICS_DB_ERROR,
                    "Unexpected character 0x%x in controller %d", j, i);
            return VME_ERROR;
        }
        if (!(status & MD_TBE_S)) {
            gnirsLogMessage(CICS_DB_ERROR,
                    "Transmit buffer not empty in controller %d", i);
            return VME_ERROR;
        }

        /* set up the message variables; for init'ing we need to set
         * both variables.
         */
        md->messageVars.countInc = -1;
        md->messageVars.crCount = 0;

        /* all appears well, so enable interrupts
         * except for transmitter interrupt which will be set when
         * there's something to send
         */
        mdr->control = (MD_DONE_E | MD_IBF_E | MD_IRQ_E);

    }
    return VME_OK;
}


/* The three interrupt counters are just debugging aids */
int gnirsInt240 = 0;
int gnirsInt241 = 0;
int gnirsInt242 = 0;

void gnirsMotor0IntHndlr(int dummy)
{
    if (gnirsInterruptsAllowed)
        motorIntHandler(0);
    gnirsInt240++;
    return;
}

void gnirsMotor1IntHndlr(int dummy)
{
    if (gnirsInterruptsAllowed)
        motorIntHandler(1);
    gnirsInt241++;
    return;
}

void gnirsMotor2IntHndlr(int dummy)
{
    if (gnirsInterruptsAllowed)
        motorIntHandler(2);
    gnirsInt242++;
    return;
}

int ovrt = 0;
void motorIntHandler(int who) {
    int status, j, n;
    RING_ID r;
    GNIRS_ST_MD *md;
    mdRegister *mdr;
    mdMessage *m;
    register int fromP;
    char c, cc, doneFlags;
    unsigned char control;
    int sendCY = FALSE;

    md = &motorDriver[who];
    mdr = md->registers;
    m = &md->messageVars;
    status = mdr->status;
    doneFlags = mdr->doneFlags;

    if ( status & (MD_CMD_S | MD_ENC_S | MD_OVRT) )
        sendCY = TRUE;
        if (status & MD_OVRT)
            ovrt++;
        if (status & MD_CMD_S) {
            /* Copy the past commands to cmdErr.  This cannot be done
             * by the status task as it could be interrupted while doing
             * its rngBufGet by the code below which may well do its own 'get'.
             * Don't copy output if flag not cleared.
             */
            if (!(md->status & MD_CMD_S)) {
                n = rngBufGet(md->sentRing, md->cmdErr, SHORT_RING);
                md->cmdErr[n] = '\0';
            }
            /* Tell the world.  getMdStatus() resets this */
            md->status |= MD_CMD_S;
        }
    /* Ignore OVRT and ENC_S as the former will be picked up by
     * the status task, and the latter is irrelevant to systems without
     * encoders.
     */

    /* If anyone is done, notify the waiting task */
    for (j = 0; j< NUM_AXES; j++) {
        if (doneFlags & 1) {
            if (md->waiting[j]) {
                md->waiting[j] = NULL;
                semGive(md->semDone[j]);
            }
        }
        doneFlags >>= 1;
    }

    /* Check on transmit buffer */
    c = 0;
    if (status & MD_TBE_S) {
        if (sendCY) {
            /* Send the clear code */
            mdr->data = CONTROL_Y;
            /* Stick '^Y' in sent buffer */
            c = '^';
            r = md->sentRing;
            if (rngIsFull(r))
                RNG_ELEM_GET(r, &cc, fromP);
            RNG_ELEM_PUT(r, c, fromP);
            c = 'Y';
            sendCY = FALSE;
        } else {
            r = md->outRing;
            /* If there's something to send, do so; otherwise, disable
             * the transmit interrupt.
             */
            if ( RNG_ELEM_GET(r, &c, fromP))
                mdr->data = c;
            else {
                control = mdr->control;
                mdr->control = control & ~MD_TBE_E;
            }
        }
    }
    
    /* Store chars for retrieval in case of illegal command */
    if (c) {
        r = md->sentRing;
        if (rngIsFull(r))
            RNG_ELEM_GET(r, &cc, fromP);
        RNG_ELEM_PUT(r, c, fromP);
    }

    /* Anything to fetch ? */
    if (status & MD_IBF_S) {
        /* Messages are either LF CR message LF CR   or
         * LF CR CR message LF CR CR
         * Count LF for debugging.  Count CR up and then
         * down to determine end of message.
         * This is all a little fragile since if anything goes wrong
         * and we get out of sync, there's no easy way back in.
         */
        c = mdr->data;
        switch(c) {
            case LF:
                m->lfCount++;
                /* toggle direction */
                m->countInc = (m->countInc == 1 ? -1 : 1);
                if (m->countInc == 1)
                    m->msgIndex = 0;
                else
                    m->message[m->msgIndex] = '\0';
                break;
            case CR:
                m->crCount += m->countInc;
                if (m->crCount == 0) {
                    /* done with message; tell whoever is waiting */
                    semGive(md->semResponse);
                }
                break;
            default:
                if (m->msgIndex < (MOTOR_MSG_LEN - 1))
                    m->message[m->msgIndex++] = c;
                break;
        }
    }
}

/* Test hardware ... no i/o to change */
int testHardware() {
    int rc;

    int motor;
    motorVars *m;
    int failures;

    failures = 0;
    rc = testOMSCards();
    if (rc != VME_OK)
        failures++;
    else {
        for (motor = 0 ; motor < NUM_MOTORS; motor++) {
            m = motors[motor];
            if (!m)
                continue;
            /* The test is to turn the motor off and verify that no error
             * is reported.  Since the motor *will* be off before this test,
             * this will be a NOP as far as the instrument is concerned.
             */
            rc = motorOff(motor);
            if (rc != VME_OK)
                failures++;
        }
    }
    rc = testXYCOM();
    if (rc != VME_OK)
        failures++;
    rc = testTempBoards();
    if (rc != VME_OK)
        failures++;
    rc = testSenTorr();
    if (rc != VME_OK)
        failures++;
    if (failures) {
        gnirsLogMessage(CICS_DB_ERROR, "%d system%sfailed hardware test",
                failures, (failures > 1 ? "s " : " "));
        return VME_ERROR;
    }
    return VME_OK;
}
