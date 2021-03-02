#ifndef PAGING_HPP
#define PAGING_HPP

#include <stdint.h>

struct PageDirectoryEntry {
    bool present : 1;
    bool readWrite : 1;
    bool userSuper : 1;
    bool writeThrough : 1;
    bool cacheDisabled : 1;
    bool accessed : 1;
    bool ignore0 : 1;
    bool largerPages : 1;
    bool ignore1 : 1;
    uint8_t available : 3;
    uint64_t addr : 52;
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
