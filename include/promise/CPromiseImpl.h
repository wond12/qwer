#ifndef _H_C_PROMISE_IPML_H_
#define _H_C_PROMISE_IPML_H_

#include "../CTypeDefine.h"
#include "IPromiseImpl.h"
#include "IPromiseInterface.h"
#include "PromiseDelegate.h"

namespace QWER
{
	QTEMPLATE<typename T> QCLASS CPromise;
	QTEMPLATE<typename T> QSTRUCT SPromise;

	QTEMPLATE<typename T, typename U>
	QCLASS CPromiseImpl: public IPromiseImpl<T>
	{
	public:
		CPromiseImpl(PTR(IPromiseInterface<T>) poOwner, CONST_PTR_OR_REF(QPACK(DPromiseResolver<T, U>)) onResolve)
		{
			m_poOwner = poOwner;
			m_onResolver = onResolve;
		}

		virtual EPromiseStatus GetStatus() QCONST_FUNC QOVERRIDE
		{
			return m_eStatus;
		}

		PTR(IPromiseInterface<T>) Exec() QOVERRIDE
		{
			QWERASSERT(m_eStatus != EPromiseStatus::Pending, NULL_PTR);
			if (IS_NOT_NULL_DELEGATE(m_onResolver))
			{
				if (m_oArg.m_ePromiseError == EPromiseErrorType::Success)
				{
					SPromiseFuncArg<U> sArg(_GetPromiseMgr(), m_oArg.m_oData);
					this->m_oRet = m_onResolver(sArg);
				}
				else
				{
					SPromiseFuncArg<U> sArg(_GetPromiseMgr(), m_oArg.m_ePromiseError);
					this->m_oRet = m_onResolver(sArg);
				}
			}
			return this->m_oRet.m_poPromise;
		}

		void RecvParam(PTR(IPromise) poPromise) QOVERRIDE
		{
			if (NULL_PTR == poPromise)
			{
				_Reject(EPromiseErrorType::CancellationError);
			}
			else
			{
				PTR(IPromiseInterface<U>) poCPromise = (PTR(IPromiseInterface<U>))poPromise;
				CONST_REF(SPromiseData<U>) sRet = poCPromise->GetRet();
				_AssignParam(sRet);
			}
		}

		virtual void Resolve() QOVERRIDE
		{
			SPromiseData<U> sArg(EPromiseErrorType::Success);
			_AssignParam(sArg);
		}

		virtual void Reject(EPromiseErrorType eErrorType) QOVERRIDE
		{
			_Reject(eErrorType);
		}

	private:
		PTR(CPromiseMgr) _GetPromiseMgr()
		{
			return m_poOwner->GetPromiseMgr();
		}

	private:
		PTR(IPromiseInterface<T>) m_poOwner = NULL_PTR;
		PTR_OR_VAL(QPACK(DPromiseResolver<T, U>)) m_onResolver;
		SPromiseData<U> m_oArg;
		EPromiseStatus m_eStatus = EPromiseStatus::Pending;

#ifdef _USING_CLI
	internal:
#else
	private:
		friend QCLASS CPromise<T>;
		QTEMPLATE<typename V> friend QSTRUCT SPromise;
		friend QCLASS CPromiseMgr;
		friend QCLASS CPromiseHelper2;
#endif
		void _AssignParam(CONST_REF(SPromiseData<U>) rsArg)
		{
			if (rsArg.m_ePromiseError != EPromiseErrorType::CancellationError &&
				m_eStatus != EPromiseStatus::Pending)
				return;
			if (rsArg.m_poPromise != NULL_PTR)
				return;
			m_oArg = rsArg;
			if (m_oArg.m_ePromiseError == EPromiseErrorType::Success)
				m_eStatus = EPromiseStatus::Fulfilled;
			else
				m_eStatus = EPromiseStatus::Rejected;
		}

		void _Resolve(CONST_REF(U) oData)
		{
			SPromiseData<U> sArg(oData);
			_AssignParam(sArg);
		}

		void _Reject(EPromiseErrorType eErrorType)
		{
			SPromiseData<U> sArg(eErrorType);
			_AssignParam(sArg);
		}
	};
}

#endif
