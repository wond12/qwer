#include "CTimerMgr.h"
#include "CTimer.h"
#include "CExecTimerInfoList.h"

namespace QWER
{
	const UINT64 CTimerMgr::m_aTimeOffsets[TIMER_LIST_COUNT] = { 100, 10100, 1010100, 101010100, 10101010100 };

	CTimerMgr::CTimerMgr()
	{
#ifdef _USING_CLI
		m_aTimers = gcnew array<UINT8>(TIMER_LIST_COUNT);
#endif
		m_poExtTimers = QNEW CVector(STimerInfo)();
		m_poTimerMgr = QNEW CObjectResMgr<PTR_OR_VAL(CTimer)>(
			NEW_DELEGATE(
				QPACK(DNewObj<CTimer>),
				this, CTimerMgr::_NewTimer,
				DELEGATE_PLACEHOLDERS(, std::placeholders::_1)
			)
		);
	}

#ifndef _USING_CLI
	CTimerMgr::~CTimerMgr() {
		for (int i = 0; i != TIMER_LIST_COUNT; ++i)
			SAFE_DELETE(m_aTimers[i]);
		SAFE_DELETE(m_poExtTimers);
		m_poTimerMgr->ReleaseAll();
		SAFE_DELETE(m_poTimerMgr);
	}
#endif

	CUniquePtr(CTimer) CTimerMgr::_NewTimer(UINT16 wObjType)
    {
        CUniquePtr(CTimer) poTimer(QNEW CTimer(TO_REF2(this)));
		return QMOVE(poTimer);
    }

	UINT32 CTimerMgr::Poll(UINT64 qwDeltaMilliSeconds)
	{
		if (qwDeltaMilliSeconds == 0 || m_dTimeScale == 0)
			return 0;
		UINT32 dwCount = 0;
		m_qwDeltaTime += (UINT64)(qwDeltaMilliSeconds * m_dTimeScale);
		while ((m_dwMaxCountPerPoll == 0 || dwCount < m_dwMaxCountPerPoll) && m_qwDeltaTime >= qwDeltaMilliSeconds) {
			m_qwDeltaTime -= qwDeltaMilliSeconds;
			m_qwCurTime += qwDeltaMilliSeconds;
			dwCount += _ExecTimer(m_dwMaxCountPerPoll == 0 ? 0 : (m_dwMaxCountPerPoll - dwCount));
		}
		return dwCount;
	}

	UINT32 CTimerMgr::_ExecTimer(UINT32 dwMaxCount)
	{
		PTR(CExecTimerInfoList) poTimers = m_aTimers[0];
		if (poTimers == NULL_PTR)
			return 0;

		UINT32 dwCount = 0;
		for (; (dwMaxCount == 0 || dwCount < dwMaxCount) && m_qwCurTime >= poTimers->m_qwEndTime - m_aTimeOffsets[0] + poTimers->m_wOffset; _Tick(0)) {
			PTR(CExecTimerInfo) poExecTimerInfo = TO_PTR2(poTimers->m_oExecTimerInfo.at(poTimers->m_wOffset));
#ifdef _USING_CLI
			if (poExecTimerInfo == NULL_PTR)
				continue;
#endif
			for (; (dwMaxCount == 0 || dwCount < dwMaxCount) && !poExecTimerInfo->m_oExecTimerInfo.IsEmpty(); poExecTimerInfo->m_oExecTimerInfo.Pop()) {
				CONST_REF(STimerUID) rsTimerUID = poExecTimerInfo->m_oExecTimerInfo.At(0).m_sTimerUID;
				PTR(CTimer) poTimer = m_poTimerMgr->GetObj(rsTimerUID.ToUINT64());
				if (poTimer != NULL_PTR) {
					if (IS_NOT_NULL_DELEGATE(poTimer->m_cbOnTimer)) {
						++dwCount;
						poTimer->m_cbOnTimer(rsTimerUID, poTimer->m_dwCurrent++, poTimer->m_dwCount);
					}
					if (IsRunningTimer(rsTimerUID)) {
						if (poTimer->m_dwCount == 0 || poTimer->m_dwCurrent < poTimer->m_dwCount)
							_AddTimer(rsTimerUID, poTimer->m_qwFirstTimeoutTime + poTimer->m_qwIntervalMilliSeconds * poTimer->m_dwCurrent);
						else
							StopTimer(rsTimerUID);
					}
				}
			}
		}
		return dwCount;
	}

	void CTimerMgr::_Tick(UINT16 wLevel)
	{
		if (wLevel >= TIMER_LIST_COUNT)
			return;
		PTR(CExecTimerInfoList) poCurTimers = m_aTimers[wLevel];
		if (poCurTimers == NULL_PTR)
			return;
		PTR(CExecTimerInfo) poPreExecInfo = TO_PTR2(poCurTimers->m_oExecTimerInfo.at(poCurTimers->m_wOffset));
		if (poPreExecInfo != NULL_PTR && poPreExecInfo->m_oExecTimerInfo.IsEmpty() == false)
			return;

		UINT64 timeInterval = wLevel == 0 ? m_aTimeOffsets[wLevel] : m_aTimeOffsets[wLevel] - m_aTimeOffsets[wLevel - 1];
		UINT64 qwStep = (m_aTimeOffsets[wLevel] - (wLevel == 0 ? 0 : m_aTimeOffsets[wLevel - 1])) / m_aTimeOffsets[0];
		if (wLevel == 0 && poCurTimers->m_wOffset >= poCurTimers->m_oExecTimerInfo.size()) {
			if (m_qwCurTime >= poCurTimers->m_qwEndTime)
				poCurTimers->m_wOffset = (UINT16)m_aTimeOffsets[wLevel] - 1;
			else
				poCurTimers->m_wOffset = (UINT16)((m_qwCurTime + timeInterval - poCurTimers->m_qwEndTime) / qwStep);
		}
		++poCurTimers->m_wOffset;
		if (poCurTimers->m_wOffset < m_aTimeOffsets[0])
			return;
		poCurTimers->m_wOffset = 0;
		poCurTimers->m_qwEndTime = poCurTimers->m_qwEndTime + timeInterval;
		{
			CVector(PTR_OR_VAL(CExecTimerInfo)) tmp;
			poCurTimers->m_oExecTimerInfo.swap(TO_PTR1(tmp));
		}
		if (wLevel + 1 == TIMER_LIST_COUNT) {
			int nLen = m_poExtTimers->size();
			if (nLen != 0) {
				CVector(STimerInfo) oExtTimers;
				m_poExtTimers->swap(TO_PTR1(oExtTimers));
				for (int i = 0; i != nLen; ++i) {
					_AddTimer(oExtTimers.at(i).m_sTimerUID, oExtTimers.at(i).m_qwExecTime);
				}
			}
			return;
		}
		PTR(CExecTimerInfoList) poNextTimers = m_aTimers[wLevel + 1];
		PTR(CExecTimerInfo) poNextTimer = TO_PTR2(poNextTimers->m_oExecTimerInfo.at(poNextTimers->m_wOffset));
#ifdef _USING_CLI
		if (poNextTimer == NULL_PTR)
			return;
#endif
		UINT64 beginTime = poCurTimers->m_qwEndTime - timeInterval;
		for (int i = 0; i != poNextTimer->m_oExecTimerInfo.GetCount(); ++i) {
			CONST_REF(STimerInfo) execInfo = poNextTimer->m_oExecTimerInfo.At(i);
			UINT16 offset = (UINT16)((execInfo.m_qwExecTime - beginTime) / qwStep);
#ifdef _USING_CLI
			if (poCurTimers->m_oExecTimerInfo.at(offset) == null)
				TO_PTR(poCurTimers->m_oExecTimerInfo)[offset] = QNEW CVector(STimerInfo);
#endif
			poCurTimers->m_oExecTimerInfo.at(offset).m_oExecTimerInfo.Push(execInfo);
		}
		poNextTimer->m_oExecTimerInfo.Clear();
		_Tick(wLevel + 1);
	}

    STimerUID CTimerMgr::StartTimer(UINT64 dwMilliSeconds, CONST_PTR_OR_REF(DOnTimer) ronTimer)
    {
		return StartTimer(dwMilliSeconds, 0, dwMilliSeconds, ronTimer);
    }

    STimerUID CTimerMgr::StartTimer(UINT64 dwMilliSeconds, UINT32 dwCount, CONST_PTR_OR_REF(DOnTimer) ronTimer)
    {
		return StartTimer(dwMilliSeconds, dwCount, dwMilliSeconds, ronTimer);
    }

    STimerUID CTimerMgr::StartTimer(UINT64 dwMilliSeconds, UINT32 dwCount, UINT64 dwFirstTimeoutMilliSeconds, CONST_PTR_OR_REF(DOnTimer) ronTimer)
    {
		PTR(CTimer) poTimer = m_poTimerMgr->CreateObj(0);
		poTimer->m_qwFirstTimeoutTime = m_qwCurTime + dwFirstTimeoutMilliSeconds;
		poTimer->m_qwIntervalMilliSeconds = dwMilliSeconds;
		poTimer->m_dwCount = dwCount;
		poTimer->m_cbOnTimer = ronTimer;
		_AddTimer(poTimer->GetTimerUID(), poTimer->m_qwFirstTimeoutTime);
		return poTimer->GetTimerUID();
    }

    BOOLN CTimerMgr::IsRunningTimer(CONST_REF(STimerUID) rsTimerUID) QCONST_FUNC
    {
		if (rsTimerUID.IsNull())
			return false;
		PTR(CTimer) poTimer = m_poTimerMgr->GetObj(rsTimerUID.ToUINT64());
        return poTimer != NULL_PTR;
    }

    void CTimerMgr::StopTimer(CONST_REF(STimerUID) rsTimerUID)
    {
        m_poTimerMgr->ReleaseObj(rsTimerUID.ToUINT64());
    }

	void CTimerMgr::_AddTimer(CONST_REF(STimerUID) rsTimerUID, UINT64 qwExecTime)
	{
		BOOLN bAdd = false;
		for (UINT16 level = 0; level != TIMER_LIST_COUNT; ++level) {
			PTR(CExecTimerInfoList) poCurTimers = m_aTimers[level];
			if (poCurTimers == NULL_PTR)
				poCurTimers = m_aTimers[level] = QNEW CExecTimerInfoList();
			if (qwExecTime >= poCurTimers->m_qwEndTime)
				continue;
			bAdd = true;
			UINT64 qwBeginTime = poCurTimers->m_qwEndTime - (level == 0 ? m_aTimeOffsets[level] : m_aTimeOffsets[level] - m_aTimeOffsets[level - 1]);
			UINT64 qwStep = (m_aTimeOffsets[level] - (level == 0 ? 0 : m_aTimeOffsets[level - 1])) / m_aTimeOffsets[0];
			UINT16 wOffset = (UINT16)((qwExecTime - qwBeginTime) / qwStep);

			if (poCurTimers->m_oExecTimerInfo.size() <= wOffset)
				poCurTimers->m_oExecTimerInfo.resize(wOffset + 1);
			PTR(CExecTimerInfo) poExecTimerInfo = TO_PTR2(poCurTimers->m_oExecTimerInfo.at(wOffset));
#ifdef _USING_CLI
			if (poExecTimerInfo == NULL_PTR)
				poExecTimerInfo = TO_PTR(poCurTimers->m_oExecTimerInfo)[wOffset] = QNEW CExecTimerInfo();
#endif
			poExecTimerInfo->m_oExecTimerInfo.Push(STimerInfo(qwExecTime, rsTimerUID));
			break;
		}
		if (!bAdd) {
			m_poExtTimers->push_back(STimerInfo(qwExecTime, rsTimerUID));
		}
	}
};
