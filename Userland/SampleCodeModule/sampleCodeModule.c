/* sampleCodeModule.c */

#include <lib.h>
#include <stdint.h>
#include <terminal.h>
#include <jueguito.h>
#include <libasm.h>


void checking(){
    
    int stdout = sys_get_stdout();
    char *str = "HOLA MUNDO";
    char buff[80];
    write_fd(stdout, str);
    

    
}

int main() {
    int fd = sys_open_pipe(1);
    uint64_t terminalInfo;
    terminalInfo = sys_create_process(1, (uint64_t)terminal);
    sys_set_stdin(terminalInfo, fd);
    sys_run_process(terminalInfo, 1);

    uint64_t checkInfo = sys_create_process(0, (uint64_t)checking);
    sys_set_stdout(checkInfo, fd);
    sys_run_process(checkInfo, 1);
    

    

    return 0;
}

