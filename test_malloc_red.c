#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>  // questa per usare uint8_t

#include "malloc_red.h"   
#include "buddy_red.h"    
#include "bit_map_red.h"  

#define MEMORY_SIZE (1024 * 1024) // 1MB

int main() {
    // Inizializza un array di 1MB per simulare la memoria gestita
    // il tipo uint8_t rappresenta esattamente 1byte senza segno
    uint8_t memory[MEMORY_SIZE];
    memset(memory, 0, MEMORY_SIZE); // Pulizia iniziale della memoria

    // Calcola la dimensione necessaria per la bitmap
    int num_bits = MEMORY_SIZE / 8; // Supponiamo che il blocco minimo sia di 8 byte
    int bitmap_size = BitMap_getBytes(num_bits);
    printf("Dimensione bitmap calcolata: %d byte\n", bitmap_size);

    // Inizializza il Buddy Allocator usando la memoria
    BuddyAllocator buddy_allocator;
    BuddyAllocator_init(&buddy_allocator, MEMORY_SIZE, memory);

    // 4. Test di allocazione e deallocazione
    printf("Inizio dei test sul Buddy Allocator...\n");

    // Test 1: Allocazione di un blocco piccolo (es. 64 byte)
    void* block1 = my_malloc(64);
    if (block1) {
        printf("Allocazione di 64 byte riuscita.\n");
    } else {
        printf("Errore nell'allocazione di 64 byte.\n");
    }

/*    // Test 2: Allocazione di un blocco grande (es. 2048 byte, quindi mmap)
    void* block2 = my_malloc(2048);
    if (block2) {
        printf("Allocazione di 2048 byte riuscita (usando mmap).\n");
    } else {
        printf("Errore nell'allocazione di 2048 byte.\n");
    } */

    // Test 3: Deallocazione del primo blocco
    my_free(block1, 64);
    printf("Deallocazione di 64 byte completata.\n");

    // Test 4: Deallocazione del secondo blocco
/*    my_free(block2, 2048);
    printf("Deallocazione di 2048 byte completata.\n"); */

    printf("Tutti i test completati.\n");

    return 0;
}

