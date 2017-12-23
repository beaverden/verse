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

int yyfmterror(const char* fmt, ...)
{
	char str[200];
	va_list args;
	va_start(args, fmt);
	vsprintf(str, fmt, args);
	yyerror(str);
}

int yyerror(const char* s)
{
	printf("Error on line %d: [%s]\n",yylineno, s);
	leave();
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
        else yyfmterror("Invalid copy type");
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
        orig->type = Type::STATEMENTS;
        orig->data.statements.data = new std::vector<AS_TREE*>;
    }
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
    else yyfmterror("Invalid default type");
    return nullptr;
}

AS_TREE* make_expression(AS_TREE* exp1, AS_TREE* exp2, Operation op)
{
    AS_TREE* exp = new AS_TREE;
    exp->type = Type::EXPRESSION;
    exp->data.expression.left = exp1;
    exp->data.expression.right = exp2;
    exp->data.expression.op = op;
    return exp;
}

AS_TREE* make_value(std::string type, void* data)
{
    AS_TREE* exp = new AS_TREE;
    exp->type = Type::VALUE;
    exp->data.value.type = new std::string(type);
    exp->data.value.data = make_default(type);
    if (type == "$INT")  INT(exp->data.value.data) = INT(data);
    else if (type == "$STR") STR(exp->data.value.data) = STR(data);
    else if (type == "$BOOL") BOOL(exp->data.value.data) = BOOL(data);
    else yyfmterror("Unknown primitive expression type");
    return exp;
}

AS_TREE* make_value(AS_TREE* id)
{
    AS_TREE* val = new AS_TREE;
    val->type = VARIABLE_VALUE;
    val->data.variable_value.val = id;
}

AS_TREE* make_assignment(AS_TREE* var, AS_TREE* value)
{
    AS_TREE* asgn = new AS_TREE;
    asgn->type = Type::ASSIGNMENT;
    asgn->data.assignment.var = var;
    asgn->data.assignment.value = value;
    return asgn;
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
        orig->type = Type::IDENTIFIER;
        orig->data.identifier.ids = new std::vector<std::string*>;
    }
    orig->data.identifier.ids->push_back(name);
    return orig;
}

AS_TREE* make_variable(std::string* type, std::string* name, bool isConst, AS_TREE* value)
{
    AS_TREE* var = new AS_TREE;
    var->type = Type::DECLARATION;
    var->data.declaration.type = type;
    var->data.declaration.name = name;
    var->data.declaration.isConst = isConst;
    var->data.declaration.value = value;
    return var;
}

AS_TREE* make_declaration_list(AS_TREE* orig, AS_TREE* val)
{
    if (orig == nullptr)
    {
        orig = new AS_TREE;
        orig->type = Type::DECL_LIST;
        orig->data.declaration_list.list = new std::vector<AS_TREE*>;
    }
    orig->data.declaration_list.list->push_back(val);
    return orig;
}

AS_TREE* make_struct_declaration(std::string* name, AS_TREE* decl_list)
{
    AS_TREE* st = new AS_TREE;
    st->type = Type::STRUCT_DECLARATION;
    st->data.struct_declaration.typeName = name;
    st->data.struct_declaration.decl_list = decl_list;
    return st; 
}

AS_TREE* make_input(AS_TREE* id)
{
    AS_TREE* in = new AS_TREE;
    in->type = Type::INPUT;
    in->data.input.input_to = id;
    return in;
}

AS_TREE* make_output(AS_TREE* expr)
{
    AS_TREE* out = new AS_TREE;
    out->type = Type::OUTPUT;
    out->data.output.expr = expr;
    return out;
}

void executeStatements(AS_TREE* tree)
{
    if (tree->type == Type::STATEMENTS)
    {
        printf("STATEMENTS ARRAY\n");
        for (auto p : AS_VECTOR(tree->data.statements.data))
        {
            executeStatement(p);
        }
    }
}

void executeStatement(AS_TREE* tree)
{
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
        default:
            yyfmterror("Unknown node type!");
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
                 || op == OP_DIVIDED || op == OP_REMINDING || op == OP_EMPOWERING)
        {
            return executeMath(v1, v2, op);
        } else yyfmterror("Unknown node operation");
    } else yyfmterror("Unknown expression type");
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
            if (v1->type == "$BOOL") yyfmterror("Cannot use lower on boolean type");
        }
        if (op == Operation::OP_GREATER)
        {
            if (v1->type == "$INT") (*result) = (INT(v1->data) > INT(v2->data));
            if (v1->type == "$STR") (*result) = (STR(v1->data) > STR(v2->data));
            if (v1->type == "$BOOL") yyfmterror("Cannot use greater on boolean type");
        }
        if (op == Operation::OP_LOWEREQ)
        {
            if (v1->type == "$INT") (*result) = (INT(v1->data) <= INT(v2->data));
            if (v1->type == "$STR") (*result) = (STR(v1->data) <= STR(v2->data));
            if (v1->type == "$BOOL") yyfmterror("Cannot use lower equal on boolean type");
        }
        if (op == Operation::OP_GREATEREQ)
        {
            if (v1->type == "$INT") (*result) = (INT(v1->data) >= INT(v2->data));
            if (v1->type == "$STR") (*result) = (STR(v1->data) >= STR(v2->data));
            if (v1->type == "$BOOL") yyfmterror("Cannot use greater equal on boolean type");
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
        case Operation::OP_DIVIDED:
            return executeDivision(v1, v2);
            break;
        case Operation::OP_REMINDING:
            return executeReminder(v1, v2);
            break;
        case Operation::OP_EMPOWERING:
            return executeMultiplication(v1, v2);
            break;
        default:
            yyfmterror("Invalid operation for math");
    }
}

Language::Variable* executeDeclaration(AS_TREE* tree)
{
    // TODO FIX SCOPE ISSUES
    std::string tempName = (*tree->data.declaration.name);
	if (variables.find(tempName) != variables.end())
	{
		yyfmterror("Two variables with the same name in the same scope");
	}
	Language::Variable* newVar = new Language::Variable;
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
                yyfmterror("Invalid type %s", newVar->type.c_str());
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
}
void executeAssignment(AS_TREE* tree)
{
};
void executeOutput(AS_TREE* tree)
{
    Language::Value* var = executeExpression(tree->data.output.expr);
    if (var->type == "$STR") printf("[QUOTE] %s\n", STR(var->data).c_str());
    if (var->type == "$INT") printf("[QUOTE] %d\n", INT(var->data));
    if (var->type == "$BOOL") printf("[QUOTE] %s\n", (BOOL(var->data) == true) ? "indeed" : "untruth");			
    free_var(var);
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
        yyfmterror("Invalid types for adding");
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
        yyfmterror("Invalid types for substraction");
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
        yyfmterror("Invalid types for division");
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
        yyfmterror("Invalid types for reminding");
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
        yyfmterror("Invalid types for empowering");
    }
    free_var(first);
    free_var(second);
    return res;
}