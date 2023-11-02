#include <stdio.h>

#include "list.h"

#define ASSISTED(funcCall)               \
    error = funcCall;                    \
    if (error) printf("%d\n", error);  

int main()
{
    List *lst = listCtor(2);
    int error = 0;

    LIST_DUMP(lst);
    LIST_GR_DUMP(lst);

    ASSISTED(listPushBack(lst, 3));
    LIST_DUMP(lst);
    LIST_GR_DUMP(lst);

    ASSISTED(listPushFront(lst, 6));
    LIST_DUMP(lst);
    LIST_GR_DUMP(lst);

    ASSISTED(listAddAfter(lst, 2, 7));
    LIST_DUMP(lst);
    LIST_GR_DUMP(lst);

    listDel(lst, 3);
    LIST_DUMP(lst);
    LIST_GR_DUMP(lst);

    listPushFront(lst, 18);
    LIST_DUMP(lst);
    LIST_GR_DUMP(lst);

    listPushBack(lst, 19);
    LIST_DUMP(lst);
    LIST_GR_DUMP(lst);

    listAddBefore(lst, 4, 23);
    LIST_DUMP(lst);
    LIST_GR_DUMP(lst);

    listDel(lst, 3);
    LIST_DUMP(lst);
    LIST_GR_DUMP(lst);

    listDtor(&lst);
    printf("list after destruction = %p\n", lst);
}
