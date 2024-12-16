#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "bit_map_red.h"

// funzione che converte bit in bytes
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


int findFreeNodes(int first_node, int last_node, char* buffer){ 

    for(int i=first_node ; i<= last_node ; i++){

        int byte_index = i / 8; // trovo il byte dentro il bufffer
        //printf("byte_index: %d\n",byte_index);
        int bit_index = i % 8;  // trovo il bit del byte trovato
        //printf("bit_index: %d\n",bit_index);

        //buffer[byte_index] |= (1 << bit_index); // Imposta il bit ad un valore
        int state = (buffer[byte_index] >> bit_index) & 1;
        printf("Stato nodo %d: %d\n",i, state); // Output: 1
 
        if (state == 0){
            return i;
           }
        
        //printf("i : %d \n", i);      
    }
    printf("Sono uscito dal ciclo");
    return -1;

}


// Funzione per stampare il buffer
void printBuffer(char* buffer, int size) {
    printf("{");
    for (int i = 0; i < size; i++) {
        printf("0b");
        // Stampiamo ogni bit del byte da sinistra a destra
        for (int j = 7; j >= 0; j--) {
            printf("%d", (buffer[i] >> j) & 1); // Estraggo il bit j-esimo
        }
        if (i < size - 1) {
            printf(", ");
        }
    }
    printf("}\n");
}


/*void updateBitmap(int free_node, char* buffer){

    int byte_index = free_node / 8;

    buffer[byte_index] |= (1 << free_node); // Imposta il bit ad un valore
} */
















