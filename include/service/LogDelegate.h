#ifndef _H_LOG_DELEGATE_H_
#define _H_LOG_DELEGATE_H_

#include "../CTypeDefine.h"
#ifdef _USING_CLI
#else
#include <functional>
#endif
#include "../SmartPtrDefine.h"

namespace QWER
{
	QCLASS CServiceBackendHelper;
	QINTERFACE ILog;

#ifdef _USING_CLI
	QDLL_EXPORT delegate CUniquePtr(ILog) DNewLogEntity(PTR(CServiceBackendHelper) poResolverHelper);
#else
	using DNewLogEntity = std::function<CUniquePtr(ILog)(REF(CServiceBackendHelper) roResolverHelper)>;
#endif
}

#endif