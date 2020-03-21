#ifndef _H_PROMISE_DELEGATE_H_
#define _H_PROMISE_DELEGATE_H_

#include "../CTypeDefine.h"
#ifdef _USING_CLI
#else
#include <functional>
#endif
#include "SPromiseFuncArg.h"

namespace QWER
{

#ifdef _USING_CLI

	QTEMPLATE<typename T, typename U>
	QDLL_EXPORT delegate SPromiseData<T> DPromiseResolver(CONST_REF(SPromiseFuncArg<U>) p);

#else

	QTEMPLATE<typename T, typename U>
	using DPromiseResolver = std::function<SPromiseData<T>(CONST_REF(SPromiseFuncArg<U>))>;
#endif
}

#endif