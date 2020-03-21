#include "IServiceBackend.h"
#include "IRunner.h"
#include "CResolverSender.h"
#include "IToServiceFrontendMsg.h"
#include "IToServiceBackendMsg.h"

namespace QWER
{
	QENUM EServiceBackendOpenState
	{
		Offline = 0,
		Awake = 1,
		Update = 2
	};

	IServiceBackend::IServiceBackend(PTR_OR_REF(IRunner) roRunner)
	{
#ifdef _USING_CLI
		MUTEX_LOCK(roRunner);
#else
		MUTEX_LOCK(TO_PTR1(roRunner.GetInitServiceMutex()));
#endif
		m_oHelper._Init(this);
		m_poRunner = TO_PTR2(roRunner);
		UINT32 dwID = m_poRunner->_AddServiceResolver(this);
		m_dwID = dwID;
		m_btOpenState = (UINT8)EServiceBackendOpenState::Awake;
#ifdef _USING_CLI
		MUTEX_UNLOCK(roRunner);
#else
		MUTEX_UNLOCK(TO_PTR1(roRunner.GetInitServiceMutex()));
#endif
	}

#ifndef _USING_CLI
	IServiceBackend::~IServiceBackend()
	{
	}
#endif

	void IServiceBackend::_CreateSender(CONST_REF(SServiceFrontendID) rsAdapterID, PTR_OR_REF(CDuplexDataPipePeer<QPACK(PTR(IServiceMsg), PTR(IServiceMsg))>) roMsgPipe)
	{
		UINT8 btIdInService = rsAdapterID.GetIdInService();
		if (btIdInService >= m_oSenders.size())
			m_oSenders.resize(btIdInService + 1);
#ifdef _USING_CLI
		PTR(CVector(PTR(CResolverSender))) poSenders = TO_PTR(m_oSenders);
		poSenders[btIdInService] = QNEW CResolverSender();
		PTR(CResolverSender) poSender = poSenders[btIdInService];
#else
		PTR(CResolverSender) poSender = TO_PTR(m_oSenders[btIdInService]);
#endif
		poSender->_Init(roMsgPipe);
	}

	void IServiceBackend::_SendMsgToAdapter(CONST_REF(SServiceFrontendID) rsAdapterID, PTR(IServiceMsg) poMsg)
	{
		UINT8 btIdInService = rsAdapterID.GetIdInService();
		PTR(CResolverSender) poSender = TO_PTR2(m_oSenders.at(btIdInService));
		poMsg->_SetAdapterID(rsAdapterID);
		poMsg->_SetResolverID(m_dwID);
		poSender->_SendMsgToAdapter(poMsg);
	}

	SPromise<PTR(IToServiceFrontendMsg)> IServiceBackend::_PostMsg(CONST_REF(SServiceFrontendID) rsAdapterID, PTR(IToServiceFrontendMsg) poRpc)
	{
		PTR(CPromiseMgr) poPromiseMgr = m_poRunner->GetPromiseMgr();
		SPromiseUID<PTR(IToServiceFrontendMsg)> sPromiseUID = 
			poPromiseMgr->CreatePromise<PTR(IToServiceFrontendMsg)>();
		poRpc->_SetCallback(sPromiseUID);
		_SendMsgToAdapter(rsAdapterID, poRpc);
		return poPromiseMgr->GetOrCancelPromise(sPromiseUID);
	}

	void IServiceBackend::_HandleMsg()
	{
		UINT32 dwSize = m_oToHandleMsg.size();
		if (dwSize == 0)
			return;
		CVector(PTR(IToServiceBackendMsg)) oTmp;
		m_oToHandleMsg.swap(TO_PTR1(oTmp));
		for (UINT32 i = 0; i != dwSize; ++i)
			_HandleMsg(oTmp.at(i));
	}

	void IServiceBackend::_HandleMsg(PTR(IToServiceBackendMsg) poToResolverMsg)
	{
		SPromise<BOOLN> sPromise = poToResolverMsg->_OnReceive(TO_REF2(this), poToResolverMsg->_GetAdapterID());
		poToResolverMsg->_SetReply();
		sPromise.Then<BOOLN>(
			CONVERT_DELEGATE(
				QPACK(DPromiseResolver<BOOLN, BOOLN>),
				PACK_DELEGATE_FUNC(
					QPACK(qwer_stl::function<SPromiseData<BOOLN>, SPromiseFuncArg<BOOLN>, PTR(IToServiceBackendMsg) poToResolverMsg>),
					this, IServiceBackend::_OnHandleMsg
				),
				DELEGATE_PLACEHOLDERS(, std::placeholders::_1),
				poToResolverMsg
			)
		);
	}

	SPromiseData<BOOLN> IServiceBackend::_OnHandleMsg(CONST_REF(SPromiseFuncArg<BOOLN>) rsArg, PTR(IToServiceBackendMsg) poToResolverMsg)
	{
		_SendMsgToAdapter(poToResolverMsg->_GetAdapterID(), poToResolverMsg);
		return SPromiseData<BOOLN>(true);
	}

	void IServiceBackend::_HandleToServiceResolverMsg(PTR(IToServiceBackendMsg) poToResolverMsg)
	{
		if ((EServiceBackendOpenState)m_btOpenState == EServiceBackendOpenState::Update)
		{
			_HandleMsg();
			_HandleMsg(poToResolverMsg);
		}
		else
		{
			m_oToHandleMsg.push_back(poToResolverMsg);
		}
	}

    void IServiceBackend::_ResolveMsgPromise(CONST_REF(SPromiseUID<PTR(IToServiceFrontendMsg)>) rsOnReply, PTR(IToServiceFrontendMsg) poMsg)
    {
        _GetRunner()->GetPromiseMgr()->ResolvePromise(rsOnReply, poMsg);
    }

	UINT32 IServiceBackend::_Poll()
	{
		EServiceBackendOpenState eState = (EServiceBackendOpenState)m_btOpenState;
		switch (eState)
		{
		case EServiceBackendOpenState::Offline:
			return 0;
		case EServiceBackendOpenState::Awake:
			m_oHelper.SetUpdating(false);
			_Awake(TO_PTR1(m_oHelper));
			++m_btOpenState;
			return 1;
		default:
			break;
		}
		return _Update();
	}
}
