#ifndef _H_C_CREATE_LOG_CHANNEL_RPC_H_
#define _H_C_CREATE_LOG_CHANNEL_RPC_H_

#include "IToServiceBackendMsg.h"
#include "SLogChannelUID.h"

namespace QWER
{
	QCLASS CCreateLogChannelRpc : public IToServiceBackendMsg
	{
	public:
		QCLASS CArg
		{
		public:
		};
		QCLASS CRet
		{
		public:
			SLogChannelUID m_sLogChannelUID;
		};

	protected:	
		virtual SPromise<BOOLN> _OnCall(PTR_OR_REF(IServiceBackend) roResolver, CONST_REF(SServiceFrontendID) rsAdapterID) QOVERRIDE QFINAL;

	public:
		CArg m_oArg;
		CRet m_oRet;
	};
}

#endif
