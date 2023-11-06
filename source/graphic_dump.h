#ifndef  __GRAPHIC_DUMP_H__
#define  __GRAPHIC_DUMP_H__

int listGraphicDump   (List *list);
int writeListToDotFileArrangedNext (List *list, FILE *dumpFile);  
int writeListToDotFileArrangedIndex(List *list, FILE *dumpFile);

char *createDumpFileName(int fileNumber, int *fileNameLength,  const char *specifier);

int createListGraphicDumpPng(char *fileName, int fileNameLength);

#endif //__GRAPHIC_DUMP_H__
