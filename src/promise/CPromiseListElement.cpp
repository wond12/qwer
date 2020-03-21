#include "CPromiseListElement.h"
#include "CPromiseContainer.h"
#include "CPromiseList.h"
#include "CPromiseMgr.h"

namespace QWER
{
	CPromiseListElement::CPromiseListElement()
	{
		
	}

	void CPromiseListElement::Init(PTR(CPromiseList) poOwner, PTR(CPromiseContainer) poPromise)
	{
		m_qwPromise = poPromise->GetUID();
		m_poOwner = poOwner;
		poPromise->_SetPromiseListElement(this);
	}

	void CPromiseListElement::QConstructor(UINT64 qwUID)
	{
		m_qwUID = qwUID;
	}

	void CPromiseListElement::QDestructor()
	{
		if (m_qwPromise != 0)
		{
			m_poOwner->GetOwner()->_ReleasePromiseContainer(m_qwPromise);
			m_qwPromise = 0;
		}
		m_poOwner = NULL_PTR;
		m_poPrev = NULL_PTR;
		m_poNext = NULL_PTR;
		m_qwUID = 0;
	}

#ifndef _USING_CLI
	CPromiseListElement::~CPromiseListElement()
	{
		if(m_qwPromise != 0)
			m_poOwner->GetOwner()->_ReleasePromiseContainer(m_qwPromise);
	}
#endif

	void CPromiseListElement::Then(PTR(CPromiseContainer) poPromiseContainer)
	{
		QWERASSERT(poPromiseContainer->GetPromiseListElement() == NULL_PTR && GetOwner() != NULL_PTR && m_poNext == NULL_PTR, );
		PTR(CPromiseListElement) p = m_poOwner->GetOwner()->_CreatePromiseListElement(m_poOwner, poPromiseContainer);
		m_poNext = p;
		p->m_poPrev = this;
	}

	void CPromiseListElement::Then(PTR(CPromiseList) poList)
	{
		QWERASSERT(poList->GetOwner() == m_poOwner->GetOwner(), );
		if (poList->IsEmpty() == false)
		{
			PTR(CPromiseListElement) poHead = poList->Swap(NULL_PTR);
			PTR(CPromiseListElement) poTail = NULL_PTR;
			for (PTR(CPromiseListElement) p = poHead; p != NULL_PTR; p = p->GetNext())
			{
				p->_SetOwner(m_poOwner);
				poTail = p;
			}
			PTR(CPromiseListElement) poOrigNext = m_poNext;
			m_poNext = poHead;
			poHead->m_poPrev = this;
			poTail->m_poNext = poOrigNext;
			if (poOrigNext != NULL_PTR)
			{
				poOrigNext->m_poPrev = poTail;
			}
		}
		poList->GetOwner()->_ReleasePromiseList(poList->GetUID());
	}

	PTR(CPromiseContainer) CPromiseListElement::GetPromise()
	{
		return m_qwPromise == 0 ? NULL_PTR : m_poOwner->GetOwner()->_GetPromiseContainer(m_qwPromise);
	}
}
