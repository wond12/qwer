#ifndef _H_C_WRITE_LOG_PTC_H_
#define _H_C_WRITE_LOG_PTC_H_

#include "IToServiceBackendMsg.h"
#ifndef _USING_CLI
#include <sstream>
#endif
#include "../StringDefine.h"
#include "IToServiceBackendMsg.h"
#include "SLogChannelUID.h"

namespace QWER
{
	QCLASS CWriteLogPtc : public IToServiceBackendMsg
	{
	public:
		inline UINT8 GetLogLevel() QCONST_FUNC{ return m_btLogLevel; }
		inline void SetLogLevel(UINT8 btLogLevel){ m_btLogLevel = btLogLevel; }

#ifndef _USING_CLI
		void SetLog(const char* pchLog) { m_strLog = pchLog; }
#endif
		void SetLog(CONST_REF(CString) rstrLog) { m_strLog = rstrLog; }

		void SetLogChannelUID(CONST_REF(SLogChannelUID) rsLogChannelUID) { m_sLogChannelUID = rsLogChannelUID; }

	protected:	
		virtual SPromise<BOOLN> _OnCall(PTR_OR_REF(IServiceBackend) roResolver, CONST_REF(SServiceFrontendID) rsAdapterID) QOVERRIDE QFINAL;

	private:
		SLogChannelUID m_sLogChannelUID;
		CString m_strLog = "";
		UINT8 m_btLogLevel = 0;
	};
}

#endif
