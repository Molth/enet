#ifndef _NATIVESOCKETS_WINSOCK2_H
#define _NATIVESOCKETS_WINSOCK2_H

#include "_nativesocketpal.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /// <summary>
    ///     Maximum length of a host name string (including the null terminator)
    ///     for use with <c>getnameinfo</c> and similar APIs.
    /// </summary>
#define _NI_MAXHOST 1025

    /// <summary>
    ///     Converts a 16-bit unsigned integer from host byte order to network byte order (big-endian).
    /// </summary>
    /// <param name="host">The value in host byte order.</param>
    /// <returns>The value in network byte order.</returns>
    _NATIVESOCKETS_API u16 _HOST_TO_NET_16(u16 host);

    /// <summary>
    ///     Converts a 16-bit unsigned integer from network byte order (big-endian) to host byte order.
    /// </summary>
    /// <param name="network">The value in network byte order.</param>
    /// <returns>The value in host byte order.</returns>
    _NATIVESOCKETS_API u16 _NET_TO_HOST_16(u16 network);

    /// <summary>
    ///     Gets whether the ip address is an Ipv4-mapped Ipv6 address.
    /// </summary>
    /// <param name="sin6_addr">The 12-byte span containing the Ipv4-mapped Ipv6 address data.</param>
    /// <returns>
    ///     Returns true if the ip address is an Ipv4-mapped Ipv6 address;
    ///     otherwise, false.
    /// </returns>
    _NATIVESOCKETS_API bool _IsIpv4MappedToIpv6(const u8 *sin6_addr);

    /// <summary>
    ///     Writes the 12-byte prefix to an Ipv6 address.
    /// </summary>
    /// <param name="sin6_addr">The 12-byte span containing the Ipv4-mapped Ipv6 address data.</param>
    _NATIVESOCKETS_API void _WriteIpv6Prefix(u8 *sin6_addr);

    /// <summary>
    ///     Maps the Ipv4 address to an Ipv6 address.
    /// </summary>
    /// <param name="sin6_addr">The 16-byte span containing the Ipv4-mapped Ipv6 address data.</param>
    /// <param name="sin4_addr">The 4-byte span containing the Ipv4 address data.</param>
    _NATIVESOCKETS_API void _MapIpv4ToIpv6(u8 *sin6_addr, u32 sin4_addr);

#ifdef __cplusplus
}
#endif

#endif
