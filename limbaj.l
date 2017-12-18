%{
#include <stdio.h>
#include "types.h"
#include "y.tab.h"

%}
%x string
%%
"whole"|"full"|"discrete" {yylval.strvalue=strdup(yytext); return INT_TYPE;}
"saying" { yylval.strvalue=strdup(yytext); return STR_TYPE; };
"doubting" { yylval.strvalue=strdup(yytext); return BOOLEAN_TYPE; };
"fixed" { return FIXED; }
"telling" { return TELLING; }
"true" { return BOOL_TRUE; }
"false" { return BOOL_FALSE; }
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
"is" {return IS;}
"up" {return UP;}
"to" {return TO;}
"down" {return DOWN;}
"lower" {return LOWER;}
"greater" {return GREATER;}
"than" {return THAN;}
"exeunt" {return EXEUNT;}
"summoned" {return SUMMONED;}
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
"indeed" {return INDEED;}
"untruth" {return UNTRUTH;}
\"(\\.|[^"\\])*\" { yylval.strvalue=strdup(yytext); return STRING;}
[ \t]|"the"|"be"|"a"|"an"|"'s" ;
[_a-zA-Z][_a-zA-Z0-9]* { yylval.strvalue=strdup(yytext); return ID;}
(\-|\+)?([0-9]|([1-9][0-9]*)) {yylval.intvalue=atoi(yytext); return NUMBER;}
\n {yylineno++;}
. {return yytext[0];}

 
