#ifndef _H_UNORDERDE_SET_DEFINE_H_
#define _H_UNORDERDE_SET_DEFINE_H_

#ifdef _USING_CLI
#define		CUnOrderedSet(k)	qwer_stl::unordered_set<k >
#ifndef SetIteratorValue
#define SetIteratorValue(it) (it.Value)
#endif
#else
#ifdef __linux__
#include <unordered_set>
#define		CUnOrderedSet(k)	std::unordered_set<k >
#else
#include <unordered_set>
#define		CUnOrderedSet(k)	std::unordered_set<k >
#endif

#ifndef SetIteratorValue
#define SetIteratorValue(it) (*it)
#endif

#endif

#endif