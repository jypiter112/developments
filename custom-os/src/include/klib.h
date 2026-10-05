#ifndef KLIB_H
#define KLIB_H

// Move to types.h in future
#define size_t unsigned int

/*
 * Function: replace libc with own implementation
 * src lib/klib.c
 */
void kgetline(char *, int);
int kstrcmpr(const char *, const char *, size_t);
int kstrlen(const char *);
void khandle_input(const char *);

// Memory management
void *kmemcpy(void *, const void *, size_t);
#endif
