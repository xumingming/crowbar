#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "memory.h"

typedef union {
  long l_dummy;
  double d_dummy;
  void *p_dummy;
} Cell;

#define CELL_SIZE (sizeof(Cell))
#define DEFAULT_PAGE_SIZE (1024)

typedef struct MemoryPage_tag MemoryPage;
typedef MemoryPage *MemoryPageList;

/**
 * 内存页,每个内存页有很多 cell
 */
struct MemoryPage_tag {
  /*  这个内存页里面一共有多少个 cell? */
  int cell_num;
  /* 有多少个 cell 被使用了 */
  int use_cell_num;
  /* 下一个内存页的引用 */
  MemoryPageList next;
  /* 实际的的提供给外面使用的内存块在这里 */
  Cell cell[1];
};

struct MEM_Storage_tag {
  /* 这个 storage 的内存页的列表 */
  MemoryPageList page_list;
  /* 当前每页 page 的大小 */
  int current_page_size;
};

#define larger(a, b) (((a) > (b)) ? (a) : (b))

MEM_Storage
MEM_open_storage_func(MEM_Controller controller,
					  char *filename, int line, int page_size) {
  MEM_Storage storage;
  storage = MEM_malloc_func(controller, filename, line, sizeof(struct MEM_Storage_tag));
  storage->page_list = NULL;
  assert(page_size >= 0);

  if (page_size > 0) {
	storage->current_page_size = page_size;
  } else {
	storage->current_page_size = DEFAULT_PAGE_SIZE;
  }

  return storage;
}

void*
MEM_storage_malloc_func(MEM_Controller controller, 
						char* filename, int line,
						MEM_Storage storage,						
						size_t size)
{
  int cell_num;
  void *ptr;
  cell_num = (size - 1) / CELL_SIZE + 1;

  if (storage->page_list->use_cell_num + cell_num <= storage->page_list->cell_num) {
	ptr = &(storage->page_list->cell[storage->page_list->use_cell_num]);
	storage->page_list->use_cell_num += cell_num;
  } else {
	int alloc_cell_num;
	MemoryPageList new_page;
	
	alloc_cell_num = larger(storage->current_page_size, cell_num);
	new_page  = MEM_malloc_func(controller, filename, line,
								(alloc_cell_num - 1) * CELL_SIZE + sizeof(struct MemoryPage_tag));
	new_page->cell_num = alloc_cell_num;
	new_page->use_cell_num = cell_num;
	new_page->next = storage->page_list;
	storage->page_list = new_page;
	
	ptr = &(new_page->cell[0]);
  }

  return ptr;
}

void
MEM_dispose_storage_func(MEM_Controller controller, MEM_Storage storage)
{
  MemoryPage *temp;
  while (storage->page_list) {
	temp = storage->page_list->next;
	MEM_free_func(controller, storage->page_list);
	storage->page_list = temp;
  }

  MEM_free_func(controller, storage);
}
