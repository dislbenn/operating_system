// C Program to design a shell in Linux 
#include<stdio.h> 
#include<string.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<sys/wait.h>
#include "../include/util.h"
  
#define INPUTMAX 1000 // max number of letters to be supported 
#define COMMANDMAX 100 // max number of commands to be supported 
#define MAXSIZE 200

// Clearing the shell using escape sequences 
#define clear() printf("\033[H\033[J") 

// Greeting shell during startup 
void init_shell() 
{ 
    clear(); 
    printf("\033[0;33m******************************************\033[0m");
    printf("\n\n\t\t\033[0;32mMYBASH SHELL\033[0m"); 
    printf("\n\n\t\033[1;34m- \033[0m\033[0;35mCREATED BY DISAIAH BENNETT\033[0m\033[1;34m -\033[0m"); 
    printf("\n\n\033[0;33m******************************************\033[0m\n");
    sleep(2);
    clear();
}
  
// Function to print Current Directory.
void get_current_directory(){
    char path[MAXSIZE];
    getcwd(path, sizeof(path));
    printf("\033[0;35m%s\033[0m\n", path);
}

// Help command builtin 
void help(){
    printf("Im ready to help\n");
}

int get_token_count(char command[], char delim[]){
    int token_count = 0;
    char temp[INPUTMAX];
    strcpy(temp, command);

    char *token = strtok(temp, delim); 
    while(token != NULL){
        token = strtok(NULL, delim);
        token_count++;
    }

    return token_count;
}

int isMultiValid(char command[]){
    const char *argv[] = {"mymkdir", "mymv", "myrm", "myls", "mycd", "touch"};
    int i = 0, len = (int)(sizeof(argv) / sizeof(argv[0]));
    char temp[INPUTMAX];

    strcpy(temp, command);
    char *token = strtok(temp, " "); 

    while(i <= len-1){
        if(strcmp(temp, argv[i]) == 0)
            return i;
        else
            i++;
    }
    return -1;
}

int isValid(char command[]){
    const char *argv[] = {"mymkdir", "mymv", "myrm", "myls", "mycd", "help", "pwd"};
    int i = 0, len = (int)(sizeof(argv) / sizeof(argv[0]));

    while(i <= len-1){
        if(strcmp(command, argv[i]) == 0)
            return i;

        else
            i++;        
    }
    return -1;
}

void touch(char file[]){
    int c; FILE *f1;

    if(fopen(file, "r"))
       printf("touch: \033[0;31m%s\033[0m: File already exist\n", file);
    else{
        f1 = fopen(file, "w");

        while((c = fgetc(f1)) != EOF)
            fputc(c, f1);
    
        fclose(f1);
    }
}

// Function where the system command is executed -> argc == 1
int exec_multi_command(char command[]){
    const char *argv[] = {"mymkdir", "mymv", "myrm", "myls", "mycd", "touch"};
    int i = 0, len = (int)(sizeof(argv) / sizeof(argv[0])), argc = get_token_count(command, " ");
    FILE *f1, *f2;
    
    char sub_commands[argc][INPUTMAX], temp[INPUTMAX];
    char path[MAXSIZE], target[MAXSIZE], destination[MAXSIZE];
    char current[100], choice;

    getcwd(current, sizeof(current));

    strcpy(temp, command);
    char *token = strtok(temp, " ");

    while(token != NULL){
        strcpy(sub_commands[i], token);
        token = strtok(NULL, " ");
        i++;
    }
    
    if(isMultiValid(command) >= 0){
        switch(isMultiValid(command)){
            case 0: //mymkdir
                if(argc ==  2)
                    strcpy(target, sub_commands[1]);
                else
                    exit(0);

                getcwd(path, sizeof(path));
                makedir(path, target);
                break;
            
            case 1: //mymv
                if(argc == 3){
                    getcwd(path, sizeof(path));
                    strcat(path, "/");
                    strcpy(target, sub_commands[1]);
                    strcpy(destination, sub_commands[2]);

                    if(opendir(sub_commands[1]) && !opendir(sub_commands[2])){
                        printf("Moving Directory: %s to %s\n\n", sub_commands[1], sub_commands[2]);   
                    }

                    else if(opendir(sub_commands[1]) && opendir(sub_commands[2]))
                        movedirfile(path, target, destination);

                    else if(fopen(sub_commands[1], "r") && !fopen(sub_commands[2], "r"))
                        movefile(sub_commands[1], sub_commands[2]);

                    else if(fopen(sub_commands[1], "r") && fopen(sub_commands[2], "r")){
                        printf("mymv: \033[1;32m%s\033[0m: override \033[1;34m%s\033[0m? Enter y/N: ", sub_commands[1], sub_commands[2]);
                        scanf("%c", &choice);

                        if(choice == 'y' || choice == 'Y')
                            movefile(sub_commands[1], sub_commands[2]);
                        else
                            break;
                    }
                    else
                        printf("mymv: \033[1;31m%s\033[0m: \033[1;31m%s\033[0m: files/directories do not exist\n", sub_commands[1], sub_commands[2]);
                }
                break;

            case 2: //myrm
                getcwd(path, sizeof(path));
                strcat(path, "/");

                /* FILE OR DIRECTORY REMOVE */
                if(argc == 3 && strcmp(sub_commands[1], "-r") == 0){
                    strcat(path, sub_commands[2]);
                    
                    if(opendir(path)){
                        removedir(path, path);
                        chdir(current);
                    }
                    else if(fopen(path, "r") && !opendir(path))
                        removefile(path);

                    else
                        printf("rm: \033[0;31m%s\033[0m: No such file or directory\n\n", path);
                }

                /* FILE REMOVE */
                else{
                    if(sub_commands[1])
                        strcat(path, sub_commands[1]);
                    else
                        break;

                    if(opendir(path))
                        printf("rm: \033[0;31m%s\033[0m: is a directory\n\n", path);

                    else if(fopen(path, "r") && !opendir(path))
                        removefile(path);

                    else
                        printf("rm: \033[0;31m%s\033[0m: No such file or directory\n\n", path);
                }
            break;

            case 3: //myls
                if(argc == 2 && strcmp(sub_commands[1], "help") == 0)
                    describe();

                else if (argc == 2 && strcmp(sub_commands[1], "-l") == 0)
                    search_directory(".", sub_commands[1]);

                else if(argc == 2)
                    search_directory(sub_commands[1], NULL);  
    
                else if(argc > 2 && strcmp(sub_commands[1], "-l") == 0){
                    const char *flag = sub_commands[1];
                    search_directory(sub_commands[2], sub_commands[1]);
                }

                else if(argc > 2)
                    search_directory(sub_commands[2], NULL);
                break;

            case 4: //mycd
                if(opendir(sub_commands[1]))
                    chdir(sub_commands[1]);
                else
                    printf("mycd: \033[0;31m%s\033[0m: file or directory does not exist\n", sub_commands[1]);
                break;

            case 5:
                touch(sub_commands[1]);
        }
        return 1;
    }
    else{
        return 0;
    }
    return 1;
}
  
// Function where the system command is executed -> argc == 1
int exec_command(char command[]){
    if(isValid(command) >= 0){
        switch(isValid(command)){
            case 0: //mymkdir
                printf("usage: \033[0;33m%s\033[0m [directory] ...\n", command);
                break;
            
            case 1: //mymv
                printf("usage: \033[0;33m%s\033[0m [-r][target][destination] ...\n", command);
                break;

            case 2: //myrm
                printf("usage: \033[0;33m%s\033[0m [-r][directory][trash] ...\n", command);
                break;

            case 3: //myls
                search_directory(".", NULL);
                break;

            case 4: //mycd
                printf("usage: \033[0;33m%s\033[0m [directory] ...\n", command);
                break;

            case 5: //help
                help();
                break;

            case 6: //pwd
                get_current_directory();
                break;                                    
        }
        return 1;
    }
    else{
        return 0;
    }
}
 
// function for parsing command words 
int tokensexist(char input[]){
    int token_count = 0;
    char *token = strtok(input, " "); 

    while(token != NULL){
        token = strtok(NULL, " ");
        token_count++;
    }

    if(token_count > 1)
        return 1;
    else
        return 0;
    
}
  
int main(){ 
    init_shell();

    int condition;
    char buffer[100], c, command[INPUTMAX], input[INPUTMAX];

    while (1) { 
        printf("mybash-disaiah-0.1:$ ");
        strcpy(input, fgets(buffer, 100, stdin));
        char c = input[0];

        if(c == '\n')
            continue;

        input[strcspn(input, "\n")] = '\0';
        if(strcmp(input, "exit") == 0){
            printf("exit\n");
            break;
        }

        else if(strcmp(input, "clear") == 0 || strcmp(input, "CLEAR") == 0){
            clear();
            continue;
        }

        strcpy(command, input);
        condition = tokensexist(input);

        if(condition == 1){
            if(exec_multi_command(command) == 1)
                continue;
            else
                printf("mybash: \033[0;31m%s\033[0m: command not found\n", command);
        }

        else // If only one argument
            if(exec_command(command) == 1) // If the argument execute successfully, then continue, else print command not found.
                continue;
            else
                printf("mybash: \033[0;31m%s\033[0m: command not found\n", command);
    } 
    return 0; 
}