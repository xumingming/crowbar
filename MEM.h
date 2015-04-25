#ifndef PUBLIC_MEM_H
#define PUBLIC_MEM_H

/**
 * 内存出错之后的处理方式
 */
typedef enum {
    MEM_FAIL_AND_EXIT,
    MEM_FAIL_AND_RETURN
} MEM_FailMode;

typedef struct MEM_Controller_tag *MEM_Controller;
typedef struct MEM_Storage_tag *MEM_Storage;
extern MEM_Controller mem_default_controller;

#ifdef MEM_CONTROLLER
#define MEM_CURRENT_CONTROLLER MEM_CONTROLLER
#else /* MEM_CONTROLLER */
#define MEM_CURRENT_CONTROLLER mem_default_controller
#endif /* MEM_CONTROLLER */

/**
 * 内存出错处理器
 */
typedef void (*MEM_ErrorHandler)(MEM_Controller, char *, int, char *);

/**
 * 分配内存的函数
 */
void *MEM_malloc_func(MEM_Controller controller,
                      char *filename, int line, size_t size);
/**
 * 把当前所有的内存详情都 dump 出来的函数
 */
void MEM_dump_blocks_func(MEM_Controller controller, FILE *fp);

/**
 * 创建一个新的内存 storage
 */
MEM_Storage MEM_open_storage_func(MEM_Controller controller, char* filename, int line, int page_size);

/**
 * 从 storage 里面分配一块内存出来
 */
void* MEM_storage_malloc_func(MEM_Controller controller,
							  char* filename,
							  int line,
							  MEM_Storage storage,
							  size_t size);
/**
 * 销毁一个内存 storage
 */
void MEM_dispose_storage_func(MEM_Controller controller, MEM_Storage storage);

/* 定义外部更简单的调用方法 */
#define MEM_malloc(size)\
  (MEM_malloc_func(MEM_CURRENT_CONTROLLER,\
                   __FILE__, __LINE__, size))
#define MEM_dump_blocks(fp)\
  (MEM_dump_blocks_func(MEM_CURRENT_CONTROLLER, fp))

#define MEM_open_storage(page_size)\
  (MEM_open_storage_func(MEM_CURRENT_CONTROLLER,\
						 __FILE__, __LINE__, page_size))

#define MEM_storage_malloc(storage, size)\
  (MEM_storage_malloc_func(MEM_CURRENT_CONTROLLER, storage,\
						   __FILE__, __LINE__, size))

#define MEM_dispose_storage(storage)\
  (MEM_dispose_storage_func(MEM_CURRENT_CONTROLLER, storage))
#endif
