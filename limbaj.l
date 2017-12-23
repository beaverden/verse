%{
#include <stdio.h>
#include "types.h"
#include "code.h"
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
"quote" {return QUOTE;}
"enter" {return ENTER;}
"proclaim" {return PROCLAIM;}
"let" {return LET;} 
"there" {return THERE;}
"equalling" {return EQUALLING;}
"upon" {return UPON;}
"added" {return ADDED;}
"descend" {return DESCEND;}
"ascend" {return ASCEND;}
"cry" {return CRY;}
"for" {return FOR;}
"divided" {return DIVIDED;}
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
"changes" {return CHANGES;}
"increases" {return INCREASES;}
"decreases" {return DECREASES;}
"whenever" {return WHENEVER;}
"does" {return DOES;}
"none" {return NONE;}
"one" {return ONE;}
"twice" {return TWICE;}

\"(\\.|[^"\\])*\" { 
        yylval.strvalue = new std::string(yytext); 
        (*yylval.strvalue) = yylval.strvalue->substr(1, yylval.strvalue->length()-2);
        return STRING;
    }

[ \t]|"the"|"be"|"a"|"an"|"'s" ;
[_a-zA-Z][_a-zA-Z0-9]* { 
        yylval.strvalue = new std::string(yytext); 
        return ID;
    }
(\-|\+)?([0-9]|([1-9][0-9]*)) { 
        yylval.intvalue = new int(atoi(yytext)); 
        return NUMBER;
    }
\n {yylineno++;}
. {return yytext[0];}

 
