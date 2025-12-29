
static struct {
    void *v;
    char *c;
} rcsid = {
    &rcsid,
    "$Id: gnDQSocket.c,v 1.2 2009/05/27 19:32:18 fkraemer Exp $"
};


#define DEBUG
/*****************************************************************************
 * Copyright 1997 Association of Universities for Research in Astronomy, Inc. 
 * See the file COPYRIGHT for more details.
 *
 * Filename: 	
 * 	gnDQSocket.c
 *
 * Description:
 * 
 *
 * Function name(s)
 * 
 *
 * Dependencies
 * 	
 *
 * Orginial Author:
 *	Nick C. Buchholz
 *
 * History:
 *	11-Jun-1997: Created original version - ncb
 *
 ***************************************************************************/

#include <stdio.h>
#include <vxWorks.h>  
#include <ioLib.h>  
#include <epCommon.h>
#include <netinet/in.h> 
#include "sockutil.h"
#include "gnDCADefs.h"
#include "gnDQSocket.h"
#include <cicsLib.h>
int socketdebug;
int sd = 0;
extern int CalcWcs(int, int);
extern long getWcsParams();
extern void getDqBancomTime(char *);
extern void getDqBancomDate(char *);
STATUS getEpics (char* name, void *val,int count,void **tmp);
STATUS putEpics (char* name, void *val,void **tmp);
/*****************************************************************************
 * Function name:
 * 	gnDCASocketIntrfc
 *
 * Invocation:
 * 	status = gnDCASocketIntrfc(  );
 *
 * PARAMETERS:
 *
 *
 * FUNCTION VALUE:
 * 	int - Status value returned by function 
 *
 * PURPOSE:
 *     A server routine for handling message passing.
 *
 * DESCRIPTION:
 *    This function is run on the machine that is running the EPICS database.  
 *  It accepts a connection, reads the socket, processes the request, sends
 *  a reply back, and then closes the socket, and waits for another connection.
 *  It is currently asynchronous, but it would be simple to spawn a routine 
 *  after the connection to allow the main program to be continuously waiting 
 *  connections.
 *
 * EXTERNAL VARIABLES:
 * dqCont - loop control variable
 *
 * PRIOR REQUIREMENTS:
 *    EPICS must be up and running on the same machine that this is on.
 *
 * DEFICIENCIES:
 * 	None known
 *
 * HISTORY:
 * 	15-May-1998  Original version  P Ruckle
 *
 *****************************************************************************/

int gnDCASocketIntrfc2(int sFd,int a,int b,int c,int d,int e,int f,int g,int h,int i);
void gnDCASocketIntrfc()
{
    int clientFd = 0;
    int tid = 0;
    int i;
    char name[80];
    char buf[80];
    int port = 5301;
    struct sockaddr_in clientAddr;
    int sFd = ERROR;
#ifdef DEBUG
    int t1 = 1;
#endif
   
    sprintf(buf,"port = %d\n",port);
    while (sFd == ERROR)
    {
	sFd = sockCreate(&port);
    }  
  
    DPRINT(t1,"Sock create successful\n");
   

    
  
    while(1)
    {

	/*connect*/
	DPRINT( t1,"waiting for new connection\n");
	if(sockAccept(sFd, &clientAddr, &clientFd)!= ERROR)
	  {

	    DPRINT(t1,"starting task\n");
	    sprintf(name,"tDcaSocket%d",i++);
	    /*start server*/
	    
	    tid = taskSpawn(name,60,VX_FP_TASK,10000,gnDCASocketIntrfc2,clientFd,0,0,0,0,0,0,0,0,0);
	  }
    }
}
#define NUM_HEADER (14+55+6) /*same as NUM_HEADER in ppc*/
static void *id[NUM_HEADER];
int gnDCASocketIntrfc2(int clientFd,int a,int b,int c,int d,int e,int f,int g,int h,int i)
{
    char buf[80];
    int status = ERROR;
  
    void *pval;
    dcaMsgStruct msg;
    char errMess[80];
    short type;
    int run;
    int dqCont = 1;
#ifdef DEBUG
    int t1 = 1;
#endif
    int len;
  

    DPRINT(t1, "gnDCASocketIntrfc starting\n");
/* 	printf("\n\nmsg size = %d\n",sizeof(msg)); */
/* 	printf("\nop size = %d\n",sizeof(msg.op)); */
/* 	printf("\nid size = %d\n",sizeof(msg.id)); */
/* 	printf("\nstatus size = %d\n",sizeof(msg.status)); */
/* 	printf("\nname size = %d\n",sizeof(msg.name)); */
/* 	printf("\ntype size = %d\n",sizeof(msg.type)); */
/* 	printf("\nval size = %d\n",sizeof(msg.val)); */
    status = ERROR;
    /* stay in loop until we can create this socket*/
	for (i=0;i<NUM_HEADER;i++)
		id[i] = NULL;
    status = OK; 
    /* infinite loop, or loop until some other program changes dqCont*/
    while( dqCont)  
	{
		run  =OK;
		
		/* keep connection open until the other side closes it*/
		while(run == OK) 
		{
			len = recv(clientFd,(char *)&msg,sizeof(dcaMsgStruct),0);
			if(len !=sizeof(dcaMsgStruct) ) 
			{
				printf("sockRead returned error %d\n",len);
				run = ERROR;
				dqCont = 0;
			}
			else  
			{

			
				switch (msg.op)	 
				{
					
                                        /* update the wcs */
					DPRINT(t1,"gnDQSocket.c getWcsParams \n\n\n\n\n");
           				if (( msg.status = getWcsParams()) == OK) {
					/*calculate	 wcs information*/
					DPRINT(t1,"gnDQSocket.c WCS\n\n\n\n\n");
					CalcWcs(0,msg.val.l);
					DPRINT(t1,"calcwcs finished\n");
                                        }
					break;
				  case CAREAD: 
/* 					printf("op = %d,id = %d, name = %s, type = %d, LONG = %d, double = %d, string = %d\n",msg.op,msg.id,msg.name,msg.type,DCALONG,DCADOUBLE,DCASTRING); */
					switch (msg.type)
					{
					  case DCALONG:
						msg.val.l = 0;
						pval = &msg.val.l;
						break;
					  case DCADOUBLE:
				
						msg.val.d = 0;
						pval = &msg.val.d;
						break;
					  case DCASTRING:
				
						pval = msg.val.s;
						break;
					}
					msg.status = OK;
/* 					printf("name = %s\n",msg.name); */
					msg.status = getEpics(msg.name,pval,1,&id[msg.id]);
					
					break; 
				  case CAWRITE:
					switch (msg.type)
					{
					  case DCALONG:
						pval = &msg.val.l;
					   
						break;
					  case DCADOUBLE:
						pval = &msg.val.d;
						break;
					  case DCASTRING:
						pval = msg.val.s;
						break;
					}
					
					msg.status = putEpics(msg.name, pval, &id[msg.id]);
					break;
				  case DCAWCS:
					
					DPRINT(t1,"gnDQSocket.c WCS\n\n\n\n\n");
					CalcWcs(0,msg.val.l);
					DPRINT(t1,"calcwcs finished\n");
					printf("calcwcs finished in gnDQSocket.c line 235\n");
					break;
					/* reading an epics variable*/
				  case DCAREAD:
					switch (msg.type)
					{
					  case DCALONG:
						type = DBF_LONG; 
						status = getDbInfo(msg.name,errMess,type,&msg.val.l); 
						/*sprintf(buf,"read  %s = %d\n",msg.name,msg.val.l); */
						/*DPRINT(t1,buf); */
						if(status != OK)
							printf("error in dcaread %s %s %d\n",msg.name,
								   errMess,msg.val.l);
						
						break;
					  case DCADOUBLE:
						type = DBF_DOUBLE; 
						status = getDbInfo(msg.name, errMess,type,&msg.val.d); 
						/*sprintf(buf,"read  %s = %f\n",msg.name,msg.val.d);
						DPRINT(0,buf); */
						if(status != OK)
							printf("error in dcawrite %s %s %f\n",msg.name,
								   errMess,msg.val.d);
						
						break;
					  case DCASTRING:
						type = DBF_STRING; 
						status = getDbInfo(msg.name, errMess,type,msg.val.s); 
						/*sprintf(buf,"read  %s = %s\n",msg.name,msg.val.s);
						DPRINT(0,buf); */
						if(status != OK)
							printf("error in dcawrite %s %s %s\n",msg.name,
								   errMess,msg.val.s);
						
						break;
					}
					sprintf(buf,"done\n\n");
					DPRINT(0,buf);
					msg.status = status;
					break;
					/* writing to an epics variable*/
				  case DCAWRITE:
					DPRINT(0,buf);
					switch (msg.type) {
					  case DCAUSHORT:
						type = DBF_USHORT;
						printf("val = %d",msg.val.us);
						status = putDbInfo(msg.name, errMess,type,&msg.val.us);
						if(status != OK)
							printf("error in dcawrite %s %s %d\n",msg.name,
								   errMess,msg.val.us);
						break;
					  case DCALONG:
						type = DBF_LONG; 
						sprintf(buf,"write  %s = %d\n",msg.name,msg.val.l);
						DPRINT(0,buf);
						status = putDbInfo(msg.name, errMess,type,&msg.val.l);
						if(status != OK)
							printf("error in dcawrite %s %s %d\n",msg.name,
								   errMess,msg.val.l);
						
						break;
					  case DCADOUBLE:
						type = DBF_DOUBLE; 
						sprintf(buf,"write %s = %f\n",msg.name,msg.val.d);
						DPRINT(0,buf);
						status = putDbInfo(msg.name, errMess,type,&msg.val.d); 
						if(status != OK)
							printf("error in dcawrite %s %s %f\n",msg.name,
								   errMess,msg.val.d);
						
						break;
					  case DCASTRING:
						type = DBF_STRING; 
						sprintf(buf,"write  %s = %s\n",msg.name,msg.val.s);
						DPRINT(0,buf);
						printf(buf);
						status = putDbInfo(msg.name, errMess, type,msg.val.s); 
						if(status != OK)
							printf("error in dcawrite %s %s %s\n",msg.name,
								   errMess,msg.val.s);
						
						
						break;
					}
					
					msg.status = status;
					break;
				  case DCALOG:
					sprintf(buf,"log = %s\n",msg.name);
					DPRINT(0,buf);
					msg.status = OK;
					break;
				  case DCACLOSESOCKET:
					run = ERROR;
				  default:
					printf("message = %d\n",msg.op);
					
				}
				/* send back response*/
				
				status = sockWrite(clientFd, (char *)&msg, 
								   sizeof(dcaMsgStruct));
			} 
		}
		printf("closing socket\n");
		if(clientFd != 0)
		{
			sockClose(&clientFd); 
			clientFd = 0;
		}
    }
    
    return OK;
 
}
