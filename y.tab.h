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
    NUMERIC = 268,
    VERBOSE = 269,
    ID = 270,
    TELLING = 271,
    FIXED = 272,
    INT_TYPE = 273,
    STR_TYPE = 274,
    BOOLEAN_TYPE = 275,
    NONE = 276,
    ONE = 277,
    NUMBER = 278,
    STRING = 279,
    BOOL_TRUE = 280,
    BOOL_FALSE = 281,
    QUOTE = 282,
    SUMMON = 283,
    DIVIDING = 284,
    REMINDING = 285,
    ADDING = 286,
    CONSPIRING = 287,
    EMPOWERING = 288,
    DIVIDE = 289,
    ADDED = 290,
    EMPOWER = 291,
    CONSPIRED = 292,
    REMIND = 293,
    ASCEND = 294,
    DESCEND = 295,
    BOOL_IS = 296,
    BOOL_UP = 297,
    BOOL_TO = 298,
    BOOL_DOWN = 299,
    BOOL_LOWER = 300,
    BOOL_GREATER = 301,
    BOOL_THAN = 302,
    FLOW_BREAK = 303,
    FLOW_FREE = 304,
    FLOW_CONTINUE = 305,
    EXEUNT = 306,
    DESCRIBE = 307,
    ADVENTURE = 308,
    AS = 309,
    OF = 310,
    CRY = 311,
    FOR = 312,
    ASK = 313,
    WHETHER = 314,
    OTHERWISE = 315,
    BE = 316,
    ENTER = 317,
    PROCLAIM = 318,
    LET = 319,
    THERE = 320
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
#define NUMERIC 268
#define VERBOSE 269
#define ID 270
#define TELLING 271
#define FIXED 272
#define INT_TYPE 273
#define STR_TYPE 274
#define BOOLEAN_TYPE 275
#define NONE 276
#define ONE 277
#define NUMBER 278
#define STRING 279
#define BOOL_TRUE 280
#define BOOL_FALSE 281
#define QUOTE 282
#define SUMMON 283
#define DIVIDING 284
#define REMINDING 285
#define ADDING 286
#define CONSPIRING 287
#define EMPOWERING 288
#define DIVIDE 289
#define ADDED 290
#define EMPOWER 291
#define CONSPIRED 292
#define REMIND 293
#define ASCEND 294
#define DESCEND 295
#define BOOL_IS 296
#define BOOL_UP 297
#define BOOL_TO 298
#define BOOL_DOWN 299
#define BOOL_LOWER 300
#define BOOL_GREATER 301
#define BOOL_THAN 302
#define FLOW_BREAK 303
#define FLOW_FREE 304
#define FLOW_CONTINUE 305
#define EXEUNT 306
#define DESCRIBE 307
#define ADVENTURE 308
#define AS 309
#define OF 310
#define CRY 311
#define FOR 312
#define ASK 313
#define WHETHER 314
#define OTHERWISE 315
#define BE 316
#define ENTER 317
#define PROCLAIM 318
#define LET 319
#define THERE 320

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 16 "limbaj.ypp" /* yacc.c:1909  */

	AS_TREE* 		treevalue;
	int* 			intvalue;
	bool* 			boolvalue;
	std::string* 	strvalue;
	Operation 		optype;

#line 192 "y.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
