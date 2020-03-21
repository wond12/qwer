#ifndef _H_E_PROMISE_STATUS_H_
#define _H_E_PROMISE_STATUS_H_

#include "../CTypeDefine.h"

namespace QWER
{
	QDLL_EXPORT QENUM EPromiseStatus
	{
		Pending,
		Fulfilled,
		Rejected
	};
}

#endif