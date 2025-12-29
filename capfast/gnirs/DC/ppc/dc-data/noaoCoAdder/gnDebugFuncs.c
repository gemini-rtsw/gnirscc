#include "gnDCADefs.h"
#include "gnDCAVars.h"
#include        <vxWorks.h>
#include        <vme.h>
#include        <dbDefs.h>
#include        <dbScan.h>
#include        <drvSup.h>
#include        <module_types.h>
#include        <task_params.h>
#include        <taskwd.h>
  
void dcaFindBoard()
{
char 	char_value=1;			/* 1 bytes (D8)   */ 
short 	word_value=1;			/* 2 bytes (D16)  */
int 	value=1;				/* 4 bytes (D32)  */ 
char                    *ladd;
char *add;
int i;

/*
 * VME_AM_EXT_SUP_PGM
 * VME_AM_EXT_SUP_DATA
 * VME_AM_EXT_USR_PGM
 * VME_AM_EXT_USR_DATA
 * VME_AM_STD_SUP_PGM
 * VME_AM_STD_SUP_DATA
 * VME_AM_STD_USR_PGM
 * VME_AM_STD_USR_DATA
 * VME_AM_SUP_SHORT_IO
 * VME_AM_USR_SHORT_IO
 * VME_AM_EXT_SUP_PGM
 * VME_AM_EXT_USR_PGM
 * VME_AM_EXT_SUP_DATA
 * VME_AM_EXT_USR_DATA
 * VME_AM_STD_SUP_PGM
 * VME_AM_STD_USR_PGM
 * VME_AM_STD_SUP_DATA
 * VME_AM_STD_USR_DATA
 * VME_AM_SUP_SHORT_IO
 * VME_AM_USR_SHORT_IO
 */

printf ("probing VME_AM_USR_SHORT_IO A16/D16 window ...\n");
for (i=0;i<256;i++) {
	add = (i << 8) + (TESTENBLNSEL * (sizeof(int)));
	if (sysBusToLocalAdrs(VME_AM_USR_SHORT_IO,add, &ladd) != OK) {
		printf ("   bus error 0x%x\n",(unsigned int)add);
		continue;
		}
 	if (vxMemProbe(ladd,WRITE,sizeof(word_value),&word_value) != OK) {
		/* printf ("   access error 0x%x\n",add); */
		continue;
		}
	printf ("   found 0x%x\n",add);
 	}
printf ("probing VME_AM_USR_SHORT_IO A16/D32 window ...\n");
for (i=0;i<256;i++) {
	add = (i << 8) + (TESTENBLNSEL * (sizeof(int)));
	if (sysBusToLocalAdrs(VME_AM_USR_SHORT_IO,add, &ladd) != OK) {
		printf ("   bus error 0x%x\n",(unsigned int)add);
		continue;
		}
 	if (vxMemProbe(ladd,WRITE,sizeof(value),(void *)&value) != OK) {
		/* printf ("   access error 0x%x\n",add); */
		continue;
		}
	printf ("   found 0x%x\n",(unsigned int)add);
 	}


printf ("probing VME_AM_STD_USR_DATA A16/D16 window ...\n");
for (i=0;i<256;i++) {
	add = (i << 8) + (TESTENBLNSEL * (sizeof(int)));
	if (sysBusToLocalAdrs(VME_AM_STD_USR_DATA,add, &ladd) != OK) {
		printf ("   bus error 0x%x\n",(unsigned int)add);
		continue;
		}
 	if (vxMemProbe(ladd,WRITE,sizeof(word_value),&word_value) != OK) {
		/* printf ("   access error 0x%x\n",add); */
		continue;
		}
	printf ("   found 0x%x\n",(unsigned int)add);
 	}
printf ("probing VME_AM_STD_USR_DATA A16/D32 window ...\n");
for (i=0;i<256;i++) {
	add = (i << 8) + (TESTENBLNSEL * (sizeof(int)));
	if (sysBusToLocalAdrs(VME_AM_STD_USR_DATA,add, &ladd) != OK) {
		printf ("   bus error 0x%x\n",(unsigned int)add);
		continue;
		}
 	if (vxMemProbe(ladd,WRITE,sizeof(value),&value) != OK) {
		/* printf ("   access error 0x%x\n",add); */
		continue;
		}
	printf ("   found 0x%x\n",(unsigned int)add);
 	}
 	
printf ("probing VME_AM_EXT_USR_DATA A16/D16 window ...\n");
for (i=0;i<256;i++) {
	add = (i << 8) + (TESTENBLNSEL * (sizeof(int)));
	if (sysBusToLocalAdrs(VME_AM_EXT_USR_DATA,add, &ladd) != OK) {
		printf ("   bus error 0x%x\n",(unsigned int)add);
		continue;
		}
 	if (vxMemProbe(ladd,WRITE,sizeof(word_value),&word_value) != OK) {
		/* printf ("   access error 0x%x\n",add); */
		continue;
		}
	printf ("   found 0x%x\n",(unsigned int)add);
 	}
printf ("probing VME_AM_EXT_USR_DATA A16/D32 window ...\n");
for (i=0;i<256;i++) {
	add = (i << 8) + (TESTENBLNSEL * (sizeof(int)));
	if (sysBusToLocalAdrs(VME_AM_EXT_USR_DATA,add, &ladd) != OK) {
		printf ("   bus error 0x%x\n",(unsigned int)add);
		continue;
		}
 	if (vxMemProbe(ladd,WRITE,sizeof(value),&value) != OK) {
		/* printf ("   access error 0x%x\n",add); */
		continue;
		}
	printf ("   found 0x%x\n",(unsigned int)add);
 	}













}

