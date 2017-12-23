
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
	};

	struct ComplexType
	{
		std::string typeName;
		std::unordered_map<std::string, Variable*> vars;	
	};
}




#endif
