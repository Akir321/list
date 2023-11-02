#ifndef  __LIST_H__
#define  __LIST_H__

#include <stddef.h>

typedef int elem_t;
#define elemFormat "%5d"

const int Poison          = 0;
const int ListReallocRate = 2;

#define LIST_DUMP(list) listDump(list, __FILE__,  __LINE__, __func__)

struct List;

enum listErrors
{
    LIST_MEMORY_ERROR   = -1,
    LIST_DATA_NULL      = -2,
    LIST_NEXT_NULL      = -3,
    LIST_PREV_NULL      = -4,
    LIST_OVERFLOW       = -5,
    LIST_BAD_VARIABLES  = -6,
    LIST_BAD_POINTERS   = -7,
    LIST_INVALID_INDEX  = -8
};

List *listCtor(int capacity);

int listDtor (List **listPtr);
int listError(List *list);
int listDump (List *list, const char *file, int line, const char *function);

int listCapacity(List *list);
int listSize    (List *list);
int listFree    (List *list);

int listHeadIndex   (List *list);
int listTailIndex   (List *list);
int listNextIndex   (List *list, int arrayIndex);
int listPrevIndex   (List *list, int arrayIndex);
int listValueByIndex(List *list, int arrayIndex);

int listPushFront(List *list, elem_t value);
int listPushBack (List *list, elem_t value);
int listAddAfter (List *list, int arrayAnchorIndex, elem_t value);
int listAddBefore(List *list, int arrayAnchorIndex, elem_t value);
int listDel      (List *list, int arrElemIndex);

int listReallocUp(List *list, int reallocRate);

#include "graphic_dump.h"

#ifdef  __GRAPHIC_DUMP_H__
#define LIST_GR_DUMP(list) listGraphicDump(list)

#else
#define LIST_GR_DUMP(list)

#endif //__GRAPHIC_DUMP_H__

#endif //__LIST_H__
