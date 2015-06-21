#include <stdlib.h>
#include <stdio.h>
#include "MEM.h"

static void
dump_buffer(unsigned char *buf, int size)
{
  int i;
  for (i = 0; i < size; i++) {
	printf("%02x ", buf[i]);
	if (i % 16 == 15) {
	  printf("\n");
	}
  }
  printf("\n");
}

static void
fill_buffer(unsigned char *buf, int size)
{
    int i;

    for (i = 0; i < size; i++) {
        buf[i] = i;
    }
}

static void
test_memory()
{
  unsigned char *p1;
  p1 = MEM_malloc(10);
  dump_buffer(p1, 10);  
  fill_buffer(p1, 10);
  dump_buffer(p1, 10);
  MEM_dump_blocks(stdout);
}

struct Header {
  int size;
  char c;
  char c1;
};

union Align {
  double d_dummy;
  long l_dummy;
  void *p_dummy;
};

typedef union {
  struct Header header;
  union Align align;
} HeaderWrapper;

void
test_size()
{
  printf("size of char: %ld\n", sizeof(char));
  printf("size of unsigned char: %ld\n", sizeof(unsigned char));    
  printf("size of int: %ld\n", sizeof(int));
  printf("size of long: %ld\n", sizeof(long));
  printf("size of double: %ld\n", sizeof(double));
  printf("size of pointer: %ld\n", sizeof(void*));
  printf("size of Align: %ld\n", sizeof(union Align));      
  printf("size of long long: %ld\n", sizeof(long long));
  printf("size of Header: %ld\n", sizeof(struct Header));
  printf("size of HeaderWrapper: %ld\n", sizeof(HeaderWrapper));        
}

int main(void)
{
  /* test_memory(); */
  test_size();
}


