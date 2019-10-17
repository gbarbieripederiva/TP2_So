
section .text

GLOBAL _sti
;enable interrupts
_sti:
	sti
	ret
;disable interrupts
GLOBAL _cli
_cli:
	cli
	ret
;apply mask to master pic
GLOBAL picMasterMask
picMasterMask:
	push rbp
    mov rbp, rsp
    mov ax, di
    out	21h,al
    pop rbp
    retn
;apply mask to slave pic
GLOBAL picSlaveMask
picSlaveMask:
	push    rbp
    mov     rbp, rsp
    mov     ax, di  ; ax = mascara de 16 bits
    out	0A1h,al
    pop     rbp
    retn

;Halt the program until we get an interrupt:
GLOBAL haltFunction
haltFunction:
	hlt
	ret


;push all registers
%macro pushState 0
	push rax
	push rbx
	push rcx
	push rdx
	push rbp
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
%endmacro

;pop all registers
%macro popState 0
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rbp
	pop rdx
	pop rcx
	pop rbx
	pop rax
%endmacro

;basic interrupt template
;most hardware interrupts follow this pattern
extern interruptDispatcher
%macro interrupt 1
	pushState
	cld
	mov rdi,%1
	call interruptDispatcher

	mov al, 20h
	out 20h, al

	popState
	iretq
%endmacro

;int 20h
GLOBAL interrupt1
extern scheduler
interrupt1:
	pushState
	cld
	mov rdi,1
	call interruptDispatcher
	mov al, 20h
	out 20h, al

	mov rdi, rsp
	call scheduler
	mov rsp, rax

	popState
	iretq

;int 21h
GLOBAL interrupt2
interrupt2:
	interrupt 2

;int 80h
;this one is special because it is used as a
;interface between kernel and user space
extern interruptAction80Dispatcher
GLOBAL interrupt80
interrupt80:
	sti
	call interruptAction80Dispatcher

	;save the rax value where the return value will be returned
	push rax
	mov al, 20h
	out 20h, al
	pop rax

	iretq
	




;basically checks that keyboard has input and then gets that input
;if it doesnt have then return 0xffff
GLOBAL interrupt_action_2_asm
interrupt_action_2_asm:
	xor rax,rax
	in al,64h
	and al,0x01
	cmp al,0x01
	jne readCharFromKeyboard_noData

	in ax,60h
	mov ah,0x00
	ret
readCharFromKeyboard_noData:
	mov eax,0xffff
	ret


;print a register
extern printRegister
;print exception
extern printException
;reboot kernel
extern reboot
%macro exception 1
	;preserve registers
	push rbp
	mov rbp,rsp
	pushState
	;print exception data
	mov rdi,%1
	call printException
	mov rdi,0;rsi
	call printRegister
	mov rdi,1;rax
	mov rsi,rax
	call printRegister
	mov rdi,2;rbx
	mov rsi,rbx
	call printRegister
	mov rdi,3;rcx
	mov rsi,rcx
	call printRegister
	mov rdi,4;rdx
	mov rsi,rdx
	call printRegister
	mov rdi,5;rsp
	mov rsi,[rbp+32]
	call printRegister
	mov rdi,6;rbp
	mov rsi,[rbp];rbp apunta al push de rbp
	call printRegister
	sti
	call reboot
	cli
	hlt
%endmacro

;division by zero exception
GLOBAL exception0
exception0:
	exception 0

;invalid opcode exception
GLOBAL exception6
exception6:
	exception 6

