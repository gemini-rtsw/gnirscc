/*
 * Include file for GNIRS components controller
 *
 * @(#)gnirsCC.h	1.11 09/24/03
 */
#include <semLib.h>
#include <stdio.h>
#include <msgQLib.h>
#include <sys/types.h>
#include <wdLib.h>
#include <objLib.h>
#include <rngLib.h>
#include <string.h>
#include <lstLib.h>
#include <time.h>
#include "ccDefines.h"
#ifndef __GNIRSCCH


#define __GNIRSCCH

typedef unsigned int uint32;

typedef enum statCodes
{
    VME_OK,                     /* No error detected - success   */
    VME_ERROR,                  /* General, unspecified error    */
    VME_MEMORY_ERROR,           /* Couldn't allocate needed mem  */
    VME_TIMEOUT,                /* command timed out */
    VME_DRIVE_FAULT,            /* Phytron motor driver fault */
    VME_CABLE_HARDW,            /* Cable or hardware problem */
    VME_ABORTED,                /* motion stopped by request */
    VME_ILLEGAL_MOTOR,          /* motor not init'ed */
    VME_SENTORR                 /* Problem with SenTorr pressure monitor */
}
VMESTATUS;


/* Gnirs CAR values */
enum {G_DONE = 0, G_BUSY, G_ERROR};
enum {NOSIM = 0, VSM, FASTSIM, FULLSIM};
enum {LABVIEW = 0, MEDM};
enum {LVSERVER = 0, LVNFS};
/* State and health */
enum {BOOTING = 0, INITIALIZING, RUNNING, CONFIGURING};

enum {GOOD = 0, WARNING, BAD};

/* Labview */

#define SVFLAG          0xBEADCAFE

enum {SVOPEN = 101,
    SVSEEK,
    SVTELL,
    SVWRITE,
    SVCLOSE,
    SVDELETE,
    SVEXIT
};

typedef struct
  {
      int magic;                /* flag */
      int opcode;               /* what to do */
      int count;                /* byte count to follow or return value */
  }
SVMSG;

#define SVMSG_LEN sizeof(SVMSG)
#define LV_MAX_MSG_LEN  80
#define ERR_MSG_LEN 80



/* generally global variables */
typedef struct {
    int initDone;               /* init done */
    int clockRate;
    int TStamp;                 /* index for time stamps */
    int state;                  /* current EPICS state */
    int health;                 /* current overall health */
    BOOL parked, datumed;       /* all parked, datumed? */
    int simulation;             /* simulation mode */
    int guiMode;                /* labview, MEDM CA, ... */
    int svFd;                   /* server Fd */
    int lvFileType;             /* via SERVER or FILE */
    int cryoSelect;             /* cpu or manual mode */
    int cryoCpuState;           /* commanded cyrohead on/off state */
    BOOL newConfiguration;      /* True if motor is about to move */
} GNIRS_ST_GLOBALS;

/*********
 * MOTORS
 *********/

/*
 * VME44 motor controller backplane addresses.
 *   board address is BASE + (n * INCR)
 */
#define NUM_CARDS   3   /* number of motor controller VME cards */
#define NUM_AXES    4   /* number of axes per board */

#define GNIRS_MD_ADDR_BASE  0xFFFFFC00
#define GNIRS_MD_ADDR_INCR  0x10

/* VME44 register offsets from base address */
#define MD_REG_DATA 1
#define MD_REG_DONE 3
#define MD_REG_CTRL 5
#define MD_REG_STAT 7
#define MD_REG_IVEC 9

/* VME44 status register bit masks */
#define MD_CMD_S    0x1
#define MD_INIT     0x2
#define MD_ENC_S    0x4
#define MD_OVRT     0x8
#define MD_DON_S    0x10
#define MD_IBF_S    0x20
#define MD_TBE_S    0x40
#define MD_IRQ_S    0x80

#define MD_DONE_E   0x10
#define MD_IBF_E    0x20
#define MD_TBE_E    0x40
#define MD_IRQ_E    0x80

#define CONTROL_X   0x18
#define CONTROL_Y   0x19
#define CR          0xD
#define LF          0xA

/* Motor controller interrupts */
#define GNIRS_MOTOR_INTERRUPT_LEVEL    5
#define GNIRS_MOTOR0_INT_NUM          240
#define GNIRS_MOTOR1_INT_NUM          (GNIRS_MOTOR0_INT_NUM + 1)
#define GNIRS_MOTOR2_INT_NUM          (GNIRS_MOTOR0_INT_NUM + 2)
/* Following is probably not going to be used */
#define GNIRS_MOTOR3_INT_NUM          (GNIRS_MOTOR0_INT_NUM + 3)

/* Motor Controllers */
#define MOTOR_MSG_LEN   48
#define MD_RING_SIZE 512
#define SHORT_RING 60

 /* status bits per motor
  * MH_ are for the hardware
  * MS_ are software status bits
  */
 
#define MH_PLUS     0x0   /*positive direction*/
#define MH_MINUS    0x8    /*negative direction*/
#define MH_DIR      0x8
#define MH_DONE     0x4
#define MH_LIMIT    0x2
#define MH_HOME     0x1
#define MH_STAT_MASK 0xF
#define MH_STAT_SIZE 4

#define MS_BUSY    0x010
#define MS_OTRAVEL 0x020        /* overtravel */
#define MS_FAULT   0x040        /* Could be power supply out of range */
#define MS_ERROR   0x100
#define MS_STUCK   0x300
#define MS_STALL   0x500   /* stalled or misconfigured */
#define MS_HARDW   0x900   /* Bad cable, failed power supply, etc. */
#define MS_STAT_MASK 0xFF0

#define DISABLED "disabled"
typedef struct {
    unsigned char pad0;
    unsigned char data;
    unsigned char pad1;
    unsigned char doneFlags;
    unsigned char pad2;
    unsigned char control;
    unsigned char pad3;
    unsigned char status;
    unsigned char pad4;
    unsigned char interruptVector;
} mdRegister;

typedef struct {
    int lfCount;
    int crCount;
    int countInc;
    int msgIndex;
    char message[MOTOR_MSG_LEN];
} mdMessage;

typedef struct {
    int status;             /* controller status */
    mdRegister *registers;
    SEM_ID semOut;          /* to make writes to card atomic */
    RING_ID outRing;
    RING_ID sentRing;
    SEM_ID semResponse;     /* whoever wants a response is waiting on this */
    SEM_ID waiting[NUM_AXES];
    SEM_ID semDone[NUM_AXES];
    char cmdErr[SHORT_RING + 1];    /* where ring buffer gets copied to */
    char *aliveID;          /* what card should return when asked 'WY' */
    mdMessage messageVars;
} GNIRS_ST_MD;


/********************
 * INDIVIDUAL MOTORS
 ********************/
#define NUM_MOTORS      (NUM_CARDS * NUM_AXES)
/* Name of axis: "slit", "grating", etc. */
#define MOTOR_ID_LEN    24
/* Max length of a command string (arbitrary) */
#define MOTOR_CMD_LEN   64
/* Number of microsteps per "real" step.  All positioning should be done
 * modulo this number. --- not needed XXX
 */
#define MICRO_STEPS      1
/* Factor by which to fudge backoff so can be sure we've gone far enough */
#define BKOFF_FUDGE      2
/* aux limit checked for in distance = offset + LIM_FUDGE */
#define LIM_FUDGE       40
#define TIMEOUT_MIN     150
#define MANUALFOCUS 0
#define AUTOFOCUS  1
#define AUTO "auto"
enum {ROTARY = 0, LINEAR, BINARY};
enum {QUIESCENT = 0, MOVING, ABORTED};
/* Switches and Limits */
enum {HOME_SW = 0, POS_LIM, NEG_LIM, HARD_POS_LIM, HARD_NEG_LIM };
enum {NEGATIVE = 0, POSITIVE, EITHER};

/* One of these for each different kind of positioning. */
typedef struct {
    int acceleration;
    int velocity;
} motion;

typedef struct {
    char port, bit;     /* port and bit for a given i/o bit */
} ioBit;

typedef struct {
    int type;           /* HOME_SW, POS_LIM, NEG_LIM */
    int position;       /* where switch is */
    int offset;         /* offset of second switch or hard limit */
                        /* --- following only needed by home switch */
    ioBit control;      /* where the aux control is */
    int useAlt;         /* Use the alternative switch ? */
} switchType;

/* Implements the variables of each instance of class 'motor' */
typedef struct {
    char name[MOTOR_ID_LEN];    /* name of motor function */
    int controller;             /* 0 through NUM_CARDS-1 */
    int intAxis;                /* 0 through NUM_AXES-1 */
    char charAxis;              /* X, Y, Z, or T */
    char homeLevel;             /* 'L' or 'H' (used in HH / HL)*/
    int status;                 /* status flags */
    int health;
    int type;                   /* What type of motor: linear, rotary ... */
    int fullTravel;             /* one revolution or length of linear travel */
    motion seek;                /* for standard positioning */
    motion backOff;             /* backing out for backlash removal */
    motion probe;               /* final seek: probe for position */
    motion probeH;              /* probe for home position */
    switchType home;            /* home switch */
    switchType posLimit;
    switchType negLimit;
    ioBit enable;
    ioBit reset;
    ioBit fault;
    ioBit isEnabled;
    int backlash;               /* distance to back off removing backlash */
    int currVel;                /* current velocity */
    int currPos;                /* current position */
    BOOL aborted;               /* stop the motion */
    BOOL datumed;
    BOOL parked;
    int parkPosition;           /* where to park */
    char initString[MOTOR_CMD_LEN]; /* initialization string */
    char cfgInitString[MOTOR_CMD_LEN]; /* from config file */
    char command[MOTOR_CMD_LEN];/* current command */
    int taskID;                 /* task ID for spawned task */
    SEM_ID semMoveMotor;        /* Motor motion requested */
    int (*reqOp)();             /* function to call to perform MoveMotor op */
    int reqArg;                 /* argument to requested operation */
    int returnValue;            /* VME_OK, VME_ERROR, etc. for req operation */
    char errorMsg[ERR_MSG_LEN];    /* storage for error message */
} motorVars;


#define ITEM_ID_LEN 32

/*************
 * Components
 ************/

/* Head of each linked list of component information */
typedef struct {
    struct compItem *head;
    struct compItem *tail;
    int count;
    int type;
} compHead;

enum {NODE_MECH = 0,
      NODE_FILTERS
};


/**************
 * MECHANISMS
 **************/

/* Number of "real" mechanisms */
/* #define NUM_MECH    10 */

/* These "defines" must agree with the order of mechanisms in the mechanism
 * array, declared in globals.c
 *
 * All these are "real" mechansisms except "FILTER" which is the composite
 * of FW1 and FW2.
 */
enum {COVER = 0, FW1, FW2, SLIT, DECKER, ACQ, XDISP, GRATING, CAMERA, FOCUS, FILTER, SPARE1, SPARE2};
enum {DATUM=0,PARK,STEPS,POS};



/* Mechanism descriptor
 * The mechanisms in configuration file have a fixed name which is
 * given in the 'configName' member of this structure.
 */
typedef struct {
    char configName[ITEM_ID_LEN]; /* fixed ID for config. file section header */
    int (*fShift)();             /* calculate focus shift */
    char name[MOTOR_ID_LEN];     /* name of mechanism, set by user */
    int motor;                   /* Motor with which it is associated */
    compHead *pTable;            /* pointer to linked list (which holds name
                                  * assoc. with position or filter pairs).
                                  */
    char reqPosition[ITEM_ID_LEN];  /* requested position, as a string */
  char positionName[ITEM_ID_LEN];  /* requested position, as a string */
    int reqStepPosition;         /* requested position in steps */
    int focusShift;              /* Shift associated with this position */
    /* Other items, such as function to validate */
} mechDescriptor;

/* NODE in position table */
typedef struct mechItemP {
    struct mechItemP *next;
    struct mechItemP *prev;
    char name[ITEM_ID_LEN];     /* name of position */
    char fName[ITEM_ID_LEN];     /* name of position for filters*/
    int position;               /* steps from home */
    int focusShift;              /* Shift associated with this position */
} mechNode;

/**********************
 * Binary Mechanisms
 **********************/
/* Used as a flag to signify the negative limit */
#define BINARY_LIMIT    -99999

/**********************
 * Filter Wheels
 **********************/
/* Filter index bias.  First wheel has indicies FIBIAS to 2*FIBIAS -1;
 * second wheel uses 2*FIBIAS to 3*FIBIAS-1
 */
#define FIBIAS          100
#define NFILT_IN_WHEEL  10  /* including the open/clear aperture */

/**********************
 * Filter Descriptions
 **********************/

/* NODE for composite filter description */
typedef struct filterItem {
    struct filterhItem *next;
    struct filterhItem *prev;
    char name[ITEM_ID_LEN];
    char f1[ITEM_ID_LEN];      /* name of filter */
    char f2[ITEM_ID_LEN];      /* name of filter in other wheel */
} filterNode;

/*********************
 * Gratings
 *********************/
#define NUM_GRATINGS    20
/* If you change NUM_GRAT_BOUNDS, change util.c */
#define NUM_GRAT_BOUNDS 9
typedef struct {
    double A;         /* Equation constant (~ d cos(13.5) ) */
    int zeroPt;       /* grating zero tilt in steps from home */
    double orderLimits[NUM_GRAT_BOUNDS];    /* Preferred order boundaries */
} gdata;

/*********************
 * Focus Mechanism
 *********************/
/* If you change focusType, change the CONF_ entry for zeropoint in
 * mechConfig in globals.c .
 */
typedef int focusType;

/*********************
 * CONFIGURATION FILE
 *********************/

/* Find the offset of a member of motorVars; 'motorV' is an array
 * of motorVars.
 */
#define OFFSETM(X)   ((char *)&(motorV[0].X) - (char *)&(motorV[0]))
/* Ditto, mechanisms */
#define OFFSETMECH(X)   ((char *)&(mechanism[0].X) - (char *)&(mechanism[0]))
/* Temperatures */
#define OFFSETTEMP(X)   ((char *)&(temperatures[0].X) - (char *)&(temperatures[0]))
/* Gratings */
#define OFFSETGRAT(X)   ((char *)&(gratingData[0].X) - (char *)&(gratingData[0]))


#define CONFIG_CHAR_LEN     80
#define CONFIG_DIR_LEN     160

/* Configuration types */
enum {CONF_MOTOR=0, CONF_MECHANISM, CONF_FILTER,
      CONF_TEMP, CONF_CRYO, CONF_GRATING};

enum {CONF_INT =0,
      CONF_CHAR,
      CONF_DOUBLE,
      CONF_STRING,
      CONF_TYPE,
      CONF_APPEND_STR,
      CONF_SWITCH,      /* Motor specific */
      CONF_MOTION,      /* Motor specific */
      CONF_POSITION,    /* Mechanism specific */
      CONF_GPARAM,      /* For gratings only */
      CONF_GBOUNDS,     /*   wavelength bounds */
      CONF_FOCUS,       /* Focus zero point */
      CONF_BIT          /* for ioBit structure */
};

typedef struct {
    char *item;             /* item name */
    int type;               /* type: INT, STRING, FLOAT */
    int maxLen;             /* max (string) length */
    int offset;             /* where is item in structure */
} itemConfig;


/*************************
 * XYCOM 240 DIGITAL I/O
 *************************/

#define XYCOM_BASE          0xFFFFD000
#define XYCOM_REGISTERS (XYCOM_BASE + 0x80)

#define GNIRS_XYCOM0_INT_NUM          (GNIRS_MOTOR0_INT_NUM + 4)

#define XYCOM_NPORTS    9
/* Flag register, output only, can be used as port 9 (ie, ==8 zero based) */
#define FLAG_PORT       8


#define XYCOM_SRESET         0x10
#define XYCOM_INTR_ENABLE    0x8
#define XYCOM_INTR_IS_PEND   0x4

/*xy240 memory structure*/
struct dio_xy240{
    unsigned char boardID[0x80];        /* board ID and padding */
    unsigned char intInputs;            /* interrupt inputs */
    unsigned char status;               /* control status register*/
    unsigned char intPending;           /* interrupts pending */
    unsigned char intMask;              /* interrupt masks */
    unsigned char intClear;             /* interrupt clear */
    unsigned char intVector;            /* interrupt service routine*/
    unsigned char flagOutput;           /* flag outputs */
    unsigned char portDirection;        /* port direction*/
    unsigned char port[8];              /* i/o ports */
};

/* CryoHeads */
/* MANUAL/COMPUTER assume that the manual->computer transition is
 * high to low
 */
#define MANUAL      1
#define COMPUTER    0
#define CRYO_ON     TRUE
#define CRYO_OFF    FALSE

enum {COMPUTER_MANUAL = 0, ON_OFF_SWITCH, COMPUTER_CONTROL};

 /* The typedef makes it easy to write any line that uses it.
  * Pointer to a Function that returns a Void
  */
typedef void (*PFV)();

#define OFFSETCRYO(X)   ((char *)&cryoSwitches[X] - (char *)&cryoSwitches[0])

/*************************
 * TEMPERATURES
 *************************/
/* address of primary temperature sensor board */
#define TEMP_P_ADDR   0xFFFF3300
/* address of Diagnostic temp board */
#define TEMP_D_ADDR   0xFFFF3700
/* Bad temperature reading, or parameter error (impossible temperature) */
#define TEMP_ERROR  ((double)10000.)

/* Number of Temperature boards and number of temperatures per board */
/* #define NUM_T_BOARDS    2 */
/* #define NUM_T_PER_BOARD   32 */
/* #define NUM_TEMPS     (NUM_T_PER_BOARD * NUM_T_BOARDS) */
/* #define NUM_REFTEMP    5 */

/* #define NUM_TEMP_COEFF  6 */

enum {T_NORMAL = 0, T_REF, T_COLD_HEAD };

typedef struct {
    char name[ITEM_ID_LEN];             /* name (location) of item sensed */
    short type;                         /* NORMAL, COLD_HEAD, etc. */
    double offset;                      /* offset in voltage */
} tempDescriptor;

/* Pressure */
#define SENTORR_LEN     24


/* EPICS support */

/* #define EPICS_LEN   40 */
/* #define GNIRS_MESSAGE_LENGTH    512 */

#define SWID    "GNIRS 1.0Beta"

/*************************
 * Status and Alarm Database Information
 *************************/

/* These arrays are in .../sys/global/ccGlobal.c; not in the 'local' globals.c */


typedef struct {
    char name[EPICS_LEN];
    char swID[EPICS_LEN];
} SAD_STATIC;

typedef struct {
    char state[EPICS_LEN];
    char health[EPICS_LEN];
    int heartBeat;
} SAD_DYNAMIC;

/* Logging */
/* Taken directly from cicsLib.h */

/*
 * These constants define the numerical codes used to store the current
 * debugging level. The levels are:
 *
 * NOLOG - only error messages;
 * NONE  - only error messages and log messages;
 * MIN   - minimal debugging (log messages plus a few others);
 * FULL  - full debugging (messages giving a full running comentary);
 */

#define CICS_DB_NOLOG 0
#define CICS_DB_NONE  1
#define CICS_DB_MIN   2
#define CICS_DB_FULL  3

/*
 * The above constants may also be used to define the level of each
 * message logged to the system. The following constants are used as
 * synonyms.
 */

#define CICS_DB_ERROR 0
#define CICS_DB_LOG   1

/* special flags marking errors that don't equate to failed controller health
 * 100 -- marks level above which special errors are to be found
 * 101 -- Can't open or parse configuration file
 * 102 -- command rejected as inappropriate, such as pausing
 *        when no exposure is in progress.
 * 103 -- Too many time stamps requested ... data ok, headers incomplete
 * 104 -- Inappropriate parameter or out of range.
 * 105 -- Problem with LABVIEW
 * *** These added by rjw
 */
#define CICS_DB_MARK   100
#define CICS_DB_ERROR1 101
#define CICS_DB_ERROR2 102
#define CICS_DB_ERROR3 103
#define CICS_DB_ERROR4 104
#define CICS_DB_ERROR5 105
 

#define CICS_DB_MARK    100
#define CICS_DB_FILE    101
#define CICS_DB_ERROR_TS    103

#ifndef NO_EXTERNS

/* globals */
extern SEM_ID semWd;
extern SEM_ID semScan;
extern int hwdbg;
extern int focusMode;
extern GNIRS_ST_GLOBALS gnirsG;
extern GNIRS_ST_MD motorDriver[];
extern motorVars motorV[];
extern motorVars *motors[];
extern char axisLetter[];

extern long gnirsDebugLevel;
extern char gnirsErrorMessage[];
extern char *motorInitString;
extern char *motorID;
extern itemConfig motorConfig[];

extern mechDescriptor mechanism[];
extern compHead mechHead[];
extern itemConfig mechConfig[];

extern compHead filterHead;
extern gdata gratingData[];
extern reqFilterPosition[];
extern itemConfig gratConfig[];
extern double gratingWavelength;
extern double gratingAngle;
extern int gratingOrder;
extern int gratingStep;
extern double gratingEnd[2];
extern double gratingResolution;

extern focusType focusZeroPoint;

extern tempDescriptor temperatures[];
extern itemConfig tempConfig[];
extern double tempCoeffLow[];
extern double tempCoeffHigh[];
extern double tempCutoff;

extern char *xycomID;
extern unsigned char xycomOutputInit[];
extern int xycomOutputs;
extern ioBit cryoSwitches[];
extern itemConfig cryoConfig[];

extern double senTorrIg;
extern double senTorrCcg;
extern double senTorrTc1;
extern double senTorrTc2;

extern int mdMax;       /* debugging aid */

#endif

/*
 * Function Prototypes
 */
void cryoHead(int);
void getClockSpeed(void);
int vmeInit(void);
int vmeBoardInit();
int testHardware();
int testOMSCards();

int sendToMotorDriver(int, char *, char *);
void getMdStatus(int);
int initMotors();
int setupMotor(int);
void calcTravel(int);
int motorTimeout(int, motion *);
int noWait(int, char *);
int waitFor(int, char *, int);
int tellPV(int);
int motorPos(int, int, int);
int goToPos(int, int, int);
int gotoLimit(int, int);
int checkLimit(int, int);
int motorOn(int);
int motorOff(int);
int motorOnOff(int, BOOL);
int motorCreep(int, int, int);
int position(int, int);
void finishMotorConfig(int);
int probeSwitch(int, int, int);
int findSwitch(int, int);
int setHome(int);
int findLimit(int, int);
int datum(int);
int motorHealthTree();
BOOL isBusy(int);
BOOL isInError(int);
void lvMotorDump();
void abortMotor(int);
int motorTask(int, int, int, int, int, int, int, int, int, int);
void checkParked(int);
int park(int);
int datumMech(int);
int parkMech(int);
void resetFault();

int isSet(ioBit);
int isClear(ioBit);
int setBit(ioBit);
int clearBit(ioBit);
int initXycom();
void xycomStart();
int testXYCOM();
int xycomBoRead( int, unsigned char, unsigned char *);
int xycomBoWrite( int, unsigned char, unsigned char);
int xycomBiRead( int, unsigned char, unsigned char *);
int initIOBits();
void xycomIntHndlr(int);
void xycomSetInterrupt(int, PFV);


void gnirsLogMessage(const long, const char *, ...);
int readConfig(char *);
int parseKeywordValue(char *, itemConfig *, char *, char *, char *);
void strnlc(char *, char *, int);
int findchar(char *, char, int);
void healthString(char *, int, int);

int mechDescInit();
NODE *makeNode(int);
NODE *nodeLookup(char *, compHead *, int);
void dumpTable(int);
int finishMechConfig();
void finishCryoConfig();

focusType fsCover();
focusType fsFW1();
focusType fsFW2();
focusType fsSlit();
focusType fsDecker();
focusType fsAcq();
focusType fsXdisp();
focusType fsGrating();
focusType fsCamera();
focusType fsFocus();

int filterDescInit();
int setFilters(char *, char *, char *);
int finishFiltersConfig();
void dumpMech(int);
void dumpFilters();

int initTempBoards();
int testTempBoards();
double rddewVolt(unsigned char);
double rddewDegC(unsigned char);
double rddewDegK(unsigned char);
int initSenTorr();
int testSenTorr();
int sendSenTorr(char *, char *, int);
int readCCG();
int readSenTorr();

void lvFileSever();
void lvFileString(char *);
int lvDump(int, int, int, int, int, int, int, int, int, int);

int gnirsReboot();
int gnirsTest();
int parkAll();
int datumAll();
int moveAll(int );
int doAll();
int moveMech(int, char *);
int moveOneMotor(int);
void mErrMsg(motorVars *, const char *, ...);
int motorSteps(int, int, int);
int mechMotor(int);
int valid(int, char *);
int setGratingRot(double);
int setGratingTilt(double);
int validGratingWavelength(double, int);
int validPreferredWavelength(double);
int setPositionReq(int, int);
int setEngPos(int, int);
int gnirsInit();
int setSimulation(int);
SAD_STATIC getSadStatic();
SAD_DYNAMIC getSadDynamic();
void resetHealth();
int scanStatusTask(int, int, int, int, int, int, int, int, int, int);
int tempScanTask(int, int, int, int, int, int, int, int, int, int);
void stopMech (int mechNum);
#endif 
