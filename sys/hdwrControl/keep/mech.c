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
#include "stdarg.h"
#include "gnirsCC.h"
#include "timeLib.h"
#include "epicsTypes.h"


/* Mechanism and filter aliases */

/* Mechanism descriptions and control */

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
 */
int finishMechConfig() {
    compHead *pTable, *pTable2;
    NODE *node;
    mechNode *pnode;
    int mech;
    motorVars *m;

    /*
     * Check that each named position on filter wheel 1 is not duplicated
     * on filter wheel 2 (and by reciprocity, v.v.).
     */
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

    /* Fill in all SAD informatation that we can for each mechanism.
     * Also, fill in mech entry in motor structure.
     */
    for (mech = 0; mech < FILTER; mech++) {
        strncpy(mechName[mech], mechanism[mech].name, EPICS_LEN - 1);
        m = motors[mech];
        if (m) {
            mechParkPos[mech] = m->parkPosition;
        } else {
            mechParkPos[mech] = 0;
            strncpy(mechPos[mech], "Not in Use", EPICS_LEN - 1);
        }
        strncpy(mechHealth[mech], "GOOD", 5);
        strncpy(mechState[mech], "OK", 5);
    }

    return VME_OK;
}

void dumpMech(int mech) {
    LIST *pTable;
    NODE *node;
    mechNode *pnode;
    int m;

    if (mech >= FILTER)
        return;
    pTable = (LIST *)mechanism[mech].pTable;
    if(!lstCount(pTable))
        return;
    printf("%s (using motor %d):\n", mechanism[mech].name, mech);
    /* Find longest name (for pretty printing) */
    m = 0;
    for(node = lstFirst(pTable); node ; node = lstNext(node)) {
        pnode = (mechNode *)node;
        if (strlen(pnode->name) > m)
            m = strlen(pnode->name);
    }
    /* and print the information */
    for(node = lstFirst(pTable); node ; node = lstNext(node)) {
        pnode = (mechNode *)node;
        printf(" %*s at %5d\n", m, pnode->name, pnode->position);
    }
    printf("\n");
}


/* ** Cover ** (This is silly, but for consistency and ease of programming: */

focusType fsCover() {
    return 0;
}

/* ** Filter Wheels -- They are before the slit and can't affect
 * ** the focus.
 */

focusType fsFW1() {
    return 0;
}

focusType fsFW2() {
    return 0;
}

/* ** Slit ** */

focusType fsSlit() {

    return ((mechDescriptor *)&mechanism[SLIT])->focusShift;
}


/* ** Decker ** */

focusType fsDecker() {

    return ((mechDescriptor *)&mechanism[DECKER])->focusShift;
}


/* ** Acquisition mirror ** */

focusType fsAcq() {
    return 0;
}


/* ** Cross Dispersion ** */

focusType fsXdisp() {

    return ((mechDescriptor *)&mechanism[XDISP])->focusShift;
}


/* ** Grating ** */

focusType fsGrating() {

    return ((mechDescriptor *)&mechanism[GRATING])->focusShift;
}


/* ** Camera ** */

focusType fsCamera() {

    return ((mechDescriptor *)&mechanism[CAMERA])->focusShift;
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
        /* look for second filter in the other wheel */
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
