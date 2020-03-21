#ifndef _H_I_RESOLVER_SENDER_H_
#define _H_I_RESOLVER_SENDER_H_

#include "../CTypeDefine.h"

namespace QWER
{
	QCLASS IServiceMsg;
	QTEMPLATE<typename T1, typename T2> QCLASS CDuplexDataPipePeer;

	QCLASS CResolverSender
	{
#ifdef _USING_CLI
	internal:
#else
	private:
		friend class IServiceBackend;
#endif
		void _SendMsgToAdapter(PTR(IServiceMsg) poMsg);
		inline void _Init(PTR_OR_REF(CDuplexDataPipePeer<QPACK(PTR(IServiceMsg), PTR(IServiceMsg))>) roMsgPipe)
		{ 
			m_poMsgPipe = TO_PTR2(roMsgPipe);
		}
	private:
		PTR(CDuplexDataPipePeer<QPACK(PTR(IServiceMsg), PTR(IServiceMsg))>) m_poMsgPipe = NULL_PTR;
	};
}

#endif