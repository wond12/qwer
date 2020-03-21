#ifndef _H_UNORDERDE_MAP_DEFINE_H_
#define _H_UNORDERDE_MAP_DEFINE_H_

#ifdef _USING_CLI

#define		CUnOrderedMap(k, v)	qwer_stl::unordered_map<k, v>
#define		CUnOrderedMapPtr(k, v) qwer_stl::unordered_map<k, v>^
#ifndef MapIteratorFirst
#define		MapIteratorFirst(m) (m.first)
#define		MapIteratorSecond(m) (m.second)
#endif
#else
#ifdef __linux__
#include <unordered_map>
#define		CUnOrderedMap(k, v)	std::unordered_map<k, v>

#else
#include <unordered_map>
#define		CUnOrderedMap(k, v)	std::unordered_map<k, v>

#endif
#ifndef MapIteratorFirst
#define		MapIteratorFirst(m) (m->first)
#define		MapIteratorSecond(m) (m->second)
#endif
#endif

#endif