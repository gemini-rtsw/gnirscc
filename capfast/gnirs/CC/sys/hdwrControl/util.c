static struct
  {
      void *v;
      char *c;
  }
sccsid =
{
    &sccsid,
        "@(#)util.c	1.7 07/30/03"
};

/* included headers*/
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

/* function prototypes*/
int doPosition(char *, char *, char *);




void getClockSpeed(void)
{
    gnirsG.clockRate = sysClkRateGet();
}

int readConfig(char * file) {
    int rc, type, i, motor;
    FILE *fd;
    char buffer[CONFIG_CHAR_LEN];
    char orig[CONFIG_CHAR_LEN];
    char *p, *q, *start, *base;
    BOOL sectionSeen;
    BOOL inString;
    itemConfig * pItem;

    /* Open the file
     * For each line
     *   Strip out comments
     *   Strip out trailing blanks
     *   If line is now null, get next line
     *   Find '=', and set to '\0', thus delimiting the keyword
     *   Except within quoted strings, replace all ',' with a space
     *   Set keyword to lowercase and go to proper pocessing "routine"
     * Close the file
     */
	printf("file = %s\n",file);
    fd = fopen(file, "r");
    if (fd == NULL) {
        gnirsLogMessage(CICS_DB_ERROR1, "Cannot open '%s'.", file);
        return VME_ERROR;
    }
    sectionSeen = FALSE;
    motor = -1;   /* flag for finishMotorConfig call */
    while (fgets(buffer, CONFIG_CHAR_LEN, fd) != NULL) {
        buffer[CONFIG_CHAR_LEN-1] = '\0';      /* just to be sure */
	   
        strcpy(orig, buffer);
        p = strchr(buffer, '\n');
        if (p != NULL)
            *p = '\0';
        /* remove comments */
        p = strchr(buffer, '#');
        if (p != NULL)
            *p = '\0'; /* this is a comment*/
        /* Strip leading blanks */
        p = buffer;
        while ((*p == ' ') || (*p == '\t'))
            p++;
		/* if the current character is null*/
        if (!*p)   /* reached a null ... blank line */
            continue;
        /* Parse Section Header */
        if (*p == '[') {
            p++;
            sectionSeen = TRUE;
            if ((q = strchr(p, ']')) == NULL) {
                gnirsLogMessage(CICS_DB_FILE, "No ']' in section header: %s",
                        orig);
				printf("Error in readConfig(%s)\n",file);
                return VME_ERROR;
            }
            *q = '\0';
            /* lower case */
            for (q = p; *q; q++)
                if (isupper(*q))
                    *q = tolower(*q);
            /* Skip any leading white space within '[]' */
            while ((*p == ' ') || (*p == '\t'))
                p++;
            q = strchr(p, ' ');
            /* ok for q to be NULL as it means we have [keyword] with no spaces
             * and the ']' has already been set to null, delimiting the
             * keyword.
             */
            if (q)
                *q++ = '\0';
            /* Now 'q' points to what follows the keyword, if anything */

            /* Find the header type:
             *      motor
             *      mechanism
             *      filters
             *      temperature
             *      cryo
             *      grating
             */
            if (strncmp (p, "motor", 5) == 0) {
                /* which motor */
                i = atoi(q);
                if ( (i < 0) || (i >= NUM_MOTORS)) {
                    gnirsLogMessage(CICS_DB_FILE,
                            "Illegal motor number in %s", orig);
				printf("Error in readConfig(%s)\n",file);
                    return VME_ERROR;
                }
                /* Moving on to different motor, so finish previous one */
                if (motor != -1)
                    finishMotorConfig(motor);
                rc = setupMotor(i);
                if (rc != VME_OK)
				{
					printf("Error in readConfig(%s)\n",file);
                    return rc;
				}
                type = CONF_MOTOR;
                motor = i;
                base = (char *) motors[motor];
                pItem = &motorConfig[0];
            } else if (strncmp (p, "mechanism", 9) == 0) {
                /* Change to lower case and remove spaces */
                for (p = start = q; *p; p++) {
                    if (isupper(*p))
                        *q++ = tolower(*p);
                    else if (!isspace(*p))
                        *q++ = *p;
                }
                *q = '\0';
                type = CONF_MECHANISM;
                /* which mechanism */
                base = (char  *)NULL;
                for (i = 0; i < NUM_MECH; i++) {
                    if (strncmp(mechanism[i].configName, start,
                                        strlen(mechanism[i].configName)) == 0) {
                        base = (char *) &mechanism[i];
                    break;
                    }
                }
                if (!base) {
                    gnirsLogMessage(CICS_DB_FILE,
                            "Cannot find mechanism in %s\n", orig);
				printf("Error in readConfig(%s)\n",file);
                    return VME_ERROR;
                }
                pItem = &mechConfig[0];

            } else if (strncmp (p, "filters", 7) == 0) {
                type = CONF_FILTER;
            } else if (strncmp (p, "temperature", 10) == 0) {
                /* which sensor */
                i = atoi(q);
                if ( (i < 0) || (i > (NUM_TEMPS - 1))) {
                    gnirsLogMessage(CICS_DB_FILE,
                            "Illegal temperature sensor in %s", orig);
				printf("Error in readConfig(%s)\n",file);
                    return VME_ERROR;
                }
                type = CONF_TEMP;
                base = (char *)&temperatures[i];
                pItem = &tempConfig[0];
                /* If it is a reference input, should user be allowed
                 * to change it? ... No way to prohibit this given the
                 * way code written (too general).
                 */
            } else if (strncmp(p, "cryo", 4) == 0) {
                type = CONF_CRYO;
                base = (char *)cryoSwitches;
                pItem = &cryoConfig[0];
            } else if (strncmp(p, "grating", 7) == 0) {
                type = CONF_GRATING;
                /* which grating */
                i = atoi(q);
                if ( (i < 0) || (i >= NUM_GRATINGS)) {
                    gnirsLogMessage(CICS_DB_FILE,
                            "Illegal grating number in %s", orig);
				printf("Error in readConfig(%s)\n",file);
                    return VME_ERROR;
                }
                base = (char *)&gratingData[i];
                pItem = &gratConfig[0];
            } else {
                gnirsLogMessage(CICS_DB_ERROR,
                        "Section '%s' not recognized", q);
				printf("Error in readConfig(%s)\n",file);
                return VME_ERROR;
            }
            /* Valid section header seen; get next line */
            continue;
        }
        if (!sectionSeen) {
            gnirsLogMessage(CICS_DB_FILE,
                    "Section header must be first: file %s", file);
				printf("Error in readConfig(%s)\n",file);
            return VME_ERROR;
        }
        start = p;
        inString = FALSE;
        /* Replace ',' with space when not in string */
        while (*p) {
            switch (*p) {
                case '"':
                    if (inString == TRUE)
                        inString = FALSE;
                    else
                        inString = TRUE;
                    break;
                case ',':
                    if (inString == FALSE)
                        *p = ' ';
                    break;
            }
            p++;
        }
        /* Find the '=' and mark the end of the keyword */
        p = strchr(start, '=');
        if (p == NULL) {
            gnirsLogMessage(CICS_DB_FILE, "No '=' in %s", orig);
				printf("Error in readConfig(%s)\n",file);
            return VME_ERROR;
        }
        *p++ = '\0';
        /* Skip any leading white space */
        while ((*p == ' ') || (*p == '\t'))
            p++;
        /* 'keyword' to lowercase and then find it */
        for(q = start; *q; q++)
            if isupper(*q)
                *q = tolower(*q);
        if (type == CONF_FILTER)
            rc = setFilters(start, p, orig);
        else
            rc = parseKeywordValue(start, pItem, base, p, orig);
        if (rc != VME_OK)
		{
			printf("Error in readConfig(%s)\n",file);
            return rc;
		}
    }
    fclose(fd);
    /* Rerun finishMotorConfig on last motor if this wasn't done before */
    if (motor != -1)
        finishMotorConfig(motor);
    return VME_OK;
}

/* Parse a keyword = value line */
int parseKeywordValue(char *keyword, itemConfig *pItem, char * base,
                                            char *rhs, char *orig) {
    switchType *pSw;
    motion *pMotion;
    ioBit *iob;
    char *where;
    char *p;
    itemConfig *pI;
    int i, type;
    int port, bit, level, mask;
    BOOL found;
    double *dq;
    int errors = 0;

    found = FALSE;
    for (pI = pItem; pI->item && !found; pI++) {
        if (strncmp(keyword, pI->item, strlen(pI->item))== 0) {
            found = TRUE;
            /* At the end of the code dealing with each keyword, i is
             * to equal 1 if the line has been parsed correctly;
             * anything else is failure.
             * The '1' is not so much TRUE as it is the result of
             * a successful sscanf call with one conversion.
             */
            i = 0;
            where = pI->offset + base;
            type = pI->type;
            /* First deal with a special case */
            switch(type) {
                case CONF_FOCUS:
                    where = (char *)&focusZeroPoint;
                    type = CONF_INT;
                    break;
            }
            switch (type) {
                case CONF_INT:
                    i = sscanf(rhs, "%d", (int *)where);
                    break;
                case CONF_DOUBLE:
                    i = sscanf(rhs, "%lf", (double *)where);
                    break;
                case CONF_CHAR:
                    /* For quoted char constant, as in "L" */
                    i = 1;
                    if (*rhs == '"')
                        if (*(rhs+2) == '"')
                            *(char *)where = rhs[1];
                        else
                            i = 0;  /* multi char string not wanted */
                    else
                        *(char *)where = rhs[0];
                    break;
                case CONF_STRING:
                    /* Can store maxLen-1 char plus null; rhs has two '"'
                     * which don't get stored.  Hence  maxLen+1
                     */
                    if (strlen(rhs) >= (pI->maxLen + 1))
                        i = 0;
                    else {
                        i = 1;
                        p = strchr(rhs,'"');
                        /* skip a null string */
                        if (p && (*(p+1) != '"'))
                            i = sscanf(rhs, " \"%[^\"]\"", (char *)where);
                    }
                    break;
                case CONF_APPEND_STR:
                    if ((strlen(rhs) + strlen(where))
                                >= pI->maxLen)
                        i = 0;
                    else {
                        i = 1;
                        p = strchr(rhs,'"');
                        /* skip a null string */
                        if (*p && (*(p+1) != '"')) {
                            i = strlen(where);
                            i = sscanf(&rhs[i], " \"%[^\"]\"", (char *)where);
                        }
                    }
                    break;
                case CONF_TYPE:
                    /* ugly */
                    i = 1;
                    for (p = rhs; *p; p++)
                        if (isupper(*p))
                            *p = tolower(*p);
                    if (strncmp(rhs, "rotary", 6) == 0)
                        *(int *)where = ROTARY;
                    else if (strncmp(rhs, "linear", 6) == 0)
                        *(int *)where = LINEAR;
                    else if (strncmp(rhs, "binary", 6) == 0)
                        *(int *)where = BINARY;
                    else
                        i = 0;
                    break;
                case CONF_SWITCH:
                    pSw = (switchType *)where;
                    /* Home switches have 6 items; other switches have 2 */
                    i = sscanf(rhs, "%d %d %d %d %d %d", &(pSw->position),
                            &(pSw->offset), &port, &bit,
                            &(pSw->useAlt), &(pSw->type));
                    if ( (i != 6) && (i != 2))
                        i = 0;
                    else if (i == 6) 
					{
                        pSw->control.port= port & 0xFF;
                        pSw->control.bit = bit & 0xFF;
					   						
						if ((1 << port) & xycomOutputs) 
						{
							mask = 1 << bit;
							if (pSw->useAlt)
								xycomOutputInit[port] |= mask;
							else
								xycomOutputInit[port] &= ~mask;
						}
					}
                    break;
                case CONF_MOTION:
                    pMotion = (motion *)where;
                    i = sscanf(rhs, "%d %d", &(pMotion->acceleration),
                            &(pMotion->velocity));
                    if (i != 2)
                        i = 0;
                    break;
                case CONF_POSITION:
                    i = doPosition(base, rhs, orig);
                    break;
                case CONF_GBOUNDS:
                    i = 0;
                    dq = (double *)where;
                    if ((p = strtok(rhs, " ,")) &&
                                    sscanf(p, "%lf", dq) ) {
                        i++;
                        dq++;
                        while ((p = strtok(NULL, " ,")) &&
                                            sscanf(p, "%lf", dq)) {
                            i++;
                            dq++;
                            if (i >= NUM_GRAT_BOUNDS)
                                break;
                        }
                    }
                    for(; i < NUM_GRAT_BOUNDS; i++ )
                        *dq++ = 100000.;  /* an unlikely micron wavelength!! */
                    break;
                case CONF_BIT:
                    iob = (ioBit *)where;
                    i = sscanf(rhs, "%d %d %d", &port, &bit, &level);
                    if (i != 3)
                        i = 0;
                    else {
                        port &= 0xFF;
                        bit &= 0xFF;
                        iob->port= port;
                        iob->bit = bit;
                        /* Set output ports */
                        if ((1 << port) & xycomOutputs) {
                            mask = 1 << bit;
                            if (level)
                                xycomOutputInit[port] |= mask;
                            else
                                xycomOutputInit[port] &= ~mask;
                        }
                    }
                    break;

            }
            if ( i == 0) {
                gnirsLogMessage(CICS_DB_ERROR, "Failed to convert rhs <%s>", rhs);
                errors++;
            }
        }
    }
    if (!found) {
        gnirsLogMessage(CICS_DB_ERROR, "readConfigfailed to find keyword in %s", orig);
        return VME_ERROR;
    }
    if (errors)
        return VME_ERROR;
    return VME_OK;
}

int doPosition(char *base, char *rhs, char *orig) {
    int i;
    char *p;
    char name[ITEM_ID_LEN];
    char fName[ITEM_ID_LEN];
    int position, shift;
    NODE *pNode;
    motorVars *m;
    mechDescriptor *pMech;

    pMech = (mechDescriptor *)base;
    /*printf("doPosition: %s\n",rhs);*/
    p = strchr(rhs,'"');
    if (!p) {
        gnirsLogMessage(CICS_DB_FILE, "string expected in <%s>", orig);
        return 0;
    }

    i = sscanf(rhs, " \"%[^\"]\" %d %d \"%[^\"]\"", name, &position, &shift, fName);

    printf("fname = \"%s\" %d %d \"%s\" %d\n", name, position, shift, fName, i);

    if (i == 3) {
      strcpy(fName,name);
      i = 4;
    }

    if (i==4)
    { 
	/* validate position */
        m = motors[pMech->motor];
        if (!m)     /* if not defined, ignore for now --- testing XXX XXX */
            return i;
        if (m->type == ROTARY) {
            if (abs(position) > m->fullTravel) {
                gnirsLogMessage(CICS_DB_FILE,
                        "Invalid position for rotary mechanism <%s>",
                        orig);
                return 0;
            }
        } else if (m->type == BINARY) {
            /* Should be home (==0) or neg limit, and at least between
             * the two.
             */
            if (position >= 0)
                position = m->posLimit.position;
            else if (position <= BINARY_LIMIT)
                position = m->negLimit.position;
        } else if ((position <= m->negLimit.position) ||
                   (position >= m->posLimit.position)) {
                gnirsLogMessage(CICS_DB_FILE,
                        "Invalid position for linear mechanism <%s>",
                        orig);
				printf("%d<%d<%d\n",m->negLimit.position,position,m->posLimit.position);
                return 0;
        }
        /* lookup name, and insert it along with position in list */
        pNode = nodeLookup(name, pMech->pTable, TRUE);
        if (pNode == NULL) {
            gnirsLogMessage(CICS_DB_FILE, "Can't find <%s>", name);
            return 0;
        }

        /* insert values */
        strncpy(((mechNode *)pNode)->name, name, ITEM_ID_LEN);
        strncpy(((mechNode *)pNode)->fName, fName, ITEM_ID_LEN);
        ((mechNode *)pNode)->position = position;
        ((mechNode *)pNode)->focusShift = shift;
        i = 1;
    }
    return i;
}

void strnlc(char *out, char *in, int len) {
    char *p, *q;
    int i;

    for (q = out, p = in, i = 0; (i < len) && *p ; i++) {
        if (isupper((int)*p)) {
            *q++ = (char) tolower((int)*p);
            p++;        /* not in "tolower" ... side effects in compiler */
    } else
            *q++ = *p++;
    }
    *q = '\0';
}

int findchar(char *in, char mychar, int len) {
  int i=0;
  char *p;
	
  p = in;
  while (i<len) {
     
     if(*p++ == mychar) 
	return i;
     i++;
  }
  return -1;
}

void healthString(char *hp, int health, int len)
{
    char *p;

    switch (health) {
    case GOOD:
        p = "GOOD";
        break;
    case WARNING:
        p = "WARNING";
        break;
    case BAD:
        p = "BAD";
        break;
    }
    strncpy(hp, p, len);
}

