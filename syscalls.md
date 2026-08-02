# AlderKernel Syscalls

`req-syscallop <number>` takes a **decimal** argument (str_to_uint has no
hex parsing yet), so both forms are listed below - use the Dec column
when typing into the shell.

| Hex  | Dec | Name              | Status                                          |
|------|-----|-------------------|--------------------------------------------------|
| 0x00 | 0   | Read              | TODO - needs VFS-backed file reads              |
| 0x01 | 1   | Write             | TODO - needs VFS-backed file writes             |
| 0x02 | 2   | Request more RAM  | TODO - needs heap allocator                     |
| 0x03 | 3   | ???               | Reserved, not defined yet                       |
| 0x10 | 16  | Kernel panic      | Implemented - prints banner, cli + hlt loop     |
| 0x11 | 17  | Force divide-by-zero (test) | Implemented - triggers real #DE (vector 0) to exercise the exception handler |

(more later)

## Notes

- Numbers not in this table print `Unknown syscall number: <n>` and
  return normally (does not halt).
- 0x10 and 0x11 both halt the CPU (0x10 intentionally via `cli`+`hlt`,
  0x11 by causing a genuine CPU fault) - after either, the shell will
  not respond to further input; that's expected, not a bug.
- Source of truth for behavior: `kernel/syscalls/syscall.c`.