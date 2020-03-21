#ifndef _H_C_PROMISE_ALL_H_
#define _H_C_PROMISE_ALL_H_

#include "../CTypeDefine.h"
#include "../TupleDefine.h"
#include "../VectorDefine.h"
#include "SPromiseFuncArg.h"
#include "CPromiseHelper2.h"
#include "CPromiseHelper.h"
#include "CPromiseContainer.h"

#define _PROMISE_DATA_SIGN_1	0x1u
#define _PROMISE_DATA_SIGN_2	0x2u
#define _PROMISE_DATA_SIGN_3	0x4u
#define _PROMISE_DATA_SIGN_4	0x8u
#define _PROMISE_DATA_SIGN_5	0x10u
#define _PROMISE_DATA_SIGN_6	0x20u
#define _PROMISE_DATA_SIGN_7	0x40u
#define _PROMISE_DATA_SIGN_8	0x80u
#define _PROMISE_DATA_SIGN_9	0x100u
#define _PROMISE_DATA_SIGN_10	0x200u
#define _PROMISE_DATA_SIGN_11	0x400u
#define _PROMISE_DATA_SIGN_12	0x800u
#define _PROMISE_DATA_SIGN_13	0x1000u
#define _PROMISE_DATA_SIGN_14	0x2000u
#define _PROMISE_DATA_SIGN_15	0x4000u
#define _PROMISE_DATA_SIGN_16	0x8000u
#define _PROMISE_DATA_SIGN_17	0x10000u
#define _PROMISE_DATA_SIGN_18	0x20000u
#define _PROMISE_DATA_SIGN_19	0x40000u
#define _PROMISE_DATA_SIGN_20	0x80000u
#define _PROMISE_DATA_SIGN_21	0x100000u
#define _PROMISE_DATA_SIGN_22	0x200000u
#define _PROMISE_DATA_SIGN_23	0x400000u
#define _PROMISE_DATA_SIGN_24	0x800000u
#define _PROMISE_DATA_SIGN_25	0x1000000u
#define _PROMISE_DATA_SIGN_26	0x2000000u
#define _PROMISE_DATA_SIGN_27	0x4000000u
#define _PROMISE_DATA_SIGN_28	0x8000000u
#define _PROMISE_DATA_SIGN_29	0x10000000u
#define _PROMISE_DATA_SIGN_30	0x20000000u
#define _PROMISE_DATA_SIGN_31	0x40000000u
#define _PROMISE_DATA_SIGN_32	0x80000000u

namespace QWER
{
	QTEMPLATE<typename T>
	QCLASS CPromiseAll
	{
	public:
		QCLASS CPromiseAllExec
		{
		public:
			SPromiseData<BOOLN> FinishT(CONST_REF(SPromiseFuncArg<T>) roArg)
			{
				(TO_PTR1(m_poOwner->m_oData))[m_dwIndex] = roArg;
				++m_poOwner->m_dwCount;
				m_poOwner->_TryFinishPromise();
				return SPromiseData<BOOLN>(true);
			}
			inline void Init(PTR(CPromiseAll) poOwner, UINT32 dwIndex){ m_poOwner = poOwner; m_dwIndex = dwIndex; }

		private:
			UINT32 m_dwIndex = 0;
			PTR(CPromiseAll) m_poOwner = NULL_PTR;
		};

	public:
		CPromiseAll(PTR(CPromise<PTR_OR_VAL(CVector(SPromiseFuncArg<T>)) >) poPromise, UINT32 dwArgCount)
		{ 
			m_qwPromiseUID = poPromise->GetUID();
			m_poPromiseMgr = poPromise->GetPromiseMgr();
			m_oData.resize(dwArgCount); 
			m_oExec.resize(dwArgCount);
			for (VECINDEX i = 0; i != m_oExec.size(); ++i)
			{
#ifdef _USING_CLI
				(TO_PTR(m_oExec))[i] = QNEW CPromiseAllExec();
#endif
				PTR(CPromiseAllExec) poExec = TO_PTR2(m_oExec.at(i));
				poExec->Init(this, (UINT32)i);
			}
		}
		PTR(CPromiseAllExec) GetExec(UINT32 dwIndex)
		{
			return TO_PTR2(m_oExec.at(dwIndex));
		}

	private:
		void _TryFinishPromise()
		{
			if (m_dwCount != m_oData.size())
				return;
			PTR(CPromise<PTR_OR_VAL(CVector(SPromiseFuncArg<T>)) >) poPromise =
				(PTR(CPromise<PTR_OR_VAL(CVector(SPromiseFuncArg<T>)) >))CPromiseHelper::GetPromise(m_poPromiseMgr, m_qwPromiseUID);
			CPromiseHelper2::ResolvePromise(poPromise, TO_PTR1(m_oData));
#ifndef _USING_CLI
			delete this;
#endif
		}

	private:
		UINT64 m_qwPromiseUID = 0;
		PTR(CPromiseMgr) m_poPromiseMgr = NULL_PTR;
		CVector(SPromiseFuncArg<T>) m_oData;
		CVector(PTR_OR_VAL(CPromiseAllExec)) m_oExec;
		UINT32 m_dwCount = 0;
	};

	QTEMPLATE<typename T1, typename T2>
	QCLASS CPromiseAll2
	{
#define _PROMISE_ALL_2_PARAMS_TYPE_ PTR(CPromise<STuple(SPromiseFuncArg<T1>, SPromiseFuncArg<T2>) >)
	public:
		CPromiseAll2(_PROMISE_ALL_2_PARAMS_TYPE_ poPromise){ m_qwPromiseUID = poPromise->GetUID(); m_poPromiseMgr = poPromise->GetPromiseMgr(); }

		SPromiseData<BOOLN> FinishT1(CONST_REF(SPromiseFuncArg<T1>) roArg){ m_oData1 = roArg; m_dwFinishSign &= ~_PROMISE_DATA_SIGN_1; _TryFinishPromise(); return SPromiseData<BOOLN>(true); }
		SPromiseData<BOOLN> FinishT2(CONST_REF(SPromiseFuncArg<T2>) roArg){ m_oData2 = roArg; m_dwFinishSign &= ~_PROMISE_DATA_SIGN_2; _TryFinishPromise(); return SPromiseData<BOOLN>(true); }

	private:
		void _TryFinishPromise()
		{
			if (m_dwFinishSign != 0)
				return;
			_PROMISE_ALL_2_PARAMS_TYPE_ poPromise = (_PROMISE_ALL_2_PARAMS_TYPE_)CPromiseHelper::GetPromise(m_poPromiseMgr, m_qwPromiseUID);
            CONST_REF(auto) oTuple = MakeTuple(m_oData1, m_oData2);
			CPromiseHelper2::ResolvePromise(poPromise, oTuple);
#ifndef _USING_CLI
			delete this;
#endif
		}

	private:
		UINT64 m_qwPromiseUID = 0;
		PTR(CPromiseMgr) m_poPromiseMgr = NULL_PTR;
		SPromiseFuncArg<T1> m_oData1;
		SPromiseFuncArg<T2> m_oData2;
		UINT32 m_dwFinishSign = (_PROMISE_DATA_SIGN_1 | _PROMISE_DATA_SIGN_2);
	};

	QTEMPLATE<typename T1, typename T2, typename T3>
		QCLASS CPromiseAll3
	{
		//DO
#define _PROMISE_ALL_3_PARAMS_TYPE_ PTR(CPromise<STuple(SPromiseFuncArg<T1>, SPromiseFuncArg<T2>, SPromiseFuncArg<T3>) >)
	public:
		//DO
		CPromiseAll3(_PROMISE_ALL_3_PARAMS_TYPE_ poPromise){ m_qwPromiseUID = poPromise->GetUID(); m_poPromiseMgr = poPromise->GetPromiseMgr(); }
		//DO
		SPromiseData<BOOLN> FinishT1(CONST_REF(SPromiseFuncArg<T1>) roArg){ m_oData1 = roArg; m_dwFinishSign &= ~_PROMISE_DATA_SIGN_1; _TryFinishPromise(); return SPromiseData<BOOLN>(true); }
		SPromiseData<BOOLN> FinishT2(CONST_REF(SPromiseFuncArg<T2>) roArg){ m_oData2 = roArg; m_dwFinishSign &= ~_PROMISE_DATA_SIGN_2; _TryFinishPromise(); return SPromiseData<BOOLN>(true); }
		SPromiseData<BOOLN> FinishT3(CONST_REF(SPromiseFuncArg<T3>) roArg){ m_oData3 = roArg; m_dwFinishSign &= ~_PROMISE_DATA_SIGN_3; _TryFinishPromise(); return SPromiseData<BOOLN>(true); }

	private:
		void _TryFinishPromise()
		{
			if (m_dwFinishSign != 0)
				return;
			//DO
			_PROMISE_ALL_3_PARAMS_TYPE_ poPromise = (_PROMISE_ALL_3_PARAMS_TYPE_)CPromiseHelper::GetPromise(m_poPromiseMgr, m_qwPromiseUID);
            CONST_REF(auto) oTuple = MakeTuple(m_oData1, m_oData2, m_oData3);
			CPromiseHelper2::ResolvePromise(poPromise, oTuple);
#ifndef _USING_CLI
			delete this;
#endif
		}

	private:
		UINT64 m_qwPromiseUID = 0;
		PTR(CPromiseMgr) m_poPromiseMgr = NULL_PTR;
		//DO
		SPromiseFuncArg<T1> m_oData1;
		SPromiseFuncArg<T2> m_oData2;
		SPromiseFuncArg<T3> m_oData3;
		//DO
		UINT32 m_dwFinishSign = (_PROMISE_DATA_SIGN_1 | _PROMISE_DATA_SIGN_2 | _PROMISE_DATA_SIGN_3);
	};

	QTEMPLATE<typename T1, typename T2, typename T3, typename T4>
		QCLASS CPromiseAll4
	{
		//DO
#define _PROMISE_ALL_4_PARAMS_TYPE_ PTR(CPromise<STuple(SPromiseFuncArg<T1>, SPromiseFuncArg<T2>, SPromiseFuncArg<T3>, SPromiseFuncArg<T4>) >)
	public:
		//DO
		CPromiseAll4(_PROMISE_ALL_4_PARAMS_TYPE_ poPromise){ m_qwPromiseUID = poPromise->GetUID(); m_poPromiseMgr = poPromise->GetPromiseMgr(); }
		//DO
		SPromiseData<BOOLN> FinishT1(CONST_REF(SPromiseFuncArg<T1>) roArg){ m_oData1 = roArg; m_dwFinishSign &= ~_PROMISE_DATA_SIGN_1; _TryFinishPromise(); return SPromiseData<BOOLN>(true); }
		SPromiseData<BOOLN> FinishT2(CONST_REF(SPromiseFuncArg<T2>) roArg){ m_oData2 = roArg; m_dwFinishSign &= ~_PROMISE_DATA_SIGN_2; _TryFinishPromise(); return SPromiseData<BOOLN>(true); }
		SPromiseData<BOOLN> FinishT3(CONST_REF(SPromiseFuncArg<T3>) roArg){ m_oData3 = roArg; m_dwFinishSign &= ~_PROMISE_DATA_SIGN_3; _TryFinishPromise(); return SPromiseData<BOOLN>(true); }
		SPromiseData<BOOLN> FinishT4(CONST_REF(SPromiseFuncArg<T4>) roArg){ m_oData4 = roArg; m_dwFinishSign &= ~_PROMISE_DATA_SIGN_4; _TryFinishPromise(); return SPromiseData<BOOLN>(true); }

	private:
		void _TryFinishPromise()
		{
			if (m_dwFinishSign != 0)
				return;
			//DO
			_PROMISE_ALL_4_PARAMS_TYPE_ poPromise = (_PROMISE_ALL_4_PARAMS_TYPE_)CPromiseHelper::GetPromise(m_poPromiseMgr, m_qwPromiseUID);
            CONST_REF(auto) oTuple = MakeTuple(m_oData1, m_oData2, m_oData3, m_oData4);
			CPromiseHelper2::ResolvePromise(poPromise, oTuple);
#ifndef _USING_CLI
			delete this;
#endif
		}

	private:
		UINT64 m_qwPromiseUID = 0;
		PTR(CPromiseMgr) m_poPromiseMgr = NULL_PTR;
		//DO
		SPromiseFuncArg<T1> m_oData1;
		SPromiseFuncArg<T2> m_oData2;
		SPromiseFuncArg<T3> m_oData3;
		SPromiseFuncArg<T4> m_oData4;
		//DO
		UINT32 m_dwFinishSign = (_PROMISE_DATA_SIGN_1 | _PROMISE_DATA_SIGN_2 | _PROMISE_DATA_SIGN_3 | _PROMISE_DATA_SIGN_4);
	};

	QTEMPLATE<typename T1, typename T2, typename T3, typename T4, typename T5>
		QCLASS CPromiseAll5
	{
		//DO
#define _PROMISE_ALL_5_PARAMS_TYPE_ PTR(CPromise<STuple(SPromiseFuncArg<T1>, SPromiseFuncArg<T2>, SPromiseFuncArg<T3>, SPromiseFuncArg<T4>, SPromiseFuncArg<T5>) >)
	public:
		//DO
		CPromiseAll5(_PROMISE_ALL_5_PARAMS_TYPE_ poPromise){ m_qwPromiseUID = poPromise->GetUID(); m_poPromiseMgr = poPromise->GetPromiseMgr(); }
		//DO
		SPromiseData<BOOLN> FinishT1(CONST_REF(SPromiseFuncArg<T1>) roArg){ m_oData1 = roArg; m_dwFinishSign &= ~_PROMISE_DATA_SIGN_1; _TryFinishPromise(); return SPromiseData<BOOLN>(true); }
		SPromiseData<BOOLN> FinishT2(CONST_REF(SPromiseFuncArg<T2>) roArg){ m_oData2 = roArg; m_dwFinishSign &= ~_PROMISE_DATA_SIGN_2; _TryFinishPromise(); return SPromiseData<BOOLN>(true); }
		SPromiseData<BOOLN> FinishT3(CONST_REF(SPromiseFuncArg<T3>) roArg){ m_oData3 = roArg; m_dwFinishSign &= ~_PROMISE_DATA_SIGN_3; _TryFinishPromise(); return SPromiseData<BOOLN>(true); }
		SPromiseData<BOOLN> FinishT4(CONST_REF(SPromiseFuncArg<T4>) roArg){ m_oData4 = roArg; m_dwFinishSign &= ~_PROMISE_DATA_SIGN_4; _TryFinishPromise(); return SPromiseData<BOOLN>(true); }
		SPromiseData<BOOLN> FinishT5(CONST_REF(SPromiseFuncArg<T5>) roArg){ m_oData5 = roArg; m_dwFinishSign &= ~_PROMISE_DATA_SIGN_5; _TryFinishPromise(); return SPromiseData<BOOLN>(true); }

	private:
		void _TryFinishPromise()
		{
			if (m_dwFinishSign != 0)
				return;
			//DO
			_PROMISE_ALL_5_PARAMS_TYPE_ poPromise = (_PROMISE_ALL_5_PARAMS_TYPE_)CPromiseHelper::GetPromise(m_poPromiseMgr, m_qwPromiseUID);
            CONST_REF(auto) oTuple = MakeTuple(m_oData1, m_oData2, m_oData3, m_oData4, m_oData5);
			CPromiseHelper2::ResolvePromise(poPromise, oTuple);
#ifndef _USING_CLI
			delete this;
#endif
		}

	private:
		UINT64 m_qwPromiseUID = 0;
		PTR(CPromiseMgr) m_poPromiseMgr = NULL_PTR;
		//DO
		SPromiseFuncArg<T1> m_oData1;
		SPromiseFuncArg<T2> m_oData2;
		SPromiseFuncArg<T3> m_oData3;
		SPromiseFuncArg<T4> m_oData4;
		SPromiseFuncArg<T5> m_oData5;
		//DO
		UINT32 m_dwFinishSign = (_PROMISE_DATA_SIGN_1 | _PROMISE_DATA_SIGN_2 | _PROMISE_DATA_SIGN_3 | _PROMISE_DATA_SIGN_4 | _PROMISE_DATA_SIGN_5);
	};

	QTEMPLATE<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
		QCLASS CPromiseAll6
	{
		//DO
#define _PROMISE_ALL_6_PARAMS_TYPE_ PTR(CPromise<STuple(SPromiseFuncArg<T1>, SPromiseFuncArg<T2>, SPromiseFuncArg<T3>, SPromiseFuncArg<T4>, SPromiseFuncArg<T5>, \
		SPromiseFuncArg<T6>) >)
	public:
		//DO
		CPromiseAll6(_PROMISE_ALL_6_PARAMS_TYPE_ poPromise){ m_qwPromiseUID = poPromise->GetUID(); m_poPromiseMgr = poPromise->GetPromiseMgr(); }
		//DO
		SPromiseData<BOOLN> FinishT1(CONST_REF(SPromiseFuncArg<T1>) roArg){ m_oData1 = roArg; m_dwFinishSign &= ~_PROMISE_DATA_SIGN_1; _TryFinishPromise(); return SPromiseData<BOOLN>(true); }
		SPromiseData<BOOLN> FinishT2(CONST_REF(SPromiseFuncArg<T2>) roArg){ m_oData2 = roArg; m_dwFinishSign &= ~_PROMISE_DATA_SIGN_2; _TryFinishPromise(); return SPromiseData<BOOLN>(true); }
		SPromiseData<BOOLN> FinishT3(CONST_REF(SPromiseFuncArg<T3>) roArg){ m_oData3 = roArg; m_dwFinishSign &= ~_PROMISE_DATA_SIGN_3; _TryFinishPromise(); return SPromiseData<BOOLN>(true); }
		SPromiseData<BOOLN> FinishT4(CONST_REF(SPromiseFuncArg<T4>) roArg){ m_oData4 = roArg; m_dwFinishSign &= ~_PROMISE_DATA_SIGN_4; _TryFinishPromise(); return SPromiseData<BOOLN>(true); }
		SPromiseData<BOOLN> FinishT5(CONST_REF(SPromiseFuncArg<T5>) roArg){ m_oData5 = roArg; m_dwFinishSign &= ~_PROMISE_DATA_SIGN_5; _TryFinishPromise(); return SPromiseData<BOOLN>(true); }
		SPromiseData<BOOLN> FinishT6(CONST_REF(SPromiseFuncArg<T6>) roArg){ m_oData6 = roArg; m_dwFinishSign &= ~_PROMISE_DATA_SIGN_6; _TryFinishPromise(); return SPromiseData<BOOLN>(true); }

	private:
		void _TryFinishPromise()
		{
			if (m_dwFinishSign != 0)
				return;
			//DO
			_PROMISE_ALL_6_PARAMS_TYPE_ poPromise = (_PROMISE_ALL_6_PARAMS_TYPE_)CPromiseHelper::GetPromise(m_poPromiseMgr, m_qwPromiseUID);
            CONST_REF(auto) oTuple = MakeTuple(m_oData1, m_oData2, m_oData3, m_oData4, m_oData5, m_oData6);
			CPromiseHelper2::ResolvePromise(poPromise, oTuple);
#ifndef _USING_CLI
			delete this;
#endif
		}

	private:
		UINT64 m_qwPromiseUID = 0;
		PTR(CPromiseMgr) m_poPromiseMgr = NULL_PTR;
		//DO
		SPromiseFuncArg<T1> m_oData1;
		SPromiseFuncArg<T2> m_oData2;
		SPromiseFuncArg<T3> m_oData3;
		SPromiseFuncArg<T4> m_oData4;
		SPromiseFuncArg<T5> m_oData5;
		SPromiseFuncArg<T6> m_oData6;

		//DO
		UINT32 m_dwFinishSign = (_PROMISE_DATA_SIGN_1 | _PROMISE_DATA_SIGN_2 | _PROMISE_DATA_SIGN_3 | _PROMISE_DATA_SIGN_4 | _PROMISE_DATA_SIGN_5 |
			_PROMISE_DATA_SIGN_6);
	};

	QTEMPLATE<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
		QCLASS CPromiseAll7
	{
		//DO
#define _PROMISE_ALL_7_PARAMS_TYPE_ PTR(CPromise<STuple(SPromiseFuncArg<T1>, SPromiseFuncArg<T2>, SPromiseFuncArg<T3>, SPromiseFuncArg<T4>, SPromiseFuncArg<T5>, \
		SPromiseFuncArg<T6>, SPromiseFuncArg<T7>) >)
	public:
		//DO
		CPromiseAll7(_PROMISE_ALL_7_PARAMS_TYPE_ poPromise){ m_qwPromiseUID = poPromise->GetUID(); m_poPromiseMgr = poPromise->GetPromiseMgr(); }
		//DO
		SPromiseData<BOOLN> FinishT1(CONST_REF(SPromiseFuncArg<T1>) roArg){ m_oData1 = roArg; m_dwFinishSign &= ~_PROMISE_DATA_SIGN_1; _TryFinishPromise(); return SPromiseData<BOOLN>(true); }
		SPromiseData<BOOLN> FinishT2(CONST_REF(SPromiseFuncArg<T2>) roArg){ m_oData2 = roArg; m_dwFinishSign &= ~_PROMISE_DATA_SIGN_2; _TryFinishPromise(); return SPromiseData<BOOLN>(true); }
		SPromiseData<BOOLN> FinishT3(CONST_REF(SPromiseFuncArg<T3>) roArg){ m_oData3 = roArg; m_dwFinishSign &= ~_PROMISE_DATA_SIGN_3; _TryFinishPromise(); return SPromiseData<BOOLN>(true); }
		SPromiseData<BOOLN> FinishT4(CONST_REF(SPromiseFuncArg<T4>) roArg){ m_oData4 = roArg; m_dwFinishSign &= ~_PROMISE_DATA_SIGN_4; _TryFinishPromise(); return SPromiseData<BOOLN>(true); }
		SPromiseData<BOOLN> FinishT5(CONST_REF(SPromiseFuncArg<T5>) roArg){ m_oData5 = roArg; m_dwFinishSign &= ~_PROMISE_DATA_SIGN_5; _TryFinishPromise(); return SPromiseData<BOOLN>(true); }
		SPromiseData<BOOLN> FinishT6(CONST_REF(SPromiseFuncArg<T6>) roArg){ m_oData6 = roArg; m_dwFinishSign &= ~_PROMISE_DATA_SIGN_6; _TryFinishPromise(); return SPromiseData<BOOLN>(true); }
		SPromiseData<BOOLN> FinishT7(CONST_REF(SPromiseFuncArg<T7>) roArg){ m_oData7 = roArg; m_dwFinishSign &= ~_PROMISE_DATA_SIGN_7; _TryFinishPromise(); return SPromiseData<BOOLN>(true); }

	private:
		void _TryFinishPromise()
		{
			if (m_dwFinishSign != 0)
				return;
			//DO
			_PROMISE_ALL_7_PARAMS_TYPE_ poPromise = (_PROMISE_ALL_7_PARAMS_TYPE_)CPromiseHelper::GetPromise(m_poPromiseMgr, m_qwPromiseUID);
            CONST_REF(auto) oTuple = MakeTuple(m_oData1, m_oData2, m_oData3, m_oData4, m_oData5, m_oData6, m_oData7);
			CPromiseHelper2::ResolvePromise(poPromise, oTuple);
#ifndef _USING_CLI
			delete this;
#endif
		}

	private:
		UINT64 m_qwPromiseUID = 0;
		PTR(CPromiseMgr) m_poPromiseMgr = NULL_PTR;
		//DO
		SPromiseFuncArg<T1> m_oData1;
		SPromiseFuncArg<T2> m_oData2;
		SPromiseFuncArg<T3> m_oData3;
		SPromiseFuncArg<T4> m_oData4;
		SPromiseFuncArg<T5> m_oData5;
		SPromiseFuncArg<T6> m_oData6;
		SPromiseFuncArg<T7> m_oData7;

		//DO
		UINT32 m_dwFinishSign = (_PROMISE_DATA_SIGN_1 | _PROMISE_DATA_SIGN_2 | _PROMISE_DATA_SIGN_3 | _PROMISE_DATA_SIGN_4 | _PROMISE_DATA_SIGN_5 |
			_PROMISE_DATA_SIGN_6 | _PROMISE_DATA_SIGN_7);
	};

	QTEMPLATE<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
		QCLASS CPromiseAll8
	{
		//DO
#define _PROMISE_ALL_8_PARAMS_TYPE_ PTR(CPromise<STuple(SPromiseFuncArg<T1>, SPromiseFuncArg<T2>, SPromiseFuncArg<T3>, SPromiseFuncArg<T4>, SPromiseFuncArg<T5>, \
		SPromiseFuncArg<T6>, SPromiseFuncArg<T7>, SPromiseFuncArg<T8>) >)
	public:
		//DO
		CPromiseAll8(_PROMISE_ALL_8_PARAMS_TYPE_ poPromise){ m_qwPromiseUID = poPromise->GetUID(); m_poPromiseMgr = poPromise->GetPromiseMgr(); }
		//DO
		SPromiseData<BOOLN> FinishT1(CONST_REF(SPromiseFuncArg<T1>) roArg){ m_oData1 = roArg; m_dwFinishSign &= ~_PROMISE_DATA_SIGN_1; _TryFinishPromise(); return SPromiseData<BOOLN>(true); }
		SPromiseData<BOOLN> FinishT2(CONST_REF(SPromiseFuncArg<T2>) roArg){ m_oData2 = roArg; m_dwFinishSign &= ~_PROMISE_DATA_SIGN_2; _TryFinishPromise(); return SPromiseData<BOOLN>(true); }
		SPromiseData<BOOLN> FinishT3(CONST_REF(SPromiseFuncArg<T3>) roArg){ m_oData3 = roArg; m_dwFinishSign &= ~_PROMISE_DATA_SIGN_3; _TryFinishPromise(); return SPromiseData<BOOLN>(true); }
		SPromiseData<BOOLN> FinishT4(CONST_REF(SPromiseFuncArg<T4>) roArg){ m_oData4 = roArg; m_dwFinishSign &= ~_PROMISE_DATA_SIGN_4; _TryFinishPromise(); return SPromiseData<BOOLN>(true); }
		SPromiseData<BOOLN> FinishT5(CONST_REF(SPromiseFuncArg<T5>) roArg){ m_oData5 = roArg; m_dwFinishSign &= ~_PROMISE_DATA_SIGN_5; _TryFinishPromise(); return SPromiseData<BOOLN>(true); }
		SPromiseData<BOOLN> FinishT6(CONST_REF(SPromiseFuncArg<T6>) roArg){ m_oData6 = roArg; m_dwFinishSign &= ~_PROMISE_DATA_SIGN_6; _TryFinishPromise(); return SPromiseData<BOOLN>(true); }
		SPromiseData<BOOLN> FinishT7(CONST_REF(SPromiseFuncArg<T7>) roArg){ m_oData7 = roArg; m_dwFinishSign &= ~_PROMISE_DATA_SIGN_7; _TryFinishPromise(); return SPromiseData<BOOLN>(true); }
		SPromiseData<BOOLN> FinishT8(CONST_REF(SPromiseFuncArg<T8>) roArg){ m_oData8 = roArg; m_dwFinishSign &= ~_PROMISE_DATA_SIGN_8; _TryFinishPromise(); return SPromiseData<BOOLN>(true); }

	private:
		void _TryFinishPromise()
		{
			if (m_dwFinishSign != 0)
				return;
			_PROMISE_ALL_8_PARAMS_TYPE_ poPromise = (_PROMISE_ALL_8_PARAMS_TYPE_)CPromiseHelper::GetPromise(m_poPromiseMgr, m_qwPromiseUID);
            CONST_REF(auto) oTuple = MakeTuple(m_oData1, m_oData2, m_oData3, m_oData4, m_oData5, m_oData6, m_oData7, m_oData8);
			CPromiseHelper2::ResolvePromise(poPromise, oTuple);
#ifndef _USING_CLI
			delete this;
#endif
		}

	private:
		UINT64 m_qwPromiseUID = 0;
		PTR(CPromiseMgr) m_poPromiseMgr = NULL_PTR;
		//DO
		SPromiseFuncArg<T1> m_oData1;
		SPromiseFuncArg<T2> m_oData2;
		SPromiseFuncArg<T3> m_oData3;
		SPromiseFuncArg<T4> m_oData4;
		SPromiseFuncArg<T5> m_oData5;
		SPromiseFuncArg<T6> m_oData6;
		SPromiseFuncArg<T7> m_oData7;
		SPromiseFuncArg<T8> m_oData8;

		//DO
		UINT32 m_dwFinishSign = (_PROMISE_DATA_SIGN_1 | _PROMISE_DATA_SIGN_2 | _PROMISE_DATA_SIGN_3 | _PROMISE_DATA_SIGN_4 | _PROMISE_DATA_SIGN_5 |
			_PROMISE_DATA_SIGN_6 | _PROMISE_DATA_SIGN_7 | _PROMISE_DATA_SIGN_8);
	};
}

#endif
