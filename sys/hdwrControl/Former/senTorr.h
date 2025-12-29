/* included files*/
#include <vxWorks.h>
#include <stdio.h>
#include        <string.h>

#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sysLib.h>
#include <fcntl.h>
#include <ioLib.h>
#include <unistd.h>
#include <tickLib.h> /* tickGet*/
#include        <sysLib.h> /* sysClkRateGet*/
#include <drv/serial/z8530.h>
#include <cicsLib.h>
#include        <taskLib.h>/* task delay, spawn*/
#include        <dbAccess.h>
#include <aiRecord.h>
#include        <recSup.h>
#include        <devSup.h>
#include <epCommon.h>
#include        <semLib.h>  /* semaphores*/


/* defines*/
#define SEM_TIMEOUT (sysClkRateGet())

#define BAUD 9600
#define EPSILON .1
#define THRESHOLD .0015
#define STR_LEN_OUT 8
#define STR_LEN_IN 11
#define NOTHING "                         "

/* record names*/
#define THERMOCOUPLE1 "tc1"
#define THERMOCOUPLE2 "tc2"
#define COLD_CATHODE_GAUGE "ccg"
#define EMIS_TIME 3000

/* function prototypes*/
int setdtr(int dtr,long port);
long initTC(struct aiRecord *pai);
long readTC(struct aiRecord *pai);
long initCCG(struct aiRecord *pai);
long readCCG(struct aiRecord *pai);
long openSenTorrPort();
long readPort(int port,char *msg,int n,int time);
/* structures*/
struct 
{
  long            number;
  DEVSUPFUN       report;
  DEVSUPFUN       init;
  DEVSUPFUN       init_record;
  DEVSUPFUN       get_ioint_info;
  /* The elements above this line must conform to dset and the elements
   * below must conform to aodset.
   */
  DEVSUPFUN       write_ao;
  DEVSUPFUN       special_linconv;
} devAiTC= {
  6,
  NULL,
  NULL,
  initTC,
  NULL,
  readTC,
  NULL
};

struct 
{
  long            number;
  DEVSUPFUN       report;
  DEVSUPFUN       init;
  DEVSUPFUN       init_record;
  DEVSUPFUN       get_ioint_info;
  /* The elements above this line must conform to dset and the elements
   * below must conform to aodset.
   */
  DEVSUPFUN       write_ao;
  DEVSUPFUN       special_linconv;
} devAiCCG= {
  6,
  NULL,
  NULL,
  initCCG,
  NULL,
  readCCG,
  NULL
};

