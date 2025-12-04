
# Virtual Memory

### What is Virtual Memory

Physical memory is limited on real hardware. If a program needs more memory than is available on the device, it takes advantage of **virtual memory**. Virtual memory enables programmers to write code without thinking about the exact physical memory layout of their program.

When a process is created, the stack and heap are defined virtually. That is, the relationship between addresses in the stack and heap are internally consistent, but do not refer to actual phsyical addresses on the device. When the program executes, these virtual addresses are mapped to physical addresses by the memory management unit (MMU). This is a coordinated process between the operating system and hardware.

### Motivation and Drawbacks

The virtual memory abstraction enables the programmer to write code with arbitrary memory usage. The physical device is of course still memory limited, but behind the scenes the operating system will store data too large to fit into DRAM to disk and load it back into DRAM when needed. Making frequent trips to and from disk will hurt performance, but enables a broader class of programs to be written than would be possible otherwise. 

### How VM Works

Facilitating this process involves a kernel data structure called the **Page Table**.

Consider the following parameters


Let $N$ be the number of virtual addresses, $M$ be the number of physical addresses, $P$ be the page size.

$$
N = 2^n
$$

$$
M = 2^m
$$

$$
P = 2^p
$$

Programs that access virtual memory do so via **virtual addresses**. The bits of a virtual address are split into a **virtual page number (VPN)** and a **virtual page offset (VPO)**.

![page_table](../photos/page_table.png)

The overall virtual address takes up $n$ bits. Whereby the VPN occupies the most significant $n-p$ bits. The VPO occupies the least significant $p$ bits.

The VPN is used to index the page table data structure. Each entry of the page table is comprised of a **valid bit**, as well as a **physical page number (PPN)**. The PPN refers to the most significant $m-p$ bits of a physical address. The least significant $p$ bits of the physical address are the same as the $p$ VPO bits. This section of the physical address is called the **physical page offset (PPO)**.

The mechanism that enables the virtual address space to be orders of magnitude larger than what is available in DRAM is that the page tables provides a way to *page in and out to disk*, while themselves living in RAM . That is, the amount of bytes the page table can refer to may be many times larger than the physical address space because each page table entry refers to a $p$ size chunk of bytes, allowing for an efficient indexing via this address mapping. The PPN specifies a **physical page frame**, ie a collection of contiguous bytes indexed by the VPO/PPO.

In this way, a page table can be thought of as a cache, whereby if the valid bit is set to $1$, it indicates that at the associated physical address mapping, there is data already loaded into memory ready to be accessed. If the valid bit is 0, the page is not currently in physical memory. A page fault occurs, and the operating system loads the page from disk into a physical frame before resuming execution. It is called a page hit when the data is already in memory.

Also note that this virtual memory caching operates one level of abstraction away from L1-L3 caches. That is, when the CPU is attempting to access some data, it will first check if it is present in registers, then L1-L3 cache, then DRAM. However, as DRAM is virtual from the perspective of the program, it will check if the valid bit is set for the virtual address in question, and if not will fetch from disk. It is loads from disk via this caching mechanism that incur the most significant performance degradation.

### TLB Cache

A **translation lookaside buffer (TLB)** is a set associative cache used to speed up virtual memory address mapping in hardware. When the CPU attempts a virtual address read, the TLB is consulted before the L1 cache lookup. This is because the L1 cache uses physical addresses. The TLB stores page table entries (the address mapping, not the data within the physical page frame).

By caching these address mappings, it saves clock cycles. This is because the page table itself is stored in DRAM. This means the hardware must conduct a search of the page table in DRAM to determine the address mapping. Placing a cache in between the CPU and DRAM enables hot data within the page table to sit closer to the processor, speeding up retrieval time and saving on computations.

# Virtual Address Translation Example

Consider the case of $14$ bit VA, $12$ bit PA, and $64$ byte page size with a $4$-way set associative TLB cache with $16$ total elements, as well as a direct mapped L1 cache with 16 lines and 4-byte block size.

Say we have the virtual address

    00011110101001

The virtual address is broken up into VPN and VPO. The page size is 64 bytes = $1 << 6$. This means we need 6 bits to represent the VPO. This is because there are $1 << 6$ values within the memory block a page table maps to, and we must be able to offset our index to represent each one. The VPN, our index into the page table, is the remaining $14-6=8$ bits.

The VPO is the least significant $6$ bits. The VPN is the remaining prefix.

    00011110 101001
    VPN      VPO

Immediately we know the VPO is the same as the PPO.

    101001
    PPO

Since PA are $12$ bits, and the PPO takes up $6$ of those bits already, the PPN will be $6$ bits too. 


    ______ 101001
    PPN    PPO



The exact value of the PPN cannot itself be determined by just the VA since it will depend on the exact location of where that data is currently living on the system. 

To determine what that value is, the TLB is consulted first, before the page table. The reason is because the TLB is cache implemented in hardware, whereas the page table is a kernel data structure that lives in DRAM. Accessing DRAM is higher in the memory heirarcy.

First, we observe that the TLB is a $4$-way set associative cache with $16$ total elements. This means that there are $4$ elements per set. And for the total elements of the TLB to be $16$, there must be $4$ sets to achieve $16 = 4 * 4$ total elements.

The TLB takes as input the 8 bit VPN. Since we have $1 << 2$ sets, the least significant $2$ bits of the VPN refer to the TLB Index (TLBI). The remaining $6$ bit prefix is the TLB tag (TLBT).

    00011110 
    VPN     

    000111 10 
    TLBT   TLBI 

The TLBI is used to index the which set. If there is a valid tag within that set, then the value stored within the cache is the PPN of the physical address. Note that we already have the PPO. 

For the sake of example, lets say the TLB produced a cache hit, and retrieve the PPN

    010101
    PPN

In this case, we assemble the full $12$ bit PA

    010101 101001
    PPN    PPO

    010101101001
    PA


Now that we have the PA, we move on to access physical memory. Since we have an L1 cache, we will consult the cache first, then DRAM. Note that disk is only involved on a page fault (when the valid bit in the page table is 0), not on an L1 cache miss. 

The L1 cache is direct mapped with $16$ lines and a $4$-byte block size. This means we are dealing with a cache with only $1$ line per set. Since there are $16$ total lines, there are $16$ sets. This means we need $log(16)=4$ bits forming our set index. And since each line has a $4$ byte block size, we must also have $log(4)=2$ bits for our block offset. The remaining $6$ bits form the tag.


    010101101001
    PA

    010101 1010      01
    tag    set_index block_offset

The L1 cache will first index into the sets using the set_index. If the line in that set has a matching tag, it will index into the line with the block_offset and retrived the data associated with the original VA. Otherwise, the data at that line will be evicted and accessed from DRAM.