#include "File.h"

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
