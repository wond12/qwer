#ifndef H_C_SIMPLEX_DATA_PIPE_H
#define H_C_SIMPLEX_DATA_PIPE_H

#include "../CTypeDefine.h"
#include "../CQueueDefine.h"

namespace QWER
{
	//在生产者线程中执行
	QTEMPLATE<typename T>
	QDLL_EXPORT QCLASS CSimplexDataProducer
	{
	public:
		CSimplexDataProducer(PTR_OR_REF(CQueue<PTR(CQueue<T>)>) roQueueToConsumer, PTR_OR_REF(CQueue<PTR(CQueue<T>)>) roQueueToProducer)
		{
			m_poQueueToConsumer = TO_PTR2(roQueueToConsumer);
			m_poQueueToProducer = TO_PTR2(roQueueToProducer);
			m_poCurCache = TO_PTR(m_oCache2);
			m_poQueueToConsumer->Push(TO_PTR(m_oCache1));
		}

		void Push(CONST_VAL_OR_REF(T) roData)
		{
			m_poCurCache->Push(roData);
		}
		
		void Poll(UINT64 qwDeltaTickMilliSeconds)
		{
			m_qwTickCount += qwDeltaTickMilliSeconds;
			if (m_qwLastTickCountCalcCurCacheSize + 1000 <= m_qwTickCount) {
				++m_btNumber;
				m_dwSumOfCurCacheSize += m_poCurCache->GetCount();
				m_qwLastTickCountCalcCurCacheSize = m_qwTickCount;
			}
			if (m_poQueueToProducer->IsEmpty())
				return;
			PTR(CQueue<T>) poNextCache = m_poQueueToProducer->At(0);
			BOOLN bCalc = false;
			if (m_btNumber >= 10) {
				bCalc = true;
				UINT32 dwCurCacheSize = m_dwSumOfCurCacheSize / m_btNumber;
				UINT32 dwNextCacheCapacity = poNextCache->GetCapacity();
				if (dwCurCacheSize < dwNextCacheCapacity) {
					UINT32 dwExceptCapacity = dwCurCacheSize == 0 ? 1 : dwCurCacheSize;
					if ((dwExceptCapacity & (dwExceptCapacity - 1)) != 0) {
						dwExceptCapacity |= (dwExceptCapacity >> 1);
						dwExceptCapacity |= (dwExceptCapacity >> 2);
						dwExceptCapacity |= (dwExceptCapacity >> 4);
						dwExceptCapacity |= (dwExceptCapacity >> 8);
						dwExceptCapacity |= (dwExceptCapacity >> 16);
					}
					if (dwExceptCapacity < dwNextCacheCapacity) {
						CQueue<T> m_oSwapCache(dwExceptCapacity);
						poNextCache->Swap(TO_PTR1(m_oSwapCache));
					}
				}
				m_btNumber = 0;
				m_dwSumOfCurCacheSize = 0;
			}
			if (m_poCurCache->GetCount() == 0 && (bCalc == false || (m_poCurCache->GetCapacity() <= 1 && poNextCache->GetCapacity() <= 1)))
				return;
			m_poQueueToConsumer->Push(m_poCurCache);
			m_poCurCache = poNextCache;
			m_poQueueToProducer->Pop();
		}

	private:
		CQueue<T> m_oCache1;
		CQueue<T> m_oCache2;
		PTR(CQueue<T>) m_poCurCache = NULL_PTR;
		PTR(CQueue<PTR(CQueue<T>)>) m_poQueueToConsumer;
		PTR(CQueue<PTR(CQueue<T>)>) m_poQueueToProducer;
		UINT8 m_btNumber = 0;
		UINT32 m_dwSumOfCurCacheSize = 0;
		UINT64 m_qwLastTickCountCalcCurCacheSize = 0;
		UINT64 m_qwTickCount = 0;
	};

	//在消费者线程中执行
	QTEMPLATE<typename T>
	QDLL_EXPORT QCLASS CSimplexDataConsumer
	{
	public:
		CSimplexDataConsumer(PTR_OR_REF(CQueue<PTR(CQueue<T>)>) roQueueToConsumer, PTR_OR_REF(CQueue<PTR(CQueue<T>)>) roQueueToProducer)
		{
			m_poQueueToConsumer = TO_PTR2(roQueueToConsumer);
			m_poQueueToProducer = TO_PTR2(roQueueToProducer);
		}

#ifndef _USING_CLI
		CONST_VAL_OR_REF(T) Top() QCONST_FUNC
		{
#ifdef _USING_CLI
			PTR(CQueue<T>) poCurCache = _GetCurCache();
#else
			PTR(CQueue<T>) poCurCache = const_cast<PTR(CSimplexDataConsumer)>(this)->_GetCurCache();
#endif
			return poCurCache->At(0);
		}
#endif
		
		VAL_OR_REF(T) Top()
		{
			PTR(CQueue<T>) poCurCache = _GetCurCache();
			return poCurCache->At(0);
		}
		
		inline BOOLN IsEmpty() QCONST_FUNC
		{
#ifdef _USING_CLI
			PTR(CQueue<T>) poCurCache = _GetCurCache();
#else
			PTR(CQueue<T>) poCurCache = const_cast<PTR(CSimplexDataConsumer)>(this)->_GetCurCache();
#endif
			return poCurCache == NULL_PTR || poCurCache->IsEmpty();
		}
		
		void Pop()
		{
			PTR(CQueue<T>) poCurCache = _GetCurCache();
			poCurCache->Pop();
			if (poCurCache == m_poGotCacheFromProducer) {
				UINT32 dwCacheSize = m_poGotCacheFromProducer->GetCount();
				if (dwCacheSize != 0 && dwCacheSize <= 3) {
					while (!m_poGotCacheFromProducer->IsEmpty()) {
						m_oCache.Push(m_poGotCacheFromProducer->At(0));
						m_poGotCacheFromProducer->Pop();
					}
					m_poQueueToProducer->Push(m_poGotCacheFromProducer);
					m_poGotCacheFromProducer = NULL_PTR;
				}
			}
		}

	private:
		PTR(CQueue<T>) _GetCurCache()
		{
			if (!m_oCache.IsEmpty())
				return TO_PTR(m_oCache);
			if (m_poGotCacheFromProducer != NULL_PTR)
				return m_poGotCacheFromProducer;
			if (!m_poQueueToConsumer->IsEmpty()) {
				m_poGotCacheFromProducer = m_poQueueToConsumer->At(0);
				m_poQueueToConsumer->Pop();
				if (m_poGotCacheFromProducer->IsEmpty()) {
					m_poQueueToProducer->Push(m_poGotCacheFromProducer);
					m_poGotCacheFromProducer = NULL_PTR;
				}
			}
			return m_poGotCacheFromProducer;
		}

	private:
		CQueue<T> m_oCache;
		PTR(CQueue<PTR(CQueue<T>)>) m_poQueueToConsumer;
		PTR(CQueue<PTR(CQueue<T>)>) m_poQueueToProducer;
		PTR(CQueue<T>) m_poGotCacheFromProducer = NULL_PTR;
	};

	QTEMPLATE<typename T>
	QDLL_EXPORT QCLASS CSimplexDataPipe
	{
		NON_COPYABLE(CSimplexDataPipe)
	public:
		CSimplexDataPipe()
#ifndef _USING_CLI
			: m_oQueueToConsumer(2), m_oQueueToProducer(2)
			, m_oProducer(m_oQueueToConsumer, m_oQueueToProducer)
			, m_oConsumer(m_oQueueToConsumer, m_oQueueToProducer)
#endif
		{
#ifdef _USING_CLI
			m_oQueueToConsumer = QNEW CQueue<PTR(CQueue<T>)>(2);
			m_oQueueToProducer = QNEW CQueue<PTR(CQueue<T>)>(2);
			m_oProducer = QNEW CSimplexDataProducer<T>(m_oQueueToConsumer, m_oQueueToProducer);
			m_oConsumer = QNEW CSimplexDataConsumer<T>(m_oQueueToConsumer, m_oQueueToProducer);
#endif
		}

		//在生产者线程中执行
		inline PTR_OR_REF(CSimplexDataProducer<T>) GetProducer() { return m_oProducer; }

		//在消费者线程中执行
		inline PTR_OR_REF(CSimplexDataConsumer<T>) GetConsumer() { return m_oConsumer; }

	private:
		PTR_OR_VAL(CQueue<PTR(CQueue<T>)>) m_oQueueToConsumer;
		PTR_OR_VAL(CQueue<PTR(CQueue<T>)>) m_oQueueToProducer;
		PTR_OR_VAL(CSimplexDataProducer<T>) m_oProducer;
		PTR_OR_VAL(CSimplexDataConsumer<T>) m_oConsumer;
	};
}

#endif