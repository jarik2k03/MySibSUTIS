#include <stdio.h>
#include <inttypes.h> 
#include "command.h"
#include "coder.h" 
#include <string.h>

int main(int argc, char* argv[])
{
    if (argc != 4)
    {
        printf("Usage:\ncoder encode <in-file-name> <out-file- name>\n");
        printf("coder decode <in-file-name> <out-file-name>\n");
        return 0;
    } 
    else if (!strcmp(argv[1], "decode"))
    {
        decode_file(argv[2], argv[3]);
    } 
    else if (!strcmp(argv[1], "encode"))
    {
        encode_file(argv[2], argv[3]);
    }
    else 
    {
        printf("Usage:\ncoder encode <in-file-name> <out-file-name>\n");
        printf("coder decode <in-file-name> <out-file-name>\n");
        return 0;
        
    }

}

