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

void free_var(Language::Variable* val) 
{
    // TODO Implement (fuck memory management)
}
void free_val(Language::Value* val) {}

void printIntList(Language::Value* l, int depth = 0)
{
    #ifdef DEBUG_MODE
        if (l->type == "$INT")
        {
            for (int i = 0; i < depth; i++) printf("\t");
            printf("INT: %d\n", INT(l->data));
        }
        else if (l->type == "$LIST")
        {
            for (int i = 0; i < depth; i++) printf("\t");
            printf("LIST OF\n");
            std::vector<Language::Value*>* vals = (std::vector<Language::Value*>*)(l->data);
            for (auto p : (*vals))
            {
                printIntList(p, depth + 1);
            }
        }
        else printf("Not a correct type\n");
    #endif
}

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
        else if (type == "$LIST")
        {
            var->data = (void*)(new std::vector<Language::Value*>);
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
    if (tree->type == Type::IDENTIFIER)
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
    else if (tree->type == Type::INDEXER)
    {
        Language::Variable* var = resolveIdentifier(tree->data.indexer.from);
        Language::Value* indexVal = executeExpression(tree->data.indexer.index);
        if (indexVal->type != "$INT")
        {
            yyfmterror(tree->lineno, "Index type not integer");
        }
        int index = INT(indexVal->data);
        if (var->type != "$LIST")
        {
            yyfmterror(tree->lineno, "Cannot index this type");
        }
        std::vector<Language::Value*>* list = (std::vector<Language::Value*>*)(var->data);
        int listSize = list->size();
        if ((index >= listSize) ||
            (index < 0 && -1*index > listSize) ||
            (listSize == 0))
        {
            yyfmterror(tree->lineno, "Array out of bounds [%d]", index);
        }
        int real = ((index % listSize) + listSize) % listSize;
        Language::Variable* ref = new Language::Variable;
        Language::Value* val = list->at(real);
        ref->isConstant = val->isConstant;
        ref->type = val->type;
        ref->data = val->data;
        ref->isComplex = val->isComplex;
        return ref;
    }
}

Language::Value* make_val_copy_of_var(Language::Variable* old)
{
    Language::Value v;
    v.type = old->type;
    v.data = old->data;
    return make_val_copy(&v);
}


Language::Value* make_val_copy(Language::Value* old)
{
    unsigned char* newData;
    Language::Value* newVal = new Language::Value;
    newVal->type = old->type;
    newVal->isComplex = old->isComplex;
    if (old->type.at(0) == '$')
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
        else if (old->type == "$LIST")
        {
            std::vector<Language::Value*>* vals = new std::vector<Language::Value*>;
            for (auto ptr : VALVECTOR(old->data))
            {
                Language::Value* vcop = make_val_copy(ptr);
                vals->push_back(vcop);
            }
            newData = (unsigned char*)(vals);
        }   
        else if (old->type == "$VOID") 
        {
            // IGNORE   
        }
        else yyfmterror(-1, "Invalid copy type");
        newVal->data = (void*)(newData);     
    }
    return newVal;
}

Language::Variable* make_copy(Language::Variable* old)
{
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
        else if (old->type == "$LIST")
        {
            std::vector<Language::Value*>* vals = new std::vector<Language::Value*>;
            for (auto ptr : VALVECTOR(old->data))
            {
                Language::Value* vcop = make_val_copy(ptr);
                vals->push_back(vcop);
            }
            newData = (unsigned char*)(vals);
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
        case Type::LIST_ADD:
            return executeListAlter(tree);
        case Type::LIST_REMOVE:
            return executeListAlter(tree);
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

std::string getOutput(Language::Value* val)
{
    if (val->type == "$STR") return STR(val->data);
    else if (val->type == "$INT") return std::to_string(INT(val->data));
    else if (val->type == "$BOOL") return ((BOOL(val->data) == true) ? "indeed" : "untruth");			
    else if (val->type == "$LIST")
    {
        std::string res = "[";
        std::vector<Language::Value*>* values = (std::vector<Language::Value*>*)(val->data);
        if (values->size() != 0)
        {
            for (size_t i = 0; i < values->size() - 1; i++)
            {
                Language::Value* v = values->at(i);
                res += getOutput(values->at(i)) + ", ";
            }
            res += getOutput(values->back());
        }
        
        res += "]";
        return res;
    }
    else yyfmterror(0, "Invalid type for printing");
}

AS_TREE* executeOutput(AS_TREE* tree)
{
    Language::Value* var = executeExpression(tree->data.output.expr);
    std::cout << "[QUOTE] " << getOutput(var) << std::endl;
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
        (*val) = 0;
        std::cin >> (*val);
        var->data = (void*)val;
    }
    else if (var->type == "$BOOL")
    {
        bool* val = new bool;
        (*val) = false;
        std::cin >> (*val);
        var->data = (void*)val;
    }
    else
    {
        yyfmterror(tree->lineno, "Can't read into such type");
    }
    Language::Value* val = make_val_copy_of_var(var);
    return val;
}

Language::Value* executeConversion(AS_TREE* tree, Type type)
{
    Language::Value* ret = new Language::Value;
    Language::Value* val = executeExpression(tree);
    if (type == Type::CONV_VERBOSE)
    {
        ret->type = "$STR";
        ret->data = (void*)(new std::string());
        if (val->type == "$INT")
        {
            STR(ret->data) = std::to_string(INT(val->data));
        }
        else if (val->type == "$STR")
        {
            STR(ret->data) = STR(val->data);
        }
        else if (val->type == "$LIST")
        {
            STR(ret->data) = getOutput(val);
        }
        else 
        {
            yyfmterror(tree->lineno, "Invalid type for verbose conversion");
        }
    }
    else if (type == Type::CONV_NUMERIC)
    {
        ret->type = "$INT";
        ret->data = (void*)(new int());
        if (val->type == "$INT")
        {
            INT(ret->data) = INT(val->data);
        }
        else if (val->type == "$STR") {
            if (STR(val->data) == "")
            {
                yyfmterror(tree->lineno, "Can't convert empty string to number");
            }
            INT(ret->data) = atoi(STR(val->data).c_str());
        }
    }
    else if (type == Type::CONV_LIST)
    {
        ret->type = "$LIST";
        std::vector<Language::Value*>* values = new std::vector<Language::Value*>;
        if (val->type == "$INT" || val->type == "$BOOL")
        {
            values->push_back(make_val_copy(val));
        }
        else if (val->type == "$STR")
        {
            std::string* data = (std::string*)(val->data);
            for (size_t i = 0; i < data->length(); i++)
            {
                Language::Value* add = new Language::Value;
                add->isComplex = false;
                add->isConstant = false;
                add->type = "$STR";
                std::string* s = new std::string;
                (*s) = data->at(i);
                add->data = (void*)(s);
                
                values->push_back(add);
            }
        }
        ret->data = (void*)(values);
    }
    free_val(val);
    return ret;
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
    Language::Value* val1 = make_val_copy_of_var(var);
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


AS_TREE* executeListAlter(AS_TREE* tree)
{
    Language::Value* indexVal = executeExpression(tree->data.list_alter.index);
    int* index;
    if (indexVal == nullptr)
    {
        index = nullptr;
    }
    else
    {
        if (indexVal->type != "$INT")
        {
            yyfmterror(tree->lineno, "Can't use that expression as index");
        }
        index = (int*)(indexVal->data);
    }
    
    
    Language::Variable* listVar = resolveIdentifier(tree->data.list_alter.id);
    if (listVar->type != "$LIST")
    {
        yyfmterror(tree->lineno, "%s is not a list", listVar->name.c_str());
    }
    std::vector<Language::Value*>* list = (std::vector<Language::Value*>*)(listVar->data);
    int listSize = list->size();
    if (tree->type == Type::LIST_ADD)
    {     
        Language::Value* exprVal = executeExpression(tree->data.list_alter.expr);
        if (index == nullptr) list->push_back(exprVal);
        else
        {
            if (*index > listSize || *index < 0)
            {
                yyfmterror(tree->lineno, "Invalid insert point at %d", *index);
            }
            //int real = ((*index % listSize) + listSize ) % listSize;
            list->insert(list->begin() + *index, exprVal);
        }
    }
    else if (tree->type == Type::LIST_REMOVE)
    {
        if ((*index) >= listSize || (*index) < 0)
        {
            yyfmterror(tree->lineno, "Invalid chapter number %d", *index);
        }
        //int real = ((*index % listSize) + listSize ) % listSize;
        list->erase(list->begin() + *index);
    }
    return make_void();
}

Language::Value* executeLength(AS_TREE* expr)
{
    Language::Value* val = executeExpression(expr);
    Language::Value* res = new Language::Value;
    res->type = "$INT";
    res->data = (void*)(new int(0));
    if (val->type == "$LIST")
    {
        std::vector<Language::Value*>* values = (std::vector<Language::Value*>*)(val->data);
        INT(res->data) = values->size();
    }
    else if (val->type == "$STR")
    {
        std::string* s = (std::string*)(val->data);
        INT(res->data) = s->length();
    }
    else 
    {
        yyfmterror(expr->lineno, "Type [%s] doesn't have length property", val->type.c_str());
    }
    return res;
}

Language::Value* executeFunction(AS_TREE* tree)
{
    std::string id = (*tree->data.function_call.id);
    if (functions.find(id) == functions.end())
    {
        yyfmterror(tree->lineno, "Function %s doesn't exist", id.c_str());
    }
    AS_TREE* proto = functions[id];
    std::vector<AS_TREE*> expr_list;
    
    if (tree->data.function_call.list != nullptr)
    {
        expr_list = (*tree->data.function_call.list->data.expression_list.list);
    }
    std::vector<AS_TREE*> param_proto;
    if (proto->data.function_decl.decl_list != nullptr)
    {
        param_proto = (*proto->data.function_decl.decl_list->data.function_decl_list.list);
    };
    
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
            yyfmterror(tree->lineno, "Invalid [%d] parameter type [%s] for function", paramNo+1, id.c_str());
        }
        Language::Variable* vv = new Language::Variable;
        vv->type = type;
        vv->name = id;
        vv->isConstant = isConstant;
        vv->isComplex = v->isComplex;
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
    if (executionContext.size() == 1)
    {
        global.inFunction = false;
    }

    Language::Value* ret;
    if (res->type == Type::RETURN)
    {
        ret = executeExpression(res->data.return_value.value);
        if (ret->type != (*proto->data.function_decl.returnType))
        {
            yyfmterror(res->lineno, "Invalid return type");
        }
    }
    else
    {
        if ((*proto->data.function_decl.returnType) != "$VOID")
        {
            yyfmterror(res->lineno, "No return statement found");
        }
        ret = new Language::Value;
        ret->type = "$VOID";
    }
    for (auto var : executionContext.top())
    {
        free_var(var.second);
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
        if (v->type == "$LIST")
        {
            std::vector<AS_TREE*>* list = nullptr;
            if (v->data != nullptr)
            {
                list = ((AS_TREE*)(v->data))->data.expression_list.list;
            }
            std::vector<Language::Value*>* vals = new std::vector<Language::Value*>;
            if (list != nullptr)
            {
                for (auto expr : (*list))
                {                    
                    Language::Value* cVal = executeExpression(expr);
                    vals->push_back(cVal);
                }
            }
            v->data = (void*)(vals);
        }
        
        return v;
    }
    else if (tree->type == Type::VARIABLE_VALUE)
    {
        Language::Variable* original = resolveIdentifier(tree->data.variable_value.val);
        Language::Value* vv = new Language::Value;
        vv->type = original->type;
        vv->data = original->data;
        Language::Variable* copy = make_copy(original);
        Language::Value* newVal = new Language::Value;

        newVal->type = copy->type;
        newVal->data = copy->data;
        newVal->isComplex = copy->isComplex;
        
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
    else if (tree->type == Type::CONV_NUMERIC || 
             tree->type == Type::CONV_VERBOSE ||
             tree->type == Type::CONV_LIST)
    {
        return executeConversion(tree->data.conversion.expr, tree->type);
    }
    else if (tree->type == Type::LENGTH_OF)
    {
        return executeLength(tree->data.length_of.expr);
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
    else if (first->type == "$LIST" && second->type == "$LIST")
    {
        res->type = "$LIST";
        std::vector<Language::Value*>* v = new std::vector<Language::Value*>;
        (*v) = VALVECTOR(first->data);
        v->insert(v->end(), VALVECTOR(second->data).begin(), VALVECTOR(second->data).end());
        res->data = (void*)(v);
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
	if (INT(second->data) == 0) yyfmterror(first->lineno, "Can't divide by zero");
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
	if (INT(second->data) == 0) yyfmterror(first->lineno, "Can't divide by zero");
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
