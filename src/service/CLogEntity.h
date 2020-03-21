#ifndef _H_C_LOG_ENTITY_H_
#define _H_C_LOG_ENTITY_H_

#include "../CObjectCacheDefine.h"
#include "SLogEntityUID.h"
#include "../SmartPtrDefine.h"
#ifndef _USING_CLI
#include <list>
#endif
#include "ILog.h"

namespace QWER
{
	QINTERFACE ILog;
	QCLASS CLogChannel;

	QCLASS CLogEntity : public IObjRes
	{
	public:
		virtual void QConstructor(UINT64 qwUID);
		virtual void QDestructor();

		inline CONST_REF(SLogEntityUID) GetUID() QCONST_FUNC { return m_sUID; }
		void SetLog(QMOVEABLE(ILog) poLog);
		inline VAL_OR_REF(CUniquePtr(ILog)) GetLog() { return m_poLog; }
		inline PTR(CLogChannel) GetLogChannel() { return m_poLogChannel; }
		inline void SetLogChannel(PTR(CLogChannel) poLogChannel) { m_poLogChannel = poLogChannel; }
#ifdef _USING_CLI
		void SetNodeInLogChannel(System::Collections::Generic::LinkedListNode<CLogEntity^>^ poNode) { return m_poNodeInLogChannel = poNode; };
#else
		void SetNodeInLogChannel(CONST_REF(std::list<CLogEntity*>::iterator) poNode) { m_poNodeInLogChannel = poNode; };
#endif

	private:
		PTR(CLogChannel) m_poLogChannel = NULL_PTR;
		SLogEntityUID m_sUID;
		CUniquePtr(ILog) m_poLog;
#ifdef _USING_CLI
		System::Collections::Generic::LinkedListNode<CLogEntity^>^ m_poNodeInLogChannel;
#else
		std::list<CLogEntity*>::iterator m_poNodeInLogChannel;
#endif

	};

}

#endif