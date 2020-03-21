#ifndef _H_C_EXEC_TIMER_INFO_H_
#define	_H_C_EXEC_TIMER_INFO_H_

#include "STimerInfo.h"
#include "../CQueueDefine.h"

namespace QWER
{
	QCLASS CExecTimerInfo
	{
	public:
		CExecTimerInfo() {}
		CQueue<STimerInfo> m_oExecTimerInfo;
	};
};

#endif

