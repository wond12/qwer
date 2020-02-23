#ifndef _H_SMART_PTR_DEFINE_H_
#define _H_SMART_PTR_DEFINE_H_

#ifdef _USING_CLI
#define CUniquePtr(class_type) class_type^
#define QMOVEABLE(class_type) class_type^%
#define QMOVETO(srcObj, dstObj) do {dstObj = srcObj; srcObj = nullptr;}while (false)
#define QMOVE(obj) obj
#define QGetFromUniquePtr(obj) obj
#define QReleaseFromUniquePtr(obj) obj
#define QMakeUniquePtr(type, ...) gcnew type(__VA_ARGS__)
#else
#include <memory>
#include <utility>
#define CUniquePtr(class_type) std::unique_ptr<class_type>
#define QMOVEABLE(class_type) std::unique_ptr<class_type>&&
#define QMOVETO(srcObj, dstObj) dstObj = std::move(srcObj)
#define QMOVE(obj) std::move(obj)
#define QGetFromUniquePtr(obj) obj.get()
#define QReleaseFromUniquePtr(obj) obj.release()
#define QMakeUniquePtr(type, ...) std::make_unique<type>(__VA_ARGS__)
#endif

#endif
