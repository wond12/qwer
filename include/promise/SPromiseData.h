#ifndef _H_S_PROMISE_DATA_H_
#define _H_S_PROMISE_DATA_H_

#include "../CTypeDefine.h"

namespace QWER
{
	QCLASS CPromiseMgr;
	QCLASS IPromise;
	QTEMPLATE<typename T> QCLASS IPromiseInterface;
	QTEMPLATE<typename T> QSTRUCT SPromise;

	QDLL_EXPORT QENUM EPromiseErrorType
	{
		Success = 0,
		TimeoutError = 1,
		CancellationError = 2,
	};

	QTEMPLATE<typename T>
	QDLL_EXPORT QSTRUCT SPromiseData
	{
	public:
#ifndef _USING_CLI
		SPromiseData()
		{
		}
#endif

		explicit SPromiseData(EPromiseErrorType ePromiseError)
		{
			m_ePromiseError = ePromiseError;
			m_oData = T();
			m_poPromise = NULL_PTR;
		}

		explicit SPromiseData(CONST_REF(T) oData)
		{
			m_ePromiseError = EPromiseErrorType::Success;
			m_oData = oData;
			m_poPromise = NULL_PTR;
		}

#ifdef _USING_CLI
	internal:
#else
	private:
		friend QSTRUCT SPromise<T>;
#endif
		SPromiseData(PTR(IPromiseInterface<T>) t, BOOLN)
		{
			m_ePromiseError = EPromiseErrorType::Success;
			m_oData = T();
			m_poPromise = t;
		}

	public:
		QINIT_ONLY EPromiseErrorType STRUCT_MEMBER_DEFINE(m_ePromiseError, EPromiseErrorType::Success);
		QINIT_ONLY T STRUCT_MEMBER_DEFINE(m_oData, T());
		QINIT_ONLY PTR(IPromiseInterface<T>) STRUCT_MEMBER_DEFINE(m_poPromise, NULL_PTR);
	};
}

#endif