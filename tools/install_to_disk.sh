#!/bin/bash
set -e

DISK_IMG="${1:-disk.img}"
KERNEL_BIN="kernel.bin"

if [ ! -f "$DISK_IMG" ]; then
    echo "Error: $DISK_IMG not found."
    exit 1
fi

if [ ! -f "$KERNEL_BIN" ]; then
    echo "Error: $KERNEL_BIN not found - run 'make' first."
    exit 1
fi

echo "==> Attaching $DISK_IMG"
LOOPDEV=$(sudo losetup -P --show -f "$DISK_IMG")
echo "    using $LOOPDEV"

cleanup() {
    echo "==> Cleaning up"
    sudo umount /mnt/alder 2>/dev/null || true
    sudo losetup -d "$LOOPDEV" 2>/dev/null || true
}
trap cleanup EXIT

sudo mkdir -p /mnt/alder
sudo mount -t vfat "${LOOPDEV}p1" /mnt/alder

if ! mount | grep -q alder; then
    echo "Error: mount failed - did you run 'install' inside AlderKernel first?"
    exit 1
fi

echo "==> Copying kernel.bin"
sudo mkdir -p /mnt/alder/boot/grub
sudo cp "$KERNEL_BIN" /mnt/alder/boot/kernel.bin

cat << 'GRUBCFG' | sudo tee /mnt/alder/boot/grub/grub.cfg > /dev/null
menuentry "AlderKernel" {
    multiboot /boot/kernel.bin
}
GRUBCFG

echo "==> Installing GRUB"
sudo grub-install --target=i386-pc --boot-directory=/mnt/alder/boot --modules="part_msdos fat multiboot biosdisk" "$LOOPDEV"

echo "==> Done. Boot with: qemu-system-i386 -hda $DISK_IMG"