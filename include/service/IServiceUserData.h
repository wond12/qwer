#ifndef _H_I_SERVICE_ADAPTER_USER_DATA_H_
#define _H_I_SERVICE_ADAPTER_USER_DATA_H_

#include "../CTypeDefine.h"

namespace QWER
{
	QDLL_EXPORT QINTERFACE IServiceUserData
	{
	public:
#ifdef _USING_CLI
#else
		virtual ~IServiceUserData(){}
#endif
	};
}

#endif