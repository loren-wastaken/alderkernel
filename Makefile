# object files
OBJ = boot.o main.o print.o

# flags and path to headers folder
CFLAGS = -m32 -ffreestanding -O0 -fno-pic -fno-pie -fno-stack-protector -Ikernel/headers -Ikernel -c

all: moj_system.iso

boot.o: boot.asm
	# assemble bootloader to elf32 for grub
	nasm -f elf32 boot.asm -o boot.o
	
main.o: kernel/main.c
	# compile main kernel code
	gcc $(CFLAGS) kernel/main.c -o main.o

print.o: kernel/print.c
	# compile printing and screen functions
	gcc $(CFLAGS) kernel/print.c -o print.o

kernel.bin: $(OBJ)
	# link everything using linker script
	ld -m elf_i386 -T linker.ld --build-id=none $(OBJ) -o kernel.bin

moj_system.iso: kernel.bin
	# build grub iso image
	mkdir -p iso/boot/grub
	cp kernel.bin iso/boot/
	cp grub.cfg iso/boot/grub/
	grub-mkrescue -o moj_system.iso iso

clean:
	rm -rf *.o *.bin moj_system.iso iso/

run: moj_system.iso
	# run iso in qemu
	qemu-system-i386 -cdrom moj_system.iso
