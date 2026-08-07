# Alder Kernel
## A hobby operating system kernel written from scratch in C and Assembly for educational purposes.

This project is intended for learning and experimentation. You are welcome to use it as a foundation for your own operating system.

**Architecture:** i386 (32-bit x86)

## Legend:

🛠️ - Building/working on it

[x] - Working/Stable

## Features

- [x] PS/2 Keyboard support 

- [x] InitramFS 

- [x] Shell

- [x] Shell history 

- [x] Booting from disk

## Requirements

- NASM
- GCC
- GNU Make
- GNU ld
- QEMU

## Build

```bash
git clone https://github.com/loren-wastaken/alderkernel
cd alderkernel
make
```
Create disk if you wanna run with disk
```bash
qemu-img create -f raw disk.img 2G
```
## How to run

```bash
make run
```
or with disk (disk.img needed)
```bash
make run_wdisk
```

or manually:

```bash
qemu-system-i386 -cdrom alderkernel.iso`
```
## Building apps for AlderKernel

Not possible yet - see [apps.md](apps.md) for the planned prerequisite
chain (installer, disk/partition detection, ext4, ELF loading) and
current status.

[Author](https://github.com/loren-wastaken)

**License:** [MIT](LICENSE)
                     
