#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "list.h"

#define LIST_VERIFY            \
    if (listError(list))       \
    {                          \
        LIST_DUMP(list);       \
        return listError(list);\
    }

int listCtor(List *list, int capacity)
{
    assert(list);

    list->free = 1;

    list->capacity = capacity;
    list->size     = 0;

    list->data = (elem_t *)calloc(capacity + 1, sizeof(elem_t));
    if (!list->data) { perror("data->next:"); return LIST_MEMORY_ERROR; }

    list->next = (elem_t *)calloc(capacity + 1, sizeof(elem_t));
    if (!list->next) { perror("list->next:"); return LIST_MEMORY_ERROR; }

    list->prev = (elem_t *)calloc(capacity + 1, sizeof(elem_t));
    if (!list->prev) { perror("list->prev:"); return LIST_MEMORY_ERROR; }

    list->data[0] = Poison;
    list->next[0] = 0;   //head
    list->prev[0] = 0;   //tail

    for (int i = 1; i <= capacity; i++)
    {
        list->next[i] = - i - 1;
        list->prev[i] =     - 1;
    }

    return EXIT_SUCCESS;
}

int listDtor(List *list)
{
    assert(list);

    list->free = -1;

    list->capacity = 0;
    list->size     = -1;

    if (list->data)
    {
        free(list->data);
        list->data = 0;
    }

    if (list->next)
    {
        free(list->next);
        list->next = 0;
    }

    if (list->prev)
    {
        free(list->prev);
        list->prev = 0;
    }

    return EXIT_SUCCESS;
}

int listError(List *list)
{
    assert(list);

    if (!list->data) return LIST_DATA_NULL;
    if (!list->next) return LIST_NEXT_NULL;
    if (!list->prev) return LIST_PREV_NULL;

    if (list->capacity < list->size)     return LIST_OVERFLOW;

    if (list->next[0] > list->capacity) return LIST_BAD_VARIABLES;
    if (list->prev[0] > list->capacity) return LIST_BAD_VARIABLES;
    if (list->free    > list->capacity) return LIST_BAD_VARIABLES;

    int current = list->next[0];
    while (list->next[current] > 0)
    {
        elem_t nextElem = list->next[current];
        if (current != list->prev[nextElem]) return LIST_BAD_POINTERS;

        current = nextElem;
    }

    if (current != list->prev[0]) return LIST_BAD_POINTERS;

    return EXIT_SUCCESS;
}

int listDump(List *list, const char *file, int line, const char *function)
{
    assert(list);

    printf("I'm listDump() called from %s %s(%d)\n", function, file, line);

    printf(" capacity = %d\n", list->capacity);
    printf(" size     = %d\n", list->size);

    printf("  head    = %d\n", list->next[0]);
    printf("  tail    = %d\n", list->prev[0]);
    printf("  free    = %d\n", list->free);

    printf("Arrays:\n");
    printf("index  data  next  prev:\n");

    for (int i = 0; i < list->capacity; i++)
    {
        if      (i == 0)             putchar('-');
        else if (list->prev[i] >= 0) putchar('*');
        else                         putchar(' ');

        printf("[%2d] " elemFormat " %5d %5d\n", i, list->data[i], list->next[i], list->prev[i]);
    }
    printf("\n\n");

    for (int cur = list->next[0]; cur != 0; cur = list->next[cur])
    {
        printf(elemFormat "[%d] -> ", list->data[cur], cur);
    }
    printf("\n\n");

    return EXIT_SUCCESS;
}

int listAdd(List *list, int arrayAnchorIndex, elem_t value)
{
    LIST_VERIFY;

    if (list->size == list->capacity)       return LIST_OVERFLOW;

    if (arrayAnchorIndex > list->capacity)  return LIST_INVALID_INDEX;
    if (list->prev[arrayAnchorIndex] == -1) return LIST_INVALID_INDEX;

    int index  =  list->free;
    list->free = -list->next[index];

    list->data[index] = value;

    list->next[index]     = list->next[arrayAnchorIndex];
    int nextIndex         = list->next[index];
    list->prev[nextIndex] = index; 

    list->prev[index]            = arrayAnchorIndex;
    list->next[arrayAnchorIndex] = index;

    list->size++;

    LIST_VERIFY;
    return EXIT_SUCCESS;
}

int listDel(List *list, int arrayElemIndex)
{
    LIST_VERIFY;

    if (arrayElemIndex >= list->size)     return LIST_INVALID_INDEX;
    if (list->prev[arrayElemIndex] == -1) return LIST_INVALID_INDEX;

    int indexPrev = list->prev[arrayElemIndex];
    int indexNext = list->next[arrayElemIndex];

    list->next[indexPrev] = indexNext;
    list->prev[indexNext] = indexPrev;

    list->data[arrayElemIndex] = Poison;
    list->prev[arrayElemIndex] = -1;
    list->next[arrayElemIndex] = -list->free;
    list->free                 = arrayElemIndex;

    list->size--;

    LIST_VERIFY;
    return EXIT_SUCCESS;
}