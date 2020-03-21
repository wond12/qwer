#ifndef _H_TUPLE_DEFINE_H_
#define _H_TUPLE_DEFINE_H_

#ifdef _USING_CLI

#define STuple(...) qwer_stl::tuple<__VA_ARGS__ >
#define MakeTuple(...) qwer_stl::CTuple::Create(__VA_ARGS__)

#else

#include <tuple>
#define	STuple(...)	std::tuple<__VA_ARGS__ >
#define MakeTuple(...) std::make_tuple(__VA_ARGS__)

#endif

#endif
