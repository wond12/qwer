#ifndef _H_C_OBJECT_RES_MGR_H_
#define	_H_C_OBJECT_RES_MGR_H_

#include "../CTypeDefine.h"
#include "CObjectCache.h"
#include "../CQueueDefine.h"
#include "../VectorDefine.h"
#ifdef _USING_CLI
//#include "stl/vector.h"
#else
#include <functional>
#endif
#include "../SmartPtrDefine.h"

namespace QWER
{

#ifdef _USING_CLI
	generic<typename T> where T : IObjRes 
	public delegate T DNewObj(UINT16 wObjType);
#else
	template <typename T>
	using DNewObj = std::function<CUniquePtr(T)(UINT16)>;
#endif

	QCLASS CObjResUIDHelper
	{
	public:
		inline static UINT64 ToUID(UINT32 dwIndex, UINT32 dwCheckNum, UINT8 btIndexLen, UINT8 btCheckNumLen)
		{
			return (((UINT64)(dwIndex & ((((UINT64)1) << btIndexLen) - 1))) << btCheckNumLen) | (UINT64)dwCheckNum;
		}
		inline static UINT32 ToIndex(UINT64 qwUID, UINT8 btIndexLen, UINT8 btCheckNumLen)
		{
			return (UINT32)((qwUID >> btCheckNumLen) & ((((UINT64)1) << btIndexLen) - 1));
		}
		inline static UINT32 ToCheckNum(UINT64 qwUID, UINT8 btCheckNumLen)
		{
			return (UINT32)(qwUID & ((((UINT64)1) << btCheckNumLen) - 1));
		}
		inline static UINT64 EncodeUserData(UINT64 qwUID, UINT32 dwUserData, UINT8 btIndexLen, UINT8 btCheckNumLen)
		{
			INT32 dwMaxUserDataLen = 64 - btIndexLen - btCheckNumLen;
			UINT32 dwMaxUserData = (UINT32)((((UINT64)1) << dwMaxUserDataLen) - 1);
			QWERASSERT(dwUserData <= dwMaxUserData, qwUID);
			return (((UINT64)(dwUserData & dwMaxUserData)) << (btIndexLen + btCheckNumLen)) | qwUID;
		}
		inline static UINT32 DecodeUserData(UINT64 qwUID, UINT8 btIndexLen, UINT8 btCheckNumLen)
		{
			UINT32 dwRet = (UINT32)(qwUID >> (btIndexLen + btCheckNumLen));
			return dwRet;
		}
	};

#ifdef _USING_CLI
	generic<typename T> where T : IObjRes
#else
	template<typename T> //where T : IObjRes
#endif
	QCLASS CObjResInfo
	{
	public:
		CObjResInfo():
			m_dwIndex(0), m_dwCheckNum(1), m_qwUID(0), m_wObjType(0)
		{
#ifndef _USING_CLI
			m_poObjRes = NULL_PTR;
#endif
		}
	public:
#ifdef _USING_CLI
		T m_poObjRes;
#else
		PTR(T) m_poObjRes;
#endif
		UINT32 m_dwIndex;	//若非Released，则表示在工作池中的索引，若为Released，则表示在空闲池中的索引
		UINT32 m_dwCheckNum;
		UINT64 m_qwUID;
        UINT16 m_wObjType;
	};

#ifdef _USING_CLI
	generic<typename T> where T : IObjRes
#else
	template<typename T> //where T : IObjRes
#endif
	QDLL_EXPORT QCLASS CObjectResMgrImp
	{
	public:

		CObjectResMgrImp(CONST_PTR_OR_REF(DNewObj<T>) cbNewObjFunc)
		{
			m_cbNewObjFunc = onNewObjFunc;
		}

		CObjectResMgrImp(CONST_PTR_OR_REF(DNewObj<T>) cbNewObjFunc, BOOLN bForceCreateByUID)
		{
			m_cbNewObjFunc = cbNewObjFunc;
			m_bForceCreateByUID = bForceCreateByUID;
		}

		CObjectResMgrImp(CONST_PTR_OR_REF(DNewObj<T>) cbNewObjFunc, UINT8 btIndexLenInUID, UINT8 btCheckNumLenInUID)
		{
			m_cbNewObjFunc = cbNewObjFunc;
			m_btCheckNumLen = btCheckNumLenInUID;
			m_btIndexLen = btIndexLenInUID;
		}

		CObjectResMgrImp(CONST_PTR_OR_REF(DNewObj<T>) cbNewObjFunc, BOOLN bForceCreateByUID, UINT8 btIndexLenInUID, UINT8 btCheckNumLenInUID)
		{
			m_cbNewObjFunc = cbNewObjFunc;
			m_bForceCreateByUID = bForceCreateByUID;
			m_btCheckNumLen = btCheckNumLenInUID;
			m_btIndexLen = btIndexLenInUID;
		}

#ifndef _USING_CLI
		~CObjectResMgrImp()
		{
			for(UINT32 i = 0; i != m_oObjRes.size(); i++)
			{
				SAFE_DELETE(m_oObjRes[i]->m_poObjRes);
				SAFE_DELETE(m_oObjRes[i]);
			}
			for(UINT32 i = 0; i != m_oFreeIndexs.size(); i++)
			{
				SAFE_DELETE(m_oFreeIndexs[i]);
			}
		}
#endif

		void ReleaseAll()
		{
			for(UINT32 i = 0; i != m_oObjRes.size(); i++)
			{
				PTR(CObjResInfo<T>) poRet = m_oObjRes.at(i);
				if (poRet == NULL_PTR || poRet->m_poObjRes == NULL_PTR || poRet->m_qwUID == 0)
					continue;
				UINT16 wObjType = poRet->m_wObjType;
				poRet->m_poObjRes->QDestructor();
                poRet->m_wObjType = 0;
				poRet->m_qwUID = 0;
				--m_dwSize;
				if (++poRet->m_dwCheckNum == (UINT32)((((UINT64)1) << m_btCheckNumLen)))
					poRet->m_dwCheckNum = 1;
				if (m_bDestory)
				{
#ifdef _USING_CLI
					poRet->m_poObjRes = T();
#else
					SAFE_DELETE(poRet->m_poObjRes);
#endif
				}
				m_bDestory = !m_bDestory;
				if (!m_bForceCreateByUID)
				{
					m_oFreeIndexs.at(wObjType)->Push(poRet->m_dwIndex);
					poRet->m_dwIndex = m_oFreeIndexs.at(wObjType)->GetCount() - 1;
				}
			}
			m_dwSize = 0;
		}

		inline UINT32 Size() QCONST_FUNC
		{
			return m_dwSize;
		}

	protected:
		inline UINT64 _ToUID(UINT32 dwIndex, UINT32 dwCheckNum)
		{
			return CObjResUIDHelper::ToUID(dwIndex, dwCheckNum, m_btIndexLen, m_btCheckNumLen);
		}
		inline UINT32 _ToIndex(UINT64 qwUID)
		{
			return CObjResUIDHelper::ToIndex(qwUID, m_btIndexLen, m_btCheckNumLen);
		}
		inline UINT32 _ToCheckNum(UINT64 qwUID)
		{
			return CObjResUIDHelper::ToCheckNum(qwUID, m_btCheckNumLen);
		}

		VAL_OR_PTR(T) _GetObj(UINT64 qwUID)
		{
			PTR(CObjResInfo<T>) poRet = _GetObjRes(qwUID);
			return poRet == NULL_PTR ?
#ifdef _USING_CLI
				T()
#else
				NULL_PTR
#endif
				: poRet->m_poObjRes;
		}

		void _ReleaseObj(UINT64 qwUID)
		{
			PTR(CObjResInfo<T>) poObjRes = _GetObjRes(qwUID);
			if (NULL_PTR == poObjRes)
				return;
			VAL_OR_PTR(T) poObj = poObjRes->m_poObjRes;
			UINT16 wObjType = poObjRes->m_wObjType;
			poObj->QDestructor();
            poObjRes->m_wObjType = 0;
			poObjRes->m_qwUID = 0;
			--m_dwSize;
			UINT32 dwIndex = CObjResUIDHelper::ToIndex(qwUID, m_btIndexLen, m_btCheckNumLen);
			PTR(CObjResInfo<T>) poRet = m_oObjRes.at(dwIndex);
			if (++poRet->m_dwCheckNum == (UINT32)((((UINT64)1) << m_btCheckNumLen)))
				poRet->m_dwCheckNum = 1;
			if (m_bDestory)
			{
#ifdef _USING_CLI
				poRet->m_poObjRes = T();
#else
				SAFE_DELETE(poRet->m_poObjRes);
#endif
			}
			m_bDestory = !m_bDestory;
			if (!m_bForceCreateByUID)
			{
				PTR(CQueue<UINT32>) poObjectFreeIndexCache = m_oFreeIndexs.at(wObjType);
				poObjectFreeIndexCache->Push(poRet->m_dwIndex);
				poRet->m_dwIndex = poObjectFreeIndexCache->GetCount() - 1;
			}
		}

#ifdef _USING_CLI
		T _CreateObj(UINT16 wObjType)
#else
		PTR(T) _CreateObj(UINT16 wObjType)
#endif
		{
			if (m_bForceCreateByUID)
			{
#ifdef _USING_CLI
				throw QNEW System::InvalidOperationException("only can be create by uid!");
#else
				throw std::runtime_error("only can be create by uid!");
#endif
			}
			PTR(CObjResInfo<T>) poObjResInfo = NULL_PTR;
			if (wObjType >= m_oFreeIndexs.size())
			{
				m_oFreeIndexs.resize(wObjType + 1);
				TO_PTR1(m_oFreeIndexs)[wObjType] = NULL_PTR;
			}
			if (m_oFreeIndexs.at(wObjType) == NULL_PTR)
			{
				TO_PTR1(m_oFreeIndexs)[wObjType] = QNEW CQueue<UINT32>();
			}
			PTR(CQueue<UINT32>) poObjectFreeIndexCache = m_oFreeIndexs.at(wObjType);
			if (poObjectFreeIndexCache->GetCount() == 0)
			{
				UINT32 dwSize = (UINT32)m_oObjRes.size();
				UINT32 dwMaxSize = (m_btIndexLen >= 32 ? (UINT32_MAX_VALUE / 2) : ((1u << m_btIndexLen) - 1u));
				if (dwSize >= dwMaxSize)
#ifdef _USING_CLI
					return T();
#else
					return NULL_PTR;
#endif
				poObjResInfo = QNEW CObjResInfo<T>();
				m_oObjRes.push_back(poObjResInfo);
				poObjResInfo->m_dwIndex = m_oObjRes.size() - 1;
				poObjResInfo->m_dwCheckNum = 1;
			}
			else
			{
#ifdef _USING_CLI
				UINT32 dwFreeIndex = poObjectFreeIndexCache[0];
#else
				UINT32 dwFreeIndex = (*poObjectFreeIndexCache)[0];
#endif
				poObjectFreeIndexCache->Pop();
				poObjResInfo = m_oObjRes.at(dwFreeIndex);
				poObjResInfo->m_dwIndex = dwFreeIndex;
			}
			if (poObjResInfo->m_poObjRes == NULL_PTR)
			{
                poObjResInfo->m_wObjType = wObjType;
				poObjResInfo->m_poObjRes = QReleaseFromUniquePtr(m_cbNewObjFunc(wObjType));
			}
			poObjResInfo->m_qwUID = CObjResUIDHelper::ToUID(
				poObjResInfo->m_dwIndex, poObjResInfo->m_dwCheckNum, m_btIndexLen, m_btCheckNumLen);
			++m_dwSize;
			//QWERASSERT(m_dwSize < 100, NULL_PTR);
			poObjResInfo->m_poObjRes->QConstructor(poObjResInfo->m_qwUID);
			return poObjResInfo->m_poObjRes;
		}

#ifdef _USING_CLI
		T _CreateObjByUID(UINT16 wObjType, UINT64 qwUID)
#else
		PTR(T) _CreateObjByUID(UINT16 wObjType, UINT64 qwUID)
#endif
		{
			if (!m_bForceCreateByUID)
#ifdef _USING_CLI
				throw QNEW System::InvalidOperationException("can not be create by uid!");
#else
				throw std::runtime_error("can not be create by uid!");
#endif
			PTR(CObjResInfo<T>) poObjResInfo = NULL_PTR;
			UINT32 dwIndex = CObjResUIDHelper::ToIndex(qwUID, m_btIndexLen, m_btCheckNumLen);
			UINT32 dwCheckNum = CObjResUIDHelper::ToCheckNum(qwUID, m_btCheckNumLen);
			if ((INT32)dwIndex >= (INT32)m_oObjRes.size())
			{
				m_oObjRes.resize(dwIndex + 1);
			}
			poObjResInfo = m_oObjRes.at(dwIndex);
			if (poObjResInfo == NULL_PTR)
			{
				TO_PTR1(m_oObjRes)[dwIndex] = QNEW CObjResInfo<T>();
				poObjResInfo = m_oObjRes.at(dwIndex);
			}
			if (poObjResInfo->m_poObjRes == NULL_PTR)
			{
                poObjResInfo->m_wObjType = wObjType;
				poObjResInfo->m_poObjRes = QReleaseFromUniquePtr(m_cbNewObjFunc(wObjType));
			}
			else if (poObjResInfo->m_wObjType != wObjType || poObjResInfo->m_qwUID != 0)
#ifdef _USING_CLI
				throw QNEW System::Exception("conflict index when create object!");
#else
				throw std::runtime_error("conflict index when create object!");
#endif
			poObjResInfo->m_dwIndex = dwIndex;
			poObjResInfo->m_dwCheckNum = dwCheckNum;
			poObjResInfo->m_qwUID = qwUID;
			++m_dwSize;
			poObjResInfo->m_poObjRes->QConstructor(poObjResInfo->m_qwUID);
			return poObjResInfo->m_poObjRes;
		}

	private:
		PTR(CObjResInfo<T>) _GetObjRes(UINT64 qwUID)
		{
            if (qwUID == 0)
                return NULL_PTR;
			UINT32 dwIndex = CObjResUIDHelper::ToIndex(qwUID, m_btIndexLen, m_btCheckNumLen),
				dwCheckNum = CObjResUIDHelper::ToCheckNum(qwUID, m_btCheckNumLen);
			if (dwCheckNum == 0)
				return NULL_PTR;
			if (dwIndex >= (UINT32)m_oObjRes.size())
				return NULL_PTR;
			PTR(CObjResInfo<T>) poRet = m_oObjRes.at(dwIndex);
			if (poRet == NULL_PTR || poRet->m_dwCheckNum != dwCheckNum)
				return NULL_PTR;
			return poRet;
		}

	private:
		CVector(PTR(CObjResInfo<T>)) m_oObjRes;
		CVector(PTR(CQueue<UINT32>)) m_oFreeIndexs;
		PTR_OR_VAL(DNewObj<T>) m_cbNewObjFunc;
		UINT32 m_dwSize = 0;
		BOOLN m_bDestory = false;
		UINT8 m_btIndexLen = 32;
		UINT8 m_btCheckNumLen = 32;
		BOOLN m_bForceCreateByUID = false;
	};

#define OBJ_RES_INDEX_LEN 32
#define OBJ_RES_CHECK_NUM_LEN 32

	QDLL_EXPORT QCLASS CObjResHelper
	{
	public:
		inline static UINT64 ToUID(UINT32 dwIndex, UINT32 dwCheckNum)
		{
			return CObjResUIDHelper::ToUID(dwIndex, dwCheckNum, OBJ_RES_INDEX_LEN, OBJ_RES_CHECK_NUM_LEN);
		}
		inline static UINT32 ToIndex(UINT64 qwUID)
		{
			return CObjResUIDHelper::ToIndex(qwUID, OBJ_RES_INDEX_LEN, OBJ_RES_CHECK_NUM_LEN);
		}
		inline static UINT32 ToCheckNum(UINT64 qwUID)
		{
			return CObjResUIDHelper::ToCheckNum(qwUID, OBJ_RES_CHECK_NUM_LEN);
		}
	};

#define OBJ_RES_INDEX_MINI_LEN 20
#define OBJ_RES_CHECK_NUM_MINI_LEN 24

	QDLL_EXPORT QCLASS CObjResMiniHelper
	{
	public:
		inline static UINT64 ToUID(UINT32 dwIndex, UINT32 dwCheckNum)
		{
			return CObjResUIDHelper::ToUID(dwIndex, dwCheckNum, OBJ_RES_INDEX_MINI_LEN, OBJ_RES_CHECK_NUM_MINI_LEN);
		}
		inline static UINT32 ToIndex(UINT64 qwUID)
		{
			return CObjResUIDHelper::ToIndex(qwUID, OBJ_RES_INDEX_MINI_LEN, OBJ_RES_CHECK_NUM_MINI_LEN);
		}
		inline static UINT32 ToCheckNum(UINT64 qwUID)
		{
			return CObjResUIDHelper::ToCheckNum(qwUID, OBJ_RES_CHECK_NUM_MINI_LEN);
		}
		inline static UINT64 EncodeUserData(UINT64 qwUID, UINT32 dwUserData)
		{
			return CObjResUIDHelper::EncodeUserData(qwUID, dwUserData, OBJ_RES_INDEX_MINI_LEN, OBJ_RES_CHECK_NUM_MINI_LEN);
		}
		inline static UINT32 DecodeUserData(UINT64 qwUID)
		{
			return CObjResUIDHelper::DecodeUserData(qwUID, OBJ_RES_INDEX_MINI_LEN, OBJ_RES_CHECK_NUM_MINI_LEN);
		}
	};

#ifdef _USING_CLI
	generic<typename T> where T : IObjRes
#else
	template<typename T> //where T : IObjRes
#endif
	QDLL_EXPORT QCLASS CObjectResMgr : public CObjectResMgrImp<T>
	{
	public:

		CObjectResMgr(CONST_PTR_OR_REF(DNewObj<T>) cbNewObjFunc):
			CObjectResMgrImp<T>(cbNewObjFunc, OBJ_RES_INDEX_LEN, OBJ_RES_CHECK_NUM_LEN)
		{
		}

		VAL_OR_PTR(T) CreateObj(UINT16 wObjType)
		{
			return this->_CreateObj(wObjType);
		}

		VAL_OR_PTR(T) GetObj(UINT64 qwUID)
		{
			return this->_GetObj(qwUID);
		}

		void ReleaseObj(UINT64 qwUID)
		{
			return this->_ReleaseObj(qwUID);
		}

	};

#ifdef _USING_CLI
	generic<typename T> where T : IObjRes
#else
	template<typename T> //where T : IObjRes
#endif
	QDLL_EXPORT QCLASS CObjectCreatedByUIDMgr : public CObjectResMgrImp<T>
	{
	public:

		CObjectCreatedByUIDMgr(CONST_PTR_OR_REF(DNewObj<T>) cbNewObjFunc) :
			CObjectResMgrImp<T>(cbNewObjFunc, true, UINT32_MAX_VALUE, OBJ_RES_INDEX_LEN, OBJ_RES_CHECK_NUM_LEN)
		{
		}

		VAL_OR_PTR(T) CreateObjByUID(UINT16 wObjType, UINT64 qwUID)
		{
			return this->_CreateObjByUID(wObjType, qwUID);
		}

		VAL_OR_PTR(T) GetObj(UINT64 qwUID)
		{
			return this->_GetObj(qwUID);
		}

		void ReleaseObj(UINT64 qwUID)
		{
			return this->_ReleaseObj(qwUID);
		}

	};

#ifdef _USING_CLI
	generic<typename T> where T : IObjRes
#else
	template<typename T> //where T : IObjRes
#endif
	QDLL_EXPORT QCLASS CObjectResMiniMgr : public CObjectResMgrImp<T>
	{
	public:

		CObjectResMiniMgr(CONST_PTR_OR_REF(DNewObj<T>) cbNewObjFunc) :
			CObjectResMgrImp<T>(cbNewObjFunc, OBJ_RES_INDEX_MINI_LEN, OBJ_RES_CHECK_NUM_MINI_LEN)
		{
		}

		VAL_OR_PTR(T) CreateObj(UINT16 wObjType)
		{
			return this->_CreateObj(wObjType);
		}

		VAL_OR_PTR(T) GetObj(UINT64 qwUID)
		{
			return this->_GetObj(qwUID);
		}

		void ReleaseObj(UINT64 qwUID)
		{
			return this->_ReleaseObj(qwUID);
		}
	};

#ifdef _USING_CLI
	generic<typename T> where T : IObjRes
#else
	template<typename T> //where T : IObjRes
#endif
	QDLL_EXPORT QCLASS CObjectCreatedByUIDMiniMgr : public CObjectResMgrImp<T>
	{
	public:

		CObjectCreatedByUIDMiniMgr(CONST_PTR_OR_REF(DNewObj<T>) cbNewObjFunc) :
			CObjectResMgrImp<T>(cbNewObjFunc, true, UINT32_MAX_VALUE, OBJ_RES_INDEX_MINI_LEN, OBJ_RES_CHECK_NUM_MINI_LEN)
		{
		}

		VAL_OR_PTR(T) CreateObjByUID(UINT16 wObjType, UINT64 qwUID)
		{
			return this->_CreateObjByUID(wObjType, qwUID);
		}

		VAL_OR_PTR(T) GetObj(UINT64 qwUID)
		{
			return this->_GetObj(qwUID);
		}

		void ReleaseObj(UINT64 qwUID)
		{
			return this->_ReleaseObj(qwUID);
		}
	};

}

#endif
