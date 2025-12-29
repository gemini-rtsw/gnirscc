/* date */

/*
 * This file shouldn't be modified. if more functions want to be added, create
 * a new define with a unique number(probably one more than the last).  Also, 
 * create a prototype for it. These changes need to be
 * reflected in (epdq_clnt.c, epdq_svc.c gnRpcServer.c gnRpcClient.c )
 */

#if     defined(VXWORKS)
#include <rpc/rpctypes.h>
#else
#include <rpc/types.h>
#endif


#define EPDQPROG ((u_long)99)
#define EPDQVERS ((u_long)1)

/* rpc to start taking a frame*/
#define DQOBSERVERPC ((u_long)1)
extern int *dqobserverpc_1();

/* rpc to start setup procedures on data coadder*/
#define DQOBSSETUPRPC ((u_long)2)
extern int *dqobssetuprpc_1();


#define DQOBSSTATUSRPC ((u_long)3)
extern int *dqobsstatusrpc_1();

/* rpc to abort an observe*/
#define DQOBSABORTRPC ((u_long)4)
extern int *dqobsabortrpc_1();

/* rpc to stop an observe*/
#define DQOBSSTOPRPC ((u_long)5)
extern int *dqobsstoprpc_1();

/* rpc to stop an observe*/
#define DQREBOOTRPC ((u_long)6)
extern int *dqrebootrpc_1();

/* rpc to dhsconnection */
#define DQDHSCONNECTIONRPC ((u_long)7)
int *dqdhsconnectionrpc_1(int *argp,CLIENT *clnt);

