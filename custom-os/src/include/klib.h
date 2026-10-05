#ifndef KLIB_H
#define KLIB_H
#include <stddef.h>
/*
 * Function: replace libc with own implementation
 * src lib/klib.c
 */
void kgetline(char *, int);
void kprintf(const char*);
int kstrcmpr(const char *, const char *, size_t);
int kstrlen(const char *);
void khandle_input(const char *);

// Memory management
void *kmemcpy(void *, const void *, size_t);
#endif
