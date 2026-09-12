# ENet

A modified build of the original [enet](https://github.com/lsalzman/enet).

The only significant change is the socket layer, which is provided by [NativeSockets2](https://www.nuget.org/packages/NativeSockets2). Everything else is minimal — comments and a few extra functions only. **The original ENet protocol stack is completely untouched**, so this library stays fully wire-compatible with the original C library.

## Features

- **Wire-compatible with the original ENet** — no protocol changes, peers built on different ENet implementations interoperate freely.
- **Address-family selection at runtime** — `ENetHostOption` switches a host between IPv4, IPv6-only and IPv6-dualmode without recompiling or configuring macros. IPv4 is genuinely supported instead of being faked through an IPv6 dual-stack socket.
- **Interoperability** — wire-compatible with:
  - the original [enet](https://github.com/lsalzman/enet)
  - [xENet](https://www.nuget.org/packages/xENet/) and the xENet-based [yENet](https://www.nuget.org/packages/yENet/) / [zENet](https://www.nuget.org/packages/zENet/)
  - the ENet built into [Godot](https://github.com/godotengine/godot)

## Changes vs upstream

| Area | Change |
| --- | --- |
| Socket layer | Replaced with NativeSockets2 — native IPv4/IPv6 support |
| Protocol stack | Untouched |
| Everything else | Comments only, plus a few extra functions |

### Extra API

- `enet_host_checksum_with_crc32()` — enable CRC32 packet checksums
- `enet_host_duplicate_peers()` — set the duplicate-peer limit
- `enet_host_maximum_packet_size()` / `enet_host_maximum_waiting_data()` — set per-peer limits
- `enet_host_ignore_connect_requests()` — ignore incoming connect requests
- `enet_host_mtu()` — set the host MTU
- `enet_host_get_peer()` / `enet_host_ping()` — peer lookup and ping helpers
- `enet_host_broadcast_selected()` — broadcast to a selected set of peers
- `enet_peer_data()` — attach application data to a peer
- `enet_address_set_ip_ipv4()` / `enet_address_set_ip_ipv6()` / `enet_address_get_ip()` / `enet_address_get_hostname()` — literal-IP address helpers

## Build

The `Build binaries` workflow ([GitHub Actions](https://github.com/Molth/enet/actions)) produces binaries for Linux (x64/arm64), macOS, Windows x64, Android, iOS, tvOS, watchOS and visionOS.

### a lifetime of living in obscurity, struggling to make ends meet, and enduring humiliation.