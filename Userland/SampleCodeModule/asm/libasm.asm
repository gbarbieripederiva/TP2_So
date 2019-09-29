GLOBAL testFun

section .text


;push all registers except rax
%macro pushState 0
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

;pop all registers except rax
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
%endmacro

;Macro to adjust register for a standard sys_call, because we want in the first argument 
;the sys_call number
; rdi -> rsi, rsi->rdx, rdx -> rcx,
%macro adjust_to_sys_call 0
	mov rcx,rdx
	mov rdx,rsi
	mov rsi,rdi
%endmacro

;--------------------------SYS_CALLS FUNCTIONS--------------------------------;
GLOBAL sys_read_call
sys_read_call:
	pushState
	adjust_to_sys_call
	;sys_call read is call number 0
	mov rdi,0
	int 80h
	popState
	ret


GLOBAL sys_write_call
sys_write_call:
	pushState
	adjust_to_sys_call
	;sys_call write is call number 1
	mov rdi,1
	int 80h
	popState
	ret


GLOBAL sys_put_char_call
sys_put_char_call:
	pushState
	adjust_to_sys_call
	;sys_call put char is call number 2
	mov rdi,2
	int 80h
	popState
	ret

GLOBAL sys_print_action_call
sys_print_action_call:
	pushState
	adjust_to_sys_call
	;sys_call print action is call number 3
	mov rdi,3
	int 80h
	popState
	ret

GLOBAL sys_print_number_call
sys_print_number_call:
	pushState
	adjust_to_sys_call
	;sys_print_number is call number 4
	mov rdi,4
	int 80h
	popState
	ret

GLOBAL sys_change_sound_call
sys_change_sound_call:
	pushState
	adjust_to_sys_call
	;sys_change_sound is call number 5
	mov rdi,5
	int 80h
	popState
	ret


GLOBAL sys_put_char_fixed_call
sys_put_char_fixed_call:
	pushState
	adjust_to_sys_call
	;sys_call put char fixed is call number 6
	mov rdi,6
	int 80h
	popState
	ret


GLOBAL sys_get_ticks_call
sys_get_ticks_call:
	pushState
	adjust_to_sys_call
	;sys_call get_ticks is call number 10
	mov rdi, 10
	int 80h
	popState
	ret


GLOBAL sys_get_last_in_call
sys_get_last_in_call:
	pushState
	adjust_to_sys_call
	;sys_call get last in is call number 11
	mov rdi,11
	int 80h
	popState
	ret


GLOBAL sys_get_clock_call
sys_get_clock_call:
	pushState
	adjust_to_sys_call
	;sys_call get_clock is call number 12.
	mov rdi, 12
	int 80h
	popState
	ret


GLOBAL sys_screen
sys_screen:
	pushState
	adjust_to_sys_call
	;sys call sys_screen is call number 20
	mov rdi,20;
	int 80h
	popState
	ret