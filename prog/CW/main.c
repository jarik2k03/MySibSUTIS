#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kmp.h"
#define TEXTLIM 180

int main(int argc, char* argv[])
{
    short count = 0, i = 0;

    char** filecutted = malloc(2 * sizeof(char));


    if (argc == 3)
    {
        filecutted = ne_scissors(argv[1]);
        char* sub = malloc(TEXTLIM * sizeof(char));
        printf("Input symbol or word for search:\n");
        scanf("%s", sub);
        search(filecutted ,argv[2], 'n', sub);
        free(filecutted);
    }
    else if (argc == 4 && !strcmp(argv[1], "-r"))
    {
        filecutted = ne_scissors(argv[2]);
        char* sub = malloc(TEXTLIM * sizeof(char));
        printf("Input symbol or word for search:\n");
        scanf("%s", sub);
        search(filecutted, argv[3], 'r', sub);
        free(filecutted);

    }
    else
    {
        printf("Launch in one folder: \n");
        printf("$ ./kmpmatch <*.files> <rel_dir> <search_word>\n");
        printf("Launch in folder and below: \n");
        printf("$ ./kmpmatch -r <*.files> <rel_dir> <search_word>\n");
    }
}