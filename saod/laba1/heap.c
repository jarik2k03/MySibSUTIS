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

void Swap(int *a, int *b) {
    int temp = *a;
    *a = *b; 
    *b = temp;
}

void Heapify(int arr[], int n, int i)
{
    int largest = i;   
// Инициализируем наибольший элемент как корень
    int l = 2*i + 1; // левый = 2*i + 1
    int r = 2*i + 2; // правый = 2*i + 2

 // Если левый дочерний элемент больше корня
    if (l < n && arr[l] > arr[largest])
        largest = l;

   // Если правый дочерний элемент больше, чем самый большой элемент на данный момент
    if (r < n && arr[r] > arr[largest])
        largest = r;

    // Если самый большой элемент не корень
    if (largest != i)
    {
        Swap(&arr[i], &arr[largest]);

// Рекурсивно преобразуем в двоичную кучу затронутое поддерево
        Heapify(arr, n, largest);
    }
}

void HeapSort(int arr[], int n)
{
  // Построение кучи (перегруппируем массив)
    for (int i = n / 2 - 1; i >= 0; i--)
        Heapify(arr, n, i);

   // Один за другим извлекаем элементы из кучи
    for (int i=n-1; i>=0; i--)
    {
        // Перемещаем текущий корень в конец
        Swap(&arr[0], &arr[i]);

        // вызываем heapify на уменьшенной куче
        Heapify(arr, i, 0);
    }
}

int main()
{
    int i;


    int arr[1000000];
    int n = sizeof(arr)/sizeof(arr[0]);
    for (i = 0; i < n; i++) arr[i] = rand() % 100;   

    double t = wtime();
    HeapSort(arr, n); // sort
    printf("Sort time: %f \n", wtime() - t);
    //printf("Sorted array is\n");
    //for (i = 0; i < n; i++) printf("%d ", arr[i]);
    //printf("\n");
}