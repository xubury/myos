export OSNAME=myos

export ROOT_DIR=$(shell pwd)
export BUILD_DIR=$(ROOT_DIR)/build

export LIB_DIR=/usr/lib

export kernel_elf=$(BUILD_DIR)/kernel.elf
export font_file=$(ROOT_DIR)/zap-light16.psf
export CC=gcc
export LD=ld
export ASMC=nasm

CXX_FLAGS=-c -ffreestanding -fshort-wchar -Wall -Wextra -Wundef -pedantic -std=c++17
CXX_INCLUDES=-Isrc -Isrc/common -Isrc/math -Isrc/string -Isrc/memory \
			 -Isrc/gdt -Isrc/interrupt
LD_FLAGS=-T kernel.ld  -static -Bsymbolic -nostdlib

ASM_FLAGS=-f elf64

kernel_source_files := $(shell find src -name *.cpp)
kernel_header_files := $(shell find src -name *.hpp -or -name *.h)
kernel_object_files := $(patsubst %.cpp, $(BUILD_DIR)/%.o, $(kernel_source_files))

kernel_asm_files := $(shell find src -name *.asm)
kernel_object_files += $(patsubst %.asm, $(BUILD_DIR)/%_asm.o, $(kernel_asm_files))

$(BUILD_DIR)/src/interrupt/Interrupt.o: src/interrupt/Interrupt.cpp
	mkdir -p $(dir $@) && \
	$(CC) -mno-red-zone -mgeneral-regs-only \
	$(CXX_FLAGS) $(CXX_INCLUDES) $(patsubst $(BUILD_DIR)/%.o, %.cpp, $@) -o $@

$(BUILD_DIR)/%.o : %.cpp $(kernel_header_files)
	mkdir -p $(dir $@) && \
	$(CC) $(CXX_FLAGS) $(CXX_INCLUDES) $(patsubst $(BUILD_DIR)/%.o, %.cpp, $@) -o $@

$(BUILD_DIR)/%_asm.o : %.asm
	mkdir -p $(dir $@) && \
	$(ASMC) $(ASM_FLAGS) $^ -o $@

link: $(kernel_object_files)
	$(LD) $(LD_FLAGS) -o $(kernel_elf) $(kernel_object_files)

.PHONY: all
all: link
	make -C boot img

.PHONY: clean
clean:
	rm -f $(kernel_object_files) $(kernel_elf) && \
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
