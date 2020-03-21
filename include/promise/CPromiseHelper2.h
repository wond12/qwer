#ifndef _H_C_PROMISE_ALL_HELPER2_H_
#define _H_C_PROMISE_ALL_HELPER2_H_

#include "../CTypeDefine.h"
#include "CPromise.h"
#include "SPromiseUID.h"

namespace QWER
{
	QCLASS CPromiseHelper2
	{
	public:
		QTEMPLATE<typename T>
		static void ResolvePromise(PTR(CPromise<T>) poPromise, CONST_REF(T) oData)
		{
			if (NULL_PTR == poPromise)
				return;
			PTR(QPACK(CPromiseImpl<T, T>)) poImpl = (PTR(QPACK(CPromiseImpl<T, T>)))poPromise->_GetImpl();
			poImpl->_Resolve(oData);
			CPromiseHelper::AddToExecList(poPromise);
		}

		QTEMPLATE<typename T>
		static void RejectPromise(PTR(CPromise<T>) poPromise, EPromiseErrorType eErrorType)
		{
			if (NULL_PTR == poPromise)
				return;
			PTR(QPACK(CPromiseImpl<T, T>)) poImpl = (PTR(QPACK(CPromiseImpl<T, T>)))poPromise->_GetImpl();
			poImpl->_Reject(eErrorType);
			CPromiseHelper::AddToExecList(poPromise);
		}
	};
}

#endif