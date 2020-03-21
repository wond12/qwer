#include "CDefaultLog.h"
#ifdef _USING_CLI
#else
#include <iostream>
#ifdef WIN32
#include <windows.h>
#endif
#endif

namespace QWER
{
	void CStdLog::Print(UINT8 btLogLevel, CONST_REF(CString) strLog)
	{
#ifdef _USING_CLI
		System::Console::WriteLine(strLog);
#else
		std::cout << strLog << std::endl;
#endif
	}
}
