#include <vxWorks.h>
#include <pingLib.h>
#include <taskLib.h>

void pinger(int *host, int interval) {
   
   for (;;) {
      if (ping ((char*)host,3,PING_OPT_SILENT) != OK ) {
         printf ("connection to %s lost\n",(char*)host);
         };
      taskDelay(60*interval);
      
      }
   }
