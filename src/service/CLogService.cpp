#include "CLogService.h"
#include "CLogServiceBackend.h"
#include "CLogServiceFrontend.h"

namespace QWER
{
	CLogServiceImpl::CLogServiceImpl(PTR_OR_REF(IRunner) roRunner)
	{
		m_poResolver = QNEW CLogServiceBackend(roRunner);
	}

#ifndef _USING_CLI
	CLogServiceImpl::~CLogServiceImpl()
	{
		SAFE_DELETE(m_poResolver);
	}
#endif

	CUniquePtr(IServiceFrontend) CLogServiceImpl::_NewFrontend()
	{
		return QMakeUniquePtr(CLogServiceFrontend);
	}

	PTR(IServiceBackend) CLogServiceImpl::_GetBackend()
	{
		return m_poResolver;
	}

	CLogService::CLogService(PTR_OR_REF(IRunner) roRunner)
	{
		m_poImpl = QNEW CLogServiceImpl(roRunner);
	}

#ifndef _USING_CLI
	CLogService::~CLogService()
	{
		SAFE_DELETE(m_poImpl);
	}
#endif

	PTR(CLogServiceFrontend) CLogService::Connect(PTR_OR_REF(CServiceFrontendHelper) roHelper)
	{
		return (PTR(CLogServiceFrontend))(TO_PTR2(roHelper)->Connect(TO_REF2(m_poImpl)));
	}

	PTR(CLogServiceFrontend) CLogService::Connect(PTR_OR_REF(CServiceBackendHelper) roHelper)
	{
		return (PTR(CLogServiceFrontend))(TO_PTR2(roHelper)->Connect(TO_REF2(m_poImpl)));
	}
}
