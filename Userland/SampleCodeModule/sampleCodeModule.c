/* sampleCodeModule.c */

#include <lib.h>
#include <stdint.h>
#include <terminal.h>
#include <jueguito.h>
#include <libasm.h>

int main() {
    processInfo terminalInfo;
    terminalInfo = sys_create_process("terminal",0, (uint64_t)terminal);
    sys_run_process(terminalInfo);
    return 0;
}