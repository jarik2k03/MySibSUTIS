#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include "kmp.h"
#define TEXTLIM 180
#define S_MAX 10000

int* compute_prefix(char* substr, int m)
{
	short i, j;
	int *d = (int*)malloc(m * sizeof(int));
	d[0] = 0;
	for(i = 1, j = 0; i < m; i++)
	{
		while(j > 0 && substr[j] != substr[i])
			j = d[j-1];
		if(substr[j] == substr[i])
			j++;
		d[i] = j;
	}
	return d;

}

int KMP_matcher(char* str, char* sub)
{
	int count = 0;
	int i, j, n, m;
	int *pi = NULL;
	n = strlen(str);
	m = strlen(sub);
	int q = 0;
	pi = compute_prefix(sub, m);
	// printf("\tPrefix massiv: ");
	// for (i = 0; i < m; i++)
	// 	printf("%d", pi[i]);
	// printf("\n");

	for (i = 0; i < n; i++)
	{
		while(q > 0 && sub[q] != str[i])
			q = pi[q - 1];
		if (sub[q] == str[i])
			q++;

		if (q == m)
			{
				count++;
				printf("\t-Sovpadeniye posle %d simvola\n", i - m + 1);
			}
	}
		free(pi);
		return count;

}

int Kmp_for_filename(char* str, char* sub)
{
	int count = 0;
	int i, j, n, m;
	int *pi = NULL;
	n = strlen(str);
	m = strlen(sub);
	int q = 0;
	pi = compute_prefix(sub, m);

	for (i = 0; i < n; i++)
	{
		while(q > 0 && sub[q] != str[i])
			q = pi[q - 1];
		if (sub[q] == str[i])
			q++;

		if (q == m)
			{
				count++;
			}
	}
		free(pi);
		return count;

}
void file_analyzer(char* dir, char* sub)
{
	short i = 0;
	char* c;
	char* s = malloc(TEXTLIM * sizeof(char));
	FILE* f = fopen(dir, "r+");
	
	printf("---------------------------------------\n");
	printf("Otkrityy fail: %s\n", dir);
	while (!feof(f))
	{
		c = fgets(s, S_MAX, f);
		i++;	
		printf("\t\t--Vse sovpadeniya v stroke %d: %d\n", i, KMP_matcher(s, sub));		
	}
    printf("\n---------------------------------------\n");

	fclose(f);
	free(s);
}

void search(char* file[], char* dir, char key, char* sub)
{
    char path[TEXTLIM];
    // printf("Poisk v kataloge: %s\n", dir);
    struct dirent *fi;
    if(strcmp(dir, "/")) strcat(dir, "/");
    DIR * di = opendir(dir);
    struct stat stt;
    if(!di)
	{
        printf("Ошибка открытия %s\n", dir);
        return;
    }
    while ((fi = readdir(di))) {
         if (!strcmp(fi->d_name, ".") || !strcmp(fi->d_name, "..")) continue;
         strcpy(path, dir);
         strcat(path, fi->d_name);
         lstat(path, &stt);

		 if(S_ISREG(stt.st_mode) && (Kmp_for_filename(fi->d_name, file[1]) == 1))
			 if ((Kmp_for_filename(fi->d_name, file[0]) > 0 ) || (!strcmp(file[0], "*")))
			 	 file_analyzer(path, sub);
		 if(key == 'r' && S_ISDIR(stt.st_mode))
             search(file, path, 'r', sub); 
    }
    closedir(di);
}

char** ne_scissors(char* file)
{
	char* name = malloc(TEXTLIM * sizeof(char));
	char* ext = malloc(TEXTLIM * sizeof(char));
	char** cutted = malloc(2 * sizeof(char));

	short i, count;
	for (i = 0; file[i] != '.'; i++)
    {      
        name[i] = file[i];
        count++;
    }
    for (i = 0; file[i+count-1] != '\0'; i++)
    {
        ext[i] = file[i+count];
    }

    cutted[0] = name;
    cutted[1] = ext;
	// printf("\n%s\n", cutted[0]);
    // printf("\n%s\n", cutted[1]);
	return cutted;
}