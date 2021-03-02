#include "Memory.hpp"
#include "PageFrameAllocator.hpp"
#include "PageTableManager.hpp"

PageTableManager::PageTableManager(PageTable *p4Addr) : m_p4Addr(p4Addr) {}

void PageTableManager::mapMemory(void *virtualMemory, void *physicalMemory) {
    PageMapIndexer indexer((uint64_t)virtualMemory);

    PageDirectoryEntry pDE = m_p4Addr->entries[indexer.pDPIndex];
    PageTable *pDP;
    if (!pDE.present) {
        pDP = (PageTable *)g_PageFrameAllocator.requestPage();
        memset(pDP, 0, 0x1000);
        pDE.addr = (uint64_t)pDP >> 12;
        pDE.present = true;
        pDE.readWrite = true;
        m_p4Addr->entries[indexer.pDPIndex] = pDE;
    } else {
        pDP = (PageTable *)((uint64_t)pDE.addr << 12);
    }

    pDE = pDP->entries[indexer.pDIndex];
    PageTable *pD;
    if (!pDE.present) {
        pD = (PageTable *)g_PageFrameAllocator.requestPage();
        memset(pD, 0, 0x1000);
        pDE.addr = (uint64_t)pD >> 12;
        pDE.present = true;
        pDE.readWrite = true;
        pDP->entries[indexer.pDIndex] = pDE;
    } else {
        pD = (PageTable *)((uint64_t)pDE.addr << 12);
    }

    pDE = pD->entries[indexer.pTIndex];
    PageTable *pT;
    if (!pDE.present) {
        pT = (PageTable *)g_PageFrameAllocator.requestPage();
        memset(pT, 0, 0x1000);
        pDE.addr = (uint64_t)pT >> 12;
        pDE.present = true;
        pDE.readWrite = true;
        pD->entries[indexer.pTIndex] = pDE;
    } else {
        pT = (PageTable *)((uint64_t)pDE.addr << 12);
    }

    pDE = pT->entries[indexer.pIndex];
    pDE.addr = (uint64_t)physicalMemory >> 12;
    pDE.present = true;
    pDE.readWrite = true;
    pT->entries[indexer.pIndex] = pDE;
}
