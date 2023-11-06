#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctime>
#include <sys/time.h>

#include "list.h"
#include "graphic_dump.h"
#include "html_logfile.h"

const int DumpFileNameAddedLength = 64;
const int CommandSize             = 28;

struct timeHolder
{
    int year;
    int month;
    int day;
    int hours;
    int minutes;
    int seconds;
    int miliseconds;
};

timeHolder getTime(void);

static timeHolder OpenTime = getTime();

int listGraphicDump(List *list)
{
    assert(list);

    static int dumpNumber   = 1;
    int fileNameLengthNext  = 0;
    int fileNameLengthIndex = 0;

    char *fileNameNext      = createDumpFileName(dumpNumber, &fileNameLengthNext,  "Next");
    char *fileNameIndex     = createDumpFileName(dumpNumber, &fileNameLengthIndex, "Index");
    dumpNumber++;

    FILE* dumpFileNext  = fopen(fileNameNext,  "w");
    FILE* dumpFileIndex = fopen(fileNameIndex, "w");

    writeListToDotFileArrangedNext (list, dumpFileNext);
    writeListToDotFileArrangedIndex(list, dumpFileIndex);
    
    fclose(dumpFileNext);
    fclose(dumpFileIndex);

    createListGraphicDumpPng(fileNameNext,  fileNameLengthNext);
    createListGraphicDumpPng(fileNameIndex, fileNameLengthIndex);

    LOG("%s:\n<img src = ../%s.png width = 50%%>\n", "next",  fileNameNext);
    LOG("%s:\n<img src = ../%s.png width = 50%%>\n", "index", fileNameIndex);

    free(fileNameNext);
    free(fileNameIndex);

    return EXIT_SUCCESS;
}

char *createDumpFileName(int fileNumber, int *fileNameLength, const char *specifier)
{
    int numberLength = 0;
    int number = fileNumber;

    for (  ; number > 0; number /= 10, numberLength++) {}

    *fileNameLength = DumpFileNameAddedLength + numberLength;
    char *fileName  = (char *)calloc(*fileNameLength, sizeof(char));

    sprintf(fileName, "gr_dump/dump_%d.%d.%d_%d.%d.%d.%d_%s%d.dot",
            OpenTime.day, OpenTime.month, OpenTime.year,
            OpenTime.hours, OpenTime.minutes, OpenTime.seconds, OpenTime.miliseconds,
            specifier, fileNumber);

    return fileName;
}

int createListGraphicDumpPng(char *fileName, int fileNameLength)
{
    assert(fileName);

    char *command = (char *)calloc(CommandSize + fileNameLength * 2, sizeof(char));
    sprintf(command, "dot %s -T png -o %s.png", fileName, fileName);

    system(command);
    free  (command);

    return EXIT_SUCCESS;
}

timeHolder getTime()
{
    timeHolder time = {};

    timeval openTime        = {};
    time_t  openTimeSec     = {};
    tm      openTimeTM = {};
    
    if (gettimeofday(&openTime, NULL) == -1)
    {
        printf("couldn't get time for logfile name\n"); 
        return time;
    }
    
    openTimeSec = openTime.tv_sec;

    if (localtime_s(&openTimeTM, &openTimeSec) != 0)
    {
        printf("couldn't convert time to tm\n"); 
        return time;
    }

    time.year        =  openTimeTM.tm_year + 1900;
    time.month       = openTimeTM.tm_mon + 1;
    time.day         = openTimeTM.tm_mday;

    time.hours       = openTimeTM.tm_hour;
    time.minutes     = openTimeTM.tm_min;
    time.seconds     = openTimeTM.tm_sec;
    time.miliseconds = openTime.tv_usec / 1000;

    return time;
}

int writeListToDotFileArrangedIndex(List *list, FILE *dumpFile)
{
    assert(list);
    assert(dumpFile);

    fprintf(dumpFile, "digraph\n{\n");
    fprintf(dumpFile, "rankdir = LR;\n\n");

    fprintf(dumpFile, "node [shape = Mrecord, color  = \"navy\", style = \"filled\"];\n");

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
    fprintf(dumpFile, "fillcolor = \"#afeeee\"];\n");

    fprintf(dumpFile, "node02 [label = \"list | tail = %d\", ", tailIndex);
    fprintf(dumpFile, "fillcolor = \"#afeeee\"];\n\n");

    for (int i = 1; i <= capacity; i++)
    {
        const char *color = NULL;
        if (listPrevIndex(list, i) == -1) color = "#badeba";
        else                              color = "#fff3e0";

        fprintf(dumpFile, 
                "node%d[label = \"%d | data = %d | next = %d | prev = %d\", fillcolor = \"%s\"];\n",
                i, i, listValueByIndex(list, i), abs(listNextIndex(list, i)), 
                listPrevIndex(list, i), color);
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

    fprintf(dumpFile, "node [shape = Mrecord, color  = \"navy\", style = \"filled\"];\n");

    fprintf(dumpFile, "next [fillcolor = \"cornFlowerBlue\"];\n");
    fprintf(dumpFile, "prev [fillcolor = \"salmon\"];\n");

    fprintf(dumpFile, "node01 [label = \"list | head = %d | tail = %d\",", 
            listHeadIndex(list), listTailIndex(list));
    fprintf(dumpFile, "fillcolor = \"#afeeee\", ];\n");

    fprintf(dumpFile, "node02 [label = \"list | head = %d | tail = %d\",", 
            listHeadIndex(list), listTailIndex(list));
    fprintf(dumpFile, "fillcolor = \"#afeeee\", ];\n");

    int capacity = listCapacity(list);

    for (int i = 1; i <= capacity; i++)
    {
        const char *color = NULL;
        if (listPrevIndex(list, i) == -1) color = "#badeba";
        else                              color = "#fff3e0";

        fprintf(dumpFile, 
                "node%d[label = \"%d | data = %d | next = %d | prev = %d\", fillcolor = \"%s\"];\n",
                i, i, listValueByIndex(list, i), abs(listNextIndex(list, i)), listPrevIndex(list, i),
                color);
    }
    fprintf(dumpFile, "node%d [style = \"dashed\", label = \"idx = %d\"];\n", 
            capacity + 1, capacity + 1);

    fprintf(dumpFile, "free  [label = \"free = %d\", ", listFree(list));
    fprintf(dumpFile, "fillcolor = \"#33ff66\"];\n");

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
