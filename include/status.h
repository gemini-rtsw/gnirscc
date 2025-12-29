#ifndef STATUSQ
#define STATUSQ
#include <msgQLib.h>
extern MSG_Q_ID statusQ;
typedef union VAL
{
  char s[80];
  double d;
  long l;
  long us;
}VAL;
#define EPDOUBLE 0
#define EPLONG 1
#define EPSTRING 2
typedef struct statusMsg
{
	int index;
	union VAL val;
	

}statusMsg;
typedef struct EpicsStatus
{
	void *id;
	long type; 
	char name[40];
}EpicsStatus;
extern int numStatus ;



#define NUM_STATUS 256
/* extern long mechDatumed[NUM_MECH]; */
#define MECHDATUMED 2

/* extern long mechParked[NUM_MECH]; */
#define MECHPARKED 12

/* extern long mechEng[NUM_MECH]; */
#define MECHENG 22

/* extern long mechnLim[NUM_MECH]; */
#define MECHNLIM 32

/* extern long mechpLim[NUM_MECH]; */
#define MECHPLIM 42

/* extern long mechHome[NUM_MECH]; */
#define MECHHOME 52

/* extern long mechFault[NUM_MECH]; */
#define MECHFAULT 62

/* extern long mechOT[NUM_MECH]; */
#define MECHOT 72

/* extern long mechParkPos[NUM_MECH]; */
#define MECHPARKPOS 82

/* extern long mechName[NUM_MECH]; */
#define MECHNAME 92

/* extern long mechPos[NUM_MECH]; */
#define MECHPOS 102

/* extern long mechHealth[NUM_MECH]; */
#define MECHHEALTH 112

/* extern long mechState[NUM_MECH] */
#define MECHSTATE 122

/* extern long cryoSelectSw; */
#define CRYOSELECTSW 133
/* extern long cryoOnOffSw; */
#define CRYOONOFFSW 134
/* extern long cryoCpuControl; */
#define CRYOCPUCONTROL 135
/* extern int healthCC; */
#define HEALTHCC 136
extern int ccPressureHealth;
#define CCPRESSUREHEALTH 137
extern int ccTempHealth;
#define CCTEMPHEALTH 138
/* extern int datumedCC; */
#define DATUMEDCC 139
/* extern int parkedCC; */
#define PARKEDCC 140
/* extern int initCCStatus; */
#define INITCCSTATUS 141
/* extern double temperatureCC[NUM_TEMPS]; */
#define TEMPERATURECC 150
#define GRATINGORDER 142
#define GRATINGWVLENGTH 143
#define GRATINGANGLE 144
#define PRESSURETC1 145
#define PRESSURETC2 146
#define PRESSUREIG 147
#endif
