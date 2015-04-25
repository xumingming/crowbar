#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "memory.h"

static void default_error_handler(MEM_Controller controller,
								  char *file, int line, char* msg);

static struct MEM_Controller_tag st_default_controller = {
  NULL,
  default_error_handler,
  MEM_FAIL_AND_EXIT
};
MEM_Controller mem_default_controller = &st_default_controller;


static void
default_error_handler(MEM_Controller controller,
					  char* filename, int line, char *msg)
{
  fprintf(controller->error_fp,
		  "MEM:%s failed in %s at %d\n", msg, filename, line);
}
MEM_Controller
MEM_create_controller(void)
{
  MEM_Controller p;
  p = (MEM_Controller)MEM_malloc_func(&st_default_controller, __FILE__, __LINE__,
					  sizeof(struct MEM_Controller_tag));
  *p = st_default_controller;

  return p;
}

/**
 * prepend 一个新的内存块到系统里面去
 */
static void
chain_block(MEM_Controller controller, Header *new_header)
{
  if (controller->block_header) {
	controller->block_header->s.prev = new_header;
  }

  new_header->s.prev = NULL;
  new_header->s.next = controller->block_header;
  controller->block_header = new_header;
}

/**
 * 没有太明白这个函数的作用
 */
static void
rechain_block(MEM_Controller controller, Header *header)
{
  if (header->s.prev) {
	header->s.prev->s.next = header;
  } else {
	controller->block_header = header;
  }

  if (header->s.next) {
	header->s.next->s.prev = header;
  }
}

/**
 * 把一块内存从整个链子里面干掉
 */
static void
unchain_block(MEM_Controller controller, Header *header)
{
  if (header->s.prev) {
	header->s.prev->s.next = header->s.next;
  } else {
	controller->block_header = header->s.next;
  }

  if (header->s.next) {
	header->s.next->s.prev = header->s.prev;
  }
}

static void
error_handler(MEM_Controller controller, char *filename, int line, char*msg)
{
  if (controller->error_fp == NULL) {
	controller->error_fp = stderr;
  }
  controller->error_handler(controller, filename, line, msg);

  if (controller->fail_mode == MEM_FAIL_AND_EXIT) {
	exit(1);
  }
}

/* 为什么上面这些方法是 static 的？ */

/**
 * 这里的 header size 为什么不是 MARK_SIZE, 而要这么算出来？
 *  因为一个结构的结尾会有 padding
 */
int
cal_actual_header_mark_size(Header *header)
{
  return (char*)&header[1] - (char*)header->s.mark;
}

/**
 *  初始化一个 header
 */
void
set_header(Header *header, int size, char *filename, int line) {
  header->s.size = size;
  header->s.filename = filename;
  header->s.line = line;

  int actual_header_mark_size = cal_actual_header_mark_size(header);
  memset(header->s.mark, MARK, actual_header_mark_size);
}

/**
 * 把尾子也设置成 Mark
 */
void
set_tail(void *ptr, int alloc_size)
{
  char *tail;
  tail = ((char*)ptr) + alloc_size - MARK_SIZE;
  memset(tail, MARK, MARK_SIZE);
}

void
check_mark_sub(unsigned char *mark, int size)
{
  int i;
  for (i = 0; i < size; i++) {
	if (mark[i] != MARK) {
	  fprintf(stderr, "bad mark!\n");
	  abort();
	}
  }
}

void
check_mark(Header *header)
{
  unsigned char *tail;
  check_mark_sub(header->s.mark, cal_actual_header_mark_size(header));
  tail = ((unsigned char*)header) + header->s.size + sizeof(Header);
  check_mark_sub(tail, MARK_SIZE);
}

/**
 * 分配内存
 */
void*
MEM_malloc_func(MEM_Controller controller, char *filename, int line, size_t size)
{
  void *ptr;
  size_t alloc_size;

  alloc_size = size + sizeof(Header) + MARK_SIZE;
  ptr = malloc(alloc_size);
  if (ptr == NULL) {
	error_handler(controller, filename, line, "malloc");
  }

  memset(ptr, 0xCC, alloc_size);
  set_header(ptr, size, filename, line);
  set_tail(ptr, alloc_size);
  chain_block(controller, (Header*)ptr);
  ptr = (char*)ptr + sizeof(Header);

  return ptr;
}

void
MEM_free_func(MEM_Controller controller, void* ptr)
{
  Header* header;
  size_t size;
  /* 找到内存的头 */
  void *actual_ptr = (char*)ptr - sizeof(Header);
  /* check the mark */
  check_mark((Header*)actual_ptr);

  header = (Header*)actual_ptr;
  
  size = header->s.size;
  /* 把这块内存从总的链里面除下来 */
  unchain_block(controller, actual_ptr);
  /* 这里 memset 是为了什么？ */
  memset(actual_ptr, 0xCC, size + sizeof(Header));
  free(actual_ptr);
}

/**
 * 设置一个内存控制器的「出错处理函数」
 */
void
MEM_set_error_handler(MEM_Controller controller, MEM_ErrorHandler handler)
{
  controller->error_handler = handler;
}

void
MEM_set_fail_mode(MEM_Controller controller, MEM_FailMode mode)
{
  controller->fail_mode = mode;
}


void
MEM_dump_blocks_func(MEM_Controller controller, FILE *fp)
{
  Header *pos;
  int counter = 0;

  for (pos = controller->block_header; pos; pos = pos->s.next) {
	check_mark(pos);
	fprintf(fp, "[%04d]%p ******************** \n", counter,
			(char*)pos + sizeof(Header));
	fprintf(fp, "%s line %d size..%d\n", pos->s.filename, pos->s.line,
			pos->s.size);
	counter++;
  }
}
