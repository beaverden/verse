
#ifndef TYPES_H
#define TYPES_H

#include <string>
#include <string.h>
#include <map>
#include <vector>
#include <stack>	
#include <algorithm>
#include <unordered_map>
#include <set>
#include <iostream>
#include <memory>

#define DEBUG_MODE

#define STR(ptr) (*(std::string*)(ptr))
#define INT(ptr) (*(int*)(ptr))
#define BOOL(ptr) (*(bool*)(ptr))
#define VARVECTOR(ptr) (*(std::vector<Language::Variable*>*)(ptr))
#define AS_VECTOR(data) (*(std::vector<AS_TREE*>*)(data))

namespace Language
{
	struct Value
	{
		std::string 	type;
		void* 			data;
		int lineno;
	};

	struct Variable
	{
		std::string 	type;
		std::string		name;
		std::string		scope;
		std::string 	scopedName;
		bool 			isConstant;
		bool			isComplex;
		void*			data;
		int lineno;
	};

	struct ComplexType
	{
		std::string typeName;
		std::unordered_map<std::string, Variable*> vars;	
	};

	struct GlobalContext
	{
		bool inLoop = false;
		bool inFunction = true;
	};

	struct PrototypeParam
	{
		std::string		name;
		std::string		type;
	};

}




#endif
