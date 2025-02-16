#pragma once
#include <stdint.h>

// struct bitmap
typedef struct  {
  char *buffer;  // buffer di memoria bitmap  [a b c d e] //converto unit8_t in char
  int buffer_size;
  int num_bits; 
} BitMap;

void BitMap_init(BitMap* bit_map, int num_bits, char* buffer);  //converto unit8_t in char


int getMaxNodes(int num_levels);

int BitMap_getBytes(int bits);

int findFreeNodes(int first_node, int last_node, char* buffer); //converto unit8_t in char

void printBuffer(char* buffer, int size);

void updateBitmap(int free_node, char* buffer);

int findParent(int free_node);

// 16.02.2025
