#include "Paging.hpp"

PageMapIndexer::PageMapIndexer(uint64_t virtualAddr) {
    virtualAddr >>= 12;
    pIndex = virtualAddr & 0x1ff;
    virtualAddr >>= 9;
    pTIndex = virtualAddr & 0x1ff;
    virtualAddr >>= 9;
    pDIndex = virtualAddr & 0x1ff;
    virtualAddr >>= 9;
    pDPIndex = virtualAddr & 0x1ff;
}
