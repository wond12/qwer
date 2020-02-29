#ifndef _H_C_EXEC_TIMER_INFO_LIST_H_
#define	_H_C_EXEC_TIMER_INFO_LIST_H_

#include "CExecTimerInfo.h"

namespace QWER
{
	QCLASS CExecTimerInfoList
	{
	public:
		CVector(PTR_OR_VAL(CExecTimerInfo)) m_oExecTimerInfo;
		UINT64 m_qwEndTime = 0;
		UINT16 m_wOffset = 0;
	};
};

#endif

