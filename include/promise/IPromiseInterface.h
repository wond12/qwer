#ifndef _H_I_PROMISE_INTERFACE_H_
#define _H_I_PROMISE_INTERFACE_H_

#include "../CTypeDefine.h"
#include "SPromiseData.h"
#include "IPromise.h"

namespace QWER
{
	QTEMPLATE<typename T>
	QDLL_EXPORT QCLASS IPromiseInterface QABSTRACT: public IPromise
	{
	public:
#ifndef _USING_CLI
		virtual ~IPromiseInterface(){}
#endif
		virtual CONST_REF(SPromiseData<T>) GetRet() QCONST_FUNC = 0;
	};
}

#endif