#ifndef H_S_TIMER_UID_H
#define H_S_TIMER_UID_H

#include "../CTypeDefine.h"

namespace QWER
{
	QDLL_EXPORT QSTRUCT STimerUID
    {
    public:
#ifndef _USING_CLI
        STimerUID() {}
#endif
        STimerUID(UINT64 qwUID)
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