GLOBAL enter_region
extern add_waiting_proc
enter_region:
    mov rax, 1
    xchg rax, qword [rdi]
    cmp rax, 0
    jnz add_waiting_proc
    ret

GLOBAL leave_region
extern check_blocked
leave_region:
    mov rax, 0
    xchg rax, qword [rdi]
    call check_blocked
    ret


GLOBAL xchg1
xchg1:
    mov rax, 1
    xchg rax, qword [rdi]
    ret

