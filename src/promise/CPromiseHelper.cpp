#include "CPromiseHelper.h"
#include "CPromiseMgr.h"
#include "IPromise.h"
#include "CPromiseContainer.h"
#include "CPromiseList.h"
#include "CPromiseListElement.h"

namespace QWER
{
	void CPromiseHelper::InitPromise(PTR(CPromiseMgr) poPromiseMgr, PTR(IPromise) poPromise)
	{
		PTR(CPromiseContainer) poContainer = poPromiseMgr->_CreatePromiseContainer(poPromise);
		poPromise->_SetOwner(poContainer);
	}

	void CPromiseHelper::AddToExecList(PTR(IPromise) poPromise)
	{
		poPromise->GetOwner()->GetPromiseListElement()->GetOwner()->AddToExecList();
	}

	PTR(IPromise) CPromiseHelper::Delay(PTR(CPromiseMgr) poPromiseMgr, UINT64 qwMilliSeconds)
	{
		return poPromiseMgr->_Delay(qwMilliSeconds);
	}

	void CPromiseHelper::AddToPromiseList(PTR(IPromise) poPromise)
	{
		PTR(CPromiseContainer) poPromiseContainer = poPromise->GetOwner();
		if (poPromiseContainer->GetPromiseListElement() != NULL_PTR)
			return;
		PTR(CPromiseMgr) poPromiseMgr = poPromiseContainer->GetOwner();
		poPromiseMgr->_CreatePromiseList(poPromiseContainer);
	}

	void CPromiseHelper::Then(PTR(IPromise) poPrev, PTR(IPromise) poNext)
	{
		poPrev->GetOwner()->GetPromiseListElement()->Then(poNext->GetOwner());
	}

	PTR(IPromise) CPromiseHelper::GetHeader(PTR(IPromise) poPromise)
	{
		PTR(CPromiseList) poPromiseList = poPromise->GetOwner()->GetPromiseListElement()->GetOwner();
		PTR(CPromiseListElement) poHead = poPromiseList->GetHead();
		return poHead->GetPromise()->GetPromise();
	}

	PTR(IPromise) CPromiseHelper::GetPromise(PTR(CPromiseMgr) poPromiseMgr, UINT64 qwPromiseUID)
	{
		return (poPromiseMgr == NULL_PTR || qwPromiseUID == 0) ? NULL_PTR : poPromiseMgr->__GetPromise(qwPromiseUID);
	}
}
