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

       
    }
}

int main() {
    uint64_t terminalInfo;
    terminalInfo = sys_create_process("terminal",1, (uint64_t)terminal);
    sys_run_process(terminalInfo, 1);
    

    

    return 0;
}

