#include "memory.h"
#include "common.h"
#include <stdio.h>
#include <stdlib.h>

void *reallocate(void *pointer, size_t oldSize, size_t newSize) {
    UNUSED(oldSize);
    if (newSize == 0) {
        free(pointer);
        return NULL;
    }

    void *result = realloc(pointer, newSize);

    if (result == NULL) {
        fprintf(stderr, "ERROR: Failed to allocate %zu bytes\n", newSize);
        exit(1);
    }

    return result;
}

void *arena_alloc(size_t size) {
    // 16 byte alignment
    size = (size + 15) & ~15;

    if (thread_arena.offset + size > ARENA_SIZE) {
        fprintf(stderr, "ERRORE: Arena esaurita!\n");
        return NULL;
    }

    void *ptr            = &thread_arena.buffer[thread_arena.offset];
    thread_arena.offset += size;
    return ptr;
}

void arena_reset(void) { thread_arena.offset = 0; }
