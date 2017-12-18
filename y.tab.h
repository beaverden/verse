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
    LET = 270,
    BE = 271,
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
    COMMA = 288,
    UP = 289,
    TO = 290,
    DOWN = 291,
    LOWER = 292,
    GREATER = 293,
    THAN = 294,
    EXEUNT = 295,
    SUMMONED = 296,
    NOT = 297,
    SINCE = 298,
    FROM = 299,
    ASK = 300,
    WHETHER = 301,
    THEN = 302,
    OTHERWISE = 303,
    CHANGES = 304,
    INCREASES = 305,
    DECREASES = 306,
    WHENEVER = 307,
    DOES = 308,
    NONE = 309,
    ONE = 310,
    TWICE = 311,
    UNTRUTH = 312,
    INDEED = 313
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
#define LET 270
#define BE 271
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
#define COMMA 288
#define UP 289
#define TO 290
#define DOWN 291
#define LOWER 292
#define GREATER 293
#define THAN 294
#define EXEUNT 295
#define SUMMONED 296
#define NOT 297
#define SINCE 298
#define FROM 299
#define ASK 300
#define WHETHER 301
#define THEN 302
#define OTHERWISE 303
#define CHANGES 304
#define INCREASES 305
#define DECREASES 306
#define WHENEVER 307
#define DOES 308
#define NONE 309
#define ONE 310
#define TWICE 311
#define UNTRUTH 312
#define INDEED 313

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 90 "limbaj.ypp" /* yacc.c:1909  */

	int intvalue;
	bool boolvalue;
	char* strvalue;	
	Language::Type typevalue;
	Language::Value exprvalue;

#line 178 "y.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
