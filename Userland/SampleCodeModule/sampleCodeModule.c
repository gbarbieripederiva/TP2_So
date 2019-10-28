/* sampleCodeModule.c */

#include <lib.h>
#include <stdint.h>
#include <terminal.h>
#include <jueguito.h>
#include <libasm.h>
#include <naiveConsole.h>

void checking(){
    while(1){
        ncPrint("Hola");
        ncNewLine();
    }
}

int main() {
    uint64_t terminalInfo;
    terminalInfo = sys_create_process("terminal",0, (uint64_t)terminal);
    sys_run_process(terminalInfo, 1);

    uint64_t checkInfo = sys_create_process("checking", 1, (uint64_t) checking);
    sys_run_process(checkInfo, 1);

    return 0;
}

