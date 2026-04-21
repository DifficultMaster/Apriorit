# Apriorit4 — TCP Client/Server Echo Example (C++23, CMake)

This repository contains a small networking exercise implemented in C++23 using C-style Unix POSIX sockets.

It builds two console applications:
- **Server** — listens on a TCP port and handles multiple clients concurrently.
- **Client** — connects to the server and sends interactive messages.

The shared networking logic is implemented in `UnixSocketService` inside the `shared` module.

## Table of Contents
- [Overview](#overview)
- [System Requirements](#system-requirements)
- [Build](#build)
- [Run](#run)
- [How It Works](#how-it-works)
- [Message Protocol](#message-protocol)
- [Error Handling](#error-handling)
- [Troubleshooting](#troubleshooting)
- [Future Improvements](#future-improvements)

## Overview

The project demonstrates:
- Basic socket lifecycle management (create, bind/listen, accept, connect, send/receive, close).
- RAII ownership for socket descriptors.
- Move-only socket wrapper semantics.
- Simple per-client threading model on the server (concurrent client handling).

## System Requirements

- **CMake** 3.20+
- **C++23** compiler
- Threading support
- POSIX socket APIs (`arpa/inet.h`, `sys/socket.h`, `unistd.h`)

## Build

From the repository root:

```bash
cmake -S . -B out/build
cmake --build out/build
```

Generated executables are:
- `Server`
- `Client`

## Run

### 1) Start server

```bash
./Server
```

When prompted, enter a port (for example `8080`).

### 2) Start client

```bash
./Client
```

When prompted, enter:
- IP (for local testing: `127.0.0.1`)
- Port (same as server)

Then type messages interactively and receive appropriate responses automatically.

## How It Works

### Server flow
1. Create socket.
2. Bind to selected port.
3. Listen with `SOMAXCONN` backlog.
4. Accept incoming clients.
5. Handle each client in a detached thread (self-destructs upon disconnect).

### Client flow
1. Create socket.
2. Connect to the target IP/port.
3. Read user input.
4. Send user input to server.
5. Print server response.

## Message Protocol

Current protocol is plain text:
- Sending `hello` returns `world`.
- Any other message is echoed back.
- Sending `disconnect` closes the client session.
- Message interpretator is case-sensitive.

## Error Handling

- Networking failures throw `UnixSocketException` derived from `std::runtime_error`.
- Main functions catch in the written order:
  - `UnixSocketException`
  - `std::exception`
  - unknown exceptions
- Socket resource cleanup is handled in `UnixSocketService` destructor.

## Troubleshooting

- **Failed to bind**: port is already in use or insufficient permissions.
- **Failed to connect**: server not running, wrong IP/port, or firewall issues.
- **Immediate disconnect**: verify server process is running and listening on expected port.

## Future Improvements

- Add graceful server shutdown (signal handling and thread coordination).
- Add configurable read/write timeouts.
- Improve protocol framing for messages larger than buffer size (currently is fixed at 1024 bytes).
- Add unit/integration tests for socket wrapper and protocol behavior.
- Add Windows-native socket implementation (`winsock2`) for full portability.
