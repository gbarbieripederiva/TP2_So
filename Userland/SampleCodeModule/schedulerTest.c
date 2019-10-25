#include <libasm.h>
#include <lib.h>
#include <stdint.h>

void test1(){
    while(1){
        print("test1");
        printAction(0);
    }
}

void test0(){
    while(1){
        print("test0");
        printAction(0);
    }
}

void test(){
    uint64_t proc1 = sys_create_process("test 1", 1, test1);
    sys_run_process(proc1, READY);

    uint64_t proc2 = sys_create_process("test 0", 0, test0);
    sys_run_process(proc2, READY);

    print("All processes:");
    sys_print_processes();


}