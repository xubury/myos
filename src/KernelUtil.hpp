#ifndef KERNEL_UTIL_HPP
#define KERNEL_UTIL_HPP

#include "BasicRenderer.hpp"
#include "Bitmap.hpp"
#include "BootInfo.h"
#include "PageFrameAllocator.hpp"
#include "PageTableManager.hpp"
#include "Paging.hpp"
#include "String.hpp"

extern uint64_t _kernelStart;
extern uint64_t _kernelEnd;

class KernelManager {
   public:
    KernelManager(BootInfo* info);
    void initKernel();

    BasicRenderer& renderer() { return m_renderer; }

    void printMemory();
    void testString();
    void testBitmap();
    void testPageIndexer();

   private:
    void prepareMemory();

    PageTableManager m_pageTableManager;
    BootInfo* m_bootInfo;

    BasicRenderer m_renderer;
};

#endif
