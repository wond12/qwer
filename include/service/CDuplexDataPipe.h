#ifndef H_C_DUPLEX_DATA_PIPE_H
#define H_C_DUPLEX_DATA_PIPE_H

#include "../CTypeDefine.h"
#include "CSimplexDataPipe.h"
#include "CDuplexDataPipePeer.h"

namespace QWER
{
	QTEMPLATE<typename T1, typename T2>
	QDLL_EXPORT QCLASS CDuplexDataPipe
	{
	public:
		CDuplexDataPipe()
#ifndef _USING_CLI
			: m_oPeer1(m_oFirst, m_oSecond), m_oPeer2(m_oSecond, m_oFirst)
#endif
		{
#ifdef _USING_CLI
			m_oPeer1 = QNEW CDuplexDataPipePeer<T1, T2>(TO_PTR2(m_oFirst), TO_PTR2(m_oSecond));
			m_oPeer2 = QNEW CDuplexDataPipePeer<T2, T1>(TO_PTR2(m_oSecond), TO_PTR2(m_oFirst));
#endif
		}
		inline PTR_OR_REF(QPACK(CDuplexDataPipePeer<T1, T2>)) GetPeer1(){ return m_oPeer1; }
		inline PTR_OR_REF(QPACK(CDuplexDataPipePeer<T2, T1>)) GetPeer2(){ return m_oPeer2; }
	private:
		PTR_OR_VAL(CSimplexDataPipe<T1>) m_oFirst;
		PTR_OR_VAL(CSimplexDataPipe<T2>) m_oSecond;
		PTR_OR_VAL(QPACK(CDuplexDataPipePeer<T1, T2>)) m_oPeer1;
		PTR_OR_VAL(QPACK(CDuplexDataPipePeer<T2, T1>)) m_oPeer2;
	};
}

#endif