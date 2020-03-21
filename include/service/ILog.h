#ifndef _H_LOG_H_
#define _H_LOG_H_

#include "../CTypeDefine.h"
#include "../StringDefine.h"

namespace QWER
{
	QDLL_EXPORT QINTERFACE ILog
	{
	public:
		virtual void Print(UINT8 btLogLevel, CONST_REF(CString) strLog) = 0;
	};

}

#endif