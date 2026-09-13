/**
 @file  enet.h
 @brief ENet public header file
*/
#ifndef __ENET_ENET_H__
#define __ENET_ENET_H__

#include <stdlib.h>

#include "enet/win32.h"
#include "enet/utility.h"

#include "enet/types.h"
#include "enet/protocol.h"
#include "enet/list.h"
#include "enet/callbacks.h"

/**
  The major component of the ENet version.
*/
#define ENET_VERSION_MAJOR 1
/**
  The minor component of the ENet version.
*/
#define ENET_VERSION_MINOR 3
/**
  The patch component of the ENet version.
*/
#define ENET_VERSION_PATCH 18
/**
  Packs a major, minor and patch version into a single version value.
  @param major The major version component.
  @param minor The minor version component.
  @param patch The patch version component.
  @returns The packed version value.
*/
#define ENET_VERSION_CREATE(major, minor, patch) (((major) << 16) | ((minor) << 8) | (patch))
/**
  Extracts the major version component from a packed version value.
  @param version The packed version value.
  @returns The major version component.
*/
#define ENET_VERSION_GET_MAJOR(version) (((version) >> 16) & 0xFF)
/**
  Extracts the minor version component from a packed version value.
  @param version The packed version value.
  @returns The minor version component.
*/
#define ENET_VERSION_GET_MINOR(version) (((version) >> 8) & 0xFF)
/**
  Extracts the patch version component from a packed version value.
  @param version The packed version value.
  @returns The patch version component.
*/
#define ENET_VERSION_GET_PATCH(version) ((version) & 0xFF)
/**
  The packed ENet version value.
*/
#define ENET_VERSION ENET_VERSION_CREATE(ENET_VERSION_MAJOR, ENET_VERSION_MINOR, ENET_VERSION_PATCH)

/**
  The version number of the linked ENet library.
*/
typedef enet_uint32 ENetVersion;

struct _ENetHost;

struct _ENetEvent;

struct _ENetPacket;

/**
  The type of a native socket.
*/
typedef enum _ENetSocketType
{
  /** A connectionless datagram socket. */
  ENET_SOCKET_TYPE_DATAGRAM = 2
} ENetSocketType;

/**
  The wait states that can be requested on a socket.
*/
typedef enum _ENetSocketWait
{
  /** Wait for no specific state. */
  ENET_SOCKET_WAIT_NONE = 0,

  /** Wait until the socket can send data. */
  ENET_SOCKET_WAIT_SEND = (1 << 0),

  /** Wait until the socket can receive data. */
  ENET_SOCKET_WAIT_RECEIVE = (1 << 1),

  /** Wait until the wait is interrupted. */
  ENET_SOCKET_WAIT_INTERRUPT = (1 << 2)
} ENetSocketWait;

/**
  The socket options that can be configured on a native socket.
*/
typedef enum _ENetSocketOption
{
  /** Toggles non-blocking mode on the socket. */
  ENET_SOCKOPT_NONBLOCK = 1,

  /** Enables or disables broadcast on the socket. */
  ENET_SOCKOPT_BROADCAST = 2,

  /** Sets the receive buffer size of the socket. */
  ENET_SOCKOPT_RCVBUF = 3,

  /** Sets the send buffer size of the socket. */
  ENET_SOCKOPT_SNDBUF = 4,

  /** Allows the socket to reuse a bound address. */
  ENET_SOCKOPT_REUSEADDR = 5,

  /** Sets the receive timeout of the socket. */
  ENET_SOCKOPT_RCVTIMEO = 6,

  /** Sets the send timeout of the socket. */
  ENET_SOCKOPT_SNDTIMEO = 7,

  /** Retrieves the last error of the socket. */
  ENET_SOCKOPT_ERROR = 8,

  /** Sets the time to live of packets sent on the socket. */
  ENET_SOCKOPT_TTL = 10,

  /** Restricts the socket to Ipv6 only. */
  ENET_SOCKOPT_IPV6_ONLY = 11
} ENetSocketOption;

/**
  The addressing mode used when creating a host.
*/
typedef enum _ENetHostOption
{
  /** Use Ipv4 addressing only. */
  ENET_HOSTOPT_IPV4 = 0,

  /** Use Ipv6 addressing only. */
  ENET_HOSTOPT_IPV6_ONLY = 1,

  /** Use Ipv6 dual stack addressing, accepting both Ipv4 and Ipv6. */
  ENET_HOSTOPT_IPV6_DUALMODE = 2
} ENetHostOption;

/** The default server host address value (0.0.0.0). */
#define ENET_HOST_ANY 0
/** The broadcast address value (255.255.255.255). */
#define ENET_HOST_BROADCAST 0xFFFFFFFFU
/** A port value indicating that the operating system should choose an available port. */
#define ENET_PORT_ANY 0

/**
  Packet flag bit constants.
  @sa ENetPacket
*/
typedef enum _ENetPacketFlag
{
  /** packet must be received by the target peer and resend attempts should be
   * made until the packet is delivered */
  ENET_PACKET_FLAG_RELIABLE = (1 << 0),

  /** packet will not be sequenced with other packets
   */
  ENET_PACKET_FLAG_UNSEQUENCED = (1 << 1),

  /** packet will not allocate data, and user must supply it instead */
  ENET_PACKET_FLAG_NO_ALLOCATE = (1 << 2),

  /** packet will be fragmented using unreliable (instead of reliable) sends
   * if it exceeds the MTU */
  ENET_PACKET_FLAG_UNRELIABLE_FRAGMENT = (1 << 3),

  /** whether the packet has been sent from all queues it has been entered into */
  ENET_PACKET_FLAG_SENT = (1 << 8)
} ENetPacketFlag;

/** Callback that is called when a packet is no longer in use. */
typedef void(ENET_CALLBACK *ENetPacketFreeCallback)(struct _ENetPacket *);

/**
  ENet packet structure.
  An ENet data packet that may be sent to or received from a peer. The shown
  fields should only be read and never modified. The data field contains the
  allocated data for the packet. The dataLength fields specifies the length
  of the allocated data.  The flags field is either 0 (specifying no flags),
  or a bitwise-or of any combination of the following flags:
  - ENET_PACKET_FLAG_RELIABLE - packet must be received by the target peer
    and resend attempts should be made until the packet is delivered
  - ENET_PACKET_FLAG_UNSEQUENCED - packet will not be sequenced with other packets
    (not supported for reliable packets)
  - ENET_PACKET_FLAG_NO_ALLOCATE - packet will not allocate data, and user must supply it instead
  - ENET_PACKET_FLAG_UNRELIABLE_FRAGMENT - packet will be fragmented using unreliable
    (instead of reliable) sends if it exceeds the MTU
  - ENET_PACKET_FLAG_SENT - whether the packet has been sent from all queues it has been entered into
  @sa ENetPacketFlag
*/
typedef struct _ENetPacket
{
  size_t referenceCount;               /**< internal use only */
  enet_uint32 flags;                   /**< bitwise-or of ENetPacketFlag constants */
  enet_uint8 *data;                    /**< allocated data for packet */
  size_t dataLength;                   /**< length of data */
  ENetPacketFreeCallback freeCallback; /**< function to be called when the packet is no longer in use */
  void *userData;                      /**< application private data, may be freely modified */
} ENetPacket;

/**
  An acknowledgement tracking a reliably sent command waiting for confirmation.
*/
typedef struct _ENetAcknowledgement
{
  /** The list node linking this acknowledgement into the peer acknowledgement queue. */
  ENetListNode acknowledgementList;

  /** The time the acknowledged command was sent. */
  enet_uint32 sentTime;

  /** The protocol command being acknowledged. */
  ENetProtocol command;
} ENetAcknowledgement;

/**
  A command queued for transmission to a peer.
*/
typedef struct _ENetOutgoingCommand
{
  /** The list node linking this command into the peer outgoing command queue. */
  ENetListNode outgoingCommandList;

  /** The reliable sequence number assigned to this command. */
  enet_uint16 reliableSequenceNumber;

  /** The unreliable sequence number assigned to this command. */
  enet_uint16 unreliableSequenceNumber;

  /** The time this command was last sent. */
  enet_uint32 sentTime;

  /** The timeout in milliseconds after which the command is resent. */
  enet_uint32 roundTripTimeout;

  /** The time this command was enqueued. */
  enet_uint32 queueTime;

  /** The byte offset of the current fragment within the packet. */
  enet_uint32 fragmentOffset;

  /** The length in bytes of the current fragment payload. */
  enet_uint16 fragmentLength;

  /** The number of times this command has been sent. */
  enet_uint16 sendAttempts;

  /** The protocol command data to transmit. */
  ENetProtocol command;

  /** The packet associated with this command, or NULL for control commands. */
  ENetPacket *packet;
} ENetOutgoingCommand;

/**
  A command received from a peer awaiting processing or reassembly.
*/
typedef struct _ENetIncomingCommand
{
  /** The list node linking this command into the peer incoming command queue. */
  ENetListNode incomingCommandList;

  /** The reliable sequence number of this command. */
  enet_uint16 reliableSequenceNumber;

  /** The unreliable sequence number of this command. */
  enet_uint16 unreliableSequenceNumber;

  /** The protocol command data received. */
  ENetProtocol command;

  /** The total number of fragments expected for the fragmented packet. */
  enet_uint32 fragmentCount;

  /** The number of fragments still awaiting arrival. */
  enet_uint32 fragmentsRemaining;

  /** The bit field tracking which fragments have been received. */
  enet_uint32 *fragments;

  /** The packet being assembled from this command. */
  ENetPacket *packet;
} ENetIncomingCommand;

/**
  The connection states a peer can occupy.
*/
typedef enum _ENetPeerState
{
  /** The peer is not connected. */
  ENET_PEER_STATE_DISCONNECTED = 0,

  /** A connection attempt is in progress. */
  ENET_PEER_STATE_CONNECTING = 1,

  /** The connection request has been acknowledged and awaits the final confirmation. */
  ENET_PEER_STATE_ACKNOWLEDGING_CONNECT = 2,

  /** The connection is pending but has not yet been established. */
  ENET_PEER_STATE_CONNECTION_PENDING = 3,

  /** The connection attempt has succeeded. */
  ENET_PEER_STATE_CONNECTION_SUCCEEDED = 4,

  /** The peer is connected and ready for communication. */
  ENET_PEER_STATE_CONNECTED = 5,

  /** The peer will be disconnected after pending commands are flushed. */
  ENET_PEER_STATE_DISCONNECT_LATER = 6,

  /** A disconnect request is in progress. */
  ENET_PEER_STATE_DISCONNECTING = 7,

  /** The disconnect request has been acknowledged and awaits the final confirmation. */
  ENET_PEER_STATE_ACKNOWLEDGING_DISCONNECT = 8,

  /** The peer connection has been terminated and only awaits cleanup. */
  ENET_PEER_STATE_ZOMBIE = 9
} ENetPeerState;

/**
  The maximum number of buffers used when assembling a packet.
*/
#ifndef ENET_BUFFER_MAXIMUM
#define ENET_BUFFER_MAXIMUM (1 + 2 * ENET_PROTOCOL_MAXIMUM_PACKET_COMMANDS)
#endif

enum
{
  /** The default size of the host receive buffer in bytes. */
  ENET_HOST_RECEIVE_BUFFER_SIZE = 256 * 1024,

  /** The default size of the host send buffer in bytes. */
  ENET_HOST_SEND_BUFFER_SIZE = 256 * 1024,

  /** The interval in milliseconds between bandwidth throttle computations. */
  ENET_HOST_BANDWIDTH_THROTTLE_INTERVAL = 1000,

  /** The default maximum transmission unit used when the peer does not negotiate one. */
  ENET_HOST_DEFAULT_MTU = 1392,

  /** The default maximum packet size that may be sent or received on a peer. */
  ENET_HOST_DEFAULT_MAXIMUM_PACKET_SIZE = 32 * 1024 * 1024,

  /** The default maximum aggregate amount of buffer space a peer may use waiting for delivery. */
  ENET_HOST_DEFAULT_MAXIMUM_WAITING_DATA = 32 * 1024 * 1024,

  /** The default round trip time in milliseconds assumed for a new peer. */
  ENET_PEER_DEFAULT_ROUND_TRIP_TIME = 500,

  /** The default packet throttle value assumed for a new peer. */
  ENET_PEER_DEFAULT_PACKET_THROTTLE = 32,

  /** The scale factor applied to the packet throttle value. */
  ENET_PEER_PACKET_THROTTLE_SCALE = 32,

  /** The number of throttle samples collected before the throttle value is adjusted. */
  ENET_PEER_PACKET_THROTTLE_COUNTER = 7,

  /** The rate at which the packet throttle value is increased on successful samples. */
  ENET_PEER_PACKET_THROTTLE_ACCELERATION = 2,

  /** The rate at which the packet throttle value is decreased on failed samples. */
  ENET_PEER_PACKET_THROTTLE_DECELERATION = 2,

  /** The default interval in milliseconds between packet throttle measurements. */
  ENET_PEER_PACKET_THROTTLE_INTERVAL = 5000,

  /** The scale factor applied to the packet loss ratio. */
  ENET_PEER_PACKET_LOSS_SCALE = (1 << 16),

  /** The interval in milliseconds over which packet loss is measured. */
  ENET_PEER_PACKET_LOSS_INTERVAL = 10000,

  /** The scale factor applied to the window size. */
  ENET_PEER_WINDOW_SIZE_SCALE = 64 * 1024,

  /** The default timeout limit in milliseconds before a connection is considered timed out. */
  ENET_PEER_TIMEOUT_LIMIT = 32,

  /** The default minimum timeout in milliseconds before a connection is considered timed out. */
  ENET_PEER_TIMEOUT_MINIMUM = 5000,

  /** The default maximum timeout in milliseconds before a connection is considered timed out. */
  ENET_PEER_TIMEOUT_MAXIMUM = 30000,

  /** The default interval in milliseconds between keep-alive pings. */
  ENET_PEER_PING_INTERVAL = 500,

  /** The number of windows used to track unsequenced packets. */
  ENET_PEER_UNSEQUENCED_WINDOWS = 64,

  /** The number of unsequenced packets tracked per window. */
  ENET_PEER_UNSEQUENCED_WINDOW_SIZE = 1024,

  /** The number of free unsequenced windows a peer may shift. */
  ENET_PEER_FREE_UNSEQUENCED_WINDOWS = 32,

  /** The number of windows used to track reliable packets. */
  ENET_PEER_RELIABLE_WINDOWS = 16,

  /** The number of reliable packets tracked per window. */
  ENET_PEER_RELIABLE_WINDOW_SIZE = 0x1000,

  /** The number of free reliable windows a peer may shift. */
  ENET_PEER_FREE_RELIABLE_WINDOWS = 8
};

/**
  The per-channel state tracking reliable and unreliable sequencing for a peer.
*/
typedef struct _ENetChannel
{
  /** The next reliable sequence number to assign on this channel. */
  enet_uint16 outgoingReliableSequenceNumber;

  /** The next unreliable sequence number to assign on this channel. */
  enet_uint16 outgoingUnreliableSequenceNumber;

  /** The number of reliable windows currently in use. */
  enet_uint16 usedReliableWindows;

  /** The sliding windows tracking which reliable packets have been sent on this channel. */
  enet_uint16 reliableWindows[ENET_PEER_RELIABLE_WINDOWS];

  /** The highest reliable sequence number received on this channel. */
  enet_uint16 incomingReliableSequenceNumber;

  /** The highest unreliable sequence number received on this channel. */
  enet_uint16 incomingUnreliableSequenceNumber;

  /** The queue of reliable commands received on this channel. */
  ENetList incomingReliableCommands;

  /** The queue of unreliable commands received on this channel. */
  ENetList incomingUnreliableCommands;
} ENetChannel;

/**
  The flags that can be set on a peer.
*/
typedef enum _ENetPeerFlag
{
  /** Indicates the peer has events waiting to be dispatched. */
  ENET_PEER_FLAG_NEEDS_DISPATCH = (1 << 0),

  /** Indicates the peer still has commands queued for sending. */
  ENET_PEER_FLAG_CONTINUE_SENDING = (1 << 1)
} ENetPeerFlag;

/**
  An ENet peer which data packets may be sent or received from.
  No fields should be modified unless otherwise specified.
*/
typedef struct _ENetPeer
{
  /** The list node linking this peer into the host dispatch queue. */
  ENetListNode dispatchList;

  /** The host this peer belongs to. */
  struct _ENetHost *host;

  /** The peer identifier by which the remote side knows this peer. */
  enet_uint16 outgoingPeerID;

  /** The peer identifier by which this host knows the remote peer. */
  enet_uint16 incomingPeerID;

  /** A unique value identifying the connection attempt. */
  enet_uint32 connectID;

  /** The session identifier of the outgoing connection. */
  enet_uint8 outgoingSessionID;

  /** The session identifier of the incoming connection. */
  enet_uint8 incomingSessionID;

  /** Internet address of the peer */
  ENetAddress address;

  /** Application private data, may be freely modified */
  void *data;

  /** The current connection state of the peer. */
  ENetPeerState state;

  /** The array of channels allocated for the peer. */
  ENetChannel *channels;

  /** Number of channels allocated for communication with peer */
  size_t channelCount;

  /** Downstream bandwidth of the client in bytes/second */
  enet_uint32 incomingBandwidth;

  /** Upstream bandwidth of the client in bytes/second */
  enet_uint32 outgoingBandwidth;

  /** The time of the next incoming bandwidth throttle sample. */
  enet_uint32 incomingBandwidthThrottleEpoch;

  /** The time of the next outgoing bandwidth throttle sample. */
  enet_uint32 outgoingBandwidthThrottleEpoch;

  /** The total number of bytes received from the peer. */
  enet_uint32 incomingDataTotal;

  /** The total number of bytes sent to the peer. */
  enet_uint32 outgoingDataTotal;

  /** The last time data was sent to the peer. */
  enet_uint32 lastSendTime;

  /** The last time data was received from the peer. */
  enet_uint32 lastReceiveTime;

  /** The time at which the current timeout period expires. */
  enet_uint32 nextTimeout;

  /** The earliest time at which a pending command times out. */
  enet_uint32 earliestTimeout;

  /** The time of the next packet loss sample. */
  enet_uint32 packetLossEpoch;

  /** The total number of reliable packets sent to the peer. */
  enet_uint32 packetsSent;

  /** The total number of reliable packets lost to the peer. */
  enet_uint32 packetsLost;

  /** mean packet loss of reliable packets as a ratio with respect to the constant ENET_PEER_PACKET_LOSS_SCALE */
  enet_uint32 packetLoss;

  /** The variance of the packet loss ratio. */
  enet_uint32 packetLossVariance;

  /** The current packet throttle value in the range zero to ENET_PEER_PACKET_THROTTLE_SCALE. */
  enet_uint32 packetThrottle;

  /** The maximum packet throttle value allowed for the peer. */
  enet_uint32 packetThrottleLimit;

  /** The number of samples collected within the current throttle interval. */
  enet_uint32 packetThrottleCounter;

  /** The time of the next packet throttle sample. */
  enet_uint32 packetThrottleEpoch;

  /** The packet throttle acceleration rate. */
  enet_uint32 packetThrottleAcceleration;

  /** The packet throttle deceleration rate. */
  enet_uint32 packetThrottleDeceleration;

  /** The packet throttle measurement interval in milliseconds. */
  enet_uint32 packetThrottleInterval;

  /** The interval in milliseconds between keep-alive pings. */
  enet_uint32 pingInterval;

  /** The timeout limit in milliseconds before the connection is considered timed out. */
  enet_uint32 timeoutLimit;

  /** The minimum timeout in milliseconds before the connection is considered timed out. */
  enet_uint32 timeoutMinimum;

  /** The maximum timeout in milliseconds before the connection is considered timed out. */
  enet_uint32 timeoutMaximum;

  /** The most recently measured round trip time in milliseconds. */
  enet_uint32 lastRoundTripTime;

  /** The lowest round trip time ever measured for the peer. */
  enet_uint32 lowestRoundTripTime;

  /** The variance of the most recent round trip time measurement. */
  enet_uint32 lastRoundTripTimeVariance;

  /** The highest round trip time variance ever measured for the peer. */
  enet_uint32 highestRoundTripTimeVariance;

  /** mean round trip time (RTT), in milliseconds, between sending a reliable packet and receiving its acknowledgement */
  enet_uint32 roundTripTime;

  /** The variance of the round trip time. */
  enet_uint32 roundTripTimeVariance;

  /** The maximum transmission unit negotiated for the peer. */
  enet_uint32 mtu;

  /** The receive window size negotiated for the peer. */
  enet_uint32 windowSize;

  /** The number of reliable bytes currently awaiting acknowledgement. */
  enet_uint32 reliableDataInTransit;

  /** The next reliable sequence number to assign on the peer. */
  enet_uint16 outgoingReliableSequenceNumber;

  /** The queue of acknowledgements awaiting transmission. */
  ENetList acknowledgements;

  /** The queue of reliable commands sent but not yet acknowledged. */
  ENetList sentReliableCommands;

  /** The queue of reliable commands waiting to be sent. */
  ENetList outgoingSendReliableCommands;

  /** The queue of commands waiting to be sent. */
  ENetList outgoingCommands;

  /** The queue of commands awaiting dispatch to the application. */
  ENetList dispatchedCommands;

  /** The flags set on this peer. */
  enet_uint16 flags;

  /** Reserved for internal use. */
  enet_uint16 reserved;

  /** The highest unsequenced group received from the peer. */
  enet_uint16 incomingUnsequencedGroup;

  /** The next unsequenced group to assign on the peer. */
  enet_uint16 outgoingUnsequencedGroup;

  /** The sliding windows tracking which unsequenced packets have been received. */
  enet_uint32 unsequencedWindow[ENET_PEER_UNSEQUENCED_WINDOW_SIZE / 32];

  /** Application defined data carried with the last disconnect event. */
  enet_uint32 eventData;

  /** The total number of bytes queued waiting to be delivered to the peer. */
  size_t totalWaitingData;
} ENetPeer;

/** An ENet packet compressor for compressing UDP packets before socket sends or receives.
 */
typedef struct _ENetCompressor
{
  /** Context data for the compressor. Must be non-NULL. */
  void *context;

  /** Compresses from inBuffers[0:inBufferCount-1], containing inLimit bytes, to outData, outputting at most outLimit bytes. Should return 0 on failure. */
  size_t(ENET_CALLBACK *compress)(void *context, const ENetBuffer *inBuffers, size_t inBufferCount, size_t inLimit, enet_uint8 *outData, size_t outLimit);

  /** Decompresses from inData, containing inLimit bytes, to outData, outputting at most outLimit bytes. Should return 0 on failure. */
  size_t(ENET_CALLBACK *decompress)(void *context, const enet_uint8 *inData, size_t inLimit, enet_uint8 *outData, size_t outLimit);

  /** Destroys the context when compression is disabled or the host is destroyed. May be NULL. */
  void(ENET_CALLBACK *destroy)(void *context);
} ENetCompressor;

/** Callback that computes the checksum of the data held in buffers[0:bufferCount-1] */
typedef enet_uint32(ENET_CALLBACK *ENetChecksumCallback)(const ENetBuffer *buffers, size_t bufferCount);

/** Callback for intercepting received raw UDP packets. Should return 1 to intercept, 0 to ignore, or -1 to propagate an error. */
typedef int(ENET_CALLBACK *ENetInterceptCallback)(struct _ENetHost *host, struct _ENetEvent *event);

/** An ENet host for communicating with peers.
  *
  * No fields should be modified unless otherwise stated.

    @sa enet_host_create()
    @sa enet_host_destroy()
    @sa enet_host_connect()
    @sa enet_host_service()
    @sa enet_host_flush()
    @sa enet_host_broadcast()
    @sa enet_host_compress()
    @sa enet_host_compress_with_range_coder()
    @sa enet_host_channel_limit()
    @sa enet_host_bandwidth_limit()
    @sa enet_host_bandwidth_throttle()
    @sa enet_host_checksum()
    @sa enet_host_checksum_with_crc32()
    @sa enet_host_intercept()
    @sa enet_host_duplicate_peers()
    @sa enet_host_maximum_packet_size()
    @sa enet_host_maximum_waiting_data()
    @sa enet_host_ignore_connect_requests()
    @sa enet_host_mtu()
    @sa enet_host_get_peer()
    @sa enet_host_ping()
    @sa enet_host_broadcast_selected()
  */
typedef struct _ENetHost
{
  /** The native socket the host receives and sends data on. */
  ENetSocket socket;

  /** Internet address of the host */
  ENetAddress address;

  /** downstream bandwidth of the host */
  enet_uint32 incomingBandwidth;

  /** upstream bandwidth of the host */
  enet_uint32 outgoingBandwidth;

  /** The time of the next bandwidth throttle computation. */
  enet_uint32 bandwidthThrottleEpoch;

  /** The maximum transmission unit of the host socket. */
  enet_uint32 mtu;

  /** The random seed used to generate connect identifiers. */
  enet_uint32 randomSeed;

  /** Non-zero when the bandwidth limits must be recalculated for all peers. */
  int recalculateBandwidthLimits;

  /** array of peers allocated for this host */
  ENetPeer *peers;

  /** number of peers allocated for this host */
  size_t peerCount;

  /** maximum number of channels allowed for connected peers */
  size_t channelLimit;

  /** The time of the last service pass. */
  enet_uint32 serviceTime;

  /** The queue of peers with events waiting to be dispatched. */
  ENetList dispatchQueue;

  /** The total number of bytes queued waiting for delivery across all peers. */
  enet_uint32 totalQueued;

  /** The size in bytes of the packets being assembled for transmission. */
  size_t packetSize;

  /** The flags applied to the header of outgoing packets. */
  enet_uint16 headerFlags;

  /** When non-zero, the host ignores incoming connection requests instead of accepting them */
  enet_uint16 ignoreConnectRequests;

  /** The array of protocol commands being assembled for the next packet. */
  ENetProtocol commands[ENET_PROTOCOL_MAXIMUM_PACKET_COMMANDS];

  /** The number of commands currently assembled for the next packet. */
  size_t commandCount;

  /** The array of buffers being assembled for the next packet. */
  ENetBuffer buffers[ENET_BUFFER_MAXIMUM];

  /** The number of buffers currently assembled for the next packet. */
  size_t bufferCount;

  /** callback the user can set to enable packet checksums for this host */
  ENetChecksumCallback checksum;

  /** The compressor applied to packets, if any. */
  ENetCompressor compressor;

  /** The fixed buffers used to store the payload data of outgoing packets. */
  enet_uint8 packetData[2][ENET_PROTOCOL_MAXIMUM_MTU];

  /** The address the currently received packet came from. */
  ENetAddress receivedAddress;

  /** The buffer holding the currently received packet payload. */
  enet_uint8 *receivedData;

  /** The length in bytes of the currently received packet payload. */
  size_t receivedDataLength;

  /** total data sent, user should reset to 0 as needed to prevent overflow */
  enet_uint32 totalSentData;

  /** total UDP packets sent, user should reset to 0 as needed to prevent overflow */
  enet_uint32 totalSentPackets;

  /** total data received, user should reset to 0 as needed to prevent overflow */
  enet_uint32 totalReceivedData;

  /** total UDP packets received, user should reset to 0 as needed to prevent overflow */
  enet_uint32 totalReceivedPackets;

  /** callback the user can set to intercept received raw UDP packets */
  ENetInterceptCallback intercept;

  /** The number of peers currently in the connected state. */
  size_t connectedPeers;

  /** The number of peers currently limited by bandwidth throttling. */
  size_t bandwidthLimitedPeers;

  /** optional number of allowed peers from duplicate IPs, defaults to ENET_PROTOCOL_MAXIMUM_PEER_ID */
  size_t duplicatePeers;

  /** the maximum allowable packet size that may be sent or received on a peer */
  size_t maximumPacketSize;

  /** the maximum aggregate amount of buffer space a peer may use waiting for packets to be delivered */
  size_t maximumWaitingData;
} ENetHost;

/**
  An ENet event type, as specified in @ref ENetEvent.
*/
typedef enum _ENetEventType
{
  /** no event occurred within the specified time limit */
  ENET_EVENT_TYPE_NONE = 0,

  /** a connection request initiated by enet_host_connect has completed.
   * The peer field contains the peer which successfully connected.
   */
  ENET_EVENT_TYPE_CONNECT = 1,

  /** a peer has disconnected.  This event is generated on a successful
   * completion of a disconnect initiated by enet_peer_disconnect, if
   * a peer has timed out, or if a connection request intialized by
   * enet_host_connect has timed out.  The peer field contains the peer
   * which disconnected. The data field contains user supplied data
   * describing the disconnection, or 0, if none is available.
   */
  ENET_EVENT_TYPE_DISCONNECT = 2,

  /** a packet has been received from a peer.  The peer field specifies the
   * peer which sent the packet.  The channelID field specifies the channel
   * number upon which the packet was received.  The packet field contains
   * the packet that was received; this packet must be destroyed with
   * enet_packet_destroy after use.
   */
  ENET_EVENT_TYPE_RECEIVE = 3
} ENetEventType;

/**
  An ENet event as returned by enet_host_service().
  @sa enet_host_service
*/
typedef struct _ENetEvent
{
  /** type of the event */
  ENetEventType type;

  /** peer that generated a connect, disconnect or receive event */
  ENetPeer *peer;

  /** channel on the peer that generated the event, if appropriate */
  enet_uint8 channelID;

  /** data associated with the event, if appropriate */
  enet_uint32 data;

  /** packet associated with the event, if appropriate */
  ENetPacket *packet;
} ENetEvent;

#ifdef __cplusplus
extern "C"
{
#endif

  /** @defgroup global ENet global functions
      @{
  */

  /**
    Initializes ENet globally.  Must be called prior to using any functions in
    ENet.
    @returns 0 on success, < 0 on failure
  */
  ENET_API int enet_initialize(void);

  /**
    Initializes ENet globally and supplies user-overridden callbacks. Must be called prior to using any functions in ENet. Do not use enet_initialize() if you use this variant. Make sure the ENetCallbacks structure is zeroed out so that any additional callbacks added in future versions will be properly ignored.

    @param version the constant ENET_VERSION should be supplied so ENet knows which version of ENetCallbacks struct to use
    @param inits user-overridden callbacks where any NULL callbacks will use ENet's defaults
    @returns 0 on success, < 0 on failure
  */
  ENET_API int enet_initialize_with_callbacks(ENetVersion version, const ENetCallbacks *inits);

  /**
    Shuts down ENet globally.  Should be called when a program that has
    initialized ENet exits.
  */
  ENET_API void enet_deinitialize(void);

  /**
    Gives the linked version of the ENet library.
    @returns the version number
  */
  ENET_API ENetVersion enet_linked_version(void);

  /** @} */

  /** @defgroup private ENet private implementation functions */

  /**
    Returns the wall-time in milliseconds.  Its initial value is unspecified
    unless otherwise set.
    */
  ENET_API enet_uint32 enet_time_get(void);

  /**
    Sets the current wall-time in milliseconds.
    */
  ENET_API void enet_time_set(enet_uint32);

  /** @defgroup socket ENet socket functions
      @{
  */

  /**
    Creates a native socket of the requested type and addressing mode.
    @param type The type of socket to create.
    @param option The addressing mode to use.
    @returns The created socket, or an invalid socket on failure.
  */
  ENET_API ENetSocket enet_socket_create(ENetSocketType, ENetHostOption);

  /**
    Binds the socket to the specified local address.
    @param socket The socket to bind.
    @param address The local address to bind to.
    @retval 0 on success
    @retval SOCKET_ERROR on failure
  */
  ENET_API int enet_socket_bind(ENetSocket, const ENetAddress *);

  /**
    Retrieves the local address the socket is bound to.
    @param socket The socket to query.
    @param address Receives the local address.
    @retval 0 on success
    @retval SOCKET_ERROR on failure
  */
  ENET_API int enet_socket_get_address(ENetSocket, ENetAddress *);

  /**
    Sends a vectored payload to the specified address on the socket.
    @param socket The socket to send on.
    @param address The destination address.
    @param buffers The buffers holding the payload.
    @param bufferCount The number of buffers.
    @returns The number of bytes sent, 0 when the send would block, -1 on failure.
  */
  ENET_API int enet_socket_send(ENetSocket, const ENetAddress *, const ENetBuffer *, size_t);

  /**
    Receives a vectored payload on the socket, reporting the sender address.
    @param socket The socket to receive on.
    @param address Receives the source address.
    @param buffers The buffers receiving the payload.
    @param bufferCount The number of buffers.
    @returns The number of bytes received, 0 when no data is available, -1 on failure.
  */
  ENET_API int enet_socket_receive(ENetSocket, ENetAddress *, ENetBuffer *, size_t);

  /**
    Waits until the socket becomes ready for the requested conditions or the timeout elapses.
    @param socket The socket to wait on.
    @param condition On input the conditions to wait for; on output the conditions that became ready.
    @param milliseconds The maximum time to wait in milliseconds.
    @retval 0 on success
    @retval -1 on failure
  */
  ENET_API int enet_socket_wait(ENetSocket, enet_uint32 *, enet_uint32);

  /**
    Applies a socket option to the given socket.
    @param socket The socket to configure.
    @param option The option to apply.
    @param value The option value.
    @retval 0 on success
    @retval -1 on failure or for unsupported options
  */
  ENET_API int enet_socket_set_option(ENetSocket, ENetSocketOption, int);


  /**
    Closes and invalidates the given socket.
    @param socket The socket to destroy.
  */
  ENET_API void enet_socket_destroy(ENetSocket);

  /** @} */

  /** @defgroup Address ENet address functions
      @{
  */

  /** Populates an ENet address by parsing an Ipv4 address string and port.
      @param address The address to populate.
      @param ip The Ipv4 address string.
      @param port The port number.
      @retval 0 on success
      @retval -1 on failure
  */
  ENET_API int enet_address_set_ip_ipv4(ENetAddress *address, const char *ip, enet_uint16 port);

  /** Populates an ENet address by parsing an Ipv6 address string, port and scope.
      @param address The address to populate.
      @param ip The Ipv6 address string.
      @param port The port number.
      @param scopeId The Ipv6 scope identifier.
      @retval 0 on success
      @retval -1 on failure
  */
  ENET_API int enet_address_set_ip_ipv6(ENetAddress *address, const char *ip, enet_uint16 port, enet_uint32 scopeId);

  /** Populates an ENet address by resolving a host name to an Ipv4 address.
      @param address The address to populate.
      @param hostName The host name to resolve.
      @param port The port number.
      @retval 0 on success
      @retval -1 on failure
  */
  ENET_API int enet_address_set_hostname_ipv4(ENetAddress *address, const char *hostName, enet_uint16 port);

  /** Populates an ENet address by resolving a host name to an Ipv6 address.
      @param address The address to populate.
      @param hostName The host name to resolve.
      @param port The port number.
      @param scopeId The Ipv6 scope identifier.
      @retval 0 on success
      @retval -1 on failure
  */
  ENET_API int enet_address_set_hostname_ipv6(ENetAddress *address, const char *hostName, enet_uint16 port, enet_uint32 scopeId);

  /** Retrieves the IP address of an ENet address as a character span.
      @param address The address to query.
      @param ip Receives the address characters.
      @retval 0 on success
      @retval -1 on failure
  */
  ENET_API int enet_address_get_ip(const ENetAddress *address, char *ip, size_t ipLength);

  /** Retrieves the host name (reverse DNS) of an ENet address.
      @param address The address to query.
      @param hostName Receives the host name characters.
      @retval 0 on success
      @retval -1 on failure
  */
  ENET_API int enet_address_get_hostname(const ENetAddress *address, char *hostName, size_t nameLength);

  /** @} */

  /**
    Creates a packet that may be sent to a peer.
    @param data initial contents of the packet's data; the packet's data will remain uninitialized if data is NULL.
    @param dataLength size of the data allocated for this packet
    @param flags flags for this packet as described for the ENetPacket structure.
    @returns the packet on success, NULL on failure
  */
  ENET_API ENetPacket *enet_packet_create(const void *, size_t, enet_uint32);

  /**
    Destroys the packet and deallocates its data.
    @param packet packet to be destroyed
  */
  ENET_API void enet_packet_destroy(ENetPacket *);

  /**
    Attempts to resize the data in the packet to length specified in the
    dataLength parameter.
    @param packet packet to resize
    @param dataLength new size for the packet data
    @retval 0 on success
    @retval < 0 on failure
  */
  ENET_API int enet_packet_resize(ENetPacket *, size_t);

  /**
    Computes the CRC-32 checksum of the given buffers and returns it in network byte order.
    @param buffers The buffers to checksum.
    @param bufferCount The number of buffers.
    @returns The CRC-32 checksum in network byte order.
  */
  ENET_API enet_uint32 enet_crc32(const ENetBuffer *, size_t);

  /**
    Creates a host for communicating to peers.
    @param address The address at which other peers may connect to this host. If NULL, then no peers may connect to the host.
    @param peerCount The maximum number of peers that should be allocated for the host.
    @param channelLimit The maximum number of channels allowed; if 0, then this is equivalent to ENET_PROTOCOL_MAXIMUM_CHANNEL_COUNT
    @param incomingBandwidth Downstream bandwidth of the host in bytes/second; if 0, ENet will assume unlimited bandwidth.
    @param outgoingBandwidth Upstream bandwidth of the host in bytes/second; if 0, ENet will assume unlimited bandwidth.
    @param option The addressing mode to use: ENET_HOSTOPT_IPV4 (default), ENET_HOSTOPT_IPV6_ONLY, or ENET_HOSTOPT_IPV6_DUALMODE.
    @returns The host on success and NULL on failure
    @remarks ENet will strategically drop packets on specific sides of a connection between hosts
             to ensure the host's bandwidth is not overwhelmed. The bandwidth parameters also determine
             the window size of a connection which limits the amount of reliable packets that may be in transit
             at any given time.
  */
  ENET_API ENetHost *enet_host_create(const ENetAddress *, size_t, size_t, enet_uint32, enet_uint32, ENetHostOption);

  /**
    Destroys the host and all resources associated with it.
    @param host pointer to the host to destroy
  */
  ENET_API void enet_host_destroy(ENetHost *);

  /**
    Initiates a connection to a foreign host.
    @param host host seeking the connection
    @param address destination for the connection
    @param channelCount number of channels to allocate
    @param data user data supplied to the receiving host
    @returns a peer representing the foreign host on success, NULL on failure
    @remarks The peer returned will have not completed the connection until enet_host_service()
             notifies of an ENET_EVENT_TYPE_CONNECT event for the peer.
  */
  ENET_API ENetPeer *enet_host_connect(ENetHost *, const ENetAddress *, size_t, enet_uint32);

  /**
    Checks for any queued events on the host and dispatches one if available.
    @param host host to check for events
    @param event an event structure where event details will be placed if available
    @retval > 0 if an event was dispatched
    @retval 0 if no events are available
    @retval < 0 on failure
  */
  ENET_API int enet_host_check_events(ENetHost *, ENetEvent *);

  /**
    Waits for events on the host specified and shuttles packets between
    the host and its peers.
    @param host host to service
    @param event an event structure where event details will be placed if one occurs
           if event == NULL then no events will be delivered
    @param timeout number of milliseconds that ENet should wait for events
    @retval > 0 if an event occurred within the specified time limit
    @retval 0 if no event occurred
    @retval < 0 on failure
    @remarks enet_host_service should be called fairly regularly for adequate performance
  */
  ENET_API int enet_host_service(ENetHost *, ENetEvent *, enet_uint32);

  /**
    Sends any queued packets on the host specified to its designated peers.
    @param host host to flush
    @remarks This function need only be used in circumstances where one wishes to send queued packets earlier than in a call to
             enet_host_service().
  */
  ENET_API void enet_host_flush(ENetHost *);

  /**
    Queues a packet to be sent to all peers associated with the host.
    @param host host on which to broadcast the packet
    @param channelID channel on which to broadcast
    @param packet packet to broadcast
    @remarks This function always transfers ownership of the packet to the host. If no peer is
             connected (and thus the packet is never queued), the packet is destroyed.
  */
  ENET_API void enet_host_broadcast(ENetHost *, enet_uint8, ENetPacket *);

  /**
    Sets the packet compressor the host should use to compress and decompress packets.
    @param host host to enable or disable compression for
    @param compressor callbacks for the packet compressor; if NULL, then compression is disabled
  */
  ENET_API void enet_host_compress(ENetHost *, const ENetCompressor *);

  /**
    Sets the packet compressor the host should use to the default range coder.
    @param host host to enable the range coder for
    @retval 0 on success
    @retval < 0 on failure
  */
  ENET_API int enet_host_compress_with_range_coder(ENetHost *host);

  /**
    Limits the maximum allowed channels of future incoming connections.
    @param host host to limit
    @param channelLimit the maximum number of channels allowed; if 0, then this is equivalent to ENET_PROTOCOL_MAXIMUM_CHANNEL_COUNT
  */
  ENET_API void enet_host_channel_limit(ENetHost *, size_t);

  /**
    Adjusts the bandwidth limits of a host.
    @param host host to adjust
    @param incomingBandwidth new incoming bandwidth
    @param outgoingBandwidth new outgoing bandwidth
    @remarks the incoming and outgoing bandwidth parameters are identical in function to those
             specified in enet_host_create().
  */
  ENET_API void enet_host_bandwidth_limit(ENetHost *, enet_uint32, enet_uint32);

  /**
    Recomputes the packet throttle limits of all connected peers to respect the host bandwidth constraints.
    @param host The host to throttle.
  */
  extern void enet_host_bandwidth_throttle(ENetHost *);

  /** Sets the checksum callback used by the host.
      @param host The host whose checksum callback is being set.
      @param checksum The checksum callback to use, or NULL to disable checksums.
  */
  ENET_API void enet_host_checksum(ENetHost *host, ENetChecksumCallback checksum);

  /** Enables packet checksums for the host by setting its checksum callback to the built-in CRC-32 implementation.
      @param host The host on which to enable CRC-32 packet checksums.
  */
  ENET_API void enet_host_checksum_with_crc32(ENetHost *host);

  /** Sets the intercept callback used by the host.
      @param host The host whose intercept callback is being set.
      @param intercept The intercept callback to use, or NULL to disable interception.
  */
  ENET_API void enet_host_intercept(ENetHost *host, ENetInterceptCallback intercept);

  /** Sets the maximum number of duplicate peers that the host will track.
      @param host The host whose duplicate peer limit is being set.
      @param duplicatePeers The maximum number of duplicate peers to maintain; if 0, the default is used.
  */
  ENET_API void enet_host_duplicate_peers(ENetHost *host, size_t duplicatePeers);

  /** Sets the maximum allowable packet size that may be sent or received on a peer.
      @param host The host whose maximum packet size is being set.
      @param maximumPacketSize The maximum allowable packet size; if 0, the default is used.
  */
  ENET_API void enet_host_maximum_packet_size(ENetHost *host, size_t maximumPacketSize);

  /** Sets the maximum aggregate amount of buffer space a peer may use waiting for packets to be delivered.
      @param host The host whose maximum waiting data is being set.
      @param maximumWaitingData The maximum aggregate waiting data; if 0, the default is used.
  */
  ENET_API void enet_host_maximum_waiting_data(ENetHost *host, size_t maximumWaitingData);

  /** Sets whether the host ignores incoming connection requests.
      @param host The host on which to set the ignore-connection-requests behavior.
      @param ignoreConnectRequests Non-zero to ignore incoming connection requests, or zero to accept them.
  */
  ENET_API void enet_host_ignore_connect_requests(ENetHost *host, int ignoreConnectRequests);

  /** Sets the MTU of the host.
      @param host The host whose MTU is being set.
      @param mtu The MTU to set, in bytes. If 0, the host default MTU is used.
      @retval 0 on success
      @retval -1 if the MTU exceeds ENET_PROTOCOL_MAXIMUM_MTU
  */
  ENET_API int enet_host_mtu(ENetHost *host, enet_uint32 mtu);

  /** Gets the peer associated with the specified incoming peer identifier.
      @param host The host whose peer is being retrieved.
      @param incomingPeerID The local identifier of the peer slot to retrieve within the host.
      @returns A pointer to the peer at the specified slot, or NULL if incomingPeerID is out of range
               of the host's pre-allocated peers array.
      @remarks The identifier corresponds to a fixed slot in the host's internal peers array, which is
               allocated at host creation time based on the peerCount parameter. It is the index into
               that array and does not verify whether the peer is currently connected.
  */
  ENET_API ENetPeer *enet_host_get_peer(ENetHost *host, enet_uint16 incomingPeerID);

  /** Sends a 1-byte dummy packet directly to the specified address without queuing.
      This is typically used for NAT hole-punching or to elicit a response from a remote host.
      @param host host ping the address
      @param address The destination address to ping.
      @retval 0 if the packet was successfully sent
      @retval -1 otherwise
      @remarks The packet contains a single byte of arbitrary data and is sent immediately via the
               host's socket, bypassing the usual ENet queuing and reliability mechanisms.
               This function does not affect the peer's state or round-trip time statistics.
  */
  ENET_API int enet_host_ping(ENetHost *host, const ENetAddress *address);

  /** Queues a packet to be sent to the connected peers selected by the supplied bit array.
      @param host host on which to broadcast the packet
      @param channelID channel on which to broadcast
      @param bitArray a bit array in which bit i (i.e. the bit at byte i / 8, bit offset i % 8)
                      selects the peer whose incoming peer identifier is i
      @param bitArrayLength the length of the bit array in bytes
      @param packet packet to broadcast
      @remarks Only peers that are both selected by bitArray and currently in the connected state
               receive the packet. Bits beyond host->peerCount are ignored. This function always
               transfers ownership of the packet to the host. If no selected peer is connected (and
               thus the packet is never queued), the packet is destroyed.
  */
  ENET_API void enet_host_broadcast_selected(ENetHost *host, enet_uint8 channelID, const enet_uint8 *bitArray, size_t bitArrayLength, ENetPacket *packet);

  /**
    Generates a new random seed for a host.
    @returns the generated random seed.
  */
  extern enet_uint32 enet_host_random_seed(void);

  /**
    Returns a random value from the host's random number generator.
    @param host The host whose random number generator is used.
    @returns a random value.
  */
  extern enet_uint32 enet_host_random(ENetHost *);

  /**
    Queues a packet to be sent.
    @param peer destination for the packet
    @param channelID channel on which to send
    @param packet packet to send
    @retval 0 on success
    @retval < 0 on failure
    @remarks On success, ENet will assume ownership of the packet, and so enet_packet_destroy
             should not be called on it thereafter. On failure, the caller still must destroy
             the packet on its own as ENet has not queued the packet. The caller can also
             check the packet's referenceCount field after sending to check if ENet queued
             the packet and thus incremented the referenceCount.
  */
  ENET_API int enet_peer_send(ENetPeer *, enet_uint8, ENetPacket *);

  /**
    Attempts to dequeue any incoming queued packet.
    @param peer peer to dequeue packets from
    @param channelID holds the channel ID of the channel the packet was received on success
    @returns a pointer to the packet, or NULL if there are no available incoming queued packets
  */
  ENET_API ENetPacket *enet_peer_receive(ENetPeer *, enet_uint8 *channelID);

  /**
    Sends a ping request to a peer.
    @param peer destination for the ping request
    @remarks ping requests factor into the mean round trip time as designated by the
             roundTripTime field in the ENetPeer structure. ENet automatically pings all connected
             peers at regular intervals, however, this function may be called to ensure more
             frequent ping requests.
  */
  ENET_API void enet_peer_ping(ENetPeer *);

  /**
    Sets the interval at which pings will be sent to a peer.
    @param peer the peer to adjust
    @param pingInterval the interval at which to send pings; defaults to ENET_PEER_PING_INTERVAL if 0
    @remarks Pings are used both to monitor the liveness of the connection and also to dynamically
             adjust the throttle during periods of low traffic so that the throttle has reasonable
             responsiveness during traffic spikes.
  */
  ENET_API void enet_peer_ping_interval(ENetPeer *, enet_uint32);

  /**
    Sets the timeout parameters for a peer.
    @param peer the peer to adjust
    @param timeoutLimit the timeout limit; defaults to ENET_PEER_TIMEOUT_LIMIT if 0
    @param timeoutMinimum the timeout minimum; defaults to ENET_PEER_TIMEOUT_MINIMUM if 0
    @param timeoutMaximum the timeout maximum; defaults to ENET_PEER_TIMEOUT_MAXIMUM if 0
    @remarks The timeout parameters control how and when a peer will timeout from a failure to acknowledge
             reliable traffic. Timeout values use an exponential backoff mechanism, where if a reliable
             packet is not acknowledged within some multiple of the average RTT plus a variance tolerance,
             the timeout will be doubled until it reaches a set limit. If the timeout is thus at this
             limit and reliable packets have been sent but not acknowledged within a certain minimum time
             period, the peer will be disconnected. Alternatively, if reliable packets have been sent
             but not acknowledged for a certain maximum time period, the peer will be disconnected regardless
             of the current timeout limit value.
  */
  ENET_API void enet_peer_timeout(ENetPeer *, enet_uint32, enet_uint32, enet_uint32);

  /**
    Forcefully disconnects a peer.
    @param peer peer to forcefully disconnect
    @remarks The foreign host represented by the peer is not notified of the disconnection and will timeout
             on its connection to the local host.
  */
  ENET_API void enet_peer_reset(ENetPeer *);

  /**
    Request a disconnection from a peer.
    @param peer peer to request a disconnection
    @param data data describing the disconnection
    @remarks An ENET_EVENT_DISCONNECT event will be generated by enet_host_service()
             once the disconnection is complete.
  */
  ENET_API void enet_peer_disconnect(ENetPeer *, enet_uint32);

  /**
    Force an immediate disconnection from a peer.
    @param peer peer to disconnect
    @param data data describing the disconnection
    @remarks No ENET_EVENT_DISCONNECT event will be generated. The foreign peer is not
             guaranteed to receive the disconnect notification, and is reset immediately upon
             return from this function.
  */
  ENET_API void enet_peer_disconnect_now(ENetPeer *, enet_uint32);

  /**
    Request a disconnection from a peer, but only after all queued outgoing packets are sent.
    @param peer peer to request a disconnection
    @param data data describing the disconnection
    @remarks An ENET_EVENT_DISCONNECT event will be generated by enet_host_service()
             once the disconnection is complete.
  */
  ENET_API void enet_peer_disconnect_later(ENetPeer *, enet_uint32);

  /**
    Configures throttle parameter for a peer.
    @param peer peer to configure
    @param interval interval, in milliseconds, over which to measure lowest mean RTT; the default value is ENET_PEER_PACKET_THROTTLE_INTERVAL.
    @param acceleration rate at which to increase the throttle probability as mean RTT declines
    @param deceleration rate at which to decrease the throttle probability as mean RTT increases
    @remarks Unreliable packets are dropped by ENet in response to the varying conditions
             of the Internet connection to the peer. The throttle represents a probability
             that an unreliable packet should not be dropped and thus sent by ENet to the peer.
             The lowest mean round trip time from the sending of a reliable packet to the
             receipt of its acknowledgement is measured over an amount of time specified by
             the interval parameter in milliseconds. If a measured round trip time happens to
             be significantly less than the mean round trip time measured over the interval,
             then the throttle probability is increased to allow more traffic by an amount
             specified in the acceleration parameter, which is a ratio to the ENET_PEER_PACKET_THROTTLE_SCALE
             constant. If a measured round trip time happens to be significantly greater than
             the mean round trip time measured over the interval, then the throttle probability
             is decreased to limit traffic by an amount specified in the deceleration parameter, which
             is a ratio to the ENET_PEER_PACKET_THROTTLE_SCALE constant. When the throttle has
             a value of ENET_PEER_PACKET_THROTTLE_SCALE, no unreliable packets are dropped by
             ENet, and so 100% of all unreliable packets will be sent. When the throttle has a
             value of 0, all unreliable packets are dropped by ENet, and so 0% of all unreliable
             packets will be sent. Intermediate values for the throttle represent intermediate
             probabilities between 0% and 100% of unreliable packets being sent. The bandwidth
             limits of the local and foreign hosts are taken into account to determine a
             sensible limit for the throttle probability above which it should not raise even in
             the best of conditions.
  */
  ENET_API void enet_peer_throttle_configure(ENetPeer *, enet_uint32, enet_uint32, enet_uint32);

  /** Sets the application private data associated with a peer.
      @param peer peer whose data is being set
      @param data the application private data to associate with the peer
  */
  ENET_API void enet_peer_data(ENetPeer *peer, void *data);

  /**
    Recomputes the peer packet throttle based on the current round trip time.
    @param peer the peer whose throttle is being adjusted
    @param rtt the current round trip time in milliseconds
    @retval 1 if the throttle was adjusted
    @retval 0 otherwise
  */
  extern int enet_peer_throttle(ENetPeer *, enet_uint32);

  /**
    Resets all outgoing and incoming command queues of a peer.
    @param peer the peer whose queues are being reset
  */
  extern void enet_peer_reset_queues(ENetPeer *);

  /**
    Checks whether a peer has any outgoing commands queued.
    @param peer the peer to check
    @retval 1 if the peer has outgoing commands
    @retval 0 otherwise
  */
  extern int enet_peer_has_outgoing_commands(ENetPeer *);

  /**
    Initializes the fields of an outgoing command before it is queued.
    @param peer the peer the command is being queued for
    @param outgoingCommand the outgoing command to set up
  */
  extern void enet_peer_setup_outgoing_command(ENetPeer *, ENetOutgoingCommand *);

  /**
    Queues an outgoing command for transmission to a peer.
    @param peer the peer the command is being queued for
    @param command the protocol command to queue
    @param packet the packet associated with the command, or NULL for control commands
    @param reliableSequenceNumber the reliable sequence number assigned to the command
    @param channelID the channel on which the command is sent
    @returns the queued outgoing command, or NULL on failure
  */
  extern ENetOutgoingCommand *enet_peer_queue_outgoing_command(ENetPeer *, const ENetProtocol *, ENetPacket *, enet_uint32, enet_uint16);

  /**
    Queues an incoming command for processing or reassembly.
    @param peer the peer the command was received from
    @param command the protocol command received
    @param packetData the payload data of the command, or NULL if none
    @param dataLength the length of the payload data
    @param flags flags for the incoming command
    @param fragmentCount the total number of fragments expected
    @returns the queued incoming command, or NULL on failure
  */
  extern ENetIncomingCommand *enet_peer_queue_incoming_command(ENetPeer *, const ENetProtocol *, const void *, size_t, enet_uint32, enet_uint32);

  /**
    Queues an acknowledgement for a reliably received command.
    @param peer the peer the command was received from
    @param command the protocol command being acknowledged
    @param sentTime the sent time reported by the acknowledged command
    @returns the queued acknowledgement, or NULL on failure
  */
  extern ENetAcknowledgement *enet_peer_queue_acknowledgement(ENetPeer *, const ENetProtocol *, enet_uint16);

  /**
    Dispatches any complete unreliable incoming commands on a channel to the application.
    @param peer the peer the commands were received from
    @param channel the channel whose commands are being dispatched
    @param incomingCommand the incoming command to start dispatching from
  */
  extern void enet_peer_dispatch_incoming_unreliable_commands(ENetPeer *, ENetChannel *, ENetIncomingCommand *);

  /**
    Dispatches any complete reliable incoming commands on a channel to the application.
    @param peer the peer the commands were received from
    @param channel the channel whose commands are being dispatched
    @param incomingCommand the incoming command to start dispatching from
  */
  extern void enet_peer_dispatch_incoming_reliable_commands(ENetPeer *, ENetChannel *, ENetIncomingCommand *);

  /**
    Handles the completion of a connection for a peer.
    @param peer the peer whose connection has completed
  */
  extern void enet_peer_on_connect(ENetPeer *);

  /**
    Handles the disconnection of a peer.
    @param peer the peer that has disconnected
  */
  extern void enet_peer_on_disconnect(ENetPeer *);

  /**
    Creates a range coder context for the default compressor.
    @returns a new range coder context, or NULL on failure.
  */
  ENET_API void *enet_range_coder_create(void);

  /**
    Destroys a range coder context.
    @param context the range coder context to destroy.
  */
  ENET_API void enet_range_coder_destroy(void *);

  /**
    Compresses the given buffers using the range coder.
    @param context the range coder context.
    @param inBuffers the buffers holding the data to compress.
    @param inBufferCount the number of buffers.
    @param inLimit the total number of bytes to compress.
    @param outData the destination buffer.
    @param outLimit the maximum number of bytes to output.
    @returns the number of bytes written to outData, or 0 on failure.
  */
  ENET_API size_t enet_range_coder_compress(void *, const ENetBuffer *, size_t, size_t, enet_uint8 *, size_t);

  /**
    Decompresses the given data using the range coder.
    @param context the range coder context.
    @param inData the data to decompress.
    @param inLimit the number of bytes of data to decompress.
    @param outData the destination buffer.
    @param outLimit the maximum number of bytes to output.
    @returns the number of bytes written to outData, or 0 on failure.
  */
  ENET_API size_t enet_range_coder_decompress(void *, const enet_uint8 *, size_t, enet_uint8 *, size_t);

  /**
    Returns the size in bytes of the protocol command with the given command type.
    @param commandType the type of the protocol command.
    @returns the size in bytes of the command.
  */
  extern size_t enet_protocol_command_size(enet_uint8);

#ifdef __cplusplus
}
#endif

#endif /* __ENET_ENET_H__ */
