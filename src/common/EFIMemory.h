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
    uint32_t Type;  // Field size is 32 bits followed by 32 bit pad
    uint32_t Pad;
    uint64_t PhysicalStart;  // Field size is 64 bits
    uint64_t VirtualStart;   // Field size is 64 bits
    uint64_t NumberOfPages;  // Field size is 64 bits
    uint64_t Attribute;      // Field size is 64 bits
} EFIMemoryDescriptor;

extern const char* EFI_MEMORY_TYPE_STRINGS[];

#endif
