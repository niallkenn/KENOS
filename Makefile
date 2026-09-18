CC = i386-elf-gcc
AS = i386-elf-as
LD = i386-elf-gcc

CC_FLAGS = -std=gnu99 -ffreestanding -O2 -Wall -Wextra \
	-I$(SRC_DIR) \
	-I$(SRC_DIR)/sys \
	-I$(SRC_DIR)/processes \
	-I$(SRC_DIR)/interrupts

LD_FLAGS = -ffreestanding -O2 -nostdlib -lgcc

SRC_DIR = src
BUILD_DIR = build
ISO_DIR = iso
ISO_STAGE_DIR = $(BUILD_DIR)/iso

C_SRCS := $(shell find $(SRC_DIR) -type f -name '*.c')
S_SRCS := $(shell find $(SRC_DIR) -type f -name '*.s')

C_OBJS := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/c/%.o, $(C_SRCS))
S_OBJS := $(patsubst $(SRC_DIR)/%.s, $(BUILD_DIR)/s/%.o, $(S_SRCS))

LINKER_SCRIPT = $(SRC_DIR)/linker.ld

KENOS = $(BUILD_DIR)/KENOS
KENOS_ISO = $(ISO_DIR)/KENOS.iso

.PHONY: all clean

all: $(KENOS_ISO)

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

$(ISO_DIR):
	@mkdir -p $(ISO_DIR)/boot/grub

$(ISO_STAGE_DIR):
	@mkdir -p $(ISO_STAGE_DIR)/boot/grub

$(BUILD_DIR)/c/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	@$(CC) $(CC_FLAGS) -c $< -o $@

$(BUILD_DIR)/s/%.o: $(SRC_DIR)/%.s | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	@$(AS) $< -o $@

$(KENOS): $(C_OBJS) $(S_OBJS)
	@$(LD) -T $(LINKER_SCRIPT) -o $(KENOS) $(LD_FLAGS) $^

$(KENOS_ISO): $(ISO_DIR) $(ISO_STAGE_DIR) $(KENOS)
	@cp $(KENOS) $(ISO_STAGE_DIR)/boot/KENOS
	@cp $(SRC_DIR)/grub/grub.cfg $(ISO_STAGE_DIR)/boot/grub/grub.cfg
	@grub-mkrescue -o $(KENOS_ISO) $(ISO_STAGE_DIR)

clean:
	@rm -rf $(BUILD_DIR) $(ISO_DIR)

run: $(KENOS_ISO)
	@qemu-system-i386 -cdrom $(KENOS_ISO)