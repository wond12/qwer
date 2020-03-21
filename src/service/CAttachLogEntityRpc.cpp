#include "CAttachLogEntityRpc.h"
#include "CLogServiceBackend.h"
#include "IRunner.h"

namespace QWER
{
	SPromise<BOOLN> CAttachLogEntity::_OnCall(PTR_OR_REF(IServiceBackend) roResolver, CONST_REF(SServiceFrontendID) rsAdapterID)
	{
		PTR(CLogServiceBackend) poResolver = (PTR(CLogServiceBackend))(TO_PTR2(roResolver));
		PTR(CLogChannel) poLogChannel = poResolver->GetLogChannel(m_oArg.m_sLogChannelUID);
		if (poLogChannel != NULL_PTR) {
			PTR(CLogEntity) poLogEntity = poResolver->CreateLogEntity(poLogChannel, m_oArg.m_cbNewLogEntity);
			if (NULL_PTR != poLogEntity)
				m_oRet.m_sLogEntityUID = poLogEntity->GetUID();
		}
		PTR(CPromiseMgr) poPromiseMgr = poResolver->GetRunner()->GetPromiseMgr();
		return poPromiseMgr->Resolve(true);
	}
}
