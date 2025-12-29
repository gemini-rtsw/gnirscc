/* reset.c */



 
#include "reset.h"
 
#define TNET_TASK_NAME  "tTnet"
#define TNET_TASK_OPT   (VX_SUPERVISOR_MODE | VX_UNBREAKABLE | VX_STDIO)
 
static int      tnetOpen (char *name, unsigned short port);


/*******************************************************************************
*
* Description
*  This routine is run from the sun, it connects to the reset module 
*  through a remote annex and sends a string that tells the module to reset 
*  the vme that it is connected to
*
* Parameters
*  argc[1]  name of remote annex .  This is the name  of the
*               remote annex that you're trying to talk to.

*  argc[2]   the strings ("coadd", "epics", or "both") without the quotes
*  Which side of the vme to reset.
*
*
*
*  
*
* RETURNS
*  
*/


int main (int argc, char *argv[])		
{
  int sd;
  int n,i;
  char machine[80];
  char command[80];
  char buf[80];
  int val;
  unsigned short port;
  int a,b; 
  u_long addr;
  struct hostent *hEnt;
  struct sockaddr_in in;

  printf ("%d\n",argc);
  if (argc <=2)  
    {
      printf(" usage:  gnReset {annex} {machine}       \n possible machines\n\t %s = reboot dc coadd\n\t%s = reboot dcepics\n\t%s = reboot dcboth\n\t%s = reboot waveFront sensor\n\t%s = reboot components controller\n\n", COADD,EPICS,BOTH,WFS,CC);
      return(0); 
    }
  strcpy(machine,argv[2]);
  
  /* change letters to lowercase*/
  for (i=0;i<strlen(machine);i++)
    machine[i] = tolower(machine[i]);
  
  printf("%s\n",machine);
  /* set command string depending on what 'machine' */
  if (strcmp (COADD,machine) == 0)/* connected to port 7008 Do3*/
    {
      printf ( "rebooting coadd\n");  
      sprintf (command,"%s08%c",RESET_STRING,13);
	  port = PORT1;
    }
  else if  (strcmp (EPICS,machine)==0)/* connected to port 7008 Do5*/
	{
	  printf ( "rebooting epics\n"); 
	  sprintf (command,"%s20%c",RESET_STRING,13);
	  port = PORT1;
	}
  else if (strcmp (BOTH,machine)==0)
	{
	  printf ( "rebooting coadd and epics\n"); 
	  sprintf (command,"%s2851%c",RESET_STRING,13); 
	  port = PORT1;
	}
  else if  (strcmp (WFS,machine)==0)/* connected to port 7004 (serial port 3)
									Do1*/
	{
	  printf ( "rebooting WFS\n"); 
	  sprintf (command,"%s01%c",RESET_STRING,13);
	  port = PORT2;
	}
  else if  (strcmp (CC,machine)==0)/* connected to port 7004 (serial port 3)
									Do0*/
	{
	  printf ( "rebooting CC\n"); 
	  sprintf (command,"%s02%c",RESET_STRING,13);
	  port = PORT2;
	}
  else
	{
	  printf("usage:   reset {annex name} {%s %s %s %s %s}; ex: reset pancake epics", COADD,EPICS,BOTH,WFS,CC);  
	  return(0);  
	}
  
  printf("%s, %d %s\n",argv[1], port,command);
  printf("opening socket\n"); 
   /* open network connection */
   if ((sd = tnetOpen (argv[1],port)) == ERROR)
     {
       printf("Couldn't open socket to reset box\n");
       return;
     }  
   
  
 
   
  write (sd, command, strlen(command));/* this should reset system*/ 
   sleep (1);   
  n = read (sd, buf, 80); 
    sleep(1);      

  sprintf (command,"%s0047%c",RESET_STRING,13); 
  printf("rebooting\n");
   write (sd, command, strlen(command));    
    printf("after write\n");
    sleep(1);  
   n = read (sd, buf, 80);    
  printf("done\n");
  
  close (sd);
}


/******************************************************************************* 
* tnetOpen
*/

static int tnetOpen (char *name, unsigned short port)		 
{
  struct sockaddr_in sin;
  int one = 1;
  int prtMsg = TRUE;
  int sd;
  unsigned long ipAddrs;
/*  unsigned short port; */
 struct hostent *hEnt;

 sd = ERROR;
 while (sd == ERROR) 
 { 
     /* configure socket */
     if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == ERROR)
     {
	 perror("socket error 1");
	 break; 
		 
     } 
/*      port = PORT;   */
     hEnt = gethostbyname(name);  
     if (hEnt == NULL)  
     { 
	 (void) printf("host information for %s not found\n", name); 
	 exit (3); 
     } 

     /* set values in sockadr_in with values returned in hostent*/
     bzero ((char *) &sin, sizeof (sin));
     (void) memcpy(&sin.sin_addr, hEnt->h_addr, hEnt->h_length); 
     setsockopt (sd, SOL_SOCKET, SO_KEEPALIVE, (char *) &one, sizeof (one));
     /* configure inet */
     sin.sin_family = AF_INET;
     sin.sin_port = htons (port);
     if (connect (sd, (struct sockaddr *) &sin, sizeof (sin)) == ERROR)
     {
	 close (sd); 
	 perror("socket error");
	 sd = ERROR; 
     }
 } 

 return sd;
}

