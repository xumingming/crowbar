#ifndef PRIVATE_DBG_H_INCLUDED
#define PRIVATE_DBG_H_INCLUDED
#include <stdio.h>
#include "DBG.h"
struct DBG_Controller_tag {
  /* debug 信息写到哪里去 */
  FILE *debug_write_fp;
  /* 当前的 debug level? 可能的值是哪些? */
  int current_debug_level;
};

#endif /* PRIVATE_DBG_H_INCLUDED */
