#ifndef  __LIST_H__
#define  __LIST_H__

#include <stddef.h>

typedef int elem_t;
#define elemFormat "%5d"

const int Poison          = 0;
const int ListReallocRate = 2;

#define LIST_DUMP(list) listDump(list, __FILE__,  __LINE__, __func__)

struct List
{
    int size;
    int capacity;

    int free;

    elem_t *data;
    int    *next;
    int    *prev;
};

enum listErrors
{
    LIST_MEMORY_ERROR   = 1,
    LIST_DATA_NULL      = 2,
    LIST_NEXT_NULL      = 3,
    LIST_PREV_NULL      = 4,
    LIST_OVERFLOW       = 5,
    LIST_BAD_VARIABLES  = 6,
    LIST_BAD_POINTERS   = 7,
    LIST_INVALID_INDEX  = 8
};

int listCtor (List *list, int capacity);
int listDtor (List *list);
int listError(List *list);
int listDump (List *list, const char *file, int line, const char *function);

int listAdd  (List *list, int arrayAnchorIndex, elem_t value);
int listDel  (List *list, int arrElemIndex);

int listReallocUp(List *list, int reallocRate);


#endif //__LIST_H__
