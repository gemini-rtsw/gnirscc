#include        <string.h>
#include        <vxWorks.h>
#include        <stdlib.h>
#include        <ioLib.h>  /* ioctl */
#include        <types.h>
#include <tickLib.h> /* tickGet*/
#include        <sysLib.h> /* sysClkRateGet*/
#include        <taskLib.h>/* task delay, spawn*/
#include        <semLib.h>/* semaphores*/
#include        <alarm.h>
#include  <cicsLib.h>
#include        <dbDefs.h>
#include        <dbAccess.h>
#include        <recSup.h>
#include        <devSup.h>
#include        <module_types.h>
#include        <aoRecord.h>
#include        <aiRecord.h>
#include <epCommon.h>
#include <subRecord.h>
/* #include        <longoutRecord.h> */
/* #include        <longinRecord.h> */
/* #include        <stringinRecord.h> */
/* #include        <stringoutRecord.h> */


/* #include <vxWorks.h> */
/* #include <types.h> */
/* #include <stdioLib.h> */
/* #include <semLib.h> */

/* #include <dbDefs.h> */
/* #include <subRecord.h> */
/* #include <dbCommon.h> */
/* #include <recSup.h> */

#define SemTakeTimeout (sysClkRateGet()/2)
#define CONVERT 0
#define DO_NOT_CONVERT 2
int subOn = 0;
int serOn = 0;
SEM_ID semTempPort;
extern int Port2;/* file descriptor for serial port on temp board */
int openport2();
static long init_aoTempPort();
static long write_aoTempPort();
struct {
	long		number;
	DEVSUPFUN	report;
	DEVSUPFUN	init;
	DEVSUPFUN	init_record;
	DEVSUPFUN	get_ioint_info;
	/* The elements above this line must conform to dset and the elements
	 * below must conform to aodset.
	 */
	DEVSUPFUN	write_ao;
	DEVSUPFUN	special_linconv;
}devAoTemp={
	6,
	NULL,
	NULL,
	init_aoTempPort,
	NULL,
	write_aoTempPort,
	NULL};

static long init_aiTempPort();
static long read_aiTempPort();

struct {
	long		number;
	DEVSUPFUN	report;
	DEVSUPFUN	init;
	DEVSUPFUN	init_record;
	DEVSUPFUN	get_ioint_info;
	/* The elements above this line must conform to dset and the elements
	 * below must conform to aidset.
	 */
	DEVSUPFUN	read_ai;/*(0,2)=> success and convert,don't convert)*/
			/* if convert then raw value stored in rval */
	DEVSUPFUN	special_linconv;
}devAiTemp={
	6,
	NULL,
	NULL,
	init_aiTempPort,
	NULL,
	read_aiTempPort,
	NULL};


