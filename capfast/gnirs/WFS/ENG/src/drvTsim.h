/* $Id: drvTsim.h,v 1.2 2009/05/27 19:34:44 fkraemer Exp $ */

#if !defined(DRV_TSIM_H)
#define DRV_TSIM_H

/*
 * Note: "COOL" is left over from an earlier implementation attempt,
 * and has been replaced by motor.
 */

#define TSIM_NTC (3)    /* How many temperature controllers? */
#define TSIM_NCOOL (2)  /* How many cooling motors? */
#define TSIM_NMOTOR (2) /* How many cooling motors? */

typedef struct tsim_tsen_ {
	double temperature;         /* Current temperature */
} TsimTsen;

typedef struct tsim_tcon_ {
	double setPoint;            /* Temperature setpoint (C) */
	double temperature;         /* Current temperature */
	double heating;             /* Heater power */
} TsimTcon;

typedef struct tsim_motor_ {
	int speed;                /* Motor speed */
	int pos;
	int moving;
	int dir;
	int jog;
	int event;
	long (*callback)(void *);
	void *callbackArg;
	int dest;
} TsimMotor;

typedef struct tsim_values_ {
	SEM_ID sem;
	TsimTsen ts;
	TsimTcon tc[TSIM_NTC];
	TsimMotor motor[TSIM_NMOTOR];
} TsimValues;

/*
 * Temperature control functions
 */

long tsimRecInit(void *, void **);

long tsimMotorCallback(TsimMotor *);
long tsimMotorInit(void *, int, void **);
long tsimMotorJog(TsimMotor *);
long tsimMotorStart(TsimMotor *);
long tsimMotorStop(TsimMotor *);
long tsimMotorSetDest(TsimMotor *, int, int);
long tsimMotorSetDir(TsimMotor *, int);
long tsimMotorSetPos(TsimMotor *, int);
long tsimMotorSetSpeed(TsimMotor *, int);
void tsimMotorSetCallback(TsimMotor *, long (*)(void *), void *);

#endif /* DRV_TSIM_H */
