Physical memory: Managed through a physical frame allocator.
Page size: 4 KiB.
Virtual memory: Every process receives its own virtual address space.
Paging: IA-32 paging initially using non-PAE two-level page tables.
PAE: The memory subsystem must be designed to allow PAE later without changing higher-level VM interfaces.
Kernel mapping: The kernel occupies a fixed higher-half virtual address range shared by all address spaces.
User mapping: User processes occupy the lower portion of their virtual address space.
Protection: User pages are inaccessible from Ring 0/3 as dictated by page permissions; kernel pages are never user-accessible.
Isolation: A process must not be able to read, write, or execute another process's memory.
Page faults: Invalid memory accesses generate page-fault exceptions handled by the VM subsystem.
Allocation: Physical frames and virtual address ranges are separate resources and must be managed independently.
Future: Demand paging, copy-on-write, shared memory, memory mapping, and PAE are planned.