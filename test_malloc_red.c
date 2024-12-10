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
    int max_nodes = getMaxNodes(num_levels);

    // Dimensione della bitmap
    int bitmap_size = getBitmapSize(max_nodes);

    // Creazione della memoria per la bitmap e il buddy allocator
    char buffer[bitmap_size];          // Array per la bitmap
    char memory[memory_size];          // Array per la memoria gestita dal buddy allocator

    // Creazione del BuddyAllocator
    BuddyAllocator allocator;

    // Inizializzazione del BuddyAllocator
    int success = BuddyAllocator_init(&allocator, num_levels, buffer, bitmap_size, memory, memory_size, min_bucket_size);

    int num_bits = max_nodes;

    if (success) {
        printf("\n\nBuddyAllocator inizializzato correttamente!\n");
        printf("Numero livelli: %d\n", num_levels);
        printf("Dimensione memoria: %d byte\n", memory_size);
        printf("Dimensione minima bucket: %d byte\n\n", min_bucket_size);

        printf("Il numero di nodi considerando %d livelli è: %d\n",num_levels,max_nodes);

        printf("Prima variabile buffer bitmap: %d \n", *buffer);
        printf("Il buffer bitmap avrà una dimensione di: %ld bytes \n", sizeof(buffer));
        printf("Il buffer conterrà : %d bit \n", num_bits); 

    } else {
        printf("Errore nell'inizializzazione del BuddyAllocator.\n");
    }

    return 0;
}


// 10 12 24


