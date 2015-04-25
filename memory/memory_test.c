#include <stdlib.h>
#include <stdio.h>
#include "minunit.h"
#include "memory.h"

int tests_run = 0;
int foo = 7;
int bar = 4;

static char *test_foo() {
  mu_assert("error, foo != 7", foo == 7);
  return 0;
}

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

static char* test_memory() {
  unsigned char *p1;
  Header* rawp;
  
  p1 = MEM_malloc(10);
  p1 = p1 - 2;
  dump_buffer(p1, 10);  
  fill_buffer(p1, 10);
  dump_buffer(p1, 15);
  rawp = (Header*)(p1 - sizeof(Header));
  mu_assert("memory size is not 10!", rawp->s.size == 10);
  
  return 0;
}

static char * all_tests() {
  mu_run_test(test_foo);
  mu_run_test(test_memory);
}


 int main(int argc, char **argv) {
     char *result = all_tests();
     if (result != 0) {
         printf("%s\n", result);
     }
     else {
         printf("ALL TESTS PASSED\n");
     }
     printf("Tests run: %d\n", tests_run);
 
     return result != 0;
 }


