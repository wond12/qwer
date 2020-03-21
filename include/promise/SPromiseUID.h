#ifndef _H_S_PROMISE_UID_H_
#define _H_S_PROMISE_UID_H_

#include "../CTypeDefine.h"

namespace QWER
{
	QCLASS CPromiseMgr;

	QTEMPLATE<typename T>
	QDLL_EXPORT QSTRUCT SPromiseUID
	{
	public:
#ifndef _USING_CLI
		SPromiseUID()
		{
		}

		BOOLN operator==(const SPromiseUID<T>& orig) const
		{
			return m_qwUID == orig.m_qwUID && m_poPromiseMgr == orig.m_poPromiseMgr;
		}

		BOOLN operator!=(const SPromiseUID<T>& orig) const
		{
			return !operator==(orig);
		}
#endif
		BOOLN IsNull() QCONST_FUNC
		{
			return m_poPromiseMgr == NULL_PTR;
		}
	private:
		QINIT_ONLY UINT64 STRUCT_MEMBER_DEFINE(m_qwUID, 0);
		QINIT_ONLY PTR(CPromiseMgr) STRUCT_MEMBER_DEFINE(m_poPromiseMgr, NULL_PTR);

#ifdef _USING_CLI
	internal:
#else
	private:
		friend class CPromiseMgr;
#endif

		SPromiseUID(UINT64 qwUID, PTR(CPromiseMgr) poMgr)
		{
			m_qwUID = qwUID;
			m_poPromiseMgr = poMgr;
		}

		inline UINT64 _GetUID() QCONST_FUNC{ return m_qwUID; }
		inline CONST_PTR(CPromiseMgr) _GetOwner() QCONST_FUNC{ return m_poPromiseMgr; }
	};
}

#endif