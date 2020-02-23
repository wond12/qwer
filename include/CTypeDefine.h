#ifndef CTYPEDEFINE_H
#define	CTYPEDEFINE_H

#if defined(MBED_OPERATORS)
#undef compatible_close
#define compatible_close(fd) mbed3_delete_tcp_stream_socket(fd)
#ifndef BIG_ENDIAN
#define BIG_ENDIAN    4321  /* to show byte order (taken from gcc) */
#endif
#ifndef LITTLE_ENDIAN
#define LITTLE_ENDIAN 1234
#endif
#ifndef BYTE_ORDER
#define BYTE_ORDER LITTLE_ENDIAN
#endif
#endif

#if defined(WIN32) || defined(_WIN32)

#ifndef BIG_ENDIAN
#define BIG_ENDIAN    4321  /* to show byte order (taken from gcc) */
#endif
#ifndef LITTLE_ENDIAN
#define LITTLE_ENDIAN 1234
#endif
#ifndef BYTE_ORDER
#define BYTE_ORDER LITTLE_ENDIAN
#endif
#ifndef u_int64_t
typedef unsigned __int64 u_int64_t;
#endif

#undef __P
#ifndef __P
#if __STDC__
#define __P(protos) protos
#else
#define __P(protos) ()
#endif
#endif

#else

#include <sys/stat.h>
#include <sys/cdefs.h>
#include <sys/time.h>

#if defined(__APPLE__)
#include <machine/endian.h>
#elif defined(__FreeBSD__)
#include <sys/endian.h>
#elif defined(__linux__)
#include <endian.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif
#include <stddef.h>
#ifdef __cplusplus
}
#endif

#ifndef container_of
#define container_of(P,T,M)	((T *)((char *)(P) - offsetof(T, M)))
#endif

#if defined(__QNX__)
#include <gulliver.h>
#if defined(__LITTLEENDIAN__)
#define BYTE_ORDER __LITTLEENDIAN__
#define LITTLE_ENDIAN __LITTLEENDIAN__
#define BIG_ENDIAN 4321  /* to show byte order (taken from gcc); for suppres warning that BIG_ENDIAN is not defined. */
#endif
#if defined(__BIGENDIAN__)
#define BYTE_ORDER __BIGENDIAN__
#define LITTLE_ENDIAN 1234  /* to show byte order (taken from gcc); for suppres warning that LITTLE_ENDIAN is not defined. */
#define BIG_ENDIAN __BIGENDIAN__
#endif
#endif

#if !defined(BYTE_ORDER)
# define BYTE_ORDER __BYTE_ORDER
#endif
#if !defined(LITTLE_ENDIAN)
# define LITTLE_ENDIAN __LITTLE_ENDIAN
#endif
#if !defined(BIG_ENDIAN)
# define BIG_ENDIAN __BIG_ENDIAN
#endif

#endif

	/*
	* Mac OSX as well as iOS do not define the MSG_NOSIGNAL flag,
	* but happily have something equivalent in the SO_NOSIGPIPE flag.
	*/
#ifdef __APPLE__
#define MSG_NOSIGNAL SO_NOSIGPIPE
#endif

#ifdef _USING_CLI
#else
#ifdef __linux__
#include <bits/types.h>
#else
#include <cstdint>
#endif

#include <stdio.h>
#endif

#define MAX_SESSION_COUNT 65536

#ifdef _USING_CLI
#define INT8_MAX_VALUE System::SByte::MaxValue
#define UINT8_MAX_VALUE System::Byte::MaxValue
#define INT16_MAX_VALUE System::Int16::MaxValue
#define UINT16_MAX_VALUE System::UInt16::MaxValue
#define INT32_MAX_VALUE System::Int32::MaxValue
#define UINT32_MAX_VALUE System::UInt32::MaxValue
#else
#define INT8_MAX_VALUE 127
#define UINT8_MAX_VALUE 255
#define INT16_MAX_VALUE 32767
#define UINT16_MAX_VALUE 65535
#define INT32_MAX_VALUE 2147483647
#define UINT32_MAX_VALUE 4294967295
#endif

#ifdef _USING_CLI
#define INT8 System::SByte
#define UINT8 System::Byte
#define INT16 System::Int16
#define UINT16 System::UInt16
#define INT32 System::Int32
#define UINT32 System::UInt32
#define INT64 System::Int64
#define UINT64 System::UInt64
#define BOOLN System::Boolean
#define FLOAT System::Single
#define DOUBLE System::Double
#define VECINDEX System::Int32
#else
#ifdef __linux__
typedef		__int8_t		INT8;
typedef		__uint8_t		UINT8;
typedef		__int16_t		INT16;
typedef		__uint16_t		UINT16;
typedef		__int32_t		INT32;
typedef		__uint32_t		UINT32;
typedef		__int64_t		INT64;
typedef		__uint64_t		UINT64;
typedef		bool			BOOLN;
typedef		float			FLOAT;
typedef		double			DOUBLE;
#else
typedef     int8_t      INT8;
typedef     uint8_t     UINT8;
typedef     int16_t     INT16;
typedef     uint16_t    UINT16;
typedef     int32_t     INT32;
typedef     uint32_t    UINT32;
typedef     int64_t     INT64;
typedef     uint64_t    UINT64;
typedef		bool		BOOLN;
typedef		float		FLOAT;
typedef		double		DOUBLE;
#endif
typedef UINT32 VECINDEX;
#endif

//#ifdef _USING_CLI
//#define INT8_PTR INT8^
//#define INT8_REF INT8%
//#define UINT8_PTR UINT8^
//#define UINT8_REF UINT8%
//#define INT16_PTR INT16^
//#define INT16_REF INT16%
//#define UINT16_PTR UINT16^
//#define UINT16_REF UINT16%
//#define INT32_PTR INT32^
//#define INT32_REF INT32%
//#define UINT32_PTR UINT32^
//#define UINT32_REF UINT32%
//#define INT64_PTR INT64^
//#define INT64_REF INT64%
//#define UINT64_PTR UINT64^
//#define UINT64_REF UINT64%
//#else
//#define INT8_PTR INT8*
//#define INT8_REF INT8&
//#define UINT8_PTR UINT8*
//#define UINT8_REF UINT8&
//#define INT16_PTR INT16*
//#define INT16_REF INT16&
//#define UINT16_PTR UINT16*
//#define UINT16_REF UINT16&
//#define INT32_PTR INT32*
//#define INT32_REF INT32&
//#define UINT32_PTR UINT32*
//#define UINT32_REF UINT32&
//#define INT64_PTR INT64*
//#define INT64_REF INT64&
//#define UINT64_PTR UINT64*
//#define UINT64_REF UINT64&
//#endif

#define SIZE_OF_INT8 1
#define SIZE_OF_UINT8 1
#define SIZE_OF_INT16 2
#define SIZE_OF_UINT16 2
#define SIZE_OF_INT32 4
#define SIZE_OF_UINT32 4
#define SIZE_OF_INT64 8
#define SIZE_OF_UINT64 8
#define SIZE_OF_FLOAT 4
#define SIZE_OF_DOUBLE 8
#define SIZE_OF_BOOL 1

//#ifdef _USING_CLI
//#include "CVecMap.h"
//#include <cliext/map>
//#include <cliext/set>
//#include <cliext/hash_map>
//#include <cliext/hash_set>
//#define CSortedList QWER::CVecMap
//#define CSortedDictionary QSTD::map;
//#define		UnOrderedMap	QSTD::hash_map
//#define		UnOrderedSet	QSTD::hash_set
//#else
//#include "CVecMap.h"
//#define CSortedList QWER::CVecMap
//#include <map>
//#define CSortedDictionary QSTD::map
//#endif

#ifdef _USING_CLI
#define QNEW gcnew
#define QDLL_EXPORT public
#define QCLASS ref class
#define QSTRUCT value struct
#define QENUM enum class
#define QCONST_FUNC
#define CHAR_PTR char^
#define NULL_PTR nullptr
#define QABSTRACT abstract
#define QINTERFACE interface class
#define QOVERRIDE override
#define QINIT_ONLY initonly
#define QFINAL sealed
#define QTEMPLATE generic
#define QWHERE(...) where __VA_ARGS__
#define QPACK(...) __VA_ARGS__
#define IS_NOT_NULL_DELEGATE(d) (d!=NULL_PTR)
#define SET_DELEGATE_TO_NULL(d) (d=NULL_PTR)
#define PACK_DELEGATE_FUNC(delegate_type, obj_ptr, func) QNEW delegate_type(obj_ptr, &func)
#define PACK_STATIC_DELEGATE_FUNC(delegate_type, func) QNEW delegate_type(&func)
#define NEW_DELEGATE(delegate_type, obj_ptr, func, ...) PACK_DELEGATE_FUNC(delegate_type, obj_ptr, func)
#define CONVERT_DELEGATE(delegate_type, origDelegate, ...) QNEW delegate_type(qwer_stl::function::bind(origDelegate __VA_ARGS__))
#define NEW_STATIC_DELEGATE(d, func, ...) QNEW d(func)
#define NEW_FUNC(d, func) QNEW d(func)
#define DELEGATE_PLACEHOLDERS(...)
#define MUTEX_LOCK(mutex) System::Threading::Monitor::Enter(mutex)
#define MUTEX_UNLOCK(mutex) System::Threading::Monitor::Exit(mutex)
#define STRUCT_MEMBER_DEFINE(member, value) member
#define QLITERAL literal
#else
#define QNEW new
#define QDLL_EXPORT
#define QCLASS class
#define QSTRUCT struct
#define QENUM enum class
#define QCONST_FUNC const
#define CHAR_PTR char*
#define NULL_PTR nullptr
#define QABSTRACT
#define QINTERFACE class
#define QOVERRIDE override
#define QINIT_ONLY 
#define QFINAL final
#define QTEMPLATE template
#define QWHERE(...)
#define QPACK(...) __VA_ARGS__
#define IS_NOT_NULL_DELEGATE(d) (d)
#define SET_DELEGATE_TO_NULL(d) (d=NULL_PTR)
#define PACK_DELEGATE_FUNC(delegate_type, obj_ptr, func) &func, obj_ptr
#define PACK_STATIC_DELEGATE_FUNC(delegate_type, func) &func
#define NEW_DELEGATE(delegate_type, obj_ptr, func, ...) std::bind(&func, obj_ptr __VA_ARGS__)
#define CONVERT_DELEGATE(delegate_type, origDelegate, ...) std::bind(origDelegate __VA_ARGS__)
#define NEW_STATIC_DELEGATE(d, func, ...) std::bind(&func __VA_ARGS__)
#define NEW_FUNC(d, func) &func
#define DELEGATE_PLACEHOLDERS(...) __VA_ARGS__
#define MUTEX_LOCK(mutex) mutex.lock();
#define MUTEX_UNLOCK(mutex) mutex.unlock();
#define STRUCT_MEMBER_DEFINE(member, value) member = value
#define QLITERAL static const
#endif

#ifdef _USING_CLI
#define SAFE_DELETE(p)			(p) = NULL_PTR
#else
#define SAFE_DELETE(p)			do { delete (p); (p) = NULL_PTR; } while(false)
#endif

#ifdef _USING_CLI
#define SAFE_DELETE_ARRAY(p)			(p) = NULL_PTR
#else
#define SAFE_DELETE_ARRAY(p)	do { delete[] (p); (p) = NULL_PTR; } while(false)
#endif


#ifdef _DEBUG
#ifdef _USING_CLI
#define QWERASSERT(condition, ret) System::Diagnostics::Debug::Assert(condition)
#else
#include <assert.h>
#define QWERASSERT(condition, ret) assert(condition)
#endif
#else 
#ifdef _USING_CLI
#define QWERASSERT(condition, ret) do {                        \
	if (!(condition))							\
{\
	return ret;	\
}\
} while(false)
#else
#include <stdio.h>
#include <stdexcept>
#define QWERASSERT(condition, ret) do {                        \
	if (!(condition))							\
{\
	printf("[ASSERT] %s, %d\n", __FILE__, __LINE__); \
	return ret;	\
}\
} while(false)
#endif
#endif

//换行符
#ifndef _USING_CLI
#ifdef __linux__
#define ENDL "\n"
#else
#define ENDL "\r\n"
#endif
#endif

#ifdef _USING_CLI
#define TO_PTR(...) (%(__VA_ARGS__))
#define TO_REF(...) (*(__VA_ARGS__))
#define PTR(...) __VA_ARGS__^
#define REF(...) __VA_ARGS__%
#define CONST_PTR(...) __VA_ARGS__^
#define CONST_REF(...) __VA_ARGS__
#define PTR_OR_REF(...) __VA_ARGS__^
#define CONST_PTR_OR_REF(...) __VA_ARGS__^
#define VAL_OR_PTR(...) __VA_ARGS__
#define CONST_VAL_OR_PTR(...) __VA_ARGS__
#define VAL_OR_REF(...) __VA_ARGS__
#define CONST_VAL_OR_REF(...) __VA_ARGS__
#define PTR_OR_VAL(...) __VA_ARGS__^
#define TO_PTR1(...) (%(__VA_ARGS__))
#define TO_PTR2(...) __VA_ARGS__
#define TO_REF2(...) __VA_ARGS__
#else
#define TO_PTR(...) (&(__VA_ARGS__))
#define TO_REF(...) (*(__VA_ARGS__))
#define PTR(...) __VA_ARGS__*
#define REF(...) __VA_ARGS__&
#define CONST_PTR(...) const __VA_ARGS__*
#define CONST_REF(...) const __VA_ARGS__&
#define PTR_OR_REF(...) __VA_ARGS__&
#define CONST_PTR_OR_REF(...) const __VA_ARGS__&
#define VAL_OR_PTR(...) __VA_ARGS__*
#define CONST_VAL_OR_PTR(...) const __VA_ARGS__*
#define VAL_OR_REF(...) __VA_ARGS__&
#define CONST_VAL_OR_REF(...) const __VA_ARGS__&
#define PTR_OR_VAL(...) __VA_ARGS__
#define TO_PTR1(...) __VA_ARGS__
#define TO_PTR2(...) (&(__VA_ARGS__))
#define TO_REF2(...) (*(__VA_ARGS__))
#endif

#ifdef _USING_CLI
#define PROTOCOL_ID PROTOCOL_ID_()
#endif

#ifdef _USING_CLI
#define NON_COPYABLE(tType)
#else
#define NON_COPYABLE(tType) private: tType(const tType& orig) { operator=(orig); } tType& operator=(const tType&) { return *this; }
#endif

#endif	/* CTYPEDEFINE_H */

