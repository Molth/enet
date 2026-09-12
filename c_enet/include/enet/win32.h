/**
 @file  win32.h
 @brief ENet Win32 header
*/
#ifndef __ENET_WIN32_H__
#define __ENET_WIN32_H__

#ifdef _MSC_VER
#ifdef ENET_BUILDING_LIB
#pragma warning(disable : 4267) // size_t to int conversion
#pragma warning(disable : 4244) // 64bit to 32bit int
#pragma warning(disable : 4018) // signed/unsigned mismatch
#pragma warning(disable : 4146) // unary minus operator applied to unsigned type
#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE
#endif
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#endif
#endif

#include <stdlib.h>
#include "../../c_nativesockets/include/_nativesockets.h"

/**
  Represents a native socket handle with its associated address family.
*/
typedef struct _ENetSocket
{
  /** The underlying native socket. */
  _NativeSocket inner;
} ENetSocket;

/**
  Portable internet address structure.
  No fields should be modified unless otherwise stated.
*/
typedef struct _ENetAddress
{
  /** The underlying native socket address. */
  _NativeSocketAddress inner;
} ENetAddress;

/** The sentinel value used to indicate the absence of a socket. */
#define ENET_SOCKET_NULL ((ENetSocket){.inner = {(isize) - 1, 0}})
/** Determines whether the given socket is the null socket. */
#define ENET_SOCKET_IS_NULL(socket) ((socket).inner.handle == (isize) - 1)

#ifdef _WIN32
#include <winsock2.h>

/** Converts a 16-bit host-order value to network byte order (big endian). */
#define ENET_HOST_TO_NET_16(value) (htons(value))
/** Converts a 32-bit host-order value to network byte order (big endian). */
#define ENET_HOST_TO_NET_32(value) (htonl(value))

/** Converts a 16-bit network-order value (big endian) to host byte order. */
#define ENET_NET_TO_HOST_16(value) (ntohs(value))
/** Converts a 32-bit network-order value (big endian) to host byte order. */
#define ENET_NET_TO_HOST_32(value) (ntohl(value))

/** The calling convention used for ENet callback functions. */
#define ENET_CALLBACK __cdecl

#ifdef ENET_DLL
#ifdef ENET_BUILDING_LIB
#define ENET_API __declspec(dllexport)
#else
#define ENET_API __declspec(dllimport)
#endif /* ENET_BUILDING_LIB */
#else  /* !ENET_DLL */
#define ENET_API extern
#endif /* ENET_DLL */
#else
#include <arpa/inet.h>

/** Converts a 16-bit host-order value to network byte order (big endian). */
#define ENET_HOST_TO_NET_16(value) (htons(value))
/** Converts a 32-bit host-order value to network byte order (big endian). */
#define ENET_HOST_TO_NET_32(value) (htonl(value))

/** Converts a 16-bit network-order value (big endian) to host byte order. */
#define ENET_NET_TO_HOST_16(value) (ntohs(value))
/** Converts a 32-bit network-order value (big endian) to host byte order. */
#define ENET_NET_TO_HOST_32(value) (ntohl(value))

/** The calling convention used for ENet callback functions. */
#define ENET_CALLBACK
#define ENET_API extern
#endif

/**
  Describes a contiguous block of packet data with its length.
*/
typedef struct
{
  /** The number of valid bytes in the buffer. */
  size_t dataLength;

  /** Pointer to the start of the data. */
  void *data;
} ENetBuffer;

#endif /* __ENET_WIN32_H__ */
