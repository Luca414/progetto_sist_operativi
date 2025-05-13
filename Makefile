CC = gcc
CFLAGS = -Wall -Wextra -g

# Sorgenti
SRCS = test_malloc_red.c malloc_red.c buddy_red.c bit_map_red.c
OBJS = $(SRCS:.c=.o)

# Output finale
TARGET = test_allocator

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) -lm

clean:
	rm -f $(OBJS) $(TARGET)



