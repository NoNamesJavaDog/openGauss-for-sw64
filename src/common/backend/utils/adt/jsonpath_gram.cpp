/* A Bison parser, made by GNU Bison 3.6.4.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.6.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 1

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1


/* Substitute the variable and function names.  */
#define yyparse         jsonpath_yyparse
#define yylex           jsonpath_yylex
#define yyerror         jsonpath_yyerror
#define yydebug         jsonpath_yydebug
#define yynerrs         jsonpath_yynerrs

/* First part of user prologue.  */
#line 1 "jsonpath_gram.y"

/*-------------------------------------------------------------------------
 *
 * jsonpath_gram.y
 *     Grammar definitions for jsonpath datatype
 *
 * Transforms tokenized jsonpath into tree of JsonPathParseItem structs.
 *
 * Portions Copyright (c) 2026 Huawei Technologies Co.,Ltd.
 * Copyright (c) 2019-2025, PostgreSQL Global Development Group
 *
 * IDENTIFICATION
 *    src/common/backend/utils/adt/jsonpath_gram.y
 *
 *-------------------------------------------------------------------------
 */

#include "postgres.h"

#include "catalog/pg_collation.h"
#include "fmgr.h"
#include "miscadmin.h"
#include "nodes/pg_list.h"
#include "parser/scanner.h"
#include "regex/regex.h"
#include "utils/builtins.h"
#include "utils/jsonpath.h"

static JsonPathParseItem *makeItemType(JsonPathItemType type);
static JsonPathParseItem *makeItemString(JsonPathString *s);
static JsonPathParseItem *makeItemKey(JsonPathString *s);
static JsonPathParseItem *makeItemNumeric(JsonPathString *s);
static JsonPathParseItem *makeItemBool(bool val);
static JsonPathParseItem *makeItemBinary(JsonPathItemType type,
                                         JsonPathParseItem *la,
                                         JsonPathParseItem *ra);
static JsonPathParseItem *makeItemList(List *list);
static JsonPathParseItem *makeIndexArray(List *list);
static JsonPathParseItem * makeItemVariable(JsonPathString *s);
static JsonPathParseItem *makeAny(int first, int last);
static JsonPathParseItem *makeItemUnary(JsonPathItemType type,
                                        JsonPathParseItem *a);
static bool makeItemLikeRegex(JsonPathParseItem *expr,
                              JsonPathString *pattern,
                              JsonPathString *flags,
                              JsonPathParseItem ** result);

static void checkAFormatIndex(JsonPathParseItem* item);

/*
 * Bison doesn't allocate anything that needs to live across parser calls,
 * so we can easily have it use palloc instead of malloc.  This prevents
 * memory leaks if we error out during parsing.
 */
#define YYMALLOC palloc
#define YYFREE   pfree

#define YY_DECL extern int     jsonpath_yylex(YYSTYPE *yylval_param, \
                              JsonPathParseResult **result)
YY_DECL;

static void
jsonpath_yyerror(const char *message);
#define yyerror(result, escontext, msg) jsonpath_yyerror(msg)


#line 143 "jsonpath_gram.cpp"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
#ifndef YY_JSONPATH_YY_JSONPATH_GRAM_HPP_INCLUDED
# define YY_JSONPATH_YY_JSONPATH_GRAM_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int jsonpath_yydebug;
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
    TO_P = 258,                    /* TO_P  */
    NULL_P = 259,                  /* NULL_P  */
    TRUE_P = 260,                  /* TRUE_P  */
    FALSE_P = 261,                 /* FALSE_P  */
    STRICT_P = 262,                /* STRICT_P  */
    LAX_P = 263,                   /* LAX_P  */
    IDENT_P = 264,                 /* IDENT_P  */
    STRING_P = 265,                /* STRING_P  */
    NUMERIC_P = 266,               /* NUMERIC_P  */
    INT_P = 267,                   /* INT_P  */
    VARIABLE_P = 268,              /* VARIABLE_P  */
    LESS_P = 269,                  /* LESS_P  */
    LESSEQUAL_P = 270,             /* LESSEQUAL_P  */
    EQUAL_P = 271,                 /* EQUAL_P  */
    NOTEQUAL_P = 272,              /* NOTEQUAL_P  */
    GREATEREQUAL_P = 273,          /* GREATEREQUAL_P  */
    GREATER_P = 274,               /* GREATER_P  */
    IS_P = 275,                    /* IS_P  */
    UNKNOWN_P = 276,               /* UNKNOWN_P  */
    EXISTS_P = 277,                /* EXISTS_P  */
    STARTS_P = 278,                /* STARTS_P  */
    WITH_P = 279,                  /* WITH_P  */
    LIKE_REGEX_P = 280,            /* LIKE_REGEX_P  */
    FLAG_P = 281,                  /* FLAG_P  */
    OR_P = 282,                    /* OR_P  */
    AND_P = 283,                   /* AND_P  */
    NOT_P = 284,                   /* NOT_P  */
    ABS_P = 285,                   /* ABS_P  */
    SIZE_P = 286,                  /* SIZE_P  */
    TYPE_P = 287,                  /* TYPE_P  */
    CEILING_P = 288,               /* CEILING_P  */
    FLOOR_P = 289,                 /* FLOOR_P  */
    KEYVALUE_P = 290,              /* KEYVALUE_P  */
    TIME_P = 291,                  /* TIME_P  */
    TIME_TZ_P = 292,               /* TIME_TZ_P  */
    TIMESTAMP_P = 293,             /* TIMESTAMP_P  */
    TIMESTAMP_TZ_P = 294,          /* TIMESTAMP_TZ_P  */
    DATE_P = 295,                  /* DATE_P  */
    DATETIME_P = 296,              /* DATETIME_P  */
    STRINGFUNC_P = 297,            /* STRINGFUNC_P  */
    NUMBER_P = 298,                /* NUMBER_P  */
    DECIMAL_P = 299,               /* DECIMAL_P  */
    BIGINT_P = 300,                /* BIGINT_P  */
    INTEGER_P = 301,               /* INTEGER_P  */
    DOUBLE_P = 302,                /* DOUBLE_P  */
    BOOLEAN_P = 303,               /* BOOLEAN_P  */
    ANY_P = 304,                   /* ANY_P  */
    LAST_P = 305,                  /* LAST_P  */
    UMINUS = 306                   /* UMINUS  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 77 "jsonpath_gram.y"

    JsonPathString        str;
    List               *elems;    /* list of JsonPathParseItem */
    List               *indexs;    /* list of integers */
    JsonPathParseItem  *value;
    JsonPathParseResult *result;
    JsonPathItemType    optype;
    bool                boolean;
    int                    integer;

#line 255 "jsonpath_gram.cpp"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif



int jsonpath_yyparse (JsonPathParseResult **result, struct Node *escontext);

#endif /* !YY_JSONPATH_YY_JSONPATH_GRAM_HPP_INCLUDED  */
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_TO_P = 3,                       /* TO_P  */
  YYSYMBOL_NULL_P = 4,                     /* NULL_P  */
  YYSYMBOL_TRUE_P = 5,                     /* TRUE_P  */
  YYSYMBOL_FALSE_P = 6,                    /* FALSE_P  */
  YYSYMBOL_STRICT_P = 7,                   /* STRICT_P  */
  YYSYMBOL_LAX_P = 8,                      /* LAX_P  */
  YYSYMBOL_IDENT_P = 9,                    /* IDENT_P  */
  YYSYMBOL_STRING_P = 10,                  /* STRING_P  */
  YYSYMBOL_NUMERIC_P = 11,                 /* NUMERIC_P  */
  YYSYMBOL_INT_P = 12,                     /* INT_P  */
  YYSYMBOL_VARIABLE_P = 13,                /* VARIABLE_P  */
  YYSYMBOL_LESS_P = 14,                    /* LESS_P  */
  YYSYMBOL_LESSEQUAL_P = 15,               /* LESSEQUAL_P  */
  YYSYMBOL_EQUAL_P = 16,                   /* EQUAL_P  */
  YYSYMBOL_NOTEQUAL_P = 17,                /* NOTEQUAL_P  */
  YYSYMBOL_GREATEREQUAL_P = 18,            /* GREATEREQUAL_P  */
  YYSYMBOL_GREATER_P = 19,                 /* GREATER_P  */
  YYSYMBOL_IS_P = 20,                      /* IS_P  */
  YYSYMBOL_UNKNOWN_P = 21,                 /* UNKNOWN_P  */
  YYSYMBOL_EXISTS_P = 22,                  /* EXISTS_P  */
  YYSYMBOL_STARTS_P = 23,                  /* STARTS_P  */
  YYSYMBOL_WITH_P = 24,                    /* WITH_P  */
  YYSYMBOL_LIKE_REGEX_P = 25,              /* LIKE_REGEX_P  */
  YYSYMBOL_FLAG_P = 26,                    /* FLAG_P  */
  YYSYMBOL_OR_P = 27,                      /* OR_P  */
  YYSYMBOL_AND_P = 28,                     /* AND_P  */
  YYSYMBOL_NOT_P = 29,                     /* NOT_P  */
  YYSYMBOL_ABS_P = 30,                     /* ABS_P  */
  YYSYMBOL_SIZE_P = 31,                    /* SIZE_P  */
  YYSYMBOL_TYPE_P = 32,                    /* TYPE_P  */
  YYSYMBOL_CEILING_P = 33,                 /* CEILING_P  */
  YYSYMBOL_FLOOR_P = 34,                   /* FLOOR_P  */
  YYSYMBOL_KEYVALUE_P = 35,                /* KEYVALUE_P  */
  YYSYMBOL_TIME_P = 36,                    /* TIME_P  */
  YYSYMBOL_TIME_TZ_P = 37,                 /* TIME_TZ_P  */
  YYSYMBOL_TIMESTAMP_P = 38,               /* TIMESTAMP_P  */
  YYSYMBOL_TIMESTAMP_TZ_P = 39,            /* TIMESTAMP_TZ_P  */
  YYSYMBOL_DATE_P = 40,                    /* DATE_P  */
  YYSYMBOL_DATETIME_P = 41,                /* DATETIME_P  */
  YYSYMBOL_STRINGFUNC_P = 42,              /* STRINGFUNC_P  */
  YYSYMBOL_NUMBER_P = 43,                  /* NUMBER_P  */
  YYSYMBOL_DECIMAL_P = 44,                 /* DECIMAL_P  */
  YYSYMBOL_BIGINT_P = 45,                  /* BIGINT_P  */
  YYSYMBOL_INTEGER_P = 46,                 /* INTEGER_P  */
  YYSYMBOL_DOUBLE_P = 47,                  /* DOUBLE_P  */
  YYSYMBOL_BOOLEAN_P = 48,                 /* BOOLEAN_P  */
  YYSYMBOL_ANY_P = 49,                     /* ANY_P  */
  YYSYMBOL_LAST_P = 50,                    /* LAST_P  */
  YYSYMBOL_51_ = 51,                       /* '+'  */
  YYSYMBOL_52_ = 52,                       /* '-'  */
  YYSYMBOL_53_ = 53,                       /* '*'  */
  YYSYMBOL_54_ = 54,                       /* '/'  */
  YYSYMBOL_55_ = 55,                       /* '%'  */
  YYSYMBOL_UMINUS = 56,                    /* UMINUS  */
  YYSYMBOL_57_ = 57,                       /* '('  */
  YYSYMBOL_58_ = 58,                       /* ')'  */
  YYSYMBOL_59_ = 59,                       /* '$'  */
  YYSYMBOL_60_ = 60,                       /* '@'  */
  YYSYMBOL_61_ = 61,                       /* ','  */
  YYSYMBOL_62_ = 62,                       /* '['  */
  YYSYMBOL_63_ = 63,                       /* ']'  */
  YYSYMBOL_64_ = 64,                       /* '{'  */
  YYSYMBOL_65_ = 65,                       /* '}'  */
  YYSYMBOL_66_ = 66,                       /* '.'  */
  YYSYMBOL_67_ = 67,                       /* '?'  */
  YYSYMBOL_YYACCEPT = 68,                  /* $accept  */
  YYSYMBOL_result = 69,                    /* result  */
  YYSYMBOL_expr_or_predicate = 70,         /* expr_or_predicate  */
  YYSYMBOL_mode = 71,                      /* mode  */
  YYSYMBOL_scalar_value = 72,              /* scalar_value  */
  YYSYMBOL_starts_with_initial = 73,       /* starts_with_initial  */
  YYSYMBOL_path_primary = 74,              /* path_primary  */
  YYSYMBOL_comp_op = 75,                   /* comp_op  */
  YYSYMBOL_delimited_predicate = 76,       /* delimited_predicate  */
  YYSYMBOL_predicate = 77,                 /* predicate  */
  YYSYMBOL_accessor_expr = 78,             /* accessor_expr  */
  YYSYMBOL_expr = 79,                      /* expr  */
  YYSYMBOL_index_elem = 80,                /* index_elem  */
  YYSYMBOL_index_list = 81,                /* index_list  */
  YYSYMBOL_array_accessor = 82,            /* array_accessor  */
  YYSYMBOL_any_level = 83,                 /* any_level  */
  YYSYMBOL_any_path = 84,                  /* any_path  */
  YYSYMBOL_accessor_op = 85,               /* accessor_op  */
  YYSYMBOL_csv_elem = 86,                  /* csv_elem  */
  YYSYMBOL_csv_list = 87,                  /* csv_list  */
  YYSYMBOL_opt_csv_list = 88,              /* opt_csv_list  */
  YYSYMBOL_datetime_precision = 89,        /* datetime_precision  */
  YYSYMBOL_opt_datetime_precision = 90,    /* opt_datetime_precision  */
  YYSYMBOL_datetime_template = 91,         /* datetime_template  */
  YYSYMBOL_opt_datetime_template = 92,     /* opt_datetime_template  */
  YYSYMBOL_key = 93,                       /* key  */
  YYSYMBOL_key_name = 94,                  /* key_name  */
  YYSYMBOL_method = 95                     /* method  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  5
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   239

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  68
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  28
/* YYNRULES -- Number of rules.  */
#define YYNRULES  136
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  180

#define YYMAXUTOK   306


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,    59,    55,     2,     2,
      57,    58,    53,    51,    61,    52,    66,    54,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    67,    60,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    62,     2,    63,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    64,     2,    65,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    56
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   125,   125,   131,   135,   136,   140,   151,   162,   166,
     167,   168,   169,   170,   171,   172,   176,   177,   181,   182,
     183,   184,   189,   190,   191,   192,   193,   194,   198,   199,
     203,   204,   205,   206,   207,   208,   209,   210,   218,   235,
     236,   237,   238,   242,   243,   244,   245,   246,   247,   248,
     249,   250,   254,   255,   259,   264,   272,   273,   277,   278,
     282,   283,   284,   289,   290,   291,   292,   293,   294,   295,
     310,   312,   314,   316,   318,   323,   325,   327,   332,   333,
     337,   338,   342,   346,   347,   351,   355,   356,   360,   364,
     365,   366,   367,   368,   369,   370,   371,   372,   373,   374,
     375,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   389,   390,   391,   392,   393,   394,
     395,   396,   397,   398,   402,   403,   404,   405,   406,   407,
     408,   409,   410,   411,   412,   413,   414
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "TO_P", "NULL_P",
  "TRUE_P", "FALSE_P", "STRICT_P", "LAX_P", "IDENT_P", "STRING_P",
  "NUMERIC_P", "INT_P", "VARIABLE_P", "LESS_P", "LESSEQUAL_P", "EQUAL_P",
  "NOTEQUAL_P", "GREATEREQUAL_P", "GREATER_P", "IS_P", "UNKNOWN_P",
  "EXISTS_P", "STARTS_P", "WITH_P", "LIKE_REGEX_P", "FLAG_P", "OR_P",
  "AND_P", "NOT_P", "ABS_P", "SIZE_P", "TYPE_P", "CEILING_P", "FLOOR_P",
  "KEYVALUE_P", "TIME_P", "TIME_TZ_P", "TIMESTAMP_P", "TIMESTAMP_TZ_P",
  "DATE_P", "DATETIME_P", "STRINGFUNC_P", "NUMBER_P", "DECIMAL_P",
  "BIGINT_P", "INTEGER_P", "DOUBLE_P", "BOOLEAN_P", "ANY_P", "LAST_P",
  "'+'", "'-'", "'*'", "'/'", "'%'", "UMINUS", "'('", "')'", "'$'", "'@'",
  "','", "'['", "']'", "'{'", "'}'", "'.'", "'?'", "$accept", "result",
  "expr_or_predicate", "mode", "scalar_value", "starts_with_initial",
  "path_primary", "comp_op", "delimited_predicate", "predicate",
  "accessor_expr", "expr", "index_elem", "index_list", "array_accessor",
  "any_level", "any_path", "accessor_op", "csv_elem", "csv_list",
  "opt_csv_list", "datetime_precision", "opt_datetime_precision",
  "datetime_template", "opt_datetime_template", "key", "key_name",
  "method", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,    43,    45,    42,    47,    37,   306,    40,    41,    36,
      64,    44,    91,    93,   123,   125,    46,    63
};
#endif

#define YYPACT_NINF (-49)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-137)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      11,   -49,   -49,     5,    31,   -49,   -49,   -49,   -49,   -49,
     -49,   -49,   -49,   -48,   -12,   -49,    74,    74,    31,   -49,
     -49,   -49,   -49,   -49,   -49,    47,   -46,   184,    74,    31,
     -49,    31,   -49,   -49,   -20,   172,    31,    31,    60,   132,
     -32,   -49,   -49,   -49,   -49,   -49,   -49,   -49,   -49,     2,
      19,    74,    74,    74,    74,    74,    74,    92,     4,   184,
      49,    39,   -46,    18,   -49,     6,     3,   -49,    53,   -49,
     -49,   -49,   -49,   -49,   -49,   -49,   -49,   -49,   -49,   -49,
     -49,   -49,   -49,   -49,    32,    35,    42,    43,    51,    52,
      58,    61,    64,    72,    73,    75,    91,    94,   102,   103,
     104,   126,   127,    -3,   -49,   -49,   -49,   -49,   -49,   135,
      31,    20,   101,    40,    40,   -49,   -49,   -49,   159,   -49,
     -49,   -46,   128,   -49,   -49,   -49,    74,    74,   -49,   181,
     181,   181,   181,   186,    16,   -10,   136,    70,   -49,   -49,
     -49,   194,   -49,   159,   -49,   -49,   -49,   147,   148,   150,
     157,   -49,   -49,   158,   -49,   205,   206,   -49,   160,   161,
     -49,   -49,    -2,   -49,   -49,   -49,   -49,   -49,   -49,   -49,
     -49,   -49,   -49,    16,   -49,   -10,   -49,   -49,   155,   -49
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       8,     6,     7,     0,     0,     1,    10,    11,    12,     9,
      13,    14,    15,     0,     0,    21,     0,     0,     0,    19,
      20,     2,    18,    39,    30,     5,    43,     4,     0,     0,
      34,     0,    46,    45,     0,     0,     0,     0,     0,     0,
       0,    65,    42,    24,    26,    22,    23,    27,    25,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    28,    44,    33,    32,     0,    52,    54,     0,    91,
      92,    93,    94,    98,    99,    89,    90,    95,    96,    97,
     107,   108,   109,   110,   100,   101,   102,   104,   103,   105,
     120,   121,   122,   123,   118,   119,   111,   113,   112,   116,
     117,   114,   115,    60,   106,    64,    66,    63,    88,     0,
       0,     0,    37,    47,    48,    49,    50,    51,    31,    29,
      28,     0,     0,    41,    40,    56,     0,     0,    57,    84,
      84,    84,    84,    87,    81,     0,     0,     0,    16,    17,
      36,     0,    35,    53,    55,    82,    83,     0,     0,     0,
       0,    85,    86,     0,    75,     0,     0,    78,    80,     0,
      58,    59,     0,    67,    68,    38,    71,    72,    73,    74,
      70,    76,    77,     0,    69,     0,    61,    79,     0,    62
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -49,   -49,   -49,   -49,   -49,   -49,   -49,   -49,   208,   -14,
     -49,    -4,   105,   -49,   -49,    54,   -49,   -23,    55,   -49,
     -49,   -49,   -28,   -49,   -49,   -49,   -49,   -49
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     3,    21,     4,    22,   140,    23,    56,    24,    25,
      26,    59,    67,    68,    41,   162,   106,   123,   157,   158,
     159,   146,   147,   152,   153,   107,   108,   109
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      27,   175,   160,    42,    34,     5,   126,    36,    37,    28,
      13,    -3,    32,    33,    35,    58,    38,    60,     1,     2,
      39,    40,    63,    64,    57,   110,   111,    35,   154,   112,
     138,    36,    37,   139,    66,     6,     7,     8,    61,   124,
     161,     9,    10,    11,    12,    29,    37,   113,   114,   115,
     116,   117,   118,    13,    51,    52,    53,    54,    55,   122,
      14,   135,   120,   176,     6,     7,     8,   155,   156,   125,
       9,    10,    11,    12,    36,    37,    36,    37,     6,     7,
       8,    15,    16,    17,     9,    10,    11,    12,    18,  -124,
      19,    20,  -125,    53,    54,    55,   137,    36,    37,  -126,
    -128,    38,   148,   149,   150,    39,    40,   121,  -127,  -129,
      15,    16,    17,    65,   127,   129,   128,    31,   130,    19,
      20,   131,   143,    66,    15,    16,    17,   141,   164,   132,
    -130,    31,   133,    19,    20,    69,    70,    71,    72,    73,
      74,    75,    76,    51,    52,    53,    54,    55,  -132,   142,
     119,  -131,    77,    78,    79,    80,    81,    82,    83,   134,
    -135,  -136,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,  -133,  -134,   105,    43,    44,    45,    46,
      47,    48,   136,   145,   163,    49,   151,    50,    43,    44,
      45,    46,    47,    48,   165,   166,   167,    49,   168,    50,
      51,    52,    53,    54,    55,   169,   170,   171,   172,   174,
     179,   173,    30,    51,    52,    53,    54,    55,   177,   178,
      62,     0,   144,     0,     0,    51,    52,    53,    54,    55
};

static const yytype_int16 yycheck[] =
{
       4,     3,    12,    26,    18,     0,     3,    27,    28,    57,
      22,     0,    16,    17,    18,    29,    62,    31,     7,     8,
      66,    67,    36,    37,    28,    57,    24,    31,    12,    10,
      10,    27,    28,    13,    38,     4,     5,     6,    58,    62,
      50,    10,    11,    12,    13,    57,    28,    51,    52,    53,
      54,    55,    56,    22,    51,    52,    53,    54,    55,    20,
      29,    64,    58,    65,     4,     5,     6,    51,    52,    63,
      10,    11,    12,    13,    27,    28,    27,    28,     4,     5,
       6,    50,    51,    52,    10,    11,    12,    13,    57,    57,
      59,    60,    57,    53,    54,    55,   110,    27,    28,    57,
      57,    62,   130,   131,   132,    66,    67,    58,    57,    57,
      50,    51,    52,    53,    61,    57,    63,    57,    57,    59,
      60,    57,   126,   127,    50,    51,    52,    26,    58,    57,
      57,    57,    57,    59,    60,     3,     4,     5,     6,     7,
       8,     9,    10,    51,    52,    53,    54,    55,    57,    21,
      58,    57,    20,    21,    22,    23,    24,    25,    26,    57,
      57,    57,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    57,    57,    53,    14,    15,    16,    17,
      18,    19,    57,    12,    58,    23,    10,    25,    14,    15,
      16,    17,    18,    19,    10,    58,    58,    23,    58,    25,
      51,    52,    53,    54,    55,    58,    58,    12,    12,    58,
      65,    61,    14,    51,    52,    53,    54,    55,   173,   175,
      58,    -1,   127,    -1,    -1,    51,    52,    53,    54,    55
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     7,     8,    69,    71,     0,     4,     5,     6,    10,
      11,    12,    13,    22,    29,    50,    51,    52,    57,    59,
      60,    70,    72,    74,    76,    77,    78,    79,    57,    57,
      76,    57,    79,    79,    77,    79,    27,    28,    62,    66,
      67,    82,    85,    14,    15,    16,    17,    18,    19,    23,
      25,    51,    52,    53,    54,    55,    75,    79,    77,    79,
      77,    58,    58,    77,    77,    53,    79,    80,    81,     3,
       4,     5,     6,     7,     8,     9,    10,    20,    21,    22,
      23,    24,    25,    26,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    53,    84,    93,    94,    95,
      57,    24,    10,    79,    79,    79,    79,    79,    79,    58,
      58,    58,    20,    85,    85,    63,     3,    61,    63,    57,
      57,    57,    57,    57,    57,    64,    57,    77,    10,    13,
      73,    26,    21,    79,    80,    12,    89,    90,    90,    90,
      90,    10,    91,    92,    12,    51,    52,    86,    87,    88,
      12,    50,    83,    58,    58,    10,    58,    58,    58,    58,
      58,    12,    12,    61,    58,     3,    65,    86,    83,    65
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    68,    69,    69,    70,    70,    71,    71,    71,    72,
      72,    72,    72,    72,    72,    72,    73,    73,    74,    74,
      74,    74,    75,    75,    75,    75,    75,    75,    76,    76,
      77,    77,    77,    77,    77,    77,    77,    77,    77,    78,
      78,    78,    78,    79,    79,    79,    79,    79,    79,    79,
      79,    79,    80,    80,    81,    81,    82,    82,    83,    83,
      84,    84,    84,    85,    85,    85,    85,    85,    85,    85,
      85,    85,    85,    85,    85,    86,    86,    86,    87,    87,
      88,    88,    89,    90,    90,    91,    92,    92,    93,    94,
      94,    94,    94,    94,    94,    94,    94,    94,    94,    94,
      94,    94,    94,    94,    94,    94,    94,    94,    94,    94,
      94,    94,    94,    94,    94,    94,    94,    94,    94,    94,
      94,    94,    94,    94,    95,    95,    95,    95,    95,    95,
      95,    95,    95,    95,    95,    95,    95
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     2,     0,     1,     1,     1,     1,     0,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     3,     4,
       1,     3,     3,     3,     2,     5,     4,     3,     5,     1,
       4,     4,     2,     1,     3,     2,     2,     3,     3,     3,
       3,     3,     1,     3,     1,     3,     3,     3,     1,     1,
       1,     4,     6,     2,     2,     1,     2,     4,     4,     5,
       5,     5,     5,     5,     5,     1,     2,     2,     1,     3,
       1,     0,     1,     1,     0,     1,     1,     0,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (result, escontext, YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
# ifndef YY_LOCATION_PRINT
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif


# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value, result, escontext); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, JsonPathParseResult **result, struct Node *escontext)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  YYUSE (result);
  YYUSE (escontext);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yykind < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yykind], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, JsonPathParseResult **result, struct Node *escontext)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep, result, escontext);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule, JsonPathParseResult **result, struct Node *escontext)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)], result, escontext);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule, result, escontext); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, JsonPathParseResult **result, struct Node *escontext)
{
  YYUSE (yyvaluep);
  YYUSE (result);
  YYUSE (escontext);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}






/*----------.
| yyparse.  |
`----------*/

int
yyparse (JsonPathParseResult **result, struct Node *escontext)
{
/* The lookahead symbol.  */
int yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
YY_INITIAL_VALUE (static YYSTYPE yyval_default;)
YYSTYPE yylval YY_INITIAL_VALUE (= yyval_default);

    /* Number of syntax errors so far.  */
    int yynerrs;

    yy_state_fast_t yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize;

    /* The state stack.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss;
    yy_state_t *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yynerrs = 0;
  yystate = 0;
  yyerrstatus = 0;

  yystacksize = YYINITDEPTH;
  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;


  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex (&yylval, result);
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2:
#line 125 "jsonpath_gram.y"
                                      {
                                        *result = (JsonPathParseResult*)palloc(sizeof(JsonPathParseResult));
                                        (*result)->expr = (yyvsp[0].value);
                                        (*result)->lax = (yyvsp[-1].boolean);
                                        (void) yynerrs;
                                 }
#line 1501 "jsonpath_gram.cpp"
    break;

  case 3:
#line 131 "jsonpath_gram.y"
                                     { *result = NULL; }
#line 1507 "jsonpath_gram.cpp"
    break;

  case 4:
#line 135 "jsonpath_gram.y"
                                    { (yyval.value) = (yyvsp[0].value); }
#line 1513 "jsonpath_gram.cpp"
    break;

  case 5:
#line 136 "jsonpath_gram.y"
                                       { (yyval.value) = (yyvsp[0].value); }
#line 1519 "jsonpath_gram.cpp"
    break;

  case 6:
#line 141 "jsonpath_gram.y"
    {
        if (DB_IS_CMPT(A_FORMAT)) {
            ereport(ERROR,
                    (errcode(ERRCODE_SYNTAX_ERROR),
                        errmsg("invalid input syntax for type %s", "jsonpath"),
                        errdetail("specifying strict/lax mode is not supported "
                            "in A_FORMAT database")));
        }
        (yyval.boolean) = false;
    }
#line 1534 "jsonpath_gram.cpp"
    break;

  case 7:
#line 152 "jsonpath_gram.y"
    {
        if (DB_IS_CMPT(A_FORMAT)) {
            ereport(ERROR,
                    (errcode(ERRCODE_SYNTAX_ERROR),
                        errmsg("invalid input syntax for type %s", "jsonpath"),
                        errdetail("specifying strict/lax mode is not supported "
                            "in A_FORMAT database")));
        }
        (yyval.boolean) = true;
    }
#line 1549 "jsonpath_gram.cpp"
    break;

  case 8:
#line 162 "jsonpath_gram.y"
                                     { (yyval.boolean) = true; }
#line 1555 "jsonpath_gram.cpp"
    break;

  case 9:
#line 166 "jsonpath_gram.y"
                                    { (yyval.value) = makeItemString(&(yyvsp[0].str)); }
#line 1561 "jsonpath_gram.cpp"
    break;

  case 10:
#line 167 "jsonpath_gram.y"
                                    { (yyval.value) = makeItemString(NULL); }
#line 1567 "jsonpath_gram.cpp"
    break;

  case 11:
#line 168 "jsonpath_gram.y"
                                    { (yyval.value) = makeItemBool(true); }
#line 1573 "jsonpath_gram.cpp"
    break;

  case 12:
#line 169 "jsonpath_gram.y"
                                     { (yyval.value) = makeItemBool(false); }
#line 1579 "jsonpath_gram.cpp"
    break;

  case 13:
#line 170 "jsonpath_gram.y"
                                       { (yyval.value) = makeItemNumeric(&(yyvsp[0].str)); }
#line 1585 "jsonpath_gram.cpp"
    break;

  case 14:
#line 171 "jsonpath_gram.y"
                                       { (yyval.value) = makeItemNumeric(&(yyvsp[0].str)); }
#line 1591 "jsonpath_gram.cpp"
    break;

  case 15:
#line 172 "jsonpath_gram.y"
                                    { (yyval.value) = makeItemVariable(&(yyvsp[0].str)); }
#line 1597 "jsonpath_gram.cpp"
    break;

  case 16:
#line 176 "jsonpath_gram.y"
                                    { (yyval.value) = makeItemString(&(yyvsp[0].str)); }
#line 1603 "jsonpath_gram.cpp"
    break;

  case 17:
#line 177 "jsonpath_gram.y"
                                    { (yyval.value) = makeItemVariable(&(yyvsp[0].str)); }
#line 1609 "jsonpath_gram.cpp"
    break;

  case 18:
#line 181 "jsonpath_gram.y"
                                    { (yyval.value) = (yyvsp[0].value); }
#line 1615 "jsonpath_gram.cpp"
    break;

  case 19:
#line 182 "jsonpath_gram.y"
                                     { (yyval.value) = makeItemType(JPI_ROOT); }
#line 1621 "jsonpath_gram.cpp"
    break;

  case 20:
#line 183 "jsonpath_gram.y"
                                     { (yyval.value) = makeItemType(JPI_CURRENT); }
#line 1627 "jsonpath_gram.cpp"
    break;

  case 21:
#line 184 "jsonpath_gram.y"
                                    { (yyval.value) = makeItemType(JPI_LAST); }
#line 1633 "jsonpath_gram.cpp"
    break;

  case 22:
#line 189 "jsonpath_gram.y"
                                       { (yyval.optype) = JPI_EQUAL; }
#line 1639 "jsonpath_gram.cpp"
    break;

  case 23:
#line 190 "jsonpath_gram.y"
                                    { (yyval.optype) = JPI_NOT_EQUAL; }
#line 1645 "jsonpath_gram.cpp"
    break;

  case 24:
#line 191 "jsonpath_gram.y"
                                    { (yyval.optype) = JPI_LESS; }
#line 1651 "jsonpath_gram.cpp"
    break;

  case 25:
#line 192 "jsonpath_gram.y"
                                       { (yyval.optype) = JPI_GREATER; }
#line 1657 "jsonpath_gram.cpp"
    break;

  case 26:
#line 193 "jsonpath_gram.y"
                                     { (yyval.optype) = JPI_LESS_OR_EQUAL; }
#line 1663 "jsonpath_gram.cpp"
    break;

  case 27:
#line 194 "jsonpath_gram.y"
                                    { (yyval.optype) = JPI_GREATER_OR_EQUAL; }
#line 1669 "jsonpath_gram.cpp"
    break;

  case 28:
#line 198 "jsonpath_gram.y"
                                     { (yyval.value) = (yyvsp[-1].value); }
#line 1675 "jsonpath_gram.cpp"
    break;

  case 29:
#line 199 "jsonpath_gram.y"
                                       { (yyval.value) = makeItemUnary(JPI_EXISTS, (yyvsp[-1].value)); }
#line 1681 "jsonpath_gram.cpp"
    break;

  case 30:
#line 203 "jsonpath_gram.y"
                                       { (yyval.value) = (yyvsp[0].value); }
#line 1687 "jsonpath_gram.cpp"
    break;

  case 31:
#line 204 "jsonpath_gram.y"
                                       { (yyval.value) = makeItemBinary((yyvsp[-1].optype), (yyvsp[-2].value), (yyvsp[0].value)); }
#line 1693 "jsonpath_gram.cpp"
    break;

  case 32:
#line 205 "jsonpath_gram.y"
                                       { (yyval.value) = makeItemBinary(JPI_AND, (yyvsp[-2].value), (yyvsp[0].value)); }
#line 1699 "jsonpath_gram.cpp"
    break;

  case 33:
#line 206 "jsonpath_gram.y"
                                      { (yyval.value) = makeItemBinary(JPI_OR, (yyvsp[-2].value), (yyvsp[0].value)); }
#line 1705 "jsonpath_gram.cpp"
    break;

  case 34:
#line 207 "jsonpath_gram.y"
                                       { (yyval.value) = makeItemUnary(JPI_NOT, (yyvsp[0].value)); }
#line 1711 "jsonpath_gram.cpp"
    break;

  case 35:
#line 208 "jsonpath_gram.y"
                                       { (yyval.value) = makeItemUnary(JPI_IS_UNKNOWN, (yyvsp[-3].value)); }
#line 1717 "jsonpath_gram.cpp"
    break;

  case 36:
#line 209 "jsonpath_gram.y"
                                               { (yyval.value) = makeItemBinary(JPI_STARTS_WITH, (yyvsp[-3].value), (yyvsp[0].value)); }
#line 1723 "jsonpath_gram.cpp"
    break;

  case 37:
#line 211 "jsonpath_gram.y"
    {
        JsonPathParseItem *jppitem;
        if (! makeItemLikeRegex((yyvsp[-2].value), &(yyvsp[0].str), NULL, &jppitem)) {
            YYABORT;
        }
        (yyval.value) = jppitem;
    }
#line 1735 "jsonpath_gram.cpp"
    break;

  case 38:
#line 219 "jsonpath_gram.y"
    {
        JsonPathParseItem *jppitem;
        if (DB_IS_CMPT(A_FORMAT)) {
            ereport(ERROR,
                    (errcode(ERRCODE_SYNTAX_ERROR),
                        errmsg("invalid input syntax for type %s", "jsonpath"),
                        errdetail("specifying like_regex flags is not supportted for A_FORMAT jsonpath")));
        }
        if (! makeItemLikeRegex((yyvsp[-4].value), &(yyvsp[-2].str), &(yyvsp[0].str), &jppitem)) {
            YYABORT;
        }
        (yyval.value) = jppitem;
    }
#line 1753 "jsonpath_gram.cpp"
    break;

  case 39:
#line 235 "jsonpath_gram.y"
                                    { (yyval.elems) = list_make1((yyvsp[0].value)); }
#line 1759 "jsonpath_gram.cpp"
    break;

  case 40:
#line 236 "jsonpath_gram.y"
                                      { (yyval.elems) = list_make2((yyvsp[-2].value), (yyvsp[0].value)); }
#line 1765 "jsonpath_gram.cpp"
    break;

  case 41:
#line 237 "jsonpath_gram.y"
                                       { (yyval.elems) = list_make2((yyvsp[-2].value), (yyvsp[0].value)); }
#line 1771 "jsonpath_gram.cpp"
    break;

  case 42:
#line 238 "jsonpath_gram.y"
                                       { (yyval.elems) = lappend((yyvsp[-1].elems), (yyvsp[0].value)); }
#line 1777 "jsonpath_gram.cpp"
    break;

  case 43:
#line 242 "jsonpath_gram.y"
                                     { (yyval.value) = makeItemList((yyvsp[0].elems)); }
#line 1783 "jsonpath_gram.cpp"
    break;

  case 44:
#line 243 "jsonpath_gram.y"
                                      { (yyval.value) = (yyvsp[-1].value); }
#line 1789 "jsonpath_gram.cpp"
    break;

  case 45:
#line 244 "jsonpath_gram.y"
                                       { (yyval.value) = makeItemUnary(JPI_MINUS, (yyvsp[0].value)); }
#line 1795 "jsonpath_gram.cpp"
    break;

  case 46:
#line 245 "jsonpath_gram.y"
                                       { (yyval.value) = makeItemUnary(JPI_PLUS, (yyvsp[0].value)); }
#line 1801 "jsonpath_gram.cpp"
    break;

  case 47:
#line 246 "jsonpath_gram.y"
                                       { (yyval.value) = makeItemBinary(JPI_ADD, (yyvsp[-2].value), (yyvsp[0].value)); }
#line 1807 "jsonpath_gram.cpp"
    break;

  case 48:
#line 247 "jsonpath_gram.y"
                                       { (yyval.value) = makeItemBinary(JPI_SUB, (yyvsp[-2].value), (yyvsp[0].value)); }
#line 1813 "jsonpath_gram.cpp"
    break;

  case 49:
#line 248 "jsonpath_gram.y"
                                       { (yyval.value) = makeItemBinary(JPI_MUL, (yyvsp[-2].value), (yyvsp[0].value)); }
#line 1819 "jsonpath_gram.cpp"
    break;

  case 50:
#line 249 "jsonpath_gram.y"
                                       { (yyval.value) = makeItemBinary(JPI_DIV, (yyvsp[-2].value), (yyvsp[0].value)); }
#line 1825 "jsonpath_gram.cpp"
    break;

  case 51:
#line 250 "jsonpath_gram.y"
                                       { (yyval.value) = makeItemBinary(JPI_MOD, (yyvsp[-2].value), (yyvsp[0].value)); }
#line 1831 "jsonpath_gram.cpp"
    break;

  case 52:
#line 254 "jsonpath_gram.y"
                                    { (yyval.value) = makeItemBinary(JPI_SUBSCRIPT, (yyvsp[0].value), NULL); }
#line 1837 "jsonpath_gram.cpp"
    break;

  case 53:
#line 255 "jsonpath_gram.y"
                                    { (yyval.value) = makeItemBinary(JPI_SUBSCRIPT, (yyvsp[-2].value), (yyvsp[0].value)); }
#line 1843 "jsonpath_gram.cpp"
    break;

  case 54:
#line 260 "jsonpath_gram.y"
    {
        checkAFormatIndex((yyvsp[0].value));
        (yyval.indexs) = list_make1((yyvsp[0].value));
    }
#line 1852 "jsonpath_gram.cpp"
    break;

  case 55:
#line 265 "jsonpath_gram.y"
    {
        checkAFormatIndex((yyvsp[0].value));
        (yyval.indexs) = lappend((yyvsp[-2].indexs), (yyvsp[0].value));
    }
#line 1861 "jsonpath_gram.cpp"
    break;

  case 56:
#line 272 "jsonpath_gram.y"
                                       { (yyval.value) = makeItemType(JPI_ANY_ARRAY); }
#line 1867 "jsonpath_gram.cpp"
    break;

  case 57:
#line 273 "jsonpath_gram.y"
                                    { (yyval.value) = makeIndexArray((yyvsp[-1].indexs)); }
#line 1873 "jsonpath_gram.cpp"
    break;

  case 58:
#line 277 "jsonpath_gram.y"
                                     { (yyval.integer) = pg_strtoint32((yyvsp[0].str).val); }
#line 1879 "jsonpath_gram.cpp"
    break;

  case 59:
#line 278 "jsonpath_gram.y"
                                    { (yyval.integer) = -1; }
#line 1885 "jsonpath_gram.cpp"
    break;

  case 60:
#line 282 "jsonpath_gram.y"
                                     { (yyval.value) = makeAny(0, -1); }
#line 1891 "jsonpath_gram.cpp"
    break;

  case 61:
#line 283 "jsonpath_gram.y"
                                     { (yyval.value) = makeAny((yyvsp[-1].integer), (yyvsp[-1].integer)); }
#line 1897 "jsonpath_gram.cpp"
    break;

  case 62:
#line 285 "jsonpath_gram.y"
                                    { (yyval.value) = makeAny((yyvsp[-3].integer), (yyvsp[-1].integer)); }
#line 1903 "jsonpath_gram.cpp"
    break;

  case 63:
#line 289 "jsonpath_gram.y"
                                       { (yyval.value) = (yyvsp[0].value); }
#line 1909 "jsonpath_gram.cpp"
    break;

  case 64:
#line 290 "jsonpath_gram.y"
                                     { (yyval.value) = makeItemType(JPI_ANY_KEY); }
#line 1915 "jsonpath_gram.cpp"
    break;

  case 65:
#line 291 "jsonpath_gram.y"
                                    { (yyval.value) = (yyvsp[0].value); }
#line 1921 "jsonpath_gram.cpp"
    break;

  case 66:
#line 292 "jsonpath_gram.y"
                                      { (yyval.value) = (yyvsp[0].value); }
#line 1927 "jsonpath_gram.cpp"
    break;

  case 67:
#line 293 "jsonpath_gram.y"
                                    { (yyval.value) = makeItemType((yyvsp[-2].optype)); }
#line 1933 "jsonpath_gram.cpp"
    break;

  case 68:
#line 294 "jsonpath_gram.y"
                                       { (yyval.value) = makeItemUnary(JPI_FILTER, (yyvsp[-1].value)); }
#line 1939 "jsonpath_gram.cpp"
    break;

  case 69:
#line 296 "jsonpath_gram.y"
        {
            if (list_length((yyvsp[-1].elems)) == 0) {
                (yyval.value) = makeItemBinary(JPI_DECIMAL, NULL, NULL);
            } else if (list_length((yyvsp[-1].elems)) == 1) {
                (yyval.value) = makeItemBinary(JPI_DECIMAL, (JsonPathParseItem*)linitial((yyvsp[-1].elems)), NULL);
            } else if (list_length((yyvsp[-1].elems)) == 2) {
                (yyval.value) = makeItemBinary(JPI_DECIMAL, (JsonPathParseItem*)linitial((yyvsp[-1].elems)), (JsonPathParseItem*)lsecond((yyvsp[-1].elems)));
            } else {
                ereport(ERROR,
                        (errcode(ERRCODE_SYNTAX_ERROR),
                         errmsg("invalid input syntax for type %s", "jsonpath"),
                         errdetail(".decimal() can only have an optional precision[,scale].")));
            }
        }
#line 1958 "jsonpath_gram.cpp"
    break;

  case 70:
#line 311 "jsonpath_gram.y"
        { (yyval.value) = makeItemUnary(JPI_DATETIME, (yyvsp[-1].value)); }
#line 1964 "jsonpath_gram.cpp"
    break;

  case 71:
#line 313 "jsonpath_gram.y"
        { (yyval.value) = makeItemUnary(JPI_TIME, (yyvsp[-1].value)); }
#line 1970 "jsonpath_gram.cpp"
    break;

  case 72:
#line 315 "jsonpath_gram.y"
        { (yyval.value) = makeItemUnary(JPI_TIME_TZ, (yyvsp[-1].value)); }
#line 1976 "jsonpath_gram.cpp"
    break;

  case 73:
#line 317 "jsonpath_gram.y"
        { (yyval.value) = makeItemUnary(JPI_TIMESTAMP, (yyvsp[-1].value)); }
#line 1982 "jsonpath_gram.cpp"
    break;

  case 74:
#line 319 "jsonpath_gram.y"
        { (yyval.value) = makeItemUnary(JPI_TIMESTAMP_TZ, (yyvsp[-1].value)); }
#line 1988 "jsonpath_gram.cpp"
    break;

  case 75:
#line 324 "jsonpath_gram.y"
        { (yyval.value) = makeItemNumeric(&(yyvsp[0].str)); }
#line 1994 "jsonpath_gram.cpp"
    break;

  case 76:
#line 326 "jsonpath_gram.y"
        { (yyval.value) = makeItemUnary(JPI_PLUS, makeItemNumeric(&(yyvsp[0].str))); }
#line 2000 "jsonpath_gram.cpp"
    break;

  case 77:
#line 328 "jsonpath_gram.y"
        { (yyval.value) = makeItemUnary(JPI_MINUS, makeItemNumeric(&(yyvsp[0].str))); }
#line 2006 "jsonpath_gram.cpp"
    break;

  case 78:
#line 332 "jsonpath_gram.y"
                                    { (yyval.elems) = list_make1((yyvsp[0].value)); }
#line 2012 "jsonpath_gram.cpp"
    break;

  case 79:
#line 333 "jsonpath_gram.y"
                                       { (yyval.elems) = lappend((yyvsp[-2].elems), (yyvsp[0].value)); }
#line 2018 "jsonpath_gram.cpp"
    break;

  case 80:
#line 337 "jsonpath_gram.y"
                                    { (yyval.elems) = (yyvsp[0].elems); }
#line 2024 "jsonpath_gram.cpp"
    break;

  case 81:
#line 338 "jsonpath_gram.y"
                                     { (yyval.elems) = NULL; }
#line 2030 "jsonpath_gram.cpp"
    break;

  case 82:
#line 342 "jsonpath_gram.y"
                                     { (yyval.value) = makeItemNumeric(&(yyvsp[0].str)); }
#line 2036 "jsonpath_gram.cpp"
    break;

  case 83:
#line 346 "jsonpath_gram.y"
                                      { (yyval.value) = (yyvsp[0].value); }
#line 2042 "jsonpath_gram.cpp"
    break;

  case 84:
#line 347 "jsonpath_gram.y"
                                     { (yyval.value) = NULL; }
#line 2048 "jsonpath_gram.cpp"
    break;

  case 85:
#line 351 "jsonpath_gram.y"
                                    { (yyval.value) = makeItemString(&(yyvsp[0].str)); }
#line 2054 "jsonpath_gram.cpp"
    break;

  case 86:
#line 355 "jsonpath_gram.y"
                                     { (yyval.value) = (yyvsp[0].value); }
#line 2060 "jsonpath_gram.cpp"
    break;

  case 87:
#line 356 "jsonpath_gram.y"
                                     { (yyval.value) = NULL; }
#line 2066 "jsonpath_gram.cpp"
    break;

  case 88:
#line 360 "jsonpath_gram.y"
                                    { (yyval.value) = makeItemKey(&(yyvsp[0].str)); }
#line 2072 "jsonpath_gram.cpp"
    break;

  case 124:
#line 402 "jsonpath_gram.y"
                                     { (yyval.optype) = JPI_ABS; }
#line 2078 "jsonpath_gram.cpp"
    break;

  case 125:
#line 403 "jsonpath_gram.y"
                                    { (yyval.optype) = JPI_SIZE; }
#line 2084 "jsonpath_gram.cpp"
    break;

  case 126:
#line 404 "jsonpath_gram.y"
                                    { (yyval.optype) = JPI_TYPE; }
#line 2090 "jsonpath_gram.cpp"
    break;

  case 127:
#line 405 "jsonpath_gram.y"
                                     { (yyval.optype) = JPI_FLOOR; }
#line 2096 "jsonpath_gram.cpp"
    break;

  case 128:
#line 406 "jsonpath_gram.y"
                                       { (yyval.optype) = JPI_CEILING; }
#line 2102 "jsonpath_gram.cpp"
    break;

  case 129:
#line 407 "jsonpath_gram.y"
                                    { (yyval.optype) = JPI_KEYVALUE; }
#line 2108 "jsonpath_gram.cpp"
    break;

  case 130:
#line 408 "jsonpath_gram.y"
                                    { (yyval.optype) = JPI_DATE; }
#line 2114 "jsonpath_gram.cpp"
    break;

  case 131:
#line 409 "jsonpath_gram.y"
                                      { (yyval.optype) = JPI_NUMBER; }
#line 2120 "jsonpath_gram.cpp"
    break;

  case 132:
#line 410 "jsonpath_gram.y"
                                      { (yyval.optype) = JPI_STRING_FUNC; }
#line 2126 "jsonpath_gram.cpp"
    break;

  case 133:
#line 411 "jsonpath_gram.y"
                                      { (yyval.optype) = JPI_DOUBLE; }
#line 2132 "jsonpath_gram.cpp"
    break;

  case 134:
#line 412 "jsonpath_gram.y"
                                       { (yyval.optype) = JPI_BOOLEAN; }
#line 2138 "jsonpath_gram.cpp"
    break;

  case 135:
#line 413 "jsonpath_gram.y"
                                      { (yyval.optype) = JPI_BIGINT; }
#line 2144 "jsonpath_gram.cpp"
    break;

  case 136:
#line 414 "jsonpath_gram.y"
                                       { (yyval.optype) = JPI_INTEGER; }
#line 2150 "jsonpath_gram.cpp"
    break;


#line 2154 "jsonpath_gram.cpp"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (result, escontext, YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, result, escontext);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp, result, escontext);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;


#if !defined yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (result, escontext, YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif


/*-----------------------------------------------------.
| yyreturn -- parsing is finished, return the result.  |
`-----------------------------------------------------*/
yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, result, escontext);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp, result, escontext);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 416 "jsonpath_gram.y"


static void jsonpath_yyerror(const char *msg)
{
    ereport(ERROR,
            (errcode(ERRCODE_SYNTAX_ERROR),
            errmsg("%s in json path expression", _(msg))));
}

/*
 * The helper functions below allocate and fill JsonPathParseItem's of various
 * types.
 */

static JsonPathParseItem * makeItemType(JsonPathItemType type)
{
    JsonPathParseItem *v = (JsonPathParseItem*)palloc(sizeof(*v));

    CHECK_FOR_INTERRUPTS();

    v->type = type;
    v->next = NULL;

    return v;
}

static JsonPathParseItem * makeItemString(JsonPathString *s)
{
    JsonPathParseItem *v;

    if (s == NULL) {
        v = makeItemType(JPI_NULL);
    } else {
        v = makeItemType(JPI_STRING);
        v->value.string.val = s->val;
        v->value.string.len = s->len;
    }

    return v;
}

static JsonPathParseItem * makeItemKey(JsonPathString *s)
{
    JsonPathParseItem *v;

    v = makeItemString(s);
    v->type = JPI_KEY;

    return v;
}

static JsonPathParseItem * makeItemNumeric(JsonPathString *s)
{
    JsonPathParseItem *v;

    v = makeItemType(JPI_NUMERIC);
    v->value.numeric =
        DatumGetNumeric(DirectFunctionCall3(numeric_in,
                                            CStringGetDatum(s->val),
                                            ObjectIdGetDatum(InvalidOid),
                                            Int32GetDatum(-1)));

    return v;
}

static JsonPathParseItem * makeItemBool(bool val)
{
    JsonPathParseItem *v = makeItemType(JPI_BOOL);

    v->value.boolean = val;

    return v;
}

static JsonPathParseItem * makeItemBinary
    (JsonPathItemType type, JsonPathParseItem *la, JsonPathParseItem *ra)
{
    JsonPathParseItem *v = makeItemType(type);

    v->value.args.left = la;
    v->value.args.right = ra;

    return v;
}

static void checkSingleAFormatIndex(JsonPathParseItem* item)
{
    if (((item->type == JPI_NUMERIC) || (item->type == JPI_VARIABLE) ||
        (item->type == JPI_LAST))) {
        return;
    }
    if ((item->type == JPI_ADD) || (item->type == JPI_SUB)) {
        JsonPathParseItem* la = item->value.args.left;
        JsonPathParseItem* ra = item->value.args.right;
        if (la->type == JPI_LAST &&
            (ra->type == JPI_NUMERIC || ra->type == JPI_VARIABLE)) {
            return;
        }
    }
    ereport(ERROR,
            (errcode(ERRCODE_SYNTAX_ERROR),
                errmsg("invalid input syntax for type %s", "jsonpath"),
                errdetail("A_FORMAT jsonpath array index can only be a non-negative integer, "
                "or an add/substraction expression of \"LAST\", with \"LAST\" "
                "as the left operand, and a non-negative integer as the right operand.")));
}

static void checkAFormatIndex(JsonPathParseItem* item)
{
    JsonPathParseItem* la = item->value.args.left;
    JsonPathParseItem* ra = item->value.args.right;

    if (!DB_IS_CMPT(A_FORMAT)) {
        return;
    }

    if (la != NULL) {
        checkSingleAFormatIndex(la);
    }
    if (ra != NULL) {
        checkSingleAFormatIndex(ra);
    }
}

static JsonPathParseItem * makeItemList(List *list)
{
    JsonPathParseItem *head,
               *end;
    ListCell   *cell;

    head = end = (JsonPathParseItem *) linitial(list);

    if (list_length(list) == 1) {
        return head;
    }

    /* append items to the end of already existing list */
    while (end->next) {
        end = end->next;
    }

    for_each_cell(cell, list_nth_cell(list, 1)) {
        JsonPathParseItem *c = (JsonPathParseItem *) lfirst(cell);

        end->next = c;
        end = c;
    }

    return head;
}

static JsonPathParseItem * makeIndexArray(List *list)
{
    JsonPathParseItem *v = makeItemType(JPI_INDEX_ARRAY);
    ListCell   *cell;
    int            i = 0;

    Assert(list != NIL);
    v->value.array.nelems = list_length(list);

    v->value.array.elems = (ArrayIndexRange*)palloc(sizeof(v->value.array.elems[0]) *
                                  v->value.array.nelems);

    foreach(cell, list) {
        JsonPathParseItem *jpi = (JsonPathParseItem*)lfirst(cell);

        Assert(jpi->type == JPI_SUBSCRIPT);

        v->value.array.elems[i].from = jpi->value.args.left;
        v->value.array.elems[i++].to = jpi->value.args.right;
    }

    return v;
}

static JsonPathParseItem * makeItemVariable(JsonPathString *s)
{
    JsonPathParseItem *v;

    v = makeItemType(JPI_VARIABLE);
    v->value.string.val = s->val;
    v->value.string.len = s->len;

    return v;
}

static JsonPathParseItem * makeAny(int first, int last)
{
    JsonPathParseItem *v = makeItemType(JPI_ANY);

    v->value.anybounds.first = (first >= 0) ? first : PG_UINT32_MAX;
    v->value.anybounds.last = (last >= 0) ? last : PG_UINT32_MAX;

    return v;
}

static JsonPathParseItem * makeItemUnary(
    JsonPathItemType type, JsonPathParseItem *a)
{
    JsonPathParseItem *v;

    if (type == JPI_PLUS && a->type == JPI_NUMERIC && !a->next) {
        return a;
    }

    if (type == JPI_MINUS && a->type == JPI_NUMERIC && !a->next) {
        v = makeItemType(JPI_NUMERIC);
        v->value.numeric =
            DatumGetNumeric(DirectFunctionCall1(numeric_uminus,
                                                NumericGetDatum(a->value.numeric)));
        return v;
    }

    v = makeItemType(type);

    v->value.arg = a;

    return v;
}

static bool makeItemLikeRegex(JsonPathParseItem *expr, JsonPathString *pattern,
    JsonPathString *flags, JsonPathParseItem **result)
{
    JsonPathParseItem *v = makeItemType(JPI_LIKE_REGEX);
    int            i;
    int            cflags;

    v->value.like_regex.expr = expr;
    v->value.like_regex.pattern = pattern->val;
    v->value.like_regex.patternlen = pattern->len;
    /* Parse the flags string, convert to bitmask.  Duplicate flags are OK. */
    v->value.like_regex.flags = 0;
    for (i = 0; flags && i < flags->len; i++) {
        switch (flags->val[i]) {
            case 'i':
                v->value.like_regex.flags |= JSP_REGEX_ICASE;
                break;
            case 's':
                v->value.like_regex.flags |= JSP_REGEX_DOTALL;
                break;
            case 'm':
                v->value.like_regex.flags |= JSP_REGEX_MLINE;
                break;
            case 'x':
                v->value.like_regex.flags |= JSP_REGEX_WSPACE;
                break;
            case 'q':
                v->value.like_regex.flags |= JSP_REGEX_QUOTE;
                break;
            default:
                ereport(ERROR,
                        (errcode(ERRCODE_SYNTAX_ERROR),
                         errmsg("invalid input syntax for type %s", "jsonpath"),
                         errdetail("Unrecognized flag character \"%.*s\" in LIKE_REGEX predicate.",
                                   pg_mblen(flags->val + i), flags->val + i)));
                break;
        }
    }

    /* Convert flags to what pg_regcomp needs */
    if (!JspConvertRegexFlags(v->value.like_regex.flags, &cflags)) {
        return false;
    }

    /* check regex validity */
    {
        regex_t        re_tmp;
        pg_wchar   *wpattern;
        int            wpattern_len;
        int            re_result;

        wpattern = (pg_wchar *) palloc((pattern->len + 1) * sizeof(pg_wchar));
        wpattern_len = pg_mb2wchar_with_len(pattern->val,
                                            wpattern,
                                            pattern->len);
        if ((re_result = pg_regcomp(&re_tmp, wpattern, wpattern_len, cflags,
                                    DEFAULT_COLLATION_OID)) != REG_OKAY) {
            char        errMsg[100];

            pg_regerror(re_result, &re_tmp, errMsg, sizeof(errMsg));
            ereport(ERROR,
                    (errcode(ERRCODE_INVALID_REGULAR_EXPRESSION),
                     errmsg("invalid regular expression: %s", errMsg)));
        }

        pg_regfree(&re_tmp);
    }

    *result = v;

    return true;
}

/*
 * Convert from XQuery regex flags to those recognized by our regex library.
 */
bool JspConvertRegexFlags(uint32 xflags, int *result)
{
    /* By default, XQuery is very nearly the same as Spencer's AREs */
    int            cflags = REG_ADVANCED;

    /* Ignore-case means the same thing, too, modulo locale issues */
    if (xflags & JSP_REGEX_ICASE) {
        cflags |= REG_ICASE;
    }

    /* Per XQuery spec, if 'q' is specified then 'm', 's', 'x' are ignored */
    if (xflags & JSP_REGEX_QUOTE) {
        cflags &= ~REG_ADVANCED;
        cflags |= REG_QUOTE;
    } else {
        /* Note that dotall mode is the default in POSIX */
        if (!(xflags & JSP_REGEX_DOTALL)) {
            cflags |= REG_NLSTOP;
        }
        if (xflags & JSP_REGEX_MLINE) {
            cflags |= REG_NLANCH;
        }

        /*
         * XQuery's 'x' mode is related to Spencer's expanded mode, but it's
         * not really enough alike to justify treating JSP_REGEX_WSPACE as
         * REG_EXPANDED.  For now we treat 'x' as unimplemented; perhaps in
         * future we'll modify the regex library to have an option for
         * XQuery-style ignore-whitespace mode.
         */
        if (xflags & JSP_REGEX_WSPACE) {
            ereport(ERROR,
                    (errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
                     errmsg("XQuery \"x\" flag (expanded regular expressions) is not implemented")));
        }
    }

    *result = cflags;

    return true;
}

#undef yyerror
#undef yylval
#undef yylloc
#undef yylex

#include "jsonpath_scan.inc"
