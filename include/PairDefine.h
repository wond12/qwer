#ifndef _H_PAIR_DEFINE_H_
#define _H_PAIR_DEFINE_H_

#ifdef _USING_CLI

#define SPair(f, s) qwer_stl::pair<f, s >
#define MakePair(f, s) qwer_stl::pair_helper::make_pair(f, s)

#else

#include <map>
#define	SPair(f, s)	std::pair<f, s >
#define MakePair(f, s) std::make_pair(f, s)

#endif

#endif
