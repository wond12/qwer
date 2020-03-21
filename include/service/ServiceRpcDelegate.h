#ifndef _H_SERVICE_RPC_DELEGATE_H_
#define _H_SERVICE_RPC_DELEGATE_H_

#include "../CTypeDefine.h"
#ifdef _USING_CLI
#else
#include <functional>
#endif

namespace QWER
{
	QTEMPLATE<typename T> QCLASS CPromise;
	QCLASS IServiceFrontend;
	QCLASS IServiceBackend;
	QCLASS IToServiceAdapterRpc;
	QCLASS IToServiceResolverRpc;
	//QSTRUCT SToAdapterRpcRet;
	//QSTRUCT SToResolverRpcRet;

#ifdef _USING_CLI
	QDLL_EXPORT delegate PTR(CPromise) DToServiceAdapterRpcReply(PTR(IServiceBackend) roResolver, PTR(IToServiceAdapterRpc) roRpc);
	QDLL_EXPORT delegate PTR(CPromise) DToServiceResolverRpcReply(PTR(IServiceFrontend) roAdapter, PTR(IToServiceResolverRpc) roRpc);
#else
	using DToServiceAdapterRpcReply = std::function<void(REF(IServiceBackend) roResolver, REF(IToServiceAdapterRpc) roRpc)>;
	using DToServiceResolverRpcReply = std::function<void(REF(IServiceFrontend) roAdapter, REF(IToServiceResolverRpc) roRpc)>;
#endif
}

#endif