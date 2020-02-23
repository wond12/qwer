#ifndef H_OBJECT_CACHE_H
#define	H_OBJECT_CACHE_H

#include "../CTypeDefine.h"

namespace QWER
{
	QDLL_EXPORT QINTERFACE IObjRes
	{
#ifndef _USING_CLI
	public:
		virtual ~IObjRes(){}
#endif
	public:
		virtual void QConstructor(UINT64 qwUID) = 0;
		virtual void QDestructor() = 0;
	};
};

#endif
