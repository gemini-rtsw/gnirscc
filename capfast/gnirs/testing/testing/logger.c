

#include <netinet/in.h> 
#include "saverCommon.h"
#include "sockutil.h"
#define SEED
#ifndef SEED

STATUS getEpics (char* name, void *val,int count,void **ch);
#endif
/* int INLOG = 1; */
/* int OUTLOG = 1; */
#define NUM_NAMES 80
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
int testtime(int a)
{
    return a;
}
int getVals(int time,int num, char *recordNames[NUM_NAMES],char *str)
{
	int i;
	int status = OK;
	double val;

	sprintf(str,"%8d",time);
	for (i=0;i < num && status == OK;i++)
	{
/* 	    printf("num = %d,i = %d\n",num,i); */
/*   	    printf("getVals name = %s\n",recordNames[i]); */
#ifdef SEED
		val = 1;
#else
	    status = getEpics(recordNames[i],&val,1,&id[i]);
#endif
	   
	    sprintf(str,"%s,%8.3f",str,val);
	}
/* 	printf("getVals %s done\n",str); */
	return status;
}
int parseNames(char *line, char *recordNames[NUM_NAMES])
{
	int i,len,totallen;
	i = 0;
	sleep(5,0);
	totallen = 0;
	while (totallen < strlen(line))
	{
		recordNames[i] = malloc((len = strcspn(line+totallen,","))+1);
		strncpy (recordNames[i],line+totallen,len);
		recordNames[i][len] = 0;
/* 		printf("\nname = %s\n",recordNames[i]); */
		i++;
		totallen = totallen+len +1;
	}

	return i;
}
int socket2(int sFdIn,int sfdOut,int time,int a3,int a4,int a5,int a6,int a7,int a8,int a9)
{  
  int OUTLOG = 1;
  int INLOG = 1;
  int i,num;
  
  int totalTime = 0;
  char line[NUM_NAMES*40];
  struct sockaddr_in clientAddrIn;
  ssize_t len;
  int clientFdIn;
  char str[NUM_NAMES*20];
  int status = OK;
  char *recordNames[NUM_NAMES];
  
  for (i=0;i<NUM_NAMES;i++)
    id[i] = NULL;
    printf("socket2 running %d, %d, %d\n",sFdIn,sfdOut,time);
    
    sprintf(line,"");
    bzero((char *)&clientAddrIn,sizeof(struct sockaddr_in));
  /*   while (sFdIn == ERROR) */
/*     { */
/* 		sFdIn = sockCreate(&portIn); */
/*     } */
    sockAccept(sFdIn, &clientAddrIn, &clientFdIn);
    while (INLOG == 1)
    {
		printf("waiting for command 2\n");
		sleep(2,0);
		len = recv(clientFdIn,line,4096,0);
		/*reply*/
		printf("sending reply\n");
		sockWrite(clientFdIn, "OK", strlen("OK"));
 
		line[len] = 0;
		printf("%s, size = %d\n",line,len);
		if(strcmp(line,"end") == 0)
		{
			OUTLOG = 0;
			INLOG = 0;
			break;
		}
		num = parseNames(line,recordNames);
/* 		printf("num = %d, line = %s\n",num,line); */
/* 		OUTLOG = 0; */
		while (OUTLOG == 1)
		  {
		
			/*    sprintf(str,"fRef =%.2f, fLGain =%.2f, fHGain =%.2f, fHtrFB =%.2f, mLGain =%f, mHtrFB %f\n",a,b,c,d,b1,d1); */
		/* 	sprintf(str,"%d,%8.3f,%8.3f,%8.3f,%8.3f,%8.3f,%8.3f,%8.3f,%8.3f,\n",totalTime,e,f,g,h,ii,j,k,l); */

		    status = getVals(totalTime,num,recordNames,str);
/* 			  printf(str); */
			if(status == OK)
			{
				sleep (2,0);
/* 				printf("write to socket\n"); */
				status = sockWrite(sfdOut, str, strlen(str));
				/*reply*/
/* 				printf("waiting for reply 1\n"); */
				sleep(2,0);
				len = recv(sfdOut,line,20,0);
/* 				printf("Reply = %s\n",line); */

			}
			else
			  {
			    printf("EPICS error\n");
			    OUTLOG = 1;
			    INLOG = 1;
			    break;
			  }
			
			if(status == ERROR)
			{
				printf("Error writing to socket\n exiting logger\n");
				OUTLOG = 0;
				INLOG = 0;

			}
			if(status != ERROR)
			{
			/* 	fprintf(stderr,"%d,%8.2f,%8.2f,%8.2f,%8.2f,%8.2f,%8.2f\n",totalTime,e,f,g,h,ii,j); */
				sleep(time,0);
				totalTime +=time;
			}
		}
		i = 0;
		while (i<num ) 
		{ 
			free (recordNames[i]);
 		
			i++;
 		} 
    }
    INLOG = 0;
	printf("killing socket connection\n");
	sockClose (&sfdOut);
	sockClose(&clientFdIn);  
	sleep (20,0);
    exit(0);
}

/*time is in seconds*/
long test(int time, int portIn,int portOut)
{
    static int sFdOut = ERROR, sFdIn = ERROR;
    int i = 0;
    struct sockaddr_in clientAddrOut;
    int clientFdOut;
    char name1[80];
    char name[80];

	/*set values if they weren't given in command line*/
	if(time == 0)
	   time = 300;
    if(portIn == 0)
    {
		portIn = 5548;
		portOut = 5547;
    }
    
    while (1)
    {
	/* 	sFdOut = ERROR; */
		printf("port = %d\n",portOut);
		while (sFdOut == ERROR)
		{
			sFdOut = sockCreate(&portOut);
		
		}	while (sFdIn == ERROR)
		{
		
			sFdIn = sockCreate(&portIn);
		}
		printf("sockCreate successful\n");
		sockAccept(sFdOut, &clientAddrOut, &clientFdOut);
		printf("got connection\n");
		sprintf(name,"tInSocket%d",i);
    printf("socket running %d, %d, %d\n",sFdIn,clientFdOut,time);
		taskSpawn (name,50,0,20000,socket2,sFdIn,clientFdOut,time,0,0,0,0,0,0,0);
		
	  
		sprintf(name1,"%s%d.dat",name1,i);
		
	
		
	/* 	if(status != ERROR) */
/* 		{ */
/* 			status = sockWrite(clientFdOut, "end", strlen("end")); */
/* 		} */
/* 		sockClose(&clientFdOut);  */
		/* 	sockClose(&clientFdIn);  */
		
		
		/* 	fclose(fileBuf); */
		i++;
    }
    return (OK);
}
