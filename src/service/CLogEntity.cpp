#include "CLogEntity.h"
#include "CLogChannel.h"

namespace QWER
{
	void CLogEntity::QConstructor(UINT64 qwUID)
	{
		m_sUID = SLogEntityUID(qwUID);
	}

	void CLogEntity::QDestructor()
	{
		m_poLog = NULL_PTR;
		if (m_poLogChannel != NULL_PTR) {
			m_poLogChannel->Remove(m_poNodeInLogChannel);
			m_poLogChannel = NULL_PTR;
		}
	}

	void CLogEntity::SetLog(QMOVEABLE(ILog) poLog)
	{
		QMOVETO(poLog, m_poLog);
	}
}
