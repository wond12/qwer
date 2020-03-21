#ifndef _H_S_SERVICE_ADAPTER_ID_H_
#define _H_S_SERVICE_ADAPTER_ID_H_

#include "../CTypeDefine.h"

namespace QWER
{
	QCLASS IToServiceBackendMsg;
	QCLASS IService;
	QCLASS IRunner;
	QCLASS IServiceMsg;

	QDLL_EXPORT QSTRUCT SServiceFrontendID
	{
#ifndef _USING_CLI
	public:
		SServiceFrontendID(){}
#endif
		inline UINT32 GetIdInRunner() QCONST_FUNC{ return m_dwIdInRunner; }
		inline UINT8 GetIdInService() QCONST_FUNC{return m_btIdInService;}
        inline UINT64 GetValue() QCONST_FUNC { return (((UINT64)m_dwIdInRunner) << 32) | m_btIdInService; }
#ifdef _USING_CLI
	internal:
#else
	private:
		friend QCLASS IServiceFrontend;
#endif
		SServiceFrontendID(UINT32 dwIdInRunner, UINT8 btIdInService)
		{
			m_dwIdInRunner = dwIdInRunner;
			m_btIdInService = btIdInService;
		}

	private:
		QINIT_ONLY UINT32 STRUCT_MEMBER_DEFINE(m_dwIdInRunner, 0);
		QINIT_ONLY UINT8 STRUCT_MEMBER_DEFINE(m_btIdInService, 0);
	};
}

#endif