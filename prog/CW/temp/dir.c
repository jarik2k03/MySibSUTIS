#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

void search(char * pt)
{
    char path[5000];
    printf("Каталог: %s\n", pt);
    struct dirent *fi;
    if(strcmp(pt, "/")) strcat(pt, "/");
    DIR * di = opendir(pt);
    struct stat stt;
    if(!di){
        printf("Ошибка открытия %s\n", pt);
        return;
    }
    while ((fi = readdir(di))) {
         if (!strcmp(fi->d_name, ".")||!strcmp(fi->d_name, "..")) continue;
         strcpy(path, pt);
         strcat(path, fi->d_name);
         lstat(path, &stt);
         if( S_ISDIR(stt.st_mode)){
             search(path);
         } else {
            if(S_ISREG(stt.st_mode)) printf("File: ");
            if(S_ISLNK(stt.st_mode)) printf("Slink: ");
            if(S_ISCHR(stt.st_mode)) printf("Sym: ");
            if(S_ISBLK(stt.st_mode)) printf("Blok: ");
            if(S_ISFIFO(stt.st_mode)) printf("Fifo: ");
            if(S_ISSOCK(stt.st_mode)) printf("Socket: ");
            printf("%s\n", path);
         }
    }
    closedir(di);
}

int main()
{
    char pt[5000];
    search(pt);
}