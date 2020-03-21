#ifndef _H_I_PROMISE_H_
#define _H_I_PROMISE_H_

#include "../CTypeDefine.h"
#include "SPromiseData.h"
#include "PromiseDelegate.h"
#ifdef _USING_CLI
#else
#include <list>
#include <stdexcept>
#endif
#include "EPromiseStatus.h"
#include "../STimerUIDDefine.h"

namespace QWER
{
	QCLASS CPromiseContainer;

	QDLL_EXPORT QCLASS IPromise QABSTRACT
	{
	public:
#ifndef _USING_CLI
		virtual ~IPromise();
#endif
		virtual EPromiseStatus GetStatus() QCONST_FUNC = 0;
		PTR(CPromiseMgr) GetPromiseMgr();
		inline void Reject(EPromiseErrorType eErrorType){ return _Reject(eErrorType); }
		UINT64 GetUID();

	protected:
		virtual PTR(IPromise) _Exec() = 0;
		virtual void _RecvParam(PTR(IPromise) poPromise) = 0;
		virtual void _Resolve() = 0;
		virtual void _Reject(EPromiseErrorType eErrorType) = 0;
		void _SetDelay(UINT64 qwMilliSec);
		void _SetTimeout(UINT64 qwMilliSec);

	private:
		PTR(CPromiseContainer) m_poOwner = NULL_PTR;
		STimerUID m_sDelayUID;
		STimerUID m_sTimeoutUID;

#ifdef _USING_CLI
	internal:
#else
	protected:
		friend QCLASS CPromiseHelper;
		friend QCLASS CPromiseMgr;
		friend QCLASS CPromiseContainer;
		friend QCLASS CPromiseList;
#endif
		inline PTR(CPromiseContainer) GetOwner() { return m_poOwner; }
		void _OnTimeout(CONST_REF(STimerUID) rsTimerUID, UINT32 dwCurrent, UINT32 dwCount);
		inline void __SetDelay(UINT64 qwMilliSec) { return _SetDelay(qwMilliSec); }
		inline void __SetTimeout(UINT64 qwMilliSec) { return _SetTimeout(qwMilliSec); }
		PTR(IPromise) __Exec();
		inline void __RecvParam(PTR(IPromise) poPromise){ return _RecvParam(poPromise); }
		inline void _SetOwner(PTR(CPromiseContainer) poOwner){ m_poOwner = poOwner; }
	};
}

#endif