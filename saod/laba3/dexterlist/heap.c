#include "heap.h"
#include <stdlib.h>
#include <stdio.h>

heap *heap_create(int maxsize)
{
	heap *h;

	h = malloc(sizeof(*h));

	if (h != NULL) {
		h->maxsize = maxsize;
		h->nnodes = 0;
		h->nodes = (heapnode*) malloc(sizeof(*h->nodes) * (maxsize + 1));
		if (h->nodes == NULL) {
			free(h);
			return NULL;
		}
	}
	return h;
}


void heap_free(heap *h)
{
	free(h->nodes);
	free(h);
}


void heap_swap(heapnode *l, heapnode *y)
{
	heapnode temp = *l;
	*l = *y;
	*y = temp;
}


heapnode *heap_min(heap *h)
{
	if (h->nnodes == 0) {
		return NULL;
	}

	return &h->nodes[1];
}


int heap_insert(heap *h, int key, int value)
{
	if (h->nnodes >= h->maxsize) {
		return -1;
	}

	h->nnodes++;
	h->nodes[h->nnodes].key = key;
	h->nodes[h->nnodes].value = value;

	for (int i = h->nnodes; i > 1 && h->nodes[i].key < h->nodes[i / 2].key; i = i / 2) {
		heap_swap(&h->nodes[i], &h->nodes[i / 2]);
	}
	return 0;
}


heapnode heap_extract_min(heap *h)
{
	if (h->nnodes == 0) {
		return (heapnode){0, 0};
	}

	heapnode minnode = h->nodes[1]; //извлекаем минимальное число, оно стоит на месте 1.
	h->nodes[1] = h->nodes[h->nnodes]; // На место 1 ставим последнее, теперь число на месте 1 наибольшее
	h->nnodes--;
	heap_heapify(h, 1);

	return minnode;
}


void heap_heapify(heap *h, int index)
{
	while (1){
		int left = 2 * index;
		int right = 2 * index + 1;
		int least = index;
		
		//ищем наименьший ключ и левого правого и нанешнего
		if (left <= h->nnodes && h->nodes[left].key < h->nodes[least].key) {
			least = left;
		}
		if (right <= h->nnodes && h-> nodes[right].key < h->nodes[least].key) {
			least = right;
		}
		if (least == index) {
			break;
		}
		heap_swap(&h->nodes[index], &h->nodes[least]);
		index = least;
	}
}


int heap_decrease_key(heap *h, int index, int key)
{
	if (h->nodes[index].key < key) {
		return -1;
	}
	h->nodes[index].key = key;
	while (index > 1 && h->nodes[index].key < h->nodes[index / 2].key) {
		heap_swap(&h->nodes[index], &h->nodes[index / 2]);
		index = index / 2;
	}
	return index;
}
