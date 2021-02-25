global start

section .text
bits 32
start:
	mov esp, stack_top

	call check_multiboot
	call check_cpuid
	call check_long_mode

	call setup_page_tables
	call enable_paging
	;print 'OK'
	mov dword [0xb8000], 0x2f4b2f4f
	hlt

check_multiboot:
	cmp eax, 0x36d76289
	jne .no_multiboot
	ret
.no_multiboot:
	mov al, "M"
	jmp error

check_cpuid:
	;flip a bit in flag register
	pushfd
	pop eax
	mov ecx, eax
	;flip a bit
	xor eax, 1 << 21
	;push back to fd
	push eax
	popfd
	;push to eax to see if the flip success
	pushfd
	pop eax
	;restore original value in flag register
	push ecx
	popfd
	cmp eax, ecx
	;if can't flip, cpuid not available
	je .no_cpuid
	ret
.no_cpuid:
	mov al, "C"
	jmp error

check_long_mode:
	;check if extended information available
	mov eax, 0x80000000
	cpuid
	cmp eax, 0x80000001
	jb .no_long_mode

	;check long mode
	mov eax, 0x80000001
	cpuid
	test edx, 1 << 29
	jz .no_long_mode
	ret
.no_long_mode:
	mov al, "L"
	jmp error

setup_page_tables:
	mov eax, page_table_l3
	or eax, 0b11 ;present, writable
	mov [page_table_l4], eax

	mov eax, page_table_l2
	or eax, 0b11 ;present, writable
	mov [page_table_l3], eax

	mov ecx, 0
.loop:
	mov eax, 0x200000
	mul ecx
	or eax, 0b10000011 ;present, writable, huge page
	mov [page_table_l2 + ecx * 8], eax
	inc ecx ;increment counter
	cmp ecx, 512 ;check if whole table is mapped
	jne .loop

	ret

enable_paging:
	mov eax, page_table_l4
	mov cr3, eax

	;enable PAE
	mov eax, cr4
	or eax, 1 << 5
	mov cr4, eax

	;enable long mode
	mov ecx, 0xc0000080
	rdmsr
	or eax, 1 << 8
	wrmsr

	;enable paging
	mov eax, cr0
	or eax, 1 << 31
	mov cr0, eax

	ret

error:
	;print "ERR: X"
	mov dword [0xb8000], 0x4f524f45
	mov dword [0xb8004], 0x4f3a4f52
	mov dword [0xb8008], 0x4f204f20
	mov byte  [0xb800a], al
	hlt

section .bss
align 4096
page_table_l4:
	resb 4096
page_table_l3:
	resb 4096
page_table_l2:
	resb 4096
stack_bottom:
	resb 4096 * 4
stack_top: