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
    printf("Dimensione bitmap non corretta\n");
    return 0;
    }
  //qui verifico che la memoria sia allineata correttamente, altrimenti arrotondo ad una potenza di 2
  if (log2(memory_size) != floor(log2(memory_size)) ){
    memory_size = min_bucket_size << num_levels;
    }
    
  // popolo i campi della struct   
  alloc->num_levels=num_levels;     
  alloc->memory=memory;
  alloc->min_bucket_size=min_bucket_size;
  alloc->memory_size=memory_size;
  
  for (int i=0;i<bitmap_size;i++){  // popolo la memoria della bitmap, settando ogni byte a 0: libero (nella bitmap init setto i bit a 0)
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


void* allocateMemory(BuddyAllocator* alloc, int node) {       
    if (node < 0) return NULL; // Controllo nodo non valido

    int level = floor(log2(node + 1));  // Trovo il livello del nodo
    int first_node_level = (1 << level) - 1; // Primo nodo del livello
    int block_size = alloc->memory_size >> level; // Dimensione blocco

    int offset = block_size * (node - first_node_level); // Calcolo l'offset

    // Debug print
    printf("\nNodo: %d", node);
    printf("\nLivello: %d", level);
    printf("\nPrimo nodo del livello: %d\n", first_node_level);
    //printf("\nBlock size: %d", block_size);
    //printf("\nNodo - Primo nodo livello: %d", node - first_node_level);
    //printf("\nValore offset: %d\n", offset);

    return alloc->memory + offset; // Restituisco il puntatore alla memoria allocata
}


// =================================================================================================================================
// =================================================================================================================================

void* buddy_malloc(BuddyAllocator* alloc, int request_size, char* buffer) {
    // Aggiungo lo spazio per l'header (8 byte)
    request_size += sizeof(AllocationHeader);

    int level = BuddyAllocator_findLevel(alloc->memory_size, alloc->min_bucket_size, request_size);
    if (level < 0) return NULL;

    int first_node, last_node;
    getNodesInLevel(level, &first_node, &last_node);

    int free_node = findFreeNodes(first_node, last_node, buffer);
    if (free_node < 0) return NULL;

    updateBitmap(free_node, buffer, request_size);

    void* raw_ptr = allocateMemory(alloc, free_node);
    if (!raw_ptr) return NULL;

    // Scrivo header
    AllocationHeader* header = (AllocationHeader*) raw_ptr;
    header->is_buddy = 1;
    header->node = free_node;

    // Restituisco il puntatore all'area utilizzabile (subito dopo l'header)
    return (void*)(header + 1);  // avanzamento di 8 byte
} 


void buddy_free(BuddyAllocator* alloc, void* ptr, char* buffer) {
    (void)alloc;
    if (ptr == NULL) return;

    // Torna indietro di 8 byte per ottenere l'header
    AllocationHeader* header = (AllocationHeader*)ptr - 1;

    // Verifica se l'allocazione è davvero da buddy
    if (header->is_buddy != 1) {
        printf("Errore: tentativo di liberare un blocco non gestito dal buddy allocator.\n");
        return;
    }

    int node = header->node;

    // Libera il nodo nella bitmap
    setBit(buffer, node, 0);  // 0 = libero

    // Prova a fondere ricorsivamente i buddy se anche il buddy è libero
    while (node != 0) {
        int parent = findParent(node);
        int buddy = (node % 2 == 0) ? node - 1 : node + 1;

        if (getBit(buffer, buddy) == 0) {
            // Entrambi i figli liberi: libera il padre
            setBit(buffer, parent, 0);
            setBit(buffer, buddy, 0);
            setBit(buffer, node, 0);
            node = parent;
        } else {
            break;  // il buddy è occupato: fine unione
        }
    }

    printf("Blocco di memoria liberato (nodo %d).\n", header->node);
} 












