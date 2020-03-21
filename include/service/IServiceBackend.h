#ifndef _H_I_SERVICE_RESOLVER_H_
#define _H_I_SERVICE_RESOLVER_H_

#include "../CTypeDefine.h"
#include "../VectorDefine.h"
#include "CResolverSender.h"
#include "../SPromiseDefine.h"
#include "SServiceFrontendID.h"
#include "CServiceBackendHelper.h"
#ifdef _USING_CLI
#include "core/CServiceHandleMsgCallback.h"
#endif
#include "../CObjectResMgrDefine.h"
#include "../SPromiseUIDDefine.h"

namespace QWER
{
	QCLASS IToServiceBackendMsg;
	QCLASS IService;
	QCLASS IRunner;
	QCLASS IServiceMsg;
	QTEMPLATE<typename T1, typename T2> QCLASS CDuplexDataPipePeer;
	QCLASS IToServiceFrontendMsg;

	QDLL_EXPORT QCLASS IServiceBackend QABSTRACT
	{
	public:
		IServiceBackend(PTR_OR_REF(IRunner) roRunner);
#ifndef _USING_CLI
		virtual ~IServiceBackend();
#endif

	protected:
		virtual void _Awake(PTR_OR_REF(CServiceBackendHelper) roHelper) = 0;
		virtual UINT32 _Update() = 0;

	private:
		void _HandleMsg();
		void _HandleMsg(PTR(IToServiceBackendMsg) poToResolverMsg);

	private:
		UINT32 m_dwID = 0;
#ifdef _USING_CLI
		CVector(PTR(CResolverSender)) m_oSenders;
#else
		CVector(CResolverSender) m_oSenders;
#endif
		PTR(IRunner) m_poRunner = NULL_PTR;
		CServiceBackendHelper m_oHelper;
		UINT8 m_btOpenState = 0;
		CVector(PTR(IToServiceBackendMsg)) m_oToHandleMsg;

#ifdef _USING_CLI
	internal:
#else
	private:
		friend class IService;
		friend class IApplication;
		friend class IRunner;
		friend class CServiceBackendHelper;
		friend class IToServiceFrontendMsg;
#endif
		void _CreateSender(CONST_REF(SServiceFrontendID) rsAdapterID, PTR_OR_REF(CDuplexDataPipePeer<QPACK(PTR(IServiceMsg), PTR(IServiceMsg))>) poMsgPipe);
		void _SendMsgToAdapter(CONST_REF(SServiceFrontendID) rsAdapterID, PTR(IServiceMsg) poMsg);
		UINT32 _Poll();
		inline PTR(IRunner) _GetRunner(){ return m_poRunner; }
		SPromise<PTR(IToServiceFrontendMsg)> _PostMsg(CONST_REF(SServiceFrontendID) rsAdapterID, PTR(IToServiceFrontendMsg) poRpc);
		inline UINT32 _GetID() QCONST_FUNC{ return m_dwID; }
		void _HandleToServiceResolverMsg(PTR(IToServiceBackendMsg) poToResolverMsg);
		BOOLN _IsOpened(){ return m_btOpenState != 0; }
		SPromiseData<BOOLN> _OnHandleMsg(CONST_REF(SPromiseFuncArg<BOOLN>) rsArg, PTR(IToServiceBackendMsg) poToResolverMsg);
        void _ResolveMsgPromise(CONST_REF(SPromiseUID<PTR(IToServiceFrontendMsg)>) rsOnReply, PTR(IToServiceFrontendMsg) poMsg);
		PTR(CServiceBackendHelper) _ServiceBackendHelper() { return TO_PTR(m_oHelper); }
	};
}

#endif
