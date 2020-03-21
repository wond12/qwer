#ifndef _H_C_LOG_SERVICE_ADAPTER_H_
#define _H_C_LOG_SERVICE_ADAPTER_H_

#include "IServiceFrontend.h"
#ifndef _USING_CLI
#include <sstream>
#endif
#include "../StringDefine.h"
#include "SLogChannelUID.h"
#include "SLogEntityUID.h"
#include "LogDelegate.h"
#include "../VectorDefine.h"

namespace QWER
{
	QCLASS CLogServiceHelper;

	QDLL_EXPORT QCLASS CLogServiceFrontend : public IServiceFrontend
	{
	public:	

#ifdef _USING_CLI
		void Print(CONST_REF(SLogChannelUID) rsLogChannelUID, UINT8 btLogLevel, ...array<System::Object^>^ msg);
			
#else
		template<typename... Args>
		void Print(CONST_REF(SLogChannelUID) rsLogChannelUID, UINT8 btLogLevel, Args... args)
		{
			std::stringstream ss;
			_Print(rsLogChannelUID, ss, btLogLevel, std::forward<Args>(args)...);
		}
		void Print(CONST_REF(SLogChannelUID) rsLogChannelUID, UINT8 btLogLevel, CONST_REF(CString) strLog);
		void Print(CONST_REF(SLogChannelUID) rsLogChannelUID, UINT8 btLogLevel, const char* pchLog);
#endif
		SPromise<SLogChannelUID> Create();
#ifdef _USING_CLI	
		SPromise<SLogEntityUID> Attach(CONST_REF(SLogChannelUID) rsLogChannelUID, CONST_PTR_OR_REF(DNewLogEntity) cbNewLogEntity, ...array<UINT8>^ aLogLevels);
		void Detach(CONST_REF(SLogChannelUID) rsLogChannelUID, CONST_REF(SLogEntityUID) rsLogEntityUID, ...array<UINT8>^ aLogLevels);
#else
		template<typename... Args>
		SPromise<SLogEntityUID> Attach(CONST_REF(SLogChannelUID) rsLogChannelUID, CONST_PTR_OR_REF(DNewLogEntity) cbNewLogEntity, Args... aLogLevels)
		{
			CVector(UINT8) oLogLevels;
			_LogLevels(oLogLevels, , std::forward<Args>(aLogLevels)...);
			return Attach(rsLogChannelUID, cbNewLogEntity, oLogLevels);
		}
		template<typename... Args>
		void Detach(CONST_REF(SLogEntityUID) rsLogEntityUID, Args... aLogLevels)
		{
			CVector(UINT8) oLogLevels;
			_LogLevels(oLogLevels, , std::forward<Args>(aLogLevels)...);
			Detach(rsLogEntityUID, oLogLevels);
		}
#endif
		SPromise<SLogEntityUID> Attach(CONST_REF(SLogChannelUID) rsLogChannelUID, CONST_PTR_OR_REF(DNewLogEntity) cbNewLogEntity, CONST_PTR_OR_REF(CVector(UINT8)) roLogLevels);
		void Detach(CONST_REF(SLogEntityUID) rsLogEntityUID, CONST_PTR_OR_REF(CVector(UINT8)) roLogLevels);
		void Detach(CONST_REF(SLogEntityUID) rsLogEntityUID);

	protected:
		virtual void _Awake(PTR_OR_REF(CServiceFrontendHelper) roHelper) QOVERRIDE;
		virtual UINT32 _Update() QOVERRIDE;

	private:
#ifndef _USING_CLI
		template<typename T, typename... Args>
		void _Print(CONST_REF(SLogChannelUID) rsLogChannelUID, std::stringstream& ss, UINT8 btLogLevel, T t, Args... args)
		{
			ss << t;
			_Print(ss, btLogLevel, std::forward<Args>(args)...);
		}
		template<typename T>
		void _Print(CONST_REF(SLogChannelUID) rsLogChannelUID, std::stringstream& ss, UINT8 btLogLevel, T t)
		{
			ss << t;
			_Print(btLogLevel, ss);
		}
		void _Print(CONST_REF(SLogChannelUID) rsLogChannelUID, UINT8 btLogLevel, std::stringstream& roMsg);
		template<typename... Args>
		void _LogLevels(CVector(UINT8)& roLogLevels, UINT8 btLogLevel, Args... aLogLevels)
		{
			roLogLevels.push_back(btLogLevel);
			_LogLevels(roLogLevels, std::forward<Args>(aLogLevels)...);
		}
		void _LogLevels(CVector(UINT8)& roLogLevels, UINT8 btLogLevel)
		{
			roLogLevels.push_back(btLogLevel);
		}
#endif

		SPromiseData<BOOLN> _OnWriteLogReply(CONST_REF(SPromiseFuncArg<PTR(IToServiceBackendMsg)>) p);
		SPromiseData<SLogChannelUID> _OnCreateLogChannelReply(CONST_REF(SPromiseFuncArg<PTR(IToServiceBackendMsg)>) p);
		SPromiseData<SLogEntityUID> _OnAttachLogEntityReply(CONST_REF(SPromiseFuncArg<PTR(IToServiceBackendMsg)>) p);
		SPromiseData<BOOLN> _OnDetachLogEntityReply(CONST_REF(SPromiseFuncArg<PTR(IToServiceBackendMsg)>) p);

	private:
		PTR(CServiceFrontendHelper) m_poHelper = NULL_PTR;
	};
}

#endif
