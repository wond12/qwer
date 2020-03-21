#ifndef _H_S_LOG_CHANNEL_UID_H_
#define _H_S_LOG_CHANNEL_UID_H_

#include "../CTypeDefine.h"

namespace QWER
{
	QDLL_EXPORT QSTRUCT SLogChannelUID
	{
	public:
#ifndef _USING_CLI
		SLogChannelUID(){}
#endif
		SLogChannelUID(UINT64 qwUID)
		{
			m_qwUID = qwUID;
		}
        inline UINT64 ToUINT64() QCONST_FUNC { return m_qwUID; }
		inline BOOLN IsNull() QCONST_FUNC { return m_qwUID == 0; }

	private:
		QINIT_ONLY UINT64 STRUCT_MEMBER_DEFINE(m_qwUID, 0);
	};
}

#endif