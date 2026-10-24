MAKEFLAGS += --no-print-directory

TOOLCHAIN ?=
ifeq ($(TOOLCHAIN),)
ifneq ($(MAKECMDGOALS),clean)
$(error no toolchain specified)
endif
endif

KERNEL_DIR := kernel

.PHONY: all clean

all:
	@echo "Building kernel..."
	@$(MAKE) -C $(KERNEL_DIR)

clean:
	@echo "Cleaning kernel..."
	@$(MAKE) -C $(KERNEL_DIR) clean