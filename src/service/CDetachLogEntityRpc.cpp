#include "CDetachLogEntityRpc.h"
#include "CLogServiceBackend.h"
#include "IRunner.h"

namespace QWER
{
	SPromise<BOOLN> CDetachLogEntity::_OnCall(PTR_OR_REF(IServiceBackend) roResolver, CONST_REF(SServiceFrontendID) rsAdapterID)
	{
		PTR(CLogServiceBackend) poResolver = (PTR(CLogServiceBackend))(TO_PTR2(roResolver));
		poResolver->ReleaseLogEntity(m_oArg.m_sLogEntityUID);
		PTR(CPromiseMgr) poPromiseMgr = poResolver->GetRunner()->GetPromiseMgr();
		return poPromiseMgr->Resolve(true);
	}
}