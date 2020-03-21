#include "CPromiseMgr.h"
#include "CPromiseContainer.h"
#ifndef _USING_CLI
#include <exception>
#endif
#include "CPromiseList.h"
#include "CPromiseDataHelper.h"
#include "CPromiseListElement.h"

namespace QWER
{
	CPromiseMgr::CPromiseMgr(PTR_OR_REF(CTimerMgr) poTimerMgr)
	{
		m_poTimerMgr = TO_PTR2(poTimerMgr);
		m_poPromiseMgr = QNEW CObjectResMgr<PTR_OR_VAL(CPromiseContainer)>(
			NEW_DELEGATE(
				QPACK(DNewObj<CTimer>),
				this, CPromiseMgr::_NewPromise,
				DELEGATE_PLACEHOLDERS(, std::placeholders::_1)
			)
		);
		m_poPromiseListMgr = QNEW CObjectResMgr<PTR_OR_VAL(CPromiseList)>(
			NEW_DELEGATE(
				QPACK(DNewObj<CTimer>),
				this, CPromiseMgr::_NewPromiseList,
				DELEGATE_PLACEHOLDERS(, std::placeholders::_1)
			)
		);
		m_poPromiseListElementMgr = QNEW CObjectResMgr<PTR_OR_VAL(CPromiseListElement)>(
			NEW_DELEGATE(
				QPACK(DNewObj<CTimer>),
				this, CPromiseMgr::_NewPromiseListElement,
				DELEGATE_PLACEHOLDERS(, std::placeholders::_1)
			)
		);
	}
#ifndef _USING_CLI
	CPromiseMgr::~CPromiseMgr()
	{
		if(m_poPromiseMgr != NULL_PTR)
			m_poPromiseMgr->ReleaseAll();
		if (m_poPromiseListElementMgr != NULL_PTR)
			m_poPromiseListElementMgr->ReleaseAll();
		if(m_poPromiseListMgr != NULL_PTR)
			m_poPromiseListMgr->ReleaseAll();
		SAFE_DELETE(m_poPromiseMgr);
		SAFE_DELETE(m_poPromiseListMgr);
		SAFE_DELETE(m_poPromiseListElementMgr);
	}
#endif

	UINT32 CPromiseMgr::Poll()
	{
		UINT32 dwRet = 0;
		while((m_dwMaxCountPerPoll == 0 || dwRet < m_dwMaxCountPerPoll) && !m_oToExecPromiseList.IsEmpty())
		{
			PTR(CPromiseList) poNode = m_poPromiseListMgr->GetObj(m_oToExecPromiseList.At(0));
			m_oToExecPromiseList.Pop();
			if (poNode == NULL_PTR)
				continue;
			dwRet += poNode->Exec();
			poNode->SetInExecList(false);
			if (poNode->IsEmpty())
				_ReleasePromiseList(poNode->GetUID());
		}
		return dwRet;
	}

	CUniquePtr(CPromiseContainer) CPromiseMgr::_NewPromise(UINT16 wObjType)
	{
		CUniquePtr(CPromiseContainer) poObj(QNEW CPromiseContainer(this));
		return QMOVE(poObj);
	}

	CUniquePtr(CPromiseList) CPromiseMgr::_NewPromiseList(UINT16 wObjType)
	{
		CUniquePtr(CPromiseList) poObj(QNEW CPromiseList(this));
		return QMOVE(poObj);
	}

	CUniquePtr(CPromiseListElement) CPromiseMgr::_NewPromiseListElement(UINT16 wObjType)
	{
		CUniquePtr(CPromiseListElement) poObj(QNEW CPromiseListElement());
		return QMOVE(poObj);
	}

	void CPromiseMgr::_ReleasePromiseContainer(UINT64 qwPromiseUID)
	{
		m_poPromiseMgr->ReleaseObj(qwPromiseUID);
	}

	PTR(CPromiseContainer) CPromiseMgr::_GetPromiseContainer(UINT64 qwPromiseUID)
	{
		return m_poPromiseMgr->GetObj(qwPromiseUID);
	}

	void CPromiseMgr::_AddToExecList(PTR(CPromiseList) poPromiseList)
	{
		if (poPromiseList->IsInExecList())
			return;
		m_oToExecPromiseList.Push(poPromiseList->GetUID());
		poPromiseList->SetInExecList(true);
	}

	SPromise<BOOLN> CPromiseMgr::Delay(UINT64 qwMilliSeconds)
	{
		return SPromise<BOOLN>(TO_REF2(_Delay(qwMilliSeconds)));
	}

	PTR(CPromise<BOOLN>) CPromiseMgr::_Delay(UINT64 qwMilliSeconds)
	{
		SPromiseUID<BOOLN> sPromiseUID = CreatePromise<BOOLN>();
		PTR(CPromise<BOOLN>) poPromise = _GetPromise<BOOLN>(sPromiseUID._GetUID());
		poPromise->__SetDelay(qwMilliSeconds);
		return poPromise;
	}

	PTR(CPromiseContainer) CPromiseMgr::_CreatePromiseContainer(PTR(IPromise) poPromise)
	{
		PTR(CPromiseContainer) poContainer = m_poPromiseMgr->CreateObj(0);
		poContainer->SetPromise(poPromise);
		return poContainer;
	}

	PTR(CPromiseList) CPromiseMgr::_CreatePromiseList(PTR(CPromiseContainer) poPromiseContainer)
	{
		PTR(CPromiseList) poPromiseList = m_poPromiseListMgr->CreateObj(0);
		poPromiseList->Init(poPromiseContainer);
		return poPromiseList;
	}

	void CPromiseMgr::_ReleasePromiseList(UINT64 qwPromiseListUID)
	{
		m_poPromiseListMgr->ReleaseObj(qwPromiseListUID);
	}

	PTR(CPromiseListElement) CPromiseMgr::_CreatePromiseListElement(PTR(CPromiseList) poOwner, PTR(CPromiseContainer) poPromise)
	{
		PTR(CPromiseListElement) poRet = m_poPromiseListElementMgr->CreateObj(0);
		poRet->Init(poOwner, poPromise);
		return poRet;
	}

	void CPromiseMgr::_ReleasePromiseListElement(UINT64 qwPromiseListElementUID)
	{
		m_poPromiseListElementMgr->ReleaseObj(qwPromiseListElementUID);
	}

	PTR(IPromise) CPromiseMgr::__GetPromise(UINT64 qwPromiseUID)
	{
		PTR(CPromiseContainer) poContainer = m_poPromiseMgr->GetObj(qwPromiseUID);
		return poContainer == NULL_PTR ? NULL_PTR : poContainer->GetPromise();
	}
}
