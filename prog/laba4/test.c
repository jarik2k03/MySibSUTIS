#include <stdio.h>

int sequal(const char *s1, const char* s2)
{
    for (int i = 0; s2[i] != '\0' || s1[i] != '\0'; i++)
    {
        if (s1[i] != s2[i]) return 1;   
    }
    return 0;
}

int s_spn(char* s1, char* s2)
{
    int cnt = 0;
    int i, j;
    for (i = 0; s1[i] != '\0'; i++)
    {
        for (j = 0; s2[j] != '\0'; j++)
        {
            
            if (s1[i] != s2[j]) return cnt;
            cnt++;
        }
    }
    return 0;
}

int main()
{
    char s1[] = "123";
    char s2[] = {'1', '2', '3'};
    if (!s_spn(s1, s2)) printf("returned 0\n");
    printf("count %d\n", s_spn(s1, s2));

}
