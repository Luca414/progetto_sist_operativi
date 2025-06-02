#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "malloc_red.h"
#include "buddy_red.h"

#define MEMORY_SIZE (1024 * 1024)
#define BITMAP_SIZE 8192
#define LEVELS 15
#define MIN_BLOCK 32

// Allocatori globali
static char memory[MEMORY_SIZE];
static char bitmap[BITMAP_SIZE];
BuddyAllocator allocator;

int main() {
    
    printf("\n=== TEST INIZIALIZZAZIONE BUDDYALLOCATOR ===\n");
    printf("Inizializzo il buddyAllocator con valori non corretti: \n\n");
    
    printf("\n[TEST 1] Inizializzo con MEMORY_SIZE = 0:\n");
    if (!BuddyAllocator_init(&allocator, LEVELS, bitmap, BITMAP_SIZE, memory, 0, MIN_BLOCK)) {
        printf("Errore durante l'inizializzazione del BuddyAllocator!\n");
    }
    else printf("Buddy inizializzato con successo");
    
    printf("\n[TEST 2] Inizializzo con MIN_BLOCK = 13 (non potenza di 2):\n");
    if (!BuddyAllocator_init(&allocator, LEVELS, bitmap, BITMAP_SIZE, memory, MEMORY_SIZE, 13)) {
        printf("Errore durante l'inizializzazione del BuddyAllocator!\n");        
    }
    else printf("Buddy inizializzato con successo");
    
    printf("\n[TEST 3] Inizializzo con BITMAP_SIZE = 0:\n");
    if (!BuddyAllocator_init(&allocator, LEVELS, bitmap, 0, memory, MEMORY_SIZE, MIN_BLOCK)) {
        printf("Errore durante l'inizializzazione del BuddyAllocator!\n");
        //return 1;
    }
    else printf("Buddy inizializzato con successo");
    
    printf("\n[TEST 4] Inizializzazione con valori corretti:\n");
    if (!BuddyAllocator_init(&allocator, LEVELS, bitmap, BITMAP_SIZE, memory, MEMORY_SIZE, MIN_BLOCK)) {
        printf("Errore durante l'inizializzazione del BuddyAllocator!\n");
        return 1;
    }
    else printf("Buddy inizializzato con successo");

    // Test allocazione < 1024 (usa buddy)
    printf("\n\n=== TEST ALLOCAZIONE BUDDY ===\n\n\n");
    
    printf("Parte iniziale della Bitamp prima di eseguire allocazioni: ");
    int size = sizeof(bitmap) / sizeof(bitmap[0]);
    printBuffer(bitmap,size);
    
    printf("\nProvo ad allocare 10*sizeof(int) = %zu byte:\n", 10 * sizeof(int));
    int* buddy_ptr = (int*) malloc_red(10 * sizeof(int));
    if (!buddy_ptr) {
        printf("Errore: buddy_malloc fallita.\n");
        return 1;
    }
    printf("Allocazione effettuata con successo, puntatore ottenuto è: %p\n",(void*)buddy_ptr);
    printf("Popolo la memoria che mi sono riservato:\n\n");
    for (int i = 0; i < 10; i++) {
        buddy_ptr[i] = i * 2;
        printf("buddy_ptr[%d] = %d\n", i, buddy_ptr[i]);
    }
    
    printf("\nBitmap dopo aver eseguito l'allocazione: ");
    printBuffer(bitmap,size);
    printf("\n");
    
    

    // Test allocazione >= 1024 MMAP
    printf("\n\n=== TEST ALLOCAZIONE MMAP ===\n");
    int* big_ptr = (int*) malloc_red(2048);
    if (!big_ptr) {
        printf("Errore: MMAP fallita.\n");
        return 1;
    }
    for (int i = 0; i < 512; i++) {
        big_ptr[i] = i + 100;
    }
    printf("Valori scritti in memoria allocando con MMAP: big_ptr[0]=%d, big_ptr[100]=%d\n", big_ptr[0], big_ptr[100]);

    // Libera entrambe
    printf("\n\n=== LIBERAZIONE BUDDY ===\n");
    free_red(buddy_ptr);
    printf("Buffer (bitmap) dopo aver liberato: ");
    printBuffer(bitmap,size); //BuddyAllocator_print(&allocator);

    printf("\n=== FREE MMAP ===\n");
    free_red(big_ptr);
    //printf("\n=== DOPPIA FREE MMAP ===\n");
    //free_red(big_ptr);

    // Test doppia free (buddy)
    printf("\n\n=== DOPPIA FREE SU BUDDY: Non dovrebbe generare errori/crashare ===\n");
    free_red(buddy_ptr); // Non dovrebbe crashare

    // Test ulteriore allocazione
    printf("\n\n=== ALLOCAZIONE MULTIPLA ===\n");

    printf("Buffer Bitmap prima dell'allocazione multipla: "); 
    printBuffer(bitmap,size); 
    
    printf("\nChiedo di allocare 128 byte:");
    void* p1 = malloc_red(128);
    if (!p1) { printf("Errore malloc p1\n"); return 1; }
    printf("Allocazione effettuata con successo, puntatore ottenuto è: %p\n",(void*)p1);
    printf("L'inizio del buffer dopo la prima allocazione è: ");
    printBuffer(bitmap, size);

    printf("\n\nChiedo di allocare 200 byte:\n");
    void* p2 = malloc_red(200);
    if (!p2) { printf("Errore malloc p2\n"); return 1; }
    printf("Allocazione effettuata con successo, puntatore ottenuto è: %p\n",(void*)p2);
    printf("Buffer: ");
    printBuffer(bitmap, size);

    printf("\n\nChiedo di allocare 80 byte:\n");
    void* p3 = malloc_red(80);
    if (!p3) { printf("Errore malloc p3\n"); return 1; }
    printf("Allocazione effettuata con successo, puntatore ottenuto è: %p\n",(void*)p3);
    printf("Buffer: ");
    printBuffer(bitmap, size);

    // Accesso simulato
    memset(p1, 1, 128);  // Riempie i primi 128 byte puntati da p1 con il valore 1
    memset(p2, 2, 200);  
    memset(p3, 3, 80);  
    
    printf("\n\n== FREE IN ORDINE DIVERSO DA QUELLO DELL'ALLOCAZIONE == \n\n");
    printf("Libero il blocco p2 da 200 byte: ");
    free_red(p2);
    printf("Buffer: ");
    printBuffer(bitmap, size);

    printf("\n\nLibero il blocco p1 da 128 byte: ");
    free_red(p1);
    printf("Buffer: ");
    printBuffer(bitmap, size);

    printf("\n\nLibero il blocco p3 da 80 byte: ");
    free_red(p3);
    printf("Buffer: ");
    printBuffer(bitmap, size);

    bool tutto_libero = true;
    for (int i = 0; i < size; ++i) {
      if (bitmap[i] != 0) {
        tutto_libero = false;
        break;
      }
    }
    
    if (tutto_libero)
      printf("\nTutti i blocchi sono stati liberati correttamente: ogni elemento della bitmap è = 0\n");
    else
    printf("\nAttenzione: alcuni blocchi risultano ancora occupati.\n");


    printf("\n\n ========== Tutti i test completati con successo! ==========\n\n");

    return 0;
}















/*#include <stdio.h>
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
        
        request_size=request_size+4; //aggiungo 4 byte per stoccare i dati che mi servirarnno nella free
        if (request_size == 3) { // -1+4=3
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
            *(char*)allocated_memory = 'B';  //casto a char
            
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
*/
























// Dentro il file buddy_red.c creati la funzione buddy_malloc.c dove chiami tutte le funzioni dentro il while qui sopra. Nella funzione buddy_malloc.c ritorno il puntatore 
// dopo aver riservato 8 byte per me. Nei primi 4 mi salvo se utilizzo buddy (1) oppure mmap (0), negli altri 4 : caso buddy il nodo, caso mmap la grandezza del blocco richiesto+overhead
// nel file buddy_red.c creati la funzione free_buddy(&allocator, free_node). 

// creare file.c pseudo_malloc_red.c , dentro il file pseudo_malloc_red.c creo 2 funzioni: pseudo_malloc e pseudo_free. Dentro pseudo_malloc metto il controllo
// di mmap (>1024 byte) oppure buddy.  

/*

char* pseudo_malloc(){

    if(request_size > 1024){   (caso mmap)
      chiamo mmap 
      if (mmap){  
        ottengo un puntatore alla prima cella allocata
        nella prima cella = m  (nella prima cella libera scrivo m, così capisco che sono nel caso mmap)
        nelle 4 celle dopo = request_memory+overhead (nelle 4 celle successive ci scrivo quell'intero)
          

        return puntatore alla cella immediatamente libera (quinta cella)
        
    else{ (caso buddy)
      chiamo buddy_malloc
     
    }
}



char* buddy_malloc(allocator,request_size){
        puntato ottenuto da allocateMemory = b
        puntato ottenuto da allocateMemory+4 = free_node
        return puntatore alla cella immediatamente libera
}

in pseudo_malloc_red.c
void my_free(){
  leggo le info che ho salvato
  se buddy
      chiamo buddy_free
  se mmap
     chiamo free_mmap
     
in buddy_red.c
  void buddy_free(&allocator, free_node){
     setto a 0 i nodi che devo settare a 0 nella bitamp
     }
     
*/
