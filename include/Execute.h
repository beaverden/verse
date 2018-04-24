#include "../include/AST.h"
#include "../include/types.h"

AS_TREE*            executeStatements(AS_TREE* tree);
AS_TREE*            executeStatement(AS_TREE* tree);

/* EXPRESSIONS */
Language::Value*    executeExpression(AS_TREE* tree);
Language::Value*    executeBool(Language::Value* v1, Language::Value* v2, Operation op);
Language::Value*    executeMath(Language::Value* v1, Language::Value* v2, Operation op);
Language::Value*    executeAddition(Language::Value* first, Language::Value* second);
Language::Value*    executeSubstraction(Language::Value* first, Language::Value* second);
Language::Value*    executeDivision(Language::Value* first, Language::Value* second);
Language::Value*    executeReminder(Language::Value* first, Language::Value* second);
Language::Value*    executeMultiplication(Language::Value* first, Language::Value* second);
/* EXPRESSIONS */

/* STATEMENTS */
Language::Variable* executeDeclaration(AS_TREE* tree);
AS_TREE*            executeStructDeclaration(AS_TREE* tree);
AS_TREE*            executeAssignment(AS_TREE* tree);
AS_TREE*            executeOutput(AS_TREE* tree);
Language::Value*    executeInput(AS_TREE* tree);
Language::Value*    executeConversion(AS_TREE* tree, Type type);
AS_TREE*            executeIf(AS_TREE* tree);
AS_TREE*            executeWhile(AS_TREE* tree);
AS_TREE*            executeIncDec(AS_TREE* tree);
AS_TREE*            executeFor(AS_TREE* tree);
AS_TREE*            executeFunctionDecl(AS_TREE* tree);
AS_TREE*            executeListAlter(AS_TREE* tree);
Language::Value*    executeFunction(AS_TREE* tree);

/* STATEMENTS */

/* HELPERS */
Language::Variable* resolveIdentifier(AS_TREE* id);
Language::Variable* getVariable(std::string name);
void                addVariable(Language::Variable* var);
Language::Variable* make_default_variable(std::string type);
Language::Variable* make_copy(Language::Variable* old);
Language::Value*    make_val_copy_of_var(Language::Variable* old);
Language::Value*    make_val_copy(Language::Value* old);
void                free_var(Language::Variable* var);
void                free_val(Language::Value* val);
/* HELPERS */


/* ERROR HANDLING */
void                leave();
int                 yyerror(const char* s);
int                 yyfmterror(int lineno, const char* fmt, ...);
/* ERROR HANDLING */