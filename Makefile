# object files
OBJ = boot.o main.o print.o idt_asm.o idt_c-code.o io.o ps2_driver.o shell.o interpreter.o pic_driver.o util.o sysinfo_commands.o vfs.o initramfs.o fs_commands.o

# flags and path to headers folder
CFLAGS = -m32 -ffreestanding -O0 -fno-pic -fno-pie -fno-stack-protector -Ikernel/headers -Ikernel -mno-sse -mno-sse2 -mno-mmx -msoft-float -c

all: alderkernel.iso

boot.o: boot.asm
	# assemble bootloader to elf32 for grub
	nasm -f elf32 boot.asm -o boot.o
	
main.o: kernel/main.c
	# compile main kernel code
	gcc $(CFLAGS) kernel/main.c -o main.o

print.o: kernel/print.c
	# compile printing and screen functions
	gcc $(CFLAGS) kernel/print.c -o print.o

idt_asm.o: kernel/idt/idt_asm.asm
	# compile interrupt descriptor table
	nasm -f elf32 kernel/idt/idt_asm.asm -o idt_asm.o

idt_c-code.o: kernel/idt/idt.c
	# compile c version of idt
	gcc $(CFLAGS) kernel/idt/idt.c -o idt_c-code.o

io.o:
	# compile hardware port input/output fn
	gcc $(CFLAGS) kernel/io.c -o io.o

ps2_driver.o:
	# compile ps2 driver
	gcc $(CFLAGS) kernel/drivers/ps2.c -o ps2_driver.o

shell.o:
	# compile zSlash Shell
	gcc $(CFLAGS) kernel/shell/shell.c -o shell.o

interpreter.o:
	# compile Shell command interpreter
	gcc $(CFLAGS) kernel/shell/interpreter.c -o interpreter.o

pic_driver.o: kernel/interrupts/pic.c
	# compile pic driver
	gcc $(CFLAGS) kernel/interrupts/pic.c -o pic_driver.o

util.o: kernel/shell/util.c
	# compile shared shell string/number utilities
	gcc $(CFLAGS) kernel/shell/util.c -o util.o

sysinfo_commands.o: kernel/shell/sysinfo_commands.c
	# compile uname/req-syscallop/memtest commands
	gcc $(CFLAGS) kernel/shell/sysinfo_commands.c -o sysinfo_commands.o

vfs.o: kernel/fs/vfs.c
	# compile in-ram virtual file system
	gcc $(CFLAGS) kernel/fs/vfs.c -o vfs.o

initramfs.o: kernel/initramfs.c
	# compile initramfs: mounts the VFS, then hands control to kernel_main
	gcc $(CFLAGS) kernel/initramfs.c -o initramfs.o

fs_commands.o: kernel/shell/fs_commands.c
	# compile ls/cd/pwd/cat shell commands
	gcc $(CFLAGS) kernel/shell/fs_commands.c -o fs_commands.o

kernel.bin: $(OBJ)
	# link everything using linker script
	ld -m elf_i386 -T linker.ld --build-id=none $(OBJ) -o kernel.bin

alderkernel.iso: kernel.bin
	# build grub iso image
	mkdir -p iso/boot/grub
	cp kernel.bin iso/boot/
	cp grub.cfg iso/boot/grub/
	grub-mkrescue -o alderkernel.iso iso

clean:
	rm -rf *.o *.bin alderkernel.iso iso/

run: alderkernel.iso
	# run iso in qemu
	qemu-system-i386 -cdrom alderkernel.iso
