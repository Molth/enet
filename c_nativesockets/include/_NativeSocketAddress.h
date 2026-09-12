#ifndef _NATIVESOCKETS_NATIVESOCKETADDRESS_H
#define _NATIVESOCKETS_NATIVESOCKETADDRESS_H

#include "_nativesocketpal.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /// <summary>
    ///     Represents a native socket address structure that can hold either an Ipv4 or Ipv6 address.
    /// </summary>
    /// <remarks>
    ///     The structure has a fixed size of 28 bytes, which is sufficient for
    ///     both Ipv4 (16 bytes) and Ipv6 (28 bytes) addresses.
    ///     It is a union to allow direct interpretation as
    ///     a byte buffer or as a properly aligned structure for native calls.
    /// </remarks>
    typedef union _NativeSocketAddress
    {
        /// <summary>
        ///     The raw buffer containing the socket address bytes.
        /// </summary>
        u8 buffer[28];

        /// <summary>
        ///     The address family and port number.
        /// </summary>
        struct
        {
            /// <summary>
            ///     The address family.
            /// </summary>
            u16 ss_family;

            /// <summary>
            ///     The port number in network byte order.
            /// </summary>
            u16 ss_port;
        };

        /// <summary>
        ///     Represents a native Ipv4 socket address structure (<c>sockaddr_in</c>).
        /// </summary>
        _sockaddr_in4 sin4;

        /// <summary>
        ///     Represents a native Ipv6 socket address structure (<c>sockaddr_in6</c>).
        /// </summary>
        _sockaddr_in6 sin6;
    } _NativeSocketAddress;

    /// <summary>
    ///     Gets whether the address is an Ipv4 address.
    /// </summary>
    /// <param name="socketAddress">The socket address.</param>
    /// <returns>true if the address is an Ipv4 address; otherwise, false.</returns>
    _NATIVESOCKETS_API bool _NativeSocketAddress_IsIpv4(const _NativeSocketAddress *socketAddress);

    /// <summary>
    ///     Gets whether the address is an Ipv6 address.
    /// </summary>
    /// <param name="socketAddress">The socket address.</param>
    /// <returns>true if the address is an Ipv6 address; otherwise, false.</returns>
    _NATIVESOCKETS_API bool _NativeSocketAddress_IsIpv6(const _NativeSocketAddress *socketAddress);

    /// <summary>
    ///     Gets the address family of the socket address.
    /// </summary>
    /// <param name="socketAddress">The socket address.</param>
    /// <returns>The address family value used by the current platform.</returns>
    _NATIVESOCKETS_API u16 _NativeSocketAddress_Get_Family(_NativeSocketAddress *socketAddress);

    /// <summary>
    ///     Sets the address family of the socket address.
    /// </summary>
    /// <param name="socketAddress">The socket address.</param>
    /// <param name="family">The address family value used by the current platform.</param>
    _NATIVESOCKETS_API void _NativeSocketAddress_Set_Family(_NativeSocketAddress *socketAddress, u16 family);

    /// <summary>
    ///     Gets the port number of the socket address.
    /// </summary>
    /// <param name="socketAddress">The socket address.</param>
    /// <returns>An unsigned integer value indicating the port number of the socket address.</returns>
    _NATIVESOCKETS_API u16 _NativeSocketAddress_Get_Port(_NativeSocketAddress *socketAddress);

    /// <summary>
    ///     Sets the port number of the socket address.
    /// </summary>
    /// <param name="socketAddress">The socket address.</param>
    /// <param name="port">An unsigned integer value indicating the port number of the socket address.</param>
    _NATIVESOCKETS_API void _NativeSocketAddress_Set_Port(_NativeSocketAddress *socketAddress, u16 port);

    /// <summary>
    ///     Gets the Ipv6 address scope identifier.
    /// </summary>
    /// <param name="socketAddress">The socket address.</param>
    /// <returns>An unsigned integer that specifies the scope identifier of the address.</returns>
    _NATIVESOCKETS_API u32 _NativeSocketAddress_Get_ScopeId(_NativeSocketAddress *socketAddress);

    /// <summary>
    ///     Sets the Ipv6 address scope identifier.
    /// </summary>
    /// <param name="socketAddress">The socket address.</param>
    /// <param name="scopeId">An unsigned integer that specifies the scope identifier of the address.</param>
    _NATIVESOCKETS_API void _NativeSocketAddress_Set_ScopeId(_NativeSocketAddress *socketAddress, u32 scopeId);

    /// <summary>
    ///     Gets whether the socket address is an Ipv4-mapped Ipv6 address.
    /// </summary>
    /// <param name="socketAddress">The socket address.</param>
    /// <returns>
    ///     Returns true if the socket address is an Ipv4-mapped Ipv6 address;
    ///     otherwise, false.
    /// </returns>
    _NATIVESOCKETS_API bool _NativeSocketAddress_IsIpv4MappedToIpv6(_NativeSocketAddress *socketAddress);

    /// <summary>
    ///     Gets the underlying buffer size of this.
    /// </summary>
    /// <param name="socketAddress">The socket address.</param>
    /// <returns>The underlying buffer size of this.</returns>
    _NATIVESOCKETS_API i32 _NativeSocketAddress_Get_Size(_NativeSocketAddress *socketAddress);

    /// <summary>
    ///     Gets the specified index element in the underlying buffer.
    /// </summary>
    /// <param name="socketAddress">The socket address.</param>
    /// <param name="index">The array index element of the desired information.</param>
    /// <returns>The value of the specified index element in the underlying buffer.</returns>
    _NATIVESOCKETS_API u8 _NativeSocketAddress_Get_Item(_NativeSocketAddress *socketAddress, i32 index);

    /// <summary>
    ///     Sets the specified index element in the underlying buffer.
    /// </summary>
    /// <param name="socketAddress">The socket address.</param>
    /// <param name="index">The array index element of the desired information.</param>
    /// <param name="value">The value of the specified index element in the underlying buffer.</param>
    _NATIVESOCKETS_API void _NativeSocketAddress_Set_Item(_NativeSocketAddress *socketAddress, i32 index, u8 value);

    /// <summary>
    ///     Maps the socket address object to an Ipv6 address.
    /// </summary>
    /// <param name="socketAddress">The socket address.</param>
    /// <param name="result">The resulting Ipv6 socket address.</param>
    _NATIVESOCKETS_API void _NativeSocketAddress_MapToIpv6(const _NativeSocketAddress *socketAddress, _NativeSocketAddress *result);

    /// <summary>
    ///     Maps the socket address object to an Ipv4 address.
    /// </summary>
    /// <param name="socketAddress">The socket address.</param>
    /// <param name="result">The resulting Ipv4 socket address.</param>
    _NATIVESOCKETS_API void _NativeSocketAddress_MapToIpv4(const _NativeSocketAddress *socketAddress, _NativeSocketAddress *result);

    /// <summary>
    ///     Converts an Ipv4 address and port into a <see cref="_NativeSocketAddress" />.
    /// </summary>
    /// <param name="socketAddress">The socket address to populate.</param>
    /// <param name="ip">The null-terminated Ipv4 address string.</param>
    /// <param name="port">The port number.</param>
    /// <returns><see cref="_SOCKET_ERROR_SUCCESS" /> if successful; otherwise an error code.</returns>
    _NATIVESOCKETS_API i32 _NativeSocketAddress_SetIpIpv4(_NativeSocketAddress *socketAddress, const char *ip, u16 port);

    /// <summary>
    ///     Converts an Ipv6 address, port, and scope id into a <see cref="_NativeSocketAddress" />.
    /// </summary>
    /// <param name="socketAddress">The socket address to populate.</param>
    /// <param name="ip">The null-terminated Ipv6 address string.</param>
    /// <param name="port">The port number.</param>
    /// <param name="scopeId">The scope identifier of the address.</param>
    /// <returns><see cref="_SOCKET_ERROR_SUCCESS" /> if successful; otherwise an error code.</returns>
    _NATIVESOCKETS_API i32 _NativeSocketAddress_SetIpIpv6(_NativeSocketAddress *socketAddress, const char *ip, u16 port, u32 scopeId);

    /// <summary>
    ///     Retrieves the address from a socket address structure.
    /// </summary>
    /// <param name="socketAddress">The socket address.</param>
    /// <param name="ip">A buffer to receive the null-terminated address string.</param>
    /// <param name="ipLength">The length of the buffer in bytes.</param>
    /// <returns><see cref="_SOCKET_ERROR_SUCCESS" /> on success; otherwise <see cref="_SOCKET_ERROR_FAULT" />.</returns>
    _NATIVESOCKETS_API i32 _NativeSocketAddress_GetIp(_NativeSocketAddress *socketAddress, char *ip, i32 ipLength);

    /// <summary>
    ///     Populates a <see cref="_NativeSocketAddress" /> by resolving the specified host name to an Ipv4 address.
    /// </summary>
    /// <param name="socketAddress">The socket address to populate.</param>
    /// <param name="hostName">The null-terminated host name string.</param>
    /// <param name="port">The port number.</param>
    /// <returns><see cref="_SOCKET_ERROR_SUCCESS" /> on success; otherwise an error code.</returns>
    _NATIVESOCKETS_API i32 _NativeSocketAddress_SetHostNameIpv4(_NativeSocketAddress *socketAddress, const char *hostName, u16 port);

    /// <summary>
    ///     Populates a <see cref="_NativeSocketAddress" /> by resolving the specified host name to an Ipv6 address.
    /// </summary>
    /// <param name="socketAddress">The socket address to populate.</param>
    /// <param name="hostName">The null-terminated host name string.</param>
    /// <param name="port">The port number.</param>
    /// <param name="scopeId">The scope identifier of the address.</param>
    /// <returns><see cref="_SOCKET_ERROR_SUCCESS" /> on success; otherwise an error code.</returns>
    _NATIVESOCKETS_API i32 _NativeSocketAddress_SetHostNameIpv6(_NativeSocketAddress *socketAddress, const char *hostName, u16 port, u32 scopeId);

    /// <summary>
    ///     Gets the host name (reverse DNS) from an address.
    /// </summary>
    /// <param name="socketAddress">The socket address.</param>
    /// <param name="hostName">A buffer to receive the null-terminated host name string.</param>
    /// <param name="hostNameLength">The length of the buffer in bytes.</param>
    /// <returns><see cref="_SOCKET_ERROR_SUCCESS" /> on success; otherwise <see cref="_SOCKET_ERROR_FAULT" />.</returns>
    _NATIVESOCKETS_API i32 _NativeSocketAddress_GetHostName(_NativeSocketAddress *socketAddress, char *hostName, i32 hostNameLength);

#ifdef __cplusplus
}
#endif

#endif
