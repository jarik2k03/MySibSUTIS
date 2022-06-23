#include <stdint.h> 
#include <stdio.h>
#include "coder.h"
int encode(uint32_t code_point, CodeUnits* code_units)
{
    uint32_t r = code_point, count = 0; 
    while (r > 0) 
    {
        r = r >> 1;
        count++;
    }
    if (count < 8) 
    { 
        code_units->length = 1;
        code_units->code[0] = code_point;
    }
    if (count > 8 && count <= 11) 
    { 
        code_units->length = 2;
        code_units->code[0] = code_units->code[0] | 192; // 11000000 
        code_units->code[1] = code_point & 63;	// 00xxxxxx 
        code_units->code[1] = code_units->code[1] | 128; // 10xxxxxx 
        code_point = code_point >> 6;
        code_units->code[0] = code_units->code[0] | code_point;
    }
    if (count > 11 && count <= 16) 
    { 
        code_units->length = 3;
        code_units->code[0] = code_units->code[0] | 224; // 11100000 
        for (int i = code_units->length - 1; i >= 1; i--) 
        {
            code_units->code[i] = code_point & 63;	// 00xxxxxx
            code_units->code[i] = code_units->code[i] | 128; // 10xxxxxx
            code_point = code_point >> 6;
        }
        code_units->code[0] = code_units->code[0] | code_point;
    }
    if (count > 16 && count <= 21) 
    { 
        code_units->length = 4;
        code_units->code[0] = code_units->code[0] | 240; // 11110000 
        for (int i = code_units->length - 1; i >= 1; i--) 
        {
            code_units->code[i] = code_point & 63;	// 00xxxxxx
            code_units->code[i] = code_units->code[i] | 128; // 10xxxxxx
            code_point = code_point >> 6;
        }
        code_units->code[0] = code_units->code[0] | code_point;
    }
    if (count > 21) return -1;
    return 0;
}

uint32_t decode(const CodeUnits* code_unit)
{
    uint32_t dec = 0;
    if (code_unit->length == 1) 
        dec = code_unit->code[0];
    if (code_unit->length == 2) 
    {
        dec = code_unit->code[0] & 31; // 00011111
        dec = dec << 6;
        dec = dec | (63 & code_unit->code[1]);
    }
    if (code_unit->length == 3) 
    {
        dec = code_unit->code[0] & 15; // 00001111
        for (size_t i = 1; i < code_unit->length; i++) 
        { 
            dec = dec << 6;
            dec = dec | (63 & code_unit->code[i]);
        }
    }
    if (code_unit->length == 4) 
    {
        dec = code_unit->code[0] & 7; // 00000111
        for (size_t i = 1; i < code_unit->length; i++) 
        { 
            dec = dec << 6;
            dec = dec | (63 & code_unit->code[i]);
        }
    }
    return dec;
}

int write_code_unit(FILE* out, const CodeUnits* code_unit)
{
    for (size_t i = code_unit->length; i > 0; i--) 
    { 
        size_t count = fwrite(&code_unit->code[code_unit->length - i], 1, sizeof(uint8_t), out);
        if (count != sizeof(uint8_t)) return -1;
    }
    return 0;
}

int read_next_code_unit(FILE* in, CodeUnits* code_units)
{
    code_units->length = 0;
    size_t t = fread(&code_units->code[0], sizeof(uint8_t), 1, in); 
    if (t != 1) return -1;
    if (code_units->code[0] < 128) 
    {
        code_units->length = 1;
    }
    else 
    {
        if (code_units->code[0] < 192)
            return read_next_code_unit(in, code_units);
        if (code_units->code[0] < 248) // 11110xxx
            code_units->length = 4;
        if (code_units->code[0] < 240) // 1110xxxx 
            code_units->length = 3;
        if (code_units->code[0] < 224) // 110xxxxx 
            code_units->length = 2;
        for (size_t i = 1; i < code_units->length; i++) 
        {
            t = fread(&code_units->code[i], sizeof(uint8_t), 1, in); 
            if (t != 1) return -1;
            if (code_units->code[i] >> 6 != 2) return read_next_code_unit(in, code_units);
        }
    }
    if (code_units->length == 0) return read_next_code_unit(in, code_units); 
    return 0;
}
