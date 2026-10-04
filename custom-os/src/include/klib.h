#ifndef KLIB_H
#define KLIB_H
/*
 * Function: replace libc with own implementation
 * src lib/klib.c
 */
void kgetline(char *, int);
int kstrcmpr(const char *, const char *, int);
int kstrlen(const char *);
void khandle_input(const char *);
#endif
