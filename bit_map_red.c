#include <stdio.h>
#include <assert.h>

#include "bit_map_red.h"

// funzione che converte bit in bytes
int BitMap_getBytes(int bits){
  return bits/8 + (bits%8)!=0;
}

// inizializzo bitmap

void BitMap_init(BitMap* bit_map, int num_bits, uint8_t* buffer){

  bit_map->buffer=buffer;

  bit_map->num_bits=num_bits;

  bit_map->buffer_size=BitMap_getBytes(num_bits);


}
  
int getMaxNodes(int num_levels) {
    return (1<<(num_levels))-1;
}
