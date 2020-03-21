#include "CPromiseList.h"
#include "CPromiseContainer.h"
#include "IPromise.h"
#include "CPromiseMgr.h"
#include "CPromiseListElement.h"

namespace QWER
{
#ifndef _USING_CLI
	CPromiseList::~CPromiseList()
	{
		PTR(CPromiseMgr) poPromiseMgr = GetOwner();
		for (PTR(CPromiseListElement) p = m_poHead; p != NULL_PTR;)
		{
			PTR(CPromiseListElement) p1 = p;
			p = p->GetNext();
			poPromiseMgr->_ReleasePromiseListElement(p1->GetUID());
		}
	}
#endif

	void CPromiseList::QConstructor(UINT64 qwUID)
	{
		m_qwUID = qwUID;
	}

	void CPromiseList::QDestructor()
	{
		PTR(CPromiseMgr) poPromiseMgr = GetOwner();
		for (PTR(CPromiseListElement) p = m_poHead; p != NULL_PTR;)
		{
			PTR(CPromiseListElement) p1 = p;
			p = p->GetNext();
			poPromiseMgr->_ReleasePromiseListElement(p1->GetUID());
		}
		m_poHead = NULL_PTR;
		m_bInExecList = false;
		m_qwUID = 0;
	}

	void CPromiseList::AddToExecList()
	{
		m_poOwner->_AddToExecList(this);
	}

	UINT32 CPromiseList::Exec()
	{
		UINT32 dwCount = 0;
		PTR(CPromiseMgr) poPromiseMgr = GetOwner();
		while (m_poHead != NULL_PTR)
		{
			BOOLN bNeedRecvParam = true;
			PTR(CPromiseContainer) poPromiseContainer = m_poHead->GetPromise();
			if (NULL_PTR != poPromiseContainer)
			{
				PTR(IPromise) poPromise = poPromiseContainer->GetPromise();
				if (poPromise->GetStatus() == EPromiseStatus::Pending)
					break;
				PTR(IPromise) poNext = poPromise->__Exec();
				if (poNext != NULL_PTR)
				{
					m_poHead->Then(poNext->GetOwner()->GetPromiseListElement()->GetOwner());
					bNeedRecvParam = false;
				}
				++dwCount;
			}
			if (bNeedRecvParam)
			{
				PTR(CPromiseListElement) p = m_poHead->GetNext();
				if (p != NULL_PTR)
				{
					p->GetPromise()->GetPromise()->__RecvParam(poPromiseContainer == NULL_PTR ? NULL_PTR : poPromiseContainer->GetPromise());
				}
			}

			PTR(CPromiseListElement) poOrigHead = _PopHead();
			poPromiseMgr->_ReleasePromiseListElement(poOrigHead->GetUID());
		}
		return dwCount;
	}

	PTR(CPromiseListElement) CPromiseList::_PopHead()
	{
		if (m_poHead == NULL_PTR)
			return NULL_PTR;
		PTR(CPromiseListElement) poRet = m_poHead;
		m_poHead = poRet->GetNext();
		poRet->ClearNext();
		if(NULL_PTR != m_poHead) m_poHead->ClearPrev();
		return poRet;
	}

	void CPromiseList::Init(PTR(CPromiseContainer) poPromise)
	{
		QWERASSERT(m_poHead == NULL_PTR, );
		m_poHead = GetOwner()->_CreatePromiseListElement(this, poPromise);
	}

	PTR(CPromiseListElement) CPromiseList::Swap(PTR(CPromiseListElement) poHead)
	{
		PTR(CPromiseListElement) p = m_poHead;
		m_poHead = poHead;
		return p;
	}
}
