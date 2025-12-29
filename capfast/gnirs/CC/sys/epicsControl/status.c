
#include <vxWorks.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ioLib.h>  /* ioctl */
#include "ccDefines.h"
#include "status.h"

/* Used to mark nirs:dc:selectWcs when camera moves*/
#define CAMERA_MOTOR_STATUS_INDEX 110  

extern char *tempNames[NUM_TEMPS];
static EpicsStatus epicsStatus[NUM_STATUS];
extern char *dbTop;
extern char *sadTop;

void initStatusNames();
STATUS putEpics (char* name, void *val,void **tmp);

long markSelectWcs()
{
	long mywcsstatus;
	char mywcsname[80];
	long mywcsmark=1;
	void *pval;
        void *ch=NULL;

	pval = &mywcsmark;
	sprintf (mywcsname, "%s%s.PROC",dbTop,"camerawcsMark");
	/*printf("seting wcsmark...record %s\n", mywcsname);*/
	mywcsstatus = putEpics(mywcsname, pval, &ch);
	/* printf("...done setting wcsmark\n"); */
	return mywcsstatus;
}

int statusTask(int a1,int a2,int a3,int a4,int a5,int a6,int a7,int a8,int a9,int a10)
{
	int status;
	statusMsg msg;
	void *pval;
	EpicsStatus *statVal;

	statusQ = msgQCreate(300,sizeof(statusMsg),MSG_Q_FIFO);
	if(statusQ)
		printf("statusQ created successfully");
	else
	{

		printf("exiting status task\n");
		exit(1);
	}
	initStatusNames();
	while (1)
	{
		/*wait for value from lower level software*/
		if(msgQReceive(statusQ,(char *)&msg,sizeof(statusMsg),WAIT_FOREVER) == ERROR)
		   printf("status.c:  error in msgQReceive\n");

	
		statVal = &epicsStatus[msg.index];
		/*if(msg.index == INITCCSTATUS)*/
		/*if(msg.index == 110) ------note: CAMERA MOTOR is index 110*/
		  /* printf("name = %s, val = %s\n",statVal->name,msg.val.s); */
	   	switch (statVal->type)
		{
		  case EPLONG:
			pval = &msg.val.l;
			
			break;
		  case EPDOUBLE:
			pval = &msg.val.d;
			break;
		  case EPSTRING:
			pval = msg.val.s;
		   
		}
		if((strlen(statVal->name)>0)&&(strcmp (statVal->name,"END") != 0))
		{
			status = putEpics(statVal->name,pval,&statVal->id);
			if(status != OK)
				printf("putEpics error:  name = %s, index = %d\n",statVal->name,msg.index);
		}

		/* Used to mark nirs:dc:selectWcs when camera moves*/
#if 0
		if( (msg.index == CAMERA_MOTOR_STATUS_INDEX) && (strncmp(msg.val.s,"Moving", 6)==0) ) {
		   int setwcsstatus = -1;
		   setwcsstatus = markSelectWcs();
		   /*printf("setwcs status = %d\n", setwcsstatus);*/
		}
#endif
	}
	
}

void initStatusNames()
{
	int i;
	char *mechNames[10] =
	{{"cover"},{"fw1"},{"fw2"},{"slit"},{"decker"},{"acq"},{"xdisp"},{"grating"},{"camera"},{"focus"}};

	/*initialize id to null*/
	for (i=0; i<NUM_STATUS; i++)
		epicsStatus[i].id = NULL;


	sprintf (epicsStatus[CRYOSELECTSW].name,"%sC_M_SW",dbTop);
	epicsStatus[CRYOSELECTSW].type = EPLONG;
	sprintf (epicsStatus[CRYOONOFFSW].name,"%sM_SW",dbTop);
	epicsStatus[CRYOONOFFSW].type = EPLONG;
	sprintf (epicsStatus[CRYOCPUCONTROL].name,"%sC_SW",dbTop);
	epicsStatus[CRYOCPUCONTROL].type = EPLONG;
	sprintf (epicsStatus[HEALTHCC].name,"%shealth",sadTop);
	epicsStatus[HEALTHCC].type = EPSTRING;
	sprintf (epicsStatus[CCPRESSUREHEALTH].name,"%spressureHealth",sadTop);
	epicsStatus[CCPRESSUREHEALTH].type = EPSTRING;
	sprintf (epicsStatus[CCTEMPHEALTH].name,"%stempHealth",sadTop);
	epicsStatus[CCTEMPHEALTH].type = EPSTRING;
	sprintf (epicsStatus[DATUMEDCC].name,"%sdatumed",sadTop);
	epicsStatus[DATUMEDCC].type = EPLONG;
	sprintf (epicsStatus[PARKEDCC].name,"%sparked",sadTop);
	epicsStatus[PARKEDCC].type = EPLONG;
	sprintf (epicsStatus[INITCCSTATUS].name,"%sstate",sadTop);
	epicsStatus[INITCCSTATUS].type = EPSTRING;
	sprintf (epicsStatus[GRATINGORDER].name,"%sgratingOrder",sadTop);
	epicsStatus[GRATINGORDER].type = EPDOUBLE;
	sprintf (epicsStatus[GRATINGWVLENGTH].name,"%sgratingWvlength",sadTop);
	epicsStatus[GRATINGWVLENGTH].type = EPDOUBLE;
	sprintf (epicsStatus[GRATINGANGLE].name,"%sgratingTilt",sadTop);
	epicsStatus[GRATINGANGLE].type = EPDOUBLE;

	sprintf (epicsStatus[PRESSURETC1].name,"%spressureTC1",sadTop);
	epicsStatus[PRESSURETC1].type = EPDOUBLE;
	sprintf (epicsStatus[PRESSURETC2].name,"%spressureTC2",sadTop);
	epicsStatus[PRESSURETC2].type = EPDOUBLE;
	sprintf (epicsStatus[PRESSUREIG].name,"%spressureIG",sadTop);
	epicsStatus[PRESSUREIG].type = EPDOUBLE;

	for (i=0;i<10;i++)
	{
		sprintf (epicsStatus[MECHPARKED+i].name,"%s%sParked",sadTop,mechNames[i]);
		epicsStatus[MECHPARKED+i].type = EPLONG;
		sprintf (epicsStatus[MECHDATUMED+i].name,"%s%sDatumed",sadTop,mechNames[i]);
	/* 	printf("datumed = %s\n",epicsStatus[MECHDATUMED+i].name); */				 
		epicsStatus[MECHDATUMED+i].type = EPLONG;

		sprintf (epicsStatus[MECHENG+i].name,"%s%sEng",sadTop,mechNames[i]);
		epicsStatus[MECHENG+i].type = EPLONG;

		sprintf (epicsStatus[MECHNLIM+i].name,"%s%snLim",sadTop,mechNames[i]);
		epicsStatus[MECHNLIM+i].type = EPLONG;

		sprintf (epicsStatus[MECHPLIM+i].name,"%s%spLim",sadTop,mechNames[i]);
		epicsStatus[MECHPLIM+i].type = EPLONG;

		sprintf (epicsStatus[MECHHOME+i].name,"%s%sHome",sadTop,mechNames[i]);
		epicsStatus[MECHHOME+i].type = EPLONG;

		sprintf (epicsStatus[MECHFAULT+i].name,"%s%sFault",sadTop,mechNames[i]);
		epicsStatus[MECHFAULT+i].type = EPLONG;

		sprintf (epicsStatus[MECHOT+i].name,"%s%sOT",sadTop,mechNames[i]);
		epicsStatus[MECHOT+i].type = EPLONG;

		sprintf (epicsStatus[MECHPARKPOS+i].name,"%s%sParkPos",sadTop,mechNames[i]);
		epicsStatus[MECHPARKPOS+i].type = EPLONG;

		sprintf (epicsStatus[MECHNAME+i].name,"%s%sName",sadTop,mechNames[i]);
		epicsStatus[MECHNAME+i].type = EPSTRING;

		sprintf (epicsStatus[MECHPOS+i].name,"%s%sPosition",sadTop,mechNames[i]);
		epicsStatus[MECHPOS+i].type = EPSTRING;
		
		sprintf (epicsStatus[MECHHEALTH+i].name,"%s%sHealth",sadTop,mechNames[i]);
		epicsStatus[MECHHEALTH+i].type = EPSTRING;

		sprintf (epicsStatus[MECHSTATE+i].name,"%s%sState",sadTop,mechNames[i]);
		epicsStatus[MECHSTATE+i].type = EPSTRING;
	}
	for (i=0;i<NUM_TEMPS;i++)
	{
		if((strlen(tempNames[i])>0)&&(strcmp(tempNames[i],"END") != 0))
		{
			sprintf (epicsStatus[TEMPERATURECC+i].name,"%s%s",sadTop,tempNames[i]);
/* 			printf("temp %d = %s\n",i,epicsStatus[TEMPERATURECC+i].name); */
			epicsStatus[TEMPERATURECC+i].type = EPDOUBLE;
		}
		else
		{
			/* 			printf("temp %d = NULL\n",i); */
			strcpy(epicsStatus[TEMPERATURECC+i].name,"");
		}
	}
}
#include <taskLib.h>
void startStatustask()
{

	taskSpawn ("tStatus",50,VX_FP_TASK,4000,statusTask, 0,0,0,0,0,0,0,0,0,0);
}
