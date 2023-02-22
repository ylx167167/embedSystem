#ifndef _ELEMENT_IO
#define _ELEMENT_IO
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "/usr/local/include/pbc/pbc.h"
void element_fread(FILE *fp,char *format,element_t *e,int base);
void element_fread_line(FILE *fp,element_t *e,int base);
//void elementStringToFile(char *outputFile,element_t element);
//void elementFileToString(char *inputFile,element_t element);
#endif
