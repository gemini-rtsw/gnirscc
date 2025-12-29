

#include <netinet/in.h> 
#include "saverCommon.h"
#include "sockutil.h"
/* #define SEED */
#ifndef SEED

STATUS getEpics (char* name, void *val,int count,void **ch);
#endif
/* int INLOG = 1; */
/* int OUTLOG = 1; */
#define NUM_NAMES 42
#define STRING "string"
#define INT "int"
#define FLOAT "float"
static  char  *recordNames[NUM_NAMES][3] = 
{
	{FLOAT,"BENCH_TEMP","nirs:sad:cc:benchTempPoint"},/*1*/
	{STRING,"FW1_POS","nirs:sad:cc:fw1Position"},/*2*/
	{FLOAT,"FW1_ENG","nirs:sad:cc:fw1Eng"},/*3*/
	{STRING,"FW2_POS","nirs:sad:cc:fw2Position"},/*4*/
	{FLOAT,"FW2_ENG","nirs:sad:cc:fw2Eng"},/*5*/
	{STRING,"SLIT_POS","nirs:sad:cc:slitPosition"},/*6*/
	{FLOAT,"SLIT_ENG","nirs:sad:cc:slitEng"},/*7*/
	{STRING,"DECKER_POS","nirs:sad:cc:deckerPosition"},/*8*/
	{FLOAT,"DECKER_ENG","nirs:sad:cc:deckerEng"},/*9*/
	{STRING,"GRATING_POS","nirs:sad:cc:gratingPosition"},/*10*/
	{FLOAT,"GRATING_ENG","nirs:sad:cc:gratingEng"},/*11*/
	{FLOAT,"GRATING_WAVELENGTH","nirs:cc:gratingPosCad.C"},/*12*/
	{INT,"GRATING_ORDER","nirs:sad:cc:gratingOrder"},/*13*/
	{FLOAT,"GRATING_TILT","nirs:sad:cc:gratingTilt"},/*14*/
	{STRING,"PRISM_POS","nirs:sad:cc:xdispPosition"},/*15*/
	{FLOAT,"PRISM_ENG","nirs:sad:cc:xdispEng"},/*16*/
	{STRING,"ACQ_POS","nirs:sad:cc:acqPosition"},/*17*/
	{STRING,"COVER_POS","nirs:sad:cc:coverPosition"},/*18*/
	{STRING,"FOCUS_POS","nirs:sad:cc:focusPosition"},/*19*/
	{STRING,"FOCUS_MODE","nirs:cc:focusPosCad.VALB"},/*20*/
	{FLOAT,"FOCUS_ENG","nirs:sad:cc:focusEng"},/*21*/
	{STRING,"UCODE_PATH","nirs:dc:arSetup.A"},/*22*/
	{STRING,"UCODE_NAME","nirs:dc:arSetup.B"},/*23*/
	{INT,"DC_COADDS","nirs:dc:obsSetup.VALE"},/*24*/
	{INT,"DC_LNR","nirs:dc:obsSetup.VALD"},/*25*/
	{INT,"DC_DA","nirs:dc:obsSetup.VALC"},/*26*/
	{INT,"DC_ROWS","nirs:dc:curMaxRow.VAL"},/*27*/
	{STRING,"DC_PROC_MODE","nirs:dc:pMode"},/*28*/
	{INT,"DC_COLS","nirs:dc:curMaxCol.VAL"},/*29*/
	{FLOAT,"DC_INT_TIME","nirs:dc:integTime.VAL"},/*30*/
	{FLOAT,"DC_MIN_INT","nirs:dc:minInt.VAL"},/*31*/
	{INT,"DC_FRAMES_P_EXP","nirs:dc:ucFrmsPCycle.VAL"},/*32*/
	{STRING,"DC_COMMENT","nirs:dc:commStr.VAL"},/*33*/
	{STRING,"DC_IMAGE_TITLE","nirs:dc:titleStr.VAL"},/*34*/
	{FLOAT,"DC_PHOTON_TIME","nirs:dc:maxPhotonTime.VAL"},/*35*/
	{FLOAT,"DC_DESIRED_TEMP","nirs:dc:footRef"},/*36*/
	{FLOAT,"DC_TEMP","nirs:dc:footLGain"},/*37*/
	{FLOAT,"DC_TEMP_ERROR","nirs:dc:rdFootHGain"},/*38*/
	{FLOAT,"DC_FOOT_POWER","nirs:dc:footHtrFB"},/*39*/
	{FLOAT,"DC_MNT_POWER","nirs:dc:mntHtrFB"},/*40*/
	{STRING,"IMAGE_NAME","nirs:dc:imName.VAL"},/*41*/
	{STRING,"IMAGE_NUM","nirs:dc:imNum.VAL"},/*42*/


};
#define STR_LEN 10000
static char str[STR_LEN];
static void *id[NUM_NAMES];
int moving[4],limit[4];
void sleep (int a, int b)
{
        struct timespec to;
        struct timespec rm;
        to.tv_sec = a;
        to.tv_nsec = b;
        nanosleep(&to,&rm);
}

int getHeaderString()
{
	int i;
	int status = OK;
	void *val;
	double dval;
	long lval;
	char cval[80];

	for (i=0;i < NUM_NAMES && status == OK;i++)
	{


		if(strcmp (STRING,recordNames[i][0]) == 0)
			val = cval;
		if(strcmp (INT,recordNames[i][0]) == 0)
			val = &lval;
		if(strcmp (FLOAT,recordNames[i][0]) == 0)
			val = &dval;
#ifdef SEED
		val = 1;
#else
	    status = getEpics(recordNames[i][1],val,1,&id[i]);
#endif
	   
		if(strlen(str) > (STR_LEN -200))
			status = ERROR;
		else
		{
			if(strcmp (STRING,recordNames[i][0]) == 0)
			{
				sprintf(str,"%s %s %s\n",str,recordNames[i][1], (char *)val);
			}
			if(strcmp (INT,recordNames[i][0]) == 0)
			{
				sprintf(str,"%s %s %d\n",str,recordNames[i][1],*(long*)val);
			}
			if(strcmp (FLOAT,recordNames[i][0]) == 0)
			{
				sprintf(str,"%s %s %8.3f\n",str,recordNames[i][1],*(double*)val);
			}
		}
	}

	/*other values*/


	return status;
}

int saveHeaders( char *name)
{
	long status;
	char fileName[256];
	FILE *fd;
	sprintf(fileName,"/kiwi/staging/perm/%s.hdr",name);

	/*open file*/
	fd = fopen(fileName,"w");
	if(fd == NULL)
		printf("error opening file %s\n",fileName);
	else
	{
		status = getHeaderString();
		if(status == ERROR)
		{
			sprintf(str,"%s Headers not complete\n",str);
			printf("increase buffer size for header file\n");
		}
		
		/*write str to file*/
		if(fputs(str,fd) != strlen(str))
		{
			status = ERROR;
			printf("Error writing header string to file\n");
		}
		/*close file*/
		if(fclose (fd) != 0)
		{
			status = ERROR;
			printf("Error closing header file\n");
		}
	}
	return status;	
}
