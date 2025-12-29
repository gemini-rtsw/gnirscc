static struct {
        void *v;
        char *c;
} rcsid = {
        &rcsid,
        "$Id: b014link.c,v 1.2 2009/05/27 19:33:32 fkraemer Exp $"
};
/*
 *      ISERVER  -  INMOS standard file server
 *
 *      b014link.c
 *
 *      Link module for B014 boards with S514 device driver
 *
 *      Copyright (c) INMOS Ltd., 1988.
 *      All Rights Reserved.
 */


/* Modification:
 * 31/08/90 BJ  Thorough overhaul - checking return from 'ioctl'
 *                                - use IMS_IO instead of B014_IO
 *                                - add 'lseek' in 'ReadLink' & 'WriteLink'
 *                                - fix timeout return code
 */
int b014debug = 1;

#if	defined(vxWorks)
#include "vxWorks.h"
#include "stdio.h"
#include "ioLib.h"
#include "taskLib.h"
#include "sysLib.h"
#define ETIME ETIMEDOUT
static BOOL SetTimeout () ;
#else
#include <stdio.h>
#include <fcntl.h>

#include <sys/types.h>
#include <sys/file.h>      /* Declarations for 'lseek' */
#endif
#include "cicsLib.h"
#include "ims_bcmd.h"      /* INMOS device driver 'ioctl' codes */
/* #include "debug.h" */
#include "inmos.h"
#if	defined(vxWorks)
#undef	DEBUG
#endif
#include "iserver.h"

#include <errno.h>

#include	<semLib.h>


#define NULL_LINK          -1
#define REWIND_THRESHOLD   (1024L * 1024L)

#include <time.h>
/* void printTime(); */
static LINK ActiveLink = NULL_LINK;
static long int Bytes = 0L;
extern SEM_ID tpSem;


void init_vxw_link ()
{
    /* reset global variable,  any routine not in this file that uses vxw_link
     *    must initialize with this function*/
    Bytes = 0L;
}


/* OpenLink --- open a link to the transputer */

LINK OpenLink (Name)
     BYTE *Name;
{ 
    static char DefaultDevice[] = "/dev/bxiv0";
    /* Already open ? */
#ifdef TRACE
      printf ("\n\n&&&&&&\nopenlink 0,%d %d\n&&&&&&\n\n\n",ActiveLink,NULL_LINK); 
#endif
    if(semTake(tpSem,sysClkRateGet()) != ERROR)
	{
	   
	    if (ActiveLink != NULL_LINK)
		{
		    semGive(tpSem);
		    return (ER_LINK_CANT);
		}
	   
	   
	    /* Use d	efault name ? */
	    if ((Name == NULL) || (*Name == '\0')) 
		{
		   
#if defined(vxWorks	)
		    if ((ActiveLink = open (DefaultDevice, O_RDWR, 0664)) >= 0)
#else			
		    if ((ActiveLink = open (DefaultDevice, O_RDWR)) >= 0)
#endif			
			{
			    semGive(tpSem);
			    return (ActiveLink);
			    
			}
		    
		}	
	    else 	
		{	
		   
#if defined(vxWorks	)
		    if ((ActiveLink = open ((char *)Name, O_RDWR, 0664)) >= 0)
#else		
		    if ((ActiveLink = open (Name, O_RDWR)) >= 0)
#endif		
			{
			    semGive(tpSem);
			    return (ActiveLink);
			}
		   
		   
		}
	    semGive(tpSem);
	    if (errno == EBUSY)
		return (ER_LINK_BUSY);
	    else if (errno == ENOENT)
		return (ER_LINK_SYNTAX);
	    else if ((errno == ENXIO) || (errno == ENODEV))
		return (ER_NO_LINK);
	    else
		return (ER_LINK_CANT);
	}
    cicsLogMessage(0,"OpenLink: couldn't get tp Semaphore\n");
    return (ER_SEM_TIMEOUT);
}


/* CloseLink --- close down the link connection */

int CloseLink (LinkId)
     LINK LinkId;
{
    /* Note: On a Sun-4, 'close' always returns EPERM, so the code below
       simply ignores the return value.  Kludge of the millenium */
#ifdef TRACE
       printf("\n\n\n***************\nLink is closed\n*************\n\n\n\n\n"); 
#endif
    if(semTake(tpSem,sysClkRateGet()) != ERROR)
	{
	    if (LinkId != ActiveLink)
		{
		    semGive(tpSem);  
		    return (ER_LINK_BAD);
		}
	    
	    
	    ActiveLink = NULL_LINK;
	    
	    if (close (LinkId) == -1)
		{
		    semGive(tpSem);
		    return (SUCCEEDED);     /* Should be ER_LINK_CANT */
		}
	    semGive(tpSem);
	    return (SUCCEEDED);
	}
    cicsLogMessage(0,"CloseLink: tp semaphore timeout\n");
    return (ER_SEM_TIMEOUT);
    
}


/* ReadLink --- */
/* int debug = 0; */
int ReadLink (LinkId, Buffer, Count, Timeout)
     LINK LinkId;
     char *Buffer;
     unsigned int Count;
     int Timeout;
{
    register int ret;
    /*  if (debug)  */
    /*  	 printf ("ReadLink \n");  */
#ifdef TRACE
    printf ("ReadLink: %d bytes requested\n", Count);
#endif
    if(semTake(tpSem,sysClkRateGet()) != ERROR)
	{
	    
	    if (LinkId != ActiveLink)
		{
		    /*  if (d	ebug) */
		    cicsLogMessage (0,"\nReadLink not 	ActiveLink\n");
		  /*   printTime(); */
		    semGive(tpSem);
		    return (ER_LINK_BAD);
		}
	    
	    if (Count < 1)
		{
		    /*  if (d	ebug) */
		    cicsLogMessage (0,"\nReadLink not 	enough bytes\n");
		   /*  printTime(); */
		    semGive(tpSem);
		    return (ER_LINK_CANT);
		}
	    
	    if (!SetTimeout (LinkId, Timeout))
		{
		    /*  if (d	ebug) */
		    cicsLogMessage(0,"\nReadLink time	out\n");
		   /*  printTime(); */
		    semGive(tpSem);
		    return (ER_LINK_CANT);
		}
	    
	    ret = read (LinkId, Buffer, Count);
	    
	    if (ret == -1)
		{
		    /*   if (	debug) */
		    cicsLogMessage(0,"\nReadLink read	 failed\n");
		   /*  printTime(); */
		    semGive(tpSem);
		    ret = ER_LINK_CANT;
		}
#if 0	
	    else
		{
		    Bytes += (long int)ret;
		    
		    /* Rewind if	 we've sent enough */
		    if (Bytes > REWIND_THRESHOLD) {
			/*   if (lseek (LinkId, 0L, L_SET) == -1L	) */
			if (lseek (LinkId, 0L,SEEK_SET) == -1L)
			    {
				cicsLogMessage(0,"\nRe	adLink read failed 2\n");
			/* 	printTime(); */
				ret = ER_LINK_CANT;
			    }
			Bytes = 0L;
		    }
		}
#endif	
#ifdef TRACE
	    printf ("ReadLink: %d bytes read\n", ret);

#endif	
	    semGive(tpSem);
	    return (ret);
	}
    
    cicsLogMessage(0,"ReadLink: couldn't get semaphore\n");
    return (ER_SEM_TIMEOUT);
 
   
}   


/* WriteLink --- */

int WriteLink (LinkId, Buffer, Count, Timeout)
     LINK LinkId;
     char *Buffer;
     unsigned int Count;
     int Timeout;
{
    register int ret;
    
#ifdef TRACE
    printf ("WriteLink: %d bytes requested %d\n", Count, Timeout);
#endif
    if(semTake(tpSem,sysClkRateGet()) != ERROR)
	{
   
	    if (LinkId != ActiveLink)
		{
		    cicsLogMessage(0,"WriteLink: ERROR ActiveLink");
		   /*  printTime(); */
		    semGive(tpSem);
		    return (ER_LINK_BAD);
		}
	    
	    if (Count < 1)
		{
		    
		    cicsLogMessage(0,"WriteLink: ERROR count");
		  /*   printTime(); */
		    semGive(tpSem);
		    return (ER_LINK_CANT);
		}
	    
	    if (!SetTimeout (LinkId, Timeout))
		{
		    
		    cicsLogMessage(0,"WriteLink: ERROR time out");
		  /*   printTime(); */
		    semGive(tpSem);
		    return (ER_LINK_CANT);
		}
	   
	    ret = write (LinkId, Buffer, Count);

	    
	    if (ret == -1)
		{
		    
		    cicsLogMessage(0,"WriteLink: ERROR write");
		  /*   printTime(); */
		    semGive(tpSem);
		    ret = ER_LINK_CANT;
		}
	    else {
		Bytes += (long int)ret;
		
		/* Rewin	d if we've sent enough */
		if (Bytes > REWIND_THRESHOLD) {
		    printf("WriteLink: rewind\n");
		    
		    if (lseek (LinkId, 0L, L_SET) == -1L)
			{
			    
			    cicsLogMessage(0,"WriteLink: ERROR lseek");
			   /*  printTime(); */
			    ret = ER_LINK_CANT;
			}
		    
		    Bytes = 0L;
		}
	    }
	    
#ifdef TRACE	
	    printf ("WriteLink: %d bytes written\n", ret);
#endif	
	    
	    semGive(tpSem);
	    return (ret);
	}
    cicsLogMessage(0,"WriteLink: tp semaphore timeout\n");
    return (ER_SEM_TIMEOUT);
}


/* SetTimeout --- set the timeout duration */

static BOOL SetTimeout (LinkId, Timeout)
     LINK LinkId;
     int Timeout;
{
    union IMS_IO io;
    static int TheCurrentTimeout = -1;
    if (Timeout != TheCurrentTimeout) 
	{
	    io.set.op = SETTIMEOUT;
	    io.set.val = Timeout;
#if	defined(vxWorks)	
	    if (ioctl (LinkId, SETFLAGS, (int)&io) == -1)		
#else	
	    if (ioctl (LinkId, SETFLAGS, &io) == -1)
		   
			
#endif	
	 
		return (FALSE);
	
	    TheCurrentTimeout = Timeout;
	}
    return (TRUE);

}


/* ResetLink --- */

int ResetLink (LinkId)
     LINK LinkId;
{
    union IMS_IO io;
    if(semTake(tpSem,sysClkRateGet()) != ERROR)
	{
	    if (LinkId != ActiveLink)
		{   
		    semGive(tpSem);
		    return (ER_LINK_BAD);
		}
	   
	   
	    io.set.op = RESET;
	   
#if	defined(vxWorks)
	    if (ioctl (LinkId, SETFLAGS, (int)&io) == -1) 
#else	
	    if (ioctl (LinkId, SETFLAGS, &io) == -1) 
#endif	
		{
		    
#ifdef DB
		    printf ("errno = %d\n", errno);
#endif	
		    semGive(tpSem);
		    return (ER_LINK_CANT);
		}
		    
	    semGive(tpSem);
	    return (SUCCEEDED);
	    
	}
    cicsLogMessage(0,"ResetLink: tp semaphore timeout\n");
    return (ER_SEM_TIMEOUT);
       
}


/* AnalyseLink --- */

int AnalyseLink (LinkId)
     LINK LinkId;
{
    union IMS_IO io;
    if(semTake(tpSem,sysClkRateGet()) != ERROR)
	{
	    if (LinkId != ActiveLink)
		{		  
		    semGive(tpSem);  
		    return (ER_LINK_BAD);
		}
	   
	   
	    io.set.op = ANALYSE;
	  
#if	defined(vxWorks)
	    if (ioctl (LinkId, SETFLAGS, (int)&io) == -1) 
	       
#else		
	    if (ioctl (LinkId, SETFLAGS, &io) == -1) 
			
#endif	
		{
		    
#ifdef DB
		    printf ("errno = %d\n", errno);
#endif		
		    semGive(tpSem);  
		    return (ER_LINK_CANT);
		}
	    
	    semGive(tpSem);  
	    return (SUCCEEDED);
	}

    cicsLogMessage(0,"AnalyseLink: tp semaphore timeout\n");
    return (ER_SEM_TIMEOUT);
} 


/* TestError --- */

int TestError (LinkId)
     LINK LinkId;
{
    union IMS_IO io;
    if(semTake(tpSem,sysClkRateGet()) != ERROR)
	{
	    if (LinkId != ActiveLink)
		{		   
		    semGive(tpSem);  
		    return (ER_LINK_BAD);
		}
	   
	   
#if	defined(vxWorks)
	    if (ioctl (LinkId, READFLAGS, (int)&io) == -1) 
#else	
	    if (ioctl (LinkId, READFLAGS, &io) == -1) 
#endif
		{
			
#ifdef DB	
		    printf ("errno = %d\n", errno);
#endif			
		    semGive(tpSem);  
		    return (ER_LINK_CANT);
		}
	       
		semGive(tpSem);  
		return ((int) io.status.error_f);
	    }
   
    cicsLogMessage(0,"CloseLink: tp semaphore timeout\n");
    return (ER_SEM_TIMEOUT);
   
	   
}  


/* TestRead --- */

int TestRead (LinkId)
     LINK LinkId;
{
    union IMS_IO io;
    if(semTake(tpSem,sysClkRateGet()) != ERROR)
	{
	 
	    if (LinkId != ActiveLink)
		{
		    semGive(tpSem);
		
		    
		    return (ER_LINK_BAD);
		}
	   
   
  
#if	defined(vxWorks)
	    if (ioctl (LinkId, READFLAGS, (int)&io) == -1) 
#else
       	    if (ioctl (LinkId, READFLAGS, &io) == -1) 
#endif	
		{
		    
#ifdef DB
		    printf ("errno = %d\n	", errno);
#endif
			   
		    semGive(tpSem);
		    
		    return (ER_LINK_CANT);
		}
	    semGive(tpSem);
		  
	    return ((int) io.status.read_f); 
	}
   
    cicsLogMessage(0,"TestRead: tp semaphore timeout\n");
   
    return (ER_SEM_TIMEOUT);
}
   
   
/* TestWrite --- */

int TestWrite (LinkId)
     LINK LinkId;
{
    union IMS_IO io;
    if(semTake(tpSem,sysClkRateGet()) != ERROR)
	{
	    if (LinkId != ActiveLink)
		{
		    semGive(tpSem);   
		    return (ER_LINK_BAD);
		}
	   

#if	defined(vxWorks)
	    if (ioctl (LinkId, READFLAGS, (int)&io) == -1) 
#else	
	    if (ioctl (LinkId, READFLAGS, &io) == -1) 
#endif
		{
		    
#ifdef DB
		    printf ("errno = %d\n", errno);
#endif	
		    semGive(tpSem);  
		    return (ER_LINK_CANT);
		}
	    
	    semGive(tpSem);  
	    return ((int) io.status.write_f);
	}
   
    cicsLogMessage(0,"CloseLink: tp semaphore timeout\n");
    return (ER_SEM_TIMEOUT);
    
}
