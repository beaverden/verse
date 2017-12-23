#include "types.h"
#include <stdarg.h>

#ifndef CODE_H
#define CODE_H
#define DEBUG_MODE


/* IMPORTANT VARIABLES */


Language::Variable* make_variable(std::string type, std::string name, bool isConst, Language::Variable* value = NULL);
std::string* make_type(std::string name);
Language::Variable* make_expression(std::string type, void* value = NULL);
Language::Variable* make_unnamed(Language::Variable* proto);
void* make_default(std::string type);

Language::Variable* make_addition(Language::Variable* first, Language::Variable* second);
Language::Variable* make_substraction(Language::Variable* first, Language::Variable* second);
Language::Variable* make_division(Language::Variable* first, Language::Variable* second);
Language::Variable* make_reminder(Language::Variable* first, Language::Variable* second);
Language::Variable* make_multiplication(Language::Variable* first, Language::Variable* second);
Language::Variable* make_boolean(Language::Variable* first, Language::Variable* second, Language::BOOL_OP op);

bool make_if(Language::Variable* var);

void make_assign(Language::Variable* var, Language::Variable* value);
std::vector<Language::Variable*>* make_list(Language::Variable* initial, std::vector<Language::Variable*>* old);
void make_struct(std::string typeName, std::vector<Language::Variable*>* vars);
Language::Variable* make_print(Language::Variable* var);
Language::Variable* make_input(Language::Variable* var);

Language::Variable* get_var(std::string name, Language::Variable* complex = NULL);


Language::Variable* make_copy(Language::Variable* old);

void add_scope(std::string name);
void pop_scope();
void free_var(Language::Variable* var);



void init();
#endif