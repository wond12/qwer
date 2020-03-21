#include "CRunner.h"
#include "CLogServiceRunner.h"

namespace QWER
{
	CRunner::CRunner(PTR_OR_REF(CLogServiceRunner) roLogServiceRunner)
	{
		m_poLogService = TO_PTR2(roLogServiceRunner)->GetLogService();
	}
}