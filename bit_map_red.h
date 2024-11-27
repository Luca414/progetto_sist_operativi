#pragma once
#include <stdint.h>

// simple bit array
typedef struct  {
  uint8_t *buffer;  // externally allocated buffer
  int buffer_size;
  int num_bits; 
} BitMap;

void BitMap_init(BitMap* bit_map, int num_bits, uint8_t* buffer);
