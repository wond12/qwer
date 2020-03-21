#ifndef _H_S_PROMISE_FUNC_ARG_H_
#define _H_S_PROMISE_FUNC_ARG_H_

#include "../CTypeDefine.h"
#include "SPromiseData.h"

namespace QWER
{
	QCLASS CPromiseMgr;

	QTEMPLATE<typename T>
	QDLL_EXPORT QSTRUCT SPromiseFuncArg
	{
	public:
#ifndef _USING_CLI
		SPromiseFuncArg()
		{
		}
#endif

		SPromiseFuncArg(PTR(CPromiseMgr) poPromiseMgr, EPromiseErrorType eErrorType)
		{
			m_ePromiseError = eErrorType;
			m_poPromiseMgr = poPromiseMgr;
			m_oData = T();
		}

		SPromiseFuncArg(PTR(CPromiseMgr) poPromiseMgr, CONST_REF(T) oData)
		{
			m_ePromiseError = EPromiseErrorType::Success;
			m_poPromiseMgr = poPromiseMgr;
			m_oData = oData;
		}

		SPromiseData<T> ToPromiseData() QCONST_FUNC
		{
			if (m_ePromiseError == EPromiseErrorType::Success)
				return SPromiseData<T>(m_oData);
			return SPromiseData<T>(m_ePromiseError);
		}

	public:
		QINIT_ONLY EPromiseErrorType STRUCT_MEMBER_DEFINE(m_ePromiseError, EPromiseErrorType::Success);
		QINIT_ONLY PTR(CPromiseMgr) STRUCT_MEMBER_DEFINE(m_poPromiseMgr, NULL_PTR);
		QINIT_ONLY T STRUCT_MEMBER_DEFINE(m_oData, T());
	};
}

#endif