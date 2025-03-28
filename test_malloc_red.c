#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include "buddy_red.h"
#include "bit_map_red.h"


int main(){
    int num_levels, memory_size, min_bucket_size, request_size;

    printf("\n\nConfigurazione buddy allocator:");

    // Chiedi all'utente di inserire i parametri
    printf("\nInserisci il numero di livelli dell'albero: ");
    scanf("%d", &num_levels);

    printf("Inserisci la dimensione totale della memoria (in byte): "); 
    scanf("%d", &memory_size);

    printf("Inserisci la dimensione minima di un bucket (in byte): ");
    scanf("%d", &min_bucket_size);

    // Calcolo del numero massimo di nodi
    int max_nodes = getMaxNodes(num_levels);


    // PASSO A INIZIALIZZARE LA BITMAP
    // Dimensione della bitmap
    int bitmap_size = getBitmapSize(max_nodes);

    // Creazione della memoria per la bitmap e il buddy allocator
    char buffer[bitmap_size];          // Array per la bitmap
    char memory[memory_size];          // Array per la memoria gestita dal buddy allocator

    // Creazione del BuddyAllocator
    BuddyAllocator allocator;

    // Inizializzazione del BuddyAllocator
    int success = BuddyAllocator_init(&allocator, num_levels, buffer, bitmap_size, memory, memory_size, min_bucket_size);
    if(success==0)exit(1);

    int num_bits = max_nodes; 

    if (success) {
        printf("\nBuddyAllocator inizializzato correttamente!\n\n");
        printf("Numero livelli: %d\n", num_levels);
        printf("Dimensione memoria: %d byte\n", memory_size);
        printf("Dimensione minima bucket: %d byte\n", min_bucket_size);

        printf("Il numero di nodi considerando %d livelli è: %d\n",num_levels,max_nodes);

        printf("Il buffer bitmap avrà una dimensione di: %ld bytes \n", sizeof(buffer));
        printf("Il buffer conterrà : %d bit \n", num_bits); 

    } else {
        printf("Errore nell'inizializzazione del BuddyAllocator.\n");
    }
    

    bool isTrue = true;

    while(isTrue){
        printf("\n\nInizia a chiedere la memoria da allocare, inserisci '-1' per uscire");
        printf("\nInserisci la dimensione di memoria da allocare (in byte): "); 
        scanf("%d", &request_size);
        if (request_size == -1) {
            break;
        }
        else{
            int level = BuddyAllocator_findLevel(memory_size, min_bucket_size, request_size); 
            printf("\nIl livello adatto ad allocare %d byte è: %d\n",request_size, level);
    
            int first_node, last_node;
            getNodesInLevel(level, &first_node, &last_node);
            printf("Nel livello %d ci sono i nodi da %d a %d\n\n", level, first_node, last_node);


            int free_node = findFreeNodes(first_node, last_node, buffer);
            printf("Il nodo libero trovato è il nodo: %d\n\n",free_node);

                
            printf("\nBuffer prima di essere aggiornato:                  ");    
            int size = sizeof(buffer) / sizeof(buffer[0]);
            printBuffer(buffer,size);

            updateBitmap(free_node,buffer, request_size); 
            printf("\nBuffer dopo aver lanciato la funzione updateBitmap: ");

            printBuffer(buffer,size);
            
            printf("\n\n ===============================================\n");
            printf(" ===============================================");
            printf("\n\n____________TEST SU ALLOCAZIONE______________\n\n");
            void* allocated_memory = allocateMemory(&allocator, free_node);
            
            //Controllo allocazione
            if (allocated_memory != NULL) {
            printf("\nMemoria allocata con successo! Indirizzo: %p\n", allocated_memory);
            
            // Scrivo il carattere 'A' nella memoria allocata
            printf("\nScrivo manualmente il carattere 'A' nella memoria allocata...");
            *(char*)allocated_memory = 'A';  //casto a char
            
            // Calcolo l'offset della memoria allocata nel buffer
            int offset = (char*)allocated_memory - memory; 
            
            // Stampo il carattere in quella zona di memoria
            printf("\nContenuto di memory[%d]: %c\n", offset, memory[offset]);
            
           }
            
            else {
            printf("\nErrore nell'allocazione della memoria.\n");
            }
            printf("\n\n ===============================================\n");
            printf(" ===============================================");

        }
    }
    
    printf("\n\nProgramma terminato!\n\n");

    return 0;
}




