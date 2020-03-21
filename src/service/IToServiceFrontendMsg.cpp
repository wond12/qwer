#include "IToServiceFrontendMsg.h"
#include "../CPromiseMgrDefine.h"
#include "IServiceBackend.h"
#include "IRunner.h"

namespace QWER
{
	SPromise<BOOLN> IToServiceFrontendMsg::_OnReceive(PTR_OR_REF(IServiceFrontend) roAdapter)
	{
		return _OnCall(roAdapter);
	}

	void IToServiceFrontendMsg::_OnReply(PTR_OR_REF(IServiceBackend) roResolver)
	{
		SPromiseUID<PTR(IToServiceFrontendMsg)> sOnReply = m_sOnReply;
		_Reset();
		PTR(IServiceBackend) poServiceResolver = TO_PTR2(roResolver);
        poServiceResolver->_ResolveMsgPromise(sOnReply, this);
	}

	void IToServiceFrontendMsg::_SetCallback(SPromiseUID<PTR(IToServiceFrontendMsg)> rsOnReply)
	{
		m_sOnReply = rsOnReply;
	}
}
