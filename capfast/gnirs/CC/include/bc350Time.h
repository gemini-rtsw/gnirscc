/*****************************************************************************
*
*    Defines for these programs 
*
******************************************************************************/
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

struct time_struct {
          int day;               /* day of the year 1 - 366 */
          int hour;
          int min;
          int sec;
          int msec;              /* micro seconds to 10E-7  */
};

/*****************************************************************************
*
*      Routines in the BanCom Control program file
*
******************************************************************************/
extern void   printTime (void);
extern int    setDate (int mon, int day, int year);
extern int    setTime (int hour, int min, int sec);
extern int    initTime (int uT);
extern struct time_struct *getTime(struct time_struct *timeSt);
extern struct time_struct *getUTime(struct time_struct *timeSt);
