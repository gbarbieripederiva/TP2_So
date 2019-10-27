#include <libasm.h>

void basura(){
    char * basura1 = (char *)sys_get_memory(1024);
    char * basura2 = (char *)sys_get_memory(1024);

    basura1[0] = 1;
    basura1[1] = 1;
    basura1[2] = 1;
    basura1[3] = 1;
    basura1[4] = 1;
    basura1[5] = 1;
    basura2[0] = 1;
    basura2[1] = 1;
    basura2[2] = 1;
    basura2[3] = 1;
    basura2[4] = 1;
    basura2[5] = 1;
    basura2[6] = 1;

}