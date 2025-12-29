static struct {
        void *v;
        char *c;
} rcsid = {
        &rcsid,
        "$Id: getTime.c,v 1.2 2009/05/27 19:33:34 fkraemer Exp $"
};
#include <stdio.h>
#include <vxWorks.h>
#include <time.h>
#include <stdlib.h>
#include "vxSockUtil.h"
#define OK      0
#define BASE_1970    2208988800        /* amount to add to seconds past 1900 */
#define BASE 0xFFFFC000
#define SOH     0x01
#define ETB     0x17
#define EVENT0  (short*)(BASE+0x16)
#define CMD     (short*)(BASE+0x24)
#define VECTOR  (short*)(BASE+0x2C)
#define MASK    (short*)(BASE+0x28)
#define INTSTAT (short*)(BASE+0x2A)
#define FIFO    (char*)(BASE+0x27)
#define ACK     (short*)(BASE+0x22)

/******************************************************************************
 * Routine: getTime
 * Purpose: get the time from the server
 * Inputs:  none
 * Returns: time_t the seconds since 01/01/1970
 *
 ******************************************************************************/
void printtime()
{
    struct tm *tm;	
    time_t clock;     /* holds the time in seconds since some known time */
    int sfd;          /* sock discriptor    */
    char *address;    /* network address of server from which to get time */
    int timezone = -7;  /* hours between GMT and local time */
    int daylight = 0;   /* for daylight savings   */
    time_t adjust;      /* adjustment to time for timezone and daylight */
  
    adjust = (time_t)(timezone+daylight);
    adjust *= (60*60); 
    adjust ++;
    address = getenv("SERVER");
    if (address == NULL)
	address = "140.252.31.41"; 
    sfd = sockConnect(37, address);
    sockRead(sfd, (char *)&clock, 4);   /* time ret in secs since 01/01/1900 */
    clock -= BASE_1970;                  /* adjust tp 1970 */
    clock += adjust;           /* adjust to local time         */
    
    tm = gmtime(&clock);      /* put time into a struct tm    */
    /*  printf ("%d\n",clock); */

    printf ("\n day = %d,  %d:%d:%d\n",tm->tm_yday,tm->tm_hour,tm->tm_min,tm->tm_sec);
    
    /*  return (clock);  */
}


