/* myls implements the idea of the bash ls command. This command will list the files and directories
   inside of the current directory the user has inputed.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/util.h"

/* Author: Disaiah Bennett
   Version: 0.1 */

int main(int argc, char *argv[]){
    // When the user run the program with the argument [help], print out instructions on how the command work.
    if(argc == 2 && strcmp(argv[1], "help") == 0)
        describe();

    else if (argc == 2 && strcmp(argv[1], "-l") == 0){
        search_directory(".", argv[1]);
    }

    else if(argc == 2)
        search_directory(argv[1], NULL);  
    
    else if(argc > 2 && strcmp(argv[1], "-l") == 0){
        const char *flag = argv[1];
        search_directory(argv[2], argv[1]);
    }

    else if(argc > 2)
        search_directory(argv[2], NULL);

    else
        search_directory(".", NULL);

    return 0;
}