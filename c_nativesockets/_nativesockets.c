#ifndef _NATIVESOCKETS_C
#define _NATIVESOCKETS_C

#include "include/_nativesockets.h"

#ifdef _WIN32

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <winsock2.h>

#else

#include <arpa/inet.h>

#endif

#include "_WinSock2.c"
#include "_NativeSocketAddress.c"
#include "_NativeSocket.c"

#endif
