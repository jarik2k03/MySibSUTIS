#include "func.h"
#include "strings.h"
#include <stdio.h>
#define MAX_STR 260

int main()
{
    int correctLenghtData, correctSymFlag;
    char paths[MAX_STR];
    char* pathsc = (char*)paths;
    char d;

    printf("del: ");
    do {
        scanf("%c", &d);
    } while (getchar() != '\n');
    correctLenghtData = allinput(paths);
    if (correctLenghtData)
        return -1;
    correctSymFlag = correctcheck(paths);
    if (correctSymFlag)
        return -1;
    process(pathsc, d);

    return 0;
}
