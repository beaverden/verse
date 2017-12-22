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
    ID = 258,
    TELLING = 259,
    FIXED = 260,
    INT_TYPE = 261,
    STR_TYPE = 262,
    BOOLEAN_TYPE = 263,
    NUMBER = 264,
    STRING = 265,
    BOOL_TRUE = 266,
    BOOL_FALSE = 267,
    QUOTE = 268,
    ENTER = 269,
    PROCLAIM = 270,
    LET = 271,
    THERE = 272,
    EQUALLING = 273,
    UPON = 274,
    ADDED = 275,
    ASCEND = 276,
    DESCEND = 277,
    CRY = 278,
    FOR = 279,
    DIVIDED = 280,
    REMINDING = 281,
    ADDING = 282,
    CONSPIRING = 283,
    EMPOWERING = 284,
    IS = 285,
    LEFT = 286,
    RIGHT = 287,
    UP = 288,
    TO = 289,
    DOWN = 290,
    LOWER = 291,
    GREATER = 292,
    THAN = 293,
    EXEUNT = 294,
    SUMMONED = 295,
    NOT = 296,
    SINCE = 297,
    FROM = 298,
    ASK = 299,
    WHETHER = 300,
    THEN = 301,
    OTHERWISE = 302,
    CHANGES = 303,
    INCREASES = 304,
    DECREASES = 305,
    WHENEVER = 306,
    DOES = 307,
    NONE = 308,
    ONE = 309,
    TWICE = 310
  };
#endif
/* Tokens.  */
#define ID 258
#define TELLING 259
#define FIXED 260
#define INT_TYPE 261
#define STR_TYPE 262
#define BOOLEAN_TYPE 263
#define NUMBER 264
#define STRING 265
#define BOOL_TRUE 266
#define BOOL_FALSE 267
#define QUOTE 268
#define ENTER 269
#define PROCLAIM 270
#define LET 271
#define THERE 272
#define EQUALLING 273
#define UPON 274
#define ADDED 275
#define ASCEND 276
#define DESCEND 277
#define CRY 278
#define FOR 279
#define DIVIDED 280
#define REMINDING 281
#define ADDING 282
#define CONSPIRING 283
#define EMPOWERING 284
#define IS 285
#define LEFT 286
#define RIGHT 287
#define UP 288
#define TO 289
#define DOWN 290
#define LOWER 291
#define GREATER 292
#define THAN 293
#define EXEUNT 294
#define SUMMONED 295
#define NOT 296
#define SINCE 297
#define FROM 298
#define ASK 299
#define WHETHER 300
#define THEN 301
#define OTHERWISE 302
#define CHANGES 303
#define INCREASES 304
#define DECREASES 305
#define WHENEVER 306
#define DOES 307
#define NONE 308
#define ONE 309
#define TWICE 310

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 17 "limbaj.ypp" /* yacc.c:1909  */

	int intvalue;
	bool boolvalue;
	std::string* strvalue;
	Language::Type typevalue;
	Language::Value* exprptrvalue;
	Language::Variable* varptrvalue;
	
	std::vector<Language::Variable*>* decllist;

#line 175 "y.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
