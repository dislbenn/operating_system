#ifndef UTIL_H
#define UTIL_H
#include <dirent.h>
#include <sys/stat.h>
 
void get_permission(struct stat buffer);
int get_files(char *path, struct stat buffer, DIR *file, char *flag);
int get_directories(char *path, struct stat buffer, DIR *directory, char *flag);
void search_directory(char *path, char *flag);
void describe();

void movedir(char path[], char destination[]);
void movefile(char path[], char destination[]);
void movedirfile(char path[], char target[], char destination[]);

void removedir(char orig[], char path[]);
void removefile(char path[]);
void removeall(char orig[], char path[], char flag[]);

void makedir(char path[], char target[]);
 
#endif