# Building Apps for AlderKernel

**Status: not possible yet.** This document describes the plan, not
something you can do today. It exists so the eventual app-loading
work has a clear target instead of getting designed from scratch
later.

## Why this isn't ready

Running a user-supplied app means the kernel has to:
1. Find it on a real, persistent disk (not the in-RAM initramfs VFS)
2. Understand the disk's partition layout
3. Read a real filesystem's format to pull the file out
4. Parse and load an ELF32 binary into memory
5. Actually jump into it safely, and recover if it misbehaves

Right now AlderKernel has **none** of these. The current `/kernel`
and `/shell` "filesystem" (`kernel/fs/vfs.c`) is a fixed, in-RAM
tree seeded at boot - there is no disk driver, no partition
awareness, and no on-disk filesystem support at all. None of that
is a criticism of where the project is - it's just the honest
starting point for this feature.

## Planned prerequisite chain

In order, since each step depends on the one before it:

1. **Installer** - something that writes AlderKernel to a real disk
   and either installs GRUB or adds a boot entry to an existing
   GRUB install.
2. **Disk detection** - enumerate attached disks (ATA/AHCI at a
   minimum).
3. **Partition detection** - parse a partition table (MBR and/or
   GPT) to find where AlderKernel's partition actually starts.
4. **ext4 read/write support** - read (and eventually write) files
   from a real filesystem on that partition, replacing/extending
   the current in-RAM-only VFS.
5. **ELF32 loader** - parse an ELF32 binary's program headers and
   load its segments into memory correctly.
6. **App boot-check** - on startup, scan the app directory for
   compiled `.elf` apps, load and briefly test each one, and route
   any warning/error output back into the shell (`2>shell`
   equivalent) instead of silently failing.

## Once this exists

This section will be rewritten with the real, working process:
what a "hello world" app's source layout looks like, what to link
against (the kernel won't provide a full libc - probably a small
syscall wrapper library instead, built on `kernel/syscalls/`), how
to compile/link it as ELF32, where to place it so the boot-check
picks it up, and how to read back any errors it reported.

## Related, already in progress

- `kernel/syscalls/` - the syscall interface apps will eventually
  use to talk to the kernel. See `syscalls.md` for the current
  (still mostly stubbed) syscall table.