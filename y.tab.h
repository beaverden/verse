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
    BE = 258,
    ENTER = 259,
    PROCLAIM = 260,
    LET = 261,
    THERE = 262,
    EQUALLING = 263,
    UPON = 264,
    ASCEND = 265,
    DESCEND = 266,
    CRY = 267,
    FOR = 268,
    EXEUNT = 269,
    NOT = 270,
    SINCE = 271,
    FROM = 272,
    ASK = 273,
    WHETHER = 274,
    THEN = 275,
    OTHERWISE = 276,
    CHANGES = 277,
    INCREASES = 278,
    DECREASES = 279,
    WHENEVER = 280,
    DOES = 281,
    NONE = 282,
    ONE = 283,
    TWICE = 284,
    ID = 285,
    TELLING = 286,
    FIXED = 287,
    INT_TYPE = 288,
    STR_TYPE = 289,
    BOOLEAN_TYPE = 290,
    NUMBER = 291,
    STRING = 292,
    BOOL_TRUE = 293,
    BOOL_FALSE = 294,
    QUOTE = 295,
    SUMMON = 296,
    DIVIDING = 297,
    REMINDING = 298,
    ADDING = 299,
    CONSPIRING = 300,
    EMPOWERING = 301,
    DIVIDE = 302,
    ADDED = 303,
    EMPOWER = 304,
    CONSPIRED = 305,
    REMIND = 306,
    BOOL_IS = 307,
    BOOL_UP = 308,
    BOOL_TO = 309,
    BOOL_DOWN = 310,
    BOOL_LOWER = 311,
    BOOL_GREATER = 312,
    BOOL_THAN = 313
  };
#endif
/* Tokens.  */
#define BE 258
#define ENTER 259
#define PROCLAIM 260
#define LET 261
#define THERE 262
#define EQUALLING 263
#define UPON 264
#define ASCEND 265
#define DESCEND 266
#define CRY 267
#define FOR 268
#define EXEUNT 269
#define NOT 270
#define SINCE 271
#define FROM 272
#define ASK 273
#define WHETHER 274
#define THEN 275
#define OTHERWISE 276
#define CHANGES 277
#define INCREASES 278
#define DECREASES 279
#define WHENEVER 280
#define DOES 281
#define NONE 282
#define ONE 283
#define TWICE 284
#define ID 285
#define TELLING 286
#define FIXED 287
#define INT_TYPE 288
#define STR_TYPE 289
#define BOOLEAN_TYPE 290
#define NUMBER 291
#define STRING 292
#define BOOL_TRUE 293
#define BOOL_FALSE 294
#define QUOTE 295
#define SUMMON 296
#define DIVIDING 297
#define REMINDING 298
#define ADDING 299
#define CONSPIRING 300
#define EMPOWERING 301
#define DIVIDE 302
#define ADDED 303
#define EMPOWER 304
#define CONSPIRED 305
#define REMIND 306
#define BOOL_IS 307
#define BOOL_UP 308
#define BOOL_TO 309
#define BOOL_DOWN 310
#define BOOL_LOWER 311
#define BOOL_GREATER 312
#define BOOL_THAN 313

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

#line 178 "y.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
