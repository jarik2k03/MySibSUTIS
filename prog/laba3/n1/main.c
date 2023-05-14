#include <stdint.h> 
#include <stdio.h> 
#include <stdlib.h>
#include "indecoder.h"
#define N 1000000

int main()
{
    int lenCompr, lenUncompr, example, j; 
    FILE *fUnc, *fCompr; 
    unsigned int i;
    size_t encode; 
    unsigned char c;
    unsigned int arrUnc[N], arrCompr[N]; 

    fUnc = fopen("uncompressed.dat", "wb"); 
    for (j = 0; j < N; j++) 
    {
        i = generate_number();
        arrUnc[j] = i; 
        fwrite(&i, 8, 1, fUnc);
    }
    lenUncompr = ftell(fUnc); 
    fclose(fUnc);
    fUnc = fopen("uncompressed.dat", "rb"); 
    fCompr = fopen("compressed.dat", "wb"); 
    for (j = 0; j < N; j++) 
    {
        fread(&i, 8, 1, fUnc); 
        encode = encode_varint(i, &c);
        fwrite(&c, encode, 1, fCompr);
    }
    fclose(fCompr); 
    fclose(fUnc);
    fCompr = fopen("compressed.dat", "rb"); 
    fseek(fCompr, 0, SEEK_END);
    lenCompr = ftell(fCompr);
    unsigned char* arr = (unsigned char*) malloc(sizeof(char) * lenCompr);
    const unsigned char* t = arr; 
    fseek(fCompr, 0, SEEK_SET);
    fread(arr, lenCompr, 1, fCompr); 
    fclose(fCompr);

    for (j = 0; j < N; j++)
    {
        arrCompr[j] = decode_varint(&t);
    } 
        printf("Test endecoder:\n");
    for (i = 0; i < 10; i++) 
    { 
        example = generate_number();
        if (arrUnc[example] == arrCompr[example]) 
        { 
            printf("Uncompressed (%d) == Compressed (%d)\n", arrUnc[example], arrCompr[example]);
        }
        else
        {
            printf("\n");
            printf("Uncompressed (%d) != Compressed (%d)\n", arrUnc[example], arrCompr[example]);
        }

    }
    printf("Compressed file - %d\n", lenCompr); 
    printf("Uncompressed file - %d\n", lenUncompr); 
    printf("Compression efficiency %d\n", lenUncompr / lenCompr);
    return 0;
}
