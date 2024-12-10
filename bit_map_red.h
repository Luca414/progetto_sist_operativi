#pragma once
#include <stdint.h>

// struct bitmap
typedef struct  {
  uint8_t *buffer;  // buffer di memoria bitmap  [a b c d e]
  int buffer_size;
  int num_bits; 
} BitMap;

void BitMap_init(BitMap* bit_map, int num_bits, uint8_t* buffer);


int getMaxNodes(int num_levels);

int BitMap_getBytes(int bits);
