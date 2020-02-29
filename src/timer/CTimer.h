#ifndef _H_C_TIMER_H_
#define	_H_C_TIMER_H_

#include "TimerDelegate.h"
#include "../CObjectCacheDefine.h"
#include "STimerUID.h"

namespace QWER
{
	QCLASS CTimerMgr;

	QCLASS CTimer QFINAL : public IObjRes
	{
	public:
		CTimer(PTR_OR_REF(CTimerMgr) roTimerMgr);
		void QConstructor(UINT64 qwUID);
		void QDestructor();

		inline PTR(CTimerMgr) GetTimerMgr() { return m_poTimerMgr; }
		inline CONST_REF(STimerUID) GetTimerUID() QCONST_FUNC { return m_sTimerUID; }

	public:
		PTR_OR_VAL(DOnTimer) m_cbOnTimer;
		UINT32 m_dwCurrent = 0;
		UINT32 m_dwCount = 0;
		UINT64 m_qwIntervalMilliSeconds = 0;
		UINT64 m_qwFirstTimeoutTime = 0;

	private:
		PTR(CTimerMgr) m_poTimerMgr = NULL_PTR;
		STimerUID m_sTimerUID;
	};
};

#endif

