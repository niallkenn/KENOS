SRC_DIR   = src
BUILD_DIR = build

BOOT_SRC   = $(SRC_DIR)/boot/boot.asm
ENTRY_SRC  = $(SRC_DIR)/kernel/kernel_entry.asm
KERNEL_SRC = $(SRC_DIR)/kernel/kernel.cpp
LINKER_SCRIPT = $(SRC_DIR)/linker.ld

BOOT_BIN   = $(BUILD_DIR)/boot.bin
ENTRY_OBJ  = $(BUILD_DIR)/kernel_entry.o
KERNEL_OBJ = $(BUILD_DIR)/kernel.o
KERNEL_BIN = $(BUILD_DIR)/kernel.bin
OS_IMAGE   = $(BUILD_DIR)/os_image.bin

CXXFLAGS = -m32 -ffreestanding -fno-pie -fno-rtti -fno-exceptions -nostdlib -Wall -Wextra

all: $(OS_IMAGE)

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

$(OS_IMAGE): $(BOOT_BIN) $(KERNEL_BIN) | $(BUILD_DIR)
	@cat $(BOOT_BIN) $(KERNEL_BIN) > $(OS_IMAGE)
	@truncate -s 8192 $(OS_IMAGE)

$(BOOT_BIN): $(BOOT_SRC) | $(BUILD_DIR)
	@nasm -f bin $(BOOT_SRC) -o $(BOOT_BIN)

$(KERNEL_BIN): $(ENTRY_OBJ) $(KERNEL_OBJ) $(LINKER_SCRIPT) | $(BUILD_DIR)
	@ld -m elf_i386 -T $(LINKER_SCRIPT) $(ENTRY_OBJ) $(KERNEL_OBJ) -o $(KERNEL_BIN) --oformat binary

$(ENTRY_OBJ): $(ENTRY_SRC) | $(BUILD_DIR)
	@nasm -f elf32 $(ENTRY_SRC) -o $(ENTRY_OBJ)

$(KERNEL_OBJ): $(KERNEL_SRC) | $(BUILD_DIR)
	@g++ $(CXXFLAGS) -c $(KERNEL_SRC) -o $(KERNEL_OBJ)

run: $(OS_IMAGE)
	qemu-system-i386 -fda $(OS_IMAGE)

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all run clean