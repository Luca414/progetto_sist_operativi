#pragma once

#include <stdlib.h>  // Per size_t

#define HEADER_SIZE 8
#define BUDDY_FLAG 1
#define OTHER_FLAG 0

void* malloc_red(size_t size);
void free_red(void* ptr);

