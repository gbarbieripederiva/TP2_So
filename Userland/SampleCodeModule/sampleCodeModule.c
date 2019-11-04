/* sampleCodeModule.c */

#include <lib.h>
#include <stdint.h>
#include <terminal.h>
#include <jueguito.h>
#include <libasm.h>


void checking(){
    
    int sid = sys_create_semaphore(1, SEM_LOCKED);
    printDec(sid);
    printAction(0);
    sys_sem_post(sid);
    sys_sem_close(sid);
}

int main() {
    uint64_t terminalInfo;
    terminalInfo = sys_create_process(1, (uint64_t)terminal);
    sys_run_process(terminalInfo, 1);
    
    uint64_t checkInfo = sys_create_process(1, (uint64_t)checking);
    sys_run_process(checkInfo, 1);

    

    return 0;
}

