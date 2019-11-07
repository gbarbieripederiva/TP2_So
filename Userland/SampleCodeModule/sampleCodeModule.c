/* sampleCodeModule.c */

#include <lib.h>
#include <stdint.h>
#include <terminal.h>
#include <jueguito.h>
#include <libasm.h>






int main() {
    int fd = sys_open_pipe(1);
    uint64_t terminalInfo;
    terminalInfo = sys_create_process(1, (uint64_t)terminal);
    sys_set_stdin(terminalInfo, fd);
    sys_run_process(terminalInfo, 1);
    

    

    return 0;
}

