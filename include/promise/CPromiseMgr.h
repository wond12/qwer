#ifndef _H_C_PROMISE_MGR_H_
#define _H_C_PROMISE_MGR_H_

#include "../CTypeDefine.h"
#include "../CObjectResMgrDefine.h"
#include "../CTimerMgrDefine.h"
#include "SPromise.h"
#include "SPromiseUID.h"
#include "CPromiseContainer.h"
#include "../TupleDefine.h"
#include "CPromiseHelper2.h"
#include "CPromiseAll.h"
#ifdef _USING_CLI
#include "core/CPromiseList.h"
#include "core/CPromiseListElement.h"
#endif

namespace QWER
{
	QCLASS CPromiseList;
	QCLASS CPromiseListElement;

	QCLASS CPromiseTimer;
	QTEMPLATE<typename T> QCLASS CPromise;

	QDLL_EXPORT QCLASS CPromiseMgr
	{
		NON_COPYABLE(CPromiseMgr)
	public:
		CPromiseMgr(PTR_OR_REF(CTimerMgr) poTimerMgr);
#ifndef _USING_CLI
		~CPromiseMgr();
#endif

		QTEMPLATE<typename T>
		SPromise<T> Resolve(CONST_REF(T) oData)
		{
			return SPromise<T>(oData, this);
		}

		QTEMPLATE<typename T>
		SPromise<T> Reject(EPromiseErrorType eErrorType)
		{
			return SPromise<T>(eErrorType, this);
		}

		QTEMPLATE<typename T>
		SPromise<T> GetOrCancelPromise(CONST_REF(SPromiseUID<T>) roUID)
		{
			SPromise<T> sRet(EPromiseErrorType::CancellationError, this);
			GetPromise(roUID, sRet);
			return sRet;
		}

		QTEMPLATE<typename T>
		BOOLN GetPromise(CONST_REF(SPromiseUID<T>) roUID, REF(SPromise<T>) rsRet)
		{
			if (roUID.IsNull())
				return false;
			QWERASSERT(roUID._GetOwner() == this, false);
			PTR(CPromise<T>) poPromise = _GetPromise<T>(roUID._GetUID());
			if (NULL_PTR == poPromise)
				return false;
			rsRet = SPromise<T>(TO_REF2(poPromise));
			return true;
		}


		QTEMPLATE<typename T>
		void ResolvePromise(CONST_REF(SPromiseUID<T>) roUID, CONST_REF(T) oData)
		{
			if (roUID.IsNull())
				return;
			QWERASSERT(roUID._GetOwner() == this, );
			PTR(CPromise<T>) poPromise = _GetPromise<T>(roUID._GetUID());
			CPromiseHelper2::ResolvePromise<T>(poPromise, oData);
		}

		QTEMPLATE<typename T>
		void RejectPromise(CONST_REF(SPromiseUID<T>) roUID, EPromiseErrorType eErrorType)
		{
			if (roUID.IsNull())
				return;
			QWERASSERT(roUID._GetOwner() == this, );
			PTR(CPromise<T>) poPromise = _GetPromise<T>(roUID._GetUID());
			CPromiseHelper2::RejectPromise<T>(poPromise, eErrorType);
		}

		QTEMPLATE<typename T>
		void CancelPromise(CONST_REF(SPromiseUID<T>) roUID)
		{
			return RejectPromise(roUID, EPromiseErrorType::CancellationError);
		}

		QTEMPLATE<typename T>
		SPromiseUID<T> CreatePromise()
		{
			PTR(CPromise<T>) poPromise = _CreatePromise<T>();
			return SPromiseUID<T>(poPromise->GetOwner()->GetUID(), this);
		}

		QTEMPLATE<typename T1, typename T2>
		SPromise<STuple(SPromiseFuncArg<T1>, SPromiseFuncArg<T2>) > 
		All(VAL_OR_REF(SPromise<T1>) sPromise1, VAL_OR_REF(SPromise<T2>) sPromise2)
		{
			PTR(CPromise<STuple(SPromiseFuncArg<T1>, SPromiseFuncArg<T2>) >) poRetPromise = 
				_CreatePromise<STuple(SPromiseFuncArg<T1>, SPromiseFuncArg<T2>) >();
			PTR(CPromiseAll2<QPACK(T1, T2)>) poAll = QNEW CPromiseAll2<T1, T2>(poRetPromise);
			sPromise1.Then(
#ifndef _USING_CLI
                (std::function<SPromiseData<BOOLN>(CONST_REF(SPromiseFuncArg<T1>))>)
#endif
                NEW_DELEGATE(QPACK(DPromiseResolver<BOOLN, T1>), poAll, CPromiseAll2<QPACK(T1, T2)>::FinishT1, DELEGATE_PLACEHOLDERS(, std::placeholders::_1))
            );
			sPromise2.Then(
#ifndef _USING_CLI
                (std::function<SPromiseData<BOOLN>(CONST_REF(SPromiseFuncArg<T2>))>)
#endif
                NEW_DELEGATE(QPACK(DPromiseResolver<BOOLN, T2>), poAll, CPromiseAll2<QPACK(T1, T2)>::FinishT2, DELEGATE_PLACEHOLDERS(, std::placeholders::_1))
            );
			return SPromise<STuple(SPromiseFuncArg<T1>, SPromiseFuncArg<T2>) >(TO_REF2(poRetPromise));
		}

		QTEMPLATE<typename T1, typename T2, typename T3>
			SPromise<STuple(SPromiseFuncArg<T1>, SPromiseFuncArg<T2>, SPromiseFuncArg<T3>) > 
			All(VAL_OR_REF(SPromise<T1>) sPromise1, VAL_OR_REF(SPromise<T2>) sPromise2, VAL_OR_REF(SPromise<T3>) sPromise3)
		{
			PTR(CPromise<STuple(SPromiseFuncArg<T1>, SPromiseFuncArg<T2>, SPromiseFuncArg<T3>) >) poRetPromise =
				_CreatePromise<STuple(SPromiseFuncArg<T1>, SPromiseFuncArg<T2>, SPromiseFuncArg<T3>) >();
			PTR(CPromiseAll3<QPACK(T1, T2, T3)>) poAll = QNEW CPromiseAll3<T1, T2, T3>(poRetPromise);
			sPromise1.Then(
#ifndef _USING_CLI
                (std::function<SPromiseData<BOOLN>(CONST_REF(SPromiseFuncArg<T1>))>)
#endif
                NEW_DELEGATE(QPACK(DPromiseResolver<BOOLN, T1>), poAll, CPromiseAll3<QPACK(T1, T2, T3)>::FinishT1, DELEGATE_PLACEHOLDERS(, std::placeholders::_1))
            );
			sPromise2.Then(
#ifndef _USING_CLI
                (std::function<SPromiseData<BOOLN>(CONST_REF(SPromiseFuncArg<T2>))>)
#endif
                NEW_DELEGATE(QPACK(DPromiseResolver<BOOLN, T2>), poAll, CPromiseAll3<QPACK(T1, T2, T3)>::FinishT2, DELEGATE_PLACEHOLDERS(, std::placeholders::_1))
            );
			sPromise3.Then(
#ifndef _USING_CLI
                (std::function<SPromiseData<BOOLN>(CONST_REF(SPromiseFuncArg<T3>))>)
#endif
                NEW_DELEGATE(QPACK(DPromiseResolver<BOOLN, T3>), poAll, CPromiseAll3<QPACK(T1, T2, T3)>::FinishT3, DELEGATE_PLACEHOLDERS(, std::placeholders::_1))
            );
			return SPromise<STuple(SPromiseFuncArg<T1>, SPromiseFuncArg<T2>, SPromiseFuncArg<T3>) >(TO_REF2(poRetPromise));
		}

		QTEMPLATE<typename T1, typename T2, typename T3, typename T4>
			SPromise<STuple(SPromiseFuncArg<T1>, SPromiseFuncArg<T2>, SPromiseFuncArg<T3>, SPromiseFuncArg<T4>) >
			All(VAL_OR_REF(SPromise<T1>) sPromise1, VAL_OR_REF(SPromise<T2>) sPromise2, VAL_OR_REF(SPromise<T3>) sPromise3, VAL_OR_REF(SPromise<T4>) sPromise4)
		{
			PTR(CPromise<STuple(SPromiseFuncArg<T1>, SPromiseFuncArg<T2>, SPromiseFuncArg<T3>, SPromiseFuncArg<T4>) >) poRetPromise =
				_CreatePromise<STuple(SPromiseFuncArg<T1>, SPromiseFuncArg<T2>, SPromiseFuncArg<T3>, SPromiseFuncArg<T4>) >();
			PTR(CPromiseAll4<QPACK(T1, T2, T3, T4)>) poAll = QNEW CPromiseAll4<T1, T2, T3, T4>(poRetPromise);
			sPromise1.Then(
#ifndef _USING_CLI
                (std::function<SPromiseData<BOOLN>(CONST_REF(SPromiseFuncArg<T1>))>)
#endif
                NEW_DELEGATE(QPACK(DPromiseResolver<BOOLN, T1>), poAll, CPromiseAll4<QPACK(T1, T2, T3, T4)>::FinishT1, DELEGATE_PLACEHOLDERS(, std::placeholders::_1))
            );
			sPromise2.Then(
#ifndef _USING_CLI
                (std::function<SPromiseData<BOOLN>(CONST_REF(SPromiseFuncArg<T2>))>)
#endif
                NEW_DELEGATE(QPACK(DPromiseResolver<BOOLN, T2>), poAll, CPromiseAll4<QPACK(T1, T2, T3, T4)>::FinishT2, DELEGATE_PLACEHOLDERS(, std::placeholders::_1))
            );
			sPromise3.Then(
#ifndef _USING_CLI
                (std::function<SPromiseData<BOOLN>(CONST_REF(SPromiseFuncArg<T3>))>)
#endif
                NEW_DELEGATE(QPACK(DPromiseResolver<BOOLN, T3>), poAll, CPromiseAll4<QPACK(T1, T2, T3, T4)>::FinishT3, DELEGATE_PLACEHOLDERS(, std::placeholders::_1))
            );
			sPromise4.Then(
#ifndef _USING_CLI
                (std::function<SPromiseData<BOOLN>(CONST_REF(SPromiseFuncArg<T4>))>)
#endif
                NEW_DELEGATE(QPACK(DPromiseResolver<BOOLN, T4>), poAll, CPromiseAll4<QPACK(T1, T2, T3, T4)>::FinishT4, DELEGATE_PLACEHOLDERS(, std::placeholders::_1))
            );
			return SPromise<STuple(SPromiseFuncArg<T1>, SPromiseFuncArg<T2>, SPromiseFuncArg<T3>, SPromiseFuncArg<T4>) >(TO_REF2(poRetPromise));
		}

		QTEMPLATE<typename T1, typename T2, typename T3, typename T4, typename T5>
			SPromise<STuple(SPromiseFuncArg<T1>, SPromiseFuncArg<T2>, SPromiseFuncArg<T3>, SPromiseFuncArg<T4>, SPromiseFuncArg<T5>) >
			All(VAL_OR_REF(SPromise<T1>) sPromise1, VAL_OR_REF(SPromise<T2>) sPromise2, VAL_OR_REF(SPromise<T3>) sPromise3, VAL_OR_REF(SPromise<T4>) sPromise4, VAL_OR_REF(SPromise<T5>) sPromise5)
		{
			PTR(CPromise<STuple(SPromiseFuncArg<T1>, SPromiseFuncArg<T2>, SPromiseFuncArg<T3>, SPromiseFuncArg<T4>, SPromiseFuncArg<T5>) >) poRetPromise =
				_CreatePromise<STuple(SPromiseFuncArg<T1>, SPromiseFuncArg<T2>, SPromiseFuncArg<T3>, SPromiseFuncArg<T4>, SPromiseFuncArg<T5>) >();
			PTR(CPromiseAll5<QPACK(T1, T2, T3, T4, T5)>) poAll = QNEW CPromiseAll5<T1, T2, T3, T4, T5>(poRetPromise);
			sPromise1.Then(NEW_DELEGATE(QPACK(DPromiseResolver<BOOLN, T1>), poAll, CPromiseAll5<QPACK(T1, T2, T3, T4, T5)>::FinishT1, DELEGATE_PLACEHOLDERS(, std::placeholders::_1)));
			sPromise2.Then(NEW_DELEGATE(QPACK(DPromiseResolver<BOOLN, T2>), poAll, CPromiseAll5<QPACK(T1, T2, T3, T4, T5)>::FinishT2, DELEGATE_PLACEHOLDERS(, std::placeholders::_1)));
			sPromise3.Then(NEW_DELEGATE(QPACK(DPromiseResolver<BOOLN, T3>), poAll, CPromiseAll5<QPACK(T1, T2, T3, T4, T5)>::FinishT3, DELEGATE_PLACEHOLDERS(, std::placeholders::_1)));
			sPromise4.Then(NEW_DELEGATE(QPACK(DPromiseResolver<BOOLN, T4>), poAll, CPromiseAll5<QPACK(T1, T2, T3, T4, T5)>::FinishT4, DELEGATE_PLACEHOLDERS(, std::placeholders::_1)));
			sPromise5.Then(NEW_DELEGATE(QPACK(DPromiseResolver<BOOLN, T5>), poAll, CPromiseAll5<QPACK(T1, T2, T3, T4, T5)>::FinishT5, DELEGATE_PLACEHOLDERS(, std::placeholders::_1)));
			return SPromise<STuple(SPromiseFuncArg<T1>, SPromiseFuncArg<T2>, SPromiseFuncArg<T3>, SPromiseFuncArg<T4>, SPromiseFuncArg<T5>) >(TO_REF2(poRetPromise));
		}

		QTEMPLATE<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
			SPromise<STuple(SPromiseFuncArg<T1>, SPromiseFuncArg<T2>, SPromiseFuncArg<T3>, SPromiseFuncArg<T4>, SPromiseFuncArg<T5>, SPromiseFuncArg<T6>) >
			All(VAL_OR_REF(SPromise<T1>) sPromise1, VAL_OR_REF(SPromise<T2>) sPromise2, VAL_OR_REF(SPromise<T3>) sPromise3, VAL_OR_REF(SPromise<T4>) sPromise4, VAL_OR_REF(SPromise<T5>) sPromise5, VAL_OR_REF(SPromise<T6>) sPromise6)
		{
			PTR(CPromise<STuple(SPromiseFuncArg<T1>, SPromiseFuncArg<T2>, SPromiseFuncArg<T3>, SPromiseFuncArg<T4>, SPromiseFuncArg<T5>, SPromiseFuncArg<T6>) >) poRetPromise =
				_CreatePromise<STuple(SPromiseFuncArg<T1>, SPromiseFuncArg<T2>, SPromiseFuncArg<T3>, SPromiseFuncArg<T4>, SPromiseFuncArg<T5>, SPromiseFuncArg<T6>) >();
			PTR(CPromiseAll6<QPACK(T1, T2, T3, T4, T5, T6)>) poAll = QNEW CPromiseAll6<T1, T2, T3, T4, T5, T6>(poRetPromise);
			sPromise1.Then(NEW_DELEGATE(QPACK(DPromiseResolver<BOOLN, T1>), poAll, CPromiseAll6<QPACK(T1, T2, T3, T4, T5, T6)>::FinishT1, DELEGATE_PLACEHOLDERS(, std::placeholders::_1)));
			sPromise2.Then(NEW_DELEGATE(QPACK(DPromiseResolver<BOOLN, T2>), poAll, CPromiseAll6<QPACK(T1, T2, T3, T4, T5, T6)>::FinishT2, DELEGATE_PLACEHOLDERS(, std::placeholders::_1)));
			sPromise3.Then(NEW_DELEGATE(QPACK(DPromiseResolver<BOOLN, T3>), poAll, CPromiseAll6<QPACK(T1, T2, T3, T4, T5, T6)>::FinishT3, DELEGATE_PLACEHOLDERS(, std::placeholders::_1)));
			sPromise4.Then(NEW_DELEGATE(QPACK(DPromiseResolver<BOOLN, T4>), poAll, CPromiseAll6<QPACK(T1, T2, T3, T4, T5, T6)>::FinishT4, DELEGATE_PLACEHOLDERS(, std::placeholders::_1)));
			sPromise5.Then(NEW_DELEGATE(QPACK(DPromiseResolver<BOOLN, T5>), poAll, CPromiseAll6<QPACK(T1, T2, T3, T4, T5, T6)>::FinishT5, DELEGATE_PLACEHOLDERS(, std::placeholders::_1)));
			sPromise6.Then(NEW_DELEGATE(QPACK(DPromiseResolver<BOOLN, T6>), poAll, CPromiseAll6<QPACK(T1, T2, T3, T4, T5, T6)>::FinishT6, DELEGATE_PLACEHOLDERS(, std::placeholders::_1)));
			return SPromise<STuple(SPromiseFuncArg<T1>, SPromiseFuncArg<T2>, SPromiseFuncArg<T3>, SPromiseFuncArg<T4>, SPromiseFuncArg<T5>, SPromiseFuncArg<T6>) >(TO_REF2(poRetPromise));
		}

		QTEMPLATE<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
			SPromise<STuple(SPromiseFuncArg<T1>, SPromiseFuncArg<T2>, SPromiseFuncArg<T3>, SPromiseFuncArg<T4>, SPromiseFuncArg<T5>, SPromiseFuncArg<T6>, SPromiseFuncArg<T7>) >
			All(VAL_OR_REF(SPromise<T1>) sPromise1, VAL_OR_REF(SPromise<T2>) sPromise2, VAL_OR_REF(SPromise<T3>) sPromise3, VAL_OR_REF(SPromise<T4>) sPromise4, VAL_OR_REF(SPromise<T5>) sPromise5, VAL_OR_REF(SPromise<T6>) sPromise6, VAL_OR_REF(SPromise<T7>) sPromise7)
		{
			PTR(CPromise<STuple(SPromiseFuncArg<T1>, SPromiseFuncArg<T2>, SPromiseFuncArg<T3>, SPromiseFuncArg<T4>, SPromiseFuncArg<T5>, SPromiseFuncArg<T6>, SPromiseFuncArg<T7>) >) poRetPromise =
				_CreatePromise<STuple(SPromiseFuncArg<T1>, SPromiseFuncArg<T2>, SPromiseFuncArg<T3>, SPromiseFuncArg<T4>, SPromiseFuncArg<T5>, SPromiseFuncArg<T6>, SPromiseFuncArg<T7>) >();
			PTR(CPromiseAll7<QPACK(T1, T2, T3, T4, T5, T6, T7)>) poAll = QNEW CPromiseAll7<T1, T2, T3, T4, T5, T6, T7>(poRetPromise);
			sPromise1.Then(NEW_DELEGATE(QPACK(DPromiseResolver<BOOLN, T1>), poAll, CPromiseAll7<QPACK(T1, T2, T3, T4, T5, T6, T7)>::FinishT1, DELEGATE_PLACEHOLDERS(, std::placeholders::_1)));
			sPromise2.Then(NEW_DELEGATE(QPACK(DPromiseResolver<BOOLN, T2>), poAll, CPromiseAll7<QPACK(T1, T2, T3, T4, T5, T6, T7)>::FinishT2, DELEGATE_PLACEHOLDERS(, std::placeholders::_1)));
			sPromise3.Then(NEW_DELEGATE(QPACK(DPromiseResolver<BOOLN, T3>), poAll, CPromiseAll7<QPACK(T1, T2, T3, T4, T5, T6, T7)>::FinishT3, DELEGATE_PLACEHOLDERS(, std::placeholders::_1)));
			sPromise4.Then(NEW_DELEGATE(QPACK(DPromiseResolver<BOOLN, T4>), poAll, CPromiseAll7<QPACK(T1, T2, T3, T4, T5, T6, T7)>::FinishT4, DELEGATE_PLACEHOLDERS(, std::placeholders::_1)));
			sPromise5.Then(NEW_DELEGATE(QPACK(DPromiseResolver<BOOLN, T5>), poAll, CPromiseAll7<QPACK(T1, T2, T3, T4, T5, T6, T7)>::FinishT5, DELEGATE_PLACEHOLDERS(, std::placeholders::_1)));
			sPromise6.Then(NEW_DELEGATE(QPACK(DPromiseResolver<BOOLN, T6>), poAll, CPromiseAll7<QPACK(T1, T2, T3, T4, T5, T6, T7)>::FinishT6, DELEGATE_PLACEHOLDERS(, std::placeholders::_1)));
			sPromise7.Then(NEW_DELEGATE(QPACK(DPromiseResolver<BOOLN, T7>), poAll, CPromiseAll7<QPACK(T1, T2, T3, T4, T5, T6, T7)>::FinishT7, DELEGATE_PLACEHOLDERS(, std::placeholders::_1)));
			return SPromise<STuple(SPromiseFuncArg<T1>, SPromiseFuncArg<T2>, SPromiseFuncArg<T3>, SPromiseFuncArg<T4>, SPromiseFuncArg<T5>, SPromiseFuncArg<T6>, SPromiseFuncArg<T7>) >(TO_REF2(poRetPromise));
		}

		QTEMPLATE<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
			SPromise<STuple(SPromiseFuncArg<T1>, SPromiseFuncArg<T2>, SPromiseFuncArg<T3>, SPromiseFuncArg<T4>, SPromiseFuncArg<T5>, SPromiseFuncArg<T6>, SPromiseFuncArg<T7>, SPromiseFuncArg<T8>) >
			All(VAL_OR_REF(SPromise<T1>) sPromise1, VAL_OR_REF(SPromise<T2>) sPromise2, VAL_OR_REF(SPromise<T3>) sPromise3, VAL_OR_REF(SPromise<T4>) sPromise4, VAL_OR_REF(SPromise<T5>) sPromise5, VAL_OR_REF(SPromise<T6>) sPromise6, VAL_OR_REF(SPromise<T7>) sPromise7, VAL_OR_REF(SPromise<T8>) sPromise8)
		{
			PTR(CPromise<STuple(SPromiseFuncArg<T1>, SPromiseFuncArg<T2>, SPromiseFuncArg<T3>, SPromiseFuncArg<T4>, SPromiseFuncArg<T5>, SPromiseFuncArg<T6>, SPromiseFuncArg<T7>, SPromiseFuncArg<T8>) >) poRetPromise =
				_CreatePromise<STuple(SPromiseFuncArg<T1>, SPromiseFuncArg<T2>, SPromiseFuncArg<T3>, SPromiseFuncArg<T4>, SPromiseFuncArg<T5>, SPromiseFuncArg<T6>, SPromiseFuncArg<T7>, SPromiseFuncArg<T8>) >();
			PTR(CPromiseAll8<QPACK(T1, T2, T3, T4, T5, T6, T7, T8)>) poAll = QNEW CPromiseAll8<T1, T2, T3, T4, T5, T6, T7, T8>(poRetPromise);
			sPromise1.Then(NEW_DELEGATE(QPACK(DPromiseResolver<BOOLN, T1>), poAll, CPromiseAll8<QPACK(T1, T2, T3, T4, T5, T6, T7, T8)>::FinishT1, DELEGATE_PLACEHOLDERS(, std::placeholders::_1)));
			sPromise2.Then(NEW_DELEGATE(QPACK(DPromiseResolver<BOOLN, T2>), poAll, CPromiseAll8<QPACK(T1, T2, T3, T4, T5, T6, T7, T8)>::FinishT2, DELEGATE_PLACEHOLDERS(, std::placeholders::_1)));
			sPromise3.Then(NEW_DELEGATE(QPACK(DPromiseResolver<BOOLN, T3>), poAll, CPromiseAll8<QPACK(T1, T2, T3, T4, T5, T6, T7, T8)>::FinishT3, DELEGATE_PLACEHOLDERS(, std::placeholders::_1)));
			sPromise4.Then(NEW_DELEGATE(QPACK(DPromiseResolver<BOOLN, T4>), poAll, CPromiseAll8<QPACK(T1, T2, T3, T4, T5, T6, T7, T8)>::FinishT4, DELEGATE_PLACEHOLDERS(, std::placeholders::_1)));
			sPromise5.Then(NEW_DELEGATE(QPACK(DPromiseResolver<BOOLN, T5>), poAll, CPromiseAll8<QPACK(T1, T2, T3, T4, T5, T6, T7, T8)>::FinishT5, DELEGATE_PLACEHOLDERS(, std::placeholders::_1)));
			sPromise6.Then(NEW_DELEGATE(QPACK(DPromiseResolver<BOOLN, T6>), poAll, CPromiseAll8<QPACK(T1, T2, T3, T4, T5, T6, T7, T8)>::FinishT6, DELEGATE_PLACEHOLDERS(, std::placeholders::_1)));
			sPromise7.Then(NEW_DELEGATE(QPACK(DPromiseResolver<BOOLN, T7>), poAll, CPromiseAll8<QPACK(T1, T2, T3, T4, T5, T6, T7, T8)>::FinishT7, DELEGATE_PLACEHOLDERS(, std::placeholders::_1)));
			sPromise8.Then(NEW_DELEGATE(QPACK(DPromiseResolver<BOOLN, T8>), poAll, CPromiseAll8<QPACK(T1, T2, T3, T4, T5, T6, T7, T8)>::FinishT8, DELEGATE_PLACEHOLDERS(, std::placeholders::_1)));
			return SPromise<STuple(SPromiseFuncArg<T1>, SPromiseFuncArg<T2>, SPromiseFuncArg<T3>, SPromiseFuncArg<T4>, SPromiseFuncArg<T5>, SPromiseFuncArg<T6>, SPromiseFuncArg<T7>, SPromiseFuncArg<T8>) >(TO_REF2(poRetPromise));
		}

		QTEMPLATE<typename T>
			SPromise<PTR_OR_VAL(CVector(SPromiseFuncArg<T>)) > All(PTR_OR_REF(CVector(SPromise<T>)) roPromises)
		{
			PTR(CVector(SPromise<T>)) poPromises = TO_PTR2(roPromises);
			if (poPromises == NULL_PTR || poPromises->size() == 0)
				return Resolve<PTR_OR_VAL(CVector(SPromiseFuncArg<T>))>(
#ifdef _USING_CLI
				NULL_PTR
#else
				CVector(SPromiseFuncArg<T>)()
#endif
				);
			PTR(CPromise<PTR_OR_VAL(CVector(SPromiseFuncArg<T>))>) poRetPromise = _CreatePromise<PTR_OR_VAL(CVector(SPromiseFuncArg<T>))>();
			int dwSize = poPromises->size();
			PTR(CPromiseAll<T>) poAll = QNEW CPromiseAll<T>(poRetPromise, dwSize);
			for (int i = 0; i != dwSize; ++i)
			{
				typename PTR(CPromiseAll<T>::CPromiseAllExec) poExec = poAll->GetExec((UINT32)i);
                VAL_OR_REF(SPromise<T>) pr = poPromises->at(i);
#ifdef _USING_CLI
				pr.Then(NEW_DELEGATE(QPACK(DPromiseResolver<BOOLN, T>), poExec, CPromiseAll<T>::CPromiseAllExec::FinishT, DELEGATE_PLACEHOLDERS(, std::placeholders::_1)));
#else
                std::function<SPromiseData<BOOLN>(CONST_REF(SPromiseFuncArg<T>))> cb = NEW_DELEGATE(QPACK(DPromiseResolver<BOOLN, T>), poExec, CPromiseAll<T>::CPromiseAllExec::FinishT, DELEGATE_PLACEHOLDERS(, std::placeholders::_1));
                pr.Then(cb);
#endif
			}
			return SPromise<PTR_OR_VAL(CVector(SPromiseFuncArg<T>)) >(TO_REF2(poRetPromise));
		}

		UINT32 Poll();
		SPromise<BOOLN> Delay(UINT64 qwMilliSeconds);
		QTEMPLATE<typename T>
		SPromise<T> Timeout(CONST_REF(SPromise<T>) rsPromise, UINT64 qwMilliSeconds)
		{
			SPromiseUID<T> sPromiseUID = CreatePromise<T>();
			PTR(CPromise<T>) poPromise = _GetPromise<T>(sPromiseUID._GetUID());
			poPromise->__SetTimeout(qwMilliSeconds);
			rsPromise.Then<BOOLN>(
				CONVERT_DELEGATE(
					QPACK(DPromiseResolver<BOOLN, T>),
					PACK_DELEGATE_FUNC(
						QPACK(qwer_stl::function<SPromiseData<BOOLN>, SPromiseFuncArg<T>, SPromiseUID<T>>),
						this, CPromiseMgr::_OnBeforePromiseTimeout
					),
					DELEGATE_PLACEHOLDERS(, std::placeholders::_1),
					sPromiseUID
				)
			);
			return SPromise<T>(TO_REF2(poPromise));
		}
		inline PTR(CTimerMgr) GetTimerMgr() { return m_poTimerMgr; }
		inline UINT32 GetMaxCountPerPoll() QCONST_FUNC { return m_dwMaxCountPerPoll; }
		inline void SetMaxCountPerPoll(UINT32 dwMaxCountPerPoll) { m_dwMaxCountPerPoll = dwMaxCountPerPoll; }

	private:
		QTEMPLATE<typename T>
		SPromiseData<BOOLN> _OnBeforePromiseTimeout(CONST_REF(SPromiseFuncArg<T>) rsArg, CONST_REF(SPromiseUID<T>) rsTimeoutPromiseUID) {
			if (rsArg.m_ePromiseError != EPromiseErrorType::Success)
				RejectPromise(rsTimeoutPromiseUID, rsArg.m_ePromiseError);
			else
				ResolvePromise(rsTimeoutPromiseUID, rsArg.m_oData);
			return SPromiseData<BOOLN>(true);
		}

		QTEMPLATE<typename T>
		PTR(CPromise<T>) _GetPromise(UINT64 qwPromiseUID)
		{
			PTR(CPromiseContainer) p = m_poPromiseMgr->GetObj(qwPromiseUID);
			if (p == NULL_PTR) return NULL_PTR;
			return (PTR(CPromise<T>))p->GetPromise();
		}
		QTEMPLATE<typename T>
		PTR(CPromise<T>) _CreatePromise()
		{
            PTR_OR_VAL(QPACK(DPromiseResolver<T, T>)) onResolve = NEW_STATIC_DELEGATE(QPACK(DPromiseResolver<T, T>), CPromise<T>::_OnPostResolve, DELEGATE_PLACEHOLDERS(, std::placeholders::_1));
			PTR(CPromise<T>) poPromise = CPromise<T>::_CreatePromise1(this, onResolve);
			CPromiseHelper::AddToPromiseList(poPromise);
			return poPromise;
		}

		CUniquePtr(CPromiseContainer) _NewPromise(UINT16 wObjType);
		CUniquePtr(CPromiseList) _NewPromiseList(UINT16 wObjType);
		CUniquePtr(CPromiseListElement) _NewPromiseListElement(UINT16 wObjType);

	private:
		//const UINT32 m_dwTimeoutMilliSec = 30000000;
		CQueue<UINT64> m_oToExecPromiseList;
		//CQueue<SPendingPromise> m_oPendingPromiseList;
		PTR(CObjectResMgr<PTR_OR_VAL(CPromiseContainer)>) m_poPromiseMgr = NULL_PTR;
		PTR(CObjectResMgr<PTR_OR_VAL(CPromiseList)>) m_poPromiseListMgr = NULL_PTR;
		PTR(CObjectResMgr<PTR_OR_VAL(CPromiseListElement)>) m_poPromiseListElementMgr = NULL_PTR;
		PTR(CTimerMgr) m_poTimerMgr = NULL_PTR;
		UINT32 m_dwMaxCountPerPoll = 100;

#ifdef _USING_CLI
	internal:
#else
	private:
		template<typename T> friend class CPromise;
		friend class CPromiseTimer;
		friend class CPromiseHelper;
		friend class CPromiseList;
		friend class CPromiseListElement;
#endif
		void _AddToExecList(PTR(CPromiseList) poPromiseList);
		PTR(CPromiseList) _CreatePromiseList(PTR(CPromiseContainer) poPromiseContainer);
		void _ReleasePromiseList(UINT64 qwPromiseListUID);
		PTR(CPromiseListElement) _CreatePromiseListElement(PTR(CPromiseList) poOwner, PTR(CPromiseContainer) poPromise);
		void _ReleasePromiseListElement(UINT64 qwPromiseListElementUID);
		PTR(CPromiseContainer) _CreatePromiseContainer(PTR(IPromise) roPromise);
		PTR(CPromiseContainer) _GetPromiseContainer(UINT64 qwPromiseUID);
		void _ReleasePromiseContainer(UINT64 qwPromiseUID);
		PTR(CPromise<BOOLN>) _Delay(UINT64 qwMilliSeconds);
		PTR(IPromise) __GetPromise(UINT64 qwPromiseUID);
	};
}

#endif
