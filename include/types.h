
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

//#define DEBUG_MODE

#define STR(ptr) (*(std::string*)(ptr))
#define INT(ptr) (*(int*)(ptr))
#define BOOL(ptr) (*(bool*)(ptr))
#define VARVECTOR(ptr) (*(std::vector<Language::Variable*>*)(ptr))
#define VALVECTOR(ptr) (*(std::vector<Language::Value*>*)(ptr))
#define AS_VECTOR(data) (*(std::vector<AS_TREE*>*)(data))

namespace Language
{
	struct Value
	{
		std::string 	type;
		void* 			data;
		bool 			isComplex = false;
		bool			isConstant = false;
		int 			lineno;
	};

	struct Variable
	{
		std::string 	type;
		std::string		name;
		bool 			isConstant;
		bool			isComplex;
		void*			data;
		int lineno;
	};

	struct PrototypeParam
	{
		bool 			isConstant;
		std::string		name;
		std::string		type;
	};

	struct ComplexType
	{
		std::string typeName;
		std::unordered_map<std::string, PrototypeParam> vars;
	};

	struct GlobalContext
	{
		bool inLoop 	= false;
		bool inFunction = false;
	};

	struct Structure
	{
		std::map<std::string, Language::Variable*> vars;
	};

	


}




#endif
