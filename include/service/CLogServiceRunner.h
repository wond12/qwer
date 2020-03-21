#ifndef _H_C_LOG_SERVICE_RUNNER_H_
#define _H_C_LOG_SERVICE_RUNNER_H_

#include "IRunner.h"

namespace QWER
{
	QCLASS CLogService;

	QDLL_EXPORT QCLASS CLogServiceRunner: public IRunner
	{
	public:
		CLogServiceRunner();
#ifndef _USING_CLI
		~CLogServiceRunner();
#endif
		inline PTR(CLogService) GetLogService() { return m_poLogService; }

	private:
		PTR(CLogService) m_poLogService = NULL_PTR;
	};
}

#endif