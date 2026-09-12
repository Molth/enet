#ifndef _NATIVESOCKETS_H
#define _NATIVESOCKETS_H

#include "_nativesocketpal.h"

#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef _WIN32

#ifdef _NATIVESOCKETS_BUILD_DLL
#define _NATIVESOCKETS_API __declspec(dllexport)
#else
#define _NATIVESOCKETS_API
#endif

#else
#define _NATIVESOCKETS_API __attribute__((visibility("default")))
#endif

#include "_WinSock2.h"
#include "_NativeSocketAddress.h"
#include "_NativeSocket.h"

#ifdef __cplusplus
}
#endif

#endif
