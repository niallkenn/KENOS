# Big dumb brain-dump
##### (say that quick)

###### 04/08
so the memory allocator I have does things well. Obviously not well enough. Its just allocating from a
static array which is no good no good for my high high standards (not satisfactory). I want it to *live in
actual RAM.* instead of MemoryAllocator having
uint8_t heap[256 \* 1024];

I want it to have

**uint8_t\* heap_start;**
**size_t heap_size;**

To get these I need a **E820 memory map**, it's constructed in BIOS and it returns a list of places in memory and
their type. An output has a base physical address, length of memory area and type, like free memory, reserved memory
and whatever else complicated other thingys. So I baically need a section of memory, that is of type *free memory, and
pull its address into heap start, and its size into heap size*. Easy i did the same thing before except that was riding
a bike compared to this being flying a plane. Easy. 

I obv want the heap to be huge because it's supposed to be. for that the ***free memory parts should be together. A pager would do this by using memory map to defragment the free memory 
into virtual memory***, and also hide the memory that the kernel lives in. This would allow huge RAM which is what I want
but its literally gonna take weeks.

I want the challenge but chatgpt thinks i'm dumb so I will argue with it about whether i should write a pager instead of something boring it suggests to solve my huge problem.

look at this chatgpt wrote this i didtn type this

Go learn paging.

waheyyy fuck you 

ok lock in. 

i did a bit of reading and this shit is genuinely so complicated i need to sleep holy mother of jesus