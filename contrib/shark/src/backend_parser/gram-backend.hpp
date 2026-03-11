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

#ifndef YY_PGTSQL_BASE_YY_SRC_BACKEND_PARSER_GRAM_BACKEND_HPP_INCLUDED
# define YY_PGTSQL_BASE_YY_SRC_BACKEND_PARSER_GRAM_BACKEND_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int pgtsql_base_yydebug;
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
    DIALECT_TSQL = 282,            /* DIALECT_TSQL  */
    TSQL_XCONST = 283,             /* TSQL_XCONST  */
    ABORT_P = 284,                 /* ABORT_P  */
    ABSOLUTE_P = 285,              /* ABSOLUTE_P  */
    ACCESS = 286,                  /* ACCESS  */
    ACCOUNT = 287,                 /* ACCOUNT  */
    ACTION = 288,                  /* ACTION  */
    ADD_P = 289,                   /* ADD_P  */
    ADMIN = 290,                   /* ADMIN  */
    AFTER = 291,                   /* AFTER  */
    AGGREGATE = 292,               /* AGGREGATE  */
    ALGORITHM = 293,               /* ALGORITHM  */
    ALL = 294,                     /* ALL  */
    ALSO = 295,                    /* ALSO  */
    ALTER = 296,                   /* ALTER  */
    ALWAYS = 297,                  /* ALWAYS  */
    ANALYSE = 298,                 /* ANALYSE  */
    ANALYZE = 299,                 /* ANALYZE  */
    AND = 300,                     /* AND  */
    ANY = 301,                     /* ANY  */
    APP = 302,                     /* APP  */
    APPEND = 303,                  /* APPEND  */
    APPLY = 304,                   /* APPLY  */
    ARCHIVE = 305,                 /* ARCHIVE  */
    ARRAY = 306,                   /* ARRAY  */
    AS = 307,                      /* AS  */
    ASC = 308,                     /* ASC  */
    ASOF_P = 309,                  /* ASOF_P  */
    ASSERTION = 310,               /* ASSERTION  */
    ASSIGNMENT = 311,              /* ASSIGNMENT  */
    ASYMMETRIC = 312,              /* ASYMMETRIC  */
    AT = 313,                      /* AT  */
    ATTRIBUTE = 314,               /* ATTRIBUTE  */
    AUDIT = 315,                   /* AUDIT  */
    AUTHID = 316,                  /* AUTHID  */
    AUTHORIZATION = 317,           /* AUTHORIZATION  */
    AUTOEXTEND = 318,              /* AUTOEXTEND  */
    AUTOMAPPED = 319,              /* AUTOMAPPED  */
    AUTO_INCREMENT = 320,          /* AUTO_INCREMENT  */
    BACKWARD = 321,                /* BACKWARD  */
    BARRIER = 322,                 /* BARRIER  */
    BEFORE = 323,                  /* BEFORE  */
    BEGIN_NON_ANOYBLOCK = 324,     /* BEGIN_NON_ANOYBLOCK  */
    BEGIN_P = 325,                 /* BEGIN_P  */
    BETWEEN = 326,                 /* BETWEEN  */
    BIGINT = 327,                  /* BIGINT  */
    BINARY = 328,                  /* BINARY  */
    BINARY_DOUBLE = 329,           /* BINARY_DOUBLE  */
    BINARY_DOUBLE_INF = 330,       /* BINARY_DOUBLE_INF  */
    BINARY_DOUBLE_NAN = 331,       /* BINARY_DOUBLE_NAN  */
    BINARY_INTEGER = 332,          /* BINARY_INTEGER  */
    BIT = 333,                     /* BIT  */
    BLANKS = 334,                  /* BLANKS  */
    BLOB_P = 335,                  /* BLOB_P  */
    BLOCKCHAIN = 336,              /* BLOCKCHAIN  */
    BODY_P = 337,                  /* BODY_P  */
    BOGUS = 338,                   /* BOGUS  */
    BOOLEAN_P = 339,               /* BOOLEAN_P  */
    BOTH = 340,                    /* BOTH  */
    BUCKETCNT = 341,               /* BUCKETCNT  */
    BUCKETS = 342,                 /* BUCKETS  */
    BUILD = 343,                   /* BUILD  */
    BY = 344,                      /* BY  */
    BYTE_P = 345,                  /* BYTE_P  */
    BYTEAWITHOUTORDER = 346,       /* BYTEAWITHOUTORDER  */
    BYTEAWITHOUTORDERWITHEQUAL = 347, /* BYTEAWITHOUTORDERWITHEQUAL  */
    CACHE = 348,                   /* CACHE  */
    CALL = 349,                    /* CALL  */
    CALLED = 350,                  /* CALLED  */
    CANCELABLE = 351,              /* CANCELABLE  */
    CASCADE = 352,                 /* CASCADE  */
    CASCADED = 353,                /* CASCADED  */
    CASE = 354,                    /* CASE  */
    CAST = 355,                    /* CAST  */
    CATALOG_P = 356,               /* CATALOG_P  */
    CATALOG_NAME = 357,            /* CATALOG_NAME  */
    CHAIN = 358,                   /* CHAIN  */
    CHANGE = 359,                  /* CHANGE  */
    CHAR_P = 360,                  /* CHAR_P  */
    CHARACTER = 361,               /* CHARACTER  */
    CHARACTERISTICS = 362,         /* CHARACTERISTICS  */
    CHARACTERSET = 363,            /* CHARACTERSET  */
    CHARSET = 364,                 /* CHARSET  */
    CHECK = 365,                   /* CHECK  */
    CHECKPOINT = 366,              /* CHECKPOINT  */
    CLASS = 367,                   /* CLASS  */
    CLASS_ORIGIN = 368,            /* CLASS_ORIGIN  */
    CLEAN = 369,                   /* CLEAN  */
    CLIENT = 370,                  /* CLIENT  */
    CLIENT_MASTER_KEY = 371,       /* CLIENT_MASTER_KEY  */
    CLIENT_MASTER_KEYS = 372,      /* CLIENT_MASTER_KEYS  */
    CLOB = 373,                    /* CLOB  */
    CLOSE = 374,                   /* CLOSE  */
    CLUSTER = 375,                 /* CLUSTER  */
    COALESCE = 376,                /* COALESCE  */
    COLLATE = 377,                 /* COLLATE  */
    COLLATION = 378,               /* COLLATION  */
    COLUMN = 379,                  /* COLUMN  */
    COLUMN_ENCRYPTION_KEY = 380,   /* COLUMN_ENCRYPTION_KEY  */
    COLUMN_ENCRYPTION_KEYS = 381,  /* COLUMN_ENCRYPTION_KEYS  */
    COLUMN_NAME = 382,             /* COLUMN_NAME  */
    COLUMNS = 383,                 /* COLUMNS  */
    COMMENT = 384,                 /* COMMENT  */
    COMMENTS = 385,                /* COMMENTS  */
    COMMIT = 386,                  /* COMMIT  */
    COMMITTED = 387,               /* COMMITTED  */
    COMPACT = 388,                 /* COMPACT  */
    COMPATIBLE_ILLEGAL_CHARS = 389, /* COMPATIBLE_ILLEGAL_CHARS  */
    COMPILE = 390,                 /* COMPILE  */
    COMPLETE = 391,                /* COMPLETE  */
    COMPLETION = 392,              /* COMPLETION  */
    COMPRESS = 393,                /* COMPRESS  */
    CONCURRENTLY = 394,            /* CONCURRENTLY  */
    CONDITION = 395,               /* CONDITION  */
    CONFIGURATION = 396,           /* CONFIGURATION  */
    CONNECTION = 397,              /* CONNECTION  */
    CONSISTENT = 398,              /* CONSISTENT  */
    CONSTANT = 399,                /* CONSTANT  */
    CONSTRAINT = 400,              /* CONSTRAINT  */
    CONSTRAINT_CATALOG = 401,      /* CONSTRAINT_CATALOG  */
    CONSTRAINT_NAME = 402,         /* CONSTRAINT_NAME  */
    CONSTRAINT_SCHEMA = 403,       /* CONSTRAINT_SCHEMA  */
    CONSTRAINTS = 404,             /* CONSTRAINTS  */
    CONTENT_P = 405,               /* CONTENT_P  */
    CONTINUE_P = 406,              /* CONTINUE_P  */
    CONTVIEW = 407,                /* CONTVIEW  */
    CONVERSION_P = 408,            /* CONVERSION_P  */
    CONVERT_P = 409,               /* CONVERT_P  */
    CONNECT = 410,                 /* CONNECT  */
    COORDINATOR = 411,             /* COORDINATOR  */
    COORDINATORS = 412,            /* COORDINATORS  */
    COPY = 413,                    /* COPY  */
    COST = 414,                    /* COST  */
    CREATE = 415,                  /* CREATE  */
    CROSS = 416,                   /* CROSS  */
    CSN = 417,                     /* CSN  */
    CSV = 418,                     /* CSV  */
    CUBE = 419,                    /* CUBE  */
    CURRENT_P = 420,               /* CURRENT_P  */
    CURRENT_CATALOG = 421,         /* CURRENT_CATALOG  */
    CURRENT_DATE = 422,            /* CURRENT_DATE  */
    CURRENT_ROLE = 423,            /* CURRENT_ROLE  */
    CURRENT_SCHEMA = 424,          /* CURRENT_SCHEMA  */
    CURRENT_TIME = 425,            /* CURRENT_TIME  */
    CURRENT_TIMESTAMP = 426,       /* CURRENT_TIMESTAMP  */
    CURRENT_USER = 427,            /* CURRENT_USER  */
    CURSOR = 428,                  /* CURSOR  */
    CURSOR_NAME = 429,             /* CURSOR_NAME  */
    CYCLE = 430,                   /* CYCLE  */
    SHRINK = 431,                  /* SHRINK  */
    USE_P = 432,                   /* USE_P  */
    DATA_P = 433,                  /* DATA_P  */
    DATABASE = 434,                /* DATABASE  */
    DATAFILE = 435,                /* DATAFILE  */
    DATANODE = 436,                /* DATANODE  */
    DATANODES = 437,               /* DATANODES  */
    DATATYPE_CL = 438,             /* DATATYPE_CL  */
    DATE_P = 439,                  /* DATE_P  */
    DATE_FORMAT_P = 440,           /* DATE_FORMAT_P  */
    DAY_P = 441,                   /* DAY_P  */
    DAY_HOUR_P = 442,              /* DAY_HOUR_P  */
    DAY_MINUTE_P = 443,            /* DAY_MINUTE_P  */
    DAY_SECOND_P = 444,            /* DAY_SECOND_P  */
    DBCOMPATIBILITY_P = 445,       /* DBCOMPATIBILITY_P  */
    DEALLOCATE = 446,              /* DEALLOCATE  */
    DEC = 447,                     /* DEC  */
    DECIMAL_P = 448,               /* DECIMAL_P  */
    DECLARE = 449,                 /* DECLARE  */
    DECODE = 450,                  /* DECODE  */
    DEFAULT = 451,                 /* DEFAULT  */
    DEFAULTS = 452,                /* DEFAULTS  */
    DEFERRABLE = 453,              /* DEFERRABLE  */
    DEFERRED = 454,                /* DEFERRED  */
    DEFINER = 455,                 /* DEFINER  */
    DELETE_P = 456,                /* DELETE_P  */
    DELIMITER = 457,               /* DELIMITER  */
    DELIMITERS = 458,              /* DELIMITERS  */
    DELTA = 459,                   /* DELTA  */
    DELTAMERGE = 460,              /* DELTAMERGE  */
    DENSE_RANK = 461,              /* DENSE_RANK  */
    DESC = 462,                    /* DESC  */
    DETERMINISTIC = 463,           /* DETERMINISTIC  */
    DIAGNOSTICS = 464,             /* DIAGNOSTICS  */
    DICTIONARY = 465,              /* DICTIONARY  */
    DIRECT = 466,                  /* DIRECT  */
    DIRECTORY = 467,               /* DIRECTORY  */
    DISABLE_P = 468,               /* DISABLE_P  */
    DISCARD = 469,                 /* DISCARD  */
    DISTINCT = 470,                /* DISTINCT  */
    DISTRIBUTE = 471,              /* DISTRIBUTE  */
    DISTRIBUTION = 472,            /* DISTRIBUTION  */
    DO = 473,                      /* DO  */
    DOCUMENT_P = 474,              /* DOCUMENT_P  */
    DOMAIN_P = 475,                /* DOMAIN_P  */
    DOUBLE_P = 476,                /* DOUBLE_P  */
    DROP = 477,                    /* DROP  */
    DUPLICATE = 478,               /* DUPLICATE  */
    DISCONNECT = 479,              /* DISCONNECT  */
    DUMPFILE = 480,                /* DUMPFILE  */
    EACH = 481,                    /* EACH  */
    ELASTIC = 482,                 /* ELASTIC  */
    ELSE = 483,                    /* ELSE  */
    ENABLE_P = 484,                /* ENABLE_P  */
    ENCLOSED = 485,                /* ENCLOSED  */
    ENCODING = 486,                /* ENCODING  */
    ENCRYPTED = 487,               /* ENCRYPTED  */
    ENCRYPTED_VALUE = 488,         /* ENCRYPTED_VALUE  */
    ENCRYPTION = 489,              /* ENCRYPTION  */
    ENCRYPTION_TYPE = 490,         /* ENCRYPTION_TYPE  */
    END_P = 491,                   /* END_P  */
    ENDS = 492,                    /* ENDS  */
    ENFORCED = 493,                /* ENFORCED  */
    ENUM_P = 494,                  /* ENUM_P  */
    ERROR_P = 495,                 /* ERROR_P  */
    ERRORS = 496,                  /* ERRORS  */
    ESCAPE = 497,                  /* ESCAPE  */
    EOL = 498,                     /* EOL  */
    ESCAPING = 499,                /* ESCAPING  */
    EVENT = 500,                   /* EVENT  */
    EVENTS = 501,                  /* EVENTS  */
    EVERY = 502,                   /* EVERY  */
    EXCEPT = 503,                  /* EXCEPT  */
    EXCHANGE = 504,                /* EXCHANGE  */
    EXCLUDE = 505,                 /* EXCLUDE  */
    EXCLUDED = 506,                /* EXCLUDED  */
    EXCLUDING = 507,               /* EXCLUDING  */
    EXCLUSIVE = 508,               /* EXCLUSIVE  */
    EXECUTE = 509,                 /* EXECUTE  */
    EXISTS = 510,                  /* EXISTS  */
    EXPIRED_P = 511,               /* EXPIRED_P  */
    EXPLAIN = 512,                 /* EXPLAIN  */
    EXTENSION = 513,               /* EXTENSION  */
    EXTERNAL = 514,                /* EXTERNAL  */
    EXTRACT = 515,                 /* EXTRACT  */
    ESCAPED = 516,                 /* ESCAPED  */
    FALSE_P = 517,                 /* FALSE_P  */
    FAMILY = 518,                  /* FAMILY  */
    FAST = 519,                    /* FAST  */
    FENCED = 520,                  /* FENCED  */
    FETCH = 521,                   /* FETCH  */
    FIELDS = 522,                  /* FIELDS  */
    FILEHEADER_P = 523,            /* FILEHEADER_P  */
    FILL_MISSING_FIELDS = 524,     /* FILL_MISSING_FIELDS  */
    FILLER = 525,                  /* FILLER  */
    FILTER = 526,                  /* FILTER  */
    FIRST_P = 527,                 /* FIRST_P  */
    FIXED_P = 528,                 /* FIXED_P  */
    FLOAT_P = 529,                 /* FLOAT_P  */
    FOLLOWING = 530,               /* FOLLOWING  */
    FOLLOWS_P = 531,               /* FOLLOWS_P  */
    FOR = 532,                     /* FOR  */
    FORCE = 533,                   /* FORCE  */
    FOREIGN = 534,                 /* FOREIGN  */
    FORMATTER = 535,               /* FORMATTER  */
    FORWARD = 536,                 /* FORWARD  */
    FEATURES = 537,                /* FEATURES  */
    FREEZE = 538,                  /* FREEZE  */
    FROM = 539,                    /* FROM  */
    FULL = 540,                    /* FULL  */
    FUNCTION = 541,                /* FUNCTION  */
    FUNCTIONS = 542,               /* FUNCTIONS  */
    GENERATED = 543,               /* GENERATED  */
    GET = 544,                     /* GET  */
    GLOBAL = 545,                  /* GLOBAL  */
    GRANT = 546,                   /* GRANT  */
    GRANTED = 547,                 /* GRANTED  */
    GREATEST = 548,                /* GREATEST  */
    GROUP_P = 549,                 /* GROUP_P  */
    GROUPING_P = 550,              /* GROUPING_P  */
    GROUPPARENT = 551,             /* GROUPPARENT  */
    HANDLER = 552,                 /* HANDLER  */
    HAVING = 553,                  /* HAVING  */
    HDFSDIRECTORY = 554,           /* HDFSDIRECTORY  */
    HEADER_P = 555,                /* HEADER_P  */
    HOLD = 556,                    /* HOLD  */
    HOUR_P = 557,                  /* HOUR_P  */
    HOUR_MINUTE_P = 558,           /* HOUR_MINUTE_P  */
    HOUR_SECOND_P = 559,           /* HOUR_SECOND_P  */
    IDENTIFIED = 560,              /* IDENTIFIED  */
    IDENTITY_P = 561,              /* IDENTITY_P  */
    IF_P = 562,                    /* IF_P  */
    IGNORE = 563,                  /* IGNORE  */
    IGNORE_EXTRA_DATA = 564,       /* IGNORE_EXTRA_DATA  */
    ILIKE = 565,                   /* ILIKE  */
    IMMEDIATE = 566,               /* IMMEDIATE  */
    IMMUTABLE = 567,               /* IMMUTABLE  */
    IMPLICIT_P = 568,              /* IMPLICIT_P  */
    IN_P = 569,                    /* IN_P  */
    INCLUDE = 570,                 /* INCLUDE  */
    IMCSTORED = 571,               /* IMCSTORED  */
    INCLUDING = 572,               /* INCLUDING  */
    INCREMENT = 573,               /* INCREMENT  */
    INCREMENTAL = 574,             /* INCREMENTAL  */
    INDEX = 575,                   /* INDEX  */
    INDEXES = 576,                 /* INDEXES  */
    INFILE = 577,                  /* INFILE  */
    INFINITE_P = 578,              /* INFINITE_P  */
    INHERIT = 579,                 /* INHERIT  */
    INHERITS = 580,                /* INHERITS  */
    INITIAL_P = 581,               /* INITIAL_P  */
    INITIALLY = 582,               /* INITIALLY  */
    INITRANS = 583,                /* INITRANS  */
    INLINE_P = 584,                /* INLINE_P  */
    INNER_P = 585,                 /* INNER_P  */
    INOUT = 586,                   /* INOUT  */
    INPUT_P = 587,                 /* INPUT_P  */
    INSENSITIVE = 588,             /* INSENSITIVE  */
    INSERT = 589,                  /* INSERT  */
    INSTEAD = 590,                 /* INSTEAD  */
    INT_P = 591,                   /* INT_P  */
    INTEGER = 592,                 /* INTEGER  */
    INTERNAL = 593,                /* INTERNAL  */
    INTERSECT = 594,               /* INTERSECT  */
    INTERVAL = 595,                /* INTERVAL  */
    INTO = 596,                    /* INTO  */
    INVISIBLE = 597,               /* INVISIBLE  */
    INVOKER = 598,                 /* INVOKER  */
    IP = 599,                      /* IP  */
    IS = 600,                      /* IS  */
    ISNULL = 601,                  /* ISNULL  */
    ISOLATION = 602,               /* ISOLATION  */
    JOIN = 603,                    /* JOIN  */
    JSON_EXISTS = 604,             /* JSON_EXISTS  */
    KEEP = 605,                    /* KEEP  */
    KEY = 606,                     /* KEY  */
    KILL = 607,                    /* KILL  */
    KEY_PATH = 608,                /* KEY_PATH  */
    KEY_STORE = 609,               /* KEY_STORE  */
    LABEL = 610,                   /* LABEL  */
    LANGUAGE = 611,                /* LANGUAGE  */
    LARGE_P = 612,                 /* LARGE_P  */
    LAST_P = 613,                  /* LAST_P  */
    LATERAL_P = 614,               /* LATERAL_P  */
    LC_COLLATE_P = 615,            /* LC_COLLATE_P  */
    LC_CTYPE_P = 616,              /* LC_CTYPE_P  */
    LEADING = 617,                 /* LEADING  */
    LEAKPROOF = 618,               /* LEAKPROOF  */
    LINES = 619,                   /* LINES  */
    LEAST = 620,                   /* LEAST  */
    LESS = 621,                    /* LESS  */
    LEFT = 622,                    /* LEFT  */
    LEVEL = 623,                   /* LEVEL  */
    LIKE = 624,                    /* LIKE  */
    LIMIT = 625,                   /* LIMIT  */
    LIST = 626,                    /* LIST  */
    LISTEN = 627,                  /* LISTEN  */
    LOAD = 628,                    /* LOAD  */
    LOCAL = 629,                   /* LOCAL  */
    LOCALTIME = 630,               /* LOCALTIME  */
    LOCALTIMESTAMP = 631,          /* LOCALTIMESTAMP  */
    LOCATION = 632,                /* LOCATION  */
    LOCK_P = 633,                  /* LOCK_P  */
    LOCKED = 634,                  /* LOCKED  */
    LOG_P = 635,                   /* LOG_P  */
    LOGGING = 636,                 /* LOGGING  */
    LOGIN_ANY = 637,               /* LOGIN_ANY  */
    LOGIN_FAILURE = 638,           /* LOGIN_FAILURE  */
    LOGIN_SUCCESS = 639,           /* LOGIN_SUCCESS  */
    LOGOUT = 640,                  /* LOGOUT  */
    LONG = 641,                    /* LONG  */
    LOOP = 642,                    /* LOOP  */
    MAPPING = 643,                 /* MAPPING  */
    MASKING = 644,                 /* MASKING  */
    MASTER = 645,                  /* MASTER  */
    MATCH = 646,                   /* MATCH  */
    MATERIALIZED = 647,            /* MATERIALIZED  */
    MATCHED = 648,                 /* MATCHED  */
    MAXEXTENTS = 649,              /* MAXEXTENTS  */
    MAXSIZE = 650,                 /* MAXSIZE  */
    MAXTRANS = 651,                /* MAXTRANS  */
    MAXVALUE = 652,                /* MAXVALUE  */
    MERGE = 653,                   /* MERGE  */
    MESSAGE_TEXT = 654,            /* MESSAGE_TEXT  */
    METHOD = 655,                  /* METHOD  */
    MINUS_P = 656,                 /* MINUS_P  */
    MINUTE_P = 657,                /* MINUTE_P  */
    MINUTE_SECOND_P = 658,         /* MINUTE_SECOND_P  */
    MINVALUE = 659,                /* MINVALUE  */
    MINEXTENTS = 660,              /* MINEXTENTS  */
    MODE = 661,                    /* MODE  */
    MODEL = 662,                   /* MODEL  */
    MODIFY_P = 663,                /* MODIFY_P  */
    MONTH_P = 664,                 /* MONTH_P  */
    MOVE = 665,                    /* MOVE  */
    MOVEMENT = 666,                /* MOVEMENT  */
    MYSQL_ERRNO = 667,             /* MYSQL_ERRNO  */
    NAME_P = 668,                  /* NAME_P  */
    NAMES = 669,                   /* NAMES  */
    NAN_P = 670,                   /* NAN_P  */
    NATIONAL = 671,                /* NATIONAL  */
    NATURAL = 672,                 /* NATURAL  */
    NCHAR = 673,                   /* NCHAR  */
    NEXT = 674,                    /* NEXT  */
    NO = 675,                      /* NO  */
    NOCOMPRESS = 676,              /* NOCOMPRESS  */
    NOCYCLE = 677,                 /* NOCYCLE  */
    NODE = 678,                    /* NODE  */
    NOLOGGING = 679,               /* NOLOGGING  */
    NOMAXVALUE = 680,              /* NOMAXVALUE  */
    NOMINVALUE = 681,              /* NOMINVALUE  */
    NONE = 682,                    /* NONE  */
    NOT = 683,                     /* NOT  */
    NOTHING = 684,                 /* NOTHING  */
    NOTIFY = 685,                  /* NOTIFY  */
    NOTNULL = 686,                 /* NOTNULL  */
    NOVALIDATE = 687,              /* NOVALIDATE  */
    NOWAIT = 688,                  /* NOWAIT  */
    NTH_VALUE_P = 689,             /* NTH_VALUE_P  */
    NULL_P = 690,                  /* NULL_P  */
    NULLCOLS = 691,                /* NULLCOLS  */
    NULLIF = 692,                  /* NULLIF  */
    NULLS_P = 693,                 /* NULLS_P  */
    NUMBER_P = 694,                /* NUMBER_P  */
    NUMERIC = 695,                 /* NUMERIC  */
    NUMSTR = 696,                  /* NUMSTR  */
    NVARCHAR = 697,                /* NVARCHAR  */
    NVARCHAR2 = 698,               /* NVARCHAR2  */
    NVL = 699,                     /* NVL  */
    OBJECT_P = 700,                /* OBJECT_P  */
    OF = 701,                      /* OF  */
    OFF = 702,                     /* OFF  */
    OFFSET = 703,                  /* OFFSET  */
    OIDS = 704,                    /* OIDS  */
    ON = 705,                      /* ON  */
    ONLY = 706,                    /* ONLY  */
    OPERATOR = 707,                /* OPERATOR  */
    OPTIMIZATION = 708,            /* OPTIMIZATION  */
    OPTION = 709,                  /* OPTION  */
    OPTIONALLY = 710,              /* OPTIONALLY  */
    OPTIONS = 711,                 /* OPTIONS  */
    OR = 712,                      /* OR  */
    ORDER = 713,                   /* ORDER  */
    ORDINALITY = 714,              /* ORDINALITY  */
    OUT_P = 715,                   /* OUT_P  */
    OUTER_P = 716,                 /* OUTER_P  */
    OVER = 717,                    /* OVER  */
    OVERLAPS = 718,                /* OVERLAPS  */
    OVERLAY = 719,                 /* OVERLAY  */
    OWNED = 720,                   /* OWNED  */
    OWNER = 721,                   /* OWNER  */
    OUTFILE = 722,                 /* OUTFILE  */
    PACKAGE = 723,                 /* PACKAGE  */
    PACKAGES = 724,                /* PACKAGES  */
    PARALLEL_ENABLE = 725,         /* PARALLEL_ENABLE  */
    PARSER = 726,                  /* PARSER  */
    PARTIAL = 727,                 /* PARTIAL  */
    PARTITION = 728,               /* PARTITION  */
    PARTITIONS = 729,              /* PARTITIONS  */
    PASSING = 730,                 /* PASSING  */
    PASSWORD = 731,                /* PASSWORD  */
    PCTFREE = 732,                 /* PCTFREE  */
    PER_P = 733,                   /* PER_P  */
    PERCENT = 734,                 /* PERCENT  */
    PERFORMANCE = 735,             /* PERFORMANCE  */
    PERM = 736,                    /* PERM  */
    PLACING = 737,                 /* PLACING  */
    PLAN = 738,                    /* PLAN  */
    PLANS = 739,                   /* PLANS  */
    POLICY = 740,                  /* POLICY  */
    POSITION = 741,                /* POSITION  */
    PIPELINED = 742,               /* PIPELINED  */
    POOL = 743,                    /* POOL  */
    PRECEDING = 744,               /* PRECEDING  */
    PRECISION = 745,               /* PRECISION  */
    PREDICT = 746,                 /* PREDICT  */
    PREFERRED = 747,               /* PREFERRED  */
    PREFIX = 748,                  /* PREFIX  */
    PRESERVE = 749,                /* PRESERVE  */
    PREPARE = 750,                 /* PREPARE  */
    PREPARED = 751,                /* PREPARED  */
    PRIMARY = 752,                 /* PRIMARY  */
    PRECEDES_P = 753,              /* PRECEDES_P  */
    PRIVATE = 754,                 /* PRIVATE  */
    PRIOR = 755,                   /* PRIOR  */
    PRIORER = 756,                 /* PRIORER  */
    PRIVILEGES = 757,              /* PRIVILEGES  */
    PRIVILEGE = 758,               /* PRIVILEGE  */
    PROCEDURAL = 759,              /* PROCEDURAL  */
    PROCEDURE = 760,               /* PROCEDURE  */
    PROFILE = 761,                 /* PROFILE  */
    PUBLICATION = 762,             /* PUBLICATION  */
    PUBLISH = 763,                 /* PUBLISH  */
    PURGE = 764,                   /* PURGE  */
    QUERY = 765,                   /* QUERY  */
    QUOTE = 766,                   /* QUOTE  */
    RANDOMIZED = 767,              /* RANDOMIZED  */
    RANGE = 768,                   /* RANGE  */
    RATIO = 769,                   /* RATIO  */
    RAW = 770,                     /* RAW  */
    READ = 771,                    /* READ  */
    REAL = 772,                    /* REAL  */
    REASSIGN = 773,                /* REASSIGN  */
    REBUILD = 774,                 /* REBUILD  */
    RECHECK = 775,                 /* RECHECK  */
    RECURSIVE = 776,               /* RECURSIVE  */
    RECYCLEBIN = 777,              /* RECYCLEBIN  */
    REDISANYVALUE = 778,           /* REDISANYVALUE  */
    REF = 779,                     /* REF  */
    REFERENCES = 780,              /* REFERENCES  */
    REFRESH = 781,                 /* REFRESH  */
    REINDEX = 782,                 /* REINDEX  */
    REJECT_P = 783,                /* REJECT_P  */
    RELATIVE_P = 784,              /* RELATIVE_P  */
    RELEASE = 785,                 /* RELEASE  */
    RELOPTIONS = 786,              /* RELOPTIONS  */
    REMOTE_P = 787,                /* REMOTE_P  */
    REMOVE = 788,                  /* REMOVE  */
    RENAME = 789,                  /* RENAME  */
    REPEAT = 790,                  /* REPEAT  */
    REPEATABLE = 791,              /* REPEATABLE  */
    REPLACE = 792,                 /* REPLACE  */
    REPLICA = 793,                 /* REPLICA  */
    RESET = 794,                   /* RESET  */
    RESIZE = 795,                  /* RESIZE  */
    RESOURCE = 796,                /* RESOURCE  */
    RESPECT_P = 797,               /* RESPECT_P  */
    RESTART = 798,                 /* RESTART  */
    RESTRICT = 799,                /* RESTRICT  */
    RESULT_CACHE = 800,            /* RESULT_CACHE  */
    RETURN = 801,                  /* RETURN  */
    RETURNED_SQLSTATE = 802,       /* RETURNED_SQLSTATE  */
    RETURNING = 803,               /* RETURNING  */
    RETURNS = 804,                 /* RETURNS  */
    REUSE = 805,                   /* REUSE  */
    REVOKE = 806,                  /* REVOKE  */
    RIGHT = 807,                   /* RIGHT  */
    ROLE = 808,                    /* ROLE  */
    ROLES = 809,                   /* ROLES  */
    ROLLBACK = 810,                /* ROLLBACK  */
    ROLLUP = 811,                  /* ROLLUP  */
    ROTATE = 812,                  /* ROTATE  */
    ROTATION = 813,                /* ROTATION  */
    ROW = 814,                     /* ROW  */
    ROW_COUNT = 815,               /* ROW_COUNT  */
    ROWNUM = 816,                  /* ROWNUM  */
    ROWS = 817,                    /* ROWS  */
    ROWTYPE_P = 818,               /* ROWTYPE_P  */
    RULE = 819,                    /* RULE  */
    SAMPLE = 820,                  /* SAMPLE  */
    SAVEPOINT = 821,               /* SAVEPOINT  */
    SCHEDULE = 822,                /* SCHEDULE  */
    SCHEMA = 823,                  /* SCHEMA  */
    SCHEMA_NAME = 824,             /* SCHEMA_NAME  */
    SCROLL = 825,                  /* SCROLL  */
    SEARCH = 826,                  /* SEARCH  */
    SECOND_P = 827,                /* SECOND_P  */
    SECURITY = 828,                /* SECURITY  */
    SELECT = 829,                  /* SELECT  */
    SEPARATOR_P = 830,             /* SEPARATOR_P  */
    SEQUENCE = 831,                /* SEQUENCE  */
    SEQUENCES = 832,               /* SEQUENCES  */
    SHARE_MEMORY = 833,            /* SHARE_MEMORY  */
    SERIALIZABLE = 834,            /* SERIALIZABLE  */
    SERVER = 835,                  /* SERVER  */
    SESSION = 836,                 /* SESSION  */
    SESSION_USER = 837,            /* SESSION_USER  */
    SET = 838,                     /* SET  */
    SETS = 839,                    /* SETS  */
    SETOF = 840,                   /* SETOF  */
    SHARE = 841,                   /* SHARE  */
    SHIPPABLE = 842,               /* SHIPPABLE  */
    SHOW = 843,                    /* SHOW  */
    SHUTDOWN = 844,                /* SHUTDOWN  */
    SIBLINGS = 845,                /* SIBLINGS  */
    SIMILAR = 846,                 /* SIMILAR  */
    SIMPLE = 847,                  /* SIMPLE  */
    SIZE = 848,                    /* SIZE  */
    SKIP = 849,                    /* SKIP  */
    SLAVE = 850,                   /* SLAVE  */
    SLICE = 851,                   /* SLICE  */
    SMALLDATETIME = 852,           /* SMALLDATETIME  */
    SMALLDATETIME_FORMAT_P = 853,  /* SMALLDATETIME_FORMAT_P  */
    SMALLINT = 854,                /* SMALLINT  */
    SNAPSHOT = 855,                /* SNAPSHOT  */
    SOME = 856,                    /* SOME  */
    SOURCE_P = 857,                /* SOURCE_P  */
    SPACE = 858,                   /* SPACE  */
    SPECIFICATION = 859,           /* SPECIFICATION  */
    SPILL = 860,                   /* SPILL  */
    SPLIT = 861,                   /* SPLIT  */
    STABLE = 862,                  /* STABLE  */
    STACKED_P = 863,               /* STACKED_P  */
    STANDALONE_P = 864,            /* STANDALONE_P  */
    START = 865,                   /* START  */
    STARTS = 866,                  /* STARTS  */
    STARTWITH = 867,               /* STARTWITH  */
    STATEMENT = 868,               /* STATEMENT  */
    STATEMENT_ID = 869,            /* STATEMENT_ID  */
    STATISTICS = 870,              /* STATISTICS  */
    STDIN = 871,                   /* STDIN  */
    STDOUT = 872,                  /* STDOUT  */
    STORAGE = 873,                 /* STORAGE  */
    STORE_P = 874,                 /* STORE_P  */
    STORED = 875,                  /* STORED  */
    STRATIFY = 876,                /* STRATIFY  */
    STREAM = 877,                  /* STREAM  */
    STRICT_P = 878,                /* STRICT_P  */
    STRIP_P = 879,                 /* STRIP_P  */
    SUBCLASS_ORIGIN = 880,         /* SUBCLASS_ORIGIN  */
    SUBPARTITION = 881,            /* SUBPARTITION  */
    SUBPARTITIONS = 882,           /* SUBPARTITIONS  */
    SUBSCRIPTION = 883,            /* SUBSCRIPTION  */
    SUBSTRING = 884,               /* SUBSTRING  */
    SYMMETRIC = 885,               /* SYMMETRIC  */
    SYNONYM = 886,                 /* SYNONYM  */
    SYSDATE = 887,                 /* SYSDATE  */
    SYSID = 888,                   /* SYSID  */
    SYSTEM_P = 889,                /* SYSTEM_P  */
    SYS_REFCURSOR = 890,           /* SYS_REFCURSOR  */
    STARTING = 891,                /* STARTING  */
    SQL_P = 892,                   /* SQL_P  */
    TABLE = 893,                   /* TABLE  */
    TABLE_NAME = 894,              /* TABLE_NAME  */
    TABLES = 895,                  /* TABLES  */
    TABLESAMPLE = 896,             /* TABLESAMPLE  */
    TABLESPACE = 897,              /* TABLESPACE  */
    TARGET = 898,                  /* TARGET  */
    TEMP = 899,                    /* TEMP  */
    TEMPLATE = 900,                /* TEMPLATE  */
    TEMPORARY = 901,               /* TEMPORARY  */
    TERMINATED = 902,              /* TERMINATED  */
    TEXT_P = 903,                  /* TEXT_P  */
    THAN = 904,                    /* THAN  */
    THEN = 905,                    /* THEN  */
    TIME = 906,                    /* TIME  */
    TIME_FORMAT_P = 907,           /* TIME_FORMAT_P  */
    TIES = 908,                    /* TIES  */
    TIMECAPSULE = 909,             /* TIMECAPSULE  */
    TIMESTAMP = 910,               /* TIMESTAMP  */
    TIMESTAMP_FORMAT_P = 911,      /* TIMESTAMP_FORMAT_P  */
    TIMESTAMPDIFF = 912,           /* TIMESTAMPDIFF  */
    TIMEZONE_HOUR_P = 913,         /* TIMEZONE_HOUR_P  */
    TIMEZONE_MINUTE_P = 914,       /* TIMEZONE_MINUTE_P  */
    TINYINT = 915,                 /* TINYINT  */
    TO = 916,                      /* TO  */
    TRAILING = 917,                /* TRAILING  */
    TRANSACTION = 918,             /* TRANSACTION  */
    TRANSFORM = 919,               /* TRANSFORM  */
    TREAT = 920,                   /* TREAT  */
    TRIGGER = 921,                 /* TRIGGER  */
    TRIM = 922,                    /* TRIM  */
    TRUE_P = 923,                  /* TRUE_P  */
    TRUNCATE = 924,                /* TRUNCATE  */
    TRUSTED = 925,                 /* TRUSTED  */
    TSFIELD = 926,                 /* TSFIELD  */
    TSTAG = 927,                   /* TSTAG  */
    TSTIME = 928,                  /* TSTIME  */
    TYPE_P = 929,                  /* TYPE_P  */
    TYPES_P = 930,                 /* TYPES_P  */
    UNBOUNDED = 931,               /* UNBOUNDED  */
    UNCOMMITTED = 932,             /* UNCOMMITTED  */
    UNENCRYPTED = 933,             /* UNENCRYPTED  */
    UNION = 934,                   /* UNION  */
    UNIQUE = 935,                  /* UNIQUE  */
    UNKNOWN = 936,                 /* UNKNOWN  */
    UNLIMITED = 937,               /* UNLIMITED  */
    UNLISTEN = 938,                /* UNLISTEN  */
    UNLOCK = 939,                  /* UNLOCK  */
    UNLOGGED = 940,                /* UNLOGGED  */
    UNIMCSTORED = 941,             /* UNIMCSTORED  */
    UNTIL = 942,                   /* UNTIL  */
    UNUSABLE = 943,                /* UNUSABLE  */
    UPDATE = 944,                  /* UPDATE  */
    USEEOF = 945,                  /* USEEOF  */
    USER = 946,                    /* USER  */
    USING = 947,                   /* USING  */
    VACUUM = 948,                  /* VACUUM  */
    VALID = 949,                   /* VALID  */
    VALIDATE = 950,                /* VALIDATE  */
    VALIDATION = 951,              /* VALIDATION  */
    VALIDATOR = 952,               /* VALIDATOR  */
    VALUE_P = 953,                 /* VALUE_P  */
    VALUES = 954,                  /* VALUES  */
    VARCHAR = 955,                 /* VARCHAR  */
    VARCHAR2 = 956,                /* VARCHAR2  */
    VARIABLES = 957,               /* VARIABLES  */
    VARIADIC = 958,                /* VARIADIC  */
    VARRAY = 959,                  /* VARRAY  */
    VARYING = 960,                 /* VARYING  */
    VCGROUP = 961,                 /* VCGROUP  */
    VERBOSE = 962,                 /* VERBOSE  */
    VERIFY = 963,                  /* VERIFY  */
    VERSION_P = 964,               /* VERSION_P  */
    VIEW = 965,                    /* VIEW  */
    VISIBLE = 966,                 /* VISIBLE  */
    VOLATILE = 967,                /* VOLATILE  */
    WAIT = 968,                    /* WAIT  */
    WARNINGS = 969,                /* WARNINGS  */
    WEAK = 970,                    /* WEAK  */
    WHEN = 971,                    /* WHEN  */
    WHERE = 972,                   /* WHERE  */
    WHILE_P = 973,                 /* WHILE_P  */
    WHITESPACE_P = 974,            /* WHITESPACE_P  */
    WINDOW = 975,                  /* WINDOW  */
    WITH = 976,                    /* WITH  */
    WITHIN = 977,                  /* WITHIN  */
    WITHOUT = 978,                 /* WITHOUT  */
    WORK = 979,                    /* WORK  */
    WORKLOAD = 980,                /* WORKLOAD  */
    WRAPPER = 981,                 /* WRAPPER  */
    WRITE = 982,                   /* WRITE  */
    XML_P = 983,                   /* XML_P  */
    XMLATTRIBUTES = 984,           /* XMLATTRIBUTES  */
    XMLCONCAT = 985,               /* XMLCONCAT  */
    XMLELEMENT = 986,              /* XMLELEMENT  */
    XMLEXISTS = 987,               /* XMLEXISTS  */
    XMLFOREST = 988,               /* XMLFOREST  */
    XMLNAMESPACES = 989,           /* XMLNAMESPACES  */
    XMLPARSE = 990,                /* XMLPARSE  */
    XMLPI = 991,                   /* XMLPI  */
    XMLROOT = 992,                 /* XMLROOT  */
    XMLSERIALIZE = 993,            /* XMLSERIALIZE  */
    XMLTABLE = 994,                /* XMLTABLE  */
    YEAR_P = 995,                  /* YEAR_P  */
    YEAR_MONTH_P = 996,            /* YEAR_MONTH_P  */
    YES_P = 997,                   /* YES_P  */
    ZONE = 998,                    /* ZONE  */
    NULLS_FIRST = 999,             /* NULLS_FIRST  */
    NULLS_LAST = 1000,             /* NULLS_LAST  */
    WITH_ORDINALITY = 1001,        /* WITH_ORDINALITY  */
    WITH_TIME = 1002,              /* WITH_TIME  */
    INCLUDING_ALL = 1003,          /* INCLUDING_ALL  */
    RENAME_PARTITION = 1004,       /* RENAME_PARTITION  */
    PARTITION_FOR = 1005,          /* PARTITION_FOR  */
    SUBPARTITION_FOR = 1006,       /* SUBPARTITION_FOR  */
    ADD_PARTITION = 1007,          /* ADD_PARTITION  */
    DROP_PARTITION = 1008,         /* DROP_PARTITION  */
    REBUILD_PARTITION = 1009,      /* REBUILD_PARTITION  */
    MODIFY_PARTITION = 1010,       /* MODIFY_PARTITION  */
    ADD_SUBPARTITION = 1011,       /* ADD_SUBPARTITION  */
    DROP_SUBPARTITION = 1012,      /* DROP_SUBPARTITION  */
    NOT_ENFORCED = 1013,           /* NOT_ENFORCED  */
    VALID_BEGIN = 1014,            /* VALID_BEGIN  */
    DECLARE_CURSOR = 1015,         /* DECLARE_CURSOR  */
    ON_UPDATE_TIME = 1016,         /* ON_UPDATE_TIME  */
    START_WITH = 1017,             /* START_WITH  */
    CONNECT_BY = 1018,             /* CONNECT_BY  */
    SHOW_ERRORS = 1019,            /* SHOW_ERRORS  */
    END_OF_INPUT = 1020,           /* END_OF_INPUT  */
    END_OF_INPUT_COLON = 1021,     /* END_OF_INPUT_COLON  */
    END_OF_PROC = 1022,            /* END_OF_PROC  */
    EVENT_TRIGGER = 1023,          /* EVENT_TRIGGER  */
    NOT_IN = 1024,                 /* NOT_IN  */
    NOT_BETWEEN = 1025,            /* NOT_BETWEEN  */
    NOT_LIKE = 1026,               /* NOT_LIKE  */
    NOT_ILIKE = 1027,              /* NOT_ILIKE  */
    NOT_SIMILAR = 1028,            /* NOT_SIMILAR  */
    FORCE_INDEX = 1029,            /* FORCE_INDEX  */
    USE_INDEX = 1030,              /* USE_INDEX  */
    IGNORE_INDEX = 1031,           /* IGNORE_INDEX  */
    CURSOR_EXPR = 1032,            /* CURSOR_EXPR  */
    LATERAL_EXPR = 1033,           /* LATERAL_EXPR  */
    FALSE_ON_ERROR = 1034,         /* FALSE_ON_ERROR  */
    TRUE_ON_ERROR = 1035,          /* TRUE_ON_ERROR  */
    ERROR_ON_ERROR = 1036,         /* ERROR_ON_ERROR  */
    PARTIAL_EMPTY_PREC = 1037,     /* PARTIAL_EMPTY_PREC  */
    lower_than_index = 1038,       /* lower_than_index  */
    lower_than_under = 1039,       /* lower_than_under  */
    UNDER = 1040,                  /* UNDER  */
    higher_than_rotate = 1041,     /* higher_than_rotate  */
    TSQL_PERSISTED = 1042,         /* TSQL_PERSISTED  */
    UMINUS = 1043,                 /* UMINUS  */
    EMPTY_FROM_CLAUSE = 1044,      /* EMPTY_FROM_CLAUSE  */
    CONSTRUCTOR = 1045,            /* CONSTRUCTOR  */
    FINAL = 1046,                  /* FINAL  */
    MAP = 1047,                    /* MAP  */
    MEMBER = 1048,                 /* MEMBER  */
    RESULT = 1049,                 /* RESULT  */
    SELF = 1050,                   /* SELF  */
    STATIC_P = 1051,               /* STATIC_P  */
    SELF_INOUT = 1052,             /* SELF_INOUT  */
    STATIC_FUNCTION = 1053,        /* STATIC_FUNCTION  */
    MEMBER_FUNCTION = 1054,        /* MEMBER_FUNCTION  */
    STATIC_PROCEDURE = 1055,       /* STATIC_PROCEDURE  */
    MEMBER_PROCEDURE = 1056,       /* MEMBER_PROCEDURE  */
    CONSTRUCTOR_FUNCTION = 1057,   /* CONSTRUCTOR_FUNCTION  */
    MAP_MEMBER = 1058,             /* MAP_MEMBER  */
    CATCH = 1059,                  /* CATCH  */
    CHECKIDENT = 1060,             /* CHECKIDENT  */
    DBCC = 1061,                   /* DBCC  */
    NO_INFOMSGS = 1062,            /* NO_INFOMSGS  */
    NORESEED = 1063,               /* NORESEED  */
    RESEED = 1064,                 /* RESEED  */
    SAVE = 1065,                   /* SAVE  */
    TRAN = 1066,                   /* TRAN  */
    TRY = 1067,                    /* TRY  */
    TSQL_CLUSTERED = 1068,         /* TSQL_CLUSTERED  */
    TSQL_NONCLUSTERED = 1069,      /* TSQL_NONCLUSTERED  */
    TSQL_COLUMNSTORE = 1070,       /* TSQL_COLUMNSTORE  */
    TSQL_TOP = 1071,               /* TSQL_TOP  */
    TSQL_PERCENT = 1072,           /* TSQL_PERCENT  */
    TSQL_NOLOCK = 1073,            /* TSQL_NOLOCK  */
    TSQL_READUNCOMMITTED = 1074,   /* TSQL_READUNCOMMITTED  */
    TSQL_UPDLOCK = 1075,           /* TSQL_UPDLOCK  */
    TSQL_REPEATABLEREAD = 1076,    /* TSQL_REPEATABLEREAD  */
    TSQL_READCOMMITTED = 1077,     /* TSQL_READCOMMITTED  */
    TSQL_TABLOCK = 1078,           /* TSQL_TABLOCK  */
    TSQL_TABLOCKX = 1079,          /* TSQL_TABLOCKX  */
    TSQL_PAGLOCK = 1080,           /* TSQL_PAGLOCK  */
    TSQL_ROWLOCK = 1081,           /* TSQL_ROWLOCK  */
    TSQL_READPAST = 1082,          /* TSQL_READPAST  */
    TSQL_XLOCK = 1083,             /* TSQL_XLOCK  */
    TSQL_NOEXPAND = 1084,          /* TSQL_NOEXPAND  */
    TSQL_PROC = 1085,              /* TSQL_PROC  */
    TSQL_TEXTIMAGE_ON = 1086,      /* TSQL_TEXTIMAGE_ON  */
    TSQL_MINUTES_P = 1087,         /* TSQL_MINUTES_P  */
    XACT_ABORT = 1088,             /* XACT_ABORT  */
    TSQL_TRY_CAST = 1089,          /* TSQL_TRY_CAST  */
    TSQL_TRY_CONVERT = 1090,       /* TSQL_TRY_CONVERT  */
    TSQL_CONVERT = 1091,           /* TSQL_CONVERT  */
    TSQL_DATEDIFF = 1092,          /* TSQL_DATEDIFF  */
    TSQL_DATEDIFF_BIG = 1093,      /* TSQL_DATEDIFF_BIG  */
    TSQL_D = 1094,                 /* TSQL_D  */
    TSQL_DAYOFYEAR = 1095,         /* TSQL_DAYOFYEAR  */
    TSQL_DW = 1096,                /* TSQL_DW  */
    TSQL_DY = 1097,                /* TSQL_DY  */
    TSQL_HH = 1098,                /* TSQL_HH  */
    TSQL_M = 1099,                 /* TSQL_M  */
    TSQL_MCS = 1100,               /* TSQL_MCS  */
    TSQL_MI = 1101,                /* TSQL_MI  */
    TSQL_MICROSECOND = 1102,       /* TSQL_MICROSECOND  */
    TSQL_MILLISECOND = 1103,       /* TSQL_MILLISECOND  */
    TSQL_MM = 1104,                /* TSQL_MM  */
    TSQL_MS = 1105,                /* TSQL_MS  */
    TSQL_N = 1106,                 /* TSQL_N  */
    TSQL_NS = 1107,                /* TSQL_NS  */
    TSQL_Q = 1108,                 /* TSQL_Q  */
    TSQL_QQ = 1109,                /* TSQL_QQ  */
    TSQL_QUARTER = 1110,           /* TSQL_QUARTER  */
    TSQL_SS = 1111,                /* TSQL_SS  */
    TSQL_WEEK = 1112,              /* TSQL_WEEK  */
    TSQL_WEEKDAY = 1113,           /* TSQL_WEEKDAY  */
    TSQL_WK = 1114,                /* TSQL_WK  */
    TSQL_WW = 1115,                /* TSQL_WW  */
    TSQL_W = 1116,                 /* TSQL_W  */
    TSQL_Y = 1117,                 /* TSQL_Y  */
    TSQL_S = 1118,                 /* TSQL_S  */
    TSQL_YYYY = 1119,              /* TSQL_YYYY  */
    TSQL_YY = 1120,                /* TSQL_YY  */
    TSQL_DD = 1121,                /* TSQL_DD  */
    TSQL_NANOSECOND = 1122,        /* TSQL_NANOSECOND  */
    TSQL_CAST = 1123,              /* TSQL_CAST  */
    TSQL_DOUBLE_PRECISION = 1124,  /* TSQL_DOUBLE_PRECISION  */
    TSQL_BIGINT = 1125,            /* TSQL_BIGINT  */
    TSQL_MAX = 1126,               /* TSQL_MAX  */
    TSQL_ATAT_IDENT = 1127,        /* TSQL_ATAT_IDENT  */
    TSQL_UNIQUE_CLUSTERED = 1128,  /* TSQL_UNIQUE_CLUSTERED  */
    TSQL_UNIQUE_NONCLUSTERED = 1129, /* TSQL_UNIQUE_NONCLUSTERED  */
    TSQL_PRIMAY_KEY_NONCLUSTERED = 1130, /* TSQL_PRIMAY_KEY_NONCLUSTERED  */
    TSQL_PRIMAY_KEY_CLUSTERED = 1131, /* TSQL_PRIMAY_KEY_CLUSTERED  */
    WITH_paren = 1132,             /* WITH_paren  */
    TSQL_HINT_START_BRACKET = 1133, /* TSQL_HINT_START_BRACKET  */
    TSQL_EXEC = 1134,              /* TSQL_EXEC  */
    TSQL_OUTPUT = 1135             /* TSQL_OUTPUT  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 378 "src/backend_parser/gram-backend.y"

#line 334 "gram.y"
	core_YYSTYPE		core_yystype;
	/* these fields must match core_YYSTYPE: */
	int					ival;
	char				*str;
	const char			*keyword;

	char				chr;
	bool				boolean;
	int64				ival64;
	JoinType			jtype;
	DropBehavior		dbehavior;
	OnCommitAction		oncommit;
	List				*list;
	Node				*node;
	Value				*value;
	ObjectType			objtype;
	TypeName			*typnam;
	FunctionSources		*fun_src;
	FunctionParameter   *fun_param;
	FunctionParameterMode fun_param_mode;
	FuncWithArgs		*funwithargs;
	DefElem				*defelt;
	SortBy				*sortby;
	WindowDef			*windef;
        KeepClause                      *keep;
	JoinExpr			*jexpr;
	IndexElem			*ielem;
	Alias				*alias;
	RangeVar			*range;
	IntoClause			*into;
	WithClause			*with;
	A_Indices			*aind;
	ResTarget			*target;
	struct PrivTarget	*privtarget;
	struct TrgCharacter *trgcharacter;
	struct FetchLimit	*fetchlimit;
	AccessPriv			*accesspriv;
	DbPriv				*dbpriv;
	InsertStmt			*istmt;
	VariableSetStmt		*vsetstmt;
/* PGXC_BEGIN */
	DistributeBy		*distby;
	PGXCSubCluster		*subclus;
/* PGXC_END */
    ForeignPartState    *foreignpartby;
	MergeWhenClause		*mergewhen;
	UpsertClause *upsert;
	EncryptionType algtype;
	LockClauseStrength lockstrength;
	CharsetCollateOptions *charsetcollateopt;
	OnDuplicateAction onduplicate;
	struct CondInfo*	condinfo;
	RotateClause         *rotateinfo;
	UnrotateClause       *unrotateinfo;
	FunctionPartitionInfo *funcPartInfo;

#line 1003 "src/backend_parser/gram-backend.hpp"

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



int pgtsql_base_yyparse (core_yyscan_t yyscanner);

#endif /* !YY_PGTSQL_BASE_YY_SRC_BACKEND_PARSER_GRAM_BACKEND_HPP_INCLUDED  */
