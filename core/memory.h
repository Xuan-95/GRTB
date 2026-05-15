#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>
#include <stdint.h>

#define GROW_CAPACITY(capacity) ((capacity) < 8 ? 8 : (capacity) * 2)
#define GROW_ARRAY(type, pointer, oldCount, newCount)                                                                  \
    ((type *)reallocate(pointer, sizeof(type) * (oldCount), sizeof(type) * (newCount)))

#define FREE(type, pointer)                 reallocate(pointer, sizeof(type), 0)
#define FREE_ARRAY(type, pointer, oldCount) reallocate(pointer, sizeof(type) * oldCount, 0)
#define ALLOCATE(type, count)               (type *)reallocate(NULL, 0, sizeof(type) * (count))

//
#define ARENA_SIZE 1024 * 16

typedef struct {
    uint8_t buffer[ARENA_SIZE];
    size_t  offset;
} ScratchArena;

// One area per thread
static __thread ScratchArena thread_arena;

void                        *arena_alloc(size_t size);
void                         arena_reset(void);
void                        *reallocate(void *pointer, size_t oldSize, size_t newSize);

#endif
