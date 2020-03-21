#ifndef _H_C_LOG_SERVICE_H_
#define _H_C_LOG_SERVICE_H_

#include "IService.h"
#include "CLogServiceBackend.h"

namespace QWER
{
	QINTERFACE ILog;
	QCLASS CLogServiceFrontend;
	QCLASS CLogServiceBackend;

	QCLASS CLogServiceImpl : public IService
	{
	public:
		CLogServiceImpl(PTR_OR_REF(IRunner) roRunner);
#ifndef _USING_CLI
		~CLogServiceImpl();
#endif

	protected:
		virtual CUniquePtr(IServiceFrontend) _NewFrontend() QOVERRIDE;
		virtual PTR(IServiceBackend) _GetBackend() QOVERRIDE;

	private:
		PTR(CLogServiceBackend) m_poResolver = NULL_PTR;
	};

	QDLL_EXPORT QCLASS CLogService
	{
		NON_COPYABLE(CLogService)
	public:
		CLogService(PTR_OR_REF(IRunner) roRunner);
#ifndef _USING_CLI
		~CLogService();
#endif
		PTR(CLogServiceFrontend) Connect(PTR_OR_REF(CServiceFrontendHelper) roHelper);
		PTR(CLogServiceFrontend) Connect(PTR_OR_REF(CServiceBackendHelper) roHelper);
		PTR(IRunner) GetRunner(){ return m_poImpl->GetRunner(); }

	private:
		PTR(CLogServiceImpl) m_poImpl;
	};
}

#endif
