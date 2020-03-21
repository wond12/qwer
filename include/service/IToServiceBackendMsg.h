#ifndef _H_I_TO_SERVICE_RESOLVER_MSG_H_
#define _H_I_TO_SERVICE_RESOLVER_MSG_H_

#include "../CTypeDefine.h"
#include "IServiceMsg.h"
#include "EServiceMsgType.h"
#include "../SPromiseUIDDefine.h"
#include "../SPromiseDefine.h"
#include "SServiceFrontendID.h"

namespace QWER
{
	QCLASS IServiceBackend;
	QCLASS IServiceFrontend;

	QDLL_EXPORT QCLASS IToServiceBackendMsg QABSTRACT : public IServiceMsg
	{
	public:
		virtual UINT16 GetMsgType() QCONST_FUNC QOVERRIDE
		{
			return (UINT16)EServiceMsgType::ToResolverMsg;
		}

	protected:
		virtual SPromise<BOOLN> _OnCall(PTR_OR_REF(IServiceBackend) roResolver, CONST_REF(SServiceFrontendID) rsAdapterID) = 0;
		void _Reset()
		{
			m_bIsReply = false;
			m_sOnReply = SPromiseUID<PTR(IToServiceBackendMsg)>();
		}

	private:
		BOOLN m_bIsReply = false;
		SPromiseUID<PTR(IToServiceBackendMsg)> m_sOnReply;

#ifdef _USING_CLI
	internal:
#else
	private:
		friend class IRunner;
		friend class IServiceFrontend;
		friend class IServiceBackend;
#endif
		inline BOOLN _IsReply() QCONST_FUNC{ return m_bIsReply; }

		inline void _SetReply()
		{
			m_bIsReply = true;
		}

		void _SetCallback(SPromiseUID<PTR(IToServiceBackendMsg)> rsOnReply);

		SPromise<BOOLN> _OnReceive(PTR_OR_REF(IServiceBackend) roResolver, CONST_REF(SServiceFrontendID) rsAdapterID);
		void _OnReply(PTR_OR_REF(IServiceFrontend) roAdapter);
	};
}

#endif