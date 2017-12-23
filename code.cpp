#include "code.h"

static std::stack<std::string> currentScope;
static std::map<std::string, Language::Variable*> variables;
static std::map<std::string, Language::Type> actualTypes;
static std::map<std::string, Language::ComplexType*> complexTypes;
static unsigned int temp_count = 0;


void add_scope(std::string name)
{
    currentScope.push(name);
}

void pop_scope()
{
    currentScope.pop();
}

void free_var(Language::Variable* old)
{
    if (old->data != NULL)
    {
        if (!old->isComplex)
        {       
            if (old->type == "$INT")
            {
                int* ptr = (int*)(old->data);
                delete ptr;
            }
            else if (old->type == "$STR") 
            {
                std::string* ptr = (std::string*)(old->data);
                delete ptr;
            }
            else if (old->type == "$BOOL") 
            {
                bool* ptr = (bool*)(old->data);
                delete ptr;
            }
            else yyfmterror("Invalid dealloc type");
        }
        else
        {
            Language::ComplexType* oldType = (Language::ComplexType*)(old->data);
            for (auto p : oldType->vars)
            {
                free_var(p.second);
            }
        }
    }
    delete old;
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
            printf("Copy var %s\n", p.second->name.c_str());
            newType->vars[p.first] = make_copy(p.second);
        }
        newVar->data = (void*)(newType);
    }
    return newVar;
}

Language::Variable* make_variable(std::string type, std::string name, bool isConst, Language::Variable* value) 
{
	std::string tempName = currentScope.top() + "$$" + name;
	if (variables.find(tempName) != variables.end())
	{
		yyfmterror("Two variables with the same name in the same scope");
	}
	Language::Variable* newVar = new Language::Variable;
	newVar->type = type;
	newVar->name = name;
	newVar->isConstant = isConst;
	newVar->scope = currentScope.top();
	newVar->scopedName = tempName;
	if (type.at(0) == '$') 
	{
		newVar->isComplex = false;
		newVar->data = value->data;
	} 
	else
	{
		newVar->isComplex = true;
        if (complexTypes.find(type) == complexTypes.end())
        {
            yyfmterror("Invalid type %s", type.c_str());
        }
		Language::ComplexType* t = new Language::ComplexType;
		Language::ComplexType* model = complexTypes[type];
		t->typeName = model->typeName;
        for (auto v : model->vars)
        {
            t->vars[v.first] = make_copy(v.second);
        }
		newVar->data = (void*)t;

	}
	#ifdef DEBUG_MODE
		printf("Variable:\n\tTYPE: %s\n\tNAME: %s\n\tFULL: %s\n", type.c_str(), name.c_str(), tempName.c_str());
	#endif
	variables[tempName] = newVar;
	return newVar;
}


std::string* make_type(std::string name)
{
    if (name == "$INT" || name == "$STR" || name == "$BOOL" || name == "$LIST")
    {
        return new std::string(name);
    }
    else
    {
        if (complexTypes.find(name) != complexTypes.end())
		{
			return new std::string(name);	
		}
        else 
        {
            yyfmterror("Type %s doesn't exist", name.c_str());
        } 
    }
}

Language::Variable* make_expression(std::string type, unsigned int valueSize, void* value)
{
    Language::Variable* var = new Language::Variable;
    var->name = "$" + std::to_string(temp_count++);
    var->type = type;
    unsigned char* newData = new unsigned char[valueSize];
    if (type == "$INT") INT(newData) = INT(value);
    if (type == "$STR") STR(newData) = STR(value);
    if (type == "$BOOL") BOOL(newData) = BOOL(value);
    var->data = newData;
    var->isConstant = true;
    var->isComplex = false;
    var->scope = currentScope.top();
    var->scopedName = var->scope + "$$" + var->name;
    return var;
}

Language::Variable* make_addition(Language::Variable* first, Language::Variable* second)
{
    Language::Variable* res = new Language::Variable;
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


Language::Variable* make_substraction(Language::Variable* first, Language::Variable* second)
{
    Language::Variable* res = new Language::Variable;
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

Language::Variable* make_division(Language::Variable* first, Language::Variable* second)
{
    Language::Variable* res = new Language::Variable;
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

Language::Variable* make_reminder(Language::Variable* first, Language::Variable* second)
{
    Language::Variable* res = new Language::Variable;
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

Language::Variable* make_multiplication(Language::Variable* first, Language::Variable* second)
{
    Language::Variable* res = new Language::Variable;
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


Language::Variable* get_var(std::string name, Language::Variable* complex)
{
    std::string tempName = currentScope.top() + "$$" + name;
    if (complex == NULL)
    {
        if (variables.find(tempName) != variables.end())
        {
            return variables.at(tempName);
        }
        else
        {
            yyfmterror("Variable %s doesn't exist", name.c_str());
        }
    }
    else
    {
        if (!complex->isComplex) yyfmterror("Variable %s doesn't have members", complex->type.c_str());
        Language::ComplexType* t = (Language::ComplexType*)complex->data;
        if (t->vars.find(name) == t->vars.end()) yyfmterror("There is no %s in %s", name.c_str(), complex->name.c_str());
        return t->vars[name];
    }
    
}

void make_assign(Language::Variable* var, Language::Variable* value)
{
    if (var->type != var->type)
    {
        yyfmterror("Type of %s is compatible with the expression", var->name.c_str());
    }
    if (var->isConstant)
    {
        yyfmterror("Variable %s is constant", var->name.c_str());
    }
    Language::Variable* cvar = make_copy(value);
    var->data = cvar->data;
    //var->data = value->data;
}

std::vector<Language::Variable*>* make_list(Language::Variable* initial, std::vector<Language::Variable*>* old)
{
    std::vector<Language::Variable*>* ret = old;
    if (ret == NULL)
    {
        ret = new std::vector<Language::Variable*>;
    }
    if (initial != NULL)
    {
        ret->push_back(initial);
    }
    return ret;
}


void make_struct(std::string typeName, std::vector<Language::Variable*>* vars)
{
    if (complexTypes.find(typeName) == complexTypes.end())
    {
        std::vector<Language::Variable*> varvector = *(vars);
        std::reverse(varvector.begin(), varvector.end());
        Language::ComplexType* s = new Language::ComplexType;
        s->typeName = typeName;
        for (Language::Variable* v : varvector)
        {
            v->scope = typeName;
            if (s->vars.find(v->name) == s->vars.end())
            {

                s->vars[v->name] = v;
            } else yyfmterror("There already exists a variable with name %s", v->name.c_str());
                        
        }
        complexTypes[typeName] = s;
        #ifdef DEBUG_MODE
            printf("Complex type [%s] defined:\n", s->typeName.c_str());
            for (auto p : s->vars)
            {
                printf("\t%s %s;\n", p.second->type.c_str(), p.first.c_str());				
            }
        #endif
    
    }
    else yyfmterror("There already exists a type named %s", typeName.c_str());	
}

Language::Variable* make_print(Language::Variable* var)
{
    Language::Variable* ret = new Language::Variable;
    ret->type = "$VOID";
    if (var->type == "$STR") printf("[QUOTE] %s\n", STR(var->data).c_str());
    if (var->type == "$INT") printf("[QUOTE] %d\n", INT(var->data));
    if (var->type == "$BOOL") printf("[QUOTE] %s\n", (BOOL(var->data) == true) ? "indeed" : "untruth");			
	free_var(var);
    return ret;
}

Language::Variable* make_input(std::string name)
{
    Language::Variable* var = get_var(name, NULL);
    if (var->isComplex || var->type == "$VOID" || var->type == "$LIST")
    {
        yyfmterror("Can't read into %s: incorrect type", var->name.c_str());
    }
    if (var->type == "$STR")
    {
        std::string* val = new std::string;
        std::cin >> (*val);
        var->data = (void*)val;
    }
    if (var->type == "$INT")
    {
        int* val = new int;
        std::cin >> (*val);
        var->data = (void*)val;
    }
    if (var->type == "$BOOL")
    {
        bool* val = new bool;
        std::cin >> (*val);
        var->data = (void*)val;
    }
    else
    {
        yyfmterror("Urecognized type");
    }
    return var;
}


void leave()
{
	for (auto var : (variables))
	{
		delete var.second;		
	}
	exit(-1);
}


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
