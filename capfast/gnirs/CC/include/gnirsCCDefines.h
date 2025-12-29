
#ifndef __GNIRSCCDEFINESH
#define __GNIRSCCDEFINESH
/**************
 * MECHANISMS
 **************/

/* Number of "real" mechanisms */
#define NUM_MECH    10
/* EPICS support */

/*Don't change the value of EPICS_LEN, Changes to this require changes in the capfast diagrams that pull sad values from vxworks variables*/
#define EPICS_LEN   40
#define GNIRS_MESSAGE_LENGTH    512

#define SVMSG_LEN sizeof(SVMSG)
#define LV_MAX_MSG_LEN  80
#define ERR_MSG_LEN 80
/*********
 * MOTORS
 *********/
#define IDEAL_ORDER "IDEAL"


#endif
