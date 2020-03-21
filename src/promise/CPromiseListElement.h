#ifndef _H_C_PROMISE_LIST_ELEMENT_H_
#define _H_C_PROMISE_LIST_ELEMENT_H_

#include "../CTypeDefine.h"
#include "../CObjectCacheDefine.h"

namespace QWER
{
	QCLASS CPromiseContainer;
	QCLASS CPromiseList;

	QCLASS CPromiseListElement: public IObjRes
	{
	public:
		CPromiseListElement();
#ifndef _USING_CLI
		~CPromiseListElement();
#endif
		UINT64 GetUID() QCONST_FUNC
		{
			return m_qwUID;
		}
		void Init(PTR(CPromiseList) poOwner, PTR(CPromiseContainer) poPromise);
		virtual void QConstructor(UINT64 qwUID);
		virtual void QDestructor();
		inline PTR(CPromiseListElement) GetPrev(){ return m_poPrev; }
		inline PTR(CPromiseListElement) GetNext(){ return m_poNext; }
		PTR(CPromiseContainer) GetPromise();
		inline PTR(CPromiseList) GetOwner(){ return m_poOwner; }
		void Then(PTR(CPromiseContainer) poPromiseContainer);
		void Then(PTR(CPromiseList) poList);
		inline void ClearPrev(){ m_poPrev = NULL_PTR; }
		inline void ClearNext(){ m_poNext = NULL_PTR; }

	private:
		void _SetOwner(PTR(CPromiseList) p){ m_poOwner = p; }

	private:
		UINT64 m_qwUID = 0;
		PTR(CPromiseListElement) m_poPrev = NULL_PTR;
		PTR(CPromiseListElement) m_poNext = NULL_PTR;
		UINT64 m_qwPromise = 0;
		PTR(CPromiseList) m_poOwner = NULL_PTR;
	};
}

#endif