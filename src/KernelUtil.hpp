#ifndef KERNEL_UTIL_HPP
#define KERNEL_UTIL_HPP

#include "BasicRenderer.hpp"
#include "Bitmap.hpp"
#include "BootInfo.h"
#include "GDT.hpp"
#include "IDT.hpp"
#include "PageFrameAllocator.hpp"
#include "PageTableManager.hpp"
#include "Paging.hpp"
#include "String.hpp"

extern uint64_t _kernelStart;
extern uint64_t _kernelEnd;

class KernelManager {
   public:
    void initKernel(BootInfo* info);

    BasicRenderer& renderer();
    PageFrameAllocator& pageFrameAllocator();
    void printMemory();
    void testString();
    void testBitmap();
    void testPageIndexer();

   private:
    void prepareMemory();
    void prepareInterrupts();

    BootInfo* m_bootInfo;

    PageFrameAllocator m_pageFrameAllocator;
    PageTableManager m_pageTableManager;

    BasicRenderer m_renderer;
    IDTR m_idtr;
};

extern KernelManager& manager();

#endif
