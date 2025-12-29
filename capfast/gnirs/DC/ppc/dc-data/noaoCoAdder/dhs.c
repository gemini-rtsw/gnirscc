static struct
  {
      void *v;
      char *c;
  }
rcsid =
{
    &rcsid,
        "$Id: dhs.c,v 1.3 2011/08/18 20:59:33 gemvx Exp $"
};

/*****************************************************************************
 * Copyright 1999 Association of Universities for Research in Astronomy, Inc. 
 * See the file COPYRIGHT for more details.
 *
 * Filename:
 * 	gnStdSysInit.c
 *
 * Description:
 *      
 *
 * Routines:
 * 	dropDhs - Disconnect from the DHS.
 *	useFits - 
 * 	useDHS - 
 *	DhsConnect - 
 *	dhsGetCallback - 
 *	dhsErrorCallback - 
 *
 * Dependencies
 * 
 *	
 * Original Author:      
 * 
 *
 * History:	
 *	
 *
 ***************************************************************************/
#define DHS_SAVER
#include <vxWorks.h>
/* #include <vme.h> */
#include <taskLib.h>
#include <msgQLib.h>
#include <stat.h>
#include <nfsDrv.h>
#include <nfsLib.h>
#include <netinet/tcp.h>
#include <errnoLib.h>
/* #include "sdsu.h" */
#include <sockLib.h>
#include "sockutil.h"
#include <cicsLib.h>
#include <dhs.h>
#include "gnDCADefs.h"
#include "gnDCAVars.h"
#include "saver.h"
#include "string.h"
extern saverParams svrP;
void dhsGetCallback(DHS_CONNECT connect, DHS_STATUS errorNum,
                      DHS_ERR_LEVEL errorLev, char *msg, DHS_TAG tag,
                      void *userData);


int makeDhsFrame(int, int, int, int);

/*****************************************************************************
 * Function name:
 *	dropDhs
 *
 * Invocation:
 *	dropDhs();
 *
 * PARAMETERS:
 *	None
 *
 * FUNCTION VALUE:
 *	none
 *
 * PURPOSE:
 *	Disconnect from the DHS.
 *
 * DESCRIPTION:
 *	Disconnects current connection from dhs if already connected.
 *	kill dhs event loop.
 *	kill allother connections
 *
 * EXTERNAL VARIABLES:
 *	svrP - saver structure
 *
 * PRIOR REQUIREMENTS:
 *
 * DEFICIENCIES:
 * 	None known
 *
 * HISTORY:
 *
 *****************************************************************************/
/* Drop the connection to the dhs */
void dropDhs()
{
    DHS_STATUS status;
    dhsParams *dhs;

    dhs = &svrP.param.dhs;
  
    if (!dhs->initDone)
        return;
    status = DHS_S_SUCCESS;
    if (dhs->connect != DHS_CONNECT_NULL)
    {
        dhsDisconnect(dhs->connect, &status);
        dhs->connect = DHS_CONNECT_NULL;
    }
    status = DHS_S_SUCCESS;
    dhsEventLoopEnd(&status);
    (void) dhsExit(&status);
	
    dhs->initDone = FALSE;
    dhs->connected = FALSE;
	
    printf(" dhs->connected = %i\n",dhs->connected);
	status=0;
    if(gnPutEpicsT(dbSadTop, DHSCONNECTED ".VAL", DCALONG, &status)!= OK)
  		 printf ("dropDhs: failed to set DHSCONNECTED\n");
	
}
#if 0

/*****************************************************************************
 * Function name:
 *	useFits
 *
 * Invocation:
 * 	int status;
 *	status = useFits(port,host,ip);
 *
 * PARAMETERS:
 *	port - port number that fits server is using
 *	host - host that fits server is running on
 *	ip - ip address of fits server
 *
 * FUNCTION VALUE:
 *	status
 *
 * PURPOSE:
 *	Set up structures for using fits saver
 *
 * DESCRIPTION:
 *
 * EXTERNAL VARIABLES:
 *	svrP - saver structure
 *
 * PRIOR REQUIREMENTS:
 *
 * DEFICIENCIES:
 * 	None known
 *
 * HISTORY:
 *
 *****************************************************************************/
int useFits(int port, char *host,char *ip)
{
    int status=OK;
    fitsParams *fits;

    fits = &svrP.param.fits;
  
    logMessage(CICS_DB_FULL, "Into Fits Mode");

    svrP.server = FITS_SAVE;
    fits->port = port;
    strncpy(fits->host,host,MAXSTRING);
    strncpy(fits->ip,ip,MAXSTRING);
    fits->format = IRF_LONG;
    printf("useFits: socket = %ld, ip = %s\n",fits->port,fits->ip);

    return status;
}
#endif
/*****************************************************************************
 * Function name:
 *	useDHS
 *
 * Invocation:
 *	useDHS(ip,server,simulation,impName,qlStreams);
 *
 * PARAMETERS:
 *	ip - ip address of dhs server
 *	server - server name
 *	simulation - simulation boolean (not used in this version)
 *	impName - name used to identify this computer
 *
 * FUNCTION VALUE:
 *	none
 *
 * PURPOSE:
 * 	set up structures for use with dhs
 *
 * DESCRIPTION:
 *
 * EXTERNAL VARIABLES:
 *	svrP - saver structure
 *
 * PRIOR REQUIREMENTS:
 *
 * DEFICIENCIES:
 * 	None known
 *
 * HISTORY:
 *****************************************************************************/
static int dhsSimulation = FALSE;
/* call in startup file to set server name and its ip address.
 * part of the API
 */
void useDHS(char *ip, char *server, int simulation,char *impName)
{
    
    dhsParams *dhs;

    dhs = &svrP.param.dhs;
    dhs->format = DHS_DT_INT32;
	printf("********************* useDHS: format = %d\n",svrP.param.dhs.format);
    strncpy(dhs->impName,impName,MAXSTRING);
    dhs->initDone = 0;
    dhs->connected = 0;
    strncpy(dhs->name, server, MAXSTRING);
    strncpy(dhs->ip, ip, MAXSTRING);
    svrP.server = DHS_SAVE;
    dhsSimulation = simulation;
    printf ("useDHS dhs %x connect %ld\n",dhs,dhs->connect);
}

/*****************************************************************************
 * servP_DhsInit
 *****************************************************************************/
int servP_DhsInit() {
   DHS_STATUS status = DHS_S_SUCCESS;
   dhsParams *dhs;
   dhs = &svrP.param.dhs;

   printf ("servP_DhsInit dhs %x connect %ld\n",dhs,dhs->connect);

   /* simulation mode */
   if (dhsSimulation) {
      dhs->initDone = TRUE;
      svrP.health = GOOD;
	   return OK;
      }

   if (!dhs->initDone) {
      dhs->initDone = FALSE;
      svrP.health = BAD;
      /* init */
      dhsInit(dhs->impName, 16, &status);
      if (status != DHS_S_SUCCESS) {
         printf( "dhsInit failed (status=%d)", status);
         return ERROR;
         }
      /* callbacks */
      dhsCallbackSet(DHS_CBT_ERROR, dhsErrorCallback, &status);
      if (status != DHS_S_SUCCESS) {
         printf( "dhsCallbackSet failed (status=%d)", status);
         return ERROR;
         }
   	/* event loop */
      dhsEventLoop(DHS_ELT_THREADED, (DHS_THREAD *) NULL, &status); 
      if (status != DHS_S_SUCCESS) {
         printf( "dhsEventLoop failed (status=%d)", status);
         return ERROR;
         }
      dhs->initDone = TRUE;
      }
   else {
      printf( "dhs already initialised\n");
      }
   
   /* dhsDebugLevel(DHS_DEBUG_ON,&status); */
   svrP.health = GOOD;
   printf("Dhs initialized\n");
   return OK;
   }


void printFormat()
{

	printf("format = %d\n",svrP.param.dhs.format);	
}
/*****************************************************************************
 * Function name:
 *	dhsGetCallback
 *
 * Invocation:
 *	called automatically when dhs receives a dhsGet command
 *
 * PARAMETERS:
 *	refer to icd 3 bulk data transfer
 *
 * FUNCTION VALUE:
 *	none
 *
 * PURPOSE:
 *	
 *
 * DESCRIPTION:
 *
 * EXTERNAL VARIABLES:
 *
 * PRIOR REQUIREMENTS:
 *
 * DEFICIENCIES:
 * 	None known
 *
 * HISTORY:
 *
 *****************************************************************************/
void dhsGetCallback(DHS_CONNECT connect, DHS_STATUS errorNum,
                      DHS_ERR_LEVEL errorLev, char *msg, DHS_TAG tag,
                      void *userData)
{
    return;
}
/*****************************************************************************
 * Function name:
 *	dhsErrorCallback
 *
 * Invocation:
 *	Called by dhs server
 *
 * PARAMETERS:
 *	 refer to icd 3 bulk data transfer
 *
 * FUNCTION VALUE:
 *
 * PURPOSE:
 *	called when an error is detected by the dhs server
 *
 * DESCRIPTION:
 *
 * EXTERNAL VARIABLES:
 *
 * PRIOR REQUIREMENTS:
 *
 * DEFICIENCIES:
 * 	None known
 *
 * HISTORY:
 *
 *****************************************************************************/
void dhsErrorCallback(DHS_CONNECT connect, DHS_STATUS errorNum,
                      DHS_ERR_LEVEL errorLev, char *msg, DHS_TAG tag,
                      void *userData)
{
    dhsParams *dhs;
    DHS_STATUS status;
    
    printf("\ndhsErrorCallback\n");
    printf(msg);
    return;
	
	
	
    dhs = &svrP.param.dhs;
    /* this switch is sort of silly; i.e., needs work XXX */
    switch (errorNum) {
    case DHS_S_CMD_BULK_SENT:
    case DHS_S_CMD_DELETE:
    case DHS_S_CMD_MSG_SENT:
    case DHS_S_CMD_NEW:
    case DHS_S_COND_TIMEOUT:
    case DHS_S_CONNECT:
    case DHS_S_DISCONNECT:
    case DHS_S_EL_EXIT:
    case DHS_S_FN_ENTRY:
    case DHS_S_FN_EXIT:
    case DHS_S_IMP_EVENT:
    case DHS_S_MUTEX_LOCK:
    case DHS_S_MUTEX_UNLOCK:
    case DHS_S_NO_ATTRIB:
    case DHS_S_NO_DATA:
    case DHS_S_NO_FRAME:
    case DHS_S_NO_MESSAGE:
    case DHS_S_NO_RESP:
    case DHS_S_NULL:
    case DHS_S_RECONNECT:
    case DHS_S_SHUTDOWN:
    case DHS_S_SYS_EVENT:
    case DHS_S_USER_EVENT:
    case DHS_S_SUCCESS:
        printf( "Not an error %d", errorNum);
        break;
    case DHS_E_AVLIST_ARRAY:
    case DHS_E_BT_FIND:
    case DHS_E_BT_NOT_FOUND:
    case DHS_E_CB_NULL:
    case DHS_E_CB_TYPE:
    case DHS_E_CMD_DELETED:
    case DHS_E_CMD_LOST:
    case DHS_E_CMD_FIND:
    case DHS_E_CMD_NOT_FOUND:
    case DHS_E_CON_FIND:
    case DHS_E_CON_INVALID:
    case DHS_E_CON_LOCKOUT:
    case DHS_E_CON_LOST:
    case DHS_E_CON_NOT_FOUND:
    case DHS_E_COND:
    case DHS_E_CONNECT:
    case DHS_E_CTL_CMD:
    case DHS_E_DISCONNECT:
    case DHS_E_EL_RUNNING:
    case DHS_E_ERS:
    case DHS_E_ERS_MSG:
    case DHS_E_IMP:
    case DHS_E_IMP_MSG:
    case DHS_E_IMP_REGISTER:
    case DHS_E_IMP_SYS_MSG:
    case DHS_E_IMP_USER_MSG:
    case DHS_E_INIT:
    case DHS_E_LOCATE:
    case DHS_E_LOCATE_MSG:
    case DHS_E_LOOP_TYPE:
    case DHS_E_MEMORY:
    case DHS_E_MSG_LENGTH:
    case DHS_E_MUTEX:
    case DHS_E_NO_ATTRIB:
    case DHS_E_NOT_AVLIST:
    case DHS_E_NULLVALUE:
    case DHS_E_PTR_SIZE:
    case DHS_E_SDS:
    case DHS_E_TASK_UNKNOWN:
    case DHS_E_THREAD_CREATE:
    case DHS_E_TSD:
    case DHS_E_TYPE:
       printf("Error callback error number %d", errorNum);
        break;
    default:
        printf("Error callback: unknown error %d", errorNum);
    }

    switch (errorLev) {
    case DHS_EL_SEVERE:
        printf("\nSevere dhs error: %d\n%s", errorNum, msg);
        svrP.health = BAD;
    
	status=0;
    if(gnPutEpicsT(dbSadTop, DHSCONNECTED ".VAL", DCALONG, &status)!= OK)
  		 printf ("dhsErrorCallback: failed to set DHSCONNECTED\n");

 
        status = DHS_S_SUCCESS;
 
        dhsExit(&status);
        dhs->initDone = FALSE;
        break;
    case DHS_EL_ERROR:
        printf( "\ndhs error, operation failed: %d\n%s", errorNum, msg);
        svrP.health = BAD;
        break;
    case DHS_EL_WARNING:
        printf( "\ndhs warning: %d\n%s.", errorNum, msg);
        svrP.health = WARNING;
        break;
    case DHS_EL_INFO:
       printf( "\ndhs info: %d\n%s", errorNum, msg);
        break;
    default:
        break;
    }
        printf("\nerrornum : %d\n message = %s", errorNum, msg);
}


/*****************************************************************************
 * servP_DhsConnect
 *****************************************************************************/
int servP_DhsConnect() {
   DHS_STATUS status = DHS_S_SUCCESS;
   dhsParams *dhs;
   dhs = &svrP.param.dhs;

   printf ("IN servP_DhsConnect dhs %x connect %ld\n",dhs,dhs->connect);

    printf("dhs->connected = %i\n",dhs->connected);

   if (dhs->connected == FALSE) {
      /* connection */
      status = DHS_S_SUCCESS;
      printf("IN servP_DhsConnect: dhsConnect : ip %s,name %s ... \n",dhs->ip,dhs->name);
      dhs->connect = dhsConnect(dhs->ip, dhs->name, NULL, &status);
      if (status != DHS_S_SUCCESS) {
              printf( "Cannot connect to DHS server.\n");
         dhs->connect = DHS_CONNECT_NULL;
         dhs->connected = FALSE;
         svrP.health = BAD;
         if(gnPutEpicsT(dbSadTop, DHSCONNECTED ".VAL", DCALONG, &dhs->connected)!= OK)
            printf ("failed to set DHSCONNECTED\n");
         return ERROR;
         }
      dhs->connected = TRUE;
      if(gnPutEpicsT(dbSadTop, DHSCONNECTED ".VAL", DCALONG, &dhs->connected)!= OK)
         printf ("failed to set DHSCONNECTED\n");
      printf("done %ld\n",dhs->connect);
      svrP.health = GOOD;
      printf("all good\n");
      return OK;
      }
   else {
      printf("already connected to DHS\n");
      return OK;
      }
   }

/*****************************************************************************
 * servP_DhsDisConnect
 *****************************************************************************/
int servP_DhsDisConnect() {
   DHS_STATUS status = DHS_S_SUCCESS;
   dhsParams *dhs;
   dhs = &svrP.param.dhs;

   if (dhs->connected == TRUE) {
      /* connection */
      status = DHS_S_SUCCESS;
      printf("dhsDisconnect : ip %s,name %s ... \n",dhs->ip,dhs->name);
      dhsDisconnect(dhs->connect, &status);
      if (status != DHS_S_SUCCESS) {
              printf( "Cannot disconnect from DHS.\n");
         return ERROR;
         }
      dhs->connected = FALSE;
      if(gnPutEpicsT(dbSadTop, DHSCONNECTED ".VAL", DCALONG, &dhs->connected)!= OK)
         printf ("failed to set DHSCONNECTED\n");
      printf("!!!!!! disconnected\n");
      svrP.health = BAD;
      dhs->connect = DHS_CONNECT_NULL;
      return OK;
      }
   else {
      printf("already disconnected to DHS\n");
      return OK;
      }
   }


