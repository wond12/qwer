#ifndef _H_SORTED_MAP_DEFINE_H_
#define _H_SORTED_MAP_DEFINE_H_

#ifdef _USING_CLI

#define CSortedMap(k, v) qwer_stl::ordered_map<k, v >
#ifndef MapIteratorFirst
#define		MapIteratorFirst(m) (m.first)
#define		MapIteratorSecond(m) (m.second)
#endif

#else

#include <map>
#define CSortedMap(k, v) std::map<k, v >
#ifndef MapIteratorFirst
#define		MapIteratorFirst(m) (m->first)
#define		MapIteratorSecond(m) (m->second)
#endif

#endif

#endif