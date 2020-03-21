#ifndef _H_I_Runner_H_
#define _H_I_Runner_H_

#include "../CTypeDefine.h"
#include "../VectorDefine.h"
#include "../UnOrderedMapDefine.h"
#include "../PairDefine.h"
#include "CDuplexDataPipe.h"
#include "../CPromiseMgrDefine.h"
#ifdef _USING_CLI
#include "core/CRunnerHandleMsgCallback.h"
#else
#include <mutex>
#endif
#include "../StringDefine.h"
#include "../CObjectResMgrDefine.h"

namespace QWER
{
	QCLASS IServiceFrontend;
	QCLASS IServiceBackend;
	QCLASS IServiceMsg;
	QCLASS IToServiceFrontendMsg;
	QCLASS IToServiceBackendMsg;

	QDLL_EXPORT QCLASS IRunner QABSTRACT
	{
		NON_COPYABLE(IRunner)
	public:
		IRunner();
#ifndef _USING_CLI
		~IRunner();
#endif
		SPair(PTR(CDuplexDataPipePeer<QPACK(PTR(IServiceMsg), PTR(IServiceMsg))>), PTR(CDuplexDataPipePeer<QPACK(PTR(IServiceMsg), PTR(IServiceMsg))>)) Connect(PTR_OR_REF(IRunner) roRunner);
		inline PTR(CTimerMgr) GetTimerMgr(){ return TO_PTR(m_oTimerMgr); }
		inline PTR(CPromiseMgr) GetPromiseMgr(){ return m_poPromiseMgr; }
#ifndef _USING_CLI
		inline std::mutex& GetInitServiceMutex() { return m_oInitServiceMutex; }
#endif
		UINT32 Poll(UINT64 qwDeltaMilliSeconds);

	private:
		void _HandleToServiceAdapterMsg(PTR(IToServiceFrontendMsg) poToAdapterMsg);
		void _HandleToServiceResolverMsg(PTR(IToServiceBackendMsg) poToResolverMsg);

		SPromiseData<BOOLN> _OnHandleMsg(CONST_REF(SPromiseFuncArg<BOOLN>) rsArg, PTR(IServiceFrontend) poAdapter, PTR(IToServiceFrontendMsg) poToAdapterMsg);

	private:
		CVector(PTR(IServiceFrontend)) m_oFrontendList;
		CVector(PTR(IServiceBackend)) m_oBackendList;
		CQueue<PTR(IServiceBackend)> m_oBackendUpdateList;
		CQueue<PTR(IServiceFrontend)> m_oFrontendUpdateList;
		CUnOrderedMap(PTR(IRunner), PTR(CDuplexDataPipe<QPACK(PTR(IServiceMsg), PTR(IServiceMsg))>)) m_oRunnerPipe;
#ifdef _USING_CLI
		CString m_oConnectMutex = "RunnerConnectMutex";
#else
		std::mutex m_oConnectMutex;
		std::mutex m_oInitServiceMutex;
#endif
		CQueue<PTR(CDuplexDataPipePeer<QPACK(PTR(IServiceMsg), PTR(IServiceMsg))>)> m_oPipeList;//用来加速遍历
		CTimerMgr m_oTimerMgr;
		PTR(CPromiseMgr) m_poPromiseMgr = NULL_PTR;

#ifdef _USING_CLI
	internal:
#else
	private:
		friend class IServiceFrontend;
		friend class IServiceBackend;
		friend class CServiceBackendHelper;
		friend class CServiceFrontendHelper;
#endif

		UINT32 _AddServiceAdapter(PTR(IServiceFrontend) poAdapter);
		UINT32 _AddServiceResolver(PTR(IServiceBackend) poResolver);
		inline void _AddUpdate(PTR(IServiceFrontend) poServiceFrontend) { m_oFrontendUpdateList.Push(poServiceFrontend); }
		inline void _AddUpdate(PTR(IServiceBackend) poServiceBackend) { m_oBackendUpdateList.Push(poServiceBackend); }
	};
}

#endif