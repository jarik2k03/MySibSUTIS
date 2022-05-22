#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int* data;
    size_t size;
    size_t capacity;
} IntVector;


IntVector *int_vector_new(size_t initial_capacity)
{
    IntVector* v = malloc(sizeof(IntVector));
    if (!v)
    {
        return NULL;
    }
    v->data = malloc(sizeof(int) * initial_capacity);
    if (!v->data)
    {
        return NULL;
    }
}

void int_vector_free(IntVector *v)
{
    free(v);
}

int int_vector_reserve(IntVector *v, size_t new_capacity)
{
    int* new_buffer = realloc(v->data, sizeof(int) * new_capacity);
        if(!new_buffer)
        {
            return -1;
        }
        v->data = new_buffer;
        v->capacity = new_capacity;
        return 0;
}
