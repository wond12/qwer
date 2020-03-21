#include "CLogServiceFrontend.h"
#include "CWriteLogPtc.h"
#include "CCreateLogChannelRpc.h"
#include "../CPromiseDataHelperDefine.h"
#include "CAttachLogEntityRpc.h"
#include "CDetachLogEntityRpc.h"

namespace QWER
{

	void CLogServiceFrontend::_Awake(PTR_OR_REF(CServiceFrontendHelper) roHelper)
	{
		m_poHelper = TO_PTR2(roHelper);
	}

	UINT32 CLogServiceFrontend::_Update()
	{
		return 0;
	}

#ifdef _USING_CLI

	void CLogServiceFrontend::Print(CONST_REF(SLogChannelUID) rsLogChannelUID, UINT8 btLogLevel, ...array<System::Object^>^ msg)
	{
		PTR(CWriteLogPtc) poWriteLogMsg = QNEW CWriteLogPtc();
		poWriteLogMsg->SetLogChannelUID(rsLogChannelUID);
		poWriteLogMsg->SetLogLevel(btLogLevel);
		CString strLog = "";
		for (INT32 i = 0; i != msg->Length; i++)
			strLog += msg[i];
		poWriteLogMsg->SetLog(strLog);
		m_poHelper->PostMsg(poWriteLogMsg).Then<BOOLN>(NEW_DELEGATE(QPACK(DPromiseResolver<BOOLN, PTR(IToServiceBackendMsg)>),
			this, CLogServiceFrontend::_OnWriteLogReply, DELEGATE_PLACEHOLDERS(, std::placeholders::_1)));
	}

#else
	void CLogServiceFrontend::Print(CONST_REF(SLogChannelUID) rsLogChannelUID, UINT8 btLogLevel, CONST_REF(CString) strLog)
	{
		PTR(CWriteLogPtc) poWriteLogMsg = QNEW CWriteLogPtc();
		poWriteLogMsg->SetLogChannelUID(rsLogChannelUID);
		poWriteLogMsg->SetLogLevel(btLogLevel);
		poWriteLogMsg->SetLog(strLog);
		m_poHelper->PostMsg(poWriteLogMsg).Then<BOOLN>(std::bind(&CLogServiceFrontend::_OnWriteLogReply, this,
			std::placeholders::_1));
	}

	void CLogServiceFrontend::Print(CONST_REF(SLogChannelUID) rsLogChannelUID, UINT8 btLogLevel, const char* pchLog)
	{
		PTR(CWriteLogPtc) poWriteLogMsg = QNEW CWriteLogPtc();
		poWriteLogMsg->SetLogChannelUID(rsLogChannelUID);
		poWriteLogMsg->SetLogLevel(btLogLevel);
		poWriteLogMsg->SetLog(pchLog);
		m_poHelper->PostMsg(poWriteLogMsg).Then<BOOLN>(std::bind(&CLogServiceFrontend::_OnWriteLogReply, this,
			std::placeholders::_1));
	}

	void CLogServiceFrontend::_Print(CONST_REF(SLogChannelUID) rsLogChannelUID, UINT8 btLogLevel, std::stringstream& roLog)
	{
		PTR(CWriteLogPtc) poWriteLogMsg = QNEW CWriteLogPtc();
		poWriteLogMsg->SetLogChannelUID(rsLogChannelUID);
		poWriteLogMsg->SetLogLevel(btLogLevel);
		poWriteLogMsg->SetLog(roLog.str());
		m_poHelper->PostMsg(poWriteLogMsg).Then<BOOLN>(std::bind(&CLogServiceFrontend::_OnWriteLogReply, this,
			std::placeholders::_1));
	}
#endif

	SPromiseData<BOOLN> CLogServiceFrontend::_OnWriteLogReply(CONST_REF(SPromiseFuncArg<PTR(IToServiceBackendMsg)>) p)
	{
		PTR(CWriteLogPtc) pTmp = (PTR(CWriteLogPtc))p.m_oData;
		SAFE_DELETE(pTmp);
		if (p.m_ePromiseError != EPromiseErrorType::Success)
			return SPromiseData<BOOLN>(p.m_ePromiseError);
		return SPromiseData<BOOLN>();
	}

	SPromiseData<SLogChannelUID> CLogServiceFrontend::_OnCreateLogChannelReply(CONST_REF(SPromiseFuncArg<PTR(IToServiceBackendMsg)>) p)
	{
		PTR(CCreateLogChannelRpc) pTmp = (PTR(CCreateLogChannelRpc))p.m_oData;
		SLogChannelUID sLogChannelUID = pTmp->m_oRet.m_sLogChannelUID;
		SAFE_DELETE(pTmp);
		return CPromiseDataHelper::CreatePromiseData(sLogChannelUID);
	}

	SPromiseData<SLogEntityUID> CLogServiceFrontend::_OnAttachLogEntityReply(CONST_REF(SPromiseFuncArg<PTR(IToServiceBackendMsg)>) p)
	{
		PTR(CAttachLogEntity) pTmp = (PTR(CAttachLogEntity))p.m_oData;
		SLogEntityUID sLogEntityUID = pTmp->m_oRet.m_sLogEntityUID;
		SAFE_DELETE(pTmp);
		return CPromiseDataHelper::CreatePromiseData(sLogEntityUID);
	}

	SPromiseData<BOOLN> CLogServiceFrontend::_OnDetachLogEntityReply(CONST_REF(SPromiseFuncArg<PTR(IToServiceBackendMsg)>) p)
	{
		PTR(CDetachLogEntity) pTmp = (PTR(CDetachLogEntity))p.m_oData;
		SAFE_DELETE(pTmp);
		if (p.m_ePromiseError != EPromiseErrorType::Success)
			return SPromiseData<BOOLN>(p.m_ePromiseError);
		return SPromiseData<BOOLN>();
	}

	SPromise<SLogChannelUID> CLogServiceFrontend::Create()
	{
		PTR(CCreateLogChannelRpc) poCmd = QNEW CCreateLogChannelRpc();
		return m_poHelper->PostMsg(poCmd).Then<SLogChannelUID>(
			NEW_DELEGATE(
				QPACK(DPromiseResolver<BOOLN, PTR(IToServiceBackendMsg)>),
				this, CLogServiceFrontend::_OnCreateLogChannelReply,
				DELEGATE_PLACEHOLDERS(, std::placeholders::_1)
			)
		);
	}

	SPromise<SLogEntityUID> CLogServiceFrontend::Attach(CONST_REF(SLogChannelUID) rsLogChannelUID, CONST_PTR_OR_REF(DNewLogEntity) cbNewLogEntity, CONST_PTR_OR_REF(CVector(UINT8)) roLogLevels)
	{
		PTR(CAttachLogEntity) poCmd = QNEW CAttachLogEntity();
		poCmd->m_oArg.m_sLogChannelUID = rsLogChannelUID;
		poCmd->m_oArg.m_cbNewLogEntity = cbNewLogEntity;
		CONST_PTR(CVector(UINT8)) poLogLevels = TO_PTR2(roLogLevels);
		for (INT32 i = 0; i != poLogLevels->size(); ++i)
			poCmd->m_oArg.m_oLogLevels.push_back(poLogLevels->at(i));
		return m_poHelper->PostMsg(poCmd).Then<SLogEntityUID>(
			NEW_DELEGATE(
				QPACK(DPromiseResolver<BOOLN, PTR(IToServiceBackendMsg)>),
				this, CLogServiceFrontend::_OnAttachLogEntityReply,
				DELEGATE_PLACEHOLDERS(, std::placeholders::_1)
			)
		);
	}

	void CLogServiceFrontend::Detach(CONST_REF(SLogEntityUID) rsLogEntityUID, CONST_PTR_OR_REF(CVector(UINT8)) roLogLevels)
	{
		PTR(CDetachLogEntity) poCmd = QNEW CDetachLogEntity();
		poCmd->m_oArg.m_sLogEntityUID = rsLogEntityUID;
		CONST_PTR(CVector(UINT8)) poLogLevels = TO_PTR2(roLogLevels);
		for (INT32 i = 0; i != poLogLevels->size(); ++i)
			poCmd->m_oArg.m_oLogLevels.push_back(poLogLevels->at(i));
		m_poHelper->PostMsg(poCmd).Then<BOOLN>(
			NEW_DELEGATE(
				QPACK(DPromiseResolver<BOOLN, PTR(IToServiceBackendMsg)>),
				this, CLogServiceFrontend::_OnDetachLogEntityReply,
				DELEGATE_PLACEHOLDERS(, std::placeholders::_1)
			)
		);
	}

	void CLogServiceFrontend::Detach(CONST_REF(SLogEntityUID) rsLogEntityUID)
	{
		PTR(CDetachLogEntity) poCmd = QNEW CDetachLogEntity();
		poCmd->m_oArg.m_sLogEntityUID = rsLogEntityUID;
		m_poHelper->PostMsg(poCmd).Then<BOOLN>(
			NEW_DELEGATE(
				QPACK(DPromiseResolver<BOOLN, PTR(IToServiceBackendMsg)>),
				this, CLogServiceFrontend::_OnDetachLogEntityReply,
				DELEGATE_PLACEHOLDERS(, std::placeholders::_1)
			)
		);
	}

#ifdef _USING_CLI
	SPromise<SLogEntityUID> CLogServiceFrontend::Attach(CONST_REF(SLogChannelUID) rsLogChannelUID, CONST_PTR_OR_REF(DNewLogEntity) cbNewLogEntity, ...array<UINT8>^ aLogLevels)
	{
		CVector(UINT8) oLogLevels;
		for (INT32 i = 0; i != aLogLevels->Length; i++)
			oLogLevels.push_back(aLogLevels[i]);
		return Attach(rsLogChannelUID, cbNewLogEntity, TO_PTR(oLogLevels));
	}

	void CLogServiceFrontend::Detach(CONST_REF(SLogChannelUID) rsLogChannelUID, CONST_REF(SLogEntityUID) rsLogEntityUID, ...array<UINT8>^ aLogLevels)
	{
		CVector(UINT8) oLogLevels;
		for (INT32 i = 0; i != aLogLevels->Length; i++)
			oLogLevels.push_back(aLogLevels[i]);
		Detach(rsLogChannelUID, rsLogEntityUID, TO_PTR(oLogLevels));
	}
#endif
}
