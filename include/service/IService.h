#ifndef _H_I_SERVICE_H_
#define _H_I_SERVICE_H_

#include "../CTypeDefine.h"
#include "../UnOrderedMapDefine.h"
#ifndef _USING_CLI
#include <mutex>
#endif
#include "../VectorDefine.h"
#include "../SmartPtrDefine.h"

namespace QWER
{
	QCLASS IRunner;
	QCLASS IServiceFrontend;
	QCLASS IServiceBackend;
	QCLASS CServiceFrontendHelper;
	QCLASS CServiceBackendHelper;

	QDLL_EXPORT QCLASS IService QABSTRACT
	{
		NON_COPYABLE(IService)
	public:
		IService(){}
#ifndef _USING_CLI
		virtual ~IService();
#endif
		PTR(IRunner) GetRunner();

	protected:
		virtual CUniquePtr(IServiceFrontend) _NewFrontend() = 0;
		virtual PTR(IServiceBackend) _GetBackend() = 0;

	private:
		CUnOrderedMap(PTR(IRunner), PTR(IServiceFrontend)) m_oConnectedServiceAdapters;
		CVector(PTR(IServiceFrontend)) m_oServiceAdapters;
#ifdef _USING_CLI
		CRandom m_oConnectMutex;
#else
		std::mutex m_oConnectMutex;
		std::mutex m_oInitMutex;
#endif

#ifdef _USING_CLI
	internal:
#else
	private:
		friend class CServiceFrontendHelper;
		friend class CServiceBackendHelper;
		friend class ILiteService;
        friend class IRunner;
#endif
		PTR(IServiceFrontend) _Connect(PTR_OR_REF(IRunner) roRunner);
	};
}

#endif
