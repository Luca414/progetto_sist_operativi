#include <stdio.h>
#include <math.h>
#include "buddy_red.h"
#include "bit_map_red.h"

int main() {
    int num_levels, memory_size, min_bucket_size;
    
    printf("\n\nConfigurazione buddy allocator:");

    // Chiedi all'utente di inserire i parametri
    printf("\nInserisci il numero di livelli dell'albero: ");
    scanf("%d", &num_levels);

    printf("\nInserisci la dimensione totale della memoria (in byte): ");
    scanf("%d", &memory_size);

    printf("\nInserisci la dimensione minima di un bucket (in byte): ");
    scanf("%d", &min_bucket_size);

    // Calcolo del numero massimo di nodi
    int max_nodes = (1 << (num_levels + 1)) - 1;
    // Dimensione della bitmap
    int bitmap_size = (max_nodes + 7) / 8;

    // Creazione della memoria per la bitmap e il buddy allocator
    char buffer[bitmap_size];          // Array per la bitmap
    char memory[memory_size];          // Array per la memoria gestita dal buddy allocator

    // Creazione del BuddyAllocator
    BuddyAllocator allocator;

    // Inizializzazione del BuddyAllocator
    int success = BuddyAllocator_init(&allocator, num_levels, buffer, bitmap_size, memory, memory_size, min_bucket_size);

    if (success) {
        printf("\n\nBuddyAllocator inizializzato correttamente!\n");
        printf("Numero livelli: %d\n", num_levels);
        printf("Dimensione memoria: %d byte\n", memory_size);
        printf("Dimensione minima bucket: %d byte\n\n", min_bucket_size);
    } else {
        printf("Errore nell'inizializzazione del BuddyAllocator.\n");
    }

    return 0;
}



