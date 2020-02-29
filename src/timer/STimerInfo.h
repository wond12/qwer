#ifndef _H_S_EXT_TIMER_INFO_H_
#define	_H_S_EXT_TIMER_INFO_H_

#include "STimerUID.h"

namespace QWER
{
	QSTRUCT STimerInfo
	{
	public:
#ifndef _USING_CLI
		STimerInfo() {}
#endif
		STimerInfo(UINT64 qwExecTime, CONST_REF(STimerUID) rsTimerUID)
		{
			m_qwExecTime = qwExecTime;
			m_sTimerUID = rsTimerUID;
		}

		QINIT_ONLY UINT64 STRUCT_MEMBER_DEFINE(m_qwExecTime, 0);
		QINIT_ONLY STimerUID m_sTimerUID;
	};
};

#endif

