/*****************************************************************************
 * standard header file
 *
 * Copyright NOAO February 1 1996
 * Author Nick C. Buchholz
 *
 ****************************************************************************/

#if !defined(__link_sock)
#define __link_sock

#define REPLY_MSG_SIZE		(512)
#define SERVER_PORT_NUM 	(7000)  

#if 0
#define SERVER_INET_ADDR	"140.252.31.232"  /* gerbil*/
#endif
#define SERVER_INET_ADDR        "140.252.31.207" /*romeo*/

#define RESET_TP_NET 		1
#define ANALYZE_TP_NET		2
#define GET_TP_ERROR		3
#define GET_READ_STAT		4
#define GET_WRITE_STAT		5

#define MSG_SIZE (1024)

#endif
