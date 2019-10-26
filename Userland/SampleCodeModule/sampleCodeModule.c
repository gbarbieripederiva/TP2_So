/* sampleCodeModule.c */

#include <lib.h>
#include <stdint.h>
#include <terminal.h>
#include <jueguito.h>
#include <libasm.h>
#include <schedulerTest.h>

int main() {
    //uint64_t testInfo = sys_create_process("test", 1, (uint64_t) test);
    //sys_run_process(testInfo, READY);
    uint64_t terminalInfo;
    terminalInfo = sys_create_process("terminal",0, (uint64_t)terminal);
    sys_run_process(terminalInfo, 1);
    return 0;
}


