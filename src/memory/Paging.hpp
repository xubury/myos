#ifndef PAGING_HPP
#define PAGING_HPP

#include <stdint.h>

enum PTFlag {
    Present = 0,
    ReadWrite = 1,
    UserSuper = 2,
    WriteThrough = 3,
    CacheDisabled = 4,
    Accessed = 5,
    LargerPages = 7,
    Custom0 = 9,
    Custom1 = 10,
    Custom2 = 11,
    NX = 63
};

class PageDirectoryEntry {
   public:
    void setFlag(PTFlag flag, bool enabled);
    bool getFlag(PTFlag flag);

    void setAddr(uint64_t addr);
    uint64_t getAddr();

   private:
    uint64_t m_value;
};

struct PageTable {
    PageDirectoryEntry entries[512];
} __attribute__((aligned(0x1000)));

class PageMapIndexer {
   public:
    PageMapIndexer(uint64_t virtualAddr);
    uint64_t pDPIndex;
    uint64_t pDIndex;
    uint64_t pTIndex;
    uint64_t pIndex;
};

#endif
