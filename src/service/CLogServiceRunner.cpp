#include "CLogServiceRunner.h"
#include "CLogService.h"

namespace QWER
{
	CLogServiceRunner::CLogServiceRunner()
	{
		m_poLogService = QNEW CLogService(TO_REF2(this));
	}

#ifndef _USING_CLI
	CLogServiceRunner::~CLogServiceRunner()
	{
		SAFE_DELETE(m_poLogService);
	}
#endif
}