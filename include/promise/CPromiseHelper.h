#ifndef _H_C_PROMISE_HELPER_H_
#define _H_C_PROMISE_HELPER_H_

#include "../CTypeDefine.h"

namespace QWER
{
	QCLASS IPromise;
	QCLASS CPromiseMgr;

	QCLASS CPromiseHelper
	{
	public:
		static void InitPromise(PTR(CPromiseMgr) poPromiseMgr, PTR(IPromise) poPromise);
		static void AddToExecList(PTR(IPromise) poPromise);
		static PTR(IPromise) Delay(PTR(CPromiseMgr) poPromiseMgr, UINT64 qwMilliSeconds);
		static void AddToPromiseList(PTR(IPromise) poPromise);
		static void Then(PTR(IPromise) poPrev, PTR(IPromise) poNext);
		static PTR(IPromise) GetHeader(PTR(IPromise) poPromise);
		static PTR(IPromise) GetPromise(PTR(CPromiseMgr) poPromiseMgr, UINT64 qwPromiseUID);
	};
}

#endif