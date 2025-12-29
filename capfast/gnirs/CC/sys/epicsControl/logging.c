
#include        <string.h>
#include        <vxWorks.h>
#include        <stdlib.h>
#include        <ioLib.h>  /* ioctl */
#include        <types.h>
#include <tickLib.h> /* tickGet*/
#include        <sysLib.h> /* sysClkRateGet*/
#include        <taskLib.h>/* task delay, spawn*/
#include        <semLib.h>/* semaphores*/
#include        <errnoLib.h>
#include        <alarm.h>
#include  <cicsLib.h>
#include        <dbDefs.h>
#include        <dbAccess.h>
#include        <recSup.h>
#include        <devSup.h>
#include        <module_types.h>
#include        <aoRecord.h>
#include        <aiRecord.h>
#include "epCommon.h"
#include "epicsCAint.h"

/* log temperatures from console*/
SEM_ID killSem = NULL;
int sFd = ERROR;
int LOG;
#define NUM_NAMES 11
int logtemps(int time,int name1,int a3,int a4,int a5,int a6,int a7,int a8,int a9,int a10);
static char *epicsnames[NUM_NAMES] = {"shell","shield2Floating","shieldActive","benchTempPoint","benchUnderside","offner","collimator","cryo1_1s","cryo1_2s","cryo3_1s","thermalBusBar"};
/* time in minutes*/
/* name of log file*/
/*mode = a, or w */
long startLog(int time,char *name)
{
	int a3,a4,a5,a6,a7,a8,a9,a10;
	taskSpawn ("tlogTemps",50,VX_FP_TASK,4000,logtemps,time,(int)name, a3,a4,a5,a6,a7,a8,a9,a10);
	return OK;
}


int logtemps(int time,int namei,int a3,int a4,int a5,int a6,int a7,int a8,int a9,int a10)
{
  void  *chf=NULL;/*epics ca pointer for foot temp*/
    void  *chm=NULL;/*epics ca pointer for mount temp*/
    int i = 0;
	int status;
    static FILE *fileBuf = NULL;
    double a;
	char *name;
    char err[80],str[1000];  
    int totalTime = 0;
    LOG = 1;
	printf("convertTemp\n"); 

	if(killSem == NULL)
		killSem = semBCreate(SEM_Q_FIFO,SEM_EMPTY);
	totalTime = 0;
	LOG = 1;
	/* 	if (fileBuf == NULL) */
	name = (char *)namei;
	if ((fileBuf = fopen(name,"w")) == NULL)
	    
	{
		cicsLogMessage(0, "fopen temptest Failed\n");
		return ERROR;
	}


	/*print header*/
	fprintf(fileBuf,"Time");

	for(i=0;i<NUM_NAMES;i++)
	{
		fprintf(fileBuf,",%s",epicsnames[i]);	
		
	}
	/*detector temp names*/
	fprintf(fileBuf,",footTemp");  
	fprintf(fileBuf,",mountTemp");  

	fprintf(fileBuf,"\n");
/* 	write(file,str,strlen(str)); */
    
    
	while (LOG == 1)
	{
	
	
		sprintf(str,"%d",totalTime);
		for(i=0;i<NUM_NAMES;i++)
		{
		  getDbInfoT(sadTop, epicsnames[i], err, DBF_DOUBLE, &a);
		  sprintf(str,"%s,%8.2f",str,a);
		}	

		/*read detector temp*/
		getEpics("nirs:dc:footLGain", &a,1,&chf);
		sprintf(str,"%s,%8.2f",str,a);	
	/* 	printf("detector temp = %f\n",a); */

		getEpics("nirs:dc:mntLGain",  &a,1,&chm);
		sprintf(str,"%s,%8.2f",str,a);	

		getDbInfo("nirs:sad:cc:pressureIG",err,DBF_DOUBLE, &a);
		sprintf(str,"%s,%14.2e",str,a);
	/* 	printf("IG %s\n",str); */
		getDbInfo("nirs:sad:cc:pressureTC1",err,DBF_DOUBLE, &a);
		sprintf(str,"%s,%14.2e",str,a);
	/* 	printf("tc1 %s \n",str); */
		getDbInfo("nirs:sad:cc:pressureTC2",err,DBF_DOUBLE, &a);
		sprintf(str,"%s,%14.2e",str,a);	
	/* 	printf("tc2 %s \n",str); */

		sprintf(str,"%s\n",str);
/* 		write(file,str,strlen(str)); */
		fprintf(fileBuf,str);   
		printf(str);
		fflush(fileBuf);

		
			
	
		status = semTake(killSem,time*60*sysClkRateGet());
		if (status == OK)
		{
			printf("User interrupted logtemps task\n");
			LOG = 0;
			
		}
		else if (status == ERROR && errnoGet() != S_objLib_OBJ_TIMEOUT)
		{
			printf("logtemps error semaphore error\n");
			LOG = 0;
		}
		
	 /* 	if ((file = open(name, O_RDWR,0666)) == NULL)  */
				
/* 		{  */
/* 			cicsLogMessage(0, "fopen temptest Failed\n");  */
/* 			return ERROR;  */
/* 		}  */
		totalTime +=time;
	}
      
	fflush(fileBuf);
 
	fclose(fileBuf);
	i++;
    
    return (OK);
}

void killLog()
{
	semGive(killSem);
}
#if 0
void testlog(int time,char *readname)
{
    static FILE *readfile = NULL;
    static FILE *writefile = NULL;
	char writename[256];
	sprintf(writename,"%s.temp",name);
	if ((readfile = fopen(readname,"w")) == NULL)
	    
	{
		cicsLogMessage(0, "fopen temptest Failed\n");
		return ERROR;
	}
	sprintf(str,"%d",totalTime);
	for(i=0;i<NUM_NAMES;i++)
	{
		getDbInfoT(sadTop, epicsnames[i], err, DBF_DOUBLE, &a);
		sprintf(str,"%s,%8.2f",str,a);
	}
	sprintf(str,"%s\n",str);
	/* 		write(file,str,strlen(str)); */
	fprintf(readfile,str);   
	printf(str);
	fflush(readfile);
	fclose(readfile);
	
	while (LOG == 1)
	{
		status = semTake(killSem,time*60*sysClkRateGet());
		if (status == OK)
		{
			printf("User interrupted logtemps task\n");
			LOG = 0;
			
		}
		else if (status == ERROR && errnoGet() != S_objLib_OBJ_TIMEOUT)
		{
			printf("logtemps error semaphore error\n");
			LOG = 0;
		}
		if(LOG)
		{
			/*open read file*/
			if ((readfile = fopen(name,"r")) == NULL)
				
			{
				cicsLogMessage(0, "fopen temptest Failed\n");
				return ERROR;
			}
			if ((writefile = fopen(name,"w")) == NULL)
				
			{
				cicsLogMessage(0, "fopen temptest Failed\n");
				return ERROR;
			}
			/*open write file*/
			/*copy from readfile to writefile*/
			sprintf(str,"%d",totalTime);
			for(i=0;i<NUM_NAMES;i++)
			{
				getDbInfoT(sadTop, epicsnames[i], err, DBF_DOUBLE, &a);
				sprintf(str,"%s,%8.2f",str,a);
			}
			sprintf(str,"%s\n",str);
			/* 		write(file,str,strlen(str)); */
			fprintf(writefile,str);   
			printf(str);
			fflush(writefile);
			fclose(writefile);
			fclose(readfile);
			/*delete read file*/
			remove(readname);
			/*rename writefile to readfile name*/
			rename(writename,readname);
		}
		
		totalTime +=time;
	}
	
}
#endif
