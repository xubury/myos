#include "Paging.hpp"

void PageDirectoryEntry::setFlag(PTFlag flag, bool enabled) {
    uint64_t bitSelector = (uint64_t)1 << flag;
    m_value &= ~bitSelector;
    if (enabled) {
        m_value |= bitSelector;
    }
}

bool PageDirectoryEntry::getFlag(PTFlag flag) {
    uint64_t bitSelector = (uint64_t)1 << flag;
    return (m_value & bitSelector) > 0;
}

void PageDirectoryEntry::setAddr(uint64_t addr) {
    addr &= 0x000000ffffffffff;
    m_value &= 0xfff0000000000fff;
    m_value |= (addr << 12);
}

uint64_t PageDirectoryEntry::getAddr() {
    return (m_value & 0x000ffffffffff000) >> 12;
}

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
