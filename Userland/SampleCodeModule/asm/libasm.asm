
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
	mov r8, rcx
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
	mov rdi,20
	int 80h
	popState
	ret


GLOBAL sys_get_memory
sys_get_memory:
	pushState
	adjust_to_sys_call
	;sys call sys_get_memory is call number 45
	mov rdi,45
	int 80h
	popState
	ret

GLOBAL sys_mem_free
sys_mem_free:
	pushState
	adjust_to_sys_call
	;sys call sys_mem_free is call number 46
	mov rdi,46
	int 80h
	popState
	ret

GLOBAL sys_create_process
sys_create_process:
	pushState
	adjust_to_sys_call
	;sys call sys_create_process is call number 47
	mov rdi,47
	int 80h
	popState
	ret

GLOBAL sys_run_process
sys_run_process:
	pushState
	adjust_to_sys_call
	;sys call sys_run_process is call 48
	mov rdi, 48
	int 80h
	popState
	ret

GLOBAL sys_kill_process
sys_kill_process:
	pushState
	adjust_to_sys_call
	;sys call sys_kill_process is call 49
	mov rdi,49
	int 80h
	popState
	ret

GLOBAL sys_get_pid
sys_get_pid:
	pushState
	adjust_to_sys_call
	;sys call sys_get_pid is call 50
	mov rdi,50
	int 80h
	popState
	ret

GLOBAL sys_print_running_procs
sys_print_running_procs:
	pushState
	adjust_to_sys_call
	;sys_print_running_procs is call 51
	mov rdi, 51
	int 80h
	popState
	ret

GLOBAL sys_set_priority
sys_set_priority:
	pushState
	adjust_to_sys_call
	;sys_set_priority is call 52
	mov rdi, 52
	int 80h
	popState
	ret

GLOBAL sys_set_state
sys_set_state:
	pushState
	adjust_to_sys_call
	;sys_set_state is call 53
	mov rdi, 53
	int 80h
	popState
	ret

GLOBAL sys_create_process_params
sys_create_process_params:
	pushState
	adjust_to_sys_call
	;sys_create_process_params is call 54
	mov rdi, 54
	int 80h
	popState
	ret

GLOBAL sys_create_semaphore
sys_create_semaphore:
	pushState
	adjust_to_sys_call
	;sys call sys_create_semaphore is call 60
	mov rdi,60
	int 80h
	popState
	ret

GLOBAL sys_sem_close
sys_sem_close:
	pushState
	adjust_to_sys_call
	;sys call sys_sem_close is call 61
	mov rdi,61
	int 80h
	popState
	ret

GLOBAL sys_sem_post
sys_sem_post:
	pushState
	adjust_to_sys_call
	;sys call sys_sem_post is call 62
	mov rdi,62
	int 80h
	popState
	ret

GLOBAL sys_sem_wait
sys_sem_wait:
	pushState
	adjust_to_sys_call
	;sys call sys_sem_wait is call 63
	mov rdi,63
	int 80h
	popState
	ret

GLOBAL sys_print_sems
sys_print_sems:
	pushState
	adjust_to_sys_call
	;sys call sys_print_sems is call 64
	mov rdi,64
	int 80h
	popState
	ret

GLOBAL sys_open_pipe
sys_open_pipe:
	pushState
	adjust_to_sys_call
	;sys call sys_open_pipe is call 75
	mov rdi,75
	int 80h
	popState
	ret

GLOBAL sys_close_pipe
sys_close_pipe:
	pushState
	adjust_to_sys_call
	;sys call sys_close_pipe is call 76
	mov rdi,76
	int 80h
	popState
	ret

GLOBAL sys_write_pipe
sys_write_pipe:
	pushState
	adjust_to_sys_call
	;sys call sys_write_pipe is call 77
	mov rdi,77
	int 80h
	popState
	ret

GLOBAL sys_read_pipe
sys_read_pipe:
	pushState
	adjust_to_sys_call
	;sys call sys_read_pipe is call 78
	mov rdi,78
	int 80h
	popState
	ret

GLOBAL sys_print_pipe
sys_print_pipe:
	pushState
	adjust_to_sys_call
	;sys call sys_print_pipe is call 79
	mov rdi,79
	int 80h
	popState
	ret


GLOBAL testchoi
testchoi:
	pushState
	adjust_to_sys_call
	mov rdi, 120
	int 80h
	popState
	ret