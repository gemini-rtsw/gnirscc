

static struct {
	void *v;
	char *c;
} rcsid = {
	&rcsid,
	"$Id: openprt2.c,v 1.2 2009/05/27 19:33:20 fkraemer Exp $"
};
#include <stdioLib.h>
#include <ioLib.h>
#include <usrLib.h>
#include <taskLib.h>
#include <drv/serial/z8530.h>
#include <cicsLib.h>
#include "ipOctal.h"

int setdtr(int, unsigned char);
int Port2 = 0;
int Port;

/* open port command from vxworks shell*/
int openport2()
{
  
   
  if ((Port2 = open("/tyCo/1", UPDATE, 0)) == 0)
    { 
      cicsLogMessage(0,  "Opening Temp PORT failed");
	 
      Port2 = 0;
      return(ERROR);
    }
  if (ioctl(Port2, FIOBAUDRATE, 9600) == ERROR)
    { 
    
      cicsLogMessage(0, "Setting baud rate for Temp PORT failed");

    
      Port2 = 0;
      return(ERROR);
    }
  setdtr(0, '2');

  return(OK);
}

int setdtr(int dtr, unsigned char port)
{
  unsigned char *Padd;
  char tmp;

  switch (port)
    {
    case '2':
      Padd = (unsigned char *)(0xFFF45001);
      *Padd = SCC_WR0_SEL_WR5;       
      tmp = SCC_WR5_TX_EN | SCC_WR5_TX_8_BITS | SCC_WR5_RTS;
      if (dtr == 1)
	{
	  *Padd = SCC_WR5_DTR | tmp;
	  tmp = 0x0D;
	  write(Port, &tmp, 1);
	}
      else
	*Padd = tmp;
      break;

    case 'a':
    case 'b':
      if (dtr == 1)
	{
	  ioctl(Port, FIOSETRTS, RTS_ACTIVE);
	  tmp = 0x0D;
	  write(Port, &tmp, 1);
	}
      else
	ioctl(Port, FIOSETRTS, RTS_INACTIVE);
      break;

    default:
      if (dtr == 0)
	ioctl(Port, FIOSETRTS, RTS_ACTIVE);
      else
	{
	  ioctl(Port, FIOSETRTS, RTS_INACTIVE);
	  tmp = 0x0D;
	  write(Port, &tmp, 1);
	}
    }
  return(0);
}
