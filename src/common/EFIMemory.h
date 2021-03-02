#ifndef EFI_MEMORY_H
#define EFI_MEMORY_H

#include <stddef.h>
#include <stdint.h>

typedef enum {
    EFIReservedMemoryType,
    EFILoaderCode,
    EFILoaderData,
    EFIBootServicesCode,
    EFIBootServicesData,
    EFIRuntimeServicesCode,
    EFIRuntimeServicesData,
    EFIConventionalMemory,
    EFIUnusableMemory,
    EFIACPIReclaimMemory,
    EFIACPIMemoryNVS,
    EFIMemoryMappedIO,
    EFIMemoryMappedIOPortSpace,
    EFIPalCode,
    EFIMaxMemoryType
} EFIMemoryType;

typedef struct {
    uint32_t type;  // Field size is 32 bits followed by 32 bit pad
    uint32_t pad;
    uint64_t physicalAddr;   // Field size is 64 bits
    uint64_t virtualAddr;    // Field size is 64 bits
    uint64_t numberOfPages;  // Field size is 64 bits
    uint64_t attribute;      // Field size is 64 bits
} EFIMemoryDescriptor;

extern const char* EFI_MEMORY_TYPE_STRINGS[];

#endif
