#ifndef _H_S_PROMISE_H_
#define _H_S_PROMISE_H_

#include "CPromise.h"
#include "CPromiseHelper.h"
#include "CPromiseContainer.h"

namespace QWER
{
	QTEMPLATE<typename U, typename T>
	QCLASS _CPromiseConvert
	{
	public:
		static SPromiseData<U> Convert(CONST_REF(SPromiseFuncArg<T>) rsArg, CONST_REF(U) roData)
		{
			if (rsArg.m_ePromiseError != EPromiseErrorType::Success)
				return SPromiseData<U>(rsArg.m_ePromiseError);
			return SPromiseData<U>(roData);
		}
	};

	QTEMPLATE<typename T>
	QDLL_EXPORT QSTRUCT SPromise
	{
	public:
#ifndef _USING_CLI
		SPromise()
		{
		}
#endif

		SPromise<BOOLN> Delay(UINT64 qwMilliSeconds)
		{
			if (m_qwPromiseUID != 0)
			{
				PTR(CPromise<T>) poPromise = _GetPromise();
				if (poPromise != NULL_PTR)
					return SPromise<BOOLN>(TO_REF2(poPromise->Delay(qwMilliSeconds)));
				else
					return SPromise<BOOLN>(EPromiseErrorType::CancellationError, m_poPromiseMgr);
			}
			PTR(CPromise<BOOLN>) poPromise = (PTR(CPromise<BOOLN>))(CPromiseHelper::Delay(m_poPromiseMgr, qwMilliSeconds));
			return SPromise<BOOLN>(TO_REF2(poPromise));
		}

		QTEMPLATE<typename U>
		SPromise<U> Then(CONST_PTR_OR_REF(QPACK(DPromiseResolver<U, T>)) onResolve)
		{
			if (m_qwPromiseUID != 0)
			{
				PTR(CPromise<T>) poThisPromise = _GetPromise();
				if (poThisPromise != NULL_PTR)
					return SPromise<U>(TO_REF2(poThisPromise->Then(onResolve)));
				else
					return SPromise<T>(EPromiseErrorType::CancellationError, m_poPromiseMgr).Then<U>(onResolve);
			}
			PTR(CPromise<U>) poPromise = CPromise<U>::_CreatePromise1(m_poPromiseMgr, onResolve);
			CPromiseHelper::AddToPromiseList(poPromise);
			PTR(QPACK(CPromiseImpl<U, T>)) poImpl = (PTR(QPACK(CPromiseImpl<U, T>)))poPromise->_GetImpl();
			if (m_ePromiseError == EPromiseErrorType::Success)
			{
				SPromiseData<T> sArg(m_oData);
				poImpl->_AssignParam(sArg);
			}
			else
			{
				SPromiseData<T> sArg(m_ePromiseError);
				poImpl->_AssignParam(sArg);
			}
			CPromiseHelper::AddToExecList(poPromise);
			return SPromise<U>(TO_REF2(poPromise));
		}

		BOOLN IsValidPromise()
		{
			return m_poPromiseMgr != NULL_PTR && (m_qwPromiseUID == 0 || (m_qwPromiseUID != 0 && _GetPromise() != NULL_PTR));
		}

		void Cancel()
		{
			PTR(CPromise<T>) poPromise = _GetPromise();
			if (poPromise == NULL_PTR)
				return;
			PTR(IPromise) poPromiseHead = CPromiseHelper::GetHeader(poPromise);
			poPromiseHead->Reject(EPromiseErrorType::CancellationError);
			CPromiseHelper::AddToExecList(poPromiseHead);
		}

		SPromiseData<T> ToPromiseData()
		{
			if (m_qwPromiseUID != 0)
			{
				PTR(CPromise<T>) poPromise = _GetPromise();
				if (poPromise != NULL_PTR)
					return SPromiseData<T>(poPromise, true);
				else
					return SPromiseData<T>(EPromiseErrorType::CancellationError);
			}
			if (m_ePromiseError == EPromiseErrorType::Success)
				return SPromiseData<T>(m_oData);
			return SPromiseData<T>(m_ePromiseError);
		}

		QTEMPLATE<typename U>
		SPromise<U> ToPromise(CONST_REF(U) roData)
		{
			return Then<U>(
				CONVERT_DELEGATE(
					QPACK(DPromiseResolver<U, T>),
					PACK_STATIC_DELEGATE_FUNC(
						QPACK(qwer_stl::function<SPromiseData<U>, SPromiseFuncArg<T>, U>),
						QPACK(_CPromiseConvert<U, T>::Convert)
					),
					DELEGATE_PLACEHOLDERS(, std::placeholders::_1),
					roData
				)
			);
		}

	private:
		PTR(CPromise<T>) _GetPromise()
		{
			return (PTR(CPromise<T>))CPromiseHelper::GetPromise(m_poPromiseMgr, m_qwPromiseUID);
		}

#ifdef _USING_CLI
	internal:
#else
	private:
		friend QCLASS CPromiseMgr;
		QTEMPLATE<typename U> friend QSTRUCT SPromise;
#endif

		SPromise(EPromiseErrorType ePromiseError, PTR(CPromiseMgr) poMgr)
		{
			m_ePromiseError = ePromiseError;
			m_oData = T();
			m_qwPromiseUID = 0;
			m_poPromiseMgr = poMgr;
		}

		SPromise(CONST_REF(T) oData, PTR(CPromiseMgr) poMgr)
		{
			m_ePromiseError = EPromiseErrorType::Success;
			m_oData = oData;
			m_qwPromiseUID = 0;
			m_poPromiseMgr = poMgr;
		}

		SPromise(PTR_OR_REF(CPromise<T>) t)
		{
			m_ePromiseError = EPromiseErrorType::Success;
			m_oData = T();
			m_qwPromiseUID = TO_PTR2(t)->GetOwner()->GetUID();
			m_poPromiseMgr = TO_PTR2(t)->GetPromiseMgr();
		}

	private:
		QINIT_ONLY EPromiseErrorType STRUCT_MEMBER_DEFINE(m_ePromiseError, EPromiseErrorType::Success);
		QINIT_ONLY T STRUCT_MEMBER_DEFINE(m_oData, T());
		QINIT_ONLY UINT64 STRUCT_MEMBER_DEFINE(m_qwPromiseUID, 0);
		QINIT_ONLY PTR(CPromiseMgr) STRUCT_MEMBER_DEFINE(m_poPromiseMgr, NULL_PTR);
	};
}

#endif
