/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    UPON = 258,
    NOT = 259,
    SINCE = 260,
    FROM = 261,
    THEN = 262,
    CHANGES = 263,
    INCREASES = 264,
    DECREASES = 265,
    WHENEVER = 266,
    DOES = 267,
    ID = 268,
    TELLING = 269,
    FIXED = 270,
    INT_TYPE = 271,
    STR_TYPE = 272,
    BOOLEAN_TYPE = 273,
    NONE = 274,
    ONE = 275,
    NUMBER = 276,
    STRING = 277,
    BOOL_TRUE = 278,
    BOOL_FALSE = 279,
    QUOTE = 280,
    SUMMON = 281,
    DIVIDING = 282,
    REMINDING = 283,
    ADDING = 284,
    CONSPIRING = 285,
    EMPOWERING = 286,
    DIVIDE = 287,
    ADDED = 288,
    EMPOWER = 289,
    CONSPIRED = 290,
    REMIND = 291,
    ASCEND = 292,
    DESCEND = 293,
    BOOL_IS = 294,
    BOOL_UP = 295,
    BOOL_TO = 296,
    BOOL_DOWN = 297,
    BOOL_LOWER = 298,
    BOOL_GREATER = 299,
    BOOL_THAN = 300,
    FLOW_BREAK = 301,
    FLOW_FREE = 302,
    FLOW_CONTINUE = 303,
    EXEUNT = 304,
    DESCRIBE = 305,
    ADVENTURE = 306,
    AS = 307,
    OF = 308,
    CRY = 309,
    FOR = 310,
    ASK = 311,
    WHETHER = 312,
    OTHERWISE = 313,
    BE = 314,
    ENTER = 315,
    PROCLAIM = 316,
    LET = 317,
    THERE = 318
  };
#endif
/* Tokens.  */
#define UPON 258
#define NOT 259
#define SINCE 260
#define FROM 261
#define THEN 262
#define CHANGES 263
#define INCREASES 264
#define DECREASES 265
#define WHENEVER 266
#define DOES 267
#define ID 268
#define TELLING 269
#define FIXED 270
#define INT_TYPE 271
#define STR_TYPE 272
#define BOOLEAN_TYPE 273
#define NONE 274
#define ONE 275
#define NUMBER 276
#define STRING 277
#define BOOL_TRUE 278
#define BOOL_FALSE 279
#define QUOTE 280
#define SUMMON 281
#define DIVIDING 282
#define REMINDING 283
#define ADDING 284
#define CONSPIRING 285
#define EMPOWERING 286
#define DIVIDE 287
#define ADDED 288
#define EMPOWER 289
#define CONSPIRED 290
#define REMIND 291
#define ASCEND 292
#define DESCEND 293
#define BOOL_IS 294
#define BOOL_UP 295
#define BOOL_TO 296
#define BOOL_DOWN 297
#define BOOL_LOWER 298
#define BOOL_GREATER 299
#define BOOL_THAN 300
#define FLOW_BREAK 301
#define FLOW_FREE 302
#define FLOW_CONTINUE 303
#define EXEUNT 304
#define DESCRIBE 305
#define ADVENTURE 306
#define AS 307
#define OF 308
#define CRY 309
#define FOR 310
#define ASK 311
#define WHETHER 312
#define OTHERWISE 313
#define BE 314
#define ENTER 315
#define PROCLAIM 316
#define LET 317
#define THERE 318

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 15 "limbaj.ypp" /* yacc.c:1909  */

	AS_TREE* 		treevalue;
	int* 			intvalue;
	bool* 			boolvalue;
	std::string* 	strvalue;
	Operation 		optype;

#line 188 "y.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
