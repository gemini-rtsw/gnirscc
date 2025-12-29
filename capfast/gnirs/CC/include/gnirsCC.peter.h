/*
 * Include file for GNIRS components controller
 *
 * @(#)gnirsCC.peter.h	1.2 11/12/01
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

#ifndef __GNIRSCCH
#define __GNIRSCCH

#include "gnirsCCDefines.h"
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


/* State and health */
enum {BOOTING = 0, INITIALIZING, RUNNING, CONFIGURING};
enum {GOOD = 0, WARNING, BAD};
/* Gnirs CAR values */
enum {G_DONE = 0, G_BUSY, G_ERROR};
enum {NOSIM = 0, VSM, FASTSIM, FULLSIM};
enum {LABVIEW = 0, MEDM};
enum {LVSERVER = 0, LVNFS};

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
} GNIRS_ST_GLOBALS;



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




enum {ROTARY = 0, LINEAR, BINARY};
enum {QUIESCENT = 0, MOVING, ABORTED};
/* Switches and Limits */
enum {HOME_SW = 0, POS_LIM, NEG_LIM, HARD_POS_LIM, HARD_NEG_LIM };
enum {NEGATIVE = 0, POSITIVE};
enum {SOFT = 0, HARD};

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
#define NUM_MECH    10

/* These "defines" must agree with the order of mechanisms in the mechanism
 * array, declared in globals.c
 *
 * All these are "real" mechansisms except "FILTER" which is the composite
 * of FW1 and FW2.
 */
enum {COVER = 0, FW1, FW2, SLIT, DECKER, ACQ, XDISP,
              GRATING, CAMERA, FOCUS, FILTER, SPARE1, SPARE2};
enum {DATUM=0,PARK,STEPS,POS};


/* Mechanism descriptor
 * The mechanisms in configuration file have a fixed name which is
 * given in the 'configName' member of this structure.
 */
typedef struct {
    char configName[ITEM_ID_LEN]; /* fixed ID for config. file section header */
    int (*focusShift)();          /* calculate focus shift */
    char name[MOTOR_ID_LEN];    /* name of mechanism, set by user */
    int motor;                  /* Motor with which it is associated */
    compHead *pTable;           /* pointer to linked list (which holds name
                                 * assoc. with position or filter pairs).
                                 */
    char reqPosition[ITEM_ID_LEN];  /* requested position, as a string */
    int reqStepPosition;        /* requested position in steps */
    /* Other items, such as function to validate */
} mechDescriptor;

/* NODE in position table */
typedef struct mechItemP {
    struct mechItemP *next;
    struct mechItemP *prev;
    char name[ITEM_ID_LEN];     /* name of position */
    int position;               /* steps from home */
} mechNode;



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

typedef struct {
    double A;         /* Equation constant (~ d cos(13.5) ) */
    double zeroPt;    /* position zero */
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


/*xy240 memory structure*/
struct dio_xy240{
    unsigned char boardID[0x80];        /* board ID and padding */
    unsigned char intInputs;            /* interrupt inputs */
    unsigned char status;               /* control status register*/
    unsigned char intMask;              /* interrupt masks */
    unsigned char intClear;             /* interrupt clear */
    unsigned char intPending;           /* interrupts pending */
    unsigned char intVector;            /* interrupt service routine*/
    unsigned char flagOutput;           /* flag outputs */
    unsigned char portDirection;        /* port direction*/
    unsigned char port[8];              /* i/o ports */
};

/* CryoHeads */
/* MANUAL/COMPUTER assume that the manual->computer transition is
 * high to low
 */


enum {COMPUTER_MANUAL = 0, ON_OFF_SWITCH, COMPUTER_CONTROL};

 /* The typedef makes it easy to write any line that uses it.
  * Pointer to a Function that returns a Void
  */
typedef void (*PFV)();

/*************************
 * TEMPERATURES
 *************************/


enum {T_NORMAL = 0, T_REF, T_COLD_HEAD };

typedef struct {
    char name[ITEM_ID_LEN];             /* name (location) of item sensed */
    short type;                         /* NORMAL, COLD_HEAD, etc. */
    double offset;                      /* offset in voltage */
} tempDescriptor;



/*************************
 * Status and Alarm Database Information
 *************************/

/* These arrays are in .../sys/global/global.c; not in the 'local' globals.c */
extern long mechDatumed[NUM_MECH];
extern long mechParked[NUM_MECH];
extern long mechEng[NUM_MECH];
extern long mechParkPos[NUM_MECH];
extern char mechName[NUM_MECH][EPICS_LEN];
extern char mechPos[NUM_MECH][EPICS_LEN];
extern char mechHealth[NUM_MECH][EPICS_LEN];



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



#ifndef NO_EXTERNS

/* globals */
extern SEM_ID semWd;

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

extern focusType focusZeroPoint;

extern tempDescriptor temperatures[];
extern itemConfig tempConfig[];
extern double tempCoeff[];

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
int checkLimit(int);
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
void healthString(char *, int);

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
int moveAll();
int doAll();
int moveMech(int, char *);
void moveOneMotor(int);
void mErrMsg(motorVars *, const char *, ...);
int motorSteps(int, int, int);
int mechMotor(int);
int valid(int, char *);
int setGratingRot(double);
int validGratingWavelength(double, int);
int validPreferredWavelength(double);
int setPositionReq(int, int);
int gnirsInit();
int setSimulation(int);
SAD_STATIC getSadStatic();
SAD_DYNAMIC getSadDynamic();
void resetHealth();
int scanStatusTask(int, int, int, int, int, int, int, int, int, int);

#endif 
