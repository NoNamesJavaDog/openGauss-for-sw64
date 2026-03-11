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

#ifndef YY_BASE_YY_GRAM_HPP_INCLUDED
# define YY_BASE_YY_GRAM_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
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
    ABORT_P = 283,                 /* ABORT_P  */
    ABSOLUTE_P = 284,              /* ABSOLUTE_P  */
    ACCESS = 285,                  /* ACCESS  */
    ACCOUNT = 286,                 /* ACCOUNT  */
    ACTION = 287,                  /* ACTION  */
    ADD_P = 288,                   /* ADD_P  */
    ADMIN = 289,                   /* ADMIN  */
    AFTER = 290,                   /* AFTER  */
    AGGREGATE = 291,               /* AGGREGATE  */
    ALGORITHM = 292,               /* ALGORITHM  */
    ALL = 293,                     /* ALL  */
    ALSO = 294,                    /* ALSO  */
    ALTER = 295,                   /* ALTER  */
    ALWAYS = 296,                  /* ALWAYS  */
    ANALYSE = 297,                 /* ANALYSE  */
    ANALYZE = 298,                 /* ANALYZE  */
    AND = 299,                     /* AND  */
    ANY = 300,                     /* ANY  */
    APP = 301,                     /* APP  */
    APPEND = 302,                  /* APPEND  */
    APPLY = 303,                   /* APPLY  */
    ARCHIVE = 304,                 /* ARCHIVE  */
    ARRAY = 305,                   /* ARRAY  */
    AS = 306,                      /* AS  */
    ASC = 307,                     /* ASC  */
    ASOF_P = 308,                  /* ASOF_P  */
    ASSERTION = 309,               /* ASSERTION  */
    ASSIGNMENT = 310,              /* ASSIGNMENT  */
    ASYMMETRIC = 311,              /* ASYMMETRIC  */
    AT = 312,                      /* AT  */
    ATTRIBUTE = 313,               /* ATTRIBUTE  */
    AUDIT = 314,                   /* AUDIT  */
    AUTHID = 315,                  /* AUTHID  */
    AUTHORIZATION = 316,           /* AUTHORIZATION  */
    AUTOEXTEND = 317,              /* AUTOEXTEND  */
    AUTOMAPPED = 318,              /* AUTOMAPPED  */
    AUTO_INCREMENT = 319,          /* AUTO_INCREMENT  */
    BACKWARD = 320,                /* BACKWARD  */
    BARRIER = 321,                 /* BARRIER  */
    BEFORE = 322,                  /* BEFORE  */
    BEGIN_NON_ANOYBLOCK = 323,     /* BEGIN_NON_ANOYBLOCK  */
    BEGIN_P = 324,                 /* BEGIN_P  */
    BETWEEN = 325,                 /* BETWEEN  */
    BIGINT = 326,                  /* BIGINT  */
    BINARY = 327,                  /* BINARY  */
    BINARY_DOUBLE = 328,           /* BINARY_DOUBLE  */
    BINARY_DOUBLE_INF = 329,       /* BINARY_DOUBLE_INF  */
    BINARY_DOUBLE_NAN = 330,       /* BINARY_DOUBLE_NAN  */
    BINARY_INTEGER = 331,          /* BINARY_INTEGER  */
    BIT = 332,                     /* BIT  */
    BLANKS = 333,                  /* BLANKS  */
    BLOB_P = 334,                  /* BLOB_P  */
    BLOCKCHAIN = 335,              /* BLOCKCHAIN  */
    BODY_P = 336,                  /* BODY_P  */
    BOGUS = 337,                   /* BOGUS  */
    BOOLEAN_P = 338,               /* BOOLEAN_P  */
    BOTH = 339,                    /* BOTH  */
    BUCKETCNT = 340,               /* BUCKETCNT  */
    BUCKETS = 341,                 /* BUCKETS  */
    BUILD = 342,                   /* BUILD  */
    BY = 343,                      /* BY  */
    BYTE_P = 344,                  /* BYTE_P  */
    BYTEAWITHOUTORDER = 345,       /* BYTEAWITHOUTORDER  */
    BYTEAWITHOUTORDERWITHEQUAL = 346, /* BYTEAWITHOUTORDERWITHEQUAL  */
    CACHE = 347,                   /* CACHE  */
    CALL = 348,                    /* CALL  */
    CALLED = 349,                  /* CALLED  */
    CANCELABLE = 350,              /* CANCELABLE  */
    CASCADE = 351,                 /* CASCADE  */
    CASCADED = 352,                /* CASCADED  */
    CASE = 353,                    /* CASE  */
    CAST = 354,                    /* CAST  */
    CATALOG_P = 355,               /* CATALOG_P  */
    CATALOG_NAME = 356,            /* CATALOG_NAME  */
    CHAIN = 357,                   /* CHAIN  */
    CHANGE = 358,                  /* CHANGE  */
    CHAR_P = 359,                  /* CHAR_P  */
    CHARACTER = 360,               /* CHARACTER  */
    CHARACTERISTICS = 361,         /* CHARACTERISTICS  */
    CHARACTERSET = 362,            /* CHARACTERSET  */
    CHARSET = 363,                 /* CHARSET  */
    CHECK = 364,                   /* CHECK  */
    CHECKPOINT = 365,              /* CHECKPOINT  */
    CLASS = 366,                   /* CLASS  */
    CLASS_ORIGIN = 367,            /* CLASS_ORIGIN  */
    CLEAN = 368,                   /* CLEAN  */
    CLIENT = 369,                  /* CLIENT  */
    CLIENT_MASTER_KEY = 370,       /* CLIENT_MASTER_KEY  */
    CLIENT_MASTER_KEYS = 371,      /* CLIENT_MASTER_KEYS  */
    CLOB = 372,                    /* CLOB  */
    CLOSE = 373,                   /* CLOSE  */
    CLUSTER = 374,                 /* CLUSTER  */
    COALESCE = 375,                /* COALESCE  */
    COLLATE = 376,                 /* COLLATE  */
    COLLATION = 377,               /* COLLATION  */
    COLUMN = 378,                  /* COLUMN  */
    COLUMN_ENCRYPTION_KEY = 379,   /* COLUMN_ENCRYPTION_KEY  */
    COLUMN_ENCRYPTION_KEYS = 380,  /* COLUMN_ENCRYPTION_KEYS  */
    COLUMN_NAME = 381,             /* COLUMN_NAME  */
    COLUMNS = 382,                 /* COLUMNS  */
    COMMENT = 383,                 /* COMMENT  */
    COMMENTS = 384,                /* COMMENTS  */
    COMMIT = 385,                  /* COMMIT  */
    COMMITTED = 386,               /* COMMITTED  */
    COMPACT = 387,                 /* COMPACT  */
    COMPATIBLE_ILLEGAL_CHARS = 388, /* COMPATIBLE_ILLEGAL_CHARS  */
    COMPILE = 389,                 /* COMPILE  */
    COMPLETE = 390,                /* COMPLETE  */
    COMPLETION = 391,              /* COMPLETION  */
    COMPRESS = 392,                /* COMPRESS  */
    CONCURRENTLY = 393,            /* CONCURRENTLY  */
    CONDITION = 394,               /* CONDITION  */
    CONFIGURATION = 395,           /* CONFIGURATION  */
    CONNECTION = 396,              /* CONNECTION  */
    CONSISTENT = 397,              /* CONSISTENT  */
    CONSTANT = 398,                /* CONSTANT  */
    CONSTRAINT = 399,              /* CONSTRAINT  */
    CONSTRAINT_CATALOG = 400,      /* CONSTRAINT_CATALOG  */
    CONSTRAINT_NAME = 401,         /* CONSTRAINT_NAME  */
    CONSTRAINT_SCHEMA = 402,       /* CONSTRAINT_SCHEMA  */
    CONSTRAINTS = 403,             /* CONSTRAINTS  */
    CONTENT_P = 404,               /* CONTENT_P  */
    CONTINUE_P = 405,              /* CONTINUE_P  */
    CONTVIEW = 406,                /* CONTVIEW  */
    CONVERSION_P = 407,            /* CONVERSION_P  */
    CONVERT_P = 408,               /* CONVERT_P  */
    CONNECT = 409,                 /* CONNECT  */
    COORDINATOR = 410,             /* COORDINATOR  */
    COORDINATORS = 411,            /* COORDINATORS  */
    COPY = 412,                    /* COPY  */
    COST = 413,                    /* COST  */
    CREATE = 414,                  /* CREATE  */
    CROSS = 415,                   /* CROSS  */
    CSN = 416,                     /* CSN  */
    CSV = 417,                     /* CSV  */
    CUBE = 418,                    /* CUBE  */
    CURRENT_P = 419,               /* CURRENT_P  */
    CURRENT_CATALOG = 420,         /* CURRENT_CATALOG  */
    CURRENT_DATE = 421,            /* CURRENT_DATE  */
    CURRENT_ROLE = 422,            /* CURRENT_ROLE  */
    CURRENT_SCHEMA = 423,          /* CURRENT_SCHEMA  */
    CURRENT_TIME = 424,            /* CURRENT_TIME  */
    CURRENT_TIMESTAMP = 425,       /* CURRENT_TIMESTAMP  */
    CURRENT_USER = 426,            /* CURRENT_USER  */
    CURSOR = 427,                  /* CURSOR  */
    CURSOR_NAME = 428,             /* CURSOR_NAME  */
    CYCLE = 429,                   /* CYCLE  */
    SHRINK = 430,                  /* SHRINK  */
    USE_P = 431,                   /* USE_P  */
    DATA_P = 432,                  /* DATA_P  */
    DATABASE = 433,                /* DATABASE  */
    DATAFILE = 434,                /* DATAFILE  */
    DATANODE = 435,                /* DATANODE  */
    DATANODES = 436,               /* DATANODES  */
    DATATYPE_CL = 437,             /* DATATYPE_CL  */
    DATE_P = 438,                  /* DATE_P  */
    DATE_FORMAT_P = 439,           /* DATE_FORMAT_P  */
    DAY_P = 440,                   /* DAY_P  */
    DAY_HOUR_P = 441,              /* DAY_HOUR_P  */
    DAY_MINUTE_P = 442,            /* DAY_MINUTE_P  */
    DAY_SECOND_P = 443,            /* DAY_SECOND_P  */
    DBCOMPATIBILITY_P = 444,       /* DBCOMPATIBILITY_P  */
    DEALLOCATE = 445,              /* DEALLOCATE  */
    DEC = 446,                     /* DEC  */
    DECIMAL_P = 447,               /* DECIMAL_P  */
    DECLARE = 448,                 /* DECLARE  */
    DECODE = 449,                  /* DECODE  */
    DEFAULT = 450,                 /* DEFAULT  */
    DEFAULTS = 451,                /* DEFAULTS  */
    DEFERRABLE = 452,              /* DEFERRABLE  */
    DEFERRED = 453,                /* DEFERRED  */
    DEFINER = 454,                 /* DEFINER  */
    DELETE_P = 455,                /* DELETE_P  */
    DELIMITER = 456,               /* DELIMITER  */
    DELIMITERS = 457,              /* DELIMITERS  */
    DELTA = 458,                   /* DELTA  */
    DELTAMERGE = 459,              /* DELTAMERGE  */
    DENSE_RANK = 460,              /* DENSE_RANK  */
    DESC = 461,                    /* DESC  */
    DETERMINISTIC = 462,           /* DETERMINISTIC  */
    DIAGNOSTICS = 463,             /* DIAGNOSTICS  */
    DICTIONARY = 464,              /* DICTIONARY  */
    DIRECT = 465,                  /* DIRECT  */
    DIRECTORY = 466,               /* DIRECTORY  */
    DISABLE_P = 467,               /* DISABLE_P  */
    DISCARD = 468,                 /* DISCARD  */
    DISTINCT = 469,                /* DISTINCT  */
    DISTRIBUTE = 470,              /* DISTRIBUTE  */
    DISTRIBUTION = 471,            /* DISTRIBUTION  */
    DO = 472,                      /* DO  */
    DOCUMENT_P = 473,              /* DOCUMENT_P  */
    DOMAIN_P = 474,                /* DOMAIN_P  */
    DOUBLE_P = 475,                /* DOUBLE_P  */
    DROP = 476,                    /* DROP  */
    DUPLICATE = 477,               /* DUPLICATE  */
    DISCONNECT = 478,              /* DISCONNECT  */
    DUMPFILE = 479,                /* DUMPFILE  */
    EACH = 480,                    /* EACH  */
    ELASTIC = 481,                 /* ELASTIC  */
    ELSE = 482,                    /* ELSE  */
    ENABLE_P = 483,                /* ENABLE_P  */
    ENCLOSED = 484,                /* ENCLOSED  */
    ENCODING = 485,                /* ENCODING  */
    ENCRYPTED = 486,               /* ENCRYPTED  */
    ENCRYPTED_VALUE = 487,         /* ENCRYPTED_VALUE  */
    ENCRYPTION = 488,              /* ENCRYPTION  */
    ENCRYPTION_TYPE = 489,         /* ENCRYPTION_TYPE  */
    END_P = 490,                   /* END_P  */
    ENDS = 491,                    /* ENDS  */
    ENFORCED = 492,                /* ENFORCED  */
    ENUM_P = 493,                  /* ENUM_P  */
    ERROR_P = 494,                 /* ERROR_P  */
    ERRORS = 495,                  /* ERRORS  */
    ESCAPE = 496,                  /* ESCAPE  */
    EOL = 497,                     /* EOL  */
    ESCAPING = 498,                /* ESCAPING  */
    EVENT = 499,                   /* EVENT  */
    EVENTS = 500,                  /* EVENTS  */
    EVERY = 501,                   /* EVERY  */
    EXCEPT = 502,                  /* EXCEPT  */
    EXCHANGE = 503,                /* EXCHANGE  */
    EXCLUDE = 504,                 /* EXCLUDE  */
    EXCLUDED = 505,                /* EXCLUDED  */
    EXCLUDING = 506,               /* EXCLUDING  */
    EXCLUSIVE = 507,               /* EXCLUSIVE  */
    EXECUTE = 508,                 /* EXECUTE  */
    EXISTS = 509,                  /* EXISTS  */
    EXPIRED_P = 510,               /* EXPIRED_P  */
    EXPLAIN = 511,                 /* EXPLAIN  */
    EXTENSION = 512,               /* EXTENSION  */
    EXTERNAL = 513,                /* EXTERNAL  */
    EXTRACT = 514,                 /* EXTRACT  */
    ESCAPED = 515,                 /* ESCAPED  */
    FALSE_P = 516,                 /* FALSE_P  */
    FAMILY = 517,                  /* FAMILY  */
    FAST = 518,                    /* FAST  */
    FENCED = 519,                  /* FENCED  */
    FETCH = 520,                   /* FETCH  */
    FIELDS = 521,                  /* FIELDS  */
    FILEHEADER_P = 522,            /* FILEHEADER_P  */
    FILL_MISSING_FIELDS = 523,     /* FILL_MISSING_FIELDS  */
    FILLER = 524,                  /* FILLER  */
    FILTER = 525,                  /* FILTER  */
    FIRST_P = 526,                 /* FIRST_P  */
    FIXED_P = 527,                 /* FIXED_P  */
    FLOAT_P = 528,                 /* FLOAT_P  */
    FOLLOWING = 529,               /* FOLLOWING  */
    FOLLOWS_P = 530,               /* FOLLOWS_P  */
    FOR = 531,                     /* FOR  */
    FORCE = 532,                   /* FORCE  */
    FOREIGN = 533,                 /* FOREIGN  */
    FORMATTER = 534,               /* FORMATTER  */
    FORWARD = 535,                 /* FORWARD  */
    FEATURES = 536,                /* FEATURES  */
    FREEZE = 537,                  /* FREEZE  */
    FROM = 538,                    /* FROM  */
    FULL = 539,                    /* FULL  */
    FUNCTION = 540,                /* FUNCTION  */
    FUNCTIONS = 541,               /* FUNCTIONS  */
    GENERATED = 542,               /* GENERATED  */
    GET = 543,                     /* GET  */
    GLOBAL = 544,                  /* GLOBAL  */
    GRANT = 545,                   /* GRANT  */
    GRANTED = 546,                 /* GRANTED  */
    GREATEST = 547,                /* GREATEST  */
    GROUP_P = 548,                 /* GROUP_P  */
    GROUPING_P = 549,              /* GROUPING_P  */
    GROUPPARENT = 550,             /* GROUPPARENT  */
    HANDLER = 551,                 /* HANDLER  */
    HAVING = 552,                  /* HAVING  */
    HDFSDIRECTORY = 553,           /* HDFSDIRECTORY  */
    HEADER_P = 554,                /* HEADER_P  */
    HOLD = 555,                    /* HOLD  */
    HOUR_P = 556,                  /* HOUR_P  */
    HOUR_MINUTE_P = 557,           /* HOUR_MINUTE_P  */
    HOUR_SECOND_P = 558,           /* HOUR_SECOND_P  */
    IDENTIFIED = 559,              /* IDENTIFIED  */
    IDENTITY_P = 560,              /* IDENTITY_P  */
    IF_P = 561,                    /* IF_P  */
    IGNORE = 562,                  /* IGNORE  */
    IGNORE_EXTRA_DATA = 563,       /* IGNORE_EXTRA_DATA  */
    ILIKE = 564,                   /* ILIKE  */
    IMMEDIATE = 565,               /* IMMEDIATE  */
    IMMUTABLE = 566,               /* IMMUTABLE  */
    IMPLICIT_P = 567,              /* IMPLICIT_P  */
    IN_P = 568,                    /* IN_P  */
    INCLUDE = 569,                 /* INCLUDE  */
    IMCSTORED = 570,               /* IMCSTORED  */
    INCLUDING = 571,               /* INCLUDING  */
    INCREMENT = 572,               /* INCREMENT  */
    INCREMENTAL = 573,             /* INCREMENTAL  */
    INDEX = 574,                   /* INDEX  */
    INDEXES = 575,                 /* INDEXES  */
    INFILE = 576,                  /* INFILE  */
    INFINITE_P = 577,              /* INFINITE_P  */
    INHERIT = 578,                 /* INHERIT  */
    INHERITS = 579,                /* INHERITS  */
    INITIAL_P = 580,               /* INITIAL_P  */
    INITIALLY = 581,               /* INITIALLY  */
    INITRANS = 582,                /* INITRANS  */
    INLINE_P = 583,                /* INLINE_P  */
    INNER_P = 584,                 /* INNER_P  */
    INOUT = 585,                   /* INOUT  */
    INPUT_P = 586,                 /* INPUT_P  */
    INSENSITIVE = 587,             /* INSENSITIVE  */
    INSERT = 588,                  /* INSERT  */
    INSTEAD = 589,                 /* INSTEAD  */
    INT_P = 590,                   /* INT_P  */
    INTEGER = 591,                 /* INTEGER  */
    INTERNAL = 592,                /* INTERNAL  */
    INTERSECT = 593,               /* INTERSECT  */
    INTERVAL = 594,                /* INTERVAL  */
    INTO = 595,                    /* INTO  */
    INVISIBLE = 596,               /* INVISIBLE  */
    INVOKER = 597,                 /* INVOKER  */
    IP = 598,                      /* IP  */
    IS = 599,                      /* IS  */
    ISNULL = 600,                  /* ISNULL  */
    ISOLATION = 601,               /* ISOLATION  */
    JOIN = 602,                    /* JOIN  */
    JSON_EXISTS = 603,             /* JSON_EXISTS  */
    KEEP = 604,                    /* KEEP  */
    KEY = 605,                     /* KEY  */
    KILL = 606,                    /* KILL  */
    KEY_PATH = 607,                /* KEY_PATH  */
    KEY_STORE = 608,               /* KEY_STORE  */
    LABEL = 609,                   /* LABEL  */
    LANGUAGE = 610,                /* LANGUAGE  */
    LARGE_P = 611,                 /* LARGE_P  */
    LAST_P = 612,                  /* LAST_P  */
    LATERAL_P = 613,               /* LATERAL_P  */
    LC_COLLATE_P = 614,            /* LC_COLLATE_P  */
    LC_CTYPE_P = 615,              /* LC_CTYPE_P  */
    LEADING = 616,                 /* LEADING  */
    LEAKPROOF = 617,               /* LEAKPROOF  */
    LINES = 618,                   /* LINES  */
    LEAST = 619,                   /* LEAST  */
    LESS = 620,                    /* LESS  */
    LEFT = 621,                    /* LEFT  */
    LEVEL = 622,                   /* LEVEL  */
    LIKE = 623,                    /* LIKE  */
    LIMIT = 624,                   /* LIMIT  */
    LIST = 625,                    /* LIST  */
    LISTEN = 626,                  /* LISTEN  */
    LOAD = 627,                    /* LOAD  */
    LOCAL = 628,                   /* LOCAL  */
    LOCALTIME = 629,               /* LOCALTIME  */
    LOCALTIMESTAMP = 630,          /* LOCALTIMESTAMP  */
    LOCATION = 631,                /* LOCATION  */
    LOCK_P = 632,                  /* LOCK_P  */
    LOCKED = 633,                  /* LOCKED  */
    LOG_P = 634,                   /* LOG_P  */
    LOGGING = 635,                 /* LOGGING  */
    LOGIN_ANY = 636,               /* LOGIN_ANY  */
    LOGIN_FAILURE = 637,           /* LOGIN_FAILURE  */
    LOGIN_SUCCESS = 638,           /* LOGIN_SUCCESS  */
    LOGOUT = 639,                  /* LOGOUT  */
    LOOP = 640,                    /* LOOP  */
    MAPPING = 641,                 /* MAPPING  */
    MASKING = 642,                 /* MASKING  */
    MASTER = 643,                  /* MASTER  */
    MATCH = 644,                   /* MATCH  */
    MATERIALIZED = 645,            /* MATERIALIZED  */
    MATCHED = 646,                 /* MATCHED  */
    MAXEXTENTS = 647,              /* MAXEXTENTS  */
    MAXSIZE = 648,                 /* MAXSIZE  */
    MAXTRANS = 649,                /* MAXTRANS  */
    MAXVALUE = 650,                /* MAXVALUE  */
    MERGE = 651,                   /* MERGE  */
    MESSAGE_TEXT = 652,            /* MESSAGE_TEXT  */
    METHOD = 653,                  /* METHOD  */
    MINUS_P = 654,                 /* MINUS_P  */
    MINUTE_P = 655,                /* MINUTE_P  */
    MINUTE_SECOND_P = 656,         /* MINUTE_SECOND_P  */
    MINVALUE = 657,                /* MINVALUE  */
    MINEXTENTS = 658,              /* MINEXTENTS  */
    MODE = 659,                    /* MODE  */
    MODEL = 660,                   /* MODEL  */
    MODIFY_P = 661,                /* MODIFY_P  */
    MONTH_P = 662,                 /* MONTH_P  */
    MOVE = 663,                    /* MOVE  */
    MOVEMENT = 664,                /* MOVEMENT  */
    MYSQL_ERRNO = 665,             /* MYSQL_ERRNO  */
    NAME_P = 666,                  /* NAME_P  */
    NAMES = 667,                   /* NAMES  */
    NAN_P = 668,                   /* NAN_P  */
    NATIONAL = 669,                /* NATIONAL  */
    NATURAL = 670,                 /* NATURAL  */
    NCHAR = 671,                   /* NCHAR  */
    NEXT = 672,                    /* NEXT  */
    NO = 673,                      /* NO  */
    NOCOMPRESS = 674,              /* NOCOMPRESS  */
    NOCYCLE = 675,                 /* NOCYCLE  */
    NODE = 676,                    /* NODE  */
    NOLOGGING = 677,               /* NOLOGGING  */
    NOMAXVALUE = 678,              /* NOMAXVALUE  */
    NOMINVALUE = 679,              /* NOMINVALUE  */
    NONE = 680,                    /* NONE  */
    NOT = 681,                     /* NOT  */
    NOTHING = 682,                 /* NOTHING  */
    NOTIFY = 683,                  /* NOTIFY  */
    NOTNULL = 684,                 /* NOTNULL  */
    NOVALIDATE = 685,              /* NOVALIDATE  */
    NOWAIT = 686,                  /* NOWAIT  */
    NTH_VALUE_P = 687,             /* NTH_VALUE_P  */
    NULL_P = 688,                  /* NULL_P  */
    NULLCOLS = 689,                /* NULLCOLS  */
    NULLIF = 690,                  /* NULLIF  */
    NULLS_P = 691,                 /* NULLS_P  */
    NUMBER_P = 692,                /* NUMBER_P  */
    NUMERIC = 693,                 /* NUMERIC  */
    NUMSTR = 694,                  /* NUMSTR  */
    NVARCHAR = 695,                /* NVARCHAR  */
    NVARCHAR2 = 696,               /* NVARCHAR2  */
    NVL = 697,                     /* NVL  */
    OBJECT_P = 698,                /* OBJECT_P  */
    OF = 699,                      /* OF  */
    OFF = 700,                     /* OFF  */
    OFFSET = 701,                  /* OFFSET  */
    OIDS = 702,                    /* OIDS  */
    ON = 703,                      /* ON  */
    ONLY = 704,                    /* ONLY  */
    OPERATOR = 705,                /* OPERATOR  */
    OPTIMIZATION = 706,            /* OPTIMIZATION  */
    OPTION = 707,                  /* OPTION  */
    OPTIONALLY = 708,              /* OPTIONALLY  */
    OPTIONS = 709,                 /* OPTIONS  */
    OR = 710,                      /* OR  */
    ORDER = 711,                   /* ORDER  */
    OUT_P = 712,                   /* OUT_P  */
    OUTER_P = 713,                 /* OUTER_P  */
    OVER = 714,                    /* OVER  */
    OVERLAPS = 715,                /* OVERLAPS  */
    OVERLAY = 716,                 /* OVERLAY  */
    OWNED = 717,                   /* OWNED  */
    OWNER = 718,                   /* OWNER  */
    OUTFILE = 719,                 /* OUTFILE  */
    PACKAGE = 720,                 /* PACKAGE  */
    PACKAGES = 721,                /* PACKAGES  */
    PARALLEL_ENABLE = 722,         /* PARALLEL_ENABLE  */
    PARSER = 723,                  /* PARSER  */
    PARTIAL = 724,                 /* PARTIAL  */
    PARTITION = 725,               /* PARTITION  */
    PARTITIONS = 726,              /* PARTITIONS  */
    PASSING = 727,                 /* PASSING  */
    PASSWORD = 728,                /* PASSWORD  */
    PCTFREE = 729,                 /* PCTFREE  */
    PER_P = 730,                   /* PER_P  */
    PERCENT = 731,                 /* PERCENT  */
    PERFORMANCE = 732,             /* PERFORMANCE  */
    PERM = 733,                    /* PERM  */
    PLACING = 734,                 /* PLACING  */
    PLAN = 735,                    /* PLAN  */
    PLANS = 736,                   /* PLANS  */
    POLICY = 737,                  /* POLICY  */
    POSITION = 738,                /* POSITION  */
    PIPELINED = 739,               /* PIPELINED  */
    POOL = 740,                    /* POOL  */
    PRECEDING = 741,               /* PRECEDING  */
    PRECISION = 742,               /* PRECISION  */
    PREDICT = 743,                 /* PREDICT  */
    PREFERRED = 744,               /* PREFERRED  */
    PREFIX = 745,                  /* PREFIX  */
    PRESERVE = 746,                /* PRESERVE  */
    PREPARE = 747,                 /* PREPARE  */
    PREPARED = 748,                /* PREPARED  */
    PRIMARY = 749,                 /* PRIMARY  */
    PRECEDES_P = 750,              /* PRECEDES_P  */
    PRIVATE = 751,                 /* PRIVATE  */
    PRIOR = 752,                   /* PRIOR  */
    PRIORER = 753,                 /* PRIORER  */
    PRIVILEGES = 754,              /* PRIVILEGES  */
    PRIVILEGE = 755,               /* PRIVILEGE  */
    PROCEDURAL = 756,              /* PROCEDURAL  */
    PROCEDURE = 757,               /* PROCEDURE  */
    PROFILE = 758,                 /* PROFILE  */
    PUBLICATION = 759,             /* PUBLICATION  */
    PUBLISH = 760,                 /* PUBLISH  */
    PURGE = 761,                   /* PURGE  */
    QUERY = 762,                   /* QUERY  */
    QUOTE = 763,                   /* QUOTE  */
    RANDOMIZED = 764,              /* RANDOMIZED  */
    RANGE = 765,                   /* RANGE  */
    RATIO = 766,                   /* RATIO  */
    RAW = 767,                     /* RAW  */
    READ = 768,                    /* READ  */
    REAL = 769,                    /* REAL  */
    REASSIGN = 770,                /* REASSIGN  */
    REBUILD = 771,                 /* REBUILD  */
    RECHECK = 772,                 /* RECHECK  */
    RECURSIVE = 773,               /* RECURSIVE  */
    RECYCLEBIN = 774,              /* RECYCLEBIN  */
    REDISANYVALUE = 775,           /* REDISANYVALUE  */
    REF = 776,                     /* REF  */
    REFERENCES = 777,              /* REFERENCES  */
    REFRESH = 778,                 /* REFRESH  */
    REINDEX = 779,                 /* REINDEX  */
    REJECT_P = 780,                /* REJECT_P  */
    RELATIVE_P = 781,              /* RELATIVE_P  */
    RELEASE = 782,                 /* RELEASE  */
    RELOPTIONS = 783,              /* RELOPTIONS  */
    REMOTE_P = 784,                /* REMOTE_P  */
    REMOVE = 785,                  /* REMOVE  */
    RENAME = 786,                  /* RENAME  */
    REPEAT = 787,                  /* REPEAT  */
    REPEATABLE = 788,              /* REPEATABLE  */
    REPLACE = 789,                 /* REPLACE  */
    REPLICA = 790,                 /* REPLICA  */
    RESET = 791,                   /* RESET  */
    RESIZE = 792,                  /* RESIZE  */
    RESOURCE = 793,                /* RESOURCE  */
    RESPECT_P = 794,               /* RESPECT_P  */
    RESTART = 795,                 /* RESTART  */
    RESTRICT = 796,                /* RESTRICT  */
    RETURN = 797,                  /* RETURN  */
    RETURNED_SQLSTATE = 798,       /* RETURNED_SQLSTATE  */
    RETURNING = 799,               /* RETURNING  */
    RETURNS = 800,                 /* RETURNS  */
    REUSE = 801,                   /* REUSE  */
    REVOKE = 802,                  /* REVOKE  */
    RIGHT = 803,                   /* RIGHT  */
    ROLE = 804,                    /* ROLE  */
    ROLES = 805,                   /* ROLES  */
    ROLLBACK = 806,                /* ROLLBACK  */
    ROLLUP = 807,                  /* ROLLUP  */
    ROTATE = 808,                  /* ROTATE  */
    ROTATION = 809,                /* ROTATION  */
    ROW = 810,                     /* ROW  */
    ROW_COUNT = 811,               /* ROW_COUNT  */
    ROWNUM = 812,                  /* ROWNUM  */
    ROWS = 813,                    /* ROWS  */
    ROWTYPE_P = 814,               /* ROWTYPE_P  */
    RULE = 815,                    /* RULE  */
    SAMPLE = 816,                  /* SAMPLE  */
    SAVEPOINT = 817,               /* SAVEPOINT  */
    SCHEDULE = 818,                /* SCHEDULE  */
    SCHEMA = 819,                  /* SCHEMA  */
    SCHEMA_NAME = 820,             /* SCHEMA_NAME  */
    SCROLL = 821,                  /* SCROLL  */
    SEARCH = 822,                  /* SEARCH  */
    SECOND_P = 823,                /* SECOND_P  */
    SECURITY = 824,                /* SECURITY  */
    SELECT = 825,                  /* SELECT  */
    SEPARATOR_P = 826,             /* SEPARATOR_P  */
    SEQUENCE = 827,                /* SEQUENCE  */
    SEQUENCES = 828,               /* SEQUENCES  */
    SERIALIZABLE = 829,            /* SERIALIZABLE  */
    SERVER = 830,                  /* SERVER  */
    SESSION = 831,                 /* SESSION  */
    SESSION_USER = 832,            /* SESSION_USER  */
    SET = 833,                     /* SET  */
    SETS = 834,                    /* SETS  */
    SETOF = 835,                   /* SETOF  */
    SHARE = 836,                   /* SHARE  */
    SHIPPABLE = 837,               /* SHIPPABLE  */
    SHOW = 838,                    /* SHOW  */
    SHUTDOWN = 839,                /* SHUTDOWN  */
    SIBLINGS = 840,                /* SIBLINGS  */
    SIMILAR = 841,                 /* SIMILAR  */
    SIMPLE = 842,                  /* SIMPLE  */
    SIZE = 843,                    /* SIZE  */
    SKIP = 844,                    /* SKIP  */
    SLAVE = 845,                   /* SLAVE  */
    SLICE = 846,                   /* SLICE  */
    SMALLDATETIME = 847,           /* SMALLDATETIME  */
    SMALLDATETIME_FORMAT_P = 848,  /* SMALLDATETIME_FORMAT_P  */
    SMALLINT = 849,                /* SMALLINT  */
    SNAPSHOT = 850,                /* SNAPSHOT  */
    SOME = 851,                    /* SOME  */
    SOURCE_P = 852,                /* SOURCE_P  */
    SPACE = 853,                   /* SPACE  */
    SPECIFICATION = 854,           /* SPECIFICATION  */
    SPILL = 855,                   /* SPILL  */
    SPLIT = 856,                   /* SPLIT  */
    STABLE = 857,                  /* STABLE  */
    STACKED_P = 858,               /* STACKED_P  */
    STANDALONE_P = 859,            /* STANDALONE_P  */
    START = 860,                   /* START  */
    STARTS = 861,                  /* STARTS  */
    STARTWITH = 862,               /* STARTWITH  */
    STATEMENT = 863,               /* STATEMENT  */
    STATEMENT_ID = 864,            /* STATEMENT_ID  */
    STATISTICS = 865,              /* STATISTICS  */
    STDIN = 866,                   /* STDIN  */
    STDOUT = 867,                  /* STDOUT  */
    STORAGE = 868,                 /* STORAGE  */
    STORE_P = 869,                 /* STORE_P  */
    STORED = 870,                  /* STORED  */
    STRATIFY = 871,                /* STRATIFY  */
    STREAM = 872,                  /* STREAM  */
    STRICT_P = 873,                /* STRICT_P  */
    STRIP_P = 874,                 /* STRIP_P  */
    SUBCLASS_ORIGIN = 875,         /* SUBCLASS_ORIGIN  */
    SUBPARTITION = 876,            /* SUBPARTITION  */
    SUBPARTITIONS = 877,           /* SUBPARTITIONS  */
    SUBSCRIPTION = 878,            /* SUBSCRIPTION  */
    SUBSTRING = 879,               /* SUBSTRING  */
    SYMMETRIC = 880,               /* SYMMETRIC  */
    SYNONYM = 881,                 /* SYNONYM  */
    SYSDATE = 882,                 /* SYSDATE  */
    SYSID = 883,                   /* SYSID  */
    SYSTEM_P = 884,                /* SYSTEM_P  */
    SYS_REFCURSOR = 885,           /* SYS_REFCURSOR  */
    STARTING = 886,                /* STARTING  */
    SQL_P = 887,                   /* SQL_P  */
    TABLE = 888,                   /* TABLE  */
    TABLE_NAME = 889,              /* TABLE_NAME  */
    TABLES = 890,                  /* TABLES  */
    TABLESAMPLE = 891,             /* TABLESAMPLE  */
    TABLESPACE = 892,              /* TABLESPACE  */
    TARGET = 893,                  /* TARGET  */
    TEMP = 894,                    /* TEMP  */
    TEMPLATE = 895,                /* TEMPLATE  */
    TEMPORARY = 896,               /* TEMPORARY  */
    TERMINATED = 897,              /* TERMINATED  */
    TEXT_P = 898,                  /* TEXT_P  */
    THAN = 899,                    /* THAN  */
    THEN = 900,                    /* THEN  */
    TIME = 901,                    /* TIME  */
    TIME_FORMAT_P = 902,           /* TIME_FORMAT_P  */
    TIES = 903,                    /* TIES  */
    TIMECAPSULE = 904,             /* TIMECAPSULE  */
    TIMESTAMP = 905,               /* TIMESTAMP  */
    TIMESTAMP_FORMAT_P = 906,      /* TIMESTAMP_FORMAT_P  */
    TIMESTAMPDIFF = 907,           /* TIMESTAMPDIFF  */
    TIMEZONE_HOUR_P = 908,         /* TIMEZONE_HOUR_P  */
    TIMEZONE_MINUTE_P = 909,       /* TIMEZONE_MINUTE_P  */
    TINYINT = 910,                 /* TINYINT  */
    TO = 911,                      /* TO  */
    TRAILING = 912,                /* TRAILING  */
    TRANSACTION = 913,             /* TRANSACTION  */
    TRANSFORM = 914,               /* TRANSFORM  */
    TREAT = 915,                   /* TREAT  */
    TRIGGER = 916,                 /* TRIGGER  */
    TRIM = 917,                    /* TRIM  */
    TRUE_P = 918,                  /* TRUE_P  */
    TRUNCATE = 919,                /* TRUNCATE  */
    TRUSTED = 920,                 /* TRUSTED  */
    TSFIELD = 921,                 /* TSFIELD  */
    TSTAG = 922,                   /* TSTAG  */
    TSTIME = 923,                  /* TSTIME  */
    TYPE_P = 924,                  /* TYPE_P  */
    TYPES_P = 925,                 /* TYPES_P  */
    UNBOUNDED = 926,               /* UNBOUNDED  */
    UNCOMMITTED = 927,             /* UNCOMMITTED  */
    UNENCRYPTED = 928,             /* UNENCRYPTED  */
    UNION = 929,                   /* UNION  */
    UNIQUE = 930,                  /* UNIQUE  */
    UNKNOWN = 931,                 /* UNKNOWN  */
    UNLIMITED = 932,               /* UNLIMITED  */
    UNLISTEN = 933,                /* UNLISTEN  */
    UNLOCK = 934,                  /* UNLOCK  */
    UNLOGGED = 935,                /* UNLOGGED  */
    UNIMCSTORED = 936,             /* UNIMCSTORED  */
    UNTIL = 937,                   /* UNTIL  */
    UNUSABLE = 938,                /* UNUSABLE  */
    UPDATE = 939,                  /* UPDATE  */
    USEEOF = 940,                  /* USEEOF  */
    USER = 941,                    /* USER  */
    USING = 942,                   /* USING  */
    VACUUM = 943,                  /* VACUUM  */
    VALID = 944,                   /* VALID  */
    VALIDATE = 945,                /* VALIDATE  */
    VALIDATION = 946,              /* VALIDATION  */
    VALIDATOR = 947,               /* VALIDATOR  */
    VALUE_P = 948,                 /* VALUE_P  */
    VALUES = 949,                  /* VALUES  */
    VARCHAR = 950,                 /* VARCHAR  */
    VARCHAR2 = 951,                /* VARCHAR2  */
    VARIABLES = 952,               /* VARIABLES  */
    VARIADIC = 953,                /* VARIADIC  */
    VARRAY = 954,                  /* VARRAY  */
    VARYING = 955,                 /* VARYING  */
    VCGROUP = 956,                 /* VCGROUP  */
    VERBOSE = 957,                 /* VERBOSE  */
    VERIFY = 958,                  /* VERIFY  */
    VERSION_P = 959,               /* VERSION_P  */
    VIEW = 960,                    /* VIEW  */
    VISIBLE = 961,                 /* VISIBLE  */
    VOLATILE = 962,                /* VOLATILE  */
    WAIT = 963,                    /* WAIT  */
    WARNINGS = 964,                /* WARNINGS  */
    WEAK = 965,                    /* WEAK  */
    WHEN = 966,                    /* WHEN  */
    WHERE = 967,                   /* WHERE  */
    WHILE_P = 968,                 /* WHILE_P  */
    WHITESPACE_P = 969,            /* WHITESPACE_P  */
    WINDOW = 970,                  /* WINDOW  */
    WITH = 971,                    /* WITH  */
    WITHIN = 972,                  /* WITHIN  */
    WITHOUT = 973,                 /* WITHOUT  */
    WORK = 974,                    /* WORK  */
    WORKLOAD = 975,                /* WORKLOAD  */
    WRAPPER = 976,                 /* WRAPPER  */
    WRITE = 977,                   /* WRITE  */
    XML_P = 978,                   /* XML_P  */
    XMLATTRIBUTES = 979,           /* XMLATTRIBUTES  */
    XMLCONCAT = 980,               /* XMLCONCAT  */
    XMLELEMENT = 981,              /* XMLELEMENT  */
    XMLEXISTS = 982,               /* XMLEXISTS  */
    XMLFOREST = 983,               /* XMLFOREST  */
    XMLPARSE = 984,                /* XMLPARSE  */
    XMLPI = 985,                   /* XMLPI  */
    XMLROOT = 986,                 /* XMLROOT  */
    XMLSERIALIZE = 987,            /* XMLSERIALIZE  */
    YEAR_P = 988,                  /* YEAR_P  */
    YEAR_MONTH_P = 989,            /* YEAR_MONTH_P  */
    YES_P = 990,                   /* YES_P  */
    ZONE = 991,                    /* ZONE  */
    NULLS_FIRST = 992,             /* NULLS_FIRST  */
    NULLS_LAST = 993,              /* NULLS_LAST  */
    WITH_TIME = 994,               /* WITH_TIME  */
    INCLUDING_ALL = 995,           /* INCLUDING_ALL  */
    RENAME_PARTITION = 996,        /* RENAME_PARTITION  */
    PARTITION_FOR = 997,           /* PARTITION_FOR  */
    SUBPARTITION_FOR = 998,        /* SUBPARTITION_FOR  */
    ADD_PARTITION = 999,           /* ADD_PARTITION  */
    DROP_PARTITION = 1000,         /* DROP_PARTITION  */
    REBUILD_PARTITION = 1001,      /* REBUILD_PARTITION  */
    MODIFY_PARTITION = 1002,       /* MODIFY_PARTITION  */
    ADD_SUBPARTITION = 1003,       /* ADD_SUBPARTITION  */
    DROP_SUBPARTITION = 1004,      /* DROP_SUBPARTITION  */
    NOT_ENFORCED = 1005,           /* NOT_ENFORCED  */
    VALID_BEGIN = 1006,            /* VALID_BEGIN  */
    DECLARE_CURSOR = 1007,         /* DECLARE_CURSOR  */
    ON_UPDATE_TIME = 1008,         /* ON_UPDATE_TIME  */
    START_WITH = 1009,             /* START_WITH  */
    CONNECT_BY = 1010,             /* CONNECT_BY  */
    SHOW_ERRORS = 1011,            /* SHOW_ERRORS  */
    END_OF_INPUT = 1012,           /* END_OF_INPUT  */
    END_OF_INPUT_COLON = 1013,     /* END_OF_INPUT_COLON  */
    END_OF_PROC = 1014,            /* END_OF_PROC  */
    EVENT_TRIGGER = 1015,          /* EVENT_TRIGGER  */
    NOT_IN = 1016,                 /* NOT_IN  */
    NOT_BETWEEN = 1017,            /* NOT_BETWEEN  */
    NOT_LIKE = 1018,               /* NOT_LIKE  */
    NOT_ILIKE = 1019,              /* NOT_ILIKE  */
    NOT_SIMILAR = 1020,            /* NOT_SIMILAR  */
    FORCE_INDEX = 1021,            /* FORCE_INDEX  */
    USE_INDEX = 1022,              /* USE_INDEX  */
    IGNORE_INDEX = 1023,           /* IGNORE_INDEX  */
    CURSOR_EXPR = 1024,            /* CURSOR_EXPR  */
    LATERAL_EXPR = 1025,           /* LATERAL_EXPR  */
    FALSE_ON_ERROR = 1026,         /* FALSE_ON_ERROR  */
    TRUE_ON_ERROR = 1027,          /* TRUE_ON_ERROR  */
    ERROR_ON_ERROR = 1028,         /* ERROR_ON_ERROR  */
    PARTIAL_EMPTY_PREC = 1029,     /* PARTIAL_EMPTY_PREC  */
    POSTFIXOP = 1030,              /* POSTFIXOP  */
    lower_than_index = 1031,       /* lower_than_index  */
    lower_than_under = 1032,       /* lower_than_under  */
    higher_than_rotate = 1033,     /* higher_than_rotate  */
    UMINUS = 1034,                 /* UMINUS  */
    EMPTY_FROM_CLAUSE = 1035,      /* EMPTY_FROM_CLAUSE  */
    CONSTRUCTOR = 1036,            /* CONSTRUCTOR  */
    FINAL = 1037,                  /* FINAL  */
    MAP = 1038,                    /* MAP  */
    MEMBER = 1039,                 /* MEMBER  */
    RESULT = 1040,                 /* RESULT  */
    SELF = 1041,                   /* SELF  */
    STATIC_P = 1042,               /* STATIC_P  */
    UNDER = 1043,                  /* UNDER  */
    SELF_INOUT = 1044,             /* SELF_INOUT  */
    STATIC_FUNCTION = 1045,        /* STATIC_FUNCTION  */
    MEMBER_FUNCTION = 1046,        /* MEMBER_FUNCTION  */
    STATIC_PROCEDURE = 1047,       /* STATIC_PROCEDURE  */
    MEMBER_PROCEDURE = 1048,       /* MEMBER_PROCEDURE  */
    CONSTRUCTOR_FUNCTION = 1049,   /* CONSTRUCTOR_FUNCTION  */
    MAP_MEMBER = 1050              /* MAP_MEMBER  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 326 "gram.y"

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

#line 917 "gram.hpp"

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




int base_yyparse (core_yyscan_t yyscanner);


#endif /* !YY_BASE_YY_GRAM_HPP_INCLUDED  */
