
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
    DECL_LIST           = 8,
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
    EXPRESSION_LIST     = 25
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
    } data;
};

/* PARSER */
std::string* make_type(std::string type);
AS_TREE* make_statements(AS_TREE* orig, AS_TREE* val);
AS_TREE* make_assignment(AS_TREE* var, AS_TREE* value);
AS_TREE* make_incdec(AS_TREE* var, AS_TREE* delta, Operation op);
AS_TREE* make_expression(AS_TREE* exp1, AS_TREE* exp2, Operation op);
AS_TREE* make_value(std::string type, void* data);
AS_TREE* make_value(AS_TREE* val);
AS_TREE* make_identifier(AS_TREE* orig, std::string* name);
AS_TREE* make_variable(std::string* type, std::string* name, bool isConst, AS_TREE* value);
AS_TREE* make_declaration_list(AS_TREE* orig, AS_TREE* val);
AS_TREE* make_struct_declaration(std::string* name, AS_TREE* decl_list);
AS_TREE* make_input(AS_TREE* id);
AS_TREE* make_output(AS_TREE* expr);
AS_TREE* make_if(AS_TREE* expr, AS_TREE* st1, AS_TREE* st2);
AS_TREE* make_while(AS_TREE* expr, AS_TREE* st);
AS_TREE* make_for(AS_TREE* as1, AS_TREE* expr, AS_TREE* as2, AS_TREE* stmts);
AS_TREE* make_flow_break(Type type, AS_TREE* expr);

AS_TREE* make_function_decl_list(AS_TREE* orig, AS_TREE* val);
AS_TREE* make_function_decl_item(std::string* type, std::string* id);
AS_TREE* make_function_declaration(std::string* id, std::string* returnType, AS_TREE* decl_list, AS_TREE* stmts);
AS_TREE* make_function_call(std::string* id, AS_TREE* list);
AS_TREE* make_expression_list(AS_TREE* orig, AS_TREE* val);
/* PARSER */

/* HANDLER */
AS_TREE* executeStatements(AS_TREE* tree);
AS_TREE* executeStatement(AS_TREE* tree);

Language::Value* executeExpression(AS_TREE* tree);
Language::Value* executeBool(Language::Value* v1, Language::Value* v2, Operation op);
Language::Value* executeMath(Language::Value* v1, Language::Value* v2, Operation op);
Language::Value* executeAddition(Language::Value* first, Language::Value* second);
Language::Value* executeSubstraction(Language::Value* first, Language::Value* second);
Language::Value* executeDivision(Language::Value* first, Language::Value* second);
Language::Value* executeReminder(Language::Value* first, Language::Value* second);
Language::Value* executeMultiplication(Language::Value* first, Language::Value* second);

Language::Variable* executeDeclaration(AS_TREE* tree);
AS_TREE* executeStructDeclaration(AS_TREE* tree);
AS_TREE* executeAssignment(AS_TREE* tree);
AS_TREE* executeOutput(AS_TREE* tree);
Language::Value* executeInput(AS_TREE* tree);
AS_TREE* executeIf(AS_TREE* tree);
AS_TREE* executeWhile(AS_TREE* tree);
AS_TREE* executeIncDec(AS_TREE* tree);
AS_TREE* executeFor(AS_TREE* tree);
AS_TREE* executeFunctionDecl(AS_TREE* tree);
Language::Value* executeFunction(AS_TREE* tree);

Language::Variable* getVar(AS_TREE* id);
/* HANDLER */

void leave();
int yyerror(const char* s);
int yyfmterror(int lineno, const char* fmt, ...);

#endif