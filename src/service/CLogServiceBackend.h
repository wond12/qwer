#ifndef _H_C_LOG_SERVICE_RESOLVER_H_
#define _H_C_LOG_SERVICE_RESOLVER_H_

#include "CLogHelper.h"
#include "IServiceBackend.h"
#include "CLogEntity.h"
#include "../CObjectResMgrDefine.h"
#include "LogDelegate.h"
#include "CLogChannel.h"

namespace QWER
{
	QDLL_EXPORT QCLASS CLogServiceBackend : public IServiceBackend
	{
	public:
		CLogServiceBackend(PTR_OR_REF(IRunner) roRunner);
#ifndef _USING_CLI
		~CLogServiceBackend();
#endif
#ifdef _USING_CLI
		void Register(PTR(ILog) pologInstance, ...array<UINT8>^ args)
		{
			if (args == NULL_PTR || args->Length == 0)
				return;
			for (INT32 i = 0; i != args->Length; ++i)
				_Register(pologInstance, args[i]);
		}
#else
		template<typename... Args>
		void Register(PTR(ILog) rologInstance, Args... args)
		{
			_Register(rologInstance, std::forward<Args>(args)...);
		}
#endif
		PTR(IRunner) GetRunner(){ return m_poHelper->GetRunner(); }
		PTR(CLogChannel) CreateLogChannel();
		PTR(CLogChannel) GetLogChannel(CONST_REF(SLogChannelUID) rsLogChannelUID);
		void ReleaseLogChannel(CONST_REF(SLogChannelUID) rsLogChannelUID);
		PTR(CLogEntity) CreateLogEntity(PTR(CLogChannel) poLogChanel, CONST_PTR_OR_REF(DNewLogEntity) cbNewLogEntity);
		void ReleaseLogEntity(CONST_REF(SLogEntityUID) rsLogEntityUID);
		void Print(CONST_REF(SLogChannelUID) rsLogChannelUID, UINT8 btLogLevel, CONST_REF(CString) rstrLog);

	protected:
		virtual void _Awake(PTR_OR_REF(CServiceBackendHelper) roHelper) QOVERRIDE;
		virtual UINT32 _Update() QOVERRIDE;

	private:
#ifndef _USING_CLI
		template<typename... Args>
		void _Register(PTR(ILog) rologInstance, UINT8 btLogType, Args... args)
		{
			m_oLogHelper.Register(rologInstance, btLogType);
			_Register(rologInstance, std::forward<Args>(args)...);
		}
#endif

		void _Register(PTR(ILog) rologInstance, UINT8 btLogType);
		CUniquePtr(CLogEntity) _NewLogEntity(UINT16 wObjType);
		CUniquePtr(CLogChannel) _NewLogChannel(UINT16 wObjType);

	private:
		PTR(CServiceBackendHelper) m_poHelper = NULL_PTR;
		PTR(CObjectResMgr<PTR_OR_VAL(CLogEntity)>) m_poLogEntityMgr = NULL_PTR;
		PTR(CObjectResMgr<PTR_OR_VAL(CLogChannel)>) m_poLogChannelMgr = NULL_PTR;
	};
}

#endif