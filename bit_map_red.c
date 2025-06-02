#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "bit_map_red.h"

// funzione che converte bit in byte
int BitMap_getBytes(int bits){
  return bits/8 + (bits%8)!=0;
}

// inizializzo bitmap

void BitMap_init(BitMap* bit_map, int num_bits, char* buffer){  //converto unit8_t in char

  bit_map->buffer=buffer;

  bit_map->num_bits=num_bits;

  bit_map->buffer_size=BitMap_getBytes(num_bits);

  //setto tutti i bit del buffer a zero
  memset(bit_map->buffer, 0, bit_map->buffer_size); // Imposta ogni bit a 0
 
}

  
int getMaxNodes(int num_levels) {
    return (1<<(num_levels))-1;
}


// Funzione per stampare il buffer
void printBuffer(char* buffer, int size) {
    (void)size;
    printf("{");
    for (int i = 0; i < 5; i++) {  // teoricamente ci vorrebbe "size" al posto di 5
        printf("0b");
        // Stampiamo ogni bit del byte da sinistra a destra
        for (int j = 7; j >= 0; j--) {
            printf("%d", (buffer[i] >> j) & 1); // Estraggo il bit j-esimo
        }
        if (i < 5 - 1) { // teoricamente ci vorrebbe "size" al posto di 5
            printf(", "); 
        }
    }
    printf("}\n");
}


// Flag globale per la prima allocazione. Sto macello è per gestire il caso in cui la prima richiesta occupi il nodo 0.
int isFirstAllocation = 1;
int isMemoryFullyAllocated = 0;


int findFreeNodes(int first_node, int last_node, char* buffer) {
    if (isMemoryFullyAllocated) {
        printf("Memoria completamente occupata, nessun nodo libero disponibile.\n");
        return -1; // Non ci sono nodi liberi
    }

    for (int i = first_node; i <= last_node; i++) {
        int byte_index = i / 8;
        int bit_index = i % 8;
        int state = (buffer[byte_index] >> bit_index) & 1;

        printf("\nIl nodo individuato è %d, il suo stato è: %d", i, state);

        // Se il nodo è libero, controlliamo i suoi genitori
        if (state == 0) {
            int parent = (i - 1) / 2;
            int valid = 1;

            while (parent > 0) {  // anche quì risalgo tutti i genitori e controllo che siano liberi
                int p_byte_index = parent / 8;
                int p_bit_index = parent % 8;
                int p_state = (buffer[p_byte_index] >> p_bit_index) & 1;

                // Se il genitore è occupato, controlliamo il buddy
                if (p_state == 1) {
                    int sibling = (parent * 2) + 1;  // Primo figlio
                    if (sibling == i) sibling++;  // Se il nodo corrente è il primo figlio, il buddy è il secondo

                    int s_byte_index = sibling / 8;
                    int s_bit_index = sibling % 8;
                    int s_state = (buffer[s_byte_index] >> s_bit_index) & 1;

                    // Se il buddy è occupato, allora posso allocare in questo nodo
                    if (s_state == 1) {
                        //printf("Il nodo %d è utilizzabile, il suo buddy %d è già allocato.\n", i, sibling);
                        return i;
                    } else {
                        valid = 0;
                        //printf("Il nodo %d non è utilizzabile perché il suo genitore %d è occupato, il buddy %d è libero.\n", i, parent, sibling);
                        break;
                    }
                }
                parent = (parent - 1) / 2;
            }

            if (valid) {
                return i;
            }
        }
    }

    printf("Nessun nodo libero trovato\n");
    return -1; // Nessun nodo disponibile
}


void updateBitmap(int node, char* buffer, int requestedSize) {
    // Verifico se è la prima allocazione
    if (isFirstAllocation && requestedSize >= 524289) {
        // Se la prima allocazione è >= 524289 byte, segnalo la memoria come completamente occupata
        isMemoryFullyAllocated = 1;  // Imposto la memoria come completamente occupata
    }

    // Procedi con la normale allocazione
    while (node >= 0) {
        int byte_index = node / 8; // Trova il byte nel buffer
        int bit_index = node % 8;  // Trova il bit nel byte
        buffer[byte_index] |= (1 << bit_index); // Imposta il bit a 1

        // Se settato il nodo 0, segnalo che la memoria è completamente allocata
        if (node == 0) {
            isFirstAllocation = 0;  // Dopo la prima allocazione, non è più la prima allocazione (:
            break;
        }

        // Risaliamo all'antenato
        node = (node - 1) / 2; // Calcoliamo il genitore
    }
}

///////////////////////////////////////////////////// 05 /////////////////////

void setBit(char* bitmap, int index, int value) {
    int byteIndex = index / 8;
    int bitOffset = index % 8;

    if (value)
        bitmap[byteIndex] |= (1 << bitOffset);  // imposta il bit a 1
    else
        bitmap[byteIndex] &= ~(1 << bitOffset); // imposta il bit a 0
}

int getBit(char* bitmap, int index) {
    int byteIndex = index / 8;
    int bitOffset = index % 8;
    return (bitmap[byteIndex] >> bitOffset) & 1;
}































