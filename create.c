#include "MEM.h"
#include "DBG.h"
#include "crowbar.h"

void
crb_function_define(char *identifier, ParameterList *parameter_list,
					Block *block)
{
  FunctionDefinition *f;
  CRB_Interpreter *iter;

  if (crb_search_function(identifier)) {
	crb_compile_error(FUNCTION_MULTIPLE_DEFINE_ERR,
					  STRING_MESSAGE_ARGUMENT, "name", identifier,
					  MESSAGE_ARGUMENT_END);
	return;
  }

  inter = crb_get_current_interpreter();

  f = crb_malloc(sizeof(FunctionDefinition));
  f->name = identifier;
  f->type = CROWBAR_FUNCTION_DEFINITION;
  f->u.crowbar_f.parameter = parameter_list;
  f->u.crowbar_f.block = block;
  f->next = inter->function_list;
  inter->function_list = f;
}

StatementList *
crb_create_statement_list(Statement *statement)
{
  StatementList *sl;
  sl = crb_malloc(sizeof(StatementList));
  sl->statement = statement;
  sl->next = NULL;

  return sl;
}

StatementList *
crb_chain_statement_list(StatementList *list, Statement *statement)
{
  StatementList *pos;

  if (list == NULL) {
	return crb_create_statement_list(statement);
  }

  for (pos = list; pos->next; pos = pos->next)
	;

  pos->next = crb_create_statement_list(statement);
}

Expression *
crb_alloc_expression(ExpressionType type)
{
  Expression *exp;
  exp = crb_alloc(sizeof(Expression));
  exp->type = type;
  exp->line_number = crb_get_current_interpreter()->current_line_number;

  return exp;
}

ArgumentList *
crb_create_argument_list(Expression *expression)
{
  ArgumentList *al;
  al = crb_malloc(sizeof(ArgumentList));
  al->expression = expression;
  al->next = NULL;

  return al;
}

ArgumentList *
crb_chain_argument_list(ArgumentList * list, Expression *expression)
{
  ArgumentList *pos;
  for (pos = list; pos; pos = pos->next)
	;

  pos->next = crb_create_argument_list(expression);

  return list;
}

Expression *
crb_create_assign_expression(char *variable, Expression *operand)
{
  Expression *exp;
  exp = crb_alloc_expression(ASSIGN_EXPRESSION);
  exp->u.assign_expression.variable = variable;
  exp->u.assign_expression.operand = operand;

  return exp;
}

/**
 * 把常量(int, double, boolean) 根据类型包装成对应的表达式。
 */
static Expression
convert_value_to_expression(CRB_Value *v)
{
  Expression expr;

  if (v->type == CRB_INT_VALUE) {
	expr.type = INT_EXPRESSION;
	expr.u.int_value = v.u.int_value;
  } else if (v-> type == CRB_DOUBLE_VALUE) {
	expr.type = DOUBLE_EXPRESSION;
	expr.u.double_value = v.u.double_value;
  } else {
	DBG_assert(v-> type == CRB_BOOLEAN_VALUE,
			   ("v->type..%d\n", v->type));
	expr.type = BOOLEAN_EXPRESSION;
	expr.u.boolean_value = v->u.boolean_value;
  }

  return expr;
}

Expression *
crb_create_binary_expression(ExpressionType operator,
							 Expression *left, Expression *right)
{
  // 如果左右两边都是数字，那么做常量折叠
  if ((left->type == INT_EXPRESSION
	   || left->type == DOUBLE_EXPRESSION)
	  && (right->type == INT_EXPRESSION
		  || right->type = DOUBLE_EXPRESSION)) {
	CRB_Value v;
	v = crb_eval_binary_expression(crb_get_current_interpreter(), NULL, operator, left, right);
	*left = convert_value_to_expression(&v);

	return left;
  } else {
	Expression *exp;
	exp = crb_alloc_expression(operator);
	exp->u.binary_expression.left = left;
	exp->u.binary_expression.right = right;

	return exp;
  }
}

