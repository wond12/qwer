#include "CServiceBackendHelper.h"
#include "IServiceBackend.h"
#include "IService.h"
#include "IRunner.h"

namespace QWER
{
	PTR(IRunner) CServiceBackendHelper::GetRunner()
	{
		return m_poServiceResolver->_GetRunner();
	}

	SPromise<PTR(IToServiceFrontendMsg)> CServiceBackendHelper::PostMsg(CONST_REF(SServiceFrontendID) rsAdapterID, PTR(IToServiceFrontendMsg) poRpc)
	{
		return m_poServiceResolver->_PostMsg(rsAdapterID, poRpc);
	}

	UINT16 CServiceBackendHelper::GetID() QCONST_FUNC
	{
		return m_poServiceResolver->_GetID();
	}

	void CServiceBackendHelper::_Init(PTR(IServiceBackend) poServiceResolver)
	{
		m_poServiceResolver = poServiceResolver;
	}

	PTR(IServiceFrontend) CServiceBackendHelper::Connect(PTR_OR_REF(IService) roService)
	{
		return TO_PTR2(roService)->_Connect(TO_REF2(GetRunner()));
	}

	void CServiceBackendHelper::SetUpdating(BOOLN bUpdating)
	{
		if (m_bUpdating == bUpdating)
			return;
		m_bUpdating = bUpdating;
		if (m_bUpdating)
			GetRunner()->_AddUpdate(m_poServiceResolver);
	}
}
