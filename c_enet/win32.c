/**
 @file  win32.c
 @brief ENet system specific functions (NativeSockets backend)
*/
#define ENET_BUILDING_LIB 1
#include "enet/enet.h"
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
_toNativeIoVecs(_NativeIoSlice *iovecs, const ENetBuffer *buffers, size_t bufferCount)
{
    size_t i;

    for (i = 0; i < bufferCount; ++i)
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

/** Attempts to parse the printable form of the IP address in the parameter name and sets the host field in the address parameter if successful.
    @param address destination to store the parsed IP address
    @param name IP address to parse
    @retval 0 on success
    @retval < 0 on failure
*/
int enet_address_set_host_ip(ENetAddress *address, const char *name)
{
    i32 error;

    if (strchr(name, ':') != NULL)
        error = _NativeSocketAddress_SetIpIpv6(&address->inner, name, 0, 0);
    else
        error = _NativeSocketAddress_SetIpIpv4(&address->inner, name, 0);

    return error == _SOCKET_ERROR_SUCCESS ? 0 : -1;
}

/** Attempts to resolve the host named by the parameter name and sets the host field in the address parameter if successful.
    @param address destination to store resolved address
    @param name host name to lookup
    @retval 0 on success
    @retval < 0 on failure
*/
int enet_address_set_host(ENetAddress *address, const char *name)
{
    i32 error;

    if (strchr(name, ':') != NULL)
    {
        error = _NativeSocketAddress_SetHostNameIpv6(&address->inner, name, 0, 0);
        if (error == _SOCKET_ERROR_SUCCESS)
            return 0;
    }
    else
    {
        error = _NativeSocketAddress_SetHostNameIpv4(&address->inner, name, 0);
        if (error == _SOCKET_ERROR_SUCCESS)
            return 0;
    }

    return enet_address_set_host_ip(address, name);
}

/** Gives the printable form of the IP address specified in the address parameter.
    @param address address printed
    @param name destination for name, must not be NULL
    @param nameLength maximum length of name
    @retval 0 on success
    @retval < 0 on failure
*/
int enet_address_get_host_ip(const ENetAddress *address, char *name, size_t nameLength)
{
    i32 error = _NativeSocketAddress_GetIp(&((ENetAddress *)address)->inner, name, (i32)nameLength);

    return error == _SOCKET_ERROR_SUCCESS ? 0 : -1;
}

/** Attempts to do a reverse lookup of the host field in the address parameter.
    @param address address used for reverse lookup
    @param name destination for name, must not be NULL
    @param nameLength maximum length of name
    @retval 0 on success
    @retval < 0 on failure
*/
int enet_address_get_host(const ENetAddress *address, char *name, size_t nameLength)
{
    i32 error = _NativeSocketAddress_GetHostName(&((ENetAddress *)address)->inner, name, (i32)nameLength);

    if (error != _SOCKET_ERROR_SUCCESS)
        return enet_address_get_host_ip(address, name, nameLength);

    return 0;
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

    if (error != _SOCKET_ERROR_SUCCESS)
        return enet_address_get_host_ip(address, hostName, nameLength);

    return 0;
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

    socket.inner.handle = _Create(ipv6);
    socket.inner.family = ipv6 ? _SOCKET_FAMILY_INTER_NETWORK_V6 : _SOCKET_FAMILY_INTER_NETWORK;

    if (socket.inner.handle == (isize)-1)
        return socket;

    if (option == ENET_HOSTOPT_IPV6_ONLY)
        _SetDualModeIpv6(socket.inner.handle, 0);
    else if (option == ENET_HOSTOPT_IPV6_DUALMODE)
        _SetDualModeIpv6(socket.inner.handle, 1);

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
    i32 result;

    if (address != NULL)
    {
        if (_NativeSocketAddress_IsIpv4(&address->inner))
            result = _BindIpv4(socket.inner.handle, (_sockaddr_in4 *)&address->inner.sin4);
        else
            result = _BindIpv6(socket.inner.handle, (_sockaddr_in6 *)&address->inner.sin6);
    }
    else if (socket.inner.family == _SOCKET_FAMILY_INTER_NETWORK)
        result = _BindIpv4(socket.inner.handle, NULL);
    else
        result = _BindIpv6(socket.inner.handle, NULL);

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
    i32 result;

    if (socket.inner.family == _SOCKET_FAMILY_INTER_NETWORK)
        result = _GetNameIpv4(socket.inner.handle, &address->inner.sin4);
    else
        result = _GetNameIpv6(socket.inner.handle, &address->inner.sin6);

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
        result = _SetBlocking(socket.inner.handle, value ? 0 : 1);
        break;

    case ENET_SOCKOPT_BROADCAST:
        result = _SetOption(socket.inner.handle, _SOCKET_OPTION_LEVEL_SOCKET, _SOCKET_OPTION_NAME_BROADCAST, (u8 *)&value, sizeof(int));
        break;

    case ENET_SOCKOPT_REUSEADDR:
        result = _SetOption(socket.inner.handle, _SOCKET_OPTION_LEVEL_SOCKET, _SOCKET_OPTION_NAME_REUSE_ADDRESS, (u8 *)&value, sizeof(int));
        break;

    case ENET_SOCKOPT_RCVBUF:
        result = _SetOption(socket.inner.handle, _SOCKET_OPTION_LEVEL_SOCKET, _SOCKET_OPTION_NAME_RECEIVE_BUFFER, (u8 *)&value, sizeof(int));
        break;

    case ENET_SOCKOPT_SNDBUF:
        result = _SetOption(socket.inner.handle, _SOCKET_OPTION_LEVEL_SOCKET, _SOCKET_OPTION_NAME_SEND_BUFFER, (u8 *)&value, sizeof(int));
        break;

    case ENET_SOCKOPT_RCVTIMEO:
        result = _SetOption(socket.inner.handle, _SOCKET_OPTION_LEVEL_SOCKET, _SOCKET_OPTION_NAME_RECEIVE_TIMEOUT, (u8 *)&value, sizeof(int));
        break;

    case ENET_SOCKOPT_SNDTIMEO:
        result = _SetOption(socket.inner.handle, _SOCKET_OPTION_LEVEL_SOCKET, _SOCKET_OPTION_NAME_SEND_TIMEOUT, (u8 *)&value, sizeof(int));
        break;

    case ENET_SOCKOPT_TTL:
        result = _SetOption(socket.inner.handle, _SOCKET_OPTION_LEVEL_IP, _SOCKET_OPTION_NAME_IP_TIME_TO_LIVE, (u8 *)&value, sizeof(int));
        break;

    case ENET_SOCKOPT_IPV6_ONLY:
        result = _SetOption(socket.inner.handle, _SOCKET_OPTION_LEVEL_IPV6, _SOCKET_OPTION_NAME_IPV6_V6ONLY, (u8 *)&value, sizeof(int));
        break;

    default:
        break;
    }
    return result == _SOCKET_ERROR_SUCCESS ? 0 : -1;
}

/** Retrieves the given option for the socket.
    @param socket The socket to query.
    @param option The option to retrieve.
    @param value Receives the option value.
    @retval 0 on success
    @retval -1 on failure
*/
int enet_socket_get_option(ENetSocket socket, ENetSocketOption option, int *value)
{
    int result = _SOCKET_ERROR_INVALID_ARGUMENT;
    int length = sizeof(int);

    switch (option)
    {
    case ENET_SOCKOPT_ERROR:
        result = _GetOption(socket.inner.handle, _SOCKET_OPTION_LEVEL_SOCKET, _SOCKET_OPTION_NAME_ERROR, (u8 *)value, &length);
        break;

    case ENET_SOCKOPT_TTL:
        result = _GetOption(socket.inner.handle, _SOCKET_OPTION_LEVEL_IP, _SOCKET_OPTION_NAME_IP_TIME_TO_LIVE, (u8 *)value, &length);
        break;

    default:
        break;
    }
    return result == _SOCKET_ERROR_SUCCESS ? 0 : -1;
}

/** Closes and invalidates the given socket.
    @param socket The socket to destroy.
*/
void enet_socket_destroy(ENetSocket socket)
{
    if (socket.inner.handle != (isize)-1)
        _Close(socket.inner.handle);
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
    _NativeIoSlice iovecs[ENET_BUFFER_MAXIMUM];
    int result;

    _toNativeIoVecs(iovecs, buffers, bufferCount);

    if (address != NULL)
    {
        if (_NativeSocketAddress_IsIpv4(&address->inner))
            result = _SendToVectoredIpv4(socket.inner.handle, iovecs, (i32)bufferCount, 0, (_sockaddr_in4 *)&address->inner.sin4);
        else
            result = _SendToVectoredIpv6(socket.inner.handle, iovecs, (i32)bufferCount, 0, (_sockaddr_in6 *)&address->inner.sin6);
    }
    else if (socket.inner.family == _SOCKET_FAMILY_INTER_NETWORK)
        result = _SendToVectoredIpv4(socket.inner.handle, iovecs, (i32)bufferCount, 0, NULL);
    else
        result = _SendToVectoredIpv6(socket.inner.handle, iovecs, (i32)bufferCount, 0, NULL);

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
    _NativeIoSlice iovecs[ENET_BUFFER_MAXIMUM];
    i32 flags = 0;
    int result;

    _toNativeIoVecs(iovecs, buffers, bufferCount);

    if (socket.inner.family == _SOCKET_FAMILY_INTER_NETWORK)
        result = _ReceiveFromVectoredIpv4(socket.inner.handle, iovecs, (i32)bufferCount, &flags, address != NULL ? &address->inner.sin4 : NULL);
    else
        result = _ReceiveFromVectoredIpv6(socket.inner.handle, iovecs, (i32)bufferCount, &flags, address != NULL ? &address->inner.sin6 : NULL);

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

    result = _PollFlags(socket.inner.handle, (i32)(timeout * 1000), inFlags, &outFlags);
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
