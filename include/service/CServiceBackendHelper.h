#ifndef _H_C_SERVICE_RESOLVER_HELPER_H_
#define _H_C_SERVICE_RESOLVER_HELPER_H_

#include "../SPromiseDefine.h"
#include "SServiceFrontendID.h"
#include "../SmartPtrDefine.h"

namespace QWER
{
	QCLASS IToServiceFrontendMsg;
	QCLASS IRunner;
	QCLASS IServiceBackend;
	QCLASS IServiceFrontend;
	QCLASS IService;

	QDLL_EXPORT QCLASS CServiceBackendHelper
	{
	public:
		PTR(IRunner) GetRunner();
		SPromise<PTR(IToServiceFrontendMsg)> PostMsg(CONST_REF(SServiceFrontendID) rsAdapterID, PTR(IToServiceFrontendMsg) poRpc);
		UINT16 GetID() QCONST_FUNC;
		PTR(IServiceFrontend) Connect(PTR_OR_REF(IService) roService);
		inline BOOLN IsUpdating() QCONST_FUNC { return m_bUpdating; }
		void SetUpdating(BOOLN bUpdating);

	private:
		PTR(IServiceBackend) m_poServiceResolver = NULL_PTR;
		BOOLN m_bUpdating = true;

#ifdef _USING_CLI
	internal:
#else
	private:
		friend class IServiceBackend;
#endif
		void _Init(PTR(IServiceBackend) poServiceResolver);
	};
}

#endif
