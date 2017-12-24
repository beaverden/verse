#include "../include/AST.h"

static std::stack<std::string> currentScope;
static std::map<std::string, Language::Variable*> variables;
static std::map<std::string, Language::ComplexType*> complexTypes;
static unsigned int temp_count = 0;

void leave()
{
	exit(-1);
}

void init()
{

}

void free_var(Language::Value* val) {}

int yyfmterror(int lineno, const char* fmt, ...)
{
	char str[200];
	va_list args;
	va_start(args, fmt);
	vsprintf(str, fmt, args);
    if (lineno != -1)
    {
        printf("Error on line %d: [%s]\n",lineno, str);
    }
    else
    {
        printf("Error [%s]\n", str);
    }
    
	leave();
}

int yyerror(const char* s)
{	
    printf("Syntax error on line %d\n", yylineno);
	leave();
}

Language::Variable* getVar(AS_TREE* tree)
{
    if (tree->data.identifier.from == nullptr)
    {
        std::string name = *(tree->data.identifier.name);
        if (variables.find(name) == variables.end())
            yyfmterror(tree->lineno, "Variable %s doesn't exist", name.c_str());
        return variables[name];
    }
    else 
    {
        Language::Variable* var = getVar(tree->data.identifier.from);
        if (!var->isComplex)
        {
            yyfmterror(tree->lineno, "Variable %s not complex", var->name.c_str());
        }
        Language::ComplexType* typ = (Language::ComplexType*)(var->data);
        std::string name = (*tree->data.identifier.name);
        if (typ->vars.find(name) == typ->vars.end())
        {
            yyfmterror(tree->lineno, "Variable %s has no member %s", var->name.c_str(), name.c_str());
        }
        return typ->vars[name];
    }
}

Language::Value* copyValue(Language::Variable* old)
{
    unsigned char* newData;
    Language::Value* newVal = new Language::Value;
    newVal->type = old->type;

    if (!old->isComplex)
    {       
        if (old->type == "$INT")
        {
            newData = new unsigned char[sizeof(int)];
            INT(newData) = INT(old->data);
            
        }
        else if (old->type == "$STR") 
        {
            std::string* newStr = new std::string;
            (*newStr) = STR(old->data);
            newData = (unsigned char*)(newStr);
        }
        else if (old->type == "$BOOL") 
        {
            newData = new unsigned char[sizeof(bool)];
            BOOL(newData) = BOOL(old->data);
        }
        else if (old->type == "$VOID") 
        {
            // IGNORE   
        }
        else yyfmterror(-1, "Invalid copy type");
        newVal->data = (void*)(newData);
    }
    else
    {
        // TODO complex assign
        yyfmterror(old->lineno, "Can't assign a complex type");
    }
    return newVal;   
}

Language::Variable* make_copy(Language::Variable* old)
{
    unsigned char* newData;
    Language::Variable* newVar = new Language::Variable;
    newVar->type = old->type;
    //printf("TYPE %s\n", newVar->type.c_str());
    newVar->name = old->name;
    newVar->scope = old->scope;
    newVar->scopedName = old->scopedName;
    newVar->isConstant = old->isConstant;
    newVar->isComplex = old->isComplex;

    if (!old->isComplex)
    {       
        if (old->type == "$INT")
        {
            newData = new unsigned char[sizeof(int)];
            INT(newData) = INT(old->data);
            
        }
        else if (old->type == "$STR") 
        {
            std::string* newStr = new std::string;
            (*newStr) = STR(old->data);
            newData = (unsigned char*)(newStr);
        }
        else if (old->type == "$BOOL") 
        {
            newData = new unsigned char[sizeof(bool)];
            BOOL(newData) = BOOL(old->data);
        }
        else if (old->type == "$VOID") 
        {
            // IGNORE   
        }
        else yyfmterror(-1, "Invalid copy type");
        newVar->data = (void*)(newData);
    }
    else
    {
        Language::ComplexType* newType = new Language::ComplexType;
        Language::ComplexType* oldType = (Language::ComplexType*)(old->data);
        newType->typeName = oldType->typeName;
        for (auto p : oldType->vars)
        {
            newType->vars[p.first] = make_copy(p.second);
        }
        newVar->data = (void*)(newType);
    }
    return newVar;
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

void* make_default(std::string type)
{
    if (type == "$INT")
    {
        return new int(0);
    }
    else if (type == "$STR")
    {
        return new std::string("");
    }
    else if (type == "$BOOL")
    {
        return new bool(false);
    }
    else yyfmterror(-1, "Invalid default type");
    return nullptr;
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

AS_TREE* make_value(std::string type, void* data)
{
    AS_TREE* node = new AS_TREE;
    node->lineno = yylineno;
    node->type = Type::VALUE;
    node->data.value.type = new std::string(type);
    node->data.value.data = make_default(type);
    if (type == "$INT")  INT(node->data.value.data) = INT(data);
    else if (type == "$STR") STR(node->data.value.data) = STR(data);
    else if (type == "$BOOL") BOOL(node->data.value.data) = BOOL(data);
    else yyfmterror(-1, "Unknown primitive expression type");

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

AS_TREE* make_declaration_list(AS_TREE* orig, AS_TREE* val)
{
    if (orig == nullptr)
    {
        orig = new AS_TREE;
        orig->lineno = yylineno;
        orig->type = Type::DECL_LIST;
        orig->data.declaration_list.list = new std::vector<AS_TREE*>;
    }
    val->lineno = yylineno;
    orig->data.declaration_list.list->push_back(val);
    return orig;
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


void executeStatements(AS_TREE* tree)
{
    if (tree == nullptr) return;
    if (tree->type == Type::STATEMENTS)
    {
        for (auto p : AS_VECTOR(tree->data.statements.data))
        {
            executeStatement(p);
        }
    }
}

void executeStatement(AS_TREE* tree)
{
    if (tree == nullptr) return;
    switch (tree->type)
    {
        case Type::EXPRESSION:
            executeExpression(tree);
            break;
        case Type::DECLARATION:
            executeDeclaration(tree);
            break;
        case Type::STRUCT_DECLARATION:
            executeStructDeclaration(tree);
            break;
        case Type::ASSIGNMENT:
            executeAssignment(tree);
            break;
        case Type::OUTPUT:
            executeOutput(tree);
            break;
        case Type::IF_STATEMENT:
            executeIf(tree);
            break;
        case Type::WHILE_STATEMENT:
            executeWhile(tree);
            break;
        case Type::INCDEC:
            executeIncDec(tree);
            break;
        case Type::FOR_LOOP:
            executeFor(tree);
            break;
        default:
            yyfmterror(tree->lineno, "Unknown node type!");
    }
}

Language::Value* executeExpression(AS_TREE* tree)
{
    if (tree == nullptr) return nullptr;
    if (tree->type == Type::VALUE)
    {
        Language::Value* v = new Language::Value;
        v->type = (*tree->data.value.type);
        v->data = tree->data.value.data;
        return v;
    }
    else if (tree->type == Type::VARIABLE_VALUE)
    {
        Language::Variable* original = getVar(tree->data.variable_value.val);
        Language::Value* newVal = copyValue(original);
        return newVal;
    }
    else if (tree->type == Type::INPUT)
    {
        return executeInput(tree->data.input.input_to);
    }
    else if (tree->type == Type::EXPRESSION)
    {
        Language::Value* v1 = executeExpression(tree->data.expression.left);
        Language::Value* v2 = executeExpression(tree->data.expression.right);
        Operation op = tree->data.expression.op;
        if (  op == OP_EQUAL || op == OP_LOWER || op == OP_GREATER
           || op == OP_LOWEREQ || op == OP_GREATEREQ || op == OP_NEQUAL)
        {
            return executeBool(v1, v2, op);
        }
        else if (   op == OP_ADDING || op == OP_CONSPIRING 
                 || op == OP_DIVIDING || op == OP_REMINDING || op == OP_EMPOWERING)
        {
            return executeMath(v1, v2, op);
        } 
        else if ( op == OP_NOTHING)
        {
            return v1;
        }
        else yyfmterror(tree->lineno, "Unknown node operation");
    } else yyfmterror(tree->lineno, "Unknown expression type");
}

Language::Value* executeBool(Language::Value* v1, Language::Value* v2, Operation op)
{
    Language::Value* res = new Language::Value;
    res->type = "$BOOL";
    if (v1->type == v2->type)
    {
        bool* result = new bool(false);
        if (op == Operation::OP_EQUAL)
        {
            if (v1->type == "$INT") (*result) = (INT(v1->data) == INT(v2->data));
            if (v1->type == "$STR") (*result) = (STR(v1->data) == STR(v2->data));
            if (v1->type == "$BOOL") (*result) = (BOOL(v1->data) == BOOL(v2->data));
        }
        if (op == Operation::OP_NEQUAL)
        {
            if (v1->type == "$INT") (*result) = (INT(v1->data) != INT(v2->data));
            if (v1->type == "$STR") (*result) = (STR(v1->data) != STR(v2->data));
            if (v1->type == "$BOOL") (*result) = (BOOL(v1->data) != BOOL(v2->data));
        }
        if (op == Operation::OP_LOWER)
        {
            if (v1->type == "$INT") (*result) = (INT(v1->data) < INT(v2->data));
            if (v1->type == "$STR") (*result) = (STR(v1->data) < STR(v2->data));
            if (v1->type == "$BOOL") yyfmterror(v1->lineno, "Cannot use lower on boolean type");
        }
        if (op == Operation::OP_GREATER)
        {
            if (v1->type == "$INT") (*result) = (INT(v1->data) > INT(v2->data));
            if (v1->type == "$STR") (*result) = (STR(v1->data) > STR(v2->data));
            if (v1->type == "$BOOL") yyfmterror(v1->lineno, "Cannot use greater on boolean type");
        }
        if (op == Operation::OP_LOWEREQ)
        {
            if (v1->type == "$INT") (*result) = (INT(v1->data) <= INT(v2->data));
            if (v1->type == "$STR") (*result) = (STR(v1->data) <= STR(v2->data));
            if (v1->type == "$BOOL") yyfmterror(v1->lineno, "Cannot use lower equal on boolean type");
        }
        if (op == Operation::OP_GREATEREQ)
        {
            if (v1->type == "$INT") (*result) = (INT(v1->data) >= INT(v2->data));
            if (v1->type == "$STR") (*result) = (STR(v1->data) >= STR(v2->data));
            if (v1->type == "$BOOL") yyfmterror(v1->lineno, "Cannot use greater equal on boolean type");
        }
        res->data = (void*)(result);
    }
    
}

Language::Value* executeMath(Language::Value* v1, Language::Value* v2, Operation op)
{
    switch (op)
    {
        case Operation::OP_ADDING:
            return executeAddition(v1, v2);
            break;
        case Operation::OP_CONSPIRING:
            return executeSubstraction(v1, v2);
            break;
        case Operation::OP_DIVIDING:
            return executeDivision(v1, v2);
            break;
        case Operation::OP_REMINDING:
            return executeReminder(v1, v2);
            break;
        case Operation::OP_EMPOWERING:
            return executeMultiplication(v1, v2);
            break;
        default:
            yyfmterror(v1->lineno, "Invalid operation for math");
    }
}

Language::Variable* executeDeclaration(AS_TREE* tree)
{
    if (tree == nullptr) return nullptr;
    // TODO FIX SCOPE ISSUES
    std::string tempName = (*tree->data.declaration.name);
	if (variables.find(tempName) != variables.end())
	{
		yyfmterror(tree->lineno, "Two variables with the same name in the same scope");
	}
	Language::Variable* newVar = new Language::Variable;
    newVar->lineno = tree->lineno;
	newVar->type = (*tree->data.declaration.type);
	newVar->name = (*tree->data.declaration.name);
	newVar->isConstant = tree->data.declaration.isConst;

    Language::Value* value = executeExpression(tree->data.declaration.value);
	if (newVar->type.at(0) == '$') 
	{
		newVar->isComplex = false;
        if (value == nullptr)
        {
            newVar->data = make_default(newVar->type);
        }
		else
        {
            newVar->data = value->data;
        }
	} 
	else
	{
		newVar->isComplex = true;
        if (value == nullptr)
        {
            if (complexTypes.find(newVar->type) == complexTypes.end())
            {
                yyfmterror(tree->lineno, "Invalid type %s", newVar->type.c_str());
            }
            Language::ComplexType* t = new Language::ComplexType;
            Language::ComplexType* model = complexTypes[newVar->type];
            t->typeName = model->typeName;
            for (auto v : model->vars)
            {
                t->vars[v.first] = make_copy(v.second);
            }
            newVar->data = (void*)t;
        }
        else 
        {
            newVar->data = value->data;
        }

	}
	#ifdef DEBUG_MODE
		printf("Variable:\n\tTYPE: %s\n\tNAME: %s\n", newVar->type.c_str(), newVar->name.c_str());
	#endif
	variables[tempName] = newVar;
	return newVar;
}

void executeStructDeclaration(AS_TREE* tree)
{
    if (tree->type == Type::STRUCT_DECLARATION)
    {
        std::string typeName = (*tree->data.struct_declaration.typeName);
        if (complexTypes.find(typeName) != complexTypes.end())
        {
            yyfmterror(tree->lineno, "There already exists a type %s", typeName.c_str());
        }
        AS_TREE* decl_list = tree->data.struct_declaration.decl_list;
        std::vector<AS_TREE*> dlist = (*decl_list->data.declaration_list.list);
        Language::ComplexType* typ = new Language::ComplexType;
        typ->typeName = typeName;
        for (auto p: dlist)
        {
            Language::Variable* var = executeDeclaration(p);
            typ->vars[var->name] = var;
        }
        complexTypes[typeName] = typ;
        #ifdef DEBUG_MODE
            printf("Complex type [%s] defined:\n", typ->typeName.c_str());
            for (auto p : typ->vars)
            {
                printf("\t%s %s;\n", p.second->type.c_str(), p.first.c_str());				
            }
        #endif
    }
    else yyfmterror(tree->lineno, "Invalid type struct declaration");
}
void executeAssignment(AS_TREE* tree)
{
    if (tree->type == Type::INCDEC) 
    {
        executeIncDec(tree);
    }
    else
    {
        Language::Variable* var = getVar(tree->data.assignment.var);
        Language::Value* val = executeExpression(tree->data.assignment.value);
        if (var->type != val->type)
        {
            yyfmterror(tree->lineno, "Type of %s is compatible with the expression", var->name.c_str());
        }
        if (var->isConstant)
        {
            yyfmterror(tree->lineno, "Variable %s is constant", var->name.c_str());
        }
        var->data = val->data;
    }
    

};

void executeOutput(AS_TREE* tree)
{
    Language::Value* var = executeExpression(tree->data.output.expr);
    if (var->type == "$STR") printf("[QUOTE] %s\n", STR(var->data).c_str());
    if (var->type == "$INT") printf("[QUOTE] %d\n", INT(var->data));
    if (var->type == "$BOOL") printf("[QUOTE] %s\n", (BOOL(var->data) == true) ? "indeed" : "untruth");			
    free_var(var);
}

Language::Value* executeInput(AS_TREE* tree)
{
    Language::Variable* var = getVar(tree);
    if (var->isConstant) yyfmterror(tree->lineno, "Can't read to constant variable");
    if (var->isComplex || var->type == "$VOID" || var->type == "$LIST")
    {
        yyfmterror(tree->lineno, "Can't read into %s: incorrect type", var->name.c_str());
    }
    if (var->type == "$STR")
    {
        std::string* val = new std::string;
        std::cin >> (*val);
        var->data = (void*)val;
    }
    else if (var->type == "$INT")
    {
        int* val = new int;
        std::cin >> (*val);
        var->data = (void*)val;
    }
    else if (var->type == "$BOOL")
    {
        bool* val = new bool;
        std::cin >> (*val);
        var->data = (void*)val;
    }
    else
    {
        yyfmterror(tree->lineno, "Urecognized type");
    }
    Language::Value* val = copyValue(var);
    return val;
}

void executeIf(AS_TREE* tree)
{
    Language::Value* val = executeExpression(tree->data.if_statement.expr);
    if (val->type != "$BOOL")
    {
        yyfmterror(tree->lineno, "If expression must be boolean");
    }
    if (BOOL(val->data) == true)
    {
        executeStatements(tree->data.if_statement.stmts1);
    }
    else
    {
        executeStatements(tree->data.if_statement.stmts2);
    }
}

void executeWhile(AS_TREE* tree)
{
    if (tree == nullptr) return;
    while (true)
    {
        Language::Value* val = executeExpression(tree->data.while_statement.expr);
        if (val->type != "$BOOL")
        {
            yyfmterror(tree->lineno, "While expression must be boolean");
        }
        if (BOOL(val->data) == true)
        {
            executeStatements(tree->data.while_statement.stmts);
        }
        else
        {
            break;
        }
    }
}

void executeFor(AS_TREE* tree)
{
    if (tree == nullptr) return;
    executeAssignment(tree->data.for_loop.assign1);
    Language::Value* val = executeExpression(tree->data.for_loop.expr);
    while (true)
    {
        if (val->type != "$BOOL")
        {
            yyfmterror(tree->lineno, "For expression must be boolean");
        }
        if (BOOL(val->data) == false) break;
        executeStatements(tree->data.for_loop.stmts);
        executeAssignment(tree->data.for_loop.assign2);
        val = executeExpression(tree->data.for_loop.expr);
    }
}

void executeIncDec(AS_TREE* tree)
{
    if (tree == nullptr) return;
    Language::Variable* var = getVar(tree->data.incdec.var);
    Language::Value* val2 = executeExpression(tree->data.incdec.expr);
    Language::Value* val1 = copyValue(var);
    Language::Value* result = executeMath(val1, val2, tree->data.incdec.op);
    //printf("%s %d\n", var->name.c_str(), INT(result->data));
    if (var->type != result->type)
    {
        yyfmterror(tree->lineno, "Expression with different type");
    }
    var->data = result->data;
}

Language::Value* executeAddition(Language::Value* first, Language::Value* second)
{
    Language::Value* res = new Language::Value;
    if (first->type == "$INT" && second->type == "$INT")
    {
        res->type = "$INT";
        int* val = new int;
        (*val) = INT(first->data) + INT(second->data);
        res->data = (void*)val;
    }
    else if (first->type == "$STR" && second->type == "$STR") 
    {
        res->type = "$STR";
        std::string* val = new std::string;
        (*val) = STR(first->data) + STR(second->data);
        res->data = (void*)val;
    }
    else
    {
        yyfmterror(first->lineno, "Invalid types for adding");
    }
    free_var(first);
    free_var(second);
    return res;
}

Language::Value* executeSubstraction(Language::Value* first, Language::Value* second)
{
    Language::Value* res = new Language::Value;
    if (first->type == "$INT" && second->type == "$INT")
    {
        res->type = "$INT";
        int* val = new int;
        (*val) = INT(first->data) - INT(second->data);
        res->data = (void*)val;
    }
    else
    {
        yyfmterror(first->lineno, "Invalid types for substraction");
    }
    free_var(first);
    free_var(second);
    return res;
}

Language::Value* executeDivision(Language::Value* first, Language::Value* second)
{
    Language::Value* res = new Language::Value;
    if (first->type == "$INT" && second->type == "$INT")
    {
        res->type = "$INT";
        int* val = new int;
        (*val) = INT(first->data) / INT(second->data);
        res->data = (void*)val;
    }
    else
    {
        yyfmterror(first->lineno, "Invalid types for division");
    }
    free_var(first);
    free_var(second);
    return res;
}

Language::Value* executeReminder(Language::Value* first, Language::Value* second)
{
    Language::Value* res = new Language::Value;
    if (first->type == "$INT" && second->type == "$INT")
    {
        res->type = "$INT";
        int* val = new int;
        (*val) = INT(first->data) % INT(second->data);
        res->data = (void*)val;
    }
    else
    {
        yyfmterror(first->lineno, "Invalid types for reminding");
    }
    free_var(first);
    free_var(second);
    return res;
}

Language::Value* executeMultiplication(Language::Value* first, Language::Value* second)
{
    Language::Value* res = new Language::Value;
    if (first->type == "$INT" && second->type == "$INT")
    {
        res->type = "$INT";
        int* val = new int;
        (*val) = INT(first->data) * INT(second->data);
        res->data = (void*)val;
    }
    else if (  (first->type == "$INT" && second->type == "$STR")
			|| (first->type == "$STR" && second->type == "$INT"))
		{
            res->type = "$STR";
			std::string str = "";
			int value = 0;

			if (first->type == "$STR") 
            { 
                str = STR(first->data);
                value = INT(second->data);
            }
			else if (first->type == "$INT") 
            { 
                str = STR(second->data);
                value = INT(first->data);
            }
            std::string* res_str = new std::string;
			(*res_str) = "";
			for (int i = 0; i < value; i++) (*res_str) += str;	
            res->data = (void*)(res_str);
		}
    else
    {
        yyfmterror(first->lineno, "Invalid types for empowering");
    }
    free_var(first);
    free_var(second);
    return res;
}