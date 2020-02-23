#ifndef H_C_QUEUE_H
#define H_C_QUEUE_H

#include "../CTypeDefine.h"
#ifdef _USING_CLI
#else
#include <stdexcept>
#include <string.h>
#endif
#include "../PairDefine.h"

namespace QWER
{
#ifdef _USING_CLI
	using System::ArgumentOutOfRangeException;
#endif

#ifdef _USING_CLI
	generic<typename T>
#else
	template<typename T>
#endif
	QDLL_EXPORT QCLASS CQueue
	{
	public:
		CQueue()
		{
#ifdef _USING_CLI
			m_oList = _CreateList(2);
#else
			m_dwCapacity = 2;
			m_oList = _CreateList(m_dwCapacity);
#endif
			m_dwBegin = 0;
			m_dwEnd = 0;
		}

		CQueue(UINT32 dwCapacity)
		{
			dwCapacity = dwCapacity == 0 ? 1 : 0;
#ifdef _USING_CLI
			m_oList = _CreateList(dwCapacity + 1);
#else
			m_dwCapacity = dwCapacity + 1;
			m_oList = _CreateList(m_dwCapacity);
#endif
			m_dwBegin = 0;
			m_dwEnd = 0;
		}

#ifndef _USING_CLI
		~CQueue()
		{
			_DestoryList(m_oList);
			m_oList = NULL_PTR;
		}
#endif

#ifdef _USING_CLI
		property T default[UINT32]
		{
			T get(UINT32 index)
			{
				return At(index);
			}
			void set(UINT32 index, T value)
			{
				if(index >= GetCount())
					throw QNEW ArgumentOutOfRangeException();
				m_oList[(m_dwBegin + index) % m_oList->Length] = value;
			}
		}
#else
		CONST_REF(T) operator[](UINT32 dwIndex) QCONST_FUNC
		{
			return At(dwIndex);
		}

		REF(T) operator[](UINT32 dwIndex)
		{
			return At(dwIndex);
		}

		REF(T) At(UINT32 dwIndex)
		{
			if (dwIndex >= GetCount())
				throw std::out_of_range("");
			return m_oList[(m_dwBegin + dwIndex) % m_dwCapacity];
		}
#endif

		CONST_REF(T) At(UINT32 dwIndex) QCONST_FUNC
		{
#ifdef _USING_CLI
			if (dwIndex >= GetCount())
				throw QNEW ArgumentOutOfRangeException();
			return m_oList[(m_dwBegin + dwIndex) % m_oList->Length];
#else
			if(dwIndex >= GetCount())
				throw std::out_of_range("");
			return m_oList[(m_dwBegin + dwIndex) % m_dwCapacity];
#endif
		}

		void Clear()
		{
#ifdef _USING_CLI
			while (!IsEmpty())
			{
				Pop();
			}
#else
			m_dwEnd = m_dwBegin;
#endif
		}

		void Pop()
		{
			if (m_dwBegin == m_dwEnd)
#ifdef _USING_CLI
				throw QNEW ArgumentOutOfRangeException();
#else
				throw std::out_of_range("");
#endif
			//T ret = m_oList[m_dwBegin];
#ifdef _USING_CLI
			m_oList[m_dwBegin] = T();
#endif
#ifdef _USING_CLI
			m_dwBegin = (++m_dwBegin) % m_oList->Length;
#else
			m_dwBegin = (m_dwBegin + 1) % m_dwCapacity;
#endif
			//return ret;
		}
		
#ifdef _USING_CLI
		void Push(T item)
#else
		void Push(const T& item)
#endif
		{
#ifdef _USING_CLI
			if ((m_dwEnd + 1) % m_oList->Length == m_dwBegin)
#else
			if ((m_dwEnd + 1) % m_dwCapacity == m_dwBegin)
#endif
			{
				UINT32 dwCount = GetCount();
#ifdef _USING_CLI
				PTR(array<T>) oTmp = m_oList;
				m_oList = _CreateList(oTmp->Length * 2);
				int dwEnd = m_dwEnd;
				if (m_dwEnd < m_dwBegin)
					dwEnd = oTmp->Length;
				array<T>::Copy(oTmp, m_dwBegin, m_oList, 0, dwEnd - m_dwBegin);
				if (m_dwEnd < m_dwBegin)
				{
					array<T>::Copy(oTmp, 0, m_oList, dwEnd - m_dwBegin, m_dwEnd);
				}
#else
				PTR(T) oTmp = m_oList;
				m_dwCapacity *= 2;
				m_oList = _CreateList(m_dwCapacity);
				int dwEnd = m_dwEnd;
				if (m_dwEnd < m_dwBegin)
					dwEnd = m_dwCapacity / 2;
				memcpy(m_oList, oTmp + m_dwBegin, (dwEnd - m_dwBegin) * sizeof(T));
				if (m_dwEnd < m_dwBegin)
				{
					memcpy(m_oList + dwEnd - m_dwBegin, oTmp, m_dwEnd * sizeof(T));
				}
				_DestoryList(oTmp);
				oTmp = NULL_PTR;
#endif
				m_dwBegin = 0;
				m_dwEnd = dwCount;
			}
			m_oList[m_dwEnd] = item;
#ifdef _USING_CLI
			m_dwEnd = (++m_dwEnd) % m_oList->Length;
#else
			m_dwEnd = (m_dwEnd + 1) % m_dwCapacity;
#endif
		}

		void TransmitUntilFull(PTR_OR_REF(CQueue<T>) roDst)
		{
			if (IsEmpty()) return;
			PTR(CQueue<T>) poDst = TO_PTR2(roDst);
			UINT32 dwCount = _CopyQueue(m_oList, m_dwBegin, m_dwEnd
#ifndef _USING_CLI
				, m_dwCapacity
#endif
				, poDst->m_oList, poDst->m_dwBegin, poDst->m_dwEnd
#ifndef _USING_CLI
				, poDst->m_dwCapacity
#endif
				);
#ifdef _USING_CLI
			m_dwBegin = (m_dwBegin + dwCount) % m_oList->Length;
			poDst->m_dwEnd = (poDst->m_dwEnd + dwCount) % m_oList->Length;
#else
			m_dwBegin = (m_dwBegin + dwCount) % m_dwCapacity;
			poDst->m_dwEnd = (poDst->m_dwEnd + dwCount) % poDst->m_dwCapacity;
#endif
		}

		void Swap(PTR_OR_REF(CQueue<T>) roDst)
		{
			PTR(CQueue<T>) poDst = TO_PTR2(roDst);
#ifdef _USING_CLI
			PTR(array<T>) oTmpList = m_oList;
#else
			UINT32 dwTmpCapacity = m_dwCapacity;
			m_dwCapacity = poDst->m_dwCapacity;
			poDst->m_dwCapacity = dwTmpCapacity;

			PTR(T) oTmpList = m_oList;
#endif
			m_oList = poDst->m_oList;
			poDst->m_oList = oTmpList;

			UINT32 dwTmpBegin = m_dwBegin;
			m_dwBegin = poDst->m_dwBegin;
			poDst->m_dwBegin = dwTmpBegin;

			UINT32 dwTmpEnd = m_dwEnd;
			m_dwEnd = poDst->m_dwEnd;
			poDst->m_dwEnd = dwTmpEnd;
		}

		inline BOOLN IsEmpty() QCONST_FUNC
		{
			return m_dwBegin == m_dwEnd;
		}

		inline BOOLN IsFull() QCONST_FUNC
		{
#ifdef _USING_CLI
			return (m_dwEnd + 1) % m_oList->Length == m_dwBegin;
#else
			return (m_dwEnd + 1) % m_dwCapacity == m_dwBegin;
#endif
		}

		inline UINT32 GetCount() QCONST_FUNC
		{
#ifdef _USING_CLI
			return m_dwEnd < m_dwBegin ? m_oList->Length - m_dwBegin + m_dwEnd : m_dwEnd - m_dwBegin;
#else
			return m_dwEnd < m_dwBegin ? m_dwCapacity - m_dwBegin + m_dwEnd : m_dwEnd - m_dwBegin;
#endif
		}

		inline UINT32 GetCapacity() QCONST_FUNC
		{
#ifdef _USING_CLI
			return m_oList->Length - 1;
#else
			return m_dwCapacity - 1;
#endif
		}

			
	private:
#ifdef _USING_CLI
		PTR(array<T>) _CreateList(UINT32 dwCapacity)
		{
			return QNEW array<T>(dwCapacity);
		}
#else
		PTR(T) _CreateList(UINT32 dwCapacity)
		{
			return QNEW T[dwCapacity];
		}
		void _DestoryList(PTR(T) poT)
		{
			SAFE_DELETE_ARRAY(poT);
		}
#endif
		
		UINT32 _CopyQueue(
#ifdef _USING_CLI
			PTR(array<T>) aSrcList,
#else
			PTR(T) aSrcList,
#endif
			UINT32 dwSrcBegin, UINT32 dwSrcEnd,
#ifndef _USING_CLI
			UINT32 dwSrcCapacity,
#endif
#ifdef _USING_CLI
			PTR(array<T>) aDstList,
#else
			PTR(T) aDstList,
#endif
			UINT32 dwDstBegin, UINT32 dwDstEnd
#ifndef _USING_CLI
			, UINT32 dwDstCapacity
#endif
			)
		{
#ifdef _USING_CLI
			UINT32 dwSrcCapacity = aSrcList->Length;
			UINT32 dwDstCapacity = aDstList->Length;
#endif
			UINT32 dwCount = 0;
			SPair(UINT32, UINT32) dataX;
			SPair(UINT32, UINT32) dataY;
			SPair(UINT32, UINT32) slotX;
			SPair(UINT32, UINT32) slotY;
			if (dwSrcEnd >= dwSrcBegin)
			{
				dataX = MakePair(dwSrcBegin, dwSrcEnd - dwSrcBegin);
				dataY = MakePair(dwSrcEnd, 0u);
			}
			else
			{
				dataX = MakePair(dwSrcBegin, dwSrcCapacity - dwSrcBegin);
				dataY = MakePair(0u, dwSrcEnd);
			}

			if (dwDstEnd < dwDstBegin)
			{
				slotX = MakePair(dwDstEnd, dwDstBegin - 1 - dwDstEnd);
				slotY = MakePair(dwDstBegin - 1u, 0u);
			}
			else
			{
				slotX = MakePair(dwDstEnd, dwDstCapacity - dwDstEnd);
				slotY = MakePair(0u, dwDstBegin);
			}
			if (dataX.second != 0 && slotX.second != 0)
			{
				UINT32 dwCopyCount = _Copy(aSrcList, dataX.first, dataX.second, aDstList, slotX.first, slotX.second);
				dataX = MakePair(dataX.first + dwCopyCount, dataX.second - dwCopyCount);
				slotX = MakePair(slotX.first + dwCopyCount, slotX.second - dwCopyCount);
				dwCount += dwCopyCount;
			}
			if(dataX.second != 0 && slotY.second != 0)
			{
				UINT32 dwCopyCount = _Copy(aSrcList, dataX.first, dataX.second, aDstList, slotY.first, slotY.second);
				slotY = MakePair(slotY.first + dwCopyCount, slotY.second - dwCopyCount);
				dwCount += dwCopyCount;
			}
			if (dataY.second != 0 && slotX.second != 0)
			{
				UINT32 dwCopyCount = _Copy(aSrcList, dataY.first, dataY.second, aDstList, slotX.first, slotX.second);
				dataY = MakePair(dataY.first + dwCopyCount, dataY.second - dwCopyCount);
				slotX = MakePair(slotX.first + dwCopyCount, slotX.second - dwCopyCount);
				dwCount += dwCopyCount;
			}
			if (dataY.second != 0 && slotY.second != 0)
			{
				UINT32 dwCopyCount = _Copy(aSrcList, dataY.first, dataY.second, aDstList, slotY.first, slotY.second);
				dwCount += dwCopyCount;
			}
			return dwCount;
		}

		UINT32 _Copy(
#ifdef _USING_CLI
			PTR(array<T>) aSrcList,
#else
			PTR(T) aSrcList,
#endif
			UINT32 dwSrcBegin, UINT32 dwSrcLen,
#ifdef _USING_CLI
			PTR(array<T>) aDstList,
#else
			PTR(T) aDstList,
#endif
			UINT32 dwDstBegin, UINT32 dwDstLen)
		{
			UINT32 dwCount = dwSrcLen > dwDstLen ? dwDstLen : dwSrcLen;
#ifdef _USING_CLI
			array<T>::Copy(aSrcList, (INT32)dwSrcBegin, aDstList, (INT32)dwDstBegin, (INT32)dwCount);
#else
			memcpy(aDstList + dwDstBegin, aSrcList + dwSrcBegin, dwCount * sizeof(T));
#endif
			return dwCount;
		}

#ifndef _USING_CLI
	private:
		CQueue(CONST_REF(CQueue) roQueue)
		{
			operator=(roQueue);
		}

		REF(CQueue) operator=(CONST_REF(CQueue) roQueue)
		{
			return this;
		}
#endif

	private:
#ifdef _USING_CLI
		PTR(array<T>) m_oList;
#else
		PTR(T) m_oList = NULL_PTR;
#endif
		UINT32 m_dwBegin;
		UINT32 m_dwEnd;
#ifndef _USING_CLI
		UINT32 m_dwCapacity;
#endif
	};
}

#endif