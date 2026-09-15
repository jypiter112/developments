# cpp_tcp_server

A minimal, object-oriented TCP server written in C++ using the Winsock2 API. It's meant as a **base/starting point** for building out more featureful TCP servers on Windows.

## What it does

- Wraps the Winsock2 socket API in a `Server` class (create, listen, accept, get handles, cleanup).
- Binds to `0.0.0.0` on a configurable port (currently hardcoded to **8080** in `main`).
- Accepts a single client connection at a time and echoes back whatever it receives.
- Cleans up sockets and calls `WSACleanup()` on destruction.

## Requirements

- Windows (uses `<WinSock2.h>`, `<WS2tcpip.h>`, `<Windows.h>`)
- A C++ compiler with MSVC or MinGW support
- Links against `Ws2_32.lib` (already declared via `#pragma comment`)

## Build

### MSVC (Developer Command Prompt)
```bash
cl server.cpp /link Ws2_32.lib
```

### MinGW
```bash
g++ server.cpp -o server.exe -lws2_32
```

## Usage

Run the server:
```bash
server.exe
```

It will start listening on port `8080`. Connect with any TCP client (e.g. `telnet`, `nc`, or a custom client) and send data — the server will echo it back and print it to the console.

```bash
telnet localhost 8080
```

## Project structure

```
cpp_tcp_server/
└── server.cpp   # Server class + main() listen/accept/echo loop
```

## Notes

- This is a **base implementation** — no multithreading yet, so only one client is handled at a time in a blocking loop.
- Intended as a starting point to extend with things like multi-client support, a proper protocol/parser, or non-blocking I/O.

## License

No license specified — all rights reserved by default unless stated otherwise.
