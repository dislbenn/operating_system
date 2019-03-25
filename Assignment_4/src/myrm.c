#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "../include/myrm.h"

#define MAXSIZE 200

int main(int argc, char * argv[]){
    char destination[MAXSIZE], path[MAXSIZE], source[MAXSIZE], trash[MAXSIZE];
    FILE *f1, *f2;
    
    getcwd(path, sizeof(path));
    strcat(path, "/");

    if(argv[1]){
        if(strcmp(argv[1], "*") == 0){
            removeall(path, path, NULL);
            return(0);
        }
    }
    
    if(argv[2]){
        if(strcmp(argv[2], "*") == 0 && strcmp(argv[1], "-r") == 0){
            removeall(path, path, argv[1]);
            return 0;
        }
    }

    /* REMOVING FILE TO TRASH DIRECTORY */
    if(argc == 4 && strcmp(argv[1], "-r") == 0 && strcmp(argv[3], "trash") == 0){
        /*  SOURCE FILE */
        strcpy(source, argv[2]);
        strcat(path, source);
        
        printf("Path: %s\n", path);
        printf("Source: %s\n", source);

        /*  TRASH FILE */
        strcpy(trash, getenv("HOME"));
        strcat(trash, "/.Trash/");

        printf("Trash: %s\n", trash);
        chdir(trash);

        /*  DESTINATION FILE */
        strcpy(destination, trash);
        strcat(destination, source);

        printf("Destination: %s\n\n", destination);

        if(opendir(path))
            movedir(path, destination);
        else
            movefile(path, destination);
    }

    /* FILE OR DIRECTORY REMOVE */
    else if(argc == 3 && strcmp(argv[1], "-r") == 0){
        strcat(path, argv[2]);
        
        if(opendir(path))
            removedir(path, path);

        else if(fopen(path, "r") && !opendir(path))
            removefile(path);

        else
            printf("rm: \033[0;31m%s\033[0m: No such file or directory\n\n", path);
    }

    /* FILE REMOVE */
    else{
        if(argv[1])
            strcat(path, argv[1]);
        else
            exit(0);

        if(opendir(path))
            printf("rm: \033[0;31m%s\033[0m: is a directory\n\n", path);

        else if(fopen(path, "r") && !opendir(path))
            removefile(path);

        else
            printf("rm: \033[0;31m%s\033[0m: No such file or directory\n\n", path);
    }
    return 0;
}

