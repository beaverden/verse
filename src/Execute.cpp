#include "../include/Execute.h"


static std::map<std::string, Language::ComplexType*> complexTypes;
static std::map<std::string, AS_TREE*> functions;
static std::map<std::string, Language::Variable*> globalVariables;
static std::stack<std::map<std::string, Language::Variable*>> executionContext;
static Language::GlobalContext global;
static unsigned int temp_count = 0;


Language::Variable* getVariable(std::string name)
{
    if (!executionContext.empty())
    {
        if (executionContext.top().find(name) != executionContext.top().end())
        {
            return executionContext.top()[name];
        }
    }
    
    if (globalVariables.find(name) != globalVariables.end())
    {
        return globalVariables[name];
    }
    return nullptr;
}

void addVariable(Language::Variable* var)
{
    if (!executionContext.empty())
    {
        executionContext.top()[var->name] = var;
    }
    else
    {
        globalVariables[var->name] = var;
    }
}

void free_var(Language::Variable* val) {}
void free_val(Language::Value* val) {}

Language::Variable* make_default_variable(std::string type)
{
    Language::Variable* var = new Language::Variable;
    var->type = type;
    if (type.at(0) == '$')
    {
        var->isComplex = false;
        if (type == "$INT")
        {
            var->data = (void*)(new int(0));
        }
        else if (type == "$STR")
        {
            var->data = (void*)(new std::string(""));
        }
        else if (type == "$BOOL")
        {
            var->data = (void*)(new bool(false));
        }
    }
    else
    {
        var->isComplex = true;
        Language::ComplexType* proto = complexTypes[type];
        Language::Structure* st = new Language::Structure;
        for (auto p : proto->vars)
        {
            Language::Variable* addVar = make_default_variable(p.second.type);
            addVar->name = p.second.name;
            addVar->type = p.second.type;
            addVar->isConstant = p.second.isConstant;
            st->vars[addVar->name] = addVar;
            
        }
        var->data = (void*)(st);
    }
    return var;
}


Language::Variable* resolveIdentifier(AS_TREE* tree)
{
    if (tree->data.identifier.from == nullptr)
    {
        std::string name = *(tree->data.identifier.name);
        Language::Variable* v = getVariable(name);
        if (v == nullptr) 
        {
            yyfmterror(tree->lineno, "Variable %s doesn't exist", name.c_str());
        }
        return v;
    }
    else 
    {
        Language::Variable* var = resolveIdentifier(tree->data.identifier.from);
        if (!var->isComplex)
        {
            yyfmterror(tree->lineno, "Variable %s is not complex", var->name.c_str());
        }

        Language::Structure* typ = (Language::Structure*)(var->data);
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
    // TODO MAKE PROPER COPY
    unsigned char* newData;
    Language::Variable* newVar = new Language::Variable;
    newVar->type = old->type;
    newVar->name = old->name;
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
        Language::Structure* newType = new Language::Structure;
        Language::Structure* oldType = (Language::Structure*)(old->data);
        for (auto p : oldType->vars)
        {
            newType->vars[p.first] = make_copy(p.second);
        }
        newVar->data = (void*)(newType);
    }
    return newVar;
}


AS_TREE* executeStatements(AS_TREE* tree)
{
    if (tree == nullptr) return make_void();
    if (tree->type == Type::STATEMENTS)
    {
        for (auto p : AS_VECTOR(tree->data.statements.data))
        {
            AS_TREE* res = executeStatement(p);
            if (res->type == Type::BREAK) 
            {
                if (!global.inLoop)
                {
                    yyfmterror(res->lineno, "Cannot break without a loop");
                }
                return make_break();
            }
            if (res->type == Type::CONTINUE) 
            {
                if (!global.inLoop)
                {
                    yyfmterror(res->lineno, "Cannot continue without a loop");
                }
                return make_continue();
            }
            if (res->type == Type::RETURN)
            {
                return make_return(res->data.return_value.value);
            }
        }
    }
}

AS_TREE* executeStatement(AS_TREE* tree)
{
    if (tree == nullptr) return make_void();
    switch (tree->type)
    {
        case Type::EXPRESSION:
            executeExpression(tree);
            break;
        case Type::DECLARATION:
            executeDeclaration(tree);
            break;
        case Type::STRUCT_DECLARATION:
            return executeStructDeclaration(tree);
            break;
        case Type::ASSIGNMENT:
            return executeAssignment(tree);
            break;
        case Type::OUTPUT:
            return executeOutput(tree);
            break;
        case Type::IF_STATEMENT:
            return executeIf(tree);
            break;
        case Type::WHILE_STATEMENT:
            return executeWhile(tree);
            break;
        case Type::INCDEC:
            return executeIncDec(tree);
            break;
        case Type::FOR_LOOP:
            return executeFor(tree);
            break;
        case Type::BREAK:
            return tree;
        case Type::CONTINUE:
            return tree;
        case Type::RETURN:
            return tree;
        case Type::FUNCTION_DECL:
            return executeFunctionDecl(tree);
            break;
        default:
            yyfmterror(tree->lineno, "Unknown node type!");
    }
}


/* =============================== STATEMENTS =================================== */

Language::Variable* executeDeclaration(AS_TREE* tree)
{
    if (tree == nullptr) return nullptr;
    std::string name = (*tree->data.declaration.name);
	if (getVariable(name) != nullptr)
    {
		yyfmterror(tree->lineno, "Two variables with the same name in the same scope");
	}
	Language::Variable* newVar = new Language::Variable;
    newVar->lineno = tree->lineno;
	newVar->type = (*tree->data.declaration.type);
	newVar->name = (*tree->data.declaration.name);
	newVar->isConstant = (tree->data.declaration.isConst);

    Language::Value* value = executeExpression(tree->data.declaration.value);
	if (newVar->type.at(0) == '$') 
	{
		newVar->isComplex = false;
        if (value == nullptr)
        {
            Language::Variable* def_var = make_default_variable(newVar->type);
            newVar->data = def_var->data;
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
            Language::Variable* def_var = make_default_variable(newVar->type);
            newVar->data = def_var->data;
        }
        else 
        {
            yyfmterror(tree->lineno, "Can't assign value to struct");
        }

	}
	#ifdef DEBUG_MODE
		printf("Variable:\n\tTYPE: %s\n\tNAME: %s\n", newVar->type.c_str(), newVar->name.c_str());
	#endif
    addVariable(newVar);
	return newVar;
}

AS_TREE* executeStructDeclaration(AS_TREE* tree)
{
    if (global.inLoop || global.inFunction)
    {
        yyfmterror(tree->lineno, "Type declarations are not allowed in loops or functions");
    }
    if (tree->type == Type::STRUCT_DECLARATION)
    {
        std::string typeName = (*tree->data.struct_declaration.typeName);
        if (complexTypes.find(typeName) != complexTypes.end())
        {
            yyfmterror(tree->lineno, "There already exists a type %s", typeName.c_str());
        }
        
        Language::ComplexType* ctype = new Language::ComplexType;
        std::vector<AS_TREE*> list = (*tree->data.struct_declaration.decl_list->data.function_decl_list.list);
        ctype->typeName = typeName;

        for (auto p: list)
        {
            Language::PrototypeParam proto;
            proto.isConstant = p->data.function_decl_item.isConstant;
            proto.type = (*p->data.function_decl_item.type);
            proto.name = (*p->data.function_decl_item.id);
            if (ctype->vars.find(proto.name) != ctype->vars.end())
            {
                yyfmterror(tree->lineno, "There can't exist two members with the same name");
            }
            ctype->vars[proto.name] = proto;
        }
        complexTypes[typeName] = ctype;
        #ifdef DEBUG_MODE
            printf("Complex type [%s] defined:\n", ctype->typeName.c_str());
            for (auto p : ctype->vars)
            {
                printf("\t%s %s;\n", p.second.type.c_str(), p.first.c_str());				
            }
        #endif
    }
    else yyfmterror(tree->lineno, "Invalid type struct declaration");
}

AS_TREE* executeAssignment(AS_TREE* tree)
{
    if (tree->type == Type::INCDEC) 
    {
        executeIncDec(tree);
    }
    else
    {
        Language::Variable* var = resolveIdentifier(tree->data.assignment.var);
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

AS_TREE* executeOutput(AS_TREE* tree)
{
    Language::Value* var = executeExpression(tree->data.output.expr);
    if (var->type == "$STR") printf("[QUOTE] %s\n", STR(var->data).c_str());
    if (var->type == "$INT") printf("[QUOTE] %d\n", INT(var->data));
    if (var->type == "$BOOL") printf("[QUOTE] %s\n", (BOOL(var->data) == true) ? "indeed" : "untruth");			
    free_val(var);
}

Language::Value* executeInput(AS_TREE* tree)
{
    Language::Variable* var = resolveIdentifier(tree);
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

AS_TREE* executeIf(AS_TREE* tree)
{
    Language::Value* val = executeExpression(tree->data.if_statement.expr);
    if (val->type != "$BOOL")
    {
        yyfmterror(tree->lineno, "If expression must be boolean");
    }
    AS_TREE* res;
    if (BOOL(val->data) == true)
    {
        res = executeStatements(tree->data.if_statement.stmts1);
    }
    else
    {
        res = executeStatements(tree->data.if_statement.stmts2);
    }
    return res;
}

AS_TREE* executeWhile(AS_TREE* tree)
{
    if (tree == nullptr) return make_void();
    global.inLoop = true;
    while (true)
    {
        Language::Value* val = executeExpression(tree->data.while_statement.expr);
        if (val->type != "$BOOL")
        {
            yyfmterror(tree->lineno, "While expression must be boolean");
        }
        if (BOOL(val->data) == true)
        {
            AS_TREE* res = executeStatements(tree->data.while_statement.stmts);
            if (res->type == Type::BREAK) break;
            if (res->type == Type::RETURN) 
            {
                global.inLoop = false;
                return res;
            }
        }
        else
        {
            break;
        }
    }
    global.inLoop = false;
    return make_void();
}

AS_TREE* executeFor(AS_TREE* tree)
{
    if (tree == nullptr) return make_void();
    global.inLoop = true;
    executeAssignment(tree->data.for_loop.assign1);
    Language::Value* val = executeExpression(tree->data.for_loop.expr);
    while (true)
    {
        if (val->type != "$BOOL")
        {
            yyfmterror(tree->lineno, "For expression must be boolean");
        }
        if (BOOL(val->data) == false) break;
        AS_TREE* res = executeStatements(tree->data.for_loop.stmts);
        if (res->type == Type::BREAK) break;
        if (res->type == Type::RETURN) 
        {
            global.inLoop = false;
            return res;
        }
        executeAssignment(tree->data.for_loop.assign2);
        val = executeExpression(tree->data.for_loop.expr);
    }
    global.inLoop = false;
    return make_void();
}

AS_TREE* executeIncDec(AS_TREE* tree)
{
    if (tree == nullptr) return make_void();
    Language::Variable* var = resolveIdentifier(tree->data.incdec.var);
    Language::Value* val2 = executeExpression(tree->data.incdec.expr);
    Language::Value* val1 = copyValue(var);
    Language::Value* result = executeMath(val1, val2, tree->data.incdec.op);
    //printf("%s %d\n", var->name.c_str(), INT(result->data));
    if (var->type != result->type)
    {
        yyfmterror(tree->lineno, "Expression with different type");
    }
    var->data = result->data;
    return make_void();
}

AS_TREE* executeFunctionDecl(AS_TREE* tree)
{
    if (tree == nullptr) return make_void();
    std::string name = (*tree->data.function_decl.id);
    if (functions.find(name) != functions.end())
    {
        yyfmterror(tree->lineno, "Function with this name already exists");
    }
    functions[name] = tree;
    return make_void();
}

Language::Value* executeFunction(AS_TREE* tree)
{
    std::string id = (*tree->data.function_call.id);
    if (functions.find(id) == functions.end())
    {
        yyfmterror(tree->lineno, "Function %s doesn't exist", id.c_str());
    }
    AS_TREE* proto = functions[id];
    std::vector<AS_TREE*> expr_list = (*tree->data.function_call.list->data.expression_list.list);
    std::vector<AS_TREE*> param_proto = (*proto->data.function_decl.decl_list->data.function_decl_list.list);
    if (expr_list.size() != param_proto.size())
    {
        yyfmterror(tree->lineno, "Function call needs %d params", param_proto.size());
    }
    std::vector<Language::Variable*> variables;
    int paramNo = 0;
    for (AS_TREE* expr : expr_list)
    {
        std::string type = (*param_proto[paramNo]->data.function_decl_item.type);
        std::string id = (*param_proto[paramNo]->data.function_decl_item.id);
        bool isConstant = (param_proto[paramNo]->data.function_decl_item.isConstant);
        Language::Value* v = executeExpression(expr);
        if (v->type != type)
        {
            yyfmterror(tree->lineno, "Invalid [%d] parameter type for function %s", paramNo+1, id.c_str());
        }
        Language::Variable* vv = new Language::Variable;
        vv->type = type;
        vv->name = id;
        vv->isConstant = isConstant;
        vv->isComplex = false;
        vv->data = v->data;
        
        variables.push_back(vv);
        paramNo++;
    }
    #ifdef DEBUG_MODE
        printf("Function call\n");
        for (auto v : variables)
        {
            printf("\tParam: %s\n", v->type.c_str());
        }
    #endif
    std::map<std::string, Language::Variable*> newContext;
    for (auto v : variables)
    {
        newContext[v->name] = v;
    }
    executionContext.push(newContext);
    global.inFunction = true;
    AS_TREE* res = executeStatements(proto->data.function_decl.stmts);
    global.inFunction = false;
    Language::Value* ret;
    if (res->type == Type::RETURN)
    {
        ret = executeExpression(res->data.return_value.value);
    }
    else
    {
        ret = new Language::Value;
        ret->type = "$VOID";
    }
    executionContext.pop();
    return ret;
}
/* =============================== /.STATEMENTS =================================== */




/* =============================== EXPRESSIONS =================================*/
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
        Language::Variable* original = resolveIdentifier(tree->data.variable_value.val);
        Language::Value* newVal = copyValue(original);
        return newVal;
    }
    else if (tree->type == Type::FUNCTION_CALL)
    {
        return executeFunction(tree);
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
    free_val(first);
    free_val(second);
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
    free_val(first);
    free_val(second);
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
    free_val(first);
    free_val(second);
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
    free_val(first);
    free_val(second);
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
    free_val(first);
    free_val(second);
    return res;
}
/* =============================== /.EXPRESSIONS =================================*/


void leave()
{
	exit(-1);
}


int yyerror(const char* s)
{	
    printf("Syntax error on line %d\n", yylineno);
	leave();
}

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