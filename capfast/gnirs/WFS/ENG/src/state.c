/* $Id: state.c,v 1.4 2009/11/19 00:50:30 mrippa Exp $ */

/*
 * This file implements routines for changing system parameters when the
 * as the temperature of the mechanisms change.  Currently, this is
 * just a front-end to pvload a collection of files in a directory
 * that is named after the system state, though it is designed to be
 * flexible enough to allow other actions to be carried out.
 */

#include "state.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <subRecord.h>
#include <devSup.h>
#include <recHallStep.h>

extern int pvload(const char *, const char *, int);

typedef enum { Pvload } Op;

typedef struct {
	char *fileName;
	char *dataPath;
	char *args;
	int flags;
} PvloadArgs;

typedef struct {
	Op op;
	union {
		PvloadArgs pvload;
	} args;
} Action;

static int nActions = 0;
static int maxActions = 0;
static Action *actions = NULL;

typedef struct {
	const char *state;
	const char *dir;
} Map;

static int nMaps = 0;
static int maxMaps = 0;
static Map *maps = NULL;

static char *strdup(const char *);
static int addPvload(const char *, const char *, const char *, int);
static Action *addAction(Op);

/*
 * How many times to check for activity on the temperature status
 * link before assuming a problem.
 */

#define TMP_RETRY (5)

/*
 * These constants are used for signalling the success or failure of
 * a function call.
 */

#define PASS           0
#define FAIL           1

#define WHITESPACE " \t\r\n"

/*
 * Load in a new configuration file, containing a list of files
 * to be pvload'ed.
 */

int
stateConfig(const char *config, const char *dataPath, const char *args,
	int flags)
{
	FILE *in;
	int status = OK;

	/*
	 * Parse the configuration file.  The configuration file is made
	 * a string of commands, one command per line.  Blank lines are
	 * ignored, and everything after a '#' is treated as a comment.
	 * 
	 * The commands are:
	 *     pvload FILENAME                Use pvload to load the file
	 */

	if (status == OK) {
		char buf[128];
		char *cp;
		char *cmd;
		int line = 1;

		if ((in = fopen(config, "r")) == NULL) {
			fprintf(stderr, "Cannot open %s\n", config);
			status = ERROR;
		}

		while (status == OK && fgets(buf, sizeof(buf), in) != NULL) {
			/*
			 * If the line is too long for the buffer, there won't
			 * be a termininating newline.
			 */

			if (strchr(buf, '\n') == NULL) {
				fprintf(stderr, "%s(%d): Input line too long\n",
					config, line);
				status = ERROR;
				break;
			}

			/*
			 * Strip off comments.
			 */
			
			if ((cp = strchr(buf, '#')) != NULL)
				*cp = '\0';

			/*
			 * Look for an action to be taken
			 */

			cmd = strtok(buf, WHITESPACE);

			/*
			 * Just ignore blank lines and comment lines.
			 */

			if (cmd == NULL)
				continue;

			/*
			 * Look for a valid command.
			 */

			if (strcmp(cmd, "pvload") == 0) {
				cp = strtok(NULL, WHITESPACE);
				if (cp == NULL) {
					fprintf(stderr, "%s(%d): pvload -- missing argument\n",
						config, line);
					status = ERROR;
					break;
				}

				addPvload(cp, dataPath, args, flags);
			} else {
				fprintf(stderr, "%s(%d): Illegal command: \"%s\"\n",
					config, line, cmd);
				status = ERROR;
				break;
			}

			line++;
		}
	}

	return status;
}

/*
 * Create a new action.  Returns a pointer to the action, or NULL if 
 * there is a failure.  
 */

static Action *
addAction(Op op)
{
	int status = OK;

	/*
	 * Make sure that there is enough room in the table for another
	 * action.  If not, then allocate more room.
	 */

	if (status == OK && nActions == maxActions) {
		maxActions = maxActions * 2 + 1;

		if (actions == NULL)
			actions = (Action *)malloc(maxActions * sizeof(Action));
		else
			actions = (Action *)realloc(actions, maxActions * sizeof(Action));
		if (actions == NULL) {
			fprintf(stderr, "Out of memory\n");
			status = ERROR;
		}
	}

	if (status == OK)
		actions[nActions].op = op;

	nActions++;

	if (status == OK)
		return &actions[nActions - 1];
	else
		return NULL;
}

static int
addPvload(const char *fileName, const char *dataPath, const char *args,
	int flags)
{
	int status = OK;
	Action *const pAction = addAction(Pvload);

	if (pAction == NULL)
		status = ERROR;

	if (status == OK) {
		PvloadArgs *const pArg = &pAction->args.pvload;

		pArg->flags = flags;

		if (status == OK && (pArg->fileName = strdup(fileName)) == NULL) {
			fprintf(stderr, "Out of memory\n");
			status = ERROR;
		}

		if (status == OK && (pArg->dataPath = strdup(dataPath)) == NULL) {
			fprintf(stderr, "Out of memory\n");
			status = ERROR;
		}

		if (status == OK && (pArg->args = strdup(args)) == NULL) {
			fprintf(stderr, "Out of memory\n");
			status = ERROR;
		}
	}

	return status;
}

int
stateSet(const char state[])
{
	int status = OK;
	char pvfile[256];
	Action *const pLastAction = actions + nActions;
	Action *pAction;
	const char *dir;
	int i;

	/*
	 * This is a linear search, which is acceptable, because there should 
	 * never be more than four maps (cold, warm, changing, default). 
	 */

	if (status == OK) {
		for (i = 0; i < nMaps; i++) {
			if (strcmp(maps[i].state, state) == 0) {
				dir = maps[i].dir;
				break;
			}
		}

		if (i == nMaps) {
			fprintf(stderr, "Unknown state: \"%s\"\n", state);
			status = ERROR;
		}
	}

	for (pAction = actions; status == OK && pAction < pLastAction;
			pAction++) {
		PvloadArgs *const pArgs = &pAction->args.pvload;

		switch (pAction->op) {
		case Pvload:
			if (strlen(pArgs->dataPath) + strlen(dir) 
					+ strlen(pArgs->fileName) + 3 > sizeof(pvfile)) {
				fprintf(stderr,
					"String \"%s/%s/%s\" is too long for buffer\n",
					pArgs->dataPath, dir,
					pArgs->fileName);
				status = ERROR;
			} else {
				strcpy(pvfile, pArgs->dataPath);
				strcat(pvfile, "/");
				strcat(pvfile, dir);
				strcat(pvfile, "/");
				strcat(pvfile, pArgs->fileName);

				printf("stateSet: pvload \"%s\", \"%s\", %d\n",
					pvfile, pArgs->args ? pArgs->args : "", pArgs->flags);
				if (pvload(pvfile, pArgs->args, pArgs->flags) != 0) {
					status = ERROR;
					fprintf(stderr, "stateSet() pvload returns error\n");

				}
			}
			break;

		default:
			fprintf(stderr, "Illegal action: %d\n", pAction->op);
			status = ERROR;
		}
	}

	return status;
}

static char *
strdup(const char *str)
{
	char *dup;
	
	dup = malloc(strlen(str) + 1);

	if (dup != NULL)
		strcpy(dup, str);
	
	return dup;
}

/*
 * Define a subdirectory containing files to be loaded when the system
 * moves to the specified state.  'State' and 'dir' must not be NULL.
 */

int
stateMap(const char *state, const char *dir)
{
	int status = OK;

	if (nMaps == maxMaps) {
		maxMaps = maxMaps * 2 + 1;

		if (maps == NULL)
			maps = (Map *)malloc(maxMaps * sizeof(Map));
		else
			maps = (Map *)realloc(maps, maxMaps * sizeof(Map));
		if (maps == NULL) {
			fprintf(stderr, "Out of memory\n");
			status = ERROR;
		}
	}

	if (status == OK) {
		if ((maps[nMaps].state = strdup(state)) == NULL) {
			fprintf(stderr, "Out of memory\n");
			status = ERROR;
		}
	}


	if (status == OK) {
		if ((maps[nMaps].dir = strdup(dir)) == NULL) {
			fprintf(stderr, "Out of memory\n");
			status = ERROR;
		}
	}

	nMaps++;

	return status;
}

/***************************************************************************
 * This really doesn't belong here, but it's just one short function,
 * not worth starting a new file.
 *
 * This would be well suited for SNL code.  Unfortunately, for some
 * reason, when SNL code with similar functionality is used, it crashes
 * with a memory fault.  As far as I can tell, when SNL is used to trigger
 * pvload (indirectly), there is some conflict.  Maybe there is some
 * non-reentrant function.
 ***************************************************************************/

#include <devSup.h>
#include <link.h>
#include <recHallStep.h>
#include <time.h>

int
stateCheck()
{
	int oldLockTmp = HS_TMP_INVALID;

	/*mrippa: Use for warm environment */
	/*int oldLockTmp = HS_TMP_WARM; */ 

	/*
	 * Wait for the temperature state to change.  There is no need
	 * to check very often.
	 */

	for (;;) {
		struct timespec ts;

		/*
		 * Don't use hsLockTmp or hsForceLockTmp directly, because the
		 * value could change while we are using it.  Once 
		 * the value of hsForceLockTmp is read out, set it to
		 * HS_TMP_INVALID, so that it won't continue to trigger a
		 * state change.
		 *
		 * TODO:  There is a possible race condition if hsForceLockTmp
		 * is set more than once, in a short period of time.  It
		 * shouldn't ever come up, in practice, but should really be
		 * fixed.
		 */

		const int forceLockTmp = hsForceLockTmp;
		int lockTmp = hsLockTmp;

		if (forceLockTmp != HS_TMP_INVALID) {
			hsForceLockTmp = HS_TMP_INVALID;
			hsLockTmp = forceLockTmp;
		}

		/*
		 * Checking every half second or so should be enough.  Actually,
		 * for normal use, 0.5 seconds is massive overkill.  Once every
		 * ten or twenty minutes would be sufficient.
		 *
		 * hsForceLockTmp is a massive kludge.  The high level interface
		 * hangs if we set hsLockTmp to be its current value, because
		 * the records downstream don't get processed.  hsForceLockTmp
		 * clears its value after it is used, so it always causes
		 * processing to take place, even if it is unnecessary.  There
		 * should be a better way to do this, probably by giving this
		 * record direct control over Busy/Idle.
		 *
		 * TODO:  If hsForceLockTmp == oldLockTmp, we really only need to
		 * toggle Busy/Idle.  We don't need to load the new state.
		 */

		ts.tv_sec = 0;
		ts.tv_nsec = 500000000; 
		nanosleep(&ts, NULL);

		if (forceLockTmp != HS_TMP_INVALID || lockTmp != oldLockTmp) {
			if (forceLockTmp != HS_TMP_INVALID)
				lockTmp = forceLockTmp;

			 switch (lockTmp) {
			case HS_TMP_COLD:
				stateSet("cold");
				break;

			case HS_TMP_WARM:
				stateSet("warm");
				break;

			case HS_TMP_CHANGING:
				stateSet("changing");
				break;

			default:
				fprintf(stderr, "hsLockTmp: Illegal value: %d\n", lockTmp);
			}  /* switch commented out for interlock. Under normal operation
			should be uncommented. CUR 4 Sept 2008 */

			oldLockTmp = lockTmp;
		} 
		/*stateSet("warm");*/
	}
}

/***********************************************************************
 * These two functions really doesn't belong here, but there are just
 * two short functions, not worth starting a new file.
 ***********************************************************************/

/*
 *+
 * FUNCTION NAME: tmpExtInit
 *
 * INVOCATION: tmpExtInit(psr)
 *
 * PARAMETERS:
 *
 *     (!) psr (struct subRecord *) Subroutine record pointer
 *     (>) psr->e (double) Number of times to retry, before assuming a problem
 *
 * FUNCTION VALUE: (long) Error status (nonzero indicates an error)
 *
 * SCOPE: global
 *
 * PURPOSE: Initialize record to monitor temperature state
 *
 * DESCRIPTION:
 *
 *     This record is used to set default values for a subroutine 
 *     record, which is used to monitor the current temperature state
 *     of a system, using tmpExtSub().
 *
 * EXTERNAL VARIABLES:
 *
 *     hsLockTmpDB: Set to non-zero if there is a communications problem
 *
 * PRIOR REQUIREMENTS:
 *
 *    This routine is intended to be the processing routine for a
 *    subroutine record, and is not intended to be called
 *    directly.  It assumes that all standard record initialization
 *    has taken place, before it has been called.
 * 
 * DEFICIENCIES:
 *-
 */

int
tmpExtInit(struct subRecord *psr)
{
	psr->e = TMP_RETRY;
	return PASS;
}

/*
 *+
 * FUNCTION NAME: tmpExtSub
 *
 * INVOCATION: tmpExtSub(psr)
 *
 * PARAMETERS:
 *
 *     (!) psr (struct subRecord *) Subroutine record pointer
 *     (<) psr->a (double) Current temperature state
 *     (<) psr->b (double) Current value of heartbeat variable
 *     (!) psr->c (double) Previous value of heartbeat variable
 *     (!) psr->d (double) Number of communications failures
 *     (<) psr->e (double) Number of times to retry, before assuming a problem
 *     (>) psr->val (double) Modified temperature state 
 *
 * FUNCTION VALUE: (long) Error status (nonzero indicates an error)
 *
 * SCOPE: global
 *
 * PURPOSE: Monitor and output temperature state
 *
 * DESCRIPTION:
 *
 * This subroutine is used to monitor a temperature state EPICS variable
 * (one of the HS_TMP_* values) and output it.  In order
 * to detect problems with the remote IOC, it monitors an EPICS
 * variable which should be changing periodically (the heartbeat).  If
 * the heartbeat is not changing, is assumes that there is a problem,
 * and forces the system to the HS_TMP_CHANGING state, which is a 
 * safe state, in which no mechanisms may be moved.
 *
 * EXTERNAL VARIABLES:
 *
 *     hsLockTmpDB: Set to non-zero if there is a communications problem
 *
 * PRIOR REQUIREMENTS:
 *
 * This routine is intended to be the processing routine for a subroutine
 * record, and is not intended to be called directly.  It assumes that
 * all standard record initialization has taken place, before it has
 * been called.  The subroutine record should have be initialized by
 * tmpExtInit().
 *
 * DEFICIENCIES:
 *-
 */

int
tmpExtSub(struct subRecord *psr)
{
/*   fprintf(stderr,"temp = %f, hb1 = %f, hb2 = %f\n", psr->a, psr->b, psr->c); */
   if (psr->b != psr->c) { /* Healthy heartbeat */
      psr->c = psr->b;
      psr->val = psr->a;
      psr->d = 0;
      hsLockTmpHB = 0;
   } else {
      psr->d++;

      if (psr->d > psr->e) {
	 	hsLockTmpHB = 1;
	 	psr->val = HS_TMP_CHANGING;
      } else { /* Failed, but not enough to worry about yet */
	 	hsLockTmpHB = 0;
	 	psr->val = psr->a;
      }
   }

   return PASS;
}
