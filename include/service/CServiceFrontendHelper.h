#ifndef _H_C_SERVICE_ADAPTER_HELPER_H_
#define _H_C_SERVICE_ADAPTER_HELPER_H_

#include "../SPromiseDefine.h"
#include "SServiceFrontendID.h"
#include "../SmartPtrDefine.h"

namespace QWER
{
	QCLASS IServiceFrontend;
	QCLASS IRunner;
	QCLASS IToServiceBackendMsg;
	QCLASS IService;
	QCLASS ILiteService;

	QDLL_EXPORT QCLASS CServiceFrontendHelper
	{
	public:
		PTR(IRunner) GetRunner();
		SPromise<PTR(IToServiceBackendMsg)> PostMsg(PTR(IToServiceBackendMsg) poRpc);
		CONST_REF(SServiceFrontendID) GetID() QCONST_FUNC;
		PTR(IServiceFrontend) Connect(PTR_OR_REF(IService) roService);
		inline BOOLN IsUpdating() QCONST_FUNC { return m_bUpdating; }
		void SetUpdating(BOOLN bUpdating);

	private:
		PTR(IServiceFrontend) m_poServiceAdapter = NULL_PTR;
		BOOLN m_bUpdating = false;

#ifdef _USING_CLI
	internal:
#else
	private:
		friend class IServiceFrontend;
#endif
		void _Init(PTR(IServiceFrontend) poServiceAdapter);
	};
}

#endif
