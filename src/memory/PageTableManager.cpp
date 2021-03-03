#include "Memory.hpp"
#include "PageFrameAllocator.hpp"
#include "PageTableManager.hpp"

PageTableManager::PageTableManager(PageTable *p4Addr) : m_p4Addr(p4Addr) {}

void PageTableManager::mapMemory(void *virtualMemory, void *physicalMemory) {
    PageMapIndexer indexer((uint64_t)virtualMemory);

    PageDirectoryEntry pDE = m_p4Addr->entries[indexer.pDPIndex];
    PageTable *pDP;
    if (!pDE.getFlag(Present)) {
        pDP = (PageTable *)g_PageFrameAllocator.requestPage();
        memset(pDP, 0, 0x1000);
        pDE.setAddr((uint64_t)pDP >> 12);
        pDE.setFlag(Present, true);
        pDE.setFlag(ReadWrite, true);
        m_p4Addr->entries[indexer.pDPIndex] = pDE;
    } else {
        pDP = (PageTable *)((uint64_t)pDE.getAddr() << 12);
    }

    pDE = pDP->entries[indexer.pDIndex];
    PageTable *pD;
    if (!pDE.getFlag(Present)) {
        pD = (PageTable *)g_PageFrameAllocator.requestPage();
        memset(pD, 0, 0x1000);
        pDE.setAddr((uint64_t)pD >> 12);
        pDE.setFlag(Present, true);
        pDE.setFlag(ReadWrite, true);
        pDP->entries[indexer.pDIndex] = pDE;
    } else {
        pD = (PageTable *)((uint64_t)pDE.getAddr() << 12);
    }

    pDE = pD->entries[indexer.pTIndex];
    PageTable *pT;
    if (!pDE.getFlag(Present)) {
        pT = (PageTable *)g_PageFrameAllocator.requestPage();
        memset(pT, 0, 0x1000);
        pDE.setAddr((uint64_t)pT >> 12);
        pDE.setFlag(Present, true);
        pDE.setFlag(ReadWrite, true);
        pD->entries[indexer.pTIndex] = pDE;
    } else {
        pT = (PageTable *)((uint64_t)pDE.getAddr() << 12);
    }

    pDE = pT->entries[indexer.pIndex];
    pDE.setAddr((uint64_t)physicalMemory >> 12);
    pDE.setFlag(Present, true);
    pDE.setFlag(ReadWrite, true);
    pT->entries[indexer.pIndex] = pDE;
}
