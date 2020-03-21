#include "CServiceFrontendHelper.h"
#include "IServiceFrontend.h"
#include "IService.h"
#include "IRunner.h"

namespace QWER
{
	PTR(IRunner) CServiceFrontendHelper::GetRunner()
	{
		return m_poServiceAdapter->_GetRunner();
	}

	SPromise<PTR(IToServiceBackendMsg)> CServiceFrontendHelper::PostMsg(PTR(IToServiceBackendMsg) poRpc)
	{
		return m_poServiceAdapter->_PostMsg(poRpc);
	}

	CONST_REF(SServiceFrontendID) CServiceFrontendHelper::GetID() QCONST_FUNC
	{
		return m_poServiceAdapter->_GetID();
	}

	void CServiceFrontendHelper::_Init(PTR(IServiceFrontend) poServiceAdapter)
	{
		m_poServiceAdapter = poServiceAdapter;
	}

	PTR(IServiceFrontend) CServiceFrontendHelper::Connect(PTR_OR_REF(IService) roService)
	{
		return TO_PTR2(roService)->_Connect(TO_REF2(GetRunner()));
	}

	void CServiceFrontendHelper::SetUpdating(BOOLN bUpdating)
	{
		if (m_bUpdating == bUpdating)
			return;
		m_bUpdating = bUpdating;
		if (m_bUpdating)
			GetRunner()->_AddUpdate(m_poServiceAdapter);
	}
}
