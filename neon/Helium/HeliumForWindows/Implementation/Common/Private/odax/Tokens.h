// types (items)

// types (items)
#define TOKEN_END					1	// virtual token - end of script
#define TOKEN_UNA					2	// eg -
#define TOKEN_BIN					3	// eg +
#define TOKEN_L_BKT				4	// (
#define TOKEN_R_BKT				5	// )
#define TOKEN_N_VAR				6	// eg n%
#define TOKEN_S_VAR				7	// eg s$
#define TOKEN_N_CONST			8	//	eg 1
#define TOKEN_S_CONST			9	// eg "hello"
#define TOKEN_IDNT				14
#define TOKEN_SEP					17	// ,
#define TOKEN_L_CURLY_BKT		31	// block start
#define TOKEN_R_CURLY_BKT		32	// block end

#define TOKEN_SELECT				41	// SELECT
#define TOKEN_FROM				42	// FROM
#define TOKEN_WHERE				43	// WHERE
#define TOKEN_INSERT				44	// INSERT
#define TOKEN_INTO				45	// INTO
#define TOKEN_VALUES				46	// VALUES
#define TOKEN_UPDATE				50 // UPDATE
#define TOKEN_SET					51 // SET
#define TOKEN_DELETE				52 // DELETE
#define TOKEN_ORDER				53	// ORDER
#define TOKEN_BY					54	// BY
#define TOKEN_ASC					55	// ASC
#define TOKEN_DESC				56	// DESC
#define TOKEN_STATEMENT_END	60	// end of the SQL statement


//subtypes (values)
#define UOP_NONE			0
#define UOP_INC			1	// ++
#define UOP_DEC			2	// --
#define UOP_NOT			3	// not
#define BOP_ADD			4	// and
#define BOP_SUB			5	// -
#define BOP_MLT			6	// *
#define BOP_DIV			7	// /
#define BOP_MOD			8	// %
#define BOP_EQU			9	// =
#define BOP_AND			10	// and
#define BOP_OR				11	// or
#define BOP_LT				12	// <
#define BOP_LTE			13 // <=
#define BOP_GT				14 // >
#define BOP_GTE			15 // >=
#define BOP_NE				16 // <>

#define MAX_TOKEN_LEN	1024