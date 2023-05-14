#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <time.h>

enum {N = 100000000};

double wtime()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec * 1E-9;
}

int vec_sum(int *vec, int n)
{
    int s = 0;
    for (int i = 0; i < n; i++) {
        s += vec[i];
    }
    return s;
}

int vec_sum_v2(int *vec, int n)
{
    unsigned int i;
    int s = 0;

    for (i = 0; i + 3 < n; i += 4) {
        s = s + vec[i];
        s = s + vec[i + 1];
        s = s + vec[i + 2];
        s = s + vec[i + 3];
    }

    for (; i < n; i++) {
        s += vec[i];
    }
    return s;
}

int vec_sum_v3(int *vec, int n)
{
    int s = 0, s1 = 0, s2 = 0, s3 = 0;
    // TODO: remove data dependency in unrolled loop
    unsigned int i;

    for (i = 0; i + 3 < n; i += 4) {
        s = s + vec[i];
        s1 = s1 + vec[i + 1];
        s2 = s2 + vec[i + 2];
        s3 = s3 + vec[i + 3];
    }
    s1 = s1 + s2 + s3;
    for (; i < n; i++) {
        s += vec[i];
    }
    return (s + s1);
}

void run(int (*fn)(int *, int))
{
    int *vec = malloc(sizeof(*vec) * N);
    for (int i = 0; i < N; i++) {
        vec[i] = 1;
    }

    double t = wtime();
    int s = fn(vec, N);
    t = wtime() - t;

    printf("Time %.6f, sum %d\n", t, s);

    free(vec);
}

int main()
{
    run(vec_sum);
    run(vec_sum_v2);
    run(vec_sum_v3);

    return 0;
}
