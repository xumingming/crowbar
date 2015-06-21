#ifndef PRIVATE_CROWBAR_H_INCLUDED
#define PRIVATE_CROWBAR_H_INCLUDED
#include <stdio.h>
#include "MEM.h"
#include "CRB.h"
#include "CRB_dev.h"

#define LINE_BUF_SIZE (1024)

/**
 * 编译错误
 */
typedef enum {
  /* 解析出错 */
  PARSE_ERR = 1,
  /* 错误的字符 */
  CHARACTER_INVALID_ERR,
  /* 一个函数定义了多次错误 */
  FUNCTION_MULTIPLE_DEFINE_ERR,
  COMPILE_ERROR_COUNT_PLUS_1
} CompileError;

/**
 * 消息的参数类型
 */
typedef enum {
  INT_MESSAGE_ARGUMENT = 1,
  DOUBLE_MESSAGE_ARGUMENT,
  STRING_MESSAGE_ARGUMENT,
  CHARACTER_MESSAGE_ARGUMENT,
  POINTER_MESSAGE_ARGUMENT,
  MESSAGE_ARGUMENT_END
} MessageArgumentType;

typedef struct Variable_tag
{
  /* 变量的名字 */
  char *name;
  /* 变量的值 */
  CRB_Value value;
  /* 下一个变量，组成一个链表 */
  struct Variable_tag *next;
} Variable;

typedef enum {
  /* crowbar 里面的函数 */
  CROWBAR_FUNCTION_DEFINITION = 1,
  /* native 函数 -- 其实就是 C 函数 */
  NATIVE_FUNCTION_DEFINITION
} FunctionDefinitionType;

/**
 * 参数列表
 */
typedef struct ParameterList_tag {
  /* 参数的名字 */
  char *name;
  /* 下一个参数 */
  struct ParameterList_tag *next;
} ParameterList;

typedef struct Statement_tag Statement;

typedef struct FunctionDefinition_tag {
  /* 函数的名字 */
  char *name;
  /* 函数的类型 */
  FunctionDefinitionType type;
  union {
	/* crowbar 函数体 */
	struct {
	  /* 参数列表 */
	  ParameterList *parameter;
	  /* 函数体 */
	  Block *block;
	} crowbar_f;
	struct {
	  CRB_NativeFunctionProc *proc;
	} native_f;
  } u;
  /* 下一个函数定义 */
  struct FunctionDefinition_tag *next;
} FunctionDefinition;

struct CRB_Interpreter_tag {
  /* 编译使用用的内存 */
  MEM_Storage interpreter_storage;
  /* 执行时候用的内存 */
  MEM_Storage execute_storage;
  /* 全局变量列表 */
  Variable *variable;
  /*  函数列表 */
  FunctionDefinition *function_list;
  /*  顶层语句列表 */
  StatementList *statement_list;
  /*  当前行号 */
  int current_line_number;
};

typedef struct Expression_tag Expression;
typedef enum {
  BOOLEAN_EXPRESSION = 1,
  INT_EXPRESSION,
  DOUBLE_EXPRESSION,
  STRING_EXPRESSION,
  IDENTIFIER_EXPRESSION,
  ASSIGN_EXPRESSION,
  ADD_EXPRESSION,
  SUB_EXPRESSION,
  MUL_EXPRESSION,
  DIV_EXPRESSION,
  MOD_EXPRESSION,
  EQ_EXPRESSION,
  NE_EXPRESSION,
  GT_EXPRESSION,
  GE_EXPRESSION,
  LT_EXPRESSION,
  LE_EXPRESSION,
  LOGICAL_AND_EXPRESSION,
  LOGICAL_OR_EXPRESSION,
  MINUS_EXPRESSION,
  FUNCTION_CALL_EXPRESSION,
  NULL_EXPRESSION,
  EXPRESSION_TYPE_COUNT_PLUS_1
} ExpressionType;

typedef struct ArgumentList_tag {
  Expression *expression;
  struct ArgumentList_tag *next;
} ArgumentList;

typedef struct {
  char *variable;
  Expression *operand;
} AssignExpression;

typedef struct {
  Expression *left;
  Expression *right;
} BinaryExpression;

typedef struct {
  char *identifier;
  ArgumentList *argument;
} FunctionCallExpression;

struct Expression_tag {
  ExpressionType type;
  int line_number;
  union {
	CRB_Boolean				 boolean_value;
	int						 int_value;
	double					 double_value;
	char					 *string_value;
	char					 *identifier;
	AssignExpression		 assign_expression;
	BinaryExpression		 binary_expression;
	Expression				 *minus_expression;
	FunctionCallExpression	 function_call_expression;
  } u;
};

typedef struct StatementList_tag {
  Statement *statement;
  struct StatementList_tag *next;
} StatementList;

typedef struct {
  StatementList *statemen_list;
} Block;

typedef struct IdentifierList_tag {
  char *name;
  struct IdentifierList_tag *next;
} IdentifierList;

typedef struct {
  IdentifierList *identifier_list;
} GlobalStatement;

typedef struct Elsif_tag {
  Expression *condition;
  Block *block;
  struct Elsif_tag *next;
} Elsif;

typedef struct {
  Expression *condition;
  Block *then_block;
  Elsif *elsif_list;
  Block *else_block;
} IfStatement;

typedef struct {
  Expression *condition;
  Block *block;
} WhileStatement;

typedef struct {
  Expression *init;
  Expression *condition;
  Expression *post;
  Block *block;
} ForStatement;

typedef struct {
  Expression *return_value;
} ReturnStatement;

typedef enum {
  EXPRESSION_STATEMENT = 1,
  GLOBAL_STATEMENT,
  IF_STATEMENT,
  WHILE_STATEMENT,
  FOR_STATEMENT,
  RETURN_STATEMENT,
  BREAK_STATEMENT,
  CONTINUE_STATEMENT,
  STATEMENT_TYPE_COUNT_PLUS_1
} StatementType;

struct Statement_tag {
  StatementType type;
  int line_number;
  union {
	Expression		 *expression_s;
	GlobalStatement	 global_s;
	IfStatement		 if_s;
	WhileStatement	 while_s;
	ForStatement	 for_s;
	ReturnStatement	 return_s;
  } u;
};

typedef struct ParameterList_tag {
  char *name;
  struct ParameterList_tag *next;
} ParameterList;

typedef enum {
  CROWBAR_FUNCTION_DEFINITION = 1,
  NATIVE_FUNCTION_DEFINITION
} FunctionDefinitionType;

typedef struct FunctionDefinition_tag {
  char *name;
  FunctionDefinitionType type;
  union {
	struct {
	  ParameterList *parameter;
	  Block *block；
	} crowbar_f;
	struct {
	  CRB_NativeFunctionProc *proc;
	} native_f;
  } u;

  struct FunctionDefinition_tag *next;
} FunctionDefinition;


typedef struct Variable_tag {
  char *name;
  CRB_Value value;
  struct Variable_tag *next;
} Variable;

typedef enum {
  NORMAL_STATEMENT_RESULT = 1,
  RETURN_STATEMENT_RESULT,
  BREAK_STATEMENT_RESULT,
  CONTINUE_STATEMENT_RESULT,
  STATEMENT_RESULT_TYPE_COUNT_PLUS_1
} StatementResultType;

typedef struct {
  StatementResultType type;
  union {
	CRB_Value return_value;
  } u;
} StatementResult;

typedef struct GlobalVariableRef_tag {
  Variable *variable;
  struct GlobalVariableRef_tag *next;
} GlobalVariableRef;

typedef struct {
  Variable *variable;
  GlobalVariableRef *next;
} LocalEnvironment;

struct CRB_String_tag {
  int ref_count;
  char *string;
  CRB_Boolean is_literal;
};

typedef struct {
  CRB_String *strings;
} StringPool;
  
/* create.c */
void crb_function_define(char *identifier, ParameterList *parameter_list,
                         Block *block);

ParameterList *crb_create_parameter(char *identifier);

ParameterList *crb_chain_parameter(ParameterList *list,
                                   char *identifier);

ArgumentList *crb_create_argument_list(Expression *expression);

ArgumentList *crb_chain_argument_list(ArgumentList *list, Expression *expr);

StatementList *crb_create_statement_list(Statement *statement);
StatementList *crb_chain_statement_list(StatementList *list,
                                        Statement *statement);
Expression *crb_create_assign_expression(char *variable,
										 Expression *operand);
Expression *crb_create_binary_expression(ExpressionType operator,
                                         Expression *left,
                                         Expression *right);

/* eval.c */
CRB_Value crb_eval_binary_expression(CRB_Interpreter *inter,
									 LocalEnvironment *env,
									 ExpressionType operator,
									 Expression *left,
									 Expression *right);
/* string.c */
void crb_open_string_literal(void);

/* error.c */
void crb_compile_error(CompileError id, ...);
#endif /* PRIVATE_CROWBAR_H_INCLUDED */

