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

#ifndef YY_BASE_YY_PREPROC_HPP_INCLUDED
# define YY_BASE_YY_PREPROC_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int base_yydebug;
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
    SQL_ALLOCATE = 258,            /* SQL_ALLOCATE  */
    SQL_AUTOCOMMIT = 259,          /* SQL_AUTOCOMMIT  */
    SQL_BOOL = 260,                /* SQL_BOOL  */
    SQL_BREAK = 261,               /* SQL_BREAK  */
    SQL_CALL = 262,                /* SQL_CALL  */
    SQL_CARDINALITY = 263,         /* SQL_CARDINALITY  */
    SQL_COUNT = 264,               /* SQL_COUNT  */
    SQL_DATETIME_INTERVAL_CODE = 265, /* SQL_DATETIME_INTERVAL_CODE  */
    SQL_DATETIME_INTERVAL_PRECISION = 266, /* SQL_DATETIME_INTERVAL_PRECISION  */
    SQL_DESCRIBE = 267,            /* SQL_DESCRIBE  */
    SQL_DESCRIPTOR = 268,          /* SQL_DESCRIPTOR  */
    SQL_FOUND = 269,               /* SQL_FOUND  */
    SQL_FREE = 270,                /* SQL_FREE  */
    SQL_GET = 271,                 /* SQL_GET  */
    SQL_GO = 272,                  /* SQL_GO  */
    SQL_GOTO = 273,                /* SQL_GOTO  */
    SQL_IDENTIFIED = 274,          /* SQL_IDENTIFIED  */
    SQL_INDICATOR = 275,           /* SQL_INDICATOR  */
    SQL_KEY_MEMBER = 276,          /* SQL_KEY_MEMBER  */
    SQL_LENGTH = 277,              /* SQL_LENGTH  */
    SQL_LONG = 278,                /* SQL_LONG  */
    SQL_NULLABLE = 279,            /* SQL_NULLABLE  */
    SQL_OCTET_LENGTH = 280,        /* SQL_OCTET_LENGTH  */
    SQL_OPEN = 281,                /* SQL_OPEN  */
    SQL_OUTPUT = 282,              /* SQL_OUTPUT  */
    SQL_REFERENCE = 283,           /* SQL_REFERENCE  */
    SQL_RETURNED_LENGTH = 284,     /* SQL_RETURNED_LENGTH  */
    SQL_RETURNED_OCTET_LENGTH = 285, /* SQL_RETURNED_OCTET_LENGTH  */
    SQL_SCALE = 286,               /* SQL_SCALE  */
    SQL_SECTION = 287,             /* SQL_SECTION  */
    SQL_SHORT = 288,               /* SQL_SHORT  */
    SQL_SIGNED = 289,              /* SQL_SIGNED  */
    SQL_SQL = 290,                 /* SQL_SQL  */
    SQL_SQLERROR = 291,            /* SQL_SQLERROR  */
    SQL_SQLPRINT = 292,            /* SQL_SQLPRINT  */
    SQL_SQLWARNING = 293,          /* SQL_SQLWARNING  */
    SQL_START = 294,               /* SQL_START  */
    SQL_STOP = 295,                /* SQL_STOP  */
    SQL_STRUCT = 296,              /* SQL_STRUCT  */
    SQL_UNSIGNED = 297,            /* SQL_UNSIGNED  */
    SQL_VAR = 298,                 /* SQL_VAR  */
    SQL_WHENEVER = 299,            /* SQL_WHENEVER  */
    S_ADD = 300,                   /* S_ADD  */
    S_AND = 301,                   /* S_AND  */
    S_ANYTHING = 302,              /* S_ANYTHING  */
    S_AUTO = 303,                  /* S_AUTO  */
    S_CONST = 304,                 /* S_CONST  */
    S_DEC = 305,                   /* S_DEC  */
    S_DIV = 306,                   /* S_DIV  */
    S_DOTPOINT = 307,              /* S_DOTPOINT  */
    S_EQUAL = 308,                 /* S_EQUAL  */
    S_EXTERN = 309,                /* S_EXTERN  */
    S_INC = 310,                   /* S_INC  */
    S_LSHIFT = 311,                /* S_LSHIFT  */
    S_MEMPOINT = 312,              /* S_MEMPOINT  */
    S_MEMBER = 313,                /* S_MEMBER  */
    S_MOD = 314,                   /* S_MOD  */
    S_MUL = 315,                   /* S_MUL  */
    S_NEQUAL = 316,                /* S_NEQUAL  */
    S_OR = 317,                    /* S_OR  */
    S_REGISTER = 318,              /* S_REGISTER  */
    S_RSHIFT = 319,                /* S_RSHIFT  */
    S_STATIC = 320,                /* S_STATIC  */
    S_SUB = 321,                   /* S_SUB  */
    S_VOLATILE = 322,              /* S_VOLATILE  */
    S_TYPEDEF = 323,               /* S_TYPEDEF  */
    CSTRING = 324,                 /* CSTRING  */
    CVARIABLE = 325,               /* CVARIABLE  */
    CPP_LINE = 326,                /* CPP_LINE  */
    SQL_IP = 327,                  /* SQL_IP  */
    DOLCONST = 328,                /* DOLCONST  */
    ECONST = 329,                  /* ECONST  */
    NCONST = 330,                  /* NCONST  */
    UCONST = 331,                  /* UCONST  */
    UIDENT = 332,                  /* UIDENT  */
    IDENT = 333,                   /* IDENT  */
    FCONST = 334,                  /* FCONST  */
    SCONST = 335,                  /* SCONST  */
    BCONST = 336,                  /* BCONST  */
    VCONST = 337,                  /* VCONST  */
    XCONST = 338,                  /* XCONST  */
    Op = 339,                      /* Op  */
    CmpOp = 340,                   /* CmpOp  */
    CmpNullOp = 341,               /* CmpNullOp  */
    COMMENTSTRING = 342,           /* COMMENTSTRING  */
    SET_USER_IDENT = 343,          /* SET_USER_IDENT  */
    SET_IDENT = 344,               /* SET_IDENT  */
    UNDERSCORE_CHARSET = 345,      /* UNDERSCORE_CHARSET  */
    FCONST_F = 346,                /* FCONST_F  */
    FCONST_D = 347,                /* FCONST_D  */
    ICONST = 348,                  /* ICONST  */
    PARAM = 349,                   /* PARAM  */
    TYPECAST = 350,                /* TYPECAST  */
    ORA_JOINOP = 351,              /* ORA_JOINOP  */
    DOT_DOT = 352,                 /* DOT_DOT  */
    COLON_EQUALS = 353,            /* COLON_EQUALS  */
    PARA_EQUALS = 354,             /* PARA_EQUALS  */
    SET_IDENT_SESSION = 355,       /* SET_IDENT_SESSION  */
    SET_IDENT_GLOBAL = 356,        /* SET_IDENT_GLOBAL  */
    DIALECT_TSQL = 357,            /* DIALECT_TSQL  */
    TSQL_XCONST = 358,             /* TSQL_XCONST  */
    ABORT_P = 359,                 /* ABORT_P  */
    ABSOLUTE_P = 360,              /* ABSOLUTE_P  */
    ACCESS = 361,                  /* ACCESS  */
    ACCOUNT = 362,                 /* ACCOUNT  */
    ACTION = 363,                  /* ACTION  */
    ADD_P = 364,                   /* ADD_P  */
    ADMIN = 365,                   /* ADMIN  */
    AFTER = 366,                   /* AFTER  */
    AGGREGATE = 367,               /* AGGREGATE  */
    ALGORITHM = 368,               /* ALGORITHM  */
    ALL = 369,                     /* ALL  */
    ALSO = 370,                    /* ALSO  */
    ALTER = 371,                   /* ALTER  */
    ALWAYS = 372,                  /* ALWAYS  */
    ANALYSE = 373,                 /* ANALYSE  */
    ANALYZE = 374,                 /* ANALYZE  */
    AND = 375,                     /* AND  */
    ANY = 376,                     /* ANY  */
    APP = 377,                     /* APP  */
    APPEND = 378,                  /* APPEND  */
    APPLY = 379,                   /* APPLY  */
    ARCHIVE = 380,                 /* ARCHIVE  */
    ARRAY = 381,                   /* ARRAY  */
    AS = 382,                      /* AS  */
    ASC = 383,                     /* ASC  */
    ASOF_P = 384,                  /* ASOF_P  */
    ASSERTION = 385,               /* ASSERTION  */
    ASSIGNMENT = 386,              /* ASSIGNMENT  */
    ASYMMETRIC = 387,              /* ASYMMETRIC  */
    AT = 388,                      /* AT  */
    ATTRIBUTE = 389,               /* ATTRIBUTE  */
    AUDIT = 390,                   /* AUDIT  */
    AUTHID = 391,                  /* AUTHID  */
    AUTHORIZATION = 392,           /* AUTHORIZATION  */
    AUTOEXTEND = 393,              /* AUTOEXTEND  */
    AUTOMAPPED = 394,              /* AUTOMAPPED  */
    AUTO_INCREMENT = 395,          /* AUTO_INCREMENT  */
    BACKWARD = 396,                /* BACKWARD  */
    BARRIER = 397,                 /* BARRIER  */
    BEFORE = 398,                  /* BEFORE  */
    BEGIN_NON_ANOYBLOCK = 399,     /* BEGIN_NON_ANOYBLOCK  */
    BEGIN_P = 400,                 /* BEGIN_P  */
    BETWEEN = 401,                 /* BETWEEN  */
    BIGINT = 402,                  /* BIGINT  */
    BINARY = 403,                  /* BINARY  */
    BINARY_DOUBLE = 404,           /* BINARY_DOUBLE  */
    BINARY_DOUBLE_INF = 405,       /* BINARY_DOUBLE_INF  */
    BINARY_DOUBLE_NAN = 406,       /* BINARY_DOUBLE_NAN  */
    BINARY_INTEGER = 407,          /* BINARY_INTEGER  */
    BIT = 408,                     /* BIT  */
    BLANKS = 409,                  /* BLANKS  */
    BLOB_P = 410,                  /* BLOB_P  */
    BLOCKCHAIN = 411,              /* BLOCKCHAIN  */
    BODY_P = 412,                  /* BODY_P  */
    BOGUS = 413,                   /* BOGUS  */
    BOOLEAN_P = 414,               /* BOOLEAN_P  */
    BOTH = 415,                    /* BOTH  */
    BUCKETCNT = 416,               /* BUCKETCNT  */
    BUCKETS = 417,                 /* BUCKETS  */
    BUILD = 418,                   /* BUILD  */
    BY = 419,                      /* BY  */
    BYTE_P = 420,                  /* BYTE_P  */
    BYTEAWITHOUTORDER = 421,       /* BYTEAWITHOUTORDER  */
    BYTEAWITHOUTORDERWITHEQUAL = 422, /* BYTEAWITHOUTORDERWITHEQUAL  */
    CACHE = 423,                   /* CACHE  */
    CALL = 424,                    /* CALL  */
    CALLED = 425,                  /* CALLED  */
    CANCELABLE = 426,              /* CANCELABLE  */
    CASCADE = 427,                 /* CASCADE  */
    CASCADED = 428,                /* CASCADED  */
    CASE = 429,                    /* CASE  */
    CAST = 430,                    /* CAST  */
    CATALOG_P = 431,               /* CATALOG_P  */
    CATALOG_NAME = 432,            /* CATALOG_NAME  */
    CHAIN = 433,                   /* CHAIN  */
    CHANGE = 434,                  /* CHANGE  */
    CHAR_P = 435,                  /* CHAR_P  */
    CHARACTER = 436,               /* CHARACTER  */
    CHARACTERISTICS = 437,         /* CHARACTERISTICS  */
    CHARACTERSET = 438,            /* CHARACTERSET  */
    CHARSET = 439,                 /* CHARSET  */
    CHECK = 440,                   /* CHECK  */
    CHECKPOINT = 441,              /* CHECKPOINT  */
    CLASS = 442,                   /* CLASS  */
    CLASS_ORIGIN = 443,            /* CLASS_ORIGIN  */
    CLEAN = 444,                   /* CLEAN  */
    CLIENT = 445,                  /* CLIENT  */
    CLIENT_MASTER_KEY = 446,       /* CLIENT_MASTER_KEY  */
    CLIENT_MASTER_KEYS = 447,      /* CLIENT_MASTER_KEYS  */
    CLOB = 448,                    /* CLOB  */
    CLOSE = 449,                   /* CLOSE  */
    CLUSTER = 450,                 /* CLUSTER  */
    COALESCE = 451,                /* COALESCE  */
    COLLATE = 452,                 /* COLLATE  */
    COLLATION = 453,               /* COLLATION  */
    COLUMN = 454,                  /* COLUMN  */
    COLUMN_ENCRYPTION_KEY = 455,   /* COLUMN_ENCRYPTION_KEY  */
    COLUMN_ENCRYPTION_KEYS = 456,  /* COLUMN_ENCRYPTION_KEYS  */
    COLUMN_NAME = 457,             /* COLUMN_NAME  */
    COLUMNS = 458,                 /* COLUMNS  */
    COMMENT = 459,                 /* COMMENT  */
    COMMENTS = 460,                /* COMMENTS  */
    COMMIT = 461,                  /* COMMIT  */
    COMMITTED = 462,               /* COMMITTED  */
    COMPACT = 463,                 /* COMPACT  */
    COMPATIBLE_ILLEGAL_CHARS = 464, /* COMPATIBLE_ILLEGAL_CHARS  */
    COMPILE = 465,                 /* COMPILE  */
    COMPLETE = 466,                /* COMPLETE  */
    COMPLETION = 467,              /* COMPLETION  */
    COMPRESS = 468,                /* COMPRESS  */
    CONCURRENTLY = 469,            /* CONCURRENTLY  */
    CONDITION = 470,               /* CONDITION  */
    CONFIGURATION = 471,           /* CONFIGURATION  */
    CONNECTION = 472,              /* CONNECTION  */
    CONSISTENT = 473,              /* CONSISTENT  */
    CONSTANT = 474,                /* CONSTANT  */
    CONSTRAINT = 475,              /* CONSTRAINT  */
    CONSTRAINT_CATALOG = 476,      /* CONSTRAINT_CATALOG  */
    CONSTRAINT_NAME = 477,         /* CONSTRAINT_NAME  */
    CONSTRAINT_SCHEMA = 478,       /* CONSTRAINT_SCHEMA  */
    CONSTRAINTS = 479,             /* CONSTRAINTS  */
    CONTENT_P = 480,               /* CONTENT_P  */
    CONTINUE_P = 481,              /* CONTINUE_P  */
    CONTVIEW = 482,                /* CONTVIEW  */
    CONVERSION_P = 483,            /* CONVERSION_P  */
    CONVERT_P = 484,               /* CONVERT_P  */
    CONNECT = 485,                 /* CONNECT  */
    COORDINATOR = 486,             /* COORDINATOR  */
    COORDINATORS = 487,            /* COORDINATORS  */
    COPY = 488,                    /* COPY  */
    COST = 489,                    /* COST  */
    CREATE = 490,                  /* CREATE  */
    CROSS = 491,                   /* CROSS  */
    CSN = 492,                     /* CSN  */
    CSV = 493,                     /* CSV  */
    CUBE = 494,                    /* CUBE  */
    CURRENT_P = 495,               /* CURRENT_P  */
    CURRENT_CATALOG = 496,         /* CURRENT_CATALOG  */
    CURRENT_DATE = 497,            /* CURRENT_DATE  */
    CURRENT_ROLE = 498,            /* CURRENT_ROLE  */
    CURRENT_SCHEMA = 499,          /* CURRENT_SCHEMA  */
    CURRENT_TIME = 500,            /* CURRENT_TIME  */
    CURRENT_TIMESTAMP = 501,       /* CURRENT_TIMESTAMP  */
    CURRENT_USER = 502,            /* CURRENT_USER  */
    CURSOR = 503,                  /* CURSOR  */
    CURSOR_NAME = 504,             /* CURSOR_NAME  */
    CYCLE = 505,                   /* CYCLE  */
    SHRINK = 506,                  /* SHRINK  */
    USE_P = 507,                   /* USE_P  */
    DATA_P = 508,                  /* DATA_P  */
    DATABASE = 509,                /* DATABASE  */
    DATAFILE = 510,                /* DATAFILE  */
    DATANODE = 511,                /* DATANODE  */
    DATANODES = 512,               /* DATANODES  */
    DATATYPE_CL = 513,             /* DATATYPE_CL  */
    DATE_P = 514,                  /* DATE_P  */
    DATE_FORMAT_P = 515,           /* DATE_FORMAT_P  */
    DAY_P = 516,                   /* DAY_P  */
    DAY_HOUR_P = 517,              /* DAY_HOUR_P  */
    DAY_MINUTE_P = 518,            /* DAY_MINUTE_P  */
    DAY_SECOND_P = 519,            /* DAY_SECOND_P  */
    DBCOMPATIBILITY_P = 520,       /* DBCOMPATIBILITY_P  */
    DEALLOCATE = 521,              /* DEALLOCATE  */
    DEC = 522,                     /* DEC  */
    DECIMAL_P = 523,               /* DECIMAL_P  */
    DECLARE = 524,                 /* DECLARE  */
    DECODE = 525,                  /* DECODE  */
    DEFAULT = 526,                 /* DEFAULT  */
    DEFAULTS = 527,                /* DEFAULTS  */
    DEFERRABLE = 528,              /* DEFERRABLE  */
    DEFERRED = 529,                /* DEFERRED  */
    DEFINER = 530,                 /* DEFINER  */
    DELETE_P = 531,                /* DELETE_P  */
    DELIMITER = 532,               /* DELIMITER  */
    DELIMITERS = 533,              /* DELIMITERS  */
    DELTA = 534,                   /* DELTA  */
    DELTAMERGE = 535,              /* DELTAMERGE  */
    DENSE_RANK = 536,              /* DENSE_RANK  */
    DESC = 537,                    /* DESC  */
    DETERMINISTIC = 538,           /* DETERMINISTIC  */
    DIAGNOSTICS = 539,             /* DIAGNOSTICS  */
    DICTIONARY = 540,              /* DICTIONARY  */
    DIRECT = 541,                  /* DIRECT  */
    DIRECTORY = 542,               /* DIRECTORY  */
    DISABLE_P = 543,               /* DISABLE_P  */
    DISCARD = 544,                 /* DISCARD  */
    DISTINCT = 545,                /* DISTINCT  */
    DISTRIBUTE = 546,              /* DISTRIBUTE  */
    DISTRIBUTION = 547,            /* DISTRIBUTION  */
    DO = 548,                      /* DO  */
    DOCUMENT_P = 549,              /* DOCUMENT_P  */
    DOMAIN_P = 550,                /* DOMAIN_P  */
    DOUBLE_P = 551,                /* DOUBLE_P  */
    DROP = 552,                    /* DROP  */
    DUPLICATE = 553,               /* DUPLICATE  */
    DISCONNECT = 554,              /* DISCONNECT  */
    DUMPFILE = 555,                /* DUMPFILE  */
    EACH = 556,                    /* EACH  */
    ELASTIC = 557,                 /* ELASTIC  */
    ELSE = 558,                    /* ELSE  */
    ENABLE_P = 559,                /* ENABLE_P  */
    ENCLOSED = 560,                /* ENCLOSED  */
    ENCODING = 561,                /* ENCODING  */
    ENCRYPTED = 562,               /* ENCRYPTED  */
    ENCRYPTED_VALUE = 563,         /* ENCRYPTED_VALUE  */
    ENCRYPTION = 564,              /* ENCRYPTION  */
    ENCRYPTION_TYPE = 565,         /* ENCRYPTION_TYPE  */
    END_P = 566,                   /* END_P  */
    ENDS = 567,                    /* ENDS  */
    ENFORCED = 568,                /* ENFORCED  */
    ENUM_P = 569,                  /* ENUM_P  */
    ERROR_P = 570,                 /* ERROR_P  */
    ERRORS = 571,                  /* ERRORS  */
    ESCAPE = 572,                  /* ESCAPE  */
    EOL = 573,                     /* EOL  */
    ESCAPING = 574,                /* ESCAPING  */
    EVENT = 575,                   /* EVENT  */
    EVENTS = 576,                  /* EVENTS  */
    EVERY = 577,                   /* EVERY  */
    EXCEPT = 578,                  /* EXCEPT  */
    EXCHANGE = 579,                /* EXCHANGE  */
    EXCLUDE = 580,                 /* EXCLUDE  */
    EXCLUDED = 581,                /* EXCLUDED  */
    EXCLUDING = 582,               /* EXCLUDING  */
    EXCLUSIVE = 583,               /* EXCLUSIVE  */
    EXECUTE = 584,                 /* EXECUTE  */
    EXISTS = 585,                  /* EXISTS  */
    EXPIRED_P = 586,               /* EXPIRED_P  */
    EXPLAIN = 587,                 /* EXPLAIN  */
    EXTENSION = 588,               /* EXTENSION  */
    EXTERNAL = 589,                /* EXTERNAL  */
    EXTRACT = 590,                 /* EXTRACT  */
    ESCAPED = 591,                 /* ESCAPED  */
    FALSE_P = 592,                 /* FALSE_P  */
    FAMILY = 593,                  /* FAMILY  */
    FAST = 594,                    /* FAST  */
    FENCED = 595,                  /* FENCED  */
    FETCH = 596,                   /* FETCH  */
    FIELDS = 597,                  /* FIELDS  */
    FILEHEADER_P = 598,            /* FILEHEADER_P  */
    FILL_MISSING_FIELDS = 599,     /* FILL_MISSING_FIELDS  */
    FILLER = 600,                  /* FILLER  */
    FILTER = 601,                  /* FILTER  */
    FIRST_P = 602,                 /* FIRST_P  */
    FIXED_P = 603,                 /* FIXED_P  */
    FLOAT_P = 604,                 /* FLOAT_P  */
    FOLLOWING = 605,               /* FOLLOWING  */
    FOLLOWS_P = 606,               /* FOLLOWS_P  */
    FOR = 607,                     /* FOR  */
    FORCE = 608,                   /* FORCE  */
    FOREIGN = 609,                 /* FOREIGN  */
    FORMATTER = 610,               /* FORMATTER  */
    FORWARD = 611,                 /* FORWARD  */
    FEATURES = 612,                /* FEATURES  */
    FREEZE = 613,                  /* FREEZE  */
    FROM = 614,                    /* FROM  */
    FULL = 615,                    /* FULL  */
    FUNCTION = 616,                /* FUNCTION  */
    FUNCTIONS = 617,               /* FUNCTIONS  */
    GENERATED = 618,               /* GENERATED  */
    GET = 619,                     /* GET  */
    GLOBAL = 620,                  /* GLOBAL  */
    GRANT = 621,                   /* GRANT  */
    GRANTED = 622,                 /* GRANTED  */
    GREATEST = 623,                /* GREATEST  */
    GROUP_P = 624,                 /* GROUP_P  */
    GROUPING_P = 625,              /* GROUPING_P  */
    GROUPPARENT = 626,             /* GROUPPARENT  */
    HANDLER = 627,                 /* HANDLER  */
    HAVING = 628,                  /* HAVING  */
    HDFSDIRECTORY = 629,           /* HDFSDIRECTORY  */
    HEADER_P = 630,                /* HEADER_P  */
    HOLD = 631,                    /* HOLD  */
    HOUR_P = 632,                  /* HOUR_P  */
    HOUR_MINUTE_P = 633,           /* HOUR_MINUTE_P  */
    HOUR_SECOND_P = 634,           /* HOUR_SECOND_P  */
    IDENTIFIED = 635,              /* IDENTIFIED  */
    IDENTITY_P = 636,              /* IDENTITY_P  */
    IF_P = 637,                    /* IF_P  */
    IGNORE = 638,                  /* IGNORE  */
    IGNORE_EXTRA_DATA = 639,       /* IGNORE_EXTRA_DATA  */
    ILIKE = 640,                   /* ILIKE  */
    IMMEDIATE = 641,               /* IMMEDIATE  */
    IMMUTABLE = 642,               /* IMMUTABLE  */
    IMPLICIT_P = 643,              /* IMPLICIT_P  */
    IN_P = 644,                    /* IN_P  */
    INCLUDE = 645,                 /* INCLUDE  */
    IMCSTORED = 646,               /* IMCSTORED  */
    INCLUDING = 647,               /* INCLUDING  */
    INCREMENT = 648,               /* INCREMENT  */
    INCREMENTAL = 649,             /* INCREMENTAL  */
    INDEX = 650,                   /* INDEX  */
    INDEXES = 651,                 /* INDEXES  */
    INFILE = 652,                  /* INFILE  */
    INFINITE_P = 653,              /* INFINITE_P  */
    INHERIT = 654,                 /* INHERIT  */
    INHERITS = 655,                /* INHERITS  */
    INITIAL_P = 656,               /* INITIAL_P  */
    INITIALLY = 657,               /* INITIALLY  */
    INITRANS = 658,                /* INITRANS  */
    INLINE_P = 659,                /* INLINE_P  */
    INNER_P = 660,                 /* INNER_P  */
    INOUT = 661,                   /* INOUT  */
    INPUT_P = 662,                 /* INPUT_P  */
    INSENSITIVE = 663,             /* INSENSITIVE  */
    INSERT = 664,                  /* INSERT  */
    INSTEAD = 665,                 /* INSTEAD  */
    INT_P = 666,                   /* INT_P  */
    INTEGER = 667,                 /* INTEGER  */
    INTERNAL = 668,                /* INTERNAL  */
    INTERSECT = 669,               /* INTERSECT  */
    INTERVAL = 670,                /* INTERVAL  */
    INTO = 671,                    /* INTO  */
    INVISIBLE = 672,               /* INVISIBLE  */
    INVOKER = 673,                 /* INVOKER  */
    IP = 674,                      /* IP  */
    IS = 675,                      /* IS  */
    ISNULL = 676,                  /* ISNULL  */
    ISOLATION = 677,               /* ISOLATION  */
    JOIN = 678,                    /* JOIN  */
    JSON_EXISTS = 679,             /* JSON_EXISTS  */
    KEEP = 680,                    /* KEEP  */
    KEY = 681,                     /* KEY  */
    KILL = 682,                    /* KILL  */
    KEY_PATH = 683,                /* KEY_PATH  */
    KEY_STORE = 684,               /* KEY_STORE  */
    LABEL = 685,                   /* LABEL  */
    LANGUAGE = 686,                /* LANGUAGE  */
    LARGE_P = 687,                 /* LARGE_P  */
    LAST_P = 688,                  /* LAST_P  */
    LATERAL_P = 689,               /* LATERAL_P  */
    LC_COLLATE_P = 690,            /* LC_COLLATE_P  */
    LC_CTYPE_P = 691,              /* LC_CTYPE_P  */
    LEADING = 692,                 /* LEADING  */
    LEAKPROOF = 693,               /* LEAKPROOF  */
    LINES = 694,                   /* LINES  */
    LEAST = 695,                   /* LEAST  */
    LESS = 696,                    /* LESS  */
    LEFT = 697,                    /* LEFT  */
    LEVEL = 698,                   /* LEVEL  */
    LIKE = 699,                    /* LIKE  */
    LIMIT = 700,                   /* LIMIT  */
    LIST = 701,                    /* LIST  */
    LISTEN = 702,                  /* LISTEN  */
    LOAD = 703,                    /* LOAD  */
    LOCAL = 704,                   /* LOCAL  */
    LOCALTIME = 705,               /* LOCALTIME  */
    LOCALTIMESTAMP = 706,          /* LOCALTIMESTAMP  */
    LOCATION = 707,                /* LOCATION  */
    LOCK_P = 708,                  /* LOCK_P  */
    LOCKED = 709,                  /* LOCKED  */
    LOG_P = 710,                   /* LOG_P  */
    LOGGING = 711,                 /* LOGGING  */
    LOGIN_ANY = 712,               /* LOGIN_ANY  */
    LOGIN_FAILURE = 713,           /* LOGIN_FAILURE  */
    LOGIN_SUCCESS = 714,           /* LOGIN_SUCCESS  */
    LOGOUT = 715,                  /* LOGOUT  */
    LONG = 716,                    /* LONG  */
    LOOP = 717,                    /* LOOP  */
    MAPPING = 718,                 /* MAPPING  */
    MASKING = 719,                 /* MASKING  */
    MASTER = 720,                  /* MASTER  */
    MATCH = 721,                   /* MATCH  */
    MATERIALIZED = 722,            /* MATERIALIZED  */
    MATCHED = 723,                 /* MATCHED  */
    MAXEXTENTS = 724,              /* MAXEXTENTS  */
    MAXSIZE = 725,                 /* MAXSIZE  */
    MAXTRANS = 726,                /* MAXTRANS  */
    MAXVALUE = 727,                /* MAXVALUE  */
    MERGE = 728,                   /* MERGE  */
    MESSAGE_TEXT = 729,            /* MESSAGE_TEXT  */
    METHOD = 730,                  /* METHOD  */
    MINUS_P = 731,                 /* MINUS_P  */
    MINUTE_P = 732,                /* MINUTE_P  */
    MINUTE_SECOND_P = 733,         /* MINUTE_SECOND_P  */
    MINVALUE = 734,                /* MINVALUE  */
    MINEXTENTS = 735,              /* MINEXTENTS  */
    MODE = 736,                    /* MODE  */
    MODEL = 737,                   /* MODEL  */
    MODIFY_P = 738,                /* MODIFY_P  */
    MONTH_P = 739,                 /* MONTH_P  */
    MOVE = 740,                    /* MOVE  */
    MOVEMENT = 741,                /* MOVEMENT  */
    MYSQL_ERRNO = 742,             /* MYSQL_ERRNO  */
    NAME_P = 743,                  /* NAME_P  */
    NAMES = 744,                   /* NAMES  */
    NAN_P = 745,                   /* NAN_P  */
    NATIONAL = 746,                /* NATIONAL  */
    NATURAL = 747,                 /* NATURAL  */
    NCHAR = 748,                   /* NCHAR  */
    NEXT = 749,                    /* NEXT  */
    NO = 750,                      /* NO  */
    NOCOMPRESS = 751,              /* NOCOMPRESS  */
    NOCYCLE = 752,                 /* NOCYCLE  */
    NODE = 753,                    /* NODE  */
    NOLOGGING = 754,               /* NOLOGGING  */
    NOMAXVALUE = 755,              /* NOMAXVALUE  */
    NOMINVALUE = 756,              /* NOMINVALUE  */
    NONE = 757,                    /* NONE  */
    NOT = 758,                     /* NOT  */
    NOTHING = 759,                 /* NOTHING  */
    NOTIFY = 760,                  /* NOTIFY  */
    NOTNULL = 761,                 /* NOTNULL  */
    NOVALIDATE = 762,              /* NOVALIDATE  */
    NOWAIT = 763,                  /* NOWAIT  */
    NTH_VALUE_P = 764,             /* NTH_VALUE_P  */
    NULL_P = 765,                  /* NULL_P  */
    NULLCOLS = 766,                /* NULLCOLS  */
    NULLIF = 767,                  /* NULLIF  */
    NULLS_P = 768,                 /* NULLS_P  */
    NUMBER_P = 769,                /* NUMBER_P  */
    NUMERIC = 770,                 /* NUMERIC  */
    NUMSTR = 771,                  /* NUMSTR  */
    NVARCHAR = 772,                /* NVARCHAR  */
    NVARCHAR2 = 773,               /* NVARCHAR2  */
    NVL = 774,                     /* NVL  */
    OBJECT_P = 775,                /* OBJECT_P  */
    OF = 776,                      /* OF  */
    OFF = 777,                     /* OFF  */
    OFFSET = 778,                  /* OFFSET  */
    OIDS = 779,                    /* OIDS  */
    ON = 780,                      /* ON  */
    ONLY = 781,                    /* ONLY  */
    OPERATOR = 782,                /* OPERATOR  */
    OPTIMIZATION = 783,            /* OPTIMIZATION  */
    OPTION = 784,                  /* OPTION  */
    OPTIONALLY = 785,              /* OPTIONALLY  */
    OPTIONS = 786,                 /* OPTIONS  */
    OR = 787,                      /* OR  */
    ORDER = 788,                   /* ORDER  */
    ORDINALITY = 789,              /* ORDINALITY  */
    OUT_P = 790,                   /* OUT_P  */
    OUTER_P = 791,                 /* OUTER_P  */
    OVER = 792,                    /* OVER  */
    OVERLAPS = 793,                /* OVERLAPS  */
    OVERLAY = 794,                 /* OVERLAY  */
    OWNED = 795,                   /* OWNED  */
    OWNER = 796,                   /* OWNER  */
    OUTFILE = 797,                 /* OUTFILE  */
    PACKAGE = 798,                 /* PACKAGE  */
    PACKAGES = 799,                /* PACKAGES  */
    PARALLEL_ENABLE = 800,         /* PARALLEL_ENABLE  */
    PARSER = 801,                  /* PARSER  */
    PARTIAL = 802,                 /* PARTIAL  */
    PARTITION = 803,               /* PARTITION  */
    PARTITIONS = 804,              /* PARTITIONS  */
    PASSING = 805,                 /* PASSING  */
    PASSWORD = 806,                /* PASSWORD  */
    PCTFREE = 807,                 /* PCTFREE  */
    PER_P = 808,                   /* PER_P  */
    PERCENT = 809,                 /* PERCENT  */
    PERFORMANCE = 810,             /* PERFORMANCE  */
    PERM = 811,                    /* PERM  */
    PLACING = 812,                 /* PLACING  */
    PLAN = 813,                    /* PLAN  */
    PLANS = 814,                   /* PLANS  */
    POLICY = 815,                  /* POLICY  */
    POSITION = 816,                /* POSITION  */
    PIPELINED = 817,               /* PIPELINED  */
    POOL = 818,                    /* POOL  */
    PRECEDING = 819,               /* PRECEDING  */
    PRECISION = 820,               /* PRECISION  */
    PREDICT = 821,                 /* PREDICT  */
    PREFERRED = 822,               /* PREFERRED  */
    PREFIX = 823,                  /* PREFIX  */
    PRESERVE = 824,                /* PRESERVE  */
    PREPARE = 825,                 /* PREPARE  */
    PREPARED = 826,                /* PREPARED  */
    PRIMARY = 827,                 /* PRIMARY  */
    PRECEDES_P = 828,              /* PRECEDES_P  */
    PRIVATE = 829,                 /* PRIVATE  */
    PRIOR = 830,                   /* PRIOR  */
    PRIORER = 831,                 /* PRIORER  */
    PRIVILEGES = 832,              /* PRIVILEGES  */
    PRIVILEGE = 833,               /* PRIVILEGE  */
    PROCEDURAL = 834,              /* PROCEDURAL  */
    PROCEDURE = 835,               /* PROCEDURE  */
    PROFILE = 836,                 /* PROFILE  */
    PUBLICATION = 837,             /* PUBLICATION  */
    PUBLISH = 838,                 /* PUBLISH  */
    PURGE = 839,                   /* PURGE  */
    QUERY = 840,                   /* QUERY  */
    QUOTE = 841,                   /* QUOTE  */
    RANDOMIZED = 842,              /* RANDOMIZED  */
    RANGE = 843,                   /* RANGE  */
    RATIO = 844,                   /* RATIO  */
    RAW = 845,                     /* RAW  */
    READ = 846,                    /* READ  */
    REAL = 847,                    /* REAL  */
    REASSIGN = 848,                /* REASSIGN  */
    REBUILD = 849,                 /* REBUILD  */
    RECHECK = 850,                 /* RECHECK  */
    RECURSIVE = 851,               /* RECURSIVE  */
    RECYCLEBIN = 852,              /* RECYCLEBIN  */
    REDISANYVALUE = 853,           /* REDISANYVALUE  */
    REF = 854,                     /* REF  */
    REFERENCES = 855,              /* REFERENCES  */
    REFRESH = 856,                 /* REFRESH  */
    REINDEX = 857,                 /* REINDEX  */
    REJECT_P = 858,                /* REJECT_P  */
    RELATIVE_P = 859,              /* RELATIVE_P  */
    RELEASE = 860,                 /* RELEASE  */
    RELOPTIONS = 861,              /* RELOPTIONS  */
    REMOTE_P = 862,                /* REMOTE_P  */
    REMOVE = 863,                  /* REMOVE  */
    RENAME = 864,                  /* RENAME  */
    REPEAT = 865,                  /* REPEAT  */
    REPEATABLE = 866,              /* REPEATABLE  */
    REPLACE = 867,                 /* REPLACE  */
    REPLICA = 868,                 /* REPLICA  */
    RESET = 869,                   /* RESET  */
    RESIZE = 870,                  /* RESIZE  */
    RESOURCE = 871,                /* RESOURCE  */
    RESPECT_P = 872,               /* RESPECT_P  */
    RESTART = 873,                 /* RESTART  */
    RESTRICT = 874,                /* RESTRICT  */
    RESULT_CACHE = 875,            /* RESULT_CACHE  */
    RETURN = 876,                  /* RETURN  */
    RETURNED_SQLSTATE = 877,       /* RETURNED_SQLSTATE  */
    RETURNING = 878,               /* RETURNING  */
    RETURNS = 879,                 /* RETURNS  */
    REUSE = 880,                   /* REUSE  */
    REVOKE = 881,                  /* REVOKE  */
    RIGHT = 882,                   /* RIGHT  */
    ROLE = 883,                    /* ROLE  */
    ROLES = 884,                   /* ROLES  */
    ROLLBACK = 885,                /* ROLLBACK  */
    ROLLUP = 886,                  /* ROLLUP  */
    ROTATE = 887,                  /* ROTATE  */
    ROTATION = 888,                /* ROTATION  */
    ROW = 889,                     /* ROW  */
    ROW_COUNT = 890,               /* ROW_COUNT  */
    ROWNUM = 891,                  /* ROWNUM  */
    ROWS = 892,                    /* ROWS  */
    ROWTYPE_P = 893,               /* ROWTYPE_P  */
    RULE = 894,                    /* RULE  */
    SAMPLE = 895,                  /* SAMPLE  */
    SAVEPOINT = 896,               /* SAVEPOINT  */
    SCHEDULE = 897,                /* SCHEDULE  */
    SCHEMA = 898,                  /* SCHEMA  */
    SCHEMA_NAME = 899,             /* SCHEMA_NAME  */
    SCROLL = 900,                  /* SCROLL  */
    SEARCH = 901,                  /* SEARCH  */
    SECOND_P = 902,                /* SECOND_P  */
    SECURITY = 903,                /* SECURITY  */
    SELECT = 904,                  /* SELECT  */
    SEPARATOR_P = 905,             /* SEPARATOR_P  */
    SEQUENCE = 906,                /* SEQUENCE  */
    SEQUENCES = 907,               /* SEQUENCES  */
    SHARE_MEMORY = 908,            /* SHARE_MEMORY  */
    SERIALIZABLE = 909,            /* SERIALIZABLE  */
    SERVER = 910,                  /* SERVER  */
    SESSION = 911,                 /* SESSION  */
    SESSION_USER = 912,            /* SESSION_USER  */
    SET = 913,                     /* SET  */
    SETS = 914,                    /* SETS  */
    SETOF = 915,                   /* SETOF  */
    SHARE = 916,                   /* SHARE  */
    SHIPPABLE = 917,               /* SHIPPABLE  */
    SHOW = 918,                    /* SHOW  */
    SHUTDOWN = 919,                /* SHUTDOWN  */
    SIBLINGS = 920,                /* SIBLINGS  */
    SIMILAR = 921,                 /* SIMILAR  */
    SIMPLE = 922,                  /* SIMPLE  */
    SIZE = 923,                    /* SIZE  */
    SKIP = 924,                    /* SKIP  */
    SLAVE = 925,                   /* SLAVE  */
    SLICE = 926,                   /* SLICE  */
    SMALLDATETIME = 927,           /* SMALLDATETIME  */
    SMALLDATETIME_FORMAT_P = 928,  /* SMALLDATETIME_FORMAT_P  */
    SMALLINT = 929,                /* SMALLINT  */
    SNAPSHOT = 930,                /* SNAPSHOT  */
    SOME = 931,                    /* SOME  */
    SOURCE_P = 932,                /* SOURCE_P  */
    SPACE = 933,                   /* SPACE  */
    SPECIFICATION = 934,           /* SPECIFICATION  */
    SPILL = 935,                   /* SPILL  */
    SPLIT = 936,                   /* SPLIT  */
    STABLE = 937,                  /* STABLE  */
    STACKED_P = 938,               /* STACKED_P  */
    STANDALONE_P = 939,            /* STANDALONE_P  */
    START = 940,                   /* START  */
    STARTS = 941,                  /* STARTS  */
    STARTWITH = 942,               /* STARTWITH  */
    STATEMENT = 943,               /* STATEMENT  */
    STATEMENT_ID = 944,            /* STATEMENT_ID  */
    STATISTICS = 945,              /* STATISTICS  */
    STDIN = 946,                   /* STDIN  */
    STDOUT = 947,                  /* STDOUT  */
    STORAGE = 948,                 /* STORAGE  */
    STORE_P = 949,                 /* STORE_P  */
    STORED = 950,                  /* STORED  */
    STRATIFY = 951,                /* STRATIFY  */
    STREAM = 952,                  /* STREAM  */
    STRICT_P = 953,                /* STRICT_P  */
    STRIP_P = 954,                 /* STRIP_P  */
    SUBCLASS_ORIGIN = 955,         /* SUBCLASS_ORIGIN  */
    SUBPARTITION = 956,            /* SUBPARTITION  */
    SUBPARTITIONS = 957,           /* SUBPARTITIONS  */
    SUBSCRIPTION = 958,            /* SUBSCRIPTION  */
    SUBSTRING = 959,               /* SUBSTRING  */
    SYMMETRIC = 960,               /* SYMMETRIC  */
    SYNONYM = 961,                 /* SYNONYM  */
    SYSDATE = 962,                 /* SYSDATE  */
    SYSID = 963,                   /* SYSID  */
    SYSTEM_P = 964,                /* SYSTEM_P  */
    SYS_REFCURSOR = 965,           /* SYS_REFCURSOR  */
    STARTING = 966,                /* STARTING  */
    SQL_P = 967,                   /* SQL_P  */
    TABLE = 968,                   /* TABLE  */
    TABLE_NAME = 969,              /* TABLE_NAME  */
    TABLES = 970,                  /* TABLES  */
    TABLESAMPLE = 971,             /* TABLESAMPLE  */
    TABLESPACE = 972,              /* TABLESPACE  */
    TARGET = 973,                  /* TARGET  */
    TEMP = 974,                    /* TEMP  */
    TEMPLATE = 975,                /* TEMPLATE  */
    TEMPORARY = 976,               /* TEMPORARY  */
    TERMINATED = 977,              /* TERMINATED  */
    TEXT_P = 978,                  /* TEXT_P  */
    THAN = 979,                    /* THAN  */
    THEN = 980,                    /* THEN  */
    TIME = 981,                    /* TIME  */
    TIME_FORMAT_P = 982,           /* TIME_FORMAT_P  */
    TIES = 983,                    /* TIES  */
    TIMECAPSULE = 984,             /* TIMECAPSULE  */
    TIMESTAMP = 985,               /* TIMESTAMP  */
    TIMESTAMP_FORMAT_P = 986,      /* TIMESTAMP_FORMAT_P  */
    TIMESTAMPDIFF = 987,           /* TIMESTAMPDIFF  */
    TIMEZONE_HOUR_P = 988,         /* TIMEZONE_HOUR_P  */
    TIMEZONE_MINUTE_P = 989,       /* TIMEZONE_MINUTE_P  */
    TINYINT = 990,                 /* TINYINT  */
    TO = 991,                      /* TO  */
    TRAILING = 992,                /* TRAILING  */
    TRANSACTION = 993,             /* TRANSACTION  */
    TRANSFORM = 994,               /* TRANSFORM  */
    TREAT = 995,                   /* TREAT  */
    TRIGGER = 996,                 /* TRIGGER  */
    TRIM = 997,                    /* TRIM  */
    TRUE_P = 998,                  /* TRUE_P  */
    TRUNCATE = 999,                /* TRUNCATE  */
    TRUSTED = 1000,                /* TRUSTED  */
    TSFIELD = 1001,                /* TSFIELD  */
    TSTAG = 1002,                  /* TSTAG  */
    TSTIME = 1003,                 /* TSTIME  */
    TYPE_P = 1004,                 /* TYPE_P  */
    TYPES_P = 1005,                /* TYPES_P  */
    UNBOUNDED = 1006,              /* UNBOUNDED  */
    UNCOMMITTED = 1007,            /* UNCOMMITTED  */
    UNENCRYPTED = 1008,            /* UNENCRYPTED  */
    UNION = 1009,                  /* UNION  */
    UNIQUE = 1010,                 /* UNIQUE  */
    UNKNOWN = 1011,                /* UNKNOWN  */
    UNLIMITED = 1012,              /* UNLIMITED  */
    UNLISTEN = 1013,               /* UNLISTEN  */
    UNLOCK = 1014,                 /* UNLOCK  */
    UNLOGGED = 1015,               /* UNLOGGED  */
    UNIMCSTORED = 1016,            /* UNIMCSTORED  */
    UNTIL = 1017,                  /* UNTIL  */
    UNUSABLE = 1018,               /* UNUSABLE  */
    UPDATE = 1019,                 /* UPDATE  */
    USEEOF = 1020,                 /* USEEOF  */
    USER = 1021,                   /* USER  */
    USING = 1022,                  /* USING  */
    VACUUM = 1023,                 /* VACUUM  */
    VALID = 1024,                  /* VALID  */
    VALIDATE = 1025,               /* VALIDATE  */
    VALIDATION = 1026,             /* VALIDATION  */
    VALIDATOR = 1027,              /* VALIDATOR  */
    VALUE_P = 1028,                /* VALUE_P  */
    VALUES = 1029,                 /* VALUES  */
    VARCHAR = 1030,                /* VARCHAR  */
    VARCHAR2 = 1031,               /* VARCHAR2  */
    VARIABLES = 1032,              /* VARIABLES  */
    VARIADIC = 1033,               /* VARIADIC  */
    VARRAY = 1034,                 /* VARRAY  */
    VARYING = 1035,                /* VARYING  */
    VCGROUP = 1036,                /* VCGROUP  */
    VERBOSE = 1037,                /* VERBOSE  */
    VERIFY = 1038,                 /* VERIFY  */
    VERSION_P = 1039,              /* VERSION_P  */
    VIEW = 1040,                   /* VIEW  */
    VISIBLE = 1041,                /* VISIBLE  */
    VOLATILE = 1042,               /* VOLATILE  */
    WAIT = 1043,                   /* WAIT  */
    WARNINGS = 1044,               /* WARNINGS  */
    WEAK = 1045,                   /* WEAK  */
    WHEN = 1046,                   /* WHEN  */
    WHERE = 1047,                  /* WHERE  */
    WHILE_P = 1048,                /* WHILE_P  */
    WHITESPACE_P = 1049,           /* WHITESPACE_P  */
    WINDOW = 1050,                 /* WINDOW  */
    WITH = 1051,                   /* WITH  */
    WITHIN = 1052,                 /* WITHIN  */
    WITHOUT = 1053,                /* WITHOUT  */
    WORK = 1054,                   /* WORK  */
    WORKLOAD = 1055,               /* WORKLOAD  */
    WRAPPER = 1056,                /* WRAPPER  */
    WRITE = 1057,                  /* WRITE  */
    XML_P = 1058,                  /* XML_P  */
    XMLATTRIBUTES = 1059,          /* XMLATTRIBUTES  */
    XMLCONCAT = 1060,              /* XMLCONCAT  */
    XMLELEMENT = 1061,             /* XMLELEMENT  */
    XMLEXISTS = 1062,              /* XMLEXISTS  */
    XMLFOREST = 1063,              /* XMLFOREST  */
    XMLNAMESPACES = 1064,          /* XMLNAMESPACES  */
    XMLPARSE = 1065,               /* XMLPARSE  */
    XMLPI = 1066,                  /* XMLPI  */
    XMLROOT = 1067,                /* XMLROOT  */
    XMLSERIALIZE = 1068,           /* XMLSERIALIZE  */
    XMLTABLE = 1069,               /* XMLTABLE  */
    YEAR_P = 1070,                 /* YEAR_P  */
    YEAR_MONTH_P = 1071,           /* YEAR_MONTH_P  */
    YES_P = 1072,                  /* YES_P  */
    ZONE = 1073,                   /* ZONE  */
    NULLS_FIRST = 1074,            /* NULLS_FIRST  */
    NULLS_LAST = 1075,             /* NULLS_LAST  */
    WITH_ORDINALITY = 1076,        /* WITH_ORDINALITY  */
    WITH_TIME = 1077,              /* WITH_TIME  */
    INCLUDING_ALL = 1078,          /* INCLUDING_ALL  */
    RENAME_PARTITION = 1079,       /* RENAME_PARTITION  */
    PARTITION_FOR = 1080,          /* PARTITION_FOR  */
    SUBPARTITION_FOR = 1081,       /* SUBPARTITION_FOR  */
    ADD_PARTITION = 1082,          /* ADD_PARTITION  */
    DROP_PARTITION = 1083,         /* DROP_PARTITION  */
    REBUILD_PARTITION = 1084,      /* REBUILD_PARTITION  */
    MODIFY_PARTITION = 1085,       /* MODIFY_PARTITION  */
    ADD_SUBPARTITION = 1086,       /* ADD_SUBPARTITION  */
    DROP_SUBPARTITION = 1087,      /* DROP_SUBPARTITION  */
    NOT_ENFORCED = 1088,           /* NOT_ENFORCED  */
    VALID_BEGIN = 1089,            /* VALID_BEGIN  */
    DECLARE_CURSOR = 1090,         /* DECLARE_CURSOR  */
    ON_UPDATE_TIME = 1091,         /* ON_UPDATE_TIME  */
    START_WITH = 1092,             /* START_WITH  */
    CONNECT_BY = 1093,             /* CONNECT_BY  */
    SHOW_ERRORS = 1094,            /* SHOW_ERRORS  */
    END_OF_INPUT = 1095,           /* END_OF_INPUT  */
    END_OF_INPUT_COLON = 1096,     /* END_OF_INPUT_COLON  */
    END_OF_PROC = 1097,            /* END_OF_PROC  */
    EVENT_TRIGGER = 1098,          /* EVENT_TRIGGER  */
    NOT_IN = 1099,                 /* NOT_IN  */
    NOT_BETWEEN = 1100,            /* NOT_BETWEEN  */
    NOT_LIKE = 1101,               /* NOT_LIKE  */
    NOT_ILIKE = 1102,              /* NOT_ILIKE  */
    NOT_SIMILAR = 1103,            /* NOT_SIMILAR  */
    FORCE_INDEX = 1104,            /* FORCE_INDEX  */
    USE_INDEX = 1105,              /* USE_INDEX  */
    IGNORE_INDEX = 1106,           /* IGNORE_INDEX  */
    CURSOR_EXPR = 1107,            /* CURSOR_EXPR  */
    LATERAL_EXPR = 1108,           /* LATERAL_EXPR  */
    FALSE_ON_ERROR = 1109,         /* FALSE_ON_ERROR  */
    TRUE_ON_ERROR = 1110,          /* TRUE_ON_ERROR  */
    ERROR_ON_ERROR = 1111,         /* ERROR_ON_ERROR  */
    PARTIAL_EMPTY_PREC = 1112,     /* PARTIAL_EMPTY_PREC  */
    POSTFIXOP = 1113,              /* POSTFIXOP  */
    lower_than_index = 1114,       /* lower_than_index  */
    lower_than_under = 1115,       /* lower_than_under  */
    UNDER = 1116,                  /* UNDER  */
    higher_than_rotate = 1117,     /* higher_than_rotate  */
    UMINUS = 1118,                 /* UMINUS  */
    EMPTY_FROM_CLAUSE = 1119,      /* EMPTY_FROM_CLAUSE  */
    CONSTRUCTOR = 1120,            /* CONSTRUCTOR  */
    FINAL = 1121,                  /* FINAL  */
    MAP = 1122,                    /* MAP  */
    MEMBER = 1123,                 /* MEMBER  */
    RESULT = 1124,                 /* RESULT  */
    SELF = 1125,                   /* SELF  */
    STATIC_P = 1126,               /* STATIC_P  */
    SELF_INOUT = 1127,             /* SELF_INOUT  */
    STATIC_FUNCTION = 1128,        /* STATIC_FUNCTION  */
    MEMBER_FUNCTION = 1129,        /* MEMBER_FUNCTION  */
    STATIC_PROCEDURE = 1130,       /* STATIC_PROCEDURE  */
    MEMBER_PROCEDURE = 1131,       /* MEMBER_PROCEDURE  */
    CONSTRUCTOR_FUNCTION = 1132,   /* CONSTRUCTOR_FUNCTION  */
    MAP_MEMBER = 1133              /* MAP_MEMBER  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 564 "preproc.y"

	double	dval;
	char	*str;
	int		ival;
	struct	when		action;
	struct	index		index;
	int		tagname;
	struct	this_type	type;
	enum	ECPGttype	type_enum;
	enum	ECPGdtype	dtype_enum;
	struct	fetch_desc	descriptor;
	struct  su_symbol	struct_union;
	struct	prep		prep;

#line 957 "preproc.hpp"

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


extern YYSTYPE base_yylval;
extern YYLTYPE base_yylloc;
int base_yyparse (void);

#endif /* !YY_BASE_YY_PREPROC_HPP_INCLUDED  */
