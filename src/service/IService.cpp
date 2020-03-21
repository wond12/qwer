#include "IService.h"
#include "IRunner.h"
#include "IServiceFrontend.h"
#include "IServiceBackend.h"
#include "../PairDefine.h"

namespace QWER
{
#ifndef _USING_CLI
	IService::~IService()
	{
	}
#endif

	PTR(IServiceFrontend) IService::_Connect(PTR_OR_REF(IRunner) roRunner)
	{
		PTR(IRunner) poRunner = TO_PTR2(roRunner);
		PTR(IServiceBackend) poResolver = _GetBackend();
		if (poResolver == NULL_PTR || poResolver->_IsOpened() == false)
			return NULL_PTR;
		MUTEX_LOCK(TO_PTR1(m_oConnectMutex));
		SPair(CUnOrderedMap(PTR(IRunner), PTR(IServiceFrontend))::iterator, bool) ret =
			m_oConnectedServiceAdapters.insert(MakePair(poRunner, (PTR(IServiceFrontend))NULL_PTR));
		CUnOrderedMap(PTR(IRunner), PTR(IServiceFrontend))::iterator retIt = ret.first;
		if (ret.second == false)
		{
			PTR(IServiceFrontend) poRet = MapIteratorSecond(retIt);
			MUTEX_UNLOCK(TO_PTR1(m_oConnectMutex));
			return poRet;
		}
		if (m_oConnectedServiceAdapters.size() > UINT8_MAX_VALUE)
		{
			MUTEX_UNLOCK(TO_PTR1(m_oConnectMutex));
#ifdef _USING_CLI
			throw QNEW System::Exception("too many service adapters!");
#else
			throw std::runtime_error("too many service adapters!");
#endif
		}
#ifdef _USING_CLI
		PTR(IServiceFrontend) poAdapter = _NewFrontend();
#else
		PTR(IServiceFrontend) poAdapter = _NewFrontend().release();
#endif
		MapIteratorSecond(retIt) = poAdapter;
		m_oServiceAdapters.push_back(poAdapter);
		PTR(IRunner) poResolverRunner = GetRunner();
		SPair(PTR(CDuplexDataPipePeer<QPACK(PTR(IServiceMsg), PTR(IServiceMsg))>), PTR(CDuplexDataPipePeer<QPACK(PTR(IServiceMsg), PTR(IServiceMsg))>)) sPipe = poRunner->Connect(TO_REF2(poResolverRunner));

		poAdapter->_Init(roRunner, (UINT8)(m_oConnectedServiceAdapters.size() - 1), poResolver->_GetID(), TO_REF2(sPipe.first));
		poResolver->_CreateSender(poAdapter->_GetID(), TO_REF2(sPipe.second));
		MUTEX_UNLOCK(TO_PTR1(m_oConnectMutex));
		poAdapter->__OnInit();
		return poAdapter;
	}

	PTR(IRunner) IService::GetRunner()
	{
		PTR(IServiceBackend) poResolver = _GetBackend();
		return poResolver == NULL_PTR ? NULL_PTR : poResolver->_GetRunner();
	}
}