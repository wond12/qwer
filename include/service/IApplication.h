#ifndef _H_I_APPLICATION_H_
#define _H_I_APPLICATION_H_

#include "../CTypeDefine.h"

namespace QWER
{
	QCLASS IRunner;
	QCLASS IServiceBackend;

	QDLL_EXPORT QCLASS IApplication QABSTRACT
	{
	public:
		IApplication();
#ifndef _USING_CLI
		virtual ~IApplication(){}
#endif

	protected:
		virtual PTR(IServiceBackend) _GetResolver() = 0;

	};
}

#endif