#include "Interrupt.hpp"
#include "KernelUtil.hpp"

KernelManager g_manager;

KernelManager::KernelManager() : m_pageTableManager(nullptr) {}

void KernelManager::prepareMemory() {
    g_PageFrameAllocator.readEFIMemoryMap(m_bootInfo->map, m_bootInfo->mapSize,
                                          m_bootInfo->mapDescriptorSize);
    uint64_t kernelSize = (uint64_t)&_kernelEnd - (uint64_t)&_kernelStart;
    uint64_t kernelPages = (uint64_t)kernelSize / 4096 + 1;
    g_PageFrameAllocator.lockPages(&_kernelStart, kernelPages);

    PageTable *p4Table = (PageTable *)g_PageFrameAllocator.requestPage();
    memset(p4Table, 0, 0x1000);
    m_pageTableManager.m_p4Addr = p4Table;

    size_t memorySize = getMemorySize(m_bootInfo->map, m_bootInfo->mapSize,
                                      m_bootInfo->mapDescriptorSize);
    for (uint64_t i = 0; i < memorySize; i += 0x1000) {
        m_pageTableManager.mapMemory((void *)i, (void *)i);
    }

    uint64_t fbBase = (uint64_t)m_bootInfo->frameBuffer->baseAddr;
    uint64_t fbSize = (uint64_t)m_bootInfo->frameBuffer->bufferSize + 0x1000;
    for (uint64_t i = fbBase; i < fbBase + fbSize; i += 0x1000) {
        m_pageTableManager.mapMemory((void *)i, (void *)i);
    }
    asm("mov %0, %%cr3" : : "r"(p4Table));
}

void KernelManager::prepareInterrupts() {
    m_idtr.limit = 0x0FFF;
    m_idtr.offset = (uint64_t)g_PageFrameAllocator.requestPage();

    IDTDescEntry *intPageFault =
        (IDTDescEntry *)(m_idtr.offset + 0xE * sizeof(IDTDescEntry));
    intPageFault->setOffset((uint64_t)pageFaultHandler);
    intPageFault->typeAttribute = IDT_TA_InterruptGate;
    intPageFault->selector = 0x08;

    asm("lidt %0" : : "m"(m_idtr));
}

void KernelManager::initKernel(BootInfo *info) {
    m_bootInfo = info;
    prepareMemory();
    GDTDescriptor gdtDescriptor;
    gdtDescriptor.size = sizeof(GDT) - 1;
    gdtDescriptor.offset = (uint64_t)&defaultGDT;
    loadGDT(&gdtDescriptor);
    m_renderer.init(m_bootInfo->frameBuffer, m_bootInfo->psf1Font, 0, 0,
                    RGBA(255, 255, 255), RGBA());
    m_renderer.clearScreen();
    m_renderer.print("Kernel initialized successfully!\n");
    prepareInterrupts();
}

void KernelManager::printMemory() {
    m_renderer.print("\nMemory:\n");
    m_renderer.setColor(RGBA(255, 0, 0), RGBA());
    m_renderer.print("Free:");
    m_renderer.print(
        toString((float)g_PageFrameAllocator.getFreeRAM() / (1024 * 1024)));
    m_renderer.print(" MB\n");
    m_renderer.print("Used:");
    m_renderer.print(
        toString((float)g_PageFrameAllocator.getUsedRAM() / (1024 * 1024)));
    m_renderer.print(" MB\n");
    m_renderer.print("Reserved:");
    m_renderer.print(
        toString((float)g_PageFrameAllocator.getReservedRAM() / (1024 * 1024)));
    m_renderer.print(" MB\n");
    m_renderer.setColor(RGBA(255, 255, 255), RGBA());
}

void KernelManager::testString() {
    m_renderer.print("\nTesting string conversion:\n");
    m_renderer.print("Expecting: 123.23 123 -1 0 0x000000FF 0x3F99999A\n");
    m_renderer.print("Getting: ");
    m_renderer.print(toString(123.232));
    m_renderer.print(" ");
    m_renderer.print(toString(123));
    m_renderer.print(" ");
    m_renderer.print(toString(-1));
    m_renderer.print(" ");
    m_renderer.print(toString(0));
    m_renderer.print(" ");
    m_renderer.print(toHexString(255));  // 0xFF
    m_renderer.print(" ");
    m_renderer.print(toHexString<float>(1.2));  // 0x3F99999A
    m_renderer.newLine();
}

void KernelManager::testBitmap() {
    m_renderer.print("\nTesting bitmap:\n");
    m_renderer.print("Expecting: 0x48 0x00\n");
    m_renderer.print("Getting: ");
    uint8_t buffer[2] = {0};
    Bitmap bitmap;
    bitmap.buffer = buffer;
    bitmap.set(0, false);
    bitmap.set(1, true);
    bitmap.set(2, false);
    bitmap.set(3, false);
    bitmap.set(4, true);
    for (size_t i = 0; i < sizeof(buffer); ++i) {
        m_renderer.print(toHexString(buffer[i]));
        m_renderer.print(" ");
    }
    m_renderer.newLine();
}

void KernelManager::testPageIndexer() {
    m_renderer.print("\nTesting page indexer:\n");
    m_renderer.print("Expecting: 0-0-0-1\n");
    m_renderer.print("Getting: ");
    PageMapIndexer pageIndexer = PageMapIndexer(0x1000);
    m_renderer.print(toString(pageIndexer.pDPIndex));
    m_renderer.print("-");
    m_renderer.print(toString(pageIndexer.pDIndex));
    m_renderer.print("-");
    m_renderer.print(toString(pageIndexer.pTIndex));
    m_renderer.print("-");
    m_renderer.print(toString(pageIndexer.pIndex));
    m_renderer.newLine();
}
