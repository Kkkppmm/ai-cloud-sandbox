# Security Model

- Namespace isolation for process, mount, IPC, UTS, and optional network separation
- cgroups v2 resource limits
- seccomp strict mode with no_new_privs
- Permission-gated tool execution
- API key authentication and immutable audit logs
