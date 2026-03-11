/* A Bison parser, made by GNU Bison 3.6.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_PLPGSQL_YY_SRC_PLTSQL_PL_GRAM_HPP_INCLUDED
# define YY_PLPGSQL_YY_SRC_PLTSQL_PL_GRAM_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int plpgsql_yydebug;
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
    VCONST = 262,                  /* VCONST  */
    XCONST = 263,                  /* XCONST  */
    Op = 264,                      /* Op  */
    CmpOp = 265,                   /* CmpOp  */
    CmpNullOp = 266,               /* CmpNullOp  */
    COMMENTSTRING = 267,           /* COMMENTSTRING  */
    SET_USER_IDENT = 268,          /* SET_USER_IDENT  */
    SET_IDENT = 269,               /* SET_IDENT  */
    UNDERSCORE_CHARSET = 270,      /* UNDERSCORE_CHARSET  */
    FCONST_F = 271,                /* FCONST_F  */
    FCONST_D = 272,                /* FCONST_D  */
    ICONST = 273,                  /* ICONST  */
    PARAM = 274,                   /* PARAM  */
    TYPECAST = 275,                /* TYPECAST  */
    ORA_JOINOP = 276,              /* ORA_JOINOP  */
    DOT_DOT = 277,                 /* DOT_DOT  */
    COLON_EQUALS = 278,            /* COLON_EQUALS  */
    PARA_EQUALS = 279,             /* PARA_EQUALS  */
    SET_IDENT_SESSION = 280,       /* SET_IDENT_SESSION  */
    SET_IDENT_GLOBAL = 281,        /* SET_IDENT_GLOBAL  */
    TSQL_XCONST = 282,             /* TSQL_XCONST  */
    DIALECT_TSQL = 283,            /* DIALECT_TSQL  */
    T_WORD = 284,                  /* T_WORD  */
    T_CWORD = 285,                 /* T_CWORD  */
    T_DATUM = 286,                 /* T_DATUM  */
    T_PLACEHOLDER = 287,           /* T_PLACEHOLDER  */
    T_LABELLOOP = 288,             /* T_LABELLOOP  */
    T_LABELWHILE = 289,            /* T_LABELWHILE  */
    T_LABELREPEAT = 290,           /* T_LABELREPEAT  */
    T_VARRAY = 291,                /* T_VARRAY  */
    T_ARRAY_FIRST = 292,           /* T_ARRAY_FIRST  */
    T_ARRAY_LAST = 293,            /* T_ARRAY_LAST  */
    T_ARRAY_COUNT = 294,           /* T_ARRAY_COUNT  */
    T_ARRAY_EXISTS = 295,          /* T_ARRAY_EXISTS  */
    T_ARRAY_PRIOR = 296,           /* T_ARRAY_PRIOR  */
    T_ARRAY_NEXT = 297,            /* T_ARRAY_NEXT  */
    T_ARRAY_DELETE = 298,          /* T_ARRAY_DELETE  */
    T_ARRAY_EXTEND = 299,          /* T_ARRAY_EXTEND  */
    T_ARRAY_TRIM = 300,            /* T_ARRAY_TRIM  */
    T_VARRAY_VAR = 301,            /* T_VARRAY_VAR  */
    T_RECORD = 302,                /* T_RECORD  */
    T_TABLE = 303,                 /* T_TABLE  */
    T_TABLE_VAR = 304,             /* T_TABLE_VAR  */
    T_PACKAGE_VARIABLE = 305,      /* T_PACKAGE_VARIABLE  */
    T_OBJECT_TYPE_VAR_METHOD = 306, /* T_OBJECT_TYPE_VAR_METHOD  */
    T_PACKAGE_CURSOR_ISOPEN = 307, /* T_PACKAGE_CURSOR_ISOPEN  */
    T_PACKAGE_CURSOR_FOUND = 308,  /* T_PACKAGE_CURSOR_FOUND  */
    T_PACKAGE_CURSOR_NOTFOUND = 309, /* T_PACKAGE_CURSOR_NOTFOUND  */
    T_PACKAGE_CURSOR_ROWCOUNT = 310, /* T_PACKAGE_CURSOR_ROWCOUNT  */
    LESS_LESS = 311,               /* LESS_LESS  */
    GREATER_GREATER = 312,         /* GREATER_GREATER  */
    T_REFCURSOR = 313,             /* T_REFCURSOR  */
    T_SQL_ISOPEN = 314,            /* T_SQL_ISOPEN  */
    T_SQL_FOUND = 315,             /* T_SQL_FOUND  */
    T_SQL_NOTFOUND = 316,          /* T_SQL_NOTFOUND  */
    T_SQL_ROWCOUNT = 317,          /* T_SQL_ROWCOUNT  */
    T_SQL_BULK_EXCEPTIONS = 318,   /* T_SQL_BULK_EXCEPTIONS  */
    T_CURSOR_ISOPEN = 319,         /* T_CURSOR_ISOPEN  */
    T_CURSOR_FOUND = 320,          /* T_CURSOR_FOUND  */
    T_CURSOR_NOTFOUND = 321,       /* T_CURSOR_NOTFOUND  */
    T_CURSOR_ROWCOUNT = 322,       /* T_CURSOR_ROWCOUNT  */
    T_DECLARE_CURSOR = 323,        /* T_DECLARE_CURSOR  */
    T_DECLARE_CONDITION = 324,     /* T_DECLARE_CONDITION  */
    T_DECLARE_HANDLER = 325,       /* T_DECLARE_HANDLER  */
    K_ABSOLUTE = 326,              /* K_ABSOLUTE  */
    K_ALIAS = 327,                 /* K_ALIAS  */
    K_ALL = 328,                   /* K_ALL  */
    K_ALTER = 329,                 /* K_ALTER  */
    K_ARRAY = 330,                 /* K_ARRAY  */
    K_AS = 331,                    /* K_AS  */
    K_BACKWARD = 332,              /* K_BACKWARD  */
    K_BEGIN = 333,                 /* K_BEGIN  */
    K_BULK = 334,                  /* K_BULK  */
    K_BY = 335,                    /* K_BY  */
    K_CALL = 336,                  /* K_CALL  */
    K_CASE = 337,                  /* K_CASE  */
    K_CATALOG_NAME = 338,          /* K_CATALOG_NAME  */
    K_CATCH = 339,                 /* K_CATCH  */
    K_CLASS_ORIGIN = 340,          /* K_CLASS_ORIGIN  */
    K_CLOSE = 341,                 /* K_CLOSE  */
    K_COLLATE = 342,               /* K_COLLATE  */
    K_COLLECT = 343,               /* K_COLLECT  */
    K_COLUMN_NAME = 344,           /* K_COLUMN_NAME  */
    K_COMMIT = 345,                /* K_COMMIT  */
    K_CONDITION = 346,             /* K_CONDITION  */
    K_CONSTANT = 347,              /* K_CONSTANT  */
    K_CONSTRAINT_CATALOG = 348,    /* K_CONSTRAINT_CATALOG  */
    K_CONSTRAINT_NAME = 349,       /* K_CONSTRAINT_NAME  */
    K_CONSTRAINT_SCHEMA = 350,     /* K_CONSTRAINT_SCHEMA  */
    K_CONTINUE = 351,              /* K_CONTINUE  */
    K_CURRENT = 352,               /* K_CURRENT  */
    K_CURSOR = 353,                /* K_CURSOR  */
    K_CURSOR_NAME = 354,           /* K_CURSOR_NAME  */
    K_DEBUG = 355,                 /* K_DEBUG  */
    K_DECLARE = 356,               /* K_DECLARE  */
    K_DEFAULT = 357,               /* K_DEFAULT  */
    K_DELETE = 358,                /* K_DELETE  */
    K_DETAIL = 359,                /* K_DETAIL  */
    K_DETERMINISTIC = 360,         /* K_DETERMINISTIC  */
    K_DIAGNOSTICS = 361,           /* K_DIAGNOSTICS  */
    K_DISTINCT = 362,              /* K_DISTINCT  */
    K_DO = 363,                    /* K_DO  */
    K_DUMP = 364,                  /* K_DUMP  */
    K_ELSE = 365,                  /* K_ELSE  */
    K_ELSIF = 366,                 /* K_ELSIF  */
    K_END = 367,                   /* K_END  */
    K_ERRCODE = 368,               /* K_ERRCODE  */
    K_ERROR = 369,                 /* K_ERROR  */
    K_EXCEPT = 370,                /* K_EXCEPT  */
    K_EXCEPTION = 371,             /* K_EXCEPTION  */
    K_EXCEPTIONS = 372,            /* K_EXCEPTIONS  */
    K_EXEC = 373,                  /* K_EXEC  */
    K_EXECUTE = 374,               /* K_EXECUTE  */
    K_EXIT = 375,                  /* K_EXIT  */
    K_FALSE = 376,                 /* K_FALSE  */
    K_FETCH = 377,                 /* K_FETCH  */
    K_FIRST = 378,                 /* K_FIRST  */
    K_FOR = 379,                   /* K_FOR  */
    K_FORALL = 380,                /* K_FORALL  */
    K_FOREACH = 381,               /* K_FOREACH  */
    K_FORWARD = 382,               /* K_FORWARD  */
    K_FOUND = 383,                 /* K_FOUND  */
    K_FROM = 384,                  /* K_FROM  */
    K_FUNCTION = 385,              /* K_FUNCTION  */
    K_GET = 386,                   /* K_GET  */
    K_GOTO = 387,                  /* K_GOTO  */
    K_HANDLER = 388,               /* K_HANDLER  */
    K_HINT = 389,                  /* K_HINT  */
    K_IF = 390,                    /* K_IF  */
    K_IMMEDIATE = 391,             /* K_IMMEDIATE  */
    K_INSTANTIATION = 392,         /* K_INSTANTIATION  */
    K_IN = 393,                    /* K_IN  */
    K_INDEX = 394,                 /* K_INDEX  */
    K_INFO = 395,                  /* K_INFO  */
    K_INSERT = 396,                /* K_INSERT  */
    K_INTERSECT = 397,             /* K_INTERSECT  */
    K_INTO = 398,                  /* K_INTO  */
    K_IS = 399,                    /* K_IS  */
    K_ITERATE = 400,               /* K_ITERATE  */
    K_LAST = 401,                  /* K_LAST  */
    K_LEAVE = 402,                 /* K_LEAVE  */
    K_LIMIT = 403,                 /* K_LIMIT  */
    K_LOG = 404,                   /* K_LOG  */
    K_LOOP = 405,                  /* K_LOOP  */
    K_MERGE = 406,                 /* K_MERGE  */
    K_MESSAGE = 407,               /* K_MESSAGE  */
    K_MESSAGE_TEXT = 408,          /* K_MESSAGE_TEXT  */
    K_MOVE = 409,                  /* K_MOVE  */
    K_MULTISET = 410,              /* K_MULTISET  */
    K_MULTISETS = 411,             /* K_MULTISETS  */
    K_MYSQL_ERRNO = 412,           /* K_MYSQL_ERRNO  */
    K_NUMBER = 413,                /* K_NUMBER  */
    K_NEXT = 414,                  /* K_NEXT  */
    K_NO = 415,                    /* K_NO  */
    K_NOT = 416,                   /* K_NOT  */
    K_NOTICE = 417,                /* K_NOTICE  */
    K_NULL = 418,                  /* K_NULL  */
    K_OF = 419,                    /* K_OF  */
    K_OPEN = 420,                  /* K_OPEN  */
    K_OPTION = 421,                /* K_OPTION  */
    K_OR = 422,                    /* K_OR  */
    K_OUT = 423,                   /* K_OUT  */
    K_PACKAGE = 424,               /* K_PACKAGE  */
    K_PERFORM = 425,               /* K_PERFORM  */
    K_PIPE = 426,                  /* K_PIPE  */
    K_PG_EXCEPTION_CONTEXT = 427,  /* K_PG_EXCEPTION_CONTEXT  */
    K_PG_EXCEPTION_DETAIL = 428,   /* K_PG_EXCEPTION_DETAIL  */
    K_PG_EXCEPTION_HINT = 429,     /* K_PG_EXCEPTION_HINT  */
    K_PRAGMA = 430,                /* K_PRAGMA  */
    K_PRIOR = 431,                 /* K_PRIOR  */
    K_PROCEDURE = 432,             /* K_PROCEDURE  */
    K_QUERY = 433,                 /* K_QUERY  */
    K_RAISE = 434,                 /* K_RAISE  */
    K_RECORD = 435,                /* K_RECORD  */
    K_REF = 436,                   /* K_REF  */
    K_RELATIVE = 437,              /* K_RELATIVE  */
    K_RELEASE = 438,               /* K_RELEASE  */
    K_REPEAT = 439,                /* K_REPEAT  */
    K_REPLACE = 440,               /* K_REPLACE  */
    K_RESULT_OID = 441,            /* K_RESULT_OID  */
    K_RESIGNAL = 442,              /* K_RESIGNAL  */
    K_RETURN = 443,                /* K_RETURN  */
    K_RETURNED_SQLSTATE = 444,     /* K_RETURNED_SQLSTATE  */
    K_REVERSE = 445,               /* K_REVERSE  */
    K_ROLLBACK = 446,              /* K_ROLLBACK  */
    K_ROW = 447,                   /* K_ROW  */
    K_ROWTYPE = 448,               /* K_ROWTYPE  */
    K_ROW_COUNT = 449,             /* K_ROW_COUNT  */
    K_SAVE = 450,                  /* K_SAVE  */
    K_SAVEPOINT = 451,             /* K_SAVEPOINT  */
    K_SCHEMA_NAME = 452,           /* K_SCHEMA_NAME  */
    K_SELECT = 453,                /* K_SELECT  */
    K_SCROLL = 454,                /* K_SCROLL  */
    K_SIGNAL = 455,                /* K_SIGNAL  */
    K_SLICE = 456,                 /* K_SLICE  */
    K_SQLEXCEPTION = 457,          /* K_SQLEXCEPTION  */
    K_SQLSTATE = 458,              /* K_SQLSTATE  */
    K_SQLWARNING = 459,            /* K_SQLWARNING  */
    K_STACKED = 460,               /* K_STACKED  */
    K_STRICT = 461,                /* K_STRICT  */
    K_SUBCLASS_ORIGIN = 462,       /* K_SUBCLASS_ORIGIN  */
    K_SUBTYPE = 463,               /* K_SUBTYPE  */
    K_SYS_REFCURSOR = 464,         /* K_SYS_REFCURSOR  */
    K_TABLE = 465,                 /* K_TABLE  */
    K_TABLE_NAME = 466,            /* K_TABLE_NAME  */
    K_THEN = 467,                  /* K_THEN  */
    K_TO = 468,                    /* K_TO  */
    K_TRAN = 469,                  /* K_TRAN  */
    K_TRANSACTION = 470,           /* K_TRANSACTION  */
    K_TRUE = 471,                  /* K_TRUE  */
    K_TRY = 472,                   /* K_TRY  */
    K_TYPE = 473,                  /* K_TYPE  */
    K_UNION = 474,                 /* K_UNION  */
    K_UNTIL = 475,                 /* K_UNTIL  */
    K_UPDATE = 476,                /* K_UPDATE  */
    K_USE_COLUMN = 477,            /* K_USE_COLUMN  */
    K_USE_VARIABLE = 478,          /* K_USE_VARIABLE  */
    K_USING = 479,                 /* K_USING  */
    K_VARIABLE_CONFLICT = 480,     /* K_VARIABLE_CONFLICT  */
    K_VARRAY = 481,                /* K_VARRAY  */
    K_WARNING = 482,               /* K_WARNING  */
    K_WHEN = 483,                  /* K_WHEN  */
    K_WHILE = 484,                 /* K_WHILE  */
    K_WITH = 485                   /* K_WITH  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 317 "src/pltsql/gram.y"

        core_YYSTYPE			core_yystype;
        /* these fields must match core_YYSTYPE: */
        int						ival;
        char					*str;
        const char				*keyword;

        PLword					word;
        PLcword					cword;
        PLwdatum				wdatum;
        bool					boolean;
        Oid						oid;
        VarName					*varname;
        struct
        {
            char *name;
            int  lineno;
            PLpgSQL_datum   *scalar;
            PLpgSQL_rec		*rec;
            PLpgSQL_row		*row;
            int  dno;
        }						forvariable;
        struct
        {
            char *label;
            int  n_initvars;
            int  *initvarnos;
            bool isAutonomous;
        }						declhdr;
        struct
        {
            List *stmts;
            char *end_label;
            int   end_label_location;
        }						loop_body;
        struct
        {
            List *stmts;
            char *end_label;
            int   end_label_location;
        }                                               while_body;
        struct
        {
            PLpgSQL_expr                    *expr;
            char *end_label;
            int   end_label_location;
        }                                               repeat_condition;
        struct
        {
            PLpgSQL_expr  *expr;
            int            endtoken;
        }                                               expr_until_while_loop;
        List					*list;
        PLpgSQL_type			*dtype;
        PLpgSQL_datum			*datum;
        PLpgSQL_var				*var;
        PLpgSQL_expr			*expr;
        PLpgSQL_stmt			*stmt;
        PLpgSQL_condition		*condition;
        PLpgSQL_exception		*exception;
        PLpgSQL_exception_block	*exception_block;
        PLpgSQL_nsitem			*nsitem;
        PLpgSQL_diag_item		*diagitem;
        PLpgSQL_stmt_fetch		*fetch;
        PLpgSQL_case_when		*casewhen;
        PLpgSQL_declare_handler declare_handler_type;
        PLpgSQL_rec_attr	*recattr;
        Node                            *plnode;
        DefElem             *def;

#line 365 "src/pltsql/pl_gram.hpp"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


extern THR_LOCAL YYSTYPE plpgsql_yylval;
extern THR_LOCAL YYLTYPE plpgsql_yylloc;
int pltsql_yyparse (void);

#endif /* !YY_PLPGSQL_YY_SRC_PLTSQL_PL_GRAM_HPP_INCLUDED  */
