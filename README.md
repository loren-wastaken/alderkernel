# Alder Kernel
## A hobby operating system kernel written from scratch in C and Assembly for educational purposes.

This project is intended for learning and experimentation. You are welcome to use it as a foundation for your own operating system.

**Architecture:** i386 (32-bit x86)

## Features

- PS/2 Keyboard support [X]

- InitramFS 🛠️

- Shell 🛠️

- Shell history [X]

# How to build 
 - Clone git repo
### by typing `git clone https://github.com/loren-wastaken/alderkernel`
 - After downloading
### `cd` into the directory alderkernel
 - Making sure that you have EVERY tool
### This project needs: NASM,gcc,make,ld and QEMU
What these tools do?
### NASM assembles needed files for boot (boot.asm and etc.) that are NEEDED to turn on the kernel.
### gcc or 'GNU Compiler Collection' compiles all .c (C code) files to .o and they get into file kernel.bin.
### make is a tool for [Makefile](Makefile) file that makes compiling easier instead of typing long commands that you can misspell.
### ld links all files so they can get usable.
### QEMU turns on Virtual Machine or 'VM' and makes debugging/turning on machine without rebooting easier!

# How to run

After building:

### Using QEMU

`make run`

or:

`qemu-system-i386 -cdrom alderkernel.iso`

[Author](https://github.com/loren-wastaken)
[License](LICENSE)
                     
