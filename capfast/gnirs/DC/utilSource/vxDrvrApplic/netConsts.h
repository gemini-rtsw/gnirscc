
/* tcpExample.h - testing TCP server and client */

#define SERVER_PORT_NUM (7000)
#define MSG_SIZE (1024)

/* request structure */
struct request 
{
   int reply;          /* if TRUE client expects reply from server */
   int msgLen;         /* length of message text */
   char message[MSG_SIZE]; /* message buffer */
 };

