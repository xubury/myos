OSNAME=myos
BUILD_DIR=build

OVMF_DIR=/usr/share/OVMF/x64

GNU_EFI_INCLUDE_DIR=-I/usr/include/efi -I/usr/include/efi/x86_64 -I/usr/include/efi/protocol
GNU_EFI_LIBS_DIR=-L/usr/lib/
GNU_EFI_CRT_OBJS=/usr/lib/crt0-efi-x86_64.o
GNU_EFI_LINKER=/usr/lib/elf_x86_64_efi.lds

CC=gcc
LD=ld

CXX_FLAGS=-c -ffreestanding -Wall -Wextra -Wundef -pedantic \
		  -fpic -fno-stack-protector -fno-stack-check -fshort-wchar -mno-red-zone
LD_FLAGS=-nostdlib -shared -Bsymbolic -T $(GNU_EFI_LINKER) -L/usr/lib -lgnuefi -lefi

boot_so=$(BUILD_DIR)/main.so
boot_efi=$(BUILD_DIR)/main.efi

kernel_source_files := $(shell find src/kernel -name *.c)
kernel_object_files := $(patsubst src/kernel/%.c, $(BUILD_DIR)/kernel/%.o, $(kernel_source_files))

$(kernel_object_files): $(BUILD_DIR)/kernel/%.o : src/kernel/%.c
	mkdir -p $(dir $@) && \
	$(CC) $(CXX_FLAGS) $(GNU_EFI_INCLUDE_DIR) \
	$(patsubst $(BUILD_DIR)/kernel/%.o, src/kernel/%.c, $@) -o $@

$(boot_so): $(kernel_object_files)
	$(LD) $(LD_FLAGS) $(GNU_EFI_CRT_OBJS) $(kernel_object_files) -o $@

$(boot_efi): $(boot_so)
	objcopy -j .text -j .sdata -j .data -j .dynamic -j .dynsym -j .rel -j .rela -j .rel.* -j .rela.* -j .reloc --target efi-app-x86_64 --subsystem=10 $^ $@

.PHONY: all
all: $(boot_efi)
	dd if=/dev/zero of=$(BUILD_DIR)/$(OSNAME).img bs=512 count=93750
	mformat -i $(BUILD_DIR)/$(OSNAME).img -f 1440 ::
	mmd -i $(BUILD_DIR)/$(OSNAME).img ::/EFI
	mmd -i $(BUILD_DIR)/$(OSNAME).img ::/EFI/BOOT
	mcopy -i $(BUILD_DIR)/$(OSNAME).img startup.nsh ::
	mcopy -i $(BUILD_DIR)/$(OSNAME).img $(boot_efi) ::/EFI/BOOT
	mcopy -i $(BUILD_DIR)/$(OSNAME).img $(boot_so) ::

.PHONY: clean
clean:$(boot_efi) $(boot_so) $(kernel_object_files)
	rm $(boot_efi) $(boot_so) $(kernel_object_files)

run:
	sudo qemu-system-x86_64 -drive file=$(BUILD_DIR)/$(OSNAME).img -m 256M -cpu qemu64 \
	-drive if=pflash,format=raw,unit=0,file="$(OVMF_DIR)/OVMF_CODE.fd",readonly=on \
	-drive if=pflash,format=raw,unit=1,file="$(OVMF_DIR)/OVMF_VARS.fd" \
	-net none

