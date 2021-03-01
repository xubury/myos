export OSNAME=myos

export ROOT_DIR=$(shell pwd)
export BUILD_DIR=$(ROOT_DIR)/build

export kernel_elf=$(BUILD_DIR)/kernel.elf
export font_file=$(ROOT_DIR)/zap-light16.psf
export CC=gcc
export LD=ld


CXX_FLAGS=-c -ffreestanding -fshort-wchar -Wall -Wextra -Wundef -pedantic -std=c++17
CXX_INCLUDES=-Isrc/common -Isrc/string
LD_FLAGS=-T kernel.ld  -static -Bsymbolic -nostdlib

kernel_source_files := $(shell find src -name *.cpp)
kernel_header_files := $(shell find src -name *.hpp)
kernel_object_files := $(patsubst %.cpp, $(BUILD_DIR)/%.o, $(kernel_source_files))

$(kernel_object_files):  $(BUILD_DIR)/%.o : %.cpp $(kernel_header_files)
	mkdir -p $(dir $@) && \
	$(CC) $(CXX_FLAGS) $(GNU_EFI_INCLUDES) $(CXX_INCLUDES) \
	$(patsubst $(BUILD_DIR)/%.o, %.cpp, $@) -o $@

link:$(kernel_object_files)
	$(LD) $(LD_FLAGS) -o $(kernel_elf) $(kernel_object_files)

.PHONY: all
all:link
	$(MAKE) -C boot img

.PHONY: clean
clean:
	rm $(kernel_elf) $(kernel_object_files)
	make -C boot clean

.PHONY: run
run:
	make all && \
	qemu-system-x86_64 -drive file=$(BUILD_DIR)/$(OSNAME).img -m 256M -cpu qemu64 \
	-drive if=pflash,format=raw,unit=0,file="build/ovmf/OVMF_CODE.fd",readonly=on \
	-drive if=pflash,format=raw,unit=1,file="build/ovmf/OVMF_VARS.fd" \
	-net none

.PHONY: bear
bear:
	mkdir -p $(BUILD_DIR) && \
	bear --output build/compile_commands.json -- make all
