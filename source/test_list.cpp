#include <stdio.h>

#include "list.h"

int main()
{
    List lst = {};
    listCtor(&lst, 8);

    LIST_DUMP(&lst);

    printf("%d\n", listAdd(&lst, 0, 3));
    listAdd(&lst, 1, 6);
    LIST_DUMP(&lst);
    listAdd(&lst, 2, 7);
    LIST_DUMP(&lst);

    listDel(&lst, 1);

    LIST_DUMP(&lst);

    listDtor(&lst);
    LIST_DUMP(&lst);
}
