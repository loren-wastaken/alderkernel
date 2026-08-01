# object files
OBJ_DIR = objects
OBJ = $(addprefix $(OBJ_DIR)/, boot.o main.o print.o idt_asm.o idt_c-code.o io.o ps2_driver.o shell.o interpreter.o pic_driver.o util.o sysinfo_commands.o vfs.o initramfs.o fs_commands.o syscall.o)
# flags and path to headers folder
CFLAGS = -m32 -ffreestanding -O0 -fno-pic -fno-pie -fno-stack-protector -Ikernel/headers -Ikernel -mno-sse -mno-sse2 -mno-mmx -msoft-float -c

all: alderkernel.iso

# rule to ensure objects directory exists
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/boot.o: boot.asm | $(OBJ_DIR)
	# assemble bootloader to elf32 for grub
	nasm -f elf32 boot.asm -o $(OBJ_DIR)/boot.o
	
$(OBJ_DIR)/main.o: kernel/main.c | $(OBJ_DIR)
	# compile main kernel code
	gcc $(CFLAGS) kernel/main.c -o $(OBJ_DIR)/main.o

$(OBJ_DIR)/print.o: kernel/print.c | $(OBJ_DIR)
	# compile printing and screen functions
	gcc $(CFLAGS) kernel/print.c -o $(OBJ_DIR)/print.o

$(OBJ_DIR)/idt_asm.o: kernel/idt/idt_asm.asm | $(OBJ_DIR)
	# compile interrupt descriptor table
	nasm -f elf32 kernel/idt/idt_asm.asm -o $(OBJ_DIR)/idt_asm.o

$(OBJ_DIR)/idt_c-code.o: kernel/idt/idt.c | $(OBJ_DIR)
	# compile c version of idt
	gcc $(CFLAGS) kernel/idt/idt.c -o $(OBJ_DIR)/idt_c-code.o

$(OBJ_DIR)/io.o: kernel/io.c | $(OBJ_DIR)
	# compile hardware port input/output fn
	gcc $(CFLAGS) kernel/io.c -o $(OBJ_DIR)/io.o

$(OBJ_DIR)/ps2_driver.o: kernel/drivers/ps2.c | $(OBJ_DIR)
	# compile ps2 driver
	gcc $(CFLAGS) kernel/drivers/ps2.c -o $(OBJ_DIR)/ps2_driver.o

$(OBJ_DIR)/shell.o: kernel/shell/shell.c | $(OBJ_DIR)
	# compile zSlash Shell
	gcc $(CFLAGS) kernel/shell/shell.c -o $(OBJ_DIR)/shell.o

$(OBJ_DIR)/interpreter.o: kernel/shell/interpreter.c | $(OBJ_DIR)
	# compile Shell command interpreter
	gcc $(CFLAGS) kernel/shell/interpreter.c -o $(OBJ_DIR)/interpreter.o

$(OBJ_DIR)/pic_driver.o: kernel/interrupts/pic.c | $(OBJ_DIR)
	# compile pic driver
	gcc $(CFLAGS) kernel/interrupts/pic.c -o $(OBJ_DIR)/pic_driver.o

$(OBJ_DIR)/util.o: kernel/shell/util.c | $(OBJ_DIR)
	# compile shared shell string/number utilities
	gcc $(CFLAGS) kernel/shell/util.c -o $(OBJ_DIR)/util.o

$(OBJ_DIR)/sysinfo_commands.o: kernel/shell/sysinfo_commands.c | $(OBJ_DIR)
	# compile uname/req-syscallop/memtest commands
	gcc $(CFLAGS) kernel/shell/sysinfo_commands.c -o $(OBJ_DIR)/sysinfo_commands.o

$(OBJ_DIR)/vfs.o: kernel/fs/vfs.c | $(OBJ_DIR)
	# compile in-ram virtual file system
	gcc $(CFLAGS) kernel/fs/vfs.c -o $(OBJ_DIR)/vfs.o

$(OBJ_DIR)/initramfs.o: kernel/initramfs.c | $(OBJ_DIR)
	# compile initramfs: mounts the VFS, then hands control to kernel_main
	gcc $(CFLAGS) kernel/initramfs.c -o $(OBJ_DIR)/initramfs.o

$(OBJ_DIR)/fs_commands.o: kernel/shell/fs_commands.c | $(OBJ_DIR)
	# compile ls/cd/pwd/cat shell commands
	gcc $(CFLAGS) kernel/shell/fs_commands.c -o $(OBJ_DIR)/fs_commands.o

$(OBJ_DIR)/syscall.o: kernel/syscalls/syscall.c | $(OBJ_DIR)
	# compile syscall dispatcher
	gcc $(CFLAGS) kernel/syscalls/syscall.c -o $(OBJ_DIR)/syscall.o
	
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
	rm -rf $(OBJ_DIR) *.bin alderkernel.iso iso/

run: alderkernel.iso
	# run iso in qemu
	qemu-system-i386 -cdrom alderkernel.iso
