#include "CLogServiceBackend.h"

namespace QWER
{
	CLogServiceBackend::CLogServiceBackend(PTR_OR_REF(IRunner) roRunner):
		IServiceBackend(roRunner)
	{
		m_poLogEntityMgr = QNEW CObjectResMgr<PTR_OR_VAL(CLogEntity)>(
			NEW_DELEGATE(
				QPACK(DNewObj<PTR_OR_VAL(CLogEntity)>),
				this, CLogServiceBackend::_NewLogEntity,
				DELEGATE_PLACEHOLDERS(, std::placeholders::_1)
			)
		);
		m_poLogChannelMgr = QNEW CObjectResMgr<PTR_OR_VAL(CLogChannel)>(
			NEW_DELEGATE(
				QPACK(DNewObj<PTR_OR_VAL(CLogChannel)>),
				this, CLogServiceBackend::_NewLogChannel,
				DELEGATE_PLACEHOLDERS(, std::placeholders::_1)
			)
		);
	}
#ifndef _USING_CLI
	CLogServiceBackend::~CLogServiceBackend()
	{
		SAFE_DELETE(m_poLogEntityMgr);
		SAFE_DELETE(m_poLogChannelMgr);
	}
#endif

	CUniquePtr(CLogEntity) CLogServiceBackend::_NewLogEntity(UINT16 wObjType)
	{
		CUniquePtr(CLogEntity) poObj(QNEW CLogEntity());
		return poObj;
	}
	CUniquePtr(CLogChannel) CLogServiceBackend::_NewLogChannel(UINT16 wObjType)
	{
		CUniquePtr(CLogChannel) poObj(QNEW CLogChannel(this));
		return poObj;
	}

	void CLogServiceBackend::_Awake(PTR_OR_REF(CServiceBackendHelper) roHelper)
	{
		m_poHelper = TO_PTR2(roHelper);
	}

	UINT32 CLogServiceBackend::_Update() 
	{
		return 0;
	}

	PTR(CLogChannel) CLogServiceBackend::CreateLogChannel()
	{
		return m_poLogChannelMgr->CreateObj(0);
	}

	PTR(CLogChannel) CLogServiceBackend::GetLogChannel(CONST_REF(SLogChannelUID) rsLogChannelUID)
	{
		return m_poLogChannelMgr->GetObj(rsLogChannelUID.ToUINT64());
	}

	void CLogServiceBackend::ReleaseLogChannel(CONST_REF(SLogChannelUID) rsLogChannelUID)
	{
		m_poLogChannelMgr->ReleaseObj(rsLogChannelUID.ToUINT64());
	}

	PTR(CLogEntity) CLogServiceBackend::CreateLogEntity(PTR(CLogChannel) poLogChanel, CONST_PTR_OR_REF(DNewLogEntity) cbNewLogEntity)
	{
		PTR(CLogEntity) poLogEntity = m_poLogEntityMgr->CreateObj(0);
		poLogEntity->SetLog(cbNewLogEntity(TO_REF2(m_poHelper)));
		poLogEntity->SetLogChannel(poLogChanel);
		poLogEntity->SetNodeInLogChannel(poLogChanel->Add(poLogEntity));
		return poLogEntity;
	}

	void CLogServiceBackend::ReleaseLogEntity(CONST_REF(SLogEntityUID) rsLogEntityUID)
	{
		m_poLogEntityMgr->ReleaseObj(rsLogEntityUID.ToUINT64());
	}

	void CLogServiceBackend::Print(CONST_REF(SLogChannelUID) rsLogChannelUID, UINT8 btLogLevel, CONST_REF(CString) rstrLog)
	{
		PTR(CLogChannel) poLogChannel = m_poLogChannelMgr->GetObj(rsLogChannelUID.ToUINT64());
		if (NULL_PTR == poLogChannel)
			return;
		poLogChannel->Print(btLogLevel, rstrLog);
	}

}
