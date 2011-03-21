using System;
using System.Diagnostics;
using System.IO;
using System.Text;

using u8 = System.Byte;


using YYCODETYPE = System.Int32;
using YYACTIONTYPE = System.Int32;

namespace Community.CsharpSqlite
{
  using sqlite3ParserTOKENTYPE = Sqlite3.Token;

  public partial class Sqlite3
  {
    /*
    *************************************************************************
    **  Included in SQLite3 port to C#-SQLite;  2008 Noah B Hart
    **  C#-SQLite is an independent reimplementation of the SQLite software library
    **
    **  SQLITE_SOURCE_ID: 2010-03-09 19:31:43 4ae453ea7be69018d8c16eb8dabe05617397dc4d
    **
    **  $Header$
    *************************************************************************
    */

    /* Driver template for the LEMON parser generator.
    ** The author disclaims copyright to this source code.
    **
    ** This version of "lempar.c" is modified, slightly, for use by SQLite.
    ** The only modifications are the addition of a couple of NEVER()
    ** macros to disable tests that are needed in the case of a general
    ** LALR(1) grammar but which are always false in the
    ** specific grammar used by SQLite.
    */
    /* First off, code is included that follows the "include" declaration
    ** in the input grammar file. */
    //#include <stdio.h>
    //#line 51 "parse.y"

    //#include "sqliteInt.h"
    /*
    ** Disable all error recovery processing in the parser push-down
    ** automaton.
    */
    //#define YYNOERRORRECOVERY 1
    const int YYNOERRORRECOVERY = 1;

    /*
    ** Make yytestcase() the same as testcase()
    */
    //#define yytestcase(X) testcase(X)
    static void yytestcase<T>( T X ) { testcase( X ); }

    /*
    ** An instance of this structure holds information about the
    ** LIMIT clause of a SELECT statement.
    */
    public struct LimitVal
    {
      public Expr pLimit;    /* The LIMIT expression.  NULL if there is no limit */
      public Expr pOffset;   /* The OFFSET expression.  NULL if there is none */
    };

    /*
    ** An instance of this structure is used to store the LIKE,
    ** GLOB, NOT LIKE, and NOT GLOB operators.
    */
    public struct LikeOp
    {
      public Token eOperator;  /* "like" or "glob" or "regexp" */
      public bool not;         /* True if the NOT keyword is present */
    };

    /*
    ** An instance of the following structure describes the event of a
    ** TRIGGER.  "a" is the event type, one of TK_UPDATE, TK_INSERT,
    ** TK_DELETE, or TK_INSTEAD.  If the event is of the form
    **
    **      UPDATE ON (a,b,c)
    **
    ** Then the "b" IdList records the list "a,b,c".
    */
    public struct TrigEvent { public int a; public IdList b; };
    /*
** An instance of this structure holds the ATTACH key and the key type.
*/
    public struct AttachKey { public int type; public Token key; };

    //#line 722 "parse.y"

    /* This is a utility routine used to set the ExprSpan.zStart and
    ** ExprSpan.zEnd values of pOut so that the span covers the complete
    ** range of text beginning with pStart and going to the end of pEnd.
    */
    static void spanSet( ExprSpan pOut, Token pStart, Token pEnd )
    {
      pOut.zStart = pStart.z;
      pOut.zEnd = pEnd.z.Substring( pEnd.n );
    }

    /* Construct a new Expr object from a single identifier.  Use the
    ** new Expr to populate pOut.  Set the span of pOut to be the identifier
    ** that created the expression.
    */
    static void spanExpr( ExprSpan pOut, Parse pParse, int op, Token pValue )
    {
      pOut.pExpr = sqlite3PExpr( pParse, op, 0, 0, pValue );
      pOut.zStart = pValue.z;
      pOut.zEnd = pValue.z.Substring( pValue.n );
    }
    //#line 817 "parse.y"

    /* This routine constructs a binary expression node out of two ExprSpan
    ** objects and uses the result to populate a new ExprSpan object.
    */
    static void spanBinaryExpr(
    ExprSpan pOut,     /* Write the result here */
    Parse pParse,      /* The parsing context.  Errors accumulate here */
    int op,            /* The binary operation */
    ExprSpan pLeft,    /* The left operand */
    ExprSpan pRight    /* The right operand */
    )
    {
      pOut.pExpr = sqlite3PExpr( pParse, op, pLeft.pExpr, pRight.pExpr, 0 );
      pOut.zStart = pLeft.zStart;
      pOut.zEnd = pRight.zEnd;
    }
    //#line 869 "parse.y"

    /* Construct an expression node for a unary postfix operator
    */
    static void spanUnaryPostfix(
    ExprSpan pOut,        /* Write the new expression node here */
    Parse pParse,         /* Parsing context to record errors */
    int op,               /* The operator */
    ExprSpan pOperand,    /* The operand */
    Token pPostOp         /* The operand token for setting the span */
    )
    {
      pOut.pExpr = sqlite3PExpr( pParse, op, pOperand.pExpr, 0, 0 );
      pOut.zStart = pOperand.zStart;
      pOut.zEnd = pPostOp.z.Substring( pPostOp.n );
    }
    //#line 888 "parse.y"

    /* A routine to convert a binary TK_IS or TK_ISNOT expression into a
  ** unary TK_ISNULL or TK_NOTNULL expression. */
    static void binaryToUnaryIfNull( Parse pParse, Expr pY, Expr pA, int op )
    {
      sqlite3 db = pParse.db;
      if ( /*db.mallocFailed == 0 && */pY.op == TK_NULL )
      {
        pA.op = (u8)op;
        sqlite3ExprDelete( db, ref pA.pRight );
        pA.pRight = null;
      }
    }
    //#line 916 "parse.y"

    /* Construct an expression node for a unary prefix operator
    */
    static void spanUnaryPrefix(
    ExprSpan pOut,        /* Write the new expression node here */
    Parse pParse,         /* Parsing context to record errors */
    int op,               /* The operator */
    ExprSpan pOperand,    /* The operand */
    Token pPreOp          /* The operand token for setting the span */
    )
    {
      pOut.pExpr = sqlite3PExpr( pParse, op, pOperand.pExpr, 0, 0 );
      pOut.zStart = pPreOp.z;
      pOut.zEnd = pOperand.zEnd;
    }
    //#line 141 "parse.c"
    /* Next is all token values, in a form suitable for use by makeheaders.
    ** This section will be null unless lemon is run with the -m switch.
    */
    /*
    ** These constants (all generated automatically by the parser generator)
    ** specify the various kinds of tokens (terminals) that the parser
    ** understands.
    **
    ** Each symbol here is a terminal symbol in the grammar.
    */
    /* Make sure the INTERFACE macro is defined.
    */
#if !INTERFACE
    //# define INTERFACE 1
#endif
    /* The next thing included is series of defines which control
** various aspects of the generated parser.
**    YYCODETYPE         is the data type used for storing terminal
**                       and nonterminal numbers.  "unsigned char" is
**                       used if there are fewer than 250 terminals
**                       and nonterminals.  "int" is used otherwise.
**    YYNOCODE           is a number of type YYCODETYPE which corresponds
**                       to no legal terminal or nonterminal number.  This
**                       number is used to fill in empty slots of the hash
**                       table.
**    YYFALLBACK         If defined, this indicates that one or more tokens
**                       have fall-back values which should be used if the
**                       original value of the token will not parse.
**    YYACTIONTYPE       is the data type used for storing terminal
**                       and nonterminal numbers.  "unsigned char" is
**                       used if there are fewer than 250 rules and
**                       states combined.  "int" is used otherwise.
**    sqlite3ParserTOKENTYPE     is the data type used for minor tokens given
**                       directly to the parser from the tokenizer.
**    YYMINORTYPE        is the data type used for all minor tokens.
**                       This is typically a union of many types, one of
**                       which is sqlite3ParserTOKENTYPE.  The entry in the union
**                       for base tokens is called "yy0".
**    YYSTACKDEPTH       is the maximum depth of the parser's stack.  If
**                       zero the stack is dynamically sized using realloc()
**    sqlite3ParserARG_SDECL     A static variable declaration for the %extra_argument
**    sqlite3ParserARG_PDECL     A parameter declaration for the %extra_argument
**    sqlite3ParserARG_STORE     Code to store %extra_argument into yypParser
**    sqlite3ParserARG_FETCH     Code to extract %extra_argument from yypParser
**    YYNSTATE           the combined number of states.
**    YYNRULE            the number of rules in the grammar
**    YYERRORSYMBOL      is the code number of the error symbol.  If not
**                       defined, then do no error processing.
*/
    //#define YYCODETYPE unsigned short char
    const int YYNOCODE = 254;
    //#define YYACTIONTYPE unsigned short int
    const int YYWILDCARD = 67;
    //#define sqlite3ParserTOKENTYPE Token
    public class YYMINORTYPE
    {
      public int yyinit;
      public sqlite3ParserTOKENTYPE yy0 = new sqlite3ParserTOKENTYPE();
      public Select yy3;
      public ExprList yy14;
      public SrcList yy65;
      public LikeOp yy96;
      public Expr yy132;
      public u8 yy186;
      public int yy328;
      public ExprSpan yy346 = new ExprSpan();
      public TrigEvent yy378;
      public IdList yy408;
      public struct _yy429 { public int value; public int mask;}public _yy429 yy429;
      public TriggerStep yy473;
      public LimitVal yy476;
    }

#if !YYSTACKDEPTH
    const int YYSTACKDEPTH = 100;
#endif
    //#define sqlite3ParserARG_SDECL Parse pParse;
    //#define sqlite3ParserARG_PDECL ,Parse pParse
    //#define sqlite3ParserARG_FETCH Parse pParse = yypParser.pParse
    //#define sqlite3ParserARG_STORE yypParser.pParse = pParse
    const int YYNSTATE = 631;
    const int YYNRULE = 330;
    //#define YYFALLBACK 1
    const int YYFALLBACK = 1;
    const int YY_NO_ACTION = ( YYNSTATE + YYNRULE + 2 );
    const int YY_ACCEPT_ACTION = ( YYNSTATE + YYNRULE + 1 );
    const int YY_ERROR_ACTION = ( YYNSTATE + YYNRULE );

    /* The yyzerominor constant is used to initialize instances of
    ** YYMINORTYPE objects to zero. */
    YYMINORTYPE yyzerominor = new YYMINORTYPE();//static const YYMINORTYPE yyzerominor = { 0 };

    /* Define the yytestcase() macro to be a no-op if is not already defined
    ** otherwise.
    **
    ** Applications can choose to define yytestcase() in the %include section
    ** to a macro that can assist in verifying code coverage.  For production
    ** code the yytestcase() macro should be turned off.  But it is useful
    ** for testing.
    */
    //#if !yytestcase
    //# define yytestcase(X)
    //#endif

    /* Next are the tables used to determine what action to take based on the
    ** current state and lookahead token.  These tables are used to implement
    ** functions that take a state number and lookahead value and return an
    ** action integer.
    **
    ** Suppose the action integer is N.  Then the action is determined as
    ** follows
    **
    **   0 <= N < YYNSTATE                  Shift N.  That is, push the lookahead
    **                                      token onto the stack and goto state N.
    **
    **   YYNSTATE <= N < YYNSTATE+YYNRULE   Reduce by rule N-YYNSTATE.
    **
    **   N == YYNSTATE+YYNRULE              A syntax error has occurred.
    **
    **   N == YYNSTATE+YYNRULE+1            The parser accepts its input.
    **
    **   N == YYNSTATE+YYNRULE+2            No such action.  Denotes unused
    **                                      slots in the yy_action[] table.
    **
    ** The action table is constructed as a single large table named yy_action[].
    ** Given state S and lookahead X, the action is computed as
    **
    **      yy_action[ yy_shift_ofst[S] + X ]
    **
    ** If the index value yy_shift_ofst[S]+X is out of range or if the value
    ** yy_lookahead[yy_shift_ofst[S]+X] is not equal to X or if yy_shift_ofst[S]
    ** is equal to YY_SHIFT_USE_DFLT, it means that the action is not in the table
    ** and that yy_default[S] should be used instead.
    **
    ** The formula above is for computing the action when the lookahead is
    ** a terminal symbol.  If the lookahead is a non-terminal (as occurs after
    ** a reduce action) then the yy_reduce_ofst[] array is used in place of
    ** the yy_shift_ofst[] array and YY_REDUCE_USE_DFLT is used in place of
    ** YY_SHIFT_USE_DFLT.
    **
    ** The following are the tables generated in this section:
    **
    **  yy_action[]        A single table containing all actions.
    **  yy_lookahead[]     A table containing the lookahead for each entry in
    **                     yy_action.  Used to detect hash collisions.
    **  yy_shift_ofst[]    For each state, the offset into yy_action for
    **                     shifting terminals.
    **  yy_reduce_ofst[]   For each state, the offset into yy_action for
    **                     shifting non-terminals after a reduce.
    **  yy_default[]       Default action for each state.
    */
    //#define YY_ACTTAB_COUNT (1550)
    const int YY_ACTTAB_COUNT = 1550;
    static YYACTIONTYPE[] yy_action = new YYACTIONTYPE[]{
 /*     0 */   313,   49,  556,   46,  147,  172,  628,  598,   55,   55,
 /*    10 */    55,   55,  302,   53,   53,   53,   53,   52,   52,   51,
 /*    20 */    51,   51,   50,  238,  603,   66,  624,  623,  604,  598,
 /*    30 */   591,  585,   48,   53,   53,   53,   53,   52,   52,   51,
 /*    40 */    51,   51,   50,  238,   51,   51,   51,   50,  238,   56,
 /*    50 */    57,   47,  583,  582,  584,  584,   54,   54,   55,   55,
 /*    60 */    55,   55,  609,   53,   53,   53,   53,   52,   52,   51,
 /*    70 */    51,   51,   50,  238,  313,  598,  672,  330,  411,  217,
 /*    80 */    32,   53,   53,   53,   53,   52,   52,   51,   51,   51,
 /*    90 */    50,  238,  330,  414,  621,  620,  166,  598,  673,  382,
 /*   100 */   379,  378,  602,   73,  591,  585,  307,  424,  166,   58,
 /*   110 */   377,  382,  379,  378,  516,  515,  624,  623,  254,  200,
 /*   120 */   199,  198,  377,   56,   57,   47,  583,  582,  584,  584,
 /*   130 */    54,   54,   55,   55,   55,   55,  581,   53,   53,   53,
 /*   140 */    53,   52,   52,   51,   51,   51,   50,  238,  313,  270,
 /*   150 */   226,  422,  283,  133,  177,  139,  284,  385,  279,  384,
 /*   160 */   169,  197,  251,  282,  253,  226,  411,  275,  440,  167,
 /*   170 */   139,  284,  385,  279,  384,  169,  571,  236,  591,  585,
 /*   180 */   240,  414,  275,  622,  621,  620,  674,  437,  441,  442,
 /*   190 */   602,   88,  352,  266,  439,  268,  438,   56,   57,   47,
 /*   200 */   583,  582,  584,  584,   54,   54,   55,   55,   55,   55,
 /*   210 */   465,   53,   53,   53,   53,   52,   52,   51,   51,   51,
 /*   220 */    50,  238,  313,  471,   52,   52,   51,   51,   51,   50,
 /*   230 */   238,  234,  166,  491,  567,  382,  379,  378,    1,  440,
 /*   240 */   252,  176,  624,  623,  608,   67,  377,  513,  622,  443,
 /*   250 */   237,  577,  591,  585,  622,  172,  466,  598,  554,  441,
 /*   260 */   340,  409,  526,  580,  580,  349,  596,  553,  194,  482,
 /*   270 */   175,   56,   57,   47,  583,  582,  584,  584,   54,   54,
 /*   280 */    55,   55,   55,   55,  562,   53,   53,   53,   53,   52,
 /*   290 */    52,   51,   51,   51,   50,  238,  313,  594,  594,  594,
 /*   300 */   561,  578,  469,   65,  259,  351,  258,  411,  624,  623,
 /*   310 */   621,  620,  332,  576,  575,  240,  560,  568,  520,  411,
 /*   320 */   341,  237,  414,  624,  623,  598,  591,  585,  542,  519,
 /*   330 */   171,  602,   95,   68,  414,  624,  623,  624,  623,   38,
 /*   340 */   877,  506,  507,  602,   88,   56,   57,   47,  583,  582,
 /*   350 */   584,  584,   54,   54,   55,   55,   55,   55,  532,   53,
 /*   360 */    53,   53,   53,   52,   52,   51,   51,   51,   50,  238,
 /*   370 */   313,  411,  579,  398,  531,  237,  621,  620,  388,  625,
 /*   380 */   500,  206,  167,  396,  233,  312,  414,  387,  569,  492,
 /*   390 */   216,  621,  620,  566,  622,  602,   74,  533,  210,  491,
 /*   400 */   591,  585,  548,  621,  620,  621,  620,  300,  598,  466,
 /*   410 */   481,   67,  603,   35,  622,  601,  604,  547,    6,   56,
 /*   420 */    57,   47,  583,  582,  584,  584,   54,   54,   55,   55,
 /*   430 */    55,   55,  601,   53,   53,   53,   53,   52,   52,   51,
 /*   440 */    51,   51,   50,  238,  313,  411,  184,  409,  528,  580,
 /*   450 */   580,  551,  962,  186,  419,    2,  353,  259,  351,  258,
 /*   460 */   414,  409,  411,  580,  580,   44,  411,  544,  240,  602,
 /*   470 */    94,  190,    7,   62,  591,  585,  598,  414,  350,  607,
 /*   480 */   493,  414,  409,  317,  580,  580,  602,   95,  496,  565,
 /*   490 */   602,   80,  203,   56,   57,   47,  583,  582,  584,  584,
 /*   500 */    54,   54,   55,   55,   55,   55,  535,   53,   53,   53,
 /*   510 */    53,   52,   52,   51,   51,   51,   50,  238,  313,  202,
 /*   520 */   564,  293,  511,   49,  562,   46,  147,  411,  394,  183,
 /*   530 */   563,  549,  505,  549,  174,  409,  322,  580,  580,   39,
 /*   540 */   561,   37,  414,  624,  623,  192,  473,  383,  591,  585,
 /*   550 */   474,  602,   80,  601,  504,  544,  560,  364,  402,  210,
 /*   560 */   421,  952,  361,  952,  365,  201,  144,   56,   57,   47,
 /*   570 */   583,  582,  584,  584,   54,   54,   55,   55,   55,   55,
 /*   580 */   559,   53,   53,   53,   53,   52,   52,   51,   51,   51,
 /*   590 */    50,  238,  313,  601,  232,  264,  272,  321,  374,  484,
 /*   600 */   510,  146,  342,  146,  328,  425,  485,  407,  576,  575,
 /*   610 */   622,  621,  620,   49,  168,   46,  147,  353,  546,  491,
 /*   620 */   204,  240,  591,  585,  421,  951,  549,  951,  549,  168,
 /*   630 */   429,   67,  390,  343,  622,  434,  307,  423,  338,  360,
 /*   640 */   391,   56,   57,   47,  583,  582,  584,  584,   54,   54,
 /*   650 */    55,   55,   55,   55,  601,   53,   53,   53,   53,   52,
 /*   660 */    52,   51,   51,   51,   50,  238,  313,   34,  318,  425,
 /*   670 */   237,   21,  359,  273,  411,  167,  411,  276,  411,  540,
 /*   680 */   411,  422,   13,  318,  619,  618,  617,  622,  275,  414,
 /*   690 */   336,  414,  622,  414,  622,  414,  591,  585,  602,   69,
 /*   700 */   602,   97,  602,  100,  602,   98,  631,  629,  334,  475,
 /*   710 */   475,  367,  319,  148,  327,   56,   57,   47,  583,  582,
 /*   720 */   584,  584,   54,   54,   55,   55,   55,   55,  411,   53,
 /*   730 */    53,   53,   53,   52,   52,   51,   51,   51,   50,  238,
 /*   740 */   313,  411,  331,  414,  411,   49,  276,   46,  147,  569,
 /*   750 */   406,  216,  602,  106,  573,  573,  414,  354,  524,  414,
 /*   760 */   411,  622,  411,  224,    4,  602,  104,  605,  602,  108,
 /*   770 */   591,  585,  622,   20,  375,  414,  167,  414,  215,  144,
 /*   780 */   470,  239,  167,  225,  602,  109,  602,  134,   18,   56,
 /*   790 */    57,   47,  583,  582,  584,  584,   54,   54,   55,   55,
 /*   800 */    55,   55,  411,   53,   53,   53,   53,   52,   52,   51,
 /*   810 */    51,   51,   50,  238,  313,  411,  276,  414,   12,  459,
 /*   820 */   276,  171,  411,   16,  223,  189,  602,  135,  354,  170,
 /*   830 */   414,  622,  630,    2,  411,  622,  540,  414,  143,  602,
 /*   840 */    61,  359,  132,  622,  591,  585,  602,  105,  458,  414,
 /*   850 */    23,  622,  446,  326,   23,  538,  622,  325,  602,  103,
 /*   860 */   427,  530,  309,   56,   57,   47,  583,  582,  584,  584,
 /*   870 */    54,   54,   55,   55,   55,   55,  411,   53,   53,   53,
 /*   880 */    53,   52,   52,   51,   51,   51,   50,  238,  313,  411,
 /*   890 */   264,  414,  411,  276,  359,  219,  157,  214,  357,  366,
 /*   900 */   602,   96,  522,  521,  414,  622,  358,  414,  622,  622,
 /*   910 */   411,  613,  612,  602,  102,  142,  602,   77,  591,  585,
 /*   920 */   529,  540,  231,  426,  308,  414,  622,  622,  468,  521,
 /*   930 */   324,  601,  257,  263,  602,   99,  622,   56,   45,   47,
 /*   940 */   583,  582,  584,  584,   54,   54,   55,   55,   55,   55,
 /*   950 */   411,   53,   53,   53,   53,   52,   52,   51,   51,   51,
 /*   960 */    50,  238,  313,  264,  264,  414,  411,  213,  209,  544,
 /*   970 */   544,  207,  611,   28,  602,  138,   50,  238,  622,  622,
 /*   980 */   381,  414,  503,  140,  323,  222,  274,  622,  590,  589,
 /*   990 */   602,  137,  591,  585,  629,  334,  606,   30,  622,  571,
 /*  1000 */   236,  601,  601,  130,  496,  601,  453,  451,  288,  286,
 /*  1010 */   587,  586,   57,   47,  583,  582,  584,  584,   54,   54,
 /*  1020 */    55,   55,   55,   55,  411,   53,   53,   53,   53,   52,
 /*  1030 */    52,   51,   51,   51,   50,  238,  313,  588,  411,  414,
 /*  1040 */   411,  264,  410,  129,  595,  400,   27,  376,  602,  136,
 /*  1050 */   128,  165,  479,  414,  282,  414,  622,  622,  411,  622,
 /*  1060 */   622,  411,  602,   76,  602,   93,  591,  585,  188,  372,
 /*  1070 */   368,  125,  476,  414,  261,  160,  414,  171,  124,  472,
 /*  1080 */   123,   15,  602,   92,  450,  602,   75,   47,  583,  582,
 /*  1090 */   584,  584,   54,   54,   55,   55,   55,   55,  464,   53,
 /*  1100 */    53,   53,   53,   52,   52,   51,   51,   51,   50,  238,
 /*  1110 */    43,  405,  264,    3,  558,  264,  545,  415,  623,  159,
 /*  1120 */   541,  158,  539,  278,   25,  461,  121,  622,  408,  622,
 /*  1130 */   622,  622,   24,   43,  405,  622,    3,  622,  622,  120,
 /*  1140 */   415,  623,   11,  456,  411,  156,  452,  403,  509,  277,
 /*  1150 */   118,  408,  489,  113,  205,  449,  271,  567,  221,  414,
 /*  1160 */   269,  267,  155,  622,  622,  111,  411,  622,  602,   95,
 /*  1170 */   403,  622,  411,  110,   10,  622,  622,   40,   41,  534,
 /*  1180 */   567,  414,   64,  264,   42,  413,  412,  414,  601,  596,
 /*  1190 */   602,   91,  445,  436,  150,  435,  602,   90,  622,  265,
 /*  1200 */    40,   41,  337,  242,  411,  191,  333,   42,  413,  412,
 /*  1210 */   398,  420,  596,  316,  622,  399,  260,  107,  230,  414,
 /*  1220 */   594,  594,  594,  593,  592,   14,  220,  411,  602,  101,
 /*  1230 */   240,  622,   43,  405,  362,    3,  149,  315,  626,  415,
 /*  1240 */   623,  127,  414,  594,  594,  594,  593,  592,   14,  622,
 /*  1250 */   408,  602,   89,  411,  181,   33,  405,  463,    3,  411,
 /*  1260 */   264,  462,  415,  623,  616,  615,  614,  355,  414,  403,
 /*  1270 */   417,  416,  622,  408,  414,  622,  622,  602,   87,  567,
 /*  1280 */   418,  627,  622,  602,   86,    8,  241,  180,  126,  255,
 /*  1290 */   600,  178,  403,  240,  208,  455,  395,  294,  444,   40,
 /*  1300 */    41,  297,  567,  248,  622,  296,   42,  413,  412,  247,
 /*  1310 */   622,  596,  244,  622,   30,   60,   31,  243,  430,  624,
 /*  1320 */   623,  292,   40,   41,  622,  295,  145,  622,  601,   42,
 /*  1330 */   413,  412,  622,  622,  596,  393,  622,  397,  599,   59,
 /*  1340 */   235,  622,  594,  594,  594,  593,  592,   14,  218,  291,
 /*  1350 */   622,   36,  344,  305,  304,  303,  179,  301,  411,  567,
 /*  1360 */   454,  557,  173,  185,  622,  594,  594,  594,  593,  592,
 /*  1370 */    14,  411,   29,  414,  151,  289,  246,  523,  411,  196,
 /*  1380 */   195,  335,  602,   85,  411,  245,  414,  526,  392,  543,
 /*  1390 */   411,  596,  287,  414,  285,  602,   72,  537,  153,  414,
 /*  1400 */   466,  411,  602,   71,  154,  414,  411,  152,  602,   84,
 /*  1410 */   386,  536,  329,  411,  602,   83,  414,  518,  280,  411,
 /*  1420 */   513,  414,  594,  594,  594,  602,   82,  517,  414,  311,
 /*  1430 */   602,   81,  411,  514,  414,  512,  131,  602,   70,  229,
 /*  1440 */   228,  227,  494,  602,   17,  411,  488,  414,  259,  346,
 /*  1450 */   249,  389,  487,  486,  314,  164,  602,   79,  310,  240,
 /*  1460 */   414,  373,  480,  163,  262,  371,  414,  162,  369,  602,
 /*  1470 */    78,  212,  478,   26,  477,  602,    9,  161,  467,  363,
 /*  1480 */   141,  122,  339,  187,  119,  457,  348,  347,  117,  116,
 /*  1490 */   115,  112,  114,  448,  182,   22,  320,  433,  432,  431,
 /*  1500 */    19,  428,  610,  597,  574,  193,  572,   63,  298,  404,
 /*  1510 */   555,  552,  290,  281,  510,  460,  498,  499,  495,  447,
 /*  1520 */   356,  497,  256,  380,  306,  570,    5,  250,  345,  238,
 /*  1530 */   299,  550,  527,  490,  508,  525,  502,  401,  501,  963,
 /*  1540 */   211,  963,  483,  963,  963,  963,  963,  963,  963,  370,
};
    static YYCODETYPE[] yy_lookahead = new YYCODETYPE[]{
 /*     0 */    19,  222,  223,  224,  225,   24,    1,   26,   77,   78,
 /*    10 */    79,   80,   15,   82,   83,   84,   85,   86,   87,   88,
 /*    20 */    89,   90,   91,   92,  113,   22,   26,   27,  117,   26,
 /*    30 */    49,   50,   81,   82,   83,   84,   85,   86,   87,   88,
 /*    40 */    89,   90,   91,   92,   88,   89,   90,   91,   92,   68,
 /*    50 */    69,   70,   71,   72,   73,   74,   75,   76,   77,   78,
 /*    60 */    79,   80,   23,   82,   83,   84,   85,   86,   87,   88,
 /*    70 */    89,   90,   91,   92,   19,   94,  118,   19,  150,   22,
 /*    80 */    25,   82,   83,   84,   85,   86,   87,   88,   89,   90,
 /*    90 */    91,   92,   19,  165,   94,   95,   96,   94,  118,   99,
 /*   100 */   100,  101,  174,  175,   49,   50,   22,   23,   96,   54,
 /*   110 */   110,   99,  100,  101,    7,    8,   26,   27,   16,  105,
 /*   120 */   106,  107,  110,   68,   69,   70,   71,   72,   73,   74,
 /*   130 */    75,   76,   77,   78,   79,   80,  113,   82,   83,   84,
 /*   140 */    85,   86,   87,   88,   89,   90,   91,   92,   19,   16,
 /*   150 */    92,   67,   98,   24,   96,   97,   98,   99,  100,  101,
 /*   160 */   102,   25,   60,  109,   62,   92,  150,  109,  150,   25,
 /*   170 */    97,   98,   99,  100,  101,  102,   86,   87,   49,   50,
 /*   180 */   116,  165,  109,  165,   94,   95,  118,   97,  170,  171,
 /*   190 */   174,  175,  128,   60,  104,   62,  106,   68,   69,   70,
 /*   200 */    71,   72,   73,   74,   75,   76,   77,   78,   79,   80,
 /*   210 */    11,   82,   83,   84,   85,   86,   87,   88,   89,   90,
 /*   220 */    91,   92,   19,   21,   86,   87,   88,   89,   90,   91,
 /*   230 */    92,  215,   96,  150,   66,   99,  100,  101,   22,  150,
 /*   240 */   138,  118,   26,   27,  161,  162,  110,  103,  165,  231,
 /*   250 */   232,   23,   49,   50,  165,   24,   57,   26,   32,  170,
 /*   260 */   171,  112,   94,  114,  115,   63,   98,   41,  185,  186,
 /*   270 */   118,   68,   69,   70,   71,   72,   73,   74,   75,   76,
 /*   280 */    77,   78,   79,   80,   12,   82,   83,   84,   85,   86,
 /*   290 */    87,   88,   89,   90,   91,   92,   19,  129,  130,  131,
 /*   300 */    28,   23,  100,   25,  105,  106,  107,  150,   26,   27,
 /*   310 */    94,   95,  169,  170,  171,  116,   44,   23,   46,  150,
 /*   320 */   231,  232,  165,   26,   27,   94,   49,   50,   23,   57,
 /*   330 */    25,  174,  175,   22,  165,   26,   27,   26,   27,  136,
 /*   340 */   138,   97,   98,  174,  175,   68,   69,   70,   71,   72,
 /*   350 */    73,   74,   75,   76,   77,   78,   79,   80,   23,   82,
 /*   360 */    83,   84,   85,   86,   87,   88,   89,   90,   91,   92,
 /*   370 */    19,  150,   23,  216,   23,  232,   94,   95,  221,  150,
 /*   380 */    23,  160,   25,  214,  215,  163,  165,   88,  166,  167,
 /*   390 */   168,   94,   95,   23,  165,  174,  175,   88,  160,  150,
 /*   400 */    49,   50,  120,   94,   95,   94,   95,  158,   26,   57,
 /*   410 */   161,  162,  113,  136,  165,  194,  117,  120,   22,   68,
 /*   420 */    69,   70,   71,   72,   73,   74,   75,   76,   77,   78,
 /*   430 */    79,   80,  194,   82,   83,   84,   85,   86,   87,   88,
 /*   440 */    89,   90,   91,   92,   19,  150,   23,  112,   23,  114,
 /*   450 */   115,   25,  142,  143,  144,  145,  218,  105,  106,  107,
 /*   460 */   165,  112,  150,  114,  115,   22,  150,  166,  116,  174,
 /*   470 */   175,   22,   76,  235,   49,   50,   94,  165,  240,  172,
 /*   480 */   173,  165,  112,  155,  114,  115,  174,  175,  181,   11,
 /*   490 */   174,  175,   22,   68,   69,   70,   71,   72,   73,   74,
 /*   500 */    75,   76,   77,   78,   79,   80,  205,   82,   83,   84,
 /*   510 */    85,   86,   87,   88,   89,   90,   91,   92,   19,  160,
 /*   520 */    23,  226,   23,  222,   12,  224,  225,  150,  216,   23,
 /*   530 */    23,   25,   36,   25,   25,  112,  220,  114,  115,  135,
 /*   540 */    28,  137,  165,   26,   27,  119,   30,   51,   49,   50,
 /*   550 */    34,  174,  175,  194,   58,  166,   44,  229,   46,  160,
 /*   560 */    22,   23,  234,   25,   48,  206,  207,   68,   69,   70,
 /*   570 */    71,   72,   73,   74,   75,   76,   77,   78,   79,   80,
 /*   580 */    23,   82,   83,   84,   85,   86,   87,   88,   89,   90,
 /*   590 */    91,   92,   19,  194,  205,  150,   23,  220,   19,  181,
 /*   600 */   182,   95,   97,   95,  108,   67,  188,  169,  170,  171,
 /*   610 */   165,   94,   95,  222,   50,  224,  225,  218,  120,  150,
 /*   620 */   160,  116,   49,   50,   22,   23,  120,   25,  120,   50,
 /*   630 */   161,  162,   19,  128,  165,  244,   22,   23,  193,  240,
 /*   640 */    27,   68,   69,   70,   71,   72,   73,   74,   75,   76,
 /*   650 */    77,   78,   79,   80,  194,   82,   83,   84,   85,   86,
 /*   660 */    87,   88,   89,   90,   91,   92,   19,   25,  104,   67,
 /*   670 */   232,   24,  150,   23,  150,   25,  150,  150,  150,  150,
 /*   680 */   150,   67,   25,  104,    7,    8,    9,  165,  109,  165,
 /*   690 */   245,  165,  165,  165,  165,  165,   49,   50,  174,  175,
 /*   700 */   174,  175,  174,  175,  174,  175,    0,    1,    2,  105,
 /*   710 */   106,  107,  248,  249,  187,   68,   69,   70,   71,   72,
 /*   720 */    73,   74,   75,   76,   77,   78,   79,   80,  150,   82,
 /*   730 */    83,   84,   85,   86,   87,   88,   89,   90,   91,   92,
 /*   740 */    19,  150,  213,  165,  150,  222,  150,  224,  225,  166,
 /*   750 */   167,  168,  174,  175,  129,  130,  165,  150,  165,  165,
 /*   760 */   150,  165,  150,  241,   35,  174,  175,  174,  174,  175,
 /*   770 */    49,   50,  165,   52,   23,  165,   25,  165,  206,  207,
 /*   780 */    23,  197,   25,  187,  174,  175,  174,  175,  204,   68,
 /*   790 */    69,   70,   71,   72,   73,   74,   75,   76,   77,   78,
 /*   800 */    79,   80,  150,   82,   83,   84,   85,   86,   87,   88,
 /*   810 */    89,   90,   91,   92,   19,  150,  150,  165,   35,   23,
 /*   820 */   150,   25,  150,   22,  217,   24,  174,  175,  150,   35,
 /*   830 */   165,  165,  144,  145,  150,  165,  150,  165,  118,  174,
 /*   840 */   175,  150,   22,  165,   49,   50,  174,  175,   23,  165,
 /*   850 */    25,  165,   23,  187,   25,   27,  165,  187,  174,  175,
 /*   860 */    23,   23,   25,   68,   69,   70,   71,   72,   73,   74,
 /*   870 */    75,   76,   77,   78,   79,   80,  150,   82,   83,   84,
 /*   880 */    85,   86,   87,   88,   89,   90,   91,   92,   19,  150,
 /*   890 */   150,  165,  150,  150,  150,  217,   25,  160,   19,  213,
 /*   900 */   174,  175,  190,  191,  165,  165,   27,  165,  165,  165,
 /*   910 */   150,  150,  150,  174,  175,   39,  174,  175,   49,   50,
 /*   920 */    23,  150,   52,  250,  251,  165,  165,  165,  190,  191,
 /*   930 */   187,  194,  241,  193,  174,  175,  165,   68,   69,   70,
 /*   940 */    71,   72,   73,   74,   75,   76,   77,   78,   79,   80,
 /*   950 */   150,   82,   83,   84,   85,   86,   87,   88,   89,   90,
 /*   960 */    91,   92,   19,  150,  150,  165,  150,  160,  160,  166,
 /*   970 */   166,  160,  150,   22,  174,  175,   91,   92,  165,  165,
 /*   980 */    52,  165,   29,  150,  213,  241,   23,  165,   49,   50,
 /*   990 */   174,  175,   49,   50,    1,    2,  173,  126,  165,   86,
 /*  1000 */    87,  194,  194,   22,  181,  194,  193,  193,  205,  205,
 /*  1010 */    71,   72,   69,   70,   71,   72,   73,   74,   75,   76,
 /*  1020 */    77,   78,   79,   80,  150,   82,   83,   84,   85,   86,
 /*  1030 */    87,   88,   89,   90,   91,   92,   19,   98,  150,  165,
 /*  1040 */   150,  150,  150,   22,  150,  150,   22,   52,  174,  175,
 /*  1050 */    22,  102,   20,  165,  109,  165,  165,  165,  150,  165,
 /*  1060 */   165,  150,  174,  175,  174,  175,   49,   50,   24,   19,
 /*  1070 */    43,  104,   59,  165,  138,  104,  165,   25,   53,   53,
 /*  1080 */    22,    5,  174,  175,  193,  174,  175,   70,   71,   72,
 /*  1090 */    73,   74,   75,   76,   77,   78,   79,   80,    1,   82,
 /*  1100 */    83,   84,   85,   86,   87,   88,   89,   90,   91,   92,
 /*  1110 */    19,   20,  150,   22,  150,  150,  150,   26,   27,  118,
 /*  1120 */   150,   35,  150,  150,   76,   27,  108,  165,   37,  165,
 /*  1130 */   165,  165,   76,   19,   20,  165,   22,  165,  165,  127,
 /*  1140 */    26,   27,   22,    1,  150,   16,   20,   56,  150,  150,
 /*  1150 */   119,   37,  150,  119,  160,  193,  150,   66,  193,  165,
 /*  1160 */   150,  150,  121,  165,  165,  108,  150,  165,  174,  175,
 /*  1170 */    56,  165,  150,  127,   22,  165,  165,   86,   87,   88,
 /*  1180 */    66,  165,   16,  150,   93,   94,   95,  165,  194,   98,
 /*  1190 */   174,  175,  128,   23,   15,   23,  174,  175,  165,  150,
 /*  1200 */    86,   87,   65,  140,  150,   22,    3,   93,   94,   95,
 /*  1210 */   216,    4,   98,  252,  165,  221,  150,  164,  180,  165,
 /*  1220 */   129,  130,  131,  132,  133,  134,  193,  150,  174,  175,
 /*  1230 */   116,  165,   19,   20,  150,   22,  249,  252,  149,   26,
 /*  1240 */    27,  180,  165,  129,  130,  131,  132,  133,  134,  165,
 /*  1250 */    37,  174,  175,  150,    6,   19,   20,  150,   22,  150,
 /*  1260 */   150,  150,   26,   27,  149,  149,   13,  150,  165,   56,
 /*  1270 */   149,  159,  165,   37,  165,  165,  165,  174,  175,   66,
 /*  1280 */   146,  147,  165,  174,  175,   25,  152,  151,  154,  150,
 /*  1290 */   194,  151,   56,  116,  160,  150,  123,  202,  150,   86,
 /*  1300 */    87,  199,   66,  193,  165,  200,   93,   94,   95,  150,
 /*  1310 */   165,   98,  150,  165,  126,   22,  124,  150,  150,   26,
 /*  1320 */    27,  150,   86,   87,  165,  201,  150,  165,  194,   93,
 /*  1330 */    94,   95,  165,  165,   98,  150,  165,  122,  203,  125,
 /*  1340 */   227,  165,  129,  130,  131,  132,  133,  134,    5,  150,
 /*  1350 */   165,  135,  218,   10,   11,   12,   13,   14,  150,   66,
 /*  1360 */    17,  157,  118,  157,  165,  129,  130,  131,  132,  133,
 /*  1370 */   134,  150,  104,  165,   31,  210,   33,  176,  150,   86,
 /*  1380 */    87,  247,  174,  175,  150,   42,  165,   94,  121,  211,
 /*  1390 */   150,   98,  210,  165,  210,  174,  175,  211,   55,  165,
 /*  1400 */    57,  150,  174,  175,   61,  165,  150,   64,  174,  175,
 /*  1410 */   104,  211,   47,  150,  174,  175,  165,  176,  176,  150,
 /*  1420 */   103,  165,  129,  130,  131,  174,  175,  184,  165,  179,
 /*  1430 */   174,  175,  150,  178,  165,  176,   22,  174,  175,  230,
 /*  1440 */    92,  230,  184,  174,  175,  150,  176,  165,  105,  106,
 /*  1450 */   107,  150,  176,  176,  111,  156,  174,  175,  179,  116,
 /*  1460 */   165,   18,  157,  156,  238,  157,  165,  156,   45,  174,
 /*  1470 */   175,  157,  157,  135,  239,  174,  175,  156,  189,  157,
 /*  1480 */    68,  189,  139,  219,   22,  199,  157,   18,  192,  192,
 /*  1490 */   192,  189,  192,  199,  219,  243,  157,   40,  157,  157,
 /*  1500 */   243,   38,  153,  166,  233,  196,  233,  246,  198,  228,
 /*  1510 */   177,  177,  209,  177,  182,  199,  166,  177,  166,  199,
 /*  1520 */   242,  177,  242,  178,  148,  166,  196,  209,  209,   92,
 /*  1530 */   195,  208,  174,  186,  183,  174,  183,  191,  183,  253,
 /*  1540 */   236,  253,  186,  253,  253,  253,  253,  253,  253,  237,
};

    const int YY_SHIFT_USE_DFLT = -90;//#define YY_SHIFT_USE_DFLT (-90)
    const int YY_SHIFT_COUNT = 418;   //#define YY_SHIFT_COUNT (418)
    const int YY_SHIFT_MIN = -89;     //#define YY_SHIFT_MIN   (-89)
    const int YY_SHIFT_MAX = 1469;    //#define YY_SHIFT_MAX   (1469)

    static short[] yy_shift_ofst = new short[]{
 /*     0 */   993, 1114, 1343, 1114, 1213, 1213,   90,   90,    0,  -19,
 /*    10 */  1213, 1213, 1213, 1213, 1213,  352,  517,  721, 1091, 1213,
 /*    20 */  1213, 1213, 1213, 1213, 1213, 1213, 1213, 1213, 1213, 1213,
 /*    30 */  1213, 1213, 1213, 1213, 1213, 1213, 1213, 1213, 1213, 1213,
 /*    40 */  1213, 1213, 1213, 1213, 1213, 1213, 1213, 1236, 1213, 1213,
 /*    50 */  1213, 1213, 1213, 1213, 1213, 1213, 1213, 1213, 1213, 1213,
 /*    60 */  1213,  -49,  199,  517,  517,  913,  913,  382, 1177,   55,
 /*    70 */   647,  573,  499,  425,  351,  277,  203,  129,  795,  795,
 /*    80 */   795,  795,  795,  795,  795,  795,  795,  795,  795,  795,
 /*    90 */   795,  795,  795,  795,  795,  795,  869,  795,  943, 1017,
 /*   100 */  1017,  -69,  -69,  -69,  -69,   -1,   -1,   58,  138,  -44,
 /*   110 */   517,  517,  517,  517,  517,  517,  517,  517,  517,  517,
 /*   120 */   517,  517,  517,  517,  517,  517,  202,  579,  517,  517,
 /*   130 */   517,  517,  517,  382,  885, 1437,  -90,  -90,  -90, 1293,
 /*   140 */    73,  272,  272,  309,  311,  297,  282,  216,  602,  538,
 /*   150 */   517,  517,  517,  517,  517,  517,  517,  517,  517,  517,
 /*   160 */   517,  517,  517,  517,  517,  517,  517,  517,  517,  517,
 /*   170 */   517,  517,  517,  517,  517,  517,  517,  517,  517,  517,
 /*   180 */   517,  517,  505,  231,  231,  231,  706,   64, 1177, 1177,
 /*   190 */  1177,  -90,  -90,  -90,  136,  168,  168,   12,  496,  496,
 /*   200 */   496,  506,  423,  512,  370,  349,  335,  149,  149,  149,
 /*   210 */   149,  604,  516,  149,  149,  508,    3,  299,  677,  871,
 /*   220 */   613,  613,  879,  871,  879,  144,  382,  226,  382,  226,
 /*   230 */   564,  226,  613,  226,  226,  404,  625,  625,  382,  426,
 /*   240 */   -89,  801, 1463, 1244, 1244, 1457, 1457, 1244, 1462, 1412,
 /*   250 */  1188, 1469, 1469, 1469, 1469, 1244, 1188, 1462, 1412, 1412,
 /*   260 */  1244, 1443, 1338, 1423, 1244, 1244, 1443, 1244, 1443, 1244,
 /*   270 */  1443, 1414, 1306, 1306, 1306, 1365, 1348, 1348, 1414, 1306,
 /*   280 */  1317, 1306, 1365, 1306, 1306, 1267, 1268, 1267, 1268, 1267,
 /*   290 */  1268, 1244, 1244, 1216, 1214, 1215, 1192, 1173, 1188, 1177,
 /*   300 */  1260, 1253, 1253, 1248, 1248, 1248, 1248,  -90,  -90,  -90,
 /*   310 */   -90,  -90,  -90,  939,  102,  614,   84,  133,   14,  837,
 /*   320 */   396,  829,  825,  796,  757,  751,  650,  357,  244,  107,
 /*   330 */    54,  305,  278, 1207, 1203, 1183, 1063, 1179, 1137, 1166,
 /*   340 */  1172, 1170, 1064, 1152, 1046, 1057, 1034, 1126, 1041, 1129,
 /*   350 */  1142, 1031, 1120, 1012, 1056, 1048, 1018, 1098, 1086, 1001,
 /*   360 */  1097, 1076, 1058,  971,  936, 1026, 1052, 1025, 1013, 1027,
 /*   370 */   967, 1044, 1032, 1050,  945,  949, 1028,  995, 1024, 1021,
 /*   380 */   963,  981,  928,  953,  951,  870,  876,  897,  838,  720,
 /*   390 */   828,  794,  820,  498,  642,  783,  657,  729,  642,  557,
 /*   400 */   507,  509,  497,  470,  478,  449,  294,  228,  443,   23,
 /*   410 */   152,  123,   68,  -20,  -42,   57,   39,   -3,    5,
};

    const int YY_REDUCE_USE_DFLT = -222;  //#define YY_REDUCE_USE_DFLT (-222)
    const int YY_REDUCE_COUNT = 312;      //#define YY_REDUCE_COUNT (312)
    const int YY_REDUCE_MIN = -221;       //#define YY_REDUCE_MIN   (-221)
    const int YY_REDUCE_MAX = 1376;       //#define YY_REDUCE_MAX   (1376)

    static short[] yy_reduce_ofst = new short[]{
 /*     0 */   310,  994, 1134,  221,  169,  157,   89,   18,   83,  301,
 /*    10 */   377,  316,  312,   16,  295,  238,  249,  391, 1301, 1295,
 /*    20 */  1282, 1269, 1263, 1256, 1251, 1240, 1234, 1228, 1221, 1208,
 /*    30 */  1109, 1103, 1077, 1054, 1022, 1016,  911,  908,  890,  888,
 /*    40 */   874,  816,  800,  760,  742,  739,  726,  684,  672,  665,
 /*    50 */   652,  612,  610,  594,  591,  578,  530,  528,  526,  524,
 /*    60 */   -72, -221,  399,  469,  445,  438,  143,  222,  359,  523,
 /*    70 */   523,  523,  523,  523,  523,  523,  523,  523,  523,  523,
 /*    80 */   523,  523,  523,  523,  523,  523,  523,  523,  523,  523,
 /*    90 */   523,  523,  523,  523,  523,  523,  523,  523,  523,  523,
 /*   100 */   523,  523,  523,  523,  523,  523,  523,  307,  523,  523,
 /*   110 */  1110,  678, 1033,  965,  962,  891,  814,  813,  744,  771,
 /*   120 */   691,  607,  522,  743,  686,  740,  328,  418,  670,  666,
 /*   130 */   596,  527,  529,  583,  523,  523,  523,  523,  523,  593,
 /*   140 */   823,  738,  712,  892, 1199, 1185, 1176, 1171,  673,  673,
 /*   150 */  1168, 1167, 1162, 1159, 1148, 1145, 1139, 1117, 1111, 1107,
 /*   160 */  1084, 1066, 1049, 1011, 1010, 1006, 1002,  999,  998,  973,
 /*   170 */   972,  970,  966,  964,  895,  894,  892,  833,  822,  762,
 /*   180 */   761,  229,  811,  804,  803,  389,  688,  808,  807,  737,
 /*   190 */   460,  464,  572,  584, 1356, 1361, 1358, 1347, 1355, 1353,
 /*   200 */  1351, 1323, 1335, 1346, 1335, 1335, 1335, 1335, 1335, 1335,
 /*   210 */  1335, 1312, 1304, 1335, 1335, 1323, 1359, 1330, 1376, 1320,
 /*   220 */  1319, 1318, 1280, 1316, 1278, 1345, 1352, 1344, 1350, 1340,
 /*   230 */  1332, 1336, 1303, 1334, 1333, 1281, 1273, 1271, 1337, 1310,
 /*   240 */  1309, 1349, 1261, 1342, 1341, 1257, 1252, 1339, 1275, 1302,
 /*   250 */  1294, 1300, 1298, 1297, 1296, 1329, 1286, 1264, 1292, 1289,
 /*   260 */  1322, 1321, 1235, 1226, 1315, 1314, 1311, 1308, 1307, 1305,
 /*   270 */  1299, 1279, 1277, 1276, 1270, 1258, 1211, 1209, 1250, 1259,
 /*   280 */  1255, 1242, 1243, 1241, 1201, 1200, 1184, 1186, 1182, 1178,
 /*   290 */  1165, 1206, 1204, 1113, 1135, 1095, 1124, 1105, 1102, 1096,
 /*   300 */  1112, 1140, 1136, 1121, 1116, 1115, 1089,  985,  961,  987,
 /*   310 */  1061, 1038, 1053,
};
    static YYACTIONTYPE[] yy_default = new YYACTIONTYPE[] {
  /*     0 */   636,  872,  961,  961,  961,  872,  901,  901,  961,  760,
 /*    10 */   961,  961,  961,  961,  870,  961,  961,  935,  961,  961,
 /*    20 */   961,  961,  961,  961,  961,  961,  961,  961,  961,  961,
 /*    30 */   961,  961,  961,  961,  961,  961,  961,  961,  961,  961,
 /*    40 */   961,  961,  961,  961,  961,  961,  961,  961,  961,  961,
 /*    50 */   961,  961,  961,  961,  961,  961,  961,  961,  961,  961,
 /*    60 */   961,  844,  961,  961,  961,  901,  901,  675,  764,  795,
 /*    70 */   961,  961,  961,  961,  961,  961,  961,  961,  934,  936,
 /*    80 */   810,  809,  803,  802,  914,  775,  800,  793,  786,  797,
 /*    90 */   873,  866,  867,  865,  869,  874,  961,  796,  832,  850,
 /*   100 */   831,  849,  856,  848,  834,  843,  833,  667,  835,  836,
 /*   110 */   961,  961,  961,  961,  961,  961,  961,  961,  961,  961,
 /*   120 */   961,  961,  961,  961,  961,  961,  662,  729,  961,  961,
 /*   130 */   961,  961,  961,  961,  837,  838,  853,  852,  851,  961,
 /*   140 */   961,  961,  961,  961,  961,  961,  961,  961,  961,  961,
 /*   150 */   961,  941,  939,  961,  885,  961,  961,  961,  961,  961,
 /*   160 */   961,  961,  961,  961,  961,  961,  961,  961,  961,  961,
 /*   170 */   961,  961,  961,  961,  961,  961,  961,  961,  961,  961,
 /*   180 */   961,  642,  961,  760,  760,  760,  636,  961,  961,  961,
 /*   190 */   961,  953,  764,  754,  720,  961,  961,  961,  961,  961,
 /*   200 */   961,  961,  961,  961,  961,  961,  961,  805,  743,  924,
 /*   210 */   926,  961,  907,  741,  664,  762,  677,  752,  644,  799,
 /*   220 */   777,  777,  919,  799,  919,  701,  961,  789,  961,  789,
 /*   230 */   698,  789,  777,  789,  789,  868,  961,  961,  961,  761,
 /*   240 */   752,  961,  946,  768,  768,  938,  938,  768,  811,  733,
 /*   250 */   799,  740,  740,  740,  740,  768,  799,  811,  733,  733,
 /*   260 */   768,  659,  913,  911,  768,  768,  659,  768,  659,  768,
 /*   270 */   659,  878,  731,  731,  731,  716,  882,  882,  878,  731,
 /*   280 */   701,  731,  716,  731,  731,  781,  776,  781,  776,  781,
 /*   290 */   776,  768,  768,  961,  794,  782,  792,  790,  799,  961,
 /*   300 */   719,  652,  652,  641,  641,  641,  641,  958,  958,  953,
 /*   310 */   703,  703,  685,  961,  961,  961,  961,  961,  961,  961,
 /*   320 */   887,  961,  961,  961,  961,  961,  961,  961,  961,  961,
 /*   330 */   961,  961,  961,  961,  637,  948,  961,  961,  945,  961,
 /*   340 */   961,  961,  961,  961,  961,  961,  961,  961,  961,  961,
 /*   350 */   961,  961,  961,  961,  961,  961,  961,  961,  961,  917,
 /*   360 */   961,  961,  961,  961,  961,  961,  910,  909,  961,  961,
 /*   370 */   961,  961,  961,  961,  961,  961,  961,  961,  961,  961,
 /*   380 */   961,  961,  961,  961,  961,  961,  961,  961,  961,  961,
 /*   390 */   961,  961,  961,  961,  791,  961,  783,  961,  871,  961,
 /*   400 */   961,  961,  961,  961,  961,  961,  961,  961,  961,  746,
 /*   410 */   820,  961,  819,  823,  818,  669,  961,  650,  961,  633,
 /*   420 */   638,  957,  960,  959,  956,  955,  954,  949,  947,  944,
 /*   430 */   943,  942,  940,  937,  933,  891,  889,  896,  895,  894,
 /*   440 */   893,  892,  890,  888,  886,  806,  804,  801,  798,  932,
 /*   450 */   884,  742,  739,  738,  658,  950,  916,  925,  923,  812,
 /*   460 */   922,  921,  920,  918,  915,  902,  808,  807,  734,  876,
 /*   470 */   875,  661,  906,  905,  904,  908,  912,  903,  770,  660,
 /*   480 */   657,  666,  723,  722,  730,  728,  727,  726,  725,  724,
 /*   490 */   721,  668,  676,  687,  715,  700,  699,  881,  883,  880,
 /*   500 */   879,  708,  707,  713,  712,  711,  710,  709,  706,  705,
 /*   510 */   704,  697,  696,  702,  695,  718,  717,  714,  694,  737,
 /*   520 */   736,  735,  732,  693,  692,  691,  823,  690,  689,  829,
 /*   530 */   828,  816,  860,  757,  756,  755,  767,  766,  779,  778,
 /*   540 */   814,  813,  780,  765,  759,  758,  774,  773,  772,  771,
 /*   550 */   763,  753,  785,  788,  787,  784,  845,  862,  769,  859,
 /*   560 */   931,  930,  929,  928,  927,  864,  863,  830,  827,  680,
 /*   570 */   681,  900,  898,  899,  897,  683,  682,  679,  678,  861,
 /*   580 */   748,  747,  857,  854,  846,  841,  858,  855,  847,  842,
 /*   590 */   840,  839,  825,  824,  822,  821,  817,  826,  671,  749,
 /*   600 */   745,  744,  815,  751,  750,  688,  686,  684,  665,  663,
 /*   610 */   656,  654,  653,  655,  651,  649,  648,  647,  646,  645,
 /*   620 */   674,  673,  672,  670,  669,  643,  640,  639,  635,  634,
 /*   630 */   632,
};

    /* The next table maps tokens into fallback tokens.  If a construct
    ** like the following:
    **
    **      %fallback ID X Y Z.
    **
    ** appears in the grammar, then ID becomes a fallback token for X, Y,
    ** and Z.  Whenever one of the tokens X, Y, or Z is input to the parser
    ** but it does not parse, the type of the token is changed to ID and
    ** the parse is retried before an error is thrown.
    */
#if YYFALLBACK || TRUE
    static YYCODETYPE[] yyFallback = new YYCODETYPE[]{
0,  /*          $ => nothing */
0,  /*       SEMI => nothing */
26,  /*    EXPLAIN => ID */
26,  /*      QUERY => ID */
26,  /*       PLAN => ID */
26,  /*      BEGIN => ID */
0,  /* TRANSACTION => nothing */
26,  /*   DEFERRED => ID */
26,  /*  IMMEDIATE => ID */
26,  /*  EXCLUSIVE => ID */
0,  /*     COMMIT => nothing */
26,  /*        END => ID */
26,  /*   ROLLBACK => ID */
26,  /*  SAVEPOINT => ID */
26,  /*    RELEASE => ID */
0,  /*         TO => nothing */
0,  /*      TABLE => nothing */
0,  /*     CREATE => nothing */
26,  /*         IF => ID */
0,  /*        NOT => nothing */
0,  /*     EXISTS => nothing */
26,  /*       TEMP => ID */
0,  /*         LP => nothing */
0,  /*         RP => nothing */
0,  /*         AS => nothing */
0,  /*      COMMA => nothing */
0,  /*         ID => nothing */
0,  /*    INDEXED => nothing */
26,  /*      ABORT => ID */
26,  /*     ACTION => ID */
26,  /*      AFTER => ID */
26,  /*    ANALYZE => ID */
26,  /*        ASC => ID */
26,  /*     ATTACH => ID */
26,  /*     BEFORE => ID */
26,  /*         BY => ID */
26,  /*    CASCADE => ID */
26,  /*       CAST => ID */
26,  /*   COLUMNKW => ID */
26,  /*   CONFLICT => ID */
26,  /*   DATABASE => ID */
26,  /*       DESC => ID */
26,  /*     DETACH => ID */
26,  /*       EACH => ID */
26,  /*       FAIL => ID */
26,  /*        FOR => ID */
26,  /*     IGNORE => ID */
26,  /*  INITIALLY => ID */
26,  /*    INSTEAD => ID */
26,  /*    LIKE_KW => ID */
26,  /*      MATCH => ID */
26,  /*         NO => ID */
26,  /*        KEY => ID */
26,  /*         OF => ID */
26,  /*     OFFSET => ID */
26,  /*     PRAGMA => ID */
26,  /*      RAISE => ID */
26,  /*    REPLACE => ID */
26,  /*   RESTRICT => ID */
26,  /*        ROW => ID */
26,  /*    TRIGGER => ID */
26,  /*     VACUUM => ID */
26,  /*       VIEW => ID */
26,  /*    VIRTUAL => ID */
26,  /*    REINDEX => ID */
26,  /*     RENAME => ID */
26,  /*   CTIME_KW => ID */
};
#endif // * YYFALLBACK */

    /* The following structure represents a single element of the
** parser's stack.  Information stored includes:
**
**   +  The state number for the parser at this level of the stack.
**
**   +  The value of the token stored at this level of the stack.
**      (In other words, the "major" token.)
**
**   +  The semantic value stored at this level of the stack.  This is
**      the information used by the action routines in the grammar.
**      It is sometimes called the "minor" token.
*/
    public class yyStackEntry
    {
      public YYACTIONTYPE stateno;       /* The state-number */
      public YYCODETYPE major;         /* The major token value.  This is the code
** number for the token at this stack level */
      public YYMINORTYPE minor; /* The user-supplied minor token value.  This
** is the value of the token  */
    };
    //typedef struct yyStackEntry yyStackEntry;

    /* The state of the parser is completely contained in an instance of
    ** the following structure */
    public class yyParser
    {
      public int yyidx;                    /* Index of top element in stack */
#if YYTRACKMAXSTACKDEPTH
int yyidxMax;                 /* Maximum value of yyidx */
#endif
      public int yyerrcnt;                 /* Shifts left before out of the error */
      public Parse pParse;  // sqlite3ParserARG_SDECL                /* A place to hold %extra_argument */
#if YYSTACKDEPTH//<=0
public int yystksz;                  /* Current side of the stack */
public yyStackEntry *yystack;        /* The parser's stack */
#else
      public yyStackEntry[] yystack = new yyStackEntry[YYSTACKDEPTH];  /* The parser's stack */
#endif
    };
    //typedef struct yyParser yyParser;

#if !NDEBUG
    //#include <stdio.h>
    static TextWriter yyTraceFILE = null;
    static string yyTracePrompt = "";
#endif // * NDEBUG */

#if !NDEBUG
    /*
** Turn parser tracing on by giving a stream to which to write the trace
** and a prompt to preface each trace message.  Tracing is turned off
** by making either argument NULL
**
** Inputs:
** <ul>
** <li> A FILE* to which trace output should be written.
**      If NULL, then tracing is turned off.
** <li> A prefix string written at the beginning of every
**      line of trace output.  If NULL, then tracing is
**      turned off.
** </ul>
**
** Outputs:
** None.
*/
    static void sqlite3ParserTrace( TextWriter TraceFILE, string zTracePrompt )
    {
      yyTraceFILE = TraceFILE;
      yyTracePrompt = zTracePrompt;
      if ( yyTraceFILE == null ) yyTracePrompt = "";
      else if ( yyTracePrompt == "" ) yyTraceFILE = null;
    }
#endif // * NDEBUG */

#if !NDEBUG
    /* For tracing shifts, the names of all terminals and nonterminals
** are required.  The following table supplies these names */
    static string[] yyTokenName = {
  "$",             "SEMI",          "EXPLAIN",       "QUERY",       
  "PLAN",          "BEGIN",         "TRANSACTION",   "DEFERRED",    
  "IMMEDIATE",     "EXCLUSIVE",     "COMMIT",        "END",         
  "ROLLBACK",      "SAVEPOINT",     "RELEASE",       "TO",          
  "TABLE",         "CREATE",        "IF",            "NOT",         
  "EXISTS",        "TEMP",          "LP",            "RP",          
  "AS",            "COMMA",         "ID",            "INDEXED",     
  "ABORT",         "ACTION",        "AFTER",         "ANALYZE",     
  "ASC",           "ATTACH",        "BEFORE",        "BY",          
  "CASCADE",       "CAST",          "COLUMNKW",      "CONFLICT",    
  "DATABASE",      "DESC",          "DETACH",        "EACH",        
  "FAIL",          "FOR",           "IGNORE",        "INITIALLY",   
  "INSTEAD",       "LIKE_KW",       "MATCH",         "NO",          
  "KEY",           "OF",            "OFFSET",        "PRAGMA",      
  "RAISE",         "REPLACE",       "RESTRICT",      "ROW",         
  "TRIGGER",       "VACUUM",        "VIEW",          "VIRTUAL",     
  "REINDEX",       "RENAME",        "CTIME_KW",      "ANY",         
  "OR",            "AND",           "IS",            "BETWEEN",     
  "IN",            "ISNULL",        "NOTNULL",       "NE",          
  "EQ",            "GT",            "LE",            "LT",          
  "GE",            "ESCAPE",        "BITAND",        "BITOR",       
  "LSHIFT",        "RSHIFT",        "PLUS",          "MINUS",       
  "STAR",          "SLASH",         "REM",           "CONCAT",      
  "COLLATE",       "BITNOT",        "STRING",        "JOIN_KW",     
  "CONSTRAINT",    "DEFAULT",       "NULL",          "PRIMARY",     
  "UNIQUE",        "CHECK",         "REFERENCES",    "AUTOINCR",    
  "ON",            "INSERT",        "DELETE",        "UPDATE",      
  "SET",           "DEFERRABLE",    "FOREIGN",       "DROP",        
  "UNION",         "ALL",           "EXCEPT",        "INTERSECT",   
  "SELECT",        "DISTINCT",      "DOT",           "FROM",        
  "JOIN",          "USING",         "ORDER",         "GROUP",       
  "HAVING",        "LIMIT",         "WHERE",         "INTO",        
  "VALUES",        "INTEGER",       "FLOAT",         "BLOB",        
  "REGISTER",      "VARIABLE",      "CASE",          "WHEN",        
  "THEN",          "ELSE",          "INDEX",         "ALTER",       
  "ADD",           "error",         "input",         "cmdlist",     
  "ecmd",          "explain",       "cmdx",          "cmd",         
  "transtype",     "trans_opt",     "nm",            "savepoint_opt",
  "create_table",  "create_table_args",  "createkw",      "temp",        
  "ifnotexists",   "dbnm",          "columnlist",    "conslist_opt",
  "select",        "column",        "columnid",      "type",        
  "carglist",      "id",            "ids",           "typetoken",   
  "typename",      "signed",        "plus_num",      "minus_num",   
  "carg",          "ccons",         "term",          "expr",        
  "onconf",        "sortorder",     "autoinc",       "idxlist_opt", 
  "refargs",       "defer_subclause",  "refarg",        "refact",      
  "init_deferred_pred_opt",  "conslist",      "tcons",         "idxlist",     
  "defer_subclause_opt",  "orconf",        "resolvetype",   "raisetype",   
  "ifexists",      "fullname",      "oneselect",     "multiselect_op",
  "distinct",      "selcollist",    "from",          "where_opt",   
  "groupby_opt",   "having_opt",    "orderby_opt",   "limit_opt",   
  "sclp",          "as",            "seltablist",    "stl_prefix",  
  "joinop",        "indexed_opt",   "on_opt",        "using_opt",   
  "joinop2",       "inscollist",    "sortlist",      "sortitem",    
  "nexprlist",     "setlist",       "insert_cmd",    "inscollist_opt",
  "itemlist",      "exprlist",      "likeop",        "escape",      
  "between_op",    "in_op",         "case_operand",  "case_exprlist",
  "case_else",     "uniqueflag",    "collate",       "nmnum",       
  "plus_opt",      "number",        "trigger_decl",  "trigger_cmd_list",
  "trigger_time",  "trigger_event",  "foreach_clause",  "when_clause", 
  "trigger_cmd",   "trnm",          "tridxby",       "database_kw_opt",
  "key_opt",       "add_column_fullname",  "kwcolumn_opt",  "create_vtab", 
  "vtabarglist",   "vtabarg",       "vtabargtoken",  "lp",          
  "anylist",     
};
#endif // * NDEBUG */

#if !NDEBUG
    /* For tracing reduce actions, the names of all rules are required.
*/
    static string[] yyRuleName = {
 /*   0 */ "input ::= cmdlist",
 /*   1 */ "cmdlist ::= cmdlist ecmd",
 /*   2 */ "cmdlist ::= ecmd",
 /*   3 */ "ecmd ::= SEMI",
 /*   4 */ "ecmd ::= explain cmdx SEMI",
 /*   5 */ "explain ::=",
 /*   6 */ "explain ::= EXPLAIN",
 /*   7 */ "explain ::= EXPLAIN QUERY PLAN",
 /*   8 */ "cmdx ::= cmd",
 /*   9 */ "cmd ::= BEGIN transtype trans_opt",
 /*  10 */ "trans_opt ::=",
 /*  11 */ "trans_opt ::= TRANSACTION",
 /*  12 */ "trans_opt ::= TRANSACTION nm",
 /*  13 */ "transtype ::=",
 /*  14 */ "transtype ::= DEFERRED",
 /*  15 */ "transtype ::= IMMEDIATE",
 /*  16 */ "transtype ::= EXCLUSIVE",
 /*  17 */ "cmd ::= COMMIT trans_opt",
 /*  18 */ "cmd ::= END trans_opt",
 /*  19 */ "cmd ::= ROLLBACK trans_opt",
 /*  20 */ "savepoint_opt ::= SAVEPOINT",
 /*  21 */ "savepoint_opt ::=",
 /*  22 */ "cmd ::= SAVEPOINT nm",
 /*  23 */ "cmd ::= RELEASE savepoint_opt nm",
 /*  24 */ "cmd ::= ROLLBACK trans_opt TO savepoint_opt nm",
 /*  25 */ "cmd ::= create_table create_table_args",
 /*  26 */ "create_table ::= createkw temp TABLE ifnotexists nm dbnm",
 /*  27 */ "createkw ::= CREATE",
 /*  28 */ "ifnotexists ::=",
 /*  29 */ "ifnotexists ::= IF NOT EXISTS",
 /*  30 */ "temp ::= TEMP",
 /*  31 */ "temp ::=",
 /*  32 */ "create_table_args ::= LP columnlist conslist_opt RP",
 /*  33 */ "create_table_args ::= AS select",
 /*  34 */ "columnlist ::= columnlist COMMA column",
 /*  35 */ "columnlist ::= column",
 /*  36 */ "column ::= columnid type carglist",
 /*  37 */ "columnid ::= nm",
 /*  38 */ "id ::= ID",
 /*  39 */ "id ::= INDEXED",
 /*  40 */ "ids ::= ID|STRING",
 /*  41 */ "nm ::= id",
 /*  42 */ "nm ::= STRING",
 /*  43 */ "nm ::= JOIN_KW",
 /*  44 */ "type ::=",
 /*  45 */ "type ::= typetoken",
 /*  46 */ "typetoken ::= typename",
 /*  47 */ "typetoken ::= typename LP signed RP",
 /*  48 */ "typetoken ::= typename LP signed COMMA signed RP",
 /*  49 */ "typename ::= ids",
 /*  50 */ "typename ::= typename ids",
 /*  51 */ "signed ::= plus_num",
 /*  52 */ "signed ::= minus_num",
 /*  53 */ "carglist ::= carglist carg",
 /*  54 */ "carglist ::=",
 /*  55 */ "carg ::= CONSTRAINT nm ccons",
 /*  56 */ "carg ::= ccons",
 /*  57 */ "ccons ::= DEFAULT term",
 /*  58 */ "ccons ::= DEFAULT LP expr RP",
 /*  59 */ "ccons ::= DEFAULT PLUS term",
 /*  60 */ "ccons ::= DEFAULT MINUS term",
 /*  61 */ "ccons ::= DEFAULT id",
 /*  62 */ "ccons ::= NULL onconf",
 /*  63 */ "ccons ::= NOT NULL onconf",
 /*  64 */ "ccons ::= PRIMARY KEY sortorder onconf autoinc",
 /*  65 */ "ccons ::= UNIQUE onconf",
 /*  66 */ "ccons ::= CHECK LP expr RP",
 /*  67 */ "ccons ::= REFERENCES nm idxlist_opt refargs",
 /*  68 */ "ccons ::= defer_subclause",
 /*  69 */ "ccons ::= COLLATE ids",
 /*  70 */ "autoinc ::=",
 /*  71 */ "autoinc ::= AUTOINCR",
 /*  72 */ "refargs ::=",
 /*  73 */ "refargs ::= refargs refarg",
 /*  74 */ "refarg ::= MATCH nm",
 /*  75 */ "refarg ::= ON INSERT refact",
 /*  76 */ "refarg ::= ON DELETE refact",
 /*  77 */ "refarg ::= ON UPDATE refact",
 /*  78 */ "refact ::= SET NULL",
 /*  79 */ "refact ::= SET DEFAULT",
 /*  80 */ "refact ::= CASCADE",
 /*  81 */ "refact ::= RESTRICT",
 /*  82 */ "refact ::= NO ACTION",
 /*  83 */ "defer_subclause ::= NOT DEFERRABLE init_deferred_pred_opt",
 /*  84 */ "defer_subclause ::= DEFERRABLE init_deferred_pred_opt",
 /*  85 */ "init_deferred_pred_opt ::=",
 /*  86 */ "init_deferred_pred_opt ::= INITIALLY DEFERRED",
 /*  87 */ "init_deferred_pred_opt ::= INITIALLY IMMEDIATE",
 /*  88 */ "conslist_opt ::=",
 /*  89 */ "conslist_opt ::= COMMA conslist",
 /*  90 */ "conslist ::= conslist COMMA tcons",
 /*  91 */ "conslist ::= conslist tcons",
 /*  92 */ "conslist ::= tcons",
 /*  93 */ "tcons ::= CONSTRAINT nm",
 /*  94 */ "tcons ::= PRIMARY KEY LP idxlist autoinc RP onconf",
 /*  95 */ "tcons ::= UNIQUE LP idxlist RP onconf",
 /*  96 */ "tcons ::= CHECK LP expr RP onconf",
 /*  97 */ "tcons ::= FOREIGN KEY LP idxlist RP REFERENCES nm idxlist_opt refargs defer_subclause_opt",
 /*  98 */ "defer_subclause_opt ::=",
 /*  99 */ "defer_subclause_opt ::= defer_subclause",
 /* 100 */ "onconf ::=",
 /* 101 */ "onconf ::= ON CONFLICT resolvetype",
 /* 102 */ "orconf ::=",
 /* 103 */ "orconf ::= OR resolvetype",
 /* 104 */ "resolvetype ::= raisetype",
 /* 105 */ "resolvetype ::= IGNORE",
 /* 106 */ "resolvetype ::= REPLACE",
 /* 107 */ "cmd ::= DROP TABLE ifexists fullname",
 /* 108 */ "ifexists ::= IF EXISTS",
 /* 109 */ "ifexists ::=",
 /* 110 */ "cmd ::= createkw temp VIEW ifnotexists nm dbnm AS select",
 /* 111 */ "cmd ::= DROP VIEW ifexists fullname",
 /* 112 */ "cmd ::= select",
 /* 113 */ "select ::= oneselect",
 /* 114 */ "select ::= select multiselect_op oneselect",
 /* 115 */ "multiselect_op ::= UNION",
 /* 116 */ "multiselect_op ::= UNION ALL",
 /* 117 */ "multiselect_op ::= EXCEPT|INTERSECT",
 /* 118 */ "oneselect ::= SELECT distinct selcollist from where_opt groupby_opt having_opt orderby_opt limit_opt",
 /* 119 */ "distinct ::= DISTINCT",
 /* 120 */ "distinct ::= ALL",
 /* 121 */ "distinct ::=",
 /* 122 */ "sclp ::= selcollist COMMA",
 /* 123 */ "sclp ::=",
 /* 124 */ "selcollist ::= sclp expr as",
 /* 125 */ "selcollist ::= sclp STAR",
 /* 126 */ "selcollist ::= sclp nm DOT STAR",
 /* 127 */ "as ::= AS nm",
 /* 128 */ "as ::= ids",
 /* 129 */ "as ::=",
 /* 130 */ "from ::=",
 /* 131 */ "from ::= FROM seltablist",
 /* 132 */ "stl_prefix ::= seltablist joinop",
 /* 133 */ "stl_prefix ::=",
 /* 134 */ "seltablist ::= stl_prefix nm dbnm as indexed_opt on_opt using_opt",
 /* 135 */ "seltablist ::= stl_prefix LP select RP as on_opt using_opt",
 /* 136 */ "seltablist ::= stl_prefix LP seltablist RP as on_opt using_opt",
 /* 137 */ "dbnm ::=",
 /* 138 */ "dbnm ::= DOT nm",
 /* 139 */ "fullname ::= nm dbnm",
 /* 140 */ "joinop ::= COMMA|JOIN",
 /* 141 */ "joinop ::= JOIN_KW JOIN",
 /* 142 */ "joinop ::= JOIN_KW nm JOIN",
 /* 143 */ "joinop ::= JOIN_KW nm nm JOIN",
 /* 144 */ "on_opt ::= ON expr",
 /* 145 */ "on_opt ::=",
 /* 146 */ "indexed_opt ::=",
 /* 147 */ "indexed_opt ::= INDEXED BY nm",
 /* 148 */ "indexed_opt ::= NOT INDEXED",
 /* 149 */ "using_opt ::= USING LP inscollist RP",
 /* 150 */ "using_opt ::=",
 /* 151 */ "orderby_opt ::=",
 /* 152 */ "orderby_opt ::= ORDER BY sortlist",
 /* 153 */ "sortlist ::= sortlist COMMA sortitem sortorder",
 /* 154 */ "sortlist ::= sortitem sortorder",
 /* 155 */ "sortitem ::= expr",
 /* 156 */ "sortorder ::= ASC",
 /* 157 */ "sortorder ::= DESC",
 /* 158 */ "sortorder ::=",
 /* 159 */ "groupby_opt ::=",
 /* 160 */ "groupby_opt ::= GROUP BY nexprlist",
 /* 161 */ "having_opt ::=",
 /* 162 */ "having_opt ::= HAVING expr",
 /* 163 */ "limit_opt ::=",
 /* 164 */ "limit_opt ::= LIMIT expr",
 /* 165 */ "limit_opt ::= LIMIT expr OFFSET expr",
 /* 166 */ "limit_opt ::= LIMIT expr COMMA expr",
 /* 167 */ "cmd ::= DELETE FROM fullname indexed_opt where_opt",
 /* 168 */ "where_opt ::=",
 /* 169 */ "where_opt ::= WHERE expr",
 /* 170 */ "cmd ::= UPDATE orconf fullname indexed_opt SET setlist where_opt",
 /* 171 */ "setlist ::= setlist COMMA nm EQ expr",
 /* 172 */ "setlist ::= nm EQ expr",
 /* 173 */ "cmd ::= insert_cmd INTO fullname inscollist_opt VALUES LP itemlist RP",
 /* 174 */ "cmd ::= insert_cmd INTO fullname inscollist_opt select",
 /* 175 */ "cmd ::= insert_cmd INTO fullname inscollist_opt DEFAULT VALUES",
 /* 176 */ "insert_cmd ::= INSERT orconf",
 /* 177 */ "insert_cmd ::= REPLACE",
 /* 178 */ "itemlist ::= itemlist COMMA expr",
 /* 179 */ "itemlist ::= expr",
 /* 180 */ "inscollist_opt ::=",
 /* 181 */ "inscollist_opt ::= LP inscollist RP",
 /* 182 */ "inscollist ::= inscollist COMMA nm",
 /* 183 */ "inscollist ::= nm",
 /* 184 */ "expr ::= term",
 /* 185 */ "expr ::= LP expr RP",
 /* 186 */ "term ::= NULL",
 /* 187 */ "expr ::= id",
 /* 188 */ "expr ::= JOIN_KW",
 /* 189 */ "expr ::= nm DOT nm",
 /* 190 */ "expr ::= nm DOT nm DOT nm",
 /* 191 */ "term ::= INTEGER|FLOAT|BLOB",
 /* 192 */ "term ::= STRING",
 /* 193 */ "expr ::= REGISTER",
 /* 194 */ "expr ::= VARIABLE",
 /* 195 */ "expr ::= expr COLLATE ids",
 /* 196 */ "expr ::= CAST LP expr AS typetoken RP",
 /* 197 */ "expr ::= ID LP distinct exprlist RP",
 /* 198 */ "expr ::= ID LP STAR RP",
 /* 199 */ "term ::= CTIME_KW",
 /* 200 */ "expr ::= expr AND expr",
 /* 201 */ "expr ::= expr OR expr",
 /* 202 */ "expr ::= expr LT|GT|GE|LE expr",
 /* 203 */ "expr ::= expr EQ|NE expr",
 /* 204 */ "expr ::= expr BITAND|BITOR|LSHIFT|RSHIFT expr",
 /* 205 */ "expr ::= expr PLUS|MINUS expr",
 /* 206 */ "expr ::= expr STAR|SLASH|REM expr",
 /* 207 */ "expr ::= expr CONCAT expr",
 /* 208 */ "likeop ::= LIKE_KW",
 /* 209 */ "likeop ::= NOT LIKE_KW",
 /* 210 */ "likeop ::= MATCH",
 /* 211 */ "likeop ::= NOT MATCH",
 /* 212 */ "escape ::= ESCAPE expr",
 /* 213 */ "escape ::=",
 /* 214 */ "expr ::= expr likeop expr escape",
 /* 215 */ "expr ::= expr ISNULL|NOTNULL",
 /* 216 */ "expr ::= expr NOT NULL",
 /* 217 */ "expr ::= expr IS expr",
 /* 218 */ "expr ::= expr IS NOT expr",
 /* 219 */ "expr ::= NOT expr",
 /* 220 */ "expr ::= BITNOT expr",
 /* 221 */ "expr ::= MINUS expr",
 /* 222 */ "expr ::= PLUS expr",
 /* 223 */ "between_op ::= BETWEEN",
 /* 224 */ "between_op ::= NOT BETWEEN",
 /* 225 */ "expr ::= expr between_op expr AND expr",
 /* 226 */ "in_op ::= IN",
 /* 227 */ "in_op ::= NOT IN",
 /* 228 */ "expr ::= expr in_op LP exprlist RP",
 /* 229 */ "expr ::= LP select RP",
 /* 230 */ "expr ::= expr in_op LP select RP",
 /* 231 */ "expr ::= expr in_op nm dbnm",
 /* 232 */ "expr ::= EXISTS LP select RP",
 /* 233 */ "expr ::= CASE case_operand case_exprlist case_else END",
 /* 234 */ "case_exprlist ::= case_exprlist WHEN expr THEN expr",
 /* 235 */ "case_exprlist ::= WHEN expr THEN expr",
 /* 236 */ "case_else ::= ELSE expr",
 /* 237 */ "case_else ::=",
 /* 238 */ "case_operand ::= expr",
 /* 239 */ "case_operand ::=",
 /* 240 */ "exprlist ::= nexprlist",
 /* 241 */ "exprlist ::=",
 /* 242 */ "nexprlist ::= nexprlist COMMA expr",
 /* 243 */ "nexprlist ::= expr",
 /* 244 */ "cmd ::= createkw uniqueflag INDEX ifnotexists nm dbnm ON nm LP idxlist RP",
 /* 245 */ "uniqueflag ::= UNIQUE",
 /* 246 */ "uniqueflag ::=",
 /* 247 */ "idxlist_opt ::=",
 /* 248 */ "idxlist_opt ::= LP idxlist RP",
 /* 249 */ "idxlist ::= idxlist COMMA nm collate sortorder",
 /* 250 */ "idxlist ::= nm collate sortorder",
 /* 251 */ "collate ::=",
 /* 252 */ "collate ::= COLLATE ids",
 /* 253 */ "cmd ::= DROP INDEX ifexists fullname",
 /* 254 */ "cmd ::= VACUUM",
 /* 255 */ "cmd ::= VACUUM nm",
 /* 256 */ "cmd ::= PRAGMA nm dbnm",
 /* 257 */ "cmd ::= PRAGMA nm dbnm EQ nmnum",
 /* 258 */ "cmd ::= PRAGMA nm dbnm LP nmnum RP",
 /* 259 */ "cmd ::= PRAGMA nm dbnm EQ minus_num",
 /* 260 */ "cmd ::= PRAGMA nm dbnm LP minus_num RP",
 /* 261 */ "nmnum ::= plus_num",
 /* 262 */ "nmnum ::= nm",
 /* 263 */ "nmnum ::= ON",
 /* 264 */ "nmnum ::= DELETE",
 /* 265 */ "nmnum ::= DEFAULT",
 /* 266 */ "plus_num ::= plus_opt number",
 /* 267 */ "minus_num ::= MINUS number",
 /* 268 */ "number ::= INTEGER|FLOAT",
 /* 269 */ "plus_opt ::= PLUS",
 /* 270 */ "plus_opt ::=",
 /* 271 */ "cmd ::= createkw trigger_decl BEGIN trigger_cmd_list END",
 /* 272 */ "trigger_decl ::= temp TRIGGER ifnotexists nm dbnm trigger_time trigger_event ON fullname foreach_clause when_clause",
 /* 273 */ "trigger_time ::= BEFORE",
 /* 274 */ "trigger_time ::= AFTER",
 /* 275 */ "trigger_time ::= INSTEAD OF",
 /* 276 */ "trigger_time ::=",
 /* 277 */ "trigger_event ::= DELETE|INSERT",
 /* 278 */ "trigger_event ::= UPDATE",
 /* 279 */ "trigger_event ::= UPDATE OF inscollist",
 /* 280 */ "foreach_clause ::=",
 /* 281 */ "foreach_clause ::= FOR EACH ROW",
 /* 282 */ "when_clause ::=",
 /* 283 */ "when_clause ::= WHEN expr",
 /* 284 */ "trigger_cmd_list ::= trigger_cmd_list trigger_cmd SEMI",
 /* 285 */ "trigger_cmd_list ::= trigger_cmd SEMI",
 /* 286 */ "trnm ::= nm",
 /* 287 */ "trnm ::= nm DOT nm",
 /* 288 */ "tridxby ::=",
 /* 289 */ "tridxby ::= INDEXED BY nm",
 /* 290 */ "tridxby ::= NOT INDEXED",
 /* 291 */ "trigger_cmd ::= UPDATE orconf trnm tridxby SET setlist where_opt",
 /* 292 */ "trigger_cmd ::= insert_cmd INTO trnm inscollist_opt VALUES LP itemlist RP",
 /* 293 */ "trigger_cmd ::= insert_cmd INTO trnm inscollist_opt select",
 /* 294 */ "trigger_cmd ::= DELETE FROM trnm tridxby where_opt",
 /* 295 */ "trigger_cmd ::= select",
 /* 296 */ "expr ::= RAISE LP IGNORE RP",
 /* 297 */ "expr ::= RAISE LP raisetype COMMA nm RP",
 /* 298 */ "raisetype ::= ROLLBACK",
 /* 299 */ "raisetype ::= ABORT",
 /* 300 */ "raisetype ::= FAIL",
 /* 301 */ "cmd ::= DROP TRIGGER ifexists fullname",
 /* 302 */ "cmd ::= ATTACH database_kw_opt expr AS expr key_opt",
 /* 303 */ "cmd ::= DETACH database_kw_opt expr",
 /* 304 */ "key_opt ::=",
 /* 305 */ "key_opt ::= KEY expr",
 /* 306 */ "database_kw_opt ::= DATABASE",
 /* 307 */ "database_kw_opt ::=",
 /* 308 */ "cmd ::= REINDEX",
 /* 309 */ "cmd ::= REINDEX nm dbnm",
 /* 310 */ "cmd ::= ANALYZE",
 /* 311 */ "cmd ::= ANALYZE nm dbnm",
 /* 312 */ "cmd ::= ALTER TABLE fullname RENAME TO nm",
 /* 313 */ "cmd ::= ALTER TABLE add_column_fullname ADD kwcolumn_opt column",
 /* 314 */ "add_column_fullname ::= fullname",
 /* 315 */ "kwcolumn_opt ::=",
 /* 316 */ "kwcolumn_opt ::= COLUMNKW",
 /* 317 */ "cmd ::= create_vtab",
 /* 318 */ "cmd ::= create_vtab LP vtabarglist RP",
 /* 319 */ "create_vtab ::= createkw VIRTUAL TABLE nm dbnm USING nm",
 /* 320 */ "vtabarglist ::= vtabarg",
 /* 321 */ "vtabarglist ::= vtabarglist COMMA vtabarg",
 /* 322 */ "vtabarg ::=",
 /* 323 */ "vtabarg ::= vtabarg vtabargtoken",
 /* 324 */ "vtabargtoken ::= ANY",
 /* 325 */ "vtabargtoken ::= lp anylist RP",
 /* 326 */ "lp ::= LP",
 /* 327 */ "anylist ::=",
 /* 328 */ "anylist ::= anylist LP anylist RP",
 /* 329 */ "anylist ::= anylist ANY",
};
#endif // * NDEBUG */


#if YYSTACKDEPTH//<=0
/*
** Try to increase the size of the parser stack.
*/
static void yyGrowStack(yyParser p){
int newSize;
//yyStackEntry pNew;

newSize = p.yystksz*2 + 100;
//pNew = realloc(p.yystack, newSize*sizeof(pNew[0]));
//if( pNew !=null){
p.yystack = Array.Resize(p.yystack,newSize); //pNew;
p.yystksz = newSize;
#if !NDEBUG
if( yyTraceFILE ){
fprintf(yyTraceFILE,"%sStack grows to %d entries!\n",
yyTracePrompt, p.yystksz);
}
#endif
//}
}
#endif

    /*
** This function allocates a new parser.
** The only argument is a pointer to a function which works like
** malloc.
**
** Inputs:
** A pointer to the function used to allocate memory.
**
** Outputs:
** A pointer to a parser.  This pointer is used in subsequent calls
** to sqlite3Parser and sqlite3ParserFree.
*/
    static yyParser sqlite3ParserAlloc()
    {//void *(*mallocProc)(size_t)){
      yyParser pParser = new yyParser();
      //pParser = (yyParser*)(*mallocProc)( (size_t)yyParser.Length );
      if ( pParser != null )
      {
        pParser.yyidx = -1;
#if YYTRACKMAXSTACKDEPTH
pParser.yyidxMax=0;
#endif

#if YYSTACKDEPTH//<=0
pParser.yystack = NULL;
pParser.yystksz = 0;
yyGrowStack(pParser);
#endif
      }
      return pParser;
    }

    /* The following function deletes the value associated with a
    ** symbol.  The symbol can be either a terminal or nonterminal.
    ** "yymajor" is the symbol code, and "yypminor" is a pointer to
    ** the value.
    */
    static void yy_destructor(
    yyParser yypParser,    /* The parser */
    YYCODETYPE yymajor,    /* Type code for object to destroy */
    YYMINORTYPE yypminor   /* The object to be destroyed */
    )
    {
      Parse pParse = yypParser.pParse; // sqlite3ParserARG_FETCH;
      switch ( yymajor )
      {
        /* Here is inserted the actions which take place when a
        ** terminal or non-terminal is destroyed.  This can happen
        ** when the symbol is popped from the stack during a
        ** reduce or during error processing or when a parser is
        ** being destroyed before it is finished parsing.
        **
        ** Note: during a reduce, the only symbols destroyed are those
        ** which appear on the RHS of the rule, but which are not used
        ** inside the C code.
        */
        case 160: /* select */
        case 194: /* oneselect */
          {
            //#line 403 "parse.y"
            sqlite3SelectDelete( pParse.db, ref ( yypminor.yy3 ) );
            //#line 1400 "parse.c"
          }
          break;
        case 174: /* term */
        case 175: /* Expr */
        case 223: /* escape */
          {
            //#line 720 "parse.y"
            sqlite3ExprDelete( pParse.db, ref ( yypminor.yy346 ).pExpr );
            //#line 1409 "parse.c"
          }
          break;
        case 179: /* idxlist_opt */
        case 187: /* idxlist */
        case 197: /* selcollist */
        case 200: /* groupby_opt */
        case 202: /* orderby_opt */
        case 204: /* sclp */
        case 214: /* sortlist */
        case 216: /* nexprlist */
        case 217: /* setlist */
        case 220: /* itemlist */
        case 221: /* exprlist */
        case 227: /* case_exprlist */
          {
            //#line 1086 "parse.y"
            sqlite3ExprListDelete( pParse.db, ref  ( yypminor.yy14 ) );
            //#line 1427 "parse.c"
          }
          break;
        case 193: /* fullname */
        case 198: /* from */
        case 206: /* seltablist */
        case 207: /* stl_prefix */
          {
            //#line 534 "parse.y"
            sqlite3SrcListDelete( pParse.db, ref  ( yypminor.yy65 ) );
            //#line 1437 "parse.c"
          }
          break;
        case 199: /* where_opt */
        case 201: /* having_opt */
        case 210: /* on_opt */
        case 215: /* sortitem */
        case 226: /* case_operand */
        case 228: /* case_else */
        case 239: /* when_clause */
        case 242: /* key_opt */
          {
            //#line 644 "parse.y"
            sqlite3ExprDelete( pParse.db, ref ( yypminor.yy132 ) );
            //#line 1451 "parse.c"
          }
          break;
        case 211: /* using_opt */
        case 213: /* inscollist */
        case 219: /* inscollist_opt */
          {
            //#line 566 "parse.y"
            sqlite3IdListDelete( pParse.db, ref ( yypminor.yy408 ) );
            //#line 1460 "parse.c"
          }
          break;
        case 235: /* trigger_cmd_list */
        case 240: /* trigger_cmd */
          {
            //#line 1193 "parse.y"
            sqlite3DeleteTriggerStep( pParse.db, ref ( yypminor.yy473 ) );
            //#line 1468 "parse.c"
          }
          break;
        case 237: /* trigger_event */
          {
            //#line 1179 "parse.y"
            sqlite3IdListDelete( pParse.db, ref ( yypminor.yy378 ).b );
            //#line 1475 "parse.c"
          }
          break;
        default: break;   /* If no destructor action specified: do nothing */
      }
    }

    /*
    ** Pop the parser's stack once.
    **
    ** If there is a destructor routine associated with the token which
    ** is popped from the stack, then call it.
    **
    ** Return the major token number for the symbol popped.
    */
    static int yy_pop_parser_stack( yyParser pParser )
    {
      YYCODETYPE yymajor;
      yyStackEntry yytos = pParser.yystack[pParser.yyidx];

      /* There is no mechanism by which the parser stack can be popped below
      ** empty in SQLite.  */
      if ( NEVER( pParser.yyidx < 0 ) ) return 0;
#if !NDEBUG
      if ( yyTraceFILE != null && pParser.yyidx >= 0 )
      {
        fprintf( yyTraceFILE, "%sPopping %s\n",
        yyTracePrompt,
        yyTokenName[yytos.major] );
      }
#endif
      yymajor = yytos.major;
      yy_destructor( pParser, yymajor, yytos.minor );
      pParser.yyidx--;
      return yymajor;
    }

    /*
    ** Deallocate and destroy a parser.  Destructors are all called for
    ** all stack elements before shutting the parser down.
    **
    ** Inputs:
    ** <ul>
    ** <li>  A pointer to the parser.  This should be a pointer
    **       obtained from sqlite3ParserAlloc.
    ** <li>  A pointer to a function used to reclaim memory obtained
    **       from malloc.
    ** </ul>
    */
    static void sqlite3ParserFree(
    yyParser p,                    /* The parser to be deleted */
    dxDel freeProc//)(void*)     /* Function used to reclaim memory */
    )
    {
      yyParser pParser = p;
      /* In SQLite, we never try to destroy a parser that was not successfully
      ** created in the first place. */
      if ( NEVER( pParser == null ) ) return;
      while ( pParser.yyidx >= 0 ) yy_pop_parser_stack( pParser );
#if YYSTACKDEPTH//<=0
pParser.yystack = null;//free(pParser.yystack);
#endif
      pParser = null;// freeProc(ref pParser);
    }

    /*
    ** Return the peak depth of the stack for a parser.
    */
#if YYTRACKMAXSTACKDEPTH
int sqlite3ParserStackPeak(void p){
yyParser pParser = (yyParser*)p;
return pParser.yyidxMax;
}
#endif

    /*
** Find the appropriate action for a parser given the terminal
** look-ahead token iLookAhead.
**
** If the look-ahead token is YYNOCODE, then check to see if the action is
** independent of the look-ahead.  If it is, return the action, otherwise
** return YY_NO_ACTION.
*/
    static int yy_find_shift_action(
    yyParser pParser,         /* The parser */
    YYCODETYPE iLookAhead     /* The look-ahead token */
    )
    {
      int i;
      int stateno = pParser.yystack[pParser.yyidx].stateno;

      if ( stateno > YY_SHIFT_COUNT
       || ( i = yy_shift_ofst[stateno] ) == YY_SHIFT_USE_DFLT )
      {
        return yy_default[stateno];
      }
      Debug.Assert( iLookAhead != YYNOCODE );
      i += iLookAhead;
      if ( i < 0 || i >= YY_ACTTAB_COUNT || yy_lookahead[i] != iLookAhead )
      {
        if ( iLookAhead > 0 )
        {
          //#if YYFALLBACK
          if ( YYFALLBACK != 0 )
          {
            YYCODETYPE iFallback;            /* Fallback token */
            if ( iLookAhead < yyFallback.Length //yyFallback.Length/sizeof(yyFallback[0])
            && ( iFallback = yyFallback[iLookAhead] ) != 0 )
            {
#if !NDEBUG
              if ( yyTraceFILE != null )
              {
                fprintf( yyTraceFILE, "%sFALLBACK %s => %s\n",
                yyTracePrompt, yyTokenName[iLookAhead], yyTokenName[iFallback] );
              }
#endif
              return yy_find_shift_action( pParser, iFallback );
            }
          }
          //#endif
          //#if YYWILDCARD
          {
            int j = i - iLookAhead + YYWILDCARD;
            if (
              //#if YY_SHIFT_MIN+YYWILDCARD<0
              j >= 0 &&
              //#endif
              //#if YY_SHIFT_MAX+YYWILDCARD>=YY_ACTTAB_COUNT
              j < YY_ACTTAB_COUNT &&
              //#endif
              yy_lookahead[j] == YYWILDCARD
            )
            {
#if !NDEBUG
              if ( yyTraceFILE != null )
              {
                Debugger.Break(); // TODO --
                //fprintf(yyTraceFILE, "%sWILDCARD %s => %s\n",
                //   yyTracePrompt, yyTokenName[iLookAhead], yyTokenName[YYWILDCARD]);
              }
#endif // * NDEBUG */
              return yy_action[j];
            }
          }
          //#endif // * YYWILDCARD */
        }
        return yy_default[stateno];
      }
      else
      {
        return yy_action[i];
      }
    }

    /*
    ** Find the appropriate action for a parser given the non-terminal
    ** look-ahead token iLookAhead.
    **
    ** If the look-ahead token is YYNOCODE, then check to see if the action is
    ** independent of the look-ahead.  If it is, return the action, otherwise
    ** return YY_NO_ACTION.
    */
    static int yy_find_reduce_action(
    int stateno,              /* Current state number */
    YYCODETYPE iLookAhead     /* The look-ahead token */
    )
    {
      int i;
#if YYERRORSYMBOL
if( stateno>YY_REDUCE_COUNT ){
return yy_default[stateno];
}
#else
      Debug.Assert( stateno <= YY_REDUCE_COUNT );
#endif
      i = yy_reduce_ofst[stateno];
      Debug.Assert( i != YY_REDUCE_USE_DFLT );
      Debug.Assert( iLookAhead != YYNOCODE );
      i += iLookAhead;
#if YYERRORSYMBOL
if( i<0 || i>=YY_ACTTAB_COUNT || yy_lookahead[i]!=iLookAhead ){
return yy_default[stateno];
}
#else
      Debug.Assert( i >= 0 && i < YY_ACTTAB_COUNT );
      Debug.Assert( yy_lookahead[i] == iLookAhead );
#endif
      return yy_action[i];
    }

    /*
    ** The following routine is called if the stack overflows.
    */
    static void yyStackOverflow( yyParser yypParser, YYMINORTYPE yypMinor )
    {
      Parse pParse = yypParser.pParse; // sqlite3ParserARG_FETCH;
      yypParser.yyidx--;
#if !NDEBUG
      if ( yyTraceFILE != null )
      {
        Debugger.Break(); // TODO --
        //fprintf(yyTraceFILE, "%sStack Overflow!\n", yyTracePrompt);
      }
#endif
      while ( yypParser.yyidx >= 0 ) yy_pop_parser_stack( yypParser );
      /* Here code is inserted which will execute if the parser
      ** stack every overflows */
      //#line 38 "parse.y"

      UNUSED_PARAMETER( yypMinor ); /* Silence some compiler warnings */
      sqlite3ErrorMsg( pParse, "parser stack overflow" );
      pParse.parseError = 1;
      //#line 1666  "parse.c"
      yypParser.pParse = pParse;//      sqlite3ParserARG_STORE; /* Suppress warning about unused %extra_argument var */
    }

    /*
    ** Perform a shift action.
    */
    static void yy_shift(
    yyParser yypParser,          /* The parser to be shifted */
    int yyNewState,               /* The new state to shift in */
    int yyMajor,                  /* The major token to shift in */
    YYMINORTYPE yypMinor         /* Pointer to the minor token to shift in */
    )
    {
      yyStackEntry yytos = new yyStackEntry();
      yypParser.yyidx++;
#if YYTRACKMAXSTACKDEPTH
if( yypParser.yyidx>yypParser.yyidxMax ){
yypParser.yyidxMax = yypParser.yyidx;
}
#endif
#if !YYSTACKDEPTH//was YYSTACKDEPTH>0
      if ( yypParser.yyidx >= YYSTACKDEPTH )
      {
        yyStackOverflow( yypParser, yypMinor );
        return;
      }
#else
if( yypParser.yyidx>=yypParser.yystksz ){
yyGrowStack(yypParser);
if( yypParser.yyidx>=yypParser.yystksz ){
yyStackOverflow(yypParser, yypMinor);
return;
}
}
#endif
      yypParser.yystack[yypParser.yyidx] = yytos;//yytos = yypParser.yystack[yypParser.yyidx];
      yytos.stateno = (YYACTIONTYPE)yyNewState;
      yytos.major = (YYCODETYPE)yyMajor;
      yytos.minor = yypMinor;
#if !NDEBUG
      if ( yyTraceFILE != null && yypParser.yyidx > 0 )
      {
        int i;
        fprintf( yyTraceFILE, "%sShift %d\n", yyTracePrompt, yyNewState );
        fprintf( yyTraceFILE, "%sStack:", yyTracePrompt );
        for ( i = 1; i <= yypParser.yyidx; i++ )
          fprintf( yyTraceFILE, " %s", yyTokenName[yypParser.yystack[i].major] );
        fprintf( yyTraceFILE, "\n" );
      }
#endif
    }
    /* The following table contains information about every rule that
    ** is used during the reduce.
    */
    public struct _yyRuleInfo
    {
      public YYCODETYPE lhs;         /* Symbol on the left-hand side of the rule */
      public byte nrhs;     /* Number of right-hand side symbols in the rule */
      public _yyRuleInfo( YYCODETYPE lhs, byte nrhs )
      {
        this.lhs = lhs;
        this.nrhs = nrhs;
      }

    }
    static _yyRuleInfo[] yyRuleInfo = new _yyRuleInfo[]{
new _yyRuleInfo( 142, 1 ),
new _yyRuleInfo( 143, 2 ),
new _yyRuleInfo( 143, 1 ),
new _yyRuleInfo( 144, 1 ),
new _yyRuleInfo( 144, 3 ),
new _yyRuleInfo( 145, 0 ),
new _yyRuleInfo( 145, 1 ),
new _yyRuleInfo( 145, 3 ),
new _yyRuleInfo( 146, 1 ),
new _yyRuleInfo( 147, 3 ),
new _yyRuleInfo( 149, 0 ),
new _yyRuleInfo( 149, 1 ),
new _yyRuleInfo( 149, 2 ),
new _yyRuleInfo( 148, 0 ),
new _yyRuleInfo( 148, 1 ),
new _yyRuleInfo( 148, 1 ),
new _yyRuleInfo( 148, 1 ),
new _yyRuleInfo( 147, 2 ),
new _yyRuleInfo( 147, 2 ),
new _yyRuleInfo( 147, 2 ),
new _yyRuleInfo( 151, 1 ),
new _yyRuleInfo( 151, 0 ),
new _yyRuleInfo( 147, 2 ),
new _yyRuleInfo( 147, 3 ),
new _yyRuleInfo( 147, 5 ),
new _yyRuleInfo( 147, 2 ),
new _yyRuleInfo( 152, 6 ),
new _yyRuleInfo( 154, 1 ),
new _yyRuleInfo( 156, 0 ),
new _yyRuleInfo( 156, 3 ),
new _yyRuleInfo( 155, 1 ),
new _yyRuleInfo( 155, 0 ),
new _yyRuleInfo( 153, 4 ),
new _yyRuleInfo( 153, 2 ),
new _yyRuleInfo( 158, 3 ),
new _yyRuleInfo( 158, 1 ),
new _yyRuleInfo( 161, 3 ),
new _yyRuleInfo( 162, 1 ),
new _yyRuleInfo( 165, 1 ),
new _yyRuleInfo( 165, 1 ),
new _yyRuleInfo( 166, 1 ),
new _yyRuleInfo( 150, 1 ),
new _yyRuleInfo( 150, 1 ),
new _yyRuleInfo( 150, 1 ),
new _yyRuleInfo( 163, 0 ),
new _yyRuleInfo( 163, 1 ),
new _yyRuleInfo( 167, 1 ),
new _yyRuleInfo( 167, 4 ),
new _yyRuleInfo( 167, 6 ),
new _yyRuleInfo( 168, 1 ),
new _yyRuleInfo( 168, 2 ),
new _yyRuleInfo( 169, 1 ),
new _yyRuleInfo( 169, 1 ),
new _yyRuleInfo( 164, 2 ),
new _yyRuleInfo( 164, 0 ),
new _yyRuleInfo( 172, 3 ),
new _yyRuleInfo( 172, 1 ),
new _yyRuleInfo( 173, 2 ),
new _yyRuleInfo( 173, 4 ),
new _yyRuleInfo( 173, 3 ),
new _yyRuleInfo( 173, 3 ),
new _yyRuleInfo( 173, 2 ),
new _yyRuleInfo( 173, 2 ),
new _yyRuleInfo( 173, 3 ),
new _yyRuleInfo( 173, 5 ),
new _yyRuleInfo( 173, 2 ),
new _yyRuleInfo( 173, 4 ),
new _yyRuleInfo( 173, 4 ),
new _yyRuleInfo( 173, 1 ),
new _yyRuleInfo( 173, 2 ),
new _yyRuleInfo( 178, 0 ),
new _yyRuleInfo( 178, 1 ),
new _yyRuleInfo( 180, 0 ),
new _yyRuleInfo( 180, 2 ),
new _yyRuleInfo( 182, 2 ),
new _yyRuleInfo( 182, 3 ),
new _yyRuleInfo( 182, 3 ),
new _yyRuleInfo( 182, 3 ),
new _yyRuleInfo( 183, 2 ),
new _yyRuleInfo( 183, 2 ),
new _yyRuleInfo( 183, 1 ),
new _yyRuleInfo( 183, 1 ),
new _yyRuleInfo( 183, 2 ),
new _yyRuleInfo( 181, 3 ),
new _yyRuleInfo( 181, 2 ),
new _yyRuleInfo( 184, 0 ),
new _yyRuleInfo( 184, 2 ),
new _yyRuleInfo( 184, 2 ),
new _yyRuleInfo( 159, 0 ),
new _yyRuleInfo( 159, 2 ),
new _yyRuleInfo( 185, 3 ),
new _yyRuleInfo( 185, 2 ),
new _yyRuleInfo( 185, 1 ),
new _yyRuleInfo( 186, 2 ),
new _yyRuleInfo( 186, 7 ),
new _yyRuleInfo( 186, 5 ),
new _yyRuleInfo( 186, 5 ),
new _yyRuleInfo( 186, 10 ),
new _yyRuleInfo( 188, 0 ),
new _yyRuleInfo( 188, 1 ),
new _yyRuleInfo( 176, 0 ),
new _yyRuleInfo( 176, 3 ),
new _yyRuleInfo( 189, 0 ),
new _yyRuleInfo( 189, 2 ),
new _yyRuleInfo( 190, 1 ),
new _yyRuleInfo( 190, 1 ),
new _yyRuleInfo( 190, 1 ),
new _yyRuleInfo( 147, 4 ),
new _yyRuleInfo( 192, 2 ),
new _yyRuleInfo( 192, 0 ),
new _yyRuleInfo( 147, 8 ),
new _yyRuleInfo( 147, 4 ),
new _yyRuleInfo( 147, 1 ),
new _yyRuleInfo( 160, 1 ),
new _yyRuleInfo( 160, 3 ),
new _yyRuleInfo( 195, 1 ),
new _yyRuleInfo( 195, 2 ),
new _yyRuleInfo( 195, 1 ),
new _yyRuleInfo( 194, 9 ),
new _yyRuleInfo( 196, 1 ),
new _yyRuleInfo( 196, 1 ),
new _yyRuleInfo( 196, 0 ),
new _yyRuleInfo( 204, 2 ),
new _yyRuleInfo( 204, 0 ),
new _yyRuleInfo( 197, 3 ),
new _yyRuleInfo( 197, 2 ),
new _yyRuleInfo( 197, 4 ),
new _yyRuleInfo( 205, 2 ),
new _yyRuleInfo( 205, 1 ),
new _yyRuleInfo( 205, 0 ),
new _yyRuleInfo( 198, 0 ),
new _yyRuleInfo( 198, 2 ),
new _yyRuleInfo( 207, 2 ),
new _yyRuleInfo( 207, 0 ),
new _yyRuleInfo( 206, 7 ),
new _yyRuleInfo( 206, 7 ),
new _yyRuleInfo( 206, 7 ),
new _yyRuleInfo( 157, 0 ),
new _yyRuleInfo( 157, 2 ),
new _yyRuleInfo( 193, 2 ),
new _yyRuleInfo( 208, 1 ),
new _yyRuleInfo( 208, 2 ),
new _yyRuleInfo( 208, 3 ),
new _yyRuleInfo( 208, 4 ),
new _yyRuleInfo( 210, 2 ),
new _yyRuleInfo( 210, 0 ),
new _yyRuleInfo( 209, 0 ),
new _yyRuleInfo( 209, 3 ),
new _yyRuleInfo( 209, 2 ),
new _yyRuleInfo( 211, 4 ),
new _yyRuleInfo( 211, 0 ),
new _yyRuleInfo( 202, 0 ),
new _yyRuleInfo( 202, 3 ),
new _yyRuleInfo( 214, 4 ),
new _yyRuleInfo( 214, 2 ),
new _yyRuleInfo( 215, 1 ),
new _yyRuleInfo( 177, 1 ),
new _yyRuleInfo( 177, 1 ),
new _yyRuleInfo( 177, 0 ),
new _yyRuleInfo( 200, 0 ),
new _yyRuleInfo( 200, 3 ),
new _yyRuleInfo( 201, 0 ),
new _yyRuleInfo( 201, 2 ),
new _yyRuleInfo( 203, 0 ),
new _yyRuleInfo( 203, 2 ),
new _yyRuleInfo( 203, 4 ),
new _yyRuleInfo( 203, 4 ),
new _yyRuleInfo( 147, 5 ),
new _yyRuleInfo( 199, 0 ),
new _yyRuleInfo( 199, 2 ),
new _yyRuleInfo( 147, 7 ),
new _yyRuleInfo( 217, 5 ),
new _yyRuleInfo( 217, 3 ),
new _yyRuleInfo( 147, 8 ),
new _yyRuleInfo( 147, 5 ),
new _yyRuleInfo( 147, 6 ),
new _yyRuleInfo( 218, 2 ),
new _yyRuleInfo( 218, 1 ),
new _yyRuleInfo( 220, 3 ),
new _yyRuleInfo( 220, 1 ),
new _yyRuleInfo( 219, 0 ),
new _yyRuleInfo( 219, 3 ),
new _yyRuleInfo( 213, 3 ),
new _yyRuleInfo( 213, 1 ),
new _yyRuleInfo( 175, 1 ),
new _yyRuleInfo( 175, 3 ),
new _yyRuleInfo( 174, 1 ),
new _yyRuleInfo( 175, 1 ),
new _yyRuleInfo( 175, 1 ),
new _yyRuleInfo( 175, 3 ),
new _yyRuleInfo( 175, 5 ),
new _yyRuleInfo( 174, 1 ),
new _yyRuleInfo( 174, 1 ),
new _yyRuleInfo( 175, 1 ),
new _yyRuleInfo( 175, 1 ),
new _yyRuleInfo( 175, 3 ),
new _yyRuleInfo( 175, 6 ),
new _yyRuleInfo( 175, 5 ),
new _yyRuleInfo( 175, 4 ),
new _yyRuleInfo( 174, 1 ),
new _yyRuleInfo( 175, 3 ),
new _yyRuleInfo( 175, 3 ),
new _yyRuleInfo( 175, 3 ),
new _yyRuleInfo( 175, 3 ),
new _yyRuleInfo( 175, 3 ),
new _yyRuleInfo( 175, 3 ),
new _yyRuleInfo( 175, 3 ),
new _yyRuleInfo( 175, 3 ),
new _yyRuleInfo( 222, 1 ),
new _yyRuleInfo( 222, 2 ),
new _yyRuleInfo( 222, 1 ),
new _yyRuleInfo( 222, 2 ),
new _yyRuleInfo( 223, 2 ),
new _yyRuleInfo( 223, 0 ),
new _yyRuleInfo( 175, 4 ),
new _yyRuleInfo( 175, 2 ),
new _yyRuleInfo( 175, 3 ),
new _yyRuleInfo( 175, 3 ),
new _yyRuleInfo( 175, 4 ),
new _yyRuleInfo( 175, 2 ),
new _yyRuleInfo( 175, 2 ),
new _yyRuleInfo( 175, 2 ),
new _yyRuleInfo( 175, 2 ),
new _yyRuleInfo( 224, 1 ),
new _yyRuleInfo( 224, 2 ),
new _yyRuleInfo( 175, 5 ),
new _yyRuleInfo( 225, 1 ),
new _yyRuleInfo( 225, 2 ),
new _yyRuleInfo( 175, 5 ),
new _yyRuleInfo( 175, 3 ),
new _yyRuleInfo( 175, 5 ),
new _yyRuleInfo( 175, 4 ),
new _yyRuleInfo( 175, 4 ),
new _yyRuleInfo( 175, 5 ),
new _yyRuleInfo( 227, 5 ),
new _yyRuleInfo( 227, 4 ),
new _yyRuleInfo( 228, 2 ),
new _yyRuleInfo( 228, 0 ),
new _yyRuleInfo( 226, 1 ),
new _yyRuleInfo( 226, 0 ),
new _yyRuleInfo( 221, 1 ),
new _yyRuleInfo( 221, 0 ),
new _yyRuleInfo( 216, 3 ),
new _yyRuleInfo( 216, 1 ),
new _yyRuleInfo( 147, 11 ),
new _yyRuleInfo( 229, 1 ),
new _yyRuleInfo( 229, 0 ),
new _yyRuleInfo( 179, 0 ),
new _yyRuleInfo( 179, 3 ),
new _yyRuleInfo( 187, 5 ),
new _yyRuleInfo( 187, 3 ),
new _yyRuleInfo( 230, 0 ),
new _yyRuleInfo( 230, 2 ),
new _yyRuleInfo( 147, 4 ),
new _yyRuleInfo( 147, 1 ),
new _yyRuleInfo( 147, 2 ),
new _yyRuleInfo( 147, 3 ),
new _yyRuleInfo( 147, 5 ),
new _yyRuleInfo( 147, 6 ),
new _yyRuleInfo( 147, 5 ),
new _yyRuleInfo( 147, 6 ),
new _yyRuleInfo( 231, 1 ),
new _yyRuleInfo( 231, 1 ),
new _yyRuleInfo( 231, 1 ),
new _yyRuleInfo( 231, 1 ),
new _yyRuleInfo( 231, 1 ),
new _yyRuleInfo( 170, 2 ),
new _yyRuleInfo( 171, 2 ),
new _yyRuleInfo( 233, 1 ),
new _yyRuleInfo( 232, 1 ),
new _yyRuleInfo( 232, 0 ),
new _yyRuleInfo( 147, 5 ),
new _yyRuleInfo( 234, 11 ),
new _yyRuleInfo( 236, 1 ),
new _yyRuleInfo( 236, 1 ),
new _yyRuleInfo( 236, 2 ),
new _yyRuleInfo( 236, 0 ),
new _yyRuleInfo( 237, 1 ),
new _yyRuleInfo( 237, 1 ),
new _yyRuleInfo( 237, 3 ),
new _yyRuleInfo( 238, 0 ),
new _yyRuleInfo( 238, 3 ),
new _yyRuleInfo( 239, 0 ),
new _yyRuleInfo( 239, 2 ),
new _yyRuleInfo( 235, 3 ),
new _yyRuleInfo( 235, 2 ),
new _yyRuleInfo( 241, 1 ),
new _yyRuleInfo( 241, 3 ),
new _yyRuleInfo( 242, 0 ),
new _yyRuleInfo( 242, 3 ),
new _yyRuleInfo( 242, 2 ),
new _yyRuleInfo( 240, 7 ),
new _yyRuleInfo( 240, 8 ),
new _yyRuleInfo( 240, 5 ),
new _yyRuleInfo( 240, 5 ),
new _yyRuleInfo( 240, 1 ),
new _yyRuleInfo( 175, 4 ),
new _yyRuleInfo( 175, 6 ),
new _yyRuleInfo( 191, 1 ),
new _yyRuleInfo( 191, 1 ),
new _yyRuleInfo( 191, 1 ),
new _yyRuleInfo( 147, 4 ),
new _yyRuleInfo( 147, 6 ),
new _yyRuleInfo( 147, 3 ),
new _yyRuleInfo( 244, 0 ),
new _yyRuleInfo( 244, 2 ),
new _yyRuleInfo( 243, 1 ),
new _yyRuleInfo( 243, 0 ),
new _yyRuleInfo( 147, 1 ),
new _yyRuleInfo( 147, 3 ),
new _yyRuleInfo( 147, 1 ),
new _yyRuleInfo( 147, 3 ),
new _yyRuleInfo( 147, 6 ),
new _yyRuleInfo( 147, 6 ),
new _yyRuleInfo( 245, 1 ),
new _yyRuleInfo( 246, 0 ),
new _yyRuleInfo( 246, 1 ),
new _yyRuleInfo( 147, 1 ),
new _yyRuleInfo( 147, 4 ),
new _yyRuleInfo( 247, 7 ),
new _yyRuleInfo( 248, 1 ),
new _yyRuleInfo( 248, 3 ),
new _yyRuleInfo( 249, 0 ),
new _yyRuleInfo( 249, 2 ),
new _yyRuleInfo( 250, 1 ),
new _yyRuleInfo( 250, 3 ),
new _yyRuleInfo( 251, 1 ),
new _yyRuleInfo( 252, 0 ),
new _yyRuleInfo( 252, 4 ),
new _yyRuleInfo( 252, 2 ),
};

    //static void yy_accept(yyParser*);  /* Forward Declaration */

    /*
    ** Perform a reduce action and the shift that must immediately
    ** follow the reduce.
    */
    static void yy_reduce(
    yyParser yypParser,         /* The parser */
    int yyruleno                 /* Number of the rule by which to reduce */
    )
    {
      int yygoto;                     /* The next state */
      int yyact;                      /* The next action */
      YYMINORTYPE yygotominor;        /* The LHS of the rule reduced */
      yymsp yymsp; // yyStackEntry[] yymsp = new yyStackEntry[0];            /* The top of the parser's stack */
      int yysize;                     /* Amount to pop the stack */
      Parse pParse = yypParser.pParse; //sqlite3ParserARG_FETCH;

      yymsp = new yymsp( ref yypParser, yypParser.yyidx ); //      yymsp[0] = yypParser.yystack[yypParser.yyidx];
#if !NDEBUG
      if ( yyTraceFILE != null && yyruleno >= 0
      && yyruleno < yyRuleName.Length )
      { //(int)(yyRuleName.Length/sizeof(yyRuleName[0])) ){
        fprintf( yyTraceFILE, "%sReduce [%s].\n", yyTracePrompt,
        yyRuleName[yyruleno] );
      }
#endif // * NDEBUG */

      /* Silence complaints from purify about yygotominor being uninitialized
** in some cases when it is copied into the stack after the following
** switch.  yygotominor is uninitialized when a rule reduces that does
** not set the value of its left-hand side nonterminal.  Leaving the
** value of the nonterminal uninitialized is utterly harmless as long
** as the value is never used.  So really the only thing this code
** accomplishes is to quieten purify.
**
** 2007-01-16:  The wireshark project (www.wireshark.org) reports that
** without this code, their parser segfaults.  I'm not sure what there
** parser is doing to make this happen.  This is the second bug report
** from wireshark this week.  Clearly they are stressing Lemon in ways
** that it has not been previously stressed...  (SQLite ticket #2172)
*/
      yygotominor = new YYMINORTYPE(); //memset(yygotominor, 0, yygotominor).Length;
      switch ( yyruleno )
      {
        /* Beginning here are the reduction cases.  A typical example
        ** follows:
        **   case 0:
        **  //#line <lineno> <grammarfile>
        **     { ... }           // User supplied code
        **  //#line <lineno> <thisfile>
        **     break;
        */
        case 5: /* explain ::= */
          //#line 107 "parse.y"
          { sqlite3BeginParse( pParse, 0 ); }
          //#line 2110 "parse.c"
          break;
        case 6: /* explain ::= EXPLAIN */
          //#line 109 "parse.y"
          { sqlite3BeginParse( pParse, 1 ); }
          //#line 2115 "parse.c"
          break;
        case 7: /* explain ::= EXPLAIN QUERY PLAN */
          //#line 110 "parse.y"
          { sqlite3BeginParse( pParse, 2 ); }
          //#line 2120 "parse.c"
          break;
        case 8: /* cmdx ::= cmd */
          //#line 112 "parse.y"
          { sqlite3FinishCoding( pParse ); }
          //#line 2125 "parse.c"
          break;
        case 9: /* cmd ::= BEGIN transtype trans_opt */
          //#line 117 "parse.y"
          { sqlite3BeginTransaction( pParse, yymsp[-1].minor.yy328 ); }
          //#line 2130 "parse.c"
          break;
        case 13: /* transtype ::= */
          //#line 122 "parse.y"
          { yygotominor.yy328 = TK_DEFERRED; }
          //#line 2135 "parse.c"
          break;
        case 14: /* transtype ::= DEFERRED */
        case 15: /* transtype ::= IMMEDIATE */ //yytestcase(yyruleno==15);
        case 16: /* transtype ::= EXCLUSIVE */ //yytestcase(yyruleno==16);
        case 115: /* multiselect_op ::= UNION */ //yytestcase(yyruleno==114);
        case 117: /* multiselect_op ::= EXCEPT|INTERSECT */ //yytestcase(yyruleno==116);
          //#line 123 "parse.y"
          { yygotominor.yy328 = yymsp[0].major; }
          //#line 2144 "parse.c"
          break;
        case 17: /* cmd ::= COMMIT trans_opt */
        case 18: /* cmd ::= END trans_opt */ //yytestcase(yyruleno==18);
          //#line 126 "parse.y"
          { sqlite3CommitTransaction( pParse ); }
          //#line 2150 "parse.c"
          break;
        case 19: /* cmd ::= ROLLBACK trans_opt */
          //#line 128 "parse.y"
          { sqlite3RollbackTransaction( pParse ); }
          //#line 2155 "parse.c"
          break;
        case 22: /* cmd ::= SAVEPOINT nm */
          //#line 132 "parse.y"
          {
            sqlite3Savepoint( pParse, SAVEPOINT_BEGIN, yymsp[0].minor.yy0 );
          }
          //#line 2162 "parse.c"
          break;
        case 23: /* cmd ::= RELEASE savepoint_opt nm */
          //#line 135 "parse.y"
          {
            sqlite3Savepoint( pParse, SAVEPOINT_RELEASE, yymsp[0].minor.yy0 );
          }
          //#line 2169 "parse.c"
          break;
        case 24: /* cmd ::= ROLLBACK trans_opt TO savepoint_opt nm */
          //#line 138 "parse.y"
          {
            sqlite3Savepoint( pParse, SAVEPOINT_ROLLBACK, yymsp[0].minor.yy0 );
          }
          //#line 2176 "parse.c"
          break;
        case 26: /* create_table ::= createkw temp TABLE ifnotexists nm dbnm */
          //#line 145 "parse.y"
          {
            sqlite3StartTable( pParse, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, yymsp[-4].minor.yy328, 0, 0, yymsp[-2].minor.yy328 );
          }
          //#line 2183 "parse.c"
          break;
        case 27: /* createkw ::= CREATE */
          //#line 148 "parse.y"
          {
            pParse.db.lookaside.bEnabled = 0;
            yygotominor.yy0 = yymsp[0].minor.yy0;
          }
          //#line 2191 "parse.c"
          break;
        case 28: /* ifnotexists ::= */
        case 31: /* temp ::= */ //yytestcase(yyruleno == 31);
        case 70: /* autoinc ::= */ //yytestcase(yyruleno == 70);
        case 83: /* defer_subclause ::= NOT DEFERRABLE init_deferred_pred_opt */ //yytestcase(yyruleno == 83);
        case 85: /* init_deferred_pred_opt ::= */ //yytestcase(yyruleno == 85);
        case 87: /* init_deferred_pred_opt ::= INITIALLY IMMEDIATE */ //yytestcase(yyruleno == 87);
        case 98: /* defer_subclause_opt ::= */ //yytestcase(yyruleno == 98);
        case 109: /* ifexists ::= */ //yytestcase(yyruleno == 109);
        case 120: /* distinct ::= ALL */ //yytestcase(yyruleno == 120);
        case 121: /* distinct ::= */ //yytestcase(yyruleno == 121);
        case 223: /* between_op ::= BETWEEN */ //yytestcase(yyruleno == 223);
        case 226: /* in_op ::= IN */ //yytestcase(yyruleno == 226);
          //#line 153 "parse.y"
          { yygotominor.yy328 = 0; }
          //#line 2207 "parse.c"
          break;
        case 29: /* ifnotexists ::= IF NOT EXISTS */
        case 30: /* temp ::= TEMP */ //yytestcase(yyruleno == 30);
        case 71: /* autoinc ::= AUTOINCR */ //yytestcase(yyruleno == 71);
        case 86: /* init_deferred_pred_opt ::= INITIALLY DEFERRED */ //yytestcase(yyruleno == 86);
        case 108: /* ifexists ::= IF EXISTS */ //yytestcase(yyruleno == 108);
        case 119: /* distinct ::= DISTINCT */ //yytestcase(yyruleno == 119);
        case 224: /* between_op ::= NOT BETWEEN */ //yytestcase(yyruleno == 224);
        case 227: /* in_op ::= NOT IN */ //yytestcase(yyruleno == 227);
          //#line 154 "parse.y"
          { yygotominor.yy328 = 1; }
          //#line 2219 "parse.c"
          break;
        case 32: /* create_table_args ::= LP columnlist conslist_opt RP */
          //#line 160 "parse.y"
          {
            sqlite3EndTable( pParse, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, 0 );
          }
          //#line 2226 "parse.c"
          break;
        case 33: /* create_table_args ::= AS select */
          //#line 163 "parse.y"
          {
            sqlite3EndTable( pParse, 0, 0, yymsp[0].minor.yy3 );
            sqlite3SelectDelete( pParse.db, ref yymsp[0].minor.yy3 );
          }
          //#line 2234 "parse.c"
          break;
        case 36: /* column ::= columnid type carglist */
          //#line 175 "parse.y"
          {
            //yygotominor.yy0.z = yymsp[-2].minor.yy0.z;
            //yygotominor.yy0.n = (int)( pParse->sLastToken.z - yymsp[-2].minor.yy0.z ) + pParse->sLastToken.n; 
            yygotominor.yy0.n = (int)( yymsp[-2].minor.yy0.z.Length - pParse.sLastToken.z.Length ) + pParse.sLastToken.n;
            yygotominor.yy0.z = yymsp[-2].minor.yy0.z.Substring( 0, yygotominor.yy0.n );
          }
          //#line 2242 "parse.c"
          break;
        case 37: /* columnid ::= nm */
          //#line 179 "parse.y"
          {
            sqlite3AddColumn( pParse, yymsp[0].minor.yy0 );
            yygotominor.yy0 = yymsp[0].minor.yy0;
          }
          //#line 2250 "parse.c"
          break;
        case 38: /* id ::= ID */
        case 39: /* id ::= INDEXED */ //yytestcase(yyruleno==39);
        case 40: /* ids ::= ID|STRING */ //yytestcase(yyruleno==40);
        case 41: /* nm ::= id */ //yytestcase(yyruleno==41);
        case 42: /* nm ::= STRING */ //yytestcase(yyruleno==42);
        case 43: /* nm ::= JOIN_KW */ //yytestcase(yyruleno==43);
        case 46: /* typetoken ::= typename */ //yytestcase(yyruleno==46);
        case 49: /* typename ::= ids */ //yytestcase(yyruleno==49);
        case 127: /* as ::= AS nm */ ////yytestcase(yyruleno == 127);
        case 128: /* as ::= ids */ ////yytestcase(yyruleno == 128);
        case 138: /* dbnm ::= DOT nm */ ////yytestcase(yyruleno == 138);
        case 147: /* indexed_opt ::= INDEXED BY nm */ ////yytestcase(yyruleno == 147);
        case 252: /* collate ::= COLLATE ids */ ////yytestcase(yyruleno == 252);
        case 261: /* nmnum ::= plus_num */ ////yytestcase(yyruleno == 261);
        case 262: /* nmnum ::= nm */ ////yytestcase(yyruleno == 262);
        case 263: /* nmnum ::= ON */ ////yytestcase(yyruleno == 263);
        case 264: /* nmnum ::= DELETE */ ////yytestcase(yyruleno == 264);
        case 265: /* nmnum ::= DEFAULT */ ////yytestcase(yyruleno == 265);
        case 266: /* plus_num ::= plus_opt number */ ////yytestcase(yyruleno == 266);
        case 267: /* minus_num ::= MINUS number */ ////yytestcase(yyruleno == 267);
        case 268: /* number ::= INTEGER|FLOAT */ ////yytestcase(yyruleno == 268);
        case 286: /* trnm ::= nm */ ////yytestcase(yyruleno == 286);
          //#line 189 "parse.y"
          { yygotominor.yy0 = yymsp[0].minor.yy0; }
          //#line 2276 "parse.c"
          break;
        case 45: /* type ::= typetoken */
          //#line 251 "parse.y"
          { sqlite3AddColumnType( pParse, yymsp[0].minor.yy0 ); }
          //#line 2281 "parse.c"
          break;
        case 47: /* typetoken ::= typename LP signed RP */
          //#line 253 "parse.y"
          {
            //yygotominor.yy0.z = yymsp[-3].minor.yy0.z;
            //yygotominor.yy0.n = (int)( &yymsp[0].minor.yy0.z[yymsp[0].minor.yy0.n] - yymsp[-3].minor.yy0.z );
            yygotominor.yy0.n = yymsp[-3].minor.yy0.z.Length - yymsp[0].minor.yy0.z.Length + yymsp[0].minor.yy0.n;
            yygotominor.yy0.z = yymsp[-3].minor.yy0.z.Substring( 0, yygotominor.yy0.n );
          }
          //#line 2289 "parse.c"
          break;
        case 48: /* typetoken ::= typename LP signed COMMA signed RP */
          //#line 257 "parse.y"
          {
            //yygotominor.yy0.z = yymsp[-5].minor.yy0.z;
            //yygotominor.yy0.n = (int)( &yymsp[0].minor.yy0.z[yymsp[0].minor.yy0.n] - yymsp[-5].minor.yy0.z );
            yygotominor.yy0.n = yymsp[-5].minor.yy0.z.Length - yymsp[0].minor.yy0.z.Length + 1;
            yygotominor.yy0.z = yymsp[-5].minor.yy0.z.Substring( 0, yygotominor.yy0.n );
          }
          //#line 2297 "parse.c"
          break;
        case 50: /* typename ::= typename ids */
          //#line 263 "parse.y"
          {
            //yygotominor.yy0.z=yymsp[-1].minor.yy0.z; yygotominor.yy0.n=yymsp[0].minor.yy0.n+(int)(yymsp[0].minor.yy0.z-yymsp[-1].minor.yy0.z);
            yygotominor.yy0.z = yymsp[-1].minor.yy0.z;
            yygotominor.yy0.n = yymsp[0].minor.yy0.n + (int)( yymsp[-1].minor.yy0.z.Length - yymsp[0].minor.yy0.z.Length );
          }
          //#line 2202 "parse.c"
          break;
        case 57: /* ccons ::= DEFAULT term */
        case 59: /* ccons ::= DEFAULT PLUS term */ //yytestcase(yyruleno==59);
          //#line 274 "parse.y"
          { sqlite3AddDefaultValue( pParse, yymsp[0].minor.yy346 ); }
          //#line 2308 "parse.c"
          break;
        case 58: /* ccons ::= DEFAULT LP expr RP */
          //#line 275 "parse.y"
          { sqlite3AddDefaultValue( pParse, yymsp[-1].minor.yy346 ); }
          //#line 2313 "parse.c"
          break;
        case 60: /* ccons ::= DEFAULT MINUS term */
          //#line 277 "parse.y"
          {
            ExprSpan v = new ExprSpan();
            v.pExpr = sqlite3PExpr( pParse, TK_UMINUS, yymsp[0].minor.yy346.pExpr, 0, 0 );
            v.zStart = yymsp[-1].minor.yy0.z;
            v.zEnd = yymsp[0].minor.yy346.zEnd;
            sqlite3AddDefaultValue( pParse, v );
          }
          //#line 2324 "parse.c"
          break;
        case 61: /* ccons ::= DEFAULT id */
          //#line 284 "parse.y"
          {
            ExprSpan v = new ExprSpan();
            spanExpr( v, pParse, TK_STRING, yymsp[0].minor.yy0 );
            sqlite3AddDefaultValue( pParse, v );
          }
          //#line 2333"parse.c"
          break;
        case 63: /* ccons ::= NOT NULL onconf */
          //#line 294 "parse.y"
          { sqlite3AddNotNull( pParse, yymsp[0].minor.yy328 ); }
          //#line 2338 "parse.c"
          break;
        case 64: /* ccons ::= PRIMARY KEY sortorder onconf autoinc */
          //#line 296 "parse.y"
          { sqlite3AddPrimaryKey( pParse, 0, yymsp[-1].minor.yy328, yymsp[0].minor.yy328, yymsp[-2].minor.yy328 ); }
          //#line 2343 "parse.c"
          break;
        case 65: /* ccons ::= UNIQUE onconf */
          //#line 297 "parse.y"
          { sqlite3CreateIndex( pParse, 0, 0, 0, 0, yymsp[0].minor.yy328, 0, 0, 0, 0 ); }
          //#line 2348 "parse.c"
          break;
        case 66: /* ccons ::= CHECK LP expr RP */
          //#line 298 "parse.y"
          { sqlite3AddCheckConstraint( pParse, yymsp[-1].minor.yy346.pExpr ); }
          //#line 2353 "parse.c"
          break;
        case 67: /* ccons ::= REFERENCES nm idxlist_opt refargs */
          //#line 300 "parse.y"
          { sqlite3CreateForeignKey( pParse, 0, yymsp[-2].minor.yy0, yymsp[-1].minor.yy14, yymsp[0].minor.yy328 ); }
          //#line 2358 "parse.c"
          break;
        case 68: /* ccons ::= defer_subclause */
          //#line 301 "parse.y"
          { sqlite3DeferForeignKey( pParse, yymsp[0].minor.yy328 ); }
          //#line 2363 "parse.c"
          break;
        case 69: /* ccons ::= COLLATE ids */
          //#line 302 "parse.y"
          { sqlite3AddCollateType( pParse, yymsp[0].minor.yy0 ); }
          //#line 2368 "parse.c"
          break;
        case 72: /* refargs ::= */
          //#line 315 "parse.y"
          { yygotominor.yy328 = OE_None * 0x0101; /* EV: R-19803-45884 */}
          //#line 2373 "parse.c"
          break;
        case 73: /* refargs ::= refargs refarg */
          //#line 316 "parse.y"
          { yygotominor.yy328 = ( yymsp[-1].minor.yy328 & ~yymsp[0].minor.yy429.mask ) | yymsp[0].minor.yy429.value; }
          //#line 2378 "parse.c"
          break;
        case 74: /* refarg ::= MATCH nm */
        case 75: /* refarg ::= ON INSERT refact */ //yytestcase(yyruleno == 75);
          //#line 318 "parse.y"
          { yygotominor.yy429.value = 0; yygotominor.yy429.mask = 0x000000; }
          //#line 2384 "parse.c"
          break;
        case 76: /* refarg ::= ON DELETE refact */
          //#line 320 "parse.y"
          { yygotominor.yy429.value = yymsp[0].minor.yy328; yygotominor.yy429.mask = 0x0000ff; }
          //#line 2389 "parse.c"
          break;
        case 77: /* refarg ::= ON UPDATE refact */
          //#line 321 "parse.y"
          { yygotominor.yy429.value = yymsp[0].minor.yy328 << 8; yygotominor.yy429.mask = 0x00ff00; }
          //#line 2394 "parse.c"
          break;
        case 78: /* refact ::= SET NULL */
          //#line 323 "parse.y"
          { yygotominor.yy328 = OE_SetNull;  /* EV: R-33326-45252 */}
          //#line 2399 "parse.c"
          break;
        case 79: /* refact ::= SET DEFAULT */
          //#line 324 "parse.y"
          { yygotominor.yy328 = OE_SetDflt;  /* EV: R-33326-45252 */}
          //#line 2404 "parse.c"
          break;
        case 80: /* refact ::= CASCADE */
          //#line 325 "parse.y"
          { yygotominor.yy328 = OE_Cascade;  /* EV: R-33326-45252 */}
          //#line 2409 "parse.c"
          break;
        case 81: /* refact ::= RESTRICT */
          //#line 326 "parse.y"
          { yygotominor.yy328 = OE_Restrict; /* EV: R-33326-45252 */}
          //#line 2414 "parse.c"
          break;
        case 82: /* refact ::= NO ACTION */
          //#line 327 "parse.y"
          { yygotominor.yy328 = OE_None;     /* EV: R-33326-45252 */}
          //#line 2419 "parse.c"
          break;
        case 84: /* defer_subclause ::= DEFERRABLE init_deferred_pred_opt */
        case 99: /* defer_subclause_opt ::= defer_subclause */ //yytestcase(yyruleno==99);
        case 101: /* onconf ::= ON CONFLICT resolvetype */ //yytestcase(yyruleno==101);
        case 104: /* resolvetype ::= raisetype */ //yytestcase(yyruleno==104);
          //#line 330 "parse.y"
          { yygotominor.yy328 = yymsp[0].minor.yy328; }
          //#line 2427 "parse.c"
          break;
        case 88: /* conslist_opt ::= */
          //#line 339 "parse.y"
          { yygotominor.yy0.n = 0; yygotominor.yy0.z = null; }
          //#line 2432 "parse.c"
          break;
        case 89: /* conslist_opt ::= COMMA conslist */
          //#line 340 "parse.y"
          { yygotominor.yy0 = yymsp[-1].minor.yy0; }
          //#line 2437 "parse.c"
          break;
        case 94: /* tcons ::= PRIMARY KEY LP idxlist autoinc RP onconf */
          //#line 346 "parse.y"
          { sqlite3AddPrimaryKey( pParse, yymsp[-3].minor.yy14, yymsp[0].minor.yy328, yymsp[-2].minor.yy328, 0 ); }
          //#line 2442 "parse.c"
          break;
        case 95: /* tcons ::= UNIQUE LP idxlist RP onconf */
          //#line 348 "parse.y"
          { sqlite3CreateIndex( pParse, 0, 0, 0, yymsp[-2].minor.yy14, yymsp[0].minor.yy328, 0, 0, 0, 0 ); }
          //#line 2447 "parse.c"
          break;
        case 96: /* tcons ::= CHECK LP expr RP onconf */
          //#line 350 "parse.y"
          { sqlite3AddCheckConstraint( pParse, yymsp[-2].minor.yy346.pExpr ); }
          //#line 2452 "parse.c"
          break;
        case 97: /* tcons ::= FOREIGN KEY LP idxlist RP REFERENCES nm idxlist_opt refargs defer_subclause_opt */
          //#line 352 "parse.y"
          {
            sqlite3CreateForeignKey( pParse, yymsp[-6].minor.yy14, yymsp[-3].minor.yy0, yymsp[-2].minor.yy14, yymsp[-1].minor.yy328 );
            sqlite3DeferForeignKey( pParse, yymsp[0].minor.yy328 );
          }
          //#line 2460 "parse.c"
          break;
        case 100: /* onconf ::= */
          //#line 366 "parse.y"
          { yygotominor.yy328 = OE_Default; }
          //#line 2465 "parse.c"
          break;
        case 102: /* orconf ::= */
          //#line 368 "parse.y"
          { yygotominor.yy186 = OE_Default; }
          //#line 2470 "parse.c"
          break;
        case 103: /* orconf ::= OR resolvetype */
          //#line 369 "parse.y"
          { yygotominor.yy186 = (u8)yymsp[0].minor.yy328; }
          //#line 2475 "parse.c"
          break;
        case 105: /* resolvetype ::= IGNORE */
          //#line 371 "parse.y"
          { yygotominor.yy328 = OE_Ignore; }
          //#line 2480 "parse.c"
          break;
        case 106: /* resolvetype ::= REPLACE */
          //#line 372 "parse.y"
          { yygotominor.yy328 = OE_Replace; }
          //#line 2485 "parse.c"
          break;
        case 107: /* cmd ::= DROP TABLE ifexists fullname */
          //#line 376 "parse.y"
          {
            sqlite3DropTable( pParse, yymsp[0].minor.yy65, 0, yymsp[-1].minor.yy328 );
          }
          //#line 2492 "parse.c"
          break;
        case 110: /* cmd ::= createkw temp VIEW ifnotexists nm dbnm AS select */
          //#line 386 "parse.y"
          {
            sqlite3CreateView( pParse, yymsp[-7].minor.yy0, yymsp[-3].minor.yy0, yymsp[-2].minor.yy0, yymsp[0].minor.yy3, yymsp[-6].minor.yy328, yymsp[-4].minor.yy328 );
          }
          //#line 2499 "parse.c"
          break;
        case 111: /* cmd ::= DROP VIEW ifexists fullname */
          //#line 389 "parse.y"
          {
            sqlite3DropTable( pParse, yymsp[0].minor.yy65, 1, yymsp[-1].minor.yy328 );
          }
          //#line 2506 "parse.c"
          break;
        case 112: /* cmd ::= select */
          //#line 396 "parse.y"
          {
            SelectDest dest = new SelectDest( SRT_Output, '\0', 0, 0, 0 );
            sqlite3Select( pParse, yymsp[0].minor.yy3, ref dest );
            sqlite3SelectDelete( pParse.db, ref yymsp[0].minor.yy3 );
          }
          //#line 2515 "parse.c"
          break;
        case 113: /* select ::= oneselect */
          //#line 407 "parse.y"
          { yygotominor.yy3 = yymsp[0].minor.yy3; }
          //#line 2520 "parse.c"
          break;
        case 114: /* select ::= select multiselect_op oneselect */
          //#line 409 "parse.y"
          {
            if ( yymsp[0].minor.yy3 != null )
            {
              yymsp[0].minor.yy3.op = (u8)yymsp[-1].minor.yy328;
              yymsp[0].minor.yy3.pPrior = yymsp[-2].minor.yy3;
            }
            else
            {
              sqlite3SelectDelete( pParse.db, ref yymsp[-2].minor.yy3 );
            }
            yygotominor.yy3 = yymsp[0].minor.yy3;
          }
          //#line 2533 "parse.c"
          break;
        case 116: /* multiselect_op ::= UNION ALL */
          //#line 420 "parse.y"
          { yygotominor.yy328 = TK_ALL; }
          //#line 2538 "parse.c"
          break;
        case 118: /* oneselect ::= SELECT distinct selcollist from where_opt groupby_opt having_opt orderby_opt limit_opt */
          //#line 424 "parse.y"
          {
            yygotominor.yy3 = sqlite3SelectNew( pParse, yymsp[-6].minor.yy14, yymsp[-5].minor.yy65, yymsp[-4].minor.yy132, yymsp[-3].minor.yy14, yymsp[-2].minor.yy132, yymsp[-1].minor.yy14, yymsp[-7].minor.yy328, yymsp[0].minor.yy476.pLimit, yymsp[0].minor.yy476.pOffset );
          }
          //#line 2545 "parse.c"
          break;
        case 122: /* sclp ::= selcollist COMMA */
        case 248: /* idxlist_opt ::= LP idxlist RP */ //yytestcase(yyruleno==248);
          //#line 445 "parse.y"
          { yygotominor.yy14 = yymsp[-1].minor.yy14; }
          //#line 2551 "parse.c"
          break;
        case 123: /* sclp ::= */
        case 151: /* orderby_opt ::= */ //yytestcase(yyruleno==151);
        case 159: /* groupby_opt ::= */ //yytestcase(yyruleno==159);
        case 241: /* exprlist ::= */ //yytestcase(yyruleno==241);
        case 247: /* idxlist_opt ::= */ //yytestcase(yyruleno==247);
          //#line 446 "parse.y"
          { yygotominor.yy14 = null; }
          //#line 2560 "parse.c"
          break;
        case 124: /* selcollist ::= sclp expr as */
          //#line 447 "parse.y"
          {
            yygotominor.yy14 = sqlite3ExprListAppend( pParse, yymsp[-2].minor.yy14, yymsp[-1].minor.yy346.pExpr );
            if ( yymsp[0].minor.yy0.n > 0 ) sqlite3ExprListSetName( pParse, yygotominor.yy14, yymsp[0].minor.yy0, 1 );
            sqlite3ExprListSetSpan( pParse, yygotominor.yy14, yymsp[-1].minor.yy346 );
          }
          //#line 2569 "parse.c"
          break;
        case 125: /* selcollist ::= sclp STAR */
          //#line 452 "parse.y"
          {
            Expr p = sqlite3Expr( pParse.db, TK_ALL, null );
            yygotominor.yy14 = sqlite3ExprListAppend( pParse, yymsp[-1].minor.yy14, p );
          }
          //#line 2577 "parse.c"
          break;
        case 126: /* selcollist ::= sclp nm DOT STAR */
          //#line 456 "parse.y"
          {
            Expr pRight = sqlite3PExpr( pParse, TK_ALL, 0, 0, yymsp[0].minor.yy0 );
            Expr pLeft = sqlite3PExpr( pParse, TK_ID, 0, 0, yymsp[-2].minor.yy0 );
            Expr pDot = sqlite3PExpr( pParse, TK_DOT, pLeft, pRight, 0 );
            yygotominor.yy14 = sqlite3ExprListAppend( pParse, yymsp[-3].minor.yy14, pDot );
          }
          //#line 2587 "parse.c"
          break;
        case 129: /* as ::= */
          //#line 469 "parse.y"
          { yygotominor.yy0.n = 0; }
          //#line 2592 "parse.c"
          break;
        case 130: /* from ::= */
          //#line 481 "parse.y"
          { yygotominor.yy65 = new SrcList(); }//sqlite3DbMallocZero(pParse.db, sizeof(*yygotominor.yy65));}
          //#line 2597 "parse.c"
          break;
        case 131: /* from ::= FROM seltablist */
          //#line 482 "parse.y"
          {
            yygotominor.yy65 = yymsp[0].minor.yy65;
            sqlite3SrcListShiftJoinType( yygotominor.yy65 );
          }
          //#line 2605 "parse.c"
          break;
        case 132: /* stl_prefix ::= seltablist joinop */
          //#line 490 "parse.y"
          {
            yygotominor.yy65 = yymsp[-1].minor.yy65;
            if ( ALWAYS( yygotominor.yy65 != null && yygotominor.yy65.nSrc > 0 ) ) yygotominor.yy65.a[yygotominor.yy65.nSrc - 1].jointype = (u8)yymsp[0].minor.yy328;
          }
          //#line 2613 "parse.c"
          break;
        case 133: /* stl_prefix ::= */
          //#line 494 "parse.y"
          { yygotominor.yy65 = null; }
          //#line 2618 "parse.c"
          break;
        case 134: /* seltablist ::= stl_prefix nm dbnm as indexed_opt on_opt using_opt */
          //#line 495 "parse.y"
          {
            yygotominor.yy65 = sqlite3SrcListAppendFromTerm( pParse, yymsp[-6].minor.yy65, yymsp[-5].minor.yy0, yymsp[-4].minor.yy0, yymsp[-3].minor.yy0, 0, yymsp[-1].minor.yy132, yymsp[0].minor.yy408 );
            sqlite3SrcListIndexedBy( pParse, yygotominor.yy65, yymsp[-2].minor.yy0 );
          }
          //#line 2626 "parse.c"
          break;
        case 135: /* seltablist ::= stl_prefix LP select RP as on_opt using_opt */
          //#line 501 "parse.y"
          {
            yygotominor.yy65 = sqlite3SrcListAppendFromTerm( pParse, yymsp[-6].minor.yy65, 0, 0, yymsp[-2].minor.yy0, yymsp[-4].minor.yy3, yymsp[-1].minor.yy132, yymsp[0].minor.yy408 );
          }
          //#line 2633 "parse.c"
          break;
        case 136: /* seltablist ::= stl_prefix LP seltablist RP as on_opt using_opt */
          //#line 505 "parse.y"
          {
            if ( yymsp[-6].minor.yy65 == null && yymsp[-2].minor.yy0.n == 0 && yymsp[-1].minor.yy132 == null && yymsp[0].minor.yy408 == null )
            {
              yygotominor.yy65 = yymsp[-4].minor.yy65;
            }
            else
            {
              Select pSubquery;
              sqlite3SrcListShiftJoinType( yymsp[-4].minor.yy65 );
              pSubquery = sqlite3SelectNew( pParse, 0, yymsp[-4].minor.yy65, 0, 0, 0, 0, 0, 0, 0 );
              yygotominor.yy65 = sqlite3SrcListAppendFromTerm( pParse, yymsp[-6].minor.yy65, 0, 0, yymsp[-2].minor.yy0, pSubquery, yymsp[-1].minor.yy132, yymsp[0].minor.yy408 );
            }
          }
          //#line 2647 "parse.c"
          break;
        case 137: /* dbnm ::= */
        case 146: /* indexed_opt ::= */ //yytestcase(yyruleno==146);
          //#line 530 "parse.y"
          { yygotominor.yy0.z = null; yygotominor.yy0.n = 0; }
          //#line 2653 "parse.c"
          break;
        case 139: /* fullname ::= nm dbnm */
          //#line 535 "parse.y"
          { yygotominor.yy65 = sqlite3SrcListAppend( pParse.db, 0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0 ); }
          //#line 2658 "parse.c"
          break;
        case 140: /* joinop ::= COMMA|JOIN */
          //#line 539 "parse.y"
          { yygotominor.yy328 = JT_INNER; }
          //#line 2663 "parse.c"
          break;
        case 141: /* joinop ::= JOIN_KW JOIN */
          //#line 540 "parse.y"
          { yygotominor.yy328 = sqlite3JoinType( pParse, yymsp[-1].minor.yy0, 0, 0 ); }
          //#line 2668 "parse.c"
          break;
        case 142: /* joinop ::= JOIN_KW nm JOIN */
          //#line 541 "parse.y"
          { yygotominor.yy328 = sqlite3JoinType( pParse, yymsp[-2].minor.yy0, yymsp[-1].minor.yy0, 0 ); }
          //#line 2673 "parse.c"
          break;
        case 143: /* joinop ::= JOIN_KW nm nm JOIN */
          //#line 543 "parse.y"
          { yygotominor.yy328 = sqlite3JoinType( pParse, yymsp[-3].minor.yy0, yymsp[-2].minor.yy0, yymsp[-1].minor.yy0 ); }
          //#line 2678 "parse.c"
          break;
        case 144: /* on_opt ::= ON expr */
        case 155: /* sortitem ::= expr */ //yytestcase(yyruleno==155);
        case 162: /* having_opt ::= HAVING expr */ //yytestcase(yyruleno==162);
        case 169: /* where_opt ::= WHERE expr */ //yytestcase(yyruleno==169);
        case 236: /* case_else ::= ELSE expr */ //yytestcase(yyruleno==236);
        case 238: /* case_operand ::= expr */ //yytestcase(yyruleno==238);
          //#line 547 "parse.y"
          { yygotominor.yy132 = yymsp[0].minor.yy346.pExpr; }
          //#line 2688 "parse.c"
          break;
        case 145: /* on_opt ::= */
        case 161: /* having_opt ::= */ //yytestcase(yyruleno==161);
        case 168: /* where_opt ::= */ //yytestcase(yyruleno==168);
        case 237: /* case_else ::= */ //yytestcase(yyruleno==237);
        case 239: /* case_operand ::= */ //yytestcase(yyruleno==239);
          //#line 548 "parse.y"
          { yygotominor.yy132 = null; }
          //#line 2697 "parse.c"
          break;
        case 148: /* indexed_opt ::= NOT INDEXED */
          //#line 563 "parse.y"
          { yygotominor.yy0.z = null; yygotominor.yy0.n = 1; }
          //#line 2702 "parse.c"
          break;
        case 149: /* using_opt ::= USING LP inscollist RP */
        case 181: /* inscollist_opt ::= LP inscollist RP */ //yytestcase(yyruleno==181);
          //#line 567 "parse.y"
          { yygotominor.yy408 = yymsp[-1].minor.yy408; }
          //#line 2708 "parse.c"
          break;
        case 150: /* using_opt ::= */
        case 180: /* inscollist_opt ::= */ //yytestcase(yyruleno==180);
          //#line 568 "parse.y"
          { yygotominor.yy408 = null; }
          //#line 2714 "parse.c"
          break;
        case 152: /* orderby_opt ::= ORDER BY sortlist */
        case 160: /* groupby_opt ::= GROUP BY nexprlist */ //yytestcase(yyruleno==160);
        case 240: /* exprlist ::= nexprlist */ //yytestcase(yyruleno==240);
          //#line 579 "parse.y"
          { yygotominor.yy14 = yymsp[0].minor.yy14; }
          //#line 2721 "parse.c"
          break;
        case 153: /* sortlist ::= sortlist COMMA sortitem sortorder */
          //#line 580 "parse.y"
          {
            yygotominor.yy14 = sqlite3ExprListAppend( pParse, yymsp[-3].minor.yy14, yymsp[-1].minor.yy132 );
            if ( yygotominor.yy14 != null ) yygotominor.yy14.a[yygotominor.yy14.nExpr - 1].sortOrder = (u8)yymsp[0].minor.yy328;
          }
          //#line 2729 "parse.c"
          break;
        case 154: /* sortlist ::= sortitem sortorder */
          //#line 584 "parse.y"
          {
            yygotominor.yy14 = sqlite3ExprListAppend( pParse, 0, yymsp[-1].minor.yy132 );
            if ( yygotominor.yy14 != null && ALWAYS( yygotominor.yy14.a != null ) ) yygotominor.yy14.a[0].sortOrder = (u8)yymsp[0].minor.yy328;
          }
          //#line 2737 "parse.c"
          break;
        case 156: /* sortorder ::= ASC */
        case 158: /* sortorder ::= */ //yytestcase(yyruleno==158);
          //#line 592 "parse.y"
          { yygotominor.yy328 = SQLITE_SO_ASC; }
          //#line 2743 "parse.c"
          break;
        case 157: /* sortorder ::= DESC */
          //#line 593 "parse.y"
          { yygotominor.yy328 = SQLITE_SO_DESC; }
          //#line 2748 "parse.c"
          break;
        case 163: /* limit_opt ::= */
          //#line 619 "parse.y"
          { yygotominor.yy476.pLimit = null; yygotominor.yy476.pOffset = null; }
          //#line 2753 "parse.c"
          break;
        case 164: /* limit_opt ::= LIMIT expr */
          //#line 620 "parse.y"
          { yygotominor.yy476.pLimit = yymsp[0].minor.yy346.pExpr; yygotominor.yy476.pOffset = null; }
          //#line 2758 "parse.c"
          break;
        case 165: /* limit_opt ::= LIMIT expr OFFSET expr */
          //#line 622 "parse.y"
          { yygotominor.yy476.pLimit = yymsp[-2].minor.yy346.pExpr; yygotominor.yy476.pOffset = yymsp[0].minor.yy346.pExpr; }
          //#line 2763 "parse.c"
          break;
        case 166: /* limit_opt ::= LIMIT expr COMMA expr */
          //#line 624 "parse.y"
          { yygotominor.yy476.pOffset = yymsp[-2].minor.yy346.pExpr; yygotominor.yy476.pLimit = yymsp[0].minor.yy346.pExpr; }
          //#line 2768 "parse.c"
          break;
        case 167: /* cmd ::= DELETE FROM fullname indexed_opt where_opt */
          //#line 637 "parse.y"
          {
            sqlite3SrcListIndexedBy( pParse, yymsp[-2].minor.yy65, yymsp[-1].minor.yy0 );
            sqlite3DeleteFrom( pParse, yymsp[-2].minor.yy65, yymsp[0].minor.yy132 );
          }
          //#line 2776 "parse.c"
          break;
        case 170: /* cmd ::= UPDATE orconf fullname indexed_opt SET setlist where_opt */
          //#line 660 "parse.y"
          {
            sqlite3SrcListIndexedBy( pParse, yymsp[-4].minor.yy65, yymsp[-3].minor.yy0 );
            sqlite3ExprListCheckLength( pParse, yymsp[-1].minor.yy14, "set list" );
            sqlite3Update( pParse, yymsp[-4].minor.yy65, yymsp[-1].minor.yy14, yymsp[0].minor.yy132, yymsp[-5].minor.yy186 );
          }
          //#line 2785 "parse.c"
          break;
        case 171: /* setlist ::= setlist COMMA nm EQ expr */
          //#line 670 "parse.y"
          {
            yygotominor.yy14 = sqlite3ExprListAppend( pParse, yymsp[-4].minor.yy14, yymsp[0].minor.yy346.pExpr );
            sqlite3ExprListSetName( pParse, yygotominor.yy14, yymsp[-2].minor.yy0, 1 );
          }
          //#line 2793 "parse.c"
          break;
        case 172: /* setlist ::= nm EQ expr */
          //#line 674 "parse.y"
          {
            yygotominor.yy14 = sqlite3ExprListAppend( pParse, 0, yymsp[0].minor.yy346.pExpr );
            sqlite3ExprListSetName( pParse, yygotominor.yy14, yymsp[-2].minor.yy0, 1 );
          }
          //#line 2801 "parse.c"
          break;
        case 173: /* cmd ::= insert_cmd INTO fullname inscollist_opt VALUES LP itemlist RP */
          //#line 683 "parse.y"
          { sqlite3Insert( pParse, yymsp[-5].minor.yy65, yymsp[-1].minor.yy14, 0, yymsp[-4].minor.yy408, yymsp[-7].minor.yy186 ); }
          //#line 2806 "parse.c"
          break;
        case 174: /* cmd ::= insert_cmd INTO fullname inscollist_opt select */
          //#line 685 "parse.y"
          { sqlite3Insert( pParse, yymsp[-2].minor.yy65, 0, yymsp[0].minor.yy3, yymsp[-1].minor.yy408, yymsp[-4].minor.yy186 ); }
          //#line 2811 "parse.c"
          break;
        case 175: /* cmd ::= insert_cmd INTO fullname inscollist_opt DEFAULT VALUES */
          //#line 687 "parse.y"
          { sqlite3Insert( pParse, yymsp[-3].minor.yy65, 0, 0, yymsp[-2].minor.yy408, yymsp[-5].minor.yy186 ); }
          //#line 2816 "parse.c"
          break;
        case 176: /* insert_cmd ::= INSERT orconf */
          //#line 690 "parse.y"
          { yygotominor.yy186 = yymsp[0].minor.yy186; }
          //#line 2821 "parse.c"
          break;
        case 177: /* insert_cmd ::= REPLACE */
          //#line 691 "parse.y"
          { yygotominor.yy186 = OE_Replace; }
          //#line 2826 "parse.c"
          break;
        case 178: /* itemlist ::= itemlist COMMA expr */
        case 242: /* nexprlist ::= nexprlist COMMA expr */ //yytestcase(yyruleno==242);
          //#line 698 "parse.y"
          { yygotominor.yy14 = sqlite3ExprListAppend( pParse, yymsp[-2].minor.yy14, yymsp[0].minor.yy346.pExpr ); }
          //#line 2832 "parse.c"
          break;
        case 179: /* itemlist ::= expr */
        case 243: /* nexprlist ::= expr */ //yytestcase(yyruleno==243);
          //#line 700 "parse.y"
          { yygotominor.yy14 = sqlite3ExprListAppend( pParse, 0, yymsp[0].minor.yy346.pExpr ); }
          //#line 2838 "parse.c"
          break;
        case 182: /* inscollist ::= inscollist COMMA nm */
          //#line 710 "parse.y"
          { yygotominor.yy408 = sqlite3IdListAppend( pParse.db, yymsp[-2].minor.yy408, yymsp[0].minor.yy0 ); }
          //#line 2843 "parse.c"
          break;
        case 183: /* inscollist ::= nm */
          //#line 712 "parse.y"
          { yygotominor.yy408 = sqlite3IdListAppend( pParse.db, 0, yymsp[0].minor.yy0 ); }
          //#line 2848 "parse.c"
          break;
        case 184: /* expr ::= term */
        case 212: /* escape ::= ESCAPE expr */ //yytestcase(yyruleno==212);
          //#line 743 "parse.y"
          { yygotominor.yy346 = yymsp[0].minor.yy346; }
          //#line 2854 "parse.c"
          break;
        case 185: /* expr ::= LP expr RP */
          //#line 744 "parse.y"
          { yygotominor.yy346.pExpr = yymsp[-1].minor.yy346.pExpr; spanSet( yygotominor.yy346, yymsp[-2].minor.yy0, yymsp[0].minor.yy0 ); }
          //#line 2859 "parse.c"
          break;
        case 186: /* term ::= NULL */
        case 191: /* term ::= INTEGER|FLOAT|BLOB */ //yytestcase(yyruleno==191);
        case 192: /* term ::= STRING */ //yytestcase(yyruleno==192);
          //#line 745 "parse.y"
          { spanExpr( yygotominor.yy346, pParse, yymsp[0].major, yymsp[0].minor.yy0 ); }
          //#line 2866 "parse.c"
          break;
        case 187: /* expr ::= id */
        case 188: /* expr ::= JOIN_KW */ //yytestcase(yyruleno==188);
          //#line 746 "parse.y"
          { spanExpr( yygotominor.yy346, pParse, TK_ID, yymsp[0].minor.yy0 ); }
          //#line 2872 "parse.c"
          break;
        case 189: /* expr ::= nm DOT nm */
          //#line 748 "parse.y"
          {
            Expr temp1 = sqlite3PExpr( pParse, TK_ID, 0, 0, yymsp[-2].minor.yy0 );
            Expr temp2 = sqlite3PExpr( pParse, TK_ID, 0, 0, yymsp[0].minor.yy0 );
            yygotominor.yy346.pExpr = sqlite3PExpr( pParse, TK_DOT, temp1, temp2, 0 );
            spanSet( yygotominor.yy346, yymsp[-2].minor.yy0, yymsp[0].minor.yy0 );
          }
          //#line 2882 "parse.c"
          break;
        case 190: /* expr ::= nm DOT nm DOT nm */
          //#line 754 "parse.y"
          {
            Expr temp1 = sqlite3PExpr( pParse, TK_ID, 0, 0, yymsp[-4].minor.yy0 );
            Expr temp2 = sqlite3PExpr( pParse, TK_ID, 0, 0, yymsp[-2].minor.yy0 );
            Expr temp3 = sqlite3PExpr( pParse, TK_ID, 0, 0, yymsp[0].minor.yy0 );
            Expr temp4 = sqlite3PExpr( pParse, TK_DOT, temp2, temp3, 0 );
            yygotominor.yy346.pExpr = sqlite3PExpr( pParse, TK_DOT, temp1, temp4, 0 );
            spanSet( yygotominor.yy346, yymsp[-4].minor.yy0, yymsp[0].minor.yy0 );
          }
          //#line 2894 "parse.c"
          break;
        case 193: /* expr ::= REGISTER */
          //#line 764 "parse.y"
          {
            /* When doing a nested parse, one can include terms in an expression
            ** that look like this:   #1 #2 ...  These terms refer to registers
            ** in the virtual machine.  #N is the N-th register. */
            if ( pParse.nested == 0 )
            {
              sqlite3ErrorMsg( pParse, "near \"%T\": syntax error", yymsp[0].minor.yy0 );
              yygotominor.yy346.pExpr = null;
            }
            else
            {
              yygotominor.yy346.pExpr = sqlite3PExpr( pParse, TK_REGISTER, 0, 0, yymsp[0].minor.yy0 );
              if ( yygotominor.yy346.pExpr != null ) sqlite3GetInt32( yymsp[0].minor.yy0.z, 1, ref yygotominor.yy346.pExpr.iTable );
            }
            spanSet( yygotominor.yy346, yymsp[0].minor.yy0, yymsp[0].minor.yy0 );
          }
          //#line 2911 "parse.c"
          break;
        case 194: /* expr ::= VARIABLE */
          //#line 777 "parse.y"
          {
            spanExpr( yygotominor.yy346, pParse, TK_VARIABLE, yymsp[0].minor.yy0 );
            sqlite3ExprAssignVarNumber( pParse, yygotominor.yy346.pExpr );
            spanSet( yygotominor.yy346, yymsp[0].minor.yy0, yymsp[0].minor.yy0 );
          }
          //#line 2920 "parse.c"
          break;
        case 195: /* expr ::= expr COLLATE ids */
          //#line 782 "parse.y"
          {
            yygotominor.yy346.pExpr = sqlite3ExprSetColl( pParse, yymsp[-2].minor.yy346.pExpr, yymsp[0].minor.yy0 );
            yygotominor.yy346.zStart = yymsp[-2].minor.yy346.zStart;
            yygotominor.yy346.zEnd = yymsp[0].minor.yy0.z.Substring( yymsp[0].minor.yy0.n );
          }
          //#line 2929 "parse.c"
          break;
        case 196: /* expr ::= CAST LP expr AS typetoken RP */
          //#line 788 "parse.y"
          {
            yygotominor.yy346.pExpr = sqlite3PExpr( pParse, TK_CAST, yymsp[-3].minor.yy346.pExpr, 0, yymsp[-1].minor.yy0 );
            spanSet( yygotominor.yy346, yymsp[-5].minor.yy0, yymsp[0].minor.yy0 );
          }
          //#line 2937 "parse.c"
          break;
        case 197: /* expr ::= ID LP distinct exprlist RP */
          //#line 793 "parse.y"
          {
            if ( yymsp[-1].minor.yy14 != null && yymsp[-1].minor.yy14.nExpr > pParse.db.aLimit[SQLITE_LIMIT_FUNCTION_ARG] )
            {
              sqlite3ErrorMsg( pParse, "too many arguments on function %T", yymsp[-4].minor.yy0 );
            }
            yygotominor.yy346.pExpr = sqlite3ExprFunction( pParse, yymsp[-1].minor.yy14, yymsp[-4].minor.yy0 );
            spanSet( yygotominor.yy346, yymsp[-4].minor.yy0, yymsp[0].minor.yy0 );
            if ( yymsp[-2].minor.yy328 != 0 && yygotominor.yy346.pExpr != null )
            {
              yygotominor.yy346.pExpr.flags |= EP_Distinct;
            }
          }
          //#line 2951 "parse.c"
          break;
        case 198: /* expr ::= ID LP STAR RP */
          //#line 803 "parse.y"
          {
            yygotominor.yy346.pExpr = sqlite3ExprFunction( pParse, 0, yymsp[-3].minor.yy0 );
            spanSet( yygotominor.yy346, yymsp[-3].minor.yy0, yymsp[0].minor.yy0 );
          }
          //#line 2959 "parse.c"
          break;
        case 199: /* term ::= CTIME_KW */
          //#line 807 "parse.y"
          {
            /* The CURRENT_TIME, CURRENT_DATE, and CURRENT_TIMESTAMP values are
            ** treated as functions that return constants */
            yygotominor.yy346.pExpr = sqlite3ExprFunction( pParse, 0, yymsp[0].minor.yy0 );
            if ( yygotominor.yy346.pExpr != null )
            {
              yygotominor.yy346.pExpr.op = TK_CONST_FUNC;
            }
            spanSet( yygotominor.yy346, yymsp[0].minor.yy0, yymsp[0].minor.yy0 );
          }
          //#line 2972 "parse.c"
          break;
        case 200: /* expr ::= expr AND expr */
        case 201: /* expr ::= expr OR expr */ //yytestcase(yyruleno==201);
        case 202: /* expr ::= expr LT|GT|GE|LE expr */ //yytestcase(yyruleno==202);
        case 203: /* expr ::= expr EQ|NE expr */ //yytestcase(yyruleno==203);
        case 204: /* expr ::= expr BITAND|BITOR|LSHIFT|RSHIFT expr */ //yytestcase(yyruleno==204);
        case 205: /* expr ::= expr PLUS|MINUS expr */ //yytestcase(yyruleno==205);
        case 206: /* expr ::= expr STAR|SLASH|REM expr */ //yytestcase(yyruleno==206);
        case 207: /* expr ::= expr CONCAT expr */ //yytestcase(yyruleno==207);
          //#line 834 "parse.y"
          { spanBinaryExpr( yygotominor.yy346, pParse, yymsp[-1].major, yymsp[-2].minor.yy346, yymsp[0].minor.yy346 ); }
          //#line 2984 "parse.c"
          break;
        case 208: /* likeop ::= LIKE_KW */
        case 210: /* likeop ::= MATCH */ //yytestcase(yyruleno==210);
          //#line 847 "parse.y"
          { yygotominor.yy96.eOperator = yymsp[0].minor.yy0; yygotominor.yy96.not = false; }
          //#line 2990 "parse.c"
          break;
        case 209: /* likeop ::= NOT LIKE_KW */
        case 211: /* likeop ::= NOT MATCH */ //yytestcase(yyruleno==211);
          //#line 848 "parse.y"
          { yygotominor.yy96.eOperator = yymsp[0].minor.yy0; yygotominor.yy96.not = true; }
          //#line 2996 "parse.c"
          break;
        case 213: /* escape ::= */
          //#line 854 "parse.y"
          { yygotominor.yy346 = new ExprSpan(); }// memset(yygotominor.yy346, 0, sizeof(yygotominor.yy346)); }
          //#line 3001 "parse.c"
          break;
        case 214: /* expr ::= expr likeop expr escape */
          //#line 855 "parse.y"
          {
            ExprList pList;
            pList = sqlite3ExprListAppend( pParse, 0, yymsp[-1].minor.yy346.pExpr );
            pList = sqlite3ExprListAppend( pParse, pList, yymsp[-3].minor.yy346.pExpr );
            if ( yymsp[0].minor.yy346.pExpr != null )
            {
              pList = sqlite3ExprListAppend( pParse, pList, yymsp[0].minor.yy346.pExpr );
            }
            yygotominor.yy346.pExpr = sqlite3ExprFunction( pParse, pList, yymsp[-2].minor.yy96.eOperator );
            if ( yymsp[-2].minor.yy96.not ) yygotominor.yy346.pExpr = sqlite3PExpr( pParse, TK_NOT, yygotominor.yy346.pExpr, 0, 0 );
            yygotominor.yy346.zStart = yymsp[-3].minor.yy346.zStart;
            yygotominor.yy346.zEnd = yymsp[-1].minor.yy346.zEnd;
            if ( yygotominor.yy346.pExpr != null ) yygotominor.yy346.pExpr.flags |= EP_InfixFunc;
          }
          //#line 3018 "parse.c"
          break;
        case 215: /* expr ::= expr ISNULL|NOTNULL */
          //#line 885 "parse.y"
          { spanUnaryPostfix( yygotominor.yy346, pParse, yymsp[0].major, yymsp[-1].minor.yy346, yymsp[0].minor.yy0 ); }
          //#line 3023 "parse.c"
          break;
        case 216: /* expr ::= expr NOT NULL */
          //#line 886 "parse.y"
          { spanUnaryPostfix( yygotominor.yy346, pParse, TK_NOTNULL, yymsp[-2].minor.yy346, yymsp[0].minor.yy0 ); }
          //#line 3028 "parse.c"
          break;
        case 217: /* expr ::= expr IS expr */
          //#line 907 "parse.y"
          {
            spanBinaryExpr( yygotominor.yy346, pParse, TK_IS, yymsp[-2].minor.yy346, yymsp[0].minor.yy346 );
            binaryToUnaryIfNull( pParse, yymsp[0].minor.yy346.pExpr, yygotominor.yy346.pExpr, TK_ISNULL );
          }
          //#line 3036 "parse.c"
          break;
        case 218: /* expr ::= expr IS NOT expr */
          //#line 911 "parse.y"
          {
            spanBinaryExpr( yygotominor.yy346, pParse, TK_ISNOT, yymsp[-3].minor.yy346, yymsp[0].minor.yy346 );
            binaryToUnaryIfNull( pParse, yymsp[0].minor.yy346.pExpr, yygotominor.yy346.pExpr, TK_NOTNULL );
          }
          //#line 3044 "parse.c"
          break;
        case 219: /* expr ::= NOT expr */
        case 220: /* expr ::= BITNOT expr */ //yytestcase(yyruleno==220);
          //#line 934 "parse.y"
          { spanUnaryPrefix( yygotominor.yy346, pParse, yymsp[-1].major, yymsp[0].minor.yy346, yymsp[-1].minor.yy0 ); }
          //#line 3050 "parse.c"
          break;
        case 221: /* expr ::= MINUS expr */
          //#line 937 "parse.y"
          { spanUnaryPrefix( yygotominor.yy346, pParse, TK_UMINUS, yymsp[0].minor.yy346, yymsp[-1].minor.yy0 ); }
          //#line 3055 "parse.c"
          break;
        case 222: /* expr ::= PLUS expr */
          //#line 939 "parse.y"
          { spanUnaryPrefix( yygotominor.yy346, pParse, TK_UPLUS, yymsp[0].minor.yy346, yymsp[-1].minor.yy0 ); }
          //#line 3060 "parse.c"
          break;
        case 225: /* expr ::= expr between_op expr AND expr */
          //#line 944 "parse.y"
          {
            ExprList pList = sqlite3ExprListAppend( pParse, 0, yymsp[-2].minor.yy346.pExpr );
            pList = sqlite3ExprListAppend( pParse, pList, yymsp[0].minor.yy346.pExpr );
            yygotominor.yy346.pExpr = sqlite3PExpr( pParse, TK_BETWEEN, yymsp[-4].minor.yy346.pExpr, 0, 0 );
            if ( yygotominor.yy346.pExpr != null )
            {
              yygotominor.yy346.pExpr.x.pList = pList;
            }
            else
            {
              sqlite3ExprListDelete( pParse.db, ref pList );
            }
            if ( yymsp[-3].minor.yy328 != 0 ) yygotominor.yy346.pExpr = sqlite3PExpr( pParse, TK_NOT, yygotominor.yy346.pExpr, 0, 0 );
            yygotominor.yy346.zStart = yymsp[-4].minor.yy346.zStart;
            yygotominor.yy346.zEnd = yymsp[0].minor.yy346.zEnd;
          }
          //#line 3077 "parse.c"
          break;
        case 228: /* expr ::= expr in_op LP exprlist RP */
          //#line 961 "parse.y"
          {
            yygotominor.yy346.pExpr = sqlite3PExpr( pParse, TK_IN, yymsp[-4].minor.yy346.pExpr, 0, 0 );
            if ( yygotominor.yy346.pExpr != null )
            {
              yygotominor.yy346.pExpr.x.pList = yymsp[-1].minor.yy14;
              sqlite3ExprSetHeight( pParse, yygotominor.yy346.pExpr );
            }
            else
            {
              sqlite3ExprListDelete( pParse.db, ref yymsp[-1].minor.yy14 );
            }
            if ( yymsp[-3].minor.yy328 != 0 ) yygotominor.yy346.pExpr = sqlite3PExpr( pParse, TK_NOT, yygotominor.yy346.pExpr, 0, 0 );
            yygotominor.yy346.zStart = yymsp[-4].minor.yy346.zStart;
            yygotominor.yy346.zEnd = yymsp[0].minor.yy0.z.Substring( yymsp[0].minor.yy0.n );
          }
          //#line 3093 "parse.c"
          break;
        case 229: /* expr ::= LP select RP */
          //#line 973 "parse.y"
          {
            yygotominor.yy346.pExpr = sqlite3PExpr( pParse, TK_SELECT, 0, 0, 0 );
            if ( yygotominor.yy346.pExpr != null )
            {
              yygotominor.yy346.pExpr.x.pSelect = yymsp[-1].minor.yy3;
              ExprSetProperty( yygotominor.yy346.pExpr, EP_xIsSelect );
              sqlite3ExprSetHeight( pParse, yygotominor.yy346.pExpr );
            }
            else
            {
              sqlite3SelectDelete( pParse.db, ref yymsp[-1].minor.yy3 );
            }
            yygotominor.yy346.zStart = yymsp[-2].minor.yy0.z;
            yygotominor.yy346.zEnd = yymsp[0].minor.yy0.z.Substring( yymsp[0].minor.yy0.n );
          }
          //#line 3109 "parse.c"
          break;
        case 230: /* expr ::= expr in_op LP select RP */
          //#line 985 "parse.y"
          {
            yygotominor.yy346.pExpr = sqlite3PExpr( pParse, TK_IN, yymsp[-4].minor.yy346.pExpr, 0, 0 );
            if ( yygotominor.yy346.pExpr != null )
            {
              yygotominor.yy346.pExpr.x.pSelect = yymsp[-1].minor.yy3;
              ExprSetProperty( yygotominor.yy346.pExpr, EP_xIsSelect );
              sqlite3ExprSetHeight( pParse, yygotominor.yy346.pExpr );
            }
            else
            {
              sqlite3SelectDelete( pParse.db, ref yymsp[-1].minor.yy3 );
            }
            if ( yymsp[-3].minor.yy328 != 0 ) yygotominor.yy346.pExpr = sqlite3PExpr( pParse, TK_NOT, yygotominor.yy346.pExpr, 0, 0 );
            yygotominor.yy346.zStart = yymsp[-4].minor.yy346.zStart;
            yygotominor.yy346.zEnd = yymsp[0].minor.yy0.z.Substring( yymsp[0].minor.yy0.n );
          }
          //#line 3126 "parse.c"
          break;
        case 231: /* expr ::= expr in_op nm dbnm */
          //#line 998 "parse.y"
          {
            SrcList pSrc = sqlite3SrcListAppend( pParse.db, 0, yymsp[-1].minor.yy0, yymsp[0].minor.yy0 );
            yygotominor.yy346.pExpr = sqlite3PExpr( pParse, TK_IN, yymsp[-3].minor.yy346.pExpr, 0, 0 );
            if ( yygotominor.yy346.pExpr != null )
            {
              yygotominor.yy346.pExpr.x.pSelect = sqlite3SelectNew( pParse, 0, pSrc, 0, 0, 0, 0, 0, 0, 0 );
              ExprSetProperty( yygotominor.yy346.pExpr, EP_xIsSelect );
              sqlite3ExprSetHeight( pParse, yygotominor.yy346.pExpr );
            }
            else
            {
              sqlite3SrcListDelete( pParse.db, ref pSrc );
            }
            if ( yymsp[-2].minor.yy328 != 0 ) yygotominor.yy346.pExpr = sqlite3PExpr( pParse, TK_NOT, yygotominor.yy346.pExpr, 0, 0 );
            yygotominor.yy346.zStart = yymsp[-3].minor.yy346.zStart;
            yygotominor.yy346.zEnd = yymsp[0].minor.yy0.z != null ? yymsp[0].minor.yy0.z.Substring( yymsp[0].minor.yy0.n ) : yymsp[-1].minor.yy0.z.Substring( yymsp[-1].minor.yy0.n );
          }
          //#line 3144 "parse.c"
          break;
        case 232: /* expr ::= EXISTS LP select RP */
          //#line 1012 "parse.y"
          {
            Expr p = yygotominor.yy346.pExpr = sqlite3PExpr( pParse, TK_EXISTS, 0, 0, 0 );
            if ( p != null )
            {
              p.x.pSelect = yymsp[-1].minor.yy3;
              ExprSetProperty( p, EP_xIsSelect );
              sqlite3ExprSetHeight( pParse, p );
            }
            else
            {
              sqlite3SelectDelete( pParse.db, ref yymsp[-1].minor.yy3 );
            }
            yygotominor.yy346.zStart = yymsp[-3].minor.yy0.z;
            yygotominor.yy346.zEnd = yymsp[0].minor.yy0.z.Substring( yymsp[0].minor.yy0.n );
          }
          //#line 3160 "parse.c"
          break;
        case 233: /* expr ::= CASE case_operand case_exprlist case_else END */
          //#line 1027 "parse.y"
          {
            yygotominor.yy346.pExpr = sqlite3PExpr( pParse, TK_CASE, yymsp[-3].minor.yy132, yymsp[-1].minor.yy132, 0 );
            if ( yygotominor.yy346.pExpr != null )
            {
              yygotominor.yy346.pExpr.x.pList = yymsp[-2].minor.yy14;
              sqlite3ExprSetHeight( pParse, yygotominor.yy346.pExpr );
            }
            else
            {
              sqlite3ExprListDelete( pParse.db, ref yymsp[-2].minor.yy14 );
            }
            yygotominor.yy346.zStart = yymsp[-4].minor.yy0.z;
            yygotominor.yy346.zEnd = yymsp[0].minor.yy0.z.Substring( yymsp[0].minor.yy0.n );
          }
          //#line 3175 "parse.c"
          break;
        case 234: /* case_exprlist ::= case_exprlist WHEN expr THEN expr */
          //#line 1040 "parse.y"
          {
            yygotominor.yy14 = sqlite3ExprListAppend( pParse, yymsp[-4].minor.yy14, yymsp[-2].minor.yy346.pExpr );
            yygotominor.yy14 = sqlite3ExprListAppend( pParse, yygotominor.yy14, yymsp[0].minor.yy346.pExpr );
          }
          //#line 3183 "parse.c"
          break;
        case 235: /* case_exprlist ::= WHEN expr THEN expr */
          //#line 1044 "parse.y"
          {
            yygotominor.yy14 = sqlite3ExprListAppend( pParse, 0, yymsp[-2].minor.yy346.pExpr );
            yygotominor.yy14 = sqlite3ExprListAppend( pParse, yygotominor.yy14, yymsp[0].minor.yy346.pExpr );
          }
          //#line 3191 "parse.c"
          break;
        case 244: /* cmd ::= createkw uniqueflag INDEX ifnotexists nm dbnm ON nm LP idxlist RP */
          //#line 1073 "parse.y"
          {
            sqlite3CreateIndex( pParse, yymsp[-6].minor.yy0, yymsp[-5].minor.yy0,
                               sqlite3SrcListAppend( pParse.db, 0, yymsp[-3].minor.yy0, 0 ), yymsp[-1].minor.yy14, yymsp[-9].minor.yy328,
                                yymsp[-10].minor.yy0, yymsp[0].minor.yy0, SQLITE_SO_ASC, yymsp[-7].minor.yy328 );
          }
          //#line 3200 "parse.c"
          break;
        case 245: /* uniqueflag ::= UNIQUE */
        case 299: /* raisetype ::= ABORT */ //yytestcase(yyruleno==299);
          //#line 1080 "parse.y"
          { yygotominor.yy328 = OE_Abort; }
          //#line 3206 "parse.c"
          break;
        case 246: /* uniqueflag ::= */
          //#line 1081 "parse.y"
          { yygotominor.yy328 = OE_None; }
          //#line 3211 "parse.c"
          break;
        case 249: /* idxlist ::= idxlist COMMA nm collate sortorder */
          //#line 1090 "parse.y"
          {
            Expr p = null;
            if ( yymsp[-1].minor.yy0.n > 0 )
            {
              p = sqlite3Expr( pParse.db, TK_COLUMN, null );
              sqlite3ExprSetColl( pParse, p, yymsp[-1].minor.yy0 );
            }
            yygotominor.yy14 = sqlite3ExprListAppend( pParse, yymsp[-4].minor.yy14, p );
            sqlite3ExprListSetName( pParse, yygotominor.yy14, yymsp[-2].minor.yy0, 1 );
            sqlite3ExprListCheckLength( pParse, yygotominor.yy14, "index" );
            if ( yygotominor.yy14 != null ) yygotominor.yy14.a[yygotominor.yy14.nExpr - 1].sortOrder = (u8)yymsp[0].minor.yy328;
          }
          //#line 3226 "parse.c"
          break;
        case 250: /* idxlist ::= nm collate sortorder */
          //#line 1101 "parse.y"
          {
            Expr p = null;
            if ( yymsp[-1].minor.yy0.n > 0 )
            {
              p = sqlite3PExpr( pParse, TK_COLUMN, 0, 0, 0 );
              sqlite3ExprSetColl( pParse, p, yymsp[-1].minor.yy0 );
            }
            yygotominor.yy14 = sqlite3ExprListAppend( pParse, 0, p );
            sqlite3ExprListSetName( pParse, yygotominor.yy14, yymsp[-2].minor.yy0, 1 );
            sqlite3ExprListCheckLength( pParse, yygotominor.yy14, "index" );
            if ( yygotominor.yy14 != null ) yygotominor.yy14.a[yygotominor.yy14.nExpr - 1].sortOrder = (u8)yymsp[0].minor.yy328;
          }
          //#line 3241 "parse.c"
          break;
        case 251: /* collate ::= */
          //#line 1114 "parse.y"
          { yygotominor.yy0.z = null; yygotominor.yy0.n = 0; }
          //#line 3246 "parse.c"
          break;
        case 253: /* cmd ::= DROP INDEX ifexists fullname */
          //#line 1120 "parse.y"
          { sqlite3DropIndex( pParse, yymsp[0].minor.yy65, yymsp[-1].minor.yy328 ); }
          //#line 3251 "parse.c"
          break;
        case 254: /* cmd ::= VACUUM */
        case 255: /* cmd ::= VACUUM nm */ //yytestcase(yyruleno==255);
          //#line 1126 "parse.y"
          { sqlite3Vacuum( pParse ); }
          //#line 3257 "parse.c"
          break;
        case 256: /* cmd ::= PRAGMA nm dbnm */
          //#line 1134 "parse.y"
          { sqlite3Pragma( pParse, yymsp[-1].minor.yy0, yymsp[0].minor.yy0, 0, 0 ); }
          //#line 3262 "parse.c"
          break;
        case 257: /* cmd ::= PRAGMA nm dbnm EQ nmnum */
          //#line 1135 "parse.y"
          { sqlite3Pragma( pParse, yymsp[-3].minor.yy0, yymsp[-2].minor.yy0, yymsp[0].minor.yy0, 0 ); }
          //#line 3267 "parse.c"
          break;
        case 258: /* cmd ::= PRAGMA nm dbnm LP nmnum RP */
          //#line 1136 "parse.y"
          { sqlite3Pragma( pParse, yymsp[-4].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy0, 0 ); }
          //#line 3272 "parse.c"
          break;
        case 259: /* cmd ::= PRAGMA nm dbnm EQ minus_num */
          //#line 1138 "parse.y"
          { sqlite3Pragma( pParse, yymsp[-3].minor.yy0, yymsp[-2].minor.yy0, yymsp[0].minor.yy0, 1 ); }
          //#line 3277 "parse.c"
          break;
        case 260: /* cmd ::= PRAGMA nm dbnm LP minus_num RP */
          //#line 1140 "parse.y"
          { sqlite3Pragma( pParse, yymsp[-4].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy0, 1 ); }
          //#line 3282 "parse.c"
          break;
        case 271: /* cmd ::= createkw trigger_decl BEGIN trigger_cmd_list END */
          //#line 1158 "parse.y"
          {
            Token all = new Token();
            //all.z = yymsp[-3].minor.yy0.z;
            //all.n = (int)(yymsp[0].minor.yy0.z - yymsp[-3].minor.yy0.z) + yymsp[0].minor.yy0.n;
            all.n = (int)( yymsp[-3].minor.yy0.z.Length - yymsp[0].minor.yy0.z.Length ) + yymsp[0].minor.yy0.n;
            all.z = yymsp[-3].minor.yy0.z.Substring( 0, all.n );
            sqlite3FinishTrigger( pParse, yymsp[-1].minor.yy473, all );
          }
          //#line 3292 "parse.c"
          break;
        case 272: /* trigger_decl ::= temp TRIGGER ifnotexists nm dbnm trigger_time trigger_event ON fullname foreach_clause when_clause */
          //#line 1167 "parse.y"
          {
            sqlite3BeginTrigger( pParse, yymsp[-7].minor.yy0, yymsp[-6].minor.yy0, yymsp[-5].minor.yy328, yymsp[-4].minor.yy378.a, yymsp[-4].minor.yy378.b, yymsp[-2].minor.yy65, yymsp[0].minor.yy132, yymsp[-10].minor.yy328, yymsp[-8].minor.yy328 );
            yygotominor.yy0 = ( yymsp[-6].minor.yy0.n == 0 ? yymsp[-7].minor.yy0 : yymsp[-6].minor.yy0 );
          }
          //#line 3300 "parse.c"
          break;
        case 273: /* trigger_time ::= BEFORE */
        case 276: /* trigger_time ::= */ //yytestcase(yyruleno==276);
          //#line 1173 "parse.y"
          { yygotominor.yy328 = TK_BEFORE; }
          //#line 3306 "parse.c"
          break;
        case 274: /* trigger_time ::= AFTER */
          //#line 1174 "parse.y"
          { yygotominor.yy328 = TK_AFTER; }
          //#line 3311 "parse.c"
          break;
        case 275: /* trigger_time ::= INSTEAD OF */
          //#line 1175 "parse.y"
          { yygotominor.yy328 = TK_INSTEAD; }
          //#line 3316 "parse.c"
          break;
        case 277: /* trigger_event ::= DELETE|INSERT */
        case 278: /* trigger_event ::= UPDATE */ //yytestcase(yyruleno==278);
          //#line 1180 "parse.y"
          { yygotominor.yy378.a = yymsp[0].major; yygotominor.yy378.b = null; }
          //#line 3322 "parse.c"
          break;
        case 279: /* trigger_event ::= UPDATE OF inscollist */
          //#line 1182 "parse.y"
          { yygotominor.yy378.a = TK_UPDATE; yygotominor.yy378.b = yymsp[0].minor.yy408; }
          //#line 3327 "parse.c"
          break;
        case 282: /* when_clause ::= */
        case 304: /* key_opt ::= */ //yytestcase(yyruleno==304);
          //#line 1189 "parse.y"
          { yygotominor.yy132 = null; }
          //#line 3333 "parse.c"
          break;
        case 283: /* when_clause ::= WHEN expr */
        case 305: /* key_opt ::= KEY expr */ //yytestcase(yyruleno==305);
          //#line 1190 "parse.y"
          { yygotominor.yy132 = yymsp[0].minor.yy346.pExpr; }
          //#line 3339 "parse.c"
          break;
        case 284: /* trigger_cmd_list ::= trigger_cmd_list trigger_cmd SEMI */
          //#line 1194 "parse.y"
          {
            Debug.Assert( yymsp[-2].minor.yy473 != null );
            yymsp[-2].minor.yy473.pLast.pNext = yymsp[-1].minor.yy473;
            yymsp[-2].minor.yy473.pLast = yymsp[-1].minor.yy473;
            yygotominor.yy473 = yymsp[-2].minor.yy473;
          }
          //#line 3349 "parse.c"
          break;
        case 285: /* trigger_cmd_list ::= trigger_cmd SEMI */
          //#line 1200 "parse.y"
          {
            Debug.Assert( yymsp[-1].minor.yy473 != null );
            yymsp[-1].minor.yy473.pLast = yymsp[-1].minor.yy473;
            yygotominor.yy473 = yymsp[-1].minor.yy473;
          }
          //#line 3358 "parse.c"
          break;
        case 287: /* trnm ::= nm DOT nm */
          //#line 1212 "parse.y"
          {
            yygotominor.yy0 = yymsp[0].minor.yy0;
            sqlite3ErrorMsg( pParse,
                  "qualified table names are not allowed on INSERT, UPDATE, and DELETE " +
                  "statements within triggers" );
          }
          //#line 3368 "parse.c"
          break;
        case 289: /* tridxby ::= INDEXED BY nm */
          //#line 1224 "parse.y"
          {
            sqlite3ErrorMsg( pParse,
                  "the INDEXED BY clause is not allowed on UPDATE or DELETE statements " +
                  "within triggers" );
          }
          //#line 3377 "parse.c"
          break;
        case 290: /* tridxby ::= NOT INDEXED */
          //#line 1229 "parse.y"
          {
            sqlite3ErrorMsg( pParse,
                  "the NOT INDEXED clause is not allowed on UPDATE or DELETE statements " +
                  "within triggers" );
          }
          //#line 3386 "parse.c"
          break;
        case 291: /* trigger_cmd ::= UPDATE orconf trnm tridxby SET setlist where_opt */
          //#line 1242 "parse.y"
          { yygotominor.yy473 = sqlite3TriggerUpdateStep( pParse.db, yymsp[-4].minor.yy0, yymsp[-1].minor.yy14, yymsp[0].minor.yy132, yymsp[-5].minor.yy186 ); }
          //#line 3391 "parse.c"
          break;
        case 292: /* trigger_cmd ::= insert_cmd INTO trnm inscollist_opt VALUES LP itemlist RP */
          //#line 1247 "parse.y"
          { yygotominor.yy473 = sqlite3TriggerInsertStep( pParse.db, yymsp[-5].minor.yy0, yymsp[-4].minor.yy408, yymsp[-1].minor.yy14, null, yymsp[-7].minor.yy186 ); }
          //#line 3396 "parse.c"
          break;
        case 293: /* trigger_cmd ::= insert_cmd INTO trnm inscollist_opt select */
          //#line 1250 "parse.y"
          { yygotominor.yy473 = sqlite3TriggerInsertStep( pParse.db, yymsp[-2].minor.yy0, yymsp[-1].minor.yy408, null, yymsp[0].minor.yy3, yymsp[-4].minor.yy186 ); }
          //#line 3401 "parse.c"
          break;
        case 294: /* trigger_cmd ::= DELETE FROM trnm tridxby where_opt */
          //#line 1254 "parse.y"
          { yygotominor.yy473 = sqlite3TriggerDeleteStep( pParse.db, yymsp[-2].minor.yy0, yymsp[0].minor.yy132 ); }
          //#line 3406 "parse.c"
          break;
        case 295: /* trigger_cmd ::= select */
          //#line 1257 "parse.y"
          { yygotominor.yy473 = sqlite3TriggerSelectStep( pParse.db, yymsp[0].minor.yy3 ); }
          //#line 3411 "parse.c"
          break;
        case 296: /* expr ::= RAISE LP IGNORE RP */
          //#line 1260 "parse.y"
          {
            yygotominor.yy346.pExpr = sqlite3PExpr( pParse, TK_RAISE, 0, 0, 0 );
            if ( yygotominor.yy346.pExpr != null )
            {
              yygotominor.yy346.pExpr.affinity = (char)OE_Ignore;
            }
            yygotominor.yy346.zStart = yymsp[-3].minor.yy0.z;
            yygotominor.yy346.zEnd = yymsp[0].minor.yy0.z.Substring( yymsp[0].minor.yy0.n );
          }
          //#line 3423 "parse.c"
          break;
        case 297: /* expr ::= RAISE LP raisetype COMMA nm RP */
          //#line 1268 "parse.y"
          {
            yygotominor.yy346.pExpr = sqlite3PExpr( pParse, TK_RAISE, 0, 0, yymsp[-1].minor.yy0 );
            if ( yygotominor.yy346.pExpr != null )
            {
              yygotominor.yy346.pExpr.affinity = (char)yymsp[-3].minor.yy328;
            }
            yygotominor.yy346.zStart = yymsp[-5].minor.yy0.z;
            yygotominor.yy346.zEnd = yymsp[0].minor.yy0.z.Substring( yymsp[0].minor.yy0.n );
          }
          //#line 3435 "parse.c"
          break;
        case 298: /* raisetype ::= ROLLBACK */
          //#line 1279 "parse.y"
          { yygotominor.yy328 = OE_Rollback; }
          //#line 3440 "parse.c"
          break;
        case 300: /* raisetype ::= FAIL */
          //#line 1281 "parse.y"
          { yygotominor.yy328 = OE_Fail; }
          //#line 3445 "parse.c"
          break;
        case 301: /* cmd ::= DROP TRIGGER ifexists fullname */
          //#line 1286 "parse.y"
          {
            sqlite3DropTrigger( pParse, yymsp[0].minor.yy65, yymsp[-1].minor.yy328 );
          }
          //#line 3452 "parse.c"
          break;
        case 302: /* cmd ::= ATTACH database_kw_opt expr AS expr key_opt */
          //#line 1293 "parse.y"
          {
            sqlite3Attach( pParse, yymsp[-3].minor.yy346.pExpr, yymsp[-1].minor.yy346.pExpr, yymsp[0].minor.yy132 );
          }
          //#line 3459 "parse.c"
          break;
        case 303: /* cmd ::= DETACH database_kw_opt expr */
          //#line 1296 "parse.y"
          {
            sqlite3Detach( pParse, yymsp[0].minor.yy346.pExpr );
          }
          //#line 3466 "parse.c"
          break;
        case 308: /* cmd ::= REINDEX */
          //#line 1311 "parse.y"
          { sqlite3Reindex( pParse, 0, 0 ); }
          //#line 3471 "parse.c"
          break;
        case 309: /* cmd ::= REINDEX nm dbnm */
          //#line 1312 "parse.y"
          { sqlite3Reindex( pParse, yymsp[-1].minor.yy0, yymsp[0].minor.yy0 ); }
          //#line 3476 "parse.c"
          break;
        case 310: /* cmd ::= ANALYZE */
          //#line 1317 "parse.y"
          { sqlite3Analyze( pParse, 0, 0 ); }
          //#line 3481 "parse.c"
          break;
        case 311: /* cmd ::= ANALYZE nm dbnm */
          //#line 1318 "parse.y"
          { sqlite3Analyze( pParse, yymsp[-1].minor.yy0, yymsp[0].minor.yy0 ); }
          //#line 3486 "parse.c"
          break;
        case 312: /* cmd ::= ALTER TABLE fullname RENAME TO nm */
          //#line 1323 "parse.y"
          {
            sqlite3AlterRenameTable( pParse, yymsp[-3].minor.yy65, yymsp[0].minor.yy0 );
          }
          //#line 3493 "parse.c"
          break;
        case 313: /* cmd ::= ALTER TABLE add_column_fullname ADD kwcolumn_opt column */
          //#line 1326 "parse.y"
          {
            sqlite3AlterFinishAddColumn( pParse, yymsp[0].minor.yy0 );
          }
          //#line 3500 "parse.c"
          break;
        case 314: /* add_column_fullname ::= fullname */
          //#line 1329 "parse.y"
          {
            pParse.db.lookaside.bEnabled = 0;
            sqlite3AlterBeginAddColumn( pParse, yymsp[0].minor.yy65 );
          }
          //#line 3508 "parse.c"
          break;
        case 317: /* cmd ::= create_vtab */
          //#line 1339 "parse.y"
          { sqlite3VtabFinishParse( pParse, 0 ); }
          //#line 3513 "parse.c"
          break;
        case 318: /* cmd ::= create_vtab LP vtabarglist RP */
          //#line 1340 "parse.y"
          { sqlite3VtabFinishParse( pParse, yymsp[0].minor.yy0 ); }
          //#line 3518 "parse.c"
          break;
        case 319: /* create_vtab ::= createkw VIRTUAL TABLE nm dbnm USING nm */
          //#line 1341 "parse.y"
          {
            sqlite3VtabBeginParse( pParse, yymsp[-3].minor.yy0, yymsp[-2].minor.yy0, yymsp[0].minor.yy0 );
          }
          //#line 3525 "parse.c"
          break;
        case 322: /* vtabarg ::= */
          //#line 1346 "parse.y"
          { sqlite3VtabArgInit( pParse ); }
          //#line 3530 "parse.c"
          break;
        case 324: /* vtabargtoken ::= ANY */
        case 325: /* vtabargtoken ::= lp anylist RP */ //yytestcase(yyruleno==325);
        case 326: /* lp ::= LP */ //yytestcase(yyruleno==326);
          //#line 1348 "parse.y"
          { sqlite3VtabArgExtend( pParse, yymsp[0].minor.yy0 ); }
          //#line 3537 "parse.c"
          break;
        default:
          /* (0) input ::= cmdlist */
          //yytestcase(yyruleno==0);
          /* (1) cmdlist ::= cmdlist ecmd */
          //yytestcase(yyruleno==1);
          /* (2) cmdlist ::= ecmd */
          //yytestcase(yyruleno==2);
          /* (3) ecmd ::= SEMI */
          //yytestcase(yyruleno==3);
          /* (4) ecmd ::= explain cmdx SEMI */
          //yytestcase(yyruleno==4);
          /* (10) trans_opt ::= */
          //yytestcase(yyruleno==10);
          /* (11) trans_opt ::= TRANSACTION */
          //yytestcase(yyruleno==11);
          /* (12) trans_opt ::= TRANSACTION nm */
          //yytestcase(yyruleno==12);
          /* (20) savepoint_opt ::= SAVEPOINT */
          //yytestcase(yyruleno==20);
          /* (21) savepoint_opt ::= */
          //yytestcase(yyruleno==21);
          /* (25) cmd ::= create_table create_table_args */
          //yytestcase(yyruleno==25);
          /* (34) columnlist ::= columnlist COMMA column */
          //yytestcase(yyruleno==34);
          /* (35) columnlist ::= column */
          //yytestcase(yyruleno==35);
          /* (44) type ::= */
          //yytestcase(yyruleno==44);
          /* (51) signed ::= plus_num */
          //yytestcase(yyruleno==51);
          /* (52) signed ::= minus_num */
          //yytestcase(yyruleno==52);
          /* (53) carglist ::= carglist carg */
          //yytestcase(yyruleno==53);
          /* (54) carglist ::= */
          //yytestcase(yyruleno==54);
          /* (55) carg ::= CONSTRAINT nm ccons */
          //yytestcase(yyruleno==55);
          /* (56) carg ::= ccons */
          //yytestcase(yyruleno==56);
          /* (62) ccons ::= NULL onconf */
          //yytestcase(yyruleno==62);
          /* (90) conslist ::= conslist COMMA tcons */
          //yytestcase(yyruleno==90);
          /* (91) conslist ::= conslist tcons */
          //yytestcase(yyruleno==91);
          /* (92) conslist ::= tcons */
          //yytestcase(yyruleno==92);
          /* (93) tcons ::= CONSTRAINT nm */
          //yytestcase(yyruleno==93);
          /* (269) plus_opt ::= PLUS */
          //yytestcase(yyruleno==269);
          /* (270) plus_opt ::= */
          //yytestcase(yyruleno==270);
          /* (280) foreach_clause ::= */
          //yytestcase(yyruleno==280);
          /* (281) foreach_clause ::= FOR EACH ROW */
          //yytestcase(yyruleno==281);
          /* (288) tridxby ::= */
          //yytestcase(yyruleno==288);
          /* (306) database_kw_opt ::= DATABASE */
          //yytestcase(yyruleno==306);
          /* (307) database_kw_opt ::= */
          //yytestcase(yyruleno==307);
          /* (315) kwcolumn_opt ::= */
          //yytestcase(yyruleno==315);
          /* (316) kwcolumn_opt ::= COLUMNKW */
          //yytestcase(yyruleno==316);
          /* (320) vtabarglist ::= vtabarg */
          //yytestcase(yyruleno==320);
          /* (321) vtabarglist ::= vtabarglist COMMA vtabarg */
          //yytestcase(yyruleno==321);
          /* (323) vtabarg ::= vtabarg vtabargtoken */
          //yytestcase(yyruleno==323);
          /* (327) anylist ::= */
          //yytestcase(yyruleno==327);
          /* (328) anylist ::= anylist LP anylist RP */
          //yytestcase(yyruleno==328);
          /* (329) anylist ::= anylist ANY */
          //yytestcase(yyruleno==329);
          break;
      };
      yygoto = yyRuleInfo[yyruleno].lhs;
      yysize = yyRuleInfo[yyruleno].nrhs;
      yypParser.yyidx -= yysize;
      yyact = yy_find_reduce_action( yymsp[-yysize].stateno, (YYCODETYPE)yygoto );
      if ( yyact < YYNSTATE )
      {
#if NDEBUG
/* If we are not debugging and the reduce action popped at least
** one element off the stack, then we can push the new element back
** onto the stack here, and skip the stack overflow test in yy_shift().
** That gives a significant speed improvement. */
if( yysize!=0 ){
yypParser.yyidx++;
yymsp._yyidx -= yysize - 1;
yymsp[0].stateno = (YYACTIONTYPE)yyact;
yymsp[0].major = (YYCODETYPE)yygoto;
yymsp[0].minor = yygotominor;
}else
#endif
        {
          yy_shift( yypParser, yyact, yygoto, yygotominor );
        }
      }
      else
      {
        Debug.Assert( yyact == YYNSTATE + YYNRULE + 1 );
        yy_accept( yypParser );
      }
    }

    /*
    ** The following code executes when the parse fails
    */
#if !YYNOERRORRECOVERY
    static void yy_parse_failed(
    yyParser yypParser           /* The parser */
    )
    {
      Parse pParse = yypParser.pParse; //       sqlite3ParserARG_FETCH;
#if !NDEBUG
      if ( yyTraceFILE != null )
      {
        Debugger.Break(); // TODO --        fprintf(yyTraceFILE, "%sFail!\n", yyTracePrompt);
      }
#endif
      while ( yypParser.yyidx >= 0 ) yy_pop_parser_stack( yypParser );
      /* Here code is inserted which will be executed whenever the
      ** parser fails */
      yypParser.pParse = pParse;//      sqlite3ParserARG_STORE; /* Suppress warning about unused %extra_argument variable */
    }
#endif //* YYNOERRORRECOVERY */

    /*
** The following code executes when a syntax error first occurs.
*/
    static void yy_syntax_error(
    yyParser yypParser,           /* The parser */
    int yymajor,                   /* The major type of the error token */
    YYMINORTYPE yyminor            /* The minor type of the error token */
    )
    {
      Parse pParse = yypParser.pParse; //       sqlite3ParserARG_FETCH;
      //#define TOKEN (yyminor.yy0)
      //#line 32 "parse.y"

      UNUSED_PARAMETER( yymajor );  /* Silence some compiler warnings */
      Debug.Assert( yyminor.yy0.z.Length > 0 ); //TOKEN.z[0]);  /* The tokenizer always gives us a token */
      sqlite3ErrorMsg( pParse, "near \"%T\": syntax error", yyminor.yy0 );//&TOKEN);
      pParse.parseError = 1;
      //#line 3645 "parse.c"
      yypParser.pParse = pParse; // sqlite3ParserARG_STORE; /* Suppress warning about unused %extra_argument variable */
    }

    /*
    ** The following is executed when the parser accepts
    */
    static void yy_accept(
    yyParser yypParser           /* The parser */
    )
    {
      Parse pParse = yypParser.pParse; //       sqlite3ParserARG_FETCH;
#if !NDEBUG
      if ( yyTraceFILE != null )
      {
        fprintf( yyTraceFILE, "%sAccept!\n", yyTracePrompt );
      }
#endif
      while ( yypParser.yyidx >= 0 ) yy_pop_parser_stack( yypParser );
      /* Here code is inserted which will be executed whenever the
      ** parser accepts */
      yypParser.pParse = pParse;//      sqlite3ParserARG_STORE; /* Suppress warning about unused %extra_argument variable */
    }

    /* The main parser program.
    ** The first argument is a pointer to a structure obtained from
    ** "sqlite3ParserAlloc" which describes the current state of the parser.
    ** The second argument is the major token number.  The third is
    ** the minor token.  The fourth optional argument is whatever the
    ** user wants (and specified in the grammar) and is available for
    ** use by the action routines.
    **
    ** Inputs:
    ** <ul>
    ** <li> A pointer to the parser (an opaque structure.)
    ** <li> The major token number.
    ** <li> The minor token number.
    ** <li> An option argument of a grammar-specified type.
    ** </ul>
    **
    ** Outputs:
    ** None.
    */
    static void sqlite3Parser(
    yyParser yyp,                   /* The parser */
    int yymajor,                     /* The major token code number */
    sqlite3ParserTOKENTYPE yyminor  /* The value for the token */
    , Parse pParse //sqlite3ParserARG_PDECL           /* Optional %extra_argument parameter */
    )
    {
      YYMINORTYPE yyminorunion = new YYMINORTYPE();
      int yyact;            /* The parser action. */
      bool yyendofinput;     /* True if we are at the end of input */
#if YYERRORSYMBOL
int yyerrorhit = 0;   /* True if yymajor has invoked an error */
#endif
      yyParser yypParser;  /* The parser */

      /* (re)initialize the parser, if necessary */
      yypParser = yyp;
      if ( yypParser.yyidx < 0 )
      {
#if YYSTACKDEPTH//<=0
if( yypParser.yystksz <=0 ){
memset(yyminorunion, 0, yyminorunion).Length;
yyStackOverflow(yypParser, yyminorunion);
return;
}
#endif
        yypParser.yyidx = 0;
        yypParser.yyerrcnt = -1;
        yypParser.yystack[0] = new yyStackEntry();
        yypParser.yystack[0].stateno = 0;
        yypParser.yystack[0].major = 0;
      }
      yyminorunion.yy0 = yyminor.Copy();
      yyendofinput = ( yymajor == 0 );
      yypParser.pParse = pParse;//      sqlite3ParserARG_STORE;

#if !NDEBUG
      if ( yyTraceFILE != null )
      {
        fprintf( yyTraceFILE, "%sInput %s\n", yyTracePrompt, yyTokenName[yymajor] );
      }
#endif

      do
      {
        yyact = yy_find_shift_action( yypParser, (YYCODETYPE)yymajor );
        if ( yyact < YYNSTATE )
        {
          Debug.Assert( !yyendofinput );  /* Impossible to shift the $ token */
          yy_shift( yypParser, yyact, yymajor, yyminorunion );
          yypParser.yyerrcnt--;
          yymajor = YYNOCODE;
        }
        else if ( yyact < YYNSTATE + YYNRULE )
        {
          yy_reduce( yypParser, yyact - YYNSTATE );
        }
        else
        {
          Debug.Assert( yyact == YY_ERROR_ACTION );
#if YYERRORSYMBOL
int yymx;
#endif
#if !NDEBUG
          if ( yyTraceFILE != null )
          {
            Debugger.Break(); // TODO --            fprintf(yyTraceFILE, "%sSyntax Error!\n", yyTracePrompt);
          }
#endif
#if YYERRORSYMBOL
/* A syntax error has occurred.
** The response to an error depends upon whether or not the
** grammar defines an error token "ERROR".
**
** This is what we do if the grammar does define ERROR:
**
**  * Call the %syntax_error function.
**
**  * Begin popping the stack until we enter a state where
**    it is legal to shift the error symbol, then shift
**    the error symbol.
**
**  * Set the error count to three.
**
**  * Begin accepting and shifting new tokens.  No new error
**    processing will occur until three tokens have been
**    shifted successfully.
**
*/
if( yypParser.yyerrcnt<0 ){
yy_syntax_error(yypParser,yymajor,yyminorunion);
}
yymx = yypParser.yystack[yypParser.yyidx].major;
if( yymx==YYERRORSYMBOL || yyerrorhit ){
#if !NDEBUG
if( yyTraceFILE ){
Debug.Assert(false); // TODO --                      fprintf(yyTraceFILE,"%sDiscard input token %s\n",
yyTracePrompt,yyTokenName[yymajor]);
}
#endif
yy_destructor(yypParser,(YYCODETYPE)yymajor,yyminorunion);
yymajor = YYNOCODE;
}else{
while(
yypParser.yyidx >= 0 &&
yymx != YYERRORSYMBOL &&
(yyact = yy_find_reduce_action(
yypParser.yystack[yypParser.yyidx].stateno,
YYERRORSYMBOL)) >= YYNSTATE
){
yy_pop_parser_stack(yypParser);
}
if( yypParser.yyidx < 0 || yymajor==0 ){
yy_destructor(yypParser, (YYCODETYPE)yymajor,yyminorunion);
yy_parse_failed(yypParser);
yymajor = YYNOCODE;
}else if( yymx!=YYERRORSYMBOL ){
YYMINORTYPE u2;
u2.YYERRSYMDT = 0;
yy_shift(yypParser,yyact,YYERRORSYMBOL,u2);
}
}
yypParser.yyerrcnt = 3;
yyerrorhit = 1;
#elif (YYNOERRORRECOVERY)
/* If the YYNOERRORRECOVERY macro is defined, then do not attempt to
** do any kind of error recovery.  Instead, simply invoke the syntax
** error routine and continue going as if nothing had happened.
**
** Applications can set this macro (for example inside %include) if
** they intend to abandon the parse upon the first syntax error seen.
*/
yy_syntax_error(yypParser,yymajor,yyminorunion);
yy_destructor(yypParser,(YYCODETYPE)yymajor,yyminorunion);
yymajor = YYNOCODE;
#else  // * YYERRORSYMBOL is not defined */
          /* This is what we do if the grammar does not define ERROR:
**
**  * Report an error message, and throw away the input token.
**
**  * If the input token is $, then fail the parse.
**
** As before, subsequent error messages are suppressed until
** three input tokens have been successfully shifted.
*/
          if ( yypParser.yyerrcnt <= 0 )
          {
            yy_syntax_error( yypParser, yymajor, yyminorunion );
          }
          yypParser.yyerrcnt = 3;
          yy_destructor( yypParser, (YYCODETYPE)yymajor, yyminorunion );
          if ( yyendofinput )
          {
            yy_parse_failed( yypParser );
          }
          yymajor = YYNOCODE;
#endif
        }
      } while ( yymajor != YYNOCODE && yypParser.yyidx >= 0 );
      return;
    }
    public class yymsp
    {
      public yyParser _yyParser;
      public int _yyidx;
      // CONSTRUCTOR
      public yymsp( ref yyParser pointer_to_yyParser, int yyidx ) //' Parser and Stack Index
      {
        this._yyParser = pointer_to_yyParser;
        this._yyidx = yyidx;
      }
      // Default Value
      public yyStackEntry this[int offset]
      {
        get { return _yyParser.yystack[_yyidx + offset]; }
      }
    }
  }
}
