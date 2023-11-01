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

    writeListToDotFile(list, dumpFile);
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

int writeListToDotFile(List *list, FILE *dumpFile)
{
    assert(list);
    assert(dumpFile);

    fprintf(dumpFile, "digraph\n{\n");
    fprintf(dumpFile, "rankdir = LR;\n\n");

    fprintf(dumpFile, "node [shape = Mrecord, color  = \"navy\"];\n");
    fprintf(dumpFile, "edge [color = \"cornFlowerBlue\"];\n\n");

    fprintf(dumpFile, "next [style = \"filled\", fillcolor = \"cornFlowerBlue\"];\n");
    fprintf(dumpFile, "prev [style = \"filled\", fillcolor = \"salmon\"];\n");

    fprintf(dumpFile, "node0 [label = \"list | <next> head = %d | <prev> tail = %d\",", 
            list->next[0], list->prev[0]);
    fprintf(dumpFile, "style = \"filled\", fillcolor = \"#afeeee\", ];\n");

    for (int i = 1; i <= list->capacity; i++)
    {
        fprintf(dumpFile, 
                "node%d[label = \"idx = %d | data = %d | <next>next = %d | <prev>prev = %d\"];\n",
                i, i, list->data[i], abs(list->next[i]), list->prev[i]);
    }
    fprintf(dumpFile, "node%d [style = \"dashed\", label = \"idx = %d\"];\n", 
            list->capacity + 1, list->capacity + 1);

    fprintf(dumpFile, "free  [label = \"free = %d\", ", list->free);
    fprintf(dumpFile, "style = \"filled\", fillcolor = \"#33ff66\"];\n");

    /*fprintf(dumpFile, "\nedge [color = \"white\"];\n\n");
    for (int i = 0; i < list->capacity; i++)
    {
        int j = i + 1;
        fprintf(dumpFile, "node%d -> node%d; node%d -> node%d;\n",
                 i, j, j, i);
    }*/

    fprintf(dumpFile, "\nedge [color = \"cornFlowerBlue\"];\n\n");

    for (int i = 0; i <= list->capacity; i++)
    {
        fprintf(dumpFile, "node%d -> node%d;\n",
                 i, abs(list->next[i]));
    }

    fprintf(dumpFile, "\nfree -> node%d\n\n", list->free);

    fprintf(dumpFile, "\nedge [color = \"salmon\"];\n\n");

    for (int i = 0; i <= list->capacity; i++)
    {
        if (list->prev[i] >= 0)
        {
            fprintf(dumpFile, "node%d -> node%d;\n",
                     i, list->prev[i]);
        }
    }

    fprintf(dumpFile, "}\n");

    return EXIT_SUCCESS;
}
