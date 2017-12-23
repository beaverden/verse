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
    ENTER = 258,
    PROCLAIM = 259,
    LET = 260,
    THERE = 261,
    EQUALLING = 262,
    UPON = 263,
    ADDED = 264,
    ASCEND = 265,
    DESCEND = 266,
    CRY = 267,
    FOR = 268,
    DIVIDED = 269,
    REMINDING = 270,
    ADDING = 271,
    CONSPIRING = 272,
    EMPOWERING = 273,
    EXEUNT = 274,
    NOT = 275,
    SINCE = 276,
    FROM = 277,
    ASK = 278,
    WHETHER = 279,
    THEN = 280,
    OTHERWISE = 281,
    CHANGES = 282,
    INCREASES = 283,
    DECREASES = 284,
    WHENEVER = 285,
    DOES = 286,
    NONE = 287,
    ONE = 288,
    TWICE = 289,
    ID = 290,
    TELLING = 291,
    FIXED = 292,
    INT_TYPE = 293,
    STR_TYPE = 294,
    BOOLEAN_TYPE = 295,
    NUMBER = 296,
    STRING = 297,
    BOOL_TRUE = 298,
    BOOL_FALSE = 299,
    QUOTE = 300,
    SUMMON = 301,
    BOOL_IS = 302,
    BOOL_UP = 303,
    BOOL_TO = 304,
    BOOL_DOWN = 305,
    BOOL_LOWER = 306,
    BOOL_GREATER = 307,
    BOOL_THAN = 308
  };
#endif
/* Tokens.  */
#define ENTER 258
#define PROCLAIM 259
#define LET 260
#define THERE 261
#define EQUALLING 262
#define UPON 263
#define ADDED 264
#define ASCEND 265
#define DESCEND 266
#define CRY 267
#define FOR 268
#define DIVIDED 269
#define REMINDING 270
#define ADDING 271
#define CONSPIRING 272
#define EMPOWERING 273
#define EXEUNT 274
#define NOT 275
#define SINCE 276
#define FROM 277
#define ASK 278
#define WHETHER 279
#define THEN 280
#define OTHERWISE 281
#define CHANGES 282
#define INCREASES 283
#define DECREASES 284
#define WHENEVER 285
#define DOES 286
#define NONE 287
#define ONE 288
#define TWICE 289
#define ID 290
#define TELLING 291
#define FIXED 292
#define INT_TYPE 293
#define STR_TYPE 294
#define BOOLEAN_TYPE 295
#define NUMBER 296
#define STRING 297
#define BOOL_TRUE 298
#define BOOL_FALSE 299
#define QUOTE 300
#define SUMMON 301
#define BOOL_IS 302
#define BOOL_UP 303
#define BOOL_TO 304
#define BOOL_DOWN 305
#define BOOL_LOWER 306
#define BOOL_GREATER 307
#define BOOL_THAN 308

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 15 "limbaj.ypp" /* yacc.c:1909  */

	int* intvalue;
	bool* boolvalue;
	std::string* strvalue;
	Language::Value* exprptrvalue;
	Language::Variable* varptrvalue;
	std::vector<Language::Variable*>* decllist;

#line 169 "y.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
