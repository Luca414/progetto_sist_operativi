#include <stdio.h> 
#include <math.h> 
#include <assert.h>
#include <stdlib.h>

#include "buddy_red.h"
#include "bit_map_red.h"



//Funzione init Buddy Allocator
int BuddyAllocator_init(BuddyAllocator *alloc, int num_levels, char* buffer, int buffer_size, char* memory, int memory_size, int min_bucket_size){
  
  //Controlli vari:
  if(!memory||memory_size<=0){ printf("Memoria inserita insufficiente\n"); return 0;};
  if(min_bucket_size<16){ printf("Dimensione minima allocabile troppo piccola, richiesti almeno 16 byte\n"); return 0;};
  if(log2(min_bucket_size)!=floor(log2(min_bucket_size))){ printf("Dimensione minima allocabile non è una potenza di 2\n"); return 0;};
  
  // calcolo il numero massimo di nodi dell'albero del buddy
  int max_nodes = getMaxNodes(num_levels);

  int bitmap_size = getBitmapSize(max_nodes);
  if (buffer_size<bitmap_size){
    printf("Dimensione non corretta\n");
    return 0;
    }
  //qui verifico che la memoria sia allineata correttamente: potenza di 2
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

int getBitmapSize(int max_nodes){
    return ((max_nodes+7)/8);
}



// Funzione per trovare il livello corretto per l'allocazione
int BuddyAllocator_findLevel(int memory_size, int min_bucket_size, int request_size) { 
    // Controllo che la richiesta sia valida
    if (request_size > memory_size) {
        printf("Errore: è stata richiesta una quantità di memoria troppo grande.\n");
        return -1;
    }
    if (request_size < min_bucket_size) {
        printf("Errore: è stata richiesta una quantità di memoria troppo piccola.\n");
        return -1; 
    }
   int level = 0;
    int current_size = memory_size;

    // Continua a dimezzare finché la dimensione corrente è maggiore della richiesta
    while (current_size / 2 >= request_size && current_size / 2 >= min_bucket_size) {
        current_size /= 2;
        level++;
    }

    return level;

}

// Trova il primo e l'ultimo nodo di un livello
void getNodesInLevel(int level, int* first_node, int* last_node) {
    // Calcola il primo nodo del livello
    *first_node = (1 << level) - 1;
    // Calcola l'ultimo nodo del livello
    *last_node = (1 << (level + 1)) - 2;
}                                                                           




// Trova l'intervallo di blocchi gestiti da un nodo specifico
void findNodeBlocks(int level, int node, int memory_size, int* start_block, int* end_block) {
    /*int level = (int)log2(node + 1);  // Livello del nodo*/
    int nodes_in_level = 1 << level; // Numero di nodi nel livello
    int block_range = memory_size / nodes_in_level; // Blocchi per nodo

    int node_in_level = node - ((1 << level) - 1); // Posizione del nodo nel livello
    *start_block = node_in_level * block_range; // Primo blocco
    *end_block = *start_block + block_range - 1; // Ultimo blocco
} 

  
  int findParent(int node) {
    if (node == 0) {
        return -1; // La radice non ha genitore
    }
    return (node - 1) / 2;
}


// =================================================================================================================================
// =================================================================================================================================

// 16.02.2025

