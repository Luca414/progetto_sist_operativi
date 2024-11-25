#include <stdio.h> // Per debug (printf)
#include <math.h> // per calcoli vari




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
void findNodeBlocks(int node, int total_blocks, int* start_block, int* end_block) {
    int level = (int)log2(node + 1);  // Livello del nodo
    int nodes_in_level = 1 << level; // Numero di nodi nel livello
    int block_range = total_blocks / nodes_in_level; // Blocchi per nodo

    int node_in_level = node - ((1 << level) - 1); // Posizione del nodo nel livello
    *start_block = node_in_level * block_range; // Primo blocco
    *end_block = *start_block + block_range - 1; // Ultimo blocco
}



// =================================================================================================================================
// =================================================================================================================================


// TEST PER FUNZIONI SU NODI E BLOCCHI
int main() {
    int total_blocks = 16; // Memoria con 16 blocchi (array da 0 a 15)
    int level = 2;         // Livello da analizzare

    // Calcola primo e ultimo nodo del livello
    int first_node, last_node;
    findLevelNodes(level, &first_node, &last_node);
    printf("Livello %d: primo nodo = %d, ultimo nodo = %d\n", level, first_node, last_node);

    // Calcola intervalli di blocchi per ciascun nodo
    for (int node = first_node; node <= last_node; node++) {
        int start_block, end_block;
        findNodeBlocks(node, total_blocks, &start_block, &end_block);
        printf("Nodo %d: blocchi da %d a %d\n", node, start_block, end_block);
    }

    return 0;
}




// TEST PER CALCOLO LIVELLO
/*int main() {
    size_t total_size = 1024 * 1024;    // 1MB
    size_t block_min_size = 8;         // Blocco minimo di 8 byte
    size_t request_size = 234;         // Richiesta di 128 byte

    int level = BuddyAllocator_findLevel(total_size, block_min_size, request_size);
    if (level >= 0) {
        printf("Livello trovato: %d\n\n", level);
    } else {
        printf("Errore nella determinazione del livello.\n");
    }

    return 0;
} */

