#include "CPromiseContainer.h"
#include "IPromise.h"

namespace QWER
{
	void CPromiseContainer::QConstructor(UINT64 qwUID)
	{
		m_qwUID = qwUID;
	}

	void CPromiseContainer::QDestructor()
	{
		SAFE_DELETE(m_poPromise);
		m_poListElement = NULL_PTR;
		m_qwUID = 0;
	}
}
