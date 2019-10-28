/* sampleCodeModule.c */

#include <lib.h>
#include <stdint.h>
#include <terminal.h>
#include <jueguito.h>
#include <libasm.h>

void checkTicks(){
    while(1){
        print("hola");
    }
}

int main() {
    uint64_t terminalInfo;
    terminalInfo = sys_create_process("terminal",0, (uint64_t)terminal);
    sys_run_process(terminalInfo, 1);

    uint64_t chekInfo = sys_create_process("check", 1, checkTicks);
    sys_run_process(chekInfo, 1);


    return 0;
}