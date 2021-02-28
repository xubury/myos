#include <efi.h>
#include <efilib.h>
#include <elf.h>

EFI_FILE *loadFile(EFI_FILE *directory, const CHAR16 *path,
                   EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE *systemTable) {
    EFI_FILE *loadedFile;
    EFI_LOADED_IMAGE_PROTOCOL *loadedImage;
    uefi_call_wrapper((void *)systemTable->BootServices->HandleProtocol, 3,
                      imageHandle, &gEfiLoadedImageProtocolGuid,
                      (void **)&loadedImage);

    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *fileSystem;
    uefi_call_wrapper((void *)systemTable->BootServices->HandleProtocol, 3,
                      loadedImage->DeviceHandle,
                      &gEfiSimpleFileSystemProtocolGuid, (void **)&fileSystem);
    if (directory == NULL) {
        uefi_call_wrapper((void *)fileSystem->OpenVolume, 2, fileSystem,
                          &directory);
    }

    EFI_STATUS s =
        uefi_call_wrapper((void *)directory->Open, 5, directory, &loadedFile,
                          path, EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY);
    if (s != EFI_SUCCESS) {
        return NULL;
    }
    return loadedFile;
}

typedef uint64_t size_t;

int memcmp(const void *aptr, const void *hptr, size_t n) {
    const unsigned char *a = aptr, *b = hptr;
    for (size_t i = 0; i < n; ++i) {
        if (a[i] < b[i])
            return -1;
        else if (a[i] > b[i])
            return 1;
    }
    return 0;
}

uint64_t fileSize(EFI_FILE *FileHandle) {
    uint64_t ret;
    EFI_FILE_INFO *FileInfo; /* file information structure */
    /* get the file's size */
    FileInfo = LibFileInfo(FileHandle);
    ret = FileInfo->FileSize;
    FreePool(FileInfo);
    return ret;
}

#define SUCCESS 0
#define FAILURE 1

void readKernelHeader(EFI_FILE *kernel, Elf64_Ehdr *header) {
    uint64_t fileInfoSize = 0;
    uefi_call_wrapper((void *)kernel->GetInfo, 4, kernel, &gEfiFileInfoGuid,
                      &fileInfoSize, NULL);
    uint8_t *fileInfo = AllocatePool(fileInfoSize);

    uint64_t headerSize = sizeof(Elf64_Ehdr);
    uefi_call_wrapper((void *)kernel->Read, 3, kernel, &headerSize, header);
    FreePool(fileInfo);
}

uint8_t verifyKernelHeader(Elf64_Ehdr *header) {
    if (memcmp(&header->e_ident[EI_MAG0], ELFMAG, SELFMAG) != 0 ||
        header->e_ident[EI_CLASS] != ELFCLASS64 ||
        header->e_ident[EI_DATA] != ELFDATA2LSB || header->e_type != ET_EXEC ||
        header->e_machine != EM_X86_64 || header->e_version != EV_CURRENT) {
        return FAILURE;
    } else {
        return SUCCESS;
    }
}

EFI_STATUS
EFIAPI
efi_main(EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE *systemTable) {
    InitializeLib(imageHandle, systemTable);

    const CHAR16 *filename = L"kernel.elf";
    EFI_FILE *kernel = loadFile(NULL, filename, imageHandle, systemTable);
    if (kernel != NULL) {
        Print(L"Kernel file: %s open successfullly!\n\r", filename);
    } else {
        Print(L"Could not open kernel file: %s!\n\r", filename);
        return EFI_SUCCESS;
    }

    // read and verify header.
    Elf64_Ehdr header;
    readKernelHeader(kernel, &header);
    if (verifyKernelHeader(&header) == SUCCESS) {
        Print(L"Kernel header successfullly verified!\n\r");
    } else {
        Print(L"Kernel format is bad!\n\r");
    }

    // read the actual kernel progame into memory.
    Elf64_Phdr *phdrs;
    uefi_call_wrapper((void *)kernel->SetPosition, 2, kernel, header.e_phoff);
    uint64_t programSize = header.e_phnum * header.e_phentsize;
    phdrs = AllocatePool(programSize);
    uefi_call_wrapper((void *)kernel->Read, 3, kernel, &programSize, phdrs);

    for (Elf64_Phdr *phdr = phdrs; (char *)phdr < (char *)phdrs + programSize;
         phdr = (Elf64_Phdr *)((char *)phdr + header.e_phentsize)) {
        switch (phdr->p_type) {
            case PT_LOAD: {
                int pages = (phdr->p_memsz + 0x1000 - 1) / 0x1000;
                Elf64_Addr segment = phdr->p_paddr;
                uefi_call_wrapper(
                    (void *)systemTable->BootServices->AllocatePages, 4,
                    AllocateAddress, EfiLoaderData, pages, &segment);

                uefi_call_wrapper((void *)kernel->SetPosition, 2, kernel,
                                  phdr->p_offset);
                uint64_t size = phdr->p_filesz;
                uefi_call_wrapper((void *)kernel->Read, 3, kernel, &size,
                                  (void *)segment);
                break;
            }
        }
    }
    Print(L"Kernel loaded.\n\r");

    // calling kenrel entry
    int (*kernelStart)() =
        ((__attribute__((sysv_abi)) int (*)())header.e_entry);
    Print(L"%d\n\r", kernelStart());
    return EFI_SUCCESS;
}
