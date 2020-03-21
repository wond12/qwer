#include "IPromise.h"
#include "CPromiseContainer.h"
#include "CPromiseMgr.h"
#include "../CTimerMgrDefine.h"

namespace QWER
{
#ifndef _USING_CLI
	IPromise::~IPromise()
	{
		if (m_sDelayUID.IsNull() == false)
			GetPromiseMgr()->GetTimerMgr()->StopTimer(m_sDelayUID);
		if (m_sTimeoutUID.IsNull() == false)
			GetPromiseMgr()->GetTimerMgr()->StopTimer(m_sTimeoutUID);
	}
#endif

	PTR(CPromiseMgr) IPromise::GetPromiseMgr()
	{
		return m_poOwner->GetOwner();
	}

	UINT64 IPromise::GetUID()
	{
		return GetOwner()->GetUID();
	}

	void IPromise::_OnTimeout(CONST_REF(STimerUID) rsTimerUID, UINT32 dwCurrent, UINT32 dwCount)
	{
		if (m_sDelayUID.ToUINT64() == rsTimerUID.ToUINT64())
		{
			m_sDelayUID = STimerUID();
			if (m_sTimeoutUID.IsNull() == false) {
				GetPromiseMgr()->GetTimerMgr()->StopTimer(m_sTimeoutUID);
				m_sTimeoutUID = STimerUID();
			}
			_Resolve();
		}
		else
		{
			m_sTimeoutUID = STimerUID();
			if (m_sDelayUID.IsNull() == false) {
				GetPromiseMgr()->GetTimerMgr()->StopTimer(m_sDelayUID);
				m_sDelayUID = STimerUID();
			}
			_Reject(EPromiseErrorType::TimeoutError);
		}
		CPromiseHelper::AddToExecList(this);
	}

	void IPromise::_SetDelay(UINT64 qwMilliSec)
	{
		PTR(CTimerMgr) poTimerMgr = GetPromiseMgr()->GetTimerMgr();
		if (m_sDelayUID.IsNull() == false && poTimerMgr->IsRunningTimer(m_sDelayUID))
			return;
		m_sDelayUID = poTimerMgr->StartTimer(qwMilliSec, 1,
			NEW_DELEGATE(
				QPACK(DOnTimer),
				this, IPromise::_OnTimeout,
				DELEGATE_PLACEHOLDERS(, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)
			)
		);
	}

	void IPromise::_SetTimeout(UINT64 qwMilliSec)
	{
		PTR(CTimerMgr) poTimerMgr = GetPromiseMgr()->GetTimerMgr();
		if (m_sTimeoutUID.IsNull() == false && poTimerMgr->IsRunningTimer(m_sTimeoutUID))
			return;
		m_sTimeoutUID = poTimerMgr->StartTimer(qwMilliSec, 1,
			NEW_DELEGATE(
				QPACK(DOnTimer),
				this, IPromise::_OnTimeout,
				DELEGATE_PLACEHOLDERS(, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)
			)
		);
	}

	PTR(IPromise) IPromise::__Exec()
	{
		if (m_sDelayUID.IsNull() == false) {
			GetPromiseMgr()->GetTimerMgr()->StopTimer(m_sDelayUID);
			m_sDelayUID = STimerUID();
		}
		if (m_sTimeoutUID.IsNull() == false) {
			GetPromiseMgr()->GetTimerMgr()->StopTimer(m_sTimeoutUID);
			m_sTimeoutUID = STimerUID();
		}
		return _Exec();
	}
}
