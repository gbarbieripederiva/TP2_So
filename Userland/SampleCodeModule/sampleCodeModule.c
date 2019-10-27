/* sampleCodeModule.c */

#include <lib.h>
#include <stdint.h>
#include <terminal.h>
#include <jueguito.h>
#include <libasm.h>

void checkTicks(){
    while(1){
        printDec(getTicks());
    }
}

int main() {
    uint64_t terminalInfo;
    terminalInfo = sys_create_process("terminal",0, (uint64_t)terminal);
    sys_run_process(terminalInfo, 1);

    int sid = sys_create_semaphore("sem1", 1);
    sys_sem_wait(sid);
    return 0;
}