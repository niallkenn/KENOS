BUILD_DIR = build
SRC_DIR = src

BOOT_SRC = $(SRC_DIR)/boot/boot.asm
LINKER_SCRIPT = $(SRC_DIR)/linker.ld
KERNEL_BIN = $(BUILD_DIR)/kernel.bin
ISO_IMAGE = $(BUILD_DIR)/kenos.iso

BOOT_OBJ = $(BUILD_DIR)/boot/boot.o

ASM_SRCS := $(shell find $(SRC_DIR) -type f -name '*.asm' ! -path '$(SRC_DIR)/boot/boot.asm')
CPP_SRCS := $(shell find $(SRC_DIR) -type f -name '*.cpp')

ASM_OBJS = $(patsubst $(SRC_DIR)/%.asm, $(BUILD_DIR)/asm/%.o, $(ASM_SRCS))
CPP_OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/cpp/%.o, $(CPP_SRCS))

ALL_OBJS = $(ASM_OBJS) $(CPP_OBJS) $(BOOT_OBJ)

NASM = nasm
CXX = i386-elf-g++
LD = i386-elf-ld

CPPFLAGS = -m32 -ffreestanding -fno-pie -fno-rtti -fno-exceptions -Wall -Wextra \
	-I$(SRC_DIR) \
	-I$(SRC_DIR)/memory \
	-I$(SRC_DIR)/sys

.PHONY: all clean

all: $(ISO_IMAGE)

$(BOOT_OBJ): $(BOOT_SRC)
	mkdir -p $(dir $@)
	$(NASM) -f elf32 $< -o $@

$(KERNEL_BIN): $(ALL_OBJS) $(LINKER_SCRIPT)
	$(LD) -m elf_i386 -nostdlib -T $(LINKER_SCRIPT) $(ALL_OBJS) -o $@

$(BUILD_DIR)/asm/%.o: $(SRC_DIR)/%.asm
	mkdir -p $(dir $@)
	$(NASM) -f elf32 $< -o $@

$(BUILD_DIR)/cpp/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) -c $< -o $@

$(ISO_IMAGE): $(KERNEL_BIN) grub/grub.cfg
	mkdir -p $(BUILD_DIR)/iso/boot/grub
	cp $(KERNEL_BIN) $(BUILD_DIR)/iso/boot/kernel.bin
	cp grub/grub.cfg $(BUILD_DIR)/iso/boot/grub/grub.cfg
	grub-mkrescue -o $@ $(BUILD_DIR)/iso

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)