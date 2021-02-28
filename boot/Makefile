OVMF_DIR=/usr/share/OVMF/x64

LIB=/usr/lib64

GNU_EFI_INCLUDE=/usr/include/efi
GNU_EFI_INCLUDES=-I$(GNU_EFI_INCLUDE) -I$(GNU_EFI_INCLUDE)/x86_64 -I$(GNU_EFI_INCLUDE)/protocol
GNU_EFI_CRT_OBJ=$(LIB)/crt0-efi-x86_64.o
GNU_EFI_LINKER=$(LIB)/elf_x86_64_efi.lds
GNU_EFI_LIBS=-lgnuefi -lefi

CXX_FLAGS=-c -ffreestanding -Wall \
		  -fpic -fno-stack-protector -fshort-wchar -mno-red-zone \
		  -DEFI_FUNCTION_WRAPPER
LD_FLAGS=-nostdlib -znocombreloc -T $(GNU_EFI_LINKER) -shared -Bsymbolic \
		 -L $(LIB) $(GNU_EFI_CRT_OBJ)

boot_efi=$(BUILD_DIR)/main.efi
boot_so=$(BUILD_DIR)/main.so

boot_source_files := $(wildcard *.c)
boot_object_files := $(patsubst %.c, $(BUILD_DIR)/boot/%.o, $(boot_source_files))

$(boot_object_files): $(BUILD_DIR)/boot/%.o : %.c
	mkdir -p $(dir $@) && \
	$(CC) $(CXX_FLAGS) $(GNU_EFI_INCLUDES) -I/usr/include \
	$(patsubst $(BUILD_DIR)/boot/%.o, %.c, $@) -o $@

$(boot_so): $(boot_object_files)
	$(LD) $(LD_FLAGS) $(boot_object_files) -o $@ $(GNU_EFI_LIBS)

$(boot_efi): $(boot_so)
	objcopy -j .text -j .sdata -j .data -j .dynamic -j .dynsym -j .rel -j .rela \
		-j .rel.* -j .rela.* -j .reloc --target=efi-app-x86_64 $^ $@

.PHONY: img
img: $(boot_efi)
	cp -r $(OVMF_DIR) $(BUILD_DIR)/ovmf && \
	dd if=/dev/zero of=$(BUILD_DIR)/$(OSNAME).img bs=512 count=46875
	mformat -i $(BUILD_DIR)/$(OSNAME).img -f 2880 ::
	mmd -i $(BUILD_DIR)/$(OSNAME).img ::/EFI
	mmd -i $(BUILD_DIR)/$(OSNAME).img ::/EFI/BOOT
	mcopy -i $(BUILD_DIR)/$(OSNAME).img ./startup.nsh ::
	mcopy -i $(BUILD_DIR)/$(OSNAME).img $(boot_efi) ::/EFI/BOOT
	mcopy -i $(BUILD_DIR)/$(OSNAME).img $(kernel_elf) ::

.PHONY: clean
clean:
	rm $(boot_object_files) $(BUILD_DIR)/$(OSNAME).img $(boot_efi) $(boot_so)

