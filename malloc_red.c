#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include "buddy_red.h"
#include "bit_map_red.h"
#include "malloc_red.h"

#define HEADER_SIZE 8
#define BUDDY_FLAG 1
#define MMAP_FLAG 2

extern BuddyAllocator allocator;

void* malloc_red(size_t size) {
    size_t total_size = size + HEADER_SIZE;

    if (size <= 1024) {
        // Usa il buddy allocator
        void* block = buddy_malloc(&allocator, total_size, allocator.bitmap.buffer);
        if (!block) return NULL;

        // Scrivi header: flag e nodo 
        int* header = (int*)block;
        header[0] = BUDDY_FLAG;
        header[1] = *((int*)((char*)block + 4));  // Copia nodo scritto da buddy

        return (char*)block + HEADER_SIZE;
    } else {
        // Usa mmap per allocazioni grandi
        void* raw_block = mmap(NULL, total_size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
        if (raw_block == MAP_FAILED) {
            perror("Errore mmap");
            return NULL;
        }

        // Scrivi header: flag e metadato
        int* header = (int*)raw_block;
        header[0] = MMAP_FLAG;
        header[1] = (int)total_size;

        return (char*)raw_block + HEADER_SIZE;
    }
}


void free_red(void* ptr) {
    if (!ptr) return;

    // Torna all'inizio del blocco per accedere all'header
    char* block_start = (char*)ptr - HEADER_SIZE;
    int* header = (int*)block_start;

    int flag = header[0];

    if (flag == BUDDY_FLAG) {
        buddy_free(&allocator, block_start, allocator.bitmap.buffer);
    } else { // flag == MMAP_FLAG
        size_t size = header[1];  // dimensione totale salvata in malloc_red
        munmap(block_start, size);
    }
}


