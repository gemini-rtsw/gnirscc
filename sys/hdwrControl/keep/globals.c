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

#define NO_EXTERNS
#include "gnirsCC.h"

/* generally global variables */

GNIRS_ST_GLOBALS gnirsG = {
    FALSE,                      /* init done */
    60,                         /* clock rate */
    BOOTING,                    /* EPICS state */
    GOOD,                       /* EPICS health */
    FALSE, FALSE,               /* all parked, all datumed */
    NOSIM,                      /* simulation mode */
    LABVIEW ,                   /* labview for gui */
    NULL,                       /* server Fd */
    LVNFS,                      /* via SERVER or FILE */
    MANUAL,                     /* manual mode on */
    CRYO_OFF,                   /* computer mode setting: off */
};

char configDirectory[CONFIG_DIR_LEN];

/* Motors */
GNIRS_ST_MD motorDriver[NUM_CARDS];
char axisLetter[NUM_AXES] = {'X', 'Y', 'Z', 'T'};

/* The motorVars structures */
motorVars motorV[NUM_MOTORS];
/* pointers to motor structures
 * This indirection isn't necessary, but grew out of the way the
 * code was developed.  It's handy for some diagnostics which will skip
 * over a motor structure if the pointer is NULL.
 */
motorVars *motors[NUM_MOTORS];

/* motor initialization string.
 * HL  Home low
 * SL  Soft limits --- "affects all axes simultaneously"
 * Must end in a blank in case something is added after (m->cfgInitString)
 */
char *motorInitString = "HL AX SL ";
char *motorID = "VME44 ver 2.16-4E";

/* If entries are the same in the first n characters, the longer name
 * MUST come first.
 */
itemConfig motorConfig[] = {
    {"axis",            CONF_INT,               1, OFFSETM(intAxis) },
    {"backlash",        CONF_INT,               1, OFFSETM(backlash) },
    {"backoff",         CONF_MOTION,            1, OFFSETM(backOff) },
    {"controller",      CONF_INT,               1, OFFSETM(controller) },
    {"enable",          CONF_BIT,               1, OFFSETM(enable) },
    {"fault",           CONF_BIT,               1, OFFSETM(fault) },
    {"fulltravel",      CONF_INT,               1, OFFSETM(fullTravel) },
    {"homelevel",       CONF_CHAR,              1, OFFSETM(homeLevel) },
    {"home",            CONF_SWITCH,            1, OFFSETM(home) },
    {"initstring",      CONF_APPEND_STR, MOTOR_CMD_LEN, OFFSETM(initString) },
    {"isenabled",       CONF_BIT,               1, OFFSETM(isEnabled) },
    {"name",            CONF_STRING, MOTOR_ID_LEN, OFFSETM(name) },
    {"neglimit",        CONF_SWITCH,            1, OFFSETM(negLimit) },
    {"parkposition",    CONF_INT,               1, OFFSETM(parkPosition) },
    {"poslimit",        CONF_SWITCH,            1, OFFSETM(posLimit) },
    {"probeh",          CONF_MOTION,            1, OFFSETM(probeH) },
    {"probe",           CONF_MOTION,            1, OFFSETM(probe) },
    {"reset",           CONF_BIT,               1, OFFSETM(reset) },
    {"seek",            CONF_MOTION,            1, OFFSETM(seek) },
    {"type",            CONF_TYPE,              1, OFFSETM(type) },
    {NULL, 0, 0 }
};


/************
 * Mechanisms
 ************/

mechDescriptor mechanism[NUM_MECH] = {
    { "cover",          fsCover},
    { "filterwheel1",   fsFW1},
    { "filterwheel2",   fsFW2},
    { "slit",           fsSlit},
    { "decker",         fsDecker},
    { "acquisition",    fsAcq},
    { "cross-dispersion", fsXdisp},
    { "grating",        fsGrating},
    { "camera",         fsCamera},
    { "focus",          fsFocus},
};

compHead mechHead[NUM_MECH];

/* zeropoint is a special case for the focus mechanism only. */
itemConfig mechConfig[] = {
    {"name",            CONF_STRING, ITEM_ID_LEN,  OFFSETMECH(name) },
    {"position",        CONF_POSITION,          1, 0 },
    {"zeropoint",       CONF_FOCUS,             1, 0 },
    {NULL, 0, 0 }
};

/* The filter pairs list */
compHead filterHead;

/* Grating Information */
gdata gratingData[NUM_GRATINGS];
/* For header information, see SAD section below */

/* If the composite FILTER is used to request both FW1 and FW2, we
 * need to note that somewhere.
 */
char reqFilterPosition[ITEM_ID_LEN];

itemConfig gratConfig[] = {
    {"constant",        CONF_DOUBLE,    1, OFFSETGRAT(A) },
    {"zero",            CONF_INT,       1, OFFSETGRAT(zeroPt) },
    {"bounds",          CONF_GBOUNDS,   1, OFFSETGRAT(orderLimits[0]) },
    {NULL, 0, 0}
};


/* Focus */
focusType focusZeroPoint;


/* TEMPERATURES */
tempDescriptor temperatures[NUM_TEMPS];
double tempCoeff[NUM_TEMP_COEFF] = {
     471.3507,          /* Coeff. 'a', constant term */
    -581.5675,
     558.4024,
    -786.8676,
     628.3720,
    -219.7995           /* Coeff. 'f', fifth order term */
};
    
itemConfig tempConfig[] = {
    {"name",            CONF_STRING, ITEM_ID_LEN,  OFFSETTEMP(name) },
    {"offset",          CONF_DOUBLE,           1,  OFFSETTEMP(offset) },
    {NULL, 0, 0 }
};


/* DIGITAL I/O */
/*  Use only first 11 chars since a different board might have different ID
 *  Full ID is  char *xycomID = "VMEIDXYC240    1 11 ";
 */
char *xycomID = "VMEIDXYC240";

#define INPUT_DONT_CARE     0

/* Three input ports (0-3) and 5 output ports (4-7) plus the flag ouput
 * register (8) */
int xycomOutputs = 0x1F0;

/* Which ports are outputs must match the bit pattern in xycomOutputs;
 * the initial value of the bits can be set here or in a configuration file.
 *  (The output bits are set here according to wiring diagram and the
 *   desire to have the default state be: not enabled, reset not asserted,
 *   alternative home switch not selected.)
 */
unsigned char xycomOutputInit[XYCOM_NPORTS] = {
/* port vv  \    i/o  bit >>  7      6      5      4      3     2     1   0  */
/* 0  0x01 */ INPUT_DONT_CARE,
/* 1  0x02 */ INPUT_DONT_CARE,
/* 2  0x04 */ INPUT_DONT_CARE,
/* 3  0x08 */ INPUT_DONT_CARE,
/* 4  0x10 */               0x80 | 0x00 | 0x00 | 0x10 | 0x0 | 0x0 | 0x2 | 0,
/* 5  0x20 */               0x80 | 0x00 | 0x00 | 0x10 | 0x0 | 0x0 | 0x2 | 0,
/* 6  0x40 */               0x80 | 0x00 | 0x00 | 0x10 | 0x0 | 0x0 | 0x2 | 0,
/* 7  0x80 */               0x80 | 0x00 | 0x00 | 0x10 | 0x0 | 0x0 | 0x2 | 0,
/* 8 0x100 */               0x00 | 0x00 | 0x00 | 0x00 | 0x0 | 0x0 | 0x0 | 0,
};

ioBit cryoSwitches[3];

itemConfig cryoConfig[] = {
    {"select",      CONF_BIT,     1,  OFFSETCRYO(COMPUTER_MANUAL) },
    {"manual",      CONF_BIT,     1,  OFFSETCRYO(ON_OFF_SWITCH) },
    {"computer",    CONF_BIT,     1,  OFFSETCRYO(COMPUTER_CONTROL) },
    {NULL, 0, 0 }
};


/*****************
 *   SAD Data
 *
 *   Variables declared in .../CC/sys/global/global.c
 *   Listed here for reference only
 *   NOT declared here hence the "ifdef DO_NOT_DEFINE"
 *
 *****************/

#ifdef DO_NOT_DEFINE
/* Mechanism information */
long mechDatumed[NUM_MECH];
long mechParked[NUM_MECH];
long mechEng[NUM_MECH];
long mechParkPos[NUM_MECH];
char mechPos[NUM_MECH][EPICS_LEN];
char mechName[NUM_MECH][EPICS_LEN];
char mechHealth[NUM_MECH][EPICS_LEN];
char mechState[NUM_MECH][EPICS_LEN];
/* Cryo Head Control */
long cryoSelectSw;
long cryoOnOffSw;
long cryoCpuControl;
/* Pressure sensing */
double senTorrIg;
double senTorrCcg;
double senTorrTc1;
double senTorrTc2;
/* Grating info */
double gratingWavelength;
double gratingAngle;
int gratingOrder;
int gratingStep;
double gratingEnd[2];
double gratingResolution;
#endif

/*****************
 * End of SAD Variables
 *****************/


char gnirsErrorMessage[GNIRS_MESSAGE_LENGTH];
long gnirsDebugLevel = 1;        /* Current debugging level */
 /*            0 = NOLOG
  *                No debugging or logging.
  *                Errors and warnings only.
  *            1 = NONE (default)
  *                Logging but no debugging.
  *                Errors and warnings, plus important
  *                log messages.
  *            2 = MIN
  *                Minimum debugging.
  *                Errors, warnings, log messages, plus
  *                supplemental information.
  *            3 = FULL
  *                Full debugging. All messages.
  */
