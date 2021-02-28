#ifndef UEFI_FILE_H
#define UEFI_FILE_H

#include <efi.h>
#include <efilib.h>
#include <elf.h>
#include <stddef.h>
#include <stdint.h>

EFI_FILE *loadFile(EFI_FILE *directory, const CHAR16 *path,
                   EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE *systemTable);
#endif
