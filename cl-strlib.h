#ifndef CL_STRLIB_H
#define CL_STRLIB_H
#include "cl-strlib.c"

int cl_strlen(char* str);
int cl_substr(char* str, char* result, int startIndex, int length);
int cl_hstint(char* str, int* result);

void cl_strtest(void);
#endif