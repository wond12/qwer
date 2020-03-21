#ifndef _H_C_LOG_CHANNEL_H_
#define _H_C_LOG_CHANNEL_H_

#include "../CTypeDefine.h"
#include "../CObjectCacheDefine.h"
#ifndef _USING_CLI
#include <list>
#endif
#include "SLogChannelUID.h"
#include "../StringDefine.h"

namespace QWER
{
	QCLASS CLogEntity;
	QCLASS CLogServiceBackend;

	QCLASS CLogChannel : public IObjRes
	{
	public:
		CLogChannel(PTR(CLogServiceBackend) poLogServiceResolver);
		virtual void QConstructor(UINT64 qwUID);
		virtual void QDestructor();

		inline CONST_REF(SLogChannelUID) GetUID() QCONST_FUNC { return m_sUID; }
#ifdef _USING_CLI
		System::Collections::Generic::LinkedListNode<CLogEntity^>^ Add(PTR(CLogEntity) poLogEntity);
		void Remove(System::Collections::Generic::LinkedListNode<CLogEntity^>^ poNode);
#else
		std::list<CLogEntity*>::iterator Add(PTR(CLogEntity) poLogEntity);
		void Remove(CONST_REF(std::list<CLogEntity*>::iterator) poNode);
#endif
		void Print(UINT8, CONST_REF(CString) rstrLog);
		inline void SetMinLogLevel(UINT8 btMinLogLevel) { m_btMinLogLevel = btMinLogLevel; }

	private:
		PTR(CLogServiceBackend) m_poLogServiceResolver;
		SLogChannelUID m_sUID;
#ifdef _USING_CLI
		System::Collections::Generic::LinkedList<CLogEntity^> m_oLogEntityList;
#else
		std::list<CLogEntity*> m_oLogEntityList;
#endif
		UINT8 m_btMinLogLevel = 0;
	};
}

#endif