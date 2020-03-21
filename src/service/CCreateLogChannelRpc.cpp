#include "CCreateLogChannelRpc.h"
#include "CLogServiceBackend.h"
#include "IRunner.h"

namespace QWER
{
	SPromise<BOOLN> CCreateLogChannelRpc::_OnCall(PTR_OR_REF(IServiceBackend) roResolver, CONST_REF(SServiceFrontendID) rsAdapterID)
	{
		PTR(CLogServiceBackend) poResolver = (PTR(CLogServiceBackend))(TO_PTR2(roResolver));
		PTR(CLogChannel) poLogChannel = poResolver->CreateLogChannel();
		if (NULL_PTR != poLogChannel)
			m_oRet.m_sLogChannelUID = poLogChannel->GetUID();
		PTR(CPromiseMgr) poPromiseMgr = poResolver->GetRunner()->GetPromiseMgr();
		return poPromiseMgr->Resolve(true);
	}
}
