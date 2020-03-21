#ifndef _H_C_DEFAULT_LOG_H_
#define _H_C_DEFAULT_LOG_H_

#include "ILog.h"

namespace QWER
{
	QCLASS CStdLog : public ILog
	{
	public:
		virtual void Print(UINT8 btLogLevel, CONST_REF(CString) strLog);
	};

}

#endif