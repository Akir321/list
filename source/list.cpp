#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "list.h"
#include "html_logfile.h"

struct List
{
    int size;
    int capacity;

    int free;

    elem_t *data;
    int    *next;
    int    *prev;
};

#define LIST_VERIFY            \
    if (listError(list))       \
    {                          \
        LIST_DUMP(list);       \
        return listError(list);\
    }

List *listCtor(int capacity)
{
    List *list = (List *)calloc(1, sizeof(List));
    if (!list) return NULL;

    list->free = 1;

    list->capacity = capacity;
    list->size     = 0;

    list->data = (elem_t *)calloc(capacity + 1, sizeof(elem_t));
    if (!list->data) { perror("data->next:"); return NULL; }

    list->next = (elem_t *)calloc(capacity + 1, sizeof(elem_t));
    if (!list->next) { perror("list->next:"); return NULL; }

    list->prev = (elem_t *)calloc(capacity + 1, sizeof(elem_t));
    if (!list->prev) { perror("list->prev:"); return NULL; }

    list->data[0] = Poison;
    list->next[0] = 0;   //head
    list->prev[0] = 0;   //tail

    for (int i = 1; i <= capacity; i++)
    {
        list->next[i] = - i - 1;
        list->prev[i] =     - 1;
    }

    return list;
}

int listDtor(List **listPtr)
{
    assert(listPtr);
    assert(*listPtr);

    List *list = *listPtr;

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

    free(list);
    *listPtr = NULL;

    return EXIT_SUCCESS;
}

int listError(List *list)
{
    assert(list);

    if (!list->data) return LIST_DATA_NULL;
    if (!list->next) return LIST_NEXT_NULL;
    if (!list->prev) return LIST_PREV_NULL;

    if (list->capacity < list->size)    return LIST_OVERFLOW;

    if (list->next[0] > list->capacity) return LIST_BAD_VARIABLES;
    if (list->prev[0] > list->capacity) return LIST_BAD_VARIABLES;
    //if (list->free  > list->capacity) return LIST_BAD_VARIABLES;

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

    LOG("I'm listDump() called from %s %s(%d)\n", function, file, line);

    LOG(" capacity = %d\n", list->capacity);
    LOG(" size     = %d\n", list->size);

    LOG("  head    = %d\n", list->next[0]);
    LOG("  tail    = %d\n", list->prev[0]);
    LOG("  free    = %d\n", list->free);

    LOG("Arrays:\n");
    LOG("index  data  next  prev:\n");

    for (int i = 0; i <= list->capacity; i++)
    {
        if      (i == 0)             LOG("-");
        else if (list->prev[i] >= 0) LOG("*");
        else                         LOG(" ");

        LOG("[%2d] " elemFormat " %5d %5d\n", i, list->data[i], list->next[i], list->prev[i]);
    }
    LOG("\n\n");

    return EXIT_SUCCESS;
}

int listHeadIndex(List *list)
{
    LIST_VERIFY;

    return list->next[0];
}

int listTailIndex(List *list)
{
    LIST_VERIFY;

    return list->prev[0];
}

int listNextIndex(List *list, int arrayIndex)
{
    LIST_VERIFY;

    return list->next[arrayIndex];
}

int listPrevIndex(List *list, int arrayIndex)
{
    LIST_VERIFY;

    return list->prev[arrayIndex];
}

int listValueByIndex(List *list, int arrayIndex)
{
    LIST_VERIFY;

    return list->data[arrayIndex];
}

int listCapacity(List *list)
{
    LIST_VERIFY;

    return list->capacity;
}

int listSize(List *list)
{
    LIST_VERIFY;

    return list->size;
}

int listFree(List *list)
{
    LIST_VERIFY;

    return list->free;
}

int listAddAfter(List *list, int arrayAnchorIndex, elem_t value)
{
    LIST_VERIFY;

    if (list->size >= list->capacity)
    {
        int error = listReallocUp(list, ListReallocRate);
        if (error) return error;
    }
    LIST_VERIFY;

    if (arrayAnchorIndex > list->capacity) return LIST_INVALID_INDEX;
    if (list->prev[arrayAnchorIndex] < 0)  return LIST_INVALID_INDEX;

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
    return index;
}

int listAddBefore(List *list, int arrayAnchorIndex, elem_t value)
{
    return listAddAfter(list, listPrevIndex(list, arrayAnchorIndex), value);
}

int listPushFront(List *list, elem_t value)
{
    return listAddAfter(list, 0, value);
}

int listPushBack(List *list, elem_t value)
{
    return listAddBefore(list, 0, value);
}

int listDel(List *list, int arrayElemIndex)
{
    LIST_VERIFY;

    if (arrayElemIndex <= 0)              return LIST_INVALID_INDEX;
    if (arrayElemIndex >= list->capacity) return LIST_INVALID_INDEX;
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

int listReallocUp(List *list, int reallocRate)
{
    LIST_VERIFY;

    int prevCapacity = list->capacity;
    list->capacity  *= reallocRate;

    list->data = (elem_t *)realloc(list->data, (list->capacity + 1) * sizeof(elem_t));
    if (!list->data) { perror("data->next:"); return LIST_MEMORY_ERROR; }

    list->next = (elem_t *)realloc(list->next, (list->capacity + 1) * sizeof(elem_t));
    if (!list->next) { perror("list->next:"); return LIST_MEMORY_ERROR; }

    list->prev = (elem_t *)realloc(list->prev, (list->capacity + 1) * sizeof(elem_t));
    if (!list->prev) { perror("list->prev:"); return LIST_MEMORY_ERROR; }

    for (int i = prevCapacity + 1; i <= list->capacity; i++)
    {
        list->data[i] = Poison;
        list->next[i] = - i - 1;
        list->prev[i] =     - 1;
    }

    LIST_VERIFY;
    return EXIT_SUCCESS;
}
