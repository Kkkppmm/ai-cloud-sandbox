# Security Model

- Namespace isolation for process, mount, IPC, UTS, and optional network separation
- cgroups v2 resource limits loaded from policy files
- seccomp policy loading with no_new_privs and strict/filter modes
- Permission-gated tool execution
- API key authentication and immutable audit logs
