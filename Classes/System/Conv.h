#ifndef __Conv_H__
#define __Conv_H__

#ifdef WIN32

#define UTEXT(str) GBKToUTF8(str)
#include <iconv.h>
#include <string>
const char* GBKToUTF8(const char *strChar);

#else

#define UTEXT(str) str

#endif

#endif
