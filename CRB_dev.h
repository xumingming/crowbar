#ifndef PUBLIC_CRB_DEV_H_INCLUDED
#define PUBLIC_CRB_DEV_H_INCLUDED

/**
 * Crowbar 里面的布尔类型
 */
typedef enum {
  CRB_FALSE = 0,
  CRB_TRUE = 1
} CRB_Boolean;

/**
 * CRB 里面的字符串类型
 */
typedef struct CRB_String_tag CRB_String;

typedef struct {
  char *name;
} CRB_NativePointerInfo;

/**
 * Crowbar 里面支持的所有的值类型
 */
typedef enum {
  CRB_BOOLEAN_VALUE = 1,
  CRB_INT_VALUE,
  CRB_DOUBLE_VALUE,
  CRB_STRING_VALUE,
  CRB_NATIVE_POINTER_VALUE,
  CRB_NULL_VALUE
} CRB_ValueType;

typedef struct {
  CRB_NativePointerInfo *info;
  void *pointer;
} CRB_NativePointer;

typedef struct {
  CRB_ValueType type;
  union {
	CRB_Boolean boolean_value;
	int int_value;
	double double_value;
	CRB_String *string_value;
	CRB_NativePointer native_pointer;
  }u;
} CRB_Value;

typedef CRB_Value CRB_NativeFunctionProc(CRB_Interpreter *interpreter,
										 int arg_count,
										 CRB_Value *args);

void CRB_add_native_function(CRB_Interpreter *interpreter,
							 char *name,
							 CRB_NativeFunctionProc *proc);

void CRB_add_global_variable(CRB_Interpreter *interpreter,
							 char *identifier,
							 CRB_Value *value);

#endif /* PUBLIC_CRB_DEV_H_INCLUDED */
