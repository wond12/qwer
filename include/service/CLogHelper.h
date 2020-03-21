#ifndef _H_LOG_HELPER_H_
#define _H_LOG_HELPER_H_

#include "../StringDefine.h"
#include "../VectorDefine.h"
#include "ILog.h"

namespace QWER
{
	QDLL_EXPORT QCLASS CLogHelper
	{
	public:
		void Register(PTR(ILog) rologInstance, UINT8 btLogType);

#ifdef _USING_CLI
		//void Print(UINT8 btLogType, ...array<System::Object^>^ msg);
		//inline void DebugPrint(...array<System::Object^>^ msg)
		//{
		//	Print(CDefaultLogType::Debug, msg);
		//}
		//inline void InfoPrint(...array<System::Object^>^ msg)
		//{
		//	Print(CDefaultLogType::Info, msg);
		//}
		//inline void WarningPrint(...array<System::Object^>^ msg)
		//{
		//	Print(CDefaultLogType::Warning, msg);
		//}
		//inline void ErrorPrint(...array<System::Object^>^ msg)
		//{
		//	Print(CDefaultLogType::Error, msg);
		//}
		void Print(UINT8 btLogType, CONST_REF(CString) msg);
#else
		template<typename T>
		void Print(UINT8 btLogType, CONST_REF(T) msg)
		{
			m_oStringBuilder.Clear();
			m_oStringBuilder << msg;
			_Print(btLogType, m_oStringBuilder.c_str());
		}
		template<typename T1, typename T2>
		void Print(UINT8 btLogType, CONST_REF(T1) msg1, CONST_REF(T2) msg2)
		{
			m_oStringBuilder.Clear();
			m_oStringBuilder << msg1 << msg2;
			_Print(btLogType, m_oStringBuilder.c_str());
		}
		template<typename T1, typename T2, typename T3>
		void Print(UINT8 btLogType, CONST_REF(T1) msg1, CONST_REF(T2) msg2, CONST_REF(T3) msg3)
		{
			m_oStringBuilder.Clear();
			m_oStringBuilder << msg1 << msg2 << msg3;
			_Print(btLogType, m_oStringBuilder.c_str());
		}
		template<typename T1, typename T2, typename T3, typename T4>
		void Print(UINT8 btLogType, CONST_REF(T1) msg1, CONST_REF(T2) msg2, CONST_REF(T3) msg3, CONST_REF(T4) msg4)
		{
			m_oStringBuilder.Clear();
			m_oStringBuilder << msg1 << msg2 << msg3 << msg4;
			_Print(btLogType, m_oStringBuilder.c_str());
		}
		template<typename T1, typename T2, typename T3, typename T4, typename T5>
		void Print(UINT8 btLogType, CONST_REF(T1) msg1, CONST_REF(T2) msg2, CONST_REF(T3) msg3, CONST_REF(T4) msg4, CONST_REF(T5) msg5)
		{
			m_oStringBuilder.Clear();
			m_oStringBuilder << msg1 << msg2 << msg3 << msg4 << msg5;
			_Print(btLogType, m_oStringBuilder.c_str());
		}
		template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
		void Print(UINT8 btLogType, CONST_REF(T1) msg1, CONST_REF(T2) msg2, CONST_REF(T3) msg3, CONST_REF(T4) msg4, CONST_REF(T5) msg5, CONST_REF(T6) msg6)
		{
			m_oStringBuilder.Clear();
			m_oStringBuilder << msg1 << msg2 << msg3 << msg4 << msg5 << msg6;
			_Print(btLogType, m_oStringBuilder.c_str());
		}
		template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
		void Print(UINT8 btLogType, CONST_REF(T1) msg1, CONST_REF(T2) msg2, CONST_REF(T3) msg3, CONST_REF(T4) msg4, CONST_REF(T5) msg5, CONST_REF(T6) msg6, CONST_REF(T7) msg7)
		{
			m_oStringBuilder.Clear();
			m_oStringBuilder << msg1 << msg2 << msg3 << msg4 << msg5 << msg6 << msg7;
			_Print(btLogType, m_oStringBuilder.c_str());
		}
		template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
		void Print(UINT8 btLogType, CONST_REF(T1) msg1, CONST_REF(T2) msg2, CONST_REF(T3) msg3, CONST_REF(T4) msg4, CONST_REF(T5) msg5, CONST_REF(T6) msg6, CONST_REF(T7) msg7, CONST_REF(T8) msg8)
		{
			m_oStringBuilder.Clear();
			m_oStringBuilder << msg1 << msg2 << msg3 << msg4 << msg5 << msg6 << msg7 << msg8;
			_Print(btLogType, m_oStringBuilder.c_str());
		}
		template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
		void Print(UINT8 btLogType, CONST_REF(T1) msg1, CONST_REF(T2) msg2, CONST_REF(T3) msg3, CONST_REF(T4) msg4, CONST_REF(T5) msg5, CONST_REF(T6) msg6, CONST_REF(T7) msg7, CONST_REF(T8) msg8, CONST_REF(T9) msg9)
		{
			m_oStringBuilder.Clear();
			m_oStringBuilder << msg1 << msg2 << msg3 << msg4 << msg5 << msg6 << msg7 << msg8 << msg9;
			_Print(btLogType, m_oStringBuilder.c_str());
		}
		template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
		void Print(UINT8 btLogType, CONST_REF(T1) msg1, CONST_REF(T2) msg2, CONST_REF(T3) msg3, CONST_REF(T4) msg4, CONST_REF(T5) msg5, CONST_REF(T6) msg6, CONST_REF(T7) msg7, CONST_REF(T8) msg8, CONST_REF(T9) msg9, CONST_REF(T10) msg10)
		{
			m_oStringBuilder.Clear();
			m_oStringBuilder << msg1 << msg2 << msg3 << msg4 << msg5 << msg6 << msg7 << msg8 << msg9 << msg10;
			_Print(btLogType, m_oStringBuilder.c_str());
		}

		template<typename T>
		void DebugPrint(CONST_REF(T) msg)
		{
			Print(CDefaultLogType::Debug, msg);
		}
		template<typename T1, typename T2>
		void DebugPrint(CONST_REF(T1) msg1, CONST_REF(T2) msg2)
		{
			Print(CDefaultLogType::Debug, msg1, msg2);
		}
		template<typename T1, typename T2, typename T3>
		void DebugPrint(CONST_REF(T1) msg1, CONST_REF(T2) msg2, CONST_REF(T3) msg3)
		{
			Print(CDefaultLogType::Debug, msg1, msg2, msg3);
		}
		template<typename T1, typename T2, typename T3, typename T4>
		void DebugPrint(CONST_REF(T1) msg1, CONST_REF(T2) msg2, CONST_REF(T3) msg3, CONST_REF(T4) msg4)
		{
			Print(CDefaultLogType::Debug, msg1, msg2, msg3, msg4);
		}
		template<typename T1, typename T2, typename T3, typename T4, typename T5>
		void DebugPrint(CONST_REF(T1) msg1, CONST_REF(T2) msg2, CONST_REF(T3) msg3, CONST_REF(T4) msg4, CONST_REF(T5) msg5)
		{
			Print(CDefaultLogType::Debug, msg1, msg2, msg3, msg4, msg5);
		}
		template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
		void DebugPrint(CONST_REF(T1) msg1, CONST_REF(T2) msg2, CONST_REF(T3) msg3, CONST_REF(T4) msg4, CONST_REF(T5) msg5, CONST_REF(T6) msg6)
		{
			Print(CDefaultLogType::Debug, msg1, msg2, msg3, msg4, msg5, msg6);
		}
		template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
		void DebugPrint(CONST_REF(T1) msg1, CONST_REF(T2) msg2, CONST_REF(T3) msg3, CONST_REF(T4) msg4, CONST_REF(T5) msg5, CONST_REF(T6) msg6, CONST_REF(T7) msg7)
		{
			Print(CDefaultLogType::Debug, msg1, msg2, msg3, msg4, msg5, msg6, msg7);
		}
		template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
		void DebugPrint(CONST_REF(T1) msg1, CONST_REF(T2) msg2, CONST_REF(T3) msg3, CONST_REF(T4) msg4, CONST_REF(T5) msg5, CONST_REF(T6) msg6, CONST_REF(T7) msg7, CONST_REF(T8) msg8)
		{
			Print(CDefaultLogType::Debug, msg1, msg2, msg3, msg4, msg5, msg6, msg7, msg8);
		}
		template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
		void DebugPrint(CONST_REF(T1) msg1, CONST_REF(T2) msg2, CONST_REF(T3) msg3, CONST_REF(T4) msg4, CONST_REF(T5) msg5, CONST_REF(T6) msg6, CONST_REF(T7) msg7, CONST_REF(T8) msg8, CONST_REF(T9) msg9)
		{
			Print(CDefaultLogType::Debug, msg1, msg2, msg3, msg4, msg5, msg6, msg7, msg8, msg9);
		}
		template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
		void DebugPrint(CONST_REF(T1) msg1, CONST_REF(T2) msg2, CONST_REF(T3) msg3, CONST_REF(T4) msg4, CONST_REF(T5) msg5, CONST_REF(T6) msg6, CONST_REF(T7) msg7, CONST_REF(T8) msg8, CONST_REF(T9) msg9, CONST_REF(T10) msg10)
		{
			Print(CDefaultLogType::Debug, msg1, msg2, msg3, msg4, msg5, msg6, msg7, msg8, msg9, msg10);
		}

		template<typename T>
		void InfoPrint(CONST_REF(T) msg)
		{
			Print(CDefaultLogType::Info, msg);
		}
		template<typename T1, typename T2>
		void InfoPrint(CONST_REF(T1) msg1, CONST_REF(T2) msg2)
		{
			Print(CDefaultLogType::Info, msg1, msg2);
		}
		template<typename T1, typename T2, typename T3>
		void InfoPrint(CONST_REF(T1) msg1, CONST_REF(T2) msg2, CONST_REF(T3) msg3)
		{
			Print(CDefaultLogType::Info, msg1, msg2, msg3);
		}
		template<typename T1, typename T2, typename T3, typename T4>
		void InfoPrint(CONST_REF(T1) msg1, CONST_REF(T2) msg2, CONST_REF(T3) msg3, CONST_REF(T4) msg4)
		{
			Print(CDefaultLogType::Info, msg1, msg2, msg3, msg4);
		}
		template<typename T1, typename T2, typename T3, typename T4, typename T5>
		void InfoPrint(CONST_REF(T1) msg1, CONST_REF(T2) msg2, CONST_REF(T3) msg3, CONST_REF(T4) msg4, CONST_REF(T5) msg5)
		{
			Print(CDefaultLogType::Info, msg1, msg2, msg3, msg4, msg5);
		}
		template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
		void InfoPrint(CONST_REF(T1) msg1, CONST_REF(T2) msg2, CONST_REF(T3) msg3, CONST_REF(T4) msg4, CONST_REF(T5) msg5, CONST_REF(T6) msg6)
		{
			Print(CDefaultLogType::Info, msg1, msg2, msg3, msg4, msg5, msg6);
		}
		template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
		void InfoPrint(CONST_REF(T1) msg1, CONST_REF(T2) msg2, CONST_REF(T3) msg3, CONST_REF(T4) msg4, CONST_REF(T5) msg5, CONST_REF(T6) msg6, CONST_REF(T7) msg7)
		{
			Print(CDefaultLogType::Info, msg1, msg2, msg3, msg4, msg5, msg6, msg7);
		}
		template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
		void InfoPrint(CONST_REF(T1) msg1, CONST_REF(T2) msg2, CONST_REF(T3) msg3, CONST_REF(T4) msg4, CONST_REF(T5) msg5, CONST_REF(T6) msg6, CONST_REF(T7) msg7, CONST_REF(T8) msg8)
		{
			Print(CDefaultLogType::Info, msg1, msg2, msg3, msg4, msg5, msg6, msg7, msg8);
		}
		template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
		void InfoPrint(CONST_REF(T1) msg1, CONST_REF(T2) msg2, CONST_REF(T3) msg3, CONST_REF(T4) msg4, CONST_REF(T5) msg5, CONST_REF(T6) msg6, CONST_REF(T7) msg7, CONST_REF(T8) msg8, CONST_REF(T9) msg9)
		{
			Print(CDefaultLogType::Info, msg1, msg2, msg3, msg4, msg5, msg6, msg7, msg8, msg9);
		}
		template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
		void InfoPrint(CONST_REF(T1) msg1, CONST_REF(T2) msg2, CONST_REF(T3) msg3, CONST_REF(T4) msg4, CONST_REF(T5) msg5, CONST_REF(T6) msg6, CONST_REF(T7) msg7, CONST_REF(T8) msg8, CONST_REF(T9) msg9, CONST_REF(T10) msg10)
		{
			Print(CDefaultLogType::Info, msg1, msg2, msg3, msg4, msg5, msg6, msg7, msg8, msg9, msg10);
		}

		template<typename T>
		void WarningPrint(CONST_REF(T) msg)
		{
			Print(CDefaultLogType::Warning, msg);
		}
		template<typename T1, typename T2>
		void WarningPrint(CONST_REF(T1) msg1, CONST_REF(T2) msg2)
		{
			Print(CDefaultLogType::Warning, msg1, msg2);
		}
		template<typename T1, typename T2, typename T3>
		void WarningPrint(CONST_REF(T1) msg1, CONST_REF(T2) msg2, CONST_REF(T3) msg3)
		{
			Print(CDefaultLogType::Warning, msg1, msg2, msg3);
		}
		template<typename T1, typename T2, typename T3, typename T4>
		void WarningPrint(CONST_REF(T1) msg1, CONST_REF(T2) msg2, CONST_REF(T3) msg3, CONST_REF(T4) msg4)
		{
			Print(CDefaultLogType::Warning, msg1, msg2, msg3, msg4);
		}
		template<typename T1, typename T2, typename T3, typename T4, typename T5>
		void WarningPrint(CONST_REF(T1) msg1, CONST_REF(T2) msg2, CONST_REF(T3) msg3, CONST_REF(T4) msg4, CONST_REF(T5) msg5)
		{
			Print(CDefaultLogType::Warning, msg1, msg2, msg3, msg4, msg5);
		}
		template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
		void WarningPrint(CONST_REF(T1) msg1, CONST_REF(T2) msg2, CONST_REF(T3) msg3, CONST_REF(T4) msg4, CONST_REF(T5) msg5, CONST_REF(T6) msg6)
		{
			Print(CDefaultLogType::Warning, msg1, msg2, msg3, msg4, msg5, msg6);
		}
		template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
		void WarningPrint(CONST_REF(T1) msg1, CONST_REF(T2) msg2, CONST_REF(T3) msg3, CONST_REF(T4) msg4, CONST_REF(T5) msg5, CONST_REF(T6) msg6, CONST_REF(T7) msg7)
		{
			Print(CDefaultLogType::Warning, msg1, msg2, msg3, msg4, msg5, msg6, msg7);
		}
		template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
		void WarningPrint(CONST_REF(T1) msg1, CONST_REF(T2) msg2, CONST_REF(T3) msg3, CONST_REF(T4) msg4, CONST_REF(T5) msg5, CONST_REF(T6) msg6, CONST_REF(T7) msg7, CONST_REF(T8) msg8)
		{
			Print(CDefaultLogType::Warning, msg1, msg2, msg3, msg4, msg5, msg6, msg7, msg8);
		}
		template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
		void WarningPrint(CONST_REF(T1) msg1, CONST_REF(T2) msg2, CONST_REF(T3) msg3, CONST_REF(T4) msg4, CONST_REF(T5) msg5, CONST_REF(T6) msg6, CONST_REF(T7) msg7, CONST_REF(T8) msg8, CONST_REF(T9) msg9)
		{
			Print(CDefaultLogType::Warning, msg1, msg2, msg3, msg4, msg5, msg6, msg7, msg8, msg9);
		}
		template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
		void WarningPrint(CONST_REF(T1) msg1, CONST_REF(T2) msg2, CONST_REF(T3) msg3, CONST_REF(T4) msg4, CONST_REF(T5) msg5, CONST_REF(T6) msg6, CONST_REF(T7) msg7, CONST_REF(T8) msg8, CONST_REF(T9) msg9, CONST_REF(T10) msg10)
		{
			Print(CDefaultLogType::Warning, msg1, msg2, msg3, msg4, msg5, msg6, msg7, msg8, msg9, msg10);
		}

		template<typename T>
		void ErrorPrint(CONST_REF(T) msg)
		{
			Print(CDefaultLogType::Error, msg);
		}
		template<typename T1, typename T2>
		void ErrorPrint(CONST_REF(T1) msg1, CONST_REF(T2) msg2)
		{
			Print(CDefaultLogType::Error, msg1, msg2);
		}
		template<typename T1, typename T2, typename T3>
		void ErrorPrint(CONST_REF(T1) msg1, CONST_REF(T2) msg2, CONST_REF(T3) msg3)
		{
			Print(CDefaultLogType::Error, msg1, msg2, msg3);
		}
		template<typename T1, typename T2, typename T3, typename T4>
		void ErrorPrint(CONST_REF(T1) msg1, CONST_REF(T2) msg2, CONST_REF(T3) msg3, CONST_REF(T4) msg4)
		{
			Print(CDefaultLogType::Error, msg1, msg2, msg3, msg4);
		}
		template<typename T1, typename T2, typename T3, typename T4, typename T5>
		void ErrorPrint(CONST_REF(T1) msg1, CONST_REF(T2) msg2, CONST_REF(T3) msg3, CONST_REF(T4) msg4, CONST_REF(T5) msg5)
		{
			Print(CDefaultLogType::Error, msg1, msg2, msg3, msg4, msg5);
		}
		template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
		void ErrorPrint(CONST_REF(T1) msg1, CONST_REF(T2) msg2, CONST_REF(T3) msg3, CONST_REF(T4) msg4, CONST_REF(T5) msg5, CONST_REF(T6) msg6)
		{
			Print(CDefaultLogType::Error, msg1, msg2, msg3, msg4, msg5, msg6);
		}
		template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
		void ErrorPrint(CONST_REF(T1) msg1, CONST_REF(T2) msg2, CONST_REF(T3) msg3, CONST_REF(T4) msg4, CONST_REF(T5) msg5, CONST_REF(T6) msg6, CONST_REF(T7) msg7)
		{
			Print(CDefaultLogType::Error, msg1, msg2, msg3, msg4, msg5, msg6, msg7);
		}
		template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
		void ErrorPrint(CONST_REF(T1) msg1, CONST_REF(T2) msg2, CONST_REF(T3) msg3, CONST_REF(T4) msg4, CONST_REF(T5) msg5, CONST_REF(T6) msg6, CONST_REF(T7) msg7, CONST_REF(T8) msg8)
		{
			Print(CDefaultLogType::Error, msg1, msg2, msg3, msg4, msg5, msg6, msg7, msg8);
		}
		template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
		void ErrorPrint(CONST_REF(T1) msg1, CONST_REF(T2) msg2, CONST_REF(T3) msg3, CONST_REF(T4) msg4, CONST_REF(T5) msg5, CONST_REF(T6) msg6, CONST_REF(T7) msg7, CONST_REF(T8) msg8, CONST_REF(T9) msg9)
		{
			Print(CDefaultLogType::Error, msg1, msg2, msg3, msg4, msg5, msg6, msg7, msg8, msg9);
		}
		template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
		void ErrorPrint(CONST_REF(T1) msg1, CONST_REF(T2) msg2, CONST_REF(T3) msg3, CONST_REF(T4) msg4, CONST_REF(T5) msg5, CONST_REF(T6) msg6, CONST_REF(T7) msg7, CONST_REF(T8) msg8, CONST_REF(T9) msg9, CONST_REF(T10) msg10)
		{
			Print(CDefaultLogType::Error, msg1, msg2, msg3, msg4, msg5, msg6, msg7, msg8, msg9, msg10);
		}
#endif

	private:
#ifdef _USING_CLI
		void _Print(UINT8 btLogType, CONST_REF(CString) strLog);
#else
		void _Print(UINT8 btLogType, const char* strLog);
#endif

	private:
#ifdef _USING_CLI
		array<CVector(PTR(ILog))^>^ m_aLogInstances = QNEW array<CVector(PTR(ILog))^>(256);
#else
		CVector(PTR(ILog)) m_aLogInstances[256];
#endif
	};
}

#endif
