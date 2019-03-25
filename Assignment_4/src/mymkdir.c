#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "../include/mymkdir.h"

#define MAXSIZE 200

int main(int argc, char * argv[]){
    char path[MAXSIZE], target[MAXSIZE];

    if(argc ==  2)
        strcpy(target, argv[1]);
    else
        exit(0);

    getcwd(path, sizeof(path));
    makedir(path, target);
    return 0;
}