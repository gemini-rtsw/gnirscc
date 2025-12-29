
/* date */
void epdq_execute();

/*
 * This file contains the low level rpc code that runs on the server side.  It 
 * shouldn't be modified.  If more functions need to be added, add to the union
 * in epdqprog_1 and add a case statement for the new routine. they need to be
 * reflected here and  in (epdq.h, epdq_clnt.c gnRpcServer.c gnRpcClient.c )
 */

#if     defined(VXWORKS)
#include <stdioLib.h>
#include <stdlib.h>
#include <rpcLib.h>
#include <rpc/pmap_clnt.h>
#include <string.h>
#else
#include <stdio.h>
#endif
#include <rpc/rpc.h>
#include <epdq.h>
#include "cicsLib.h"
static void epdqprog_1();

#if     defined(VXWORKS)
void epdq_server() { rpcTaskInit(), epdq_execute() ; }
void epdq_execute()
#else
void main()
#endif
{
	register SVCXPRT *transp;

	(void) pmap_unset(EPDQPROG, EPDQVERS);

	transp = svctcp_create(RPC_ANYSOCK, 0, 0);
	if (transp == NULL) 
	  {
	    cicsLogMessage(0, "cannot create tcp service.");
	    exit(1);
	  }
	if (!svc_register(transp, EPDQPROG, EPDQVERS, epdqprog_1, IPPROTO_TCP)) 
	  {
	    cicsLogMessage(0, "unable to register (EPDQPROG, EPDQVERS, tcp).");
	    exit(1);
	  }

	printf( " rpc service running\n");
	svc_run();
	cicsLogMessage(0, "svc_run returned");
	exit(1);
	/* NOTREACHED */
}

static void
epdqprog_1(rqstp, transp)
	struct svc_req *rqstp;
	register SVCXPRT *transp;
{
	dqargument argument;
	char *result;
	bool_t (*xdr_argument)(), (*xdr_result)();
	char *(*local)();

        /*printf("antes del switch\n");	 */
	switch (rqstp->rq_proc) {
	case NULLPROC:
		(void) svc_sendreply(transp, xdr_void, (char *)NULL);
		return;

	case DQOBSERVERPC:
		xdr_argument = xdr_int;
		xdr_result = xdr_int;
		local = (char *(*)()) dqobserverpc_1;
		break;

	case DQOBSSETUPRPC:
		xdr_argument = xdr_int;
		xdr_result = xdr_int;
		local = (char *(*)()) dqobssetuprpc_1;
		break;

	case DQOBSSTATUSRPC:
		xdr_argument = xdr_int;
		xdr_result = xdr_int;
		local = (char *(*)()) dqobsstatusrpc_1;
		break;

	case DQOBSABORTRPC:
		xdr_argument = xdr_int;
		xdr_result = xdr_int;
		local = (char *(*)()) dqobsabortrpc_1;
		break;

	case DQOBSSTOPRPC:
		xdr_argument = xdr_int;
		xdr_result = xdr_int;
		local = (char *(*)()) dqobsstoprpc_1;
		break;
        case DQREBOOTRPC:
		xdr_argument = xdr_int;
		xdr_result = xdr_int;
		local = (char *(*)()) dqrebootrpc_1;
		break;

        case DQDHSCONNECTIONRPC:
                printf("in epdq_svc, DQDHSCONNECTIONRPC \n");
                xdr_argument = xdr_int;
                xdr_result = xdr_int;
                local = (char *(*)()) dqdhsconnectionrpc_1;
                break;

	default:
		svcerr_noproc(transp);
		return;
	}
	bzero((char *)&argument, sizeof(argument));
	if (!svc_getargs(transp, xdr_argument, &argument)) {
		svcerr_decode(transp);
		return;
	}
	result = (*local)(&argument, rqstp);
	if (result != NULL && !svc_sendreply(transp, xdr_result, result)) {
		svcerr_systemerr(transp);
	}
	if (!svc_freeargs(transp, xdr_argument, &argument)) {
		cicsLogMessage(0, "unable to free arguments");
		exit(1);
	}
	return;
}
