#include "CWriteLogPtc.h"
#include "CLogServiceBackend.h"
#include "IRunner.h"
		
namespace QWER
{
	SPromise<BOOLN> CWriteLogPtc::_OnCall(PTR_OR_REF(IServiceBackend) roResolver, CONST_REF(SServiceFrontendID) rsAdapterID)
	{
		PTR(CLogServiceBackend) poResolver = (PTR(CLogServiceBackend))(TO_PTR2(roResolver));
		poResolver->Print(m_sLogChannelUID, m_btLogLevel, m_strLog);
		PTR(CPromiseMgr) poPromiseMgr = poResolver->GetRunner()->GetPromiseMgr();
		return poPromiseMgr->Resolve(true);
	}
}
