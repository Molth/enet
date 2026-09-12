#ifndef _NATIVESOCKETS_NATIVESOCKETADDRESS_C
#define _NATIVESOCKETS_NATIVESOCKETADDRESS_C

#include "include/_nativesockets.h"

#include <string.h>

/// <summary>
///     Copies an Ipv4 socket address structure into a <see cref="_NativeSocketAddress" />,
///     converting the port to network byte order and zeroing the padding.
/// </summary>
static void _CopyFromIpv4(_NativeSocketAddress *socketAddress, _sockaddr_in4 *native, u16 port)
{
    native->sin4_family = _ADDRESS_FAMILY_INTER_NETWORK_V4;
    native->sin4_port = _HOST_TO_NET_16(port);
    memcpy(socketAddress->buffer, native, 8);
    memset(socketAddress->buffer + 8, 0, 20);
}

/// <summary>
///     Copies an Ipv6 socket address structure into a <see cref="_NativeSocketAddress" />,
///     converting the port to network byte order and setting the flow info and scope id.
/// </summary>
static void _CopyFromIpv6(_NativeSocketAddress *socketAddress, _sockaddr_in6 *native, u16 port, u32 scopeId)
{
    native->sin6_family = _ADDRESS_FAMILY_INTER_NETWORK_V6;
    native->sin6_port = _HOST_TO_NET_16(port);
    native->sin6_flowinfo = 0;
    native->sin6_scope_id = scopeId;
    memcpy(socketAddress->buffer, native, sizeof(_sockaddr_in6));
}

/// <summary>
///     Gets whether the address is an Ipv4 address.
/// </summary>
/// <param name="socketAddress">The socket address.</param>
/// <returns>true if the address is an Ipv4 address; otherwise, false.</returns>
bool _NativeSocketAddress_IsIpv4(const _NativeSocketAddress *socketAddress)
{
    return socketAddress->ss_family == _ADDRESS_FAMILY_INTER_NETWORK_V4;
}

/// <summary>
///     Gets whether the address is an Ipv6 address.
/// </summary>
/// <param name="socketAddress">The socket address.</param>
/// <returns>true if the address is an Ipv6 address; otherwise, false.</returns>
bool _NativeSocketAddress_IsIpv6(const _NativeSocketAddress *socketAddress)
{
    return socketAddress->ss_family == _ADDRESS_FAMILY_INTER_NETWORK_V6;
}

/// <summary>
///     Gets the address family of the socket address.
/// </summary>
/// <param name="socketAddress">The socket address.</param>
/// <returns>The address family value used by the current platform.</returns>
u16 _NativeSocketAddress_Get_Family(_NativeSocketAddress *socketAddress)
{
    return socketAddress->ss_family;
}

/// <summary>
///     Sets the address family of the socket address.
/// </summary>
/// <param name="socketAddress">The socket address.</param>
/// <param name="family">The address family value used by the current platform.</param>
void _NativeSocketAddress_Set_Family(_NativeSocketAddress *socketAddress, u16 family)
{
    socketAddress->ss_family = family;
}

/// <summary>
///     Gets the port number of the socket address.
/// </summary>
/// <param name="socketAddress">The socket address.</param>
/// <returns>An unsigned integer value indicating the port number of the socket address.</returns>
u16 _NativeSocketAddress_Get_Port(_NativeSocketAddress *socketAddress)
{
    return _NET_TO_HOST_16(socketAddress->ss_port);
}

/// <summary>
///     Sets the port number of the socket address.
/// </summary>
/// <param name="socketAddress">The socket address.</param>
/// <param name="port">An unsigned integer value indicating the port number of the socket address.</param>
void _NativeSocketAddress_Set_Port(_NativeSocketAddress *socketAddress, u16 port)
{
    socketAddress->ss_port = _HOST_TO_NET_16(port);
}

/// <summary>
///     Gets the Ipv6 address scope identifier.
/// </summary>
/// <param name="socketAddress">The socket address.</param>
/// <returns>An unsigned integer that specifies the scope identifier of the address.</returns>
u32 _NativeSocketAddress_Get_ScopeId(_NativeSocketAddress *socketAddress)
{
    return socketAddress->sin6.sin6_scope_id;
}

/// <summary>
///     Sets the Ipv6 address scope identifier.
/// </summary>
/// <param name="socketAddress">The socket address.</param>
/// <param name="scopeId">An unsigned integer that specifies the scope identifier of the address.</param>
void _NativeSocketAddress_Set_ScopeId(_NativeSocketAddress *socketAddress, u32 scopeId)
{
    socketAddress->sin6.sin6_scope_id = scopeId;
}

/// <summary>
///     Gets whether the socket address is an Ipv4-mapped Ipv6 address.
/// </summary>
/// <param name="socketAddress">The socket address.</param>
/// <returns>
///     Returns true if the socket address is an Ipv4-mapped Ipv6 address;
///     otherwise, false.
/// </returns>
bool _NativeSocketAddress_IsIpv4MappedToIpv6(_NativeSocketAddress *socketAddress)
{
    if (!_NativeSocketAddress_IsIpv6(socketAddress))
    {
        return false;
    }
    return _IsIpv4MappedToIpv6(socketAddress->sin6.sin6_addr);
}

/// <summary>
///     Gets the underlying buffer size of this.
/// </summary>
/// <param name="socketAddress">The socket address.</param>
/// <returns>The underlying buffer size of this.</returns>
i32 _NativeSocketAddress_Get_Size(_NativeSocketAddress *socketAddress)
{
    if (_NativeSocketAddress_IsIpv6(socketAddress))
    {
        return 28;
    }
    if (_NativeSocketAddress_IsIpv4(socketAddress))
    {
        return 16;
    }
    return 0;
}

/// <summary>
///     Gets the specified index element in the underlying buffer.
/// </summary>
/// <param name="socketAddress">The socket address.</param>
/// <param name="index">The array index element of the desired information.</param>
/// <returns>The value of the specified index element in the underlying buffer.</returns>
u8 _NativeSocketAddress_Get_Item(_NativeSocketAddress *socketAddress, i32 index)
{
    return socketAddress->buffer[index];
}

/// <summary>
///     Sets the specified index element in the underlying buffer.
/// </summary>
/// <param name="socketAddress">The socket address.</param>
/// <param name="index">The array index element of the desired information.</param>
/// <param name="value">The value of the specified index element in the underlying buffer.</param>
void _NativeSocketAddress_Set_Item(_NativeSocketAddress *socketAddress, i32 index, u8 value)
{
    socketAddress->buffer[index] = value;
}

/// <summary>
///     Maps the socket address object to an Ipv6 address.
/// </summary>
/// <param name="socketAddress">The socket address.</param>
/// <param name="result">The resulting Ipv6 socket address.</param>
void _NativeSocketAddress_MapToIpv6(const _NativeSocketAddress *socketAddress, _NativeSocketAddress *result)
{
    memcpy(result, socketAddress, sizeof(_NativeSocketAddress));
    if (_NativeSocketAddress_IsIpv6(socketAddress))
    {
        return;
    }
    result->ss_family = _ADDRESS_FAMILY_INTER_NETWORK_V6;
    _MapIpv4ToIpv6(result->sin6.sin6_addr, result->sin4.sin4_addr);
    result->sin6.sin6_flowinfo = 0;
    result->sin6.sin6_scope_id = 0;
}

/// <summary>
///     Maps the socket address object to an Ipv4 address.
/// </summary>
/// <param name="socketAddress">The socket address.</param>
/// <param name="result">The resulting Ipv4 socket address.</param>
void _NativeSocketAddress_MapToIpv4(const _NativeSocketAddress *socketAddress, _NativeSocketAddress *result)
{
    memcpy(result, socketAddress, sizeof(_NativeSocketAddress));
    if (_NativeSocketAddress_IsIpv4(socketAddress))
    {
        return;
    }
    result->ss_family = _ADDRESS_FAMILY_INTER_NETWORK_V4;
    result->sin4.sin4_addr = _ReadU32(result->sin6.sin6_addr + 12);
    memset(result->buffer + 8, 0, 20);
}

/// <summary>
///     Converts an Ipv4 address and port into a <see cref="_NativeSocketAddress" />.
/// </summary>
/// <param name="socketAddress">The socket address to populate.</param>
/// <param name="ip">The null-terminated Ipv4 address string.</param>
/// <param name="port">The port number.</param>
/// <returns><see cref="_SOCKET_ERROR_SUCCESS" /> if successful; otherwise an error code.</returns>
i32 _NativeSocketAddress_SetIpIpv4(_NativeSocketAddress *socketAddress, const char *ip, u16 port)
{
    _sockaddr_in4 native;
    i32 error = _SetIpIpv4(&native, (const u8 *)ip, (i32)strlen(ip) + 1);
    if (error != _SOCKET_ERROR_SUCCESS)
    {
        return error;
    }
    _CopyFromIpv4(socketAddress, &native, port);
    return _SOCKET_ERROR_SUCCESS;
}

/// <summary>
///     Converts an Ipv6 address, port, and scope id into a <see cref="_NativeSocketAddress" />.
/// </summary>
/// <param name="socketAddress">The socket address to populate.</param>
/// <param name="ip">The null-terminated Ipv6 address string.</param>
/// <param name="port">The port number.</param>
/// <param name="scopeId">The scope identifier of the address.</param>
/// <returns><see cref="_SOCKET_ERROR_SUCCESS" /> if successful; otherwise an error code.</returns>
i32 _NativeSocketAddress_SetIpIpv6(_NativeSocketAddress *socketAddress, const char *ip, u16 port, u32 scopeId)
{
    _sockaddr_in6 native;
    i32 error = _SetIpIpv6(&native, (const u8 *)ip, (i32)strlen(ip) + 1);
    if (error != _SOCKET_ERROR_SUCCESS)
    {
        return error;
    }
    _CopyFromIpv6(socketAddress, &native, port, scopeId);
    return _SOCKET_ERROR_SUCCESS;
}

/// <summary>
///     Retrieves the address from a socket address structure.
/// </summary>
/// <param name="socketAddress">The socket address.</param>
/// <param name="ip">A buffer to receive the null-terminated address string.</param>
/// <param name="ipLength">The length of the buffer in bytes.</param>
/// <returns><see cref="_SOCKET_ERROR_SUCCESS" /> on success; otherwise <see cref="_SOCKET_ERROR_FAULT" />.</returns>
i32 _NativeSocketAddress_GetIp(_NativeSocketAddress *socketAddress, char *ip, i32 ipLength)
{
    if (_NativeSocketAddress_IsIpv4(socketAddress))
    {
        return _GetIpIpv4(&socketAddress->sin4, (u8 *)ip, ipLength);
    }
    return _GetIpIpv6(&socketAddress->sin6, (u8 *)ip, ipLength);
}

/// <summary>
///     Populates a <see cref="_NativeSocketAddress" /> by resolving the specified host name to an Ipv4 address.
/// </summary>
/// <param name="socketAddress">The socket address to populate.</param>
/// <param name="hostName">The null-terminated host name string.</param>
/// <param name="port">The port number.</param>
/// <returns><see cref="_SOCKET_ERROR_SUCCESS" /> on success; otherwise an error code.</returns>
i32 _NativeSocketAddress_SetHostNameIpv4(_NativeSocketAddress *socketAddress, const char *hostName, u16 port)
{
    _sockaddr_in4 native;
    i32 error = _SetHostNameIpv4(&native, (const u8 *)hostName, (i32)strlen(hostName) + 1);
    if (error != _SOCKET_ERROR_SUCCESS)
    {
        return error;
    }
    _CopyFromIpv4(socketAddress, &native, port);
    return _SOCKET_ERROR_SUCCESS;
}

/// <summary>
///     Populates a <see cref="_NativeSocketAddress" /> by resolving the specified host name to an Ipv6 address.
/// </summary>
/// <param name="socketAddress">The socket address to populate.</param>
/// <param name="hostName">The null-terminated host name string.</param>
/// <param name="port">The port number.</param>
/// <param name="scopeId">The scope identifier of the address.</param>
/// <returns><see cref="_SOCKET_ERROR_SUCCESS" /> on success; otherwise an error code.</returns>
i32 _NativeSocketAddress_SetHostNameIpv6(_NativeSocketAddress *socketAddress, const char *hostName, u16 port, u32 scopeId)
{
    _sockaddr_in6 native;
    i32 error = _SetHostNameIpv6(&native, (const u8 *)hostName, (i32)strlen(hostName) + 1);
    if (error != _SOCKET_ERROR_SUCCESS)
    {
        return error;
    }
    _CopyFromIpv6(socketAddress, &native, port, scopeId);
    return _SOCKET_ERROR_SUCCESS;
}

/// <summary>
///     Gets the host name (reverse DNS) from an address.
/// </summary>
/// <param name="socketAddress">The socket address.</param>
/// <param name="hostName">A buffer to receive the null-terminated host name string.</param>
/// <param name="hostNameLength">The length of the buffer in bytes.</param>
/// <returns><see cref="_SOCKET_ERROR_SUCCESS" /> on success; otherwise <see cref="_SOCKET_ERROR_FAULT" />.</returns>
i32 _NativeSocketAddress_GetHostName(_NativeSocketAddress *socketAddress, char *hostName, i32 hostNameLength)
{
    if (_NativeSocketAddress_IsIpv4(socketAddress))
    {
        return _GetHostNameIpv4(&socketAddress->sin4, (u8 *)hostName, hostNameLength);
    }
    return _GetHostNameIpv6(&socketAddress->sin6, (u8 *)hostName, hostNameLength);
}

#endif
