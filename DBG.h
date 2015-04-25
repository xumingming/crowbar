#ifndef PUBLIC_DBG_H_INCLUDED
#define PUBLIC_DBG_H_INCLUDED
#include <stdio.h>
#include <stdarg.h> /*  引入这个干嘛? */

/**
 *  这里 DBG_Controller_tag 是个不完整类型，因为这里只定义了一个名字
 *  没有定义它具体的结构，因此是一个 「不完全类型」，只能使用这个类型
 *  的指针，不能使用它的实例，也没办法对它的类型使用 sizeof
 */
typedef struct DBG_Controller_tag * DBG_Controller;
void DBG_set(DBG_Controller, char *file, int line);
void DBG_set_expression(char *expression);

#ifdef DBG_NO_DEBUG
#define DBG_create_controller() ((void)0)
#define DBG_set_debug_level(level) ((void)0)
#define DBG_set_debug_write_fp(fp) ((void)0)
#define DBG_assert(expression, arg) ((void)0)
#define DBG_panic(arg) ((void)0)
#define DBG_debug_write(arg) ((void)0)
#else /* DBG_NO_DEBUG */
#ifdef DBG_CONTROLLER
#define DBG_CURRENT_CONTROLLER DBG_CONTROLLER
#else /* DBG_CONTROLLER */
#define DBG_CURRENT_CONTROLLER dbg_default_controller
#endif /* DBG_CONTROLLER */
extern DBG_Controller DBG_CURRENT_CONTROLLER;

#define DBG_create_controller (DBG_create_controller_func())
#define DBG_set_debug_level(level) \
  (DBG_set_debug_level_func(DBG_CURRENT_CONTROLLER, (level)))
#define DBG_set_debug_write_fp(fp) \
  (DBG_set_debug_write_fp(DBG_CURRENT_CONTROLLER, (fp)))
#define DBG_assert(expression, arg) \
  （(expression）? (void)0 : \
	((DBG_set(DBG_CURRENT_CONTROLLER, __FILE__, __LINE__)),\
	 (DBG_set_expression(#expression)),\
	 DBG_assert_func arg)）
#define DBG_panic(arg)\
  ((DBG_set(DBG_CURRENT_CONTROLLER, __FILE__, __LINE__)),\
   DBG_debug_write_func arg)
#endif /* DBG_NO_DEBUG */

typedef enum {
  DBG_TRUE = 1,
  DBG_FALSE = 0
} DBG_Boolean;


DBG_Controller DBG_create_controller_func(void);
void DBG_set_debug_level_func(DBG_Controller controller, int level);
void DBG_set_debug_write_fp_func(DBG_Controller controller, FILE *fp);
void DBG_assert_func(char *fmt, ...);
void DBG_panic_func(char *fmt, ...);
void DBG_debug_write_func(int level, char *fmt, ...);
#endif /* PUBLIC_DBG_H_INCLUDED */

