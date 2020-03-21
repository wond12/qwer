#ifndef _H_C_PROMISE_H_
#define _H_C_PROMISE_H_

#include "../CTypeDefine.h"
#ifdef _USING_CLI
#else
#include <list>
#include <stdexcept>
#include <iostream>
#endif
#include "CPromiseHelper.h"
#include "IPromise.h"
#include "IPromiseInterface.h"
#include "CPromiseImpl.h"

namespace QWER
{
	QCLASS CPromiseMgr;
	QCLASS CPromiseTimer;

	QTEMPLATE<typename T>
	QDLL_EXPORT QCLASS CPromise : public IPromiseInterface<T>
	{
	public:
#ifndef _USING_CLI
		~CPromise()
		{
			SAFE_DELETE(m_poImpl);
		}
#endif
		PTR(CPromise<BOOLN>) Delay(UINT64 qwMilliSeconds)
		{
			PTR(CPromise<BOOLN>) poPromise = CPromise<BOOLN>::_CreatePromise(this->GetPromiseMgr());
			poPromise->_SetDelayMilliSecond(qwMilliSeconds);
			_SetResolver<BOOLN, BOOLN>(poPromise, NEW_DELEGATE(QPACK(DPromiseResolver<BOOLN, BOOLN>), poPromise, CPromise<BOOLN>::_OnCallDelay, DELEGATE_PLACEHOLDERS(, std::placeholders::_1)));
			CPromiseHelper::Then(this, poPromise);
			return poPromise;
		}
		QTEMPLATE<typename U>
		PTR(CPromise<U>) Then(CONST_PTR_OR_REF(QPACK(DPromiseResolver<U, T>)) onResolve)
		{
			PTR(CPromise<U>) poPromise = CPromise<U>::_CreatePromise1(this->GetPromiseMgr(), onResolve);
			CPromiseHelper::Then(this, poPromise);
			return poPromise;
		}

		virtual EPromiseStatus GetStatus() QCONST_FUNC QOVERRIDE
		{
			return m_poImpl->GetStatus();
		}

		virtual CONST_REF(SPromiseData<T>) GetRet() QCONST_FUNC QOVERRIDE
		{
			return m_poImpl->GetRet();
		}

	protected:
		virtual PTR(IPromise) _Exec() QOVERRIDE
		{
			return m_poImpl->Exec();
		}

		virtual void _RecvParam(PTR(IPromise) poPromise) QOVERRIDE
		{
			return m_poImpl->RecvParam(poPromise);
		}

		virtual void _Resolve() QOVERRIDE
		{
			m_poImpl->Resolve();
		}

		virtual void _Reject(EPromiseErrorType eErrorType) QOVERRIDE
		{
			m_poImpl->Reject(eErrorType);
		}

	protected:
		PTR(IPromiseImpl<T>) m_poImpl = NULL_PTR;
		UINT64 m_qwDelayMilliSecond = 0;

#ifdef _USING_CLI
	internal:
#else
	private:
		friend QCLASS CPromiseMgr;
		template<typename U> friend QCLASS CPromise;
		template<typename U> friend QSTRUCT SPromise;
		friend QCLASS CPromiseHelper;
		friend QCLASS CPromiseHelper2;
#endif
		CPromise()
		{
		}
		PTR(IPromiseImpl<T>) _GetImpl() { return m_poImpl; }
		void _SetImpl(PTR(IPromiseImpl<T>) poImpl){ m_poImpl = poImpl; }
		void _SetDelayMilliSecond(UINT64 qwDelayMilliSecond){ m_qwDelayMilliSecond = qwDelayMilliSecond; }
		QTEMPLATE<typename U, typename V>
		static void _SetResolver(PTR(CPromise<U>) poPromise, CONST_PTR_OR_REF(QPACK(DPromiseResolver<U, V>)) onResolve)
		{
			PTR(IPromiseImpl<U>) poImpl = poPromise->_GetImpl();
			QWERASSERT(poImpl == NULL_PTR, );
			poPromise->_SetImpl(QNEW CPromiseImpl<U, V>(poPromise, onResolve));
		}

		static PTR(CPromise<T>) _CreatePromise(PTR(CPromiseMgr) poMgr)
		{
			PTR(CPromise<T>) poRet = QNEW CPromise<T>();
			CPromiseHelper::InitPromise(poMgr, poRet);
			return poRet;
		}
		QTEMPLATE<typename U, typename V>
		static PTR(CPromise<U>) _CreatePromise1(PTR(CPromiseMgr) poMgr, CONST_PTR_OR_REF(QPACK(DPromiseResolver<U, V>)) onResolve)
		{
			PTR(CPromise<U>) poPromise = CPromise<U>::_CreatePromise(poMgr);
			_SetResolver<U, V>(poPromise, onResolve);
			return poPromise;
		}
		static SPromiseData<T> _OnPostResolve(CONST_REF(SPromiseFuncArg<T>) sArg)
		{
			return sArg.ToPromiseData();
		}
		SPromiseData<BOOLN> _OnCallDelay(CONST_REF(SPromiseFuncArg<T>) sArg)
		{
			PTR(CPromise<BOOLN>) p = (PTR(CPromise<BOOLN>))CPromiseHelper::Delay(this->GetPromiseMgr(), m_qwDelayMilliSecond);
			return SPromiseData<BOOLN>(p, true);
		}
	};
}

#endif
