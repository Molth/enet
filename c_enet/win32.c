/**
 @file  win32.c
 @brief ENet system specific functions (NativeSockets backend)
*/
#define ENET_BUILDING_LIB 1
#include "enet/enet.h"
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <stdint.h>
#include <time.h>
#endif

static enet_uint32 timeBase = 0;

/** Returns the current monotonic wall-clock time in milliseconds.
 */
static enet_uint32
_timeGetMilliseconds(void)
{
#ifdef _WIN32
    return (enet_uint32)GetTickCount();
#else
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (enet_uint32)((uint64_t)ts.tv_sec * 1000u + (uint64_t)ts.tv_nsec / 1000000u);
#endif
}

/** Converts ENet buffers to native scatter/gather I/O vectors.
    @param iovecs The destination native I/O vector array.
    @param buffers The source ENet buffer array.
    @param bufferCount The number of buffers.
*/
static void
_BuildNativeIoSlices(_NativeIoSlice *iovecs, const ENetBuffer *buffers, size_t bufferCount)
{
    for (size_t i = 0; i < bufferCount; ++i)
    {
        iovecs[i]._buffer = buffers[i].data;
        iovecs[i]._length = (i32)buffers[i].dataLength;
    }
}

/** Initializes ENet globally. Must be called prior to using any functions in ENet.
    @returns 0 on success, < 0 on failure
*/
int enet_initialize(void)
{
    return _Startup() == _SOCKET_ERROR_SUCCESS ? 0 : -1;
}

/** Shuts down ENet globally. Should be called when a program that has initialized ENet exits.
 */
void enet_deinitialize(void)
{
    _Cleanup();
}

/** Generates a new random seed for a host.
    @returns the generated random seed.
*/
enet_uint32
enet_host_random_seed(void)
{
    return _timeGetMilliseconds();
}

/** Returns the wall-time in milliseconds. Its initial value is unspecified unless otherwise set.
 */
enet_uint32
enet_time_get(void)
{
    return _timeGetMilliseconds() - timeBase;
}

/** Sets the current wall-time in milliseconds.
    @param newTimeBase The new wall-time base in milliseconds.
*/
void enet_time_set(enet_uint32 newTimeBase)
{
    timeBase = _timeGetMilliseconds() - newTimeBase;
}

/** Populates an ENet address by parsing an Ipv4 address string and port.
    @param address The address to populate.
    @param ip The Ipv4 address string.
    @param port The port number.
    @retval 0 on success
    @retval -1 on failure
*/
int enet_address_set_ip_ipv4(ENetAddress *address, const char *ip, enet_uint16 port)
{
    i32 error = _NativeSocketAddress_SetIpIpv4(&address->inner, ip, port);

    return error == _SOCKET_ERROR_SUCCESS ? 0 : -1;
}

/** Populates an ENet address by parsing an Ipv6 address string, port and scope.
    @param address The address to populate.
    @param ip The Ipv6 address string.
    @param port The port number.
    @param scopeId The Ipv6 scope identifier.
    @retval 0 on success
    @retval -1 on failure
*/
int enet_address_set_ip_ipv6(ENetAddress *address, const char *ip, enet_uint16 port, enet_uint32 scopeId)
{
    i32 error = _NativeSocketAddress_SetIpIpv6(&address->inner, ip, port, scopeId);

    return error == _SOCKET_ERROR_SUCCESS ? 0 : -1;
}

/** Populates an ENet address by resolving a host name to an Ipv4 address.
    @param address The address to populate.
    @param hostName The host name to resolve.
    @param port The port number.
    @retval 0 on success
    @retval -1 on failure
*/
int enet_address_set_hostname_ipv4(ENetAddress *address, const char *hostName, enet_uint16 port)
{
    i32 error = _NativeSocketAddress_SetHostNameIpv4(&address->inner, hostName, port);

    return error == _SOCKET_ERROR_SUCCESS ? 0 : -1;
}

/** Populates an ENet address by resolving a host name to an Ipv6 address.
    @param address The address to populate.
    @param hostName The host name to resolve.
    @param port The port number.
    @param scopeId The Ipv6 scope identifier.
    @retval 0 on success
    @retval -1 on failure
*/
int enet_address_set_hostname_ipv6(ENetAddress *address, const char *hostName, enet_uint16 port, enet_uint32 scopeId)
{
    i32 error = _NativeSocketAddress_SetHostNameIpv6(&address->inner, hostName, port, scopeId);

    return error == _SOCKET_ERROR_SUCCESS ? 0 : -1;
}

/** Retrieves the IP address of an ENet address as a character span.
    @param address The address to query.
    @param ip Receives the address characters.
    @retval 0 on success
    @retval -1 on failure
*/
int enet_address_get_ip(const ENetAddress *address, char *ip, size_t ipLength)
{
    i32 error = _NativeSocketAddress_GetIp(&((ENetAddress *)address)->inner, ip, (i32)ipLength);

    return error == _SOCKET_ERROR_SUCCESS ? 0 : -1;
}

/** Retrieves the host name (reverse DNS) of an ENet address.
    @param address The address to query.
    @param hostName Receives the host name characters.
    @retval 0 on success
    @retval -1 on failure
*/
int enet_address_get_hostname(const ENetAddress *address, char *hostName, size_t nameLength)
{
    i32 error = _NativeSocketAddress_GetHostName(&((ENetAddress *)address)->inner, hostName, (i32)nameLength);

    return error == _SOCKET_ERROR_SUCCESS ? 0 : -1;
}

/** Creates a native socket of the requested type and addressing mode.
    @param type The type of socket to create.
    @param option The addressing mode to use.
    @returns The created socket, or an invalid socket on failure.
*/
ENetSocket
enet_socket_create(ENetSocketType type, ENetHostOption option)
{
    ENetSocket socket;
    i32 ipv6;

    if (type != ENET_SOCKET_TYPE_DATAGRAM)
        return ENET_SOCKET_NULL;

    ipv6 = (option == ENET_HOSTOPT_IPV6_ONLY || option == ENET_HOSTOPT_IPV6_DUALMODE) ? 1 : 0;

    if (_NativeSockets_Create(ipv6, &socket.inner) != _SOCKET_ERROR_SUCCESS)
        return ENET_SOCKET_NULL;

    if (option == ENET_HOSTOPT_IPV6_ONLY && _NativeSockets_SetDualMode(&socket.inner, 0) != _SOCKET_ERROR_SUCCESS)
    {
        _NativeSockets_Dispose(&socket.inner);
        return ENET_SOCKET_NULL;
    }
    else if (option == ENET_HOSTOPT_IPV6_DUALMODE && _NativeSockets_SetDualMode(&socket.inner, 1) != _SOCKET_ERROR_SUCCESS)
    {
        _NativeSockets_Dispose(&socket.inner);
        return ENET_SOCKET_NULL;
    }

    return socket;
}

/** Binds the socket to the specified local address.
    @param socket The socket to bind.
    @param address The local address to bind to.
    @retval 0 on success
    @retval SOCKET_ERROR on failure
*/
int enet_socket_bind(ENetSocket socket, const ENetAddress *address)
{
    i32 result = _NativeSockets_Bind(&socket.inner, address != NULL ? &address->inner : NULL);

    return result == _SOCKET_ERROR_SUCCESS ? 0 : -1;
}

/** Retrieves the local address the socket is bound to.
    @param socket The socket to query.
    @param address Receives the local address.
    @retval 0 on success
    @retval SOCKET_ERROR on failure
*/
int enet_socket_get_address(ENetSocket socket, ENetAddress *address)
{
    i32 result = _NativeSockets_GetName(&socket.inner, &address->inner);

    return result == _SOCKET_ERROR_SUCCESS ? 0 : -1;
}

/** Applies a socket option to the given socket.
    @param socket The socket to configure.
    @param option The option to apply.
    @param value The option value.
    @retval 0 on success
    @retval -1 on failure or for unsupported options
*/
int enet_socket_set_option(ENetSocket socket, ENetSocketOption option, int value)
{
    int result = _SOCKET_ERROR_INVALID_ARGUMENT;

    switch (option)
    {
    case ENET_SOCKOPT_NONBLOCK:
        result = enet_socket_set_nonblocking(socket, value);
        break;

    case ENET_SOCKOPT_BROADCAST:
        result = _NativeSockets_SetOption(&socket.inner, _SOCKET_OPTION_LEVEL_SOCKET, _SOCKET_OPTION_NAME_BROADCAST, (u8 *)&value, sizeof(int));
        break;

    case ENET_SOCKOPT_REUSEADDR:
        result = _NativeSockets_SetOption(&socket.inner, _SOCKET_OPTION_LEVEL_SOCKET, _SOCKET_OPTION_NAME_REUSE_ADDRESS, (u8 *)&value, sizeof(int));
        break;

    case ENET_SOCKOPT_RCVBUF:
        result = _NativeSockets_SetOption(&socket.inner, _SOCKET_OPTION_LEVEL_SOCKET, _SOCKET_OPTION_NAME_RECEIVE_BUFFER, (u8 *)&value, sizeof(int));
        break;

    case ENET_SOCKOPT_SNDBUF:
        result = _NativeSockets_SetOption(&socket.inner, _SOCKET_OPTION_LEVEL_SOCKET, _SOCKET_OPTION_NAME_SEND_BUFFER, (u8 *)&value, sizeof(int));
        break;

    case ENET_SOCKOPT_RCVTIMEO:
        result = _NativeSockets_SetOption(&socket.inner, _SOCKET_OPTION_LEVEL_SOCKET, _SOCKET_OPTION_NAME_RECEIVE_TIMEOUT, (u8 *)&value, sizeof(int));
        break;

    case ENET_SOCKOPT_SNDTIMEO:
        result = _NativeSockets_SetOption(&socket.inner, _SOCKET_OPTION_LEVEL_SOCKET, _SOCKET_OPTION_NAME_SEND_TIMEOUT, (u8 *)&value, sizeof(int));
        break;

    case ENET_SOCKOPT_TTL:
        result = _NativeSockets_SetOption(&socket.inner, _SOCKET_OPTION_LEVEL_IP, _SOCKET_OPTION_NAME_IP_TIME_TO_LIVE, (u8 *)&value, sizeof(int));
        break;

    case ENET_SOCKOPT_IPV6_ONLY:
        result = _NativeSockets_SetOption(&socket.inner, _SOCKET_OPTION_LEVEL_IPV6, _SOCKET_OPTION_NAME_IPV6_V6ONLY, (u8 *)&value, sizeof(int));
        break;

    default:
        break;
    }
    return result == _SOCKET_ERROR_SUCCESS ? 0 : -1;
}

/** Sets the socket to blocking or non-blocking mode.
    @param socket The socket to configure.
    @param nonBlocking Non-zero to enable non-blocking mode.
    @retval 0 on success
    @retval -1 on failure
*/
int enet_socket_set_nonblocking(ENetSocket socket, int nonBlocking)
{
    i32 result = _NativeSockets_SetBlocking(&socket.inner, nonBlocking == 0);

    return result == _SOCKET_ERROR_SUCCESS ? 0 : -1;
}

/** Closes and invalidates the given socket.
    @param socket The socket to destroy.
*/
void enet_socket_destroy(ENetSocket socket)
{
    if (socket.inner.handle != (isize)-1)
        _NativeSockets_Dispose(&socket.inner);
}

/** Sends a vectored payload to the specified address on the socket.
    @param socket The socket to send on.
    @param address The destination address.
    @param buffers The buffers holding the payload.
    @param bufferCount The number of buffers.
    @returns The number of bytes sent, 0 when the send would block, -1 on failure.
*/
int enet_socket_send(ENetSocket socket,
                     const ENetAddress *address,
                     const ENetBuffer *buffers,
                     size_t bufferCount)
{
    _NativeIoSlice stackIovecs[32];
    _NativeIoSlice *iovecs = stackIovecs;
    int result;

    if (bufferCount > 32)
    {
        iovecs = (_NativeIoSlice *)malloc(sizeof(_NativeIoSlice) * bufferCount);
        if (iovecs == NULL)
            return -1;
    }

    _BuildNativeIoSlices(iovecs, buffers, bufferCount);

    result = _NativeSockets_SendToVectored(&socket.inner, iovecs, (i32)bufferCount, 0, &address->inner);

    if (iovecs != stackIovecs)
        free(iovecs);

    if (result < 0)
    {
        if (_GetLastSocketError() == _SOCKET_ERROR_WOULD_BLOCK)
            return 0;

        return -1;
    }

    return result;
}

/** Receives a vectored payload on the socket, reporting the sender address.
    @param socket The socket to receive on.
    @param address Receives the source address.
    @param buffers The buffers receiving the payload.
    @param bufferCount The number of buffers.
    @returns The number of bytes received, 0 when no data is available, -1 on failure.
*/
int enet_socket_receive(ENetSocket socket,
                        ENetAddress *address,
                        ENetBuffer *buffers,
                        size_t bufferCount)
{
    _NativeIoSlice stackIovecs[32];
    _NativeIoSlice *iovecs = stackIovecs;
    i32 flags = 0;
    int result;

    if (bufferCount > 32)
    {
        iovecs = (_NativeIoSlice *)malloc(sizeof(_NativeIoSlice) * bufferCount);
        if (iovecs == NULL)
            return -1;
    }

    _BuildNativeIoSlices(iovecs, buffers, bufferCount);

    result = _NativeSockets_ReceiveFromVectored(&socket.inner, iovecs, (i32)bufferCount, &flags, address != NULL ? &address->inner : NULL);

    if (iovecs != stackIovecs)
        free(iovecs);

    if (result < 0)
    {
        switch (_GetLastSocketError())
        {
        case _SOCKET_ERROR_WOULD_BLOCK:
        case _SOCKET_ERROR_CONNECTION_RESET:
            return 0;
        case _SOCKET_ERROR_INTERRUPTED:
        case _SOCKET_ERROR_MESSAGE_SIZE:
            return -2;
        default:
            return -1;
        }
    }

    if (flags & (_SOCKET_FLAGS_PARTIAL | _SOCKET_FLAGS_TRUNCATED))
        return -2;

    return result;
}

/** Waits until the socket becomes ready for the requested conditions or the timeout elapses.
    @param socket The socket to wait on.
    @param condition On input the conditions to wait for; on output the conditions that became ready.
    @param timeout The maximum time to wait in milliseconds.
    @retval 0 on success
    @retval -1 on failure
*/
int enet_socket_wait(ENetSocket socket, enet_uint32 *condition, enet_uint32 timeout)
{
    i32 inFlags = 0, outFlags = 0;
    i32 result;

    if (*condition & ENET_SOCKET_WAIT_SEND)
        inFlags |= _SELECT_MODE_FLAGS_WRITE;

    if (*condition & ENET_SOCKET_WAIT_RECEIVE)
        inFlags |= _SELECT_MODE_FLAGS_READ;

    if (*condition & ENET_SOCKET_WAIT_INTERRUPT)
        inFlags |= _SELECT_MODE_FLAGS_ERROR;

    result = _NativeSockets_PollFlags(&socket.inner, (i32)(timeout * 1000), inFlags, &outFlags);
    if (result != _SOCKET_ERROR_SUCCESS)
        return -1;

    *condition = ENET_SOCKET_WAIT_NONE;

    if (outFlags & _SELECT_MODE_FLAGS_WRITE)
        *condition |= ENET_SOCKET_WAIT_SEND;

    if (outFlags & _SELECT_MODE_FLAGS_READ)
        *condition |= ENET_SOCKET_WAIT_RECEIVE;

    if (outFlags & _SELECT_MODE_FLAGS_ERROR)
        *condition |= ENET_SOCKET_WAIT_INTERRUPT;

    return 0;
}
