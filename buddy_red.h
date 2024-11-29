#pragma once
#include "bit_map_red.h" 


typedef struct{
  int num_levels;       //numero livelli dell'albero binario (15)
  char* memory;
  int memory_size;      //memoria totale disponibile: 1024*1024
  int min_bucket_size;  //la più piccola porzione di memoria allocabile (32 byte)
  BitMap bitmap;         
} BuddyAllocator;

int BuddyAllocator_init(BuddyAllocator *alloc, int num_levels, char* buffer, int buffer_size, char* memory, int memory_size, int min_bucket_size);



