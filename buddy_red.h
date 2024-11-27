#pragma once
#include "bit_map_red.h" 


typedef struct{
  int num_levels;
  char* memory;
  int memory_size;
  int min_bucket_size;
  BitMap bitmap;         
} BuddyAllocator;




