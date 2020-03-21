#ifndef H_C_DUPLEX_DATA_PIPE_PEER_H
#define H_C_DUPLEX_DATA_PIPE_PEER_H

#include "../CTypeDefine.h"
#include "CSimplexDataPipe.h"

namespace QWER
{
	QTEMPLATE<typename T1, typename T2> QCLASS CDuplexDataPipe;

	QTEMPLATE<typename T1, typename T2>
	QDLL_EXPORT QCLASS CDuplexDataPipePeer
	{
	public:
		void Push(CONST_VAL_OR_REF(T2) roData)
		{
			m_poOut->Push(roData);
		}

		void Poll(UINT64 qwDeltaTickMilliSeconds)
		{
			m_poOut->Poll(qwDeltaTickMilliSeconds);
		}

#ifndef _USING_CLI
		CONST_VAL_OR_REF(T1) Top() QCONST_FUNC
		{
			return m_poIn->Top();
		}
#endif

		VAL_OR_REF(T1) Top()
		{
			return m_poIn->Top();
		}

		inline BOOLN IsEmpty() QCONST_FUNC
		{
			return m_poIn->IsEmpty();
		}

		void Pop()
		{
			m_poIn->Pop();
		}

	private:
		PTR(CSimplexDataConsumer<T1>) m_poIn;
		PTR(CSimplexDataProducer<T2>) m_poOut;

#ifdef _USING_CLI
	internal:
#else
	private:
		friend class CDuplexDataPipe<T1, T2>;
		friend class CDuplexDataPipe<T2, T1>;
#endif
		CDuplexDataPipePeer(PTR_OR_REF(CSimplexDataPipe<T1>) roIn, PTR_OR_REF(CSimplexDataPipe<T2>) roOut)
		{
			m_poIn = TO_PTR2(TO_PTR2(roIn)->GetConsumer());
			m_poOut = TO_PTR2(TO_PTR2(roOut)->GetProducer());
		}
	};
}

#endif