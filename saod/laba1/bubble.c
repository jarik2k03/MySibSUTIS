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


void BubbleSort(int *arr, int n)
{
  // Для всех элементов
  for (int i = 0; i < n - 1; i++)
  {
    for (int j = n - 1; j > i; j--) // для всех элементов после i-ого
    {
      if (arr[j - 1] > arr[j]) // если текущий элемент меньше предыдущего
      {
        int temp = arr[j - 1]; // меняем их местами
        arr[j - 1] = arr[j];
        arr[j] = temp;
      }
    }
  }
}


int main()
{
    int i;


    int arr[100000];
    int n = sizeof(arr)/sizeof(arr[0]);
    for (i = 0; i < n; i++) arr[i] = rand() % 100;   

    double t = wtime();
    BubbleSort(arr, n); // sort
    printf("Sort time: %f \n", wtime() - t);
    //printf("Sorted array is\n");
    //for (i = 0; i < n; i++) printf("%d ", arr[i]);
    //printf("\n");
}