#include "File.h"
#include "FrameBuffer.h"
#include "PSF1Font.h"

#define SUCCESS 0
#define FAILURE 1

PSF1Font *loadPSF1Font(EFI_FILE *directory, const CHAR16 *path,
                       EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE *systemTable) {
    EFI_FILE *font = loadFile(directory, path, imageHandle, systemTable);
    if (font == NULL) return NULL;

    uint64_t headerSize = sizeof(PSF1Header);
    PSF1Header *fontHeader = AllocatePool(headerSize);
    uefi_call_wrapper(font->Read, 3, font, &headerSize, fontHeader);

    if (fontHeader->magic[0] != PSF1_MAGIC0 ||
        fontHeader->magic[1] != PSF1_MAGIC1) {
        return NULL;
    }

    uint64_t glyphBufferSize;
    if (fontHeader->mode == 1) {
        // 512 glyphmode
        glyphBufferSize = fontHeader->characterSize * 512;
    } else {
        glyphBufferSize = fontHeader->characterSize * 256;
    }

    void *glyphBuffer = AllocatePool(glyphBufferSize);
    uefi_call_wrapper(font->SetPosition, 2, font, headerSize);
    uefi_call_wrapper(font->Read, 3, font, &glyphBufferSize, glyphBuffer);

    PSF1Font *finishedFont = AllocatePool(sizeof(PSF1Font));
    finishedFont->header = fontHeader;
    finishedFont->glyphBuffer = glyphBuffer;
    return finishedFont;
}

FrameBuffer frameBuffer;

FrameBuffer *initGOP() {
    EFI_GUID gopGUID = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
    EFI_GRAPHICS_OUTPUT_PROTOCOL *gop;
    EFI_STATUS status;

    status = uefi_call_wrapper((void *)BS->LocateProtocol, 3, &gopGUID, NULL,
                               (void **)&gop);

    if (EFI_ERROR(status)) {
        Print(L"Unable to locate GOP.\n\r");
        return NULL;
    } else {
        Print(L"GOP located.\n\r");
    }

    uefi_call_wrapper(gop->SetMode, 2, gop, 2);
    frameBuffer.baseAddr = gop->Mode->FrameBufferBase;
    frameBuffer.bufferSize = gop->Mode->FrameBufferSize;
    frameBuffer.width = gop->Mode->Info->HorizontalResolution;
    frameBuffer.height = gop->Mode->Info->VerticalResolution;
    frameBuffer.pixelsPerScanline = gop->Mode->Info->PixelsPerScanLine;

    return &frameBuffer;
}

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

Elf64_Addr loadKernel(EFI_FILE *kernel, EFI_SYSTEM_TABLE *systemTable) {
    // read and verify header.
    Elf64_Ehdr header;
    uint64_t fileInfoSize = 0;
    uefi_call_wrapper((void *)kernel->GetInfo, 4, kernel, &gEfiFileInfoGuid,
                      &fileInfoSize, NULL);

    uint64_t headerSize = sizeof(Elf64_Ehdr);
    uefi_call_wrapper((void *)kernel->Read, 3, kernel, &headerSize, &header);
    if (verifyKernelHeader(&header) == SUCCESS) {
        Print(L"Kernel header successfullly verified!\n\r");
    } else {
        Print(L"Kernel format is bad!\n\r");
        return header.e_entry;
    }

    // read the actual kernel progame into memory.
    uefi_call_wrapper((void *)kernel->SetPosition, 2, kernel, header.e_phoff);
    uint64_t programSize = header.e_phnum * header.e_phentsize;
    Elf64_Phdr *phdrs = AllocatePool(programSize);
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
    return header.e_entry;
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

    Elf64_Addr kernelEntry = loadKernel(kernel, systemTable);
    Print(L"Kernel loaded.\n\r");

    // calling kenrel entry
    int (*kernelStart)() = ((__attribute__((sysv_abi)) int (*)(
        FrameBuffer *, PSF1Font *))kernelEntry);

    FrameBuffer *frame = initGOP();
    Print(
        L"GOP: base: 0x%x size:0x%x width:%d height:%d "
        L"pixelsPerScanline:%d\n\r",
        frame->baseAddr, frame->bufferSize, frame->width, frame->height,
        frame->pixelsPerScanline);

    PSF1Font *newFont =
        loadPSF1Font(NULL, L"zap-light16.psf", imageHandle, systemTable);
    if (newFont != NULL) {
        Print(L"Font loaded, char size = %d.\n\r",
              newFont->header->characterSize);
    } else {
        Print(L"Font not valid\n\r");
    }

    Print(L"0x%x\n\r", kernelStart(frame, newFont));

    return EFI_SUCCESS;
}
