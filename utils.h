#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>

#define NULLCHECK(ptr) if (!(ptr)) return NULL


void signalhandler(int siging);

/* helper functions */
void print_help();
int is_p_int(char* s);
int min(int a, int b);
int max(int a, int b);

#endif