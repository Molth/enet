#ifndef _NATIVESOCKETS_NATIVESOCKET_H
#define _NATIVESOCKETS_NATIVESOCKET_H

#include "_nativesocketpal.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /// <summary>
    ///     The managed <c>AddressFamily.InterNetwork</c> value (2).
    /// </summary>
#define _SOCKET_FAMILY_INTER_NETWORK 2

    /// <summary>
    ///     The managed <c>AddressFamily.InterNetworkV6</c> value (23).
    /// </summary>
#define _SOCKET_FAMILY_INTER_NETWORK_V6 23

    /// <summary>
    ///     Represents a native socket handle with its associated address family.
    /// </summary>
    typedef struct _NativeSocket
    {
        /// <summary>
        ///     The native socket handle.
        /// </summary>
        isize handle;

        /// <summary>
        ///     The managed address family of the socket.
        /// </summary>
        i32 family;
    } _NativeSocket;

    /// <summary>
    ///     Creates a native socket handle.
    /// </summary>
    /// <param name="ipv6">true to create an Ipv6 socket; false for Ipv4.</param>
    /// <param name="result">The native socket handle, or -1 on error.</param>
    /// <returns><see cref="_SOCKET_ERROR_SUCCESS" /> on success; otherwise <see cref="_SOCKET_ERROR_SOCKET_ERROR" />.</returns>
    _NATIVESOCKETS_API i32 _NativeSockets_Create(i32 ipv6, _NativeSocket *result);

    /// <summary>
    ///     Performs application-defined tasks associated with freeing,
    ///     releasing, or resetting unmanaged resources.
    /// </summary>
    /// <param name="socket">The socket.</param>
    /// <returns><see cref="_SOCKET_ERROR_SUCCESS" /> on success; otherwise an error code.</returns>
    _NATIVESOCKETS_API i32 _NativeSockets_Dispose(_NativeSocket *socket);

    /// <summary>
    ///     Gets the native socket handle.
    /// </summary>
    /// <param name="socket">The socket.</param>
    /// <returns>The native socket handle.</returns>
    _NATIVESOCKETS_API isize _NativeSockets_Get_Handle(_NativeSocket *socket);

    /// <summary>
    ///     Gets the address family of the socket.
    /// </summary>
    /// <param name="socket">The socket.</param>
    /// <returns>The managed address family of the socket.</returns>
    _NATIVESOCKETS_API i32 _NativeSockets_Get_Family(_NativeSocket *socket);

    /// <summary>
    ///     Gets a value indicating whether the socket uses Ipv4.
    /// </summary>
    /// <param name="socket">The socket.</param>
    /// <returns>true if the socket uses Ipv4; otherwise, false.</returns>
    _NATIVESOCKETS_API bool _NativeSockets_IsIpv4(_NativeSocket *socket);

    /// <summary>
    ///     Gets a value indicating whether the socket uses Ipv6.
    /// </summary>
    /// <param name="socket">The socket.</param>
    /// <returns>true if the socket uses Ipv6; otherwise, false.</returns>
    _NATIVESOCKETS_API bool _NativeSockets_IsIpv6(_NativeSocket *socket);

    /// <summary>
    ///     Enables or disables dual-mode (Ipv6/Ipv4) on an Ipv6 socket.
    /// </summary>
    /// <param name="socket">The socket.</param>
    /// <param name="dualMode">true to enable dual-mode; false to disable.</param>
    /// <returns><see cref="_SOCKET_ERROR_SUCCESS" /> on success; otherwise an error code.</returns>
    _NATIVESOCKETS_API i32 _NativeSockets_SetDualMode(_NativeSocket *socket, i32 dualMode);

    /// <summary>
    ///     Binds a socket to an address.
    /// </summary>
    /// <param name="socket">The socket.</param>
    /// <param name="socketAddress">Pointer to the address structure; may be NULL to bind to any address.</param>
    /// <returns><see cref="_SOCKET_ERROR_SUCCESS" /> on success; otherwise an error code.</returns>
    _NATIVESOCKETS_API i32 _NativeSockets_Bind(_NativeSocket *socket, const _NativeSocketAddress *socketAddress);

    /// <summary>
    ///     Connects a socket to an endpoint.
    /// </summary>
    /// <param name="socket">The socket.</param>
    /// <param name="socketAddress">Pointer to the address structure.</param>
    /// <returns><see cref="_SOCKET_ERROR_SUCCESS" /> on success; otherwise an error code.</returns>
    _NATIVESOCKETS_API i32 _NativeSockets_Connect(_NativeSocket *socket, const _NativeSocketAddress *socketAddress);

    /// <summary>
    ///     Sets a socket option.
    /// </summary>
    /// <param name="socket">The socket.</param>
    /// <param name="level">The option level.</param>
    /// <param name="name">The option name.</param>
    /// <param name="value">Pointer to the option value.</param>
    /// <param name="length">The length of the option value in bytes.</param>
    /// <returns><see cref="_SOCKET_ERROR_SUCCESS" /> on success; otherwise an error code.</returns>
    _NATIVESOCKETS_API i32 _NativeSockets_SetOption(_NativeSocket *socket, i32 level, i32 name, const u8 *value, i32 length);

    /// <summary>
    ///     Gets a socket option.
    /// </summary>
    /// <param name="socket">The socket.</param>
    /// <param name="level">The option level.</param>
    /// <param name="name">The option name.</param>
    /// <param name="value">Pointer to a buffer to receive the option value.</param>
    /// <param name="length">Pointer to the length of the buffer; on output, the actual size of the option.</param>
    /// <returns><see cref="_SOCKET_ERROR_SUCCESS" /> on success; otherwise an error code.</returns>
    _NATIVESOCKETS_API i32 _NativeSockets_GetOption(_NativeSocket *socket, i32 level, i32 name, u8 *value, i32 *length);

    /// <summary>
    ///     Sets a socket's blocking mode.
    /// </summary>
    /// <param name="socket">The socket.</param>
    /// <param name="blocking">true for blocking; false for non-blocking.</param>
    /// <returns><see cref="_SOCKET_ERROR_SUCCESS" /> on success; otherwise an error code.</returns>
    _NATIVESOCKETS_API i32 _NativeSockets_SetBlocking(_NativeSocket *socket, i32 blocking);

    /// <summary>
    ///     Polls a socket for pending events.
    /// </summary>
    /// <param name="socket">The socket.</param>
    /// <param name="microseconds">The timeout in microseconds.</param>
    /// <param name="mode">The select mode.</param>
    /// <param name="status">When this method returns, contains true if the socket is ready, false otherwise.</param>
    /// <returns><see cref="_SOCKET_ERROR_SUCCESS" /> on success; otherwise an error code.</returns>
    _NATIVESOCKETS_API i32 _NativeSockets_Poll(_NativeSocket *socket, i32 microseconds, i32 mode, i32 *status);

    /// <summary>
    ///     Polls a socket for pending events.
    /// </summary>
    /// <param name="socket">The socket.</param>
    /// <param name="microseconds">The timeout in microseconds.</param>
    /// <param name="inFlags">The select mode.</param>
    /// <param name="outFlags">When this method returns, contains the flags returned by the poll operation.</param>
    /// <returns><see cref="_SOCKET_ERROR_SUCCESS" /> on success; otherwise an error code.</returns>
    _NATIVESOCKETS_API i32 _NativeSockets_PollFlags(_NativeSocket *socket, i32 microseconds, i32 inFlags, i32 *outFlags);

    /// <summary>
    ///     Sends data on a connected socket.
    /// </summary>
    /// <param name="socket">The socket.</param>
    /// <param name="buffer">Pointer to the data buffer.</param>
    /// <param name="length">Length of the buffer in bytes.</param>
    /// <param name="socketFlags">A bitwise combination of the socket flags values.</param>
    /// <returns>The number of bytes sent, or -1 on error.</returns>
    _NATIVESOCKETS_API i32 _NativeSockets_Send(_NativeSocket *socket, const void *buffer, i32 length, i32 socketFlags);

    /// <summary>
    ///     Sends data to an endpoint.
    /// </summary>
    /// <param name="socket">The socket.</param>
    /// <param name="buffer">Pointer to the data buffer.</param>
    /// <param name="length">Length of the buffer.</param>
    /// <param name="socketFlags">A bitwise combination of the socket flags values.</param>
    /// <param name="socketAddress">Pointer to the destination socket address structure.</param>
    /// <returns>The number of bytes sent, or -1 on error.</returns>
    _NATIVESOCKETS_API i32 _NativeSockets_SendTo(_NativeSocket *socket, const void *buffer, i32 length, i32 socketFlags, const _NativeSocketAddress *socketAddress);

    /// <summary>
    ///     Receives data on a connected socket.
    /// </summary>
    /// <param name="socket">The socket.</param>
    /// <param name="buffer">Pointer to the receive buffer.</param>
    /// <param name="length">Length of the buffer.</param>
    /// <param name="socketFlags">A bitwise combination of the socket flags values.</param>
    /// <returns>The number of bytes received, or -1 on error.</returns>
    _NATIVESOCKETS_API i32 _NativeSockets_Receive(_NativeSocket *socket, void *buffer, i32 length, i32 socketFlags);

    /// <summary>
    ///     Receives data from an endpoint, filling the provided address structure.
    /// </summary>
    /// <param name="socket">The socket.</param>
    /// <param name="buffer">Pointer to the receive buffer.</param>
    /// <param name="length">Length of the buffer.</param>
    /// <param name="socketFlags">A bitwise combination of the socket flags values.</param>
    /// <param name="socketAddress">Pointer to the sender's socket address structure; may be NULL.</param>
    /// <returns>The number of bytes received, or -1 on error.</returns>
    _NATIVESOCKETS_API i32 _NativeSockets_ReceiveFrom(_NativeSocket *socket, void *buffer, i32 length, i32 socketFlags, _NativeSocketAddress *socketAddress);

    /// <summary>
    ///     Sends data from multiple buffers on a connected socket.
    /// </summary>
    /// <param name="socket">The socket.</param>
    /// <param name="buffers">Pointer to an array of <see cref="_NativeIoSlice" /> structures.</param>
    /// <param name="bufferCount">The number of buffers.</param>
    /// <param name="socketFlags">A bitwise combination of the socket flags values.</param>
    /// <returns>The number of bytes sent, or -1 on error.</returns>
    _NATIVESOCKETS_API i32 _NativeSockets_SendVectored(_NativeSocket *socket, const _NativeIoSlice *buffers, i32 bufferCount, i32 socketFlags);

    /// <summary>
    ///     Sends data from multiple buffers to an endpoint.
    /// </summary>
    /// <param name="socket">The socket.</param>
    /// <param name="buffers">Pointer to an array of <see cref="_NativeIoSlice" /> structures.</param>
    /// <param name="bufferCount">The number of buffers.</param>
    /// <param name="socketFlags">A bitwise combination of the socket flags values.</param>
    /// <param name="socketAddress">Pointer to the destination socket address structure.</param>
    /// <returns>The number of bytes sent, or -1 on error.</returns>
    _NATIVESOCKETS_API i32 _NativeSockets_SendToVectored(_NativeSocket *socket, const _NativeIoSlice *buffers, i32 bufferCount, i32 socketFlags, const _NativeSocketAddress *socketAddress);

    /// <summary>
    ///     Receives data into multiple buffers on a connected socket.
    /// </summary>
    /// <param name="socket">The socket.</param>
    /// <param name="buffers">Pointer to an array of <see cref="_NativeIoSlice" /> structures.</param>
    /// <param name="bufferCount">The number of buffers.</param>
    /// <param name="inOutFlags">When this method returns, contains the flags returned by the receive operation.</param>
    /// <returns>The number of bytes received, or -1 on error.</returns>
    _NATIVESOCKETS_API i32 _NativeSockets_ReceiveVectored(_NativeSocket *socket, _NativeIoSlice *buffers, i32 bufferCount, i32 *inOutFlags);

    /// <summary>
    ///     Receives data into multiple buffers from an endpoint.
    /// </summary>
    /// <param name="socket">The socket.</param>
    /// <param name="buffers">Pointer to an array of <see cref="_NativeIoSlice" /> structures.</param>
    /// <param name="bufferCount">The number of buffers.</param>
    /// <param name="inOutFlags">When this method returns, contains the flags returned by the receive operation.</param>
    /// <param name="socketAddress">Pointer to the sender's socket address structure; may be NULL.</param>
    /// <returns>The number of bytes received, or -1 on error.</returns>
    _NATIVESOCKETS_API i32 _NativeSockets_ReceiveFromVectored(_NativeSocket *socket, _NativeIoSlice *buffers, i32 bufferCount, i32 *inOutFlags, _NativeSocketAddress *socketAddress);

    /// <summary>
    ///     Gets the local name (address) of a socket.
    /// </summary>
    /// <param name="socket">The socket.</param>
    /// <param name="socketAddress">Pointer to the socket address structure to receive the name.</param>
    /// <returns><see cref="_SOCKET_ERROR_SUCCESS" /> on success; otherwise an error code.</returns>
    _NATIVESOCKETS_API i32 _NativeSockets_GetName(_NativeSocket *socket, _NativeSocketAddress *socketAddress);

#ifdef __cplusplus
}
#endif

#endif
