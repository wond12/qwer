#ifndef _H_VECTOR_DEFINE_H_
#define _H_VECTOR_DEFINE_H_

#ifdef _USING_CLI

#define CVector(tType) qwer_stl::vector<tType >
#define CVectorPtr(tType) qwer_stl::vector<tType >^
#define VectorIteratorValue(it) (it.Value)

#else

#include <vector>
#define CVector(tType) std::vector<tType >
#define VectorIteratorValue(it) (*it)

#endif

#endif