%{
#include <stdio.h>
#include "y.tab.h"
%}
%x string
%%
"whole"|"full"|"fixed"|"doubting"|"discrete"|"saying" {return TYPE;}
"quote" {return QUOTE;}
"enter" {return ENTER;}
"let" {return LET;} 
"be" {return BE;}
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
"<" {return LEFT;}
">" {return RIGHT;}
"," {return COMMA;}
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
\"(\\.|[^"\\])*\" {return STRING;}
[ \t]|"the"|"be"|"a"|"an"|"'s" ;
[_a-zA-Z][_a-zA-Z0-9]* {return ID;}
(\-|\+)?[0-9]+ {return NUMBER;}
\n {yylineno++;}
. {return yytext[0];}

 
