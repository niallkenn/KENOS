NASM = nasm

SRC_DIR   = src
BUILD_DIR = build

BOOT_BIN  = $(BUILD_DIR)/boot.bin

all: $(BOOT_BIN)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BOOT_BIN): $(SRC_DIR)/boot/boot.asm | $(BUILD_DIR)
	$(NASM) -f bin $< -o $@

run: $(BOOT_BIN)
	qemu-system-x86_64 $(BOOT_BIN)

clean:
	rm -rf $(BUILD_DIR)