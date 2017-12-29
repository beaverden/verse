
#ifndef AST_H
#define AST_H

#include <string>
#include <string.h>
#include <map>
#include <vector>
#include <stack>	
#include <algorithm>
#include <unordered_map>
#include <set>
#include <iostream>
#include <stdarg.h>
#include "types.h"
#include <functional>

extern int yylineno;

enum Type { 
    STATEMENTS          = 0, 
    STATEMENT           = 1, 
    DECLARATION         = 2, 
    STRUCT_DECLARATION  = 3,
    EXPRESSION          = 4,
    ASSIGNMENT          = 5,
    VALUE               = 6,
    TYPE                = 7,
    EXPRESSION_LIST     = 8,
    INPUT               = 9,
    OUTPUT              = 10,
    VARIABLE_VALUE      = 11,
    IDENTIFIER          = 12,
    IF_STATEMENT        = 13,
    WHILE_STATEMENT     = 14,
    INCDEC              = 15,
    FOR_LOOP            = 16,

    BREAK               = 17,
    CONTINUE            = 18,
    RETURN              = 19,
    VOID                = 20,

    FUNCTION_DECL_LIST  = 21,
    FUNCTION_DECL_ITEM  = 22,
    FUNCTION_DECL       = 23,
    FUNCTION_CALL       = 24,
    
    LIST                = 25,
    CONV_VERBOSE        = 26,
    CONV_NUMERIC        = 27,

    LIST_ADD            = 28,
    LIST_REMOVE         = 29,
    INDEXER             = 30,
    CONV_LIST           = 31,
    LENGTH_OF           = 32
    
};

enum Operation { 
    OP_EQUAL, 
    OP_LOWER, 
    OP_GREATER, 
    OP_LOWEREQ,
    OP_GREATEREQ, 
    OP_NEQUAL,

    OP_ADDING,
    OP_CONSPIRING,
    OP_DIVIDING,
    OP_REMINDING,
    OP_EMPOWERING,

    OP_NOTHING
};



struct Expression
{
    std::string     Type;
    void*           value;
};

struct AS_TREE
{
    Type type;
    int lineno = -1;
    union
    {
        struct
        {
            std::string* type;
            void* data;
        } value;
        struct
        {
            AS_TREE* val;
        } variable_value;
        struct
        {
            AS_TREE* left;
            AS_TREE* right;
            Operation op;
        } expression;
        struct
        {
            AS_TREE* var;
            AS_TREE* value;
        } assignment;
        struct
        {
            std::vector<AS_TREE*>* data;
        } statements;
        struct
        {
            std::string* type;
            std::string* name;
            bool isConst;
            AS_TREE* value;
        } declaration;
        struct
        {
            std::vector<AS_TREE*>* list;
        } declaration_list;
        struct
        {
            std::string* typeName;
            AS_TREE* decl_list;
        } struct_declaration;
        struct
        {
            AS_TREE* input_to;
        } input;
        struct
        {
            AS_TREE* expr;
        } output;
        struct
        {
            std::string* name;
            AS_TREE* from;
        } identifier;
        struct
        {
            AS_TREE* index;
            AS_TREE* from;
        } indexer;
        struct
        {
            AS_TREE* expr;
            AS_TREE* stmts1;
            AS_TREE* stmts2;
        } if_statement;
        struct
        {
            AS_TREE* expr;
            AS_TREE* stmts;
        } while_statement;
        struct
        {
            AS_TREE* var;
            AS_TREE* expr;
            Operation op;
        } incdec;
        struct
        {
            AS_TREE* assign1;
            AS_TREE* expr;
            AS_TREE* assign2;
            AS_TREE* stmts;
        } for_loop;
        struct
        {
            AS_TREE* value;
        } return_value;
        struct
        {
            std::vector<AS_TREE*>* list;
        } function_decl_list;
        struct
        {
            bool isConstant;
            std::string* type;
            std::string* id;
        } function_decl_item;
        struct
        {
            std::string* id;
            std::string* returnType;
            AS_TREE* decl_list;
            AS_TREE* stmts;
        } function_decl;
        struct
        {
            std::string* id;
            AS_TREE* list;
        } function_call;
        struct
        {
            std::vector<AS_TREE*>* list;
        } expression_list;
        struct
        {
            AS_TREE* expr;
        } conversion;
        struct
        {
            AS_TREE* expr;
            AS_TREE* index;
            AS_TREE* id;
            Type type;
        } list_alter;
        struct
        {
            AS_TREE* expr;
        } length_of;
    } data;
};

/* PARSER */
AS_TREE* make_break();
AS_TREE* make_continue();
AS_TREE* make_return(AS_TREE* expr);
AS_TREE* make_void();
std::string* make_type(std::string type);
AS_TREE* make_statements(AS_TREE* orig, AS_TREE* val);
AS_TREE* make_assignment(AS_TREE* var, AS_TREE* value);
AS_TREE* make_incdec(AS_TREE* var, AS_TREE* delta, Operation op);
AS_TREE* make_expression(AS_TREE* exp1, AS_TREE* exp2, Operation op);
AS_TREE* make_value(std::string* type, void* data);
AS_TREE* make_value(AS_TREE* val);
AS_TREE* make_identifier(AS_TREE* orig, std::string* name);
AS_TREE* make_indexer(AS_TREE* orig, AS_TREE* index);
AS_TREE* make_variable(std::string* type, std::string* name, bool isConst, AS_TREE* value);
AS_TREE* make_declaration_list(AS_TREE* orig, AS_TREE* val);
AS_TREE* make_struct_declaration(std::string* name, AS_TREE* decl_list);
AS_TREE* make_input(AS_TREE* id);
AS_TREE* make_conversion(AS_TREE* expr, Type type);
AS_TREE* make_output(AS_TREE* expr);
AS_TREE* make_if(AS_TREE* expr, AS_TREE* st1, AS_TREE* st2);
AS_TREE* make_while(AS_TREE* expr, AS_TREE* st);
AS_TREE* make_for(AS_TREE* as1, AS_TREE* expr, AS_TREE* as2, AS_TREE* stmts);
AS_TREE* make_flow_break(Type type, AS_TREE* expr);
AS_TREE* make_list_alter(AS_TREE* expr, AS_TREE* index, AS_TREE* id, Type type);
AS_TREE* make_length(AS_TREE* expr);

AS_TREE* make_function_decl_list(AS_TREE* orig, AS_TREE* val);
AS_TREE* make_function_decl_item(std::string* type, std::string* id, bool isConst);
AS_TREE* make_function_declaration(std::string* id, std::string* returnType, AS_TREE* decl_list, AS_TREE* stmts);
AS_TREE* make_function_call(std::string* id, AS_TREE* list);
AS_TREE* make_expression_list(AS_TREE* orig, AS_TREE* val);
/* PARSER */


#endif