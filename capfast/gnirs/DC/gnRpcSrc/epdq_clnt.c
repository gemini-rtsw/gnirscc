

#if     defined(VXWORKS)
#include        <stddef.h>
#include        <string.h>
#else
#include        <stdio.h>
#endif

/*
 *This file contains the low level rpc code that runs on the client side.  It 
 * shouldn't be modified.  If more functions need to be added, cut and paste
 * from one of the existing routines, and change the name and the second 
 * parameter of clnt_call to match the new routine. These changes need to be
 * reflected in (epdq.h, epdq_svc.c gnRpcServer.c gnRpcClient.c )
 */

#include <rpc/rpc.h>
#include <epdq.h>

/* Default timeout can be changed using clnt_control() */
static struct timeval TIMEOUT = { 25, 0 };

int *
dqobserverpc_1(argp, clnt)
	int *argp;
	CLIENT *clnt;
{
	static int res;

	bzero((char *)&res, sizeof(res));
	if (clnt_call(clnt, DQOBSERVERPC, xdr_int, argp, xdr_int, &res, TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&res);
}

int *
dqobssetuprpc_1(argp, clnt)
	int *argp;
	CLIENT *clnt;
{
	static int res;

	bzero((char *)&res, sizeof(res));
	if (clnt_call(clnt, DQOBSSETUPRPC, xdr_int, argp, xdr_int, &res, TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&res);
}

int *
dqobsstatusrpc_1(argp, clnt)
	int *argp;
	CLIENT *clnt;
{
	static int res;

	bzero((char *)&res, sizeof(res));
	if (clnt_call(clnt, DQOBSSTATUSRPC, xdr_int, argp, xdr_int, &res, TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&res);
}

int *
dqobsabortrpc_1(argp, clnt)
	int *argp;
	CLIENT *clnt;
{
	static int res;

	bzero((char *)&res, sizeof(res));
	if (clnt_call(clnt, DQOBSABORTRPC, xdr_int, argp, xdr_int, &res, TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&res);
}

int *
dqobsstoprpc_1(argp, clnt)
	int *argp;
	CLIENT *clnt;
{
	static int res;
 
	bzero((char *)&res, sizeof(res));
	if (clnt_call(clnt, DQOBSSTOPRPC, xdr_int, argp, xdr_int, &res, TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&res);
}

int *
dqrebootrpc_1(argp, clnt)
	int *argp;
	CLIENT *clnt;
{
	static int res;

	bzero((char *)&res, sizeof(res));
	if (clnt_call(clnt, DQREBOOTRPC, xdr_int, argp, xdr_int, &res, TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&res);
}

int *
dqdhsconnectionrpc_1(argp, clnt)
        int *argp;
        CLIENT *clnt;
{
        static int res;

        bzero((char *)&res, sizeof(res));
        if (clnt_call(clnt, DQDHSCONNECTIONRPC, xdr_int, argp, xdr_int, &res, TIMEOUT) != RPC_SUCCESS) {
                return (NULL);
        }
        return (&res);
}

