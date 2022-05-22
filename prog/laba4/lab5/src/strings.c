#include "strings.h"
#include <stdio.h>

int s_cmp(char* s1, char* s2)
{
    unsigned char c1, c2;

    while (1) {
        c1 = *s1++;
        c2 = *s2++;
        if (c1 != c2)
            return c1 < c2 ? -1 : 1;
        if (!c1 || !c2)
            break;
    }
    return 0;
}

int s_len(char* s)
{
    int n = 0;
    while (*(s + n) != '\0') {
        ++n;
    }
    return n;
}

int s_spn(char* s, char c)
{
    int cnt = 0;
    while (*(s + cnt) != '\0') {
        if (*(s + cnt) == c)
            return cnt;
        cnt++;
    }
    return 0;
}

char* s_cpy(char* dest, char* src)
{
    char* tmp = dest;

    while ((*dest++ = *src++) != '\0');
    return tmp;
}

char* s_ffut(char* s, char* c)
{
    char* s1;
    for (s1 = s; *s1 != '\0'; ++s1)
        if (*s1 == *c)
            return (char*)s1;
    return 0;
}
 
char* s_tok(char** s, char* del)
{
    char* firsttok = *s;
    char* endtok;
    if (firsttok == NULL)
        return NULL;
    endtok = s_ffut(firsttok, del);
    if (endtok)
        *endtok++ = '\0';
    *s = endtok;
    return firsttok;
}

char* s_cncat(char* str, char* strtobecpy, int cnt)
{
    char* tmp = str;

    if (cnt) {
        while (*str)
            str++;
        while ((*str++ = *strtobecpy++) != 0) {
            if (--cnt == 0) {
                *str = '\0';
                break;
            }
        }
    }
    return tmp;
}

void remove_spaces(char* s)
{
    char* d = s;
    do {
        while (*d == ' ') {
            ++d;
        }
    } while (*s++ == *d++);
}

void print(char* s)
{
    printf("new paths: %s\n", s);
}