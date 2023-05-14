#include <stdio.h>
#include <stdlib.h>
#include "vector.h"

int main()
{
    
    IntVector *v = int_vector_new(5);

    IntVector *cV = int_vector_copy(v);

    int_vector_free(v);

    for (int x=0; x<cV->capacity; x++) int_vector_set_item(cV, x, x);

    int size = int_vector_get_size(cV);

    printf("Array\n");
    for (int i=0; i<size; i++){
        int item = int_vector_get_item(cV, i);
        printf("%d ", item);
    }
    printf("\n");
    printf("\n");
    
    int_vector_push_back(cV, 5);
    printf("Pushback\n");
    for (int j=0; j<cV->size; j++){
        int item = int_vector_get_item(cV, j);
        printf("%d ", item);
    }
    printf("\n");
    int capacity = int_vector_get_capacity(cV);
    printf("Capacity - %d\n", capacity);
    size = int_vector_get_size(cV);
    printf("Size - %d\n", size);
    printf("\n");

    int_vector_pop_back(cV);
    printf("Popback\n");
    for (int j=0; j<cV->size; j++){
        int item = int_vector_get_item(cV, j);
        printf("%d ", item);
    }
    printf("\n");

    int_vector_resize(cV, 8);
    size = int_vector_get_size(cV);
    capacity = int_vector_get_capacity(cV);
    printf("\n");
    printf("Resize\n");
    for (int j=0; j<cV->size; j++){
        int item = int_vector_get_item(cV, j);
        printf("%d ", item);
    }
    printf("\n");
    printf("size - %d\n", size);
    printf("capacity - %d\n", capacity);

    int_vector_shrink_to_fit(cV);
    capacity = int_vector_get_capacity(cV);
    printf("\n");
    printf("ShrinkToFit\ncapacity - %d\n", capacity);

    int_vector_reserve(cV, 10);
    capacity = int_vector_get_capacity(cV);
    printf("\n");
    printf("Pushback\ncapacity - %d\n", capacity);
    

    return 0;
}