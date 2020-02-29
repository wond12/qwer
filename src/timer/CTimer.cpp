#include "CTimer.h"

namespace QWER
{
	CTimer::CTimer(PTR_OR_REF(CTimerMgr) roTimerMgr)
	{
		m_poTimerMgr = TO_PTR2(roTimerMgr);
	}

	void CTimer::QConstructor(UINT64 qwUID)
	{
		m_sTimerUID = STimerUID(qwUID);
	}

	void CTimer::QDestructor()
	{
		SET_DELEGATE_TO_NULL(m_cbOnTimer);
		m_dwCurrent = 0;
		m_dwCount = 0;
		m_qwIntervalMilliSeconds = 0;
		m_qwFirstTimeoutTime = 0;
	}
};
