#include "IServiceFrontend.h"
#include "IRunner.h"
#include "IToServiceBackendMsg.h"
#include "IServiceUserData.h"

namespace QWER
{
	IServiceFrontend::IServiceFrontend()
	{
		m_oHelper._Init(this);
	}

	void IServiceFrontend::_Init(PTR_OR_REF(IRunner) roRunner, UINT8 btAdapterIdInService, UINT16 wResolverID, PTR_OR_REF(CDuplexDataPipePeer<QPACK(PTR(IServiceMsg), PTR(IServiceMsg))>) roMsgPipe)
	{
		m_poRunner = TO_PTR2(roRunner);
		UINT16 wID = m_poRunner->_AddServiceAdapter(this);
		m_sID = SServiceFrontendID(wID, btAdapterIdInService);
		m_wResolverID = wResolverID;
		m_poMsgPipe = TO_PTR2(roMsgPipe);
	}

	void IServiceFrontend::_SendMsgToResolver(PTR(IServiceMsg) poMsg)
	{
		poMsg->_SetAdapterID(m_sID);
		poMsg->_SetResolverID(m_wResolverID);
		m_poMsgPipe->Push(poMsg);
	}

	SPromise<PTR(IToServiceBackendMsg)> IServiceFrontend::_PostMsg(PTR(IToServiceBackendMsg) poRpc)
	{
		PTR(CPromiseMgr) poPromiseMgr = m_poRunner->GetPromiseMgr();
		SPromiseUID<PTR(IToServiceBackendMsg)> sPromiseUID = poPromiseMgr->CreatePromise<PTR(IToServiceBackendMsg)>();
		SPromise<PTR(IToServiceBackendMsg)> sPromise = poPromiseMgr->GetOrCancelPromise(sPromiseUID);
		poRpc->_SetCallback(sPromiseUID);
		_SendMsgToResolver(poRpc);
		return sPromise;
	}

    void IServiceFrontend::_ResolveMsgPromise(CONST_REF(SPromiseUID<PTR(IToServiceBackendMsg)>) rsOnReply, PTR(IToServiceBackendMsg) poMsg)
    {
        m_poRunner->GetPromiseMgr()->ResolvePromise(rsOnReply, poMsg);
    }

	UINT32 IServiceFrontend::_Poll()
	{
		return _Update();
	}
}
