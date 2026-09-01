SRC_DIR   = src
BUILD_DIR = build

LINKER_SCRIPT = $(SRC_DIR)/linker.ld

KERNEL_BIN = $(BUILD_DIR)/kernel.bin

ASM_SRCS := $(shell find $(SRC_DIR) -type f -name '*.asm' ! -path '$(SRC_DIR)/boot/boot.asm')
CPP_SRCS := $(shell find $(SRC_DIR) -type f -name '*.cpp')
DISK_IMAGE := build/disk.img

ASM_OBJS = $(patsubst $(SRC_DIR)/%.asm, $(BUILD_DIR)/asm/%.o, $(ASM_SRCS))
CPP_OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/cpp/%.o, $(CPP_SRCS))

ALL_OBJS = $(ASM_OBJS) $(CPP_OBJS)

CPPFLAGS = -m32 -ffreestanding -fno-pie -fno-rtti -fno-exceptions -Wall -Wextra \
		-I$(SRC_DIR) \
	    -I$(SRC_DIR)/sys \
		-I$(SRC_DIR)/drivers \
		-I$(SRC_DIR)/shell \
		-I$(SRC_DIR)/memory \
		-I$(SRC_DIR)/interrupts \
		-I$(SRC_DIR)/boot \
		-I$(SRC_DIR)/lib

all: $(OS_IMAGE)

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

$(KERNEL_BIN): $(ALL_OBJS) $(LINKER_SCRIPT) | $(BUILD_DIR)
	@i386-elf-ld -m elf_i386 -nostdlib -T $(LINKER_SCRIPT) $(ALL_OBJS) -o $(KERNEL_BIN) --oformat binary

disk:
	@if [ ! -f "$(DISK_IMAGE)" ]; then \
		dd if=/dev/zero of="$(DISK_IMAGE)" bs=1M count=64; \
	fi

$(BUILD_DIR)/asm/%.o: $(SRC_DIR)/%.asm | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	@nasm -f elf32 $< -o $@

$(BUILD_DIR)/cpp/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	@i386-elf-g++ $(CPPFLAGS) -c $< -o $@

run: $(OS_IMAGE) | disk
	@qemu-system-i386 \
		-drive format=raw,file=$(OS_IMAGE),if=floppy \
		-drive format=raw,file=build/disk.img,if=ide \
		-boot order=a

clean:
	@rm -rf $(BUILD_DIR)

.PHONY: all run clean