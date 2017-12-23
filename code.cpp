#include "code.h"




void add_scope(std::string name)
{
    currentScope.push(name);
}

void pop_scope()
{
    currentScope.pop();
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
    return NULL;
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
            else if (old->type == "$VOID")
            {
                // IGNORE
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



Language::Variable* make_variable(std::string type, std::string name, bool isConst, Language::Variable* value) 
{

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

Language::Variable* make_expression(std::string type, void* value)
{
    Language::Variable* var = new Language::Variable;
    var->name = "$" + std::to_string(temp_count++);
    var->type = type;
    var->data = make_default(type);
    if (type == "$INT")  INT(var->data) = INT(value);
    if (type == "$STR") STR(var->data) = STR(value);
    if (type == "$BOOL") BOOL(var->data) = BOOL(value);
    var->isConstant = true;
    var->isComplex = false;
    var->scope = currentScope.top();
    var->scopedName = var->scope + "$$" + var->name;
    
    return var;
}



Language::Variable* make_boolean(Language::Variable* first, Language::Variable* second, Language::BOOL_OP op)
{
    Language::Variable* res = new Language::Variable;
    if (first->isComplex || second->isComplex)
    {
        yyfmterror("Cannot compare complex types");
    }
    
    }
    else yyfmterror("Cannot compare different types");

    free_var(first);
    free_var(second);
    return res;
}

bool make_if(Language::Variable* var)
{
    if (var->type != "$BOOL")
    {
        yyfmterror("Expression type is not boolean");
    }
    return BOOL(var->data);
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
    ret->data = NULL;
    if (var->type == "$STR") printf("[QUOTE] %s\n", STR(var->data).c_str());
    if (var->type == "$INT") printf("[QUOTE] %d\n", INT(var->data));
    if (var->type == "$BOOL") printf("[QUOTE] %s\n", (BOOL(var->data) == true) ? "indeed" : "untruth");			
	free_var(var);
    return ret;
}

Language::Variable* make_input(Language::Variable* var)
{
    if (var->isConstant) yyfmterror("Can't read to constant variable");
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
        yyfmterror("Urecognized type");
    }
    return var;
}



