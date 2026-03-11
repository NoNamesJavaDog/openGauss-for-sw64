/*--------------------------------------------------------------------
 * Symbols referenced in this file:
 * - yyparse
 * - yyerror
 * - yypact
 * - yytranslate
 * - yycheck
 * - yytable
 * - yydefact
 * - yyr2
 * - makeStringValue
 * - makeBoolValue
 * - doNegateFloat
 * - integerToString
 * - convert_to_numeric
 * - makeBitStringValue
 * - makeNullValue
 * - yyr1
 * - yypgoto
 * - yydefgoto
 * - yydestruct
 * - yystos
 *--------------------------------------------------------------------
 */

/* A Bison parser, made by GNU Bison 3.8.  */

/* Bison implementation for Yacc-like parsers in C

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

/* Identify Bison output, and Bison version.  */
#define YYBISON 30800

/* Bison version string.  */
#define YYBISON_VERSION "3.8"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 1

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "hint_gram.y"

#include "postgres.h"
#include "knl/knl_variable.h"
#include "nodes/pg_list.h"
#include "nodes/nodes.h"
#include "plugin_parser/parse_hint.h"
#include "plugin_parser/parser.h"

#include "plugin_parser/gramparse.h"

#pragma GCC diagnostic ignored "-Wsign-compare"
#pragma GCC diagnostic ignored "-Wunused-variable"

extern void yyerror(yyscan_t yyscanner, const char *msg);
extern void hint_scanner_yyerror(const char *msg, yyscan_t yyscanner);

static Value *makeStringValue(char *str);

static Value *makeBitStringValue(char *str);

static Value *makeNullValue();

static Value *makeBoolValue(bool state);

static void doNegateFloat(Value *v);
static Value* integerToString(Value *v);

#define YYMALLOC palloc
#define YYFREE   pfree


static double convert_to_numeric(Node *value);


#line 106 "hint_gram.cpp"

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

#include "hint_gram.hpp"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_IDENT = 3,                      /* IDENT  */
  YYSYMBOL_FCONST = 4,                     /* FCONST  */
  YYSYMBOL_SCONST = 5,                     /* SCONST  */
  YYSYMBOL_BCONST = 6,                     /* BCONST  */
  YYSYMBOL_XCONST = 7,                     /* XCONST  */
  YYSYMBOL_ICONST = 8,                     /* ICONST  */
  YYSYMBOL_NestLoop_P = 9,                 /* NestLoop_P  */
  YYSYMBOL_MergeJoin_P = 10,               /* MergeJoin_P  */
  YYSYMBOL_HashJoin_P = 11,                /* HashJoin_P  */
  YYSYMBOL_No_P = 12,                      /* No_P  */
  YYSYMBOL_Leading_P = 13,                 /* Leading_P  */
  YYSYMBOL_Rows_P = 14,                    /* Rows_P  */
  YYSYMBOL_Broadcast_P = 15,               /* Broadcast_P  */
  YYSYMBOL_Redistribute_P = 16,            /* Redistribute_P  */
  YYSYMBOL_BlockName_P = 17,               /* BlockName_P  */
  YYSYMBOL_TableScan_P = 18,               /* TableScan_P  */
  YYSYMBOL_IndexScan_P = 19,               /* IndexScan_P  */
  YYSYMBOL_IndexOnlyScan_P = 20,           /* IndexOnlyScan_P  */
  YYSYMBOL_Skew_P = 21,                    /* Skew_P  */
  YYSYMBOL_HINT_MULTI_NODE_P = 22,         /* HINT_MULTI_NODE_P  */
  YYSYMBOL_NULL_P = 23,                    /* NULL_P  */
  YYSYMBOL_TRUE_P = 24,                    /* TRUE_P  */
  YYSYMBOL_FALSE_P = 25,                   /* FALSE_P  */
  YYSYMBOL_Predpush_P = 26,                /* Predpush_P  */
  YYSYMBOL_PredpushSameLevel_P = 27,       /* PredpushSameLevel_P  */
  YYSYMBOL_Rewrite_P = 28,                 /* Rewrite_P  */
  YYSYMBOL_Gather_P = 29,                  /* Gather_P  */
  YYSYMBOL_Set_P = 30,                     /* Set_P  */
  YYSYMBOL_Set_Var_P = 31,                 /* Set_Var_P  */
  YYSYMBOL_USE_CPLAN_P = 32,               /* USE_CPLAN_P  */
  YYSYMBOL_USE_GPLAN_P = 33,               /* USE_GPLAN_P  */
  YYSYMBOL_ON_P = 34,                      /* ON_P  */
  YYSYMBOL_OFF_P = 35,                     /* OFF_P  */
  YYSYMBOL_No_expand_P = 36,               /* No_expand_P  */
  YYSYMBOL_SQL_IGNORE_P = 37,              /* SQL_IGNORE_P  */
  YYSYMBOL_NO_GPC_P = 38,                  /* NO_GPC_P  */
  YYSYMBOL_CHOOSE_ADAPTIVE_GPLAN_P = 39,   /* CHOOSE_ADAPTIVE_GPLAN_P  */
  YYSYMBOL_40_ = 40,                       /* '('  */
  YYSYMBOL_41_ = 41,                       /* ')'  */
  YYSYMBOL_42_ = 42,                       /* '+'  */
  YYSYMBOL_43_ = 43,                       /* '-'  */
  YYSYMBOL_44_ = 44,                       /* '='  */
  YYSYMBOL_45_ = 45,                       /* '.'  */
  YYSYMBOL_46_ = 46,                       /* ','  */
  YYSYMBOL_47_ = 47,                       /* '#'  */
  YYSYMBOL_48_ = 48,                       /* '*'  */
  YYSYMBOL_YYACCEPT = 49,                  /* $accept  */
  YYSYMBOL_join_hint_list = 50,            /* join_hint_list  */
  YYSYMBOL_join_hint_item = 51,            /* join_hint_item  */
  YYSYMBOL_gather_hint = 52,               /* gather_hint  */
  YYSYMBOL_no_gpc_hint = 53,               /* no_gpc_hint  */
  YYSYMBOL_no_expand_hint = 54,            /* no_expand_hint  */
  YYSYMBOL_guc_value = 55,                 /* guc_value  */
  YYSYMBOL_set_hint = 56,                  /* set_hint  */
  YYSYMBOL_dolphin_guc = 57,               /* dolphin_guc  */
  YYSYMBOL_plancache_hint = 58,            /* plancache_hint  */
  YYSYMBOL_rewrite_hint = 59,              /* rewrite_hint  */
  YYSYMBOL_pred_push_hint = 60,            /* pred_push_hint  */
  YYSYMBOL_pred_push_same_level_hint = 61, /* pred_push_same_level_hint  */
  YYSYMBOL_join_order_hint = 62,           /* join_order_hint  */
  YYSYMBOL_join_method_hint = 63,          /* join_method_hint  */
  YYSYMBOL_stream_hint = 64,               /* stream_hint  */
  YYSYMBOL_row_hint = 65,                  /* row_hint  */
  YYSYMBOL_scan_hint = 66,                 /* scan_hint  */
  YYSYMBOL_skew_hint = 67,                 /* skew_hint  */
  YYSYMBOL_relation_list_with_p = 68,      /* relation_list_with_p  */
  YYSYMBOL_relation_item = 69,             /* relation_item  */
  YYSYMBOL_relation_list = 70,             /* relation_list  */
  YYSYMBOL_ident_list = 71,                /* ident_list  */
  YYSYMBOL_expr_const = 72,                /* expr_const  */
  YYSYMBOL_skew_relist = 73,               /* skew_relist  */
  YYSYMBOL_column_list_p = 74,             /* column_list_p  */
  YYSYMBOL_column_list = 75,               /* column_list  */
  YYSYMBOL_value_list_p = 76,              /* value_list_p  */
  YYSYMBOL_value_list = 77,                /* value_list  */
  YYSYMBOL_value_list_with_bracket = 78,   /* value_list_with_bracket  */
  YYSYMBOL_value_list_item = 79,           /* value_list_item  */
  YYSYMBOL_value_type = 80                 /* value_type  */
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

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
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
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
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
#define YYFINAL  65
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   182

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  49
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  32
/* YYNRULES -- Number of rules.  */
#define YYNRULES  96
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  184

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   294


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
       2,     2,     2,     2,     2,    47,     2,     2,     2,     2,
      40,    41,    48,    42,    46,    43,    45,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    44,     2,     2,     2,     2,     2,     2,     2,     2,
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
      35,    36,    37,    38,    39
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    72,    72,    77,    81,    85,    89,    95,    99,   107,
     111,   118,   122,   128,   132,   143,   147,   151,   155,   159,
     163,   167,   171,   175,   183,   206,   215,   224,   225,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   244,
     266,   283,   306,   314,   320,   330,   340,   353,   363,   376,
     389,   403,   417,   425,   435,   446,   457,   471,   485,   502,
     514,   526,   538,   553,   562,   572,   584,   600,   618,   622,
     623,   627,   628,   632,   633,   637,   638,   642,   643,   647,
     651,   652,   656,   660,   661,   665,   666,   670,   671,   675,
     676,   677,   678,   679,   680,   681,   682
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
  "\"end of file\"", "error", "\"invalid token\"", "IDENT", "FCONST",
  "SCONST", "BCONST", "XCONST", "ICONST", "NestLoop_P", "MergeJoin_P",
  "HashJoin_P", "No_P", "Leading_P", "Rows_P", "Broadcast_P",
  "Redistribute_P", "BlockName_P", "TableScan_P", "IndexScan_P",
  "IndexOnlyScan_P", "Skew_P", "HINT_MULTI_NODE_P", "NULL_P", "TRUE_P",
  "FALSE_P", "Predpush_P", "PredpushSameLevel_P", "Rewrite_P", "Gather_P",
  "Set_P", "Set_Var_P", "USE_CPLAN_P", "USE_GPLAN_P", "ON_P", "OFF_P",
  "No_expand_P", "SQL_IGNORE_P", "NO_GPC_P", "CHOOSE_ADAPTIVE_GPLAN_P",
  "'('", "')'", "'+'", "'-'", "'='", "'.'", "','", "'#'", "'*'", "$accept",
  "join_hint_list", "join_hint_item", "gather_hint", "no_gpc_hint",
  "no_expand_hint", "guc_value", "set_hint", "dolphin_guc",
  "plancache_hint", "rewrite_hint", "pred_push_hint",
  "pred_push_same_level_hint", "join_order_hint", "join_method_hint",
  "stream_hint", "row_hint", "scan_hint", "skew_hint",
  "relation_list_with_p", "relation_item", "relation_list", "ident_list",
  "expr_const", "skew_relist", "column_list_p", "column_list",
  "value_list_p", "value_list", "value_list_with_bracket",
  "value_list_item", "value_type", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-138)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      93,   -16,    20,    36,   130,    38,    50,    51,    52,    53,
      54,    55,    56,    57,  -138,    58,    77,    78,    87,    88,
    -138,  -138,  -138,  -138,  -138,  -138,     2,    93,  -138,  -138,
    -138,  -138,  -138,  -138,  -138,  -138,  -138,  -138,  -138,  -138,
    -138,    68,    68,    68,  -138,    98,  -138,  -138,  -138,  -138,
      35,  -138,    68,    68,    68,   118,   139,    68,    68,    39,
      68,    68,   140,    40,   141,  -138,  -138,  -138,     0,    15,
      18,    68,  -138,    35,   106,    35,    14,    -3,    22,    23,
     110,   114,    24,    25,  -138,    68,   117,     7,     3,   119,
      27,    27,   116,   115,  -138,  -138,  -138,  -138,    26,  -138,
    -138,  -138,  -138,  -138,     5,     5,     5,     5,  -138,  -138,
    -138,  -138,  -138,  -138,    31,   159,   -29,  -138,   160,   161,
    -138,  -138,  -138,  -138,  -138,  -138,  -138,  -138,  -138,    69,
      81,   124,   125,   164,    27,  -138,  -138,  -138,   127,   128,
     131,   132,  -138,  -138,    33,    76,  -138,   133,   134,   135,
    -138,  -138,  -138,  -138,  -138,  -138,  -138,   136,  -138,  -138,
    -138,  -138,  -138,  -138,  -138,  -138,  -138,  -138,  -138,  -138,
    -138,  -138,  -138,   137,   142,   129,  -138,  -138,  -138,  -138,
    -138,  -138,  -138,  -138
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       3,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    14,     0,     0,     0,     0,     0,
      44,    45,    26,    23,    25,    46,     0,     3,    18,    22,
      21,    19,    20,    15,    16,     4,     5,     7,     9,    11,
      13,     0,     0,     0,    50,     0,    17,     6,     8,    12,
       0,    53,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     1,     2,    73,     0,     0,
       0,     0,    69,     0,    70,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    77,     0,     0,     0,     0,     0,
       0,     0,    43,     0,    74,    54,    55,    56,     0,    70,
      52,    71,    68,    72,     0,     0,     0,     0,    57,    58,
      10,    63,    64,    65,     0,     0,     0,    48,     0,     0,
      24,    27,    36,    28,    33,    29,    30,    31,    32,     0,
       0,     0,     0,     0,     0,    47,    76,    75,     0,     0,
       0,     0,    78,    80,     0,     0,    67,     0,     0,     0,
      37,    34,    38,    35,    39,    40,    42,     0,    60,    61,
      59,    62,    81,    79,    90,    91,    92,    93,    89,    94,
      95,    96,    85,     0,    84,    83,    87,    66,    49,    51,
      41,    82,    86,    88
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -138,   143,  -138,  -138,  -138,  -138,   -84,  -138,  -138,  -138,
    -138,  -138,  -138,  -138,   167,   175,  -138,   176,  -138,    -4,
     -18,  -138,   -38,   -19,  -138,  -138,  -138,  -137,  -138,  -138,
    -138,     6
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,    26,    27,    28,    29,    30,   131,    31,    93,    32,
      46,    33,    34,    35,    36,    37,    38,    39,    40,    99,
      75,    76,    68,   138,    86,   116,   144,   147,   173,   174,
     175,   176
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      94,    51,    65,    94,    69,    70,    94,   132,   172,   136,
      94,   145,   146,   137,    77,    78,    79,    72,    94,    82,
      83,    94,    87,    88,    41,    94,    94,    94,    94,    94,
     121,   122,   123,    98,    94,   124,   162,   182,    72,   104,
     105,    95,    84,    90,   106,   107,    74,   114,   117,   119,
     157,   125,   126,   118,    73,   102,    96,   101,   103,    97,
      42,   127,   128,   108,   109,   112,   113,   135,    91,   129,
     130,    67,   142,   150,   163,    73,    43,   151,    50,    85,
     164,   165,   166,   167,   168,   152,   139,   140,   141,   153,
      52,    53,    54,    55,    56,    57,    58,    59,    60,   169,
     170,   171,     1,     2,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    14,   145,    61,    62,    15,
      16,    80,    17,    18,    19,    20,    21,    63,    64,    22,
      23,    24,    25,   164,   165,   166,   167,   168,    71,     1,
       2,     3,    81,    89,    92,     7,     8,   100,    10,    11,
      12,   110,   169,   170,   171,   111,    44,   115,    45,   134,
     120,   133,   143,   148,   149,   154,   155,   156,   158,   159,
      66,    47,   160,   161,   177,   178,   179,   180,   181,    48,
      49,   183,   145
};

static const yytype_uint8 yycheck[] =
{
       3,     5,     0,     3,    42,    43,     3,    91,   145,     4,
       3,    40,    41,     8,    52,    53,    54,     3,     3,    57,
      58,     3,    60,    61,    40,     3,     3,     3,     3,     3,
       3,     4,     5,    71,     3,     8,     3,   174,     3,    42,
      43,    41,     3,     3,    47,    48,    50,    85,    41,    46,
     134,    24,    25,    46,    40,    41,    41,    75,    76,    41,
      40,    34,    35,    41,    41,    41,    41,    41,    28,    42,
      43,     3,    41,     4,    41,    40,    40,     8,    40,    40,
       4,     5,     6,     7,     8,     4,   105,   106,   107,     8,
      40,    40,    40,    40,    40,    40,    40,    40,    40,    23,
      24,    25,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    40,    40,    40,    26,
      27,     3,    29,    30,    31,    32,    33,    40,    40,    36,
      37,    38,    39,     4,     5,     6,     7,     8,    40,     9,
      10,    11,     3,     3,     3,    15,    16,    41,    18,    19,
      20,    41,    23,    24,    25,    41,    26,    40,    28,    44,
      41,    45,     3,     3,     3,    41,    41,     3,    41,    41,
      27,     4,    41,    41,    41,    41,    41,    41,    41,     4,
       4,   175,    40
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    26,    27,    29,    30,    31,
      32,    33,    36,    37,    38,    39,    50,    51,    52,    53,
      54,    56,    58,    60,    61,    62,    63,    64,    65,    66,
      67,    40,    40,    40,    26,    28,    59,    63,    64,    66,
      40,    68,    40,    40,    40,    40,    40,    40,    40,    40,
      40,    40,    40,    40,    40,     0,    50,     3,    71,    71,
      71,    40,     3,    40,    68,    69,    70,    71,    71,    71,
       3,     3,    71,    71,     3,    40,    73,    71,    71,     3,
       3,    28,     3,    57,     3,    41,    41,    41,    71,    68,
      41,    69,    41,    69,    42,    43,    47,    48,    41,    41,
      41,    41,    41,    41,    71,    40,    74,    41,    46,    46,
      41,     3,     4,     5,     8,    24,    25,    34,    35,    42,
      43,    55,    55,    45,    44,    41,     4,     8,    72,    72,
      72,    72,    41,     3,    75,    40,    41,    76,     3,     3,
       4,     8,     4,     8,    41,    41,     3,    55,    41,    41,
      41,    41,     3,    41,     4,     5,     6,     7,     8,    23,
      24,    25,    76,    77,    78,    79,    80,    41,    41,    41,
      41,    41,    76,    80
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    49,    50,    50,    51,    51,    51,    51,    51,    51,
      51,    51,    51,    51,    51,    51,    51,    51,    51,    51,
      51,    51,    51,    51,    52,    53,    54,    55,    55,    55,
      55,    55,    55,    55,    55,    55,    55,    55,    55,    56,
      56,    56,    57,    57,    58,    58,    58,    59,    60,    60,
      60,    61,    62,    62,    63,    63,    63,    64,    64,    65,
      65,    65,    65,    66,    66,    66,    67,    67,    68,    69,
      69,    70,    70,    71,    71,    72,    72,    73,    73,    74,
      75,    75,    76,    77,    77,    78,    78,    79,    79,    80,
      80,    80,    80,    80,    80,    80,    80
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     2,     0,     1,     1,     2,     1,     2,     1,
       4,     1,     2,     1,     1,     1,     1,     2,     1,     1,
       1,     1,     1,     1,     4,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     2,     2,     1,     2,     2,     5,
       5,     6,     3,     1,     1,     1,     1,     4,     4,     6,
       2,     6,     4,     2,     4,     4,     4,     4,     4,     6,
       6,     6,     6,     4,     4,     4,     6,     5,     3,     1,
       1,     2,     2,     1,     2,     1,     1,     1,     3,     3,
       1,     2,     3,     1,     1,     1,     2,     1,     2,     1,
       1,     1,     1,     1,     1,     1,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


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
        yyerror (yyscanner, YY_("syntax error: cannot back up")); \
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




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value, yyscanner); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, yyscan_t yyscanner)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  YY_USE (yyscanner);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, yyscan_t yyscanner)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep, yyscanner);
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
                 int yyrule, yyscan_t yyscanner)
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
                       &yyvsp[(yyi + 1) - (yynrhs)], yyscanner);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule, yyscanner); \
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
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, yyscan_t yyscanner)
{
  YY_USE (yyvaluep);
  YY_USE (yyscanner);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}






/*----------.
| yyparse.  |
`----------*/

int
yyparse (yyscan_t yyscanner)
{
/* Lookahead token kind.  */
int yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
YY_INITIAL_VALUE (static YYSTYPE yyval_default;)
YYSTYPE yylval YY_INITIAL_VALUE (= yyval_default);

    /* Number of syntax errors so far.  */
    int yynerrs = 0;

    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

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
    YYNOMEM;
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
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
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
      yychar = yylex (&yylval, yyscanner);
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
  case 2: /* join_hint_list: join_hint_item join_hint_list  */
#line 73 "hint_gram.y"
        {
		(yyval.list) = lcons((yyvsp[-1].node), (yyvsp[0].list));
		u_sess->parser_cxt.hint_list = (yyval.list);
	}
#line 1313 "hint_gram.cpp"
    break;

  case 3: /* join_hint_list: %empty  */
#line 77 "hint_gram.y"
                                { (yyval.list) = NIL; }
#line 1319 "hint_gram.cpp"
    break;

  case 4: /* join_hint_item: join_order_hint  */
#line 82 "hint_gram.y"
        {
		(yyval.node) = (yyvsp[0].node);
	}
#line 1327 "hint_gram.cpp"
    break;

  case 5: /* join_hint_item: join_method_hint  */
#line 86 "hint_gram.y"
        {
		(yyval.node) = (yyvsp[0].node);
	}
#line 1335 "hint_gram.cpp"
    break;

  case 6: /* join_hint_item: No_P join_method_hint  */
#line 90 "hint_gram.y"
        {
		JoinMethodHint *joinHint = (JoinMethodHint *) (yyvsp[0].node);
		joinHint->negative = true;
		(yyval.node) = (Node *) joinHint;
	}
#line 1345 "hint_gram.cpp"
    break;

  case 7: /* join_hint_item: stream_hint  */
#line 96 "hint_gram.y"
        {
		(yyval.node) = (yyvsp[0].node);
	}
#line 1353 "hint_gram.cpp"
    break;

  case 8: /* join_hint_item: No_P stream_hint  */
#line 100 "hint_gram.y"
        {
		StreamHint	*streamHint = (StreamHint *) (yyvsp[0].node);
		if (streamHint != NULL) {
			streamHint->negative = true;
		}
		(yyval.node) = (Node *) streamHint;
	}
#line 1365 "hint_gram.cpp"
    break;

  case 9: /* join_hint_item: row_hint  */
#line 108 "hint_gram.y"
        {
		(yyval.node) = (yyvsp[0].node);
	}
#line 1373 "hint_gram.cpp"
    break;

  case 10: /* join_hint_item: BlockName_P '(' IDENT ')'  */
#line 112 "hint_gram.y"
        {
		BlockNameHint *blockHint = makeNode(BlockNameHint);
		blockHint->base.relnames = list_make1(makeString((yyvsp[-1].str)));
		blockHint->base.hint_keyword = HINT_KEYWORD_BLOCKNAME;
		(yyval.node) = (Node *) blockHint;
	}
#line 1384 "hint_gram.cpp"
    break;

  case 11: /* join_hint_item: scan_hint  */
#line 119 "hint_gram.y"
        {
		(yyval.node) = (yyvsp[0].node);
	}
#line 1392 "hint_gram.cpp"
    break;

  case 12: /* join_hint_item: No_P scan_hint  */
#line 123 "hint_gram.y"
        {
		ScanMethodHint	*scanHint = (ScanMethodHint *) (yyvsp[0].node);
		scanHint->negative = true;
		(yyval.node) = (Node *) scanHint;
	}
#line 1402 "hint_gram.cpp"
    break;

  case 13: /* join_hint_item: skew_hint  */
#line 129 "hint_gram.y"
        {
		(yyval.node) = (yyvsp[0].node);
	}
#line 1410 "hint_gram.cpp"
    break;

  case 14: /* join_hint_item: HINT_MULTI_NODE_P  */
#line 133 "hint_gram.y"
        {
#ifndef ENABLE_MULTIPLE_NODES
		hint_scanner_yyerror("unsupport distributed hint", yyscanner);
		(yyval.node) = NULL;
#else /* ENABLE_MULTIPLE_NODES */
		MultiNodeHint *multi_node_hint = (MultiNodeHint *)makeNode(MultiNodeHint);
		multi_node_hint->multi_node_hint = true;
		(yyval.node) = (Node *) multi_node_hint;
#endif /* ENABLE_MULTIPLE_NODES */
	}
#line 1425 "hint_gram.cpp"
    break;

  case 15: /* join_hint_item: pred_push_hint  */
#line 144 "hint_gram.y"
    {
        (yyval.node) = (yyvsp[0].node);
    }
#line 1433 "hint_gram.cpp"
    break;

  case 16: /* join_hint_item: pred_push_same_level_hint  */
#line 148 "hint_gram.y"
    {
        (yyval.node) = (yyvsp[0].node);
    }
#line 1441 "hint_gram.cpp"
    break;

  case 17: /* join_hint_item: No_P rewrite_hint  */
#line 152 "hint_gram.y"
        {
		(yyval.node) = (yyvsp[0].node);
	}
#line 1449 "hint_gram.cpp"
    break;

  case 18: /* join_hint_item: gather_hint  */
#line 156 "hint_gram.y"
        {
		(yyval.node) = (yyvsp[0].node);
	}
#line 1457 "hint_gram.cpp"
    break;

  case 19: /* join_hint_item: set_hint  */
#line 160 "hint_gram.y"
        {
		(yyval.node) = (yyvsp[0].node);
	}
#line 1465 "hint_gram.cpp"
    break;

  case 20: /* join_hint_item: plancache_hint  */
#line 164 "hint_gram.y"
        {
		(yyval.node) = (yyvsp[0].node);
	}
#line 1473 "hint_gram.cpp"
    break;

  case 21: /* join_hint_item: no_expand_hint  */
#line 168 "hint_gram.y"
        {
		(yyval.node) = (yyvsp[0].node);
	}
#line 1481 "hint_gram.cpp"
    break;

  case 22: /* join_hint_item: no_gpc_hint  */
#line 172 "hint_gram.y"
        {
		(yyval.node) = (yyvsp[0].node);
	}
#line 1489 "hint_gram.cpp"
    break;

  case 23: /* join_hint_item: SQL_IGNORE_P  */
#line 176 "hint_gram.y"
        {
		SqlIgnoreHint *sql_ignore_hint = (SqlIgnoreHint *)makeNode(SqlIgnoreHint);
		sql_ignore_hint->sql_ignore_hint = true;
		(yyval.node) = (Node *) sql_ignore_hint;
	}
#line 1499 "hint_gram.cpp"
    break;

  case 24: /* gather_hint: Gather_P '(' IDENT ')'  */
#line 184 "hint_gram.y"
        {
#ifndef ENABLE_MULTIPLE_NODES
		hint_scanner_yyerror("unsupport distributed hint", yyscanner);
		(yyval.node) = NULL;
#else /* ENABLE_MULTIPLE_NODES */
		GatherHint *gatherHint = makeNode(GatherHint);
		gatherHint->base.hint_keyword = HINT_KEYWORD_GATHER;
		gatherHint->base.state = HINT_STATE_NOTUSED;
		if (pg_strcasecmp((yyvsp[-1].str), "REL") == 0) {
			gatherHint->source = HINT_GATHER_REL;
		} else if (pg_strcasecmp((yyvsp[-1].str), "JOIN") == 0) {
			gatherHint->source = HINT_GATHER_JOIN;
		} else if (pg_strcasecmp((yyvsp[-1].str), "ALL") == 0) {
			gatherHint->source = HINT_GATHER_ALL;
		} else {
			gatherHint->source = HINT_GATHER_UNKNOWN;
		}
		(yyval.node) = (Node *) gatherHint;
#endif /* ENABLE_MULTIPLE_NODES */
	}
#line 1524 "hint_gram.cpp"
    break;

  case 25: /* no_gpc_hint: NO_GPC_P  */
#line 207 "hint_gram.y"
        {
		NoGPCHint *noGPCHint = makeNode(NoGPCHint);
		noGPCHint->base.hint_keyword = HINT_KEYWORD_NO_GPC;
		noGPCHint->base.state = HINT_STATE_NOTUSED;
		(yyval.node) = (Node *) noGPCHint;
	}
#line 1535 "hint_gram.cpp"
    break;

  case 26: /* no_expand_hint: No_expand_P  */
#line 216 "hint_gram.y"
        {
		NoExpandHint *noExpandHint = makeNode(NoExpandHint);
		noExpandHint->base.hint_keyword = HINT_KEYWORD_NO_EXPAND;
		noExpandHint->base.state = HINT_STATE_NOTUSED;
		(yyval.node) = (Node *) noExpandHint;
	}
#line 1546 "hint_gram.cpp"
    break;

  case 27: /* guc_value: IDENT  */
#line 224 "hint_gram.y"
                                        { (yyval.node) = (Node*)makeStringValue((yyvsp[0].str)); }
#line 1552 "hint_gram.cpp"
    break;

  case 28: /* guc_value: SCONST  */
#line 225 "hint_gram.y"
                                        { (yyval.node) = (Node*)makeStringValue((yyvsp[0].str)); }
#line 1558 "hint_gram.cpp"
    break;

  case 29: /* guc_value: TRUE_P  */
#line 226 "hint_gram.y"
                                        { (yyval.node) = (Node*)makeBoolValue(TRUE); }
#line 1564 "hint_gram.cpp"
    break;

  case 30: /* guc_value: FALSE_P  */
#line 227 "hint_gram.y"
                                        { (yyval.node) = (Node*)makeBoolValue(FALSE); }
#line 1570 "hint_gram.cpp"
    break;

  case 31: /* guc_value: ON_P  */
#line 228 "hint_gram.y"
                                        { (yyval.node) = (Node*)makeBoolValue(TRUE); }
#line 1576 "hint_gram.cpp"
    break;

  case 32: /* guc_value: OFF_P  */
#line 229 "hint_gram.y"
                                        { (yyval.node) = (Node*)makeBoolValue(FALSE); }
#line 1582 "hint_gram.cpp"
    break;

  case 33: /* guc_value: ICONST  */
#line 230 "hint_gram.y"
                                        { (yyval.node) = (Node*)makeInteger((yyvsp[0].ival)); }
#line 1588 "hint_gram.cpp"
    break;

  case 34: /* guc_value: '+' ICONST  */
#line 231 "hint_gram.y"
                                { (yyval.node) = (Node*)makeInteger((yyvsp[0].ival)); }
#line 1594 "hint_gram.cpp"
    break;

  case 35: /* guc_value: '-' ICONST  */
#line 232 "hint_gram.y"
                                { (yyval.node) = (Node*)makeInteger(-(yyvsp[0].ival)); }
#line 1600 "hint_gram.cpp"
    break;

  case 36: /* guc_value: FCONST  */
#line 233 "hint_gram.y"
                                        { (yyval.node) = (Node*)makeFloat((yyvsp[0].str)); }
#line 1606 "hint_gram.cpp"
    break;

  case 37: /* guc_value: '+' FCONST  */
#line 234 "hint_gram.y"
                                { (yyval.node) = (Node*)makeFloat((yyvsp[0].str)); }
#line 1612 "hint_gram.cpp"
    break;

  case 38: /* guc_value: '-' FCONST  */
#line 236 "hint_gram.y"
        {
		Value *fvalue = makeFloat((yyvsp[0].str));
		doNegateFloat(fvalue);
		(yyval.node) = (Node*)fvalue;
	}
#line 1622 "hint_gram.cpp"
    break;

  case 39: /* set_hint: Set_P '(' IDENT guc_value ')'  */
#line 245 "hint_gram.y"
        {
		char* name = (yyvsp[-2].str);
		if (!check_set_hint_in_white_list(name)) {
			ereport(WARNING, (errmsg("SetHint is invalid. Parameter [%s] is not in whitelist.", name)));
			(yyval.node) = NULL;
		} else {
			Value* guc_val = NULL;
			if (IsA((yyvsp[-1].node), Integer)) {
				guc_val = integerToString((Value*)(yyvsp[-1].node));
			} else {
				guc_val = (Value*)(yyvsp[-1].node);
			}
			SetHint *setHint = makeNode(SetHint);
			setHint->base.hint_keyword = HINT_KEYWORD_SET;
			setHint->base.state = HINT_STATE_NOTUSED;
			setHint->name = name;
			setHint->value = strVal(guc_val);
			(yyval.node) = (Node *) setHint;
		}
	}
#line 1647 "hint_gram.cpp"
    break;

  case 40: /* set_hint: Set_P '(' Rewrite_P guc_value ')'  */
#line 267 "hint_gram.y"
        {
		char* name = "rewrite_rule";
		Value* guc_val = NULL;
		if (IsA((yyvsp[-1].node), Integer)) {
			guc_val = integerToString((Value*)(yyvsp[-1].node));
		} else {
			guc_val = (Value*)(yyvsp[-1].node);
		}
		SetHint *setHint = makeNode(SetHint);
		setHint->base.hint_keyword = HINT_KEYWORD_SET;
		setHint->base.state = HINT_STATE_NOTUSED;
		setHint->name = name;
		setHint->value = strVal(guc_val);
		(yyval.node) = (Node *) setHint;
	}
#line 1667 "hint_gram.cpp"
    break;

  case 41: /* set_hint: Set_Var_P '(' dolphin_guc '=' guc_value ')'  */
#line 284 "hint_gram.y"
        {
		char* name = (yyvsp[-3].str);
		if (!check_set_hint_in_white_list(name)) {
			ereport(WARNING, (errmsg("SetHint is invalid. Parameter [%s] is not in whitelist.", name)));
			(yyval.node) = NULL;
		} else {
			Value* guc_val = NULL;
			if (IsA((yyvsp[-1].node), Integer)) {
				guc_val = integerToString((Value*)(yyvsp[-1].node));
			} else {
				guc_val = (Value*)(yyvsp[-1].node);
			}
			SetHint *setHint = makeNode(SetHint);
			setHint->base.hint_keyword = HINT_KEYWORD_SET_VAR;
			setHint->base.state = HINT_STATE_NOTUSED;
			setHint->name = name;
			setHint->value = strVal(guc_val);
			(yyval.node) = (Node *) setHint;
		}
	}
#line 1692 "hint_gram.cpp"
    break;

  case 42: /* dolphin_guc: IDENT '.' IDENT  */
#line 307 "hint_gram.y"
        {
		int len = strlen((yyvsp[-2].str)) + strlen((yyvsp[0].str)) + 2;
		char* str = (char*)palloc0(len);
		errno_t rc = sprintf_s(str, len, "%s.%s", (yyvsp[-2].str), (yyvsp[0].str));
		securec_check_ss(rc, "\0", "\0");
		(yyval.str) = str;
	}
#line 1704 "hint_gram.cpp"
    break;

  case 43: /* dolphin_guc: IDENT  */
#line 315 "hint_gram.y"
        {
		(yyval.str) = (yyvsp[0].str);
	}
#line 1712 "hint_gram.cpp"
    break;

  case 44: /* plancache_hint: USE_CPLAN_P  */
#line 321 "hint_gram.y"
        {
		PlanCacheHint *planCacheHint = makeNode(PlanCacheHint);
		planCacheHint->base.hint_keyword = HINT_KEYWORD_CPLAN;
		planCacheHint->base.state = HINT_STATE_NOTUSED;
		planCacheHint->chooseCustomPlan = true;
        planCacheHint->method = CHOOSE_NONE_GPLAN;

		(yyval.node) = (Node *) planCacheHint;
	}
#line 1726 "hint_gram.cpp"
    break;

  case 45: /* plancache_hint: USE_GPLAN_P  */
#line 331 "hint_gram.y"
        {
		PlanCacheHint *planCacheHint = makeNode(PlanCacheHint);
		planCacheHint->base.hint_keyword = HINT_KEYWORD_GPLAN;
		planCacheHint->base.state = HINT_STATE_NOTUSED;
		planCacheHint->chooseCustomPlan = false;
        planCacheHint->method = CHOOSE_DEFAULT_GPLAN;

		(yyval.node) = (Node *) planCacheHint;
	}
#line 1740 "hint_gram.cpp"
    break;

  case 46: /* plancache_hint: CHOOSE_ADAPTIVE_GPLAN_P  */
#line 341 "hint_gram.y"
    {
        PlanCacheHint *planCacheHint = makeNode(PlanCacheHint);
		planCacheHint->base.hint_keyword = HINT_KEYWORD_CHOOSE_ADAPTIVE_GPLAN;
		planCacheHint->base.state = HINT_STATE_NOTUSED;
		planCacheHint->chooseCustomPlan = false;
        planCacheHint->method = CHOOSE_ADAPTIVE_GPLAN;

		(yyval.node) = (Node *) planCacheHint;
    }
#line 1754 "hint_gram.cpp"
    break;

  case 47: /* rewrite_hint: Rewrite_P '(' ident_list ')'  */
#line 354 "hint_gram.y"
        {
		RewriteHint *rewriteHint = makeNode(RewriteHint);
		rewriteHint->param_names = (yyvsp[-1].list);
		rewriteHint->param_bits = 0;
		rewriteHint->base.hint_keyword = HINT_KEYWORD_REWRITE;
		(yyval.node) = (Node *) rewriteHint;
	}
#line 1766 "hint_gram.cpp"
    break;

  case 48: /* pred_push_hint: Predpush_P '(' ident_list ')'  */
#line 364 "hint_gram.y"
    {
        PredpushHint *predpushHint = makeNode(PredpushHint);
        predpushHint->base.relnames = (yyvsp[-1].list);
        predpushHint->base.hint_keyword = HINT_KEYWORD_PREDPUSH;
        predpushHint->base.state = HINT_STATE_NOTUSED;
        predpushHint->dest_name = NULL;
        predpushHint->dest_id = 0;
        predpushHint->candidates = NULL;
        predpushHint->negative = false;
        (yyval.node) = (Node *) predpushHint;
    }
#line 1782 "hint_gram.cpp"
    break;

  case 49: /* pred_push_hint: Predpush_P '(' ident_list ',' IDENT ')'  */
#line 377 "hint_gram.y"
    {
        PredpushHint *predpushHint = makeNode(PredpushHint);
        predpushHint->base.relnames = (yyvsp[-3].list);
        predpushHint->base.hint_keyword = HINT_KEYWORD_PREDPUSH;
        predpushHint->base.state = HINT_STATE_NOTUSED;
        predpushHint->dest_name = (yyvsp[-1].str);
        predpushHint->dest_id = 0;
        predpushHint->candidates = NULL;
        predpushHint->negative = false;
        (yyval.node) = (Node *) predpushHint;
    }
#line 1798 "hint_gram.cpp"
    break;

  case 50: /* pred_push_hint: No_P Predpush_P  */
#line 390 "hint_gram.y"
    {
        PredpushHint *predpushHint = makeNode(PredpushHint);
        predpushHint->base.relnames = NULL;
        predpushHint->base.hint_keyword = HINT_KEYWORD_PREDPUSH;
        predpushHint->base.state = HINT_STATE_NOTUSED;
        predpushHint->dest_name = NULL;
        predpushHint->dest_id = 0;
        predpushHint->candidates = NULL;
        predpushHint->negative = true;
        (yyval.node) = (Node *) predpushHint;
    }
#line 1814 "hint_gram.cpp"
    break;

  case 51: /* pred_push_same_level_hint: PredpushSameLevel_P '(' ident_list ',' IDENT ')'  */
#line 404 "hint_gram.y"
    {
        PredpushSameLevelHint *predpushSameLevelHint = makeNode(PredpushSameLevelHint);
        predpushSameLevelHint->base.relnames = (yyvsp[-3].list);
        predpushSameLevelHint->base.hint_keyword = HINT_KEYWORD_PREDPUSH_SAME_LEVEL;
        predpushSameLevelHint->base.state = HINT_STATE_NOTUSED;
        predpushSameLevelHint->dest_name = (yyvsp[-1].str);
        predpushSameLevelHint->dest_id = 0;
        predpushSameLevelHint->candidates = NULL;
        predpushSameLevelHint->negative = false;
        (yyval.node) = (Node *) predpushSameLevelHint;
    }
#line 1830 "hint_gram.cpp"
    break;

  case 52: /* join_order_hint: Leading_P '(' relation_list_with_p ')'  */
#line 418 "hint_gram.y"
        {
		LeadingHint *leadingHint = makeNode(LeadingHint);
		leadingHint->base.relnames = (yyvsp[-1].list);
		leadingHint->join_order_hint = true;
		leadingHint->base.hint_keyword = HINT_KEYWORD_LEADING;
		(yyval.node) = (Node *) leadingHint;
	}
#line 1842 "hint_gram.cpp"
    break;

  case 53: /* join_order_hint: Leading_P relation_list_with_p  */
#line 426 "hint_gram.y"
        {
		LeadingHint *leadingHint = makeNode(LeadingHint);
		leadingHint->base.relnames = (yyvsp[0].list);
		leadingHint->base.hint_keyword = HINT_KEYWORD_LEADING;
		(yyval.node) = (Node *) leadingHint;
	}
#line 1853 "hint_gram.cpp"
    break;

  case 54: /* join_method_hint: NestLoop_P '(' ident_list ')'  */
#line 436 "hint_gram.y"
        {
		JoinMethodHint *joinHint = makeNode(JoinMethodHint);
		joinHint->base.relnames = (yyvsp[-1].list);
		joinHint->base.hint_keyword = HINT_KEYWORD_NESTLOOP;
		joinHint->base.state = HINT_STATE_NOTUSED;
		joinHint->joinrelids = NULL;
		joinHint->inner_joinrelids = NULL;

		(yyval.node) = (Node*)joinHint;
	}
#line 1868 "hint_gram.cpp"
    break;

  case 55: /* join_method_hint: MergeJoin_P '(' ident_list ')'  */
#line 447 "hint_gram.y"
        {
		JoinMethodHint *joinHint = makeNode(JoinMethodHint);
		joinHint->base.relnames = (yyvsp[-1].list);
		joinHint->base.hint_keyword = HINT_KEYWORD_MERGEJOIN;
		joinHint->base.state = HINT_STATE_NOTUSED;
		joinHint->joinrelids = NULL;
		joinHint->inner_joinrelids = NULL;

		(yyval.node) = (Node*)joinHint;
	}
#line 1883 "hint_gram.cpp"
    break;

  case 56: /* join_method_hint: HashJoin_P '(' ident_list ')'  */
#line 458 "hint_gram.y"
        {
		JoinMethodHint *joinHint = makeNode(JoinMethodHint);
		joinHint->base.relnames = (yyvsp[-1].list);
		joinHint->base.hint_keyword = HINT_KEYWORD_HASHJOIN;
		joinHint->base.state = HINT_STATE_NOTUSED;
		joinHint->joinrelids = NULL;
		joinHint->inner_joinrelids = NULL;

		(yyval.node) = (Node*)joinHint;
	}
#line 1898 "hint_gram.cpp"
    break;

  case 57: /* stream_hint: Broadcast_P '(' ident_list ')'  */
#line 472 "hint_gram.y"
        {
#ifndef ENABLE_MULTIPLE_NODES
		hint_scanner_yyerror("unsupport distributed hint", yyscanner);
		(yyval.node) = NULL;
#else /* ENABLE_MULTIPLE_NODES */
		StreamHint *streamHint = makeNode(StreamHint);
		streamHint->base.relnames = (yyvsp[-1].list);
		streamHint->base.hint_keyword = HINT_KEYWORD_BROADCAST;
		streamHint->stream_type = STREAM_BROADCAST;

		(yyval.node) = (Node*)streamHint;
#endif /* ENABLE_MULTIPLE_NODES */
	}
#line 1916 "hint_gram.cpp"
    break;

  case 58: /* stream_hint: Redistribute_P '(' ident_list ')'  */
#line 486 "hint_gram.y"
        {
#ifndef ENABLE_MULTIPLE_NODES
		hint_scanner_yyerror("unsupport distributed hint", yyscanner);
		(yyval.node) = NULL;
#else /* ENABLE_MULTIPLE_NODES */
		StreamHint *streamHint = makeNode(StreamHint);
		streamHint->base.relnames = (yyvsp[-1].list);
		streamHint->base.hint_keyword = HINT_KEYWORD_REDISTRIBUTE;
		streamHint->stream_type = STREAM_REDISTRIBUTE;

		(yyval.node) = (Node*)streamHint;
#endif /* ENABLE_MULTIPLE_NODES */
	}
#line 1934 "hint_gram.cpp"
    break;

  case 59: /* row_hint: Rows_P '(' ident_list '#' expr_const ')'  */
#line 503 "hint_gram.y"
        {
		RowsHint *rowHint = makeNode(RowsHint);
		rowHint->base.relnames = (yyvsp[-3].list);
		rowHint->base.hint_keyword = HINT_KEYWORD_ROWS;
		rowHint->value_type = RVT_ABSOLUTE;
		rowHint->rows = convert_to_numeric((yyvsp[-1].node));
		if (IsA((yyvsp[-1].node), Float))
			rowHint->rows_str = strVal((yyvsp[-1].node));

		(yyval.node) = (Node *) rowHint;
	}
#line 1950 "hint_gram.cpp"
    break;

  case 60: /* row_hint: Rows_P '(' ident_list '+' expr_const ')'  */
#line 515 "hint_gram.y"
        {
		RowsHint *rowHint = makeNode(RowsHint);
		rowHint->base.relnames = (yyvsp[-3].list);
		rowHint->base.hint_keyword = HINT_KEYWORD_ROWS;
		rowHint->value_type = RVT_ADD;
		rowHint->rows = convert_to_numeric((yyvsp[-1].node));
		if (IsA((yyvsp[-1].node), Float))
			rowHint->rows_str = strVal((yyvsp[-1].node));

		(yyval.node) = (Node *) rowHint;
	}
#line 1966 "hint_gram.cpp"
    break;

  case 61: /* row_hint: Rows_P '(' ident_list '-' expr_const ')'  */
#line 527 "hint_gram.y"
        {
		RowsHint *rowHint = makeNode(RowsHint);
		rowHint->base.relnames = (yyvsp[-3].list);
		rowHint->base.hint_keyword = HINT_KEYWORD_ROWS;
		rowHint->value_type = RVT_SUB;
		rowHint->rows = convert_to_numeric((yyvsp[-1].node));
		if (IsA((yyvsp[-1].node), Float))
			rowHint->rows_str = strVal((yyvsp[-1].node));

		(yyval.node) = (Node *) rowHint;
	}
#line 1982 "hint_gram.cpp"
    break;

  case 62: /* row_hint: Rows_P '(' ident_list '*' expr_const ')'  */
#line 539 "hint_gram.y"
        {
		RowsHint *rowHint = makeNode(RowsHint);
		rowHint->base.relnames = (yyvsp[-3].list);
		rowHint->base.hint_keyword = HINT_KEYWORD_ROWS;
		rowHint->value_type = RVT_MULTI;
		rowHint->rows = convert_to_numeric((yyvsp[-1].node));
		if (IsA((yyvsp[-1].node), Float))
			rowHint->rows_str = strVal((yyvsp[-1].node));

		(yyval.node) = (Node *) rowHint;
	}
#line 1998 "hint_gram.cpp"
    break;

  case 63: /* scan_hint: TableScan_P '(' IDENT ')'  */
#line 554 "hint_gram.y"
        {
		ScanMethodHint	*scanHint = makeNode(ScanMethodHint);
		scanHint->base.relnames = list_make1(makeString((yyvsp[-1].str)));
		scanHint->base.hint_keyword = HINT_KEYWORD_TABLESCAN;
		scanHint->base.state = HINT_STATE_NOTUSED;
		(yyval.node) = (Node *) scanHint;
	}
#line 2010 "hint_gram.cpp"
    break;

  case 64: /* scan_hint: IndexScan_P '(' ident_list ')'  */
#line 563 "hint_gram.y"
        {
		ScanMethodHint	*scanHint = makeNode(ScanMethodHint);
		scanHint->base.relnames = list_make1(linitial((yyvsp[-1].list)));
		scanHint->base.hint_keyword = HINT_KEYWORD_INDEXSCAN;
		scanHint->base.state = HINT_STATE_NOTUSED;
		scanHint->indexlist = list_delete_first((yyvsp[-1].list));
		(yyval.node) = (Node *) scanHint;
	}
#line 2023 "hint_gram.cpp"
    break;

  case 65: /* scan_hint: IndexOnlyScan_P '(' ident_list ')'  */
#line 573 "hint_gram.y"
        {
		ScanMethodHint	*scanHint = makeNode(ScanMethodHint);
		scanHint->base.relnames = list_make1(linitial((yyvsp[-1].list)));
		scanHint->base.hint_keyword = HINT_KEYWORD_INDEXONLYSCAN;
		scanHint->base.state = HINT_STATE_NOTUSED;
		scanHint->indexlist = list_delete_first((yyvsp[-1].list));
		(yyval.node) = (Node *) scanHint;
	}
#line 2036 "hint_gram.cpp"
    break;

  case 66: /* skew_hint: Skew_P '(' skew_relist column_list_p value_list_p ')'  */
#line 585 "hint_gram.y"
        {
#ifndef ENABLE_MULTIPLE_NODES
		hint_scanner_yyerror("unsupport distributed hint", yyscanner);
		(yyval.node) = NULL;
#else /* ENABLE_MULTIPLE_NODES */
		SkewHint	*skewHint = makeNode(SkewHint);
		skewHint->base.relnames = (yyvsp[-3].list);
		skewHint->base.hint_keyword = HINT_KEYWORD_SKEW;
		skewHint->base.state = HINT_STATE_NOTUSED;
		skewHint->column_list = (yyvsp[-2].list);
		skewHint->value_list = (yyvsp[-1].list);
		(yyval.node) = (Node *) skewHint;
#endif /* ENABLE_MULTIPLE_NODES */
	}
#line 2055 "hint_gram.cpp"
    break;

  case 67: /* skew_hint: Skew_P '(' skew_relist column_list_p ')'  */
#line 601 "hint_gram.y"
        {
#ifndef ENABLE_MULTIPLE_NODES
		hint_scanner_yyerror("unsupport distributed hint", yyscanner);
		(yyval.node) = NULL;
#else /* ENABLE_MULTIPLE_NODES */
		SkewHint	*skewHint = makeNode(SkewHint);
		skewHint->base.relnames = (yyvsp[-2].list);
		skewHint->base.hint_keyword = HINT_KEYWORD_SKEW;
		skewHint->base.state = HINT_STATE_NOTUSED;
		skewHint->column_list = (yyvsp[-1].list);
		skewHint->value_list = NIL;
		(yyval.node) = (Node *) skewHint;
#endif /* ENABLE_MULTIPLE_NODES */
	}
#line 2074 "hint_gram.cpp"
    break;

  case 68: /* relation_list_with_p: '(' relation_list ')'  */
#line 618 "hint_gram.y"
                                        { (yyval.list) = (yyvsp[-1].list); }
#line 2080 "hint_gram.cpp"
    break;

  case 69: /* relation_item: IDENT  */
#line 622 "hint_gram.y"
                                        { (yyval.list) = list_make1(makeString((yyvsp[0].str))); }
#line 2086 "hint_gram.cpp"
    break;

  case 70: /* relation_item: relation_list_with_p  */
#line 623 "hint_gram.y"
                                        { (yyval.list) = list_make1((yyvsp[0].list)); }
#line 2092 "hint_gram.cpp"
    break;

  case 71: /* relation_list: relation_item relation_item  */
#line 627 "hint_gram.y"
                                          { (yyval.list) = list_concat((yyvsp[-1].list), (yyvsp[0].list)); }
#line 2098 "hint_gram.cpp"
    break;

  case 72: /* relation_list: relation_list relation_item  */
#line 628 "hint_gram.y"
                                            { (yyval.list) = list_concat((yyvsp[-1].list), (yyvsp[0].list)); }
#line 2104 "hint_gram.cpp"
    break;

  case 73: /* ident_list: IDENT  */
#line 632 "hint_gram.y"
                                        { (yyval.list) = list_make1(makeString((yyvsp[0].str))); }
#line 2110 "hint_gram.cpp"
    break;

  case 74: /* ident_list: ident_list IDENT  */
#line 633 "hint_gram.y"
                                        { (yyval.list) = lappend((yyvsp[-1].list), makeString((yyvsp[0].str))); }
#line 2116 "hint_gram.cpp"
    break;

  case 75: /* expr_const: ICONST  */
#line 637 "hint_gram.y"
                                        { (yyval.node) = (Node *) makeInteger((yyvsp[0].ival)); }
#line 2122 "hint_gram.cpp"
    break;

  case 76: /* expr_const: FCONST  */
#line 638 "hint_gram.y"
                                        { (yyval.node) = (Node *) makeFloat((yyvsp[0].str)); }
#line 2128 "hint_gram.cpp"
    break;

  case 77: /* skew_relist: IDENT  */
#line 642 "hint_gram.y"
                                                { (yyval.list) = list_make1(makeString((yyvsp[0].str))); }
#line 2134 "hint_gram.cpp"
    break;

  case 78: /* skew_relist: '(' ident_list ')'  */
#line 643 "hint_gram.y"
                                        { (yyval.list) = (yyvsp[-1].list); }
#line 2140 "hint_gram.cpp"
    break;

  case 79: /* column_list_p: '(' column_list ')'  */
#line 647 "hint_gram.y"
                                { (yyval.list) = (yyvsp[-1].list); }
#line 2146 "hint_gram.cpp"
    break;

  case 80: /* column_list: IDENT  */
#line 651 "hint_gram.y"
                                                { (yyval.list) = list_make1(makeString((yyvsp[0].str))); }
#line 2152 "hint_gram.cpp"
    break;

  case 81: /* column_list: column_list IDENT  */
#line 652 "hint_gram.y"
                                        { (yyval.list) = lappend((yyvsp[-1].list), makeString((yyvsp[0].str))); }
#line 2158 "hint_gram.cpp"
    break;

  case 82: /* value_list_p: '(' value_list ')'  */
#line 656 "hint_gram.y"
                               { (yyval.list) = (yyvsp[-1].list); }
#line 2164 "hint_gram.cpp"
    break;

  case 83: /* value_list: value_list_item  */
#line 660 "hint_gram.y"
                                                   { (yyval.list) = (yyvsp[0].list); }
#line 2170 "hint_gram.cpp"
    break;

  case 84: /* value_list: value_list_with_bracket  */
#line 661 "hint_gram.y"
                                       { (yyval.list) = (yyvsp[0].list); }
#line 2176 "hint_gram.cpp"
    break;

  case 85: /* value_list_with_bracket: value_list_p  */
#line 665 "hint_gram.y"
                                { (yyval.list) = (yyvsp[0].list); }
#line 2182 "hint_gram.cpp"
    break;

  case 86: /* value_list_with_bracket: value_list_with_bracket value_list_p  */
#line 666 "hint_gram.y"
                                                 { (yyval.list) = list_concat((yyvsp[-1].list), (yyvsp[0].list)); }
#line 2188 "hint_gram.cpp"
    break;

  case 87: /* value_list_item: value_type  */
#line 670 "hint_gram.y"
                                        { (yyval.list) = (yyvsp[0].list); }
#line 2194 "hint_gram.cpp"
    break;

  case 88: /* value_list_item: value_list_item value_type  */
#line 671 "hint_gram.y"
                                                {(yyval.list) = list_concat((yyvsp[-1].list), (yyvsp[0].list)); }
#line 2200 "hint_gram.cpp"
    break;

  case 89: /* value_type: ICONST  */
#line 675 "hint_gram.y"
                                        { (yyval.list) = list_make1(makeInteger((yyvsp[0].ival))); }
#line 2206 "hint_gram.cpp"
    break;

  case 90: /* value_type: FCONST  */
#line 676 "hint_gram.y"
                                        { (yyval.list) = list_make1(makeFloat((yyvsp[0].str))); }
#line 2212 "hint_gram.cpp"
    break;

  case 91: /* value_type: SCONST  */
#line 677 "hint_gram.y"
                                        { (yyval.list) = list_make1(makeStringValue((yyvsp[0].str))); }
#line 2218 "hint_gram.cpp"
    break;

  case 92: /* value_type: BCONST  */
#line 678 "hint_gram.y"
                                        { (yyval.list) = list_make1(makeBitStringValue((yyvsp[0].str))); }
#line 2224 "hint_gram.cpp"
    break;

  case 93: /* value_type: XCONST  */
#line 679 "hint_gram.y"
                                        { (yyval.list) = list_make1(makeString((yyvsp[0].str))); }
#line 2230 "hint_gram.cpp"
    break;

  case 94: /* value_type: NULL_P  */
#line 680 "hint_gram.y"
                                        { (yyval.list) = list_make1(makeNullValue()); }
#line 2236 "hint_gram.cpp"
    break;

  case 95: /* value_type: TRUE_P  */
#line 681 "hint_gram.y"
                                        { (yyval.list) = list_make1(makeBoolValue(TRUE)); }
#line 2242 "hint_gram.cpp"
    break;

  case 96: /* value_type: FALSE_P  */
#line 682 "hint_gram.y"
                                        { (yyval.list) = list_make1(makeBoolValue(FALSE)); }
#line 2248 "hint_gram.cpp"
    break;


#line 2252 "hint_gram.cpp"

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
      yyerror (yyscanner, YY_("syntax error"));
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
                      yytoken, &yylval, yyscanner);
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
  ++yynerrs;

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
                  YY_ACCESSING_SYMBOL (yystate), yyvsp, yyscanner);
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
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (yyscanner, YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, yyscanner);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp, yyscanner);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 684 "hint_gram.y"


void
 yyerror(yyscan_t yyscanner, const char *msg)
{
	hint_scanner_yyerror(msg, yyscanner);
	return;
}

static double
convert_to_numeric(Node *value)
{
	double	d = 0;
	Value	*vvalue = (Value *) value;

	switch(nodeTag(vvalue))
	{
		case T_Integer:
			d = intVal(vvalue);
			break;
		case T_Float:
			d = floatVal(vvalue);
			break;
		default:
			break;
	}

	return d;
}

static Value *
makeStringValue(char *str)
{
	Value	   *val = makeNode(Value);

	if (u_sess->attr.attr_sql.sql_compatibility == A_FORMAT)
	{
		if (NULL == str || (0 == strlen(str) && !ACCEPT_EMPTY_STR))
		{
			val->type = T_Null;
			val->val.str = str;
		}
		else
		{
			val->type = T_String;
			val->val.str = str;
		}
	}
	else
	{
		val->type = T_String;
		val->val.str = str;
	}

	return val;
}

static Value *
makeBitStringValue(char *str)
{
	Value	*val = makeNode(Value);

	val->type = T_BitString;
	val->val.str = str;

	return val;
}

static Value *
makeNullValue()
{
	Value	*val = makeNode(Value);

	val->type = T_Null;

	return val;

}

static Value *
makeBoolValue(bool state)
{
	Value	*val = makeNode(Value);
	val->type = T_String;
	val->val.str = (char *)(state ? "t" : "f");

	return val;
}

static void
doNegateFloat(Value *v)
{
	char   *oldval = v->val.str;
	Assert(IsA(v, Float));
	if (*oldval == '+')
		oldval++;
	if (*oldval == '-')
		v->val.str = oldval + 1;	/* just strip the '-' */
	else
	{
		char   *newval = (char *) palloc(strlen(oldval) + 2);

		*newval = '-';
		strcpy(newval + 1, oldval);
		v->val.str = newval;
	}
}

static Value* integerToString(Value *v)
{
    Assert(IsA(v, Integer));
    long num = intVal(v);
    const int max_len_long_type = 11;
    char* str = (char*)palloc0(sizeof(char) * (max_len_long_type + 1));
    errno_t rc = sprintf_s(str, max_len_long_type + 1, "%ld", num);
    securec_check_ss(rc, "\0", "\0");
	Value* ret = makeString(str);
    pfree(v);
    return ret;
}

#include "hint_scan.inc"

