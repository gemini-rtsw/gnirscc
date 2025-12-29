#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdarg.h>


#include <epicsPrint.h>
#include <ellLib.h>
#include <semLib.h>
#include "drvSerial.h" 
#include "drvAscii.h" 

int myGetFrame( FILE *fp, char *fName, int maxLen, char *bfr );


int myGetFrame( FILE *fp, char *fName, int maxLen, char *bfr ) {
  int             	chr;
  int                   idx = 0;
  int			test=FALSE;
  int			i;

  /* 
   * Parse out token until we see the command terminator
   */  
  while ( idx < maxLen ) {
    
    chr = getc( fp );
    
    if ( chr == EOF ) {
      printf("EOF Found\n");
      return EOF;
    }

    /*  
     *  strip null characters as these cause succeeding 
     *  scanf calls to fail
     */
    if ( chr == 0 ) continue;
    
    bfr[idx++] = chr;
    bfr[idx] = 0;
    printf("%d\n", bfr[idx-1]);

    if ( (uint8_t)chr == 0xD ) {
      test = TRUE; 
      printf("0xD\n");
    }

    if (test && ((uint8_t)chr == 0xA)) {
      printf("test&0xA\n");
      break;
    }
  }

  printf(">>>");
  for (i = 0; i < strlen(bfr); i++) {
    printf("%d ", bfr[i]);
    /*     printf("%c ", bfr[i]); */
  }
  printf("<<<\n");

  if ( idx >= maxLen ) {
    /*
     *  message buffer over flow (ignore this frame)
     */

/*    errPrintf( S_drvAscii_dataErr, __FILE__, __LINE__,
	       " : [%s] buffer over flow!\n", 
	       (int)fName );
  */
      epicsPrintf(__FILE__, __LINE__,
	       " : [%s] buffer over flow!\n", 
	       (int)fName );
  
    return idx;
  }

  idx++;

  return idx;
}
