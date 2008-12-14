/* A Bison parser, made by GNU Bison 2.1.  */

/* Skeleton parser for Yacc-like parsing with Bison,
   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* Written by Richard Stallman by simplifying the original so called
   ``semantic'' parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     keyword_class = 258,
     keyword_module = 259,
     keyword_def = 260,
     keyword_undef = 261,
     keyword_begin = 262,
     keyword_rescue = 263,
     keyword_ensure = 264,
     keyword_end = 265,
     keyword_if = 266,
     keyword_unless = 267,
     keyword_then = 268,
     keyword_elsif = 269,
     keyword_else = 270,
     keyword_case = 271,
     keyword_when = 272,
     keyword_while = 273,
     keyword_until = 274,
     keyword_for = 275,
     keyword_break = 276,
     keyword_next = 277,
     keyword_redo = 278,
     keyword_retry = 279,
     keyword_in = 280,
     keyword_do = 281,
     keyword_do_cond = 282,
     keyword_do_block = 283,
     keyword_do_LAMBDA = 284,
     keyword_return = 285,
     keyword_yield = 286,
     keyword_super = 287,
     keyword_self = 288,
     keyword_nil = 289,
     keyword_true = 290,
     keyword_false = 291,
     keyword_and = 292,
     keyword_or = 293,
     keyword_not = 294,
     modifier_if = 295,
     modifier_unless = 296,
     modifier_while = 297,
     modifier_until = 298,
     modifier_rescue = 299,
     keyword_alias = 300,
     keyword_defined = 301,
     keyword_BEGIN = 302,
     keyword_END = 303,
     keyword__LINE__ = 304,
     keyword__FILE__ = 305,
     keyword__ENCODING__ = 306,
     tIDENTIFIER = 307,
     tFID = 308,
     tGVAR = 309,
     tIVAR = 310,
     tCONSTANT = 311,
     tCVAR = 312,
     tLABEL = 313,
     tINTEGER = 314,
     tFLOAT = 315,
     tSTRING_CONTENT = 316,
     tCHAR = 317,
     tNTH_REF = 318,
     tBACK_REF = 319,
     tREGEXP_END = 320,
     tUPLUS = 321,
     tUMINUS = 322,
     tPOW = 323,
     tCMP = 324,
     tEQ = 325,
     tEQQ = 326,
     tNEQ = 327,
     tGEQ = 328,
     tLEQ = 329,
     tANDOP = 330,
     tOROP = 331,
     tMATCH = 332,
     tNMATCH = 333,
     tDOT2 = 334,
     tDOT3 = 335,
     tAREF = 336,
     tASET = 337,
     tLSHFT = 338,
     tRSHFT = 339,
     tCOLON2 = 340,
     tCOLON3 = 341,
     tOP_ASGN = 342,
     tASSOC = 343,
     tLPAREN = 344,
     tLPAREN_ARG = 345,
     tRPAREN = 346,
     tLBRACK = 347,
     tLBRACE = 348,
     tLBRACE_ARG = 349,
     tSTAR = 350,
     tAMPER = 351,
     tLAMBDA = 352,
     tSYMBEG = 353,
     tSTRING_BEG = 354,
     tXSTRING_BEG = 355,
     tREGEXP_BEG = 356,
     tWORDS_BEG = 357,
     tQWORDS_BEG = 358,
     tSTRING_DBEG = 359,
     tSTRING_DVAR = 360,
     tSTRING_END = 361,
     tLAMBEG = 362,
     tLOWEST = 363,
     tUMINUS_NUM = 364,
     idNULL = 365,
     idRespond_to = 366,
     idIFUNC = 367,
     idCFUNC = 368,
     idThrowState = 369,
     id_core_set_method_alias = 370,
     id_core_set_variable_alias = 371,
     id_core_undef_method = 372,
     id_core_define_method = 373,
     id_core_define_singleton_method = 374,
     id_core_set_postexe = 375,
     tLAST_TOKEN = 376
   };
#endif
/* Tokens.  */
#define keyword_class 258
#define keyword_module 259
#define keyword_def 260
#define keyword_undef 261
#define keyword_begin 262
#define keyword_rescue 263
#define keyword_ensure 264
#define keyword_end 265
#define keyword_if 266
#define keyword_unless 267
#define keyword_then 268
#define keyword_elsif 269
#define keyword_else 270
#define keyword_case 271
#define keyword_when 272
#define keyword_while 273
#define keyword_until 274
#define keyword_for 275
#define keyword_break 276
#define keyword_next 277
#define keyword_redo 278
#define keyword_retry 279
#define keyword_in 280
#define keyword_do 281
#define keyword_do_cond 282
#define keyword_do_block 283
#define keyword_do_LAMBDA 284
#define keyword_return 285
#define keyword_yield 286
#define keyword_super 287
#define keyword_self 288
#define keyword_nil 289
#define keyword_true 290
#define keyword_false 291
#define keyword_and 292
#define keyword_or 293
#define keyword_not 294
#define modifier_if 295
#define modifier_unless 296
#define modifier_while 297
#define modifier_until 298
#define modifier_rescue 299
#define keyword_alias 300
#define keyword_defined 301
#define keyword_BEGIN 302
#define keyword_END 303
#define keyword__LINE__ 304
#define keyword__FILE__ 305
#define keyword__ENCODING__ 306
#define tIDENTIFIER 307
#define tFID 308
#define tGVAR 309
#define tIVAR 310
#define tCONSTANT 311
#define tCVAR 312
#define tLABEL 313
#define tINTEGER 314
#define tFLOAT 315
#define tSTRING_CONTENT 316
#define tCHAR 317
#define tNTH_REF 318
#define tBACK_REF 319
#define tREGEXP_END 320
#define tUPLUS 321
#define tUMINUS 322
#define tPOW 323
#define tCMP 324
#define tEQ 325
#define tEQQ 326
#define tNEQ 327
#define tGEQ 328
#define tLEQ 329
#define tANDOP 330
#define tOROP 331
#define tMATCH 332
#define tNMATCH 333
#define tDOT2 334
#define tDOT3 335
#define tAREF 336
#define tASET 337
#define tLSHFT 338
#define tRSHFT 339
#define tCOLON2 340
#define tCOLON3 341
#define tOP_ASGN 342
#define tASSOC 343
#define tLPAREN 344
#define tLPAREN_ARG 345
#define tRPAREN 346
#define tLBRACK 347
#define tLBRACE 348
#define tLBRACE_ARG 349
#define tSTAR 350
#define tAMPER 351
#define tLAMBDA 352
#define tSYMBEG 353
#define tSTRING_BEG 354
#define tXSTRING_BEG 355
#define tREGEXP_BEG 356
#define tWORDS_BEG 357
#define tQWORDS_BEG 358
#define tSTRING_DBEG 359
#define tSTRING_DVAR 360
#define tSTRING_END 361
#define tLAMBEG 362
#define tLOWEST 363
#define tUMINUS_NUM 364
#define idNULL 365
#define idRespond_to 366
#define idIFUNC 367
#define idCFUNC 368
#define idThrowState 369
#define id_core_set_method_alias 370
#define id_core_set_variable_alias 371
#define id_core_undef_method 372
#define id_core_define_method 373
#define id_core_define_singleton_method 374
#define id_core_set_postexe 375
#define tLAST_TOKEN 376




/* Copy the first part of user declarations.  */
#line 12 "parse.y"


#define YYDEBUG 1
#define YYERROR_VERBOSE 1
#define YYSTACK_USE_ALLOCA 0

#include "ruby/ruby.h"
#include "ruby/st.h"
#include "ruby/encoding.h"
#include "node.h"
#include "parse.h"
#include "id.h"
#include "regenc.h"
#include <stdio.h>
#include <errno.h>
#include <ctype.h>

#define YYMALLOC(size)		rb_parser_malloc(parser, size)
#define YYREALLOC(ptr, size)	rb_parser_realloc(parser, ptr, size)
#define YYCALLOC(nelem, size)	rb_parser_calloc(parser, nelem, size)
#define YYFREE(ptr)		rb_parser_free(parser, ptr)
#define malloc	YYMALLOC
#define realloc	YYREALLOC
#define calloc	YYCALLOC
#define free	YYFREE

#ifndef RIPPER
static ID register_symid(ID, const char *, long, rb_encoding *);
#define REGISTER_SYMID(id, name) register_symid(id, name, strlen(name), enc)
#include "id.c"
#endif

#define is_notop_id(id) ((id)>tLAST_TOKEN)
#define is_local_id(id) (is_notop_id(id)&&((id)&ID_SCOPE_MASK)==ID_LOCAL)
#define is_global_id(id) (is_notop_id(id)&&((id)&ID_SCOPE_MASK)==ID_GLOBAL)
#define is_instance_id(id) (is_notop_id(id)&&((id)&ID_SCOPE_MASK)==ID_INSTANCE)
#define is_attrset_id(id) (is_notop_id(id)&&((id)&ID_SCOPE_MASK)==ID_ATTRSET)
#define is_const_id(id) (is_notop_id(id)&&((id)&ID_SCOPE_MASK)==ID_CONST)
#define is_class_id(id) (is_notop_id(id)&&((id)&ID_SCOPE_MASK)==ID_CLASS)
#define is_junk_id(id) (is_notop_id(id)&&((id)&ID_SCOPE_MASK)==ID_JUNK)

#define is_asgn_or_id(id) ((is_notop_id(id)) && \
	(((id)&ID_SCOPE_MASK) == ID_GLOBAL || \
	 ((id)&ID_SCOPE_MASK) == ID_INSTANCE || \
	 ((id)&ID_SCOPE_MASK) == ID_CLASS))

enum lex_state_e {
    EXPR_BEG,			/* ignore newline, +/- is a sign. */
    EXPR_END,			/* newline significant, +/- is a operator. */
    EXPR_ENDARG,		/* ditto, and unbound braces. */
    EXPR_ARG,			/* newline significant, +/- is a operator. */
    EXPR_CMDARG,		/* newline significant, +/- is a operator. */
    EXPR_MID,			/* newline significant, +/- is a operator. */
    EXPR_FNAME,			/* ignore newline, no reserved words. */
    EXPR_DOT,			/* right after `.' or `::', no reserved words. */
    EXPR_CLASS,			/* immediate after `class', no here document. */
    EXPR_VALUE			/* alike EXPR_BEG but label is disallowed. */
};

# ifdef HAVE_LONG_LONG
typedef unsigned LONG_LONG stack_type;
# else
typedef unsigned long stack_type;
# endif

# define BITSTACK_PUSH(stack, n)	(stack = (stack<<1)|((n)&1))
# define BITSTACK_POP(stack)	(stack = stack >> 1)
# define BITSTACK_LEXPOP(stack)	(stack = (stack >> 1) | (stack & 1))
# define BITSTACK_SET_P(stack)	(stack&1)

#define COND_PUSH(n)	BITSTACK_PUSH(cond_stack, n)
#define COND_POP()	BITSTACK_POP(cond_stack)
#define COND_LEXPOP()	BITSTACK_LEXPOP(cond_stack)
#define COND_P()	BITSTACK_SET_P(cond_stack)

#define CMDARG_PUSH(n)	BITSTACK_PUSH(cmdarg_stack, n)
#define CMDARG_POP()	BITSTACK_POP(cmdarg_stack)
#define CMDARG_LEXPOP()	BITSTACK_LEXPOP(cmdarg_stack)
#define CMDARG_P()	BITSTACK_SET_P(cmdarg_stack)

struct vtable {
    ID *tbl;
    int pos;
    int capa;
    struct vtable *prev;
};

struct local_vars {
    struct vtable *args;
    struct vtable *vars;
    struct local_vars *prev;
};

#define DVARS_INHERIT ((void*)1)
#define DVARS_TOPSCOPE NULL
#define DVARS_SPECIAL_P(tbl) (!POINTER_P(tbl))
#define POINTER_P(val) ((VALUE)(val) & ~(VALUE)3)

#ifndef RIPPER
static int
vtable_size(const struct vtable *tbl)
{
    if (POINTER_P(tbl)) {
        return tbl->pos;
    }
    else {
        return 0;
    }
}

#define VTBL_DEBUG 0

static struct vtable *
vtable_alloc(struct vtable *prev)
{
    struct vtable *tbl = ALLOC(struct vtable);
    tbl->pos = 0;
    tbl->capa = 8;
    tbl->tbl = ALLOC_N(ID, tbl->capa);
    tbl->prev = prev;
    if (VTBL_DEBUG) printf("vtable_alloc: %p\n", tbl);
    return tbl;
}

static void
vtable_free(struct vtable *tbl)
{
    if (VTBL_DEBUG)printf("vtable_free: %p\n", tbl);
    if (POINTER_P(tbl)) {
        if (tbl->tbl) {
            xfree(tbl->tbl);
        }
        xfree(tbl);
    }
}

static void
vtable_add(struct vtable *tbl, ID id)
{
    if (!POINTER_P(tbl)) {
        rb_bug("vtable_add: vtable is not allocated (%p)", tbl);
    }
    if (VTBL_DEBUG) printf("vtable_add: %p, %s\n", tbl, rb_id2name(id));

    if (tbl->pos == tbl->capa) {
        tbl->capa = tbl->capa * 2;
        REALLOC_N(tbl->tbl, ID, tbl->capa);
    }
    tbl->tbl[tbl->pos++] = id;
}

static int
vtable_included(const struct vtable * tbl, ID id)
{
    int i;

    if (POINTER_P(tbl)) {
        for (i = 0; i < tbl->pos; i++) {
            if (tbl->tbl[i] == id) {
                return 1;
            }
        }
    }
    return 0;
}


typedef struct token_info {
    const char *token;
    int linenum;
    int column;
    int nonspc;
    struct token_info *next;
} token_info;
#endif

/*
    Structure of Lexer Buffer:

 lex_pbeg      tokp         lex_p        lex_pend
    |           |              |            |
    |-----------+--------------+------------|
                |<------------>|
                     token
*/
struct parser_params {
    int is_ripper;
    NODE *heap;

    YYSTYPE *parser_yylval;
    VALUE eofp;

    NODE *parser_lex_strterm;
    enum lex_state_e parser_lex_state;
    stack_type parser_cond_stack;
    stack_type parser_cmdarg_stack;
    int parser_class_nest;
    int parser_paren_nest;
    int parser_lpar_beg;
    int parser_in_single;
    int parser_in_def;
    int parser_compile_for_eval;
    VALUE parser_cur_mid;
    int parser_in_defined;
    char *parser_tokenbuf;
    int parser_tokidx;
    int parser_toksiz;
    VALUE parser_lex_input;
    VALUE parser_lex_lastline;
    VALUE parser_lex_nextline;
    const char *parser_lex_pbeg;
    const char *parser_lex_p;
    const char *parser_lex_pend;
    int parser_heredoc_end;
    int parser_command_start;
    NODE *parser_deferred_nodes;
    int parser_lex_gets_ptr;
    VALUE (*parser_lex_gets)(struct parser_params*,VALUE);
    struct local_vars *parser_lvtbl;
    int parser_ruby__end__seen;
    int line_count;
    int has_shebang;
    char *parser_ruby_sourcefile; /* current source file */
    int parser_ruby_sourceline;	/* current line no. */
    rb_encoding *enc;
    rb_encoding *utf8;

    int parser_yydebug;

#ifndef RIPPER
    /* Ruby core only */
    NODE *parser_eval_tree_begin;
    NODE *parser_eval_tree;
    VALUE debug_lines;
    VALUE coverage;
    int nerr;

    token_info *parser_token_info;
#else
    /* Ripper only */
    VALUE parser_ruby_sourcefile_string;
    const char *tokp;
    VALUE delayed;
    int delayed_line;
    int delayed_col;

    VALUE value;
    VALUE result;
    VALUE parsing_thread;
    int toplevel_p;
#endif
};

#define UTF8_ENC() (parser->utf8 ? parser->utf8 : \
		    (parser->utf8 = rb_utf8_encoding()))
#define STR_NEW(p,n) rb_enc_str_new((p),(n),parser->enc)
#define STR_NEW0() rb_enc_str_new(0,0,parser->enc)
#define STR_NEW2(p) rb_enc_str_new((p),strlen(p),parser->enc)
#define STR_NEW3(p,n,e,func) parser_str_new((p),(n),(e),(func),parser->enc)
#define ENC_SINGLE(cr) ((cr)==ENC_CODERANGE_7BIT)
#define TOK_INTERN(mb) rb_intern3(tok(), toklen(), parser->enc)

#ifdef YYMALLOC
void *rb_parser_malloc(struct parser_params *, size_t);
void *rb_parser_realloc(struct parser_params *, void *, size_t);
void *rb_parser_calloc(struct parser_params *, size_t, size_t);
void rb_parser_free(struct parser_params *, void *);
#endif

static int parser_yyerror(struct parser_params*, const char*);
#define yyerror(msg) parser_yyerror(parser, msg)

#define YYLEX_PARAM parser

#define lex_strterm		(parser->parser_lex_strterm)
#define lex_state		(parser->parser_lex_state)
#define cond_stack		(parser->parser_cond_stack)
#define cmdarg_stack		(parser->parser_cmdarg_stack)
#define class_nest		(parser->parser_class_nest)
#define paren_nest		(parser->parser_paren_nest)
#define lpar_beg		(parser->parser_lpar_beg)
#define in_single		(parser->parser_in_single)
#define in_def			(parser->parser_in_def)
#define compile_for_eval	(parser->parser_compile_for_eval)
#define cur_mid			(parser->parser_cur_mid)
#define in_defined		(parser->parser_in_defined)
#define tokenbuf		(parser->parser_tokenbuf)
#define tokidx			(parser->parser_tokidx)
#define toksiz			(parser->parser_toksiz)
#define lex_input		(parser->parser_lex_input)
#define lex_lastline		(parser->parser_lex_lastline)
#define lex_nextline		(parser->parser_lex_nextline)
#define lex_pbeg		(parser->parser_lex_pbeg)
#define lex_p			(parser->parser_lex_p)
#define lex_pend		(parser->parser_lex_pend)
#define heredoc_end		(parser->parser_heredoc_end)
#define command_start		(parser->parser_command_start)
#define deferred_nodes		(parser->parser_deferred_nodes)
#define lex_gets_ptr		(parser->parser_lex_gets_ptr)
#define lex_gets		(parser->parser_lex_gets)
#define lvtbl			(parser->parser_lvtbl)
#define ruby__end__seen		(parser->parser_ruby__end__seen)
#define ruby_sourceline		(parser->parser_ruby_sourceline)
#define ruby_sourcefile		(parser->parser_ruby_sourcefile)
#define yydebug			(parser->parser_yydebug)
#ifdef RIPPER
#else
#define ruby_eval_tree		(parser->parser_eval_tree)
#define ruby_eval_tree_begin	(parser->parser_eval_tree_begin)
#define ruby_debug_lines	(parser->debug_lines)
#define ruby_coverage		(parser->coverage)
#endif

static int yylex(void*, void*);

#ifndef RIPPER
#define yyparse ruby_yyparse

static NODE* node_newnode(struct parser_params *, enum node_type, VALUE, VALUE, VALUE);
#define rb_node_newnode(type, a1, a2, a3) node_newnode(parser, type, a1, a2, a3)

static NODE *cond_gen(struct parser_params*,NODE*);
#define cond(node) cond_gen(parser, node)
static NODE *logop_gen(struct parser_params*,enum node_type,NODE*,NODE*);
#define logop(type,node1,node2) logop_gen(parser, type, node1, node2)

static NODE *newline_node(NODE*);
static void fixpos(NODE*,NODE*);

static int value_expr_gen(struct parser_params*,NODE*);
static void void_expr_gen(struct parser_params*,NODE*);
static NODE *remove_begin(NODE*);
#define value_expr(node) value_expr_gen(parser, (node) = remove_begin(node))
#define void_expr0(node) void_expr_gen(parser, (node))
#define void_expr(node) void_expr0((node) = remove_begin(node))
static void void_stmts_gen(struct parser_params*,NODE*);
#define void_stmts(node) void_stmts_gen(parser, node)
static void reduce_nodes_gen(struct parser_params*,NODE**);
#define reduce_nodes(n) reduce_nodes_gen(parser,n)
static void block_dup_check_gen(struct parser_params*,NODE*,NODE*);
#define block_dup_check(n1,n2) block_dup_check_gen(parser,n1,n2)

static NODE *block_append_gen(struct parser_params*,NODE*,NODE*);
#define block_append(h,t) block_append_gen(parser,h,t)
static NODE *list_append_gen(struct parser_params*,NODE*,NODE*);
#define list_append(l,i) list_append_gen(parser,l,i)
static NODE *list_concat_gen(struct parser_params*,NODE*,NODE*);
#define list_concat(h,t) list_concat_gen(parser,h,t)
static NODE *arg_append_gen(struct parser_params*,NODE*,NODE*);
#define arg_append(h,t) arg_append_gen(parser,h,t)
static NODE *arg_concat_gen(struct parser_params*,NODE*,NODE*);
#define arg_concat(h,t) arg_concat_gen(parser,h,t)
static NODE *literal_concat_gen(struct parser_params*,NODE*,NODE*);
#define literal_concat(h,t) literal_concat_gen(parser,h,t)
static NODE *new_evstr_gen(struct parser_params*,NODE*);
#define new_evstr(n) new_evstr_gen(parser,n)
static NODE *evstr2dstr_gen(struct parser_params*,NODE*);
#define evstr2dstr(n) evstr2dstr_gen(parser,n)
static NODE *splat_array(NODE*);

static NODE *call_bin_op_gen(struct parser_params*,NODE*,ID,NODE*);
#define call_bin_op(recv,id,arg1) call_bin_op_gen(parser, recv,id,arg1)
static NODE *call_uni_op_gen(struct parser_params*,NODE*,ID);
#define call_uni_op(recv,id) call_uni_op_gen(parser, recv,id)

static NODE *new_args_gen(struct parser_params*,NODE*,NODE*,ID,NODE*,ID);
#define new_args(f,o,r,p,b) new_args_gen(parser, f,o,r,p,b)
static void shadowing_lvar_gen(struct parser_params*,ID);
#define shadowing_lvar(name) shadowing_lvar_gen(parser, name)

static NODE *negate_lit(NODE*);
static NODE *ret_args_gen(struct parser_params*,NODE*);
#define ret_args(node) ret_args_gen(parser, node)
static NODE *arg_blk_pass(NODE*,NODE*);
static NODE *new_yield_gen(struct parser_params*,NODE*);
#define new_yield(node) new_yield_gen(parser, node)

static NODE *gettable_gen(struct parser_params*,ID);
#define gettable(id) gettable_gen(parser,id)
static NODE *assignable_gen(struct parser_params*,ID,NODE*);
#define assignable(id,node) assignable_gen(parser, id, node)
static void new_bv_gen(struct parser_params*,ID);
#define new_bv(id) new_bv_gen(parser, id)
static NODE *aryset_gen(struct parser_params*,NODE*,NODE*);
#define aryset(node1,node2) aryset_gen(parser, node1, node2)
static NODE *attrset_gen(struct parser_params*,NODE*,ID);
#define attrset(node,id) attrset_gen(parser, node, id)

static void rb_backref_error_gen(struct parser_params*,NODE*);
#define rb_backref_error(n) rb_backref_error_gen(parser,n)
static NODE *node_assign_gen(struct parser_params*,NODE*,NODE*);
#define node_assign(node1, node2) node_assign_gen(parser, node1, node2)

static NODE *match_op_gen(struct parser_params*,NODE*,NODE*);
#define match_op(node1,node2) match_op_gen(parser, node1, node2)

static void local_push_gen(struct parser_params*,int);
#define local_push(top) local_push_gen(parser,top)
static void local_pop_gen(struct parser_params*);
#define local_pop() local_pop_gen(parser)
static int local_var_gen(struct parser_params*, ID);
#define local_var(id) local_var_gen(parser, id);
static int arg_var_gen(struct parser_params*, ID);
#define arg_var(id) arg_var_gen(parser, id)
static int  local_id_gen(struct parser_params*, ID);
#define local_id(id) local_id_gen(parser, id)
static ID  *local_tbl_gen(struct parser_params*);
#define local_tbl() local_tbl_gen(parser)
static ID   internal_id_gen(struct parser_params*);
#define internal_id() internal_id_gen(parser)

static void dyna_push_gen(struct parser_params*);
#define dyna_push() dyna_push_gen(parser)
static void dyna_pop_gen(struct parser_params*);
#define dyna_pop() dyna_pop_gen(parser)
static int dyna_in_block_gen(struct parser_params*);
#define dyna_in_block() dyna_in_block_gen(parser)
#define dyna_var(id) local_var(id)
static int dvar_defined_gen(struct parser_params*,ID);
#define dvar_defined(id) dvar_defined_gen(parser, id)
static int dvar_curr_gen(struct parser_params*,ID);
#define dvar_curr(id) dvar_curr_gen(parser, id)

static void fixup_nodes(NODE **);

extern int rb_dvar_defined(ID);
extern int rb_local_defined(ID);
extern int rb_parse_in_eval(void);

static VALUE reg_compile_gen(struct parser_params*, VALUE, int);
#define reg_compile(str,options) reg_compile_gen(parser, str, options)
static void reg_fragment_setenc_gen(struct parser_params*, VALUE, int);
#define reg_fragment_setenc(str,options) reg_fragment_setenc_gen(parser, str, options)
static void reg_fragment_check_gen(struct parser_params*, VALUE, int);
#define reg_fragment_check(str,options) reg_fragment_check_gen(parser, str, options)
static NODE *reg_named_capture_assign_gen(struct parser_params* parser, VALUE regexp, NODE *match);
#define reg_named_capture_assign(regexp,match) reg_named_capture_assign_gen(parser,regexp,match)
int rb_enc_symname2_p(const char *, int, rb_encoding *);
#else
#define remove_begin(node) (node)
#endif /* !RIPPER */
static int lvar_defined_gen(struct parser_params*, ID);
#define lvar_defined(id) lvar_defined_gen(parser, id)

#define RE_OPTION_ONCE (1<<16)
#define RE_OPTION_ENCODING_SHIFT 8
#define RE_OPTION_ENCODING(e) (((e)&0xff)<<RE_OPTION_ENCODING_SHIFT)
#define RE_OPTION_ENCODING_IDX(o) (((o)>>RE_OPTION_ENCODING_SHIFT)&0xff)
#define RE_OPTION_ENCODING_NONE(o) ((o)&RE_OPTION_ARG_ENCODING_NONE)
#define RE_OPTION_MASK  0xff
#define RE_OPTION_ARG_ENCODING_NONE 32

#define NODE_STRTERM NODE_ZARRAY	/* nothing to gc */
#define NODE_HEREDOC NODE_ARRAY 	/* 1, 3 to gc */
#define SIGN_EXTEND(x,n) (((1<<(n)-1)^((x)&~(~0<<(n))))-(1<<(n)-1))
#define nd_func u1.id
#if SIZEOF_SHORT == 2
#define nd_term(node) ((signed short)(node)->u2.id)
#else
#define nd_term(node) SIGN_EXTEND((node)->u2.id, CHAR_BIT*2)
#endif
#define nd_paren(node) (char)((node)->u2.id >> CHAR_BIT*2)
#define nd_nest u3.cnt

/****** Ripper *******/

#ifdef RIPPER
#define RIPPER_VERSION "0.1.0"

#include "eventids1.c"
#include "eventids2.c"
static ID ripper_id_gets;

static VALUE ripper_dispatch0(struct parser_params*,ID);
static VALUE ripper_dispatch1(struct parser_params*,ID,VALUE);
static VALUE ripper_dispatch2(struct parser_params*,ID,VALUE,VALUE);
static VALUE ripper_dispatch3(struct parser_params*,ID,VALUE,VALUE,VALUE);
static VALUE ripper_dispatch4(struct parser_params*,ID,VALUE,VALUE,VALUE,VALUE);
static VALUE ripper_dispatch5(struct parser_params*,ID,VALUE,VALUE,VALUE,VALUE,VALUE);

#define dispatch0(n)            ripper_dispatch0(parser, TOKEN_PASTE(ripper_id_, n))
#define dispatch1(n,a)          ripper_dispatch1(parser, TOKEN_PASTE(ripper_id_, n), a)
#define dispatch2(n,a,b)        ripper_dispatch2(parser, TOKEN_PASTE(ripper_id_, n), a, b)
#define dispatch3(n,a,b,c)      ripper_dispatch3(parser, TOKEN_PASTE(ripper_id_, n), a, b, c)
#define dispatch4(n,a,b,c,d)    ripper_dispatch4(parser, TOKEN_PASTE(ripper_id_, n), a, b, c, d)
#define dispatch5(n,a,b,c,d,e)  ripper_dispatch5(parser, TOKEN_PASTE(ripper_id_, n), a, b, c, d, e)

#define yyparse ripper_yyparse

static VALUE ripper_intern(const char*);
static VALUE ripper_id2sym(ID);
#ifdef __GNUC__
#define ripper_id2sym(id) ((id) < 256 && rb_ispunct(id) ? \
			   ID2SYM(id) : ripper_id2sym(id))
#endif

#define arg_new() dispatch0(args_new)
#define arg_add(l,a) dispatch2(args_add, l, a)
#define arg_prepend(l,a) dispatch2(args_prepend, l, a)
#define arg_add_star(l,a) dispatch2(args_add_star, l, a)
#define arg_add_block(l,b) dispatch2(args_add_block, l, b)
#define arg_add_optblock(l,b) ((b)==Qundef? l : dispatch2(args_add_block, l, b))
#define bare_assoc(v) dispatch1(bare_assoc_hash, v)
#define arg_add_assocs(l,b) arg_add(l, bare_assoc(b))

#define args2mrhs(a) dispatch1(mrhs_new_from_args, a)
#define mrhs_new() dispatch0(mrhs_new)
#define mrhs_add(l,a) dispatch2(mrhs_add, l, a)
#define mrhs_add_star(l,a) dispatch2(mrhs_add_star, l, a)

#define mlhs_new() dispatch0(mlhs_new)
#define mlhs_add(l,a) dispatch2(mlhs_add, l, a)
#define mlhs_add_star(l,a) dispatch2(mlhs_add_star, l, a)

#define params_new(pars, opts, rest, pars2, blk) \
        dispatch5(params, pars, opts, rest, pars2, blk)

#define blockvar_new(p,v) dispatch2(block_var, p, v)
#define blockvar_add_star(l,a) dispatch2(block_var_add_star, l, a)
#define blockvar_add_block(l,a) dispatch2(block_var_add_block, l, a)

#define method_optarg(m,a) ((a)==Qundef ? m : dispatch2(method_add_arg,m,a))
#define method_arg(m,a) dispatch2(method_add_arg,m,a)
#define method_add_block(m,b) dispatch2(method_add_block, m, b)

#define escape_Qundef(x) ((x)==Qundef ? Qnil : (x))

#define FIXME 0

#endif /* RIPPER */

#ifndef RIPPER
# define ifndef_ripper(x) x
#else
# define ifndef_ripper(x)
#endif

#ifndef RIPPER
# define rb_warn0(fmt)    rb_compile_warn(ruby_sourcefile, ruby_sourceline, fmt)
# define rb_warnI(fmt,a)  rb_compile_warn(ruby_sourcefile, ruby_sourceline, fmt, a)
# define rb_warnS(fmt,a)  rb_compile_warn(ruby_sourcefile, ruby_sourceline, fmt, a)
# define rb_warning0(fmt) rb_compile_warning(ruby_sourcefile, ruby_sourceline, fmt)
# define rb_warningS(fmt,a) rb_compile_warning(ruby_sourcefile, ruby_sourceline, fmt, a)
#else
# define rb_warn0(fmt)    ripper_warn0(parser, fmt)
# define rb_warnI(fmt,a)  ripper_warnI(parser, fmt, a)
# define rb_warnS(fmt,a)  ripper_warnS(parser, fmt, a)
# define rb_warning0(fmt) ripper_warning0(parser, fmt)
# define rb_warningS(fmt,a) ripper_warningS(parser, fmt, a)
static void ripper_warn0(struct parser_params*, const char*);
static void ripper_warnI(struct parser_params*, const char*, int);
#if 0
static void ripper_warnS(struct parser_params*, const char*, const char*);
#endif
static void ripper_warning0(struct parser_params*, const char*);
static void ripper_warningS(struct parser_params*, const char*, const char*);
#endif

#ifdef RIPPER
static void ripper_compile_error(struct parser_params*, const char *fmt, ...);
# define rb_compile_error ripper_compile_error
# define compile_error ripper_compile_error
# define PARSER_ARG parser,
#else
# define compile_error parser->nerr++,rb_compile_error
# define PARSER_ARG ruby_sourcefile, ruby_sourceline,
#endif

/* Older versions of Yacc set YYMAXDEPTH to a very low value by default (150,
   for instance).  This is too low for Ruby to parse some files, such as
   date/format.rb, therefore bump the value up to at least Bison's default. */
#ifdef OLD_YACC
#ifndef YYMAXDEPTH
#define YYMAXDEPTH 10000
#endif
#endif

#ifndef RIPPER
static void token_info_push(struct parser_params*, const char *token);
static void token_info_pop(struct parser_params*, const char *token);
#endif


/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
#line 598 "parse.y"
typedef union YYSTYPE {
    VALUE val;
    NODE *node;
    ID id;
    int num;
} YYSTYPE;
/* Line 196 of yacc.c.  */
#line 917 "parse.c"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 219 of yacc.c.  */
#line 929 "parse.c"

#if ! defined (YYSIZE_T) && defined (__SIZE_TYPE__)
# define YYSIZE_T __SIZE_TYPE__
#endif
#if ! defined (YYSIZE_T) && defined (size_t)
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T) && (defined (__STDC__) || defined (__cplusplus))
# include <stddef.h> /* INFRINGES ON USER NAME SPACE */
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T)
# define YYSIZE_T unsigned int
#endif

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

#if ! defined (yyoverflow) || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if defined (__STDC__) || defined (__cplusplus)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     define YYINCLUDED_STDLIB_H
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning. */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2005 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM ((YYSIZE_T) -1)
#  endif
#  ifdef __cplusplus
extern "C" {
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if (! defined (malloc) && ! defined (YYINCLUDED_STDLIB_H) \
	&& (defined (__STDC__) || defined (__cplusplus)))
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if (! defined (free) && ! defined (YYINCLUDED_STDLIB_H) \
	&& (defined (__STDC__) || defined (__cplusplus)))
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifdef __cplusplus
}
#  endif
# endif
#endif /* ! defined (yyoverflow) || YYERROR_VERBOSE */


#if (! defined (yyoverflow) \
     && (! defined (__cplusplus) \
	 || (defined (YYSTYPE_IS_TRIVIAL) && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  short int yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short int) + sizeof (YYSTYPE))			\
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined (__GNUC__) && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (0)
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (0)

#endif

#if defined (__STDC__) || defined (__cplusplus)
   typedef signed char yysigned_char;
#else
   typedef short int yysigned_char;
#endif

/* YYFINAL -- State number of the termination state. */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   10368

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  149
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  166
/* YYNRULES -- Number of rules. */
#define YYNRULES  554
/* YYNRULES -- Number of states. */
#define YYNSTATES  961

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   376

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned char yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     148,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,   147,   123,     2,     2,     2,   121,   116,     2,
     143,   144,   119,   117,   141,   118,   140,   120,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   111,   146,
     113,   109,   112,   110,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   139,     2,   145,   115,     2,   142,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   137,   114,   138,   124,     2,     2,     2,
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
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   122,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned short int yyprhs[] =
{
       0,     0,     3,     4,     7,    12,    15,    17,    19,    23,
      26,    27,    32,    36,    40,    44,    47,    51,    55,    59,
      63,    67,    68,    74,    79,    83,    87,    91,    98,   104,
     110,   116,   120,   124,   128,   132,   134,   136,   140,   144,
     147,   150,   152,   154,   156,   158,   161,   164,   167,   169,
     174,   179,   180,   186,   189,   193,   198,   204,   209,   215,
     218,   221,   223,   227,   229,   233,   235,   238,   242,   248,
     251,   256,   259,   264,   266,   270,   272,   276,   279,   283,
     285,   289,   291,   296,   300,   304,   308,   312,   315,   317,
     319,   324,   328,   332,   336,   340,   343,   345,   347,   349,
     352,   354,   358,   360,   362,   364,   366,   368,   370,   372,
     374,   376,   378,   379,   384,   386,   388,   390,   392,   394,
     396,   398,   400,   402,   404,   406,   408,   410,   412,   414,
     416,   418,   420,   422,   424,   426,   428,   430,   432,   434,
     436,   438,   440,   442,   444,   446,   448,   450,   452,   454,
     456,   458,   460,   462,   464,   466,   468,   470,   472,   474,
     476,   478,   480,   482,   484,   486,   488,   490,   492,   494,
     496,   498,   500,   502,   504,   506,   508,   510,   512,   514,
     516,   518,   520,   522,   524,   528,   534,   538,   544,   551,
     557,   563,   569,   575,   580,   584,   588,   592,   596,   600,
     604,   608,   612,   616,   621,   626,   629,   632,   636,   640,
     644,   648,   652,   656,   660,   664,   668,   672,   676,   680,
     684,   687,   690,   694,   698,   702,   706,   707,   712,   719,
     721,   723,   725,   728,   733,   736,   740,   742,   744,   746,
     748,   750,   753,   756,   761,   763,   764,   767,   770,   773,
     775,   777,   779,   782,   786,   791,   795,   800,   803,   805,
     807,   809,   811,   813,   815,   817,   819,   821,   822,   827,
     828,   833,   837,   841,   844,   848,   852,   854,   859,   863,
     865,   866,   873,   878,   882,   885,   887,   890,   893,   900,
     907,   908,   909,   917,   918,   919,   927,   933,   938,   939,
     940,   950,   951,   958,   959,   960,   969,   970,   976,   977,
     984,   985,   986,   996,   998,  1000,  1002,  1004,  1006,  1008,
    1010,  1012,  1014,  1016,  1018,  1020,  1022,  1024,  1026,  1028,
    1030,  1032,  1035,  1037,  1039,  1041,  1047,  1049,  1052,  1054,
    1056,  1058,  1062,  1064,  1068,  1070,  1075,  1082,  1086,  1092,
    1095,  1100,  1102,  1106,  1113,  1122,  1127,  1134,  1139,  1142,
    1149,  1152,  1157,  1164,  1167,  1172,  1175,  1180,  1182,  1184,
    1186,  1190,  1192,  1197,  1199,  1202,  1204,  1208,  1210,  1212,
    1213,  1217,  1222,  1225,  1229,  1233,  1234,  1240,  1243,  1248,
    1253,  1256,  1261,  1266,  1270,  1274,  1278,  1281,  1283,  1288,
    1289,  1295,  1296,  1302,  1308,  1310,  1312,  1319,  1321,  1323,
    1325,  1327,  1330,  1332,  1335,  1337,  1339,  1341,  1343,  1345,
    1347,  1349,  1352,  1356,  1360,  1364,  1368,  1372,  1373,  1377,
    1379,  1382,  1386,  1390,  1391,  1395,  1396,  1399,  1400,  1403,
    1405,  1406,  1410,  1411,  1416,  1418,  1420,  1422,  1424,  1427,
    1429,  1431,  1433,  1435,  1439,  1441,  1443,  1446,  1449,  1451,
    1453,  1455,  1457,  1459,  1461,  1463,  1465,  1467,  1469,  1471,
    1473,  1475,  1477,  1479,  1481,  1483,  1484,  1489,  1492,  1496,
    1499,  1506,  1515,  1520,  1527,  1532,  1539,  1542,  1547,  1554,
    1557,  1562,  1565,  1570,  1572,  1573,  1575,  1577,  1579,  1581,
    1583,  1585,  1587,  1591,  1593,  1597,  1601,  1605,  1607,  1611,
    1613,  1617,  1619,  1621,  1624,  1626,  1628,  1630,  1633,  1636,
    1638,  1640,  1641,  1646,  1648,  1651,  1653,  1657,  1661,  1664,
    1666,  1668,  1670,  1672,  1674,  1676,  1678,  1680,  1682,  1684,
    1686,  1688,  1689,  1691,  1692,  1694,  1697,  1700,  1701,  1703,
    1705,  1707,  1709,  1711,  1714
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const short int yyrhs[] =
{
     150,     0,    -1,    -1,   151,   153,    -1,   153,   252,   228,
     255,    -1,   154,   307,    -1,   314,    -1,   155,    -1,   154,
     313,   155,    -1,     1,   155,    -1,    -1,    45,   177,   156,
     177,    -1,    45,    54,    54,    -1,    45,    54,    64,    -1,
      45,    54,    63,    -1,     6,   178,    -1,   155,    40,   159,
      -1,   155,    41,   159,    -1,   155,    42,   159,    -1,   155,
      43,   159,    -1,   155,    44,   155,    -1,    -1,    47,   157,
     137,   153,   138,    -1,    48,   137,   153,   138,    -1,   172,
     109,   160,    -1,   165,   109,   160,    -1,   279,    87,   160,
      -1,   213,   139,   188,   310,    87,   160,    -1,   213,   140,
      52,    87,   160,    -1,   213,   140,    56,    87,   160,    -1,
     213,    85,    52,    87,   160,    -1,   280,    87,   160,    -1,
     172,   109,   195,    -1,   165,   109,   184,    -1,   165,   109,
     195,    -1,   158,    -1,   160,    -1,   158,    37,   158,    -1,
     158,    38,   158,    -1,    39,   158,    -1,   123,   160,    -1,
     182,    -1,   158,    -1,   164,    -1,   161,    -1,    30,   189,
      -1,    21,   189,    -1,    22,   189,    -1,   245,    -1,   245,
     140,   304,   190,    -1,   245,    85,   304,   190,    -1,    -1,
      94,   163,   234,   153,   138,    -1,   303,   190,    -1,   303,
     190,   162,    -1,   213,   140,   304,   190,    -1,   213,   140,
     304,   190,   162,    -1,   213,    85,   304,   190,    -1,   213,
      85,   304,   190,   162,    -1,    32,   190,    -1,    31,   190,
      -1,   167,    -1,    89,   166,   309,    -1,   167,    -1,    89,
     166,   309,    -1,   169,    -1,   169,   168,    -1,   169,    95,
     171,    -1,   169,    95,   171,   141,   170,    -1,   169,    95,
      -1,   169,    95,   141,   170,    -1,    95,   171,    -1,    95,
     171,   141,   170,    -1,    95,    -1,    95,   141,   170,    -1,
     171,    -1,    89,   166,   309,    -1,   168,   141,    -1,   169,
     168,   141,    -1,   168,    -1,   170,   141,   168,    -1,   277,
      -1,   213,   139,   188,   310,    -1,   213,   140,    52,    -1,
     213,    85,    52,    -1,   213,   140,    56,    -1,   213,    85,
      56,    -1,    86,    56,    -1,   280,    -1,   277,    -1,   213,
     139,   188,   310,    -1,   213,   140,    52,    -1,   213,    85,
      52,    -1,   213,   140,    56,    -1,   213,    85,    56,    -1,
      86,    56,    -1,   280,    -1,    52,    -1,    56,    -1,    86,
     173,    -1,   173,    -1,   213,    85,   173,    -1,    52,    -1,
      56,    -1,    53,    -1,   180,    -1,   181,    -1,   175,    -1,
     273,    -1,   176,    -1,   275,    -1,   177,    -1,    -1,   178,
     141,   179,   177,    -1,   114,    -1,   115,    -1,   116,    -1,
      69,    -1,    70,    -1,    71,    -1,    77,    -1,    78,    -1,
     112,    -1,    73,    -1,   113,    -1,    74,    -1,    72,    -1,
      83,    -1,    84,    -1,   117,    -1,   118,    -1,   119,    -1,
      95,    -1,   120,    -1,   121,    -1,    68,    -1,   123,    -1,
     124,    -1,    66,    -1,    67,    -1,    81,    -1,    82,    -1,
     142,    -1,    49,    -1,    50,    -1,    51,    -1,    47,    -1,
      48,    -1,    45,    -1,    37,    -1,     7,    -1,    21,    -1,
      16,    -1,     3,    -1,     5,    -1,    46,    -1,    26,    -1,
      15,    -1,    14,    -1,    10,    -1,     9,    -1,    36,    -1,
      20,    -1,    25,    -1,     4,    -1,    22,    -1,    34,    -1,
      39,    -1,    38,    -1,    23,    -1,     8,    -1,    24,    -1,
      30,    -1,    33,    -1,    32,    -1,    13,    -1,    35,    -1,
       6,    -1,    17,    -1,    31,    -1,    11,    -1,    12,    -1,
      18,    -1,    19,    -1,   172,   109,   182,    -1,   172,   109,
     182,    44,   182,    -1,   279,    87,   182,    -1,   279,    87,
     182,    44,   182,    -1,   213,   139,   188,   310,    87,   182,
      -1,   213,   140,    52,    87,   182,    -1,   213,   140,    56,
      87,   182,    -1,   213,    85,    52,    87,   182,    -1,   213,
      85,    56,    87,   182,    -1,    86,    56,    87,   182,    -1,
     280,    87,   182,    -1,   182,    79,   182,    -1,   182,    80,
     182,    -1,   182,   117,   182,    -1,   182,   118,   182,    -1,
     182,   119,   182,    -1,   182,   120,   182,    -1,   182,   121,
     182,    -1,   182,    68,   182,    -1,   122,    59,    68,   182,
      -1,   122,    60,    68,   182,    -1,    66,   182,    -1,    67,
     182,    -1,   182,   114,   182,    -1,   182,   115,   182,    -1,
     182,   116,   182,    -1,   182,    69,   182,    -1,   182,   112,
     182,    -1,   182,    73,   182,    -1,   182,   113,   182,    -1,
     182,    74,   182,    -1,   182,    70,   182,    -1,   182,    71,
     182,    -1,   182,    72,   182,    -1,   182,    77,   182,    -1,
     182,    78,   182,    -1,   123,   182,    -1,   124,   182,    -1,
     182,    83,   182,    -1,   182,    84,   182,    -1,   182,    75,
     182,    -1,   182,    76,   182,    -1,    -1,    46,   308,   183,
     182,    -1,   182,   110,   182,   308,   111,   182,    -1,   196,
      -1,   182,    -1,   314,    -1,   194,   311,    -1,   194,   141,
     301,   311,    -1,   301,   311,    -1,   143,   188,   309,    -1,
     314,    -1,   186,    -1,   314,    -1,   189,    -1,   164,    -1,
     194,   193,    -1,   301,   193,    -1,   194,   141,   301,   193,
      -1,   192,    -1,    -1,   191,   189,    -1,    96,   184,    -1,
     141,   192,    -1,   141,    -1,   314,    -1,   184,    -1,    95,
     184,    -1,   194,   141,   184,    -1,   194,   141,    95,   184,
      -1,   194,   141,   184,    -1,   194,   141,    95,   184,    -1,
      95,   184,    -1,   256,    -1,   257,    -1,   260,    -1,   261,
      -1,   262,    -1,   265,    -1,   278,    -1,   280,    -1,    53,
      -1,    -1,   214,   197,   152,   224,    -1,    -1,    90,   158,
     198,   309,    -1,    89,   153,   144,    -1,   213,    85,    56,
      -1,    86,    56,    -1,    92,   185,   145,    -1,    93,   300,
     138,    -1,    30,    -1,    31,   143,   189,   309,    -1,    31,
     143,   309,    -1,    31,    -1,    -1,    46,   308,   143,   199,
     158,   309,    -1,    39,   143,   158,   309,    -1,    39,   143,
     309,    -1,   303,   247,    -1,   246,    -1,   246,   247,    -1,
      97,   239,    -1,   215,   159,   225,   153,   227,   224,    -1,
     216,   159,   225,   153,   228,   224,    -1,    -1,    -1,   217,
     200,   159,   226,   201,   153,   224,    -1,    -1,    -1,   218,
     202,   159,   226,   203,   153,   224,    -1,   219,   159,   307,
     250,   224,    -1,   219,   307,   250,   224,    -1,    -1,    -1,
     220,   229,    25,   204,   159,   226,   205,   153,   224,    -1,
      -1,   221,   174,   281,   206,   152,   224,    -1,    -1,    -1,
     221,    83,   158,   207,   312,   208,   152,   224,    -1,    -1,
     222,   174,   209,   152,   224,    -1,    -1,   223,   175,   210,
     283,   152,   224,    -1,    -1,    -1,   223,   298,   306,   211,
     175,   212,   283,   152,   224,    -1,    21,    -1,    22,    -1,
      23,    -1,    24,    -1,   196,    -1,     7,    -1,    11,    -1,
      12,    -1,    18,    -1,    19,    -1,    16,    -1,    20,    -1,
       3,    -1,     4,    -1,     5,    -1,    10,    -1,   312,    -1,
      13,    -1,   312,    13,    -1,   312,    -1,    27,    -1,   228,
      -1,    14,   159,   225,   153,   227,    -1,   314,    -1,    15,
     153,    -1,   172,    -1,   165,    -1,   286,    -1,    89,   232,
     309,    -1,   230,    -1,   231,   141,   230,    -1,   231,    -1,
     231,   141,    95,   286,    -1,   231,   141,    95,   286,   141,
     231,    -1,   231,   141,    95,    -1,   231,   141,    95,   141,
     231,    -1,    95,   286,    -1,    95,   286,   141,   231,    -1,
      95,    -1,    95,   141,   231,    -1,   288,   141,   291,   141,
     294,   297,    -1,   288,   141,   291,   141,   294,   141,   288,
     297,    -1,   288,   141,   291,   297,    -1,   288,   141,   291,
     141,   288,   297,    -1,   288,   141,   294,   297,    -1,   288,
     141,    -1,   288,   141,   294,   141,   288,   297,    -1,   288,
     297,    -1,   291,   141,   294,   297,    -1,   291,   141,   294,
     141,   288,   297,    -1,   291,   297,    -1,   291,   141,   288,
     297,    -1,   294,   297,    -1,   294,   141,   288,   297,    -1,
     296,    -1,   314,    -1,   235,    -1,   114,   236,   114,    -1,
      76,    -1,   114,   233,   236,   114,    -1,   314,    -1,   146,
     237,    -1,   238,    -1,   237,   141,   238,    -1,    52,    -1,
     285,    -1,    -1,   240,   241,   242,    -1,   143,   284,   236,
     309,    -1,   284,   236,    -1,   107,   153,   138,    -1,    29,
     153,    10,    -1,    -1,    28,   244,   234,   153,    10,    -1,
     164,   243,    -1,   245,   140,   304,   187,    -1,   245,    85,
     304,   187,    -1,   303,   186,    -1,   213,   140,   304,   187,
      -1,   213,    85,   304,   186,    -1,   213,    85,   305,    -1,
     213,   140,   186,    -1,   213,    85,   186,    -1,    32,   186,
      -1,    32,    -1,   213,   139,   188,   310,    -1,    -1,   137,
     248,   234,   153,   138,    -1,    -1,    26,   249,   234,   153,
      10,    -1,    17,   194,   225,   153,   251,    -1,   228,    -1,
     250,    -1,     8,   253,   254,   225,   153,   252,    -1,   314,
      -1,   184,    -1,   195,    -1,   314,    -1,    88,   172,    -1,
     314,    -1,     9,   153,    -1,   314,    -1,   276,    -1,   273,
      -1,   275,    -1,   258,    -1,    62,    -1,   259,    -1,   258,
     259,    -1,    99,   267,   106,    -1,   100,   268,   106,    -1,
     101,   268,    65,    -1,   102,   147,   106,    -1,   102,   263,
     106,    -1,    -1,   263,   264,   147,    -1,   269,    -1,   264,
     269,    -1,   103,   147,   106,    -1,   103,   266,   106,    -1,
      -1,   266,    61,   147,    -1,    -1,   267,   269,    -1,    -1,
     268,   269,    -1,    61,    -1,    -1,   105,   270,   272,    -1,
      -1,   104,   271,   153,   138,    -1,    54,    -1,    55,    -1,
      57,    -1,   280,    -1,    98,   274,    -1,   175,    -1,    55,
      -1,    54,    -1,    57,    -1,    98,   268,   106,    -1,    59,
      -1,    60,    -1,   122,    59,    -1,   122,    60,    -1,    52,
      -1,    55,    -1,    54,    -1,    56,    -1,    57,    -1,    34,
      -1,    33,    -1,    35,    -1,    36,    -1,    50,    -1,    49,
      -1,    51,    -1,   277,    -1,   277,    -1,    63,    -1,    64,
      -1,   312,    -1,    -1,   113,   282,   159,   312,    -1,     1,
     312,    -1,   143,   284,   309,    -1,   284,   312,    -1,   288,
     141,   292,   141,   294,   297,    -1,   288,   141,   292,   141,
     294,   141,   288,   297,    -1,   288,   141,   292,   297,    -1,
     288,   141,   292,   141,   288,   297,    -1,   288,   141,   294,
     297,    -1,   288,   141,   294,   141,   288,   297,    -1,   288,
     297,    -1,   292,   141,   294,   297,    -1,   292,   141,   294,
     141,   288,   297,    -1,   292,   297,    -1,   292,   141,   288,
     297,    -1,   294,   297,    -1,   294,   141,   288,   297,    -1,
     296,    -1,    -1,    56,    -1,    55,    -1,    54,    -1,    57,
      -1,   285,    -1,    52,    -1,   286,    -1,    89,   232,   309,
      -1,   287,    -1,   288,   141,   287,    -1,    52,   109,   184,
      -1,    52,   109,   213,    -1,   290,    -1,   291,   141,   290,
      -1,   289,    -1,   292,   141,   289,    -1,   119,    -1,    95,
      -1,   293,    52,    -1,   293,    -1,   116,    -1,    96,    -1,
     295,    52,    -1,   141,   296,    -1,   314,    -1,   278,    -1,
      -1,   143,   299,   158,   309,    -1,   314,    -1,   301,   311,
      -1,   302,    -1,   301,   141,   302,    -1,   184,    88,   184,
      -1,    58,   184,    -1,    52,    -1,    56,    -1,    53,    -1,
      52,    -1,    56,    -1,    53,    -1,   180,    -1,    52,    -1,
      53,    -1,   180,    -1,   140,    -1,    85,    -1,    -1,   313,
      -1,    -1,   148,    -1,   308,   144,    -1,   308,   145,    -1,
      -1,   148,    -1,   141,    -1,   146,    -1,   148,    -1,   312,
      -1,   313,   146,    -1,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned short int yyrline[] =
{
       0,   763,   763,   763,   793,   826,   837,   846,   854,   862,
     868,   868,   876,   884,   895,   905,   913,   922,   931,   944,
     957,   967,   966,   988,  1000,  1009,  1019,  1048,  1069,  1086,
    1103,  1120,  1130,  1139,  1148,  1157,  1160,  1161,  1169,  1177,
    1185,  1193,  1196,  1208,  1209,  1210,  1218,  1226,  1236,  1237,
    1246,  1258,  1257,  1279,  1288,  1300,  1309,  1321,  1330,  1342,
    1351,  1362,  1363,  1373,  1374,  1384,  1392,  1400,  1408,  1416,
    1424,  1432,  1440,  1448,  1456,  1466,  1467,  1477,  1485,  1495,
    1503,  1513,  1521,  1529,  1537,  1545,  1553,  1565,  1575,  1587,
    1595,  1603,  1611,  1619,  1627,  1640,  1653,  1664,  1672,  1675,
    1683,  1691,  1701,  1702,  1703,  1704,  1714,  1726,  1727,  1730,
    1738,  1741,  1749,  1749,  1759,  1760,  1761,  1762,  1763,  1764,
    1765,  1766,  1767,  1768,  1769,  1770,  1771,  1772,  1773,  1774,
    1775,  1776,  1777,  1778,  1779,  1780,  1781,  1782,  1783,  1784,
    1785,  1786,  1787,  1790,  1790,  1790,  1791,  1791,  1792,  1792,
    1792,  1793,  1793,  1793,  1793,  1794,  1794,  1794,  1794,  1795,
    1795,  1795,  1796,  1796,  1796,  1796,  1797,  1797,  1797,  1797,
    1798,  1798,  1798,  1798,  1799,  1799,  1799,  1799,  1800,  1800,
    1800,  1800,  1801,  1801,  1804,  1813,  1823,  1852,  1883,  1904,
    1921,  1938,  1955,  1966,  1977,  1988,  2002,  2016,  2024,  2032,
    2040,  2048,  2056,  2064,  2073,  2082,  2090,  2098,  2106,  2114,
    2122,  2130,  2138,  2146,  2154,  2162,  2170,  2178,  2186,  2197,
    2205,  2213,  2221,  2229,  2237,  2245,  2253,  2253,  2263,  2273,
    2279,  2291,  2292,  2296,  2304,  2314,  2324,  2325,  2328,  2329,
    2332,  2340,  2348,  2358,  2367,  2376,  2376,  2388,  2398,  2402,
    2406,  2412,  2420,  2428,  2442,  2459,  2473,  2488,  2498,  2499,
    2500,  2501,  2502,  2503,  2504,  2505,  2506,  2515,  2514,  2539,
    2539,  2548,  2556,  2564,  2572,  2585,  2593,  2601,  2609,  2617,
    2625,  2625,  2635,  2643,  2651,  2662,  2663,  2674,  2678,  2690,
    2702,  2702,  2702,  2713,  2713,  2713,  2724,  2735,  2744,  2746,
    2743,  2810,  2809,  2831,  2840,  2830,  2866,  2865,  2887,  2886,
    2915,  2916,  2915,  2941,  2949,  2957,  2965,  2975,  2987,  2994,
    3001,  3008,  3015,  3022,  3029,  3036,  3043,  3050,  3057,  3064,
    3069,  3070,  3077,  3082,  3085,  3086,  3099,  3100,  3110,  3111,
    3114,  3122,  3132,  3140,  3150,  3158,  3166,  3174,  3182,  3190,
    3198,  3209,  3217,  3227,  3235,  3243,  3251,  3259,  3267,  3276,
    3284,  3292,  3300,  3308,  3316,  3324,  3332,  3340,  3350,  3351,
    3357,  3366,  3375,  3386,  3387,  3397,  3404,  3413,  3421,  3427,
    3427,  3449,  3457,  3467,  3471,  3478,  3477,  3498,  3509,  3518,
    3529,  3538,  3548,  3558,  3566,  3577,  3588,  3596,  3604,  3619,
    3618,  3640,  3639,  3660,  3672,  3673,  3676,  3695,  3698,  3706,
    3714,  3717,  3721,  3724,  3732,  3735,  3736,  3744,  3747,  3764,
    3765,  3766,  3776,  3786,  3813,  3855,  3863,  3870,  3877,  3887,
    3895,  3905,  3913,  3920,  3927,  3938,  3945,  3956,  3963,  3973,
    3975,  3974,  3991,  3990,  4012,  4020,  4028,  4036,  4039,  4051,
    4052,  4053,  4054,  4057,  4088,  4089,  4090,  4098,  4108,  4109,
    4110,  4111,  4112,  4113,  4114,  4115,  4116,  4117,  4118,  4119,
    4122,  4132,  4142,  4143,  4146,  4155,  4154,  4162,  4174,  4186,
    4192,  4200,  4208,  4216,  4224,  4232,  4240,  4248,  4256,  4264,
    4272,  4280,  4288,  4296,  4305,  4314,  4323,  4332,  4341,  4352,
    4353,  4365,  4373,  4392,  4399,  4412,  4426,  4440,  4448,  4464,
    4472,  4488,  4489,  4492,  4504,  4515,  4516,  4519,  4535,  4539,
    4549,  4559,  4559,  4588,  4589,  4599,  4606,  4616,  4624,  4634,
    4635,  4636,  4639,  4640,  4641,  4642,  4645,  4646,  4647,  4650,
    4655,  4662,  4663,  4666,  4667,  4670,  4673,  4676,  4677,  4678,
    4681,  4682,  4685,  4686,  4690
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals. */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "keyword_class", "keyword_module",
  "keyword_def", "keyword_undef", "keyword_begin", "keyword_rescue",
  "keyword_ensure", "keyword_end", "keyword_if", "keyword_unless",
  "keyword_then", "keyword_elsif", "keyword_else", "keyword_case",
  "keyword_when", "keyword_while", "keyword_until", "keyword_for",
  "keyword_break", "keyword_next", "keyword_redo", "keyword_retry",
  "keyword_in", "keyword_do", "keyword_do_cond", "keyword_do_block",
  "keyword_do_LAMBDA", "keyword_return", "keyword_yield", "keyword_super",
  "keyword_self", "keyword_nil", "keyword_true", "keyword_false",
  "keyword_and", "keyword_or", "keyword_not", "modifier_if",
  "modifier_unless", "modifier_while", "modifier_until", "modifier_rescue",
  "keyword_alias", "keyword_defined", "keyword_BEGIN", "keyword_END",
  "keyword__LINE__", "keyword__FILE__", "keyword__ENCODING__",
  "tIDENTIFIER", "tFID", "tGVAR", "tIVAR", "tCONSTANT", "tCVAR", "tLABEL",
  "tINTEGER", "tFLOAT", "tSTRING_CONTENT", "tCHAR", "tNTH_REF",
  "tBACK_REF", "tREGEXP_END", "tUPLUS", "tUMINUS", "tPOW", "tCMP", "tEQ",
  "tEQQ", "tNEQ", "tGEQ", "tLEQ", "tANDOP", "tOROP", "tMATCH", "tNMATCH",
  "tDOT2", "tDOT3", "tAREF", "tASET", "tLSHFT", "tRSHFT", "tCOLON2",
  "tCOLON3", "tOP_ASGN", "tASSOC", "tLPAREN", "tLPAREN_ARG", "tRPAREN",
  "tLBRACK", "tLBRACE", "tLBRACE_ARG", "tSTAR", "tAMPER", "tLAMBDA",
  "tSYMBEG", "tSTRING_BEG", "tXSTRING_BEG", "tREGEXP_BEG", "tWORDS_BEG",
  "tQWORDS_BEG", "tSTRING_DBEG", "tSTRING_DVAR", "tSTRING_END", "tLAMBEG",
  "tLOWEST", "'='", "'?'", "':'", "'>'", "'<'", "'|'", "'^'", "'&'", "'+'",
  "'-'", "'*'", "'/'", "'%'", "tUMINUS_NUM", "'!'", "'~'", "idNULL",
  "idRespond_to", "idIFUNC", "idCFUNC", "idThrowState",
  "id_core_set_method_alias", "id_core_set_variable_alias",
  "id_core_undef_method", "id_core_define_method",
  "id_core_define_singleton_method", "id_core_set_postexe", "tLAST_TOKEN",
  "'{'", "'}'", "'['", "'.'", "','", "'`'", "'('", "')'", "']'", "';'",
  "' '", "'\\n'", "$accept", "program", "@1", "bodystmt", "compstmt",
  "stmts", "stmt", "@2", "@3", "expr", "expr_value", "command_call",
  "block_command", "cmd_brace_block", "@4", "command", "mlhs",
  "mlhs_inner", "mlhs_basic", "mlhs_item", "mlhs_head", "mlhs_post",
  "mlhs_node", "lhs", "cname", "cpath", "fname", "fsym", "fitem",
  "undef_list", "@5", "op", "reswords", "arg", "@6", "arg_value",
  "aref_args", "paren_args", "opt_paren_args", "opt_call_args",
  "call_args", "command_args", "@7", "block_arg", "opt_block_arg", "args",
  "mrhs", "primary", "@8", "@9", "@10", "@11", "@12", "@13", "@14", "@15",
  "@16", "@17", "@18", "@19", "@20", "@21", "@22", "@23", "primary_value",
  "k_begin", "k_if", "k_unless", "k_while", "k_until", "k_case", "k_for",
  "k_class", "k_module", "k_def", "k_end", "then", "do", "if_tail",
  "opt_else", "for_var", "f_marg", "f_marg_list", "f_margs", "block_param",
  "opt_block_param", "block_param_def", "opt_bv_decl", "bv_decls", "bvar",
  "lambda", "@24", "f_larglist", "lambda_body", "do_block", "@25",
  "block_call", "method_call", "brace_block", "@26", "@27", "case_body",
  "cases", "opt_rescue", "exc_list", "exc_var", "opt_ensure", "literal",
  "strings", "string", "string1", "xstring", "regexp", "words",
  "word_list", "word", "qwords", "qword_list", "string_contents",
  "xstring_contents", "string_content", "@28", "@29", "string_dvar",
  "symbol", "sym", "dsym", "numeric", "variable", "var_ref", "var_lhs",
  "backref", "superclass", "@30", "f_arglist", "f_args", "f_bad_arg",
  "f_norm_arg", "f_arg_item", "f_arg", "f_opt", "f_block_opt",
  "f_block_optarg", "f_optarg", "restarg_mark", "f_rest_arg",
  "blkarg_mark", "f_block_arg", "opt_f_block_arg", "singleton", "@31",
  "assoc_list", "assocs", "assoc", "operation", "operation2", "operation3",
  "dot_or_colon", "opt_terms", "opt_nl", "rparen", "rbracket", "trailer",
  "term", "terms", "none", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const unsigned short int yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,    61,
      63,    58,    62,    60,   124,    94,    38,    43,    45,    42,
      47,    37,   364,    33,   126,   365,   366,   367,   368,   369,
     370,   371,   372,   373,   374,   375,   376,   123,   125,    91,
      46,    44,    96,    40,    41,    93,    59,    32,    10
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned short int yyr1[] =
{
       0,   149,   151,   150,   152,   153,   154,   154,   154,   154,
     156,   155,   155,   155,   155,   155,   155,   155,   155,   155,
     155,   157,   155,   155,   155,   155,   155,   155,   155,   155,
     155,   155,   155,   155,   155,   155,   158,   158,   158,   158,
     158,   158,   159,   160,   160,   160,   160,   160,   161,   161,
     161,   163,   162,   164,   164,   164,   164,   164,   164,   164,
     164,   165,   165,   166,   166,   167,   167,   167,   167,   167,
     167,   167,   167,   167,   167,   168,   168,   169,   169,   170,
     170,   171,   171,   171,   171,   171,   171,   171,   171,   172,
     172,   172,   172,   172,   172,   172,   172,   173,   173,   174,
     174,   174,   175,   175,   175,   175,   175,   176,   176,   177,
     177,   178,   179,   178,   180,   180,   180,   180,   180,   180,
     180,   180,   180,   180,   180,   180,   180,   180,   180,   180,
     180,   180,   180,   180,   180,   180,   180,   180,   180,   180,
     180,   180,   180,   181,   181,   181,   181,   181,   181,   181,
     181,   181,   181,   181,   181,   181,   181,   181,   181,   181,
     181,   181,   181,   181,   181,   181,   181,   181,   181,   181,
     181,   181,   181,   181,   181,   181,   181,   181,   181,   181,
     181,   181,   181,   181,   182,   182,   182,   182,   182,   182,
     182,   182,   182,   182,   182,   182,   182,   182,   182,   182,
     182,   182,   182,   182,   182,   182,   182,   182,   182,   182,
     182,   182,   182,   182,   182,   182,   182,   182,   182,   182,
     182,   182,   182,   182,   182,   182,   183,   182,   182,   182,
     184,   185,   185,   185,   185,   186,   187,   187,   188,   188,
     189,   189,   189,   189,   189,   191,   190,   192,   193,   193,
     193,   194,   194,   194,   194,   195,   195,   195,   196,   196,
     196,   196,   196,   196,   196,   196,   196,   197,   196,   198,
     196,   196,   196,   196,   196,   196,   196,   196,   196,   196,
     199,   196,   196,   196,   196,   196,   196,   196,   196,   196,
     200,   201,   196,   202,   203,   196,   196,   196,   204,   205,
     196,   206,   196,   207,   208,   196,   209,   196,   210,   196,
     211,   212,   196,   196,   196,   196,   196,   213,   214,   215,
     216,   217,   218,   219,   220,   221,   222,   223,   224,   225,
     225,   225,   226,   226,   227,   227,   228,   228,   229,   229,
     230,   230,   231,   231,   232,   232,   232,   232,   232,   232,
     232,   232,   232,   233,   233,   233,   233,   233,   233,   233,
     233,   233,   233,   233,   233,   233,   233,   233,   234,   234,
     235,   235,   235,   236,   236,   237,   237,   238,   238,   240,
     239,   241,   241,   242,   242,   244,   243,   245,   245,   245,
     246,   246,   246,   246,   246,   246,   246,   246,   246,   248,
     247,   249,   247,   250,   251,   251,   252,   252,   253,   253,
     253,   254,   254,   255,   255,   256,   256,   256,   257,   258,
     258,   258,   259,   260,   261,   262,   262,   263,   263,   264,
     264,   265,   265,   266,   266,   267,   267,   268,   268,   269,
     270,   269,   271,   269,   272,   272,   272,   272,   273,   274,
     274,   274,   274,   275,   276,   276,   276,   276,   277,   277,
     277,   277,   277,   277,   277,   277,   277,   277,   277,   277,
     278,   279,   280,   280,   281,   282,   281,   281,   283,   283,
     284,   284,   284,   284,   284,   284,   284,   284,   284,   284,
     284,   284,   284,   284,   284,   285,   285,   285,   285,   286,
     286,   287,   287,   288,   288,   289,   290,   291,   291,   292,
     292,   293,   293,   294,   294,   295,   295,   296,   297,   297,
     298,   299,   298,   300,   300,   301,   301,   302,   302,   303,
     303,   303,   304,   304,   304,   304,   305,   305,   305,   306,
     306,   307,   307,   308,   308,   309,   310,   311,   311,   311,
     312,   312,   313,   313,   314
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] =
{
       0,     2,     0,     2,     4,     2,     1,     1,     3,     2,
       0,     4,     3,     3,     3,     2,     3,     3,     3,     3,
       3,     0,     5,     4,     3,     3,     3,     6,     5,     5,
       5,     3,     3,     3,     3,     1,     1,     3,     3,     2,
       2,     1,     1,     1,     1,     2,     2,     2,     1,     4,
       4,     0,     5,     2,     3,     4,     5,     4,     5,     2,
       2,     1,     3,     1,     3,     1,     2,     3,     5,     2,
       4,     2,     4,     1,     3,     1,     3,     2,     3,     1,
       3,     1,     4,     3,     3,     3,     3,     2,     1,     1,
       4,     3,     3,     3,     3,     2,     1,     1,     1,     2,
       1,     3,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     0,     4,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     3,     5,     3,     5,     6,     5,
       5,     5,     5,     4,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     4,     4,     2,     2,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       2,     2,     3,     3,     3,     3,     0,     4,     6,     1,
       1,     1,     2,     4,     2,     3,     1,     1,     1,     1,
       1,     2,     2,     4,     1,     0,     2,     2,     2,     1,
       1,     1,     2,     3,     4,     3,     4,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     0,     4,     0,
       4,     3,     3,     2,     3,     3,     1,     4,     3,     1,
       0,     6,     4,     3,     2,     1,     2,     2,     6,     6,
       0,     0,     7,     0,     0,     7,     5,     4,     0,     0,
       9,     0,     6,     0,     0,     8,     0,     5,     0,     6,
       0,     0,     9,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     2,     1,     1,     1,     5,     1,     2,     1,     1,
       1,     3,     1,     3,     1,     4,     6,     3,     5,     2,
       4,     1,     3,     6,     8,     4,     6,     4,     2,     6,
       2,     4,     6,     2,     4,     2,     4,     1,     1,     1,
       3,     1,     4,     1,     2,     1,     3,     1,     1,     0,
       3,     4,     2,     3,     3,     0,     5,     2,     4,     4,
       2,     4,     4,     3,     3,     3,     2,     1,     4,     0,
       5,     0,     5,     5,     1,     1,     6,     1,     1,     1,
       1,     2,     1,     2,     1,     1,     1,     1,     1,     1,
       1,     2,     3,     3,     3,     3,     3,     0,     3,     1,
       2,     3,     3,     0,     3,     0,     2,     0,     2,     1,
       0,     3,     0,     4,     1,     1,     1,     1,     2,     1,
       1,     1,     1,     3,     1,     1,     2,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     0,     4,     2,     3,     2,
       6,     8,     4,     6,     4,     6,     2,     4,     6,     2,
       4,     2,     4,     1,     0,     1,     1,     1,     1,     1,
       1,     1,     3,     1,     3,     3,     3,     1,     3,     1,
       3,     1,     1,     2,     1,     1,     1,     2,     2,     1,
       1,     0,     4,     1,     2,     1,     3,     3,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     0,     1,     0,     1,     2,     2,     0,     1,     1,
       1,     1,     1,     2,     0
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned short int yydefact[] =
{
       2,     0,     0,     1,     0,   325,   326,   327,     0,   318,
     319,   320,   323,   321,   322,   324,   313,   314,   315,   316,
     276,   245,   245,   464,   463,   465,   466,     0,     0,   543,
      21,     0,   468,   467,   469,   458,   531,   460,   459,   461,
     462,   454,   455,   419,   472,   473,     0,     0,     0,     0,
       0,   554,   554,    73,   379,   437,   435,   437,   437,   427,
     433,     0,     0,     0,     3,   541,     7,    35,    36,    44,
      43,     0,    61,     0,    65,    75,     0,    41,   229,     0,
     267,     0,     0,   290,   293,   541,     0,     0,     0,     0,
      48,   285,   258,   259,   418,   420,   260,   261,   262,   263,
     416,   417,   415,   470,   264,     0,   265,   245,     6,     9,
     153,   164,   154,   177,   150,   170,   160,   159,   180,   181,
     175,   158,   157,   152,   178,   182,   183,   162,   151,   165,
     169,   171,   163,   156,   172,   179,   174,   173,   166,   176,
     161,   149,   168,   167,   148,   155,   146,   147,   143,   144,
     145,   102,   104,   103,   138,   139,   135,   117,   118,   119,
     126,   123,   125,   120,   121,   140,   141,   127,   128,   132,
     122,   124,   114,   115,   116,   129,   130,   131,   133,   134,
     136,   137,   142,   107,   109,   111,    15,   105,   106,   108,
     110,   313,   314,   276,     0,     0,     0,     0,     0,     0,
       0,   240,     0,   230,   251,    46,   244,   554,     0,   470,
       0,   265,   554,   525,    47,    45,   543,    60,     0,   554,
     396,    59,   543,    39,     0,    10,   544,   226,     0,     0,
     279,   397,   205,     0,     0,   206,   273,     0,     0,   543,
      63,   269,     0,   547,   547,   231,     0,     0,   547,   523,
     543,     0,     0,     0,    71,   317,     0,    81,    88,   287,
     494,   451,   450,   452,   449,     0,   448,     0,     0,     0,
       0,     0,     0,     0,   456,   457,    40,   220,   221,   550,
     551,     5,   552,   542,     0,     0,     0,     0,     0,     0,
       0,   385,   387,     0,    77,     0,    69,    66,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   554,     0,     0,    42,
       0,     0,     0,     0,   541,     0,   542,     0,   339,   338,
       0,     0,   470,   265,    97,    98,     0,     0,   100,     0,
       0,   470,   265,   306,   173,   166,   176,   161,   143,   144,
     145,   102,   103,   521,   308,   520,     0,     0,     0,   401,
     399,   286,   421,     0,     0,   390,    53,   284,   112,   528,
     273,   252,   247,     0,     0,   249,   241,   250,     0,   554,
       0,     0,     0,   249,   242,   543,     0,   278,   246,   543,
     239,   238,   543,   283,    12,    14,    13,     0,   280,     0,
       0,     0,     0,     0,     0,   543,   271,    62,   543,   274,
     549,   548,   232,   549,   234,   275,   524,     0,    87,   456,
     457,    79,    74,     0,     0,   554,     0,   500,   497,   496,
     495,   498,     0,   512,   516,   515,   511,   494,     0,   554,
     499,   501,   503,   554,   509,   554,   514,   554,     0,   493,
     439,   442,   440,   453,   438,   422,   436,   423,   424,   425,
     426,     0,   429,   431,     0,   432,     0,     0,   553,     8,
      16,    17,    18,    19,    20,    37,    38,   554,     0,    25,
      33,     0,    34,   543,     0,    67,    78,    24,   184,   251,
      32,   202,   210,   215,   216,   217,   212,   214,   224,   225,
     218,   219,   195,   196,   222,   223,   543,   211,   213,   207,
     208,   209,   197,   198,   199,   200,   201,   532,   537,   533,
     538,   395,   245,   393,   543,   532,   534,   533,   535,   394,
     245,     0,   554,   330,     0,   329,     0,     0,     0,     0,
       0,     0,   273,     0,   554,     0,   298,   303,    97,    98,
      99,     0,   475,   301,   474,     0,   554,     0,     0,     0,
     494,   540,   539,   310,   532,   533,   245,   245,   554,   554,
      26,   186,    31,   194,    51,    54,     0,   184,   527,     0,
     253,   248,   554,   536,   533,   543,   532,   533,   526,   277,
     545,   235,   282,    11,     0,   227,     0,    23,     0,   554,
     193,    64,   270,   547,     0,    72,    84,    86,   543,   532,
     533,     0,   500,     0,   351,   342,   344,   543,   340,   554,
       0,     0,   380,     0,   382,   373,     0,   486,   519,     0,
     489,   513,     0,   491,   517,     0,     0,   428,   430,   434,
     203,   204,   371,   554,     0,   369,   368,   257,     0,    76,
      70,     0,     0,     0,     0,     0,   392,    57,     0,   398,
       0,     0,   237,   391,    55,   236,   328,   268,   554,   554,
     407,   554,   331,   554,   333,   291,   332,   294,     0,     0,
     297,   536,   272,   543,   532,   533,     0,     0,   477,     0,
       0,    97,    98,   101,   543,     0,   543,   494,     0,     0,
       0,   389,    50,   388,    49,     0,     0,     0,   554,   113,
     254,   243,     0,   398,     0,     0,   543,    22,   233,    80,
      82,   505,   543,     0,   349,     0,   502,   543,     0,     0,
     377,   374,   375,   378,   504,   554,   554,   518,   554,   510,
     554,   554,     0,   444,   445,   446,   441,   447,   500,   554,
       0,   554,   507,   554,   554,   367,     0,     0,   255,    68,
     185,     0,    30,   191,   192,    58,   546,     0,    28,   189,
      29,   190,    56,   408,   409,   554,   410,     0,   554,   336,
       0,     0,   334,     0,     0,     0,   296,     0,     0,   398,
       0,   304,     0,     0,   398,   307,   522,   543,     0,   479,
     311,     0,     0,   187,     0,     0,   281,   341,   352,     0,
     347,   343,   381,   384,   383,     0,     0,   482,     0,   484,
       0,   490,     0,   487,   492,   443,     0,     0,   370,   358,
     360,     0,   363,     0,   365,   386,   256,   228,    27,   188,
       0,     0,   412,   337,     0,     4,   414,     0,   288,   289,
       0,     0,   253,   554,   299,     0,   476,   302,   478,   309,
     494,   402,   400,     0,     0,   350,     0,   345,   376,   554,
     554,   554,   554,     0,   506,   372,   554,   554,   554,   508,
     554,   554,     0,   411,     0,    89,    96,     0,   413,     0,
     292,   295,   404,   405,   403,     0,     0,     0,    52,   348,
       0,   483,     0,   480,   485,   488,   273,     0,     0,   355,
       0,   357,   364,     0,   361,   366,    95,     0,   554,     0,
     554,   554,     0,   305,     0,   346,   554,   536,   272,   554,
     554,   554,   554,   536,    94,   543,   532,   533,   406,   335,
     300,   312,   481,   356,     0,   353,   359,   362,   398,   554,
     354
};

/* YYDEFGOTO[NTERM-NUM]. */
static const short int yydefgoto[] =
{
      -1,     1,     2,   541,   542,    65,    66,   407,   228,    67,
     330,    68,    69,   585,   718,    70,    71,   239,    72,    73,
      74,   432,    75,   202,   348,   349,   183,   184,   185,   186,
     586,   538,   188,    77,   409,   204,   242,   531,   673,   399,
     400,   217,   218,   206,   386,   207,   492,    78,   328,   418,
     604,   332,   794,   333,   795,   696,   905,   700,   697,   865,
     568,   570,   710,   870,   233,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,   677,   544,   685,   791,   792,
     341,   625,   626,   627,   759,   654,   655,   634,   741,   742,
     259,   260,   448,   632,   292,   487,    90,    91,   377,   579,
     578,   551,   904,   679,   785,   851,   855,    92,    93,    94,
      95,    96,    97,    98,   271,   471,    99,   273,   267,   265,
     464,   646,   645,   756,   100,   266,   101,   102,   209,   104,
     210,   211,   563,   699,   708,   709,   450,   451,   452,   453,
     454,   762,   763,   455,   456,   457,   458,   747,   637,   366,
     569,   247,   212,   213,   234,   608,   533,   573,   281,   396,
     397,   669,   422,   545,   336,   108
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -750
static const short int yypact[] =
{
    -750,    93,  2393,  -750,  6854,  -750,  -750,  -750,  6365,  -750,
    -750,  -750,  -750,  -750,  -750,  -750,  6963,  6963,  -750,  -750,
    6963,  3086,  2660,  -750,  -750,  -750,  -750,  5944,  6225,   -37,
    -750,   -18,  -750,  -750,  -750,  5346,  2802,  -750,  -750,  5474,
    -750,  -750,  -750,  -750,  -750,  -750,  8162,  8162,    87,  4160,
    8271,  7181,  7508,  6630,  -750,  6085,  -750,  -750,  -750,    23,
      50,   291,  8380,  8162,  -750,    -4,   951,   271,  -750,  -750,
     117,   132,  -750,    16,  8489,  -750,   165,  2501,    33,   238,
    -750,  8271,  8271,  -750,  -750,  4821,  8594,  8699,  8804,  5218,
      20,    82,  -750,  -750,    69,  -750,  -750,  -750,  -750,  -750,
    -750,  -750,  -750,    66,  -750,   203,   309,   210,  -750,   951,
    -750,  -750,  -750,  -750,  -750,  -750,  -750,  -750,  -750,  -750,
    -750,  -750,  -750,  -750,  -750,  -750,  -750,  -750,  -750,  -750,
    -750,  -750,  -750,  -750,  -750,  -750,  -750,  -750,  -750,  -750,
    -750,  -750,  -750,  -750,  -750,  -750,  -750,  -750,  -750,  -750,
    -750,  -750,  -750,  -750,  -750,  -750,  -750,  -750,  -750,  -750,
    -750,  -750,  -750,  -750,  -750,  -750,  -750,  -750,  -750,  -750,
    -750,  -750,  -750,  -750,  -750,  -750,  -750,  -750,  -750,  -750,
    -750,  -750,  -750,  -750,  -750,  -750,   162,  -750,  -750,  -750,
    -750,  -750,  -750,  -750,   222,  8162,   277,  4291,  8162,  8162,
    8162,  -750,   261,  2501,   287,  -750,  -750,   270,   244,    19,
     351,    37,   300,  -750,  -750,  -750,  4699,  -750,  6963,  6963,
    -750,  -750,  4952,  -750,   322,  -750,  -750,   335,   312,  4422,
     368,   384,  -750,   267,   210,   441,   510,  4160,   308,   -37,
     401,   271,   393,   365,   371,  -750,   287,   390,   371,  -750,
     -37,   483,   302,  8909,   400,  -750,   273,   347,   362,  -750,
     592,  -750,  -750,  -750,  -750,   628,  -750,   685,   700,   420,
     449,   725,   450,    78,   506,   515,  -750,  -750,  -750,  -750,
    -750,  -750,  -750,  5074,  8271,  8271,  8271,  8271,  6854,  8271,
    8271,  -750,  -750,  7617,  -750,  4160,  6742,   453,  7617,  8162,
    8162,  8162,  8162,  8162,  8162,  8162,  8162,  8162,  8162,  8162,
    8162,  8162,  8162,  8162,  8162,  8162,  8162,  8162,  8162,  8162,
    8162,  8162,  8162,  8162,  8162,  9176,  6963,  9254,  3460,   271,
     158,   158,  8271,  8271,    -4,   573,   457,   539,  -750,  -750,
     383,   585,    44,    77,    40,   340,  8271,   537,  -750,    80,
     391,  -750,  -750,  -750,    27,    38,    46,    65,   192,   227,
     233,   249,   283,  -750,  -750,  -750,   289, 10190, 10190,  -750,
    -750,  -750,  -750,  8380,  8380,  -750,   517,  -750,  -750,  -750,
     180,  -750,  -750,  8162,  8162,  7072,  -750,  -750,  9332,  6963,
    9410,  8162,  8162,  7290,  -750,   -37,   471,  -750,  -750,   -37,
    -750,  -750,    62,  -750,  -750,  -750,  -750,  6365,  -750,  8162,
    4422,   485,  9332,  9410,  8162,   -37,  -750,   480,   -37,  -750,
    7399,  -750,  -750,  7508,  -750,  -750,  -750,   335,   397,  -750,
    -750,  -750,   486,  8909,  9488,  6963,  9566,   521,  -750,  -750,
    -750,  -750,   901,  -750,  -750,  -750,  -750,   855,    59,   490,
    -750,  -750,  -750,   496,  -750,   509,   574,   520,   579,  -750,
    -750,  -750,  -750,  -750,  -750,  -750,  -750,  -750,  -750,  -750,
    -750,    43,  -750,  -750,   519,  -750,  8162,  8162,  -750,   951,
    -750,  -750,  -750,  -750,  -750,  -750,  -750,     2,  8162,  -750,
     529,   530,  -750,   -37,  8909,   532,  -750,  -750,  1756,  -750,
    -750,   441,  2785,  2785,  2785,  2785,   636,   636,  2927,  1560,
    2785,  2785,  2643,  2643,   660,   660,  2172,   636,   636,   757,
     757,   815,    42,    42,   441,   441,   441,  3228,  5716,  3314,
    5830,  -750,   384,  -750,   -37,   513,  -750,   554,  -750,  -750,
    2944,   669,   674,  -750,  3606,   671,  3898,    67,    67,   573,
    7726,   669,    96,  9644,  6963,  9722,  -750,   271,  -750,   397,
    -750,    -4,  -750,  -750,  -750,  9800,  6963,  9878,  3460,  8271,
    1187,  -750,  -750,  -750,  -750,  -750,  1957,  1957,     2,     2,
    -750,  2253,  -750,  2501,  -750,  -750,  6365, 10247,  -750,  8162,
     287,  -750,   300,  5602,  2518,   -37,   306,   317,  -750,  -750,
    -750,  -750,  -750,  -750,  8271,  2501,   547,  -750,   384,   384,
    2501,    17,  -750,   371,  8909,   486,   414,   448,   -37,   198,
     286,  8162,  -750,   901,   417,  -750,   545,   -37,  -750,   490,
    4568,  4422,  -750,   759,  -750,  -750,   855,  -750,  -750,   855,
    -750,  -750,   524,  -750,  -750,  4422,   827,  -750,  -750,  -750,
     441,   441,  -750,  1085,  4568,  -750,  -750,   550,  7835,  -750,
     486,  8909,  8162,   590,  8380,  8162,  -750,   517,   557,   603,
    8380,  8380,  -750,  -750,   517,  -750,  -750,  -750,  7944,   688,
    -750,   421,  -750,   688,  -750,  -750,  -750,  -750,   669,   127,
    -750,    57,   156,   -37,    97,   104,  8271,    -4,  -750,  8271,
    3460,   414,   448,  -750,   -37,   669,    62,   855,  3460,    -4,
    6505,  -750,  -750,  -750,  -750,  4568,  4422,  8162,     2,  -750,
    -750,  -750,  8162,   366,  8162,  8162,    62,  -750,  -750,  -750,
     354,  -750,   -37,   886,   564,   992,  -750,   -37,   697,   571,
    -750,   569,  -750,  -750,  -750,   576,   583,  -750,   584,  -750,
     586,   584,   577,  -750,  -750,  -750,  -750,  -750,   620,   490,
     623,   602,  -750,   604,   607,  -750,   748,  8162,   619,   486,
    2501,  8162,  -750,  2501,  2501,  -750,  -750,  8380,  -750,  2501,
    -750,  2501,  -750,   529,  -750,   675,  -750,  4029,   755,  -750,
    8271,   669,  -750,   669,  4568,  4568,  -750,  8053,  3752,   108,
      67,  -750,    -4,   669,  -750,  -750,  -750,   -37,   669,  -750,
    -750,   762,   627,  2501,  4422,  8162,  -750,  -750,   626,   886,
     601,  -750,  -750,  -750,  -750,   759,   855,  -750,   524,  -750,
     524,  -750,   524,  -750,  -750,  -750,  9014,   668,  -750,  1281,
    -750,  1281,  -750,   524,  -750,  -750,   646,  2501,  -750,  2501,
    9119,   158,  -750,  -750,  4568,  -750,  -750,   158,  -750,  -750,
     669,   669,  -750,   115,  -750,  3460,  -750,  -750,  -750,  -750,
    1187,  -750,  -750,   656,   886,   626,   886,   654,  -750,   584,
     658,   584,   584,   741,   410,  -750,   659,   666,   584,  -750,
     677,   584,   745,  -750,   432,   347,   362,  3460,  -750,  3606,
    -750,  -750,  -750,  -750,  -750,  4568,   669,  3460,  -750,   626,
     886,  -750,   524,  -750,  -750,  -750,  -750,  9956,  1281,  -750,
     524,  -750,  -750,   524,  -750,  -750,   397, 10034,  6963, 10112,
     674,   421,   669,  -750,   669,   626,   584,   667,   678,   584,
     681,   584,   584,   152,   448,   -37,   174,   190,  -750,  -750,
    -750,  -750,  -750,  -750,   524,  -750,  -750,  -750,   195,   584,
    -750
};

/* YYPGOTO[NTERM-NUM].  */
static const short int yypgoto[] =
{
    -750,  -750,  -750,  -544,   435,  -750,    22,  -750,  -750,    70,
     -54,   -46,  -750,  -153,  -750,    -7,   723,  -141,    21,   -28,
    -750,  -391,   -16,  1410,  -303,   736,   -50,  -750,    10,  -750,
    -750,    12,  -750,   803,  -750,   150,  -750,     8,  -113,  -294,
      48,    28,  -750,  -276,  -200,   -43,  -279,   -52,  -750,  -750,
    -750,  -750,  -750,  -750,  -750,  -750,  -750,  -750,  -750,  -750,
    -750,  -750,  -750,  -750,    -2,  -750,  -750,  -750,  -750,  -750,
    -750,  -750,  -750,  -750,  -750,   367,  -325,  -531,  -104,  -592,
    -750,  -678,  -670,   209,  -750,  -489,  -750,  -555,  -750,    14,
    -750,  -750,  -750,  -750,  -750,  -750,  -750,  -750,   742,  -750,
    -750,  -538,  -750,   -95,  -750,  -750,  -750,  -750,  -750,  -750,
     743,  -750,  -750,  -750,  -750,  -750,  -750,  -750,  -750,   431,
    -212,  -750,  -750,  -750,    13,  -750,    15,  -750,  1273,   749,
    1485,  1362,  -750,  -750,   -24,  -253,  -579,  -127,  -574,   120,
    -583,  -749,     5,   211,  -750,  -578,  -750,  -256,   266,  -750,
    -750,  -750,    25,  -286,   172,  -254,  -750,  -750,   -58,   -26,
     281,  -566,  -199,   -32,   786,   863
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -555
static const short int yytable[] =
{
      79,   255,    79,   227,   459,   264,   546,   449,   243,   201,
     201,   688,   394,   201,   208,   208,   276,   687,   208,   500,
     187,   189,   255,   190,   705,   208,   109,   335,   331,   723,
     220,   334,   534,   282,   255,   255,   255,   254,   225,   364,
     187,   189,   615,   190,   560,   424,   297,    79,   208,   426,
     221,   256,   730,   282,   743,   466,   749,   821,   746,   472,
     208,   750,   744,   818,   205,   214,  -529,   187,   215,   -89,
     240,   532,   256,   540,   737,   764,   244,   248,   652,   208,
     208,   561,   -92,   208,   340,   350,   350,   788,   630,   715,
     716,   793,   889,     3,   684,   595,   415,   223,   760,   289,
     290,   187,   -96,   660,   460,   367,  -471,   598,   369,   591,
     299,   226,  -464,   576,   577,   375,   653,   591,  -317,   229,
     241,   -95,   -91,  -463,   392,  -458,   -62,   799,   -89,   -93,
     787,  -465,   550,   -90,   532,   376,   540,   598,   804,   474,
     543,   618,   279,   236,   280,   291,   -96,   461,   462,   875,
    -466,   329,   329,  -471,   493,   329,   803,   294,   -76,   609,
     368,   322,   323,   324,   808,   -92,   631,  -464,    56,   889,
     270,   543,  -317,  -317,   107,   -89,   107,  -529,  -463,  -458,
    -458,   -94,   609,  -529,   475,   -81,  -465,   -91,   107,   107,
     647,   459,   107,   562,   629,    79,   821,   272,   -84,   107,
    -532,   255,   246,   -93,   837,  -466,   909,   -81,   -90,   201,
     226,   201,   201,   279,   208,   280,   208,   208,   -88,   370,
     208,   107,   107,   -83,   427,   431,   279,    79,   280,   814,
     480,   481,   482,   483,   107,    79,   369,   -87,   -83,   220,
     935,   293,   375,   749,   255,   -85,   743,   489,   880,   -82,
     491,   256,   497,   107,   107,   491,   744,   107,   240,   648,
     693,   887,   703,   890,   395,   744,   398,   414,   797,   864,
     769,   902,   704,   279,   298,   280,   549,  -468,   547,   548,
     495,    79,   208,   208,   208,   208,    79,   208,   208,   -95,
     373,   208,   402,    79,   256,  -532,   208,   -86,   -92,  -533,
     -92,   609,   282,   378,   279,   479,   280,   -83,   289,   290,
     484,   -85,  -467,   609,   459,   628,   240,   564,  -469,   201,
     -91,   906,   -91,   325,   208,   903,    79,   580,   582,   388,
     208,   208,  -468,   380,  -458,   539,   -93,   530,   -93,   -83,
     940,   -90,   -83,   -90,   208,   379,   -83,   370,   381,   382,
     274,   275,   412,   219,   329,   329,   329,   329,   434,   485,
     486,   429,   430,   934,   798,   222,  -530,  -467,  -461,   107,
     383,   208,   208,  -469,   571,   384,   404,   326,   327,   958,
    -398,   255,   201,   389,   390,   405,   406,   208,   107,  -458,
     107,   107,   721,   724,   107,   -85,   374,   765,   539,   784,
     530,   107,   329,   329,   725,   431,   389,   413,    79,   107,
     592,   385,   435,   436,   728,   -91,   557,   603,   -96,   187,
     189,   539,   190,  -461,   530,  -461,   -93,   -85,   201,   572,
     -85,   256,  -470,   208,   -85,   790,   787,    64,   391,  -398,
    -536,   393,   255,   490,   539,   613,   530,  -265,   499,   410,
     -88,   459,   416,   815,   807,   107,   107,   107,   107,   107,
     107,   107,   107,   711,   713,   107,   431,   107,   553,   622,
     107,   438,   439,   440,   441,   -90,   565,  -530,   408,  -461,
    -461,   460,  -273,  -530,   238,   468,  -470,  -470,   268,   269,
     663,  -398,   256,  -398,  -398,   917,   628,   734,   107,  -536,
     107,  -265,  -265,   403,   107,   107,   420,   689,   668,   299,
     -61,   216,   423,   421,   775,   686,   686,   927,   107,   421,
     417,   782,   554,   555,   461,   462,   897,   219,   425,   698,
     566,   567,   899,  -272,   588,   590,  -273,  -273,   419,   428,
     666,   433,    79,   246,    79,   107,   107,   201,   672,   566,
     567,  -536,   208,  -536,  -536,   469,   473,  -532,   733,   201,
     667,   107,   255,   539,   208,   530,    79,   208,   674,   668,
     590,   928,   929,   246,   476,   539,   622,   530,   438,   439,
     440,   441,   107,   477,   672,   672,   729,  -272,  -272,   558,
     550,  -533,   668,   559,   496,   552,   719,   414,   187,   189,
     670,   190,   208,   478,   712,   714,   628,   107,   628,   255,
     556,   584,   256,   442,   459,   600,   666,   672,   772,   -95,
     444,   -76,   -91,   607,   778,   780,   641,   614,    79,    79,
     621,   644,   238,   431,   945,   491,   633,   636,   657,   706,
     445,   671,   800,    79,   437,   802,   438,   439,   440,   441,
     639,   -87,    79,   622,   -83,   438,   439,   440,   441,   256,
     810,   642,   208,   -93,   411,   801,   649,   668,   208,   208,
    -251,   658,   238,   661,   726,   609,   599,   809,   668,   676,
     601,   442,   678,   602,   682,   727,   735,   443,   444,   460,
     777,  -252,   628,   877,   208,   -85,   611,   208,    79,   612,
     499,   771,   776,   787,   299,   819,    79,   823,   445,   824,
     825,   446,   -90,    79,    79,   835,   107,   826,   107,   312,
     313,   640,   187,   643,   828,   830,   107,   832,   299,   836,
     238,   848,   461,   462,   463,   447,   857,   838,   107,   720,
     107,   107,   876,   839,   -82,   841,   460,   628,   843,   628,
     317,   318,   319,   320,   321,   322,   323,   324,   845,   748,
    -253,   460,   751,   850,   854,   872,   329,   874,   686,   329,
     866,   731,   871,   761,   659,   208,   107,   320,   321,   322,
     323,   324,   885,   628,   255,    79,   460,  -254,   208,   461,
     462,   465,    79,    79,   908,   910,    79,   916,   255,   912,
     918,   926,   107,   107,   461,   462,   467,   920,   768,   338,
    -532,   740,    79,   438,   439,   440,   441,   107,   923,   203,
     203,  -533,   954,   203,   353,   299,   107,   949,   783,   461,
     462,   470,   732,   371,   884,   948,   107,   372,   365,   878,
     312,   313,   107,   107,   886,   606,   907,   745,   894,   232,
     235,   283,    79,     0,   203,   203,     0,     0,     0,     0,
     329,     0,     0,    79,     0,   277,   278,     0,   107,     0,
       0,   107,   107,   319,   320,   321,   322,   323,   324,     0,
     107,   753,   754,   299,   755,     0,     0,   107,   107,     0,
      44,    45,     0,     0,     0,    79,     0,    79,   312,   313,
       0,     0,     0,    79,     0,    79,     0,   437,   736,   438,
     439,   440,   441,     0,   245,   249,     0,   846,   690,   668,
       0,   201,     0,     0,     0,     0,   208,     0,     0,   530,
       0,     0,   320,   321,   322,   323,   324,   539,   622,   530,
     438,   439,   440,   441,   442,     0,   879,   862,   881,   107,
     443,   444,   882,   622,     0,   438,   439,   440,   441,   107,
       0,   888,   107,   891,     0,     0,   107,   107,     0,     0,
     107,   445,     0,     0,   446,   623,     0,     0,     0,   681,
       0,   683,     0,     0,     0,     0,   107,   806,     0,     0,
     623,   284,   285,   286,   287,   288,   624,     0,   203,     0,
       0,   203,   203,   277,     0,     0,     0,   816,     0,     0,
       0,   827,   829,   817,   831,     0,   833,   834,   822,   203,
       0,   203,   203,     0,     0,     0,   107,   840,     0,   842,
     844,     0,   936,     0,     0,     0,     0,   107,   939,     0,
     941,     0,     0,   942,   622,     0,   438,   439,   440,   441,
       0,     0,     0,     0,     0,   796,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   738,   739,     0,     0,   107,
     387,   107,   805,     0,   959,   387,     0,   107,     0,   107,
     752,   623,   401,     0,     0,     0,     0,   820,   868,   766,
       0,     0,     0,     0,     0,     0,   203,     0,     0,     0,
     107,   498,   501,   502,   503,   504,   505,   506,   507,   508,
     509,   510,   511,   512,   513,   514,   515,   516,   517,   518,
     519,   520,   521,   522,   523,   524,   525,   526,     0,   203,
       0,     0,     0,     0,     0,     0,     0,   758,     0,   438,
     439,   440,   441,     0,     0,   911,   913,   914,   915,     0,
     811,   812,   919,   921,   922,     0,   924,   925,   858,     0,
     859,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     867,     0,     0,     0,   442,   869,   581,   583,     0,     0,
     443,   444,     0,     0,     0,     0,   587,   203,   203,   401,
       0,     0,   203,     0,   581,   583,   203,     0,     0,     0,
       0,   445,   952,     0,   446,   953,   955,   956,   957,     0,
       0,     0,   605,     0,     0,     0,     0,   610,     0,     0,
       0,     0,   853,   203,     0,   960,   203,   900,   901,   860,
     861,   633,     0,   863,     0,     0,     0,     0,   203,   437,
       0,   438,   439,   440,   441,     0,     0,     0,     0,   873,
       0,     0,   401,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   933,     0,   103,   442,   103,     0,   650,
     651,     0,   443,   444,     0,     0,     0,     0,     0,   898,
       0,   203,     0,     0,     0,     0,     0,     0,   401,   950,
       0,   951,     0,   445,     0,     0,   446,     0,     0,     0,
       0,     0,   635,     0,     0,     0,   638,     0,   638,     0,
     638,     0,   103,     0,     0,     0,   257,     0,     0,     0,
     707,     0,   930,   758,   931,   438,   439,   440,   441,     0,
     932,     0,     0,     0,     0,     0,     0,   257,     0,     0,
     656,     0,     0,   203,     0,     0,     0,   203,     0,   342,
     351,   351,   351,     0,   106,     0,   106,     0,     0,   203,
     442,     0,     0,     0,     0,     0,   443,   444,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   203,     0,     0,     0,     0,   445,     0,     0,
     446,     0,     0,   675,     0,   680,     0,     0,     0,     0,
       0,   106,    76,     0,    76,   258,     0,   401,     0,     0,
       0,     0,     0,     0,   203,     0,     0,     0,     0,   401,
       0,     0,     0,     0,     0,     0,   258,     0,     0,   675,
     675,   656,   656,     0,     0,     0,     0,     0,   343,   352,
     352,     0,     0,     0,     0,   387,     0,     0,     0,    76,
       0,   203,     0,     0,     0,   770,     0,   773,   774,     0,
     103,     0,   675,   779,   781,     0,     0,     0,     0,     0,
       0,   203,     0,     0,     0,     0,     0,   105,     0,   105,
       0,     0,   635,     0,     0,     0,   339,     0,     0,     0,
       0,     0,   103,     0,     0,     0,     0,     0,     0,     0,
     103,     0,     0,     0,     0,     0,   635,     0,     0,     0,
     813,     0,     0,     0,     0,   773,   257,   779,   781,     0,
       0,     0,     0,     0,   105,     0,     0,     0,     0,     0,
       0,   786,   789,     0,   789,     0,   789,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   103,     0,     0,   106,
       0,   103,     0,     0,     0,     0,     0,     0,   103,   257,
     203,     0,     0,     0,   847,     0,     0,     0,     0,     0,
     849,   656,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   106,     0,     0,     0,     0,     0,     0,     0,   106,
     203,   103,     0,     0,     0,     0,     0,    76,   638,   638,
       0,   638,     0,   638,   638,   258,     0,     0,   849,     0,
       0,     0,   635,     0,   638,     0,   638,   638,   299,   300,
     301,   302,   303,   304,   305,   306,     0,   308,   309,    76,
       0,     0,     0,   312,   313,   106,     0,    76,   852,     0,
     106,   856,     0,     0,     0,     0,     0,   106,   258,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   315,   316,   317,   318,   319,   320,   321,   322,
     323,   324,   105,   103,     0,     0,     0,     0,     0,     0,
     106,     0,     0,    76,     0,     0,     0,     0,    76,     0,
       0,     0,     0,     0,     0,    76,   257,     0,     0,     0,
       0,     0,     0,     0,   105,     0,     0,     0,     0,     0,
       0,     0,   105,     0,     0,     0,   789,     0,     0,     0,
       0,   203,     0,     0,     0,     0,     0,     0,    76,     0,
       0,     0,   638,   638,   638,   638,     0,     0,     0,   638,
     638,   638,     0,   638,   638,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   257,   105,     0,
       0,     0,   106,   105,     0,     0,     0,     0,     0,     0,
     105,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   401,     0,   680,   789,   258,     0,     0,     0,   638,
     662,     0,   638,   638,   638,   638,     0,     0,     0,     0,
       0,     0,     0,   105,     0,     0,     0,   103,     0,   103,
      76,     0,   638,     0,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,     0,     0,   312,
     313,   103,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   258,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   314,     0,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   257,     0,     0,
       0,     0,     0,     0,     0,   105,     0,  -230,     0,     0,
       0,     0,     0,   103,   103,     0,   106,     0,   106,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   103,     0,
       0,     0,     0,     0,     0,     0,     0,   103,     0,     0,
     106,     0,     0,     0,   257,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    76,     0,    76,  -554,     0,     0,
       0,     0,     0,     0,     0,  -554,  -554,  -554,     0,     0,
    -554,  -554,  -554,   103,  -554,     0,   258,     0,    76,     0,
       0,   103,     0,     0,  -554,     0,     0,     0,   103,   103,
       0,     0,   106,   106,  -554,  -554,     0,  -554,  -554,  -554,
    -554,  -554,     0,     0,     0,     0,     0,   106,   757,     0,
       0,     0,     0,     0,     0,     0,   106,     0,     0,     0,
       0,     0,     0,   258,     0,     0,     0,     0,     0,   105,
       0,   105,     0,     0,     0,     0,     0,     0,     0,     0,
      76,    76,  -554,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   105,     0,    76,     0,     0,     0,     0,
     103,     0,   106,     0,    76,     0,     0,   103,   103,     0,
     106,   103,     0,     0,     0,     0,     0,   106,   106,     0,
       0,     0,     0,     0,     0,     0,     0,   103,     0,     0,
       0,     0,     0,     0,     0,  -554,     0,  -554,     0,     0,
     219,  -554,     0,  -554,     0,  -554,     0,     0,     0,   351,
      76,     0,     0,     0,     0,   105,   105,     0,    76,     0,
       0,     0,     0,   895,     0,    76,    76,   103,     0,     0,
     105,     0,     0,     0,     0,     0,     0,     0,   103,   105,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   106,
       0,     0,     0,     0,     0,     0,   106,   106,     0,     0,
     106,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     103,     0,   103,     0,     0,     0,   106,     0,   103,     0,
     103,     0,     0,     0,     0,   105,     0,     0,     0,     0,
       0,     0,     0,   105,     0,     0,     0,    76,   352,     0,
     105,   105,     0,     0,    76,    76,     0,     0,    76,     0,
       0,     0,   896,     0,     0,     0,   106,     0,     0,     0,
       0,     0,     0,     0,    76,     0,     0,   106,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     299,   300,   301,   302,   303,   304,   305,   306,   307,   308,
     309,   310,   311,     0,     0,   312,   313,     0,     0,   106,
     893,   106,     0,     0,    76,     0,     0,   106,     0,   106,
       0,     0,   105,     0,     0,    76,     0,     0,     0,   105,
     105,     0,   314,   105,   315,   316,   317,   318,   319,   320,
     321,   322,   323,   324,     0,     0,     0,   717,     0,   105,
       0,     0,     0,     0,     0,     0,     0,    76,     0,    76,
       0,     0,     0,     0,     0,    76,     0,    76,     0,     0,
     226,   299,   300,   301,   302,   303,   304,   305,   306,   307,
     308,   309,   310,   311,     0,     0,   312,   313,     0,   105,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     105,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   314,     0,   315,   316,   317,   318,   319,
     320,   321,   322,   323,   324,     0,     0,     0,     0,     0,
       0,     0,   105,     0,   105,     0,     0,     0,     0,     0,
     105,     0,   105,  -554,     4,     0,     5,     6,     7,     8,
       9,     0,     0,     0,    10,    11,     0,     0,     0,    12,
       0,    13,    14,    15,    16,    17,    18,    19,     0,     0,
       0,     0,     0,    20,    21,    22,    23,    24,    25,    26,
       0,     0,    27,     0,     0,     0,     0,     0,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,     0,    41,    42,     0,    43,    44,    45,     0,    46,
      47,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    48,
       0,     0,    49,    50,     0,    51,    52,     0,    53,     0,
      54,    55,    56,    57,    58,    59,    60,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    61,    62,    63,  -272,     0,
       0,     0,     0,     0,     0,     0,  -272,  -272,  -272,     0,
       0,  -272,  -272,  -272,     0,  -272,     0,     0,     0,  -554,
       0,  -554,     0,     0,     0,  -272,  -272,  -272,     0,     0,
       0,     0,     0,     0,     0,  -272,  -272,     0,  -272,  -272,
    -272,  -272,  -272,     0,     0,     0,     0,     0,     0,   299,
     300,   301,   302,   303,   304,   305,   306,   307,   308,   309,
     310,   311,     0,     0,   312,   313,  -272,  -272,  -272,  -272,
    -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,     0,
       0,  -272,  -272,  -272,     0,   665,  -272,     0,     0,     0,
       0,   314,  -272,   315,   316,   317,   318,   319,   320,   321,
     322,   323,   324,     0,     0,  -272,     0,   -94,  -272,  -272,
    -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  -272,  -272,  -272,  -272,
    -397,     0,  -272,  -272,  -272,     0,  -272,     0,  -397,  -397,
    -397,     0,     0,  -397,  -397,  -397,     0,  -397,     0,     0,
       0,     0,     0,     0,     0,     0,  -397,  -397,  -397,     0,
       0,     0,     0,     0,     0,     0,     0,  -397,  -397,     0,
    -397,  -397,  -397,  -397,  -397,     0,     0,     0,     0,     0,
       0,   299,   300,   301,   302,   303,   304,   305,   306,   307,
     308,   309,  -555,  -555,     0,     0,   312,   313,  -397,  -397,
    -397,  -397,  -397,  -397,  -397,  -397,  -397,  -397,  -397,  -397,
    -397,     0,     0,  -397,  -397,  -397,     0,     0,  -397,     0,
       0,     0,     0,     0,  -397,   315,   316,   317,   318,   319,
     320,   321,   322,   323,   324,     0,     0,     0,     0,     0,
    -397,     0,  -397,  -397,  -397,  -397,  -397,  -397,  -397,  -397,
    -397,  -397,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  -397,  -397,  -397,
    -397,  -397,  -266,   219,  -397,  -397,  -397,     0,  -397,     0,
    -266,  -266,  -266,     0,     0,  -266,  -266,  -266,     0,  -266,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  -266,
    -266,  -266,     0,     0,     0,     0,     0,     0,     0,  -266,
    -266,     0,  -266,  -266,  -266,  -266,  -266,     0,     0,     0,
       0,     0,     0,   299,  -555,  -555,  -555,  -555,   304,   305,
       0,     0,  -555,  -555,     0,     0,     0,     0,   312,   313,
    -266,  -266,  -266,  -266,  -266,  -266,  -266,  -266,  -266,  -266,
    -266,  -266,  -266,     0,     0,  -266,  -266,  -266,     0,     0,
    -266,     0,     0,     0,     0,     0,  -266,   315,   316,   317,
     318,   319,   320,   321,   322,   323,   324,     0,     0,  -266,
       0,     0,  -266,  -266,  -266,  -266,  -266,  -266,  -266,  -266,
    -266,  -266,  -266,  -266,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    -266,  -266,  -266,  -266,  -554,     0,  -266,  -266,  -266,     0,
    -266,     0,  -554,  -554,  -554,     0,     0,  -554,  -554,  -554,
       0,  -554,     0,     0,     0,     0,     0,     0,     0,     0,
    -554,  -554,  -554,     0,     0,     0,     0,     0,     0,     0,
       0,  -554,  -554,     0,  -554,  -554,  -554,  -554,  -554,     0,
       0,     0,     0,     0,     0,   299,   300,   301,   302,   303,
     304,   305,     0,     0,   308,   309,     0,     0,     0,     0,
     312,   313,  -554,  -554,  -554,  -554,  -554,  -554,  -554,  -554,
    -554,  -554,  -554,  -554,  -554,     0,     0,  -554,  -554,  -554,
       0,     0,  -554,     0,     0,     0,     0,     0,  -554,   315,
     316,   317,   318,   319,   320,   321,   322,   323,   324,     0,
       0,     0,     0,     0,  -554,     0,  -554,  -554,  -554,  -554,
    -554,  -554,  -554,  -554,  -554,  -554,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  -554,  -554,  -554,  -554,  -554,  -279,   219,  -554,  -554,
    -554,     0,  -554,     0,  -279,  -279,  -279,     0,     0,  -279,
    -279,  -279,     0,  -279,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  -279,  -279,     0,     0,     0,     0,     0,
       0,     0,     0,  -279,  -279,     0,  -279,  -279,  -279,  -279,
    -279,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  -279,  -279,  -279,  -279,  -279,  -279,
    -279,  -279,  -279,  -279,  -279,  -279,  -279,     0,     0,  -279,
    -279,  -279,     0,     0,  -279,     0,     0,     0,     0,     0,
    -279,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  -279,     0,  -279,  -279,
    -279,  -279,  -279,  -279,  -279,  -279,  -279,  -279,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  -279,  -279,  -279,  -279,  -536,   216,
    -279,  -279,  -279,     0,  -279,     0,  -536,  -536,  -536,     0,
       0,     0,  -536,  -536,     0,  -536,     0,     0,     0,     0,
       0,     0,     0,     0,  -536,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  -536,  -536,     0,  -536,  -536,
    -536,  -536,  -536,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  -536,  -536,  -536,  -536,
    -536,  -536,  -536,  -536,  -536,  -536,  -536,  -536,  -536,     0,
       0,  -536,  -536,  -536,  -272,   664,     0,     0,     0,     0,
       0,     0,  -272,  -272,  -272,     0,     0,     0,  -272,  -272,
       0,  -272,     0,     0,     0,     0,     0,   -92,  -536,     0,
    -536,  -536,  -536,  -536,  -536,  -536,  -536,  -536,  -536,  -536,
       0,  -272,  -272,     0,  -272,  -272,  -272,  -272,  -272,     0,
       0,     0,     0,     0,     0,  -536,  -536,  -536,  -536,   -84,
       0,     0,  -536,     0,  -536,     0,  -536,     0,     0,     0,
       0,     0,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,
    -272,  -272,  -272,  -272,  -272,     0,     0,  -272,  -272,  -272,
       0,   665,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   -94,  -272,     0,  -272,  -272,  -272,  -272,
    -272,  -272,  -272,  -272,  -272,  -272,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  -272,  -272,  -272,   -86,     0,     0,  -272,     0,
    -272,     4,  -272,     5,     6,     7,     8,     9,  -554,  -554,
    -554,    10,    11,     0,     0,  -554,    12,     0,    13,    14,
      15,    16,    17,    18,    19,     0,     0,     0,     0,     0,
      20,    21,    22,    23,    24,    25,    26,     0,     0,    27,
       0,     0,     0,     0,     0,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,     0,    41,
      42,     0,    43,    44,    45,     0,    46,    47,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    48,     0,     0,    49,
      50,     0,    51,    52,     0,    53,     0,    54,    55,    56,
      57,    58,    59,    60,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    61,    62,    63,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  -554,     4,  -554,     5,
       6,     7,     8,     9,     0,     0,  -554,    10,    11,     0,
    -554,  -554,    12,     0,    13,    14,    15,    16,    17,    18,
      19,     0,     0,     0,     0,     0,    20,    21,    22,    23,
      24,    25,    26,     0,     0,    27,     0,     0,     0,     0,
       0,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,     0,    41,    42,     0,    43,    44,
      45,     0,    46,    47,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    48,     0,     0,    49,    50,     0,    51,    52,
       0,    53,     0,    54,    55,    56,    57,    58,    59,    60,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    61,    62,
      63,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  -554,     4,  -554,     5,     6,     7,     8,     9,
       0,     0,  -554,    10,    11,     0,     0,  -554,    12,  -554,
      13,    14,    15,    16,    17,    18,    19,     0,     0,     0,
       0,     0,    20,    21,    22,    23,    24,    25,    26,     0,
       0,    27,     0,     0,     0,     0,     0,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
       0,    41,    42,     0,    43,    44,    45,     0,    46,    47,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    48,     0,
       0,    49,    50,     0,    51,    52,     0,    53,     0,    54,
      55,    56,    57,    58,    59,    60,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    61,    62,    63,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  -554,     4,
    -554,     5,     6,     7,     8,     9,     0,     0,  -554,    10,
      11,     0,     0,  -554,    12,     0,    13,    14,    15,    16,
      17,    18,    19,     0,     0,     0,     0,     0,    20,    21,
      22,    23,    24,    25,    26,     0,     0,    27,     0,     0,
       0,     0,     0,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,     0,    41,    42,     0,
      43,    44,    45,     0,    46,    47,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    48,     0,     0,    49,    50,     0,
      51,    52,     0,    53,     0,    54,    55,    56,    57,    58,
      59,    60,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      61,    62,    63,     0,     0,     0,     0,     0,     0,     0,
       4,     0,     5,     6,     7,     8,     9,     0,  -554,  -554,
      10,    11,     0,     0,  -554,    12,  -554,    13,    14,    15,
      16,    17,    18,    19,     0,     0,     0,     0,     0,    20,
      21,    22,    23,    24,    25,    26,     0,     0,    27,     0,
       0,     0,     0,     0,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,     0,    41,    42,
       0,    43,    44,    45,     0,    46,    47,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    48,     0,     0,    49,    50,
       0,    51,    52,     0,    53,     0,    54,    55,    56,    57,
      58,    59,    60,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    61,    62,    63,     0,     0,     0,     0,     0,     0,
       0,     4,     0,     5,     6,     7,     8,     9,     0,     0,
       0,    10,    11,     0,     0,  -554,    12,  -554,    13,    14,
      15,    16,    17,    18,    19,     0,     0,     0,     0,     0,
      20,    21,    22,    23,    24,    25,    26,     0,     0,    27,
       0,     0,     0,     0,     0,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,     0,    41,
      42,     0,    43,    44,    45,     0,    46,    47,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    48,     0,     0,   237,
      50,     0,    51,    52,     0,    53,     0,    54,    55,    56,
      57,    58,    59,    60,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    61,    62,    63,     0,     0,     0,     0,     0,
       0,     0,     4,     0,     5,     6,     7,     8,     9,     0,
       0,     0,    10,    11,  -554,     0,  -554,    12,  -554,    13,
      14,    15,    16,    17,    18,    19,     0,     0,     0,     0,
       0,    20,    21,    22,    23,    24,    25,    26,     0,     0,
      27,     0,     0,     0,     0,     0,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,     0,
      41,    42,     0,    43,    44,    45,     0,    46,    47,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    48,     0,     0,
      49,    50,     0,    51,    52,     0,    53,     0,    54,    55,
      56,    57,    58,    59,    60,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    61,    62,    63,     0,     0,     0,     0,
       0,     0,     0,     4,     0,     5,     6,     7,     8,     9,
       0,     0,     0,    10,    11,  -554,     0,  -554,    12,  -554,
      13,    14,    15,    16,    17,    18,    19,     0,     0,     0,
       0,     0,    20,    21,    22,    23,    24,    25,    26,     0,
       0,    27,     0,     0,     0,     0,     0,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
       0,    41,    42,     0,    43,    44,    45,     0,    46,    47,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    48,     0,
       0,    49,    50,     0,    51,    52,     0,    53,     0,    54,
      55,    56,    57,    58,    59,    60,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    61,    62,    63,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    -554,     0,     0,     0,     0,     0,     0,     0,  -554,     4,
    -554,     5,     6,     7,     8,     9,     0,     0,  -554,    10,
      11,     0,     0,     0,    12,     0,    13,    14,    15,    16,
      17,    18,    19,     0,     0,     0,     0,     0,    20,    21,
      22,    23,    24,    25,    26,     0,     0,    27,     0,     0,
       0,     0,     0,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,     0,    41,    42,     0,
      43,    44,    45,     0,    46,    47,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    48,     0,     0,    49,    50,     0,
      51,    52,     0,    53,     0,    54,    55,    56,    57,    58,
      59,    60,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      61,    62,    63,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     5,     6,     7,     0,     9,     0,     0,     0,
      10,    11,     0,     0,  -554,    12,  -554,    13,    14,    15,
     191,   192,    18,    19,     0,     0,     0,     0,     0,   193,
      21,    22,    23,    24,    25,    26,     0,     0,   194,     0,
       0,     0,     0,     0,     0,    29,     0,     0,    32,    33,
      34,    35,    36,    37,    38,    39,    40,   195,    41,    42,
       0,    43,    44,    45,     0,    46,    47,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   196,     0,     0,   197,    50,
       0,    51,    52,     0,   198,   199,    54,    55,    56,    57,
      58,    59,    60,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    61,   200,    63,     5,     6,     7,     0,     9,     0,
       0,     0,    10,    11,     0,     0,     0,    12,     0,    13,
      14,    15,    16,    17,    18,    19,     0,   226,     0,     0,
       0,    20,    21,    22,    23,    24,    25,    26,     0,     0,
      27,     0,     0,     0,     0,     0,     0,    29,     0,     0,
      32,    33,    34,    35,    36,    37,    38,    39,    40,     0,
      41,    42,     0,    43,    44,    45,     0,    46,    47,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   196,     0,     0,
     197,    50,     0,    51,    52,     0,     0,     0,    54,    55,
      56,    57,    58,    59,    60,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    61,    62,    63,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     5,     6,     7,     0,     9,
       0,     0,     0,    10,    11,     0,     0,   279,    12,   280,
      13,    14,    15,    16,    17,    18,    19,     0,     0,     0,
       0,     0,    20,    21,    22,    23,    24,    25,    26,     0,
       0,    27,     0,     0,     0,     0,     0,     0,    29,     0,
       0,    32,    33,    34,    35,    36,    37,    38,    39,    40,
       0,    41,    42,     0,    43,    44,    45,     0,    46,    47,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   196,     0,
       0,   197,    50,     0,    51,    52,     0,     0,     0,    54,
      55,    56,    57,    58,    59,    60,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    61,    62,    63,     5,     6,     7,
       8,     9,     0,     0,     0,    10,    11,     0,     0,     0,
      12,     0,    13,    14,    15,    16,    17,    18,    19,     0,
     226,     0,     0,     0,    20,    21,    22,    23,    24,    25,
      26,     0,     0,    27,     0,     0,     0,     0,     0,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,     0,    41,    42,     0,    43,    44,    45,     0,
      46,    47,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      48,     0,     0,    49,    50,     0,    51,    52,     0,    53,
       0,    54,    55,    56,    57,    58,    59,    60,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    61,    62,    63,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     478,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,     0,     0,     0,   134,   135,
     136,   354,   355,   356,   357,   141,   142,   143,     0,     0,
       0,     0,     0,   144,   145,   146,   147,   358,   359,   360,
     361,   152,    37,    38,   362,    40,     0,     0,     0,     0,
       0,     0,     0,     0,   154,   155,   156,   157,   158,   159,
     160,   161,   162,     0,     0,   163,   164,     0,     0,   165,
     166,   167,   168,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   169,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
       0,   180,   181,     0,     0,     0,     0,     0,     0,  -529,
    -529,  -529,     0,  -529,     0,     0,     0,  -529,  -529,     0,
     182,   363,  -529,     0,  -529,  -529,  -529,  -529,  -529,  -529,
    -529,     0,  -529,     0,     0,     0,  -529,  -529,  -529,  -529,
    -529,  -529,  -529,     0,     0,  -529,     0,     0,     0,     0,
       0,     0,  -529,     0,     0,  -529,  -529,  -529,  -529,  -529,
    -529,  -529,  -529,  -529,  -529,  -529,  -529,     0,  -529,  -529,
    -529,     0,  -529,  -529,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  -529,     0,     0,  -529,  -529,     0,  -529,  -529,
       0,  -529,  -529,  -529,  -529,  -529,  -529,  -529,  -529,  -529,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  -529,  -529,
    -529,     0,     0,     0,     0,     0,     0,  -530,  -530,  -530,
       0,  -530,     0,  -529,     0,  -530,  -530,     0,     0,  -529,
    -530,     0,  -530,  -530,  -530,  -530,  -530,  -530,  -530,     0,
    -530,     0,     0,     0,  -530,  -530,  -530,  -530,  -530,  -530,
    -530,     0,     0,  -530,     0,     0,     0,     0,     0,     0,
    -530,     0,     0,  -530,  -530,  -530,  -530,  -530,  -530,  -530,
    -530,  -530,  -530,  -530,  -530,     0,  -530,  -530,  -530,     0,
    -530,  -530,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    -530,     0,     0,  -530,  -530,     0,  -530,  -530,     0,  -530,
    -530,  -530,  -530,  -530,  -530,  -530,  -530,  -530,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  -530,  -530,  -530,     0,
       0,     0,     0,     0,     0,  -532,  -532,  -532,     0,  -532,
       0,  -530,     0,  -532,  -532,     0,     0,  -530,  -532,     0,
    -532,  -532,  -532,  -532,  -532,  -532,  -532,     0,     0,     0,
       0,     0,  -532,  -532,  -532,  -532,  -532,  -532,  -532,     0,
       0,  -532,     0,     0,     0,     0,     0,     0,  -532,     0,
       0,  -532,  -532,  -532,  -532,  -532,  -532,  -532,  -532,  -532,
    -532,  -532,  -532,     0,  -532,  -532,  -532,     0,  -532,  -532,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  -532,   722,
       0,  -532,  -532,     0,  -532,  -532,     0,  -532,  -532,  -532,
    -532,  -532,  -532,  -532,  -532,  -532,     0,     0,     0,     0,
       0,   -92,     0,     0,     0,     0,     0,     0,     0,  -534,
    -534,  -534,     0,  -534,  -532,  -532,  -532,  -534,  -534,     0,
       0,     0,  -534,     0,  -534,  -534,  -534,  -534,  -534,  -534,
    -534,     0,     0,     0,     0,  -532,  -534,  -534,  -534,  -534,
    -534,  -534,  -534,     0,     0,  -534,     0,     0,     0,     0,
       0,     0,  -534,     0,     0,  -534,  -534,  -534,  -534,  -534,
    -534,  -534,  -534,  -534,  -534,  -534,  -534,     0,  -534,  -534,
    -534,     0,  -534,  -534,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  -534,     0,     0,  -534,  -534,     0,  -534,  -534,
       0,  -534,  -534,  -534,  -534,  -534,  -534,  -534,  -534,  -534,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  -535,  -535,  -535,     0,  -535,  -534,  -534,
    -534,  -535,  -535,     0,     0,     0,  -535,     0,  -535,  -535,
    -535,  -535,  -535,  -535,  -535,     0,     0,     0,     0,  -534,
    -535,  -535,  -535,  -535,  -535,  -535,  -535,     0,     0,  -535,
       0,     0,     0,     0,     0,     0,  -535,     0,     0,  -535,
    -535,  -535,  -535,  -535,  -535,  -535,  -535,  -535,  -535,  -535,
    -535,     0,  -535,  -535,  -535,     0,  -535,  -535,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  -535,     0,     0,  -535,
    -535,     0,  -535,  -535,     0,  -535,  -535,  -535,  -535,  -535,
    -535,  -535,  -535,  -535,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     5,     6,     7,
       0,     9,  -535,  -535,  -535,    10,    11,     0,     0,     0,
      12,     0,    13,    14,    15,    16,    17,    18,    19,     0,
       0,     0,     0,  -535,    20,    21,    22,    23,    24,    25,
      26,     0,     0,    27,     0,     0,     0,     0,     0,     0,
      29,     0,     0,    32,    33,    34,    35,    36,    37,    38,
      39,    40,     0,    41,    42,     0,    43,    44,    45,     0,
      46,    47,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     196,     0,     0,   197,    50,     0,    51,    52,     0,     0,
       0,    54,    55,    56,    57,    58,    59,    60,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    61,    62,    63,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   222,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,     0,     0,     0,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,     0,     0,     0,     0,     0,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   261,
     262,   153,   263,     0,     0,     0,     0,     0,     0,     0,
       0,   154,   155,   156,   157,   158,   159,   160,   161,   162,
       0,     0,   163,   164,     0,     0,   165,   166,   167,   168,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     169,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,     0,   180,   181,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   182,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,     0,     0,     0,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,     0,     0,     0,     0,     0,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   224,
       0,   153,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   154,   155,   156,   157,   158,   159,   160,   161,   162,
       0,     0,   163,   164,     0,     0,   165,   166,   167,   168,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     169,     0,     0,    55,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,     0,   180,   181,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   182,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,     0,     0,     0,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,     0,     0,     0,     0,     0,
     144,   145,   146,   147,   148,   149,   150,   151,   152,     0,
       0,   153,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   154,   155,   156,   157,   158,   159,   160,   161,   162,
       0,     0,   163,   164,     0,     0,   165,   166,   167,   168,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     169,     0,     0,    55,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,     0,   180,   181,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   182,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,     0,     0,     0,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,     0,     0,     0,     0,     0,
     144,   145,   146,   147,   148,   149,   150,   151,   152,     0,
       0,   153,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   154,   155,   156,   157,   158,   159,   160,   161,   162,
       0,     0,   163,   164,     0,     0,   165,   166,   167,   168,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     169,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,     0,   180,   181,
       0,     0,     0,     5,     6,     7,     0,     9,     0,     0,
       0,    10,    11,     0,     0,     0,    12,   182,    13,    14,
      15,   191,   192,    18,    19,     0,     0,     0,     0,     0,
     193,   230,   231,    23,    24,    25,    26,     0,     0,   194,
       0,     0,     0,     0,     0,     0,   250,     0,     0,    32,
      33,    34,    35,    36,    37,    38,    39,    40,     0,    41,
      42,     0,    43,    44,    45,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   251,     0,     0,   197,
      50,     0,    51,    52,     0,     0,     0,    54,    55,    56,
      57,    58,    59,    60,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     5,     6,     7,     0,     9,
       0,     0,   252,    10,    11,     0,     0,     0,    12,     0,
      13,    14,    15,   191,   192,    18,    19,     0,     0,     0,
       0,   253,   193,   230,   231,    23,    24,    25,    26,     0,
       0,   194,     0,     0,     0,     0,     0,     0,   250,     0,
       0,    32,    33,    34,    35,    36,    37,    38,    39,    40,
       0,    41,    42,     0,    43,    44,    45,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   251,     0,
       0,   197,    50,     0,    51,    52,     0,     0,     0,    54,
      55,    56,    57,    58,    59,    60,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     5,     6,     7,
       8,     9,     0,     0,   252,    10,    11,     0,     0,     0,
      12,     0,    13,    14,    15,    16,    17,    18,    19,     0,
       0,     0,     0,   494,    20,    21,    22,    23,    24,    25,
      26,     0,     0,    27,     0,     0,     0,     0,     0,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,     0,    41,    42,     0,    43,    44,    45,     0,
      46,    47,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      48,     0,     0,    49,    50,     0,    51,    52,     0,    53,
       0,    54,    55,    56,    57,    58,    59,    60,     0,     0,
       0,     0,     0,     0,     0,     0,     5,     6,     7,     0,
       9,     0,     0,     0,    10,    11,    61,    62,    63,    12,
       0,    13,    14,    15,   191,   192,    18,    19,     0,     0,
       0,     0,     0,   193,    21,    22,    23,    24,    25,    26,
       0,     0,   194,     0,     0,     0,     0,     0,     0,    29,
       0,     0,    32,    33,    34,    35,    36,    37,    38,    39,
      40,   195,    41,    42,     0,    43,    44,    45,     0,    46,
      47,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   196,
       0,     0,   197,    50,     0,    51,    52,     0,   198,   199,
      54,    55,    56,    57,    58,    59,    60,     0,     0,     0,
       0,     0,     0,     0,     0,     5,     6,     7,     0,     9,
       0,     0,     0,    10,    11,    61,   200,    63,    12,     0,
      13,    14,    15,   191,   192,    18,    19,     0,     0,     0,
       0,     0,   193,   230,   231,    23,    24,    25,    26,     0,
       0,   194,     0,     0,     0,     0,     0,     0,    29,     0,
       0,    32,    33,    34,    35,    36,    37,    38,    39,    40,
     195,    41,    42,     0,    43,    44,    45,     0,    46,    47,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   196,     0,
       0,   197,    50,     0,    51,    52,     0,   589,   199,    54,
      55,    56,    57,    58,    59,    60,     0,     0,     0,     0,
       0,     0,     0,     0,     5,     6,     7,     0,     9,     0,
       0,     0,    10,    11,    61,   200,    63,    12,     0,    13,
      14,    15,   191,   192,    18,    19,     0,     0,     0,     0,
       0,   193,   230,   231,    23,    24,    25,    26,     0,     0,
     194,     0,     0,     0,     0,     0,     0,    29,     0,     0,
      32,    33,    34,    35,    36,    37,    38,    39,    40,   195,
      41,    42,     0,    43,    44,    45,     0,    46,    47,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   196,     0,     0,
     197,    50,     0,    51,    52,     0,   198,     0,    54,    55,
      56,    57,    58,    59,    60,     0,     0,     0,     0,     0,
       0,     0,     0,     5,     6,     7,     0,     9,     0,     0,
       0,    10,    11,    61,   200,    63,    12,     0,    13,    14,
      15,   191,   192,    18,    19,     0,     0,     0,     0,     0,
     193,   230,   231,    23,    24,    25,    26,     0,     0,   194,
       0,     0,     0,     0,     0,     0,    29,     0,     0,    32,
      33,    34,    35,    36,    37,    38,    39,    40,   195,    41,
      42,     0,    43,    44,    45,     0,    46,    47,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   196,     0,     0,   197,
      50,     0,    51,    52,     0,     0,   199,    54,    55,    56,
      57,    58,    59,    60,     0,     0,     0,     0,     0,     0,
       0,     0,     5,     6,     7,     0,     9,     0,     0,     0,
      10,    11,    61,   200,    63,    12,     0,    13,    14,    15,
     191,   192,    18,    19,     0,     0,     0,     0,     0,   193,
     230,   231,    23,    24,    25,    26,     0,     0,   194,     0,
       0,     0,     0,     0,     0,    29,     0,     0,    32,    33,
      34,    35,    36,    37,    38,    39,    40,   195,    41,    42,
       0,    43,    44,    45,     0,    46,    47,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   196,     0,     0,   197,    50,
       0,    51,    52,     0,   589,     0,    54,    55,    56,    57,
      58,    59,    60,     0,     0,     0,     0,     0,     0,     0,
       0,     5,     6,     7,     0,     9,     0,     0,     0,    10,
      11,    61,   200,    63,    12,     0,    13,    14,    15,   191,
     192,    18,    19,     0,     0,     0,     0,     0,   193,   230,
     231,    23,    24,    25,    26,     0,     0,   194,     0,     0,
       0,     0,     0,     0,    29,     0,     0,    32,    33,    34,
      35,    36,    37,    38,    39,    40,   195,    41,    42,     0,
      43,    44,    45,     0,    46,    47,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   196,     0,     0,   197,    50,     0,
      51,    52,     0,     0,     0,    54,    55,    56,    57,    58,
      59,    60,     0,     0,     0,     0,     0,     0,     0,     0,
       5,     6,     7,     0,     9,     0,     0,     0,    10,    11,
      61,   200,    63,    12,     0,    13,    14,    15,    16,    17,
      18,    19,     0,     0,     0,     0,     0,    20,    21,    22,
      23,    24,    25,    26,     0,     0,   194,     0,     0,     0,
       0,     0,     0,    29,     0,     0,    32,    33,    34,    35,
      36,    37,    38,    39,    40,     0,    41,    42,     0,    43,
      44,    45,     0,    46,    47,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   196,     0,     0,   197,    50,     0,    51,
      52,     0,   488,     0,    54,    55,    56,    57,    58,    59,
      60,     0,     0,     0,     0,     0,     0,     0,     0,     5,
       6,     7,     0,     9,     0,     0,     0,    10,    11,    61,
     200,    63,    12,     0,    13,    14,    15,   191,   192,    18,
      19,     0,     0,     0,     0,     0,   193,   230,   231,    23,
      24,    25,    26,     0,     0,   194,     0,     0,     0,     0,
       0,     0,    29,     0,     0,    32,    33,    34,    35,    36,
      37,    38,    39,    40,     0,    41,    42,     0,    43,    44,
      45,     0,    46,    47,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   196,     0,     0,   197,    50,     0,    51,    52,
       0,   198,     0,    54,    55,    56,    57,    58,    59,    60,
       0,     0,     0,     0,     0,     0,     0,     0,     5,     6,
       7,     0,     9,     0,     0,     0,    10,    11,    61,   200,
      63,    12,     0,    13,    14,    15,   191,   192,    18,    19,
       0,     0,     0,     0,     0,   193,   230,   231,    23,    24,
      25,    26,     0,     0,   194,     0,     0,     0,     0,     0,
       0,    29,     0,     0,    32,    33,    34,    35,    36,    37,
      38,    39,    40,     0,    41,    42,     0,    43,    44,    45,
       0,    46,    47,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   196,     0,     0,   197,    50,     0,    51,    52,     0,
     767,     0,    54,    55,    56,    57,    58,    59,    60,     0,
       0,     0,     0,     0,     0,     0,     0,     5,     6,     7,
       0,     9,     0,     0,     0,    10,    11,    61,   200,    63,
      12,     0,    13,    14,    15,   191,   192,    18,    19,     0,
       0,     0,     0,     0,   193,   230,   231,    23,    24,    25,
      26,     0,     0,   194,     0,     0,     0,     0,     0,     0,
      29,     0,     0,    32,    33,    34,    35,    36,    37,    38,
      39,    40,     0,    41,    42,     0,    43,    44,    45,     0,
      46,    47,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     196,     0,     0,   197,    50,     0,    51,    52,     0,   488,
       0,    54,    55,    56,    57,    58,    59,    60,     0,     0,
       0,     0,     0,     0,     0,     0,     5,     6,     7,     0,
       9,     0,     0,     0,    10,    11,    61,   200,    63,    12,
       0,    13,    14,    15,   191,   192,    18,    19,     0,     0,
       0,     0,     0,   193,   230,   231,    23,    24,    25,    26,
       0,     0,   194,     0,     0,     0,     0,     0,     0,    29,
       0,     0,    32,    33,    34,    35,    36,    37,    38,    39,
      40,     0,    41,    42,     0,    43,    44,    45,     0,    46,
      47,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   196,
       0,     0,   197,    50,     0,    51,    52,     0,   589,     0,
      54,    55,    56,    57,    58,    59,    60,     0,     0,     0,
       0,     0,     0,     0,     0,     5,     6,     7,     0,     9,
       0,     0,     0,    10,    11,    61,   200,    63,    12,     0,
      13,    14,    15,   191,   192,    18,    19,     0,     0,     0,
       0,     0,   193,   230,   231,    23,    24,    25,    26,     0,
       0,   194,     0,     0,     0,     0,     0,     0,    29,     0,
       0,    32,    33,    34,    35,    36,    37,    38,    39,    40,
       0,    41,    42,     0,    43,    44,    45,     0,    46,    47,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   196,     0,
       0,   197,    50,     0,    51,    52,     0,     0,     0,    54,
      55,    56,    57,    58,    59,    60,     0,     0,     0,     0,
       0,     0,     0,     0,     5,     6,     7,     0,     9,     0,
       0,     0,    10,    11,    61,   200,    63,    12,     0,    13,
      14,    15,    16,    17,    18,    19,     0,     0,     0,     0,
       0,    20,    21,    22,    23,    24,    25,    26,     0,     0,
      27,     0,     0,     0,     0,     0,     0,    29,     0,     0,
      32,    33,    34,    35,    36,    37,    38,    39,    40,     0,
      41,    42,     0,    43,    44,    45,     0,    46,    47,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   196,     0,     0,
     197,    50,     0,    51,    52,     0,     0,     0,    54,    55,
      56,    57,    58,    59,    60,     0,     0,     0,     0,     0,
       0,     0,     0,     5,     6,     7,     0,     9,     0,     0,
       0,    10,    11,    61,    62,    63,    12,     0,    13,    14,
      15,    16,    17,    18,    19,     0,     0,     0,     0,     0,
      20,    21,    22,    23,    24,    25,    26,     0,     0,   194,
       0,     0,     0,     0,     0,     0,    29,     0,     0,    32,
      33,    34,    35,    36,    37,    38,    39,    40,     0,    41,
      42,     0,    43,    44,    45,     0,    46,    47,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   196,     0,     0,   197,
      50,     0,    51,    52,     0,     0,     0,    54,    55,    56,
      57,    58,    59,    60,     0,     0,     0,     0,     0,     0,
       0,     0,     5,     6,     7,     0,     9,     0,     0,     0,
      10,    11,    61,   200,    63,    12,     0,    13,    14,    15,
     191,   192,    18,    19,     0,     0,     0,     0,     0,   193,
     230,   231,    23,    24,    25,    26,     0,     0,   194,     0,
       0,     0,     0,     0,     0,   250,     0,     0,    32,    33,
      34,    35,    36,    37,    38,    39,    40,     0,    41,    42,
       0,    43,    44,    45,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   251,     0,     0,   295,    50,
       0,    51,    52,     0,   296,     0,    54,    55,    56,    57,
      58,    59,    60,     0,     0,     0,     0,     5,     6,     7,
       0,     9,     0,     0,     0,    10,    11,     0,     0,     0,
      12,   252,    13,    14,    15,   191,   192,    18,    19,     0,
       0,     0,     0,     0,   193,   230,   231,    23,    24,    25,
      26,     0,     0,   194,     0,     0,     0,     0,     0,     0,
     250,     0,     0,    32,    33,    34,    35,    36,    37,    38,
      39,    40,     0,    41,    42,     0,    43,    44,    45,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     337,     0,     0,    49,    50,     0,    51,    52,     0,    53,
       0,    54,    55,    56,    57,    58,    59,    60,     0,     0,
       0,     0,     5,     6,     7,     0,     9,     0,     0,     0,
      10,    11,     0,     0,     0,    12,   252,    13,    14,    15,
     191,   192,    18,    19,     0,     0,     0,     0,     0,   193,
     230,   231,    23,    24,    25,    26,     0,     0,   194,     0,
       0,     0,     0,     0,     0,   250,     0,     0,    32,    33,
      34,   344,    36,    37,    38,   345,    40,     0,    41,    42,
       0,    43,    44,    45,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   346,     0,     0,   347,     0,     0,   197,    50,
       0,    51,    52,     0,     0,     0,    54,    55,    56,    57,
      58,    59,    60,     0,     0,     0,     0,     5,     6,     7,
       0,     9,     0,     0,     0,    10,    11,     0,     0,     0,
      12,   252,    13,    14,    15,   191,   192,    18,    19,     0,
       0,     0,     0,     0,   193,   230,   231,    23,    24,    25,
      26,     0,     0,   194,     0,     0,     0,     0,     0,     0,
     250,     0,     0,    32,    33,    34,   344,    36,    37,    38,
     345,    40,     0,    41,    42,     0,    43,    44,    45,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     347,     0,     0,   197,    50,     0,    51,    52,     0,     0,
       0,    54,    55,    56,    57,    58,    59,    60,     0,     0,
       0,     0,     5,     6,     7,     0,     9,     0,     0,     0,
      10,    11,     0,     0,     0,    12,   252,    13,    14,    15,
     191,   192,    18,    19,     0,     0,     0,     0,     0,   193,
     230,   231,    23,    24,    25,    26,     0,     0,   194,     0,
       0,     0,     0,     0,     0,   250,     0,     0,    32,    33,
      34,    35,    36,    37,    38,    39,    40,     0,    41,    42,
       0,    43,    44,    45,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   251,     0,     0,   295,    50,
       0,    51,    52,     0,     0,     0,    54,    55,    56,    57,
      58,    59,    60,     0,     0,     0,     0,     5,     6,     7,
       0,     9,     0,     0,     0,    10,    11,     0,     0,     0,
      12,   252,    13,    14,    15,   191,   192,    18,    19,     0,
       0,     0,     0,     0,   193,   230,   231,    23,    24,    25,
      26,     0,     0,   194,     0,     0,     0,     0,     0,     0,
     250,     0,     0,    32,    33,    34,    35,    36,    37,    38,
      39,    40,     0,    41,    42,     0,    43,    44,    45,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     883,     0,     0,   197,    50,     0,    51,    52,     0,     0,
       0,    54,    55,    56,    57,    58,    59,    60,     0,     0,
       0,     0,     5,     6,     7,     0,     9,     0,     0,     0,
      10,    11,     0,     0,     0,    12,   252,    13,    14,    15,
     191,   192,    18,    19,     0,     0,     0,     0,     0,   193,
     230,   231,    23,    24,    25,    26,     0,     0,   194,     0,
       0,     0,     0,     0,     0,   250,     0,     0,    32,    33,
      34,    35,    36,    37,    38,    39,    40,     0,    41,    42,
       0,    43,    44,    45,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   892,     0,     0,   197,    50,
       0,    51,    52,     0,     0,     0,    54,    55,    56,    57,
      58,    59,    60,     0,     0,     0,     0,     0,   527,   528,
       0,     0,   529,     0,     0,     0,     0,     0,     0,     0,
       0,   252,   154,   155,   156,   157,   158,   159,   160,   161,
     162,     0,     0,   163,   164,     0,     0,   165,   166,   167,
     168,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   169,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,     0,   180,
     181,     0,     0,     0,     0,     0,   535,   536,     0,     0,
     537,     0,     0,     0,     0,     0,     0,     0,   182,   219,
     154,   155,   156,   157,   158,   159,   160,   161,   162,     0,
       0,   163,   164,     0,     0,   165,   166,   167,   168,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   169,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   170,   171,   172,   173,
     174,   175,   176,   177,   178,   179,     0,   180,   181,     0,
       0,     0,     0,     0,   593,   528,     0,     0,   594,     0,
       0,     0,     0,     0,     0,     0,   182,   219,   154,   155,
     156,   157,   158,   159,   160,   161,   162,     0,     0,   163,
     164,     0,     0,   165,   166,   167,   168,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   169,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   170,   171,   172,   173,   174,   175,
     176,   177,   178,   179,     0,   180,   181,     0,     0,     0,
       0,     0,   596,   536,     0,     0,   597,     0,     0,     0,
       0,     0,     0,     0,   182,   219,   154,   155,   156,   157,
     158,   159,   160,   161,   162,     0,     0,   163,   164,     0,
       0,   165,   166,   167,   168,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   169,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,     0,   180,   181,     0,     0,     0,     0,     0,
     616,   528,     0,     0,   617,     0,     0,     0,     0,     0,
       0,     0,   182,   219,   154,   155,   156,   157,   158,   159,
     160,   161,   162,     0,     0,   163,   164,     0,     0,   165,
     166,   167,   168,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   169,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
       0,   180,   181,     0,     0,     0,     0,     0,   619,   536,
       0,     0,   620,     0,     0,     0,     0,     0,     0,     0,
     182,   219,   154,   155,   156,   157,   158,   159,   160,   161,
     162,     0,     0,   163,   164,     0,     0,   165,   166,   167,
     168,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   169,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,     0,   180,
     181,     0,     0,     0,     0,     0,   691,   528,     0,     0,
     692,     0,     0,     0,     0,     0,     0,     0,   182,   219,
     154,   155,   156,   157,   158,   159,   160,   161,   162,     0,
       0,   163,   164,     0,     0,   165,   166,   167,   168,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   169,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   170,   171,   172,   173,
     174,   175,   176,   177,   178,   179,     0,   180,   181,     0,
       0,     0,     0,     0,   694,   536,     0,     0,   695,     0,
       0,     0,     0,     0,     0,     0,   182,   219,   154,   155,
     156,   157,   158,   159,   160,   161,   162,     0,     0,   163,
     164,     0,     0,   165,   166,   167,   168,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   169,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   170,   171,   172,   173,   174,   175,
     176,   177,   178,   179,     0,   180,   181,     0,     0,     0,
       0,     0,   701,   528,     0,     0,   702,     0,     0,     0,
       0,     0,     0,     0,   182,   219,   154,   155,   156,   157,
     158,   159,   160,   161,   162,     0,     0,   163,   164,     0,
       0,   165,   166,   167,   168,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   169,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,     0,   180,   181,     0,     0,     0,     0,     0,
     574,   536,     0,     0,   575,     0,     0,     0,     0,     0,
       0,     0,   182,   219,   154,   155,   156,   157,   158,   159,
     160,   161,   162,     0,     0,   163,   164,     0,     0,   165,
     166,   167,   168,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   169,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
       0,   180,   181,     0,     0,     0,     0,     0,   937,   528,
       0,     0,   938,     0,     0,     0,     0,     0,     0,     0,
     182,   219,   154,   155,   156,   157,   158,   159,   160,   161,
     162,     0,     0,   163,   164,     0,     0,   165,   166,   167,
     168,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   169,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,     0,   180,
     181,     0,     0,     0,     0,     0,   943,   528,     0,     0,
     944,     0,     0,     0,     0,     0,     0,     0,   182,   219,
     154,   155,   156,   157,   158,   159,   160,   161,   162,     0,
       0,   163,   164,     0,     0,   165,   166,   167,   168,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   169,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   170,   171,   172,   173,
     174,   175,   176,   177,   178,   179,     0,   180,   181,     0,
       0,     0,     0,     0,   946,   536,     0,     0,   947,     0,
       0,     0,     0,     0,     0,     0,   182,   219,   154,   155,
     156,   157,   158,   159,   160,   161,   162,     0,     0,   163,
     164,     0,     0,   165,   166,   167,   168,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   169,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   170,   171,   172,   173,   174,   175,
     176,   177,   178,   179,     0,   180,   181,     0,     0,     0,
       0,     0,   574,   536,     0,     0,   575,     0,     0,     0,
       0,     0,     0,     0,   182,   219,   154,   155,   156,   157,
     158,   159,   160,   161,   162,     0,     0,   163,   164,     0,
       0,   165,   166,   167,   168,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   169,     0,     0,     0,     0,
       0,   662,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,     0,   180,   181,   299,   300,   301,   302,   303,
     304,   305,   306,   307,   308,   309,   310,   311,     0,     0,
     312,   313,   182,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   314,     0,   315,
     316,   317,   318,   319,   320,   321,   322,   323,   324
};

static const short int yycheck[] =
{
       2,    53,     4,    29,   260,    55,   331,   260,    51,    16,
      17,   549,   212,    20,    16,    17,    62,   548,    20,   298,
       8,     8,    74,     8,   568,    27,     4,    85,    82,   595,
      22,    85,   326,    65,    86,    87,    88,    53,    28,    89,
      28,    28,   433,    28,   347,   244,    74,    49,    50,   248,
      22,    53,   618,    85,   633,   267,   639,   735,   636,   271,
      62,   639,   636,   733,    16,    17,    26,    55,    20,    25,
      49,   325,    74,   327,   629,   653,    51,    52,    76,    81,
      82,     1,    25,    85,    86,    87,    88,   679,    29,   578,
     579,   683,   841,     0,    27,   389,   237,    27,   653,    37,
      38,    89,    25,   494,    61,    85,    87,   393,    26,   385,
      68,   148,    85,   367,   368,   107,   114,   393,    85,   137,
      50,    25,    25,    85,    87,    85,   109,   693,   109,    25,
      15,    85,    17,    25,   388,   107,   390,   423,   704,    61,
      13,   435,   146,    56,   148,    28,   109,   104,   105,   819,
      85,    81,    82,    87,   295,    85,   700,   141,   141,   413,
     140,   119,   120,   121,   708,    13,   107,   140,    99,   918,
     147,    13,   139,   140,     2,   109,     4,   137,   140,   139,
     140,    25,   436,   143,   106,   141,   140,    13,    16,    17,
     147,   447,    20,   113,   447,   197,   874,   147,   141,    27,
     143,   253,    52,    13,   759,   140,   876,   141,    13,   216,
     148,   218,   219,   146,   216,   148,   218,   219,   141,   137,
     222,    49,    50,    25,   250,   253,   146,   229,   148,   718,
     284,   285,   286,   287,    62,   237,    26,   141,   141,   231,
     910,   109,   234,   826,   296,   141,   825,   293,   826,   141,
     293,   253,   298,    81,    82,   298,   830,    85,   237,   471,
     554,   839,   565,   841,   216,   839,   218,    87,   141,   800,
     661,   863,   566,   146,   109,   148,   334,    85,   332,   333,
     296,   283,   284,   285,   286,   287,   288,   289,   290,   109,
      87,   293,   222,   295,   296,   143,   298,   141,   146,   143,
     148,   555,   334,   141,   146,   283,   148,   109,    37,    38,
     288,    25,    85,   567,   570,   442,   295,   349,    85,   326,
     146,   865,   148,    85,   326,   863,   328,   373,   374,    85,
     332,   333,   140,    56,    85,   327,   146,   325,   148,   141,
     918,   146,   144,   148,   346,   195,   148,   137,   198,   199,
      59,    60,    85,   143,   284,   285,   286,   287,    85,   289,
     290,    59,    60,   907,   689,   143,    26,   140,    85,   197,
     109,   373,   374,   140,    85,    88,    54,   139,   140,   945,
      26,   433,   389,   139,   140,    63,    64,   389,   216,   140,
     218,   219,   592,    87,   222,   109,    87,   653,   390,   678,
     388,   229,   332,   333,    87,   433,   139,   140,   410,   237,
     385,   141,   139,   140,   613,   109,   346,   407,   109,   407,
     407,   413,   407,   140,   412,    85,   109,   141,   435,   140,
     144,   433,    85,   435,   148,    14,    15,     2,    87,    85,
      26,   141,   494,   293,   436,   420,   434,    85,   298,   137,
     141,   707,   144,    87,   707,   283,   284,   285,   286,   287,
     288,   289,   290,   576,   577,   293,   494,   295,    85,    52,
     298,    54,    55,    56,    57,   109,    85,   137,   143,   139,
     140,    61,    85,   143,    49,    65,   139,   140,    57,    58,
     516,   137,   494,   139,   140,    85,   623,   624,   326,    85,
     328,   139,   140,   222,   332,   333,   141,   550,   534,    68,
     109,   143,   141,   148,   667,   547,   548,    85,   346,   148,
     239,   674,   139,   140,   104,   105,   851,   143,   138,   561,
     139,   140,   857,    85,   384,   385,   139,   140,   145,    56,
     532,   141,   544,   393,   546,   373,   374,   554,   540,   139,
     140,   137,   554,   139,   140,   106,   106,   143,   141,   566,
     532,   389,   614,   555,   566,   553,   568,   569,   540,   595,
     420,   139,   140,   423,    68,   567,    52,   565,    54,    55,
      56,    57,   410,    68,   576,   577,   614,   139,   140,    52,
      17,   143,   618,    56,   141,    56,   586,    87,   586,   586,
      87,   586,   604,   146,   576,   577,   733,   435,   735,   661,
      25,    94,   614,    89,   870,   144,   608,   609,   664,   109,
      96,   141,   109,   138,   670,   671,    52,   141,   630,   631,
     109,    52,   197,   661,   928,   678,   146,   141,   488,   569,
     116,    87,   696,   645,    52,   699,    54,    55,    56,    57,
     141,   141,   654,    52,   141,    54,    55,    56,    57,   661,
     710,   141,   664,   109,   229,   697,   147,   693,   670,   671,
     141,   141,   237,   141,   604,   929,   395,   709,   704,    10,
     399,    89,     8,   402,    13,   138,   141,    95,    96,    61,
      87,   141,   819,   820,   696,   141,   415,   699,   700,   418,
     550,   111,   145,    15,    68,   141,   708,    10,   116,   138,
     141,   119,   109,   715,   716,   138,   544,   141,   546,    83,
      84,   455,   710,   457,   141,   141,   554,   141,    68,   109,
     295,   777,   104,   105,   106,   143,   790,   114,   566,   589,
     568,   569,   141,   141,   141,   141,    61,   874,   141,   876,
     114,   115,   116,   117,   118,   119,   120,   121,    10,   639,
     141,    61,   642,    88,     9,   138,   696,   141,   800,   699,
     802,   621,    10,   653,   493,   777,   604,   117,   118,   119,
     120,   121,   114,   910,   836,   787,    61,   141,   790,   104,
     105,   106,   794,   795,   138,   141,   798,    56,   850,   141,
     141,    56,   630,   631,   104,   105,   106,   141,   658,    86,
     143,    52,   814,    54,    55,    56,    57,   645,   141,    16,
      17,   143,   141,    20,    88,    68,   654,   931,   678,   104,
     105,   106,   623,    91,   836,   930,   664,    94,    89,   825,
      83,    84,   670,   671,   839,   410,   870,   636,   850,    46,
      47,    65,   854,    -1,    51,    52,    -1,    -1,    -1,    -1,
     790,    -1,    -1,   865,    -1,    62,    63,    -1,   696,    -1,
      -1,   699,   700,   116,   117,   118,   119,   120,   121,    -1,
     708,    54,    55,    68,    57,    -1,    -1,   715,   716,    -1,
      63,    64,    -1,    -1,    -1,   897,    -1,   899,    83,    84,
      -1,    -1,    -1,   905,    -1,   907,    -1,    52,   627,    54,
      55,    56,    57,    -1,    51,    52,    -1,   767,   551,   945,
      -1,   928,    -1,    -1,    -1,    -1,   928,    -1,    -1,   917,
      -1,    -1,   117,   118,   119,   120,   121,   929,    52,   927,
      54,    55,    56,    57,    89,    -1,   826,   797,   828,   777,
      95,    96,   832,    52,    -1,    54,    55,    56,    57,   787,
      -1,   841,   790,   843,    -1,    -1,   794,   795,    -1,    -1,
     798,   116,    -1,    -1,   119,    89,    -1,    -1,    -1,   544,
      -1,   546,    -1,    -1,    -1,    -1,   814,   706,    -1,    -1,
      89,    40,    41,    42,    43,    44,    95,    -1,   195,    -1,
      -1,   198,   199,   200,    -1,    -1,    -1,   726,    -1,    -1,
      -1,   745,   746,   732,   748,    -1,   750,   751,   737,   216,
      -1,   218,   219,    -1,    -1,    -1,   854,   761,    -1,   763,
     764,    -1,   912,    -1,    -1,    -1,    -1,   865,   918,    -1,
     920,    -1,    -1,   923,    52,    -1,    54,    55,    56,    57,
      -1,    -1,    -1,    -1,    -1,   688,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   630,   631,    -1,    -1,   897,
     207,   899,   705,    -1,   954,   212,    -1,   905,    -1,   907,
     645,    89,   219,    -1,    -1,    -1,    -1,    95,   807,   654,
      -1,    -1,    -1,    -1,    -1,    -1,   293,    -1,    -1,    -1,
     928,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,    -1,   326,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    52,    -1,    54,
      55,    56,    57,    -1,    -1,   879,   880,   881,   882,    -1,
     715,   716,   886,   887,   888,    -1,   890,   891,   791,    -1,
     793,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     803,    -1,    -1,    -1,    89,   808,   373,   374,    -1,    -1,
      95,    96,    -1,    -1,    -1,    -1,   383,   384,   385,   326,
      -1,    -1,   389,    -1,   391,   392,   393,    -1,    -1,    -1,
      -1,   116,   936,    -1,   119,   939,   940,   941,   942,    -1,
      -1,    -1,   409,    -1,    -1,    -1,    -1,   414,    -1,    -1,
      -1,    -1,   787,   420,    -1,   959,   423,   860,   861,   794,
     795,   146,    -1,   798,    -1,    -1,    -1,    -1,   435,    52,
      -1,    54,    55,    56,    57,    -1,    -1,    -1,    -1,   814,
      -1,    -1,   389,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   906,    -1,     2,    89,     4,    -1,   476,
     477,    -1,    95,    96,    -1,    -1,    -1,    -1,    -1,   854,
      -1,   488,    -1,    -1,    -1,    -1,    -1,    -1,   435,   932,
      -1,   934,    -1,   116,    -1,    -1,   119,    -1,    -1,    -1,
      -1,    -1,   449,    -1,    -1,    -1,   453,    -1,   455,    -1,
     457,    -1,    49,    -1,    -1,    -1,    53,    -1,    -1,    -1,
     143,    -1,   897,    52,   899,    54,    55,    56,    57,    -1,
     905,    -1,    -1,    -1,    -1,    -1,    -1,    74,    -1,    -1,
     487,    -1,    -1,   550,    -1,    -1,    -1,   554,    -1,    86,
      87,    88,    89,    -1,     2,    -1,     4,    -1,    -1,   566,
      89,    -1,    -1,    -1,    -1,    -1,    95,    96,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   589,    -1,    -1,    -1,    -1,   116,    -1,    -1,
     119,    -1,    -1,   540,    -1,   542,    -1,    -1,    -1,    -1,
      -1,    49,     2,    -1,     4,    53,    -1,   554,    -1,    -1,
      -1,    -1,    -1,    -1,   621,    -1,    -1,    -1,    -1,   566,
      -1,    -1,    -1,    -1,    -1,    -1,    74,    -1,    -1,   576,
     577,   578,   579,    -1,    -1,    -1,    -1,    -1,    86,    87,
      88,    -1,    -1,    -1,    -1,   592,    -1,    -1,    -1,    49,
      -1,   658,    -1,    -1,    -1,   662,    -1,   664,   665,    -1,
     197,    -1,   609,   670,   671,    -1,    -1,    -1,    -1,    -1,
      -1,   678,    -1,    -1,    -1,    -1,    -1,     2,    -1,     4,
      -1,    -1,   629,    -1,    -1,    -1,    86,    -1,    -1,    -1,
      -1,    -1,   229,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     237,    -1,    -1,    -1,    -1,    -1,   653,    -1,    -1,    -1,
     717,    -1,    -1,    -1,    -1,   722,   253,   724,   725,    -1,
      -1,    -1,    -1,    -1,    49,    -1,    -1,    -1,    -1,    -1,
      -1,   678,   679,    -1,   681,    -1,   683,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   283,    -1,    -1,   197,
      -1,   288,    -1,    -1,    -1,    -1,    -1,    -1,   295,   296,
     767,    -1,    -1,    -1,   771,    -1,    -1,    -1,    -1,    -1,
     777,   718,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   229,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   237,
     797,   328,    -1,    -1,    -1,    -1,    -1,   197,   745,   746,
      -1,   748,    -1,   750,   751,   253,    -1,    -1,   815,    -1,
      -1,    -1,   759,    -1,   761,    -1,   763,   764,    68,    69,
      70,    71,    72,    73,    74,    75,    -1,    77,    78,   229,
      -1,    -1,    -1,    83,    84,   283,    -1,   237,   785,    -1,
     288,   788,    -1,    -1,    -1,    -1,    -1,   295,   296,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   197,   410,    -1,    -1,    -1,    -1,    -1,    -1,
     328,    -1,    -1,   283,    -1,    -1,    -1,    -1,   288,    -1,
      -1,    -1,    -1,    -1,    -1,   295,   433,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   229,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   237,    -1,    -1,    -1,   863,    -1,    -1,    -1,
      -1,   928,    -1,    -1,    -1,    -1,    -1,    -1,   328,    -1,
      -1,    -1,   879,   880,   881,   882,    -1,    -1,    -1,   886,
     887,   888,    -1,   890,   891,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   494,   283,    -1,
      -1,    -1,   410,   288,    -1,    -1,    -1,    -1,    -1,    -1,
     295,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   928,    -1,   930,   931,   433,    -1,    -1,    -1,   936,
      44,    -1,   939,   940,   941,   942,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   328,    -1,    -1,    -1,   544,    -1,   546,
     410,    -1,   959,    -1,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    -1,    -1,    83,
      84,   568,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   494,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   110,    -1,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   614,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   410,    -1,   141,    -1,    -1,
      -1,    -1,    -1,   630,   631,    -1,   544,    -1,   546,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   645,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   654,    -1,    -1,
     568,    -1,    -1,    -1,   661,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   544,    -1,   546,     0,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     8,     9,    10,    -1,    -1,
      13,    14,    15,   700,    17,    -1,   614,    -1,   568,    -1,
      -1,   708,    -1,    -1,    27,    -1,    -1,    -1,   715,   716,
      -1,    -1,   630,   631,    37,    38,    -1,    40,    41,    42,
      43,    44,    -1,    -1,    -1,    -1,    -1,   645,   646,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   654,    -1,    -1,    -1,
      -1,    -1,    -1,   661,    -1,    -1,    -1,    -1,    -1,   544,
      -1,   546,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     630,   631,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   568,    -1,   645,    -1,    -1,    -1,    -1,
     787,    -1,   700,    -1,   654,    -1,    -1,   794,   795,    -1,
     708,   798,    -1,    -1,    -1,    -1,    -1,   715,   716,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   814,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   138,    -1,   140,    -1,    -1,
     143,   144,    -1,   146,    -1,   148,    -1,    -1,    -1,   836,
     700,    -1,    -1,    -1,    -1,   630,   631,    -1,   708,    -1,
      -1,    -1,    -1,   850,    -1,   715,   716,   854,    -1,    -1,
     645,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   865,   654,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   787,
      -1,    -1,    -1,    -1,    -1,    -1,   794,   795,    -1,    -1,
     798,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     897,    -1,   899,    -1,    -1,    -1,   814,    -1,   905,    -1,
     907,    -1,    -1,    -1,    -1,   700,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   708,    -1,    -1,    -1,   787,   836,    -1,
     715,   716,    -1,    -1,   794,   795,    -1,    -1,   798,    -1,
      -1,    -1,   850,    -1,    -1,    -1,   854,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   814,    -1,    -1,   865,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    -1,    -1,    83,    84,    -1,    -1,   897,
     850,   899,    -1,    -1,   854,    -1,    -1,   905,    -1,   907,
      -1,    -1,   787,    -1,    -1,   865,    -1,    -1,    -1,   794,
     795,    -1,   110,   798,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,    -1,    -1,    -1,    44,    -1,   814,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   897,    -1,   899,
      -1,    -1,    -1,    -1,    -1,   905,    -1,   907,    -1,    -1,
     148,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    -1,    83,    84,    -1,   854,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     865,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   110,    -1,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   897,    -1,   899,    -1,    -1,    -1,    -1,    -1,
     905,    -1,   907,     0,     1,    -1,     3,     4,     5,     6,
       7,    -1,    -1,    -1,    11,    12,    -1,    -1,    -1,    16,
      -1,    18,    19,    20,    21,    22,    23,    24,    -1,    -1,
      -1,    -1,    -1,    30,    31,    32,    33,    34,    35,    36,
      -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    -1,    59,    60,    -1,    62,    63,    64,    -1,    66,
      67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,
      -1,    -1,    89,    90,    -1,    92,    93,    -1,    95,    -1,
      97,    98,    99,   100,   101,   102,   103,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   122,   123,   124,     0,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     8,     9,    10,    -1,
      -1,    13,    14,    15,    -1,    17,    -1,    -1,    -1,   146,
      -1,   148,    -1,    -1,    -1,    27,    28,    29,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    37,    38,    -1,    40,    41,
      42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    -1,    -1,    83,    84,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    -1,
      -1,    83,    84,    85,    -1,    87,    88,    -1,    -1,    -1,
      -1,   110,    94,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,    -1,    -1,   107,    -1,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   138,   139,   140,   141,
       0,    -1,   144,   145,   146,    -1,   148,    -1,     8,     9,
      10,    -1,    -1,    13,    14,    15,    -1,    17,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    26,    27,    28,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    37,    38,    -1,
      40,    41,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,
      -1,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    -1,    83,    84,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    -1,    -1,    83,    84,    85,    -1,    -1,    88,    -1,
      -1,    -1,    -1,    -1,    94,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,    -1,    -1,    -1,    -1,    -1,
     110,    -1,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,   138,   139,
     140,   141,     0,   143,   144,   145,   146,    -1,   148,    -1,
       8,     9,    10,    -1,    -1,    13,    14,    15,    -1,    17,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,
      28,    29,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    37,
      38,    -1,    40,    41,    42,    43,    44,    -1,    -1,    -1,
      -1,    -1,    -1,    68,    69,    70,    71,    72,    73,    74,
      -1,    -1,    77,    78,    -1,    -1,    -1,    -1,    83,    84,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    -1,    -1,    83,    84,    85,    -1,    -1,
      88,    -1,    -1,    -1,    -1,    -1,    94,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,    -1,    -1,   107,
      -1,    -1,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     138,   139,   140,   141,     0,    -1,   144,   145,   146,    -1,
     148,    -1,     8,     9,    10,    -1,    -1,    13,    14,    15,
      -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      26,    27,    28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    37,    38,    -1,    40,    41,    42,    43,    44,    -1,
      -1,    -1,    -1,    -1,    -1,    68,    69,    70,    71,    72,
      73,    74,    -1,    -1,    77,    78,    -1,    -1,    -1,    -1,
      83,    84,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    -1,    -1,    83,    84,    85,
      -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,    94,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,    -1,
      -1,    -1,    -1,    -1,   110,    -1,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   137,   138,   139,   140,   141,     0,   143,   144,   145,
     146,    -1,   148,    -1,     8,     9,    10,    -1,    -1,    13,
      14,    15,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    27,    28,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    37,    38,    -1,    40,    41,    42,    43,
      44,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    -1,    -1,    83,
      84,    85,    -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,
      94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   110,    -1,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   138,   139,   140,   141,     0,   143,
     144,   145,   146,    -1,   148,    -1,     8,     9,    10,    -1,
      -1,    -1,    14,    15,    -1,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    26,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    37,    38,    -1,    40,    41,
      42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    -1,
      -1,    83,    84,    85,     0,    87,    -1,    -1,    -1,    -1,
      -1,    -1,     8,     9,    10,    -1,    -1,    -1,    14,    15,
      -1,    17,    -1,    -1,    -1,    -1,    -1,   109,   110,    -1,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
      -1,    37,    38,    -1,    40,    41,    42,    43,    44,    -1,
      -1,    -1,    -1,    -1,    -1,   137,   138,   139,   140,   141,
      -1,    -1,   144,    -1,   146,    -1,   148,    -1,    -1,    -1,
      -1,    -1,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    -1,    -1,    83,    84,    85,
      -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   109,   110,    -1,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   138,   139,   140,   141,    -1,    -1,   144,    -1,
     146,     1,   148,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    -1,    -1,    15,    16,    -1,    18,    19,
      20,    21,    22,    23,    24,    -1,    -1,    -1,    -1,    -1,
      30,    31,    32,    33,    34,    35,    36,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    -1,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    -1,    59,
      60,    -1,    62,    63,    64,    -1,    66,    67,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,    -1,    89,
      90,    -1,    92,    93,    -1,    95,    -1,    97,    98,    99,
     100,   101,   102,   103,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   122,   123,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   146,     1,   148,     3,
       4,     5,     6,     7,    -1,    -1,    10,    11,    12,    -1,
      14,    15,    16,    -1,    18,    19,    20,    21,    22,    23,
      24,    -1,    -1,    -1,    -1,    -1,    30,    31,    32,    33,
      34,    35,    36,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      -1,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    -1,    59,    60,    -1,    62,    63,
      64,    -1,    66,    67,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    86,    -1,    -1,    89,    90,    -1,    92,    93,
      -1,    95,    -1,    97,    98,    99,   100,   101,   102,   103,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,   123,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   146,     1,   148,     3,     4,     5,     6,     7,
      -1,    -1,    10,    11,    12,    -1,    -1,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    -1,    -1,    -1,
      -1,    -1,    30,    31,    32,    33,    34,    35,    36,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      -1,    59,    60,    -1,    62,    63,    64,    -1,    66,    67,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,
      -1,    89,    90,    -1,    92,    93,    -1,    95,    -1,    97,
      98,    99,   100,   101,   102,   103,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   122,   123,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,     1,
     148,     3,     4,     5,     6,     7,    -1,    -1,    10,    11,
      12,    -1,    -1,    15,    16,    -1,    18,    19,    20,    21,
      22,    23,    24,    -1,    -1,    -1,    -1,    -1,    30,    31,
      32,    33,    34,    35,    36,    -1,    -1,    39,    -1,    -1,
      -1,    -1,    -1,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    -1,    59,    60,    -1,
      62,    63,    64,    -1,    66,    67,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    86,    -1,    -1,    89,    90,    -1,
      92,    93,    -1,    95,    -1,    97,    98,    99,   100,   101,
     102,   103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     122,   123,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       1,    -1,     3,     4,     5,     6,     7,    -1,     9,    10,
      11,    12,    -1,    -1,   146,    16,   148,    18,    19,    20,
      21,    22,    23,    24,    -1,    -1,    -1,    -1,    -1,    30,
      31,    32,    33,    34,    35,    36,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    -1,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    -1,    59,    60,
      -1,    62,    63,    64,    -1,    66,    67,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    86,    -1,    -1,    89,    90,
      -1,    92,    93,    -1,    95,    -1,    97,    98,    99,   100,
     101,   102,   103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   122,   123,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     1,    -1,     3,     4,     5,     6,     7,    -1,    -1,
      -1,    11,    12,    -1,    -1,   146,    16,   148,    18,    19,
      20,    21,    22,    23,    24,    -1,    -1,    -1,    -1,    -1,
      30,    31,    32,    33,    34,    35,    36,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    -1,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    -1,    59,
      60,    -1,    62,    63,    64,    -1,    66,    67,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,    -1,    89,
      90,    -1,    92,    93,    -1,    95,    -1,    97,    98,    99,
     100,   101,   102,   103,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   122,   123,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     1,    -1,     3,     4,     5,     6,     7,    -1,
      -1,    -1,    11,    12,   144,    -1,   146,    16,   148,    18,
      19,    20,    21,    22,    23,    24,    -1,    -1,    -1,    -1,
      -1,    30,    31,    32,    33,    34,    35,    36,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    -1,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    -1,
      59,    60,    -1,    62,    63,    64,    -1,    66,    67,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,    -1,
      89,    90,    -1,    92,    93,    -1,    95,    -1,    97,    98,
      99,   100,   101,   102,   103,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   122,   123,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     1,    -1,     3,     4,     5,     6,     7,
      -1,    -1,    -1,    11,    12,   144,    -1,   146,    16,   148,
      18,    19,    20,    21,    22,    23,    24,    -1,    -1,    -1,
      -1,    -1,    30,    31,    32,    33,    34,    35,    36,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      -1,    59,    60,    -1,    62,    63,    64,    -1,    66,    67,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,
      -1,    89,    90,    -1,    92,    93,    -1,    95,    -1,    97,
      98,    99,   100,   101,   102,   103,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   122,   123,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   146,     1,
     148,     3,     4,     5,     6,     7,    -1,    -1,    10,    11,
      12,    -1,    -1,    -1,    16,    -1,    18,    19,    20,    21,
      22,    23,    24,    -1,    -1,    -1,    -1,    -1,    30,    31,
      32,    33,    34,    35,    36,    -1,    -1,    39,    -1,    -1,
      -1,    -1,    -1,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    -1,    59,    60,    -1,
      62,    63,    64,    -1,    66,    67,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    86,    -1,    -1,    89,    90,    -1,
      92,    93,    -1,    95,    -1,    97,    98,    99,   100,   101,
     102,   103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     122,   123,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,    -1,     7,    -1,    -1,    -1,
      11,    12,    -1,    -1,   146,    16,   148,    18,    19,    20,
      21,    22,    23,    24,    -1,    -1,    -1,    -1,    -1,    30,
      31,    32,    33,    34,    35,    36,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      -1,    62,    63,    64,    -1,    66,    67,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    86,    -1,    -1,    89,    90,
      -1,    92,    93,    -1,    95,    96,    97,    98,    99,   100,
     101,   102,   103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   122,   123,   124,     3,     4,     5,    -1,     7,    -1,
      -1,    -1,    11,    12,    -1,    -1,    -1,    16,    -1,    18,
      19,    20,    21,    22,    23,    24,    -1,   148,    -1,    -1,
      -1,    30,    31,    32,    33,    34,    35,    36,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    -1,
      59,    60,    -1,    62,    63,    64,    -1,    66,    67,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,    -1,
      89,    90,    -1,    92,    93,    -1,    -1,    -1,    97,    98,
      99,   100,   101,   102,   103,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   122,   123,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,    -1,     7,
      -1,    -1,    -1,    11,    12,    -1,    -1,   146,    16,   148,
      18,    19,    20,    21,    22,    23,    24,    -1,    -1,    -1,
      -1,    -1,    30,    31,    32,    33,    34,    35,    36,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    -1,    -1,    46,    -1,
      -1,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      -1,    59,    60,    -1,    62,    63,    64,    -1,    66,    67,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,
      -1,    89,    90,    -1,    92,    93,    -1,    -1,    -1,    97,
      98,    99,   100,   101,   102,   103,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   122,   123,   124,     3,     4,     5,
       6,     7,    -1,    -1,    -1,    11,    12,    -1,    -1,    -1,
      16,    -1,    18,    19,    20,    21,    22,    23,    24,    -1,
     148,    -1,    -1,    -1,    30,    31,    32,    33,    34,    35,
      36,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    -1,    59,    60,    -1,    62,    63,    64,    -1,
      66,    67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      86,    -1,    -1,    89,    90,    -1,    92,    93,    -1,    95,
      -1,    97,    98,    99,   100,   101,   102,   103,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   122,   123,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     146,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    -1,    -1,    -1,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    -1,    -1,
      -1,    -1,    -1,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    -1,    -1,    77,    78,    -1,    -1,    81,
      82,    83,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
      -1,   123,   124,    -1,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,    -1,     7,    -1,    -1,    -1,    11,    12,    -1,
     142,   143,    16,    -1,    18,    19,    20,    21,    22,    23,
      24,    -1,    26,    -1,    -1,    -1,    30,    31,    32,    33,
      34,    35,    36,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      -1,    -1,    46,    -1,    -1,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    -1,    62,    63,
      64,    -1,    66,    67,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    86,    -1,    -1,    89,    90,    -1,    92,    93,
      -1,    95,    96,    97,    98,    99,   100,   101,   102,   103,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,   123,
     124,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
      -1,     7,    -1,   137,    -1,    11,    12,    -1,    -1,   143,
      16,    -1,    18,    19,    20,    21,    22,    23,    24,    -1,
      26,    -1,    -1,    -1,    30,    31,    32,    33,    34,    35,
      36,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    -1,
      46,    -1,    -1,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    -1,    62,    63,    64,    -1,
      66,    67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      86,    -1,    -1,    89,    90,    -1,    92,    93,    -1,    95,
      96,    97,    98,    99,   100,   101,   102,   103,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   122,   123,   124,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,    -1,     7,
      -1,   137,    -1,    11,    12,    -1,    -1,   143,    16,    -1,
      18,    19,    20,    21,    22,    23,    24,    -1,    -1,    -1,
      -1,    -1,    30,    31,    32,    33,    34,    35,    36,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    -1,    -1,    46,    -1,
      -1,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    -1,    62,    63,    64,    -1,    66,    67,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,    87,
      -1,    89,    90,    -1,    92,    93,    -1,    95,    96,    97,
      98,    99,   100,   101,   102,   103,    -1,    -1,    -1,    -1,
      -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,    -1,     7,   122,   123,   124,    11,    12,    -1,
      -1,    -1,    16,    -1,    18,    19,    20,    21,    22,    23,
      24,    -1,    -1,    -1,    -1,   143,    30,    31,    32,    33,
      34,    35,    36,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      -1,    -1,    46,    -1,    -1,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    -1,    62,    63,
      64,    -1,    66,    67,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    86,    -1,    -1,    89,    90,    -1,    92,    93,
      -1,    95,    96,    97,    98,    99,   100,   101,   102,   103,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,    -1,     7,   122,   123,
     124,    11,    12,    -1,    -1,    -1,    16,    -1,    18,    19,
      20,    21,    22,    23,    24,    -1,    -1,    -1,    -1,   143,
      30,    31,    32,    33,    34,    35,    36,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    -1,    62,    63,    64,    -1,    66,    67,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,    -1,    89,
      90,    -1,    92,    93,    -1,    95,    96,    97,    98,    99,
     100,   101,   102,   103,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
      -1,     7,   122,   123,   124,    11,    12,    -1,    -1,    -1,
      16,    -1,    18,    19,    20,    21,    22,    23,    24,    -1,
      -1,    -1,    -1,   143,    30,    31,    32,    33,    34,    35,
      36,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    -1,
      46,    -1,    -1,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    -1,    59,    60,    -1,    62,    63,    64,    -1,
      66,    67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      86,    -1,    -1,    89,    90,    -1,    92,    93,    -1,    -1,
      -1,    97,    98,    99,   100,   101,   102,   103,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   122,   123,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    -1,    -1,    -1,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    -1,    -1,    -1,    -1,    -1,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      -1,    -1,    77,    78,    -1,    -1,    81,    82,    83,    84,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,    -1,   123,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    -1,    -1,    -1,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    -1,    -1,    -1,    -1,    -1,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      -1,    56,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      -1,    -1,    77,    78,    -1,    -1,    81,    82,    83,    84,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      95,    -1,    -1,    98,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,    -1,   123,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    -1,    -1,    -1,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    -1,    -1,    -1,    -1,    -1,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    -1,
      -1,    56,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      -1,    -1,    77,    78,    -1,    -1,    81,    82,    83,    84,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      95,    -1,    -1,    98,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,    -1,   123,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    -1,    -1,    -1,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    -1,    -1,    -1,    -1,    -1,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    -1,
      -1,    56,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      -1,    -1,    77,    78,    -1,    -1,    81,    82,    83,    84,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,    -1,   123,   124,
      -1,    -1,    -1,     3,     4,     5,    -1,     7,    -1,    -1,
      -1,    11,    12,    -1,    -1,    -1,    16,   142,    18,    19,
      20,    21,    22,    23,    24,    -1,    -1,    -1,    -1,    -1,
      30,    31,    32,    33,    34,    35,    36,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    -1,    59,
      60,    -1,    62,    63,    64,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,    -1,    89,
      90,    -1,    92,    93,    -1,    -1,    -1,    97,    98,    99,
     100,   101,   102,   103,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,    -1,     7,
      -1,    -1,   122,    11,    12,    -1,    -1,    -1,    16,    -1,
      18,    19,    20,    21,    22,    23,    24,    -1,    -1,    -1,
      -1,   141,    30,    31,    32,    33,    34,    35,    36,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    -1,    -1,    46,    -1,
      -1,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      -1,    59,    60,    -1,    62,    63,    64,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,
      -1,    89,    90,    -1,    92,    93,    -1,    -1,    -1,    97,
      98,    99,   100,   101,   102,   103,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,     7,    -1,    -1,   122,    11,    12,    -1,    -1,    -1,
      16,    -1,    18,    19,    20,    21,    22,    23,    24,    -1,
      -1,    -1,    -1,   141,    30,    31,    32,    33,    34,    35,
      36,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    -1,    59,    60,    -1,    62,    63,    64,    -1,
      66,    67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      86,    -1,    -1,    89,    90,    -1,    92,    93,    -1,    95,
      -1,    97,    98,    99,   100,   101,   102,   103,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,    -1,
       7,    -1,    -1,    -1,    11,    12,   122,   123,   124,    16,
      -1,    18,    19,    20,    21,    22,    23,    24,    -1,    -1,
      -1,    -1,    -1,    30,    31,    32,    33,    34,    35,    36,
      -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    -1,    46,
      -1,    -1,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    -1,    62,    63,    64,    -1,    66,
      67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,
      -1,    -1,    89,    90,    -1,    92,    93,    -1,    95,    96,
      97,    98,    99,   100,   101,   102,   103,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,    -1,     7,
      -1,    -1,    -1,    11,    12,   122,   123,   124,    16,    -1,
      18,    19,    20,    21,    22,    23,    24,    -1,    -1,    -1,
      -1,    -1,    30,    31,    32,    33,    34,    35,    36,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    -1,    -1,    46,    -1,
      -1,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    -1,    62,    63,    64,    -1,    66,    67,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,
      -1,    89,    90,    -1,    92,    93,    -1,    95,    96,    97,
      98,    99,   100,   101,   102,   103,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,    -1,     7,    -1,
      -1,    -1,    11,    12,   122,   123,   124,    16,    -1,    18,
      19,    20,    21,    22,    23,    24,    -1,    -1,    -1,    -1,
      -1,    30,    31,    32,    33,    34,    35,    36,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    -1,    62,    63,    64,    -1,    66,    67,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,    -1,
      89,    90,    -1,    92,    93,    -1,    95,    -1,    97,    98,
      99,   100,   101,   102,   103,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,    -1,     7,    -1,    -1,
      -1,    11,    12,   122,   123,   124,    16,    -1,    18,    19,
      20,    21,    22,    23,    24,    -1,    -1,    -1,    -1,    -1,
      30,    31,    32,    33,    34,    35,    36,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    -1,    62,    63,    64,    -1,    66,    67,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,    -1,    89,
      90,    -1,    92,    93,    -1,    -1,    96,    97,    98,    99,
     100,   101,   102,   103,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,    -1,     7,    -1,    -1,    -1,
      11,    12,   122,   123,   124,    16,    -1,    18,    19,    20,
      21,    22,    23,    24,    -1,    -1,    -1,    -1,    -1,    30,
      31,    32,    33,    34,    35,    36,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      -1,    62,    63,    64,    -1,    66,    67,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    86,    -1,    -1,    89,    90,
      -1,    92,    93,    -1,    95,    -1,    97,    98,    99,   100,
     101,   102,   103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,    -1,     7,    -1,    -1,    -1,    11,
      12,   122,   123,   124,    16,    -1,    18,    19,    20,    21,
      22,    23,    24,    -1,    -1,    -1,    -1,    -1,    30,    31,
      32,    33,    34,    35,    36,    -1,    -1,    39,    -1,    -1,
      -1,    -1,    -1,    -1,    46,    -1,    -1,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    -1,
      62,    63,    64,    -1,    66,    67,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    86,    -1,    -1,    89,    90,    -1,
      92,    93,    -1,    -1,    -1,    97,    98,    99,   100,   101,
     102,   103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,    -1,     7,    -1,    -1,    -1,    11,    12,
     122,   123,   124,    16,    -1,    18,    19,    20,    21,    22,
      23,    24,    -1,    -1,    -1,    -1,    -1,    30,    31,    32,
      33,    34,    35,    36,    -1,    -1,    39,    -1,    -1,    -1,
      -1,    -1,    -1,    46,    -1,    -1,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    -1,    59,    60,    -1,    62,
      63,    64,    -1,    66,    67,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    86,    -1,    -1,    89,    90,    -1,    92,
      93,    -1,    95,    -1,    97,    98,    99,   100,   101,   102,
     103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,    -1,     7,    -1,    -1,    -1,    11,    12,   122,
     123,   124,    16,    -1,    18,    19,    20,    21,    22,    23,
      24,    -1,    -1,    -1,    -1,    -1,    30,    31,    32,    33,
      34,    35,    36,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      -1,    -1,    46,    -1,    -1,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    -1,    59,    60,    -1,    62,    63,
      64,    -1,    66,    67,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    86,    -1,    -1,    89,    90,    -1,    92,    93,
      -1,    95,    -1,    97,    98,    99,   100,   101,   102,   103,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,    -1,     7,    -1,    -1,    -1,    11,    12,   122,   123,
     124,    16,    -1,    18,    19,    20,    21,    22,    23,    24,
      -1,    -1,    -1,    -1,    -1,    30,    31,    32,    33,    34,
      35,    36,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,
      -1,    46,    -1,    -1,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    -1,    59,    60,    -1,    62,    63,    64,
      -1,    66,    67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    86,    -1,    -1,    89,    90,    -1,    92,    93,    -1,
      95,    -1,    97,    98,    99,   100,   101,   102,   103,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
      -1,     7,    -1,    -1,    -1,    11,    12,   122,   123,   124,
      16,    -1,    18,    19,    20,    21,    22,    23,    24,    -1,
      -1,    -1,    -1,    -1,    30,    31,    32,    33,    34,    35,
      36,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    -1,
      46,    -1,    -1,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    -1,    59,    60,    -1,    62,    63,    64,    -1,
      66,    67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      86,    -1,    -1,    89,    90,    -1,    92,    93,    -1,    95,
      -1,    97,    98,    99,   100,   101,   102,   103,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,    -1,
       7,    -1,    -1,    -1,    11,    12,   122,   123,   124,    16,
      -1,    18,    19,    20,    21,    22,    23,    24,    -1,    -1,
      -1,    -1,    -1,    30,    31,    32,    33,    34,    35,    36,
      -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    -1,    46,
      -1,    -1,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    -1,    59,    60,    -1,    62,    63,    64,    -1,    66,
      67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,
      -1,    -1,    89,    90,    -1,    92,    93,    -1,    95,    -1,
      97,    98,    99,   100,   101,   102,   103,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,    -1,     7,
      -1,    -1,    -1,    11,    12,   122,   123,   124,    16,    -1,
      18,    19,    20,    21,    22,    23,    24,    -1,    -1,    -1,
      -1,    -1,    30,    31,    32,    33,    34,    35,    36,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    -1,    -1,    46,    -1,
      -1,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      -1,    59,    60,    -1,    62,    63,    64,    -1,    66,    67,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,
      -1,    89,    90,    -1,    92,    93,    -1,    -1,    -1,    97,
      98,    99,   100,   101,   102,   103,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,    -1,     7,    -1,
      -1,    -1,    11,    12,   122,   123,   124,    16,    -1,    18,
      19,    20,    21,    22,    23,    24,    -1,    -1,    -1,    -1,
      -1,    30,    31,    32,    33,    34,    35,    36,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    -1,
      59,    60,    -1,    62,    63,    64,    -1,    66,    67,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,    -1,
      89,    90,    -1,    92,    93,    -1,    -1,    -1,    97,    98,
      99,   100,   101,   102,   103,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,    -1,     7,    -1,    -1,
      -1,    11,    12,   122,   123,   124,    16,    -1,    18,    19,
      20,    21,    22,    23,    24,    -1,    -1,    -1,    -1,    -1,
      30,    31,    32,    33,    34,    35,    36,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    -1,    59,
      60,    -1,    62,    63,    64,    -1,    66,    67,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,    -1,    89,
      90,    -1,    92,    93,    -1,    -1,    -1,    97,    98,    99,
     100,   101,   102,   103,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,    -1,     7,    -1,    -1,    -1,
      11,    12,   122,   123,   124,    16,    -1,    18,    19,    20,
      21,    22,    23,    24,    -1,    -1,    -1,    -1,    -1,    30,
      31,    32,    33,    34,    35,    36,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    -1,    59,    60,
      -1,    62,    63,    64,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    86,    -1,    -1,    89,    90,
      -1,    92,    93,    -1,    95,    -1,    97,    98,    99,   100,
     101,   102,   103,    -1,    -1,    -1,    -1,     3,     4,     5,
      -1,     7,    -1,    -1,    -1,    11,    12,    -1,    -1,    -1,
      16,   122,    18,    19,    20,    21,    22,    23,    24,    -1,
      -1,    -1,    -1,    -1,    30,    31,    32,    33,    34,    35,
      36,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    -1,
      46,    -1,    -1,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    -1,    59,    60,    -1,    62,    63,    64,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      86,    -1,    -1,    89,    90,    -1,    92,    93,    -1,    95,
      -1,    97,    98,    99,   100,   101,   102,   103,    -1,    -1,
      -1,    -1,     3,     4,     5,    -1,     7,    -1,    -1,    -1,
      11,    12,    -1,    -1,    -1,    16,   122,    18,    19,    20,
      21,    22,    23,    24,    -1,    -1,    -1,    -1,    -1,    30,
      31,    32,    33,    34,    35,    36,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    -1,    59,    60,
      -1,    62,    63,    64,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    83,    -1,    -1,    86,    -1,    -1,    89,    90,
      -1,    92,    93,    -1,    -1,    -1,    97,    98,    99,   100,
     101,   102,   103,    -1,    -1,    -1,    -1,     3,     4,     5,
      -1,     7,    -1,    -1,    -1,    11,    12,    -1,    -1,    -1,
      16,   122,    18,    19,    20,    21,    22,    23,    24,    -1,
      -1,    -1,    -1,    -1,    30,    31,    32,    33,    34,    35,
      36,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    -1,
      46,    -1,    -1,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    -1,    59,    60,    -1,    62,    63,    64,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      86,    -1,    -1,    89,    90,    -1,    92,    93,    -1,    -1,
      -1,    97,    98,    99,   100,   101,   102,   103,    -1,    -1,
      -1,    -1,     3,     4,     5,    -1,     7,    -1,    -1,    -1,
      11,    12,    -1,    -1,    -1,    16,   122,    18,    19,    20,
      21,    22,    23,    24,    -1,    -1,    -1,    -1,    -1,    30,
      31,    32,    33,    34,    35,    36,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    -1,    59,    60,
      -1,    62,    63,    64,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    86,    -1,    -1,    89,    90,
      -1,    92,    93,    -1,    -1,    -1,    97,    98,    99,   100,
     101,   102,   103,    -1,    -1,    -1,    -1,     3,     4,     5,
      -1,     7,    -1,    -1,    -1,    11,    12,    -1,    -1,    -1,
      16,   122,    18,    19,    20,    21,    22,    23,    24,    -1,
      -1,    -1,    -1,    -1,    30,    31,    32,    33,    34,    35,
      36,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    -1,
      46,    -1,    -1,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    -1,    59,    60,    -1,    62,    63,    64,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      86,    -1,    -1,    89,    90,    -1,    92,    93,    -1,    -1,
      -1,    97,    98,    99,   100,   101,   102,   103,    -1,    -1,
      -1,    -1,     3,     4,     5,    -1,     7,    -1,    -1,    -1,
      11,    12,    -1,    -1,    -1,    16,   122,    18,    19,    20,
      21,    22,    23,    24,    -1,    -1,    -1,    -1,    -1,    30,
      31,    32,    33,    34,    35,    36,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    -1,    59,    60,
      -1,    62,    63,    64,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    86,    -1,    -1,    89,    90,
      -1,    92,    93,    -1,    -1,    -1,    97,    98,    99,   100,
     101,   102,   103,    -1,    -1,    -1,    -1,    -1,    52,    53,
      -1,    -1,    56,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   122,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    -1,    -1,    77,    78,    -1,    -1,    81,    82,    83,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,    -1,   123,
     124,    -1,    -1,    -1,    -1,    -1,    52,    53,    -1,    -1,
      56,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    -1,
      -1,    77,    78,    -1,    -1,    81,    82,    83,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,    -1,   123,   124,    -1,
      -1,    -1,    -1,    -1,    52,    53,    -1,    -1,    56,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   142,   143,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    -1,    -1,    77,
      78,    -1,    -1,    81,    82,    83,    84,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,    -1,   123,   124,    -1,    -1,    -1,
      -1,    -1,    52,    53,    -1,    -1,    56,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   142,   143,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    -1,    -1,    77,    78,    -1,
      -1,    81,    82,    83,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    95,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,    -1,   123,   124,    -1,    -1,    -1,    -1,    -1,
      52,    53,    -1,    -1,    56,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   142,   143,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    -1,    -1,    77,    78,    -1,    -1,    81,
      82,    83,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
      -1,   123,   124,    -1,    -1,    -1,    -1,    -1,    52,    53,
      -1,    -1,    56,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     142,   143,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    -1,    -1,    77,    78,    -1,    -1,    81,    82,    83,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,    -1,   123,
     124,    -1,    -1,    -1,    -1,    -1,    52,    53,    -1,    -1,
      56,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    -1,
      -1,    77,    78,    -1,    -1,    81,    82,    83,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,    -1,   123,   124,    -1,
      -1,    -1,    -1,    -1,    52,    53,    -1,    -1,    56,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   142,   143,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    -1,    -1,    77,
      78,    -1,    -1,    81,    82,    83,    84,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,    -1,   123,   124,    -1,    -1,    -1,
      -1,    -1,    52,    53,    -1,    -1,    56,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   142,   143,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    -1,    -1,    77,    78,    -1,
      -1,    81,    82,    83,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    95,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,    -1,   123,   124,    -1,    -1,    -1,    -1,    -1,
      52,    53,    -1,    -1,    56,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   142,   143,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    -1,    -1,    77,    78,    -1,    -1,    81,
      82,    83,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
      -1,   123,   124,    -1,    -1,    -1,    -1,    -1,    52,    53,
      -1,    -1,    56,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     142,   143,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    -1,    -1,    77,    78,    -1,    -1,    81,    82,    83,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,    -1,   123,
     124,    -1,    -1,    -1,    -1,    -1,    52,    53,    -1,    -1,
      56,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    -1,
      -1,    77,    78,    -1,    -1,    81,    82,    83,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,    -1,   123,   124,    -1,
      -1,    -1,    -1,    -1,    52,    53,    -1,    -1,    56,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   142,   143,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    -1,    -1,    77,
      78,    -1,    -1,    81,    82,    83,    84,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,    -1,   123,   124,    -1,    -1,    -1,
      -1,    -1,    52,    53,    -1,    -1,    56,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   142,   143,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    -1,    -1,    77,    78,    -1,
      -1,    81,    82,    83,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    95,    -1,    -1,    -1,    -1,
      -1,    44,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,    -1,   123,   124,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    -1,    -1,
      83,    84,   142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,    -1,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned short int yystos[] =
{
       0,   150,   151,     0,     1,     3,     4,     5,     6,     7,
      11,    12,    16,    18,    19,    20,    21,    22,    23,    24,
      30,    31,    32,    33,    34,    35,    36,    39,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    59,    60,    62,    63,    64,    66,    67,    86,    89,
      90,    92,    93,    95,    97,    98,    99,   100,   101,   102,
     103,   122,   123,   124,   153,   154,   155,   158,   160,   161,
     164,   165,   167,   168,   169,   171,   172,   182,   196,   213,
     214,   215,   216,   217,   218,   219,   220,   221,   222,   223,
     245,   246,   256,   257,   258,   259,   260,   261,   262,   265,
     273,   275,   276,   277,   278,   279,   280,   303,   314,   155,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    56,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    77,    78,    81,    82,    83,    84,    95,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     123,   124,   142,   175,   176,   177,   178,   180,   181,   273,
     275,    21,    22,    30,    39,    58,    86,    89,    95,    96,
     123,   164,   172,   182,   184,   189,   192,   194,   213,   277,
     279,   280,   301,   302,   189,   189,   143,   190,   191,   143,
     186,   190,   143,   158,    54,   177,   148,   308,   157,   137,
      31,    32,   182,   213,   303,   182,    56,    89,   153,   166,
     167,   158,   185,   194,   301,   314,   184,   300,   301,   314,
      46,    86,   122,   141,   171,   196,   213,   277,   280,   239,
     240,    54,    55,    57,   175,   268,   274,   267,   268,   268,
     147,   263,   147,   266,    59,    60,   160,   182,   182,   146,
     148,   307,   312,   313,    40,    41,    42,    43,    44,    37,
      38,    28,   243,   109,   141,    89,    95,   168,   109,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    83,    84,   110,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,    85,   139,   140,   197,   158,
     159,   159,   200,   202,   159,   307,   313,    86,   165,   172,
     213,   229,   277,   280,    52,    56,    83,    86,   173,   174,
     213,   277,   280,   174,    33,    34,    35,    36,    49,    50,
      51,    52,    56,   143,   175,   278,   298,    85,   140,    26,
     137,   247,   259,    87,    87,   186,   190,   247,   141,   184,
      56,   184,   184,   109,    88,   141,   193,   314,    85,   139,
     140,    87,    87,   141,   193,   189,   308,   309,   189,   188,
     189,   314,   158,   309,    54,    63,    64,   156,   143,   183,
     137,   153,    85,   140,    87,   166,   144,   309,   198,   145,
     141,   148,   311,   141,   311,   138,   311,   308,    56,    59,
      60,   168,   170,   141,    85,   139,   140,    52,    54,    55,
      56,    57,    89,    95,    96,   116,   119,   143,   241,   284,
     285,   286,   287,   288,   289,   292,   293,   294,   295,   296,
      61,   104,   105,   106,   269,   106,   269,   106,    65,   106,
     106,   264,   269,   106,    61,   106,    68,    68,   146,   155,
     159,   159,   159,   159,   155,   158,   158,   244,    95,   160,
     184,   194,   195,   166,   141,   171,   141,   160,   182,   184,
     195,   182,   182,   182,   182,   182,   182,   182,   182,   182,
     182,   182,   182,   182,   182,   182,   182,   182,   182,   182,
     182,   182,   182,   182,   182,   182,   182,    52,    53,    56,
     180,   186,   304,   305,   188,    52,    53,    56,   180,   186,
     304,   152,   153,    13,   225,   312,   225,   159,   159,   307,
      17,   250,    56,    85,   139,   140,    25,   158,    52,    56,
     173,     1,   113,   281,   312,    85,   139,   140,   209,   299,
     210,    85,   140,   306,    52,    56,   304,   304,   249,   248,
     160,   182,   160,   182,    94,   162,   179,   182,   184,    95,
     184,   192,   301,    52,    56,   188,    52,    56,   302,   309,
     144,   309,   309,   177,   199,   182,   153,   138,   304,   304,
     182,   309,   309,   301,   141,   170,    52,    56,   188,    52,
      56,   109,    52,    89,    95,   230,   231,   232,   286,   284,
      29,   107,   242,   146,   236,   314,   141,   297,   314,   141,
     297,    52,   141,   297,    52,   271,   270,   147,   269,   147,
     182,   182,    76,   114,   234,   235,   314,   184,   141,   309,
     170,   141,    44,   308,    87,    87,   186,   190,   308,   310,
      87,    87,   186,   187,   190,   314,    10,   224,     8,   252,
     314,   153,    13,   153,    27,   226,   312,   226,   250,   194,
     224,    52,    56,   188,    52,    56,   204,   207,   312,   282,
     206,    52,    56,   173,   188,   152,   158,   143,   283,   284,
     211,   187,   190,   187,   190,   234,   234,    44,   163,   177,
     184,   193,    87,   310,    87,    87,   158,   138,   311,   168,
     310,   184,   232,   141,   286,   141,   309,   236,   153,   153,
      52,   237,   238,   285,   287,   292,   294,   296,   288,   289,
     294,   288,   153,    54,    55,    57,   272,   280,    52,   233,
     236,   288,   290,   291,   294,   296,   153,    95,   184,   170,
     182,   111,   160,   182,   182,   162,   145,    87,   160,   182,
     160,   182,   162,   184,   195,   253,   314,    15,   228,   314,
      14,   227,   228,   228,   201,   203,   224,   141,   225,   310,
     159,   312,   159,   152,   310,   224,   309,   284,   152,   312,
     175,   153,   153,   182,   234,    87,   309,   309,   231,   141,
      95,   230,   309,    10,   138,   141,   141,   297,   141,   297,
     141,   297,   141,   297,   297,   138,   109,   236,   114,   141,
     297,   141,   297,   141,   297,    10,   184,   182,   160,   182,
      88,   254,   314,   153,     9,   255,   314,   159,   224,   224,
     153,   153,   184,   153,   226,   208,   312,   224,   309,   224,
     212,    10,   138,   153,   141,   231,   141,   286,   238,   288,
     294,   288,   288,    86,   213,   114,   291,   294,   288,   290,
     294,   288,    86,   172,   213,   277,   280,   225,   153,   225,
     224,   224,   228,   250,   251,   205,   152,   283,   138,   231,
     141,   297,   141,   297,   297,   297,    56,    85,   141,   297,
     141,   297,   297,   141,   297,   297,    56,    85,   139,   140,
     153,   153,   153,   224,   152,   231,   288,    52,    56,   288,
     294,   288,   288,    52,    56,   188,    52,    56,   252,   227,
     224,   224,   297,   297,   141,   297,   297,   297,   310,   288,
     297
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      parser_yyerror (parser, YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (0)


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (N)								\
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (0)
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
              (Loc).first_line, (Loc).first_column,	\
              (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (&yylval, YYLEX_PARAM)
#else
# define YYLEX yylex (&yylval)
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (0)

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)		\
do {								\
  if (yydebug)							\
    {								\
      YYFPRINTF (stderr, "%s ", Title);				\
      yysymprint (stderr,					\
                  Type, Value);	\
      YYFPRINTF (stderr, "\n");					\
    }								\
} while (0)

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_stack_print (short int *bottom, short int *top)
#else
static void
yy_stack_print (bottom, top)
    short int *bottom;
    short int *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (/* Nothing. */; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_reduce_print (int yyrule)
#else
static void
yy_reduce_print (yyrule)
    int yyrule;
#endif
{
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu), ",
             yyrule - 1, yylno);
  /* Print the symbols being reduced, and their result.  */
  for (yyi = yyprhs[yyrule]; 0 <= yyrhs[yyi]; yyi++)
    YYFPRINTF (stderr, "%s ", yytname[yyrhs[yyi]]);
  YYFPRINTF (stderr, "-> %s\n", yytname[yyr1[yyrule]]);
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (Rule);		\
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
#ifndef yydebug
int yydebug;
#endif
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
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



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined (__GLIBC__) && defined (_STRING_H)
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
#   if defined (__STDC__) || defined (__cplusplus)
yystrlen (const char *yystr)
#   else
yystrlen (yystr)
     const char *yystr;
#   endif
{
  const char *yys = yystr;

  while (*yys++ != '\0')
    continue;

  return yys - yystr - 1;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined (__GLIBC__) && defined (_STRING_H) && defined (_GNU_SOURCE)
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
#   if defined (__STDC__) || defined (__cplusplus)
yystpcpy (char *yydest, const char *yysrc)
#   else
yystpcpy (yydest, yysrc)
     char *yydest;
     const char *yysrc;
#   endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      size_t yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

#endif /* YYERROR_VERBOSE */



#if YYDEBUG
/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yysymprint (FILE *yyoutput, int yytype, YYSTYPE *yyvaluep)
#else
static void
yysymprint (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);


# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  switch (yytype)
    {
      default:
        break;
    }
  YYFPRINTF (yyoutput, ")");
}

#endif /* ! YYDEBUG */
/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
ruby_parser_yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, struct parser_params *parser)
#else
static void
ruby_parser_yydestruct (yymsg, yytype, yyvaluep, parser)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
    struct parser_params *parser;
#endif
#define yydestruct(m, t, v) ruby_parser_yydestruct(m, t, v, parser)
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
        break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM);
# else
int yyparse ();
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int yyparse (struct parser_params *parser);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */






/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM)
# else
int yyparse (YYPARSE_PARAM)
  void *YYPARSE_PARAM;
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int
yyparse (struct parser_params *parser)
#else
int
yyparse (parser)
    struct parser_params *parser;
#endif
#endif
{
  /* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;

  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  short int yyssa[YYINITDEPTH];
  short int *yyss = yyssa;
  short int *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK   (yyvsp--, yyssp--)

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* When reducing, the number of symbols on the RHS of the reduced
     rule.  */
  int yylen;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed. so pushing a state here evens the stacks.
     */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack. Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	short int *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	short int *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

/* Do appropriate processing given the current state.  */
/* Read a look-ahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to look-ahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
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
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;


  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  yystate = yyn;
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
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

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
#line 763 "parse.y"
    {
			lex_state = EXPR_BEG;
		    /*%%%*/
			local_push(compile_for_eval);
		    /*%
		    %*/
		    ;}
    break;

  case 3:
#line 771 "parse.y"
    {
		    /*%%%*/
			if ((yyvsp[0].node) && !compile_for_eval) {
			    /* last expression should not be void */
			    if (nd_type((yyvsp[0].node)) != NODE_BLOCK) void_expr((yyvsp[0].node));
			    else {
				NODE *node = (yyvsp[0].node);
				while (node->nd_next) {
				    node = node->nd_next;
				}
				void_expr(node->nd_head);
			    }
			}
			ruby_eval_tree = NEW_SCOPE(0, block_append(ruby_eval_tree, (yyvsp[0].node)));
			local_pop();
		    /*%
			$$ = $2;
			parser->result = dispatch1(program, $$);
		    %*/
		    ;}
    break;

  case 4:
#line 797 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = (yyvsp[-3].node);
			if ((yyvsp[-2].node)) {
			    (yyval.node) = NEW_RESCUE((yyvsp[-3].node), (yyvsp[-2].node), (yyvsp[-1].node));
			}
			else if ((yyvsp[-1].node)) {
			    rb_warn0("else without rescue is useless");
			    (yyval.node) = block_append((yyval.node), (yyvsp[-1].node));
			}
			if ((yyvsp[0].node)) {
			    if ((yyval.node)) {
				(yyval.node) = NEW_ENSURE((yyval.node), (yyvsp[0].node));
			    }
			    else {
				(yyval.node) = block_append((yyvsp[0].node), NEW_NIL());
			    }
			}
			fixpos((yyval.node), (yyvsp[-3].node));
		    /*%
			$$ = dispatch4(body_stmt,
				       escape_Qundef($1),
				       escape_Qundef($2),
				       escape_Qundef($3),
				       escape_Qundef($4));
		    %*/
		    ;}
    break;

  case 5:
#line 827 "parse.y"
    {
		    /*%%%*/
			void_stmts((yyvsp[-1].node));
			fixup_nodes(&deferred_nodes);
		    /*%
		    %*/
			(yyval.node) = (yyvsp[-1].node);
		    ;}
    break;

  case 6:
#line 838 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = NEW_BEGIN(0);
		    /*%
			$$ = dispatch2(stmts_add, dispatch0(stmts_new),
						  dispatch0(void_stmt));
		    %*/
		    ;}
    break;

  case 7:
#line 847 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = newline_node((yyvsp[0].node));
		    /*%
			$$ = dispatch2(stmts_add, dispatch0(stmts_new), $1);
		    %*/
		    ;}
    break;

  case 8:
#line 855 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = block_append((yyvsp[-2].node), newline_node((yyvsp[0].node)));
		    /*%
			$$ = dispatch2(stmts_add, $1, $3);
		    %*/
		    ;}
    break;

  case 9:
#line 863 "parse.y"
    {
			(yyval.node) = remove_begin((yyvsp[0].node));
		    ;}
    break;

  case 10:
#line 868 "parse.y"
    {lex_state = EXPR_FNAME;;}
    break;

  case 11:
#line 869 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = NEW_ALIAS((yyvsp[-2].node), (yyvsp[0].node));
		    /*%
			$$ = dispatch2(alias, $2, $4);
		    %*/
		    ;}
    break;

  case 12:
#line 877 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = NEW_VALIAS((yyvsp[-1].id), (yyvsp[0].id));
		    /*%
			$$ = dispatch2(var_alias, $2, $3);
		    %*/
		    ;}
    break;

  case 13:
#line 885 "parse.y"
    {
		    /*%%%*/
			char buf[3];

			sprintf(buf, "$%c", (char)(yyvsp[0].node)->nd_nth);
			(yyval.node) = NEW_VALIAS((yyvsp[-1].id), rb_intern(buf));
		    /*%
			$$ = dispatch2(var_alias, $2, $3);
		    %*/
		    ;}
    break;

  case 14:
#line 896 "parse.y"
    {
		    /*%%%*/
			yyerror("can't make alias for the number variables");
			(yyval.node) = NEW_BEGIN(0);
		    /*%
			$$ = dispatch2(var_alias, $2, $3);
			$$ = dispatch1(alias_error, $$);
		    %*/
		    ;}
    break;

  case 15:
#line 906 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = (yyvsp[0].node);
		    /*%
			$$ = dispatch1(undef, $2);
		    %*/
		    ;}
    break;

  case 16:
#line 914 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = NEW_IF(cond((yyvsp[0].node)), remove_begin((yyvsp[-2].node)), 0);
			fixpos((yyval.node), (yyvsp[0].node));
		    /*%
			$$ = dispatch2(if_mod, $3, $1);
		    %*/
		    ;}
    break;

  case 17:
#line 923 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = NEW_UNLESS(cond((yyvsp[0].node)), remove_begin((yyvsp[-2].node)), 0);
			fixpos((yyval.node), (yyvsp[0].node));
		    /*%
			$$ = dispatch2(unless_mod, $3, $1);
		    %*/
		    ;}
    break;

  case 18:
#line 932 "parse.y"
    {
		    /*%%%*/
			if ((yyvsp[-2].node) && nd_type((yyvsp[-2].node)) == NODE_BEGIN) {
			    (yyval.node) = NEW_WHILE(cond((yyvsp[0].node)), (yyvsp[-2].node)->nd_body, 0);
			}
			else {
			    (yyval.node) = NEW_WHILE(cond((yyvsp[0].node)), (yyvsp[-2].node), 1);
			}
		    /*%
			$$ = dispatch2(while_mod, $3, $1);
		    %*/
		    ;}
    break;

  case 19:
#line 945 "parse.y"
    {
		    /*%%%*/
			if ((yyvsp[-2].node) && nd_type((yyvsp[-2].node)) == NODE_BEGIN) {
			    (yyval.node) = NEW_UNTIL(cond((yyvsp[0].node)), (yyvsp[-2].node)->nd_body, 0);
			}
			else {
			    (yyval.node) = NEW_UNTIL(cond((yyvsp[0].node)), (yyvsp[-2].node), 1);
			}
		    /*%
			$$ = dispatch2(until_mod, $3, $1);
		    %*/
		    ;}
    break;

  case 20:
#line 958 "parse.y"
    {
		    /*%%%*/
			NODE *resq = NEW_RESBODY(0, remove_begin((yyvsp[0].node)), 0);
			(yyval.node) = NEW_RESCUE(remove_begin((yyvsp[-2].node)), resq, 0);
		    /*%
			$$ = dispatch2(rescue_mod, $3, $1);
		    %*/
		    ;}
    break;

  case 21:
#line 967 "parse.y"
    {
			if (in_def || in_single) {
			    yyerror("BEGIN in method");
			}
		    /*%%%*/
			/* local_push(0); */
		    /*%
		    %*/
		    ;}
    break;

  case 22:
#line 977 "parse.y"
    {
		    /*%%%*/
			ruby_eval_tree_begin = block_append(ruby_eval_tree_begin,
							    (yyvsp[-1].node));
			/* NEW_PREEXE($4)); */
			/* local_pop(); */
			(yyval.node) = NEW_BEGIN(0);
		    /*%
			$$ = dispatch1(BEGIN, $4);
		    %*/
		    ;}
    break;

  case 23:
#line 989 "parse.y"
    {
			if (in_def || in_single) {
			    rb_warn0("END in method; use at_exit");
			}
		    /*%%%*/
			(yyval.node) = NEW_POSTEXE(NEW_NODE(
			    NODE_SCOPE, 0 /* tbl */, (yyvsp[-1].node) /* body */, 0 /* args */));
		    /*%
			$$ = dispatch1(END, $3);
		    %*/
		    ;}
    break;

  case 24:
#line 1001 "parse.y"
    {
		    /*%%%*/
			value_expr((yyvsp[0].node));
			(yyval.node) = node_assign((yyvsp[-2].node), (yyvsp[0].node));
		    /*%
			$$ = dispatch2(assign, $1, $3);
		    %*/
		    ;}
    break;

  case 25:
#line 1010 "parse.y"
    {
		    /*%%%*/
			value_expr((yyvsp[0].node));
			(yyvsp[-2].node)->nd_value = (yyvsp[0].node);
			(yyval.node) = (yyvsp[-2].node);
		    /*%
			$$ = dispatch2(massign, $1, $3);
		    %*/
		    ;}
    break;

  case 26:
#line 1020 "parse.y"
    {
		    /*%%%*/
			value_expr((yyvsp[0].node));
			if ((yyvsp[-2].node)) {
			    ID vid = (yyvsp[-2].node)->nd_vid;
			    if ((yyvsp[-1].id) == tOROP) {
				(yyvsp[-2].node)->nd_value = (yyvsp[0].node);
				(yyval.node) = NEW_OP_ASGN_OR(gettable(vid), (yyvsp[-2].node));
				if (is_asgn_or_id(vid)) {
				    (yyval.node)->nd_aid = vid;
				}
			    }
			    else if ((yyvsp[-1].id) == tANDOP) {
				(yyvsp[-2].node)->nd_value = (yyvsp[0].node);
				(yyval.node) = NEW_OP_ASGN_AND(gettable(vid), (yyvsp[-2].node));
			    }
			    else {
				(yyval.node) = (yyvsp[-2].node);
				(yyval.node)->nd_value = NEW_CALL(gettable(vid), (yyvsp[-1].id), NEW_LIST((yyvsp[0].node)));
			    }
			}
			else {
			    (yyval.node) = NEW_BEGIN(0);
			}
		    /*%
			$$ = dispatch3(opassign, $1, $2, $3);
		    %*/
		    ;}
    break;

  case 27:
#line 1049 "parse.y"
    {
		    /*%%%*/
			NODE *args;

			value_expr((yyvsp[0].node));
			if (!(yyvsp[-3].node)) (yyvsp[-3].node) = NEW_ZARRAY();
			args = arg_concat((yyvsp[0].node), (yyvsp[-3].node));
			if ((yyvsp[-1].id) == tOROP) {
			    (yyvsp[-1].id) = 0;
			}
			else if ((yyvsp[-1].id) == tANDOP) {
			    (yyvsp[-1].id) = 1;
			}
			(yyval.node) = NEW_OP_ASGN1((yyvsp[-5].node), (yyvsp[-1].id), args);
			fixpos((yyval.node), (yyvsp[-5].node));
		    /*%
			$$ = dispatch2(aref_field, $1, escape_Qundef($3));
			$$ = dispatch3(opassign, $$, $5, $6);
		    %*/
		    ;}
    break;

  case 28:
#line 1070 "parse.y"
    {
		    /*%%%*/
			value_expr((yyvsp[0].node));
			if ((yyvsp[-1].id) == tOROP) {
			    (yyvsp[-1].id) = 0;
			}
			else if ((yyvsp[-1].id) == tANDOP) {
			    (yyvsp[-1].id) = 1;
			}
			(yyval.node) = NEW_OP_ASGN2((yyvsp[-4].node), (yyvsp[-2].id), (yyvsp[-1].id), (yyvsp[0].node));
			fixpos((yyval.node), (yyvsp[-4].node));
		    /*%
			$$ = dispatch3(field, $1, ripper_id2sym('.'), $3);
			$$ = dispatch3(opassign, $$, $4, $5);
		    %*/
		    ;}
    break;

  case 29:
#line 1087 "parse.y"
    {
		    /*%%%*/
			value_expr((yyvsp[0].node));
			if ((yyvsp[-1].id) == tOROP) {
			    (yyvsp[-1].id) = 0;
			}
			else if ((yyvsp[-1].id) == tANDOP) {
			    (yyvsp[-1].id) = 1;
			}
			(yyval.node) = NEW_OP_ASGN2((yyvsp[-4].node), (yyvsp[-2].id), (yyvsp[-1].id), (yyvsp[0].node));
			fixpos((yyval.node), (yyvsp[-4].node));
		    /*%
			$$ = dispatch3(field, $1, ripper_id2sym('.'), $3);
			$$ = dispatch3(opassign, $$, $4, $5);
		    %*/
		    ;}
    break;

  case 30:
#line 1104 "parse.y"
    {
		    /*%%%*/
			value_expr((yyvsp[0].node));
			if ((yyvsp[-1].id) == tOROP) {
			    (yyvsp[-1].id) = 0;
			}
			else if ((yyvsp[-1].id) == tANDOP) {
			    (yyvsp[-1].id) = 1;
			}
			(yyval.node) = NEW_OP_ASGN2((yyvsp[-4].node), (yyvsp[-2].id), (yyvsp[-1].id), (yyvsp[0].node));
			fixpos((yyval.node), (yyvsp[-4].node));
		    /*%
			$$ = dispatch3(field, $1, ripper_intern("::"), $3);
			$$ = dispatch3(opassign, $$, $4, $5);
		    %*/
		    ;}
    break;

  case 31:
#line 1121 "parse.y"
    {
		    /*%%%*/
			rb_backref_error((yyvsp[-2].node));
			(yyval.node) = NEW_BEGIN(0);
		    /*%
			$$ = dispatch2(assign, dispatch1(var_field, $1), $3);
			$$ = dispatch1(assign_error, $$);
		    %*/
		    ;}
    break;

  case 32:
#line 1131 "parse.y"
    {
		    /*%%%*/
			value_expr((yyvsp[0].node));
			(yyval.node) = node_assign((yyvsp[-2].node), (yyvsp[0].node));
		    /*%
			$$ = dispatch2(assign, $1, $3);
		    %*/
		    ;}
    break;

  case 33:
#line 1140 "parse.y"
    {
		    /*%%%*/
			(yyvsp[-2].node)->nd_value = (yyvsp[0].node);
			(yyval.node) = (yyvsp[-2].node);
		    /*%
			dispatch2(massign, $1, $3);
		    %*/
		    ;}
    break;

  case 34:
#line 1149 "parse.y"
    {
		    /*%%%*/
			(yyvsp[-2].node)->nd_value = (yyvsp[0].node);
			(yyval.node) = (yyvsp[-2].node);
		    /*%
			$$ = dispatch2(massign, $1, $3);
		    %*/
		    ;}
    break;

  case 37:
#line 1162 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = logop(NODE_AND, (yyvsp[-2].node), (yyvsp[0].node));
		    /*%
			$$ = dispatch3(binary, $1, ripper_intern("and"), $3);
		    %*/
		    ;}
    break;

  case 38:
#line 1170 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = logop(NODE_OR, (yyvsp[-2].node), (yyvsp[0].node));
		    /*%
			$$ = dispatch3(binary, $1, ripper_intern("or"), $3);
		    %*/
		    ;}
    break;

  case 39:
#line 1178 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = call_uni_op(cond((yyvsp[0].node)), '!');
		    /*%
			$$ = dispatch2(unary, ripper_intern("not"), $2);
		    %*/
		    ;}
    break;

  case 40:
#line 1186 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = call_uni_op(cond((yyvsp[0].node)), '!');
		    /*%
			$$ = dispatch2(unary, ripper_id2sym('!'), $2);
		    %*/
		    ;}
    break;

  case 42:
#line 1197 "parse.y"
    {
		    /*%%%*/
			value_expr((yyvsp[0].node));
			(yyval.node) = (yyvsp[0].node);
		        if (!(yyval.node)) (yyval.node) = NEW_NIL();
		    /*%
			$$ = $1;
		    %*/
		    ;}
    break;

  case 45:
#line 1211 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = NEW_RETURN(ret_args((yyvsp[0].node)));
		    /*%
			$$ = dispatch1(return, $2);
		    %*/
		    ;}
    break;

  case 46:
#line 1219 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = NEW_BREAK(ret_args((yyvsp[0].node)));
		    /*%
			$$ = dispatch1(break, $2);
		    %*/
		    ;}
    break;

  case 47:
#line 1227 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = NEW_NEXT(ret_args((yyvsp[0].node)));
		    /*%
			$$ = dispatch1(next, $2);
		    %*/
		    ;}
    break;

  case 49:
#line 1238 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = NEW_CALL((yyvsp[-3].node), (yyvsp[-1].id), (yyvsp[0].node));
		    /*%
			$$ = dispatch3(call, $1, ripper_id2sym('.'), $3);
			$$ = method_arg($$, $4);
		    %*/
		    ;}
    break;

  case 50:
#line 1247 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = NEW_CALL((yyvsp[-3].node), (yyvsp[-1].id), (yyvsp[0].node));
		    /*%
			$$ = dispatch3(call, $1, ripper_intern("::"), $3);
			$$ = method_arg($$, $4);
		    %*/
		    ;}
    break;

  case 51:
#line 1258 "parse.y"
    {
		    /*%%%*/
			dyna_push();
			(yyval.num) = ruby_sourceline;
		    /*%
		    %*/
		    ;}
    break;

  case 52:
#line 1268 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = NEW_ITER((yyvsp[-2].node),(yyvsp[-1].node));
			nd_set_line((yyval.node), (yyvsp[-3].num));
			dyna_pop();
		    /*%
			$$ = dispatch2(brace_block, escape_Qundef($3), $4);
		    %*/
		    ;}
    break;

  case 53:
#line 1280 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = NEW_FCALL((yyvsp[-1].id), (yyvsp[0].node));
			fixpos((yyval.node), (yyvsp[0].node));
		    /*%
			$$ = dispatch2(command, $1, $2);
		    %*/
		    ;}
    break;

  case 54:
#line 1289 "parse.y"
    {
		    /*%%%*/
			block_dup_check((yyvsp[-1].node),(yyvsp[0].node));
		        (yyvsp[0].node)->nd_iter = NEW_FCALL((yyvsp[-2].id), (yyvsp[-1].node));
			(yyval.node) = (yyvsp[0].node);
			fixpos((yyval.node), (yyvsp[-1].node));
		    /*%
			$$ = dispatch2(command, $1, $2);
			$$ = method_add_block($$, $3);
		    %*/
		    ;}
    break;

  case 55:
#line 1301 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = NEW_CALL((yyvsp[-3].node), (yyvsp[-1].id), (yyvsp[0].node));
			fixpos((yyval.node), (yyvsp[-3].node));
		    /*%
			$$ = dispatch4(command_call, $1, ripper_id2sym('.'), $3, $4);
		    %*/
		    ;}
    break;

  case 56:
#line 1310 "parse.y"
    {
		    /*%%%*/
			block_dup_check((yyvsp[-1].node),(yyvsp[0].node));
		        (yyvsp[0].node)->nd_iter = NEW_CALL((yyvsp[-4].node), (yyvsp[-2].id), (yyvsp[-1].node));
			(yyval.node) = (yyvsp[0].node);
			fixpos((yyval.node), (yyvsp[-4].node));
		    /*%
			$$ = dispatch4(command_call, $1, ripper_id2sym('.'), $3, $4);
			$$ = method_add_block($$, $5);
		    %*/
		   ;}
    break;

  case 57:
#line 1322 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = NEW_CALL((yyvsp[-3].node), (yyvsp[-1].id), (yyvsp[0].node));
			fixpos((yyval.node), (yyvsp[-3].node));
		    /*%
			$$ = dispatch4(command_call, $1, ripper_intern("::"), $3, $4);
		    %*/
		    ;}
    break;

  case 58:
#line 1331 "parse.y"
    {
		    /*%%%*/
			block_dup_check((yyvsp[-1].node),(yyvsp[0].node));
		        (yyvsp[0].node)->nd_iter = NEW_CALL((yyvsp[-4].node), (yyvsp[-2].id), (yyvsp[-1].node));
			(yyval.node) = (yyvsp[0].node);
			fixpos((yyval.node), (yyvsp[-4].node));
		    /*%
			$$ = dispatch4(command_call, $1, ripper_intern("::"), $3, $4);
			$$ = method_add_block($$, $5);
		    %*/
		   ;}
    break;

  case 59:
#line 1343 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = NEW_SUPER((yyvsp[0].node));
			fixpos((yyval.node), (yyvsp[0].node));
		    /*%
			$$ = dispatch1(super, $2);
		    %*/
		    ;}
    break;

  case 60:
#line 1352 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = new_yield((yyvsp[0].node));
			fixpos((yyval.node), (yyvsp[0].node));
		    /*%
			$$ = dispatch1(yield, $2);
		    %*/
		    ;}
    break;

  case 62:
#line 1364 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = (yyvsp[-1].node);
		    /*%
			$$ = dispatch1(mlhs_paren, $2);
		    %*/
		    ;}
    break;

  case 64:
#line 1375 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = NEW_MASGN(NEW_LIST((yyvsp[-1].node)), 0);
		    /*%
			$$ = dispatch1(mlhs_paren, $2);
		    %*/
		    ;}
    break;

  case 65:
#line 1385 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = NEW_MASGN((yyvsp[0].node), 0);
		    /*%
			$$ = $1;
		    %*/
		    ;}
    break;

  case 66:
#line 1393 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = NEW_MASGN(list_append((yyvsp[-1].node),(yyvsp[0].node)), 0);
		    /*%
			$$ = mlhs_add($1, $2);
		    %*/
		    ;}
    break;

  case 67:
#line 1401 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = NEW_MASGN((yyvsp[-2].node), (yyvsp[0].node));
		    /*%
			$$ = mlhs_add_star($1, $3);
		    %*/
		    ;}
    break;

  case 68:
#line 1409 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = NEW_MASGN((yyvsp[-4].node), NEW_POSTARG((yyvsp[-2].node),(yyvsp[0].node)));
		    /*%
			$$ = mlhs_add_star($1, $3);
		    %*/
		    ;}
    break;

  case 69:
#line 1417 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = NEW_MASGN((yyvsp[-1].node), -1);
		    /*%
			$$ = mlhs_add_star($1, Qnil);
		    %*/
		    ;}
    break;

  case 70:
#line 1425 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = NEW_MASGN((yyvsp[-3].node), NEW_POSTARG(-1, (yyvsp[0].node)));
		    /*%
			$$ = mlhs_add_star($1, Qnil);
		    %*/
		    ;}
    break;

  case 71:
#line 1433 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = NEW_MASGN(0, (yyvsp[0].node));
		    /*%
			$$ = mlhs_add_star(mlhs_new(), $2);
		    %*/
		    ;}
    break;

  case 72:
#line 1441 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = NEW_MASGN(0, NEW_POSTARG((yyvsp[-2].node),(yyvsp[0].node)));
		    /*%
			$$ = mlhs_add_star(mlhs_new(), $2);
		    %*/
		    ;}
    break;

  case 73:
#line 1449 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = NEW_MASGN(0, -1);
		    /*%
			$$ = mlhs_add_star(mlhs_new(), Qnil);
		    %*/
		    ;}
    break;

  case 74:
#line 1457 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = NEW_MASGN(0, NEW_POSTARG(-1, (yyvsp[0].node)));
		    /*%
			$$ = mlhs_add_star(mlhs_new(), Qnil);
		    %*/
		    ;}
    break;

  case 76:
#line 1468 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = (yyvsp[-1].node);
		    /*%
			$$ = dispatch1(mlhs_paren, $2);
		    %*/
		    ;}
    break;

  case 77:
#line 1478 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = NEW_LIST((yyvsp[-1].node));
		    /*%
			$$ = mlhs_add(mlhs_new(), $1);
		    %*/
		    ;}
    break;

  case 78:
#line 1486 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = list_append((yyvsp[-2].node), (yyvsp[-1].node));
		    /*%
			$$ = mlhs_add($1, $2);
		    %*/
		    ;}
    break;

  case 79:
#line 1496 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = NEW_LIST((yyvsp[0].node));
		    /*%
			$$ = mlhs_add(mlhs_new(), $1);
		    %*/
		    ;}
    break;

  case 80:
#line 1504 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = list_append((yyvsp[-2].node), (yyvsp[0].node));
		    /*%
			$$ = mlhs_add($1, $3);
		    %*/
		    ;}
    break;

  case 81:
#line 1514 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = assignable((yyvsp[0].id), 0);
		    /*%
			$$ = $1;
		    %*/
		    ;}
    break;

  case 82:
#line 1522 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = aryset((yyvsp[-3].node), (yyvsp[-1].node));
		    /*%
			$$ = dispatch2(aref_field, $1, escape_Qundef($3));
		    %*/
		    ;}
    break;

  case 83:
#line 1530 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = attrset((yyvsp[-2].node), (yyvsp[0].id));
		    /*%
			$$ = dispatch3(field, $1, ripper_id2sym('.'), $3);
		    %*/
		    ;}
    break;

  case 84:
#line 1538 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = attrset((yyvsp[-2].node), (yyvsp[0].id));
		    /*%
			$$ = dispatch2(const_path_field, $1, $3);
		    %*/
		    ;}
    break;

  case 85:
#line 1546 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = attrset((yyvsp[-2].node), (yyvsp[0].id));
		    /*%
			$$ = dispatch3(field, $1, ripper_id2sym('.'), $3);
		    %*/
		    ;}
    break;

  case 86:
#line 1554 "parse.y"
    {
		    /*%%%*/
			if (in_def || in_single)
			    yyerror("dynamic constant assignment");
			(yyval.node) = NEW_CDECL(0, 0, NEW_COLON2((yyvsp[-2].node), (yyvsp[0].id)));
		    /*%
			if (in_def || in_single)
			    yyerror("dynamic constant assignment");
			$$ = dispatch2(const_path_field, $1, $3);
		    %*/
		    ;}
    break;

  case 87:
#line 1566 "parse.y"
    {
		    /*%%%*/
			if (in_def || in_single)
			    yyerror("dynamic constant assignment");
			(yyval.node) = NEW_CDECL(0, 0, NEW_COLON3((yyvsp[0].id)));
		    /*%
			$$ = dispatch1(top_const_field, $2);
		    %*/
		    ;}
    break;

  case 88:
#line 1576 "parse.y"
    {
		    /*%%%*/
			rb_backref_error((yyvsp[0].node));
			(yyval.node) = NEW_BEGIN(0);
		    /*%
			$$ = dispatch1(var_field, $1);
			$$ = dispatch1(assign_error, $$);
		    %*/
		    ;}
    break;

  case 89:
#line 1588 "parse.y"
    {
		    /*%%%*/
			if (!((yyval.node) = assignable((yyvsp[0].id), 0))) (yyval.node) = NEW_BEGIN(0);
		    /*%
			$$ = dispatch1(var_field, $1);
		    %*/
		    ;}
    break;

  case 90:
#line 1596 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = aryset((yyvsp[-3].node), (yyvsp[-1].node));
		    /*%
			$$ = dispatch2(aref_field, $1, escape_Qundef($3));
		    %*/
		    ;}
    break;

  case 91:
#line 1604 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = attrset((yyvsp[-2].node), (yyvsp[0].id));
		    /*%
			$$ = dispatch3(field, $1, ripper_id2sym('.'), $3);
		    %*/
		    ;}
    break;

  case 92:
#line 1612 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = attrset((yyvsp[-2].node), (yyvsp[0].id));
		    /*%
			$$ = dispatch3(field, $1, ripper_intern("::"), $3);
		    %*/
		    ;}
    break;

  case 93:
#line 1620 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = attrset((yyvsp[-2].node), (yyvsp[0].id));
		    /*%
			$$ = dispatch3(field, $1, ripper_id2sym('.'), $3);
		    %*/
		    ;}
    break;

  case 94:
#line 1628 "parse.y"
    {
		    /*%%%*/
			if (in_def || in_single)
			    yyerror("dynamic constant assignment");
			(yyval.node) = NEW_CDECL(0, 0, NEW_COLON2((yyvsp[-2].node), (yyvsp[0].id)));
		    /*%
			$$ = dispatch2(const_path_field, $1, $3);
			if (in_def || in_single) {
			    $$ = dispatch1(assign_error, $$);
			}
		    %*/
		    ;}
    break;

  case 95:
#line 1641 "parse.y"
    {
		    /*%%%*/
			if (in_def || in_single)
			    yyerror("dynamic constant assignment");
			(yyval.node) = NEW_CDECL(0, 0, NEW_COLON3((yyvsp[0].id)));
		    /*%
			$$ = dispatch1(top_const_field, $2);
			if (in_def || in_single) {
			    $$ = dispatch1(assign_error, $$);
			}
		    %*/
		    ;}
    break;

  case 96:
#line 1654 "parse.y"
    {
		    /*%%%*/
			rb_backref_error((yyvsp[0].node));
			(yyval.node) = NEW_BEGIN(0);
		    /*%
			$$ = dispatch1(assign_error, $1);
		    %*/
		    ;}
    break;

  case 97:
#line 1665 "parse.y"
    {
		    /*%%%*/
			yyerror("class/module name must be CONSTANT");
		    /*%
			$$ = dispatch1(class_name_error, $1);
		    %*/
		    ;}
    break;

  case 99:
#line 1676 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = NEW_COLON3((yyvsp[0].id));
		    /*%
			$$ = dispatch1(top_const_ref, $2);
		    %*/
		    ;}
    break;

  case 100:
#line 1684 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = NEW_COLON2(0, (yyval.node));
		    /*%
			$$ = dispatch1(const_ref, $1);
		    %*/
		    ;}
    break;

  case 101:
#line 1692 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = NEW_COLON2((yyvsp[-2].node), (yyvsp[0].id));
		    /*%
			$$ = dispatch2(const_path_ref, $1, $3);
		    %*/
		    ;}
    break;

  case 105:
#line 1705 "parse.y"
    {
		    /*%%%*/
			lex_state = EXPR_END;
			(yyval.id) = (yyvsp[0].id);
		    /*%
			lex_state = EXPR_END;
			$$ = $1;
		    %*/
		    ;}
    break;

  case 106:
#line 1715 "parse.y"
    {
		    /*%%%*/
			lex_state = EXPR_END;
			(yyval.id) = (yyvsp[0].id);
		    /*%
			lex_state = EXPR_END;
			$$ = $1;
		    %*/
		    ;}
    break;

  case 109:
#line 1731 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = NEW_LIT(ID2SYM((yyvsp[0].id)));
		    /*%
			$$ = dispatch1(symbol_literal, $1);
		    %*/
		    ;}
    break;

  case 111:
#line 1742 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = NEW_UNDEF((yyvsp[0].node));
		    /*%
			$$ = rb_ary_new3(1, $1);
		    %*/
		    ;}
    break;

  case 112:
#line 1749 "parse.y"
    {lex_state = EXPR_FNAME;;}
    break;

  case 113:
#line 1750 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = block_append((yyvsp[-3].node), NEW_UNDEF((yyvsp[0].node)));
		    /*%
			rb_ary_push($1, $4);
		    %*/
		    ;}
    break;

  case 114:
#line 1759 "parse.y"
    { ifndef_ripper((yyval.id) = '|'); ;}
    break;

  case 115:
#line 1760 "parse.y"
    { ifndef_ripper((yyval.id) = '^'); ;}
    break;

  case 116:
#line 1761 "parse.y"
    { ifndef_ripper((yyval.id) = '&'); ;}
    break;

  case 117:
#line 1762 "parse.y"
    { ifndef_ripper((yyval.id) = tCMP); ;}
    break;

  case 118:
#line 1763 "parse.y"
    { ifndef_ripper((yyval.id) = tEQ); ;}
    break;

  case 119:
#line 1764 "parse.y"
    { ifndef_ripper((yyval.id) = tEQQ); ;}
    break;

  case 120:
#line 1765 "parse.y"
    { ifndef_ripper((yyval.id) = tMATCH); ;}
    break;

  case 121:
#line 1766 "parse.y"
    { ifndef_ripper((yyval.id) = tNMATCH); ;}
    break;

  case 122:
#line 1767 "parse.y"
    { ifndef_ripper((yyval.id) = '>'); ;}
    break;

  case 123:
#line 1768 "parse.y"
    { ifndef_ripper((yyval.id) = tGEQ); ;}
    break;

  case 124:
#line 1769 "parse.y"
    { ifndef_ripper((yyval.id) = '<'); ;}
    break;

  case 125:
#line 1770 "parse.y"
    { ifndef_ripper((yyval.id) = tLEQ); ;}
    break;

  case 126:
#line 1771 "parse.y"
    { ifndef_ripper((yyval.id) = tNEQ); ;}
    break;

  case 127:
#line 1772 "parse.y"
    { ifndef_ripper((yyval.id) = tLSHFT); ;}
    break;

  case 128:
#line 1773 "parse.y"
    { ifndef_ripper((yyval.id) = tRSHFT); ;}
    break;

  case 129:
#line 1774 "parse.y"
    { ifndef_ripper((yyval.id) = '+'); ;}
    break;

  case 130:
#line 1775 "parse.y"
    { ifndef_ripper((yyval.id) = '-'); ;}
    break;

  case 131:
#line 1776 "parse.y"
    { ifndef_ripper((yyval.id) = '*'); ;}
    break;

  case 132:
#line 1777 "parse.y"
    { ifndef_ripper((yyval.id) = '*'); ;}
    break;

  case 133:
#line 1778 "parse.y"
    { ifndef_ripper((yyval.id) = '/'); ;}
    break;

  case 134:
#line 1779 "parse.y"
    { ifndef_ripper((yyval.id) = '%'); ;}
    break;

  case 135:
#line 1780 "parse.y"
    { ifndef_ripper((yyval.id) = tPOW); ;}
    break;

  case 136:
#line 1781 "parse.y"
    { ifndef_ripper((yyval.id) = '!'); ;}
    break;

  case 137:
#line 1782 "parse.y"
    { ifndef_ripper((yyval.id) = '~'); ;}
    break;

  case 138:
#line 1783 "parse.y"
    { ifndef_ripper((yyval.id) = tUPLUS); ;}
    break;

  case 139:
#line 1784 "parse.y"
    { ifndef_ripper((yyval.id) = tUMINUS); ;}
    break;

  case 140:
#line 1785 "parse.y"
    { ifndef_ripper((yyval.id) = tAREF); ;}
    break;

  case 141:
#line 1786 "parse.y"
    { ifndef_ripper((yyval.id) = tASET); ;}
    break;

  case 142:
#line 1787 "parse.y"
    { ifndef_ripper((yyval.id) = '`'); ;}
    break;

  case 184:
#line 1805 "parse.y"
    {
		    /*%%%*/
			value_expr((yyvsp[0].node));
			(yyval.node) = node_assign((yyvsp[-2].node), (yyvsp[0].node));
		    /*%
			$$ = dispatch2(assign, $1, $3);
		    %*/
		    ;}
    break;

  case 185:
#line 1814 "parse.y"
    {
		    /*%%%*/
			value_expr((yyvsp[-2].node));
		        (yyvsp[-2].node) = NEW_RESCUE((yyvsp[-2].node), NEW_RESBODY(0,(yyvsp[0].node),0), 0);
			(yyval.node) = node_assign((yyvsp[-4].node), (yyvsp[-2].node));
		    /*%
			$$ = dispatch2(assign, $1, dispatch2(rescue_mod, $3, $5));
		    %*/
		    ;}
    break;

  case 186:
#line 1824 "parse.y"
    {
		    /*%%%*/
			value_expr((yyvsp[0].node));
			if ((yyvsp[-2].node)) {
			    ID vid = (yyvsp[-2].node)->nd_vid;
			    if ((yyvsp[-1].id) == tOROP) {
				(yyvsp[-2].node)->nd_value = (yyvsp[0].node);
				(yyval.node) = NEW_OP_ASGN_OR(gettable(vid), (yyvsp[-2].node));
				if (is_asgn_or_id(vid)) {
				    (yyval.node)->nd_aid = vid;
				}
			    }
			    else if ((yyvsp[-1].id) == tANDOP) {
				(yyvsp[-2].node)->nd_value = (yyvsp[0].node);
				(yyval.node) = NEW_OP_ASGN_AND(gettable(vid), (yyvsp[-2].node));
			    }
			    else {
				(yyval.node) = (yyvsp[-2].node);
				(yyval.node)->nd_value = NEW_CALL(gettable(vid), (yyvsp[-1].id), NEW_LIST((yyvsp[0].node)));
			    }
			}
			else {
			    (yyval.node) = NEW_BEGIN(0);
			}
		    /*%
			$$ = dispatch3(opassign, $1, $2, $3);
		    %*/
		    ;}
    break;

  case 187:
#line 1853 "parse.y"
    {
		    /*%%%*/
			value_expr((yyvsp[-2].node));
		        (yyvsp[-2].node) = NEW_RESCUE((yyvsp[-2].node), NEW_RESBODY(0,(yyvsp[0].node),0), 0);
			if ((yyvsp[-4].node)) {
			    ID vid = (yyvsp[-4].node)->nd_vid;
			    if ((yyvsp[-3].id) == tOROP) {
				(yyvsp[-4].node)->nd_value = (yyvsp[-2].node);
				(yyval.node) = NEW_OP_ASGN_OR(gettable(vid), (yyvsp[-4].node));
				if (is_asgn_or_id(vid)) {
				    (yyval.node)->nd_aid = vid;
				}
			    }
			    else if ((yyvsp[-3].id) == tANDOP) {
				(yyvsp[-4].node)->nd_value = (yyvsp[-2].node);
				(yyval.node) = NEW_OP_ASGN_AND(gettable(vid), (yyvsp[-4].node));
			    }
			    else {
				(yyval.node) = (yyvsp[-4].node);
				(yyval.node)->nd_value = NEW_CALL(gettable(vid), (yyvsp[-3].id), NEW_LIST((yyvsp[-2].node)));
			    }
			}
			else {
			    (yyval.node) = NEW_BEGIN(0);
			}
		    /*%
			$3 = dispatch2(rescue_mod, $3, $5);
			$$ = dispatch3(opassign, $1, $2, $3);
		    %*/
		    ;}
    break;

  case 188:
#line 1884 "parse.y"
    {
		    /*%%%*/
			NODE *args;

			value_expr((yyvsp[0].node));
			if (!(yyvsp[-3].node)) (yyvsp[-3].node) = NEW_ZARRAY();
			args = arg_concat((yyvsp[0].node), (yyvsp[-3].node));
			if ((yyvsp[-1].id) == tOROP) {
			    (yyvsp[-1].id) = 0;
			}
			else if ((yyvsp[-1].id) == tANDOP) {
			    (yyvsp[-1].id) = 1;
			}
			(yyval.node) = NEW_OP_ASGN1((yyvsp[-5].node), (yyvsp[-1].id), args);
			fixpos((yyval.node), (yyvsp[-5].node));
		    /*%
			$1 = dispatch2(aref_field, $1, escape_Qundef($3));
			$$ = dispatch3(opassign, $1, $5, $6);
		    %*/
		    ;}
    break;

  case 189:
#line 1905 "parse.y"
    {
		    /*%%%*/
			value_expr((yyvsp[0].node));
			if ((yyvsp[-1].id) == tOROP) {
			    (yyvsp[-1].id) = 0;
			}
			else if ((yyvsp[-1].id) == tANDOP) {
			    (yyvsp[-1].id) = 1;
			}
			(yyval.node) = NEW_OP_ASGN2((yyvsp[-4].node), (yyvsp[-2].id), (yyvsp[-1].id), (yyvsp[0].node));
			fixpos((yyval.node), (yyvsp[-4].node));
		    /*%
			$1 = dispatch3(field, $1, ripper_id2sym('.'), $3);
			$$ = dispatch3(opassign, $1, $4, $5);
		    %*/
		    ;}
    break;

  case 190:
#line 1922 "parse.y"
    {
		    /*%%%*/
			value_expr((yyvsp[0].node));
			if ((yyvsp[-1].id) == tOROP) {
			    (yyvsp[-1].id) = 0;
			}
			else if ((yyvsp[-1].id) == tANDOP) {
			    (yyvsp[-1].id) = 1;
			}
			(yyval.node) = NEW_OP_ASGN2((yyvsp[-4].node), (yyvsp[-2].id), (yyvsp[-1].id), (yyvsp[0].node));
			fixpos((yyval.node), (yyvsp[-4].node));
		    /*%
			$1 = dispatch3(field, $1, ripper_id2sym('.'), $3);
			$$ = dispatch3(opassign, $1, $4, $5);
		    %*/
		    ;}
    break;

  case 191:
#line 1939 "parse.y"
    {
		    /*%%%*/
			value_expr((yyvsp[0].node));
			if ((yyvsp[-1].id) == tOROP) {
			    (yyvsp[-1].id) = 0;
			}
			else if ((yyvsp[-1].id) == tANDOP) {
			    (yyvsp[-1].id) = 1;
			}
			(yyval.node) = NEW_OP_ASGN2((yyvsp[-4].node), (yyvsp[-2].id), (yyvsp[-1].id), (yyvsp[0].node));
			fixpos((yyval.node), (yyvsp[-4].node));
		    /*%
			$1 = dispatch3(field, $1, ripper_intern("::"), $3);
			$$ = dispatch3(opassign, $1, $4, $5);
		    %*/
		    ;}
    break;

  case 192:
#line 1956 "parse.y"
    {
		    /*%%%*/
			yyerror("constant re-assignment");
			(yyval.node) = NEW_BEGIN(0);
		    /*%
			$$ = dispatch2(const_path_field, $1, $3);
			$$ = dispatch3(opassign, $$, $4, $5);
			$$ = dispatch1(assign_error, $$);
		    %*/
		    ;}
    break;

  case 193:
#line 1967 "parse.y"
    {
		    /*%%%*/
			yyerror("constant re-assignment");
			(yyval.node) = NEW_BEGIN(0);
		    /*%
			$$ = dispatch1(top_const_field, $2);
			$$ = dispatch3(opassign, $$, $3, $4);
			$$ = dispatch1(assign_error, $$);
		    %*/
		    ;}
    break;

  case 194:
#line 1978 "parse.y"
    {
		    /*%%%*/
			rb_backref_error((yyvsp[-2].node));
			(yyval.node) = NEW_BEGIN(0);
		    /*%
			$$ = dispatch1(var_field, $1);
			$$ = dispatch3(opassign, $$, $2, $3);
			$$ = dispatch1(assign_error, $$);
		    %*/
		    ;}
    break;

  case 195:
#line 1989 "parse.y"
    {
		    /*%%%*/
			value_expr((yyvsp[-2].node));
			value_expr((yyvsp[0].node));
			(yyval.node) = NEW_DOT2((yyvsp[-2].node), (yyvsp[0].node));
			if (nd_type((yyvsp[-2].node)) == NODE_LIT && FIXNUM_P((yyvsp[-2].node)->nd_lit) &&
			    nd_type((yyvsp[0].node)) == NODE_LIT && FIXNUM_P((yyvsp[0].node)->nd_lit)) {
			    deferred_nodes = list_append(deferred_nodes, (yyval.node));
			}
		    /*%
			$$ = dispatch2(dot2, $1, $3);
		    %*/
		    ;}
    break;

  case 196:
#line 2003 "parse.y"
    {
		    /*%%%*/
			value_expr((yyvsp[-2].node));
			value_expr((yyvsp[0].node));
			(yyval.node) = NEW_DOT3((yyvsp[-2].node), (yyvsp[0].node));
			if (nd_type((yyvsp[-2].node)) == NODE_LIT && FIXNUM_P((yyvsp[-2].node)->nd_lit) &&
			    nd_type((yyvsp[0].node)) == NODE_LIT && FIXNUM_P((yyvsp[0].node)->nd_lit)) {
			    deferred_nodes = list_append(deferred_nodes, (yyval.node));
			}
		    /*%
			$$ = dispatch2(dot3, $1, $3);
		    %*/
		    ;}
    break;

  case 197:
#line 2017 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = call_bin_op((yyvsp[-2].node), '+', (yyvsp[0].node));
		    /*%
			$$ = dispatch3(binary, $1, ID2SYM('+'), $3);
		    %*/
		    ;}
    break;

  case 198:
#line 2025 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = call_bin_op((yyvsp[-2].node), '-', (yyvsp[0].node));
		    /*%
			$$ = dispatch3(binary, $1, ID2SYM('-'), $3);
		    %*/
		    ;}
    break;

  case 199:
#line 2033 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = call_bin_op((yyvsp[-2].node), '*', (yyvsp[0].node));
		    /*%
			$$ = dispatch3(binary, $1, ID2SYM('*'), $3);
		    %*/
		    ;}
    break;

  case 200:
#line 2041 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = call_bin_op((yyvsp[-2].node), '/', (yyvsp[0].node));
		    /*%
			$$ = dispatch3(binary, $1, ID2SYM('/'), $3);
		    %*/
		    ;}
    break;

  case 201:
#line 2049 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = call_bin_op((yyvsp[-2].node), '%', (yyvsp[0].node));
		    /*%
			$$ = dispatch3(binary, $1, ID2SYM('%'), $3);
		    %*/
		    ;}
    break;

  case 202:
#line 2057 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = call_bin_op((yyvsp[-2].node), tPOW, (yyvsp[0].node));
		    /*%
			$$ = dispatch3(binary, $1, ripper_intern("**"), $3);
		    %*/
		    ;}
    break;

  case 203:
#line 2065 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = NEW_CALL(call_bin_op((yyvsp[-2].node), tPOW, (yyvsp[0].node)), tUMINUS, 0);
		    /*%
			$$ = dispatch3(binary, $2, ripper_intern("**"), $4);
			$$ = dispatch2(unary, ripper_intern("-@"), $$);
		    %*/
		    ;}
    break;

  case 204:
#line 2074 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = NEW_CALL(call_bin_op((yyvsp[-2].node), tPOW, (yyvsp[0].node)), tUMINUS, 0);
		    /*%
			$$ = dispatch3(binary, $2, ripper_intern("**"), $4);
			$$ = dispatch2(unary, ripper_intern("-@"), $$);
		    %*/
		    ;}
    break;

  case 205:
#line 2083 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = call_uni_op((yyvsp[0].node), tUPLUS);
		    /*%
			$$ = dispatch2(unary, ripper_intern("+@"), $2);
		    %*/
		    ;}
    break;

  case 206:
#line 2091 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = call_uni_op((yyvsp[0].node), tUMINUS);
		    /*%
			$$ = dispatch2(unary, ripper_intern("-@"), $2);
		    %*/
		    ;}
    break;

  case 207:
#line 2099 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = call_bin_op((yyvsp[-2].node), '|', (yyvsp[0].node));
		    /*%
			$$ = dispatch3(binary, $1, ID2SYM('|'), $3);
		    %*/
		    ;}
    break;

  case 208:
#line 2107 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = call_bin_op((yyvsp[-2].node), '^', (yyvsp[0].node));
		    /*%
			$$ = dispatch3(binary, $1, ID2SYM('^'), $3);
		    %*/
		    ;}
    break;

  case 209:
#line 2115 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = call_bin_op((yyvsp[-2].node), '&', (yyvsp[0].node));
		    /*%
			$$ = dispatch3(binary, $1, ID2SYM('&'), $3);
		    %*/
		    ;}
    break;

  case 210:
#line 2123 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = call_bin_op((yyvsp[-2].node), tCMP, (yyvsp[0].node));
		    /*%
			$$ = dispatch3(binary, $1, ripper_intern("<=>"), $3);
		    %*/
		    ;}
    break;

  case 211:
#line 2131 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = call_bin_op((yyvsp[-2].node), '>', (yyvsp[0].node));
		    /*%
			$$ = dispatch3(binary, $1, ID2SYM('>'), $3);
		    %*/
		    ;}
    break;

  case 212:
#line 2139 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = call_bin_op((yyvsp[-2].node), tGEQ, (yyvsp[0].node));
		    /*%
			$$ = dispatch3(binary, $1, ripper_intern(">="), $3);
		    %*/
		    ;}
    break;

  case 213:
#line 2147 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = call_bin_op((yyvsp[-2].node), '<', (yyvsp[0].node));
		    /*%
			$$ = dispatch3(binary, $1, ID2SYM('<'), $3);
		    %*/
		    ;}
    break;

  case 214:
#line 2155 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = call_bin_op((yyvsp[-2].node), tLEQ, (yyvsp[0].node));
		    /*%
			$$ = dispatch3(binary, $1, ripper_intern("<="), $3);
		    %*/
		    ;}
    break;

  case 215:
#line 2163 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = call_bin_op((yyvsp[-2].node), tEQ, (yyvsp[0].node));
		    /*%
			$$ = dispatch3(binary, $1, ripper_intern("=="), $3);
		    %*/
		    ;}
    break;

  case 216:
#line 2171 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = call_bin_op((yyvsp[-2].node), tEQQ, (yyvsp[0].node));
		    /*%
			$$ = dispatch3(binary, $1, ripper_intern("==="), $3);
		    %*/
		    ;}
    break;

  case 217:
#line 2179 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = call_bin_op((yyvsp[-2].node), tNEQ, (yyvsp[0].node));
		    /*%
			$$ = dispatch3(binary, $1, ripper_intern("!="), $3);
		    %*/
		    ;}
    break;

  case 218:
#line 2187 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = match_op((yyvsp[-2].node), (yyvsp[0].node));
                        if (nd_type((yyvsp[-2].node)) == NODE_LIT && TYPE((yyvsp[-2].node)->nd_lit) == T_REGEXP) {
                            (yyval.node) = reg_named_capture_assign((yyvsp[-2].node)->nd_lit, (yyval.node));
                        }
		    /*%
			$$ = dispatch3(binary, $1, ripper_intern("=~"), $3);
		    %*/
		    ;}
    break;

  case 219:
#line 2198 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = call_bin_op((yyvsp[-2].node), tNMATCH, (yyvsp[0].node));
		    /*%
			$$ = dispatch3(binary, $1, ripper_intern("!~"), $3);
		    %*/
		    ;}
    break;

  case 220:
#line 2206 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = call_uni_op(cond((yyvsp[0].node)), '!');
		    /*%
			$$ = dispatch2(unary, ID2SYM('!'), $2);
		    %*/
		    ;}
    break;

  case 221:
#line 2214 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = call_uni_op((yyvsp[0].node), '~');
		    /*%
			$$ = dispatch2(unary, ID2SYM('~'), $2);
		    %*/
		    ;}
    break;

  case 222:
#line 2222 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = call_bin_op((yyvsp[-2].node), tLSHFT, (yyvsp[0].node));
		    /*%
			$$ = dispatch3(binary, $1, ripper_intern("<<"), $3);
		    %*/
		    ;}
    break;

  case 223:
#line 2230 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = call_bin_op((yyvsp[-2].node), tRSHFT, (yyvsp[0].node));
		    /*%
			$$ = dispatch3(binary, $1, ripper_intern(">>"), $3);
		    %*/
		    ;}
    break;

  case 224:
#line 2238 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = logop(NODE_AND, (yyvsp[-2].node), (yyvsp[0].node));
		    /*%
			$$ = dispatch3(binary, $1, ripper_intern("&&"), $3);
		    %*/
		    ;}
    break;

  case 225:
#line 2246 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = logop(NODE_OR, (yyvsp[-2].node), (yyvsp[0].node));
		    /*%
			$$ = dispatch3(binary, $1, ripper_intern("||"), $3);
		    %*/
		    ;}
    break;

  case 226:
#line 2253 "parse.y"
    {in_defined = 1;;}
    break;

  case 227:
#line 2254 "parse.y"
    {
		    /*%%%*/
			in_defined = 0;
			(yyval.node) = NEW_DEFINED((yyvsp[0].node));
		    /*%
			in_defined = 0;
			$$ = dispatch1(defined, $4);
		    %*/
		    ;}
    break;

  case 228:
#line 2264 "parse.y"
    {
		    /*%%%*/
			value_expr((yyvsp[-5].node));
			(yyval.node) = NEW_IF(cond((yyvsp[-5].node)), (yyvsp[-3].node), (yyvsp[0].node));
			fixpos((yyval.node), (yyvsp[-5].node));
		    /*%
			$$ = dispatch3(ifop, $1, $3, $6);
		    %*/
		    ;}
    break;

  case 229:
#line 2274 "parse.y"
    {
			(yyval.node) = (yyvsp[0].node);
		    ;}
    break;

  case 230:
#line 2280 "parse.y"
    {
		    /*%%%*/
			value_expr((yyvsp[0].node));
			(yyval.node) = (yyvsp[0].node);
		        if (!(yyval.node)) (yyval.node) = NEW_NIL();
		    /*%
			$$ = $1;
		    %*/
		    ;}
    break;

  case 232:
#line 2293 "parse.y"
    {
			(yyval.node) = (yyvsp[-1].node);
		    ;}
    break;

  case 233:
#line 2297 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = arg_append((yyvsp[-3].node), NEW_HASH((yyvsp[-1].node)));
		    /*%
			$$ = arg_add_assocs($1, $3);
		    %*/
		    ;}
    break;

  case 234:
#line 2305 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = NEW_LIST(NEW_HASH((yyvsp[-1].node)));
		    /*%
			$$ = arg_add_assocs(arg_new(), $1);
		    %*/
		    ;}
    break;

  case 235:
#line 2315 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = (yyvsp[-1].node);
		    /*%
			$$ = dispatch1(arg_paren, escape_Qundef($2));
		    %*/
		    ;}
    break;

  case 240:
#line 2333 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = NEW_LIST((yyvsp[0].node));
		    /*%
			$$ = arg_add(arg_new(), $1);
		    %*/
		    ;}
    break;

  case 241:
#line 2341 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = arg_blk_pass((yyvsp[-1].node), (yyvsp[0].node));
		    /*%
			$$ = arg_add_optblock($1, $2);
		    %*/
		    ;}
    break;

  case 242:
#line 2349 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = NEW_LIST(NEW_HASH((yyvsp[-1].node)));
			(yyval.node) = arg_blk_pass((yyval.node), (yyvsp[0].node));
		    /*%
			$$ = arg_add_assocs(arg_new(), $1);
			$$ = arg_add_optblock($$, $2);
		    %*/
		    ;}
    break;

  case 243:
#line 2359 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = arg_append((yyvsp[-3].node), NEW_HASH((yyvsp[-1].node)));
			(yyval.node) = arg_blk_pass((yyval.node), (yyvsp[0].node));
		    /*%
			$$ = arg_add_optblock(arg_add_assocs($1, $3), $4);
		    %*/
		    ;}
    break;

  case 245:
#line 2376 "parse.y"
    {
			(yyval.num) = cmdarg_stack;
			CMDARG_PUSH(1);
		    ;}
    break;

  case 246:
#line 2381 "parse.y"
    {
			/* CMDARG_POP() */
			cmdarg_stack = (yyvsp[-1].num);
			(yyval.node) = (yyvsp[0].node);
		    ;}
    break;

  case 247:
#line 2389 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = NEW_BLOCK_PASS((yyvsp[0].node));
		    /*%
			$$ = $2;
		    %*/
		    ;}
    break;

  case 248:
#line 2399 "parse.y"
    {
			(yyval.node) = (yyvsp[0].node);
		    ;}
    break;

  case 249:
#line 2403 "parse.y"
    {
			(yyval.node) = 0;
		    ;}
    break;

  case 250:
#line 2407 "parse.y"
    {
			(yyval.node) = 0;
		    ;}
    break;

  case 251:
#line 2413 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = NEW_LIST((yyvsp[0].node));
		    /*%
			$$ = arg_add(arg_new(), $1);
		    %*/
		    ;}
    break;

  case 252:
#line 2421 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = NEW_SPLAT((yyvsp[0].node));
		    /*%
			$$ = arg_add_star(arg_new(), $2);
		    %*/
		    ;}
    break;

  case 253:
#line 2429 "parse.y"
    {
		    /*%%%*/
			NODE *n1;
			if ((n1 = splat_array((yyvsp[-2].node))) != 0) {
			    (yyval.node) = list_append(n1, (yyvsp[0].node));
			}
			else {
			    (yyval.node) = arg_append((yyvsp[-2].node), (yyvsp[0].node));
			}
		    /*%
			$$ = arg_add($1, $3);
		    %*/
		    ;}
    break;

  case 254:
#line 2443 "parse.y"
    {
		    /*%%%*/
			NODE *n1;
			if (nd_type((yyvsp[0].node)) == NODE_ARRAY &&
			    (n1 = splat_array((yyvsp[-3].node))) != 0) {
			    (yyval.node) = list_concat(n1, (yyvsp[0].node));
			}
			else {
			    (yyval.node) = arg_concat((yyvsp[-3].node), (yyvsp[0].node));
			}
		    /*%
			$$ = arg_add_star($1, $4);
		    %*/
		    ;}
    break;

  case 255:
#line 2460 "parse.y"
    {
		    /*%%%*/
			NODE *n1;
			if ((n1 = splat_array((yyvsp[-2].node))) != 0) {
			    (yyval.node) = list_append(n1, (yyvsp[0].node));
			}
			else {
			    (yyval.node) = arg_append((yyvsp[-2].node), (yyvsp[0].node));
			}
		    /*%
			$$ = mrhs_add(args2mrhs($1), $3);
		    %*/
		    ;}
    break;

  case 256:
#line 2474 "parse.y"
    {
		    /*%%%*/
			NODE *n1;
			if (nd_type((yyvsp[0].node)) == NODE_ARRAY &&
			    (n1 = splat_array((yyvsp[-3].node))) != 0) {
			    (yyval.node) = list_concat(n1, (yyvsp[0].node));
			}
			else {
			    (yyval.node) = arg_concat((yyvsp[-3].node), (yyvsp[0].node));
			}
		    /*%
			$$ = mrhs_add_star(args2mrhs($1), $4);
		    %*/
		    ;}
    break;

  case 257:
#line 2489 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = NEW_SPLAT((yyvsp[0].node));
		    /*%
			$$ = mrhs_add_star(mrhs_new(), $2);
		    %*/
		    ;}
    break;

  case 266:
#line 2507 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = NEW_FCALL((yyvsp[0].id), 0);
		    /*%
			$$ = method_arg(dispatch1(fcall, $1), arg_new());
		    %*/
		    ;}
    break;

  case 267:
#line 2515 "parse.y"
    {
		    /*%%%*/
			(yyval.num) = ruby_sourceline;
		    /*%
		    %*/
		    ;}
    break;

  case 268:
#line 2523 "parse.y"
    {
		    /*%%%*/
			if ((yyvsp[-1].node) == NULL) {
			    (yyval.node) = NEW_NIL();
			}
			else {
			    if (nd_type((yyvsp[-1].node)) == NODE_RESCUE ||
				nd_type((yyvsp[-1].node)) == NODE_ENSURE)
				nd_set_line((yyvsp[-1].node), (yyvsp[-2].num));
			    (yyval.node) = NEW_BEGIN((yyvsp[-1].node));
			}
			nd_set_line((yyval.node), (yyvsp[-2].num));
		    /*%
			$$ = dispatch1(begin, $3);
		    %*/
		    ;}
    break;

  case 269:
#line 2539 "parse.y"
    {lex_state = EXPR_ENDARG;;}
    break;

  case 270:
#line 2540 "parse.y"
    {
			rb_warning0("(...) interpreted as grouped expression");
		    /*%%%*/
			(yyval.node) = (yyvsp[-2].node);
		    /*%
			$$ = dispatch1(paren, $2);
		    %*/
		    ;}
    break;

  case 271:
#line 2549 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = (yyvsp[-1].node);
		    /*%
			$$ = dispatch1(paren, $2);
		    %*/
		    ;}
    break;

  case 272:
#line 2557 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = NEW_COLON2((yyvsp[-2].node), (yyvsp[0].id));
		    /*%
			$$ = dispatch2(const_path_ref, $1, $3);
		    %*/
		    ;}
    break;

  case 273:
#line 2565 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = NEW_COLON3((yyvsp[0].id));
		    /*%
			$$ = dispatch1(top_const_ref, $2);
		    %*/
		    ;}
    break;

  case 274:
#line 2573 "parse.y"
    {
		    /*%%%*/
			if ((yyvsp[-1].node) == 0) {
			    (yyval.node) = NEW_ZARRAY(); /* zero length array*/
			}
			else {
			    (yyval.node) = (yyvsp[-1].node);
			}
		    /*%
			$$ = dispatch1(array, escape_Qundef($2));
		    %*/
		    ;}
    break;

  case 275:
#line 2586 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = NEW_HASH((yyvsp[-1].node));
		    /*%
			$$ = dispatch1(hash, escape_Qundef($2));
		    %*/
		    ;}
    break;

  case 276:
#line 2594 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = NEW_RETURN(0);
		    /*%
			$$ = dispatch0(return0);
		    %*/
		    ;}
    break;

  case 277:
#line 2602 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = new_yield((yyvsp[-1].node));
		    /*%
			$$ = dispatch1(yield, dispatch1(paren, $3));
		    %*/
		    ;}
    break;

  case 278:
#line 2610 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = NEW_YIELD(0, Qfalse);
		    /*%
			$$ = dispatch1(yield, dispatch1(paren, arg_new()));
		    %*/
		    ;}
    break;

  case 279:
#line 2618 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = NEW_YIELD(0, Qfalse);
		    /*%
			$$ = dispatch0(yield0);
		    %*/
		    ;}
    break;

  case 280:
#line 2625 "parse.y"
    {in_defined = 1;;}
    break;

  case 281:
#line 2626 "parse.y"
    {
		    /*%%%*/
			in_defined = 0;
			(yyval.node) = NEW_DEFINED((yyvsp[-1].node));
		    /*%
			in_defined = 0;
			$$ = dispatch1(defined, $5);
		    %*/
		    ;}
    break;

  case 282:
#line 2636 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = call_uni_op(cond((yyvsp[-1].node)), '!');
		    /*%
			$$ = dispatch2(unary, ripper_intern("not"), $3);
		    %*/
		    ;}
    break;

  case 283:
#line 2644 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = call_uni_op(cond(NEW_NIL()), '!');
		    /*%
			$$ = dispatch2(unary, ripper_intern("not"), Qnil);
		    %*/
		    ;}
    break;

  case 284:
#line 2652 "parse.y"
    {
		    /*%%%*/
			(yyvsp[0].node)->nd_iter = NEW_FCALL((yyvsp[-1].id), 0);
			(yyval.node) = (yyvsp[0].node);
			fixpos((yyvsp[0].node)->nd_iter, (yyvsp[0].node));
		    /*%
			$$ = method_arg(dispatch1(fcall, $1), arg_new());
			$$ = method_add_block($$, $2);
		    %*/
		    ;}
    break;

  case 286:
#line 2664 "parse.y"
    {
		    /*%%%*/
			block_dup_check((yyvsp[-1].node)->nd_args, (yyvsp[0].node));
			(yyvsp[0].node)->nd_iter = (yyvsp[-1].node);
			(yyval.node) = (yyvsp[0].node);
			fixpos((yyval.node), (yyvsp[-1].node));
		    /*%
			$$ = method_add_block($1, $2);
		    %*/
		    ;}
    break;

  case 287:
#line 2675 "parse.y"
    {
			(yyval.node) = (yyvsp[0].node);
		    ;}
    break;

  case 288:
#line 2682 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = NEW_IF(cond((yyvsp[-4].node)), (yyvsp[-2].node), (yyvsp[-1].node));
			fixpos((yyval.node), (yyvsp[-4].node));
		    /*%
			$$ = dispatch3(if, $2, $4, escape_Qundef($5));
		    %*/
		    ;}
    break;

  case 289:
#line 2694 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = NEW_UNLESS(cond((yyvsp[-4].node)), (yyvsp[-2].node), (yyvsp[-1].node));
			fixpos((yyval.node), (yyvsp[-4].node));
		    /*%
			$$ = dispatch3(unless, $2, $4, escape_Qundef($5));
		    %*/
		    ;}
    break;

  case 290:
#line 2702 "parse.y"
    {COND_PUSH(1);;}
    break;

  case 291:
#line 2702 "parse.y"
    {COND_POP();;}
    break;

  case 292:
#line 2705 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = NEW_WHILE(cond((yyvsp[-4].node)), (yyvsp[-1].node), 1);
			fixpos((yyval.node), (yyvsp[-4].node));
		    /*%
			$$ = dispatch2(while, $3, $6);
		    %*/
		    ;}
    break;

  case 293:
#line 2713 "parse.y"
    {COND_PUSH(1);;}
    break;

  case 294:
#line 2713 "parse.y"
    {COND_POP();;}
    break;

  case 295:
#line 2716 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = NEW_UNTIL(cond((yyvsp[-4].node)), (yyvsp[-1].node), 1);
			fixpos((yyval.node), (yyvsp[-4].node));
		    /*%
			$$ = dispatch2(until, $3, $6);
		    %*/
		    ;}
    break;

  case 296:
#line 2727 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = NEW_CASE((yyvsp[-3].node), (yyvsp[-1].node));
			fixpos((yyval.node), (yyvsp[-3].node));
		    /*%
			$$ = dispatch2(case, $2, $4);
		    %*/
		    ;}
    break;

  case 297:
#line 2736 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = NEW_CASE(0, (yyvsp[-1].node));
		    /*%
			$$ = dispatch2(case, Qnil, $3);
		    %*/
		    ;}
    break;

  case 298:
#line 2744 "parse.y"
    {COND_PUSH(1);;}
    break;

  case 299:
#line 2746 "parse.y"
    {COND_POP();;}
    break;

  case 300:
#line 2749 "parse.y"
    {
		    /*%%%*/
			/*
			 *  for a, b, c in e
			 *  #=>
			 *  e.each{|*x| a, b, c = x
			 *
			 *  for a in e
			 *  #=>
			 *  e.each{|x| a, = x}
			 */
			ID id = internal_id();
			ID *tbl = ALLOC_N(ID, 2);
			NODE *m = NEW_ARGS_AUX(0, 0);
			NODE *args, *scope;

			if (nd_type((yyvsp[-7].node)) == NODE_MASGN) {
			    /* if args.length == 1 && args[0].kind_of?(Array)
			     *   args = args[0]
			     * end
			     */
			    NODE *one = NEW_LIST(NEW_LIT(INT2FIX(1)));
			    NODE *zero = NEW_LIST(NEW_LIT(INT2FIX(0)));
			    m->nd_next = block_append(
				NEW_IF(
				    NEW_NODE(NODE_AND,
					     NEW_CALL(NEW_CALL(NEW_DVAR(id), rb_intern("length"), 0),
						      rb_intern("=="), one),
					     NEW_CALL(NEW_CALL(NEW_DVAR(id), rb_intern("[]"), zero),
						      rb_intern("kind_of?"), NEW_LIST(NEW_LIT(rb_cArray))),
					     0),
				    NEW_DASGN_CURR(id,
						   NEW_CALL(NEW_DVAR(id), rb_intern("[]"), zero)),
				    0),
				node_assign((yyvsp[-7].node), NEW_DVAR(id)));

			    args = new_args(m, 0, id, 0, 0);
			}
			else {
			    if (nd_type((yyvsp[-7].node)) == NODE_LASGN ||
				nd_type((yyvsp[-7].node)) == NODE_DASGN ||
				nd_type((yyvsp[-7].node)) == NODE_DASGN_CURR) {
				(yyvsp[-7].node)->nd_value = NEW_DVAR(id);
				m->nd_plen = 1;
				m->nd_next = (yyvsp[-7].node);
				args = new_args(m, 0, 0, 0, 0);
			    }
			    else {
				m->nd_next = node_assign(NEW_MASGN(NEW_LIST((yyvsp[-7].node)), 0), NEW_DVAR(id));
				args = new_args(m, 0, id, 0, 0);
			    }
			}
			scope = NEW_NODE(NODE_SCOPE, tbl, (yyvsp[-1].node), args);
			tbl[0] = 1; tbl[1] = id;
			(yyval.node) = NEW_FOR(0, (yyvsp[-4].node), scope);
			fixpos((yyval.node), (yyvsp[-7].node));
		    /*%
			$$ = dispatch3(for, $2, $5, $8);
		    %*/
		    ;}
    break;

  case 301:
#line 2810 "parse.y"
    {
			if (in_def || in_single)
			    yyerror("class definition in method body");
		    /*%%%*/
			local_push(0);
			(yyval.num) = ruby_sourceline;
		    /*%
		    %*/
		    ;}
    break;

  case 302:
#line 2821 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = NEW_CLASS((yyvsp[-4].node), (yyvsp[-1].node), (yyvsp[-3].node));
			nd_set_line((yyval.node), (yyvsp[-2].num));
			local_pop();
		    /*%
			$$ = dispatch3(class, $2, $3, $5);
		    %*/
		    ;}
    break;

  case 303:
#line 2831 "parse.y"
    {
		    /*%%%*/
			(yyval.num) = in_def;
			in_def = 0;
		    /*%
			in_def = 0;
		    %*/
		    ;}
    break;

  case 304:
#line 2840 "parse.y"
    {
		    /*%%%*/
			(yyval.num) = in_single;
			in_single = 0;
			local_push(0);
		    /*%
			$$ = in_single;
			in_single = 0;
		    %*/
		    ;}
    break;

  case 305:
#line 2852 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = NEW_SCLASS((yyvsp[-5].node), (yyvsp[-1].node));
			fixpos((yyval.node), (yyvsp[-5].node));
			local_pop();
			in_def = (yyvsp[-4].num);
			in_single = (yyvsp[-2].num);
		    /*%
			$$ = dispatch2(sclass, $3, $7);
			in_def = $<val>4;
			in_single = $<val>6;
		    %*/
		    ;}
    break;

  case 306:
#line 2866 "parse.y"
    {
			if (in_def || in_single)
			    yyerror("module definition in method body");
		    /*%%%*/
			local_push(0);
			(yyval.num) = ruby_sourceline;
		    /*%
		    %*/
		    ;}
    break;

  case 307:
#line 2877 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = NEW_MODULE((yyvsp[-3].node), (yyvsp[-1].node));
			nd_set_line((yyval.node), (yyvsp[-2].num));
			local_pop();
		    /*%
			$$ = dispatch2(module, $2, $4);
		    %*/
		    ;}
    break;

  case 308:
#line 2887 "parse.y"
    {
			(yyval.id) = cur_mid;
			cur_mid = (yyvsp[0].id);
			in_def++;
		    /*%%%*/
			local_push(0);
		    /*%
		    %*/
		    ;}
    break;

  case 309:
#line 2899 "parse.y"
    {
		    /*%%%*/
			NODE *body = remove_begin((yyvsp[-1].node));
			reduce_nodes(&body);
			(yyval.node) = NEW_DEFN((yyvsp[-4].id), (yyvsp[-2].node), body, NOEX_PRIVATE);
			fixpos((yyval.node), (yyvsp[-2].node));
			fixpos((yyval.node)->nd_defn, (yyvsp[-2].node));
			local_pop();
			in_def--;
			cur_mid = (yyvsp[-3].id);
		    /*%
			$$ = dispatch3(def, $2, $4, $5);
			in_def--;
			cur_mid = $<id>3;
		    %*/
		    ;}
    break;

  case 310:
#line 2915 "parse.y"
    {lex_state = EXPR_FNAME;;}
    break;

  case 311:
#line 2916 "parse.y"
    {
			in_single++;
			lex_state = EXPR_END; /* force for args */
		    /*%%%*/
			local_push(0);
		    /*%
		    %*/
		    ;}
    break;

  case 312:
#line 2927 "parse.y"
    {
		    /*%%%*/
			NODE *body = remove_begin((yyvsp[-1].node));
			reduce_nodes(&body);
			(yyval.node) = NEW_DEFS((yyvsp[-7].node), (yyvsp[-4].id), (yyvsp[-2].node), body);
			fixpos((yyval.node), (yyvsp[-7].node));
			fixpos((yyval.node)->nd_defn, (yyvsp[-7].node));
			local_pop();
			in_single--;
		    /*%
			$$ = dispatch5(defs, $2, $3, $5, $7, $8);
			in_single--;
		    %*/
		    ;}
    break;

  case 313:
#line 2942 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = NEW_BREAK(0);
		    /*%
			$$ = dispatch1(break, arg_new());
		    %*/
		    ;}
    break;

  case 314:
#line 2950 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = NEW_NEXT(0);
		    /*%
			$$ = dispatch1(next, arg_new());
		    %*/
		    ;}
    break;

  case 315:
#line 2958 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = NEW_REDO();
		    /*%
			$$ = dispatch0(redo);
		    %*/
		    ;}
    break;

  case 316:
#line 2966 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = NEW_RETRY();
		    /*%
			$$ = dispatch0(retry);
		    %*/
		    ;}
    break;

  case 317:
#line 2976 "parse.y"
    {
		    /*%%%*/
			value_expr((yyvsp[0].node));
			(yyval.node) = (yyvsp[0].node);
		        if (!(yyval.node)) (yyval.node) = NEW_NIL();
		    /*%
			$$ = $1;
		    %*/
		    ;}
    break;

  case 318:
#line 2988 "parse.y"
    {
#ifndef RIPPER
			if (RTEST(ruby_verbose)) token_info_push(parser, "begin");
#endif
		    ;}
    break;

  case 319:
#line 2995 "parse.y"
    {
#ifndef RIPPER
			if (RTEST(ruby_verbose)) token_info_push(parser, "if");
#endif
		    ;}
    break;

  case 320:
#line 3002 "parse.y"
    {
#ifndef RIPPER
			if (RTEST(ruby_verbose)) token_info_push(parser, "unless");
#endif
		    ;}
    break;

  case 321:
#line 3009 "parse.y"
    {
#ifndef RIPPER
			if (RTEST(ruby_verbose)) token_info_push(parser, "while");
#endif
		    ;}
    break;

  case 322:
#line 3016 "parse.y"
    {
#ifndef RIPPER
			if (RTEST(ruby_verbose)) token_info_push(parser, "until");
#endif
		    ;}
    break;

  case 323:
#line 3023 "parse.y"
    {
#ifndef RIPPER
			if (RTEST(ruby_verbose)) token_info_push(parser, "case");
#endif
		    ;}
    break;

  case 324:
#line 3030 "parse.y"
    {
#ifndef RIPPER
			if (RTEST(ruby_verbose)) token_info_push(parser, "for");
#endif
		    ;}
    break;

  case 325:
#line 3037 "parse.y"
    {
#ifndef RIPPER
			if (RTEST(ruby_verbose)) token_info_push(parser, "class");
#endif
		    ;}
    break;

  case 326:
#line 3044 "parse.y"
    {
#ifndef RIPPER
			if (RTEST(ruby_verbose)) token_info_push(parser, "module");
#endif
		    ;}
    break;

  case 327:
#line 3051 "parse.y"
    {
#ifndef RIPPER
			if (RTEST(ruby_verbose)) token_info_push(parser, "def");
#endif
		    ;}
    break;

  case 328:
#line 3058 "parse.y"
    {
#ifndef RIPPER
			if (RTEST(ruby_verbose)) token_info_pop(parser, "end");  /* POP */
#endif
		    ;}
    break;

  case 335:
#line 3089 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = NEW_IF(cond((yyvsp[-3].node)), (yyvsp[-1].node), (yyvsp[0].node));
			fixpos((yyval.node), (yyvsp[-3].node));
		    /*%
			$$ = dispatch3(elsif, $2, $4, escape_Qundef($5));
		    %*/
		    ;}
    break;

  case 337:
#line 3101 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = (yyvsp[0].node);
		    /*%
			$$ = dispatch1(else, $2);
		    %*/
		    ;}
    break;

  case 340:
#line 3115 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = assignable((yyvsp[0].id), 0);
		    /*%
			$$ = dispatch1(mlhs_paren, $1);
		    %*/
		    ;}
    break;

  case 341:
#line 3123 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = (yyvsp[-1].node);
		    /*%
			$$ = dispatch1(mlhs_paren, $2);
		    %*/
		    ;}
    break;

  case 342:
#line 3133 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = NEW_LIST((yyvsp[0].node));
		    /*%
			$$ = mlhs_add(mlhs_new(), $1);
		    %*/
		    ;}
    break;

  case 343:
#line 3141 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = list_append((yyvsp[-2].node), (yyvsp[0].node));
		    /*%
			$$ = mlhs_add($1, $3);
		    %*/
		    ;}
    break;

  case 344:
#line 3151 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = NEW_MASGN((yyvsp[0].node), 0);
		    /*%
			$$ = $1;
		    %*/
		    ;}
    break;

  case 345:
#line 3159 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = NEW_MASGN((yyvsp[-3].node), assignable((yyvsp[0].id), 0));
		    /*%
			$$ = mlhs_add_star($1, $4);
		    %*/
		    ;}
    break;

  case 346:
#line 3167 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = NEW_MASGN((yyvsp[-5].node), NEW_POSTARG(assignable((yyvsp[-2].id), 0), (yyvsp[0].node)));
		    /*%
			$$ = mlhs_add_star($1, $4);
		    %*/
		    ;}
    break;

  case 347:
#line 3175 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = NEW_MASGN((yyvsp[-2].node), -1);
		    /*%
			$$ = mlhs_add_star($1, Qnil);
		    %*/
		    ;}
    break;

  case 348:
#line 3183 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = NEW_MASGN((yyvsp[-4].node), NEW_POSTARG(-1, (yyvsp[0].node)));
		    /*%
			$$ = mlhs_add_star($1, $5);
		    %*/
		    ;}
    break;

  case 349:
#line 3191 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = NEW_MASGN(0, assignable((yyvsp[0].id), 0));
		    /*%
			$$ = mlhs_add_star(mlhs_new(), $2);
		    %*/
		    ;}
    break;

  case 350:
#line 3199 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = NEW_MASGN(0, NEW_POSTARG(assignable((yyvsp[-2].id), 0), (yyvsp[0].node)));
		    /*%
		      #if 0
		      TODO: Check me
		      #endif
			$$ = mlhs_add_star($2, $4);
		    %*/
		    ;}
    break;

  case 351:
#line 3210 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = NEW_MASGN(0, -1);
		    /*%
			$$ = mlhs_add_star(mlhs_new(), Qnil);
		    %*/
		    ;}
    break;

  case 352:
#line 3218 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = NEW_MASGN(0, NEW_POSTARG(-1, (yyvsp[0].node)));
		    /*%
			$$ = mlhs_add_star(mlhs_new(), Qnil);
		    %*/
		    ;}
    break;

  case 353:
#line 3228 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = new_args((yyvsp[-5].node), (yyvsp[-3].node), (yyvsp[-1].id), 0, (yyvsp[0].id));
		    /*%
			$$ = params_new($1, $3, $5, Qnil, escape_Qundef($6));
		    %*/
		    ;}
    break;

  case 354:
#line 3236 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = new_args((yyvsp[-7].node), (yyvsp[-5].node), (yyvsp[-3].id), (yyvsp[-1].node), (yyvsp[0].id));
		    /*%
			$$ = params_new($1, $3, $5, $7, escape_Qundef($8));
		    %*/
		    ;}
    break;

  case 355:
#line 3244 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = new_args((yyvsp[-3].node), (yyvsp[-1].node), 0, 0, (yyvsp[0].id));
		    /*%
			$$ = params_new($1, $3, Qnil, Qnil, escape_Qundef($4));
		    %*/
		    ;}
    break;

  case 356:
#line 3252 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = new_args((yyvsp[-5].node), (yyvsp[-3].node), 0, (yyvsp[-1].node), (yyvsp[0].id));
		    /*%
			$$ = params_new($1, $3, Qnil, $5, escape_Qundef($6));
		    %*/
		    ;}
    break;

  case 357:
#line 3260 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = new_args((yyvsp[-3].node), 0, (yyvsp[-1].id), 0, (yyvsp[0].id));
		    /*%
			$$ = params_new($1, Qnil, $3, Qnil, escape_Qundef($4));
		    %*/
		    ;}
    break;

  case 358:
#line 3268 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = new_args((yyvsp[-1].node), 0, 1, 0, 0);
		    /*%
			$$ = params_new($1, Qnil, Qnil, Qnil, Qnil);
                        dispatch1(excessed_comma, $$);
		    %*/
		    ;}
    break;

  case 359:
#line 3277 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = new_args((yyvsp[-5].node), 0, (yyvsp[-3].id), (yyvsp[-1].node), (yyvsp[0].id));
		    /*%
			$$ = params_new($1, Qnil, $3, $5, escape_Qundef($6));
		    %*/
		    ;}
    break;

  case 360:
#line 3285 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = new_args((yyvsp[-1].node), 0, 0, 0, (yyvsp[0].id));
		    /*%
			$$ = params_new($1, Qnil,Qnil, Qnil, escape_Qundef($2));
		    %*/
		    ;}
    break;

  case 361:
#line 3293 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = new_args(0, (yyvsp[-3].node), (yyvsp[-1].id), 0, (yyvsp[0].id));
		    /*%
			$$ = params_new(Qnil, $1, $3, Qnil, escape_Qundef($4));
		    %*/
		    ;}
    break;

  case 362:
#line 3301 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = new_args(0, (yyvsp[-5].node), (yyvsp[-3].id), (yyvsp[-1].node), (yyvsp[0].id));
		    /*%
			$$ = params_new(Qnil, $1, $3, $5, escape_Qundef($6));
		    %*/
		    ;}
    break;

  case 363:
#line 3309 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = new_args(0, (yyvsp[-1].node), 0, 0, (yyvsp[0].id));
		    /*%
			$$ = params_new(Qnil, $1, Qnil, Qnil,escape_Qundef($2));
		    %*/
		    ;}
    break;

  case 364:
#line 3317 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = new_args(0, (yyvsp[-3].node), 0, (yyvsp[-1].node), (yyvsp[0].id));
		    /*%
			$$ = params_new(Qnil, $1, Qnil, $3, escape_Qundef($4));
		    %*/
		    ;}
    break;

  case 365:
#line 3325 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = new_args(0, 0, (yyvsp[-1].id), 0, (yyvsp[0].id));
		    /*%
			$$ = params_new(Qnil, Qnil, $1, Qnil, escape_Qundef($2));
		    %*/
		    ;}
    break;

  case 366:
#line 3333 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = new_args(0, 0, (yyvsp[-3].id), (yyvsp[-1].node), (yyvsp[0].id));
		    /*%
			$$ = params_new(Qnil, Qnil, $1, $3, escape_Qundef($4));
		    %*/
		    ;}
    break;

  case 367:
#line 3341 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = new_args(0, 0, 0, 0, (yyvsp[0].id));
		    /*%
			$$ = params_new(Qnil, Qnil, Qnil, Qnil, $1);
		    %*/
		    ;}
    break;

  case 369:
#line 3352 "parse.y"
    {
			command_start = Qtrue;
		    ;}
    break;

  case 370:
#line 3358 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = 0;
		    /*%
			$$ = blockvar_new(params_new(Qnil,Qnil,Qnil,Qnil,Qnil),
                                          escape_Qundef($2));
		    %*/
		    ;}
    break;

  case 371:
#line 3367 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = 0;
		    /*%
			$$ = blockvar_new(params_new(Qnil,Qnil,Qnil,Qnil,Qnil),
                                          Qnil);
		    %*/
		    ;}
    break;

  case 372:
#line 3376 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = (yyvsp[-2].node);
		    /*%
			$$ = blockvar_new(escape_Qundef($2), escape_Qundef($3));
		    %*/
		    ;}
    break;

  case 374:
#line 3388 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = 0;
		    /*%
			$$ = $2;
		    %*/
		    ;}
    break;

  case 377:
#line 3414 "parse.y"
    {
		    /*%%%*/
			new_bv((yyvsp[0].id));
		    /*%
			$$ = $1;
		    %*/
		    ;}
    break;

  case 378:
#line 3422 "parse.y"
    {
			(yyval.node) = 0;
		    ;}
    break;

  case 379:
#line 3427 "parse.y"
    {
		    /*%%%*/
			dyna_push();
			(yyval.num) = lpar_beg;
			lpar_beg = ++paren_nest;
		    /*%
		    %*/
		    ;}
    break;

  case 380:
#line 3437 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = (yyvsp[-1].node);
			(yyval.node)->nd_body = NEW_SCOPE((yyvsp[-1].node)->nd_head, (yyvsp[0].node));
			dyna_pop();
			lpar_beg = (yyvsp[-2].num);
		    /*%
			$$ = dispatch2(lambda, $2, $3);
		    %*/
		    ;}
    break;

  case 381:
#line 3450 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = NEW_LAMBDA((yyvsp[-2].node));
		    /*%
			$$ = dispatch1(paren, $2);
		    %*/
		    ;}
    break;

  case 382:
#line 3458 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = NEW_LAMBDA((yyvsp[-1].node));
		    /*%
			$$ = $1;
		    %*/
		    ;}
    break;

  case 383:
#line 3468 "parse.y"
    {
			(yyval.node) = (yyvsp[-1].node);
		    ;}
    break;

  case 384:
#line 3472 "parse.y"
    {
			(yyval.node) = (yyvsp[-1].node);
		    ;}
    break;

  case 385:
#line 3478 "parse.y"
    {
		    /*%%%*/
			dyna_push();
			(yyval.num) = ruby_sourceline;
		    /*% %*/
		    ;}
    break;

  case 386:
#line 3487 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = NEW_ITER((yyvsp[-2].node),(yyvsp[-1].node));
			nd_set_line((yyval.node), (yyvsp[-3].num));
			dyna_pop();
		    /*%
			$$ = dispatch2(do_block, escape_Qundef($3), $4);
		    %*/
		    ;}
    break;

  case 387:
#line 3499 "parse.y"
    {
		    /*%%%*/
			block_dup_check((yyvsp[-1].node)->nd_args, (yyvsp[0].node));
			(yyvsp[0].node)->nd_iter = (yyvsp[-1].node);
			(yyval.node) = (yyvsp[0].node);
			fixpos((yyval.node), (yyvsp[-1].node));
		    /*%
			$$ = method_add_block($1, $2);
		    %*/
		    ;}
    break;

  case 388:
#line 3510 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = NEW_CALL((yyvsp[-3].node), (yyvsp[-1].id), (yyvsp[0].node));
		    /*%
			$$ = dispatch3(call, $1, ripper_id2sym('.'), $3);
			$$ = method_optarg($$, $4);
		    %*/
		    ;}
    break;

  case 389:
#line 3519 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = NEW_CALL((yyvsp[-3].node), (yyvsp[-1].id), (yyvsp[0].node));
		    /*%
			$$ = dispatch3(call, $1, ripper_intern("::"), $3);
			$$ = method_optarg($$, $4);
		    %*/
		    ;}
    break;

  case 390:
#line 3530 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = NEW_FCALL((yyvsp[-1].id), (yyvsp[0].node));
			fixpos((yyval.node), (yyvsp[0].node));
		    /*%
			$$ = method_arg(dispatch1(fcall, $1), $2);
		    %*/
		    ;}
    break;

  case 391:
#line 3539 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = NEW_CALL((yyvsp[-3].node), (yyvsp[-1].id), (yyvsp[0].node));
			fixpos((yyval.node), (yyvsp[-3].node));
		    /*%
			$$ = dispatch3(call, $1, ripper_id2sym('.'), $3);
			$$ = method_optarg($$, $4);
		    %*/
		    ;}
    break;

  case 392:
#line 3549 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = NEW_CALL((yyvsp[-3].node), (yyvsp[-1].id), (yyvsp[0].node));
			fixpos((yyval.node), (yyvsp[-3].node));
		    /*%
			$$ = dispatch3(call, $1, ripper_id2sym('.'), $3);
			$$ = method_optarg($$, $4);
		    %*/
		    ;}
    break;

  case 393:
#line 3559 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = NEW_CALL((yyvsp[-2].node), (yyvsp[0].id), 0);
		    /*%
			$$ = dispatch3(call, $1, ripper_intern("::"), $3);
		    %*/
		    ;}
    break;

  case 394:
#line 3567 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = NEW_CALL((yyvsp[-2].node), rb_intern("call"), (yyvsp[0].node));
			fixpos((yyval.node), (yyvsp[-2].node));
		    /*%
			$$ = dispatch3(call, dispatch1(paren, $1),
				       ripper_id2sym('.'), rb_intern("call"));
			$$ = method_optarg($$, $3);
		    %*/
		    ;}
    break;

  case 395:
#line 3578 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = NEW_CALL((yyvsp[-2].node), rb_intern("call"), (yyvsp[0].node));
			fixpos((yyval.node), (yyvsp[-2].node));
		    /*%
			$$ = dispatch3(call, dispatch1(paren, $1),
				       ripper_id2sym('.'), rb_intern("call"));
			$$ = method_optarg($$, $3);
		    %*/
		    ;}
    break;

  case 396:
#line 3589 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = NEW_SUPER((yyvsp[0].node));
		    /*%
			$$ = dispatch1(super, $2);
		    %*/
		    ;}
    break;

  case 397:
#line 3597 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = NEW_ZSUPER();
		    /*%
			$$ = dispatch0(zsuper);
		    %*/
		    ;}
    break;

  case 398:
#line 3605 "parse.y"
    {
		    /*%%%*/
			if ((yyvsp[-3].node) && nd_type((yyvsp[-3].node)) == NODE_SELF)
			    (yyval.node) = NEW_FCALL(tAREF, (yyvsp[-1].node));
			else
			    (yyval.node) = NEW_CALL((yyvsp[-3].node), tAREF, (yyvsp[-1].node));
			fixpos((yyval.node), (yyvsp[-3].node));
		    /*%
			$$ = dispatch2(aref, $1, escape_Qundef($3));
		    %*/
		    ;}
    break;

  case 399:
#line 3619 "parse.y"
    {
		    /*%%%*/
			dyna_push();
			(yyval.num) = ruby_sourceline;
		    /*%
                    %*/
		    ;}
    break;

  case 400:
#line 3628 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = NEW_ITER((yyvsp[-2].node),(yyvsp[-1].node));
			nd_set_line((yyval.node), (yyvsp[-3].num));
			nd_set_line((yyval.node)->nd_body, (yyvsp[-3].num));
			nd_set_line((yyval.node)->nd_body->nd_body, (yyvsp[-3].num));
			dyna_pop();
		    /*%
			$$ = dispatch2(brace_block, escape_Qundef($3), $4);
		    %*/
		    ;}
    break;

  case 401:
#line 3640 "parse.y"
    {
		    /*%%%*/
			dyna_push();
			(yyval.num) = ruby_sourceline;
		    /*%
                    %*/
		    ;}
    break;

  case 402:
#line 3649 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = NEW_ITER((yyvsp[-2].node),(yyvsp[-1].node));
			nd_set_line((yyval.node), (yyvsp[-3].num));
			dyna_pop();
		    /*%
			$$ = dispatch2(do_block, escape_Qundef($3), $4);
		    %*/
		    ;}
    break;

  case 403:
#line 3663 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = NEW_WHEN((yyvsp[-3].node), (yyvsp[-1].node), (yyvsp[0].node));
		    /*%
			$$ = dispatch3(when, $2, $4, escape_Qundef($5));
		    %*/
		    ;}
    break;

  case 406:
#line 3679 "parse.y"
    {
		    /*%%%*/
			if ((yyvsp[-3].node)) {
			    (yyvsp[-3].node) = node_assign((yyvsp[-3].node), NEW_ERRINFO());
			    (yyvsp[-1].node) = block_append((yyvsp[-3].node), (yyvsp[-1].node));
			}
			(yyval.node) = NEW_RESBODY((yyvsp[-4].node), (yyvsp[-1].node), (yyvsp[0].node));
			fixpos((yyval.node), (yyvsp[-4].node)?(yyvsp[-4].node):(yyvsp[-1].node));
		    /*%
			$$ = dispatch4(rescue,
				       escape_Qundef($2),
				       escape_Qundef($3),
				       escape_Qundef($5),
				       escape_Qundef($6));
		    %*/
		    ;}
    break;

  case 408:
#line 3699 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = NEW_LIST((yyvsp[0].node));
		    /*%
			$$ = rb_ary_new3(1, $1);
		    %*/
		    ;}
    break;

  case 409:
#line 3707 "parse.y"
    {
		    /*%%%*/
			if (!((yyval.node) = splat_array((yyvsp[0].node)))) (yyval.node) = (yyvsp[0].node);
		    /*%
			$$ = $1;
		    %*/
		    ;}
    break;

  case 411:
#line 3718 "parse.y"
    {
			(yyval.node) = (yyvsp[0].node);
		    ;}
    break;

  case 413:
#line 3725 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = (yyvsp[0].node);
		    /*%
			$$ = dispatch1(ensure, $2);
		    %*/
		    ;}
    break;

  case 416:
#line 3737 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = NEW_LIT(ID2SYM((yyvsp[0].id)));
		    /*%
			$$ = dispatch1(symbol_literal, $1);
		    %*/
		    ;}
    break;

  case 418:
#line 3748 "parse.y"
    {
		    /*%%%*/
			NODE *node = (yyvsp[0].node);
			if (!node) {
			    node = NEW_STR(STR_NEW0());
			}
			else {
			    node = evstr2dstr(node);
			}
			(yyval.node) = node;
		    /*%
			$$ = $1;
		    %*/
		    ;}
    break;

  case 421:
#line 3767 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = literal_concat((yyvsp[-1].node), (yyvsp[0].node));
		    /*%
			$$ = dispatch2(string_concat, $1, $2);
		    %*/
		    ;}
    break;

  case 422:
#line 3777 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = (yyvsp[-1].node);
		    /*%
			$$ = dispatch1(string_literal, $2);
		    %*/
		    ;}
    break;

  case 423:
#line 3787 "parse.y"
    {
		    /*%%%*/
			NODE *node = (yyvsp[-1].node);
			if (!node) {
			    node = NEW_XSTR(STR_NEW0());
			}
			else {
			    switch (nd_type(node)) {
			      case NODE_STR:
				nd_set_type(node, NODE_XSTR);
				break;
			      case NODE_DSTR:
				nd_set_type(node, NODE_DXSTR);
				break;
			      default:
				node = NEW_NODE(NODE_DXSTR, STR_NEW0(), 1, NEW_LIST(node));
				break;
			    }
			}
			(yyval.node) = node;
		    /*%
			$$ = dispatch1(xstring_literal, $2);
		    %*/
		    ;}
    break;

  case 424:
#line 3814 "parse.y"
    {
		    /*%%%*/
			int options = (yyvsp[0].num);
			NODE *node = (yyvsp[-1].node);
                        NODE *list;
			if (!node) {
			    node = NEW_LIT(reg_compile(STR_NEW0(), options));
			}
			else switch (nd_type(node)) {
			  case NODE_STR:
			    {
				VALUE src = node->nd_lit;
				nd_set_type(node, NODE_LIT);
				node->nd_lit = reg_compile(src, options);
			    }
			    break;
			  default:
			    node = NEW_NODE(NODE_DSTR, STR_NEW0(), 1, NEW_LIST(node));
			  case NODE_DSTR:
			    if (options & RE_OPTION_ONCE) {
				nd_set_type(node, NODE_DREGX_ONCE);
			    }
			    else {
				nd_set_type(node, NODE_DREGX);
			    }
			    node->nd_cflag = options & RE_OPTION_MASK;
                            reg_fragment_check(node->nd_lit, options);
                            for (list = node->nd_next; list; list = list->nd_next) {
                                if (nd_type(list->nd_head) == NODE_STR) {
                                    reg_fragment_check(list->nd_head->nd_lit, options);
                                }
                            }
			    break;
			}
			(yyval.node) = node;
		    /*%
			$$ = dispatch2(regexp_literal, $2, $3);
		    %*/
		    ;}
    break;

  case 425:
#line 3856 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = NEW_ZARRAY();
		    /*%
			$$ = dispatch0(words_new);
		    %*/
		    ;}
    break;

  case 426:
#line 3864 "parse.y"
    {
			(yyval.node) = (yyvsp[-1].node);
		    ;}
    break;

  case 427:
#line 3870 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = 0;
		    /*%
			$$ = dispatch0(words_new);
		    %*/
		    ;}
    break;

  case 428:
#line 3878 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = list_append((yyvsp[-2].node), evstr2dstr((yyvsp[-1].node)));
		    /*%
			$$ = dispatch2(words_add, $1, $2);
		    %*/
		    ;}
    break;

  case 430:
#line 3896 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = literal_concat((yyvsp[-1].node), (yyvsp[0].node));
		    /*%
			$$ = dispatch2(word_add, $1, $2);
		    %*/
		    ;}
    break;

  case 431:
#line 3906 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = NEW_ZARRAY();
		    /*%
			$$ = dispatch0(qwords_new);
		    %*/
		    ;}
    break;

  case 432:
#line 3914 "parse.y"
    {
			(yyval.node) = (yyvsp[-1].node);
		    ;}
    break;

  case 433:
#line 3920 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = 0;
		    /*%
			$$ = dispatch0(qwords_new);
		    %*/
		    ;}
    break;

  case 434:
#line 3928 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = list_append((yyvsp[-2].node), (yyvsp[-1].node));
		    /*%
			$$ = dispatch2(qwords_add, $1, $2);
		    %*/
		    ;}
    break;

  case 435:
#line 3938 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = 0;
		    /*%
			$$ = dispatch0(string_content);
		    %*/
		    ;}
    break;

  case 436:
#line 3946 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = literal_concat((yyvsp[-1].node), (yyvsp[0].node));
		    /*%
			$$ = dispatch2(string_add, $1, $2);
		    %*/
		    ;}
    break;

  case 437:
#line 3956 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = 0;
		    /*%
			$$ = dispatch0(xstring_new);
		    %*/
		    ;}
    break;

  case 438:
#line 3964 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = literal_concat((yyvsp[-1].node), (yyvsp[0].node));
		    /*%
			$$ = dispatch2(xstring_add, $1, $2);
		    %*/
		    ;}
    break;

  case 440:
#line 3975 "parse.y"
    {
			(yyval.node) = lex_strterm;
			lex_strterm = 0;
			lex_state = EXPR_BEG;
		    ;}
    break;

  case 441:
#line 3981 "parse.y"
    {
		    /*%%%*/
			lex_strterm = (yyvsp[-1].node);
			(yyval.node) = NEW_EVSTR((yyvsp[0].node));
		    /*%
			lex_strterm = $<node>2;
			$$ = dispatch1(string_dvar, $3);
		    %*/
		    ;}
    break;

  case 442:
#line 3991 "parse.y"
    {
			(yyval.node) = lex_strterm;
			lex_strterm = 0;
			lex_state = EXPR_BEG;
			COND_PUSH(0);
			CMDARG_PUSH(0);
		    ;}
    break;

  case 443:
#line 3999 "parse.y"
    {
			lex_strterm = (yyvsp[-2].node);
			COND_LEXPOP();
			CMDARG_LEXPOP();
		    /*%%%*/
			if ((yyvsp[-1].node)) (yyvsp[-1].node)->flags &= ~NODE_FL_NEWLINE;
			(yyval.node) = new_evstr((yyvsp[-1].node));
		    /*%
			$$ = dispatch1(string_embexpr, $3);
		    %*/
		    ;}
    break;

  case 444:
#line 4013 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = NEW_GVAR((yyvsp[0].id));
		    /*%
			$$ = dispatch1(var_ref, $1);
		    %*/
		    ;}
    break;

  case 445:
#line 4021 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = NEW_IVAR((yyvsp[0].id));
		    /*%
			$$ = dispatch1(var_ref, $1);
		    %*/
		    ;}
    break;

  case 446:
#line 4029 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = NEW_CVAR((yyvsp[0].id));
		    /*%
			$$ = dispatch1(var_ref, $1);
		    %*/
		    ;}
    break;

  case 448:
#line 4040 "parse.y"
    {
		    /*%%%*/
			lex_state = EXPR_END;
			(yyval.id) = (yyvsp[0].id);
		    /*%
			lex_state = EXPR_END;
			$$ = dispatch1(symbol, $2);
		    %*/
		    ;}
    break;

  case 453:
#line 4058 "parse.y"
    {
		    /*%%%*/
			lex_state = EXPR_END;
			if (!((yyval.node) = (yyvsp[-1].node))) {
			    (yyval.node) = NEW_LIT(ID2SYM(rb_intern("")));
			}
			else {
			    VALUE lit;

			    switch (nd_type((yyval.node))) {
			      case NODE_DSTR:
				nd_set_type((yyval.node), NODE_DSYM);
				break;
			      case NODE_STR:
				lit = (yyval.node)->nd_lit;
				(yyval.node)->nd_lit = ID2SYM(rb_intern_str(lit));
				nd_set_type((yyval.node), NODE_LIT);
				break;
			      default:
				(yyval.node) = NEW_NODE(NODE_DSYM, STR_NEW0(), 1, NEW_LIST((yyval.node)));
				break;
			    }
			}
		    /*%
			lex_state = EXPR_END;
			$$ = dispatch1(dyna_symbol, $2);
		    %*/
		    ;}
    break;

  case 456:
#line 4091 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = negate_lit((yyvsp[0].node));
		    /*%
			$$ = dispatch2(unary, ripper_intern("-@"), $2);
		    %*/
		    ;}
    break;

  case 457:
#line 4099 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = negate_lit((yyvsp[0].node));
		    /*%
			$$ = dispatch2(unary, ripper_intern("-@"), $2);
		    %*/
		    ;}
    break;

  case 463:
#line 4113 "parse.y"
    {ifndef_ripper((yyval.id) = keyword_nil);;}
    break;

  case 464:
#line 4114 "parse.y"
    {ifndef_ripper((yyval.id) = keyword_self);;}
    break;

  case 465:
#line 4115 "parse.y"
    {ifndef_ripper((yyval.id) = keyword_true);;}
    break;

  case 466:
#line 4116 "parse.y"
    {ifndef_ripper((yyval.id) = keyword_false);;}
    break;

  case 467:
#line 4117 "parse.y"
    {ifndef_ripper((yyval.id) = keyword__FILE__);;}
    break;

  case 468:
#line 4118 "parse.y"
    {ifndef_ripper((yyval.id) = keyword__LINE__);;}
    break;

  case 469:
#line 4119 "parse.y"
    {ifndef_ripper((yyval.id) = keyword__ENCODING__);;}
    break;

  case 470:
#line 4123 "parse.y"
    {
		    /*%%%*/
			if (!((yyval.node) = gettable((yyvsp[0].id)))) (yyval.node) = NEW_BEGIN(0);
		    /*%
			$$ = dispatch1(var_ref, $1);
		    %*/
		    ;}
    break;

  case 471:
#line 4133 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = assignable((yyvsp[0].id), 0);
		    /*%
			$$ = dispatch1(var_field, $1);
		    %*/
		    ;}
    break;

  case 474:
#line 4147 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = 0;
		    /*%
			$$ = Qnil;
		    %*/
		    ;}
    break;

  case 475:
#line 4155 "parse.y"
    {
			lex_state = EXPR_BEG;
		    ;}
    break;

  case 476:
#line 4159 "parse.y"
    {
			(yyval.node) = (yyvsp[-1].node);
		    ;}
    break;

  case 477:
#line 4163 "parse.y"
    {
		    /*%%%*/
			yyerrok;
			(yyval.node) = 0;
		    /*%
			yyerrok;
			$$ = Qnil;
		    %*/
		    ;}
    break;

  case 478:
#line 4175 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = (yyvsp[-1].node);
			lex_state = EXPR_BEG;
			command_start = Qtrue;
		    /*%
			$$ = dispatch1(paren, $2);
			lex_state = EXPR_BEG;
			command_start = Qtrue;
		    %*/
		    ;}
    break;

  case 479:
#line 4187 "parse.y"
    {
			(yyval.node) = (yyvsp[-1].node);
		    ;}
    break;

  case 480:
#line 4193 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = new_args((yyvsp[-5].node), (yyvsp[-3].node), (yyvsp[-1].id), 0, (yyvsp[0].id));
		    /*%
			$$ = params_new($1, $3, $5, Qnil, escape_Qundef($6));
		    %*/
		    ;}
    break;

  case 481:
#line 4201 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = new_args((yyvsp[-7].node), (yyvsp[-5].node), (yyvsp[-3].id), (yyvsp[-1].node), (yyvsp[0].id));
		    /*%
			$$ = params_new($1, $3, $5, $7, escape_Qundef($8));
		    %*/
		    ;}
    break;

  case 482:
#line 4209 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = new_args((yyvsp[-3].node), (yyvsp[-1].node), 0, 0, (yyvsp[0].id));
		    /*%
			$$ = params_new($1, $3, Qnil, Qnil, escape_Qundef($4));
		    %*/
		    ;}
    break;

  case 483:
#line 4217 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = new_args((yyvsp[-5].node), (yyvsp[-3].node), 0, (yyvsp[-1].node), (yyvsp[0].id));
		    /*%
			$$ = params_new($1, $3, Qnil, $5, escape_Qundef($6));
		    %*/
		    ;}
    break;

  case 484:
#line 4225 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = new_args((yyvsp[-3].node), 0, (yyvsp[-1].id), 0, (yyvsp[0].id));
		    /*%
			$$ = params_new($1, Qnil, $3, Qnil, escape_Qundef($4));
		    %*/
		    ;}
    break;

  case 485:
#line 4233 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = new_args((yyvsp[-5].node), 0, (yyvsp[-3].id), (yyvsp[-1].node), (yyvsp[0].id));
		    /*%
			$$ = params_new($1, Qnil, $3, $5, escape_Qundef($6));
		    %*/
		    ;}
    break;

  case 486:
#line 4241 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = new_args((yyvsp[-1].node), 0, 0, 0, (yyvsp[0].id));
		    /*%
			$$ = params_new($1, Qnil, Qnil, Qnil,escape_Qundef($2));
		    %*/
		    ;}
    break;

  case 487:
#line 4249 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = new_args(0, (yyvsp[-3].node), (yyvsp[-1].id), 0, (yyvsp[0].id));
		    /*%
			$$ = params_new(Qnil, $1, $3, Qnil, escape_Qundef($4));
		    %*/
		    ;}
    break;

  case 488:
#line 4257 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = new_args(0, (yyvsp[-5].node), (yyvsp[-3].id), (yyvsp[-1].node), (yyvsp[0].id));
		    /*%
			$$ = params_new(Qnil, $1, $3, $5, escape_Qundef($6));
		    %*/
		    ;}
    break;

  case 489:
#line 4265 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = new_args(0, (yyvsp[-1].node), 0, 0, (yyvsp[0].id));
		    /*%
			$$ = params_new(Qnil, $1, Qnil, Qnil,escape_Qundef($2));
		    %*/
		    ;}
    break;

  case 490:
#line 4273 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = new_args(0, (yyvsp[-3].node), 0, (yyvsp[-1].node), (yyvsp[0].id));
		    /*%
			$$ = params_new(Qnil, $1, Qnil, $3, escape_Qundef($4));
		    %*/
		    ;}
    break;

  case 491:
#line 4281 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = new_args(0, 0, (yyvsp[-1].id), 0, (yyvsp[0].id));
		    /*%
			$$ = params_new(Qnil, Qnil, $1, Qnil,escape_Qundef($2));
		    %*/
		    ;}
    break;

  case 492:
#line 4289 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = new_args(0, 0, (yyvsp[-3].id), (yyvsp[-1].node), (yyvsp[0].id));
		    /*%
			$$ = params_new(Qnil, Qnil, $1, $3, escape_Qundef($4));
		    %*/
		    ;}
    break;

  case 493:
#line 4297 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = new_args(0, 0, 0, 0, (yyvsp[0].id));
		    /*%
			$$ = params_new(Qnil, Qnil, Qnil, Qnil, $1);
		    %*/
		    ;}
    break;

  case 494:
#line 4305 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = new_args(0, 0, 0, 0, 0);
		    /*%
			$$ = params_new(Qnil, Qnil, Qnil, Qnil, Qnil);
		    %*/
		    ;}
    break;

  case 495:
#line 4315 "parse.y"
    {
		    /*%%%*/
			yyerror("formal argument cannot be a constant");
			(yyval.id) = 0;
		    /*%
			$$ = dispatch1(param_error, $1);
		    %*/
		    ;}
    break;

  case 496:
#line 4324 "parse.y"
    {
		    /*%%%*/
			yyerror("formal argument cannot be an instance variable");
			(yyval.id) = 0;
		    /*%
			$$ = dispatch1(param_error, $1);
		    %*/
		    ;}
    break;

  case 497:
#line 4333 "parse.y"
    {
		    /*%%%*/
			yyerror("formal argument cannot be a global variable");
			(yyval.id) = 0;
		    /*%
			$$ = dispatch1(param_error, $1);
		    %*/
		    ;}
    break;

  case 498:
#line 4342 "parse.y"
    {
		    /*%%%*/
			yyerror("formal argument cannot be a class variable");
			(yyval.id) = 0;
		    /*%
			$$ = dispatch1(param_error, $1);
		    %*/
		    ;}
    break;

  case 500:
#line 4354 "parse.y"
    {
		    /*%%%*/
			if (!is_local_id((yyvsp[0].id)))
			    yyerror("formal argument must be local variable");
			shadowing_lvar((yyvsp[0].id));
		    /*%
		    %*/
			(yyval.id) = (yyvsp[0].id);
		    ;}
    break;

  case 501:
#line 4366 "parse.y"
    {
		    /*%%%*/
			arg_var((yyvsp[0].id));
			(yyval.node) = NEW_ARGS_AUX((yyvsp[0].id), 1);
		    /*%
		    %*/
		    ;}
    break;

  case 502:
#line 4374 "parse.y"
    {
		    /*%%%*/
			ID tid = internal_id();
			arg_var(tid);
			if (dyna_in_block()) {
			    (yyvsp[-1].node)->nd_value = NEW_DVAR(tid);
			}
			else {
			    (yyvsp[-1].node)->nd_value = NEW_LVAR(tid);
			}
			(yyval.node) = NEW_ARGS_AUX(tid, 1);
			(yyval.node)->nd_next = (yyvsp[-1].node);
		    /*%
			$$ = dispatch1(mlhs_paren, $2);
		    %*/
		    ;}
    break;

  case 504:
#line 4400 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = (yyvsp[-2].node);
			(yyval.node)->nd_plen++;
			(yyval.node)->nd_next = block_append((yyval.node)->nd_next, (yyvsp[0].node)->nd_next);
			rb_gc_force_recycle((VALUE)(yyvsp[0].node));
		    /*%
			$$ = rb_ary_push($1, $3);
		    %*/
		    ;}
    break;

  case 505:
#line 4413 "parse.y"
    {
		    /*%%%*/
			if (!is_local_id((yyvsp[-2].id)))
			    yyerror("formal argument must be local variable");
			shadowing_lvar((yyvsp[-2].id));
			arg_var((yyvsp[-2].id));
			(yyval.node) = NEW_OPT_ARG(0, assignable((yyvsp[-2].id), (yyvsp[0].node)));
		    /*%
			$$ = rb_assoc_new($1, $3);
		    %*/
		    ;}
    break;

  case 506:
#line 4427 "parse.y"
    {
		    /*%%%*/
			if (!is_local_id((yyvsp[-2].id)))
			    yyerror("formal argument must be local variable");
			shadowing_lvar((yyvsp[-2].id));
			arg_var((yyvsp[-2].id));
			(yyval.node) = NEW_OPT_ARG(0, assignable((yyvsp[-2].id), (yyvsp[0].node)));
		    /*%
			$$ = rb_assoc_new($1, $3);
		    %*/
		    ;}
    break;

  case 507:
#line 4441 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = (yyvsp[0].node);
		    /*%
			$$ = rb_ary_new3(1, $1);
		    %*/
		    ;}
    break;

  case 508:
#line 4449 "parse.y"
    {
		    /*%%%*/
			NODE *opts = (yyvsp[-2].node);

			while (opts->nd_next) {
			    opts = opts->nd_next;
			}
			opts->nd_next = (yyvsp[0].node);
			(yyval.node) = (yyvsp[-2].node);
		    /*%
			$$ = rb_ary_push($1, $3);
		    %*/
		    ;}
    break;

  case 509:
#line 4465 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = (yyvsp[0].node);
		    /*%
			$$ = rb_ary_new3(1, $1);
		    %*/
		    ;}
    break;

  case 510:
#line 4473 "parse.y"
    {
		    /*%%%*/
			NODE *opts = (yyvsp[-2].node);

			while (opts->nd_next) {
			    opts = opts->nd_next;
			}
			opts->nd_next = (yyvsp[0].node);
			(yyval.node) = (yyvsp[-2].node);
		    /*%
			$$ = rb_ary_push($1, $3);
		    %*/
		    ;}
    break;

  case 513:
#line 4493 "parse.y"
    {
		    /*%%%*/
			if (!is_local_id((yyvsp[0].id)))
			    yyerror("rest argument must be local variable");
			shadowing_lvar((yyvsp[0].id));
			arg_var((yyvsp[0].id));
			(yyval.id) = (yyvsp[0].id);
		    /*%
			$$ = dispatch1(rest_param, $2);
		    %*/
		    ;}
    break;

  case 514:
#line 4505 "parse.y"
    {
		    /*%%%*/
			(yyval.id) = internal_id();
			arg_var((yyval.id));
		    /*%
			$$ = dispatch1(rest_param, Qnil);
		    %*/
		    ;}
    break;

  case 517:
#line 4520 "parse.y"
    {
		    /*%%%*/
			if (!is_local_id((yyvsp[0].id)))
			    yyerror("block argument must be local variable");
			else if (!dyna_in_block() && local_id((yyvsp[0].id)))
			    yyerror("duplicated block argument name");
			shadowing_lvar((yyvsp[0].id));
			arg_var((yyvsp[0].id));
			(yyval.id) = (yyvsp[0].id);
		    /*%
			$$ = dispatch1(blockarg, $2);
		    %*/
		    ;}
    break;

  case 518:
#line 4536 "parse.y"
    {
			(yyval.id) = (yyvsp[0].id);
		    ;}
    break;

  case 519:
#line 4540 "parse.y"
    {
		    /*%%%*/
			(yyval.id) = 0;
		    /*%
			$$ = Qundef;
		    %*/
		    ;}
    break;

  case 520:
#line 4550 "parse.y"
    {
		    /*%%%*/
			value_expr((yyvsp[0].node));
			(yyval.node) = (yyvsp[0].node);
		        if (!(yyval.node)) (yyval.node) = NEW_NIL();
		    /*%
			$$ = $1;
		    %*/
		    ;}
    break;

  case 521:
#line 4559 "parse.y"
    {lex_state = EXPR_BEG;;}
    break;

  case 522:
#line 4560 "parse.y"
    {
		    /*%%%*/
			if ((yyvsp[-1].node) == 0) {
			    yyerror("can't define singleton method for ().");
			}
			else {
			    switch (nd_type((yyvsp[-1].node))) {
			      case NODE_STR:
			      case NODE_DSTR:
			      case NODE_XSTR:
			      case NODE_DXSTR:
			      case NODE_DREGX:
			      case NODE_LIT:
			      case NODE_ARRAY:
			      case NODE_ZARRAY:
				yyerror("can't define singleton method for literals");
			      default:
				value_expr((yyvsp[-1].node));
				break;
			    }
			}
			(yyval.node) = (yyvsp[-1].node);
		    /*%
			$$ = dispatch1(paren, $3);
		    %*/
		    ;}
    break;

  case 524:
#line 4590 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = (yyvsp[-1].node);
		    /*%
			$$ = dispatch1(assoclist_from_args, $1);
		    %*/
		    ;}
    break;

  case 526:
#line 4607 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = list_concat((yyvsp[-2].node), (yyvsp[0].node));
		    /*%
			$$ = rb_ary_push($1, $3);
		    %*/
		    ;}
    break;

  case 527:
#line 4617 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = list_append(NEW_LIST((yyvsp[-2].node)), (yyvsp[0].node));
		    /*%
			$$ = dispatch2(assoc_new, $1, $3);
		    %*/
		    ;}
    break;

  case 528:
#line 4625 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = list_append(NEW_LIST(NEW_LIT(ID2SYM((yyvsp[-1].id)))), (yyvsp[0].node));
		    /*%
			$$ = dispatch2(assoc_new, $1, $2);
		    %*/
		    ;}
    break;

  case 550:
#line 4681 "parse.y"
    {yyerrok;;}
    break;

  case 553:
#line 4686 "parse.y"
    {yyerrok;;}
    break;

  case 554:
#line 4690 "parse.y"
    {
		    /*%%%*/
			(yyval.node) = 0;
		    /*%
			$$ = Qundef;
		    %*/
		    ;}
    break;


      default: break;
    }

/* Line 1126 of yacc.c.  */
#line 9533 "parse.c"

  yyvsp -= yylen;
  yyssp -= yylen;


  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (YYPACT_NINF < yyn && yyn < YYLAST)
	{
	  int yytype = YYTRANSLATE (yychar);
	  YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
	  YYSIZE_T yysize = yysize0;
	  YYSIZE_T yysize1;
	  int yysize_overflow = 0;
	  char *yymsg = 0;
#	  define YYERROR_VERBOSE_ARGS_MAXIMUM 5
	  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
	  int yyx;

#if 0
	  /* This is so xgettext sees the translatable formats that are
	     constructed on the fly.  */
	  YY_("syntax error, unexpected %s");
	  YY_("syntax error, unexpected %s, expecting %s");
	  YY_("syntax error, unexpected %s, expecting %s or %s");
	  YY_("syntax error, unexpected %s, expecting %s or %s or %s");
	  YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
#endif
	  char *yyfmt;
	  char const *yyf;
	  static char const yyunexpected[] = "syntax error, unexpected %s";
	  static char const yyexpecting[] = ", expecting %s";
	  static char const yyor[] = " or %s";
	  char yyformat[sizeof yyunexpected
			+ sizeof yyexpecting - 1
			+ ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
			   * (sizeof yyor - 1))];
	  char const *yyprefix = yyexpecting;

	  /* Start YYX at -YYN if negative to avoid negative indexes in
	     YYCHECK.  */
	  int yyxbegin = yyn < 0 ? -yyn : 0;

	  /* Stay within bounds of both yycheck and yytname.  */
	  int yychecklim = YYLAST - yyn;
	  int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
	  int yycount = 1;

	  yyarg[0] = yytname[yytype];
	  yyfmt = yystpcpy (yyformat, yyunexpected);

	  for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	      {
		if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
		  {
		    yycount = 1;
		    yysize = yysize0;
		    yyformat[sizeof yyunexpected - 1] = '\0';
		    break;
		  }
		yyarg[yycount++] = yytname[yyx];
		yysize1 = yysize + yytnamerr (0, yytname[yyx]);
		yysize_overflow |= yysize1 < yysize;
		yysize = yysize1;
		yyfmt = yystpcpy (yyfmt, yyprefix);
		yyprefix = yyor;
	      }

	  yyf = YY_(yyformat);
	  yysize1 = yysize + yystrlen (yyf);
	  yysize_overflow |= yysize1 < yysize;
	  yysize = yysize1;

	  if (!yysize_overflow && yysize <= YYSTACK_ALLOC_MAXIMUM)
	    yymsg = (char *) YYSTACK_ALLOC (yysize);
	  if (yymsg)
	    {
	      /* Avoid sprintf, as that infringes on the user's name space.
		 Don't have undefined behavior even if the translation
		 produced a string with the wrong number of "%s"s.  */
	      char *yyp = yymsg;
	      int yyi = 0;
	      while ((*yyp = *yyf))
		{
		  if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		    {
		      yyp += yytnamerr (yyp, yyarg[yyi++]);
		      yyf += 2;
		    }
		  else
		    {
		      yyp++;
		      yyf++;
		    }
		}
	      parser_yyerror (parser, yymsg);
	      YYSTACK_FREE (yymsg);
	    }
	  else
	    {
	      parser_yyerror (parser, YY_("syntax error"));
	      goto yyexhaustedlab;
	    }
	}
      else
#endif /* YYERROR_VERBOSE */
	parser_yyerror (parser, YY_("syntax error"));
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
        {
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
        }
      else
	{
	  yydestruct ("Error: discarding", yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (0)
     goto yyerrorlab;

yyvsp -= yylen;
  yyssp -= yylen;
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping", yystos[yystate], yyvsp);
      YYPOPSTACK;
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


  /* Shift the error token. */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

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

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  parser_yyerror (parser, YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK;
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  return yyresult;
}


#line 4698 "parse.y"

# undef parser
# undef yylex
# undef yylval
# define yylval  (*((YYSTYPE*)(parser->parser_yylval)))

static int parser_regx_options(struct parser_params*);
static int parser_tokadd_string(struct parser_params*,int,int,int,long*,rb_encoding**);
static void parser_tokaddmbc(struct parser_params *parser, int c, rb_encoding *enc);
static int parser_parse_string(struct parser_params*,NODE*);
static int parser_here_document(struct parser_params*,NODE*);


# define nextc()                   parser_nextc(parser)
# define pushback(c)               parser_pushback(parser, c)
# define newtok()                  parser_newtok(parser)
# define tokspace(n)               parser_tokspace(parser, n)
# define tokadd(c)                 parser_tokadd(parser, c)
# define tok_hex(numlen)           parser_tok_hex(parser, numlen)
# define read_escape(flags,e)      parser_read_escape(parser, flags, e)
# define tokadd_escape(e)          parser_tokadd_escape(parser, e)
# define regx_options()            parser_regx_options(parser)
# define tokadd_string(f,t,p,n,e)  parser_tokadd_string(parser,f,t,p,n,e)
# define parse_string(n)           parser_parse_string(parser,n)
# define tokaddmbc(c, enc)         parser_tokaddmbc(parser, c, enc)
# define here_document(n)          parser_here_document(parser,n)
# define heredoc_identifier()      parser_heredoc_identifier(parser)
# define heredoc_restore(n)        parser_heredoc_restore(parser,n)
# define whole_match_p(e,l,i)      parser_whole_match_p(parser,e,l,i)

#ifdef RIPPER
/* FIXME */
# define local_id(x)       1
# define dyna_in_block()   1
#endif /* RIPPER */

#ifndef RIPPER
# define set_yylval_str(x) yylval.node = NEW_STR(x)
# define set_yylval_num(x) yylval.num = x
# define set_yylval_id(x)  yylval.id = x
# define set_yylval_literal(x) yylval.node = NEW_LIT(x)
# define set_yylval_node(x) yylval.node = x
# define yylval_id() yylval.id
#else
# define set_yylval_str(x) (void)(x)
# define set_yylval_num(x) (void)(x)
# define set_yylval_id(x) (void)(x)
# define set_yylval_literal(x) (void)(x)
# define set_yylval_node(x) (void)(x)
# define yylval_id() SYM2ID(yylval.val)
#endif

#ifdef RIPPER
#define ripper_flush(p) (p->tokp = p->parser_lex_p)

static void
ripper_dispatch_scan_event(struct parser_params *parser, int t)
{
    VALUE str;

    if (lex_p < parser->tokp) rb_raise(rb_eRuntimeError, "lex_p < tokp");
    if (lex_p == parser->tokp) return;
    str = STR_NEW(parser->tokp, lex_p - parser->tokp);
    yylval.val = ripper_dispatch1(parser, ripper_token2eventid(t), str);
    ripper_flush(parser);
}

static void
ripper_dispatch_delayed_token(struct parser_params *parser, int t)
{
    int saved_line = ruby_sourceline;
    const char *saved_tokp = parser->tokp;

    ruby_sourceline = parser->delayed_line;
    parser->tokp = lex_pbeg + parser->delayed_col;
    yylval.val = ripper_dispatch1(parser, ripper_token2eventid(t), parser->delayed);
    parser->delayed = Qnil;
    ruby_sourceline = saved_line;
    parser->tokp = saved_tokp;
}
#endif /* RIPPER */

#include "ruby/regex.h"
#include "ruby/util.h"

/* We remove any previous definition of `SIGN_EXTEND_CHAR',
   since ours (we hope) works properly with all combinations of
   machines, compilers, `char' and `unsigned char' argument types.
   (Per Bothner suggested the basic approach.)  */
#undef SIGN_EXTEND_CHAR
#if __STDC__
# define SIGN_EXTEND_CHAR(c) ((signed char)(c))
#else  /* not __STDC__ */
/* As in Harbison and Steele.  */
# define SIGN_EXTEND_CHAR(c) ((((unsigned char)(c)) ^ 128) - 128)
#endif

#define parser_encoding_name()  (parser->enc->name)
#define parser_mbclen()  mbclen((lex_p-1),lex_pend,parser->enc)
#define parser_precise_mbclen()  rb_enc_precise_mbclen((lex_p-1),lex_pend,parser->enc)
#define is_identchar(p,e,enc) (rb_enc_isalnum(*p,enc) || (*p) == '_' || !ISASCII(*p))
#define parser_is_identchar() (!parser->eofp && is_identchar((lex_p-1),lex_pend,parser->enc))

#define parser_isascii() ISASCII(*(lex_p-1))

#ifndef RIPPER
static int
token_info_get_column(struct parser_params *parser, const char *token)
{
    int column = 1;
    const char *p, *pend = lex_p - strlen(token);
    for (p = lex_pbeg; p < pend; p++) {
	if (*p == '\t') {
	    column = (((column - 1) / 8) + 1) * 8;
	}
	column++;
    }
    return column;
}

static int
token_info_has_nonspaces(struct parser_params *parser, const char *token)
{
    const char *p, *pend = lex_p - strlen(token);
    for (p = lex_pbeg; p < pend; p++) {
	if (*p != ' ' && *p != '\t') {
	    return 1;
	}
    }
    return 0;
}

static void
token_info_push(struct parser_params *parser, const char *token)
{
    token_info *ptinfo;

    if (compile_for_eval) return;
    ptinfo = ALLOC(token_info);
    ptinfo->token = token;
    ptinfo->linenum = ruby_sourceline;
    ptinfo->column = token_info_get_column(parser, token);
    ptinfo->nonspc = token_info_has_nonspaces(parser, token);
    ptinfo->next = parser->parser_token_info;

    parser->parser_token_info = ptinfo;
}

static void
token_info_pop(struct parser_params *parser, const char *token)
{
    int linenum;
    token_info *ptinfo = parser->parser_token_info;

    if (!ptinfo) return;
    parser->parser_token_info = ptinfo->next;
    if (token_info_get_column(parser, token) == ptinfo->column) { /* OK */
	goto finish;
    }
    linenum = ruby_sourceline;
    if (linenum == ptinfo->linenum) { /* SKIP */
	goto finish;
    }
    if (token_info_has_nonspaces(parser, token) || ptinfo->nonspc) { /* SKIP */
	goto finish;
    }
    rb_compile_warning(ruby_sourcefile, linenum,
               "mismatched indentations at '%s' with '%s' at %d",
	       token, ptinfo->token, ptinfo->linenum);

  finish:
    xfree(ptinfo);
}
#endif	/* RIPPER */

static int
parser_yyerror(struct parser_params *parser, const char *msg)
{
#ifndef RIPPER
    const int max_line_margin = 30;
    const char *p, *pe;
    char *buf;
    int len, i;

    compile_error(PARSER_ARG "%s", msg);
    p = lex_p;
    while (lex_pbeg <= p) {
	if (*p == '\n') break;
	p--;
    }
    p++;

    pe = lex_p;
    while (pe < lex_pend) {
	if (*pe == '\n') break;
	pe++;
    }

    len = pe - p;
    if (len > 4) {
	char *p2;
	const char *pre = "", *post = "";

	if (len > max_line_margin * 2 + 10) {
	    if (lex_p - p > max_line_margin) {
		p = rb_enc_prev_char(p, lex_p - max_line_margin, pe, rb_enc_get(lex_lastline));
		pre = "...";
	    }
	    if (pe - lex_p > max_line_margin) {
		pe = rb_enc_prev_char(lex_p, lex_p + max_line_margin, pe, rb_enc_get(lex_lastline));
		post = "...";
	    }
	    len = pe - p;
	}
	buf = ALLOCA_N(char, len+2);
	MEMCPY(buf, p, char, len);
	buf[len] = '\0';
	rb_compile_error_append("%s%s%s", pre, buf, post);

	i = lex_p - p;
	p2 = buf; pe = buf + len;

	while (p2 < pe) {
	    if (*p2 != '\t') *p2 = ' ';
	    p2++;
	}
	buf[i] = '^';
	buf[i+1] = '\0';
	rb_compile_error_append("%s%s", pre, buf);
    }
#else
    dispatch1(parse_error, STR_NEW2(msg));
#endif /* !RIPPER */
    return 0;
}

static void parser_prepare(struct parser_params *parser);

#ifndef RIPPER
VALUE ruby_suppress_tracing(VALUE (*func)(VALUE, int), VALUE arg, int always);

static VALUE
debug_lines(const char *f)
{
    if (rb_const_defined_at(rb_cObject, rb_intern("SCRIPT_LINES__"))) {
	VALUE hash = rb_const_get_at(rb_cObject, rb_intern("SCRIPT_LINES__"));
	if (TYPE(hash) == T_HASH) {
	    VALUE fname = rb_str_new2(f);
	    VALUE lines = rb_ary_new();
	    rb_hash_aset(hash, fname, lines);
	    return lines;
	}
    }
    return 0;
}

static VALUE
coverage(const char *f, int n)
{
    extern VALUE rb_get_coverages(void);
    VALUE coverages = rb_get_coverages();
    if (RTEST(coverages) && RBASIC(coverages)->klass == 0) {
	VALUE fname = rb_str_new2(f);
	VALUE lines = rb_ary_new2(n);
	int i;
	RBASIC(lines)->klass = 0;
	for (i = 0; i < n; i++) RARRAY_PTR(lines)[i] = Qnil;
	RARRAY(lines)->as.heap.len = n;
	rb_hash_aset(coverages, fname, lines);
	return lines;
    }
    return 0;
}

static int
e_option_supplied(struct parser_params *parser)
{
    if (strcmp(ruby_sourcefile, "-e") == 0)
	return Qtrue;
    return Qfalse;
}

static VALUE
yycompile0(VALUE arg, int tracing)
{
    int n;
    NODE *tree;
    struct parser_params *parser = (struct parser_params *)arg;

    if (!compile_for_eval && rb_safe_level() == 0) {
	ruby_debug_lines = debug_lines(ruby_sourcefile);
	if (ruby_debug_lines && ruby_sourceline > 0) {
	    VALUE str = STR_NEW0();
	    n = ruby_sourceline;
	    do {
		rb_ary_push(ruby_debug_lines, str);
	    } while (--n);
	}

	if (!e_option_supplied(parser)) {
	    ruby_coverage = coverage(ruby_sourcefile, ruby_sourceline);
	}
    }

    parser_prepare(parser);
    deferred_nodes = 0;
    n = yyparse((void*)parser);
    ruby_debug_lines = 0;
    ruby_coverage = 0;
    compile_for_eval = 0;

    lex_strterm = 0;
    lex_p = lex_pbeg = lex_pend = 0;
    lex_lastline = lex_nextline = 0;
    if (parser->nerr) {
	return 0;
    }
    tree = ruby_eval_tree;
    if (!tree) {
	tree = NEW_NIL();
    }
    if (ruby_eval_tree_begin) {
	NODE *scope = ruby_eval_tree;

        if (scope) {
	    scope->nd_body = NEW_PRELUDE(ruby_eval_tree_begin, scope->nd_body);
	}
	tree = scope;
    }
    else {
	tree = ruby_eval_tree;
    }
    return (VALUE)tree;
}

static NODE*
yycompile(struct parser_params *parser, const char *f, int line)
{
    ruby_sourcefile = ruby_strdup(f);
    ruby_sourceline = line - 1;
    return (NODE *)ruby_suppress_tracing(yycompile0, (VALUE)parser, Qtrue);
}
#endif /* !RIPPER */

static VALUE
lex_get_str(struct parser_params *parser, VALUE s)
{
    char *beg, *end, *pend;

    beg = RSTRING_PTR(s);
    if (lex_gets_ptr) {
	if (RSTRING_LEN(s) == lex_gets_ptr) return Qnil;
	beg += lex_gets_ptr;
    }
    pend = RSTRING_PTR(s) + RSTRING_LEN(s);
    end = beg;
    while (end < pend) {
	if (*end++ == '\n') break;
    }
    lex_gets_ptr = end - RSTRING_PTR(s);
    return rb_enc_str_new(beg, end - beg, rb_enc_get(s));
}

static VALUE
lex_getline(struct parser_params *parser)
{
    VALUE line = (*parser->parser_lex_gets)(parser, parser->parser_lex_input);
#ifndef RIPPER
    if (ruby_debug_lines && !NIL_P(line)) {
	rb_ary_push(ruby_debug_lines, line);
    }
    if (ruby_coverage && !NIL_P(line)) {
	rb_ary_push(ruby_coverage, Qnil);
    }
#endif
    return line;
}

#ifndef RIPPER
NODE*
rb_compile_string(const char *f, VALUE s, int line)
{
    VALUE volatile vparser = rb_parser_new();

    return rb_parser_compile_string(vparser, f, s, line);
}

NODE*
rb_parser_compile_string(volatile VALUE vparser, const char *f, VALUE s, int line)
{
    struct parser_params *parser;
    NODE *node;
    volatile VALUE tmp;

    Data_Get_Struct(vparser, struct parser_params, parser);
    lex_gets = lex_get_str;
    lex_gets_ptr = 0;
    lex_input = s;
    lex_pbeg = lex_p = lex_pend = 0;
    compile_for_eval = rb_parse_in_eval();

    node = yycompile(parser, f, line);
    tmp = vparser; /* prohibit tail call optimization */

    return node;
}

NODE*
rb_compile_cstr(const char *f, const char *s, int len, int line)
{
    return rb_compile_string(f, rb_str_new(s, len), line);
}

NODE*
rb_parser_compile_cstr(volatile VALUE vparser, const char *f, const char *s, int len, int line)
{
    return rb_parser_compile_string(vparser, f, rb_str_new(s, len), line);
}

static VALUE
lex_io_gets(struct parser_params *parser, VALUE io)
{
    return rb_io_gets(io);
}

NODE*
rb_compile_file(const char *f, VALUE file, int start)
{
    VALUE volatile vparser = rb_parser_new();

    return rb_parser_compile_file(vparser, f, file, start);
}

NODE*
rb_parser_compile_file(volatile VALUE vparser, const char *f, VALUE file, int start)
{
    struct parser_params *parser;
    volatile VALUE tmp;
    NODE *node;

    Data_Get_Struct(vparser, struct parser_params, parser);
    lex_gets = lex_io_gets;
    lex_input = file;
    lex_pbeg = lex_p = lex_pend = 0;

    node = yycompile(parser, f, start);
    tmp = vparser; /* prohibit tail call optimization */

    return node;
}
#endif  /* !RIPPER */

#define STR_FUNC_ESCAPE 0x01
#define STR_FUNC_EXPAND 0x02
#define STR_FUNC_REGEXP 0x04
#define STR_FUNC_QWORDS 0x08
#define STR_FUNC_SYMBOL 0x10
#define STR_FUNC_INDENT 0x20

enum string_type {
    str_squote = (0),
    str_dquote = (STR_FUNC_EXPAND),
    str_xquote = (STR_FUNC_EXPAND),
    str_regexp = (STR_FUNC_REGEXP|STR_FUNC_ESCAPE|STR_FUNC_EXPAND),
    str_sword  = (STR_FUNC_QWORDS),
    str_dword  = (STR_FUNC_QWORDS|STR_FUNC_EXPAND),
    str_ssym   = (STR_FUNC_SYMBOL),
    str_dsym   = (STR_FUNC_SYMBOL|STR_FUNC_EXPAND)
};

static VALUE
parser_str_new(const char *p, long n, rb_encoding *enc, int func, rb_encoding *enc0)
{
    VALUE str;

    str = rb_enc_str_new(p, n, enc);
    if (!(func & STR_FUNC_REGEXP) && rb_enc_asciicompat(enc)) {
	if (rb_enc_str_coderange(str) == ENC_CODERANGE_7BIT) {
	}
	else if (enc0 == rb_usascii_encoding() && enc != rb_utf8_encoding()) {
	    rb_enc_associate(str, rb_ascii8bit_encoding());
	}
    }

    return str;
}

#define lex_goto_eol(parser) (parser->parser_lex_p = parser->parser_lex_pend)
#define peek(c) (lex_p < lex_pend && (c) == *lex_p)

static inline int
parser_nextc(struct parser_params *parser)
{
    int c;

    if (lex_p == lex_pend) {
	VALUE v = lex_nextline;
	lex_nextline = 0;
	if (!v) {
	    if (parser->eofp)
		return -1;

	    if (!lex_input || NIL_P(v = lex_getline(parser))) {
		parser->eofp = Qtrue;
		lex_goto_eol(parser);
		return -1;
	    }
	}
	{
#ifdef RIPPER
	    if (parser->tokp < lex_pend) {
		if (NIL_P(parser->delayed)) {
		    parser->delayed = rb_str_buf_new(1024);
		    rb_str_buf_cat(parser->delayed,
				   parser->tokp, lex_pend - parser->tokp);
		    parser->delayed_line = ruby_sourceline;
		    parser->delayed_col = parser->tokp - lex_pbeg;
		}
		else {
		    rb_str_buf_cat(parser->delayed,
				   parser->tokp, lex_pend - parser->tokp);
		}
	    }
#endif
	    if (heredoc_end > 0) {
		ruby_sourceline = heredoc_end;
		heredoc_end = 0;
	    }
	    ruby_sourceline++;
	    parser->line_count++;
	    lex_pbeg = lex_p = RSTRING_PTR(v);
	    lex_pend = lex_p + RSTRING_LEN(v);
#ifdef RIPPER
	    ripper_flush(parser);
#endif
	    lex_lastline = v;
	}
    }
    c = (unsigned char)*lex_p++;
    if (c == '\r' && peek('\n')) {
	lex_p++;
	c = '\n';
    }

    return c;
}

static void
parser_pushback(struct parser_params *parser, int c)
{
    if (c == -1) return;
    lex_p--;
    if (lex_p > lex_pbeg && lex_p[0] == '\n' && lex_p[-1] == '\r') {
	lex_p--;
    }
}

#define was_bol() (lex_p == lex_pbeg + 1)

#define tokfix() (tokenbuf[tokidx]='\0')
#define tok() tokenbuf
#define toklen() tokidx
#define toklast() (tokidx>0?tokenbuf[tokidx-1]:0)

static char*
parser_newtok(struct parser_params *parser)
{
    tokidx = 0;
    if (!tokenbuf) {
	toksiz = 60;
	tokenbuf = ALLOC_N(char, 60);
    }
    if (toksiz > 4096) {
	toksiz = 60;
	REALLOC_N(tokenbuf, char, 60);
    }
    return tokenbuf;
}

static char *
parser_tokspace(struct parser_params *parser, int n)
{
    tokidx += n;

    if (tokidx >= toksiz) {
	do {toksiz *= 2;} while (toksiz < tokidx);
	REALLOC_N(tokenbuf, char, toksiz);
    }
    return &tokenbuf[tokidx-n];
}

static void
parser_tokadd(struct parser_params *parser, int c)
{
    tokenbuf[tokidx++] = (char)c;
    if (tokidx >= toksiz) {
	toksiz *= 2;
	REALLOC_N(tokenbuf, char, toksiz);
    }
}

static int
parser_tok_hex(struct parser_params *parser, int *numlen)
{
    int c;

    c = scan_hex(lex_p, 2, numlen);
    if (!*numlen) {
	yyerror("invalid hex escape");
	return 0;
    }
    lex_p += *numlen;
    return c;
}

#define tokcopy(n) memcpy(tokspace(n), lex_p - (n), (n))

static int
parser_tokadd_utf8(struct parser_params *parser, rb_encoding **encp,
                   int string_literal, int symbol_literal, int regexp_literal)
{
    /*
     * If string_literal is true, then we allow multiple codepoints
     * in \u{}, and add the codepoints to the current token.
     * Otherwise we're parsing a character literal and return a single
     * codepoint without adding it
     */

    int codepoint;
    int numlen;

    if (regexp_literal) { tokadd('\\'); tokadd('u'); }

    if (peek('{')) {  /* handle \u{...} form */
	do {
            if (regexp_literal) { tokadd(*lex_p); }
	    nextc();
	    codepoint = scan_hex(lex_p, 6, &numlen);
	    if (numlen == 0)  {
		yyerror("invalid Unicode escape");
		return 0;
	    }
	    if (codepoint > 0x10ffff) {
		yyerror("invalid Unicode codepoint (too large)");
		return 0;
	    }
	    lex_p += numlen;
            if (regexp_literal) {
                tokcopy(numlen);
            }
            else if (codepoint >= 0x80) {
		*encp = UTF8_ENC();
		if (string_literal) tokaddmbc(codepoint, *encp);
	    }
	    else if (string_literal) {
		if (codepoint == 0 && symbol_literal) {
		    yyerror("symbol cannot contain '\\u{0}'");
		    return 0;
		}

		tokadd(codepoint);
	    }
	} while (string_literal && (peek(' ') || peek('\t')));

	if (!peek('}')) {
	    yyerror("unterminated Unicode escape");
	    return 0;
	}

        if (regexp_literal) { tokadd('}'); }
	nextc();
    }
    else {			/* handle \uxxxx form */
	codepoint = scan_hex(lex_p, 4, &numlen);
	if (numlen < 4) {
	    yyerror("invalid Unicode escape");
	    return 0;
	}
	lex_p += 4;
        if (regexp_literal) {
            tokcopy(4);
        }
	else if (codepoint >= 0x80) {
	    *encp = UTF8_ENC();
	    if (string_literal) tokaddmbc(codepoint, *encp);
	}
	else if (string_literal) {
	    if (codepoint == 0 && symbol_literal) {
		yyerror("symbol cannot contain '\\u0000'");
		return 0;
	    }

	    tokadd(codepoint);
	}
    }

    return codepoint;
}

#define ESCAPE_CONTROL 1
#define ESCAPE_META    2

static int
parser_read_escape(struct parser_params *parser, int flags,
		   rb_encoding **encp)
{
    int c;
    int numlen;

    switch (c = nextc()) {
      case '\\':	/* Backslash */
	return c;

      case 'n':	/* newline */
	return '\n';

      case 't':	/* horizontal tab */
	return '\t';

      case 'r':	/* carriage-return */
	return '\r';

      case 'f':	/* form-feed */
	return '\f';

      case 'v':	/* vertical tab */
	return '\13';

      case 'a':	/* alarm(bell) */
	return '\007';

      case 'e':	/* escape */
	return 033;

      case '0': case '1': case '2': case '3': /* octal constant */
      case '4': case '5': case '6': case '7':
	if (flags & (ESCAPE_CONTROL|ESCAPE_META)) goto eof;
	{
	    int numlen;

	    pushback(c);
	    c = scan_oct(lex_p, 3, &numlen);
	    lex_p += numlen;
	}
	return c;

      case 'x':	/* hex constant */
	if (flags & (ESCAPE_CONTROL|ESCAPE_META)) goto eof;
	c = tok_hex(&numlen);
	if (numlen == 0) return 0;
	return c;

      case 'b':	/* backspace */
	return '\010';

      case 's':	/* space */
	return ' ';

      case 'M':
	if (flags & ESCAPE_META) goto eof;
	if ((c = nextc()) != '-') {
	    pushback(c);
	    goto eof;
	}
	if ((c = nextc()) == '\\') {
	    return read_escape(flags|ESCAPE_META, encp) | 0x80;
	}
	else if (c == -1 || !ISASCII(c)) goto eof;
	else {
	    return ((c & 0xff) | 0x80);
	}

      case 'C':
	if ((c = nextc()) != '-') {
	    pushback(c);
	    goto eof;
	}
      case 'c':
	if (flags & ESCAPE_CONTROL) goto eof;
	if ((c = nextc())== '\\') {
	    c = read_escape(flags|ESCAPE_CONTROL, encp);
	}
	else if (c == '?')
	    return 0177;
	else if (c == -1 || !ISASCII(c)) goto eof;
	return c & 0x9f;

      eof:
      case -1:
        yyerror("Invalid escape character syntax");
	return '\0';

      default:
	return c;
    }
}

static void
parser_tokaddmbc(struct parser_params *parser, int c, rb_encoding *enc)
{
    int len = rb_enc_codelen(c, enc);
    rb_enc_mbcput(c, tokspace(len), enc);
}

static int
parser_tokadd_escape(struct parser_params *parser, rb_encoding **encp)
{
    int c;
    int flags = 0;

  first:
    switch (c = nextc()) {
      case '\n':
	return 0;		/* just ignore */

      case '0': case '1': case '2': case '3': /* octal constant */
      case '4': case '5': case '6': case '7':
	if (flags & (ESCAPE_CONTROL|ESCAPE_META)) goto eof;
	{
	    int numlen;
	    int oct;

	    oct = scan_oct(--lex_p, 3, &numlen);
	    if (numlen == 0) goto eof;
	    lex_p += numlen;
	    tokcopy(numlen + 1);
	}
	return 0;

      case 'x':	/* hex constant */
	if (flags & (ESCAPE_CONTROL|ESCAPE_META)) goto eof;
	{
	    int numlen;
	    int hex;

	    hex = tok_hex(&numlen);
	    if (numlen == 0) goto eof;
	    tokcopy(numlen + 2);
	}
	return 0;

      case 'M':
	if (flags & ESCAPE_META) goto eof;
	if ((c = nextc()) != '-') {
	    pushback(c);
	    goto eof;
	}
	tokcopy(3);
	flags |= ESCAPE_META;
	goto escaped;

      case 'C':
	if (flags & ESCAPE_CONTROL) goto eof;
	if ((c = nextc()) != '-') {
	    pushback(c);
	    goto eof;
	}
	tokcopy(3);
	goto escaped;

      case 'c':
	if (flags & ESCAPE_CONTROL) goto eof;
	tokcopy(2);
	flags |= ESCAPE_CONTROL;
      escaped:
	if ((c = nextc()) == '\\') {
	    goto first;
	}
	else if (c == -1) goto eof;
	tokadd(c);
	return 0;

      eof:
      case -1:
        yyerror("Invalid escape character syntax");
	return -1;

      default:
        tokadd('\\');
	tokadd(c);
    }
    return 0;
}

extern int rb_char_to_option_kcode(int c, int *option, int *kcode);

static int
parser_regx_options(struct parser_params *parser)
{
    int kcode = 0;
    int options = 0;
    int c, opt, kc;

    newtok();
    while (c = nextc(), ISALPHA(c)) {
        if (c == 'o') {
            options |= RE_OPTION_ONCE;
        }
        else if (rb_char_to_option_kcode(c, &opt, &kc)) {
            options |= opt;
	    if (kc >= 0) kcode = c;
        }
        else {
	    tokadd(c);
        }
    }
    pushback(c);
    if (toklen()) {
	tokfix();
	compile_error(PARSER_ARG "unknown regexp option%s - %s",
		      toklen() > 1 ? "s" : "", tok());
    }
    return options | RE_OPTION_ENCODING(kcode);
}

static void
dispose_string(VALUE str)
{
    /* TODO: should use another API? */
    if (RBASIC(str)->flags & RSTRING_NOEMBED)
	xfree(RSTRING_PTR(str));
    rb_gc_force_recycle(str);
}

static int
parser_tokadd_mbchar(struct parser_params *parser, int c)
{
    int len = parser_precise_mbclen();
    if (!MBCLEN_CHARFOUND_P(len)) {
	compile_error(PARSER_ARG "invalid multibyte char (%s)", parser_encoding_name());
	return -1;
    }
    tokadd(c);
    lex_p += --len;
    if (len > 0) tokcopy(len);
    return c;
}

#define tokadd_mbchar(c) parser_tokadd_mbchar(parser, c)

static int
parser_tokadd_string(struct parser_params *parser,
		     int func, int term, int paren, long *nest,
		     rb_encoding **encp)
{
    int c;
    int has_nonascii = 0;
    rb_encoding *enc = *encp;
    char *errbuf = 0;
    static const char mixed_msg[] = "%s mixed within %s source";

#define mixed_error(enc1, enc2) if (!errbuf) {	\
	int len = sizeof(mixed_msg) - 4;	\
	len += strlen(rb_enc_name(enc1));	\
	len += strlen(rb_enc_name(enc2));	\
	errbuf = ALLOCA_N(char, len);		\
	snprintf(errbuf, len, mixed_msg,	\
		 rb_enc_name(enc1),		\
		 rb_enc_name(enc2));		\
	yyerror(errbuf);			\
    }
#define mixed_escape(beg, enc1, enc2) do {	\
	const char *pos = lex_p;		\
	lex_p = beg;				\
	mixed_error(enc1, enc2);		\
	lex_p = pos;				\
    } while (0)

    while ((c = nextc()) != -1) {
	if (paren && c == paren) {
	    ++*nest;
	}
	else if (c == term) {
	    if (!nest || !*nest) {
		pushback(c);
		break;
	    }
	    --*nest;
	}
	else if ((func & STR_FUNC_EXPAND) && c == '#' && lex_p < lex_pend) {
	    int c2 = *lex_p;
	    if (c2 == '$' || c2 == '@' || c2 == '{') {
		pushback(c);
		break;
	    }
	}
	else if (c == '\\') {
	    const char *beg = lex_p - 1;
	    c = nextc();
	    switch (c) {
	      case '\n':
		if (func & STR_FUNC_QWORDS) break;
		if (func & STR_FUNC_EXPAND) continue;
		tokadd('\\');
		break;

	      case '\\':
		if (func & STR_FUNC_ESCAPE) tokadd(c);
		break;

	      case 'u':
		if ((func & STR_FUNC_EXPAND) == 0) {
		    tokadd('\\');
		    break;
		}
		parser_tokadd_utf8(parser, &enc, 1,
				   func & STR_FUNC_SYMBOL,
                                   func & STR_FUNC_REGEXP);
		if (has_nonascii && enc != *encp) {
		    mixed_escape(beg, enc, *encp);
		}
		continue;

	      default:
		if (func & STR_FUNC_REGEXP) {
		    pushback(c);
		    if ((c = tokadd_escape(&enc)) < 0)
			return -1;
		    if (has_nonascii && enc != *encp) {
			mixed_escape(beg, enc, *encp);
		    }
		    continue;
		}
		else if (func & STR_FUNC_EXPAND) {
		    pushback(c);
		    if (func & STR_FUNC_ESCAPE) tokadd('\\');
		    c = read_escape(0, &enc);
		}
		else if ((func & STR_FUNC_QWORDS) && ISSPACE(c)) {
		    /* ignore backslashed spaces in %w */
		}
		else if (c != term && !(paren && c == paren)) {
		    tokadd('\\');
		}
	    }
	}
	else if (!parser_isascii()) {
	    has_nonascii = 1;
	    if (enc != *encp) {
		mixed_error(enc, *encp);
		continue;
	    }
	    if (tokadd_mbchar(c) == -1) return -1;
	    continue;
	}
	else if ((func & STR_FUNC_QWORDS) && ISSPACE(c)) {
	    pushback(c);
	    break;
	}
	if (!c && (func & STR_FUNC_SYMBOL)) {
	    func &= ~STR_FUNC_SYMBOL;
	    compile_error(PARSER_ARG "symbol cannot contain '\\0'");
	    continue;
	}
        if (c & 0x80) {
            has_nonascii = 1;
	    if (enc != *encp) {
		mixed_error(enc, *encp);
		continue;
	    }
        }
	tokadd(c);
    }
    *encp = enc;
    return c;
}

#define NEW_STRTERM(func, term, paren) \
	rb_node_newnode(NODE_STRTERM, (func), (term) | ((paren) << (CHAR_BIT * 2)), 0)

static int
parser_parse_string(struct parser_params *parser, NODE *quote)
{
    int func = quote->nd_func;
    int term = nd_term(quote);
    int paren = nd_paren(quote);
    int c, space = 0;
    rb_encoding *enc = parser->enc;

    if (func == -1) return tSTRING_END;
    c = nextc();
    if ((func & STR_FUNC_QWORDS) && ISSPACE(c)) {
	do {c = nextc();} while (ISSPACE(c));
	space = 1;
    }
    if (c == term && !quote->nd_nest) {
	if (func & STR_FUNC_QWORDS) {
	    quote->nd_func = -1;
	    return ' ';
	}
	if (!(func & STR_FUNC_REGEXP)) return tSTRING_END;
        set_yylval_num(regx_options());
	return tREGEXP_END;
    }
    if (space) {
	pushback(c);
	return ' ';
    }
    newtok();
    if ((func & STR_FUNC_EXPAND) && c == '#') {
	switch (c = nextc()) {
	  case '$':
	  case '@':
	    pushback(c);
	    return tSTRING_DVAR;
	  case '{':
	    return tSTRING_DBEG;
	}
	tokadd('#');
    }
    pushback(c);
    if (tokadd_string(func, term, paren, &quote->nd_nest,
		      &enc) == -1) {
	ruby_sourceline = nd_line(quote);
	if (func & STR_FUNC_REGEXP) {
	    if (parser->eofp)
		compile_error(PARSER_ARG "unterminated regexp meets end of file");
	    return tREGEXP_END;
	}
	else {
	    if (parser->eofp)
		compile_error(PARSER_ARG "unterminated string meets end of file");
	    return tSTRING_END;
	}
    }

    tokfix();
    set_yylval_str(STR_NEW3(tok(), toklen(), enc, func));
    return tSTRING_CONTENT;
}

static int
parser_heredoc_identifier(struct parser_params *parser)
{
    int c = nextc(), term, func = 0, len;

    if (c == '-') {
	c = nextc();
	func = STR_FUNC_INDENT;
    }
    switch (c) {
      case '\'':
	func |= str_squote; goto quoted;
      case '"':
	func |= str_dquote; goto quoted;
      case '`':
	func |= str_xquote;
      quoted:
	newtok();
	tokadd(func);
	term = c;
	while ((c = nextc()) != -1 && c != term) {
	    if (tokadd_mbchar(c) == -1) return 0;
	}
	if (c == -1) {
	    compile_error(PARSER_ARG "unterminated here document identifier");
	    return 0;
	}
	break;

      default:
	if (!parser_is_identchar()) {
	    pushback(c);
	    if (func & STR_FUNC_INDENT) {
		pushback('-');
	    }
	    return 0;
	}
	newtok();
	term = '"';
	tokadd(func |= str_dquote);
	do {
	    if (tokadd_mbchar(c) == -1) return 0;
	} while ((c = nextc()) != -1 && parser_is_identchar());
	pushback(c);
	break;
    }

    tokfix();
#ifdef RIPPER
    ripper_dispatch_scan_event(parser, tHEREDOC_BEG);
#endif
    len = lex_p - lex_pbeg;
    lex_goto_eol(parser);
    lex_strterm = rb_node_newnode(NODE_HEREDOC,
				  STR_NEW(tok(), toklen()),	/* nd_lit */
				  len,				/* nd_nth */
				  lex_lastline);		/* nd_orig */
    nd_set_line(lex_strterm, ruby_sourceline);
#ifdef RIPPER
    ripper_flush(parser);
#endif
    return term == '`' ? tXSTRING_BEG : tSTRING_BEG;
}

static void
parser_heredoc_restore(struct parser_params *parser, NODE *here)
{
    VALUE line;

#ifdef RIPPER
    if (!NIL_P(parser->delayed))
	ripper_dispatch_delayed_token(parser, tSTRING_CONTENT);
    lex_goto_eol(parser);
    ripper_dispatch_scan_event(parser, tHEREDOC_END);
#endif
    line = here->nd_orig;
    lex_lastline = line;
    lex_pbeg = RSTRING_PTR(line);
    lex_pend = lex_pbeg + RSTRING_LEN(line);
    lex_p = lex_pbeg + here->nd_nth;
    heredoc_end = ruby_sourceline;
    ruby_sourceline = nd_line(here);
    dispose_string(here->nd_lit);
    rb_gc_force_recycle((VALUE)here);
#ifdef RIPPER
    ripper_flush(parser);
#endif
}

static int
parser_whole_match_p(struct parser_params *parser,
    const char *eos, int len, int indent)
{
    const char *p = lex_pbeg;
    int n;

    if (indent) {
	while (*p && ISSPACE(*p)) p++;
    }
    n= lex_pend - (p + len);
    if (n < 0 || (n > 0 && p[len] != '\n' && p[len] != '\r')) return Qfalse;
    if (strncmp(eos, p, len) == 0) return Qtrue;
    return Qfalse;
}

static int
parser_here_document(struct parser_params *parser, NODE *here)
{
    int c, func, indent = 0;
    const char *eos, *p, *pend;
    long len;
    VALUE str = 0;

    eos = RSTRING_PTR(here->nd_lit);
    len = RSTRING_LEN(here->nd_lit) - 1;
    indent = (func = *eos++) & STR_FUNC_INDENT;

    if ((c = nextc()) == -1) {
      error:
	compile_error(PARSER_ARG "can't find string \"%s\" anywhere before EOF", eos);
      restore:
	heredoc_restore(lex_strterm);
	lex_strterm = 0;
	return 0;
    }
    if (was_bol() && whole_match_p(eos, len, indent)) {
	heredoc_restore(lex_strterm);
	return tSTRING_END;
    }

    if (!(func & STR_FUNC_EXPAND)) {
	do {
	    p = RSTRING_PTR(lex_lastline);
	    pend = lex_pend;
	    if (pend > p) {
		switch (pend[-1]) {
		  case '\n':
		    if (--pend == p || pend[-1] != '\r') {
			pend++;
			break;
		    }
		  case '\r':
		    --pend;
		}
	    }
	    if (str)
		rb_str_cat(str, p, pend - p);
	    else
		str = STR_NEW(p, pend - p);
	    if (pend < lex_pend) rb_str_cat(str, "\n", 1);
	    lex_goto_eol(parser);
	    if (nextc() == -1) {
		if (str) dispose_string(str);
		goto error;
	    }
	} while (!whole_match_p(eos, len, indent));
    }
    else {
	/*	int mb = ENC_CODERANGE_7BIT, *mbp = &mb;*/
	rb_encoding *enc = parser->enc;
	newtok();
	if (c == '#') {
	    switch (c = nextc()) {
	      case '$':
	      case '@':
		pushback(c);
		return tSTRING_DVAR;
	      case '{':
		return tSTRING_DBEG;
	    }
	    tokadd('#');
	}
	do {
	    pushback(c);
	    if ((c = tokadd_string(func, '\n', 0, NULL, &enc)) == -1) {
		if (parser->eofp) goto error;
		goto restore;
	    }
	    if (c != '\n') {
		set_yylval_str(STR_NEW3(tok(), toklen(), enc, func));
		return tSTRING_CONTENT;
	    }
	    tokadd(nextc());
	    /*	    if (mbp && mb == ENC_CODERANGE_UNKNOWN) mbp = 0;*/
	    if ((c = nextc()) == -1) goto error;
	} while (!whole_match_p(eos, len, indent));
	str = STR_NEW3(tok(), toklen(), enc, func);
    }
    heredoc_restore(lex_strterm);
    lex_strterm = NEW_STRTERM(-1, 0, 0);
    set_yylval_str(str);
    return tSTRING_CONTENT;
}

#include "lex.c"

#ifndef RIPPER
static void
arg_ambiguous_gen(struct parser_params *parser)
{
    rb_warning0("ambiguous first argument; put parentheses or even spaces");
}
#else
static void
arg_ambiguous_gen(struct parser_params *parser)
{
    dispatch0(arg_ambiguous);
}
#endif
#define arg_ambiguous() arg_ambiguous_gen(parser)

static int
lvar_defined_gen(struct parser_params *parser, ID id)
{
#ifndef RIPPER
    return (dyna_in_block() && dvar_defined(id)) || local_id(id);
#else
    return 0;
#endif
}

/* emacsen -*- hack */
static int
parser_encode_length(struct parser_params *parser, const char *name, int len)
{
    int nlen;

    if (len > 5 && name[nlen = len - 5] == '-') {
	if (rb_memcicmp(name + nlen + 1, "unix", 4) == 0)
	    return nlen;
    }
    if (len > 4 && name[nlen = len - 5] == '-') {
	if (rb_memcicmp(name + nlen + 1, "dos", 3) == 0)
	    return nlen;
	if (rb_memcicmp(name + nlen + 1, "mac", 3) == 0)
	    return nlen;
    }
    return len;
}

static void
parser_set_encode(struct parser_params *parser, const char *name)
{
    int idx = rb_enc_find_index(name);
    rb_encoding *enc;

    if (idx < 0) {
	rb_raise(rb_eArgError, "unknown encoding name: %s", name);
    }
    enc = rb_enc_from_index(idx);
    if (!rb_enc_asciicompat(enc)) {
	rb_raise(rb_eArgError, "%s is not ASCII compatible", rb_enc_name(enc));
    }
    parser->enc = enc;
}

static int
comment_at_top(struct parser_params *parser)
{
    const char *p = lex_pbeg, *pend = lex_p - 1;
    if (parser->line_count != (parser->has_shebang ? 2 : 1)) return 0;
    while (p < pend) {
	if (!ISSPACE(*p)) return 0;
	p++;
    }
    return 1;
}

#ifndef RIPPER
typedef int (*rb_magic_comment_length_t)(struct parser_params *parser, const char *name, int len);
typedef void (*rb_magic_comment_setter_t)(struct parser_params *parser, const char *name, const char *val);

static void
magic_comment_encoding(struct parser_params *parser, const char *name, const char *val)
{
    if (!comment_at_top(parser)) {
	rb_warning("encoding '%s' is ignored, valid only in the first line except for shebang line.",
		   val);
	return;
    }
    parser_set_encode(parser, val);
}

struct magic_comment {
    const char *name;
    rb_magic_comment_setter_t func;
    rb_magic_comment_length_t length;
};

static const struct magic_comment magic_comments[] = {
    {"coding", magic_comment_encoding, parser_encode_length},
    {"encoding", magic_comment_encoding, parser_encode_length},
};
#endif

static const char *
magic_comment_marker(const char *str, int len)
{
    int i = 2;

    while (i < len) {
	switch (str[i]) {
	  case '-':
	    if (str[i-1] == '*' && str[i-2] == '-') {
		return str + i + 1;
	    }
	    i += 2;
	    break;
	  case '*':
	    if (i + 1 >= len) return 0;
	    if (str[i+1] != '-') {
		i += 4;
	    }
	    else if (str[i-1] != '-') {
		i += 2;
	    }
	    else {
		return str + i + 2;
	    }
	    break;
	  default:
	    i += 3;
	    break;
	}
    }
    return 0;
}

static int
parser_magic_comment(struct parser_params *parser, const char *str, int len)
{
    VALUE name = 0, val = 0;
    const char *beg, *end, *vbeg, *vend;
#define str_copy(_s, _p, _n) ((_s) \
	? (rb_str_resize((_s), (_n)), \
	   MEMCPY(RSTRING_PTR(_s), (_p), char, (_n)), (_s)) \
	: ((_s) = STR_NEW((_p), (_n))))

    if (len <= 7) return Qfalse;
    if (!(beg = magic_comment_marker(str, len))) return Qfalse;
    if (!(end = magic_comment_marker(beg, str + len - beg))) return Qfalse;
    str = beg;
    len = end - beg - 3;

    /* %r"([^\\s\'\":;]+)\\s*:\\s*(\"(?:\\\\.|[^\"])*\"|[^\"\\s;]+)[\\s;]*" */
    while (len > 0) {
#ifndef RIPPER
	const struct magic_comment *p = magic_comments;
#endif
	char *s;
	int i, n = 0;

	for (; len > 0 && *str; str++, --len) {
	    switch (*str) {
	      case '\'': case '"': case ':': case ';':
		continue;
	    }
	    if (!ISSPACE(*str)) break;
	}
	for (beg = str; len > 0; str++, --len) {
	    switch (*str) {
	      case '\'': case '"': case ':': case ';':
		break;
	      default:
		if (ISSPACE(*str)) break;
		continue;
	    }
	    break;
	}
	for (end = str; len > 0 && ISSPACE(*str); str++, --len);
	if (!len) break;
	if (*str != ':') continue;

	do str++; while (--len > 0 && ISSPACE(*str));
	if (!len) break;
	if (*str == '"') {
	    for (vbeg = ++str; --len > 0 && *str != '"'; str++) {
		if (*str == '\\') {
		    --len;
		    ++str;
		}
	    }
	    vend = str;
	    if (len) {
		--len;
		++str;
	    }
	}
	else {
	    for (vbeg = str; len > 0 && *str != '"' && *str != ';' && !ISSPACE(*str); --len, str++);
	    vend = str;
	}
	while (len > 0 && (*str == ';' || ISSPACE(*str))) --len, str++;

	n = end - beg;
	str_copy(name, beg, n);
	s = RSTRING_PTR(name);
	for (i = 0; i < n; ++i) {
	    if (*s == '-') *s = '_';
	}
#ifndef RIPPER
	do {
	    if (STRNCASECMP(p->name, s, n) == 0) {
		n = vend - vbeg;
		if (p->length) {
		    n = (*p->length)(parser, vbeg, n);
		}
		str_copy(val, vbeg, n);
		(*p->func)(parser, s, RSTRING_PTR(val));
		break;
	    }
	} while (++p < magic_comments + sizeof(magic_comments) / sizeof(*p));
#else
	dispatch2(magic_comment, name, val);
#endif
    }

    return Qtrue;
}

static void
set_file_encoding(struct parser_params *parser, const char *str, const char *send)
{
    int sep = 0;
    const char *beg = str;
    VALUE s;

    for (;;) {
	if (send - str <= 6) return;
	switch (str[6]) {
	  case 'C': case 'c': str += 6; continue;
	  case 'O': case 'o': str += 5; continue;
	  case 'D': case 'd': str += 4; continue;
	  case 'I': case 'i': str += 3; continue;
	  case 'N': case 'n': str += 2; continue;
	  case 'G': case 'g': str += 1; continue;
	  case '=': case ':':
	    sep = 1;
	    str += 6;
	    break;
	  default:
	    str += 6;
	    if (ISSPACE(*str)) break;
	    continue;
	}
	if (STRNCASECMP(str-6, "coding", 6) == 0) break;
    }
    for (;;) {
	do {
	    if (++str >= send) return;
	} while (ISSPACE(*str));
	if (sep) break;
	if (*str != '=' && *str != ':') return;
	sep = 1;
	str++;
    }
    beg = str;
    while ((*str == '-' || *str == '_' || ISALNUM(*str)) && ++str < send);
    s = rb_str_new(beg, parser_encode_length(parser, beg, str - beg));
    parser_set_encode(parser, RSTRING_PTR(s));
    rb_str_resize(s, 0);
}

static void
parser_prepare(struct parser_params *parser)
{
    int c = nextc();
    switch (c) {
      case '#':
	if (peek('!')) parser->has_shebang = 1;
	break;
      case 0xef:		/* UTF-8 BOM marker */
	if (lex_pend - lex_p >= 2 &&
	    (unsigned char)lex_p[0] == 0xbb &&
	    (unsigned char)lex_p[1] == 0xbf) {
	    parser->enc = rb_utf8_encoding();
	    lex_p += 2;
	    lex_pbeg = lex_p;
	    return;
	}
	break;
      case EOF:
	return;
    }
    pushback(c);
    parser->enc = rb_enc_get(lex_lastline);
}

#define IS_ARG() (lex_state == EXPR_ARG || lex_state == EXPR_CMDARG)
#define IS_BEG() (lex_state == EXPR_BEG || lex_state == EXPR_MID || lex_state == EXPR_VALUE || lex_state == EXPR_CLASS)

static int
parser_yylex(struct parser_params *parser)
{
    register int c;
    int space_seen = 0;
    int cmd_state;
    enum lex_state_e last_state;
    rb_encoding *enc;
    int mb;
#ifdef RIPPER
    int fallthru = Qfalse;
#endif

    if (lex_strterm) {
	int token;
	if (nd_type(lex_strterm) == NODE_HEREDOC) {
	    token = here_document(lex_strterm);
	    if (token == tSTRING_END) {
		lex_strterm = 0;
		lex_state = EXPR_END;
	    }
	}
	else {
	    token = parse_string(lex_strterm);
	    if (token == tSTRING_END || token == tREGEXP_END) {
		rb_gc_force_recycle((VALUE)lex_strterm);
		lex_strterm = 0;
		lex_state = EXPR_END;
	    }
	}
	return token;
    }
    cmd_state = command_start;
    command_start = Qfalse;
  retry:
    switch (c = nextc()) {
      case '\0':		/* NUL */
      case '\004':		/* ^D */
      case '\032':		/* ^Z */
      case -1:			/* end of script. */
	return 0;

	/* white spaces */
      case ' ': case '\t': case '\f': case '\r':
      case '\13': /* '\v' */
	space_seen++;
#ifdef RIPPER
	while ((c = nextc())) {
	    switch (c) {
	      case ' ': case '\t': case '\f': case '\r':
	      case '\13': /* '\v' */
		break;
	      default:
		goto outofloop;
	    }
	}
      outofloop:
	pushback(c);
	ripper_dispatch_scan_event(parser, tSP);
#endif
	goto retry;

      case '#':		/* it's a comment */
	/* no magic_comment in shebang line */
	if (!parser_magic_comment(parser, lex_p, lex_pend - lex_p)) {
	    if (comment_at_top(parser)) {
		set_file_encoding(parser, lex_p, lex_pend);
	    }
	}
	lex_p = lex_pend;
#ifdef RIPPER
        ripper_dispatch_scan_event(parser, tCOMMENT);
        fallthru = Qtrue;
#endif
	/* fall through */
      case '\n':
	switch (lex_state) {
	  case EXPR_BEG:
	  case EXPR_FNAME:
	  case EXPR_DOT:
	  case EXPR_CLASS:
	  case EXPR_VALUE:
#ifdef RIPPER
            if (!fallthru) {
                ripper_dispatch_scan_event(parser, tIGNORED_NL);
            }
            fallthru = Qfalse;
#endif
	    goto retry;
	  default:
	    break;
	}
	while ((c = nextc())) {
	    switch (c) {
	      case ' ': case '\t': case '\f': case '\r':
	      case '\13': /* '\v' */
		space_seen++;
		break;
	      case '.': {
		  if ((c = nextc()) != '.') {
		      pushback(c);
		      pushback('.');
		      goto retry;
		  }
	      }
	      default:
		--ruby_sourceline;
		lex_nextline = lex_lastline;
	      case -1:		/* EOF no decrement*/
		lex_goto_eol(parser);
#ifdef RIPPER
		if (c != -1) {
		    parser->tokp = lex_p;
		}
#endif
		goto normal_newline;
	    }
	}
      normal_newline:
	command_start = Qtrue;
	lex_state = EXPR_BEG;
	return '\n';

      case '*':
	if ((c = nextc()) == '*') {
	    if ((c = nextc()) == '=') {
                set_yylval_id(tPOW);
		lex_state = EXPR_BEG;
		return tOP_ASGN;
	    }
	    pushback(c);
	    c = tPOW;
	}
	else {
	    if (c == '=') {
                set_yylval_id('*');
		lex_state = EXPR_BEG;
		return tOP_ASGN;
	    }
	    pushback(c);
	    if (IS_ARG() && space_seen && !ISSPACE(c)) {
		rb_warning0("`*' interpreted as argument prefix");
		c = tSTAR;
	    }
	    else if (IS_BEG()) {
		c = tSTAR;
	    }
	    else {
		c = '*';
	    }
	}
	switch (lex_state) {
	  case EXPR_FNAME: case EXPR_DOT:
	    lex_state = EXPR_ARG; break;
	  default:
	    lex_state = EXPR_BEG; break;
	}
	return c;

      case '!':
	c = nextc();
	if (lex_state == EXPR_FNAME || lex_state == EXPR_DOT) {
	    lex_state = EXPR_ARG;
	    if (c == '@') {
		return '!';
	    }
	}
	else {
	    lex_state = EXPR_BEG;
	}
	if (c == '=') {
	    return tNEQ;
	}
	if (c == '~') {
	    return tNMATCH;
	}
	pushback(c);
	return '!';

      case '=':
	if (was_bol()) {
	    /* skip embedded rd document */
	    if (strncmp(lex_p, "begin", 5) == 0 && ISSPACE(lex_p[5])) {
#ifdef RIPPER
                int first_p = Qtrue;

                lex_goto_eol(parser);
                ripper_dispatch_scan_event(parser, tEMBDOC_BEG);
#endif
		for (;;) {
		    lex_goto_eol(parser);
#ifdef RIPPER
                    if (!first_p) {
                        ripper_dispatch_scan_event(parser, tEMBDOC);
                    }
                    first_p = Qfalse;
#endif
		    c = nextc();
		    if (c == -1) {
			compile_error(PARSER_ARG "embedded document meets end of file");
			return 0;
		    }
		    if (c != '=') continue;
		    if (strncmp(lex_p, "end", 3) == 0 &&
			(lex_p + 3 == lex_pend || ISSPACE(lex_p[3]))) {
			break;
		    }
		}
		lex_goto_eol(parser);
#ifdef RIPPER
                ripper_dispatch_scan_event(parser, tEMBDOC_END);
#endif
		goto retry;
	    }
	}

	switch (lex_state) {
	  case EXPR_FNAME: case EXPR_DOT:
	    lex_state = EXPR_ARG; break;
	  default:
	    lex_state = EXPR_BEG; break;
	}
	if ((c = nextc()) == '=') {
	    if ((c = nextc()) == '=') {
		return tEQQ;
	    }
	    pushback(c);
	    return tEQ;
	}
	if (c == '~') {
	    return tMATCH;
	}
	else if (c == '>') {
	    return tASSOC;
	}
	pushback(c);
	return '=';

      case '<':
	c = nextc();
	if (c == '<' &&
	    lex_state != EXPR_END &&
	    lex_state != EXPR_DOT &&
	    lex_state != EXPR_ENDARG &&
	    lex_state != EXPR_CLASS &&
	    (!IS_ARG() || space_seen)) {
	    int token = heredoc_identifier();
	    if (token) return token;
	}
	switch (lex_state) {
	  case EXPR_FNAME: case EXPR_DOT:
	    lex_state = EXPR_ARG; break;
	  default:
	    lex_state = EXPR_BEG; break;
	}
	if (c == '=') {
	    if ((c = nextc()) == '>') {
		return tCMP;
	    }
	    pushback(c);
	    return tLEQ;
	}
	if (c == '<') {
	    if ((c = nextc()) == '=') {
                set_yylval_id(tLSHFT);
		lex_state = EXPR_BEG;
		return tOP_ASGN;
	    }
	    pushback(c);
	    return tLSHFT;
	}
	pushback(c);
	return '<';

      case '>':
	switch (lex_state) {
	  case EXPR_FNAME: case EXPR_DOT:
	    lex_state = EXPR_ARG; break;
	  default:
	    lex_state = EXPR_BEG; break;
	}
	if ((c = nextc()) == '=') {
	    return tGEQ;
	}
	if (c == '>') {
	    if ((c = nextc()) == '=') {
                set_yylval_id(tRSHFT);
		lex_state = EXPR_BEG;
		return tOP_ASGN;
	    }
	    pushback(c);
	    return tRSHFT;
	}
	pushback(c);
	return '>';

      case '"':
	lex_strterm = NEW_STRTERM(str_dquote, '"', 0);
	return tSTRING_BEG;

      case '`':
	if (lex_state == EXPR_FNAME) {
	    lex_state = EXPR_END;
	    return c;
	}
	if (lex_state == EXPR_DOT) {
	    if (cmd_state)
		lex_state = EXPR_CMDARG;
	    else
		lex_state = EXPR_ARG;
	    return c;
	}
	lex_strterm = NEW_STRTERM(str_xquote, '`', 0);
	return tXSTRING_BEG;

      case '\'':
	lex_strterm = NEW_STRTERM(str_squote, '\'', 0);
	return tSTRING_BEG;

      case '?':
	if (lex_state == EXPR_END || lex_state == EXPR_ENDARG) {
	    lex_state = EXPR_VALUE;
	    return '?';
	}
	c = nextc();
	if (c == -1) {
	    compile_error(PARSER_ARG "incomplete character syntax");
	    return 0;
	}
	if (rb_enc_isspace(c, parser->enc)) {
	    if (!IS_ARG()) {
		int c2 = 0;
		switch (c) {
		  case ' ':
		    c2 = 's';
		    break;
		  case '\n':
		    c2 = 'n';
		    break;
		  case '\t':
		    c2 = 't';
		    break;
		  case '\v':
		    c2 = 'v';
		    break;
		  case '\r':
		    c2 = 'r';
		    break;
		  case '\f':
		    c2 = 'f';
		    break;
		}
		if (c2) {
		    rb_warnI("invalid character syntax; use ?\\%c", c2);
		}
	    }
	  ternary:
	    pushback(c);
	    lex_state = EXPR_VALUE;
	    return '?';
	}
	newtok();
	enc = parser->enc;
	if (!parser_isascii()) {
	    if (tokadd_mbchar(c) == -1) return 0;
	}
	else if ((rb_enc_isalnum(c, parser->enc) || c == '_') &&
		 lex_p < lex_pend && is_identchar(lex_p, lex_pend, parser->enc)) {
	    goto ternary;
	}
        else if (c == '\\') {
            if (peek('u')) {
                nextc();
                c = parser_tokadd_utf8(parser, &enc, 0, 0, 0);
                if (0x80 <= c) {
                    tokaddmbc(c, enc);
                }
                else {
                    tokadd(c);
                }
            }
            else {
                c = read_escape(0, &enc);
                tokadd(c);
            }
        }
        else {
	    tokadd(c);
        }
	tokfix();
	set_yylval_str(STR_NEW3(tok(), toklen(), enc, 0));
	lex_state = EXPR_END;
	return tCHAR;

      case '&':
	if ((c = nextc()) == '&') {
	    lex_state = EXPR_BEG;
	    if ((c = nextc()) == '=') {
                set_yylval_id(tANDOP);
		lex_state = EXPR_BEG;
		return tOP_ASGN;
	    }
	    pushback(c);
	    return tANDOP;
	}
	else if (c == '=') {
            set_yylval_id('&');
	    lex_state = EXPR_BEG;
	    return tOP_ASGN;
	}
	pushback(c);
	if (IS_ARG() && space_seen && !ISSPACE(c)) {
	    rb_warning0("`&' interpreted as argument prefix");
	    c = tAMPER;
	}
	else if (IS_BEG()) {
	    c = tAMPER;
	}
	else {
	    c = '&';
	}
	switch (lex_state) {
	  case EXPR_FNAME: case EXPR_DOT:
	    lex_state = EXPR_ARG; break;
	  default:
	    lex_state = EXPR_BEG;
	}
	return c;

      case '|':
	if ((c = nextc()) == '|') {
	    lex_state = EXPR_BEG;
	    if ((c = nextc()) == '=') {
                set_yylval_id(tOROP);
		lex_state = EXPR_BEG;
		return tOP_ASGN;
	    }
	    pushback(c);
	    return tOROP;
	}
	if (c == '=') {
            set_yylval_id('|');
	    lex_state = EXPR_BEG;
	    return tOP_ASGN;
	}
	if (lex_state == EXPR_FNAME || lex_state == EXPR_DOT) {
	    lex_state = EXPR_ARG;
	}
	else {
	    lex_state = EXPR_BEG;
	}
	pushback(c);
	return '|';

      case '+':
	c = nextc();
	if (lex_state == EXPR_FNAME || lex_state == EXPR_DOT) {
	    lex_state = EXPR_ARG;
	    if (c == '@') {
		return tUPLUS;
	    }
	    pushback(c);
	    return '+';
	}
	if (c == '=') {
            set_yylval_id('+');
	    lex_state = EXPR_BEG;
	    return tOP_ASGN;
	}
	if (IS_BEG() ||
	    (IS_ARG() && space_seen && !ISSPACE(c))) {
	    if (IS_ARG()) arg_ambiguous();
	    lex_state = EXPR_BEG;
	    pushback(c);
	    if (ISDIGIT(c)) {
		c = '+';
		goto start_num;
	    }
	    return tUPLUS;
	}
	lex_state = EXPR_BEG;
	pushback(c);
	return '+';

      case '-':
	c = nextc();
	if (lex_state == EXPR_FNAME || lex_state == EXPR_DOT) {
	    lex_state = EXPR_ARG;
	    if (c == '@') {
		return tUMINUS;
	    }
	    pushback(c);
	    return '-';
	}
	if (c == '=') {
            set_yylval_id('-');
	    lex_state = EXPR_BEG;
	    return tOP_ASGN;
	}
	if (c == '>') {
	    lex_state = EXPR_ARG;
	    return tLAMBDA;
	}
	if (IS_BEG() ||
	    (IS_ARG() && space_seen && !ISSPACE(c))) {
	    if (IS_ARG()) arg_ambiguous();
	    lex_state = EXPR_BEG;
	    pushback(c);
	    if (ISDIGIT(c)) {
		return tUMINUS_NUM;
	    }
	    return tUMINUS;
	}
	lex_state = EXPR_BEG;
	pushback(c);
	return '-';

      case '.':
	lex_state = EXPR_BEG;
	if ((c = nextc()) == '.') {
	    if ((c = nextc()) == '.') {
		return tDOT3;
	    }
	    pushback(c);
	    return tDOT2;
	}
	pushback(c);
	if (ISDIGIT(c)) {
	    yyerror("no .<digit> floating literal anymore; put 0 before dot");
	}
	lex_state = EXPR_DOT;
	return '.';

      start_num:
      case '0': case '1': case '2': case '3': case '4':
      case '5': case '6': case '7': case '8': case '9':
	{
	    int is_float, seen_point, seen_e, nondigit;

	    is_float = seen_point = seen_e = nondigit = 0;
	    lex_state = EXPR_END;
	    newtok();
	    if (c == '-' || c == '+') {
		tokadd(c);
		c = nextc();
	    }
	    if (c == '0') {
		int start = toklen();
		c = nextc();
		if (c == 'x' || c == 'X') {
		    /* hexadecimal */
		    c = nextc();
		    if (ISXDIGIT(c)) {
			do {
			    if (c == '_') {
				if (nondigit) break;
				nondigit = c;
				continue;
			    }
			    if (!ISXDIGIT(c)) break;
			    nondigit = 0;
			    tokadd(c);
			} while ((c = nextc()) != -1);
		    }
		    pushback(c);
		    tokfix();
		    if (toklen() == start) {
			yyerror("numeric literal without digits");
		    }
		    else if (nondigit) goto trailing_uc;
                    set_yylval_literal(rb_cstr_to_inum(tok(), 16, Qfalse));
		    return tINTEGER;
		}
		if (c == 'b' || c == 'B') {
		    /* binary */
		    c = nextc();
		    if (c == '0' || c == '1') {
			do {
			    if (c == '_') {
				if (nondigit) break;
				nondigit = c;
				continue;
			    }
			    if (c != '0' && c != '1') break;
			    nondigit = 0;
			    tokadd(c);
			} while ((c = nextc()) != -1);
		    }
		    pushback(c);
		    tokfix();
		    if (toklen() == start) {
			yyerror("numeric literal without digits");
		    }
		    else if (nondigit) goto trailing_uc;
                    set_yylval_literal(rb_cstr_to_inum(tok(), 2, Qfalse));
		    return tINTEGER;
		}
		if (c == 'd' || c == 'D') {
		    /* decimal */
		    c = nextc();
		    if (ISDIGIT(c)) {
			do {
			    if (c == '_') {
				if (nondigit) break;
				nondigit = c;
				continue;
			    }
			    if (!ISDIGIT(c)) break;
			    nondigit = 0;
			    tokadd(c);
			} while ((c = nextc()) != -1);
		    }
		    pushback(c);
		    tokfix();
		    if (toklen() == start) {
			yyerror("numeric literal without digits");
		    }
		    else if (nondigit) goto trailing_uc;
                    set_yylval_literal(rb_cstr_to_inum(tok(), 10, Qfalse));
		    return tINTEGER;
		}
		if (c == '_') {
		    /* 0_0 */
		    goto octal_number;
		}
		if (c == 'o' || c == 'O') {
		    /* prefixed octal */
		    c = nextc();
		    if (c == '_' || !ISDIGIT(c)) {
			yyerror("numeric literal without digits");
		    }
		}
		if (c >= '0' && c <= '7') {
		    /* octal */
		  octal_number:
	            do {
			if (c == '_') {
			    if (nondigit) break;
			    nondigit = c;
			    continue;
			}
			if (c < '0' || c > '9') break;
			if (c > '7') goto invalid_octal;
			nondigit = 0;
			tokadd(c);
		    } while ((c = nextc()) != -1);
		    if (toklen() > start) {
			pushback(c);
			tokfix();
			if (nondigit) goto trailing_uc;
                        set_yylval_literal(rb_cstr_to_inum(tok(), 8, Qfalse));
			return tINTEGER;
		    }
		    if (nondigit) {
			pushback(c);
			goto trailing_uc;
		    }
		}
		if (c > '7' && c <= '9') {
		  invalid_octal:
		    yyerror("Invalid octal digit");
		}
		else if (c == '.' || c == 'e' || c == 'E') {
		    tokadd('0');
		}
		else {
		    pushback(c);
                    set_yylval_literal(INT2FIX(0));
		    return tINTEGER;
		}
	    }

	    for (;;) {
		switch (c) {
		  case '0': case '1': case '2': case '3': case '4':
		  case '5': case '6': case '7': case '8': case '9':
		    nondigit = 0;
		    tokadd(c);
		    break;

		  case '.':
		    if (nondigit) goto trailing_uc;
		    if (seen_point || seen_e) {
			goto decode_num;
		    }
		    else {
			int c0 = nextc();
			if (!ISDIGIT(c0)) {
			    pushback(c0);
			    goto decode_num;
			}
			c = c0;
		    }
		    tokadd('.');
		    tokadd(c);
		    is_float++;
		    seen_point++;
		    nondigit = 0;
		    break;

		  case 'e':
		  case 'E':
		    if (nondigit) {
			pushback(c);
			c = nondigit;
			goto decode_num;
		    }
		    if (seen_e) {
			goto decode_num;
		    }
		    tokadd(c);
		    seen_e++;
		    is_float++;
		    nondigit = c;
		    c = nextc();
		    if (c != '-' && c != '+') continue;
		    tokadd(c);
		    nondigit = c;
		    break;

		  case '_':	/* `_' in number just ignored */
		    if (nondigit) goto decode_num;
		    nondigit = c;
		    break;

		  default:
		    goto decode_num;
		}
		c = nextc();
	    }

	  decode_num:
	    pushback(c);
	    tokfix();
	    if (nondigit) {
		char tmp[30];
	      trailing_uc:
		sprintf(tmp, "trailing `%c' in number", nondigit);
		yyerror(tmp);
	    }
	    if (is_float) {
		double d = strtod(tok(), 0);
		if (errno == ERANGE) {
		    rb_warningS("Float %s out of range", tok());
		    errno = 0;
		}
                set_yylval_literal(DBL2NUM(d));
		return tFLOAT;
	    }
            set_yylval_literal(rb_cstr_to_inum(tok(), 10, Qfalse));
	    return tINTEGER;
	}

      case ')':
      case ']':
	paren_nest--;
      case '}':
	COND_LEXPOP();
	CMDARG_LEXPOP();
	if (c == ')')
	    lex_state = EXPR_END;
	else
	    lex_state = EXPR_ENDARG;
	return c;

      case ':':
	c = nextc();
	if (c == ':') {
	    if (IS_BEG() ||
		lex_state == EXPR_CLASS || (IS_ARG() && space_seen)) {
		lex_state = EXPR_BEG;
		return tCOLON3;
	    }
	    lex_state = EXPR_DOT;
	    return tCOLON2;
	}
	if (lex_state == EXPR_END || lex_state == EXPR_ENDARG || ISSPACE(c)) {
	    pushback(c);
	    lex_state = EXPR_BEG;
	    return ':';
	}
	switch (c) {
	  case '\'':
	    lex_strterm = NEW_STRTERM(str_ssym, c, 0);
	    break;
	  case '"':
	    lex_strterm = NEW_STRTERM(str_dsym, c, 0);
	    break;
	  default:
	    pushback(c);
	    break;
	}
	lex_state = EXPR_FNAME;
	return tSYMBEG;

      case '/':
	if (IS_BEG()) {
	    lex_strterm = NEW_STRTERM(str_regexp, '/', 0);
	    return tREGEXP_BEG;
	}
	if ((c = nextc()) == '=') {
            set_yylval_id('/');
	    lex_state = EXPR_BEG;
	    return tOP_ASGN;
	}
	pushback(c);
	if (IS_ARG() && space_seen) {
	    if (!ISSPACE(c)) {
		arg_ambiguous();
		lex_strterm = NEW_STRTERM(str_regexp, '/', 0);
		return tREGEXP_BEG;
	    }
	}
	switch (lex_state) {
	  case EXPR_FNAME: case EXPR_DOT:
	    lex_state = EXPR_ARG; break;
	  default:
	    lex_state = EXPR_BEG; break;
	}
	return '/';

      case '^':
	if ((c = nextc()) == '=') {
            set_yylval_id('^');
	    lex_state = EXPR_BEG;
	    return tOP_ASGN;
	}
	switch (lex_state) {
	  case EXPR_FNAME: case EXPR_DOT:
	    lex_state = EXPR_ARG; break;
	  default:
	    lex_state = EXPR_BEG; break;
	}
	pushback(c);
	return '^';

      case ';':
	lex_state = EXPR_BEG;
	command_start = Qtrue;
	return ';';

      case ',':
	lex_state = EXPR_BEG;
	return ',';

      case '~':
	if (lex_state == EXPR_FNAME || lex_state == EXPR_DOT) {
	    if ((c = nextc()) != '@') {
		pushback(c);
	    }
	    lex_state = EXPR_ARG;
	}
	else {
	    lex_state = EXPR_BEG;
	}
	return '~';

      case '(':
	if (IS_BEG()) {
	    c = tLPAREN;
	}
	else if (space_seen) {
	    if (IS_ARG()) {
		c = tLPAREN_ARG;
	    }
	}
	paren_nest++;
	COND_PUSH(0);
	CMDARG_PUSH(0);
	lex_state = EXPR_BEG;
	return c;

      case '[':
	paren_nest++;
	if (lex_state == EXPR_FNAME || lex_state == EXPR_DOT) {
	    lex_state = EXPR_ARG;
	    if ((c = nextc()) == ']') {
		if ((c = nextc()) == '=') {
		    return tASET;
		}
		pushback(c);
		return tAREF;
	    }
	    pushback(c);
	    return '[';
	}
	else if (IS_BEG()) {
	    c = tLBRACK;
	}
	else if (IS_ARG() && space_seen) {
	    c = tLBRACK;
	}
	lex_state = EXPR_BEG;
	COND_PUSH(0);
	CMDARG_PUSH(0);
	return c;

      case '{':
	if (lpar_beg && lpar_beg == paren_nest) {
	    lex_state = EXPR_BEG;
	    lpar_beg = 0;
	    --paren_nest;
	    return tLAMBEG;
	}
	if (IS_ARG() || lex_state == EXPR_END)
	    c = '{';          /* block (primary) */
	else if (lex_state == EXPR_ENDARG)
	    c = tLBRACE_ARG;  /* block (expr) */
	else
	    c = tLBRACE;      /* hash */
	COND_PUSH(0);
	CMDARG_PUSH(0);
	lex_state = EXPR_BEG;
	if (c != tLBRACE) command_start = Qtrue;
	return c;

      case '\\':
	c = nextc();
	if (c == '\n') {
	    space_seen = 1;
#ifdef RIPPER
	    ripper_dispatch_scan_event(parser, tSP);
#endif
	    goto retry; /* skip \\n */
	}
	pushback(c);
	return '\\';

      case '%':
	if (IS_BEG()) {
	    int term;
	    int paren;

	    c = nextc();
	  quotation:
	    if (!ISALNUM(c)) {
		term = c;
		c = 'Q';
	    }
	    else {
		term = nextc();
		if (rb_enc_isalnum(term, parser->enc) || !parser_isascii()) {
		    yyerror("unknown type of %string");
		    return 0;
		}
	    }
	    if (c == -1 || term == -1) {
		compile_error(PARSER_ARG "unterminated quoted string meets end of file");
		return 0;
	    }
	    paren = term;
	    if (term == '(') term = ')';
	    else if (term == '[') term = ']';
	    else if (term == '{') term = '}';
	    else if (term == '<') term = '>';
	    else paren = 0;

	    switch (c) {
	      case 'Q':
		lex_strterm = NEW_STRTERM(str_dquote, term, paren);
		return tSTRING_BEG;

	      case 'q':
		lex_strterm = NEW_STRTERM(str_squote, term, paren);
		return tSTRING_BEG;

	      case 'W':
		lex_strterm = NEW_STRTERM(str_dword, term, paren);
		do {c = nextc();} while (ISSPACE(c));
		pushback(c);
		return tWORDS_BEG;

	      case 'w':
		lex_strterm = NEW_STRTERM(str_sword, term, paren);
		do {c = nextc();} while (ISSPACE(c));
		pushback(c);
		return tQWORDS_BEG;

	      case 'x':
		lex_strterm = NEW_STRTERM(str_xquote, term, paren);
		return tXSTRING_BEG;

	      case 'r':
		lex_strterm = NEW_STRTERM(str_regexp, term, paren);
		return tREGEXP_BEG;

	      case 's':
		lex_strterm = NEW_STRTERM(str_ssym, term, paren);
		lex_state = EXPR_FNAME;
		return tSYMBEG;

	      default:
		yyerror("unknown type of %string");
		return 0;
	    }
	}
	if ((c = nextc()) == '=') {
            set_yylval_id('%');
	    lex_state = EXPR_BEG;
	    return tOP_ASGN;
	}
	if (IS_ARG() && space_seen && !ISSPACE(c)) {
	    goto quotation;
	}
	switch (lex_state) {
	  case EXPR_FNAME: case EXPR_DOT:
	    lex_state = EXPR_ARG; break;
	  default:
	    lex_state = EXPR_BEG; break;
	}
	pushback(c);
	return '%';

      case '$':
	last_state = lex_state;
	lex_state = EXPR_END;
	newtok();
	c = nextc();
	switch (c) {
	  case '_':		/* $_: last read line string */
	    c = nextc();
	    if (parser_is_identchar()) {
		tokadd('$');
		tokadd('_');
		break;
	    }
	    pushback(c);
	    c = '_';
	    /* fall through */
	  case '~':		/* $~: match-data */
	  case '*':		/* $*: argv */
	  case '$':		/* $$: pid */
	  case '?':		/* $?: last status */
	  case '!':		/* $!: error string */
	  case '@':		/* $@: error position */
	  case '/':		/* $/: input record separator */
	  case '\\':		/* $\: output record separator */
	  case ';':		/* $;: field separator */
	  case ',':		/* $,: output field separator */
	  case '.':		/* $.: last read line number */
	  case '=':		/* $=: ignorecase */
	  case ':':		/* $:: load path */
	  case '<':		/* $<: reading filename */
	  case '>':		/* $>: default output handle */
	  case '\"':		/* $": already loaded files */
	    tokadd('$');
	    tokadd(c);
	    tokfix();
            set_yylval_id(rb_intern(tok()));
	    return tGVAR;

	  case '-':
	    tokadd('$');
	    tokadd(c);
	    c = nextc();
	    if (parser_is_identchar()) {
		if (tokadd_mbchar(c) == -1) return 0;
	    }
	    else {
		pushback(c);
	    }
	  gvar:
	    tokfix();
            set_yylval_id(rb_intern(tok()));
	    return tGVAR;

	  case '&':		/* $&: last match */
	  case '`':		/* $`: string before last match */
	  case '\'':		/* $': string after last match */
	  case '+':		/* $+: string matches last paren. */
	    if (last_state == EXPR_FNAME) {
		tokadd('$');
		tokadd(c);
		goto gvar;
	    }
	    set_yylval_node(NEW_BACK_REF(c));
	    return tBACK_REF;

	  case '1': case '2': case '3':
	  case '4': case '5': case '6':
	  case '7': case '8': case '9':
	    tokadd('$');
	    do {
		tokadd(c);
		c = nextc();
	    } while (ISDIGIT(c));
	    pushback(c);
	    if (last_state == EXPR_FNAME) goto gvar;
	    tokfix();
	    set_yylval_node(NEW_NTH_REF(atoi(tok()+1)));
	    return tNTH_REF;

	  default:
	    if (!parser_is_identchar()) {
		pushback(c);
		return '$';
	    }
	  case '0':
	    tokadd('$');
	}
	break;

      case '@':
	c = nextc();
	newtok();
	tokadd('@');
	if (c == '@') {
	    tokadd('@');
	    c = nextc();
	}
	if (ISDIGIT(c)) {
	    if (tokidx == 1) {
		compile_error(PARSER_ARG "`@%c' is not allowed as an instance variable name", c);
	    }
	    else {
		compile_error(PARSER_ARG "`@@%c' is not allowed as a class variable name", c);
	    }
	    return 0;
	}
	if (!parser_is_identchar()) {
	    pushback(c);
	    return '@';
	}
	break;

      case '_':
	if (was_bol() && whole_match_p("__END__", 7, 0)) {
	    ruby__end__seen = 1;
	    parser->eofp = Qtrue;
#ifndef RIPPER
	    return -1;
#else
            lex_goto_eol(parser);
            ripper_dispatch_scan_event(parser, k__END__);
            return 0;
#endif
	}
	newtok();
	break;

      default:
	if (!parser_is_identchar()) {
	    rb_compile_error(PARSER_ARG  "Invalid char `\\x%02X' in expression", c);
	    goto retry;
	}

	newtok();
	break;
    }

    mb = ENC_CODERANGE_7BIT;
    do {
	if (!ISASCII(c)) mb = ENC_CODERANGE_UNKNOWN;
	if (tokadd_mbchar(c) == -1) return 0;
	c = nextc();
    } while (parser_is_identchar());
    switch (tok()[0]) {
      case '@': case '$':
	pushback(c);
	break;
      default:
	if ((c == '!' || c == '?') && !peek('=')) {
	    tokadd(c);
	}
	else {
	    pushback(c);
	}
    }
    tokfix();

    {
	int result = 0;

	last_state = lex_state;
	switch (tok()[0]) {
	  case '$':
	    lex_state = EXPR_END;
	    result = tGVAR;
	    break;
	  case '@':
	    lex_state = EXPR_END;
	    if (tok()[1] == '@')
		result = tCVAR;
	    else
		result = tIVAR;
	    break;

	  default:
	    if (toklast() == '!' || toklast() == '?') {
		result = tFID;
	    }
	    else {
		if (lex_state == EXPR_FNAME) {
		    if ((c = nextc()) == '=' && !peek('~') && !peek('>') &&
			(!peek('=') || (lex_p + 1 < lex_pend && lex_p[1] == '>'))) {
			result = tIDENTIFIER;
			tokadd(c);
			tokfix();
		    }
		    else {
			pushback(c);
		    }
		}
		if (result == 0 && ISUPPER(tok()[0])) {
		    result = tCONSTANT;
		}
		else {
		    result = tIDENTIFIER;
		}
	    }

	    if ((lex_state == EXPR_BEG && !cmd_state) ||
		lex_state == EXPR_ARG ||
		lex_state == EXPR_CMDARG) {
		if (peek(':') && !(lex_p + 1 < lex_pend && lex_p[1] == ':')) {
		    lex_state = EXPR_BEG;
		    nextc();
		    set_yylval_id(TOK_INTERN(!ENC_SINGLE(mb)));
		    return tLABEL;
		}
	    }
	    if (mb == ENC_CODERANGE_7BIT && lex_state != EXPR_DOT) {
		const struct kwtable *kw;

		/* See if it is a reserved word.  */
		kw = rb_reserved_word(tok(), toklen());
		if (kw) {
		    enum lex_state_e state = lex_state;
		    lex_state = kw->state;
		    if (state == EXPR_FNAME) {
                        set_yylval_id(rb_intern(kw->name));
			return kw->id[0];
		    }
		    if (kw->id[0] == keyword_do) {
			command_start = Qtrue;
			if (lpar_beg && lpar_beg == paren_nest) {
			    lpar_beg = 0;
			    --paren_nest;
			    return keyword_do_LAMBDA;
			}
			if (COND_P()) return keyword_do_cond;
			if (CMDARG_P() && state != EXPR_CMDARG)
			    return keyword_do_block;
			if (state == EXPR_ENDARG || state == EXPR_BEG)
			    return keyword_do_block;
			return keyword_do;
		    }
		    if (state == EXPR_BEG || state == EXPR_VALUE)
			return kw->id[0];
		    else {
			if (kw->id[0] != kw->id[1])
			    lex_state = EXPR_BEG;
			return kw->id[1];
		    }
		}
	    }

	    if (IS_BEG() ||
		lex_state == EXPR_DOT ||
		IS_ARG()) {
		if (cmd_state) {
		    lex_state = EXPR_CMDARG;
		}
		else {
		    lex_state = EXPR_ARG;
		}
	    }
	    else {
		lex_state = EXPR_END;
	    }
	}
        {
            ID ident = TOK_INTERN(!ENC_SINGLE(mb));

            set_yylval_id(ident);
            if (last_state != EXPR_DOT && is_local_id(ident) && lvar_defined(ident)) {
                lex_state = EXPR_END;
            }
        }
	return result;
    }
}

#if YYPURE
static int
yylex(void *lval, void *p)
#else
yylex(void *p)
#endif
{
    struct parser_params *parser = (struct parser_params*)p;
    int t;

#if YYPURE
    parser->parser_yylval = lval;
    parser->parser_yylval->val = Qundef;
#endif
    t = parser_yylex(parser);
#ifdef RIPPER
    if (!NIL_P(parser->delayed)) {
	ripper_dispatch_delayed_token(parser, t);
	return t;
    }
    if (t != 0)
	ripper_dispatch_scan_event(parser, t);
#endif

    return t;
}

#ifndef RIPPER
static NODE*
node_newnode(struct parser_params *parser, enum node_type type, VALUE a0, VALUE a1, VALUE a2)
{
    NODE *n = (rb_node_newnode)(type, a0, a1, a2);
    nd_set_line(n, ruby_sourceline);
    return n;
}

enum node_type
nodetype(NODE *node)			/* for debug */
{
    return (enum node_type)nd_type(node);
}

int
nodeline(NODE *node)
{
    return nd_line(node);
}

static NODE*
newline_node(NODE *node)
{
    if (node) {
	node = remove_begin(node);
	node->flags |= NODE_FL_NEWLINE;
    }
    return node;
}

static void
fixpos(NODE *node, NODE *orig)
{
    if (!node) return;
    if (!orig) return;
    if (orig == (NODE*)1) return;
    nd_set_line(node, nd_line(orig));
}

static void
parser_warning(struct parser_params *parser, NODE *node, const char *mesg)
{
    rb_compile_warning(ruby_sourcefile, nd_line(node), "%s", mesg);
}
#define parser_warning(node, mesg) parser_warning(parser, node, mesg)

static void
parser_warn(struct parser_params *parser, NODE *node, const char *mesg)
{
    rb_compile_warn(ruby_sourcefile, nd_line(node), "%s", mesg);
}
#define parser_warn(node, mesg) parser_warn(parser, node, mesg)

static NODE*
block_append_gen(struct parser_params *parser, NODE *head, NODE *tail)
{
    NODE *end, *h = head, *nd;

    if (tail == 0) return head;

    if (h == 0) return tail;
    switch (nd_type(h)) {
      case NODE_LIT:
      case NODE_STR:
      case NODE_SELF:
      case NODE_TRUE:
      case NODE_FALSE:
      case NODE_NIL:
	parser_warning(h, "unused literal ignored");
	return tail;
      default:
	h = end = NEW_BLOCK(head);
	end->nd_end = end;
	fixpos(end, head);
	head = end;
	break;
      case NODE_BLOCK:
	end = h->nd_end;
	break;
    }

    nd = end->nd_head;
    switch (nd_type(nd)) {
      case NODE_RETURN:
      case NODE_BREAK:
      case NODE_NEXT:
      case NODE_REDO:
      case NODE_RETRY:
	if (RTEST(ruby_verbose)) {
	    parser_warning(nd, "statement not reached");
	}
	break;

      default:
	break;
    }

    if (nd_type(tail) != NODE_BLOCK) {
	tail = NEW_BLOCK(tail);
	tail->nd_end = tail;
    }
    end->nd_next = tail;
    h->nd_end = tail->nd_end;
    return head;
}

/* append item to the list */
static NODE*
list_append_gen(struct parser_params *parser, NODE *list, NODE *item)
{
    NODE *last;

    if (list == 0) return NEW_LIST(item);
    if (list->nd_next) {
	last = list->nd_next->nd_end;
    }
    else {
	last = list;
    }

    list->nd_alen += 1;
    last->nd_next = NEW_LIST(item);
    list->nd_next->nd_end = last->nd_next;
    return list;
}

/* concat two lists */
static NODE*
list_concat_gen(struct parser_params *parser, NODE *head, NODE *tail)
{
    NODE *last;

    if (head->nd_next) {
	last = head->nd_next->nd_end;
    }
    else {
	last = head;
    }

    head->nd_alen += tail->nd_alen;
    last->nd_next = tail;
    if (tail->nd_next) {
	head->nd_next->nd_end = tail->nd_next->nd_end;
    }
    else {
	head->nd_next->nd_end = tail;
    }

    return head;
}

static int
literal_concat0(struct parser_params *parser, VALUE head, VALUE tail)
{
    if (!rb_enc_compatible(head, tail)) {
	compile_error(PARSER_ARG "string literal encodings differ (%s / %s)",
		      rb_enc_name(rb_enc_get(head)),
		      rb_enc_name(rb_enc_get(tail)));
	rb_str_resize(head, 0);
	rb_str_resize(tail, 0);
	return 0;
    }
    rb_str_buf_append(head, tail);
    return 1;
}

/* concat two string literals */
static NODE *
literal_concat_gen(struct parser_params *parser, NODE *head, NODE *tail)
{
    enum node_type htype;

    if (!head) return tail;
    if (!tail) return head;

    htype = nd_type(head);
    if (htype == NODE_EVSTR) {
	NODE *node = NEW_DSTR(STR_NEW0());
	head = list_append(node, head);
    }
    switch (nd_type(tail)) {
      case NODE_STR:
	if (htype == NODE_STR) {
	    if (!literal_concat0(parser, head->nd_lit, tail->nd_lit)) {
	      error:
		rb_gc_force_recycle((VALUE)head);
		rb_gc_force_recycle((VALUE)tail);
		return 0;
	    }
	    rb_gc_force_recycle((VALUE)tail);
	}
	else {
	    list_append(head, tail);
	}
	break;

      case NODE_DSTR:
	if (htype == NODE_STR) {
	    if (!literal_concat0(parser, head->nd_lit, tail->nd_lit))
		goto error;
	    tail->nd_lit = head->nd_lit;
	    rb_gc_force_recycle((VALUE)head);
	    head = tail;
	}
	else {
	    nd_set_type(tail, NODE_ARRAY);
	    tail->nd_head = NEW_STR(tail->nd_lit);
	    list_concat(head, tail);
	}
	break;

      case NODE_EVSTR:
	if (htype == NODE_STR) {
	    nd_set_type(head, NODE_DSTR);
	    head->nd_alen = 1;
	}
	list_append(head, tail);
	break;
    }
    return head;
}

static NODE *
evstr2dstr_gen(struct parser_params *parser, NODE *node)
{
    if (nd_type(node) == NODE_EVSTR) {
	node = list_append(NEW_DSTR(STR_NEW0()), node);
    }
    return node;
}

static NODE *
new_evstr_gen(struct parser_params *parser, NODE *node)
{
    NODE *head = node;

    if (node) {
	switch (nd_type(node)) {
	  case NODE_STR: case NODE_DSTR: case NODE_EVSTR:
	    return node;
	}
    }
    return NEW_EVSTR(head);
}

static NODE *
call_bin_op_gen(struct parser_params *parser, NODE *recv, ID id, NODE *arg1)
{
    value_expr(recv);
    value_expr(arg1);
    return NEW_CALL(recv, id, NEW_LIST(arg1));
}

static NODE *
call_uni_op_gen(struct parser_params *parser, NODE *recv, ID id)
{
    value_expr(recv);
    return NEW_CALL(recv, id, 0);
}

static NODE*
match_op_gen(struct parser_params *parser, NODE *node1, NODE *node2)
{
    value_expr(node1);
    value_expr(node2);
    if (node1) {
	switch (nd_type(node1)) {
	  case NODE_DREGX:
	  case NODE_DREGX_ONCE:
	    return NEW_MATCH2(node1, node2);

	  case NODE_LIT:
	    if (TYPE(node1->nd_lit) == T_REGEXP) {
		return NEW_MATCH2(node1, node2);
	    }
	}
    }

    if (node2) {
	switch (nd_type(node2)) {
	  case NODE_DREGX:
	  case NODE_DREGX_ONCE:
	    return NEW_MATCH3(node2, node1);

	  case NODE_LIT:
	    if (TYPE(node2->nd_lit) == T_REGEXP) {
		return NEW_MATCH3(node2, node1);
	    }
	}
    }

    return NEW_CALL(node1, tMATCH, NEW_LIST(node2));
}

static NODE*
gettable_gen(struct parser_params *parser, ID id)
{
    if (id == keyword_self) {
	return NEW_SELF();
    }
    else if (id == keyword_nil) {
	return NEW_NIL();
    }
    else if (id == keyword_true) {
	return NEW_TRUE();
    }
    else if (id == keyword_false) {
	return NEW_FALSE();
    }
    else if (id == keyword__FILE__) {
	return NEW_STR(STR_NEW2(ruby_sourcefile));
    }
    else if (id == keyword__LINE__) {
	return NEW_LIT(INT2FIX(ruby_sourceline));
    }
    else if (id == keyword__ENCODING__) {
	return NEW_LIT(rb_enc_from_encoding(parser->enc));
    }
    else if (is_local_id(id)) {
	if (dyna_in_block() && dvar_defined(id)) return NEW_DVAR(id);
	if (local_id(id)) return NEW_LVAR(id);
	/* method call without arguments */
	return NEW_VCALL(id);
    }
    else if (is_global_id(id)) {
	return NEW_GVAR(id);
    }
    else if (is_instance_id(id)) {
	return NEW_IVAR(id);
    }
    else if (is_const_id(id)) {
	return NEW_CONST(id);
    }
    else if (is_class_id(id)) {
	return NEW_CVAR(id);
    }
    compile_error(PARSER_ARG "identifier %s is not valid to get", rb_id2name(id));
    return 0;
}

static NODE*
assignable_gen(struct parser_params *parser, ID id, NODE *val)
{
    if (!id) return 0;
    if (id == keyword_self) {
	yyerror("Can't change the value of self");
    }
    else if (id == keyword_nil) {
	yyerror("Can't assign to nil");
    }
    else if (id == keyword_true) {
	yyerror("Can't assign to true");
    }
    else if (id == keyword_false) {
	yyerror("Can't assign to false");
    }
    else if (id == keyword__FILE__) {
	yyerror("Can't assign to __FILE__");
    }
    else if (id == keyword__LINE__) {
	yyerror("Can't assign to __LINE__");
    }
    else if (id == keyword__ENCODING__) {
	yyerror("Can't assign to __ENCODING__");
    }
    else if (is_local_id(id)) {
	if (dyna_in_block()) {
	    if (dvar_curr(id)) {
		return NEW_DASGN_CURR(id, val);
	    }
	    else if (dvar_defined(id)) {
		return NEW_DASGN(id, val);
	    }
	    else if (local_id(id)) {
		return NEW_LASGN(id, val);
	    }
	    else{
		dyna_var(id);
		return NEW_DASGN_CURR(id, val);
	    }
	}
	else {
	    if (!local_id(id)) {
		local_var(id);
	    }
	    return NEW_LASGN(id, val);
	}
    }
    else if (is_global_id(id)) {
	return NEW_GASGN(id, val);
    }
    else if (is_instance_id(id)) {
	return NEW_IASGN(id, val);
    }
    else if (is_const_id(id)) {
	if (in_def || in_single)
	    yyerror("dynamic constant assignment");
	return NEW_CDECL(id, val, 0);
    }
    else if (is_class_id(id)) {
	return NEW_CVASGN(id, val);
    }
    else {
	compile_error(PARSER_ARG "identifier %s is not valid to set", rb_id2name(id));
    }
    return 0;
}

static void
shadowing_lvar_gen(struct parser_params *parser, ID name)
{
    ID uscore;

    CONST_ID(uscore, "_");
    if (uscore == name) return;
    if (dyna_in_block()) {
	if (dvar_curr(name)) {
	    yyerror("duplicated argument name");
	}
	else if (dvar_defined(name) || local_id(name)) {
	    rb_warningS("shadowing outer local variable - %s", rb_id2name(name));
	    vtable_add(lvtbl->vars, name);
	}
    }
    else {
	if (local_id(name)) {
	    yyerror("duplicated argument name");
	}
    }
}

static void
new_bv_gen(struct parser_params *parser, ID name)
{
    if (!name) return;
    if (!is_local_id(name)) {
	compile_error(PARSER_ARG "invalid local variable - %s",
		      rb_id2name(name));
	return;
    }
    shadowing_lvar(name);
    dyna_var(name);
}

static NODE *
aryset_gen(struct parser_params *parser, NODE *recv, NODE *idx)
{
    if (recv && nd_type(recv) == NODE_SELF)
	recv = (NODE *)1;
    return NEW_ATTRASGN(recv, tASET, idx);
}

static void
block_dup_check_gen(struct parser_params *parser, NODE *node1, NODE *node2)
{
    if (node2 && node1 && nd_type(node1) == NODE_BLOCK_PASS) {
	compile_error(PARSER_ARG "both block arg and actual block given");
    }
}

ID
rb_id_attrset(ID id)
{
    id &= ~ID_SCOPE_MASK;
    id |= ID_ATTRSET;
    return id;
}

static NODE *
attrset_gen(struct parser_params *parser, NODE *recv, ID id)
{
    if (recv && nd_type(recv) == NODE_SELF)
	recv = (NODE *)1;
    return NEW_ATTRASGN(recv, rb_id_attrset(id), 0);
}

static void
rb_backref_error_gen(struct parser_params *parser, NODE *node)
{
    switch (nd_type(node)) {
      case NODE_NTH_REF:
	compile_error(PARSER_ARG "Can't set variable $%ld", node->nd_nth);
	break;
      case NODE_BACK_REF:
	compile_error(PARSER_ARG "Can't set variable $%c", (int)node->nd_nth);
	break;
    }
}

static NODE *
arg_concat_gen(struct parser_params *parser, NODE *node1, NODE *node2)
{
    if (!node2) return node1;
    switch (nd_type(node1)) {
      case NODE_BLOCK_PASS:
	node1->nd_iter = arg_concat(node1->nd_iter, node2);
	return node1;
      case NODE_ARGSPUSH:
	if (nd_type(node2) != NODE_ARRAY) break;
	node1->nd_body = list_concat(NEW_LIST(node1->nd_body), node2);
	nd_set_type(node1, NODE_ARGSCAT);
	return node1;
      case NODE_ARGSCAT:
	if (nd_type(node2) != NODE_ARRAY) break;
	node1->nd_body = list_concat(node1->nd_body, node2);
	return node1;
    }
    return NEW_ARGSCAT(node1, node2);
}

static NODE *
arg_append_gen(struct parser_params *parser, NODE *node1, NODE *node2)
{
    if (!node1) return NEW_LIST(node2);
    switch (nd_type(node1))  {
      case NODE_ARRAY:
	return list_append(node1, node2);
      case NODE_BLOCK_PASS:
	node1->nd_head = arg_append(node1->nd_head, node2);
	return node1;
      case NODE_ARGSPUSH:
	node1->nd_body = list_append(NEW_LIST(node1->nd_body), node2);
	nd_set_type(node1, NODE_ARGSCAT);
	return node1;
    }
    return NEW_ARGSPUSH(node1, node2);
}

static NODE *
splat_array(NODE* node)
{
    if (nd_type(node) == NODE_SPLAT) node = node->nd_head;
    if (nd_type(node) == NODE_ARRAY) return node;
    return 0;
}

static NODE *
node_assign_gen(struct parser_params *parser, NODE *lhs, NODE *rhs)
{
    if (!lhs) return 0;

    switch (nd_type(lhs)) {
      case NODE_GASGN:
      case NODE_IASGN:
      case NODE_IASGN2:
      case NODE_LASGN:
      case NODE_DASGN:
      case NODE_DASGN_CURR:
      case NODE_MASGN:
      case NODE_CDECL:
      case NODE_CVASGN:
	lhs->nd_value = rhs;
	break;

      case NODE_ATTRASGN:
      case NODE_CALL:
	lhs->nd_args = arg_append(lhs->nd_args, rhs);
	break;

      default:
	/* should not happen */
	break;
    }

    return lhs;
}

static int
value_expr_gen(struct parser_params *parser, NODE *node)
{
    int cond = 0;

    if (!node) {
	rb_warning0("empty expression");
    }
    while (node) {
	switch (nd_type(node)) {
	  case NODE_DEFN:
	  case NODE_DEFS:
	    parser_warning(node, "void value expression");
	    return Qfalse;

	  case NODE_RETURN:
	  case NODE_BREAK:
	  case NODE_NEXT:
	  case NODE_REDO:
	  case NODE_RETRY:
	    if (!cond) yyerror("void value expression");
	    /* or "control never reach"? */
	    return Qfalse;

	  case NODE_BLOCK:
	    while (node->nd_next) {
		node = node->nd_next;
	    }
	    node = node->nd_head;
	    break;

	  case NODE_BEGIN:
	    node = node->nd_body;
	    break;

	  case NODE_IF:
	    if (!node->nd_body) {
		node = node->nd_else;
		break;
	    }
	    else if (!node->nd_else) {
		node = node->nd_body;
		break;
	    }
	    if (!value_expr(node->nd_body)) return Qfalse;
	    node = node->nd_else;
	    break;

	  case NODE_AND:
	  case NODE_OR:
	    cond = 1;
	    node = node->nd_2nd;
	    break;

	  default:
	    return Qtrue;
	}
    }

    return Qtrue;
}

static void
void_expr_gen(struct parser_params *parser, NODE *node)
{
    const char *useless = 0;

    if (!RTEST(ruby_verbose)) return;

    if (!node) return;
    switch (nd_type(node)) {
      case NODE_CALL:
	switch (node->nd_mid) {
	  case '+':
	  case '-':
	  case '*':
	  case '/':
	  case '%':
	  case tPOW:
	  case tUPLUS:
	  case tUMINUS:
	  case '|':
	  case '^':
	  case '&':
	  case tCMP:
	  case '>':
	  case tGEQ:
	  case '<':
	  case tLEQ:
	  case tEQ:
	  case tNEQ:
	    useless = rb_id2name(node->nd_mid);
	    break;
	}
	break;

      case NODE_LVAR:
      case NODE_DVAR:
      case NODE_GVAR:
      case NODE_IVAR:
      case NODE_CVAR:
      case NODE_NTH_REF:
      case NODE_BACK_REF:
	useless = "a variable";
	break;
      case NODE_CONST:
	useless = "a constant";
	break;
      case NODE_LIT:
      case NODE_STR:
      case NODE_DSTR:
      case NODE_DREGX:
      case NODE_DREGX_ONCE:
	useless = "a literal";
	break;
      case NODE_COLON2:
      case NODE_COLON3:
	useless = "::";
	break;
      case NODE_DOT2:
	useless = "..";
	break;
      case NODE_DOT3:
	useless = "...";
	break;
      case NODE_SELF:
	useless = "self";
	break;
      case NODE_NIL:
	useless = "nil";
	break;
      case NODE_TRUE:
	useless = "true";
	break;
      case NODE_FALSE:
	useless = "false";
	break;
      case NODE_DEFINED:
	useless = "defined?";
	break;
    }

    if (useless) {
	int line = ruby_sourceline;

	ruby_sourceline = nd_line(node);
	rb_warnS("useless use of %s in void context", useless);
	ruby_sourceline = line;
    }
}

static void
void_stmts_gen(struct parser_params *parser, NODE *node)
{
    if (!RTEST(ruby_verbose)) return;
    if (!node) return;
    if (nd_type(node) != NODE_BLOCK) return;

    for (;;) {
	if (!node->nd_next) return;
	void_expr0(node->nd_head);
	node = node->nd_next;
    }
}

static NODE *
remove_begin(NODE *node)
{
    NODE **n = &node, *n1 = node;
    while (n1 && nd_type(n1) == NODE_BEGIN && n1->nd_body) {
	*n = n1 = n1->nd_body;
    }
    return node;
}

static void
reduce_nodes_gen(struct parser_params *parser, NODE **body)
{
    NODE *node = *body;

    if (!node) {
	*body = NEW_NIL();
	return;
    }
#define subnodes(n1, n2) \
    ((!node->n1) ? (node->n2 ? (body = &node->n2, 1) : 0) : \
     (!node->n2) ? (body = &node->n1, 1) : \
     (reduce_nodes(&node->n1), body = &node->n2, 1))

    while (node) {
	switch (nd_type(node)) {
	  end:
	  case NODE_NIL:
	    *body = 0;
	    return;
	  case NODE_RETURN:
	    *body = node = node->nd_stts;
	    continue;
	  case NODE_BEGIN:
	    *body = node = node->nd_body;
	    continue;
	  case NODE_BLOCK:
	    body = &node->nd_end->nd_head;
	    break;
	  case NODE_IF:
	    if (subnodes(nd_body, nd_else)) break;
	    return;
	  case NODE_CASE:
	    body = &node->nd_body;
	    break;
	  case NODE_WHEN:
	    if (!subnodes(nd_body, nd_next)) goto end;
	    break;
	  case NODE_ENSURE:
	    if (!subnodes(nd_head, nd_resq)) goto end;
	    break;
	  case NODE_RESCUE:
	    if (!subnodes(nd_head, nd_resq)) goto end;
	    break;
	  default:
	    return;
	}
	node = *body;
    }

#undef subnodes
}

static int
assign_in_cond(struct parser_params *parser, NODE *node)
{
    switch (nd_type(node)) {
      case NODE_MASGN:
	yyerror("multiple assignment in conditional");
	return 1;

      case NODE_LASGN:
      case NODE_DASGN:
      case NODE_GASGN:
      case NODE_IASGN:
	break;

      default:
	return 0;
    }

    switch (nd_type(node->nd_value)) {
      case NODE_LIT:
      case NODE_STR:
      case NODE_NIL:
      case NODE_TRUE:
      case NODE_FALSE:
	/* reports always */
	parser_warn(node->nd_value, "found = in conditional, should be ==");
	return 1;

      case NODE_DSTR:
      case NODE_XSTR:
      case NODE_DXSTR:
      case NODE_EVSTR:
      case NODE_DREGX:
      default:
	break;
    }
    return 1;
}

static void
warn_unless_e_option(struct parser_params *parser, NODE *node, const char *str)
{
    if (!e_option_supplied(parser)) parser_warn(node, str);
}

static void
warning_unless_e_option(struct parser_params *parser, NODE *node, const char *str)
{
    if (!e_option_supplied(parser)) parser_warning(node, str);
}

static void
fixup_nodes(NODE **rootnode)
{
    NODE *node, *next, *head;

    for (node = *rootnode; node; node = next) {
	enum node_type type;
	VALUE val;

	next = node->nd_next;
	head = node->nd_head;
	rb_gc_force_recycle((VALUE)node);
	*rootnode = next;
	switch (type = nd_type(head)) {
	  case NODE_DOT2:
	  case NODE_DOT3:
	    val = rb_range_new(head->nd_beg->nd_lit, head->nd_end->nd_lit,
			       type == NODE_DOT3 ? Qtrue : Qfalse);
	    rb_gc_force_recycle((VALUE)head->nd_beg);
	    rb_gc_force_recycle((VALUE)head->nd_end);
	    nd_set_type(head, NODE_LIT);
	    head->nd_lit = val;
	    break;
	  default:
	    break;
	}
    }
}

static NODE *cond0(struct parser_params*,NODE*);

static NODE*
range_op(struct parser_params *parser, NODE *node)
{
    enum node_type type;

    if (node == 0) return 0;

    type = nd_type(node);
    value_expr(node);
    if (type == NODE_LIT && FIXNUM_P(node->nd_lit)) {
	warn_unless_e_option(parser, node, "integer literal in conditional range");
	return NEW_CALL(node, tEQ, NEW_LIST(NEW_GVAR(rb_intern("$."))));
    }
    return cond0(parser, node);
}

static int
literal_node(NODE *node)
{
    if (!node) return 1;	/* same as NODE_NIL */
    switch (nd_type(node)) {
      case NODE_LIT:
      case NODE_STR:
      case NODE_DSTR:
      case NODE_EVSTR:
      case NODE_DREGX:
      case NODE_DREGX_ONCE:
      case NODE_DSYM:
	return 2;
      case NODE_TRUE:
      case NODE_FALSE:
      case NODE_NIL:
	return 1;
    }
    return 0;
}

static NODE*
cond0(struct parser_params *parser, NODE *node)
{
    if (node == 0) return 0;
    assign_in_cond(parser, node);

    switch (nd_type(node)) {
      case NODE_DSTR:
      case NODE_EVSTR:
      case NODE_STR:
	rb_warn0("string literal in condition");
	break;

      case NODE_DREGX:
      case NODE_DREGX_ONCE:
	warning_unless_e_option(parser, node, "regex literal in condition");
	return NEW_MATCH2(node, NEW_GVAR(rb_intern("$_")));

      case NODE_AND:
      case NODE_OR:
	node->nd_1st = cond0(parser, node->nd_1st);
	node->nd_2nd = cond0(parser, node->nd_2nd);
	break;

      case NODE_DOT2:
      case NODE_DOT3:
	node->nd_beg = range_op(parser, node->nd_beg);
	node->nd_end = range_op(parser, node->nd_end);
	if (nd_type(node) == NODE_DOT2) nd_set_type(node,NODE_FLIP2);
	else if (nd_type(node) == NODE_DOT3) nd_set_type(node, NODE_FLIP3);
	if (!e_option_supplied(parser)) {
	    int b = literal_node(node->nd_beg);
	    int e = literal_node(node->nd_end);
	    if ((b == 1 && e == 1) || (b + e >= 2 && RTEST(ruby_verbose))) {
		parser_warn(node, "range literal in condition");
	    }
	}
	break;

      case NODE_DSYM:
	parser_warning(node, "literal in condition");
	break;

      case NODE_LIT:
	if (TYPE(node->nd_lit) == T_REGEXP) {
	    warn_unless_e_option(parser, node, "regex literal in condition");
	    nd_set_type(node, NODE_MATCH);
	}
	else {
	    parser_warning(node, "literal in condition");
	}
      default:
	break;
    }
    return node;
}

static NODE*
cond_gen(struct parser_params *parser, NODE *node)
{
    if (node == 0) return 0;
    return cond0(parser, node);
}

static NODE*
logop_gen(struct parser_params *parser, enum node_type type, NODE *left, NODE *right)
{
    value_expr(left);
    if (left && nd_type(left) == type) {
	NODE *node = left, *second;
	while ((second = node->nd_2nd) != 0 && nd_type(second) == type) {
	    node = second;
	}
	node->nd_2nd = NEW_NODE(type, second, right, 0);
	return left;
    }
    return NEW_NODE(type, left, right, 0);
}

static void
no_blockarg(struct parser_params *parser, NODE *node)
{
    if (node && nd_type(node) == NODE_BLOCK_PASS) {
	compile_error(PARSER_ARG "block argument should not be given");
    }
}

static NODE *
ret_args_gen(struct parser_params *parser, NODE *node)
{
    if (node) {
	no_blockarg(parser, node);
	if (nd_type(node) == NODE_ARRAY) {
	    if (node->nd_next == 0) {
		node = node->nd_head;
	    }
	    else {
		nd_set_type(node, NODE_VALUES);
	    }
	}
    }
    return node;
}

static NODE *
new_yield_gen(struct parser_params *parser, NODE *node)
{
    long state = Qtrue;

    if (node) {
        no_blockarg(parser, node);
	if (node && nd_type(node) == NODE_SPLAT) {
	    state = Qtrue;
	}
    }
    else {
        state = Qfalse;
    }
    return NEW_YIELD(node, state);
}

static NODE*
negate_lit(NODE *node)
{
    switch (TYPE(node->nd_lit)) {
      case T_FIXNUM:
	node->nd_lit = LONG2FIX(-FIX2LONG(node->nd_lit));
	break;
      case T_BIGNUM:
	node->nd_lit = rb_funcall(node->nd_lit,tUMINUS,0,0);
	break;
      case T_FLOAT:
	RFLOAT(node->nd_lit)->float_value = -RFLOAT_VALUE(node->nd_lit);
	break;
      default:
	break;
    }
    return node;
}

static NODE *
arg_blk_pass(NODE *node1, NODE *node2)
{
    if (node2) {
	node2->nd_head = node1;
	return node2;
    }
    return node1;
}

static NODE*
new_args_gen(struct parser_params *parser, NODE *m, NODE *o, ID r, NODE *p, ID b)
{
    int saved_line = ruby_sourceline;
    NODE *node;
    NODE *i1, *i2 = 0;

    node = NEW_ARGS(m ? m->nd_plen : 0, o);
    i1 = m ? m->nd_next : 0;
    node->nd_next = NEW_ARGS_AUX(r, b);

    if (p) {
	i2 = p->nd_next;
	node->nd_next->nd_next = NEW_ARGS_AUX(p->nd_pid, p->nd_plen);
    }
    else if (i1) {
	node->nd_next->nd_next = NEW_ARGS_AUX(0, 0);
    }
    if (i1 || i2) {
	node->nd_next->nd_next->nd_next = NEW_NODE(NODE_AND, i1, i2, 0);
    }
    ruby_sourceline = saved_line;
    return node;
}

static void
local_push_gen(struct parser_params *parser, int inherit_dvars)
{
    struct local_vars *local;

    local = ALLOC(struct local_vars);
    local->prev = lvtbl;
    local->args = vtable_alloc(0);
    local->vars = vtable_alloc(inherit_dvars ? DVARS_INHERIT : DVARS_TOPSCOPE);
    lvtbl = local;
}

static void
local_pop_gen(struct parser_params *parser)
{
    struct local_vars *local = lvtbl->prev;
    vtable_free(lvtbl->args);
    vtable_free(lvtbl->vars);
    xfree(lvtbl);
    lvtbl = local;
}

static ID*
vtable_tblcpy(ID *buf, const struct vtable *src)
{
    int i, cnt = vtable_size(src);

    if (cnt > 0) {
        buf[0] = cnt;
        for (i = 0; i < cnt; i++) {
            buf[i] = src->tbl[i];
        }
        return buf;
    }
    return 0;
}

static ID*
local_tbl_gen(struct parser_params *parser)
{
    int cnt = vtable_size(lvtbl->args) + vtable_size(lvtbl->vars);
    ID *buf;

    if (cnt <= 0) return 0;
    buf = ALLOC_N(ID, cnt + 1);
    vtable_tblcpy(buf+1, lvtbl->args);
    vtable_tblcpy(buf+vtable_size(lvtbl->args)+1, lvtbl->vars);
    buf[0] = cnt;
    return buf;
}

static int
arg_var_gen(struct parser_params *parser, ID id)
{
    vtable_add(lvtbl->args, id);
    return vtable_size(lvtbl->args) - 1;
}

static int
local_var_gen(struct parser_params *parser, ID id)
{
    vtable_add(lvtbl->vars, id);
    return vtable_size(lvtbl->vars) - 1;
}

static int
local_id_gen(struct parser_params *parser, ID id)
{
    struct vtable *vars, *args;

    vars = lvtbl->vars;
    args = lvtbl->args;

    while (vars && POINTER_P(vars->prev)) {
	vars = vars->prev;
	args = args->prev;
    }

    if (vars && vars->prev == DVARS_INHERIT) {
	return rb_local_defined(id);
    }
    else {
	return (vtable_included(args, id) ||
		vtable_included(vars, id));
    }
}

static void
dyna_push_gen(struct parser_params *parser)
{
    lvtbl->args = vtable_alloc(lvtbl->args);
    lvtbl->vars = vtable_alloc(lvtbl->vars);
}

static void
dyna_pop_gen(struct parser_params *parser)
{
    struct vtable *tmp;

    tmp = lvtbl->args;
    lvtbl->args = lvtbl->args->prev;
    vtable_free(tmp);
    tmp = lvtbl->vars;
    lvtbl->vars = lvtbl->vars->prev;
    vtable_free(tmp);
}

static int
dyna_in_block_gen(struct parser_params *parser)
{
    return POINTER_P(lvtbl->vars) && lvtbl->vars->prev != DVARS_TOPSCOPE;
}

static int
dvar_defined_gen(struct parser_params *parser, ID id)
{
    struct vtable *vars, *args;

    args = lvtbl->args;
    vars = lvtbl->vars;

    while (POINTER_P(vars)) {
	if (vtable_included(args, id)) {
	    return 1;
	}
	if (vtable_included(vars, id)) {
	    return 1;
	}
	args = args->prev;
	vars = vars->prev;
    }

    if (vars == DVARS_INHERIT) {
        return rb_dvar_defined(id);
    }

    return 0;
}

static int
dvar_curr_gen(struct parser_params *parser, ID id)
{
    return (vtable_included(lvtbl->args, id) ||
	    vtable_included(lvtbl->vars, id));
}

VALUE rb_reg_compile(VALUE str, int options);
VALUE rb_reg_check_preprocess(VALUE);

static void
reg_fragment_setenc_gen(struct parser_params* parser, VALUE str, int options)
{
    int c = RE_OPTION_ENCODING_IDX(options);

    if (c) {
	int opt, idx;
	rb_char_to_option_kcode(c, &opt, &idx);
	if (idx != ENCODING_GET(str) &&
	    rb_enc_str_coderange(str) != ENC_CODERANGE_7BIT) {
            goto error;
	}
	ENCODING_SET(str, idx);
    }
    else if (RE_OPTION_ENCODING_NONE(options)) {
        if (!ENCODING_IS_ASCII8BIT(str) &&
            rb_enc_str_coderange(str) != ENC_CODERANGE_7BIT) {
            c = 'n';
            goto error;
        }
	rb_enc_associate(str, rb_ascii8bit_encoding());
    }
    else if (parser->enc == rb_usascii_encoding()) {
	if (rb_enc_str_coderange(str) != ENC_CODERANGE_7BIT) {
	    /* raise in re.c */
	    rb_enc_associate(str, rb_usascii_encoding());
	}
	else {
	    rb_enc_associate(str, rb_ascii8bit_encoding());
	}
    }
    return;

  error:
    compile_error(PARSER_ARG
        "regexp encoding option '%c' differs from source encoding '%s'",
        c, rb_enc_name(rb_enc_get(str)));
}

static void
reg_fragment_check_gen(struct parser_params* parser, VALUE str, int options)
{
    VALUE err;
    reg_fragment_setenc_gen(parser, str, options);
    err = rb_reg_check_preprocess(str);
    if (err != Qnil) {
        err = rb_obj_as_string(err);
        compile_error(PARSER_ARG "%s", RSTRING_PTR(err));
	RB_GC_GUARD(err);
    }
}

typedef struct {
    struct parser_params* parser;
    rb_encoding *enc;
    NODE *succ_block;
    NODE *fail_block;
    int num;
} reg_named_capture_assign_t;

static int
reg_named_capture_assign_iter(const OnigUChar *name, const OnigUChar *name_end,
          int back_num, int *back_refs, OnigRegex regex, void *arg0)
{
    reg_named_capture_assign_t *arg = (reg_named_capture_assign_t*)arg0;
    struct parser_params* parser = arg->parser;
    rb_encoding *enc = arg->enc;
    int len = name_end - name;
    const char *s = (const char *)name;
    ID var;

    arg->num++;

    if (arg->succ_block == 0) {
        arg->succ_block = NEW_BEGIN(0);
        arg->fail_block = NEW_BEGIN(0);
    }

    if (!len || (*name != '_' && ISASCII(*name) && !rb_enc_islower(*name, enc)) ||
	rb_reserved_word(s, len) || !rb_enc_symname2_p(s, len, enc)) {
        return ST_CONTINUE;
    }
    var = rb_intern3(s, len, enc);
    if (dvar_defined(var) || local_id(var)) {
        rb_warningS("named capture conflicts a local variable - %s",
                    rb_id2name(var));
    }
    arg->succ_block = block_append(arg->succ_block,
        newline_node(node_assign(assignable(var,0),
            NEW_CALL(
              gettable(rb_intern("$~")),
              idAREF,
              NEW_LIST(NEW_LIT(ID2SYM(var))))
            )));
    arg->fail_block = block_append(arg->fail_block,
        newline_node(node_assign(assignable(var,0), NEW_LIT(Qnil))));
    return ST_CONTINUE;
}

static NODE *
reg_named_capture_assign_gen(struct parser_params* parser, VALUE regexp, NODE *match)
{
    reg_named_capture_assign_t arg;

    arg.parser = parser;
    arg.enc = rb_enc_get(regexp);
    arg.succ_block = 0;
    arg.fail_block = 0;
    arg.num = 0;
    onig_foreach_name(RREGEXP(regexp)->ptr, reg_named_capture_assign_iter, (void*)&arg);

    if (arg.num == 0)
        return match;

    return
        block_append(
            newline_node(match),
            NEW_IF(gettable(rb_intern("$~")),
                block_append(
                    newline_node(arg.succ_block),
                    newline_node(
                        NEW_CALL(
                          gettable(rb_intern("$~")),
                          rb_intern("begin"),
                          NEW_LIST(NEW_LIT(INT2FIX(0)))))),
                block_append(
                    newline_node(arg.fail_block),
                    newline_node(
                        NEW_LIT(Qnil)))));
}

static VALUE
reg_compile_gen(struct parser_params* parser, VALUE str, int options)
{
    VALUE re;
    VALUE err;

    reg_fragment_setenc(str, options);
    err = rb_errinfo();
    re = rb_reg_compile(str, options & RE_OPTION_MASK);
    if (NIL_P(re)) {
	ID mesg = rb_intern("mesg");
	VALUE m = rb_attr_get(rb_errinfo(), mesg);
	rb_set_errinfo(err);
	if (!NIL_P(err)) {
	    rb_str_append(rb_str_cat(rb_attr_get(err, mesg), "\n", 1), m);
	}
	else {
	    compile_error(PARSER_ARG "%s", RSTRING_PTR(m));
	}
	return Qnil;
    }
    return re;
}

void
rb_gc_mark_parser(void)
{
}

NODE*
rb_parser_append_print(VALUE vparser, NODE *node)
{
    NODE *prelude = 0;
    NODE *scope = node;
    struct parser_params *parser;

    if (!node) return node;

    Data_Get_Struct(vparser, struct parser_params, parser);

    node = node->nd_body;

    if (nd_type(node) == NODE_PRELUDE) {
	prelude = node;
	node = node->nd_body;
    }

    node = block_append(node,
			NEW_FCALL(rb_intern("print"),
				  NEW_ARRAY(NEW_GVAR(rb_intern("$_")))));
    if (prelude) {
	prelude->nd_body = node;
	scope->nd_body = prelude;
    }
    else {
	scope->nd_body = node;
    }

    return scope;
}

NODE *
rb_parser_while_loop(VALUE vparser, NODE *node, int chop, int split)
{
    NODE *prelude = 0;
    NODE *scope = node;
    struct parser_params *parser;

    if (!node) return node;

    Data_Get_Struct(vparser, struct parser_params, parser);

    node = node->nd_body;

    if (nd_type(node) == NODE_PRELUDE) {
	prelude = node;
	node = node->nd_body;
    }
    if (split) {
	node = block_append(NEW_GASGN(rb_intern("$F"),
				      NEW_CALL(NEW_GVAR(rb_intern("$_")),
					       rb_intern("split"), 0)),
			    node);
    }
    if (chop) {
	node = block_append(NEW_CALL(NEW_GVAR(rb_intern("$_")),
				     rb_intern("chop!"), 0), node);
    }

    node = NEW_OPT_N(node);

    if (prelude) {
	prelude->nd_body = node;
	scope->nd_body = prelude;
    }
    else {
	scope->nd_body = node;
    }

    return scope;
}

static const struct {
    ID token;
    const char *name;
} op_tbl[] = {
    {tDOT2,	".."},
    {tDOT3,	"..."},
    {'+',	"+(binary)"},
    {'-',	"-(binary)"},
    {tPOW,	"**"},
    {tUPLUS,	"+@"},
    {tUMINUS,	"-@"},
    {tCMP,	"<=>"},
    {tGEQ,	">="},
    {tLEQ,	"<="},
    {tEQ,	"=="},
    {tEQQ,	"==="},
    {tNEQ,	"!="},
    {tMATCH,	"=~"},
    {tNMATCH,	"!~"},
    {tAREF,	"[]"},
    {tASET,	"[]="},
    {tLSHFT,	"<<"},
    {tRSHFT,	">>"},
    {tCOLON2,	"::"},
};

#define op_tbl_count (sizeof(op_tbl) / sizeof(op_tbl[0]))

static struct symbols {
    ID last_id;
    st_table *sym_id;
    st_table *id_str;
    st_table *ivar2_id;
    st_table *id_ivar2;
    VALUE op_sym[tLAST_TOKEN];
} global_symbols = {tLAST_ID};

static const struct st_hash_type symhash = {
    rb_str_hash_cmp,
    rb_str_hash,
};

struct ivar2_key {
    ID id;
    VALUE klass;
};

static int
ivar2_cmp(struct ivar2_key *key1, struct ivar2_key *key2)
{
    if (key1->id == key2->id && key1->klass == key2->klass) {
	return 0;
    }
    return 1;
}

static int
ivar2_hash(struct ivar2_key *key)
{
    return (key->id << 8) ^ (key->klass >> 2);
}

static const struct st_hash_type ivar2_hash_type = {
    ivar2_cmp,
    ivar2_hash,
};

void
Init_sym(void)
{
    global_symbols.sym_id = st_init_table_with_size(&symhash, 1000);
    global_symbols.id_str = st_init_numtable_with_size(1000);
    global_symbols.ivar2_id = st_init_table_with_size(&ivar2_hash_type, 1000);
    global_symbols.id_ivar2 = st_init_numtable_with_size(1000);

    Init_id();
}

void
rb_gc_mark_symbols(void)
{
    rb_mark_tbl(global_symbols.id_str);
    rb_gc_mark_locations(global_symbols.op_sym,
			 global_symbols.op_sym + tLAST_TOKEN);
}

static ID
internal_id_gen(struct parser_params *parser)
{
    ID id = (ID)vtable_size(lvtbl->args) + (ID)vtable_size(lvtbl->vars);
    id += ((tLAST_TOKEN - ID_INTERNAL) >> ID_SCOPE_SHIFT) + 1;
    return ID_INTERNAL | (id << ID_SCOPE_SHIFT);
}

static int
is_special_global_name(const char *m, const char *e, rb_encoding *enc)
{
    int mb = 0;

    if (m >= e) return 0;
    switch (*m) {
      case '~': case '*': case '$': case '?': case '!': case '@':
      case '/': case '\\': case ';': case ',': case '.': case '=':
      case ':': case '<': case '>': case '\"':
      case '&': case '`': case '\'': case '+':
      case '0':
	++m;
	break;
      case '-':
	++m;
	if (m < e && is_identchar(m, e, enc)) {
	    if (!ISASCII(*m)) mb = 1;
	    m += rb_enc_mbclen(m, e, enc);
	}
	break;
      default:
	if (!rb_enc_isdigit(*m, enc)) return 0;
	do {
	    if (!ISASCII(*m)) mb = 1;
	    ++m;
	} while (rb_enc_isdigit(*m, enc));
    }
    return m == e ? mb + 1 : 0;
}

int
rb_symname_p(const char *name)
{
    return rb_enc_symname_p(name, rb_ascii8bit_encoding());
}

int
rb_enc_symname_p(const char *name, rb_encoding *enc)
{
    return rb_enc_symname2_p(name, strlen(name), enc);
}

int
rb_enc_symname2_p(const char *name, int len, rb_encoding *enc)
{
    const char *m = name;
    const char *e = m + len;
    int localid = Qfalse;

    if (!m) return Qfalse;
    switch (*m) {
      case '\0':
	return Qfalse;

      case '$':
	if (is_special_global_name(++m, e, enc)) return Qtrue;
	goto id;

      case '@':
	if (*++m == '@') ++m;
	goto id;

      case '<':
	switch (*++m) {
	  case '<': ++m; break;
	  case '=': if (*++m == '>') ++m; break;
	  default: break;
	}
	break;

      case '>':
	switch (*++m) {
	  case '>': case '=': ++m; break;
	}
	break;

      case '=':
	switch (*++m) {
	  case '~': ++m; break;
	  case '=': if (*++m == '=') ++m; break;
	  default: return Qfalse;
	}
	break;

      case '*':
	if (*++m == '*') ++m;
	break;

      case '+': case '-':
	if (*++m == '@') ++m;
	break;

      case '|': case '^': case '&': case '/': case '%': case '~': case '`':
	++m;
	break;

      case '[':
	if (*++m != ']') return Qfalse;
	if (*++m == '=') ++m;
	break;

      case '!':
	switch (*++m) {
	  case '\0': return Qtrue;
	  case '=': case '~': ++m; break;
	  default: return Qfalse;
	}
	break;

      default:
	localid = !rb_enc_isupper(*m, enc);
      id:
	if (m >= e || (*m != '_' && !rb_enc_isalpha(*m, enc) && ISASCII(*m)))
	    return Qfalse;
	while (m < e && is_identchar(m, e, enc)) m += rb_enc_mbclen(m, e, enc);
	if (localid) {
	    switch (*m) {
	      case '!': case '?': case '=': ++m;
	    }
	}
	break;
    }
    return *m ? Qfalse : Qtrue;
}

static ID
register_symid(ID id, const char *name, long len, rb_encoding *enc)
{
    VALUE str = rb_enc_str_new(name, len, enc);
    OBJ_FREEZE(str);
    st_add_direct(global_symbols.sym_id, (st_data_t)str, id);
    st_add_direct(global_symbols.id_str, id, (st_data_t)str);
    return id;
}

ID
rb_intern3(const char *name, long len, rb_encoding *enc)
{
    const char *m = name;
    const char *e = m + len;
    unsigned char c;
    VALUE str;
    ID id;
    int last;
    int mb;
    struct RString fake_str;
    fake_str.basic.flags = T_STRING|RSTRING_NOEMBED|FL_FREEZE;
    fake_str.basic.klass = rb_cString;
    fake_str.as.heap.len = len;
    fake_str.as.heap.ptr = (char *)name;
    fake_str.as.heap.aux.capa = len;
    str = (VALUE)&fake_str;
    rb_enc_associate(str, enc);

    if (st_lookup(global_symbols.sym_id, str, (st_data_t *)&id))
	return id;

    if (rb_cString && !rb_enc_asciicompat(enc)) {
	id = ID_JUNK;
	goto new_id;
    }
    last = len-1;
    id = 0;
    switch (*m) {
      case '$':
	id |= ID_GLOBAL;
	if ((mb = is_special_global_name(++m, e, enc)) != 0) {
	    if (!--mb) enc = rb_ascii8bit_encoding();
	    goto new_id;
	}
	break;
      case '@':
	if (m[1] == '@') {
	    m++;
	    id |= ID_CLASS;
	}
	else {
	    id |= ID_INSTANCE;
	}
	m++;
	break;
      default:
	c = m[0];
	if (c != '_' && rb_enc_isascii(c, enc) && rb_enc_ispunct(c, enc)) {
	    /* operators */
	    int i;

	    if (len == 1) {
		id = c;
		goto id_register;
	    }
	    for (i = 0; i < op_tbl_count; i++) {
		if (*op_tbl[i].name == *m &&
		    strcmp(op_tbl[i].name, m) == 0) {
		    id = op_tbl[i].token;
		    goto id_register;
		}
	    }
	}

	if (m[last] == '=') {
	    /* attribute assignment */
	    id = rb_intern3(name, last, enc);
	    if (id > tLAST_TOKEN && !is_attrset_id(id)) {
		enc = rb_enc_get(rb_id2str(id));
		id = rb_id_attrset(id);
		goto id_register;
	    }
	    id = ID_ATTRSET;
	}
	else if (rb_enc_isupper(m[0], enc)) {
	    id = ID_CONST;
        }
	else {
	    id = ID_LOCAL;
	}
	break;
    }
    mb = 0;
    if (!rb_enc_isdigit(*m, enc)) {
	while (m <= name + last && is_identchar(m, e, enc)) {
	    if (ISASCII(*m)) {
		m++;
	    }
	    else {
		mb = 1;
		m += rb_enc_mbclen(m, e, enc);
	    }
	}
    }
    if (m - name < len) id = ID_JUNK;
    if (enc != rb_usascii_encoding()) {
	/*
	 * this clause makes sense only when called from other than
	 * rb_intern_str() taking care of code-range.
	 */
	if (!mb) {
	    for (; m <= name + len; ++m) {
		if (!ISASCII(*m)) goto mbstr;
	    }
	    enc = rb_usascii_encoding();
	}
      mbstr:;
    }
  new_id:
    id |= ++global_symbols.last_id << ID_SCOPE_SHIFT;
  id_register:
    return register_symid(id, name, len, enc);
}

ID
rb_intern2(const char *name, long len)
{
    return rb_intern3(name, len, rb_usascii_encoding());
}

#undef rb_intern
ID
rb_intern(const char *name)
{
    return rb_intern2(name, strlen(name));
}

ID
rb_intern_str(VALUE str)
{
    rb_encoding *enc;
    ID id;

    if (rb_enc_str_coderange(str) == ENC_CODERANGE_7BIT) {
	enc = rb_usascii_encoding();
    }
    else {
	enc = rb_enc_get(str);
    }
    id = rb_intern3(RSTRING_PTR(str), RSTRING_LEN(str), enc);
    RB_GC_GUARD(str);
    return id;
}

VALUE
rb_id2str(ID id)
{
    st_data_t data;

    if (id < tLAST_TOKEN) {
	int i = 0;

	if (rb_ispunct(id)) {
	    VALUE str = global_symbols.op_sym[i = (int)id];
	    if (!str) {
		char name[2];
		name[0] = (char)id;
		name[1] = 0;
		str = rb_usascii_str_new(name, 1);
		OBJ_FREEZE(str);
		global_symbols.op_sym[i] = str;
	    }
	    return str;
	}
	for (i = 0; i < op_tbl_count; i++) {
	    if (op_tbl[i].token == id) {
		VALUE str = global_symbols.op_sym[i];
		if (!str) {
		    str = rb_usascii_str_new2(op_tbl[i].name);
		    OBJ_FREEZE(str);
		    global_symbols.op_sym[i] = str;
		}
		return str;
	    }
	}
    }

    if (st_lookup(global_symbols.id_str, id, &data)) {
        VALUE str = (VALUE)data;
        if (RBASIC(str)->klass == 0)
            RBASIC(str)->klass = rb_cString;
	return str;
    }

    if (is_attrset_id(id)) {
	ID id2 = (id & ~ID_SCOPE_MASK) | ID_LOCAL;
	VALUE str;

	while (!(str = rb_id2str(id2))) {
	    if (!is_local_id(id2)) return 0;
	    id2 = (id & ~ID_SCOPE_MASK) | ID_CONST;
	}
	str = rb_str_dup(str);
	rb_str_cat(str, "=", 1);
	rb_intern_str(str);
	if (st_lookup(global_symbols.id_str, id, &data)) {
            VALUE str = (VALUE)data;
            if (RBASIC(str)->klass == 0)
                RBASIC(str)->klass = rb_cString;
            return str;
        }
    }
    return 0;
}

const char *
rb_id2name(ID id)
{
    VALUE str = rb_id2str(id);

    if (!str) return 0;
    return RSTRING_PTR(str);
}

static int
symbols_i(VALUE sym, ID value, VALUE ary)
{
    rb_ary_push(ary, ID2SYM(value));
    return ST_CONTINUE;
}

/*
 *  call-seq:
 *     Symbol.all_symbols    => array
 *
 *  Returns an array of all the symbols currently in Ruby's symbol
 *  table.
 *
 *     Symbol.all_symbols.size    #=> 903
 *     Symbol.all_symbols[1,20]   #=> [:floor, :ARGV, :Binding, :symlink,
 *                                     :chown, :EOFError, :$;, :String,
 *                                     :LOCK_SH, :"setuid?", :$<,
 *                                     :default_proc, :compact, :extend,
 *                                     :Tms, :getwd, :$=, :ThreadGroup,
 *                                     :wait2, :$>]
 */

VALUE
rb_sym_all_symbols(void)
{
    VALUE ary = rb_ary_new2(global_symbols.sym_id->num_entries);

    st_foreach(global_symbols.sym_id, symbols_i, ary);
    return ary;
}

int
rb_is_const_id(ID id)
{
    if (is_const_id(id)) return Qtrue;
    return Qfalse;
}

int
rb_is_class_id(ID id)
{
    if (is_class_id(id)) return Qtrue;
    return Qfalse;
}

int
rb_is_instance_id(ID id)
{
    if (is_instance_id(id)) return Qtrue;
    return Qfalse;
}

int
rb_is_local_id(ID id)
{
    if (is_local_id(id)) return Qtrue;
    return Qfalse;
}

int
rb_is_junk_id(ID id)
{
    if (is_junk_id(id)) return Qtrue;
    return Qfalse;
}

#endif /* !RIPPER */

static void
parser_initialize(struct parser_params *parser)
{
    parser->eofp = Qfalse;

    parser->parser_lex_strterm = 0;
    parser->parser_cond_stack = 0;
    parser->parser_cmdarg_stack = 0;
    parser->parser_class_nest = 0;
    parser->parser_paren_nest = 0;
    parser->parser_lpar_beg = 0;
    parser->parser_in_single = 0;
    parser->parser_in_def = 0;
    parser->parser_in_defined = 0;
    parser->parser_compile_for_eval = 0;
    parser->parser_cur_mid = 0;
    parser->parser_tokenbuf = NULL;
    parser->parser_tokidx = 0;
    parser->parser_toksiz = 0;
    parser->parser_heredoc_end = 0;
    parser->parser_command_start = Qtrue;
    parser->parser_deferred_nodes = 0;
    parser->parser_lex_pbeg = 0;
    parser->parser_lex_p = 0;
    parser->parser_lex_pend = 0;
    parser->parser_lvtbl = 0;
    parser->parser_ruby__end__seen = 0;
    parser->parser_ruby_sourcefile = 0;
#ifndef RIPPER
    parser->is_ripper = 0;
    parser->parser_eval_tree_begin = 0;
    parser->parser_eval_tree = 0;
#else
    parser->is_ripper = 1;
    parser->parser_ruby_sourcefile_string = Qnil;
    parser->delayed = Qnil;

    parser->result = Qnil;
    parser->parsing_thread = Qnil;
    parser->toplevel_p = Qtrue;
#endif
#ifdef YYMALLOC
    parser->heap = NULL;
#endif
    parser->enc = rb_usascii_encoding();
}

#ifdef RIPPER
#define parser_mark ripper_parser_mark
#define parser_free ripper_parser_free
#endif

static void
parser_mark(void *ptr)
{
    struct parser_params *p = (struct parser_params*)ptr;

    rb_gc_mark((VALUE)p->parser_lex_strterm);
    rb_gc_mark((VALUE)p->parser_deferred_nodes);
    rb_gc_mark(p->parser_lex_input);
    rb_gc_mark(p->parser_lex_lastline);
    rb_gc_mark(p->parser_lex_nextline);
#ifndef RIPPER
    rb_gc_mark((VALUE)p->parser_eval_tree_begin) ;
    rb_gc_mark((VALUE)p->parser_eval_tree) ;
    rb_gc_mark(p->debug_lines);
#else
    rb_gc_mark(p->parser_ruby_sourcefile_string);
    rb_gc_mark(p->delayed);
    rb_gc_mark(p->value);
    rb_gc_mark(p->result);
    rb_gc_mark(p->parsing_thread);
#endif
#ifdef YYMALLOC
    rb_gc_mark((VALUE)p->heap);
#endif
}

static void
parser_free(void *ptr)
{
    struct parser_params *p = (struct parser_params*)ptr;
    struct local_vars *local, *prev;

    if (p->parser_tokenbuf) {
        xfree(p->parser_tokenbuf);
    }
    for (local = p->parser_lvtbl; local; local = prev) {
	if (local->vars) xfree(local->vars);
	prev = local->prev;
	xfree(local);
    }
#ifndef RIPPER
    xfree(p->parser_ruby_sourcefile);
#endif
    xfree(p);
}

VALUE rb_parser_get_yydebug(VALUE);
VALUE rb_parser_set_yydebug(VALUE, VALUE);

#ifndef RIPPER
static struct parser_params *
parser_new(void)
{
    struct parser_params *p;

    p = ALLOC_N(struct parser_params, 1);
    MEMZERO(p, struct parser_params, 1);
    parser_initialize(p);
    return p;
}

VALUE
rb_parser_new(void)
{
    struct parser_params *p = parser_new();

    return Data_Wrap_Struct(0, parser_mark, parser_free, p);
}

/*
 *  call-seq:
 *    ripper#end_seen?   -> Boolean
 *
 *  Return if parsed source ended by +\_\_END\_\_+.
 *  This number starts from 1.
 */
VALUE
rb_parser_end_seen_p(VALUE vparser)
{
    struct parser_params *parser;

    Data_Get_Struct(vparser, struct parser_params, parser);
    return ruby__end__seen ? Qtrue : Qfalse;
}

/*
 *  call-seq:
 *    ripper#encoding   -> encoding
 *
 *  Return encoding of the source.
 */
VALUE
rb_parser_encoding(VALUE vparser)
{
    struct parser_params *parser;

    Data_Get_Struct(vparser, struct parser_params, parser);
    return rb_enc_from_encoding(parser->enc);
}

/*
 *  call-seq:
 *    ripper.yydebug   -> true or false
 *
 *  Get yydebug.
 */
VALUE
rb_parser_get_yydebug(VALUE self)
{
    struct parser_params *parser;

    Data_Get_Struct(self, struct parser_params, parser);
    return yydebug ? Qtrue : Qfalse;
}

/*
 *  call-seq:
 *    ripper.yydebug = flag
 *
 *  Set yydebug.
 */
VALUE
rb_parser_set_yydebug(VALUE self, VALUE flag)
{
    struct parser_params *parser;

    Data_Get_Struct(self, struct parser_params, parser);
    yydebug = RTEST(flag);
    return flag;
}

#ifdef YYMALLOC
#define HEAPCNT(n, size) ((n) * (size) / sizeof(YYSTYPE))
#define NEWHEAP() rb_node_newnode(NODE_ALLOCA, 0, (VALUE)parser->heap, 0)
#define ADD2HEAP(n, c, p) ((parser->heap = (n))->u1.node = (p), \
			   (n)->u3.cnt = (c), (p))

void *
rb_parser_malloc(struct parser_params *parser, size_t size)
{
    size_t cnt = HEAPCNT(1, size);
    NODE *n = NEWHEAP();
    void *ptr = xmalloc(size);

    return ADD2HEAP(n, cnt, ptr);
}

void *
rb_parser_calloc(struct parser_params *parser, size_t nelem, size_t size)
{
    size_t cnt = HEAPCNT(nelem, size);
    NODE *n = NEWHEAP();
    void *ptr = xcalloc(nelem, size);

    return ADD2HEAP(n, cnt, ptr);
}

void *
rb_parser_realloc(struct parser_params *parser, void *ptr, size_t size)
{
    NODE *n;
    size_t cnt = HEAPCNT(1, size);

    if (ptr && (n = parser->heap) != NULL) {
	do {
	    if (n->u1.node == ptr) {
		n->u1.node = ptr = xrealloc(ptr, size);
		if (n->u3.cnt) n->u3.cnt = cnt;
		return ptr;
	    }
	} while ((n = n->u2.node) != NULL);
    }
    n = NEWHEAP();
    ptr = xrealloc(ptr, size);
    return ADD2HEAP(n, cnt, ptr);
}

void
rb_parser_free(struct parser_params *parser, void *ptr)
{
    NODE **prev = &parser->heap, *n;

    while ((n = *prev) != NULL) {
	if (n->u1.node == ptr) {
	    *prev = n->u2.node;
	    rb_gc_force_recycle((VALUE)n);
	    break;
	}
	prev = &n->u2.node;
    }
    xfree(ptr);
}
#endif
#endif

#ifdef RIPPER
#ifdef RIPPER_DEBUG
extern int rb_is_pointer_to_heap(VALUE);

/* :nodoc: */
static VALUE
ripper_validate_object(VALUE self, VALUE x)
{
    if (x == Qfalse) return x;
    if (x == Qtrue) return x;
    if (x == Qnil) return x;
    if (x == Qundef)
        rb_raise(rb_eArgError, "Qundef given");
    if (FIXNUM_P(x)) return x;
    if (SYMBOL_P(x)) return x;
    if (!rb_is_pointer_to_heap(x))
        rb_raise(rb_eArgError, "invalid pointer: %p", x);
    switch (TYPE(x)) {
      case T_STRING:
      case T_OBJECT:
      case T_ARRAY:
      case T_BIGNUM:
      case T_FLOAT:
        return x;
      case T_NODE:
        rb_raise(rb_eArgError, "NODE given: %p", x);
      default:
        rb_raise(rb_eArgError, "wrong type of ruby object: %p (%s)",
                 x, rb_obj_classname(x));
    }
    return x;
}
#endif

#define validate(x)

static VALUE
ripper_dispatch0(struct parser_params *parser, ID mid)
{
    return rb_funcall(parser->value, mid, 0);
}

static VALUE
ripper_dispatch1(struct parser_params *parser, ID mid, VALUE a)
{
    validate(a);
    return rb_funcall(parser->value, mid, 1, a);
}

static VALUE
ripper_dispatch2(struct parser_params *parser, ID mid, VALUE a, VALUE b)
{
    validate(a);
    validate(b);
    return rb_funcall(parser->value, mid, 2, a, b);
}

static VALUE
ripper_dispatch3(struct parser_params *parser, ID mid, VALUE a, VALUE b, VALUE c)
{
    validate(a);
    validate(b);
    validate(c);
    return rb_funcall(parser->value, mid, 3, a, b, c);
}

static VALUE
ripper_dispatch4(struct parser_params *parser, ID mid, VALUE a, VALUE b, VALUE c, VALUE d)
{
    validate(a);
    validate(b);
    validate(c);
    validate(d);
    return rb_funcall(parser->value, mid, 4, a, b, c, d);
}

static VALUE
ripper_dispatch5(struct parser_params *parser, ID mid, VALUE a, VALUE b, VALUE c, VALUE d, VALUE e)
{
    validate(a);
    validate(b);
    validate(c);
    validate(d);
    validate(e);
    return rb_funcall(parser->value, mid, 5, a, b, c, d, e);
}

static const struct kw_assoc {
    ID id;
    const char *name;
} keyword_to_name[] = {
    {keyword_class,	"class"},
    {keyword_module,	"module"},
    {keyword_def,	"def"},
    {keyword_undef,	"undef"},
    {keyword_begin,	"begin"},
    {keyword_rescue,	"rescue"},
    {keyword_ensure,	"ensure"},
    {keyword_end,	"end"},
    {keyword_if,	"if"},
    {keyword_unless,	"unless"},
    {keyword_then,	"then"},
    {keyword_elsif,	"elsif"},
    {keyword_else,	"else"},
    {keyword_case,	"case"},
    {keyword_when,	"when"},
    {keyword_while,	"while"},
    {keyword_until,	"until"},
    {keyword_for,	"for"},
    {keyword_break,	"break"},
    {keyword_next,	"next"},
    {keyword_redo,	"redo"},
    {keyword_retry,	"retry"},
    {keyword_in,	"in"},
    {keyword_do,	"do"},
    {keyword_do_cond,	"do"},
    {keyword_do_block,	"do"},
    {keyword_return,	"return"},
    {keyword_yield,	"yield"},
    {keyword_super,	"super"},
    {keyword_self,	"self"},
    {keyword_nil,	"nil"},
    {keyword_true,	"true"},
    {keyword_false,	"false"},
    {keyword_and,	"and"},
    {keyword_or,	"or"},
    {keyword_not,	"not"},
    {modifier_if,	"if"},
    {modifier_unless,	"unless"},
    {modifier_while,	"while"},
    {modifier_until,	"until"},
    {modifier_rescue,	"rescue"},
    {keyword_alias,	"alias"},
    {keyword_defined,	"defined?"},
    {keyword_BEGIN,	"BEGIN"},
    {keyword_END,	"END"},
    {keyword__LINE__,	"__LINE__"},
    {keyword__FILE__,	"__FILE__"},
    {keyword__ENCODING__, "__ENCODING__"},
    {0, NULL}
};

static const char*
keyword_id_to_str(ID id)
{
    const struct kw_assoc *a;

    for (a = keyword_to_name; a->id; a++) {
        if (a->id == id)
            return a->name;
    }
    return NULL;
}

#undef ripper_id2sym
static VALUE
ripper_id2sym(ID id)
{
    const char *name;
    char buf[8];

    if (id <= 256) {
        buf[0] = id;
        buf[1] = '\0';
        return ID2SYM(rb_intern(buf));
    }
    if ((name = keyword_id_to_str(id))) {
        return ID2SYM(rb_intern(name));
    }
    switch (id) {
      case tOROP:
        name = "||";
        break;
      case tANDOP:
        name = "&&";
        break;
      default:
        name = rb_id2name(id);
        if (!name) {
            rb_bug("cannot convert ID to string: %ld", (unsigned long)id);
        }
        break;
    }
    return ID2SYM(rb_intern(name));
}

static VALUE
ripper_intern(const char *s)
{
    return ID2SYM(rb_intern(s));
}

static void
ripper_compile_error(struct parser_params *parser, const char *fmt, ...)
{
    VALUE str;
    va_list args;

    va_start(args, fmt);
    str = rb_vsprintf(fmt, args);
    va_end(args);
    rb_funcall(parser->value, rb_intern("compile_error"), 1, str);
}

static void
ripper_warn0(struct parser_params *parser, const char *fmt)
{
    rb_funcall(parser->value, rb_intern("warn"), 1, STR_NEW2(fmt));
}

static void
ripper_warnI(struct parser_params *parser, const char *fmt, int a)
{
    rb_funcall(parser->value, rb_intern("warn"), 2,
               STR_NEW2(fmt), INT2NUM(a));
}

#if 0
static void
ripper_warnS(struct parser_params *parser, const char *fmt, const char *str)
{
    rb_funcall(parser->value, rb_intern("warn"), 2,
               STR_NEW2(fmt), STR_NEW2(str));
}
#endif

static void
ripper_warning0(struct parser_params *parser, const char *fmt)
{
    rb_funcall(parser->value, rb_intern("warning"), 1, STR_NEW2(fmt));
}

static void
ripper_warningS(struct parser_params *parser, const char *fmt, const char *str)
{
    rb_funcall(parser->value, rb_intern("warning"), 2,
               STR_NEW2(fmt), STR_NEW2(str));
}

static VALUE
ripper_lex_get_generic(struct parser_params *parser, VALUE src)
{
    return rb_funcall(src, ripper_id_gets, 0);
}

static VALUE
ripper_s_allocate(VALUE klass)
{
    struct parser_params *p;
    VALUE self;

    p = ALLOC_N(struct parser_params, 1);
    MEMZERO(p, struct parser_params, 1);
    self = Data_Wrap_Struct(klass, parser_mark, parser_free, p);
    p->value = self;
    return self;
}

#define ripper_initialized_p(r) ((r)->parser_lex_input != 0)

/*
 *  call-seq:
 *    Ripper.new(src, filename="(ripper)", lineno=1) -> ripper
 *
 *  Create a new Ripper object.
 *  _src_ must be a String, a IO, or an Object which has #gets method.
 *
 *  This method does not starts parsing.
 *  See also Ripper#parse and Ripper.parse.
 */
static VALUE
ripper_initialize(int argc, VALUE *argv, VALUE self)
{
    struct parser_params *parser;
    VALUE src, fname, lineno;

    Data_Get_Struct(self, struct parser_params, parser);
    rb_scan_args(argc, argv, "12", &src, &fname, &lineno);
    if (rb_obj_respond_to(src, ripper_id_gets, 0)) {
        parser->parser_lex_gets = ripper_lex_get_generic;
    }
    else {
        StringValue(src);
        parser->parser_lex_gets = lex_get_str;
    }
    parser->parser_lex_input = src;
    parser->eofp = Qfalse;
    if (NIL_P(fname)) {
        fname = STR_NEW2("(ripper)");
    }
    else {
        StringValue(fname);
    }
    parser_initialize(parser);

    parser->parser_ruby_sourcefile_string = fname;
    parser->parser_ruby_sourcefile = RSTRING_PTR(fname);
    parser->parser_ruby_sourceline = NIL_P(lineno) ? 0 : NUM2INT(lineno) - 1;

    return Qnil;
}

extern VALUE rb_thread_pass(void);

struct ripper_args {
    struct parser_params *parser;
    int argc;
    VALUE *argv;
};

static VALUE
ripper_parse0(VALUE parser_v)
{
    struct parser_params *parser;

    Data_Get_Struct(parser_v, struct parser_params, parser);
    parser_prepare(parser);
    ripper_yyparse((void*)parser);
    return parser->result;
}

static VALUE
ripper_ensure(VALUE parser_v)
{
    struct parser_params *parser;

    Data_Get_Struct(parser_v, struct parser_params, parser);
    parser->parsing_thread = Qnil;
    return Qnil;
}

/*
 *  call-seq:
 *    ripper#parse
 *
 *  Start parsing and returns the value of the root action.
 */
static VALUE
ripper_parse(VALUE self)
{
    struct parser_params *parser;

    Data_Get_Struct(self, struct parser_params, parser);
    if (!ripper_initialized_p(parser)) {
        rb_raise(rb_eArgError, "method called for uninitialized object");
    }
    if (!NIL_P(parser->parsing_thread)) {
        if (parser->parsing_thread == rb_thread_current())
            rb_raise(rb_eArgError, "Ripper#parse is not reentrant");
        else
            rb_raise(rb_eArgError, "Ripper#parse is not multithread-safe");
    }
    parser->parsing_thread = rb_thread_current();
    rb_ensure(ripper_parse0, self, ripper_ensure, self);

    return parser->result;
}

/*
 *  call-seq:
 *    ripper#column   -> Integer
 *
 *  Return column number of current parsing line.
 *  This number starts from 0.
 */
static VALUE
ripper_column(VALUE self)
{
    struct parser_params *parser;
    long col;

    Data_Get_Struct(self, struct parser_params, parser);
    if (!ripper_initialized_p(parser)) {
        rb_raise(rb_eArgError, "method called for uninitialized object");
    }
    if (NIL_P(parser->parsing_thread)) return Qnil;
    col = parser->tokp - parser->parser_lex_pbeg;
    return LONG2NUM(col);
}

/*
 *  call-seq:
 *    ripper#lineno   -> Integer
 *
 *  Return line number of current parsing line.
 *  This number starts from 1.
 */
static VALUE
ripper_lineno(VALUE self)
{
    struct parser_params *parser;

    Data_Get_Struct(self, struct parser_params, parser);
    if (!ripper_initialized_p(parser)) {
        rb_raise(rb_eArgError, "method called for uninitialized object");
    }
    if (NIL_P(parser->parsing_thread)) return Qnil;
    return INT2NUM(parser->parser_ruby_sourceline);
}

#ifdef RIPPER_DEBUG
/* :nodoc: */
static VALUE
ripper_assert_Qundef(VALUE self, VALUE obj, VALUE msg)
{
    StringValue(msg);
    if (obj == Qundef) {
        rb_raise(rb_eArgError, "%s", RSTRING_PTR(msg));
    }
    return Qnil;
}

/* :nodoc: */
static VALUE
ripper_value(VALUE self, VALUE obj)
{
    return ULONG2NUM(obj);
}
#endif

void
Init_ripper(void)
{
    VALUE Ripper;

    Ripper = rb_define_class("Ripper", rb_cObject);
    rb_define_const(Ripper, "Version", rb_usascii_str_new2(RIPPER_VERSION));
    rb_define_alloc_func(Ripper, ripper_s_allocate);
    rb_define_method(Ripper, "initialize", ripper_initialize, -1);
    rb_define_method(Ripper, "parse", ripper_parse, 0);
    rb_define_method(Ripper, "column", ripper_column, 0);
    rb_define_method(Ripper, "lineno", ripper_lineno, 0);
    rb_define_method(Ripper, "end_seen?", rb_parser_end_seen_p, 0);
    rb_define_method(Ripper, "encoding", rb_parser_encoding, 0);
    rb_define_method(Ripper, "yydebug", rb_parser_get_yydebug, 0);
    rb_define_method(Ripper, "yydebug=", rb_parser_set_yydebug, 1);
#ifdef RIPPER_DEBUG
    rb_define_method(rb_mKernel, "assert_Qundef", ripper_assert_Qundef, 2);
    rb_define_method(rb_mKernel, "rawVALUE", ripper_value, 1);
    rb_define_method(rb_mKernel, "validate_object", ripper_validate_object, 1);
#endif

    ripper_id_gets = rb_intern("gets");
    ripper_init_eventids1(Ripper);
    ripper_init_eventids2(Ripper);
    /* ensure existing in symbol table */
    rb_intern("||");
    rb_intern("&&");
}
#endif /* RIPPER */

