#include "../include/AST.h"



AS_TREE* make_break()
{
    AS_TREE* node = new AS_TREE;
    node->type = Type::BREAK;
    return node;
}

AS_TREE* make_continue()
{
    AS_TREE* node = new AS_TREE;
    node->type = Type::CONTINUE;
    return node;
}

AS_TREE* make_return(AS_TREE* expr)
{
    AS_TREE* node = new AS_TREE;
    node->type = Type::RETURN;
    node->data.return_value.value = expr;
    return node;
}

AS_TREE* make_void()
{
    AS_TREE* node = new AS_TREE;
    node->type = Type::VOID;
    return node;
}

AS_TREE* make_statements(AS_TREE* orig, AS_TREE* val)
{
    if (orig == nullptr)
    {
        orig = new AS_TREE;
        orig->lineno = yylineno;
        orig->type = Type::STATEMENTS;
        orig->data.statements.data = new std::vector<AS_TREE*>;
    }
    val->lineno = yylineno;
    orig->data.statements.data->push_back(val);
    return orig;
}


AS_TREE* make_expression(AS_TREE* exp1, AS_TREE* exp2, Operation op)
{
    AS_TREE* node = new AS_TREE;
    node->lineno = yylineno;
    node->type = Type::EXPRESSION;
    node->data.expression.left = exp1;
    node->data.expression.right = exp2;
    node->data.expression.op = op;
    return node;
}

AS_TREE* make_value(std::string* type, void* data)
{
    AS_TREE* node = new AS_TREE;
    node->lineno = yylineno;
    node->type = Type::VALUE;
    node->data.value.type = type;
    node->data.value.data = data;

    AS_TREE* wrap = new AS_TREE;
    wrap->lineno = yylineno;
    wrap->type = Type::EXPRESSION;
    wrap->data.expression.left = node;
    wrap->data.expression.op = Operation::OP_NOTHING;
    return wrap;
}

AS_TREE* make_value(AS_TREE* id)
{
    AS_TREE* node = new AS_TREE;
    node->lineno = yylineno;
    node->type = VARIABLE_VALUE;
    node->data.variable_value.val = id;

    AS_TREE* wrap = new AS_TREE;
    wrap->lineno = yylineno;
    wrap->type = Type::EXPRESSION;
    wrap->data.expression.left = node;
    wrap->data.expression.op = Operation::OP_NOTHING;
    return wrap;
}

AS_TREE* make_assignment(AS_TREE* var, AS_TREE* value)
{
    AS_TREE* node = new AS_TREE;
    node->lineno = yylineno;
    node->type = Type::ASSIGNMENT;
    node->data.assignment.var = var;
    node->data.assignment.value = value;
    return node;
}

AS_TREE* make_incdec(AS_TREE* var, AS_TREE* delta, Operation op)
{
    AS_TREE* node = new AS_TREE;
    node->type = Type::INCDEC;
    node->data.incdec.var = var;
    node->data.incdec.expr = delta;
    node->data.incdec.op = op;
    return node;
}

std::string* make_type(std::string type)
{
    return new std::string(type);
}

AS_TREE* make_identifier(AS_TREE* orig, std::string* name)
{
    if (orig == nullptr)
    {
        orig = new AS_TREE;
        orig->lineno = yylineno;
        orig->type = Type::IDENTIFIER;
        orig->data.identifier.name = name;
        orig->data.identifier.from = nullptr;
        return orig;
    }
    else
    {
        AS_TREE* node = new AS_TREE;
        node->lineno = yylineno;
        node->type = Type::IDENTIFIER;
        node->data.identifier.name = name;
        node->data.identifier.from = orig;
        return node;
    }
}

AS_TREE* make_variable(std::string* type, std::string* name, bool isConst, AS_TREE* value)
{
    AS_TREE* node = new AS_TREE;
    node->lineno = yylineno;
    node->type = Type::DECLARATION;
    node->data.declaration.type = type;
    node->data.declaration.name = name;
    node->data.declaration.isConst = isConst;
    node->data.declaration.value = value;
    return node;
}

AS_TREE* make_struct_declaration(std::string* name, AS_TREE* decl_list)
{
    AS_TREE* node = new AS_TREE;
    node->lineno = yylineno;
    node->type = Type::STRUCT_DECLARATION;
    node->data.struct_declaration.typeName = name;
    node->data.struct_declaration.decl_list = decl_list;
    return node; 
}

AS_TREE* make_input(AS_TREE* id)
{
    AS_TREE* node = new AS_TREE;
    node->lineno = yylineno;
    node->type = Type::INPUT;
    node->data.input.input_to = id;

    AS_TREE* wrap = new AS_TREE;
    wrap->lineno = yylineno;
    wrap->type = Type::EXPRESSION;
    wrap->data.expression.left = node;
    wrap->data.expression.op = Operation::OP_NOTHING;
    return wrap;
}

AS_TREE* make_conversion(AS_TREE* expr, Type type)
{
    AS_TREE* node = new AS_TREE;
    node->lineno = yylineno;
    node->type = type;
    node->data.conversion.expr = expr;

    AS_TREE* wrap = new AS_TREE;
    wrap->lineno = yylineno;
    wrap->type = Type::EXPRESSION;
    wrap->data.expression.left = node;
    wrap->data.expression.op = Operation::OP_NOTHING;
    return wrap; 
}


AS_TREE* make_output(AS_TREE* expr)
{
    AS_TREE* node = new AS_TREE;
    node->lineno = yylineno;
    node->type = Type::OUTPUT;
    node->data.output.expr = expr;
    return node;
}

AS_TREE* make_if(AS_TREE* expr, AS_TREE* st1, AS_TREE* st2)
{
    AS_TREE* node = new AS_TREE;
    node->lineno = yylineno;
    node->type = Type::IF_STATEMENT;
    node->data.if_statement.expr = expr;
    node->data.if_statement.stmts1 = st1;
    node->data.if_statement.stmts2 = st2;
    return node;
}

AS_TREE* make_while(AS_TREE* expr, AS_TREE* st)
{
    AS_TREE* node = new AS_TREE;
    node->lineno = yylineno;
    node->type = Type::WHILE_STATEMENT;
    node->data.while_statement.expr = expr;
    node->data.while_statement.stmts = st;
    return node;   
}

AS_TREE* make_for(AS_TREE* as1, AS_TREE* expr, AS_TREE* as2, AS_TREE* stmts)
{
    AS_TREE* node = new AS_TREE;
    node->lineno = yylineno;
    node->type = Type::FOR_LOOP;
    node->data.for_loop.assign1 = as1;
    node->data.for_loop.assign2 = as2;
    node->data.for_loop.expr = expr;
    node->data.for_loop.stmts = stmts;
    return node;
}

AS_TREE* make_flow_break(Type type, AS_TREE* expr)
{
    AS_TREE* node = new AS_TREE;
    node->lineno = yylineno;
    node->type = type;
    node->data.return_value.value = expr;
    return node;
}

AS_TREE* make_function_decl_item(std::string* type, std::string* id)
{
    AS_TREE* node = new AS_TREE;
    node->lineno = yylineno;
    node->type = Type::FUNCTION_DECL_ITEM;
    node->data.function_decl_item.type = type;
    node->data.function_decl_item.id = id;
    return node;
}

AS_TREE* make_function_decl_list(AS_TREE* orig, AS_TREE* val)
{
    if (orig == nullptr)
    {
        orig = new AS_TREE;
        orig->lineno = yylineno;
        orig->type = Type::FUNCTION_DECL_LIST;
        orig->data.function_decl_list.list = new std::vector<AS_TREE*>;
    }
    orig->data.function_decl_list.list->push_back(val);
    return orig;
}

AS_TREE* make_function_declaration(std::string* id, std::string* returnType, AS_TREE* decl_list, AS_TREE* stmts)
{
    AS_TREE* node = new AS_TREE;
    node->lineno = yylineno;
    node->type = Type::FUNCTION_DECL;
    node->data.function_decl.id = id;
    node->data.function_decl.returnType = returnType;
    node->data.function_decl.decl_list = decl_list;
    node->data.function_decl.stmts = stmts;
    
    #ifdef DEBUG_MODE
        printf("Function declaration!\n");
        std::vector<AS_TREE*> v = (*decl_list->data.function_decl_list.list);
        for (auto p : v)
        {
            printf("\tDECL: %s %s\n", p->data.function_decl_item.type->c_str(),
                p->data.function_decl_item.id->c_str());
        }
        printf("\n");
    #endif
    return node;
}

AS_TREE* make_expression_list(AS_TREE* orig, AS_TREE* val)
{
    if (orig == nullptr)
    {
        orig = new AS_TREE;
        orig->lineno = yylineno;
        orig->type = Type::EXPRESSION_LIST;
        orig->data.expression_list.list = new std::vector<AS_TREE*>;
    }
    orig->data.expression_list.list->push_back(val);
    return orig; 
}

AS_TREE* make_function_call(std::string* id, AS_TREE* list)
{
    AS_TREE* node = new AS_TREE;
    node->lineno = yylineno;
    node->type = Type::FUNCTION_CALL;
    node->data.function_call.id = id;
    node->data.function_call.list = list;

    AS_TREE* wrap = new AS_TREE;
    wrap->lineno = yylineno;
    wrap->type = Type::EXPRESSION;
    wrap->data.expression.left = node;
    wrap->data.expression.op = Operation::OP_NOTHING;
    return wrap;
}
