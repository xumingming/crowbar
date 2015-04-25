#ifndef PRIVATE_MEM_H_INCLUDED
#define PRIVATE_MEM_H_INCLUDED
#include "MEM.h"

typedef union Header_tag Header;

#define ALIGN_SIZE (sizeof(Align))
#define revalue_up_align(val) ((val) ? (((val) - 1) / ALIGN_SIZE + 1) : 0)
#define HEADER_ALIGN_SIZE (revalue_up_align(sizeof(HeaderStruct)))
#define MARK (0xCD)
#define MARK_SIZE (4)

typedef union {
  long l_dummy;
  double d_dummy;
  void *p_dummy;
} Align;

typedef struct {
  /* 这块内存有多大 */
  int size;
  /* 这块内存是哪个文件申请的? */
  char* filename;
  /* 这块内存是在哪一行申请的? */
  int line;
  /* 前一块内存是什么? */
  Header* prev;
  /* 后一块内存是什么? */
  Header* next;
  /* 内存栅栏 */
  unsigned char mark[MARK_SIZE];
} HeaderStruct;



union Header_tag {
  HeaderStruct s;
  Align u[HEADER_ALIGN_SIZE];
};

struct MEM_Controller_tag {
  /* 对应的错误文件 */
  FILE *error_fp;
  /* 出错处理程序 */
  MEM_ErrorHandler error_handler;
  /* 内存出错时候如何处理 */
  MEM_FailMode fail_mode;
  /* 整个内存块的头 */
  Header *block_header;
};

#endif
