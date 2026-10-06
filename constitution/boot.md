Bootloader: GRUB with the Multiboot2 protocol.
Initial environment: 32-bit protected mode.
Boot interface: Multiboot2 information is converted into a kernel-defined boot_info structure.
Kernel dependency: Kernel subsystems must not depend directly on Multiboot2 structures.
Memory information: The boot layer provides the physical memory map to the memory subsystem.
Framebuffer: The boot layer provides framebuffer information when available.
Modules: Multiboot2 modules are exposed through boot_info.
Future: A UEFI boot path will provide the same kernel-level boot_info interface.