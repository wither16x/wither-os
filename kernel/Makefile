include $(TOOLCHAIN)

INCLUDE_DIR := include
STDHDR_DIR := $(INCLUDE_DIR)/standard
STDCXX_DIR := $(STDHDR_DIR)/cxx
LIMINE_DIR := $(INCLUDE_DIR)/limine
SRC_DIR := src
BUILD_DIR := build
OBJ_DIR := $(BUILD_DIR)/obj
BIN_DIR := $(BUILD_DIR)/bin
LINKER_SCRIPT := linker_scripts/x86_64.lds
TARGET := $(BIN_DIR)/kernel
CFILES := $(shell find $(SRC_DIR) -name '*.c')
CXXFILES := $(shell find $(SRC_DIR) -name '*.cpp')
ASMFILES := $(shell find $(SRC_DIR) -name '*.asm')
SRCFILES := $(CFILES) $(ASMFILES)
COBJECTS := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(CFILES))
CXXOBJECTS := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(CXXFILES))
ASMOBJECTS := $(patsubst $(SRC_DIR)/%.asm,$(OBJ_DIR)/%.o,$(ASMFILES))
OBJFILES := $(COBJECTS) $(CXXOBJECTS) $(ASMOBJECTS)

NASMFLAGS += -f elf64 \
	-i $(INCLUDE_DIR)/
COMMONFLAGS := -target x86_64-unknown-none-elf \
	-Wall -Wextra -Wpedantic -Wshadow -Wcast-qual -Wsign-conversion \
	-Wstrict-prototypes -Wformat=2 -Wconversion \
	-ffreestanding -fno-common -fno-stack-protector -fno-stack-check -fno-PIC \
	-ffunction-sections -fdata-sections -fno-exceptions \
	-m64 -march=x86-64 -mno-mmx -mno-sse -mno-sse2 -mno-red-zone -mcmodel=kernel \
	-mno-80387 \
	-MMD -MP \
	-g -O2 -pipe
CCFLAGS += -xc -std=gnu11 \
	-nostdinc -I$(INCLUDE_DIR) -I$(LIMINE_DIR) -isystem $(STDHDR_DIR)
CFLAGS += $(COMMONFLAGS)
CXXFLAGS += -xc++ -std=c++23 \
	-Wnon-virtual-dtor -Wold-style-cast \
	-Woverloaded-virtual -Wnull-dereference -Wimplicit-fallthrough \
	-fno-rtti \
	-nostdinc -I$(INCLUDE_DIR) -I$(LIMINE_DIR) -isystem $(STDCXX_DIR) \
	-isystem $(STDHDR_DIR)
CXXFLAGS += $(COMMONFLAGS)
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
	@$(CC) $(CCFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	@echo "CXX $< -> $@"
	@$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.asm
	@mkdir -p $(dir $@)
	@echo "NASM $< -> $@"
	@$(NASM) $(NASMFLAGS) $< -o $@

-include $(OBJFILES:.o=.d)

clean:
	@rm -rf $(BUILD_DIR)