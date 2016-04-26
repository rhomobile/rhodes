// parse errors
#define E_BAD_CHAR						-1
#define E_NUM_TOO_LONG					-2
#define E_TOKEN_TOO_LONG				-3
#define E_MISSING_QUOTES				-4
#define E_MISPLACED_MINUS				-5
#define E_MISSING_R_VAL					-6
#define E_SYNTAX_ERR						-7
#define E_MISSING_CONDITION			-8
#define E_FILE_ERR						-9
#define E_PARSE_ERR						-10
#define E_MISSING_TAG					-11

// other
#define E_WHERE_FIELD_NOT_FOUND		-20
#define E_SORT_FIELD_NOT_FOUND		-21
#define E_UNKNOWN_FILE_FORMAT			-22

// Errors from CEvaluateResult
#define E_INVALID_RET_E					-30
#define E_OR_STRING						-31
#define E_AND_STRING						-32
#define E_EQ_TYPE							-33

#define E_CODE_ERROR						-40

// Errors returned from activeX component
#define E_INVALID_INDEX					-50
#define E_INVALID_COLUMN_NAME			-51

#define E_NO_RESULT_SET					-52

#define E_INVALID_STAT_PARAM			-53
#define E_INVALID_FORMAT_PARAM		-54
#define E_INVALID_DELIMITER_PARAM	-55
#define E_INVALID_FIRSTROW_PARAM		-56

//-----------------------------------------------------------------------------

void ClearError(void);
int StoreError(int err, char *format, ...);
int ReturnErr(void);
char *ReturnErrStr(void);
