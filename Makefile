MAKEFLAGS += --no-print-directory

TOOLCHAIN ?=
ifeq ($(TOOLCHAIN),)
ifeq ($(MAKECMDGOALS),all)
$(error no toolchain specified)
endif
endif

KERNEL_DIR := kernel
BUILD_DIR := build
ISO_DIR := $(BUILD_DIR)/iso
IMAGE_DIR := $(BUILD_DIR)/images
LIMINE_DIR := $(KERNEL_DIR)/dependencies/limine
LIMINE_CONF := config/limine.conf
ISO_IMAGE := $(ISO_DIR)/os.iso

QEMU := qemu-system-x86_64
OVMF := /usr/share/ovmf/OVMF.fd

.PHONY: all iso run-bios run-uefi clean

all:
	@echo "Building kernel..."
	@$(MAKE) -C $(KERNEL_DIR)

iso:
	@echo "Building ISO..."
	@mkdir -p $(ISO_DIR)/boot/limine
	@cp -v $(KERNEL_DIR)/build/bin/kernel $(ISO_DIR)/boot/kernel
	@cp -v $(LIMINE_CONF) $(LIMINE_DIR)/limine-bios.sys \
		$(LIMINE_DIR)/limine-bios-cd.bin $(LIMINE_DIR)/limine-uefi-cd.bin \
		$(ISO_DIR)/boot/limine/
	@mkdir -p $(ISO_DIR)/EFI/BOOT
	@cp -v $(LIMINE_DIR)/BOOTX64.EFI $(ISO_DIR)/EFI/BOOT/
	@cp -v $(LIMINE_DIR)/BOOTIA32.EFI $(ISO_DIR)/EFI/BOOT/
	@mkdir -p $(IMAGE_DIR)
	@xorriso -as mkisofs -R -r -J -b boot/limine/limine-bios-cd.bin \
			-no-emul-boot -boot-load-size 4 -boot-info-table -hfsplus \
			-apm-block-size 2048 --efi-boot boot/limine/limine-uefi-cd.bin \
			-efi-boot-part --efi-boot-image --protective-msdos-label \
			$(ISO_DIR) -o $(ISO_IMAGE)
	@./$(LIMINE_DIR)/limine bios-install $(ISO_IMAGE)

run-bios:
	@$(QEMU) -cdrom $(ISO_IMAGE)

run-uefi:
	@$(QEMU) -cdrom $(ISO_IMAGE) -bios $(OVMF)

clean:
	@echo "Cleaning kernel..."
	@$(MAKE) -C $(KERNEL_DIR) clean
	@echo "Cleaning images..."
	@rm -rf $(BUILD_DIR)