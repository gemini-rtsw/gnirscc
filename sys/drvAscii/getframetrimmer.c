#include <stdio.h>
#include <ctype.h>

int getFrameNoSpace( FILE *fp, char *fName, int maxLen, uint8_t *bfr )
{
  uint8_t         	*pC, *ptr;
  int             	chr;
  int                   count = 0,
                        done = FALSE;

  /* 
   *  Note that this function bypasses the link's line termination
   *  processing and, in fact, terminates when <cr> is received. If
   *  you need the <lf> as well then you will have to modify the code. 
   */
  pC = bfr;
  
  while ( (char *)pC < (char *)(bfr + maxLen - 1)  ) {
    
    chr = getc( fp );
    
    if ( chr == EOF ) 
      return count;
      
    /*  
     *  strip null characters as these cause succeeding 
     *  scanf calls to fail
     */
    if ( (char)chr == '\0' ) continue;
          
    /*
     *  If a carriage return was just receive then delete any 
     *  previous whitespace characters.
     */
    if ( (char)chr == 0xd ) {
      
      ptr = pC;
      
      while ( isspace( *ptr ) ) ptr--;
      
      pC = ptr;
      
      done = TRUE;  /* end on <cr> */
      
    } 

    *(pC++) = (uint8_t) chr;
    *pC = '\0';
    count++;
    
    if ( done )
      break;
  }
    
  return count; 
  
}
