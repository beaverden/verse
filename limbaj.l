%{
#include <stdio.h>
#include "include/types.h"
#include "include/AST.h"
#include "y.tab.h"

%}
%%
"whole"|"full"|"discrete" { return INT_TYPE; }
"saying" { return STR_TYPE; };
"doubting" { return BOOLEAN_TYPE; };
"fixed" { return FIXED; }
"telling" { return TELLING; }
"indeed" { yylval.boolvalue = new bool(true); return BOOL_TRUE; }
"untruth" { yylval.boolvalue = new bool(false); return BOOL_FALSE; }
"of" { return OF; }
"quote" {return QUOTE;}
"enter" {return ENTER;}
"proclaim" {return PROCLAIM;}
"let" {return LET;} 
"there" {return THERE;}
"upon" {return UPON;}
"descend" {return DESCEND;}
"ascend" {return ASCEND;}
"cry" {return CRY;}
"for" {return FOR;}
"divide" { return DIVIDE; }
"conspired" { return CONSPIRED; }
"remind" { return REMIND; }
"empower" { return EMPOWER; }
"added" {return ADDED;}
"dividing" {return DIVIDING;}
"reminding" {return REMINDING;}
"adding" {return ADDING;}
"conspiring" {return CONSPIRING;}
"empowering" {return EMPOWERING;}
"is" {return BOOL_IS;}
"up" {return BOOL_UP;}
"to" {return BOOL_TO;}
"down" {return BOOL_DOWN;}
"lower" {return BOOL_LOWER;}
"greater" {return BOOL_GREATER;}
"than" {return BOOL_THAN;}
"exeunt" {return EXEUNT;}
"summon" {return SUMMON;}
"not" {return NOT;}
"since" {return SINCE;}
"from" {return FROM;}
"ask" {return ASK;}
"whether" {return WHETHER;}
"then" {return THEN;}
"otherwise" {return OTHERWISE;}
"does" {return DOES;}
"none" {return NONE;}
"one" {return ONE;}
"be" { return BE; }
"as" { return AS; }
"break" { return FLOW_BREAK; }
"free" { return FLOW_FREE; }
"continue" { return FLOW_CONTINUE; }
"describe" { return DESCRIBE; }
"adventure" { return ADVENTURE; }
"numeric" { return NUMERIC; }
"verbose" { return VERBOSE; }
"book" { return BOOK; }
"chapter" { return CHAPTER; }
"write" { return WRITE; }
"erase" { return ERASE; }

\"(\\.|[^"\\])*\" { 
        yylval.strvalue = new std::string(yytext); 
        (*yylval.strvalue) = yylval.strvalue->substr(1, yylval.strvalue->length()-2);
        return STRING;
    }

[_a-zA-Z][_a-zA-Z0-9]* { 
        yylval.strvalue = new std::string(yytext); 
        return ID;
    }
(\-|\+)?([0-9]|([1-9][0-9]*)) { 
        yylval.intvalue = new int(atoi(yytext)); 
        return NUMBER;
    }
[ \t]|"the"|"be"|"a"|"an"|"'s" ;
\n {yylineno++;}
. {return yytext[0];}

 
