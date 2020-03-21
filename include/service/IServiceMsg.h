#ifndef _H_I_SERVICE_MSG_H_
#define _H_I_SERVICE_MSG_H_

#include "../CTypeDefine.h"
#include "SServiceFrontendID.h"

namespace QWER
{
	QDLL_EXPORT QCLASS IServiceMsg QABSTRACT
	{
	public:
#ifndef _USING_CLI
		virtual ~IServiceMsg() { }
#endif
		virtual UINT16 GetMsgType() QCONST_FUNC = 0;

	private:
		UINT32 m_dwResolverID = 0;
		SServiceFrontendID m_sAdapterID;

#ifdef _USING_CLI
	internal:
#else
	private:
		friend class IRunner;
		friend class IServiceFrontend;
		friend class IServiceBackend;
#endif
		inline UINT32 _GetResolverID() QCONST_FUNC{ return m_dwResolverID; }
		inline void _SetResolverID(UINT32 dwResolverID){ m_dwResolverID = dwResolverID; }
		inline CONST_REF(SServiceFrontendID) _GetAdapterID() QCONST_FUNC{ return m_sAdapterID; }
		inline void _SetAdapterID(CONST_REF(SServiceFrontendID) rsAdapterID){ m_sAdapterID = rsAdapterID; }
	};
}

#endif