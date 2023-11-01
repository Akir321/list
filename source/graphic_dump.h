#ifndef  __GRAPHIC_DUMP_H__
#define  __GRAPHIC_DUMP_H__

int listGraphicDump   (List *list);
int writeListToDotFile(List *list, FILE *dumpFile);   

char *createDumpFileName(int fileNumber, int *fileNameLength);

int createListGraphicDumpPng(List *list, char *fileName, int fileNameLength);

#endif //__GRAPHIC_DUMP_H__
