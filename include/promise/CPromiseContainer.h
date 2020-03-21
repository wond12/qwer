#ifndef _H_C_PROMISE_CONTAINER_H_
#define _H_C_PROMISE_CONTAINER_H_

#include "../CTypeDefine.h"
#include "../CObjectCacheDefine.h"

namespace QWER
{
	QCLASS CPromiseList;
	QCLASS IPromise;
	QCLASS CPromiseListElement;
	QCLASS CPromiseMgr;

	QCLASS CPromiseContainer : public IObjRes
	{
	public:
		CPromiseContainer(PTR(CPromiseMgr) poOwner)
		{
			m_poOwner = poOwner;
		}

		virtual UINT16 GetObjType() QCONST_FUNC
		{
			return 0;
		}
		UINT64 GetUID() QCONST_FUNC
		{
			return m_qwUID;
		}
		virtual void QConstructor(UINT64 qwUID);
		virtual void QDestructor();
		inline PTR(IPromise) GetPromise(){ return m_poPromise; }
		inline void SetPromise(PTR(IPromise) poPromise){ m_poPromise = poPromise; }
		inline PTR(CPromiseListElement) GetPromiseListElement(){ return m_poListElement; }
		inline PTR(CPromiseMgr) GetOwner(){ return m_poOwner; }

	private:
		UINT64 m_qwUID = 0;
		PTR(IPromise) m_poPromise = NULL_PTR;
		PTR(CPromiseListElement) m_poListElement = NULL_PTR;
		PTR(CPromiseMgr) m_poOwner = NULL_PTR;

#ifdef _USING_CLI
	internal:
#else
	private:
		friend QCLASS CPromiseListElement;
#endif
		inline void _SetPromiseListElement(PTR(CPromiseListElement) poListElement){ m_poListElement = poListElement; }
	};
}

#endif