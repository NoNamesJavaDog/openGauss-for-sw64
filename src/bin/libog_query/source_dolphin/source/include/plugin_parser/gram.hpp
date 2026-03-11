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
    FCONST = 258,                  /* FCONST  */
    SCONST = 259,                  /* SCONST  */
    BCONST = 260,                  /* BCONST  */
    VCONST = 261,                  /* VCONST  */
    XCONST = 262,                  /* XCONST  */
    Op = 263,                      /* Op  */
    CmpOp = 264,                   /* CmpOp  */
    CmpNullOp = 265,               /* CmpNullOp  */
    JsonOp = 266,                  /* JsonOp  */
    JsonOpText = 267,              /* JsonOpText  */
    COMMENTSTRING = 268,           /* COMMENTSTRING  */
    SET_USER_IDENT = 269,          /* SET_USER_IDENT  */
    SET_IDENT = 270,               /* SET_IDENT  */
    UNDERSCORE_CHARSET = 271,      /* UNDERSCORE_CHARSET  */
    FCONST_F = 272,                /* FCONST_F  */
    FCONST_D = 273,                /* FCONST_D  */
    OR_OR_SYM = 274,               /* OR_OR_SYM  */
    ICONST = 275,                  /* ICONST  */
    PARAM = 276,                   /* PARAM  */
    TYPECAST = 277,                /* TYPECAST  */
    ORA_JOINOP = 278,              /* ORA_JOINOP  */
    DOT_DOT = 279,                 /* DOT_DOT  */
    COLON_EQUALS = 280,            /* COLON_EQUALS  */
    PARA_EQUALS = 281,             /* PARA_EQUALS  */
    SET_IDENT_SESSION = 282,       /* SET_IDENT_SESSION  */
    SET_IDENT_GLOBAL = 283,        /* SET_IDENT_GLOBAL  */
    IDENT = 284,                   /* IDENT  */
    ABORT_P = 285,                 /* ABORT_P  */
    ABSOLUTE_P = 286,              /* ABSOLUTE_P  */
    ACCESS = 287,                  /* ACCESS  */
    ACCOUNT = 288,                 /* ACCOUNT  */
    ACTION = 289,                  /* ACTION  */
    ADD_P = 290,                   /* ADD_P  */
    ADMIN = 291,                   /* ADMIN  */
    AFTER = 292,                   /* AFTER  */
    AGGREGATE = 293,               /* AGGREGATE  */
    ALGORITHM = 294,               /* ALGORITHM  */
    ALL = 295,                     /* ALL  */
    ALSO = 296,                    /* ALSO  */
    ALTER = 297,                   /* ALTER  */
    ALWAYS = 298,                  /* ALWAYS  */
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
    ASCII = 309,                   /* ASCII  */
    ASOF_P = 310,                  /* ASOF_P  */
    ASSERTION = 311,               /* ASSERTION  */
    ASSIGNMENT = 312,              /* ASSIGNMENT  */
    ASYMMETRIC = 313,              /* ASYMMETRIC  */
    AT = 314,                      /* AT  */
    ATTRIBUTE = 315,               /* ATTRIBUTE  */
    AUDIT = 316,                   /* AUDIT  */
    AUTHID = 317,                  /* AUTHID  */
    AUTHORIZATION = 318,           /* AUTHORIZATION  */
    AUTOEXTEND = 319,              /* AUTOEXTEND  */
    AUTOEXTEND_SIZE = 320,         /* AUTOEXTEND_SIZE  */
    AUTOMAPPED = 321,              /* AUTOMAPPED  */
    AUTO_INCREMENT = 322,          /* AUTO_INCREMENT  */
    AVG_ROW_LENGTH = 323,          /* AVG_ROW_LENGTH  */
    AGAINST = 324,                 /* AGAINST  */
    BACKWARD = 325,                /* BACKWARD  */
    BARRIER = 326,                 /* BARRIER  */
    BEFORE = 327,                  /* BEFORE  */
    BEGIN_NON_ANOYBLOCK = 328,     /* BEGIN_NON_ANOYBLOCK  */
    BEGIN_P = 329,                 /* BEGIN_P  */
    BETWEEN = 330,                 /* BETWEEN  */
    BIGINT = 331,                  /* BIGINT  */
    BINARY = 332,                  /* BINARY  */
    BINARY_P = 333,                /* BINARY_P  */
    BINARY_DOUBLE = 334,           /* BINARY_DOUBLE  */
    BINARY_DOUBLE_INF = 335,       /* BINARY_DOUBLE_INF  */
    BINARY_DOUBLE_NAN = 336,       /* BINARY_DOUBLE_NAN  */
    BINARY_INTEGER = 337,          /* BINARY_INTEGER  */
    BIT = 338,                     /* BIT  */
    BLANKS = 339,                  /* BLANKS  */
    BLOB_P = 340,                  /* BLOB_P  */
    BLOCKCHAIN = 341,              /* BLOCKCHAIN  */
    BODY_P = 342,                  /* BODY_P  */
    BOGUS = 343,                   /* BOGUS  */
    BOOLEAN_P = 344,               /* BOOLEAN_P  */
    BOTH = 345,                    /* BOTH  */
    BUCKETCNT = 346,               /* BUCKETCNT  */
    BUILD = 347,                   /* BUILD  */
    BY = 348,                      /* BY  */
    BYTEAWITHOUTORDER = 349,       /* BYTEAWITHOUTORDER  */
    BYTEAWITHOUTORDERWITHEQUAL = 350, /* BYTEAWITHOUTORDERWITHEQUAL  */
    CACHE = 351,                   /* CACHE  */
    CALL = 352,                    /* CALL  */
    CALLED = 353,                  /* CALLED  */
    CANCELABLE = 354,              /* CANCELABLE  */
    CASCADE = 355,                 /* CASCADE  */
    CASCADED = 356,                /* CASCADED  */
    CASE = 357,                    /* CASE  */
    CAST = 358,                    /* CAST  */
    CATALOG_P = 359,               /* CATALOG_P  */
    CATALOG_NAME = 360,            /* CATALOG_NAME  */
    CHAIN = 361,                   /* CHAIN  */
    CHANGE = 362,                  /* CHANGE  */
    CHANNEL = 363,                 /* CHANNEL  */
    CHAR_P = 364,                  /* CHAR_P  */
    CHARACTER = 365,               /* CHARACTER  */
    CHARACTERISTICS = 366,         /* CHARACTERISTICS  */
    CHARACTERSET = 367,            /* CHARACTERSET  */
    CHARSET = 368,                 /* CHARSET  */
    CHECK = 369,                   /* CHECK  */
    CHECKPOINT = 370,              /* CHECKPOINT  */
    CHECKSUM = 371,                /* CHECKSUM  */
    CLASS = 372,                   /* CLASS  */
    CLASS_ORIGIN = 373,            /* CLASS_ORIGIN  */
    CLEAN = 374,                   /* CLEAN  */
    CLIENT = 375,                  /* CLIENT  */
    CLIENT_MASTER_KEY = 376,       /* CLIENT_MASTER_KEY  */
    CLIENT_MASTER_KEYS = 377,      /* CLIENT_MASTER_KEYS  */
    CLOB = 378,                    /* CLOB  */
    CLOSE = 379,                   /* CLOSE  */
    CLUSTER = 380,                 /* CLUSTER  */
    COALESCE = 381,                /* COALESCE  */
    COLLATE = 382,                 /* COLLATE  */
    COLLATION = 383,               /* COLLATION  */
    COLUMN = 384,                  /* COLUMN  */
    COLUMN_NAME = 385,             /* COLUMN_NAME  */
    COLUMN_ENCRYPTION_KEY = 386,   /* COLUMN_ENCRYPTION_KEY  */
    COLUMN_ENCRYPTION_KEYS = 387,  /* COLUMN_ENCRYPTION_KEYS  */
    COLUMNS = 388,                 /* COLUMNS  */
    COMMENT = 389,                 /* COMMENT  */
    COMMENTS = 390,                /* COMMENTS  */
    COMMIT = 391,                  /* COMMIT  */
    CONSISTENT = 392,              /* CONSISTENT  */
    COMMITTED = 393,               /* COMMITTED  */
    COMPATIBLE_ILLEGAL_CHARS = 394, /* COMPATIBLE_ILLEGAL_CHARS  */
    COMPILE = 395,                 /* COMPILE  */
    COMPLETE = 396,                /* COMPLETE  */
    COMPLETION = 397,              /* COMPLETION  */
    COMPRESS = 398,                /* COMPRESS  */
    COMPRESSION = 399,             /* COMPRESSION  */
    CONCURRENTLY = 400,            /* CONCURRENTLY  */
    CONDITION = 401,               /* CONDITION  */
    CONFIGURATION = 402,           /* CONFIGURATION  */
    CONNECTION = 403,              /* CONNECTION  */
    CONSTANT = 404,                /* CONSTANT  */
    CONSTRAINT = 405,              /* CONSTRAINT  */
    CONSTRAINT_CATALOG = 406,      /* CONSTRAINT_CATALOG  */
    CONSTRAINT_NAME = 407,         /* CONSTRAINT_NAME  */
    CONSTRAINT_SCHEMA = 408,       /* CONSTRAINT_SCHEMA  */
    CONSTRAINTS = 409,             /* CONSTRAINTS  */
    CONTAINS = 410,                /* CONTAINS  */
    CONTENT_P = 411,               /* CONTENT_P  */
    CONTINUE_P = 412,              /* CONTINUE_P  */
    CONTVIEW = 413,                /* CONTVIEW  */
    CONVERSION_P = 414,            /* CONVERSION_P  */
    CONVERT = 415,                 /* CONVERT  */
    CONNECT = 416,                 /* CONNECT  */
    COORDINATOR = 417,             /* COORDINATOR  */
    COORDINATORS = 418,            /* COORDINATORS  */
    COPY = 419,                    /* COPY  */
    COST = 420,                    /* COST  */
    COUNT = 421,                   /* COUNT  */
    CREATE = 422,                  /* CREATE  */
    CROSS = 423,                   /* CROSS  */
    CSN = 424,                     /* CSN  */
    CSV = 425,                     /* CSV  */
    CUBE = 426,                    /* CUBE  */
    CURRENT_P = 427,               /* CURRENT_P  */
    CURRENT_CATALOG = 428,         /* CURRENT_CATALOG  */
    CURRENT_DATE = 429,            /* CURRENT_DATE  */
    CURRENT_ROLE = 430,            /* CURRENT_ROLE  */
    CURRENT_SCHEMA = 431,          /* CURRENT_SCHEMA  */
    CURRENT_TIME = 432,            /* CURRENT_TIME  */
    CURTIME = 433,                 /* CURTIME  */
    CURRENT_TIMESTAMP = 434,       /* CURRENT_TIMESTAMP  */
    CURRENT_USER = 435,            /* CURRENT_USER  */
    CURSOR = 436,                  /* CURSOR  */
    CURSOR_NAME = 437,             /* CURSOR_NAME  */
    CYCLE = 438,                   /* CYCLE  */
    NOW_FUNC = 439,                /* NOW_FUNC  */
    SHRINK = 440,                  /* SHRINK  */
    DATA_P = 441,                  /* DATA_P  */
    DATABASE = 442,                /* DATABASE  */
    DATABASES = 443,               /* DATABASES  */
    DATAFILE = 444,                /* DATAFILE  */
    DATANODE = 445,                /* DATANODE  */
    DATANODES = 446,               /* DATANODES  */
    DATATYPE_CL = 447,             /* DATATYPE_CL  */
    DATE_P = 448,                  /* DATE_P  */
    DATETIME = 449,                /* DATETIME  */
    DATE_ADD_P = 450,              /* DATE_ADD_P  */
    DATE_FORMAT_P = 451,           /* DATE_FORMAT_P  */
    DATE_SUB_P = 452,              /* DATE_SUB_P  */
    DAY_P = 453,                   /* DAY_P  */
    DAY_HOUR_P = 454,              /* DAY_HOUR_P  */
    DAY_MICROSECOND_P = 455,       /* DAY_MICROSECOND_P  */
    DAY_MINUTE_P = 456,            /* DAY_MINUTE_P  */
    DAY_SECOND_P = 457,            /* DAY_SECOND_P  */
    DAYOFMONTH = 458,              /* DAYOFMONTH  */
    DAYOFWEEK = 459,               /* DAYOFWEEK  */
    DAYOFYEAR = 460,               /* DAYOFYEAR  */
    DBCOMPATIBILITY_P = 461,       /* DBCOMPATIBILITY_P  */
    DB_B_FORMAT = 462,             /* DB_B_FORMAT  */
    DB_B_JSOBJ = 463,              /* DB_B_JSOBJ  */
    DEALLOCATE = 464,              /* DEALLOCATE  */
    DEC = 465,                     /* DEC  */
    DECIMAL_P = 466,               /* DECIMAL_P  */
    DECLARE = 467,                 /* DECLARE  */
    DECODE = 468,                  /* DECODE  */
    DEFAULT = 469,                 /* DEFAULT  */
    DEFAULTS = 470,                /* DEFAULTS  */
    DEFERRABLE = 471,              /* DEFERRABLE  */
    DEFERRED = 472,                /* DEFERRED  */
    DEFINER = 473,                 /* DEFINER  */
    DELAYED = 474,                 /* DELAYED  */
    DELAY_KEY_WRITE = 475,         /* DELAY_KEY_WRITE  */
    DELETE_P = 476,                /* DELETE_P  */
    DELIMITER = 477,               /* DELIMITER  */
    DELIMITERS = 478,              /* DELIMITERS  */
    DELTA = 479,                   /* DELTA  */
    DELTAMERGE = 480,              /* DELTAMERGE  */
    DENSE_RANK = 481,              /* DENSE_RANK  */
    DESC = 482,                    /* DESC  */
    DESCRIBE = 483,                /* DESCRIBE  */
    DETERMINISTIC = 484,           /* DETERMINISTIC  */
    DISK = 485,                    /* DISK  */
    DIV = 486,                     /* DIV  */
    DIAGNOSTICS = 487,             /* DIAGNOSTICS  */
    DICTIONARY = 488,              /* DICTIONARY  */
    DIRECT = 489,                  /* DIRECT  */
    DIRECTORY = 490,               /* DIRECTORY  */
    DISABLE_P = 491,               /* DISABLE_P  */
    DISCARD = 492,                 /* DISCARD  */
    DISTINCT = 493,                /* DISTINCT  */
    DISTINCTROW = 494,             /* DISTINCTROW  */
    DISTRIBUTE = 495,              /* DISTRIBUTE  */
    DISTRIBUTION = 496,            /* DISTRIBUTION  */
    DO = 497,                      /* DO  */
    DOCUMENT_P = 498,              /* DOCUMENT_P  */
    DOMAIN_P = 499,                /* DOMAIN_P  */
    DOUBLE_P = 500,                /* DOUBLE_P  */
    DUAL_P = 501,                  /* DUAL_P  */
    DROP = 502,                    /* DROP  */
    DUPLICATE = 503,               /* DUPLICATE  */
    DISCONNECT = 504,              /* DISCONNECT  */
    DUMPFILE = 505,                /* DUMPFILE  */
    EACH = 506,                    /* EACH  */
    ELASTIC = 507,                 /* ELASTIC  */
    ELSE = 508,                    /* ELSE  */
    ENABLE_P = 509,                /* ENABLE_P  */
    ENCLOSED = 510,                /* ENCLOSED  */
    ENCODING = 511,                /* ENCODING  */
    ENCRYPTED = 512,               /* ENCRYPTED  */
    ENCRYPTED_VALUE = 513,         /* ENCRYPTED_VALUE  */
    ENCRYPTION = 514,              /* ENCRYPTION  */
    ENCRYPTION_TYPE = 515,         /* ENCRYPTION_TYPE  */
    END_P = 516,                   /* END_P  */
    ENDS = 517,                    /* ENDS  */
    ENFORCED = 518,                /* ENFORCED  */
    ENGINE_ATTRIBUTE = 519,        /* ENGINE_ATTRIBUTE  */
    ENGINE_P = 520,                /* ENGINE_P  */
    ENGINES = 521,                 /* ENGINES  */
    ENUM_P = 522,                  /* ENUM_P  */
    ERROR_P = 523,                 /* ERROR_P  */
    ERRORS = 524,                  /* ERRORS  */
    ESCAPE = 525,                  /* ESCAPE  */
    ESCAPED = 526,                 /* ESCAPED  */
    EOL = 527,                     /* EOL  */
    ESCAPING = 528,                /* ESCAPING  */
    EVENT = 529,                   /* EVENT  */
    EVENTS = 530,                  /* EVENTS  */
    EVERY = 531,                   /* EVERY  */
    EXCEPT = 532,                  /* EXCEPT  */
    EXCHANGE = 533,                /* EXCHANGE  */
    EXCLUDE = 534,                 /* EXCLUDE  */
    EXCLUDING = 535,               /* EXCLUDING  */
    EXCLUSIVE = 536,               /* EXCLUSIVE  */
    EXECUTE = 537,                 /* EXECUTE  */
    EXISTS = 538,                  /* EXISTS  */
    EXPANSION = 539,               /* EXPANSION  */
    EXPIRED_P = 540,               /* EXPIRED_P  */
    EXPLAIN = 541,                 /* EXPLAIN  */
    EXTENDED = 542,                /* EXTENDED  */
    EXTENSION = 543,               /* EXTENSION  */
    EXTERNAL = 544,                /* EXTERNAL  */
    EXTRACT = 545,                 /* EXTRACT  */
    FALSE_P = 546,                 /* FALSE_P  */
    FAMILY = 547,                  /* FAMILY  */
    FAST = 548,                    /* FAST  */
    FENCED = 549,                  /* FENCED  */
    FETCH = 550,                   /* FETCH  */
    FIELDS = 551,                  /* FIELDS  */
    FILEHEADER_P = 552,            /* FILEHEADER_P  */
    FILL_MISSING_FIELDS = 553,     /* FILL_MISSING_FIELDS  */
    FILLER = 554,                  /* FILLER  */
    FILTER = 555,                  /* FILTER  */
    FIRST_P = 556,                 /* FIRST_P  */
    FIXED_P = 557,                 /* FIXED_P  */
    FLOAT_P = 558,                 /* FLOAT_P  */
    FLUSH = 559,                   /* FLUSH  */
    FOLLOWING = 560,               /* FOLLOWING  */
    FOLLOWS_P = 561,               /* FOLLOWS_P  */
    FOR = 562,                     /* FOR  */
    FORCE = 563,                   /* FORCE  */
    FOREIGN = 564,                 /* FOREIGN  */
    FORMATTER = 565,               /* FORMATTER  */
    FORWARD = 566,                 /* FORWARD  */
    FEATURES = 567,                /* FEATURES  */
    FREEZE = 568,                  /* FREEZE  */
    FROM = 569,                    /* FROM  */
    FULL = 570,                    /* FULL  */
    FULLTEXT = 571,                /* FULLTEXT  */
    FUNCTION = 572,                /* FUNCTION  */
    FUNCTIONS = 573,               /* FUNCTIONS  */
    GENERATED = 574,               /* GENERATED  */
    GET = 575,                     /* GET  */
    GET_FORMAT = 576,              /* GET_FORMAT  */
    GLOBAL = 577,                  /* GLOBAL  */
    GRANT = 578,                   /* GRANT  */
    GRANTED = 579,                 /* GRANTED  */
    GREATEST = 580,                /* GREATEST  */
    GROUP_P = 581,                 /* GROUP_P  */
    GROUPING_P = 582,              /* GROUPING_P  */
    GROUPPARENT = 583,             /* GROUPPARENT  */
    GRANTS = 584,                  /* GRANTS  */
    TRIGGERS = 585,                /* TRIGGERS  */
    HANDLER = 586,                 /* HANDLER  */
    HAVING = 587,                  /* HAVING  */
    HDFSDIRECTORY = 588,           /* HDFSDIRECTORY  */
    HEADER_P = 589,                /* HEADER_P  */
    HOLD = 590,                    /* HOLD  */
    HOSTS = 591,                   /* HOSTS  */
    HOUR_P = 592,                  /* HOUR_P  */
    HOUR_MICROSECOND_P = 593,      /* HOUR_MICROSECOND_P  */
    HOUR_MINUTE_P = 594,           /* HOUR_MINUTE_P  */
    HOUR_SECOND_P = 595,           /* HOUR_SECOND_P  */
    IDENTIFIED = 596,              /* IDENTIFIED  */
    IDENTITY_P = 597,              /* IDENTITY_P  */
    IF_P = 598,                    /* IF_P  */
    IFNULL = 599,                  /* IFNULL  */
    IGNORE = 600,                  /* IGNORE  */
    IGNORE_EXTRA_DATA = 601,       /* IGNORE_EXTRA_DATA  */
    ILIKE = 602,                   /* ILIKE  */
    IMMEDIATE = 603,               /* IMMEDIATE  */
    IMMUTABLE = 604,               /* IMMUTABLE  */
    IMPLICIT_P = 605,              /* IMPLICIT_P  */
    IN_P = 606,                    /* IN_P  */
    INCLUDE = 607,                 /* INCLUDE  */
    IMCSTORED = 608,               /* IMCSTORED  */
    INCLUDING = 609,               /* INCLUDING  */
    INCREMENT = 610,               /* INCREMENT  */
    INCREMENTAL = 611,             /* INCREMENTAL  */
    INDEX = 612,                   /* INDEX  */
    INDEXES = 613,                 /* INDEXES  */
    INFILE = 614,                  /* INFILE  */
    INFINITE_P = 615,              /* INFINITE_P  */
    INHERIT = 616,                 /* INHERIT  */
    INHERITS = 617,                /* INHERITS  */
    INITIAL_P = 618,               /* INITIAL_P  */
    INITIALLY = 619,               /* INITIALLY  */
    INITRANS = 620,                /* INITRANS  */
    INLINE_P = 621,                /* INLINE_P  */
    INNER_P = 622,                 /* INNER_P  */
    INOUT = 623,                   /* INOUT  */
    INPLACE = 624,                 /* INPLACE  */
    INPUT_P = 625,                 /* INPUT_P  */
    INSENSITIVE = 626,             /* INSENSITIVE  */
    INSERT = 627,                  /* INSERT  */
    INSERT_METHOD = 628,           /* INSERT_METHOD  */
    INSTEAD = 629,                 /* INSTEAD  */
    INT_P = 630,                   /* INT_P  */
    INTEGER = 631,                 /* INTEGER  */
    INTERNAL = 632,                /* INTERNAL  */
    INTERSECT = 633,               /* INTERSECT  */
    INTERVAL = 634,                /* INTERVAL  */
    INTERVAL_S = 635,              /* INTERVAL_S  */
    INTERVAL_TYPE_YEAR = 636,      /* INTERVAL_TYPE_YEAR  */
    INTERVAL_TYPE_QUARTER = 637,   /* INTERVAL_TYPE_QUARTER  */
    INTERVAL_TYPE_MONTH = 638,     /* INTERVAL_TYPE_MONTH  */
    INTERVAL_TYPE_WEEK = 639,      /* INTERVAL_TYPE_WEEK  */
    INTERVAL_TYPE_DAY = 640,       /* INTERVAL_TYPE_DAY  */
    INTERVAL_TYPE_HOUR = 641,      /* INTERVAL_TYPE_HOUR  */
    INTERVAL_TYPE_MINUTE = 642,    /* INTERVAL_TYPE_MINUTE  */
    INTERVAL_TYPE_SECOND = 643,    /* INTERVAL_TYPE_SECOND  */
    INTERVAL_TYPE_MICROSECOND = 644, /* INTERVAL_TYPE_MICROSECOND  */
    INTERVAL_TYPE_YEAR_MONTH = 645, /* INTERVAL_TYPE_YEAR_MONTH  */
    INTERVAL_TYPE_DAY_HOUR = 646,  /* INTERVAL_TYPE_DAY_HOUR  */
    INTERVAL_TYPE_DAY_MINUTE = 647, /* INTERVAL_TYPE_DAY_MINUTE  */
    INTERVAL_TYPE_DAY_SECOND = 648, /* INTERVAL_TYPE_DAY_SECOND  */
    INTERVAL_TYPE_DAY_MICROSECOND = 649, /* INTERVAL_TYPE_DAY_MICROSECOND  */
    INTERVAL_TYPE_HOUR_MINUTE = 650, /* INTERVAL_TYPE_HOUR_MINUTE  */
    INTERVAL_TYPE_HOUR_SECOND = 651, /* INTERVAL_TYPE_HOUR_SECOND  */
    INTERVAL_TYPE_HOUR_MICROSECOND = 652, /* INTERVAL_TYPE_HOUR_MICROSECOND  */
    INTERVAL_TYPE_MINUTE_SECOND = 653, /* INTERVAL_TYPE_MINUTE_SECOND  */
    INTERVAL_TYPE_MINUTE_MICROSECOND = 654, /* INTERVAL_TYPE_MINUTE_MICROSECOND  */
    INTERVAL_TYPE_SECOND_MICROSECOND = 655, /* INTERVAL_TYPE_SECOND_MICROSECOND  */
    INTO = 656,                    /* INTO  */
    INVISIBLE = 657,               /* INVISIBLE  */
    INVOKER = 658,                 /* INVOKER  */
    IP = 659,                      /* IP  */
    IS = 660,                      /* IS  */
    ISNULL = 661,                  /* ISNULL  */
    ISOLATION = 662,               /* ISOLATION  */
    JOIN = 663,                    /* JOIN  */
    JSON_EXISTS = 664,             /* JSON_EXISTS  */
    KEEP = 665,                    /* KEEP  */
    KEY = 666,                     /* KEY  */
    KEY_BLOCK_SIZE = 667,          /* KEY_BLOCK_SIZE  */
    KEYS = 668,                    /* KEYS  */
    KILL = 669,                    /* KILL  */
    KEY_PATH = 670,                /* KEY_PATH  */
    KEY_STORE = 671,               /* KEY_STORE  */
    LABEL = 672,                   /* LABEL  */
    LANGUAGE = 673,                /* LANGUAGE  */
    LARGE_P = 674,                 /* LARGE_P  */
    LAST_DAY_FUNC = 675,           /* LAST_DAY_FUNC  */
    LAST_P = 676,                  /* LAST_P  */
    LATERAL_P = 677,               /* LATERAL_P  */
    LC_COLLATE_P = 678,            /* LC_COLLATE_P  */
    LC_CTYPE_P = 679,              /* LC_CTYPE_P  */
    LEADING = 680,                 /* LEADING  */
    LEAKPROOF = 681,               /* LEAKPROOF  */
    LEAST = 682,                   /* LEAST  */
    LESS = 683,                    /* LESS  */
    LEFT = 684,                    /* LEFT  */
    LEVEL = 685,                   /* LEVEL  */
    LIKE = 686,                    /* LIKE  */
    LINES = 687,                   /* LINES  */
    LIMIT = 688,                   /* LIMIT  */
    LIST = 689,                    /* LIST  */
    LISTEN = 690,                  /* LISTEN  */
    LOAD = 691,                    /* LOAD  */
    LOCAL = 692,                   /* LOCAL  */
    LOCALTIME = 693,               /* LOCALTIME  */
    LOCALTIMESTAMP = 694,          /* LOCALTIMESTAMP  */
    LOCATE = 695,                  /* LOCATE  */
    LOCATION = 696,                /* LOCATION  */
    LOCK_P = 697,                  /* LOCK_P  */
    LOCKED = 698,                  /* LOCKED  */
    LOG_P = 699,                   /* LOG_P  */
    LOGGING = 700,                 /* LOGGING  */
    LOGIN_ANY = 701,               /* LOGIN_ANY  */
    LOGIN_FAILURE = 702,           /* LOGIN_FAILURE  */
    LOGIN_SUCCESS = 703,           /* LOGIN_SUCCESS  */
    LOGOUT = 704,                  /* LOGOUT  */
    LOGS = 705,                    /* LOGS  */
    LOOP = 706,                    /* LOOP  */
    LOW_PRIORITY = 707,            /* LOW_PRIORITY  */
    MAPPING = 708,                 /* MAPPING  */
    MASKING = 709,                 /* MASKING  */
    MASTER = 710,                  /* MASTER  */
    MATCH = 711,                   /* MATCH  */
    MATERIALIZED = 712,            /* MATERIALIZED  */
    MATCHED = 713,                 /* MATCHED  */
    MAXEXTENTS = 714,              /* MAXEXTENTS  */
    MAXSIZE = 715,                 /* MAXSIZE  */
    MAXTRANS = 716,                /* MAXTRANS  */
    MAXVALUE = 717,                /* MAXVALUE  */
    MEDIUMINT = 718,               /* MEDIUMINT  */
    MEMORY = 719,                  /* MEMORY  */
    MERGE = 720,                   /* MERGE  */
    MESSAGE_TEXT = 721,            /* MESSAGE_TEXT  */
    METHOD = 722,                  /* METHOD  */
    MICROSECOND_P = 723,           /* MICROSECOND_P  */
    MID = 724,                     /* MID  */
    MIN_ROWS = 725,                /* MIN_ROWS  */
    MINUTE_P = 726,                /* MINUTE_P  */
    MINUTE_MICROSECOND_P = 727,    /* MINUTE_MICROSECOND_P  */
    MINUTE_SECOND_P = 728,         /* MINUTE_SECOND_P  */
    MINVALUE = 729,                /* MINVALUE  */
    MINEXTENTS = 730,              /* MINEXTENTS  */
    MODE = 731,                    /* MODE  */
    MODEL = 732,                   /* MODEL  */
    MODIFY_P = 733,                /* MODIFY_P  */
    MONTH_P = 734,                 /* MONTH_P  */
    MOVE = 735,                    /* MOVE  */
    MOVEMENT = 736,                /* MOVEMENT  */
    MYSQL_ERRNO = 737,             /* MYSQL_ERRNO  */
    MOD = 738,                     /* MOD  */
    MODIFIES = 739,                /* MODIFIES  */
    MAX_ROWS = 740,                /* MAX_ROWS  */
    NAME_P = 741,                  /* NAME_P  */
    NAMES = 742,                   /* NAMES  */
    NAN_P = 743,                   /* NAN_P  */
    NATIONAL = 744,                /* NATIONAL  */
    NATURAL = 745,                 /* NATURAL  */
    NCHAR = 746,                   /* NCHAR  */
    NEXT = 747,                    /* NEXT  */
    NGRAM = 748,                   /* NGRAM  */
    NO = 749,                      /* NO  */
    NOCOMPRESS = 750,              /* NOCOMPRESS  */
    NOCYCLE = 751,                 /* NOCYCLE  */
    NODE = 752,                    /* NODE  */
    NOLOGGING = 753,               /* NOLOGGING  */
    NOMAXVALUE = 754,              /* NOMAXVALUE  */
    NOMINVALUE = 755,              /* NOMINVALUE  */
    NONE = 756,                    /* NONE  */
    NOT = 757,                     /* NOT  */
    NOTHING = 758,                 /* NOTHING  */
    NOTIFY = 759,                  /* NOTIFY  */
    NOTNULL = 760,                 /* NOTNULL  */
    NOVALIDATE = 761,              /* NOVALIDATE  */
    NOWAIT = 762,                  /* NOWAIT  */
    NTH_VALUE_P = 763,             /* NTH_VALUE_P  */
    NULL_P = 764,                  /* NULL_P  */
    NULLCOLS = 765,                /* NULLCOLS  */
    NULLIF = 766,                  /* NULLIF  */
    NULLS_P = 767,                 /* NULLS_P  */
    NUMBER_P = 768,                /* NUMBER_P  */
    NUMERIC = 769,                 /* NUMERIC  */
    NUMSTR = 770,                  /* NUMSTR  */
    NVARCHAR = 771,                /* NVARCHAR  */
    NVARCHAR2 = 772,               /* NVARCHAR2  */
    NVL = 773,                     /* NVL  */
    NO_WRITE_TO_BINLOG = 774,      /* NO_WRITE_TO_BINLOG  */
    OBJECT_P = 775,                /* OBJECT_P  */
    OF = 776,                      /* OF  */
    OFF = 777,                     /* OFF  */
    OFFSET = 778,                  /* OFFSET  */
    OIDS = 779,                    /* OIDS  */
    ON = 780,                      /* ON  */
    ONLY = 781,                    /* ONLY  */
    OPEN = 782,                    /* OPEN  */
    OPERATOR = 783,                /* OPERATOR  */
    OPTIMIZATION = 784,            /* OPTIMIZATION  */
    OPTIMIZE = 785,                /* OPTIMIZE  */
    OPTION = 786,                  /* OPTION  */
    OPTIONALLY = 787,              /* OPTIONALLY  */
    OPTIONS = 788,                 /* OPTIONS  */
    OR = 789,                      /* OR  */
    ORDER = 790,                   /* ORDER  */
    OUT_P = 791,                   /* OUT_P  */
    OUTER_P = 792,                 /* OUTER_P  */
    OVER = 793,                    /* OVER  */
    OVERLAPS = 794,                /* OVERLAPS  */
    OVERLAY = 795,                 /* OVERLAY  */
    OWNED = 796,                   /* OWNED  */
    OWNER = 797,                   /* OWNER  */
    OUTFILE = 798,                 /* OUTFILE  */
    PACKAGE = 799,                 /* PACKAGE  */
    PACKAGES = 800,                /* PACKAGES  */
    PARALLEL_ENABLE = 801,         /* PARALLEL_ENABLE  */
    PACK_KEYS = 802,               /* PACK_KEYS  */
    PARSER = 803,                  /* PARSER  */
    PARTIAL = 804,                 /* PARTIAL  */
    PARTITION = 805,               /* PARTITION  */
    PARTITIONING = 806,            /* PARTITIONING  */
    PARTITIONS = 807,              /* PARTITIONS  */
    PASSING = 808,                 /* PASSING  */
    PASSWORD = 809,                /* PASSWORD  */
    PCTFREE = 810,                 /* PCTFREE  */
    PER_P = 811,                   /* PER_P  */
    PERCENT = 812,                 /* PERCENT  */
    PERFORMANCE = 813,             /* PERFORMANCE  */
    PERM = 814,                    /* PERM  */
    PLACING = 815,                 /* PLACING  */
    PLAN = 816,                    /* PLAN  */
    PLANS = 817,                   /* PLANS  */
    POLICY = 818,                  /* POLICY  */
    POSITION = 819,                /* POSITION  */
    PIPELINED = 820,               /* PIPELINED  */
    POOL = 821,                    /* POOL  */
    PRECEDING = 822,               /* PRECEDING  */
    PRECISION = 823,               /* PRECISION  */
    PREDICT = 824,                 /* PREDICT  */
    PREFERRED = 825,               /* PREFERRED  */
    PREFIX = 826,                  /* PREFIX  */
    PRESERVE = 827,                /* PRESERVE  */
    PREPARE = 828,                 /* PREPARE  */
    PREPARED = 829,                /* PREPARED  */
    PRIMARY = 830,                 /* PRIMARY  */
    PRECEDES_P = 831,              /* PRECEDES_P  */
    PRIVATE = 832,                 /* PRIVATE  */
    PRIOR = 833,                   /* PRIOR  */
    PRIORER = 834,                 /* PRIORER  */
    PRIVILEGES = 835,              /* PRIVILEGES  */
    PRIVILEGE = 836,               /* PRIVILEGE  */
    PROCEDURAL = 837,              /* PROCEDURAL  */
    PROCEDURE = 838,               /* PROCEDURE  */
    PROCESSLIST = 839,             /* PROCESSLIST  */
    PROFILE = 840,                 /* PROFILE  */
    PROXY = 841,                   /* PROXY  */
    PUBLICATION = 842,             /* PUBLICATION  */
    PUBLISH = 843,                 /* PUBLISH  */
    PURGE = 844,                   /* PURGE  */
    QUARTER_P = 845,               /* QUARTER_P  */
    QUERY = 846,                   /* QUERY  */
    QUICK = 847,                   /* QUICK  */
    QUOTE = 848,                   /* QUOTE  */
    RANDOMIZED = 849,              /* RANDOMIZED  */
    RANGE = 850,                   /* RANGE  */
    RATIO = 851,                   /* RATIO  */
    RAW = 852,                     /* RAW  */
    READ = 853,                    /* READ  */
    READS = 854,                   /* READS  */
    REAL = 855,                    /* REAL  */
    REASSIGN = 856,                /* REASSIGN  */
    REBUILD = 857,                 /* REBUILD  */
    RECHECK = 858,                 /* RECHECK  */
    RECURSIVE = 859,               /* RECURSIVE  */
    RECYCLEBIN = 860,              /* RECYCLEBIN  */
    REDISANYVALUE = 861,           /* REDISANYVALUE  */
    REF = 862,                     /* REF  */
    REFERENCES = 863,              /* REFERENCES  */
    REFRESH = 864,                 /* REFRESH  */
    REINDEX = 865,                 /* REINDEX  */
    REJECT_P = 866,                /* REJECT_P  */
    RELATIVE_P = 867,              /* RELATIVE_P  */
    RELEASE = 868,                 /* RELEASE  */
    RELOPTIONS = 869,              /* RELOPTIONS  */
    REMOTE_P = 870,                /* REMOTE_P  */
    REMOVE = 871,                  /* REMOVE  */
    RENAME = 872,                  /* RENAME  */
    REPEAT = 873,                  /* REPEAT  */
    REPEATABLE = 874,              /* REPEATABLE  */
    REPLACE = 875,                 /* REPLACE  */
    REPLICA = 876,                 /* REPLICA  */
    REPLICAS = 877,                /* REPLICAS  */
    REGEXP = 878,                  /* REGEXP  */
    REORGANIZE = 879,              /* REORGANIZE  */
    REPAIR = 880,                  /* REPAIR  */
    RESET = 881,                   /* RESET  */
    RESIZE = 882,                  /* RESIZE  */
    RESOURCE = 883,                /* RESOURCE  */
    RESPECT_P = 884,               /* RESPECT_P  */
    RESTART = 885,                 /* RESTART  */
    RESTRICT = 886,                /* RESTRICT  */
    RETURN = 887,                  /* RETURN  */
    RETURNED_SQLSTATE = 888,       /* RETURNED_SQLSTATE  */
    RETURNING = 889,               /* RETURNING  */
    RETURNS = 890,                 /* RETURNS  */
    REUSE = 891,                   /* REUSE  */
    REVOKE = 892,                  /* REVOKE  */
    RIGHT = 893,                   /* RIGHT  */
    ROLE = 894,                    /* ROLE  */
    ROLES = 895,                   /* ROLES  */
    ROLLBACK = 896,                /* ROLLBACK  */
    ROLLUP = 897,                  /* ROLLUP  */
    ROTATE = 898,                  /* ROTATE  */
    ROTATION = 899,                /* ROTATION  */
    ROW = 900,                     /* ROW  */
    ROW_COUNT = 901,               /* ROW_COUNT  */
    ROWS = 902,                    /* ROWS  */
    ROWTYPE_P = 903,               /* ROWTYPE_P  */
    RULE = 904,                    /* RULE  */
    RESIGNAL = 905,                /* RESIGNAL  */
    RLIKE = 906,                   /* RLIKE  */
    ROUTINE = 907,                 /* ROUTINE  */
    ROW_FORMAT = 908,              /* ROW_FORMAT  */
    SCHEMAS = 909,                 /* SCHEMAS  */
    SAMPLE = 910,                  /* SAMPLE  */
    SAVEPOINT = 911,               /* SAVEPOINT  */
    SCHEDULE = 912,                /* SCHEDULE  */
    SCHEMA = 913,                  /* SCHEMA  */
    SCHEMA_NAME = 914,             /* SCHEMA_NAME  */
    SCROLL = 915,                  /* SCROLL  */
    SEARCH = 916,                  /* SEARCH  */
    SECONDARY_ENGINE_ATTRIBUTE = 917, /* SECONDARY_ENGINE_ATTRIBUTE  */
    SECOND_P = 918,                /* SECOND_P  */
    SECOND_MICROSECOND_P = 919,    /* SECOND_MICROSECOND_P  */
    SECURITY = 920,                /* SECURITY  */
    SELECT = 921,                  /* SELECT  */
    SEPARATOR_P = 922,             /* SEPARATOR_P  */
    SEQUENCE = 923,                /* SEQUENCE  */
    SEQUENCES = 924,               /* SEQUENCES  */
    SERIALIZABLE = 925,            /* SERIALIZABLE  */
    SERVER = 926,                  /* SERVER  */
    SESSION = 927,                 /* SESSION  */
    SESSION_USER = 928,            /* SESSION_USER  */
    SET = 929,                     /* SET  */
    SETS = 930,                    /* SETS  */
    SETOF = 931,                   /* SETOF  */
    SHARE = 932,                   /* SHARE  */
    SHIPPABLE = 933,               /* SHIPPABLE  */
    SHOW = 934,                    /* SHOW  */
    SHUTDOWN = 935,                /* SHUTDOWN  */
    SIBLINGS = 936,                /* SIBLINGS  */
    SIGNAL = 937,                  /* SIGNAL  */
    SIGNED = 938,                  /* SIGNED  */
    SIMILAR = 939,                 /* SIMILAR  */
    SIMPLE = 940,                  /* SIMPLE  */
    SIZE = 941,                    /* SIZE  */
    SKIP = 942,                    /* SKIP  */
    SLAVE = 943,                   /* SLAVE  */
    SLICE = 944,                   /* SLICE  */
    SMALLDATETIME = 945,           /* SMALLDATETIME  */
    SMALLDATETIME_FORMAT_P = 946,  /* SMALLDATETIME_FORMAT_P  */
    SMALLINT = 947,                /* SMALLINT  */
    SNAPSHOT = 948,                /* SNAPSHOT  */
    SOME = 949,                    /* SOME  */
    SOUNDS = 950,                  /* SOUNDS  */
    SOURCE_P = 951,                /* SOURCE_P  */
    SPACE = 952,                   /* SPACE  */
    SPECIFICATION = 953,           /* SPECIFICATION  */
    SPILL = 954,                   /* SPILL  */
    SPLIT = 955,                   /* SPLIT  */
    SQLSTATE = 956,                /* SQLSTATE  */
    STABLE = 957,                  /* STABLE  */
    STACKED_P = 958,               /* STACKED_P  */
    STANDALONE_P = 959,            /* STANDALONE_P  */
    START = 960,                   /* START  */
    STARTS = 961,                  /* STARTS  */
    STARTWITH = 962,               /* STARTWITH  */
    STATEMENT = 963,               /* STATEMENT  */
    STATEMENT_ID = 964,            /* STATEMENT_ID  */
    STATISTICS = 965,              /* STATISTICS  */
    STATS_AUTO_RECALC = 966,       /* STATS_AUTO_RECALC  */
    STATS_PERSISTENT = 967,        /* STATS_PERSISTENT  */
    STATS_SAMPLE_PAGES = 968,      /* STATS_SAMPLE_PAGES  */
    STATUS = 969,                  /* STATUS  */
    STDIN = 970,                   /* STDIN  */
    STDOUT = 971,                  /* STDOUT  */
    STORAGE = 972,                 /* STORAGE  */
    STORE_P = 973,                 /* STORE_P  */
    STORED = 974,                  /* STORED  */
    STRAIGHT_JOIN = 975,           /* STRAIGHT_JOIN  */
    STRATIFY = 976,                /* STRATIFY  */
    STREAM = 977,                  /* STREAM  */
    STRICT_P = 978,                /* STRICT_P  */
    STRIP_P = 979,                 /* STRIP_P  */
    SUBCLASS_ORIGIN = 980,         /* SUBCLASS_ORIGIN  */
    SUBPARTITION = 981,            /* SUBPARTITION  */
    SUBPARTITIONS = 982,           /* SUBPARTITIONS  */
    SUBSCRIPTION = 983,            /* SUBSCRIPTION  */
    SUBSTR = 984,                  /* SUBSTR  */
    SUBSTRING = 985,               /* SUBSTRING  */
    SYMMETRIC = 986,               /* SYMMETRIC  */
    SYNONYM = 987,                 /* SYNONYM  */
    SYSDATE = 988,                 /* SYSDATE  */
    SYSID = 989,                   /* SYSID  */
    SYSTEM_P = 990,                /* SYSTEM_P  */
    SYS_REFCURSOR = 991,           /* SYS_REFCURSOR  */
    STARTING = 992,                /* STARTING  */
    SQL_P = 993,                   /* SQL_P  */
    TABLE = 994,                   /* TABLE  */
    TABLE_NAME = 995,              /* TABLE_NAME  */
    TABLES = 996,                  /* TABLES  */
    TABLESAMPLE = 997,             /* TABLESAMPLE  */
    TABLESPACE = 998,              /* TABLESPACE  */
    TARGET = 999,                  /* TARGET  */
    TEMP = 1000,                   /* TEMP  */
    TEMPLATE = 1001,               /* TEMPLATE  */
    TEMPORARY = 1002,              /* TEMPORARY  */
    TEMPTABLE = 1003,              /* TEMPTABLE  */
    TERMINATED = 1004,             /* TERMINATED  */
    TEXT_P = 1005,                 /* TEXT_P  */
    THAN = 1006,                   /* THAN  */
    THEN = 1007,                   /* THEN  */
    TIME = 1008,                   /* TIME  */
    TIME_FORMAT_P = 1009,          /* TIME_FORMAT_P  */
    TIES = 1010,                   /* TIES  */
    TIMECAPSULE = 1011,            /* TIMECAPSULE  */
    TIMESTAMP = 1012,              /* TIMESTAMP  */
    TIMESTAMP_FORMAT_P = 1013,     /* TIMESTAMP_FORMAT_P  */
    TIMESTAMPADD = 1014,           /* TIMESTAMPADD  */
    TIMESTAMPDIFF = 1015,          /* TIMESTAMPDIFF  */
    TIMEZONE_HOUR_P = 1016,        /* TIMEZONE_HOUR_P  */
    TIMEZONE_MINUTE_P = 1017,      /* TIMEZONE_MINUTE_P  */
    TINYINT = 1018,                /* TINYINT  */
    TO = 1019,                     /* TO  */
    TRAILING = 1020,               /* TRAILING  */
    TRANSACTION = 1021,            /* TRANSACTION  */
    TRANSFORM = 1022,              /* TRANSFORM  */
    TREAT = 1023,                  /* TREAT  */
    TRIGGER = 1024,                /* TRIGGER  */
    TRIM = 1025,                   /* TRIM  */
    TRUE_P = 1026,                 /* TRUE_P  */
    TRUNCATE = 1027,               /* TRUNCATE  */
    TRUSTED = 1028,                /* TRUSTED  */
    TSFIELD = 1029,                /* TSFIELD  */
    TSTAG = 1030,                  /* TSTAG  */
    TSTIME = 1031,                 /* TSTIME  */
    TYPE_P = 1032,                 /* TYPE_P  */
    TYPES_P = 1033,                /* TYPES_P  */
    UNBOUNDED = 1034,              /* UNBOUNDED  */
    UNCOMMITTED = 1035,            /* UNCOMMITTED  */
    UNDEFINED = 1036,              /* UNDEFINED  */
    UNENCRYPTED = 1037,            /* UNENCRYPTED  */
    UNION = 1038,                  /* UNION  */
    UNIQUE = 1039,                 /* UNIQUE  */
    UNKNOWN = 1040,                /* UNKNOWN  */
    UNLIMITED = 1041,              /* UNLIMITED  */
    UNLISTEN = 1042,               /* UNLISTEN  */
    UNLOCK = 1043,                 /* UNLOCK  */
    UNLOGGED = 1044,               /* UNLOGGED  */
    UNIMCSTORED = 1045,            /* UNIMCSTORED  */
    UNSIGNED = 1046,               /* UNSIGNED  */
    UNTIL = 1047,                  /* UNTIL  */
    UNUSABLE = 1048,               /* UNUSABLE  */
    UPDATE = 1049,                 /* UPDATE  */
    USE = 1050,                    /* USE  */
    USEEOF = 1051,                 /* USEEOF  */
    USER = 1052,                   /* USER  */
    USING = 1053,                  /* USING  */
    UTC_DATE = 1054,               /* UTC_DATE  */
    UTC_TIME = 1055,               /* UTC_TIME  */
    UTC_TIMESTAMP = 1056,          /* UTC_TIMESTAMP  */
    VACUUM = 1057,                 /* VACUUM  */
    VALID = 1058,                  /* VALID  */
    VALIDATE = 1059,               /* VALIDATE  */
    VALIDATION = 1060,             /* VALIDATION  */
    VALIDATOR = 1061,              /* VALIDATOR  */
    VALUE_P = 1062,                /* VALUE_P  */
    VALUES = 1063,                 /* VALUES  */
    VARBINARY = 1064,              /* VARBINARY  */
    VARCHAR = 1065,                /* VARCHAR  */
    VARCHAR2 = 1066,               /* VARCHAR2  */
    VARIABLES = 1067,              /* VARIABLES  */
    VARIADIC = 1068,               /* VARIADIC  */
    VARRAY = 1069,                 /* VARRAY  */
    VARYING = 1070,                /* VARYING  */
    VCGROUP = 1071,                /* VCGROUP  */
    VERBOSE = 1072,                /* VERBOSE  */
    VERIFY = 1073,                 /* VERIFY  */
    VERSION_P = 1074,              /* VERSION_P  */
    VIEW = 1075,                   /* VIEW  */
    VISIBLE = 1076,                /* VISIBLE  */
    VOLATILE = 1077,               /* VOLATILE  */
    WAIT = 1078,                   /* WAIT  */
    WARNINGS = 1079,               /* WARNINGS  */
    WEAK = 1080,                   /* WEAK  */
    WEEK_P = 1081,                 /* WEEK_P  */
    WHEN = 1082,                   /* WHEN  */
    WHERE = 1083,                  /* WHERE  */
    WHILE_P = 1084,                /* WHILE_P  */
    WHITESPACE_P = 1085,           /* WHITESPACE_P  */
    WINDOW = 1086,                 /* WINDOW  */
    WITH = 1087,                   /* WITH  */
    WITHIN = 1088,                 /* WITHIN  */
    WITHOUT = 1089,                /* WITHOUT  */
    WORK = 1090,                   /* WORK  */
    WORKLOAD = 1091,               /* WORKLOAD  */
    WRAPPER = 1092,                /* WRAPPER  */
    WRITE = 1093,                  /* WRITE  */
    XML_P = 1094,                  /* XML_P  */
    XMLATTRIBUTES = 1095,          /* XMLATTRIBUTES  */
    XMLCONCAT = 1096,              /* XMLCONCAT  */
    XMLELEMENT = 1097,             /* XMLELEMENT  */
    XMLEXISTS = 1098,              /* XMLEXISTS  */
    XMLFOREST = 1099,              /* XMLFOREST  */
    XMLPARSE = 1100,               /* XMLPARSE  */
    XOR = 1101,                    /* XOR  */
    XMLPI = 1102,                  /* XMLPI  */
    XMLROOT = 1103,                /* XMLROOT  */
    XMLSERIALIZE = 1104,           /* XMLSERIALIZE  */
    YEAR_P = 1105,                 /* YEAR_P  */
    YEAR_MONTH_P = 1106,           /* YEAR_MONTH_P  */
    YES_P = 1107,                  /* YES_P  */
    ZEROFILL = 1108,               /* ZEROFILL  */
    ZONE = 1109,                   /* ZONE  */
    AST = 1110,                    /* AST  */
    DB_B_JSON = 1111,              /* DB_B_JSON  */
    DB_B_JSONB = 1112,             /* DB_B_JSONB  */
    DB_B_BOX = 1113,               /* DB_B_BOX  */
    DB_B_CIRCLE = 1114,            /* DB_B_CIRCLE  */
    DB_B_LSEG = 1115,              /* DB_B_LSEG  */
    DB_B_PATH = 1116,              /* DB_B_PATH  */
    DB_B_POLYGON = 1117,           /* DB_B_POLYGON  */
    DB_B_BYTEA = 1118,             /* DB_B_BYTEA  */
    DB_B_TIMETZ = 1119,            /* DB_B_TIMETZ  */
    DB_B_TIMESTAMPTZ = 1120,       /* DB_B_TIMESTAMPTZ  */
    DB_B_POINT = 1121,             /* DB_B_POINT  */
    DB_B_CIDR = 1122,              /* DB_B_CIDR  */
    WEIGHT_STRING = 1123,          /* WEIGHT_STRING  */
    REVERSE = 1124,                /* REVERSE  */
    ALGORITHM_UNDEFINED = 1125,    /* ALGORITHM_UNDEFINED  */
    ALGORITHM_MERGE = 1126,        /* ALGORITHM_MERGE  */
    ALGORITHM_TEMPTABLE = 1127,    /* ALGORITHM_TEMPTABLE  */
    NULLS_FIRST = 1128,            /* NULLS_FIRST  */
    NULLS_LAST = 1129,             /* NULLS_LAST  */
    WITH_TIME = 1130,              /* WITH_TIME  */
    INCLUDING_ALL = 1131,          /* INCLUDING_ALL  */
    RENAME_PARTITION = 1132,       /* RENAME_PARTITION  */
    PARTITION_FOR = 1133,          /* PARTITION_FOR  */
    SUBPARTITION_FOR = 1134,       /* SUBPARTITION_FOR  */
    ADD_PARTITION = 1135,          /* ADD_PARTITION  */
    DROP_PARTITION = 1136,         /* DROP_PARTITION  */
    REBUILD_PARTITION = 1137,      /* REBUILD_PARTITION  */
    MODIFY_PARTITION = 1138,       /* MODIFY_PARTITION  */
    ADD_SUBPARTITION = 1139,       /* ADD_SUBPARTITION  */
    DROP_SUBPARTITION = 1140,      /* DROP_SUBPARTITION  */
    NOT_ENFORCED = 1141,           /* NOT_ENFORCED  */
    VALID_BEGIN = 1142,            /* VALID_BEGIN  */
    DECLARE_CURSOR = 1143,         /* DECLARE_CURSOR  */
    ON_UPDATE_TIME = 1144,         /* ON_UPDATE_TIME  */
    START_WITH = 1145,             /* START_WITH  */
    CONNECT_BY = 1146,             /* CONNECT_BY  */
    SHOW_ERRORS = 1147,            /* SHOW_ERRORS  */
    WITH_ROLLUP = 1148,            /* WITH_ROLLUP  */
    END_OF_INPUT = 1149,           /* END_OF_INPUT  */
    END_OF_INPUT_COLON = 1150,     /* END_OF_INPUT_COLON  */
    END_OF_PROC = 1151,            /* END_OF_PROC  */
    EVENT_TRIGGER = 1152,          /* EVENT_TRIGGER  */
    NOT_IN = 1153,                 /* NOT_IN  */
    NOT_BETWEEN = 1154,            /* NOT_BETWEEN  */
    NOT_LIKE = 1155,               /* NOT_LIKE  */
    NOT_ILIKE = 1156,              /* NOT_ILIKE  */
    NOT_SIMILAR = 1157,            /* NOT_SIMILAR  */
    DEFAULT_FUNC = 1158,           /* DEFAULT_FUNC  */
    MATCH_FUNC = 1159,             /* MATCH_FUNC  */
    DO_SCONST = 1160,              /* DO_SCONST  */
    DO_LANGUAGE = 1161,            /* DO_LANGUAGE  */
    SHOW_STATUS = 1162,            /* SHOW_STATUS  */
    BEGIN_B_BLOCK = 1163,          /* BEGIN_B_BLOCK  */
    FORCE_INDEX = 1164,            /* FORCE_INDEX  */
    USE_INDEX = 1165,              /* USE_INDEX  */
    IGNORE_INDEX = 1166,           /* IGNORE_INDEX  */
    CURSOR_EXPR = 1167,            /* CURSOR_EXPR  */
    LATERAL_EXPR = 1168,           /* LATERAL_EXPR  */
    FALSE_ON_ERROR = 1169,         /* FALSE_ON_ERROR  */
    TRUE_ON_ERROR = 1170,          /* TRUE_ON_ERROR  */
    ERROR_ON_ERROR = 1171,         /* ERROR_ON_ERROR  */
    LOCK_TABLES = 1172,            /* LOCK_TABLES  */
    LABEL_LOOP = 1173,             /* LABEL_LOOP  */
    LABEL_REPEAT = 1174,           /* LABEL_REPEAT  */
    LABEL_WHILE = 1175,            /* LABEL_WHILE  */
    WITH_PARSER = 1176,            /* WITH_PARSER  */
    STORAGE_DISK = 1177,           /* STORAGE_DISK  */
    STORAGE_MEMORY = 1178,         /* STORAGE_MEMORY  */
    FULL_OUTER = 1179,             /* FULL_OUTER  */
    lower_than_key = 1180,         /* lower_than_key  */
    lower_than_row = 1181,         /* lower_than_row  */
    lower_than_on = 1182,          /* lower_than_on  */
    KILL_OPT_EMPTY = 1183,         /* KILL_OPT_EMPTY  */
    PARTIAL_EMPTY_PREC = 1184,     /* PARTIAL_EMPTY_PREC  */
    POSTFIXOP = 1185,              /* POSTFIXOP  */
    lower_than_zerofill = 1186,    /* lower_than_zerofill  */
    lower_than_under = 1187,       /* lower_than_under  */
    higher_than_rotate = 1188,     /* higher_than_rotate  */
    UMINUS = 1189,                 /* UMINUS  */
    EMPTY_FROM_CLAUSE = 1190,      /* EMPTY_FROM_CLAUSE  */
    CONSTRUCTOR = 1191,            /* CONSTRUCTOR  */
    FINAL = 1192,                  /* FINAL  */
    MAP = 1193,                    /* MAP  */
    MEMBER = 1194,                 /* MEMBER  */
    RESULT = 1195,                 /* RESULT  */
    SELF = 1196,                   /* SELF  */
    STATIC_P = 1197,               /* STATIC_P  */
    UNDER = 1198,                  /* UNDER  */
    SELF_INOUT = 1199,             /* SELF_INOUT  */
    STATIC_FUNCTION = 1200,        /* STATIC_FUNCTION  */
    MEMBER_FUNCTION = 1201,        /* MEMBER_FUNCTION  */
    STATIC_PROCEDURE = 1202,       /* STATIC_PROCEDURE  */
    MEMBER_PROCEDURE = 1203,       /* MEMBER_PROCEDURE  */
    CONSTRUCTOR_FUNCTION = 1204,   /* CONSTRUCTOR_FUNCTION  */
    MAP_MEMBER = 1205              /* MAP_MEMBER  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 588 "gram.y"

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
	struct OptLikeWhere	*optlikewhere;
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
	struct CreateTableOptions	*createtableoptions;
	struct SingleTableOption	*singletableoption;
	struct CreateIndexOptions	*createindexoptions;
	struct SingleIndexOption	*singleindexoption;
	struct IndexMethodRelationClause *indexmethodrelationclause;
	struct DolphinString		*dolphinString;
	struct DolphinIdent			*dolphinIdent;
	struct CondInfo*	condinfo;
	struct TypeAttr* typeattr;
	RotateClause         *rotateinfo;
	UnrotateClause       *unrotateinfo;
	FunctionPartitionInfo *funcPartInfo;

#line 1081 "gram.hpp"

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
