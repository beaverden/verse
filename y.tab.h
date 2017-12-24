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
    ADDED = 265,
    ASCEND = 266,
    DESCEND = 267,
    CRY = 268,
    FOR = 269,
    EXEUNT = 270,
    NOT = 271,
    SINCE = 272,
    FROM = 273,
    ASK = 274,
    WHETHER = 275,
    THEN = 276,
    OTHERWISE = 277,
    CHANGES = 278,
    INCREASES = 279,
    DECREASES = 280,
    WHENEVER = 281,
    DOES = 282,
    NONE = 283,
    ONE = 284,
    TWICE = 285,
    ID = 286,
    TELLING = 287,
    FIXED = 288,
    INT_TYPE = 289,
    STR_TYPE = 290,
    BOOLEAN_TYPE = 291,
    NUMBER = 292,
    STRING = 293,
    BOOL_TRUE = 294,
    BOOL_FALSE = 295,
    QUOTE = 296,
    SUMMON = 297,
    DIVIDED = 298,
    REMINDING = 299,
    ADDING = 300,
    CONSPIRING = 301,
    EMPOWERING = 302,
    BOOL_IS = 303,
    BOOL_UP = 304,
    BOOL_TO = 305,
    BOOL_DOWN = 306,
    BOOL_LOWER = 307,
    BOOL_GREATER = 308,
    BOOL_THAN = 309
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
#define ADDED 265
#define ASCEND 266
#define DESCEND 267
#define CRY 268
#define FOR 269
#define EXEUNT 270
#define NOT 271
#define SINCE 272
#define FROM 273
#define ASK 274
#define WHETHER 275
#define THEN 276
#define OTHERWISE 277
#define CHANGES 278
#define INCREASES 279
#define DECREASES 280
#define WHENEVER 281
#define DOES 282
#define NONE 283
#define ONE 284
#define TWICE 285
#define ID 286
#define TELLING 287
#define FIXED 288
#define INT_TYPE 289
#define STR_TYPE 290
#define BOOLEAN_TYPE 291
#define NUMBER 292
#define STRING 293
#define BOOL_TRUE 294
#define BOOL_FALSE 295
#define QUOTE 296
#define SUMMON 297
#define DIVIDED 298
#define REMINDING 299
#define ADDING 300
#define CONSPIRING 301
#define EMPOWERING 302
#define BOOL_IS 303
#define BOOL_UP 304
#define BOOL_TO 305
#define BOOL_DOWN 306
#define BOOL_LOWER 307
#define BOOL_GREATER 308
#define BOOL_THAN 309

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

#line 170 "y.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
