
#ifndef TYPES_H
#define TYPES_H

#define _GLOBAL_
#include <string>
#include <string.h>
#include <map>
#include <vector>
#include <stack>
#include <algorithm>

namespace Language
{
	enum Type { BOOLEAN, INT, STR };
	struct Value
	{
		Type type;
		union {
			int 		int_val;
			bool 		bool_val;
			char*	 	str_val;		
		} value;
	};

	struct Variable
	{
		Type 			type;
		std::string 		name;
		std::string 		scope;
		bool 			isConstant;
		union {
			int 		int_val;
			bool 		bool_val;
			char*	 	str_val;		
		} value;
	};
}




#endif
