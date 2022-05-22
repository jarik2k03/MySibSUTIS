#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <string.h>
double wtime()
{
        struct timeval t;
        gettimeofday(&t, NULL);
        return (double)t.tv_sec + (double)t.tv_usec * 1E-6;

}
void CountingSort(int array[], int n)
{
        int output[100000];
        int max = array[0];        
        int count[100000];

    double t;
	t = wtime();
        for (int i = 1; i < n; i++)
        {
	    if (array[i] > max)     max = array[i];
        }
  // The size of count must be at least (max+1) but
  // we cannot declare it as int count(max+1) in C as
  // it does not support dynamic memory allocation.
  // So, its size is provided statically.

  // Initialize count array with all zeros.
        for (int i = 0; i <= max; ++i) {
                count[i] = 0;
        }
  // Store the count of each element
        for (int i = 0; i < n; i++) {
                count[array[i]]++;
        }
  // Store the cummulative count of each array
        for (int i = 1; i <= max; i++) {
                count[i] += count[i - 1];
        }
  // Find the index of each element of the original array in count array, and
  // place the elements in output array
        for (int i = n - 1; i >= 0; i--) {
                output[count[array[i]] - 1] = array[i];
                count[array[i]]--;
        }
  // Copy the sorted elements into original array
        for (int i = 0; i < n; i++) {
                array[i] = output[i];
        }
    printf("Time alg: %f\n", wtime() - t);

}



int main ()
{
    int array[100000];
    int n = sizeof(array) / sizeof(array[0]);
	int a;
	
	for (a = 0; a < n; a++)
	{
		array[a] = rand() % 100;
	}
 

        CountingSort(array, n);
        

        // for (a = 0; a < n; a++)
        // {
        //     printf("%d ", array[a]);
        // }
}

