/* A Bison parser, made by GNU Bison 3.8.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_HINT_GRAM_HPP_INCLUDED
# define YY_YY_HINT_GRAM_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    IDENT = 258,                   /* IDENT  */
    FCONST = 259,                  /* FCONST  */
    SCONST = 260,                  /* SCONST  */
    BCONST = 261,                  /* BCONST  */
    XCONST = 262,                  /* XCONST  */
    ICONST = 263,                  /* ICONST  */
    NestLoop_P = 264,              /* NestLoop_P  */
    MergeJoin_P = 265,             /* MergeJoin_P  */
    HashJoin_P = 266,              /* HashJoin_P  */
    No_P = 267,                    /* No_P  */
    Leading_P = 268,               /* Leading_P  */
    Rows_P = 269,                  /* Rows_P  */
    Broadcast_P = 270,             /* Broadcast_P  */
    Redistribute_P = 271,          /* Redistribute_P  */
    BlockName_P = 272,             /* BlockName_P  */
    TableScan_P = 273,             /* TableScan_P  */
    IndexScan_P = 274,             /* IndexScan_P  */
    IndexOnlyScan_P = 275,         /* IndexOnlyScan_P  */
    Skew_P = 276,                  /* Skew_P  */
    HINT_MULTI_NODE_P = 277,       /* HINT_MULTI_NODE_P  */
    NULL_P = 278,                  /* NULL_P  */
    TRUE_P = 279,                  /* TRUE_P  */
    FALSE_P = 280,                 /* FALSE_P  */
    Predpush_P = 281,              /* Predpush_P  */
    PredpushSameLevel_P = 282,     /* PredpushSameLevel_P  */
    Rewrite_P = 283,               /* Rewrite_P  */
    Gather_P = 284,                /* Gather_P  */
    Set_P = 285,                   /* Set_P  */
    Set_Var_P = 286,               /* Set_Var_P  */
    USE_CPLAN_P = 287,             /* USE_CPLAN_P  */
    USE_GPLAN_P = 288,             /* USE_GPLAN_P  */
    ON_P = 289,                    /* ON_P  */
    OFF_P = 290,                   /* OFF_P  */
    No_expand_P = 291,             /* No_expand_P  */
    SQL_IGNORE_P = 292,            /* SQL_IGNORE_P  */
    NO_GPC_P = 293,                /* NO_GPC_P  */
    CHOOSE_ADAPTIVE_GPLAN_P = 294  /* CHOOSE_ADAPTIVE_GPLAN_P  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 44 "hint_gram.y"

	int		ival;
	char		*str;
	List		*list;
	Node	*node;

#line 110 "hint_gram.hpp"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif




int yyparse (yyscan_t yyscanner);


#endif /* !YY_YY_HINT_GRAM_HPP_INCLUDED  */
