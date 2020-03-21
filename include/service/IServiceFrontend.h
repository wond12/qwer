#ifndef _H_I_SERVICE_ADAPTER_H_
#define _H_I_SERVICE_ADAPTER_H_

#include "../CTypeDefine.h"
#include "CDuplexDataPipePeer.h"
#include "../SPromiseDefine.h"
#include "SServiceFrontendID.h"
#include "CServiceFrontendHelper.h"
#include "../SPromiseUIDDefine.h"

namespace QWER
{
	QCLASS IToServiceBackendMsg;
	QCLASS IService;
	QCLASS IRunner;
	QCLASS IServiceMsg;
	QINTERFACE IServiceUserData;

	QDLL_EXPORT QCLASS IServiceFrontend QABSTRACT
	{
	public:
		IServiceFrontend();
#ifndef _USING_CLI
		virtual ~IServiceFrontend(){}
#endif
		inline void SetUserData(PTR(IServiceUserData) poUserData){ m_poUserData = poUserData; }
		inline PTR(IServiceUserData) GetUserData() { return m_poUserData; }
#ifndef _USING_CLI
		inline CONST_PTR(IServiceUserData) GetUserData() QCONST_FUNC{ return m_poUserData; }
#endif

	protected:
		inline CONST_REF(SServiceFrontendID) __GetID() QCONST_FUNC{ return m_sID; }
		virtual void _Awake(PTR_OR_REF(CServiceFrontendHelper) roHelper) = 0;
		virtual UINT32 _Update() = 0;

	private:
		SServiceFrontendID m_sID;
		UINT16 m_wResolverID = 0;
		PTR(IRunner) m_poRunner = NULL_PTR;
		PTR(CDuplexDataPipePeer<QPACK(PTR(IServiceMsg), PTR(IServiceMsg))>) m_poMsgPipe = NULL_PTR;
		PTR(IServiceUserData) m_poUserData = NULL_PTR;
		CServiceFrontendHelper m_oHelper;

#ifdef _USING_CLI
	internal:
#else
	private:
		friend class IService;
		friend class IRunner;
		friend class CServiceFrontendHelper;
		friend class IToServiceBackendMsg;
#endif
		void _Init(PTR_OR_REF(IRunner) roRunner, UINT8 btAdapterIdInService, UINT16 wResolverID, PTR_OR_REF(CDuplexDataPipePeer<QPACK(PTR(IServiceMsg), PTR(IServiceMsg))>) roMsgPipe);
		void _SendMsgToResolver(PTR(IServiceMsg) poMsg);
		inline PTR(IRunner) _GetRunner(){ return m_poRunner; }
		SPromise<PTR(IToServiceBackendMsg)> _PostMsg(PTR(IToServiceBackendMsg) poRpc);
		inline CONST_REF(SServiceFrontendID) _GetID() QCONST_FUNC{ return m_sID; }
		UINT32 _Poll();
        void _ResolveMsgPromise(CONST_REF(SPromiseUID<PTR(IToServiceBackendMsg)>) rsOnReply, PTR(IToServiceBackendMsg) poMsg);
		PTR(CServiceFrontendHelper) _ServiceFrontendHelper() { return TO_PTR(m_oHelper); }
		void __OnInit() { _Awake(TO_PTR1(m_oHelper)); }
	};
}

#endif
