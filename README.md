# Alder Kernel
## Alder Kernel is a hobby operating system kernel made purely for learning. So there might be some errors/inefficient ways to do coding and etc. I do not prefer to make real code from this Kernel.
### This kernel is built for i386 architecture (x86 or x32 how you prefer to say it.).
# Features

1st. PS/2 Keyboard support, needed USB to PS/2 converter or PS/2 compatible keyboard, or start with an VM (PS/2 Keyboard support added!)

2nd. InitramFS (Added!)

3rd. Shell (Added too!)

4th. Shell History (Added!)

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

--------------- How to run -------------------

After building:

### Using QEMU

`make run`

or:

`qemu-system-i386 -cdrom alderkernel.iso`

[Author](https://github.com/loren-wastaken)
[License](LICENSE)
                     
