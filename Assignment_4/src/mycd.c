#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#define HOME getenv("HOME")

int main(int argc, char *argv[]){
    if(argc == 2){
        if(opendir(argv[1]))
            chdir(argv[1]);
        else
            printf("mycd: \033[0;31m%s\033[0m: file or directory does not exist\n", argv[1]);
    }
    else if(argc == 1)
        chdir(getenv("HOME"));
    else
        printf("mycd: usage [directory] ...\n", argv[1]);
    return 0;
}