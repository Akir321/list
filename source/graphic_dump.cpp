#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "list.h"
#include "graphic_dump.h"

const int DumpFileNameAddedLength = 16;
const int CommandSize             = 28;

int listGraphicDump(List *list)
{
    assert(list);

    static int dumpNumber = 1;
    int fileNameLength    = 0;
    char *fileName        = createDumpFileName(dumpNumber++, &fileNameLength);

    FILE* dumpFile = fopen(fileName, "w");

    writeListToDotFileArrangedIndex(list, dumpFile);
    fclose(dumpFile);

    createListGraphicDumpPng(list, fileName, fileNameLength);
    free(fileName);

    return EXIT_SUCCESS;
}

char *createDumpFileName(int fileNumber, int *fileNameLength)
{
    int numberLength = 0;
    int number = fileNumber;

    for (  ; number > 0; number /= 10, numberLength++) {}

    *fileNameLength = DumpFileNameAddedLength + numberLength;
    char *fileName  = (char *)calloc(*fileNameLength, sizeof(char));
    sprintf(fileName, "gr_dump/dump%d.dot", fileNumber);

    return fileName;
}

int createListGraphicDumpPng(List *list, char *fileName, int fileNameLength)
{
    assert(list);
    assert(fileName);

    char *command = (char *)calloc(CommandSize + fileNameLength * 2, sizeof(char));
    sprintf(command, "dot %s -T png -o %s.png", fileName, fileName);

    system(command);
    free  (command);

    return EXIT_SUCCESS;
}

int writeListToDotFileArrangedIndex(List *list, FILE *dumpFile)
{
    assert(list);
    assert(dumpFile);

    fprintf(dumpFile, "digraph\n{\n");
    fprintf(dumpFile, "rankdir = LR;\n\n");

    fprintf(dumpFile, "node [shape = Mrecord, color  = \"navy\"];\n");

    int capacity = listCapacity(list);

    fprintf(dumpFile, "\nedge [color = \"white\", weight = 10000];\n\n");
    for (int i = 1; i <= capacity; i++)
    {
        int j = i + 1;
        fprintf(dumpFile, "node%d -> node%d; node%d -> node%d;\n",
                 i, j, j, i);
    }

    int headIndex = listHeadIndex(list);
    int tailIndex = listTailIndex(list);

    if (listSize(list) > 0)
    {
        fprintf(dumpFile, "\nedge [color = \"white\", weight = 1000];\n\n");
        fprintf(dumpFile, "node01 -> node%d; node%d -> node01;\n", headIndex, headIndex);
        fprintf(dumpFile, "node02 -> node%d; node%d -> node02;\n\n", tailIndex, tailIndex);
    }

    fprintf(dumpFile, "node01 [label = \"list | head = %d\", ", headIndex);
    fprintf(dumpFile, "style = \"filled\", fillcolor = \"#afeeee\"];\n");

    fprintf(dumpFile, "node02 [label = \"list | tail = %d\", ", tailIndex);
    fprintf(dumpFile, "style = \"filled\", fillcolor = \"#afeeee\"];\n\n");

    for (int i = 1; i <= capacity; i++)
    {
        fprintf(dumpFile, 
                "node%d[label = \"%d | data = %d | next = %d | prev = %d\"];\n",
                i, i, listValueByIndex(list, i), abs(listNextIndex(list, i)), listPrevIndex(list, i));
    }
    fprintf(dumpFile, "node%d [style = \"dashed\", label = \"idx = %d\"];\n", 
            capacity + 1, capacity + 1);

    fprintf(dumpFile, "\nfree  [label = \"free = %d\", ", listFree(list));
    fprintf(dumpFile, "style = \"filled\", fillcolor = \"#33ff66\"];\n");
    fprintf(dumpFile, "next [style = \"filled\", fillcolor = \"cornFlowerBlue\"];\n");
    fprintf(dumpFile, "prev [style = \"filled\", fillcolor = \"salmon\"];\n\n");

    fprintf(dumpFile, "prev -> next; next -> prev;\n");
    //fprintf(dumpFile, "free -> prev; prev -> free;\n");

    fprintf(dumpFile, "\nedge [color = \"cornFlowerBlue\", weight = 0, constraint = false];\n\n");

    if (headIndex) fprintf(dumpFile, "node01 -> node%d;\n", headIndex);
    else           fprintf(dumpFile, "node01 -> node02;\n");

    for (int i = 1; i <= capacity; i++)
    {
        if (listNextIndex(list, i) == 0)
        {
            fprintf(dumpFile, "node%d -> node02;\n", i);
        }
        else
        {
            fprintf(dumpFile, "node%d -> node%d;\n", i, abs(listNextIndex(list, i)));
        }
    }

    fprintf(dumpFile, "\nfree -> node%d [constraint = true]\n\n", listFree(list));

    fprintf(dumpFile, "\nedge [color = \"salmon\", weight = 0, constraint = false];\n\n");

    if (tailIndex) fprintf(dumpFile, "node02 -> node%d;\n", tailIndex);
    else           fprintf(dumpFile, "node02 -> node01;\n"); 

    for (int i = 1; i <= capacity; i++)
    {
        if (listPrevIndex(list, i) == 0)
        {
            fprintf(dumpFile, "node%d -> node01;\n", i);
        }
        else if (listPrevIndex(list, i) > 0)
        {
            fprintf(dumpFile, "node%d -> node%d;\n", i, listPrevIndex(list, i));
        }
    }

    fprintf(dumpFile, "}\n");

    return EXIT_SUCCESS;
}


int writeListToDotFileArrangedNext(List *list, FILE *dumpFile)
{
    assert(list);
    assert(dumpFile);

    fprintf(dumpFile, "digraph\n{\n");
    fprintf(dumpFile, "rankdir = LR;\n\n");

    fprintf(dumpFile, "node [shape = Mrecord, color  = \"navy\"];\n");

    fprintf(dumpFile, "next [style = \"filled\", fillcolor = \"cornFlowerBlue\"];\n");
    fprintf(dumpFile, "prev [style = \"filled\", fillcolor = \"salmon\"];\n");

    fprintf(dumpFile, "node01 [label = \"list | <next> head = %d | <prev> tail = %d\",", 
            listHeadIndex(list), listTailIndex(list));
    fprintf(dumpFile, "style = \"filled\", fillcolor = \"#afeeee\", ];\n");

    fprintf(dumpFile, "node02 [label = \"list | <next> head = %d | <prev> tail = %d\",", 
            listHeadIndex(list), listTailIndex(list));
    fprintf(dumpFile, "style = \"filled\", fillcolor = \"#afeeee\", ];\n");

    int capacity = listCapacity(list);

    for (int i = 1; i <= capacity; i++)
    {
        fprintf(dumpFile, 
                "node%d[label = \"%d | data = %d | <next>next = %d | <prev>prev = %d\"];\n",
                i, i, listValueByIndex(list, i), abs(listNextIndex(list, i)), listPrevIndex(list, i));
    }
    fprintf(dumpFile, "node%d [style = \"dashed\", label = \"idx = %d\"];\n", 
            capacity + 1, capacity + 1);

    fprintf(dumpFile, "free  [label = \"free = %d\", ", listFree(list));
    fprintf(dumpFile, "style = \"filled\", fillcolor = \"#33ff66\"];\n");

    fprintf(dumpFile, "\nedge [color = \"cornFlowerBlue\"];\n\n");

    int headIndex = listHeadIndex(list);
    if (headIndex) fprintf(dumpFile, "node01 -> node%d;\n", headIndex);
    else           fprintf(dumpFile, "node01 -> node02;\n");

    for (int i = 1; i <= capacity; i++)
    {
        if (listNextIndex(list, i) == 0)
        {
            fprintf(dumpFile, "node%d -> node02;\n", i);
        }
        else
        {
            fprintf(dumpFile, "node%d -> node%d;\n", i, abs(listNextIndex(list, i)));
        }
    }

    fprintf(dumpFile, "\nfree -> node%d\n\n", listFree(list));

    fprintf(dumpFile, "\nedge [color = \"salmon\"];\n\n");

    int tailIndex = listTailIndex(list);
    if (tailIndex) fprintf(dumpFile, "node02 -> node%d;\n", tailIndex);
    else           fprintf(dumpFile, "node02 -> node01;\n"); 

    for (int i = 1; i <= capacity; i++)
    {
        if (listPrevIndex(list, i) == 0)
        {
            fprintf(dumpFile, "node%d -> node01;\n", i);
        }
        else if (listPrevIndex(list, i) > 0)
        {
            fprintf(dumpFile, "node%d -> node%d;\n", i, listPrevIndex(list, i));
        }
    }

    fprintf(dumpFile, "}\n");

    return EXIT_SUCCESS;
}
