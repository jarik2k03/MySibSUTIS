#include <stdio.h>
#include <stdlib.h>
#include "vector.h"

IntVector *int_vector_new(size_t initial_capacity){
    IntVector *newvector = malloc(sizeof(IntVector));
    if (newvector == NULL) return NULL;
    newvector->pointer = malloc(initial_capacity * sizeof(int));
    if (newvector->pointer == NULL){
        free(newvector);
        return NULL;
    }
    newvector->size = 0;
    newvector->capacity = initial_capacity;
    return newvector;
}

IntVector *int_vector_copy(const IntVector *v){
    IntVector *copyV = malloc(sizeof(IntVector));
    if (copyV == NULL){
        return NULL;
    }
    copyV->pointer = malloc(v->capacity * sizeof(IntVector));
    if (copyV == NULL){
        free(copyV);
        return NULL;
    }
    for (int x=0; x<v->capacity; x++) copyV->pointer[x] = v->pointer[x];
    copyV->capacity = v->capacity;
    copyV->size = v->size;
    return copyV;
}

void int_vector_free(IntVector *v){
    free(v->pointer);
    free(v);
}

int int_vector_get_item(const IntVector *v, size_t index){
    if (index > v->size) return -1;
    return v->pointer[index];
}

int int_vector_set_item(IntVector *v, size_t index, int item){
    if (index < v->capacity){
        v->pointer[index] = item;
        v->size++;
        return 0;
    }
    else return -1;
}

size_t int_vector_get_size(const IntVector *v){
    return v->size;
}

size_t int_vector_get_capacity(const IntVector *v){
    return v->capacity;
}

int int_vector_push_back(IntVector *v, int item){
    if (v->size < v->capacity){
        v->pointer[v->size] = item;
        v->size++;
    }
    else {
        v->capacity*=2;
        int *p = realloc(v->pointer, v->capacity*sizeof(int));
        if (p == NULL) return -1;
        v->pointer = p;
        v->pointer[v->size] = item;
        v->size++;
    }
    return 0;
}

void int_vector_pop_back(IntVector *v){
    if (v->size != 0) v->size--; 
}

int int_vector_shrink_to_fit(IntVector *v){
    if (v->size > v->capacity) return -1;
    v->capacity = v->size;
    int *p = realloc(v->pointer, v->size*sizeof(int));
    if (p == NULL) return -1;
    v->pointer = p;
    return 0;
}

int int_vector_resize(IntVector *v, size_t new_size){
    if (v->size < v->capacity){
        if (v->size < new_size){
            int *p = realloc(v->pointer, sizeof(int));
            if (p == NULL) return -1;
            v->pointer = p;
            for (int x=v->size+1; x<new_size; x++) v->pointer[x] = 0;
        }
        else if (v->size == new_size) return 0;
        else {
            int chck = int_vector_shrink_to_fit(v);
            if (chck != 0){
                return -1;
            }
        }
        v->size = new_size;
        return 0;
    }
    else return -1;
}

int int_vector_reserve(IntVector *v, size_t new_capacity){
    if (v->capacity < new_capacity){
        int *p = realloc(v->pointer, new_capacity*sizeof(int));
        if (p == NULL) return -1;
        v->pointer = p;
        v->capacity = new_capacity;
        return 0;
    }
    return -1;
}
