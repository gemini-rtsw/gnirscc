static struct
  {
      void *v;
      char *c;
  }
sccsid =
{
    &sccsid,
        "@(#)commands.c	1.10 10/22/03"
};
extern int noTempMon;
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
#include "gnirsCC.h"
#include "timeLib.h"
#include "math.h"
#include "status.h"


int sendStatus(long index,long type,void *p);
#define HW_REG32 volatile unsigned long

#define BIT_SET(p, d)      { __typeof__ (* (p)) __temp = (* (p));        \
                             * (p) = __temp | (d); }

#define BUS_RESET_REG_MV167   0xfff40060   /* Bus reset register for MVME167 */
#define BUS_RESET_BIT_MV167   0x01800000   /* Reset-Switch-Enable and
                                            * Bus-Reset bits
                                            */

void wfsBusReset(void)
{

    gnirsLogMessage(CICS_DB_ERROR, "wfsBusReset: BUS RESET - SYSTEM WILL REBOOT.\n");

    /* Brief pause to allow message to flush..   */
    taskDelay(4 * gnirsG.clockRate);

    /* ..then waggle the hardware bits            */
    BIT_SET((HW_REG32 *) BUS_RESET_REG_MV167, BUS_RESET_BIT_MV167);
}

int gnirsReboot()
{

    taskSpawn("suicide", 20, VX_NO_STACK_FILL, 2000, (FUNCPTR) wfsBusReset, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

    return VME_OK;
}

int gnirsTest()
{
    return testHardware();
}

int moveMech(int mech, char *message) {
    motorVars *m;
    int motor;

    motor = mechMotor(mech);
    if (motor < 0)
        return VME_ILLEGAL_MOTOR;
    m = motors[motor];
    if ((motor >= NUM_MOTORS) || !m) {
        mErrMsg(m, "moveMech: motor %d not allocated", motor);
        return VME_ILLEGAL_MOTOR;
    }

    moveOneMotor(motor);
    if (m->returnValue != VME_OK) {
        strncpy(message, m->errorMsg, EPICS_LEN);
        message[EPICS_LEN] = '\0';
    }
    return m->returnValue;
}

int setEngPos(int mech, int where) {
    char str[ITEM_ID_LEN];
    mechDescriptor *pMech;

    pMech = &mechanism[mech];
    /* The '**' string is looked for in moveOneMotor(motors.c); change it
     * in both places.
     */
    sprintf(str, "** ENG %d **", where);
    strncpy(pMech->reqPosition, str, ITEM_ID_LEN);
	strncpy(pMech->positionName, str, ITEM_ID_LEN);
    return setPositionReq(mech, where);
}
    
int setPositionReq(int mech, int where ) {
    motorVars *m;
    int motor;
    int normal, lim;

    motor = mechMotor(mech);
    if (motor < 0)
        return VME_ILLEGAL_MOTOR;
    m = motors[motor];
    if (!m)
        return VME_ILLEGAL_MOTOR;
    /* Make sure where is legal */
    switch (m->type) {
        case BINARY:
            normal = TRUE;
            if(where >= 0)    /* 0 defined as travel limit */
                lim = POS_LIM;
            else if (where <= m->negLimit.position)
                lim = NEG_LIM;
            else
			{
				printf("\n\n\nmoving to interior position %d, %d\n\n\n",where,m->negLimit.position);
                normal = FALSE;
			}
            if (normal) {
                m->reqOp = gotoLimit;
                m->reqArg = lim;
                return VME_OK;
            }
            break;
        case LINEAR:
            if ((where >= m->posLimit.position) ||
                    (where <= m->negLimit.position)) {
                mErrMsg(m, "Linear Pos %d too large", where); 
                return VME_ERROR;
            }
            break;
        case ROTARY:
            if (abs(where) >= m->fullTravel){
                mErrMsg(m, "Rotary Pos %d too large", where); 
                return VME_ERROR;
            }
    }
    m->reqArg = where;
    m->reqOp = position;
    return VME_OK;
}

int parkMech(int mech) {
    motorVars *m;
    int motor;
    mechDescriptor *pMech;

    motor = mechMotor(mech);
    if (motor < 0)
        return VME_ILLEGAL_MOTOR;
    m = motors[motor];
    if (!m)
        return VME_ILLEGAL_MOTOR;
    m->reqOp = park;
    pMech = &mechanism[motor];
    strncpy(pMech->reqPosition, "**park**", ITEM_ID_LEN);
	strncpy(pMech->positionName, "**park**", ITEM_ID_LEN);
    return VME_OK;
}

int datumMech(int mech) {
    motorVars *m;
    int motor;
    mechDescriptor *pMech;

    motor = mechMotor(mech);
    if (motor < 0)
        return VME_ILLEGAL_MOTOR;
    m = motors[motor];
    if (!m)
        return VME_ILLEGAL_MOTOR;
    m->reqOp = datum;
    pMech = &mechanism[motor];
    strcpy(pMech->reqPosition, "");
    strcpy(pMech->positionName, "");
    return VME_OK;
}

int mechMotor(int mech) {
    
    if ((mech < 0) || (mech >= NUM_MECH))
        return -1;
    return mechanism[mech].motor;
}

int valid(int who, char *where) {
    char output[200];
    mechDescriptor *pMech;
    mechNode *pNode;
    motorVars *m;
    filterNode *fNode;
    int lim, normal, rc;

    /* A bit of diagnostic help */
    if (who < 0) {
        printf("valid (who, where): who is encoded 0 - 9 as follows\n");
        printf("[0] cover, [1] fw1, [2] fw2, [3] slit, [4] decker\n[5] acq, [6] xdisp, [7] grating, [8] camera, [9] focus [10] filter\n");
        return VME_ERROR;
    }
    rc = VME_OK;
    switch (who) {
        /* Positions that are defined in the config file.  For FOCUS, it's
         * not likely that such predefined positions will be used.
         */
        case COVER:
        case FW1:
        case FW2:
        case SLIT:
        case DECKER:
        case ACQ:
        case XDISP:
        case GRATING:
        case CAMERA:
        case FOCUS:
            printf("checking position\n");
            pMech = &mechanism[who];
            pNode = (mechNode *)nodeLookup(where, pMech->pTable, FALSE);
            if (pNode == NULL) {
		sprintf(output," %s, %s Unknown position ", pMech->name,where);
                gnirsLogMessage(CICS_DB_ERROR4, output);
                return VME_ERROR;
            }

            /* Now have valid step position; record the position for
             * later generation of header information.
	     * reqStepPosition is the step count from zero, except for the
	     * grating where it is simply the grating number.
	     * reqPosition is a string containing the name of the position.
             */

            pMech->reqStepPosition = pNode->position;
            strncpy(pMech->reqPosition, where, ITEM_ID_LEN);
            strncpy(pMech->positionName, pNode->fName, ITEM_ID_LEN);
            pMech->focusShift = pNode->focusShift;
            /* Erase FILTER header info if we set either of FW1 or FW2 */
            switch (who) {
                case FW1:
                case FW2:
                    reqFilterPosition[0] = '\0';
                    break;
            }
            switch (who) {
                /* These two cases use limit switches to define position */
                case COVER:
                case ACQ:
                    m = motors[who];
                    if (!m) {
                        gnirsLogMessage (CICS_DB_ERROR4,
                                "Illegal motor (mech %d) in 'valid()'", who);
                        return VME_ILLEGAL_MOTOR;
                    }
                    normal = TRUE;
                    if(pNode->position >= 0)    /* 0 defined as travel limit */
                        lim = POS_LIM;
                    else if (pNode->position < m->negLimit.position)
                        lim = NEG_LIM;
                    else
                        normal = FALSE;
                    if (normal) {
                        m->reqOp = gotoLimit;
                        m->reqArg = lim;
                    } else
                       rc = setPositionReq(who, pNode->position);
                    break;
                case GRATING:
                    /* Nothing else to do; the grating is specified by
                     * the integer stored in reqStepPosition.
                     */
                    break;
                /* Everyone else uses steps to define position */
                default:
                    rc = setPositionReq(who, pNode->position);
                    break;
            }  
            break;
        /* Set two filter positions */
        case FILTER:
            fNode = (filterNode *) nodeLookup(where, &filterHead, FALSE);
            if (fNode == NULL) {
                gnirsLogMessage(CICS_DB_ERROR4, " 'filter' position unknown");
                return VME_ERROR;
            }
            /* Record settings for FILTER header */
            strncpy((char *)reqFilterPosition, where, ITEM_ID_LEN);

            /* We now know the combo, and since the configuration reading
             * routine verified that the combo points to defined filters,
             * we don't have to check for lookup errors.
             */
            pMech = &mechanism[FW1];
            pNode = (mechNode *)nodeLookup(fNode->f1, pMech->pTable, FALSE);
            pMech->reqStepPosition = pNode->position;
            pMech->focusShift = pNode->focusShift;
            strncpy(pMech->reqPosition, fNode->f1, ITEM_ID_LEN);
            rc = setPositionReq(who, pNode->position);
            if (rc != VME_OK)
                return rc;

            pMech = &mechanism[FW2];
            pNode = (mechNode *)nodeLookup(fNode->f2, pMech->pTable, FALSE);
            pMech->reqStepPosition = pNode->position;
            pMech->focusShift = pNode->focusShift;
            strncpy(pMech->reqPosition, fNode->f2, ITEM_ID_LEN);
            rc = setPositionReq(who, pNode->position);

            break;
        default:
            gnirsLogMessage (CICS_DB_ERROR4, "Illegal device %d in 'valid()'", who);
            return VME_ERROR;
            break;
    }
    return rc;
}

int setGratingTilt(double angle) 
{

    gratingOrder = 1;
	sendStatus(GRATINGORDER,EPDOUBLE,(void *)&gratingOrder);
    return setGratingRot(angle);
}

int setGratingRot(double theta) {
    mechDescriptor *pMech;
    motorVars *m;
    int pos;
    double x;
    gdata *gdp;

    pMech = &mechanism[GRATING];
    gdp = &gratingData[pMech->reqStepPosition];

	/* theta is relative to a particular grating*/
    if (theta >= 360.)
        theta -= 360.;
    if (theta <= -360.)
        theta += 360.;

    m = motors[GRATING];
	/* pos =percentage of full travel converted to steps*/
    pos = (theta / 360.) * m->fullTravel;

	/*theta is relative to the normal of the grating which is where the zeroPt is in 
	 the rotation of the grating turret for a particular grating*/
    gratingStep = pos + gdp->zeroPt;
	printf("gratingStep = (pos+zeroPoint) = %d pos = %d, zeroPoint=%d \n",
		gratingStep, pos, gdp->zeroPt);
    /* actual differs from requested due to truncation, so
     * invert to get the actual wavelength
     */
    theta = (360. * pos) / m->fullTravel;
    if ( gratingOrder == 0) 
	{
		gratingAngle = 0.0;
		sendStatus(GRATINGANGLE,EPDOUBLE,(void *)&gratingAngle);
		gratingWavelength = 0.0; /* undefined */
		sendStatus(GRATINGWVLENGTH,EPDOUBLE,(void *)&gratingWavelength);
    } 
	else 
	{
		gratingAngle = theta;
		sendStatus(GRATINGANGLE,EPDOUBLE,(void *)&gratingAngle);
		x = sin(gratingAngle * (3.14159/180.));
		if (x < 0.)
			x *= -1.;
		gratingWavelength = x * gdp->A / gratingOrder;
		sendStatus(GRATINGWVLENGTH, EPDOUBLE, (void *)&gratingWavelength);
	 

    }

    return setPositionReq(GRATING, gratingStep);
}

int validGratingWavelength(double wavelength, int order) {
    mechDescriptor *pMech;
    double x;
    gdata *gdp;

    pMech = &mechanism[GRATING];
    gdp = &gratingData[pMech->reqStepPosition];

    if (gdp->A == 0.0) {
        gnirsLogMessage(CICS_DB_ERROR4, "Cannot set wavelength for mirror");
        return VME_ERROR;
    }

    /* Validate reasonable lambda */
    x = (wavelength * order) / gdp->A;
    if ( abs(x) >= 1.) {
        gnirsLogMessage(CICS_DB_ERROR4, "Invalid lambda/order (%f/%d), gdp->a = %f",
                wavelength, order,gdp->A);
        return VME_ERROR;
    }
    /* Record the header info */
    /* Angle in degrees */
    if (order == 0) 
	{
		gratingAngle = 0.0;
		sendStatus(GRATINGANGLE,EPDOUBLE,(void *)&gratingAngle);
		gratingWavelength = 0.0; /* undefined */
		sendStatus(GRATINGWVLENGTH,EPDOUBLE,(void *)&gratingWavelength);
    } 
	else 
	{
		gratingAngle = asin(x) * (180. / 3.1415926);  /* converted to radians */
		sendStatus(GRATINGANGLE,EPDOUBLE,(void *)&gratingAngle);
		gratingWavelength = wavelength;
		sendStatus(GRATINGWVLENGTH,EPDOUBLE,(void *)&gratingWavelength);
    }
    gratingOrder = order;
		sendStatus(GRATINGORDER,EPDOUBLE,(void *)&gratingOrder);
    return setGratingRot(gratingAngle);
}

int validPreferredWavelength(double wavelength){
    mechDescriptor *pMech;
    gdata *gdp;
    double *dp, w;
    int order;

    w = wavelength;
    if (w < 0)
        w = -w;
    pMech = &mechanism[GRATING];
    gdp = &gratingData[pMech->reqStepPosition];
    if (gdp->A == 0.0) {
        gnirsLogMessage(CICS_DB_ERROR4, "Cannot set wavelength for mirror");
        return VME_ERROR;
    }
    dp = gdp->orderLimits;
    for (order = 0; order < NUM_GRAT_BOUNDS; order++) {
        if (w >= *dp)
            break;
        dp++;
    }
    if ((order == 0) || (order == NUM_GRAT_BOUNDS)) {
        gnirsLogMessage(CICS_DB_ERROR4, "wavelength %f outside bounds",
                    wavelength);
        return VME_ERROR;
    }
    return validGratingWavelength(wavelength, order);
}

/* Set simulation level */
int setSimulation(int level) {

    switch(level) {
        case NOSIM:
        case VSM:
        case FULLSIM:
        case FASTSIM:
            gnirsG.simulation = level;
            return VME_OK;
            break;
        default:
            break;
    }
    return VME_ERROR;
}

/* scan rate for updating slow SAD information, in seconds */
int slowScanRate = 15;

int tempScanTask(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10) 
{
    int rc;
    int i;
    double t;
    
    rc = initSenTorr();
    if(rc == VME_OK) {
	for (;;) {
	    /* do slow stuff */
	    
	    semTake(semScan, WAIT_FOREVER);
	    readSenTorr();
	    semGive(semScan);
	    if(noTempMon == 0)
	    {				
		for (i = 0; i < NUM_TEMPS; i++)
		{
		    semTake(semScan, WAIT_FOREVER);

		    /* temperatureCC[i] = rddewDegK(i); 	*/
		    /*set temperatureCC in epics		*/

		    t = rddewDegK(i) ;
		    sendStatus(TEMPERATURECC + i, EPDOUBLE, &t);
		    
		    semGive(semScan);					
		}				
	    }	   
	    
	    taskDelay(gnirsG.clockRate*slowScanRate);    /* one second */
	    
	}
    }

    return VME_OK;      /* never returns */
}

int scanStatusTask(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10) 
{
long l;
	char s[20];
    int stat;
    int interval;
    int health, h;
    int i; 
/* 	double temps[NUM_TEMPS]; */
    BOOL parked, datumed;
    motorVars *m;

    /* Wait until system is (reasonably) stable */
    while (gnirsG.state != RUNNING)
      taskDelay(2 * gnirsG.clockRate);

    interval = 0;       /* Count number of seconds have waited */
    for(;;) 
	{
		/* Every second */
		semTake(semScan, WAIT_FOREVER);

	/* Calculate overall health */
		health = gnirsG.health;
		h = motorHealthTree();
		if (h > health) /* Good < warning < bad */
		{
			health = h;
		}
		if(ccTempHealth > health)
		{
			health = ccTempHealth;
		}
		if(ccPressureHealth > health)
		{
			health = ccPressureHealth;
		}
/* 		healthCC = health;	 */
		/*set mechState in epics*/
   
		switch (health)
		{
		  case GOOD:
			strcpy(s,"GOOD");
			break;
		  case WARNING:
			strcpy(s,"WARNING");
			break;
		  case BAD:
			strcpy(s,"BAD");
		}
		sendStatus(HEALTHCC, EPSTRING, s);
	
		
		parked = datumed = TRUE;

		for (i = 0; i < NUM_MOTORS-2; i++ ) 
		{
			m = motors[i];
			getMdStatus(i);
			tellPV(i);
		/* 	mechEng[i] = m->currPos; */	
			/*set eng position in epics*/
		
			sendStatus(MECHENG + i,EPLONG,&m->currPos);
		
			/* 			printf("currPos = %d\n",mechEng[i]); */
			stat = m->status;	 
/* 			mechHome[i] = stat & MH_HOME; */
			/*set mechHome in epics*/
			l = stat & MH_HOME;
			sendStatus(MECHHOME + i,EPLONG,&l);
		

			if(stat & MH_LIMIT)
			{ 
				if((stat & MH_DIR) == MH_MINUS)  /*negative limit set*/
				{
					/* 	mechpLim[i] = 0; */
					/*set mechnLim in epics*/
					l = 0;
					sendStatus(MECHPLIM + i,EPLONG,&l);
				
/* 					mechnLim[i] = 1; */
					/*set mechnLim in epics*/
					l = 1;
					sendStatus(MECHNLIM + i,EPLONG,&l);
				
				}
				else /*positive limit set*/
				{	
				/* 	mechnLim[i] = 0; */
					/*mechpLim[i] = 1; */	
					/*set mechnLim in epics*/
					l = 1;
					sendStatus(MECHPLIM + i,EPLONG,&l);
				
/* 					mechnLim[i] = 1; */
					/*set mechnLim in epics*/
					l = 0;
					sendStatus(MECHNLIM + i,EPLONG,&l);
				
				}
			}
			else /* neither limit set*/
			{
		
/* 				mechpLim[i] = 0; */
				/*set mechpLim in epics*/
					l = 0;
					sendStatus(MECHPLIM + i,EPLONG,&l);
				

					/*set mechnLim in epics*/
					l = 0;
					sendStatus(MECHNLIM + i,EPLONG,&l);
				
			}

			/* mechFault[i] = stat & MS_FAULT; */
			/*set mechnLim in epics*/
			l = stat & MS_FAULT;
			sendStatus(MECHFAULT + i,EPLONG,&l);
		
			/* 	mechOT[i] = stat & MS_OTRAVEL; */
			l = stat & MS_OTRAVEL;
			sendStatus(MECHOT + i,EPLONG,&l);

			m = motors[i];
			if (!m)
				continue;
			if (!m->parked)
				parked = FALSE;
			if (!m->datumed)
				datumed = FALSE;
		}

/* 		datumedCC = datumed ? 1 : 0; */
		/*set datumedCC in epics*/	
		l = datumed ? 1 : 0;
/* 		printf("datumed = %d %d\n",l,datumed); */
		sendStatus(DATUMEDCC, EPLONG, &l);
	
		/* 	parkedCC = parked ? 1 : 0 ; */
		/*set datumedCC in epics*/
		l = parked ? 1 : 0;
		sendStatus(PARKEDCC ,EPLONG,&l);
	/* 	initCCStatus = gnirsG.state; */
		/*set initCCStatus in epics*/
	
		switch(gnirsG.state)
		{
		  case BOOTING:
			strcpy(s,"BOOTING") ;  
			break;
			
		  case INITIALIZING:
			strcpy(s,"INITIALIZING") ; 
			break;
		  case RUNNING:
			strcpy(s,"RUNNING") ;
			break;
		  case CONFIGURING:
			strcpy(s,"CONFIGURING") ;
		}
		sendStatus(INITCCSTATUS , EPSTRING, s);
	
	
      /* Cryo head control switch.  This pretty much obviates
       * the need for interrupt handling for the computer/manual
       * switch (see tandp.c, digital.c), but looping tasks are
       * ugly so the interrupt code will stay.
       */
	/* 	cryoCpuControl = gnirsG.cryoCpuState;  */   /* 0 off, 1 on */	/*set cryoSeleceSw in epics*/
		sendStatus(CRYOCPUCONTROL, EPLONG, &gnirsG.cryoCpuState);
	
	/* 	cryoOnOffSw = isSet(cryoSwitches[ON_OFF_SWITCH]); */
		/*set cryoOnOffSw in epics*/
		l = isSet(cryoSwitches[ON_OFF_SWITCH]) ;
		sendStatus(CRYOONOFFSW, EPLONG, &l);
	
	/* 	cryoSelectSw = gnirsG.cryoSelect;  */  /* COMPUTER/MANUAL */
		/*set cryoSeleceSw in epics*/
		sendStatus(CRYOSELECTSW, EPLONG, &gnirsG.cryoSelect);
	

		/* Redo any once per reconfiguration calculations.  Since
       * the flag is set by moveOneMotor, it's possible that this
       * calculation could be done twice or so.  That's a waste
       * of cpu cycles but otherwise is not a problem.
       */
		if (gnirsG.newConfiguration) 
		{
			/* calculate grating extents */
			/* XXX fix me */
			gratingEnd[0] = 1.1;
			gratingEnd[1] = 2.2;
			/* and resolution at center */
			gratingResolution = .00056;
			gnirsG.newConfiguration = FALSE;
		}
		interval++;

	/* 	if (( interval % slowScanRate) == 0)  */
/* 		{ */
 			/* do slow stuff */ 
			
/* 			if(noTempMon == 0) */
/* 			{				 */
/* 				for (i = 0; i < NUM_TEMPS; i++) */
/* 				{ */
					
/* 					temperatureCC[i] =  rddewDegK(i); 					 */
/* 				}				 */
/* 			}	    */
			
/* 			readSenTorr(); */
/* 			interval = 0; */
/* 		} */

		semGive(semScan);
		taskDelay(gnirsG.clockRate);    /* one second */
	}
    return VME_OK;      /* never returns */
}
void printState()
{
	printf("state = %d\n",gnirsG.state);
}
int sendStatus(long index,long type,void *p)
{
	statusMsg msg;

	msg.index = index;
	switch (type)
	{
	  case EPDOUBLE:
		msg.val.d = *(double *)p ;
		break;
	  case EPLONG:
		msg.val.l = *(long *)p ;
		break;
	  case EPSTRING:
		strncpy(msg.val.s,(char *)p,EPICS_LEN) ;
	}
	
/* 	if(index == DATUMEDCC) */
/* 		printf("datumed = %d %d\n",*(long *)p,msg.val.l); */
	if(statusQ)
		return msgQSend(statusQ, (char *)&msg, sizeof(statusMsg),NO_WAIT, 
					MSG_PRI_NORMAL);

	return ERROR;
}
