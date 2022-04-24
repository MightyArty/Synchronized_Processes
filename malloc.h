#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct block
{
    size_t size;
    struct block *next;
    struct block *prev;
} block_t;

#ifndef ALLOC_UNIT
#define ALLOC_UNIT (long unsigned int )3 * (long unsigned int )sysconf(_SC_PAGESIZE)
#endif

#ifndef MIN_DEALLOC
#define MIN_DEALLOC 1 * sysconf(_SC_PAGESIZE)
#endif

#define BLOCK_MEM(ptr) ((void *)((unsigned long)ptr + sizeof(block_t)))
#define BLOCK_HEADER(ptr) ((void *)((unsigned long)ptr - sizeof(block_t)))
void stats(char *prefix);
static block_t *head = NULL;

void fl_remove(block_t *b);
void fl_add(block_t *b);
void scan_merge();
void stats(char *prefix);
block_t *split(block_t *b, size_t size);

void *_malloc(size_t size);

void _free(void *ptr);

void _cleanup();