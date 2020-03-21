#include "CResolverSender.h"
#include "CDuplexDataPipePeer.h"
#include "IToServiceFrontendMsg.h"

namespace QWER
{
	void CResolverSender::_SendMsgToAdapter(PTR(IServiceMsg) poMsg)
	{
		m_poMsgPipe->Push(poMsg);
	}
}
