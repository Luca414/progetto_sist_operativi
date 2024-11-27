#include <stdio.h> // Per debug (printf)
#include <math.h> // per calcoli vari
#include <assert.h>
#include <stdlib.h>

#include "buddy_red.h"
#include "bit_map_red.h"




int BuddyAllocator_init(BuddyAllocator *alloc, int num_levels, char* buffer, int buffer_size, char* memory, int memory_size, int min_bucket_size){
  
  // calcolo il numero massimo di nodi dell'albero del buddy
  int max_nodes = (1<<(num_levels+1))-1; 
  int bitmap_size = (max_nodes+7)/8;
  if (buffer_size<bitmap_size){
    printf("Dimensione non corretta");
    return 0;
    }
  if (log2(memory_size) != floor(log2(memory_size)) ){
    memory_size = min_bucket_size << num_levels;
    }
    
  alloc->num_levels=num_levels;
  alloc->memory=memory;
  alloc->min_bucket_size=min_bucket_size;
  alloc->memory_size=memory_size;
  
  for (int i=0;i<bitmap_size;i++){
    buffer[i]=0;
    }
  
  BitMap_init(&(alloc->bitmap),(bitmap_size*8),buffer);
  
  return 1;
  
}





// Funzione per trovare il livello corretto per l'allocazione
int BuddyAllocator_findLevel(size_t total_size, size_t block_min_size, size_t request_size) {
    // Controllo che la richiesta sia valida
    if (request_size > total_size) {
        printf("Errore: è stata richiesta una quantità di memoria troppo grande.\n");
        return -1;
    }
    if (request_size < block_min_size) {
        printf("Errore: è stata richiesta una quantità di memoria troppo piccola.\n");
        return -1; 
    }
   
    // Trova il livello corretto usando le divisioni
    int level = 0;
    size_t block_size = total_size;
    
    printf("\n\nCerco livello per %d byte di memoria:\n", request_size);

    while (block_size > request_size && block_size > block_min_size) {
        block_size /= 2; // Dividi il blocco in due
        level++;         // Passa al livello successivo
    }

    // Ritorna il livello trovato
    return level;
}





// Trova il primo e l'ultimo nodo di un livello
void findLevelNodes(int level, int* first_node, int* last_node) {
    *first_node = (1 << level) - 1;  // 2^level - 1
    *last_node = (1 << (level + 1)) - 2;  // 2^(level+1) - 2
}





// Trova l'intervallo di blocchi gestiti da un nodo specifico
void findNodeBlocks(int level, int node, int total_blocks, int* start_block, int* end_block) {
    /*int level = (int)log2(node + 1);  // Livello del nodo*/
    int nodes_in_level = 1 << level; // Numero di nodi nel livello
    int block_range = total_blocks / nodes_in_level; // Blocchi per nodo

    int node_in_level = node - ((1 << level) - 1); // Posizione del nodo nel livello
    *start_block = node_in_level * block_range; // Primo blocco
    *end_block = *start_block + block_range - 1; // Ultimo blocco
} 



// =================================================================================================================================
// =================================================================================================================================


// TEST PER FUNZIONI BUDDY A PARTIRE DAI 3 PARAMETRI FORNITI:
int main() {
    size_t total_size = 1024 * 1024;    // 1MB
    size_t block_min_size = 8;         // Blocco minimo di 8 byte
    size_t request_size = 262144;         // Richiesta di 128 byte
    
    printf("\n\nMemoria totale disponibile: %d byte",total_size);
    printf("\nMemoria da allocare: %d byte",request_size);
    
    int level = BuddyAllocator_findLevel(total_size, block_min_size, request_size);
    if (level >= 0) {
        printf("Livello trovato: %d", level);
    } else {
        printf("Errore nella determinazione del livello.\n");
    }
    
    int total_blocks = 1048576; // Memoria con 16 blocchi (array da 0 a 15)

    // Calcola primo e ultimo nodo del livello
    int first_node, last_node;
    findLevelNodes(level, &first_node, &last_node);
    printf("\n\nLivello %d: primo nodo = %d, ultimo nodo = %d\n", level, first_node, last_node);

    // Calcola intervalli di blocchi per ciascun nodo
    for (int node = first_node; node <= last_node; node++) {
        int start_block, end_block;
        findNodeBlocks(level, node, total_blocks, &start_block, &end_block);  // aggiunto level qui
        printf("Nodo %d: blocchi da %d a %d\n\n", node, start_block, end_block);
    }

    return 0;
}

