/* sampleCodeModule.c */

#include <lib.h>
#include <stdint.h>
#include <terminal.h>
#include <jueguito.h>
#include <libasm.h>


// void checkingIn(){
//     for(int i=0;i<10;i++){
//         printDec(i);
//     }
// }

// void checkingOut(){
//     char str[2];
//     str[1]=0;
//     while(1){
//         str[0]=getLastInput();
//         print(str);
//     }
// }

int main() {
    uint64_t terminalInfo;
    terminalInfo = sys_create_process(1, (uint64_t)terminal);
    sys_run_process(terminalInfo, 1);

    // uint64_t in=sys_create_process(1,(uint64_t)checkingIn);
    // uint64_t out=sys_create_process(1,(uint64_t)checkingOut);
    // int pipe[2];
    // sys_open_pipe(pipe);
    // sys_dup_in(((processInfo)in)->pid,pipe[0]);
    // sys_dup_in(((processInfo)in)->pid,pipe[1]);
    // sys_run_process(in,1);
    // sys_run_process(out,1);


    return 0;
}

