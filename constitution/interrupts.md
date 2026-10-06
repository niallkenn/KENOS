IDT: All processor exceptions and hardware interrupts are dispatched through the Interrupt Descriptor Table.
Exceptions: CPU-generated faults, traps, and aborts are handled by dedicated kernel handlers.
Hardware interrupts: Hardware IRQs are routed through the interrupt controller to kernel interrupt handlers.
Privilege transition: Interrupts originating from Ring 3 must safely transition to a Ring 0 kernel stack.
Context: Interrupt entry saves sufficient CPU state to restore the interrupted execution context.
Return: Interrupt handlers return using the architecture-defined interrupt-return mechanism.
PIC: Legacy PIC support is used initially.
APIC: Local APIC/IOAPIC support is planned for modern hardware and SMP.
System calls: System calls use a controlled user-to-kernel entry mechanism and must validate all user-supplied arguments.
Safety: User processes must never be able to invoke arbitrary kernel code or modify interrupt descriptors.