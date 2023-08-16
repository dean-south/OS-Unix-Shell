#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char const *argv[])
{
    int pid = fork();
    if(pid == 0){
        return 69;
    }
    else{
        int res = -5;
        wait(&res) ;
    printf("the result was %d\n" , WEXITSTATUS(res));
    }
    return 0;
}
