static struct {
        void *v;
        char *c;
} rcsid = {
        &rcsid,
        "$Id: putWFMsg.c,v 1.2 2009/05/27 19:33:38 fkraemer Exp $"
};
/* Work on a routine to transmit WFire messages; for use with WFireDev code.
 * Mar97, jan@noao.edu
 */

/* See sendtp() for a start. */


/* Also see an early version of send(); there is a tags file in this directory
 * which points to the appropriate version of message.c.
 */

#include	<vxWorks.h>
#include	<stdio.h>
#include	<string.h>
#include	"wFireMsgDefs.h"

#include	<.applTop/tpSource/include/protocol.h>
#include <cicsLib.h>

extern	int	ReadLink( int, char *, int, int ) ;
extern	int	WriteLink( int, char *, int, int ) ;
extern	int	OpenLink( char * ) ;
extern	int	TestRead( int ) ;

int	putWFMsg( int, int, int * ) ;
int	getWFMsg( int, int *, int * ) ;
void	lstWFMsg( FILE *, int, int * ) ;
void wfm_byte_swap( char *, int ) ;
static	char *to_int( char *, int * ) ;

/* Here is a global variable so other routines can put this code in 
 * verbose mode.
 */
int	putWFMsgVerbose = 0 ;

/* Contrary to the precedent set in send() and sendtp(), neither the input
 * header nor array is byte swapped.
 */

#define putTimeout 2
    int
putWFMsg( int fd, int head, int array[] )
{
    /* Define local copies of header and array so they can be byte swapped. */
    int	header = head, arr[MAXBUFLEN] ;
    int	len = LENGTH(head)*sizeof(int) ;
    int	n ;
 
    /* We expect link to be open. */

    /* Check for message to be read. */
    if( TestRead( fd ) ){
	if( putWFMsgVerbose ) {
	    printErr( "In putWFMsg(): " ) ;
	    printErr( "There is a message to read.\n" ) ;
	}
    }

    /* Write the header and the data array. */

    wfm_byte_swap( (char *)&header, sizeof(int) );
    /* Check for error? */
    n = WriteLink(fd, (char *)&header, sizeof(int), putTimeout);
    if( n != sizeof(int) ) {
	printErr( "In putWFMsg(), WriteLink( ... %d, ... ) returned %d.\n",
	    sizeof(int), n ) ;
	return( ERROR ) ;
    }
    bcopy( (char *)array, (char *)arr, MAXBUFLEN*sizeof(int) ) ;

    wfm_byte_swap( (char *)arr, len );
    if (len >0)
      {
	n = WriteLink(fd, (char *)arr, len, putTimeout);
	if( n != len ) 
	  {
	    printErr( "In putWFMsg(), WriteLink( ... %d, ... ) returned %d.\n",
		      len, n ) ;
	    return( ERROR ) ;
	  }
      }

    /* Looks as if all is well. */
    return( OK ) ;
}

#define getTimeout 1
    int
getWFMsg( int fd, int *header, int arr[] )
{
    int	n, len, msgHeader ;

	
	    
    if( !TestRead( fd ) ){
	return( OK ) ;
    }

    if( header == (int *)NULL ){
	printErr( "In getWFMsg(), no message buffer supplied.\n" ) ;
	return( ERROR ) ;
    }

    /* Read the header.
     * Note the use of arr[] as a read buffer.
     */
    n = ReadLink(fd, (char *)arr, sizeof(int), getTimeout);
    if( n != sizeof(int) ) {
	printErr( "In getWFMsg(), ReadLink( ... %d, ... ) returned %d.\n",
	    sizeof(int), n ) ;
	return( ERROR ) ;
    }

    /* Convert header format. */
    (void)to_int((char *)arr, (int *)&msgHeader);
    /* Send it to caller. */
    *header = msgHeader ;

    /* Read the array. */
    len = LENGTH(msgHeader)*sizeof(int) ;
    if( len <= 0 ) {
	printErr( "In getWFMsg(), array length is %d.\n", LENGTH(msgHeader) ) ;
	len = 0 ;
    }
    if( putWFMsgVerbose ) 
	printf ("getWFMsg: length = %d\n",len); 
    n = ReadLink(fd, (char *)arr, len, getTimeout);
    if( n != len ) {
	printErr( "In getWFMsg(), ReadLink( ... %d, ... ) returned %d.\n",
	    len, n ) ;
	return( ERROR ) ;
    }
    /* Byte swap the data array. */
    wfm_byte_swap( (char *)arr, len );

 
   
    return( OK ) ;
}

/* Neither header nor arr[] is byte swapped. */
    void
lstWFMsg( FILE *ifp, int header, int arr[] )
{
   /*  float f; */
 
     char tmp[(MAXBUFLEN*sizeof(int))+100]="\0", tmp2[100] ; 
     int	i, j ; 
     char convert = 'd' ; 



    /* Folowing plagarized from receive() in
     * /repos2/SAAwfire/src/sun/control/message.c
     */
    switch (MESSAGE(header)) 
    {
      case READ_VAR:
	   i = arr[VAR_NUM] ; 
 	  j = arr[REPLY_TO] ; 
	  sprintf(tmp, "READ_VAR: VAR_NUM=%d, REPLY_TO=%d", i, j);
	  if( LENGTH(header) == 3 ) {
	      sprintf(tmp2, ", LENGTH_TO_SEND=%d", arr[LENGTH_TO_SEND]);
	      (void)strcat( tmp, tmp2 ) ;
	  }
	  break;
      case VAR_READ:
	  switch (convert) 
	  {
	    case 'd':
		sprintf(tmp, "VAR_READ: VAR_NUM=%d, VAR_VAL=%d\n",
			arr[VAR_NUM], arr[RETURN_VAL] ) ;
		break;
#if 0
	    case 's': 
		to_str(buf + RETURN_VAL * 4, &str); 
		sprintf(tmp, "%s", str); 
		/* 	strcat(interp->result, tmp); */
		break;
	    case 'f': 
		to_float(arr + RETURN_VAL * 4, &f); 
		sprintf(tmp, "%f", f); 

		/* 	strcat(interp->result, tmp); */
		break;
	    case 'x':
		to_int(buf + RETURN_VAL * 4, &i);
		sprintf(tmp, "0x%8.8x", i);
		/* 	strcat(interp->result, tmp); */
		break;
#endif
	    default:
		sprintf(tmp, "bad format option '%c'",convert);
		cicsLogMessage(0,tmp);
		break;
	  }
		break;
		
      case DEBUG_MSG: 
      {
	  /* The input data array has been byte swapped.  It appears that
	   * character strings from the transputer are in "the usual order".
	   * Thus, I think the character data needs to be byte swapped
	     * back to the original order.
	     */
	  int	buf[MAXBUFLEN] ;
	  int	len = LENGTH(header)*sizeof(int) ;

	  bcopy( (char *)arr, (char *)buf, MAXBUFLEN*sizeof(int) ) ;
	  wfm_byte_swap( (char *)buf, len );
	  
	  sprintf(tmp, "lstWFMsg:DEBUG_MSG: %s", (char *)buf);
	  break;
      }
       
      case AKK:
	  sprintf(tmp, "AKK");
	  break;
      case AKK_FAIL:
	  sprintf(tmp, "NAK");
	  break;
      case NEXT_MSG:
	  sprintf(tmp, "NeXT");
	  break;

      case SET_VAR:
	  
	  /*seq and icon cases of each variable?????????????*/
	  
	  if (FROM_WHERE(header)== SEQ)
	  {
	      switch (VAR_NUMVAL(arr))
	      {
		case 7:
		case 8:
		{
		    double	d = *(float*)&arr[VAR_VAL] ;
		 
		    sprintf(tmp, "SET_VAR: VAR_NUM=%d, VAR_VAL=%f\n",
			    arr[VAR_NUM], d ) ;
		    break;
		}
		case 0:/*trace flag*/
		case 1:/*prog*/
		case 2:/*frames*/
		case 4:/*ctrl reg*/
		case 5:/*lnr*/
		case 6:/*coadds*/
		case 9:/*spad filter*/
		case 11:/*quadrant*/
		case 12:/*n davg*/
		case 13:/*roi size*/
		case 14:/*var1*/
		case 15:/*var2*/
		case 16:/*var3*/
		case 17:/*var4*/
		{
	
		    sprintf(tmp, "SET_VAR: VAR_NUM=%d, VAR_VAL=%d\n",
			    arr[VAR_NUM], arr[VAR_VAL] ) ;
		    break;		
		}
		default:
		    sprintf(tmp, "Unknown message type, %d from %d",
			    MESSAGE(header), FROM_WHERE(header));
		    break;	
		    
	      }
	  }
	  else if (FROM_WHERE(header) == ICON)
	  {
	      switch(VAR_NUMVAL(arr))
	      {
		case 0:/*trace flag*/
		case 1:/*num arrays*/
		case 2:/*echo me*/
		case 3:/*status report*/
		case 4:/*deactivate*/
		case 6:/*protection*/
		case 7:/*scb reg*/
		case 8:/*cp state*/
		case 10:/*servo ctrl*/
		case 11:/*lcd ctrl*/
		case 12:/*a2d freeze*/
		case 13:/*spad mode*/
		case 14:/*wheel pos*/
		{
		
		    sprintf(tmp, "SET_VAR: VAR_NUM=%d, VAR_VAL=%d\n",
			    arr[VAR_NUM], arr[VAR_VAL] ) ;
		    break; 	
		}
		case 9:/*arrayd2a*/
		{
		    double	d = *(float *)&arr[ISUBVAR_VAL] ;
		    sprintf(tmp,
			    "SET_VAR: VAR_NUM=%d, ARR_INDEX=%d, D2A_GROUP=%d, D2A_INDEX=%d, VAR_VAL=%f",
			    VAR_NUMVAL(arr), 
			    ARR_INDEX(arr), 
			    D2A_GROUP(arr), 
			    D2A_INDEX(arr), 
			    d ) ;
		}
		default:
		    sprintf(tmp, "Unknown message type, %d from %d",
			    MESSAGE(header), FROM_WHERE(header));
		    break;
		   
	      }
	  }
    }
  
 


}

    void
wfm_byte_swap(char *s, int len)
{   
    /* change the order of the bytes from 1234 to 4321 for every four bytes*/
    /*  "abcdefghijkl" would be "dcbahgfelkji"*/

   
	    
	
	    
    char *d = s + 3;
    char t;
 
    for ( ; len > 0; len -= 4) {
	t = *s;
	*s = *d;
	*d = t;

	s++; d--;

	t = *s;
	*s = *d;
	*d = t;

	s += 3;
	d += 5;
    }
}

/******************************************************************************
 * Routine: to_int
 * Purpose: 
 * Inputs:
 * Returns: char * -- position after the last one converted
 * 
 ******************************************************************************/

    static char *
to_int(char *s, int *i)
{
    wfm_byte_swap(s, 4);

    *i = *((int *) s);

    return s + 4;
}
