#ifndef _H_I_TO_SERVICE_ADAPTER_MSG_H_
#define _H_I_TO_SERVICE_ADAPTER_MSG_H_

#include "../CTypeDefine.h"
#include "IServiceMsg.h"
#include "EServiceMsgType.h"
#include "../SPromiseDefine.h"
#include "../SPromiseUIDDefine.h"

namespace QWER
{
	QCLASS IServiceFrontend;
	QCLASS IServiceBackend;
	QTEMPLATE<typename T> QCLASS CPromise;

	QDLL_EXPORT QCLASS IToServiceFrontendMsg QABSTRACT : public IServiceMsg
	{
	public:
		virtual UINT16 GetMsgType() QCONST_FUNC QOVERRIDE
		{
			return (UINT16)EServiceMsgType::ToAdapterMsg;
		}

	protected:
		virtual SPromise<BOOLN> _OnCall(PTR_OR_REF(IServiceFrontend) roAdapter) = 0;
		void _Reset()
		{
			m_bIsReply = false;
			m_sOnReply = SPromiseUID<PTR(IToServiceFrontendMsg)>();
		}

	private:
		BOOLN m_bIsReply = false;
		SPromiseUID<PTR(IToServiceFrontendMsg)> m_sOnReply;

#ifdef _USING_CLI
	internal:
#else
	private:
		friend class IRunner;
		friend class IServiceBackend;
#endif
		inline BOOLN _IsReply() QCONST_FUNC{ return m_bIsReply; }

		inline void _SetReply()
		{
			m_bIsReply = true;
		}

		void _SetCallback(SPromiseUID<PTR(IToServiceFrontendMsg)> rsOnReply);

		SPromise<BOOLN> _OnReceive(PTR_OR_REF(IServiceFrontend) roAdapter);
		void _OnReply(PTR_OR_REF(IServiceBackend) roResolver);
	};
}

#endif