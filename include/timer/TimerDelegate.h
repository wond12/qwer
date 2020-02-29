#ifndef _H_TIMER_DELEGATE_H_
#define	_H_TIMER_DELEGATE_H_

#include "../CTypeDefine.h"
#ifdef _USING_CLI
#else
#include <functional>
#endif
#include "STimerUID.h"

namespace QWER
{
#ifdef _USING_CLI
	public delegate void DOnTimer(CONST_REF(STimerUID) rsTimerUID, UINT32 dwCurrent, UINT32 dwCount);
#else
	using DOnTimer = std::function<void(CONST_REF(STimerUID) rsTimerUID, UINT32 dwCurrent, UINT32 dwCount)>;
#endif
}

#endif	/* ITIMER_H */

