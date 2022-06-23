#include "coder.h"
#include "command.h" 
#include <inttypes.h> 
#include <stdlib.h>
#include <stdio.h>

int encode_file(const char* in_file_name, const char* out_file_name)
{
    FILE* in = fopen(in_file_name, "r"); 
    if (in == NULL) return -1;
    FILE* out = fopen(out_file_name, "wb"); 
    if (out == NULL) 
    {
        fclose(in); 
        return -1;
    }
    CodeUnits* c = malloc(sizeof(CodeUnits)); 
    uint32_t n;
    while (feof(in) == 0) 
    {
        size_t k = fscanf(in, "%" SCNx32, &n); 
        int t = encode(n, c);
        int y = write_code_unit(out, c); 
        if (t != 0 || y != 0 || k != 1) 
        {
            free(c); 
            fclose(in); 
            fclose(out);
            return -1;
        }
    }

    fclose(in); 
    fclose(out); 
    free(c); 
    return 0;
}

int decode_file(const char* in_file_name, const char* out_file_name)
{
    FILE* in = fopen(in_file_name, "rb"); 
    if (in == NULL) return -1;
    FILE* out = fopen(out_file_name, "w"); 
    if (out == NULL) 
    {
        fclose(in); 
        return -1;
    }
    CodeUnits* c = malloc(sizeof(CodeUnits)); 
    while (read_next_code_unit(in, c) != -1) 
    {
        uint32_t dec = decode(c);
        size_t t = fprintf(out, "%" PRIx32, dec);
        fprintf(out, "\n"); 
        if (t == 0) 
        {
            free(c); 
            return -1;
        }   
    }
    free(c); 
    return 0;
}
