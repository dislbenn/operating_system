#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include "../include/mymv.h"
#define MAXSIZE 200

// Read file content into other file, if the file exist dont overwrite
// Make new directory, copy each file into 

int main(int argc, char * argv[]){
    char path[MAXSIZE], target[MAXSIZE], destination[MAXSIZE];
    char choice;

    if(argc == 3){
        getcwd(path, sizeof(path));
        strcat(path, "/");
        strcpy(target, argv[1]);
        strcpy(destination, argv[2]);

        if(opendir(argv[1]) && !opendir(argv[2])){
            printf("Moving Directory: %s to %s\n\n", argv[1], argv[2]);   
        }

        else if(opendir(argv[1]) && opendir(argv[2]))
            movedirfile(path, target, destination);

        else if(fopen(argv[1], "r") && !fopen(argv[2], "r"))
            movefile(argv[1], argv[2]);

        else if(fopen(argv[1], "r") && fopen(argv[2], "r")){
            printf("mymv: \033[1;32m%s\033[0m: override \033[1;34m%s\033[0m? Enter y/N: ", argv[1], argv[2]);
            scanf("%c", &choice);

            if(choice == 'y' || choice == 'Y')
                movefile(argv[1], argv[2]);
            else
                exit(0);
        }
        else
            printf("mymv: \033[1;31m%s\033[0m: \033[1;31m%s\033[0m: files/directories do not exist\n", argv[1], argv[2]);
    }
    
    return 0;
}