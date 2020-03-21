#ifndef _H_I_PROMISE_IPML_H_
#define _H_I_PROMISE_IPML_H_

#include "../CTypeDefine.h"
#include "EPromiseStatus.h"
#include "SPromiseData.h"

namespace QWER
{
	QCLASS IPromise;
	QTEMPLATE<typename T> QCLASS IPromiseInterface;

	QTEMPLATE<typename T>
	QDLL_EXPORT QCLASS IPromiseImpl QABSTRACT
	{
	public:
#ifndef _USING_CLI
		virtual ~IPromiseImpl(){}
#endif
		CONST_REF(SPromiseData<T>) GetRet() QCONST_FUNC
		{
			return m_oRet;
		}
		virtual EPromiseStatus GetStatus() QCONST_FUNC = 0;
		virtual PTR(IPromiseInterface<T>) Exec() = 0;
		virtual void RecvParam(PTR(IPromise) poPromise) = 0;
		virtual void Resolve() = 0;
		virtual void Reject(EPromiseErrorType eErrorType) = 0;

	protected:
		SPromiseData<T> m_oRet;
	};
}

#endif