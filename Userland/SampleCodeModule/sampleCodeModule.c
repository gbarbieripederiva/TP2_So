/* sampleCodeModule.c */

#include <lib.h>
#include <stdint.h>
#include <terminal.h>
#include <jueguito.h>
#include <libasm.h>


void checking(){
    while(1){
        print("Hola");
       printAction(0);
       print("Chau");
       printAction(0);

       int sid;
       sid = sys_create_semaphore("hola", 1);
       sys_sem_wait(sid);
    }
}

int main() {
    uint64_t terminalInfo;
    terminalInfo = sys_create_process("terminal",0, (uint64_t)terminal);
    sys_run_process(terminalInfo, 1);
    

    uint64_t checkInfo = sys_create_process("checking", 1, (uint64_t) checking);
    sys_run_process(checkInfo, 1);
    //sys_sem_post(1);

    

    return 0;
}

