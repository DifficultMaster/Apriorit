# Apriorit5 — gRPC Client/Server File Transfer Example (C++23, CMake)

This repository contains a small networking exercise implemented in C++23 using gRPC.

IMPORTANT: TLS is NOT implemented here due to this not being mentioned as a grading requirements, 
as well as overall complexity of such a task (this already implements gRPC instead of expected sockets).
In real production however, TLS would almost certainly be implemented properly.

It builds two console applications:
- **Server** — listens on a port and handles multiple clients concurrently.
- **Client** — connects to the server and sends files.

The shared networking contract is defined in `shared/protos/file_transfer.proto` and generated into gRPC/Protobuf C++ types.

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

Tested on: Windows 11, Microsoft Visual Studio Community 2026 (18.5.1), CMake 4.2.3-msvc3, Ninja, C++23.

The project demonstrates:
- gRPC-based client/server communication for file upload.
- Streaming file transfer from client to server in binary chunks (64kb each by default).
- Concurrent client handling on server-side gRPC poller threads.
- RAII-based resource management for file streams and gRPC objects.

## System Requirements

- **CMake** 3.20+
- **C++23** compiler
- gRPC and Protobuf C++ toolchain/libraries
- Threading support

Required tools/packages must be available on the receiver machine:
- `protobuf` (C++ library + CMake package)
- `gRPC` (C++ library + CMake package)
- `protoc` (Protocol Buffers compiler)
- `grpc_cpp_plugin` (used by `protoc` to generate gRPC C++ stubs)

This project uses:
- `find_package(Protobuf REQUIRED)`
- `find_package(gRPC REQUIRED)`
- `find_program(gRPC_CPP_PLUGIN_EXECUTABLE grpc_cpp_plugin)`

If these are missing, configure/build will fail.

## Build

From the repository root:

```bash
cmake -S . -B out/build
cmake --build out/build
```

### Windows / Visual Studio (recommended with vcpkg)

If dependencies are not installed system-wide, install with vcpkg and configure CMake with the vcpkg toolchain:

```powershell
vcpkg install grpc protobuf
cmake -S . -B out/build -DCMAKE_TOOLCHAIN_FILE=<path-to-vcpkg>/scripts/buildsystems/vcpkg.cmake
cmake --build out/build
```

Make sure `protoc` and `grpc_cpp_plugin` are available in `PATH` (or otherwise discoverable), because proto sources are generated during build.

Generated executables are:
- `Server` (Windows: `Server.exe`)
- `Client` (Windows: `Client.exe`)

## Run

### 1) Start server

```bash
./Server <ip:port>
```

Example:

```bash
./Server 127.0.0.1:50051
```

### 2) Start client

```bash
./Client <server_ip:port> <path_to_file>
```

Example:

```bash
./Client 127.0.0.1:50051 ./sample.bin
```

The server stores uploaded files with a `server_incoming_` prefix.

## How It Works

### Server flow
1. Parse and validate listen address (`<ipv4:port>`).
2. Build gRPC server and register `FileTransferService`.
3. Configure sync poller threads for concurrent clients.
4. Start listening and serve incoming upload streams.
5. For each request stream: expect metadata first, then data chunks, then write file.

### Client flow
1. Parse CLI args: `<server_ip:port> <path_to_file>`.
2. Open target file in binary mode.
3. Start gRPC upload stream.
4. Send metadata message with original filename.
5. Stream file data chunks, finalize stream, validate server response.

## Message Protocol

Protocol is defined in `file_transfer.proto` (package `apriorit5.network`):
- `rpc UploadFile (stream UploadFileRequest) returns (UploadFileResponse)`
- `UploadFileRequest` is a `oneof`:
  - `metadata` (filename)
  - `data` (bytes chunk)
- `UploadFileResponse` contains:
  - `success` (bool)
  - `message` (string)

## Error Handling

- Client validates file existence/opening and checks all gRPC stream statuses (`Write`, `WritesDone`, `Finish`).
- Server validates protocol order and checks file write/flush operations.
- Main entry points catch and log typed exceptions (`FileIoException`, `GrpcNetworkException`) and `std::exception`.
- Resource cleanup relies on RAII (`std::ifstream`, `std::ofstream`, smart pointers).

## Troubleshooting

- **Server fails to start**: verify listen address format (`<ipv4:port>`), for example `127.0.0.1:50051`.
- **Invalid address (WSA 10049)**: avoid invalid host addresses such as `127.0.0.0`.
- **Upload failed**: check server is running, address/port match, and file path exists.
- **No output file**: verify server process write permissions in current working directory.
- **CMake cannot find gRPC/Protobuf**: install dependencies and pass the vcpkg toolchain file (or set `CMAKE_PREFIX_PATH` to your package install).
- **`grpc_cpp_plugin` not found**: ensure it is installed and accessible in `PATH`.

## Future Improvements

- Add graceful server shutdown via signal handling (`Ctrl+C` friendly shutdown path).
- Add TLS credentials support instead of insecure local credentials.
- Add transfer progress reporting and optional retry policy on client.
- Add automated unit/integration tests for upload success/failure scenarios.
