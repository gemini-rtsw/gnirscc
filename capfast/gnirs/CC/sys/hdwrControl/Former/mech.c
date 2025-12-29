static struct
  {
      void *v;
      char *c;
  }
rcsid =
{
    &rcsid,
        "$Id: mech.c,v 1.2 2009/05/27 19:32:08 fkraemer Exp $"
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
#include "gnirsCC.h"
#include "timeLib.h"
#include "epicsTypes.h"


/* Mechanism and filter aliases */

/* Mechanism descriptions and control */

/* Set up the linked list of position descriptions.  Use the VxWorks
 * doubly linked list since it's there (the double linked-ness is not
 * useful for this application.
 */
int mechDescInit() {
    int i;
    mechDescriptor *pMech;

    for (i = 0; i < NUM_MECH; i++) {
        pMech = &mechanism[i];
        if (pMech->pTable)
            lstFree((LIST *)(pMech->pTable));
        pMech->pTable = (compHead *)&mechHead[i];
        (pMech->pTable)->type = NODE_MECH;
        lstInit((LIST *)(pMech->pTable));
    }
    return VME_OK;
}

NODE *makeNode(int type) {
    NODE *node;

    switch (type) {
        case NODE_MECH:
            node = (NODE *)calloc(1, sizeof(mechNode));
            break;
        case NODE_FILTERS:
            node = (NODE *)calloc(1, sizeof(filterNode));
            break;
        default:
            node = (NODE *)NULL;
            break;
    }
    /* in case of failure, caller will report problem */
    return node;
}

/* Look up an item in the list; the comparison is done all
 * lower case, but the names are stored as entered in the
 * configuration file...this is done to help avoid user
 * errors.
 */
NODE *nodeLookup(char * item, compHead *pList, int append) {
    NODE *node, *new;
    int type;
    char lcItem[ITEM_ID_LEN];
    char lcNode[ITEM_ID_LEN];
    
    strnlc(lcItem, item, ITEM_ID_LEN);
    type = pList->type;
    for (node = lstFirst((LIST *)pList); node; node = lstNext(node)) {
        switch (type) {
            case NODE_MECH:
                strnlc(lcNode, ((mechNode *)node)->name, ITEM_ID_LEN);
                break;
            case NODE_FILTERS:
                strnlc(lcNode, ((filterNode *)node)->name, ITEM_ID_LEN);
                break;
            default:
                return (NODE *)NULL;
                break;
        }
        if (strncmp(lcItem, lcNode, ITEM_ID_LEN) == 0)
            return node;
    }
    /* Nothing found ... if append, add a node; else return NULL */
    if (!append)
        return (NODE *)NULL;
    new = makeNode(type);
    if (new == NULL) {
        gnirsLogMessage(CICS_DB_ERROR, "Could not create position node");
        return (NODE *)NULL;
    }
    lstAdd((LIST *)pList, new);
    switch (type) {
        case NODE_MECH:
            strncpy(((mechNode *)new)->name, item, strlen(item));
            break;
        case NODE_FILTERS:
            strncpy(((filterNode *)new)->name, item, strlen(item));
            break;
    }
    return new;
}

/* Complete reading of configuration information.  This is called
 * when the whole configuration file has been read.
 *
 * Check that each named position on filter wheel 1 is not duplicated
 * on filter wheel 2 (and by reciprocity, v.v.).
 */
int finishMechConfig() {
    compHead *pTable, *pTable2;
    NODE *node;
    mechNode *pnode;

    pTable  = mechanism[FW1].pTable;
    pTable2 = mechanism[FW2].pTable;
    if(lstCount((LIST *)pTable)) {
        for(node = lstFirst((LIST *)pTable); node ; node = lstNext(node)) {
            pnode = (mechNode *)node;
            if (nodeLookup(pnode->name, (compHead *)pTable2, FALSE)) {
                gnirsLogMessage(CICS_DB_FILE,
                        "Filter duplicated in wheels: %s", pnode->name);
                return VME_ERROR; 
            }
        }
    }
    return VME_OK;
}

void dumpMech(int mech) {
    LIST *pTable;
    NODE *node;
    mechNode *pnode;
    int m;

    pTable = (LIST *)mechanism[mech].pTable;
    if(!lstCount(pTable))
        return;
    printf("%s (using motor %d):\n", mechanism[mech].name,
                            mechanism[mech].motor);
    /* Find longest name (for pretty printing) */
    m = 0;
    for(node = lstFirst(pTable); node ; node = lstNext(node)) {
        pnode = (mechNode *)node;
        if (strlen(pnode->name) > m)
            m = strlen(pnode->name);
    }
    for(node = lstFirst(pTable); node ; node = lstNext(node)) {
        pnode = (mechNode *)node;
        printf(" %*s at %5d\n", m, pnode->name, pnode->position);
    }
}


/* ** Cover ** (This is silly, but for consistency and ease of programming: */

focusType fsCover() {
    return 0;
}

/* ** Filter Wheels ** */

focusType fsFW1() {
    return 0;
}

focusType fsFW2() {
    return 0;
}

/* ** Slit ** */

focusType fsSlit() {
    return 0;
}


/* ** Decker ** */

focusType fsDecker() {
    return 0;
}


/* ** Acquisition mirror ** */

focusType fsAcq() {
    return 0;
}


/* ** Cross Dispersion ** */

focusType fsXdisp() {
    return 0;
}


/* ** Grating ** */

focusType fsGrating() {
    return 0;
}


/* ** Camera ** */

focusType fsCamera() {
    return 0;
}

/* ** Focus ** */

focusType fsFocus() {
    return focusZeroPoint;
}

/* ** Filters (filter aliases) ** */

int filterDescInit() {

    if (lstCount((LIST *)&filterHead))
        lstFree((LIST *)&filterHead);
    filterHead.type = NODE_FILTERS;
    lstInit((LIST *)&filterHead);

    return VME_OK;
}

/* format is  alias = "abc" as "kmn" and "rst" */
int setFilters(char *start, char *rhs, char *orig) {
    char alias[ITEM_ID_LEN];
    char f1[ITEM_ID_LEN];
    char f2[ITEM_ID_LEN];
    filterNode *node;
    int n, i1, i2;

    /* only one keyword */
    if (strncmp(start, "alias", 5)) {
        gnirsLogMessage(CICS_DB_FILE, "Unrecognized keyword in <%s>", orig);
        return VME_ERROR;
    }
    n = sscanf(rhs, " \"%[^\"]\" as \"%[^\"]\" and \"%[^\"]\"", alias, f1, f2);
    if (n != 3) {
        /* Try other format */
        n = sscanf(rhs, " \"%[^\"]\" as %d and %d", alias, &i1, &i2);
        if (n != 3) {
            gnirsLogMessage(CICS_DB_FILE, "Bad format in <%s>", orig);
            return VME_ERROR;
        }
        /* One index must be >= FIBIAS, < (2*FIBIAS), and the other
         *                   >= 2*FIBIAS, <(3*FIBIAS)
         */
        if ( ((i1 + i2) < (4 * FIBIAS)) && ((i1 + i2) >= (3 * FIBIAS)) ) {
            sprintf(f1, "%d", i1);
            sprintf(f2, "%d", i2);
        } else {
            gnirsLogMessage(CICS_DB_FILE, "Bad index in <%s>", orig);
            return VME_ERROR;
        }
    }

    node = (filterNode *)nodeLookup(alias, &filterHead, TRUE);
    strncpy(node->f1, f1, ITEM_ID_LEN);
    strncpy(node->f2, f2, ITEM_ID_LEN);

    return VME_OK;
}

void dumpFilters() {
    filterNode *node;
    int m, m1;

    if (lstCount((LIST *)&filterHead) == 0) {
        printf("No filter combinations defined.\n");
        return;
    }

    m = m1 = 0;
    for (node = (filterNode *)lstFirst((LIST *)&filterHead); node ;
                            node = (filterNode *)lstNext((NODE *)node)) {
        if (strlen(node->name) > m)
            m = strlen(node->name);
        if (strlen(node->f1) > m1)
            m1 = strlen(node->f1);
    }

    for (node = (filterNode *)lstFirst((LIST *)&filterHead); node ;
                            node = (filterNode *)lstNext((NODE *)node))
        printf("%*s : %*s and %s\n", m, node->name, m1, node->f1, node->f2);
}

/*
 * Check that each of the filter pairs in a 'filter' points to a named
 * position on a wheel and that the two named positions are on different
 * wheels.
 */
int finishFiltersConfig() {
    filterNode *node;
    BOOL f1IsFW1;
    char temp[ITEM_ID_LEN];

    for (node = (filterNode *)lstFirst((LIST *)&filterHead); node ;
                                node = (filterNode *)lstNext((NODE *)node)) {
        /* look for first filter, in both wheels if necessary */
        if (nodeLookup(node->f1, mechanism[FW1].pTable, FALSE) ==
                                                        (NODE *)NULL)
            if (nodeLookup(node->f1, mechanism[FW2].pTable, FALSE) ==
                                                            (NODE *)NULL) {
                gnirsLogMessage(CICS_DB_FILE, "Unknown filter <%s>", node->f1);
                return VME_ERROR;
            } else
                f1IsFW1 = FALSE;
        else
                f1IsFW1 = TRUE;
        /* look for second filter in other wheel */
        if (nodeLookup(node->f2,
            mechanism[f1IsFW1 ? FW2 : FW1].pTable, FALSE) ==
                                                        (NODE *)NULL) {
            if (nodeLookup(node->f2,
                mechanism[f1IsFW1 ? FW1 : FW2].pTable, FALSE) !=
                                                        (NODE *)NULL)
                    gnirsLogMessage(CICS_DB_FILE,
                                "Both filters in same wheel <%s, %s>",
                                node->f1, node->f2);
            else
                gnirsLogMessage(CICS_DB_FILE, "Filter <%s> unknown", node->f2);
            return VME_ERROR;
        }
        if (!f1IsFW1) {
            /* exchange f1 and f2 so that f1 is for filter wheel 1 */
            strcpy(temp, node->f1);
            strcpy(node->f1, node->f2);
            strcpy(node->f2, temp);
        }
    }
    return VME_OK;
}

/*******************
 * Cryohead Control
 *******************/

/* Turn cryoHead on or off, when in computer mode.  Set, or 'on', is
 * +5V; off is 0V; function is suitable for "manual" control  as well.
 */
void cryoHead(int state) {
    if (state)
        setBit(cryoSwitches[COMPUTER_CONTROL]);
    else
        clearBit(cryoSwitches[COMPUTER_CONTROL]);
    gnirsG.cryoCpuState = state;
}

/* interrupt handler for switching into computer mode*/
/* Read manual switch and set computer mode that way */
void computerMode() {
    int on;

    gnirsG.cryoSelect = COMPUTER;
    on = isSet(cryoSwitches[MANUAL_SWITCH]);
    cryoHead(on);
    gnirsG.cryoCpuState = on;
}

/* interrupt handler for switching out of computer mode */
void manualMode() {
    gnirsG.cryoSelect = MANUAL;
    cryoHead(CRYO_OFF);  /* might as well */
}

/* Called to finish configuration */
void finishCryoConfig() {
    /* Read the switches, set gnirsG items */
    if (isSet(cryoSwitches[COMPUTER_MANUAL]))
        manualMode();
    else
        computerMode();

    /* Set the interrupt handler and unmask the interrupt for each bit  */
    xycomSetInterrupt(0, computerMode);
    xycomSetInterrupt(1, manualMode);
}
