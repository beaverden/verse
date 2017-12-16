%{
#include <stdio.h>
extern FILE* yyin;
extern char* yytext;
extern int yylineno;
%}
%token TYPE ID NUMBER STRING QUOTE ENTER LET BE THERE EQUALLING UPON ADDED ASCEND DESCEND CRY FOR DIVIDED REMINDING ADDING CONSPIRING EMPOWERING IS LEFT RIGHT COMMA UP TO DOWN LOWER GREATER THAN EXEUNT SUMMONED NOT SINCE FROM ASK WHETHER THEN OTHERWISE CHANGES INCREASES DECREASES WHENEVER DOES NONE ONE TWICE UNTRUTH INDEED
%start progr
%%
progr: lines {printf("program corect sintactic\n");}
     ;
lines : line
      | lines line
      ;
line : declaration
     | assignment
     | output
     | function
     | return
     | call
     | since
     | from
     | ask
     | whenever
     ;
declaration : ENTER TYPE expr
	    ;
assignment : LET ID BE expr
	   | LET THERE BE ID EQUALLING expr
	   | LET UPON ID ADDED expr
	   | LET ID DESCEND
	   | LET ID ASCEND
           ;
output : QUOTE expr
       ;
function : LET TYPE ID BE list_of_arg LEFT lines RIGHT
	 | LET TYPE ID BE LEFT lines RIGHT
	 ;
call: CRY FOR ID list_of_param
    | CRY FOR ID 
    ;
list_of_arg: TYPE ID
	   | TYPE ID COMMA list_of_arg
	   ;
list_of_param: expr
	     | expr COMMA list_of_param
	     ;
math_op : DIVIDED
	| REMINDING
	| ADDING
	| CONSPIRING
	| EMPOWERING
	;
logic_op : IS
	 | UP TO LOWER THAN
	 | LOWER THAN
	 | DOWN TO GREATER THAN
	 | GREATER THAN
	   ;
return : EXEUNT expr
       ;
input : SUMMONED
      ;
condition : expr
	  | NOT condition
	  | condition logic_op condition
	  ;
since : SINCE condition LEFT lines RIGHT
      ;
from : FROM ID IS expr COMMA TO condition COMMA assignment LEFT lines RIGHT
    ; 
ask : ASK WHETHER condition THEN LEFT lines RIGHT
   | ASK WHETHER condition THEN LEFT lines RIGHT OTHERWISE LEFT lines RIGHT
   ; 
whenever_condition : ID CHANGES
		   | ID INCREASES
		   | ID DECREASES
		   ;
whenever : WHENEVER DOES whenever_condition LEFT lines RIGHT
	 ;
expr : LEFT expr RIGHT
     | expr math_op expr
     | call
     | ID
     | NUMBER
     | STRING	
     | input
     | NONE
     | ONE
     | TWICE
     | INDEED
     | UNTRUTH
     ;
%%
int yyerror(char * s){
printf("eroare: %s la linia:%d\n",s,yylineno);
}

int main(int argc, char** argv){
yyin=fopen(argv[1],"r");
yyparse();
} 
