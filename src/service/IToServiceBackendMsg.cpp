#include "IToServiceBackendMsg.h"
#include "../CPromiseMgrDefine.h"
#include "IServiceFrontend.h"
#include "IRunner.h"

namespace QWER
{
	SPromise<BOOLN> IToServiceBackendMsg::_OnReceive(PTR_OR_REF(IServiceBackend) roResolver, CONST_REF(SServiceFrontendID) rsNetServiceAdapterID)
	{
		return _OnCall(roResolver, rsNetServiceAdapterID);
	}

	void IToServiceBackendMsg::_OnReply(PTR_OR_REF(IServiceFrontend) roAdapter)
	{
		PTR(IServiceFrontend) poServiceAdapter = TO_PTR2(roAdapter);
		SPromiseUID<PTR(IToServiceBackendMsg)> sOnReply = m_sOnReply;
		_Reset();
        poServiceAdapter->_ResolveMsgPromise(sOnReply, this);
	}

	void IToServiceBackendMsg::_SetCallback(SPromiseUID<PTR(IToServiceBackendMsg)> rsOnReply)
	{
		m_sOnReply = rsOnReply;
	}
}
