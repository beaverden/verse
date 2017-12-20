
#ifndef TYPES_H
#define TYPES_H

#define _GLOBAL_
#include <string>
#include <string.h>
#include <map>
#include <vector>
#include <stack>
#include <algorithm>
#include <unordered_map>

namespace Language
{
	enum Type { BOOLEAN, INT, STR, COMPLEX, VOID };
	struct Value
	{
		std::string 		type;
		int 			int_val;
		bool 			bool_val;
		std::string 		str_val;
		void* 			data;
	};

	struct Variable
	{
		std::string 		type;
		std::string		name;
		std::string		scope;
		std::string 		scopedName;
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
