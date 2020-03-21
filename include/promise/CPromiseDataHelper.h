#ifndef _H_C_PROMISE_DATA_HELPER_H_
#define _H_C_PROMISE_DATA_HELPER_H_

#include "../CTypeDefine.h"
#include "SPromiseData.h"

namespace QWER
{
	QDLL_EXPORT QCLASS CPromiseDataHelper QABSTRACT QFINAL
	{
	public:
		QTEMPLATE<typename T>
		static SPromiseData<T> CreatePromiseData(CONST_REF(T) roData)
		{
			return SPromiseData<T>(roData);
		}
	};
}

#endif