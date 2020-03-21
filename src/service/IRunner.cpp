#include "IRunner.h"
#ifdef _USING_CLI
#else
#include <stdexcept>
#endif
#include "IServiceFrontend.h"
#include "IServiceBackend.h"
#include "IToServiceFrontendMsg.h"
#include "IToServiceBackendMsg.h"

namespace QWER
{
	IRunner::IRunner()
	{
		m_poPromiseMgr = QNEW CPromiseMgr(TO_PTR1(m_oTimerMgr));
	}

#ifndef _USING_CLI
	IRunner::~IRunner()
	{
		for (CUnOrderedMap(PTR(IRunner), PTR(CDuplexDataPipe<QPACK(PTR(IServiceMsg), PTR(IServiceMsg))>))::iterator it = m_oRunnerPipe.begin();
			it != m_oRunnerPipe.end(); ++it)
		{
			SAFE_DELETE(it->second);
		}
		SAFE_DELETE(m_poPromiseMgr);
	}
#endif

	SPair(PTR(CDuplexDataPipePeer<QPACK(PTR(IServiceMsg), PTR(IServiceMsg))>), PTR(CDuplexDataPipePeer<QPACK(PTR(IServiceMsg), PTR(IServiceMsg))>)) IRunner::Connect(PTR_OR_REF(IRunner) roRunner)
	{
		MUTEX_LOCK(m_oConnectMutex);
		PTR(IRunner) poRunner = TO_PTR2(roRunner);
		PTR(CDuplexDataPipe<QPACK(PTR(IServiceMsg), PTR(IServiceMsg))>) poDuplexDataPipe = NULL_PTR;
		CUnOrderedMap(PTR(IRunner), PTR(CDuplexDataPipe<QPACK(PTR(IServiceMsg), PTR(IServiceMsg))>))::iterator it = m_oRunnerPipe.find(poRunner);
		if (it == m_oRunnerPipe.end())
		{
			it = poRunner->m_oRunnerPipe.find(this);
			if (it == poRunner->m_oRunnerPipe.end())
			{
				poDuplexDataPipe = QNEW CDuplexDataPipe<QPACK(PTR(IServiceMsg), PTR(IServiceMsg))>();
				m_oRunnerPipe[poRunner] = poDuplexDataPipe;
				m_oPipeList.Push(TO_PTR2(poDuplexDataPipe->GetPeer1()));
				poRunner->m_oPipeList.Push(TO_PTR2(poDuplexDataPipe->GetPeer2()));
				MUTEX_UNLOCK(m_oConnectMutex);
				return MakePair(TO_PTR2(poDuplexDataPipe->GetPeer1()), TO_PTR2(poDuplexDataPipe->GetPeer2()));
			}
			poDuplexDataPipe = MapIteratorSecond(it);
			MUTEX_UNLOCK(m_oConnectMutex);
			return MakePair(TO_PTR2(poDuplexDataPipe->GetPeer2()), TO_PTR2(poDuplexDataPipe->GetPeer1()));
		}
		poDuplexDataPipe = MapIteratorSecond(it);
		MUTEX_UNLOCK(m_oConnectMutex);
		return MakePair(TO_PTR2(poDuplexDataPipe->GetPeer1()), TO_PTR2(poDuplexDataPipe->GetPeer2()));
	}

	UINT32 IRunner::Poll(UINT64 qwDeltaMilliSeconds)
	{
		UINT32 dwMaxCountPerPoll = m_oTimerMgr.GetMaxCountPerPoll();
		if (dwMaxCountPerPoll < m_poPromiseMgr->GetMaxCountPerPoll())
			dwMaxCountPerPoll = m_poPromiseMgr->GetMaxCountPerPoll();
		if (dwMaxCountPerPoll == 0)
			dwMaxCountPerPoll = UINT32_MAX_VALUE;

		UINT32 dwDeltaCount = 0;

		UINT32 dwBackendUpdateListSize = m_oBackendUpdateList.GetCount();
		for (UINT32 i = 0; i != dwBackendUpdateListSize; ++i)
		{
			PTR(IServiceBackend) poBackend = m_oBackendUpdateList.At(0);
			m_oBackendUpdateList.Pop();
			if (poBackend->_ServiceBackendHelper()->IsUpdating()) {
				dwDeltaCount += poBackend->_Poll();
				m_oBackendUpdateList.Push(poBackend);
			}
			if (dwDeltaCount >= dwMaxCountPerPoll)
				break;
		}
		UINT32 dwFrontendUpdateListSize = m_oFrontendUpdateList.GetCount();
		for (UINT32 i = 0; i != dwFrontendUpdateListSize; ++i)
		{
			PTR(IServiceFrontend) poFrontend = m_oFrontendUpdateList.At(0);
			m_oFrontendUpdateList.Pop();
			if (poFrontend->_ServiceFrontendHelper()->IsUpdating()) {
				dwDeltaCount += poFrontend->_Poll();
				m_oFrontendUpdateList.Push(poFrontend);
			}
			if (dwDeltaCount >= dwMaxCountPerPoll)
				break;
		}
		UINT32 dwPipeListSize = m_oPipeList.GetCount();
		for (UINT32 i = 0; i != dwPipeListSize && dwDeltaCount < dwMaxCountPerPoll; ++i)
		{
			PTR(CDuplexDataPipePeer<QPACK(PTR(IServiceMsg), PTR(IServiceMsg))>) poPeer = m_oPipeList.At(0);
			m_oPipeList.Pop();
			m_oPipeList.Push(poPeer);
			while (!poPeer->IsEmpty() && dwDeltaCount < dwMaxCountPerPoll)
			{
				poPeer->Poll(qwDeltaMilliSeconds);
				PTR(IServiceMsg) poServiceMsg = poPeer->Top();
				EServiceMsgType eServiceMsgType = (EServiceMsgType)poServiceMsg->GetMsgType();
				switch (eServiceMsgType)
				{
				case QWER::EServiceMsgType::ToAdapterMsg:
				{
					PTR(IToServiceFrontendMsg) poToAdapterMsg = (PTR(IToServiceFrontendMsg))poServiceMsg;
					_HandleToServiceAdapterMsg(poToAdapterMsg);
					break;
				}
				case QWER::EServiceMsgType::ToResolverMsg:
				{
					PTR(IToServiceBackendMsg) poToResolverMsg = (PTR(IToServiceBackendMsg))poServiceMsg;
					_HandleToServiceResolverMsg(poToResolverMsg);
					break;
				}
				default:
					QWERASSERT(false, dwDeltaCount);
					break;
				}
				poPeer->Pop();
				++dwDeltaCount;
			}
		}
		dwDeltaCount += m_poPromiseMgr->Poll();
		dwDeltaCount += m_oTimerMgr.Poll(qwDeltaMilliSeconds);
		return dwDeltaCount;
	}

	void IRunner::_HandleToServiceAdapterMsg(PTR(IToServiceFrontendMsg) poToAdapterMsg)
	{
		if (poToAdapterMsg->_IsReply())
		{
			UINT16 wResolverID = poToAdapterMsg->_GetResolverID();
			PTR(IServiceBackend) poResolver = m_oBackendList.at(wResolverID);
			poToAdapterMsg->_OnReply(TO_REF2(poResolver));
		}
		else
		{
			CONST_REF(SServiceFrontendID) sAdapterID = poToAdapterMsg->_GetAdapterID();
			PTR(IServiceFrontend) poAdapter = m_oFrontendList.at(sAdapterID.GetIdInRunner());
			SPromise<BOOLN> sPromise = poToAdapterMsg->_OnReceive(TO_REF2(poAdapter));
			poToAdapterMsg->_SetReply();
			sPromise.Then<BOOLN>(
				CONVERT_DELEGATE(
					QPACK(DPromiseResolver<BOOLN, BOOLN>),
					PACK_DELEGATE_FUNC(
						QPACK(qwer_stl::function<SPromiseData<BOOLN>, SPromiseFuncArg<BOOLN>, PTR(IServiceFrontend), PTR(IToServiceFrontendMsg)>),
						this, IRunner::_OnHandleMsg
					),
					DELEGATE_PLACEHOLDERS(, std::placeholders::_1),
					poAdapter, poToAdapterMsg
				)
			);
		}
	}

	SPromiseData<BOOLN> IRunner::_OnHandleMsg(CONST_REF(SPromiseFuncArg<BOOLN>) rsArg, PTR(IServiceFrontend) poAdapter, PTR(IToServiceFrontendMsg) poToAdapterMsg)
	{
		poAdapter->_SendMsgToResolver(poToAdapterMsg);
		return SPromiseData<BOOLN>();
	}

	void IRunner::_HandleToServiceResolverMsg(PTR(IToServiceBackendMsg) poToResolverMsg)
	{
		if (poToResolverMsg->_IsReply())
		{
			CONST_REF(SServiceFrontendID) sAdapterID = poToResolverMsg->_GetAdapterID();
			PTR(IServiceFrontend) poAdapter = m_oFrontendList.at(sAdapterID.GetIdInRunner());
			poToResolverMsg->_OnReply(TO_REF2(poAdapter));
		}
		else
		{
			UINT16 wResolverID = poToResolverMsg->_GetResolverID();
			PTR(IServiceBackend) poResolver = m_oBackendList.at(wResolverID);
			poResolver->_HandleToServiceResolverMsg(poToResolverMsg);
		}
	}

	UINT32 IRunner::_AddServiceAdapter(PTR(IServiceFrontend) poAdapter)
	{
		UINT32 dwIndex = (UINT32)m_oFrontendList.size();
		QWERASSERT(dwIndex < UINT32_MAX_VALUE, UINT32_MAX_VALUE);
		m_oFrontendList.push_back(poAdapter);
		m_oFrontendUpdateList.Push(poAdapter);
		return dwIndex;
	}
	UINT32 IRunner::_AddServiceResolver(PTR(IServiceBackend) poResolver)
	{
		UINT32 dwIndex = m_oBackendList.size();
		QWERASSERT(dwIndex < UINT32_MAX_VALUE, UINT32_MAX_VALUE);
		m_oBackendList.push_back(poResolver);
		m_oBackendUpdateList.Push(poResolver);
		return dwIndex;
	}
}
