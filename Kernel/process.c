#include <process.h>
#include <scheduler.h>
#include <stdint.h>
#include <memoryManager.h>
#include <interrupts.h>
#include <null.h>

static int pid = 1;

#pragma pack(push)
#pragma pack(1)
typedef struct stack{
    uint64_t r15;
    uint64_t r14;
    uint64_t r13;
    uint64_t r12;
    uint64_t r11;
    uint64_t r10;
    uint64_t r9;
    uint64_t r8;
    uint64_t rsi;
    uint64_t rdi;
    uint64_t rbp;
    uint64_t rdx;
    uint64_t rcx;
    uint64_t rbx;
    uint64_t rax;

    // what iret returns

    uint64_t rip;
    uint64_t cs;
    uint64_t eflags;
    uint64_t rsp;
    uint64_t ss;
    uint64_t base;

} stack_frame;
#pragma pack(pop)

/*
popState
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
    */


static processInfo processes[MAX_PROCESSES];
static int pid;

void init_processes(){
    pid = 0;

    for(int i = 0; i < MAX_PROCESSES; i++){
        processes[i] = NULL;
    }
}

void run_set_return(uint64_t rip, processInfo process){
    void (*main) (void) = (void (*) (void)) rip;
    (*main)();
    kill_process(process -> pid);
    if(get_current_pid() == process ->pid){

        _int20();
    }
}


uint64_t build_stack(uint64_t rip, uint64_t from, processInfo process){
    stack_frame  * stack = (stack_frame *)(from + STACK_SIZE - sizeof(stack_frame) - 1);
    stack -> r15 = 0X0;
    stack -> r14 = 0X0;
    stack -> r13 = 0X0;
    stack -> r12 = 0X0;
    stack -> r11 = 0X0;
    stack -> r10 = 0X0;
    stack -> r9 = 0X0;
    stack -> r8 = 0X0;
    stack -> rsi = (uint64_t)process;
    stack -> rdi = rip;
    stack -> rbp = 0X0;
    stack -> rdx = 0X0;
    stack -> rcx = 0X0;
    stack -> rbx = 0X0;
    stack -> rax = 0X0;
    

    stack -> rip = (uint64_t) run_set_return;
    stack -> cs = 0X8;
    stack -> eflags = 0X202;
    stack -> rsp = (uint64_t)&(stack -> base);
    stack -> ss = 0X0;
    stack -> base = 0X0;

    return (uint64_t) stack;

}

processInfo create_process(char * name, int priority, uint64_t rip){
    if(pid >= MAX_PROCESSES){
        return NULL;
    }
    processInfo process = (processInfo)giveMeMemory((long)sizeof(process));
    if(pid != 0){
    process -> ppid = get_current_pid();}
    process -> pid = pid;
    process -> priority = priority;
    process -> name = name;
    process -> stack_end = (uint64_t)giveMeMemory(STACK_SIZE);
    process -> sp = build_stack(rip, process -> stack_end, process);
    processes[pid++] = process;

    return process;
}



int set_priority(int pid, int priority){
    int curr = 0;
    while(curr < MAX_PROCESSES && processes[curr] -> pid != pid){
        curr++;
    }
    if(processes[curr]->pid == pid){
        processes[curr] ->priority = priority;
        return 0;
    }
    return -1;

}


