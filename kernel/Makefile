include $(TOOLCHAIN)

INCLUDE_DIR := include
STDHDR_DIR := $(INCLUDE_DIR)/standard
LIMINE_DIR := $(INCLUDE_DIR)/limine
SRC_DIR := src
BUILD_DIR := build
OBJ_DIR := $(BUILD_DIR)/obj
BIN_DIR := $(BUILD_DIR)/bin
LINKER_SCRIPT := linker_scripts/x86_64.lds
TARGET := $(BIN_DIR)/kernel
CFILES := $(shell find $(SRC_DIR) -name '*.c')
ASMFILES := $(shell find $(SRC_DIR) -name '*.asm')
SRCFILES := $(CFILES) $(ASMFILES)
COBJECTS := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(CFILES))
ASMOBJECTS := $(patsubst $(SRC_DIR)/%.asm,$(OBJ_DIR)/%.o,$(ASMFILES))
OBJFILES := $(COBJECTS) $(ASMOBJECTS)

NASMFLAGS += -f elf64 \
	-i $(INCLUDE_DIR)/
CCFLAGS += -target x86_64-unknown-none-elf \
	-std=gnu11 \
	-Wall -Wextra -Wpedantic -Wshadow -Wcast-qual -Wsign-conversion \
	-Wstrict-prototypes -Wformat=2 \
	-ffreestanding -fno-common -fno-stack-protector -fno-stack-check -fno-PIC \
	-ffunction-sections -fdata-sections \
	-m64 -march=x86-64 -mno-mmx -mno-sse -mno-sse2 -mno-red-zone -mcmodel=kernel \
	-mno-80387 \
	-nostdinc -I$(INCLUDE_DIR) -I$(LIMINE_DIR) -isystem $(STDHDR_DIR) \
	-MMD -MP \
	-g -O2 -pipe
LDFLAGS += -target x86_64-unknown-none-elf \
	-Wl,--build-id=sha1 \
	-Wl,-z,max-page-size=0x1000 \
	-Wl,-z,noexecstack \
	-Wl,--gc-sections \
	-Wl,-T,$(LINKER_SCRIPT) \
	-Wl,-m,elf_x86_64 \
	-Wl,-no-pie \
	-nostdlib \
	-static

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJFILES)
	@mkdir -p $(dir $@)
	@echo "LD $< -> $@"
	@$(LD) $(LDFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@echo "CC $< -> $@"
	@$(CC) $(CCFLAGS) -c $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.asm
	@mkdir -p $(dir $@)
	@echo "NASM $< -> $@"
	@$(NASM) $(NASMFLAGS) $^ -o $@

-include $(OBJFILES:.o=.d)

clean:
	@rm -rf $(BUILD_DIR)