static struct
  {
      void *v;
      char *c;
  }
sccsid =
{
    &sccsid,
        "@(#)diags.c	1.6 07/30/03"
};
#include <vxWorks.h>
#include <vme.h>
#include <taskLib.h>
/* #include <msgQLib.h> */
#include <stat.h>
#include <nfsDrv.h>
#include <sysLib.h>
#include <sockLib.h>

#include "gnirsCC.h"
#include "sockutil.h"
#include "math.h"

#ifdef DPRINT
#undef DPRINT
#endif

#define DPRINT(a) (fputs (a,stderr), fflush(stderr))

void mstatus(int, int);
void printMotorSwitch(char *, switchType *);
char * statusString(int);
void printPortBits(int, unsigned char);

/*
 * Set of simple commands to test GNIRS
 */

/* *******************
 * Input *
 * *******************/

void cleanInput(char *in)
{
    char *out,
        *p;

    for (p = in, out = in; *p; p++) {
        if (*p == 0x15) {       /* ^U */
            out = in;
            continue;
        }
        if ((*p == '\b') || (*p == 0177)) {
            out--;
            if (out < in)
                out = in;
            continue;
        }
        if (*p == '\n')
            break;
        *out++ = *p;
    }
    *out = '\0';
}

/* *******************
 * Memory *
 * *******************/

/* set a swath of the data buffer */
void setMem(int count, uint32 * pmem, uint32 value)
{
    int i;

    for (i = 0; i < count; i++)
        *pmem++ = value;
}

/* *******************
 * Motor Control *
 * *******************/

void cmdR(int motor, char *cmd) {
    char buffer[120];
    char cString[64];
    int controller;
    motorVars *m;

    m = motors[motor];
    controller = m->controller;

    strncpy(cString, cmd, 62);
    strcat(cString, " ");

    (void) sendToMotorDriver(controller, cString, buffer);
    printf("M%d: Sent <%s > Got <%s>\n", motor, cmd, buffer);
}

void doC_X(int motor) {
    motorVars *m;

    m = motors[motor];
    sendToMotorDriver(m->controller, "\0x18", NULL);
}

void cmd(int motor) {
    char inbuffer[128], previous[128];
    char finalBuffer[132];
    int done = FALSE;
    int special;
    motorVars *m;

    m = motors[motor];

    previous[0] = 0;
    while (!done) {
        printf(">M%d> ", motor);
        fgets(inbuffer, 120, stdin);
        cleanInput(inbuffer);
        special = 0;
        switch (inbuffer[0]) {
            case '\0':
                continue;
                break;
            case 0x1B:  /* escape */
                done = TRUE;
                continue;
                break;
            case 0x12:  /* control-R */
            case '"':   /* (ditto) Labview wants ^R for its own */
                strcpy (inbuffer, previous);
                printf("%s\n", inbuffer);
                break;
            case 0x17:  /* control-W */
            case '`':   /* Labview wants ^W as well*/
                mstatus(motor, 0);
                continue;
                break;
            case 'W':
                if (inbuffer[1] != 'Y')
                    break;
                /* Fall Through */
            case 'R':
            case 'Q':
                /* These require special handling as they generate a reply.
                 * Only allow one per line
                 */
                inbuffer[2] = '\0';
                special++;
                break;
        }
        strcpy(previous, inbuffer);
        sprintf(finalBuffer, "A%c ", m->charAxis);
        strcat(finalBuffer, inbuffer);
        /* Can accidentally generate stuff like vl1000mr100 if issue
         * the vl1000 command and then at the next prompt 'mr100'.  Add
         * trailing ' ' after each command to avoid this.
         */
        strcat(finalBuffer, " ");
        if (special)
            cmdR(motor, finalBuffer);
        else
            sendToMotorDriver (m->controller, finalBuffer, NULL);
    }
}

void clearErr(int motor) {
    motorVars *m;

    m = motors[motor];
    m->returnValue = VME_OK;
    m->errorMsg[0] = '\0';
}

void printIOBits(motorVars *);

void mstatus (int motor, int detailed) {
    motorVars *m;
    int stat;

    m = motors[motor];
    if(!m) {
        printf("Motor %d not yet initialized\n", motor);
        return;
    }
    getMdStatus(motor);
    stat = m->status;

    /* Print status for debugging */
    (void) tellPV(motor);
    printf("#### %14s C%d m%d %s %s %s %s %s %d\n",m->name, m->controller, motor,
            (stat & MS_BUSY) ? "Busy": "    ",
            (stat & MH_LIMIT)?
                (((stat & MH_DIR) == MH_MINUS) ?
                        "Mlim" : "Plim") : "    ",
            (stat & MH_HOME) ? "HOME" : "    ",
            (stat & MS_OTRAVEL) ? "OT!" : "   ",
            (stat & MS_FAULT) ? "FAULT" : "     ",
            m->currPos);
    if (detailed) {
        /* spill guts */
        printf("Motor %-12s (%c axis on Controller %d)\nhealth  %-8s ",
                m->name, m->charAxis, m->controller,
                (m->health == GOOD) ? "Good" :
                        ((m->health == BAD) ? "Bad" : "warning"));
        printf("  %s datumed          status %s  %s\n",
                m->datumed ? " is": "NOT",
                statusString(m->status),
                m->aborted ? " ---Aborted---" : "");
        printf("Last Op: (%d) %s\n", m->returnValue, m->errorMsg);
        printf("current %-10d\n",
                m->currPos);
        printf("%s with full travel %d\n",
                m->type == LINEAR ? "Linear" :
                    (m->type == BINARY ? "Binary" : "Rotary"), m->fullTravel);
        printf("motions:   accel   velocity\n");
        printf("  seek:    %-8d %-8d\n  backOff: %-8d %-8d\n",
                m->seek.acceleration, m->seek.velocity,
                m->backOff.acceleration, m->backOff.velocity);
        printf("  probe:   %-8d %-8d\n  probeH:  %-8d %-8d\n",
                m->probe.acceleration, m->probe.velocity,
                m->probeH.acceleration, m->probeH.velocity);
        printf("backlash: %-8d\n", m->backlash);
        printf("switches:\n");
        printMotorSwitch("home", &m->home);
        if (m->type != ROTARY) {
            printMotorSwitch("positive limit", &m->posLimit);
            printMotorSwitch("negative limit", &m->negLimit);
        }
        printIOBits(m);
    }
}
void printmotormech()
{
	int i;
	printf("%22s\tmech #\tmotor #\n","name");
	for (i=0;i<NUM_MECH;i++)
		printf ("%22s\t  %d\t  %d\n",mechanism[i].name,mechanism[i].motor,i);
}
void printMotorSwitch(char *name, switchType *s) {
        printf("  %s ", name);
        if (strcmp(name, "home") && (s->type == HOME_SW)) {
            printf("(%s limit)",
                    (s->type == POS_LIM) ? "positive": "negative");
        }
        printf(" at %8d, offset %5d\n", s->position, s->offset);

        if (!strncmp(name, "home", 4)) {
            printf("      auxiliary at %-8d on port %d/%d -- %s\n",
            s->offset, s->control.port, s->control.bit,
            s->useAlt ? "in use" : "not in use");
        }
}

char * statusString(int status) {
    static char buffer[24];

    /* default message set up first */
    sprintf(buffer, " OK      ");

    if (status & MS_BUSY)
        sprintf(buffer, " busy");
    else if (status & MS_FAULT)
        sprintf(buffer, " fault");
    else if (status & MS_STUCK)
        sprintf(buffer, " stuck");
    else if (status & MS_STALL)
        sprintf(buffer, " stalled");
    if (status & MH_LIMIT)
        sprintf(buffer, " %s %s limit",
                (status & MS_OTRAVEL ? "HARD" : "soft"),
                (status & MH_MINUS ? "neg" : "pos"));
    return buffer;
}

void printIOBits(motorVars * m) {
    printf("IO Bits: ");
    if (isSet(m->enable))
        printf(" isEnabled %s", isSet(m->isEnabled) ? "yes " : "no  ");
    else
        printf(" enable bit off");
    printf("  fault? %s\n", isClear(m->fault) ? "YES" : "no ");
}

void mstat(int flag) {
    int i, printed;

    printed = 0;
    for (i = 0; i < NUM_MOTORS; i++)
        if (motors[i]) {
            if (flag && printed++)
                printf("\n");
            mstatus(i, flag);
        }
}


#define CREEP_D 10

int creep(int motor, int sw, int d) {
    int rc;
    motorVars *m;
    int test;
    int where, dist;
    
    m = motors[motor];
    if ((motor >= NUM_MOTORS) || !m) {
        mErrMsg(m, "status req: motor %d not allocated", motor);
        return VME_ILLEGAL_MOTOR;
    }
    if (gnirsG.simulation)
        m->status = 0;
    else {
        m->status &= ~MS_STAT_MASK;
    }
    /* Find where to go and stop dist (d or CREEP_D) steps short */
    if ((sw == HOME_SW) && (m->home.type != HOME_SW))
        sw = m->home.type;
    if (d == 0)
        dist = CREEP_D;
    else
        dist = d;

    switch (sw) {
        case HOME_SW:
		  where = m->home.position;
		  if(m->backlash < 0)
			  where += dist;
		  else
			  where -= dist;
            test = MH_HOME;
            break;
        case POS_LIM:
            where = m->posLimit.position;
            where -= dist;
            test = MH_LIMIT;
            break;
        case NEG_LIM:
/* 		  if(m->backlash < 0) */
            /* have to allow enough room for backlash without hitting limit */
            /* Unsure how to make this work reliably XXX
            where = m->negLimit.position + 2 * m->backlash;
            dist = -dist;
            test = MH_LIMIT;
            break;
            */
        default:
            sprintf(m->errorMsg, "Inappropriate switch %d for creep", sw);
            gnirsLogMessage(CICS_DB_ERROR4, m->errorMsg);
            return VME_ERROR;
    }

    dist *= 2;  /* want to go dist before and after presumed sw position */
    if(m->backlash < 0)
		where = -where;
    rc = position(motor, where);
    if (rc != VME_OK) {
/* 		printf("error\n"); */
        if (rc == VME_TIMEOUT) {
            m->health = WARNING;
            /* CHECK FOR OVERTRAVEL XXX */
            m->status |= MS_STALL;
        } else
            m->health = BAD;
    } else {
		if(m->backlash < 0)
		{
/* 			printf("call motorcreep -  \n"); */
			rc = motorCreep(motor, -dist, test);
		}
		else
		{
/* 			printf("call motorcreep\n"); */
			rc = motorCreep(motor, dist, test);
		}
        if ((rc != VME_OK) && (rc != VME_ABORTED)) {
            mErrMsg(m, "in %d steps, motorCreep failed to find switch", dist);
        }
    }
    (void) tellPV(motor);

    if (gnirsG.simulation) {
        m->status |= (MH_DONE | MH_HOME);
    }
    return rc;
}

void useAlt(int motor, BOOL flag) {
    motorVars *m;

    m = motors[motor];
    if(!m)
        return;
    if (flag) {
        m->home.useAlt = TRUE;
        m->home.position = m->home.offset;
        setBit(m->home.control);
		printf("Alt home in use %d %d %d\n\n",motor,m->home.control.port,m->home.control.bit);
    } else {
        m->home.useAlt = FALSE;
        m->home.position = 0;
        clearBit(m->home.control);
    }
}

void homeOffset(int motor, int dist) {
    motorVars *m;
    int alt, primary, tmp;
    int rc;

    m = motors[motor];
    if (!m) {
        printf("Motor not initialized\n");
        return;
    }
    if (!m->datumed) {
        printf("Datum the motor with either home switch\n");
        return;
    }

    /* if alternative position is in minus direction, creep there first */
    if (m->home.offset < 0)
        useAlt(motor, TRUE);
    else
        useAlt(motor, FALSE);
    rc = creep(motor, HOME_SW, dist);
    alt = m->currPos;
    if (m->home.offset >= 0)
        useAlt(motor, TRUE);
    else
        useAlt(motor, FALSE);
    if (rc == VME_OK) {
        rc = creep(motor, HOME_SW, dist);
        primary = m->currPos;
        if (m->home.offset >= 0) {
            tmp = alt;
            alt = primary;
            primary = tmp;
        }
    }
    if (rc == VME_OK)
        printf("Primary: %d, alternate: %d;\talt offset  = %d\n",
                primary, alt, alt - primary);
    useAlt(motor,FALSE);
}

void unDatum(int motor) {
    
    motors[motor]->datumed = FALSE;
}

int parkAll() {
    int i;

    for (i = 0; i < NUM_MECH; i++ ) {
        (void)parkMech(i);
    }
    return doAll();
}

int datumAll() {
    int i;

    for (i = 0; i < NUM_MECH; i++ ) {
        (void)datumMech(i);
    }
    return doAll();
}

int moveAll(int flag) {
    motorVars *m;
    focusType pos;
    int i;

	/* Ignore focus correction unless flag set */
	if (flag) {
		m = motors[FOCUS];
		/* reqOp won't be NULL if valid had been called for the FOCUS mechanism */
		if (m->reqOp == NULL) {
			for (pos = 0, i = 0; i < NUM_MECH; i++) {
				pos += mechanism[i].fShift();
			}
			m->reqOp = position;
			m->reqArg = (int) pos;
		}
	}
    return doAll();
}

int doAll() {
    int i;
    motorVars *m;

    for (i = 0; i < NUM_MOTORS; i++ ) {
        m = motors[i];
        if (!m || m->reqOp == NULL)
            continue;
        /* Request the task (m->taskID) to make the move (see motorTask()
         * in gnirsMotors.c).  The requested operation and argument
         * have been set elsewhere.
         */
        semGive(m->semMoveMotor);
    }
    return VME_OK;
}

/* *******************
 * Temperature *
 * *******************/

void printTempVolts() {
    short i, j, k;
    double mydewdat[NUM_TEMPS];

    for (i = 0; i < NUM_TEMPS; i++)
      mydewdat[i] = rddewVolt(i);

    printf("The %d sensor voltages are:\n", NUM_TEMPS);
    i = 0; j = 8;
    for (k = 0; k < 4 * NUM_T_BOARDS ; k++, j+= 8 ){

        printf("ch %2d-%2d:   ", i, j-1);
        for (; i < j; i++) printf("%6.3f ", mydewdat[i]);
        printf("\n");
    }
}

void printTemp(unsigned char ch) {
    tempDescriptor *tp;

    if (ch >= NUM_TEMPS) {
        printf ("channel number out of range\n");
        return;
    }
    tp = (tempDescriptor *)&temperatures[ch];
    printf("Temperature probe %d %s\n", ch, tp->name);
    printf("  offset:%10.2f\n", tp->offset);
    printf("  Volts: %10.2f\n", rddewVolt(ch));
    printf("  Deg K: %10.2f\n", rddewDegK(ch));
}

/* Multiple reads of one reference channel and one user specified channel
 * in order to check consistency.
 */
void rdTemp(unsigned char ch, int n) {
    do {
        printf("30: %10.2f  %10.2f %10.2f\n",
                rddewVolt(30), rddewVolt(30), rddewVolt(30));
        printf("%02d: %10.2f  %10.2f %10.2f\n", ch,
                rddewVolt(ch), rddewVolt(ch), rddewVolt(ch));
    } while (n-- > 0);
}

/* *******************
 * Pressure *
 * *******************/


void cmdSentorr(char *cmd, int len) {
    char response [2 * SENTORR_LEN];
    int rc;

    rc = sendSenTorr(cmd, response, len);
    if (rc == VME_OK) {
        if (strlen(response) == 0)
            printf("  Response: <No message>\n");
        else
            printf("  Response: %s\n", response);
    }
}

void printPressure() {
    int rc;
    
    rc = readSenTorr();
    if (rc != VME_OK) {
        printf("Error on reading SenTorr\n");
        return;
    }
    printf("Pressures:  IG: %10.3e      TC1: ", senTorrIg);
    if (senTorrTc1 < 0)
        printf("Err %02dE        TC2: ", -1 * (int)senTorrTc1);
    else
        printf("%10.3e     TC2: ", senTorrTc1);
    if (senTorrTc2 < 0)
        printf("Err %02dE\n", -1 * (int)senTorrTc2);
    else
        printf("%10.3e\n", senTorrTc2);
    /* If that worked, then lets try for the Ccg reading */
    rc = readCCG();
    if (rc != VME_OK)
        return;
    printf("           CCG: %10.3e\n", senTorrCcg);
}

/* *******************
 * Labview *
 * *******************/

int useLv(int port, char *host) {
    int rc;

    /* testing for non-zero port would be ok too; 1024 is usually the
     * boundary for privledged ports.
     */
    gnirsG.guiMode = LABVIEW;

    if (gnirsG.svFd) {
        lvFileSever();
        sockClose(&gnirsG.svFd);
        gnirsG.svFd = NULL;
    }
    if (port > 1024) {
        printf("Start 'labview' on %s, then press 'return' ", host);
        while (getchar() != '\n');
        gnirsG.lvFileType = LVSERVER;
        rc = sockConnect(port, host);
        if (rc != ERROR) {
            gnirsG.svFd = rc;
            {
            /*
            int size = 32768;
            setsockopt(gnirsG.svFd, SOL_SOCKET, SO_SNDBUF,
                       (char *) &size, sizeof(size));
             */
            }
            rc = VME_OK;
        } else
            rc = VME_ERROR;
    } else {
        gnirsG.lvFileType = LVNFS;
        rc = VME_OK;
    }
    return rc;
}

void lvMotorDump() {
    int motor, stat;
    char buffer[LV_MAX_MSG_LEN];
    motorVars *m;


    for (motor = 0; motor < NUM_MOTORS; motor++) {
        if (m = motors[motor]) {
            getMdStatus(motor);
            stat = m->status;

            (void) tellPV(motor);
            sprintf(buffer, "M%02d %8d%c%c\n", motor, m->currPos,
                (stat & MH_LIMIT) ?
                    (((stat & MH_DIR) == MH_MINUS) ?
                                'M' : 'P') : '-',
                (stat & MH_HOME) ? 'H' : '-');
            lvFileString(buffer);
        }
    }
    return;
}

void lvFakeTemp() {
    char buffer[LV_MAX_MSG_LEN];
    double t, r;

    r = rand()/(double) RAND_MAX;
    t = -130. + r;

    sprintf(buffer, "T01%7.2f", t);
    lvFileString(buffer);

    t = -130. + sin((double)time((time_t *)NULL)/10.);
    sprintf(buffer, "T02%7.2f", t);
    lvFileString(buffer);
}

/* *******************
 * Digital I/O *
 * *******************/

/* Routines to force a bit high or low for testing */
void bitHigh(int port, int bit) {
    ioBit iob;

    iob.port = port;
    iob.bit = bit;
    (void) setBit(iob);
}

void bitLow(int port, int bit) {
    ioBit iob;

    iob.port = port;
    iob.bit = bit;
    (void) clearBit(iob);
}

/*command line interface to test bo driver */

int xycomWrite(int port, unsigned char val)
 {
    return xycomBoWrite(port, 0xff, val);
 }

void xycomReport() {
    int i;
    char ports;
    unsigned char val;

    printf("XY240 Binary IN Channels:\n");
    ports = ~xycomOutputs;
    for ( i = 0; i < XYCOM_NPORTS ; i++ ) {
        if (ports & 1) {
            xycomBiRead(i,0xFF,&val);
            printPortBits(i, val);
        }
        ports >>= 1;
    }

    printf("XY240 Binary OUT Channels:\n");
    ports = xycomOutputs;
    for ( i = 0; i < XYCOM_NPORTS ; i++ ) {
        if (ports & 1) {
            xycomBoRead(i,0xFF,&val);
            printPortBits(i, val);
        }
        ports >>= 1;
    }
}

void printPortBits(int port, unsigned char val) {
    int i;
    char bits[24];
    char *p;

    p = bits;
    for (i = 0; i < 8; i++ ) {
        *p++ = ' ';
        if (val & (1 << (7-i)))
            *p++ = '1';
        else
            *p++ = '0';
    }
    *p++ = '\0';

    if (port == FLAG_PORT)
        printf("     Flags:%s\n", bits);
    else
        printf("    Port %d:%s\n", port, bits);

}

/* *******************
 * Grating ID and information*
 * *******************/

int setGrating(int grating) {

    mechDescriptor *pMech;
    
    if ((grating < 0) || (grating >= NUM_GRATINGS) ) {
        gnirsLogMessage(CICS_DB_ERROR4, "Illegal grating number %d", grating);
        return VME_ERROR;
    }
    
    pMech = &mechanism[GRATING];
    pMech->reqStepPosition = grating;
    strcpy(pMech->reqPosition, "specified by number");
    return VME_OK;
}

void printGratingInfo() {
    mechDescriptor *pMech;
    int grating;
    gdata *gdp;

    pMech = &mechanism[GRATING];
    if (pMech->motor == -1) {
        printf("Grating mechanism not configured\n");
        return;
    }
    grating = pMech->reqStepPosition;
    if ((grating < 0) || (grating >= NUM_GRATINGS)) {
        printf("Invalid Grating number\n");
        return;
    }
    gdp = &gratingData[pMech->reqStepPosition];

    printf("Grating #%d: offset %d, constant %f\n", grating + 1,
            gdp->zeroPt, gdp->A);
    printf("  Step %7d  wavelength %5.2f  order %2d   angle %f\n",
            gratingStep, gratingWavelength, gratingOrder, gratingAngle);
}

/* *******************
 * System *
 * *******************/

void resetHealth() {
    
    gnirsG.health = GOOD;
}

void sysinfo(int which) 
{
    int allSections;
    int wanted, item;
    int cutoff, i, section;

    allSections = 0;
    wanted = 0;
    cutoff = -1;
    if (which < 0) {
        printf ("selector can't be negative\n");
        return;
    }
    if (which == 0)
        allSections = 1;
    if (which / 100) {
        wanted = which / 100;
        item = which % 100;
    } else if (which / 10) {
        wanted = which / 10;
        item = which % 10;
    } else {
        wanted = which;  /* single digit */
        item = -1;
    }
    if (wanted > 9) {
        printf("wanted can't be greater than 9\n");
        return;
    }
    if (item == 99) {
        cutoff = wanted;
        allSections = 1;
        item = -1;
    }
    
    for (section = 1; section < 10; section++) {
        if (allSections || (section == wanted)) {
            printf("\n(%d):\n", section);
            switch (section) {
                case 1:
                    printPressure();
                    break;
                case 2:
                    printTempVolts();
                    break;
                case 3:
                    if (item != -1)
                        mstatus(item, 1);
                    else {
                        mstat(1);
                    }
                    break;
                case 4:
                    if (item != -1)
                        dumpMech(item);
                    else {
                        for (i = 0; i < FILTER; i++) {
                            dumpMech(i);
                        }
                    }
                    break;
                case 5:
                    dumpFilters();
                    break;
                case 6:
                    if (gnirsG.cryoSelect == COMPUTER) {
                      printf("cryoheads under computer control, commanded %s\n",
                                gnirsG.cryoCpuState ? "on" : "off");
                    } else {
                        printf("cryoheads under manual mode, switched %s\n",
                           isSet(cryoSwitches[ON_OFF_SWITCH]) ? "on" : "off");
                    }
                    break;
                case 7:
                    printGratingInfo();
                    break;
                case 8:
                    xycomReport();
                    break;
                default:
                    break;
            }
        }
        if (section == cutoff)
            break;
    }
}

/* special test of home position */
int hrtest(int motor) {
    motorVars *m;
    char cmd[MOTOR_CMD_LEN];
    int timeout;
    int rc, rc2;

    m = motors[motor];
    if ((motor >= NUM_MOTORS) || !m) {
        mErrMsg(m, "status req: motor %d not allocated", motor);
        return VME_ILLEGAL_MOTOR;
    }

    rc = motorOn(motor);
    if (rc != VME_OK) {
        m->health = BAD;
        return rc;
    }

    sprintf(cmd, "A%c AC%d VL%d HH HR0", m->charAxis,
            m->seek.acceleration, m->seek.velocity);
    timeout = motorTimeout(m->fullTravel, &m->seek);
    rc = waitFor(motor, cmd, timeout);
    if (rc != VME_OK) {
        mErrMsg(m, "Failed to seek to home");
    }
    if (rc == VME_OK) {
        sprintf(cmd, "A%c HL", m->charAxis);
        (void) noWait(motor, cmd);
        getMdStatus(motor);
        if (m->status & MH_HOME) {
            mErrMsg(m, "Can't get off home switch");
            rc = VME_ERROR;
        }
    }
    if (rc != VME_OK) {
        xycomReport();
        mstat(0);
    }

    rc2 = motorOff(motor);
    if (rc == VME_OK)
        return rc2;
    else
        return rc;
}

int dohrtest(int motor) {
    motorVars *m;

    m = motors[motor];
    if ((motor >= NUM_MOTORS) || !m) {
        mErrMsg(m, "status req: motor %d not allocated", motor);
        return VME_ILLEGAL_MOTOR;
    }
    m->reqOp = hrtest;
    return VME_OK;
}
