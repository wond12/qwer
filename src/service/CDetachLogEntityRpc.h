#ifndef _H_C_DETACH_LOG_ENTITY_RPC_H_
#define _H_C_DETACH_LOG_ENTITY_RPC_H_

#include "IToServiceBackendMsg.h"
#include "SLogChannelUID.h"
#include "SLogEntityUID.h"
#include "../VectorDefine.h"

namespace QWER
{
	QCLASS CDetachLogEntity : public IToServiceBackendMsg
	{
	public:
		QCLASS CArg
		{
		public:
			SLogEntityUID m_sLogEntityUID;
			CVector(UINT8) m_oLogLevels;
		};

	protected:	
		virtual SPromise<BOOLN> _OnCall(PTR_OR_REF(IServiceBackend) roResolver, CONST_REF(SServiceFrontendID) rsAdapterID) QOVERRIDE QFINAL;

	public:
		CArg m_oArg;
	};
}

#endif
