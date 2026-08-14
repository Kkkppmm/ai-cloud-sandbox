# ai-cloud-sandbox

Production-oriented C++17 Linux sandbox runtime for AI agent execution.

## Build

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

## Components

- Sandbox isolation: namespaces, cgroups, seccomp, resource limits
- Multi-threaded agent manager and lifecycle control
- Safe tool registry + permission-gated tool execution
- Per-agent workspace + quota + atomic writes
- API: socket-based HTTP server plus WebSocket/gRPC components
- Security: API key auth and audit log
- Monitoring: metrics and health checks
