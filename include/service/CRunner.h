#ifndef _H_C_RUNNER_H_
#define _H_C_RUNNER_H_

#include "IRunner.h"

namespace QWER
{
	QCLASS CLogService;
	QCLASS CLogServiceRunner;

	QDLL_EXPORT QCLASS CRunner: public IRunner
	{
	public:
		CRunner(PTR_OR_REF(CLogServiceRunner) roLogServiceRunner);
		inline PTR(CLogService) GetLogService() { return m_poLogService; }

	private:
		PTR(CLogService) m_poLogService = NULL_PTR;
	};
}

#endif