static struct {
        void *v;
        char *c;
} rcsid = {
        &rcsid,
        "$Id: hd.c,v 1.2 2009/05/27 19:33:35 fkraemer Exp $"
};
/******************************************************************************
 * Program:	hd (hex dump)
 * File:	hd.c
 * Purpose:	This program will dump a .trl file out in hex.  The only data
 *		display is the code & variables of the program.  Also, hd will
 *		fail if it sees any relocation data.
 * Author:	David Koski
 * Copyright:   Aura Inc.  All rights reserved.
 * History:
 *		27Jun91	created						dak
 *		16Jun97 modified for vxworks        pr
 *			added ldmc,ldseq,killuc,startuc, getresponse,sleep
 *
 ******************************************************************************/
#include "hd.h"
#include <epCommon.h>
#include <wFireMsgDefs.h>

void sleep (int a, int b);

extern struct WFMsgTaskParameters wfParams;
#define link int
extern LINK LinkId;
extern	MSG_Q_ID	WFMsg_Q_ID_in ;
int TestRead (link LinkId); /*routine to test if there is anything on link */
extern int wfm_byte_swap(char *buf,int len);
extern char *dbTop;

char tmp[80];
 /* send to transputer*/

static void send_block() 
{
    struct WFMsgMsg	msg ;
    int header;
    int bufCp[BUF_SIZE + 1];
    int i;
    char *p;/*  = dest; pr??????? */

    bcopy((char *) buf, (char *) (bufCp + 1), sizeof(int) * bInd);
    bufCp[0] = (pInd << 16) | 1;

    wfm_byte_swap((char *)bufCp, sizeof(int) *(bInd + 1));

    p = parse(dest);
    while (p != NULL)
    {
	i = node_to_int(p);

	header = MESSAGE(SET_VAR) | TO_NODE(i) | OF_LENGTH(bInd + 1);
	wfm_byte_swap((char *) &header, 4);

	if (LinkId == -1)
	{
	    LinkId = OpenLink(NULL);

	}

	if(WriteLink(LinkId, &header, 4, 0) != 4)
	    return;

	if(WriteLink(LinkId, bufCp, sizeof(int) *(bInd + 1), 2) != sizeof(int) *(bInd + 1))
	    return;
	

	printf ("Sent message,   Waiting for reply.....");
	/* wait for return from transputers*/
 	semGive( wfParams.readLinkSem ) ; /* tell program that is reading 
					   *    replies that it should be
					   *    expecting a response*/
	while( (msgQNumMsgs( WFMsg_Q_ID_in ) ) == 0 ) /* sleep until there is a message*/
	    sleep (0,40000000);
	/* read the message and throw it away*/
	while( msgQReceive(WFMsg_Q_ID_in, (char *)&msg, sizeof( msg ), NO_WAIT ) != ERROR ) ;

	printf ("Received\n");

	p = parse(dest);
    }

    p = parse(NULL);

    pInd += bInd;
    bInd = 0;
}

static void byte(unsigned char c)

{
    ct++;
    w >>= 8;
    *cp = c;
   
    if (ct == 4)
    {
	buf[bInd ++] = w;

	if (bInd == BUF_SIZE)
	{
	   send_block();
	}
	w = ct = 0;
    }
  
}
/*
 *+
 * FUNCTION NAME:
 * ldWaveFormGen
 *
 * INVOCATION:
 *  ldWaveFormGen(char *path,char *name,char *cpath)
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 *   >path = path where microcode is in the disk
 *   >name = name of microcode file
 *   >cpath = path where command file is on the disk
 *
 * FUNCTION VALUE:
 *     return STATUS
 *
 * PURPOSE:
 *  Download microcode to transputer
 *
 * DESCRIPTION:
 *     This routine stops any code running on the transputers, loads new code,
 *          and starts it back up again.
 *
 * EXTERNAL VARIABLES:
 * None
 *
 * PRIOR REQUIREMENTS:
 *   The Transputers must have had their bootstrap code loaded by ldnet.
 *
 * DEFICIENCIES:
 * None
 *
 * HISTORY:

 *
 *-
 */


/* This stops the currently running microcode, loads a new one and starts it*/
long ldWaveFormGen (char *path, char *name, char *cpath)
{
  char dbName[80];
  long status=0;
  struct dbAddr killAddr, startAddr; 

  cicsLogMessage(3, "\nEntering LdWaveFormGen\n");
  fflush(stdout); 
  sprintf(dbName,"%s%s",dbTop,KILLUC);
  status = dbNameToAddr(dbName, &killAddr); 
  sprintf(dbName,"%s%s",dbTop,STARTUC);
  if (status == OK) 
	status = dbNameToAddr(dbName, &startAddr); 

  if (status == OK)
    {
/* 	  cicsLogMessage(3, "%s%s\n",path,name); */
	  if (LinkId > 0) /*if link is open*/
		{
		  /* process EPICS record that sends a message to kill uc*/
		  dbProcess(killAddr.precord);  
	   
		  /* take semaphore so that no EPICS records try to write to the
		   *       transputers*/
		  if (semTake( wfParams.EpicsInQSem, WFSemTakeTimeout) == OK)
			{
#if 1
			  /* download the microcode*/
			  status = ldmc (path,name,cpath); 
#else
			  sleep(60, 0);
#endif
			  /* let other people talk to the transputers now*/
			  (void)semGive( wfParams.EpicsInQSem);
			}
		  else	
			{
			  cicsLogMessage(0, "ucdl Couldn't take semaphore\n");
			  status = ERROR;
			}

		  if (status != ERROR) 
			{ 
			  cicsLogMessage(3,"start ucode\n\n\n");
			  /* process record that starts microcode*/
			  dbProcess(startAddr.precord);  
			  /* dbpf("naac:dc:StartUC","1");  */ 
			} 
		}
	  else if (LinkId <= 0) /* if link is closed*/
		{
		  cicsLogMessage(3,"opening link\n");
		  LinkId = OpenLink (NULL); 
		  if (LinkId > 0)
			{
			  cicsLogMessage(3,"link is open\n");
			  /* process EPICS record that sends a message to kill uc*/
			  dbProcess(killAddr.precord); 
			  /* take semaphore so that no EPICS records try to write to the
			   *       transputers*/
			  if (semTake( wfParams.EpicsInQSem, WFSemTakeTimeout) == OK)
				{	
#if 1
				  /* download the microcode*/
				  status = ldmc (path,name,cpath); 
#else
				  sleep(60, 0);
#endif
				  (void)semGive( wfParams.EpicsInQSem);
				}
			  else	
				{
				  cicsLogMessage(0, "Couldn't take semaphore\n");
				  status = ERROR;
				}
			  if (status == OK) 
				{  /* process record that starts microcode*/
				  dbProcess(startAddr.precord);  
				  /* dbpf("naac:dc:StartUC","1");  */ 
				} 
			  if (LinkId >0)  
				CloseLink(LinkId);  
			  LinkId = -1; 
			}
		}
    }
  else
	{
	  status = ERROR;
	  cicsLogMessage(0, "Addressing error in downloader");
	}
  return status;
}

/*
 *+
 * FUNCTION NAME:
 * ldmc
 *
 * INVOCATION:
 *  ldWaveFormGen(char *path,char *name,char *cpath)
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 *   >mcpath = path where microcode is in the disk
 *   >mcname = name of microcode file
 *   >controlpath = path where command file is on the disk
 *
 * FUNCTION VALUE:
 *     return STATUS
 *
 * PURPOSE:
 *  Download microcode to transputer
 *
 * DESCRIPTION:
 *    This routine loads the  new code to the transputer.  It is called from
 * ldWaveFormGen normally.
 *        
 *
 * EXTERNAL VARIABLES:
 * None
 *
 * PRIOR REQUIREMENTS:
 *   The Transputers must have had their bootstrap code loaded by ldnet.
 *
 * DEFICIENCIES:
 * None
 *
 * HISTORY:

 *
 *-
 */



long ldmc (char *mcpath, char *mcname, char *controlpath )
{
    unsigned int t, i;
    int fd;
    int type = 0;
/*     unsigned char lbuf[65536]; */
    unsigned char lbuf[4096];
    char name[256];
    int eof = 0;
 
    ct = 0;
    w = 0;
    cp = (unsigned char *) &w; 
    bInd = 0;
    pInd = 0;
#if 0
#ifdef SUN  
    /* pr  this is only used for tcl*/
    if (argc == 1 || argc > 3)
    {
	sprintf(interp->result, "usage: load <filename> [dest]\n");
	return ERROR;
    }

    if (argc == 3)
	dest = argv[2];
    else
	dest = "seq";
#endif
#endif
    init_vxw_link();

    dest = "seq";		/* pr */
    strcpy(name,mcpath);	/* pr */
    strcat(name,mcname);	/* pr */
    strcat(name,".tld");	/* pr */
/* printf ("ucode name = %s\n",name); */
    fd = open(name,O_RDONLY,666);
   
    if (fd == ERROR)
    {
	sprintf(tmp, "ERROR: error opening %s\n", name);
	cicsLogMessage(0,tmp);
	return ERROR;
    }

    pInd = 0;

    while (read(fd, (char *)&type, 1) > 0)
    {
	type >>= 24;
	switch (type)
	{
	  case T_RESERVED:      /**/
	      read(fd,(char *)lbuf,4);
	      break;
	  case T_REL_FILE:      /**/
	      read(fd,(char *)lbuf,1);
	      break;
	  case T_LIB_FILE:      /**/
	      read(fd,(char *)lbuf,1);
	      break;
	  case T_LD_FILE:	/**/
	      read(fd,(char *)lbuf,1);
	      break;
	  case T_SIZE:		/**/
	      read(fd,(char *)lbuf,8);
	      break;
	  case T_EOF:		/**/
	      eof = 1;
	      read(fd,(char *)lbuf,2);
	      break;
	  case T_SYMBOL:	/**/
	      read(fd,(char *)lbuf,2);
	      t = lbuf[1];
/* if (t >=4096) *//*  printf ("\ndata size = %d \n",t);  */ 
	      read(fd,(char *)lbuf,t);
	      break;
	  case T_FILENAME:
	      read(fd,(char *)lbuf,5);
	      t = lbuf[4];
/* if (t >=4096) *//*  printf ("\ndata size = %d \n",t);  */ 
	      read(fd,(char *)lbuf,t);
	      lbuf[t] = '\0';
	      break;
	  case T_MODULE:
	      read(fd,(char *)lbuf,3);
	      break;
	  case T_ALIGN:
	      read(fd,(char *)lbuf,2);
	      break;
	  case T_DATA:      /*10*/
	      read(fd,(char *)lbuf,4);
	      t = lbuf[3]*256 +lbuf[2];
/* if (t >=4096) printf ("\ndata size = %d \n",t);   */
	      read(fd,(char *)lbuf,t);
	      for (i = 0; i<t; i += 1)
		  byte((unsigned char) lbuf[i]);
/*printf ("\nbyte LinkId = %d",LinkId); */
	      break;
	  case T_REL_DATA:
	      read(fd,(char *)lbuf,6);
	      cicsLogMessage(0, "ERROR:  Relocation data! T_REL_DATA\n");
	      close(fd);
	      return ERROR;
	      break;
	  case T_RELSYM_DATA:
	      read(fd,(char *)lbuf,8);
	      cicsLogMessage(0, "ERROR:  Relocation data! T_RELSYM_DATA\n");
	      close(fd);
	      return ERROR;
	      break;
	  case T_RELREL_DATA:
	      read(fd,(char *)lbuf,10);
	      cicsLogMessage(0, "ERROR:  Relocation data! T_RELREL_DATA\n");
	      close(fd);
	      return ERROR;
	      break;
	  case T_ADDR_DATA:
	      read(fd,(char *)lbuf,8);
	      cicsLogMessage(0, "ERROR:  Relocation data! T_ADDR_DATA\n");
	      close(fd);
	      return ERROR;
	      break;
	  case T_STORAGE:	/*15*/
	      read(fd,(char *)lbuf,6);
	      t = lbuf[5] << 24;
	      t += lbuf[4] << 16;
	      t += lbuf[3] << 8;
	      t += lbuf[2];
	      for (i = 0; i < t; i++)
		  byte((unsigned char) 0);
	      break;
	  case T_DEF:
	      read(fd,(char *)lbuf,4);
	      break;
	  case T_SET:
	      read(fd,(char *)lbuf,8);
	      break;
	  case T_REL_OP:
	      read(fd,(char *)lbuf,8);
	      cicsLogMessage(0, "ERROR:  Relocation data! T_REL_OP\n");
	      close(fd);
	      return ERROR;
	      break;
	  case T_RELSYM_OP:
	      read(fd,(char *)lbuf,10);
	      cicsLogMessage(0, "ERROR:  Relocation data! T_RELSYM_OP\n");
	      close(fd);
	      return ERROR;
	      break;
	  case T_RELREL_OP:
	      read(fd,(char *)lbuf,12);
	      cicsLogMessage(0, "ERROR:  Relocation data! T_RELREL_OP\n");
	      close(fd);
	      return ERROR;
	      break;
	  case T_ADDR_OP:
	      read(fd,(char *)lbuf,10);
	      cicsLogMessage(0, "ERROR:  Relocation data! T_ADDR_OP\n");
	      close(fd);
	      return ERROR;
	      break;
	  case T_LOAD:		/*22*/
	      read(fd,(char *)lbuf,4);
	      break;
	  case T_STACK:		/*23*/
	      read(fd,(char *)lbuf,4);
	      break;
	  case T_ENTRY:		/*24*/
	      read(fd, (char *)lbuf, 4);
	      t=(lbuf[0] + lbuf[1]) << (8 + lbuf[2]) << (16 + lbuf[3]) << 24;
/*t = lbuf[0] + lbuf[1] << 8 + lbuf[2] << 16 + lbuf[3] << 24; */
	      if (t != 0)
	      {
		  cicsLogMessage(0, "ERROR: T_ENTRY not zero!\n");
		  close(fd);
		  return ERROR;
	      }
	      break;
	  case T_DEBUG_DATA:
	      read(fd,(char *)lbuf,8);
	      t = lbuf[6]*256+lbuf[7];
/* if (t >=4096) printf ("\ndata size = %d \n",t);   */
	      read(fd,(char *)lbuf,t);
	      break;
	  case T_DEBUGSYM_DATA:
	      read(fd,(char *)lbuf,10);
	      t = lbuf[6]*256+lbuf[7];
/* if (t >=4096) printf ("\ndata size = %d \n",t);  */ 
	      read(fd,(char *)lbuf,t);
	      break;
	  case T_WRELREL_OP:
	      read(fd,(char *)lbuf,12);
	      cicsLogMessage(0, "ERROR:  Relocation data! T_WRELREL_OP\n");
	      close(fd);
	      return ERROR;
	      break;
	  default:
	      sprintf(tmp, "ERROR: load: unknown: %d\n",type);
	      cicsLogMessage(0,tmp);
	      break;
	}
    }

    send_block();
    close(fd);
cicsLogMessage(3, "\nldmc finished\n");
    return OK;

}

