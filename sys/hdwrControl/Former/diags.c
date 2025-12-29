#include <vxWorks.h>
#include <vme.h>
#include <taskLib.h>
#include <msgQLib.h>
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

/* ^U erases line; backspace or delete erase previous char */
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

void getString(char *prompt) {

    char inbuffer[128];
    int exTime, rc;

    do {
	printf("%s: ", prompt);
	fgets(inbuffer, 120, stdin);
	cleanInput(inbuffer);
        rc = sscanf(inbuffer, "%d", &exTime);
    } while (rc != 1);
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

void doWY(int motor) {
    cmdR(motor, "WY");
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
    printf("#### C%d m%d %s %s %s %s %s %d\n", m->controller, motor,
            (stat & MD_DONE) ? "DONE" : "    ",
            (stat & MD_LIMIT)?
                (((stat & MD_DIR) == MD_MINUS) ?
                        "Mlim" : "Plim") : "    ",
            (stat & MD_HOME) ? "HOME" : "    ",
            (stat & MDS_OTRAVEL) ? "OT!" : "   ",
            (stat & MDS_FAULT) ? "FAULT" : "     ",
            m->currPos);
    if (detailed) {
        /* spill guts */
        printf("\nMotor %-12s (%c axis on Controller %d)\nhealth  %-8s ",
                m->name, m->charAxis, m->controller,
                (m->health == GOOD) ? "Good" :
                        ((m->health == BAD) ? "Bad" : "warning"));
        printf("  %s datumed          status %s  %s\n",
                m->datumed ? " is": "NOT",
                statusString(m->status),
                m->aborted ? " ---Aborted---" : "");
        printf("current %-10d requested %-10d last %-10d\n",
                m->currPos, m->reqPos, m->lastPos);
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
        printf("switches:\n");
        printMotorSwitch("home", &m->home);
        printMotorSwitch("positive limit", &m->posLimit);
        printMotorSwitch("negative limit", &m->negLimit);
        printf("backlash: %-8d boostTime: %-8d\n", m->backlash, m->boostTime);
    }
}

void printMotorSwitch(char *name, switchType *s) {
        printf("  %s ", name);
        if (strcmp(name, "home") && (s->type == HOME_SW)) {
            printf("(%s limit)",
                    (s->type == POS_LIM) ? "positive": "negative");
        }
        printf(" at %d\n", s->position);

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

    if (status & MDS_BUSY)
        sprintf(buffer, " BUSY");
    else if (status & MDS_STUCK)
        sprintf(buffer, " stuck");
    else if (status & MDS_STALL)
        sprintf(buffer, " stalled");
    if (status & MD_LIMIT)
        sprintf(buffer, " %s %s limit",
                (status & MDS_OTRAVEL ? "HARD" : "soft"),
                (status & MD_MINUS ? "neg" : "pos"));
    return buffer;
}

void mstat() {
    int i;

    for (i = 0; i < NUM_MOTORS; i++)
        if (motors[i])
            mstatus(i, 0);
}


#define CREEP_D 10
/* move very slowly into a switch to determine if motor steps have been
 * lost.
 */
int creep(int motor, int sw) {
    int rc;
    motorVars *m;
    int test;
    int where, dist;
    
    m = motors[motor];
    if ((motor >= NUM_MOTORS) || !m) {
        sprintf(m->errorMsg, "status req: motor %d not allocated", motor);
        gnirsLogMessage(CICS_DB_ERROR, m->errorMsg);
        return VME_ILLEGAL_MOTOR;
    }
    if (gnirsG.simulation)
        m->status = 0;
    else {
        m->status &= ~MDS_STAT_MASK;
    }
    /* Find where to go and stop 10 steps short */
    if ((sw == HOME_SW) && (m->home.type != HOME_SW))
        sw = m->home.type;
    dist = CREEP_D * 2;
    switch (sw) {
        case HOME_SW:
            where = m->home.position;
            where -= CREEP_D;
            test = MD_HOME;
            break;
        case POS_LIM:
            where = m->posLimit.position;
            where -= CREEP_D;
            test = MD_LIMIT;
            break;
        case NEG_LIM:
            /* have to allow enough room for backlash without hitting limit */
            /* Unsure how to make this work reliably XXX
            where = m->negLimit.position + 2 * m->backlash;
            dist = -dist;
            test = MD_LIMIT;
            break;
            */
        default:
            sprintf(m->errorMsg, "Inappropriate switch %d for creep", sw);
            gnirsLogMessage(CICS_DB_ERROR4, m->errorMsg);
            return VME_ERROR;
    }
    
    m->health = GOOD;
    m->lastPos = m->currPos;
    rc = position(motor, where);
    if (rc != VME_OK) {
        if (rc == VME_TIMEOUT) {
            m->health = WARNING;
            /* CHECK FOR OVERTRAVEL XXX */
            m->status |= MDS_STALL;
        } else
            m->health = BAD;
    } else {
        rc = motorCreep(motor, dist, test);
        if ((rc != VME_OK) && (rc != VME_ABORTED)) {
            sprintf(m->errorMsg,
                    "in %d steps, motorCreep failed to find switch", dist);
            gnirsLogMessage(CICS_DB_ERROR, m->errorMsg);
        }
    }

    if (gnirsG.simulation) {
        m->status |= (MD_DONE | MD_HOME);
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
    } else {
        m->home.useAlt = FALSE;
        m->home.position = 0;
        clearBit(m->home.control);
    }
}

/* Test home switch offset; requires both switches to be functional
 * and leaves the primary one in use.
 */
void homeOffset(int motor) {
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
    rc = creep(motor, HOME_SW);
    alt = m->currPos;
    if (m->home.offset >= 0)
        useAlt(motor, TRUE);
    else
        useAlt(motor, FALSE);
    if (rc == VME_OK) {
        rc = creep(motor, HOME_SW);
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

int moveAll() {
    motorVars *m;
    focusType pos;
    int i;

    m = motors[FOCUS];
    /* reqOp won't be NULL if valid had been called for the FOCUS mechanism */
    if (m->reqOp == NULL) {
        for (pos = 0, i = 0; i < NUM_MECH; i++) {
            pos += mechanism[i].focusShift();
        }
        m->reqOp = position;
        m->reqArg = (int) pos;
    }
    return doAll();
}

int doAll() {
    int i;
    motorVars *m;

    for (i = 0; i < NUM_MOTORS; i++ ) {
        m = motors[i];
        if (!m)
            continue;
        /* Request the task (m->taskID) to make the move (see motorTask()
         * in gnirsMotors.c .  The requested operation and argument
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
    printf("Pressures:  IG: %10.3e      TC1: ", SenTorrIg);
    if (SenTorrTc1 < 0)
        printf("Err %02dE        TC2: ", -1 * (int)SenTorrTc1);
    else
        printf("%10.3e     TC2: ", SenTorrTc1);
    if (SenTorrTc2 < 0)
        printf("Err %02dE\n", -1 * (int)SenTorrTc2);
    else
        printf("%10.3e\n", SenTorrTc2);
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
                (stat & MD_LIMIT) ?
                    (((stat & MD_DIR) == MD_MINUS) ?
                                'M' : 'P') : '-',
                (stat & MD_HOME) ? 'H' : '-');
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
 * Grating ID *
 * *******************/

/* Set the grating id (instead of through the 'valid' command */
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

/* *******************
 * Temp/OMS conflict *
 * *******************/

#define NUMREADS 50
void readMdr(int n, int which) {
    mdRegister *mdr;
    GNIRS_ST_MD *md;
    int i,k;
    int data[NUMREADS];

    md = &motorDriver[which];

    mdr = md->registers;
/*
    for (i = 0; i < n; i++ ) {
        printf("status: 0x%x\n", mdr->status);
    }
    mdr->data = CONTROL_Y;
    for (i = 0; i < n; i++ ) {
        printf("status: 0x%x\n", mdr->status);
    }
*/
    mdr->control = 0;
    mdr->data = CONTROL_Y;
    mdr->interruptVector = GNIRS_MOTOR0_INT_NUM;
    for (i = 0; i < NUMREADS; i++) {
        data[i] = mdr->status;
    }
    for (i = 0; i < (NUMREADS/10); i++) {
        printf("%d:\t", i*10);
        for (k = 10 * i; k < (10 * (i+1)); k++) {
            printf("0x%x  ", data[k]);
        }
        printf("\n");
    }
}
void readMdr2(int n, int which) {
    mdRegister *mdr;
    GNIRS_ST_MD *md;
    int i,k;
    int data[NUMREADS];

    md = &motorDriver[which];

    mdr = md->registers;

    /* Now try to get the interrupt vector back in there */
    mdr->interruptVector = GNIRS_MOTOR0_INT_NUM;
    for (i = 0; i < NUMREADS; i++) {
        data[i] = mdr->status;
    }
    for (i = 0; i < (NUMREADS/10); i++) {
        printf("%d:\t", i*10);
        for (k = 10 * i; k < (10 * (i+1)); k++) {
            printf("0x%x  ", data[k]);
        }
        printf("\n");
    }
}
