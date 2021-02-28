OSNAME=myos
BUILD_DIR=build

OVMF_DIR=/usr/share/OVMF/x64

LIB=/usr/lib64

GNU_EFI_INCLUDE=/usr/include/efi
GNU_EFI_INCLUDES=-I$(GNU_EFI_INCLUDE) -I$(GNU_EFI_INCLUDE)/x86_64 -I$(GNU_EFI_INCLUDE)/protocol
GNU_EFI_CRT_OBJ=$(LIB)/crt0-efi-x86_64.o
GNU_EFI_LINKER=$(LIB)/elf_x86_64_efi.lds
GNU_EFI_LIBS=-lgnuefi -lefi

CC=x86_64-elf-gcc
LD=x86_64-elf-ld

CXX_FLAGS=-c -ffreestanding -Wall -Wextra -Wundef -pedantic \
		  -fpic -fno-stack-protector -fshort-wchar -mno-red-zone \
		  -DEFI_FUNCTION_WRAPPER -DEFI_DEBUG -DEFI_DEBUG_CLEAR_MEMORY
LD_FLAGS=-nostdlib -znocombreloc -T $(GNU_EFI_LINKER) -shared -Bsymbolic \
		 -L $(LIB) $(GNU_EFI_CRT_OBJ)

boot_efi=$(BUILD_DIR)/main.efi
boot_so=$(BUILD_DIR)/main.so

kernel_source_files := $(wildcard src/kernel/*.cpp)
kernel_object_files := $(patsubst src/kernel/%.cpp, $(BUILD_DIR)/kernel/%.o, $(kernel_source_files))

$(kernel_object_files): $(BUILD_DIR)/kernel/%.o : src/kernel/%.cpp
	mkdir -p $(dir $@) && \
	$(CC) $(CXX_FLAGS) $(GNU_EFI_INCLUDES) \
	$(patsubst $(BUILD_DIR)/kernel/%.o, src/kernel/%.cpp, $@) -o $@

$(BUILD_DIR)/%.so: $(kernel_object_files)
	$(LD) $(LD_FLAGS) $(kernel_object_files) -o $@ $(GNU_EFI_LIBS)

$(boot_efi): $(boot_so)
	objcopy -j .text -j .sdata -j .data -j .dynamic -j .dynsym -j .rel -j .rela \
		-j .reloc --target=efi-app-x86_64 $^ $@

.PHONY: all
all: $(boot_efi)
	dd if=/dev/zero of=$(BUILD_DIR)/$(OSNAME).img bs=512 count=93750
	mformat -i $(BUILD_DIR)/$(OSNAME).img -f 1440 ::
	mmd -i $(BUILD_DIR)/$(OSNAME).img ::/EFI
	mmd -i $(BUILD_DIR)/$(OSNAME).img ::/EFI/BOOT
	mcopy -i $(BUILD_DIR)/$(OSNAME).img ./startup.nsh ::
	mcopy -i $(BUILD_DIR)/$(OSNAME).img $^ ::/EFI/BOOT

.PHONY: clean
clean:$(boot_efi) $(boot_so) $(kernel_object_files)
	rm $(boot_efi) $(boot_so) $(kernel_object_files)

run:
	cp -r $(OVMF_DIR) build/ovmf
	qemu-system-x86_64 -drive file=$(BUILD_DIR)/$(OSNAME).img -m 256M -cpu qemu64 \
	-drive if=pflash,format=raw,unit=0,file="build/ovmf/OVMF_CODE.fd",readonly=on \
	-drive if=pflash,format=raw,unit=1,file="build/ovmf/OVMF_VARS.fd" \
	-net none

