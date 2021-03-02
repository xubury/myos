#ifndef PAGE_TABLE_MANAGER_HPP
#define PAGE_TABLE_MANAGER_HPP

#include "Paging.hpp"

class PageTableManager {
   public:
    PageTableManager(PageTable *p4Addr);
    PageTable *m_p4Addr;
    void mapMemory(void *virtualMemory, void *physicalMemory);
};

#endif
