/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#ifndef _LOCAL_BROKER_RPC_H_RPCGEN
#define _LOCAL_BROKER_RPC_H_RPCGEN

#include <rpc/rpc.h>


#ifdef __cplusplus
extern "C" {
#endif


#define MESSAGEPROG 0x20000000
#define MESSAGEVERS 1

#if defined(__STDC__) || defined(__cplusplus)
#define PRINTMESSAGE 1
extern  int * printmessage_1(char **, CLIENT *);
extern  int * printmessage_1_svc(char **, struct svc_req *);
extern int messageprog_1_freeresult (SVCXPRT *, xdrproc_t, caddr_t);

#else /* K&R C */
#define PRINTMESSAGE 1
extern  int * printmessage_1();
extern  int * printmessage_1_svc();
extern int messageprog_1_freeresult ();
#endif /* K&R C */

#ifdef __cplusplus
}
#endif

#endif /* !_LOCAL_BROKER_RPC_H_RPCGEN */
