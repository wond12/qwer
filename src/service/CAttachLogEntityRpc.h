#ifndef _H_C_ATTACH_LOG_ENTITY_RPC_H_
#define _H_C_ATTACH_LOG_ENTITY_RPC_H_

#include "IToServiceBackendMsg.h"
#include "SLogChannelUID.h"
#include "SLogEntityUID.h"
#include "../VectorDefine.h"
#include "LogDelegate.h"

namespace QWER
{
	QCLASS CAttachLogEntity : public IToServiceBackendMsg
	{
	public:
		QCLASS CArg
		{
		public:
			SLogChannelUID m_sLogChannelUID;
			PTR_OR_VAL(DNewLogEntity) m_cbNewLogEntity;
			CVector(UINT8) m_oLogLevels;
		};
		QCLASS CRet
		{
		public:
			SLogEntityUID m_sLogEntityUID;
		};

	protected:	
		virtual SPromise<BOOLN> _OnCall(PTR_OR_REF(IServiceBackend) roResolver, CONST_REF(SServiceFrontendID) rsAdapterID) QOVERRIDE QFINAL;

	public:
		CArg m_oArg;
		CRet m_oRet;
	};
}

#endif
