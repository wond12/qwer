#ifndef _H_STRING_DEFINE_H_
#define _H_STRING_DEFINE_H_

#ifdef _USING_CLI

#define	CString	System::String^
#define	CWString	System::String^

#else

#include <string>
#define	CString	std::string
#define	CWString	std::wstring

#endif

#endif
