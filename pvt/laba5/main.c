#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

#define threshold 2000

void swap(int* a, int* b)
{
	int t = *a;
	*a = *b;
	*b = t;
}

void partition(int *v, uint32_t low, uint32_t high, uint32_t *left, uint32_t *right)
{
    uint32_t i = low;
    uint32_t j = high;
    int pivot = v[(i + j + 1) / 2];

    while (i <= j) {
        while (v[i] < pivot) i++;
        while (v[j] > pivot) j--;

        if (i <= j) {
            swap(&v[i], &v[j]);
            i++;
            j--;
        }
    }

    *left = i;
    *right = j;
}

void quicksort_tasks(int *v, uint32_t low, uint32_t high)
{
    uint32_t left = 0;
    uint32_t right = 0;
    partition(v, low, high, &left, &right);

    if (high - low < threshold || right - low < threshold || high - left < threshold) {
        if (low < right)
            quicksort_tasks(v, low, right);
        if (left < high)
            quicksort_tasks(v, left, high);
    } else {
        #pragma omp task
        { quicksort_tasks(v, low, right); }
        quicksort_tasks(v, left, high);
    }
}

void quicksort_serial(int *v, uint32_t low, uint32_t high)
{
    uint32_t left = 0;
    uint32_t right = 0;
    partition(v, low, high, &left, &right);

    if (low < right) { quicksort_serial(v, low, right); }
    if (left < high) { quicksort_serial(v, left, high); }
}

int main(int argc, char **argv)
{
    int size = (argc > 1 && argv[1] != NULL) ? atoi(argv[1]) : 10;
    int threads = (argc > 2 && argv[2] != NULL) ? atoi(argv[2]) : 4;
    omp_set_num_threads(threads);
    int *array = malloc(size * sizeof(int));
    double t;

    //printf("unsorted: ");
    for (int i = 0; i < size; i++) {
       array[i] = rand() % size + 1;
       //printf("%d ", array[i]);
    }

    if (threads == 1)
    {
        t = omp_get_wtime();
        quicksort_serial(array, 0, size - 1);
        t = omp_get_wtime() - t;
    } else {
        t = omp_get_wtime();
        #pragma omp parallel
        {
            #pragma omp single
            quicksort_tasks(array, 0, size - 1);
        }
        t = omp_get_wtime() - t;
    }

    //printf("sorted: ");
    for (int i = 0; i < size; i++) {
       //printf("%d ", array[i]);
    }

    printf("# qsort (size = %d, threads = %d)\n", size, threads);
	printf("# Elapsed time (sec): %.5f\n", t);
    return 0;
}
