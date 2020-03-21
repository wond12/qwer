#include "CLogChannel.h"
#include "CLogEntity.h"
#include "CLogServiceBackend.h"

namespace QWER
{
	CLogChannel::CLogChannel(PTR(CLogServiceBackend) poLogServiceResolver)
	{
		m_poLogServiceResolver = poLogServiceResolver;
	}
	void CLogChannel::QConstructor(UINT64 qwUID)
	{
		m_sUID = SLogChannelUID(qwUID);
	}
	void CLogChannel::QDestructor()
	{
#ifdef _USING_CLI
		while (m_oLogEntityList.Count != 0)
			m_poLogServiceResolver->ReleaseLogEntity(m_oLogEntityList.First.Value->GetUID());
#else
		while (m_oLogEntityList.size() != 0)
			m_poLogServiceResolver->ReleaseLogEntity((*m_oLogEntityList.begin())->GetUID());
#endif
	}
#ifdef _USING_CLI
	System::Collections::Generic::LinkedListNode<CLogEntity^>^ CLogChannel::Add(PTR(CLogEntity) poLogEntity)
	{
		return m_oLogEntityList.AddLast(poLogEntity);
	}
	void CLogChannel::Remove(System::Collections::Generic::LinkedListNode<CLogEntity^>^ poNode)
	{
		m_oLogEntityList.Remove(poNode);
	}
#else
	std::list<CLogEntity*>::iterator CLogChannel::Add(PTR(CLogEntity) poLogEntity)
	{
		return m_oLogEntityList.insert(m_oLogEntityList.end(), poLogEntity);
	}
	void CLogChannel::Remove(CONST_REF(std::list<CLogEntity*>::iterator) poNode)
	{
		m_oLogEntityList.erase(poNode);
	}
#endif

	void CLogChannel::Print(UINT8 btLogLevel, CONST_REF(CString) rstrLog)
	{
		if (btLogLevel < m_btMinLogLevel)
			return;
#ifdef _USING_CLI
		if (m_oLogEntityList.Count == 0)
			return;
		for (System::Collections::Generic::LinkedListNode<CLogEntity^>^ poNode = m_oLogEntityList.First; poNode != NULL_PTR; poNode = poNode->Next)
			poNode->Value->GetLog()->Print(btLogLevel, rstrLog);
#else
		for (auto it = m_oLogEntityList.begin(); it != m_oLogEntityList.end(); ++it)
			(*it)->GetLog()->Print(btLogLevel, rstrLog);
#endif
	}
}
