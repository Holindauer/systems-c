# Hierarchical Memory

### Why Does Hierarchical Memory Exist?

If you wish to write fast code, you must shift from thinking of software as a specification of abstractions, to one about physics. Underlying the fancy abstraction of modern operating systems is a physical machine that moves and modifies data. In the physical world, moving something takes energy. And the further it has to be moved, the more energy and or time is required. This is why caches exist.

One reason processors have gotten so fast in recent decades is because they continue to get smaller. When the wires of an integrated circuit are closer together, there is less energy required to move data, and less distance it must travel. When a piece of data is nearby, less work is required to move it nearby.

For this reason, computers are fundamentally limited by data movement more so than computation. It is very efficient to mutate data once in CPU registers. However, it could be hundreds of thousands of times slower if improper memory management is employed. This is because the registers are physically right next to the ALU, which performs the computation.

To address this issue, modern processors utilize **hierarchical memory**. Hierarchical memory is based on the principle of **locality**. There are two basic types of locality, **spatial locality** and **temporal locality**. Spatial locality is the idea that if one address is used in a process, it is highly likely that the nearby addresses will too. **Temporal locality** is the idea that if you use the data at a particular address once, you will likely need to access that data again soon.

Modern processors tend to employ the following general memory hierarchy where the following types of storage/memory are ranked in terms of their access speed.

- 1: CPU registers
- 2: L1 cache
- 3: L2 cache
- 4: L3 cache
- 5: DRAM
- 6: Disk

Registers, cache, and DRAM are volatile forms of memory. They only store data when the computer has electricity running through it. Disk (which now often refers to SSD/NVMe storage, also integrated circuits) is long term storage, and works regardless of if the computer is powered on.

CPU registers are the fastest form of memory on a computer. They are right where the action is. L1, L2, and L3 cache sit between CPU registers and DRAM. Their purpose is to save important data within the processor

Pulling data from cache is much faster because DRAM is a separate integrated circuit. Data must leave the CPU and enter dram to be stored there. This extra distance takes extra time, and is therefore slower. 

The difference between L1, L2, and L3 cache lies in their location within the CPU. Each core in the CPU has its own L1 cache. L1 is also the smallest cache on the system. L2 is larger than L1, and is typically private to each CPU core, positioned nearby the core. L2 is slower than L1, but still faster than DRAM. L3 is similar in spirit to L2, but shared across all cores on the processor, not just a few that are nearby. L3 is the largest cache on the system, faster than DRAM, but slower than L1 and L2.

### How Does L1 Cache Work?

There are a few types of L1 cache, but all have the principle of **hit** and **miss**. When the processor is turned on for the first time, it stores no valid data. When an address is attempted to be read, the processor first consults the cache before DRAM. In this case, the cache will produce a miss, resulting in that data to be located in DRAM and written to the L1 cache for future use. If the data is already in cache, it is a hit, and the data moves from the cache to the CPU registers without consulting DRAM.

L1 cache is organized into $S$ **sets**, that each have $E$ **lines**. Each line stores one block of $B$ bytes of data. 

$$
S = 2^s
$$

$$
E = 2^e
$$

$$
B = 2^b
$$

The total size of the cache $C$ is 

$$
C = S * E * B
$$

A cache is called **direct mapped** if it has one line per set. A cache is called an **E-way set associative cache** if each set has multiple lines.

When the processor attempts to read an address from cache, it first performs an address translation procedure that will split the address being read into components, using them to index the cache. The address is broken into **tag**, **set index**, **and block offset**. There are $e$ bits required for the set index, $b$ bits for the block offset, and the remaining bits form the tag.

For example, for a $4$ way set associative cache for $32$ bit address and a block size of $16$, we need $2$ bits for the set index, $4$ bits for the block offset, and $32 - 4 - 2 = 26$ bits for the tag.

    address

    0101_1010_1110_1000_0001_1111_0101_0100

    translation

    tag                               set_index  block_offset
    0101_1010_1110_1000_0001_1111_01  01         0100
    
This translation identifies we are dealing with set index 1 (the second set, since indexing starts from 0). The cache will check if there is a line within that set with the matching tag. If there is, it will also check if its **valid bit** is set. If so, it will retrieve the data at byte offset 4. 

If the valid bit, or a matching tag with valid bit set is not present in the cache, it will evict one of the entries, replace it with the associated memory from DRAM, and set the valid bit.