#include "strings.h"
#include <stdio.h>
#define MAX_STR 260

char symbols[] = "/\\:*?«<>|";

void delinput(char d)
{
    scanf("%c", &d);
}

void input(char s[MAX_STR])
{
    fgets(s, MAX_STR, stdin);
}

int allinput(char paths[MAX_STR])
{
    printf("paths: ");
    input(paths);
    int pathslen = s_len(paths);
    if (pathslen == 1)
    {
        printf("Paths length == 1!\n");
         return 1;
    }
    else
    {
        return 0;
    }
        
    
}

int correctcheck(char* paths)
{
    int f = 0; // len - 9
    
    for (int x = 3; x < 10; x++)
    {
        f = s_spn(paths, symbols[x]);
        if (f > 0)
            break;
    }
    if (f > 0)
    {
        printf("Problems with symbol N %d in paths\n", f++);
        return 1;
    }
    return 0;
}

void process(char s[MAX_STR], char d)
{
    char* tok;
    int y;
    char out[MAX_STR] = {' '};
    char proctok[MAX_STR] = {' '};
    char finalout[MAX_STR] = {' '};
    char cygwin[] = {"/cygdrive/"};

    while (s != NULL) {
        y = 0;
        s_cpy(out, cygwin);
        tok = s_tok(&s, &d);
        remove_spaces(tok);
        if ((*tok >= 'A' && *tok <= 'Z') && *(tok + 1) == ':' && *(tok + 2) == '\\') 
        {
            while (*(tok + 1 + y) != '\0') {
                proctok[y] = *(tok + 1 + y);

                if (proctok[y] == ':')
                    proctok[y] = (*tok) + ' ';
                if (proctok[y] == '\\')
                    proctok[y] = '/';
                y++;
            }
            s_cncat(out, proctok, s_len(proctok));
            s_cncat(finalout, out, s_len(out));
        }
        else if ((*tok >= 'A' && *tok <= 'Z') && (*(tok + 1) >= 'A' && *(tok + 1) <= 'Z') && *(tok + 2) == ':' && *(tok + 3) == '\\')
        {
            while (*(tok + 1 + y) != '\0') {
                proctok[y] = *(tok + 1 + y);

                if (proctok[y] == ':') {
                    proctok[y-1] = (*tok) + ' ';
                    proctok[y] = (*(tok + 1)) + ' ';
                }
                    
                if (proctok[y] == '\\')
                    proctok[y] = '/';
                y++;
            }
            s_cncat(out, proctok, s_len(proctok));
            s_cncat(finalout, out, s_len(out));
        }
        else
        {
            s_cncat(finalout, tok, s_len(tok));
        }
        if (s != NULL) {
            finalout[s_len(finalout)] = d;
        }
    }
    print(finalout);
}

