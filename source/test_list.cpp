#include <stdio.h>

#include "list.h"

#define ASSISTED(funcCall)               \
    error = funcCall;                    \
    if (error) printf("%d\n", error);  

int main()
{
    List lst = {};
    listCtor(&lst, 2);
    int error = 0;

    LIST_DUMP(&lst);

    ASSISTED(listAdd(&lst, 0, 3));
    LIST_DUMP(&lst);
    ASSISTED(listAdd(&lst, 1, 6));
    LIST_DUMP(&lst);
    ASSISTED(listAdd(&lst, 2, 7));
    LIST_DUMP(&lst);

    listDel(&lst, 1);

    LIST_DUMP(&lst);

    listDtor(&lst);
    LIST_DUMP(&lst);
}
