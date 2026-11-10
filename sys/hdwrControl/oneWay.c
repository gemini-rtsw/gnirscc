/* GNFR-75641: one direction movement for the camera turret.
 *
 * Enabled per motor by "oneWay = 1" (positive) or "oneWay = -1" (negative)
 * in gnirsConfig; with oneWay = 0 the normal goToPos()/setHome() code runs.
 *
 * The motor only ever turns in the configured direction.  Each time a move
 * crosses one of the two home switch edges, it slows down oneWayWindow steps
 * before the expected edge and creeps onto it with KM/KR, which stops on the
 * edge without touching the position counter (unlike HM/HR).  The counter
 * minus the expected count is the number of steps lost since the last edge;
 * it is published to cameraOneWayErr and the counter is then reloaded.
 *
 * Edge 0 is the datum edge (home.position), edge 1 the far edge
 * (oneWayFarEdge, skipped while 0).  Going positive the datum edge is seen
 * at homeLevel, going negative at the other level; the far edge is the
 * opposite of the datum edge.
 *
 * "oneWayTravel = 1" or "-1" is a simpler mode (Brian's suggestion): the
 * normal datum and no edge stops, but moves only travel in that direction
 * at seek speed and always end with the normal slow positive approach over
 * the last 'backlash' steps, so positions match the gnirsMechanisms tuning.
 * Going negative that means overshooting the target by 'backlash' and
 * coming back.  oneWay, when set, takes precedence.
 *
 * "oneWayCorrect = 1" adds the edge stops of the oneWay mode to oneWayTravel
 * moves: every home edge crossed on the way is caught and the counter is
 * reloaded.  The reload values must be in the regular datum's frame, which
 * sets home.position where the datum edge trips going positive; caught in
 * the travel direction the edge trips elsewhere (switch hysteresis), so its
 * count is configured as oneWayDatumEdge (0 = home.position).
 */

#include <vxWorks.h>
#include <stdlib.h>
#include <stdio.h>
#include "gnirsCC.h"
#include "status.h"

int sendStatus(long index, long type, void *p);

static char otherLevel(char c) {
    return (c == 'L') ? 'H' : 'L';
}

/* travel direction of whichever one way mode is on */
static int dirOf(motorVars *m) {
    if (m->oneWay)
        return m->oneWay;
    return (m->oneWayTravel > 0) ? 1 : -1;
}

/* distance from 'from' to 'to' going in the travel direction, 0 .. full-1 */
static int ahead(motorVars *m, int from, int to) {
    int n = (dirOf(m) * (to - from)) % m->fullTravel;
    return (n < 0) ? n + m->fullTravel : n;
}

static int edgeCount(motorVars *m, int edge) {
    if (edge)
        return m->oneWayFarEdge;
    if (!m->oneWay && m->oneWayDatumEdge)
        return m->oneWayDatumEdge;
    return m->home.position;
}

/* steps to the next time 'edge' comes round; sitting on it means a turn */
static int toEdge(motorVars *m, int edge) {
    int d = ahead(m, m->currPos, edgeCount(m, edge));
    return d ? d : m->fullTravel;
}

/* distance from 'from' to 'to' going in direction d, 0 .. full-1 */
static int aheadDir(motorVars *m, int d, int from, int to) {
    int n = (d * (to - from)) % m->fullTravel;
    return (n < 0) ? n + m->fullTravel : n;
}

/* Relative move of 'steps' (>= 0) in direction d */
static int stepDir(int motor, int d, int steps, motion *mo) {
    motorVars *m = motors[motor];
    char cmd[MOTOR_CMD_LEN];
    int rc;

    if (steps <= 0)
        return VME_OK;
    sprintf(cmd, "A%c AC%d VL%d MR%d GO", m->charAxis,
            mo->acceleration, mo->velocity, d * steps);
    rc = waitFor(motor, cmd, motorTimeout(steps, mo));
    if (rc == VME_OK)
        rc = tellPV(motor);
    return rc;
}

/* Relative move of 'steps' (>= 0) in the travel direction */
static int oneWayStep(int motor, int steps, motion *mo) {
    return stepDir(motor, dirOf(motors[motor]), steps, mo);
}

/* Keep the counter within half a turn, as motorPos() does */
static int halfTurn(int motor) {
    motorVars *m = motors[motor];
    char cmd[MOTOR_CMD_LEN];
    int rc = VME_OK;

    if (abs(m->currPos) > m->fullTravel / 2) {
        sprintf(cmd, "A%c LP%d", m->charAxis,
                m->currPos - ((m->currPos > 0) ? m->fullTravel : -m->fullTravel));
        if ((rc = noWait(motor, cmd)) == VME_OK)
            rc = tellPV(motor);
    }
    return rc;
}

/* Seek to the window before 'edge', creep onto it, report the error and
 * reload the counter.
 */
static int oneWayEdge(int motor, int edge, int *err) {
    motorVars *m = motors[motor];
    char cmd[MOTOR_CMD_LEN];
    char level;
    int rc, expected, half;
    long l;

    expected = edgeCount(m, edge);
    rc = oneWayStep(motor, toEdge(m, edge) - m->oneWayWindow, &m->oneWaySeek);
    if (rc != VME_OK)
        return rc;

    level = (dirOf(m) > 0) ? m->homeLevel : otherLevel(m->homeLevel);
    if (edge)
        level = otherLevel(level);
    sprintf(cmd, "A%c AC%d VL%d H%c K%c", m->charAxis,
            m->oneWayCatch.acceleration, m->oneWayCatch.velocity, level,
            (dirOf(m) > 0) ? 'M' : 'R');
    /* Lost steps make the edge come late; allow for 3 windows of them */
    rc = waitFor(motor, cmd, motorTimeout(3 * m->oneWayWindow, &m->oneWayCatch));
    sprintf(cmd, "A%c H%c", m->charAxis, m->homeLevel);
    (void) noWait(motor, cmd);
    if (rc != VME_OK) {
        if (rc == VME_TIMEOUT)
            mErrMsg(m, "%s home edge %d not found", m->name, edge);
        return rc;
    }
    if ((rc = tellPV(motor)) != VME_OK)
        return rc;

    /* positive = counter ran ahead of the turret = steps lost */
    half = m->fullTravel / 2;
    *err = ahead(m, expected, m->currPos);
    if (*err > half)
        *err -= m->fullTravel;
    printf("%s oneWay edge %d: expected %d got %d lost %d\n",
           m->name, edge, expected, m->currPos, *err);
    l = *err;
    sendStatus(CAMONEWAYERR, EPLONG, &l);

    sprintf(cmd, "A%c LP%d", m->charAxis, expected);
    rc = noWait(motor, cmd);
    if (rc == VME_OK)
        rc = tellPV(motor);
    return rc;
}

/* Stop on every home edge between here and 'where' */
static int edgesTo(int motor, int where) {
    motorVars *m = motors[motor];
    int rc, togo, edge, next, d, err;

    while ((togo = ahead(m, m->currPos, where)) > 0) {
        next = -1;
        for (edge = 0; edge < 2; edge++) {
            if (edge && !m->oneWayFarEdge)
                continue;
            d = toEdge(m, edge);
            if ((d < togo) && ((next < 0) || (d < toEdge(m, next))))
                next = edge;
        }
        if (next < 0)
            break;
        if ((rc = oneWayEdge(motor, next, &err)) != VME_OK)
            return rc;
    }
    return VME_OK;
}

int oneWayMove(int motor, int where) {
    motorVars *m = motors[motor];
    int rc, togo;

    if (gnirsG.simulation) {
        m->currPos = where;
        return VME_OK;
    }
    if (!m->datumed) {
        mErrMsg(m, "%s oneWay move when not datumed", m->name);
        return VME_ERROR;
    }
    if ((rc = tellPV(motor)) != VME_OK)
        return rc;
    if ((rc = edgesTo(motor, where)) != VME_OK)
        return rc;

    /* Fast, then the last 'backlash' steps slowly, still the same way */
    togo = ahead(m, m->currPos, where);
    rc = oneWayStep(motor, togo - m->backlash, &m->oneWaySeek);
    if (rc == VME_OK)
        rc = oneWayStep(motor, ahead(m, m->currPos, where), &m->backOff);
    if (rc != VME_OK)
        return rc;
    return halfTurn(motor);
}

/* oneWayTravel: fast in the travel direction to where - backlash, then the
 * last 'backlash' steps slowly, positive.
 */
int oneWayTravelMove(int motor, int where) {
    motorVars *m = motors[motor];
    int rc, d, fast;

    if (gnirsG.simulation) {
        m->currPos = where;
        return VME_OK;
    }
    if (!m->datumed) {
        mErrMsg(m, "%s oneWayTravel move when not datumed", m->name);
        return VME_ERROR;
    }
    if ((rc = tellPV(motor)) != VME_OK)
        return rc;

    d = dirOf(m);
    /* Going positive and already within the backlash: slow leg only */
    if ((d > 0) && (aheadDir(m, 1, m->currPos, where) <= m->backlash))
        fast = 0;
    else {
        if (m->oneWayCorrect &&
                ((rc = edgesTo(motor, where - m->backlash)) != VME_OK))
            return rc;
        fast = aheadDir(m, d, m->currPos, where - m->backlash);
    }
    printf("%s oneWayTravel %d: from %d to %d, fast %d then +%d\n", m->name,
           d, m->currPos, where, d * fast,
           aheadDir(m, 1, m->currPos + d * fast, where));

    rc = stepDir(motor, d, fast, &m->seek);
    if (rc == VME_OK)
        rc = stepDir(motor, 1, aheadDir(m, 1, m->currPos, where), &m->backOff);
    if (rc != VME_OK)
        return rc;
    return halfTurn(motor);
}

/* Coarse datum at seek speed, then oneWayRevs turns stopping on each edge.
 * cameraDatumErr is the worst error seen, not counting the first
 * edge, which corrects the coarse datum.
 */
int oneWayDatum(int motor) {
    motorVars *m = motors[motor];
    char cmd[MOTOR_CMD_LEN];
    char level;
    int rc, rev, edge, err, worst, n;
    long l;

    printf("Datuming %s one way (%d)\n", m->name, m->oneWay);
    if (gnirsG.simulation) {
        m->currPos = m->home.position;
        return VME_OK;
    }
    getMdStatus(motor);

    /* Find the other level first, so the second home is the datum edge
     * whatever the starting point.  HM/HR set the counter on the fly and
     * decelerate past the edge, so there is no reversing.
     */
    level = (m->oneWay > 0) ? m->homeLevel : otherLevel(m->homeLevel);
    sprintf(cmd, "A%c AC%d VL%d H%c H%c%d H%c H%c%d", m->charAxis,
            m->oneWaySeek.acceleration, m->oneWaySeek.velocity,
            otherLevel(level), (m->oneWay > 0) ? 'M' : 'R', m->home.position,
            level, (m->oneWay > 0) ? 'M' : 'R', m->home.position);
    rc = waitFor(motor, cmd, motorTimeout(2 * m->fullTravel, &m->oneWaySeek));
    sprintf(cmd, "A%c H%c", m->charAxis, m->homeLevel);
    (void) noWait(motor, cmd);
    if (rc != VME_OK) {
        mErrMsg(m, "%s oneWay coarse datum failed", m->name);
        return rc;
    }
    if ((rc = tellPV(motor)) != VME_OK)
        return rc;

    worst = 0;
    n = 0;
    for (rev = 0; (rev < m->oneWayRevs) || (rev == 0); rev++) {
        for (edge = 1; edge >= 0; edge--) {     /* far edge comes first */
            if (edge && !m->oneWayFarEdge)
                continue;
            if ((rc = oneWayEdge(motor, edge, &err)) != VME_OK)
                return rc;
            if (n++ && (abs(err) > worst))
                worst = abs(err);
        }
    }
    printf("%s oneWay datum: %d edges, worst %d steps\n", m->name, n, worst);
    l = worst;
    sendStatus(CAMONEWAYDATUMERR, EPLONG, &l);

    checkParked(motor);
    return VME_OK;
}
