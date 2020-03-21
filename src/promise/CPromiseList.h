#ifndef _H_C_PROMISE_LIST_H_
#define _H_C_PROMISE_LIST_H_

#include "../CTypeDefine.h"
#include "../CObjectCacheDefine.h"
#include "../PairDefine.h"

namespace QWER
{
	QCLASS CPromiseMgr;
	QCLASS CPromiseContainer;
	QCLASS CPromiseListElement;

	QCLASS CPromiseList : public IObjRes
	{
	public:
		CPromiseList(PTR(CPromiseMgr) poOwner)
		{
			m_poOwner = poOwner;
		}
#ifndef _USING_CLI
		~CPromiseList();
#endif
		UINT64 GetUID() QCONST_FUNC
		{
			return m_qwUID;
		}
		virtual void QConstructor(UINT64 qwUID);
		virtual void QDestructor();
		void AddToExecList();
		UINT32 Exec();
		void Init(PTR(CPromiseContainer) poPromise);
		PTR(CPromiseListElement) Swap(PTR(CPromiseListElement) poHead);
		inline BOOLN IsEmpty(){ return m_poHead == NULL_PTR; }
		inline PTR(CPromiseMgr) GetOwner(){ return m_poOwner; }
		//inline PTR(CPromiseListElement) GetHead(){ return m_poHead; }
		//inline PTR(CPromiseListElement) GetTail(){ return m_poTail; }
		inline void SetInExecList(BOOLN bInExecList){ m_bInExecList = bInExecList; }
		BOOLN IsInExecList() QCONST_FUNC{ return m_bInExecList; }
		inline PTR(CPromiseListElement) GetHead(){ return m_poHead; }

	private:
		PTR(CPromiseListElement) _PopHead();

	private:
		UINT64 m_qwUID = 0;
		PTR(CPromiseListElement) m_poHead = NULL_PTR;
		//PTR(CPromiseListElement) m_poTail = NULL_PTR;
		PTR(CPromiseMgr) m_poOwner = NULL_PTR;
		BOOLN m_bInExecList = false;
	};
}

#endif