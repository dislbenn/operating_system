#ifndef UTIL_H
#define UTIL_H
#include <dirent.h>
#include <sys/stat.h>
 
void movedir(char path[], char destination[]);
void movefile(char path[], char destination[]);

void removedir(char orig[], char path[]);
void removefile(char path[]);

void removeall(char orig[], char path[], char flag[]);

void describe();

#endif