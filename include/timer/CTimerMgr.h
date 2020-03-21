#ifndef CTIMERMGR_H
#define	CTIMERMGR_H

#include "../CTypeDefine.h"
#ifdef _USING_CLI
#endif
#include "../CObjectResMgrDefine.h"
#include "STimerUID.h"
#include "TimerDelegate.h"

#ifndef _USING_CLI
#define TIMER_LIST_COUNT 5
#endif

namespace QWER
{
	QCLASS CTimer;
	QCLASS CExecTimerInfoList;
	QSTRUCT STimerInfo;

	QDLL_EXPORT QCLASS CTimerMgr
	{
		NON_COPYABLE(CTimerMgr)
	public:
		CTimerMgr();
#ifndef _USING_CLI
		~CTimerMgr();
#endif
		inline void SetCurTime(UINT64 qwMilliSeconds) { m_qwCurTime = qwMilliSeconds; }
		inline UINT64 GetCurTime() const { return m_qwCurTime; }
		inline double GetTimeScale() const { return m_dTimeScale; }
		inline void SetTimeScale(double dTimeScale) { m_dTimeScale = dTimeScale; }
		UINT32 Poll(UINT64 qwDeltaMilliSeconds);
        STimerUID StartTimer(UINT64 dwMilliSeconds, CONST_PTR_OR_REF(DOnTimer) ronTimer);
        STimerUID StartTimer(UINT64 dwMilliSeconds, UINT32 dwCount, CONST_PTR_OR_REF(DOnTimer) ronTimer);
        STimerUID StartTimer(UINT64 dwMilliSeconds, UINT32 dwCount, UINT64 dwFirstTimeoutMilliSeconds, CONST_PTR_OR_REF(DOnTimer) ronTimer);
        BOOLN IsRunningTimer(CONST_REF(STimerUID) rsTimerUID) QCONST_FUNC;
        void StopTimer(CONST_REF(STimerUID) rsTimerUID);

		inline UINT32 GetMaxCountPerPoll() QCONST_FUNC { return m_dwMaxCountPerPoll; }
		inline void SetMaxCountPerPoll(UINT32 dwMaxCountPerPoll) { m_dwMaxCountPerPoll = dwMaxCountPerPoll; }

	private:
		CUniquePtr(CTimer) _NewTimer(UINT16 wObjType);
		void _AddTimer(CONST_REF(STimerUID) rsTimerUID,	UINT64 qwExecTime);
		UINT32 _ExecTimer(UINT32 dwMaxCount);
		void _Tick(UINT16 wLevel);

	private:
#ifdef _USING_CLI
		static UINT32 TIMER_LIST_COUNT = 5;
		static array<UINT64>^ m_aTimeOffsets = gcnew array<UINT32>{100, 10100, 1010100, 101010100, 10101010100};
#else
		static const UINT64 m_aTimeOffsets[TIMER_LIST_COUNT];
#endif
        PTR(CObjectResMgr<PTR_OR_VAL(CTimer)>) m_poTimerMgr = NULL_PTR;
		UINT64 m_qwCurTime = 0;
#ifdef _USING_CLI
		array<CExecTimerInfoList^>^ m_aTimers;
#else
		CExecTimerInfoList* m_aTimers[TIMER_LIST_COUNT];
#endif
		PTR(CVector(STimerInfo)) m_poExtTimers = NULL_PTR;
		double m_dTimeScale = 0;
		UINT64 m_qwDeltaTime = 0;
		UINT32 m_dwMaxCountPerPoll = 100;
	};
};

#endif	/* CTIMERMGR_H */

