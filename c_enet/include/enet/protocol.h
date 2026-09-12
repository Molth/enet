/**
 @file  protocol.h
 @brief ENet protocol
*/
#ifndef __ENET_PROTOCOL_H__
#define __ENET_PROTOCOL_H__

#include "enet/types.h"

enum
{
   /** The smallest MTU accepted by the protocol. */
   ENET_PROTOCOL_MINIMUM_MTU = 576,

   /** The largest MTU accepted by the protocol. */
   ENET_PROTOCOL_MAXIMUM_MTU = 4096,

   /** The maximum number of commands that can be packed into a single packet. */
   ENET_PROTOCOL_MAXIMUM_PACKET_COMMANDS = 32,

   /** The smallest receive window size accepted by the protocol. */
   ENET_PROTOCOL_MINIMUM_WINDOW_SIZE = 4096,

   /** The largest receive window size accepted by the protocol. */
   ENET_PROTOCOL_MAXIMUM_WINDOW_SIZE = 65536,

   /** The smallest number of channels accepted by the protocol. */
   ENET_PROTOCOL_MINIMUM_CHANNEL_COUNT = 1,

   /** The largest number of channels accepted by the protocol. */
   ENET_PROTOCOL_MAXIMUM_CHANNEL_COUNT = 255,

   /** The largest peer identifier that can be encoded in the packet header. */
   ENET_PROTOCOL_MAXIMUM_PEER_ID = 0xFFF,

   /** The maximum number of fragments allowed for a single packet. */
   ENET_PROTOCOL_MAXIMUM_FRAGMENT_COUNT = 1024 * 1024
};

/** The command types that can appear in a protocol packet. */
typedef enum _ENetProtocolCommand
{
   /** An empty command with no meaning. */
   ENET_PROTOCOL_COMMAND_NONE = 0,

   /** Acknowledges receipt of a reliable command. */
   ENET_PROTOCOL_COMMAND_ACKNOWLEDGE = 1,

   /** Initiates a connection from a peer to a host. */
   ENET_PROTOCOL_COMMAND_CONNECT = 2,

   /** Confirms a connection request from a host to a peer. */
   ENET_PROTOCOL_COMMAND_VERIFY_CONNECT = 3,

   /** Requests termination of a connection. */
   ENET_PROTOCOL_COMMAND_DISCONNECT = 4,

   /** A keep-alive probe sent to maintain a connection. */
   ENET_PROTOCOL_COMMAND_PING = 5,

   /** Carries a reliably delivered packet. */
   ENET_PROTOCOL_COMMAND_SEND_RELIABLE = 6,

   /** Carries a packet delivered without acknowledgement. */
   ENET_PROTOCOL_COMMAND_SEND_UNRELIABLE = 7,

   /** Carries a fragment of a fragmented reliable packet. */
   ENET_PROTOCOL_COMMAND_SEND_FRAGMENT = 8,

   /** Carries a packet delivered without sequence ordering. */
   ENET_PROTOCOL_COMMAND_SEND_UNSEQUENCED = 9,

   /** Adjusts the bandwidth limits of the remote peer. */
   ENET_PROTOCOL_COMMAND_BANDWIDTH_LIMIT = 10,

   /** Adjusts the packet throttle parameters of the remote peer. */
   ENET_PROTOCOL_COMMAND_THROTTLE_CONFIGURE = 11,

   /** Carries a fragment of an unreliable packet. */
   ENET_PROTOCOL_COMMAND_SEND_UNRELIABLE_FRAGMENT = 12,

   /** The number of defined command types. */
   ENET_PROTOCOL_COMMAND_COUNT = 13,

   /** Bit mask used to isolate the command type from the command flags. */
   ENET_PROTOCOL_COMMAND_MASK = 0x0F
} ENetProtocolCommand;

/** Flags and header bit masks used by the protocol. */
typedef enum _ENetProtocolFlag
{
   /** Marks a command as requiring an acknowledgement. */
   ENET_PROTOCOL_COMMAND_FLAG_ACKNOWLEDGE = (1 << 7),

   /** Marks a command as unsequenced. */
   ENET_PROTOCOL_COMMAND_FLAG_UNSEQUENCED = (1 << 6),

   /** Header flag indicating the payload is compressed. */
   ENET_PROTOCOL_HEADER_FLAG_COMPRESSED = (1 << 14),

   /** Header flag indicating the packet carries a sent time. */
   ENET_PROTOCOL_HEADER_FLAG_SENT_TIME = (1 << 15),

   /** Bit mask covering all header flags. */
   ENET_PROTOCOL_HEADER_FLAG_MASK = ENET_PROTOCOL_HEADER_FLAG_COMPRESSED | ENET_PROTOCOL_HEADER_FLAG_SENT_TIME,

   /** Bit mask isolating the session field in the packet header. */
   ENET_PROTOCOL_HEADER_SESSION_MASK = (3 << 12),

   /** Bit offset of the session field in the packet header. */
   ENET_PROTOCOL_HEADER_SESSION_SHIFT = 12
} ENetProtocolFlag;

#ifdef _MSC_VER
#pragma pack(push, 1)
#define ENET_PACKED
#elif defined(__GNUC__) || defined(__clang__)
#define ENET_PACKED __attribute__((packed))
#else
#define ENET_PACKED
#endif

/** The fixed header that precedes every protocol packet. */
typedef struct _ENetProtocolHeader
{
   /** The identifier of the sending peer, with session bits in the high nibble. */
   enet_uint16 peerID;

   /** The time the packet was sent, present only when the sent time flag is set. */
   enet_uint16 sentTime;
} ENET_PACKED ENetProtocolHeader;

/** The header shared by every protocol command. */
typedef struct _ENetProtocolCommandHeader
{
   /** The command type combined with its flags. */
   enet_uint8 command;

   /** The channel the command is delivered on. */
   enet_uint8 channelID;

   /** The reliable sequence number of the command. */
   enet_uint16 reliableSequenceNumber;
} ENET_PACKED ENetProtocolCommandHeader;

/** The acknowledgement command used to confirm receipt of reliable commands. */
typedef struct _ENetProtocolAcknowledge
{
   /** The common command header. */
   ENetProtocolCommandHeader header;

   /** The reliable sequence number being acknowledged. */
   enet_uint16 receivedReliableSequenceNumber;

   /** The sent time reported by the acknowledged command. */
   enet_uint16 receivedSentTime;
} ENET_PACKED ENetProtocolAcknowledge;

/** The connection request command sent when a peer initiates a connection. */
typedef struct _ENetProtocolConnect
{
   /** The common command header. */
   ENetProtocolCommandHeader header;

   /** The peer identifier assigned to the connecting peer by the initiator. */
   enet_uint16 outgoingPeerID;

   /** The session identifier of the incoming connection. */
   enet_uint8 incomingSessionID;

   /** The session identifier of the outgoing connection. */
   enet_uint8 outgoingSessionID;

   /** The maximum transmission unit negotiated for the connection. */
   enet_uint32 mtu;

   /** The receive window size requested by the initiator. */
   enet_uint32 windowSize;

   /** The number of channels negotiated for the connection. */
   enet_uint32 channelCount;

   /** The inbound bandwidth limit in bytes per second. */
   enet_uint32 incomingBandwidth;

   /** The outbound bandwidth limit in bytes per second. */
   enet_uint32 outgoingBandwidth;

   /** The packet throttle measurement interval in milliseconds. */
   enet_uint32 packetThrottleInterval;

   /** The packet throttle acceleration rate. */
   enet_uint32 packetThrottleAcceleration;

   /** The packet throttle deceleration rate. */
   enet_uint32 packetThrottleDeceleration;

   /** A unique identifier used to match the connection with its verification. */
   enet_uint32 connectID;

   /** Application defined data carried with the connection request. */
   enet_uint32 data;
} ENET_PACKED ENetProtocolConnect;

/** The connection verification command sent in reply to a connection request. */
typedef struct _ENetProtocolVerifyConnect
{
   /** The common command header. */
   ENetProtocolCommandHeader header;

   /** The peer identifier assigned to the connecting peer by the responder. */
   enet_uint16 outgoingPeerID;

   /** The session identifier of the incoming connection. */
   enet_uint8 incomingSessionID;

   /** The session identifier of the outgoing connection. */
   enet_uint8 outgoingSessionID;

   /** The maximum transmission unit negotiated for the connection. */
   enet_uint32 mtu;

   /** The receive window size accepted by the responder. */
   enet_uint32 windowSize;

   /** The number of channels negotiated for the connection. */
   enet_uint32 channelCount;

   /** The inbound bandwidth limit in bytes per second. */
   enet_uint32 incomingBandwidth;

   /** The outbound bandwidth limit in bytes per second. */
   enet_uint32 outgoingBandwidth;

   /** The packet throttle measurement interval in milliseconds. */
   enet_uint32 packetThrottleInterval;

   /** The packet throttle acceleration rate. */
   enet_uint32 packetThrottleAcceleration;

   /** The packet throttle deceleration rate. */
   enet_uint32 packetThrottleDeceleration;

   /** The connection identifier echoed from the connection request. */
   enet_uint32 connectID;
} ENET_PACKED ENetProtocolVerifyConnect;

/** The bandwidth limit command used to adjust the remote peer bandwidth. */
typedef struct _ENetProtocolBandwidthLimit
{
   /** The common command header. */
   ENetProtocolCommandHeader header;

   /** The inbound bandwidth limit in bytes per second. */
   enet_uint32 incomingBandwidth;

   /** The outbound bandwidth limit in bytes per second. */
   enet_uint32 outgoingBandwidth;
} ENET_PACKED ENetProtocolBandwidthLimit;

/** The throttle configuration command used to adjust the remote peer throttle parameters. */
typedef struct _ENetProtocolThrottleConfigure
{
   /** The common command header. */
   ENetProtocolCommandHeader header;

   /** The packet throttle measurement interval in milliseconds. */
   enet_uint32 packetThrottleInterval;

   /** The packet throttle acceleration rate. */
   enet_uint32 packetThrottleAcceleration;

   /** The packet throttle deceleration rate. */
   enet_uint32 packetThrottleDeceleration;
} ENET_PACKED ENetProtocolThrottleConfigure;

/** The disconnect command used to terminate a connection. */
typedef struct _ENetProtocolDisconnect
{
   /** The common command header. */
   ENetProtocolCommandHeader header;

   /** Application defined data carried with the disconnect request. */
   enet_uint32 data;
} ENET_PACKED ENetProtocolDisconnect;

/** The keep-alive ping command. */
typedef struct _ENetProtocolPing
{
   /** The common command header. */
   ENetProtocolCommandHeader header;
} ENET_PACKED ENetProtocolPing;

/** The reliable send command carrying an acknowledged payload. */
typedef struct _ENetProtocolSendReliable
{
   /** The common command header. */
   ENetProtocolCommandHeader header;

   /** The length in bytes of the payload that follows the command. */
   enet_uint16 dataLength;
} ENET_PACKED ENetProtocolSendReliable;

/** The unreliable send command carrying an unacknowledged payload. */
typedef struct _ENetProtocolSendUnreliable
{
   /** The common command header. */
   ENetProtocolCommandHeader header;

   /** The unreliable sequence number of the command. */
   enet_uint16 unreliableSequenceNumber;

   /** The length in bytes of the payload that follows the command. */
   enet_uint16 dataLength;
} ENET_PACKED ENetProtocolSendUnreliable;

/** The unsequenced send command carrying an out-of-order payload. */
typedef struct _ENetProtocolSendUnsequenced
{
   /** The common command header. */
   ENetProtocolCommandHeader header;

   /** The group used to track the unsequenced payload. */
   enet_uint16 unsequencedGroup;

   /** The length in bytes of the payload that follows the command. */
   enet_uint16 dataLength;
} ENET_PACKED ENetProtocolSendUnsequenced;

/** The fragment send command carrying one piece of a fragmented packet. */
typedef struct _ENetProtocolSendFragment
{
   /** The common command header. */
   ENetProtocolCommandHeader header;

   /** The reliable sequence number of the first fragment of the packet. */
   enet_uint16 startSequenceNumber;

   /** The length in bytes of the payload carried by this fragment. */
   enet_uint16 dataLength;

   /** The total number of fragments the packet was split into. */
   enet_uint32 fragmentCount;

   /** The zero based index of this fragment. */
   enet_uint32 fragmentNumber;

   /** The total length in bytes of the original unfragmented packet. */
   enet_uint32 totalLength;

   /** The byte offset of this fragment within the original packet. */
   enet_uint32 fragmentOffset;
} ENET_PACKED ENetProtocolSendFragment;

/** A union of all protocol commands overlaid at offset zero so the active command is read through the common header. */
typedef union _ENetProtocol
{
   /** The common command header shared by all command variants. */
   ENetProtocolCommandHeader header;

   /** The acknowledgement command view. */
   ENetProtocolAcknowledge acknowledge;

   /** The connection request command view. */
   ENetProtocolConnect connect;

   /** The connection verification command view. */
   ENetProtocolVerifyConnect verifyConnect;

   /** The disconnect command view. */
   ENetProtocolDisconnect disconnect;

   /** The ping command view. */
   ENetProtocolPing ping;

   /** The reliable send command view. */
   ENetProtocolSendReliable sendReliable;

   /** The unreliable send command view. */
   ENetProtocolSendUnreliable sendUnreliable;

   /** The unsequenced send command view. */
   ENetProtocolSendUnsequenced sendUnsequenced;

   /** The fragment send command view. */
   ENetProtocolSendFragment sendFragment;

   /** The bandwidth limit command view. */
   ENetProtocolBandwidthLimit bandwidthLimit;

   /** The throttle configuration command view. */
   ENetProtocolThrottleConfigure throttleConfigure;
} ENET_PACKED ENetProtocol;

#ifdef _MSC_VER
#pragma pack(pop)
#endif

#endif /* __ENET_PROTOCOL_H__ */
