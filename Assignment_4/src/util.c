#include <dirent.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <grp.h>
#include <pwd.h>
#include "../include/util.h"
#include "../include/myrm.h"

void get_permission(struct stat buffer){
    printf( (S_ISDIR(buffer.st_mode)) ? "d" : "-");
    printf( (buffer.st_mode & S_IRUSR) ? "r" : "-");
    printf( (buffer.st_mode & S_IWUSR) ? "w" : "-");
    printf( (buffer.st_mode & S_IXUSR) ? "x" : "-");
    printf( (buffer.st_mode & S_IRGRP) ? "r" : "-");
    printf( (buffer.st_mode & S_IWGRP) ? "w" : "-");
    printf( (buffer.st_mode & S_IXGRP) ? "x" : "-");
    printf( (buffer.st_mode & S_IROTH) ? "r" : "-");
    printf( (buffer.st_mode & S_IWOTH) ? "w" : "-");
    printf( "%-4s", (buffer.st_mode & S_IXOTH) ? "x" : "-");
}

int get_files(char *path, struct stat buffer, DIR *file, char *flag){
    /* Prints and returns the # of files located in the targeted directory.
        Param:
            path: char - directory path.
            buffer: - buffer for the directory path. 
            file: - file in directory.
        Example:
            >>> const char *flag = argv[1];
            >>> struct stat buffer;
            >>> DIR *file;
            >>> file = opendir(argv[2]);
            >>> get_file(argv[2], buffer, file, flag);
    */

    printf("\033[1;31m# of files in the directory\n-------------------------------------------------\n\033[0m");    
    int f_count = 0; // # of files in current directory
    struct dirent *dir;
    struct passwd *pwd;
    struct group *grp;

    while((dir = readdir(file)) != NULL){
        stat(dir->d_name, &buffer);
        off_t size = buffer.st_size;
        pwd = getpwuid(buffer.st_uid);
        grp = getgrgid(buffer.st_gid);

        if(S_ISREG(buffer.st_mode)){
            if(flag == NULL)
                printf("> %s\n", dir->d_name);
            else{
                printf(">> %-14s ", dir->d_name);
                get_permission(buffer);
                printf("%-7lld %-7s %s\n", size, pwd->pw_name, grp->gr_name);
            }
            f_count += 1;
        }
    }
    printf("\nTOTAL AMOUNT OF FILES: %d\n\n", f_count);
    return f_count;
}

int get_directories(char *path, struct stat buffer, DIR *directory, char *flag){
    /* Prints and returns the # of directories located in the targeted directory.
        Param:
            path: char - directory path.
            buffer: struct stat - buffer for the directory path. 
            file: DIR - file in directory.
            flag: char - command flag.
        Example:
            >>> const char *flag = argv[1];
            >>> struct stat buffer;
            >>> DIR *directory;
            >>> directory = opendir(argv[2]);
            >>> get_directories(argv[2], buffer, directory, flag);
    */

    printf("\033[1;32m# of directories in the directory\n-------------------------------------------------\n\033[0m");
    int d_count = 0; // # of directories in current directory
    struct dirent *dir;
    struct passwd *pwd;
    struct group *grp;

    while((dir = readdir(directory)) != NULL){
        stat(dir->d_name, &buffer);
        off_t size = buffer.st_size;
        pwd = getpwuid(buffer.st_uid);
        grp = getgrgid(buffer.st_gid);    

        if(S_ISDIR(buffer.st_mode)){
            if(flag == NULL)
                printf("> %s\n", dir->d_name);
            else{
                printf(">> %-15s", dir->d_name);
                get_permission(buffer);
                printf("%-5lld %-9s %s\n", size, pwd->pw_name, grp->gr_name);
            }
            d_count += 1;
        }
    }
    printf("\nTOTAL # OF DIRECTORIES: %d\n\n" , d_count);
    return d_count;
}

void search_directory(char *path, char *flag){
    /* Search the targeted directories and prints the all files and directories in that directory.
        Param:
            path: char - directory path.
            flag: char - command flag.
        Example:
            >>> const char *flag = argv[1];
            >>> get_directories(argv[1], NULL);
            >>> get_directories(argv[2], NULL)
            >>> get_directories(argv[2], argv[1])
    */
    char orig[200];
    getcwd(orig, sizeof(orig));

    DIR *file = opendir(path), *directory = opendir(path);
    struct stat buffer;
    char *f = flag;

    if(file == NULL || directory == NULL){ // If the directory path cannot be opened.
        printf("[ERROR] - Unable to open file/directory, path does not exist!\n\n");
        exit(1);
    }
    else
        chdir(path);

    printf("\n");
    get_files(path, buffer, file, f);
    get_directories(path, buffer, directory, f);
    chdir(orig);
}

void describe(){
    /// Help function explains the functions inside of the myls program./
    printf("\033[1;35m-----------------------------------------------------------------------------------------------\n\033[0m");
    printf("\033[1;36mmyls COMMAND\033[0m:\n\n");
    printf("\033[1;33mImplements the idea of the bash ls command. This command will list the files and\n");
    printf("directories inside of the current directory the user has inputed.\033[0m");
    printf("\n\n");

    // get_files
    printf("\033[1;32mint get_files(char *path, struct stat buffer, DIR *file, char *flag)\033[0m\n");
    printf("\033[1;37mPrints and returns the # of files located in the targeted directory.\n\n\033[0m");
    
    // get_directories
    printf("\033[1;32mint get_directories(char *path, struct stat buffer, DIR *directory, char *flag)\033[0m\n");
    printf("\033[1;37mPrints and returns the # of directories located in the targeted directory.\n\n\033[0m");
    
    // search_directories
    printf("\033[1;32mvoid search_directory(char *path, char *flag)\033[0m\n");
    printf("\033[1;37mCalls the functions get_files and get_directories to list the files in the targeted directory.\n\033[0m");
    printf("\033[1;35m-----------------------------------------------------------------------------------------------\n\033[0m");
    exit(1);
}

void movedir(char path[], char destination[]){
    int c; char choice;
    if(opendir(path) && opendir(destination)){
        printf("Directory Already Exist: \033[1;31m%s\033[0m\n", destination);
        printf("Remove Existing Directory? Enter y/N: ");
        scanf("%c", &choice);

        if(choice == 'y' || choice == 'Y'){
            remove(destination);
            umask(0);
            mkdir(destination, S_IRWXU | S_IRWXG | S_IRWXO);
            printf("Removed & Created New Directory: \033[1;32m%s\033[0m\n\n", destination);
        }
        else
            exit(0);
    }
    else if(opendir(path) && !opendir(destination)){
        umask(0);
        mkdir(destination, S_IRWXU | S_IRWXG | S_IRWXO);
        printf("Created New Directory: \033[1;32m%s\033[0m\n\n", destination);
    }
}

void movefile(char path[], char destination[]){
    int c; FILE *f1, *f2;

    f1 = fopen(path, "r");
    f2 = fopen(destination, "w");

    if(!f1){
        printf("\033[1;31mUnable to read from the file -> %s\033[0m\n\n", path);
        fclose(f2);
        exit(0);
    }

    if(!f2){
        printf("\033[1;31mUnable to write to the file -> %s\033[0m\n\n", destination);
        exit(0);
    }

    while((c = fgetc(f1)) != EOF){
        fputc(c, f2);
    }

    fclose(f1);
    fclose(f2);

    remove(path);
}

void removedir(char orig[], char path[]){
    DIR *directory = opendir(orig);
    struct dirent *dir; struct stat buffer;
    int n = 0, n2 = 0; char temp[200];

    if(chdir(path) == 0){ // If you can successfully change into that directory change.
        chdir(path);
        directory = opendir(path);

        while((dir = readdir(directory)) != NULL){ // While directory file is not none
            if(strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0)
                continue; // Pass directory . and ..
            ++n; // Increment if there are more files.
            stat(dir->d_name, &buffer);

            strcpy(temp, path);
            strcat(temp, "/");
            strcat(temp, dir->d_name);

            //printf("%d %s\n", n, temp);
            if(opendir(temp)){ // If it is a directory
                if(rmdir(temp) == 0){ // Check if the directory is empty
                    rmdir(temp); // If so, delete the path to the directory
                    //printf("d - %s removed\n", dir->d_name);
                    --n;
                }
                else
                    removedir(orig, temp);
            }    
            else{
                //printf("f - %s removed\n", dir->d_name);
                remove(temp);
                --n;
            }
        }
    }
    
    if(n == 0 && rmdir(path) == 0){
        //printf("Removing: %s\n\n", path);
        rmdir(path);
        removedir(orig, orig);
    }
}

void removefile(char path[]){
    if(fopen(path, "r"))
        remove(path);
}

void removeall(char orig[], char path[], char flag[]){
    DIR *directory = opendir(orig);
    struct dirent *dir; struct stat buffer;
    int n = 0, n2 = 0; char temp[200];

    if(flag)
        removedir(orig, path);
    
    else{
        while((dir = readdir(directory)) != NULL){
            if(strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0)
                continue; // Pass directory . and ..
            ++n; // Increment if there are more files.
            stat(dir->d_name, &buffer);

            strcpy(temp, path);
            strcat(temp, "/");
            strcat(temp, dir->d_name);

            if(!opendir(temp))
                remove(temp);
        }
    }
}

void makedir(char path[], char target[]){
    char destination[200]; // Destination path
    int n = 0;

    strcpy(destination, path);
    strcat(destination, "/");

    char *token = strtok(target, "/"); 
    while(token != NULL){
        strcat(destination, token);
        strcat(destination, "/");
        
        if(opendir(destination))
            chdir(destination);

        token = strtok(NULL, "/");

        ++n;
        if(n > 5)
            break;
    }
    
    if(opendir(destination))
        printf("mymkdir: \033[1;31m%s\033[0m: already exist\n", destination);
    
    else
        mkdir(destination, S_IRWXU | S_IRWXG | S_IRWXO);

    chdir(path);
}

void movedirfile(char path[], char target[], char destination[]){
    DIR *directory = opendir(target);
    struct dirent *dir; struct stat buffer;
    char f[100], temp[100], temp2[100];
    int c; FILE *f1, *f2;

    strcpy(temp, destination);

    strcat(destination, "/");
    strcat(destination, target);

    if(opendir(destination)){
        printf("mymv: \033[1;31m%s\033[0m: directory already exist\n", destination);
        exit(0);
    }
    else
        mkdir(destination, S_IRWXU | S_IRWXG | S_IRWXO);
    
    printf("Path: \033[0;36m%s\033[0m Target: \033[0;35m%s\033[0m\n\n", path, destination);

    while((dir = readdir(directory)) != NULL){ // While directory file is not none
        if(strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0)
            continue; // Pass directory . and ..
    
        strcpy(destination, temp);
        strcat(destination, "/");
        strcat(destination, target);
        strcat(destination, "/");
        
        strcpy(f, target);
        strcat(f, "/"); 
        strcat(f, dir->d_name);

        strcat(destination, dir->d_name);

        if(opendir(f))
            continue;
        
        else
            movefile(f, destination);
    }
    if(rmdir(target) == 0)
        rmdir(target);
}

void changedir(char path[]){
    
}