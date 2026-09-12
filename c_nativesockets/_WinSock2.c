#ifndef _NATIVESOCKETS_WINSOCK2_C
#define _NATIVESOCKETS_WINSOCK2_C

#include "include/_nativesockets.h"

#include <string.h>

#ifdef _WIN32

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <winsock2.h>

#else

#include <arpa/inet.h>

#endif

/// <summary>
///     Reads a 16-bit unsigned integer from an unaligned memory location.
/// </summary>
static u16 _ReadU16(const void *p)
{
    u16 value;
    memcpy(&value, p, sizeof(value));
    return value;
}

/// <summary>
///     Writes a 16-bit unsigned integer to an unaligned memory location.
/// </summary>
static void _WriteU16(void *p, u16 value)
{
    memcpy(p, &value, sizeof(value));
}

/// <summary>
///     Reads a 32-bit unsigned integer from an unaligned memory location.
/// </summary>
static u32 _ReadU32(const void *p)
{
    u32 value;
    memcpy(&value, p, sizeof(value));
    return value;
}

/// <summary>
///     Writes a 32-bit unsigned integer to an unaligned memory location.
/// </summary>
static void _WriteU32(void *p, u32 value)
{
    memcpy(p, &value, sizeof(value));
}

/// <summary>
///     Converts a 16-bit unsigned integer from host byte order to network byte order (big-endian).
/// </summary>
/// <param name="host">The value in host byte order.</param>
/// <returns>The value in network byte order.</returns>
u16 _HOST_TO_NET_16(u16 host)
{
    return htons(host);
}

/// <summary>
///     Converts a 16-bit unsigned integer from network byte order (big-endian) to host byte order.
/// </summary>
/// <param name="network">The value in network byte order.</param>
/// <returns>The value in host byte order.</returns>
u16 _NET_TO_HOST_16(u16 network)
{
    return ntohs(network);
}

/// <summary>
///     Gets whether the ip address is an Ipv4-mapped Ipv6 address.
/// </summary>
/// <param name="sin6_addr">The 12-byte span containing the Ipv4-mapped Ipv6 address data.</param>
/// <returns>
///     Returns true if the ip address is an Ipv4-mapped Ipv6 address;
///     otherwise, false.
/// </returns>
bool _IsIpv4MappedToIpv6(const u8 *sin6_addr)
{
    static const u8 prefix[12] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xFF, 0xFF };
    return memcmp(sin6_addr, prefix, sizeof(prefix)) == 0;
}

/// <summary>
///     Writes the 12-byte prefix to an Ipv6 address.
/// </summary>
/// <param name="sin6_addr">The 12-byte span containing the Ipv4-mapped Ipv6 address data.</param>
void _WriteIpv6Prefix(u8 *sin6_addr)
{
    static const u8 prefix[12] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xFF, 0xFF };
    memcpy(sin6_addr, prefix, sizeof(prefix));
}

/// <summary>
///     Maps the Ipv4 address to an Ipv6 address.
/// </summary>
/// <param name="sin6_addr">The 16-byte span containing the Ipv4-mapped Ipv6 address data.</param>
/// <param name="sin4_addr">The 4-byte span containing the Ipv4 address data.</param>
void _MapIpv4ToIpv6(u8 *sin6_addr, u32 sin4_addr)
{
    _WriteIpv6Prefix(sin6_addr);
    _WriteU32(sin6_addr + 12, sin4_addr);
}

#endif
