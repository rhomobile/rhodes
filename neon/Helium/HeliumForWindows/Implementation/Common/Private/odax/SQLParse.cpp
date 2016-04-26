// SQLParse.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

int StatementPos, StatementLen;
char *Statement;

int LexItem, LexValue;
char LexString[MAX_TOKEN_LEN + 1];

// Keywords table
struct TKeyword
	{
	char	*Name;
	int	LexItem;
	int	LexValue;
	} Keywords[] =	{	{"SELECT",			TOKEN_SELECT,	0},
							{"FROM",				TOKEN_FROM,		0},
							{"WHERE",			TOKEN_WHERE,	0},
							{"INSERT",			TOKEN_INSERT,	0},
							{"INTO",				TOKEN_INTO,		0},
							{"VALUES",			TOKEN_VALUES,	0},
							{"UPDATE",			TOKEN_UPDATE,	0},
							{"SET",				TOKEN_SET,		0},
							{"DELETE",			TOKEN_DELETE,	0},
							{"ORDER",			TOKEN_ORDER,	0},
							{"BY",				TOKEN_BY,		0},
							{"ASC",				TOKEN_ASC,		0},
							{"DESC",				TOKEN_DESC,		0},
							{"OR",				TOKEN_BIN,		BOP_OR},
							{"AND",				TOKEN_BIN,		BOP_AND}
		};


CResultSet *pResultSet;
int RecordsAffected;

//-----------------------------------------------------------------------------

static int ParseSelect(int format, char delimiter, int firstrow);
static int HandleSelectEnd(CStatementData *p_statement_data, int format);
static int ParseInsert(int format, char delimiter, int firstrow);
static int ParseUpdate(int format, char delimiter, int firstrow);
static int ParseDelete(int format, char delimiter, int firstrow);

char GetNextChar(void);
void RewindChar(void);
void GetNextToken(void);
void ReadInString(char *word);
static bool IsKeyword(char *word, int *lex_item, int *lex_value);
char *SafeStrNCpy(char *dest, char *src, int len);
CExpr *ParseExpression(void);
CExpr *ParsePrimary(void);
static enumInputType GetInputType(CStatementData *p_statement_data, int format);

//-----------------------------------------------------------------------------

int DoSelect(char *statement, char delimiter, int firstrow)
	{
	int ret = 0;
	Statement = statement;
	StatementPos = 0;
	StatementLen = strlen(Statement);

	GetNextToken();

	ret = ParseSelect(DETERMINE_FORMAT_FROM_FILE, delimiter, firstrow);

	if (ret)
		return ret;

	return pResultSet->GetNRecs();
	}

//-----------------------------------------------------------------------------

int DoExecute(char *statement, int format, char delimiter, int firstrow)
	{
	Statement = statement;
	StatementPos = 0;
	StatementLen = strlen(Statement);
	int ret = 0;

	RecordsAffected = 0;

	GetNextToken();

	switch(LexItem)
		{
		case TOKEN_INSERT:
			ret = ParseInsert(format, delimiter, firstrow);
			break;

		case TOKEN_UPDATE:
			ret = ParseUpdate(format, delimiter, firstrow);
			break;

		case TOKEN_DELETE:
			ret = ParseDelete(format, delimiter, firstrow);
			break;

		case TOKEN_SELECT:
			ret = StoreError(E_SYNTAX_ERR, "SELECT commands handled by Select function.");
			// throw Exception(E_SYNTAX_ERR, "SELECT commands handled by Select function.");
			break;
		}

	if (ret)
		return ret;

	return RecordsAffected;
	}

//-----------------------------------------------------------------------------

#define SEL_ST_SELECT		1	// got SELECT, checking for FROM
#define SEL_ST_FROM			2	// got FROM, checking for table
#define SEL_ST_TABLE			3	// got table, checking for WHERE
#define SEL_ST_WHERE			4	// got WHERE, checking for condition
#define SEL_ST_CONDITION	5	// got condition, checking for ORDER
#define SEL_ORDER				6	// got ORDER, checking for BY
#define SEL_BY					7	// got BY, checking for sort field
#define SEL_SORT_FIELD		8	// got sort field, checking for sort order (ASC or DESC)
//#define SEL_SORT_ORDER		9	// got sort order, checking for end
//#define SEL_ST_END			10

static int ParseSelect(int format, char delimiter, int firstrow)
	{
	int state = SEL_ST_SELECT;
	bool done = false;
	CString sort_field;
	bool got_sort_order = false;
	int ret = 0;

	CRecord current_result;
	CStatementData statement_data;
	memset(&statement_data, 0, sizeof (CStatementData));
	statement_data.pCurrentResult = &current_result;
	statement_data.StatementType = SQL_SELECT;
	statement_data.OutputType = RESULT_SET_OUTPUT;
	// the next 2 should only contain valid data, when processing CSV files...
	statement_data.Delimiter = delimiter;
	statement_data.FirstRow = firstrow;

	// Make a new result set object
	if (pResultSet)
		{
		delete pResultSet;
		pResultSet = NULL;
		}

	pResultSet = new CResultSet;

	GetNextToken();

//	printf ("Parsing SELECT...\n");

	while (!done)
		{
		switch (state)
			{
			case SEL_ST_SELECT: // got select
				if ((LexItem == TOKEN_BIN) && (LexValue == BOP_MLT))
					{
					// All fields.
					GetNextToken();
					}
				else if (LexItem == TOKEN_SEP)
					{
					// Fine, go round again
					GetNextToken();
					}
				else if (LexItem == TOKEN_FROM)
					{
					state = SEL_ST_FROM;
					GetNextToken();
					}
				else
//					throw Exception(E_SYNTAX_ERR, "Field name, ',' or FROM expected at pos %d", StatementPos);
					{
					ret = StoreError(E_SYNTAX_ERR, "Field name, ',' or FROM expected at pos %d", StatementPos);
					goto Exit;
					}

				break;

			case SEL_ST_FROM: // got from, checking for table
				if (LexItem == TOKEN_IDNT || LexItem == TOKEN_S_CONST)
					{
//					printf ("Table = %s\n", LexString);

					// Its the table name, store it
					statement_data.SourceDbName.SetStr(LexString);
					state = SEL_ST_TABLE;
					GetNextToken();
					}
				else
//					throw Exception(E_SYNTAX_ERR, "Table name expected at pos %d", StatementPos);
					{
					ret = StoreError(E_SYNTAX_ERR, "Table name expected at pos %d", StatementPos);
					goto Exit;
					}
				break;

			case SEL_ST_TABLE: // got table, checking for where
				if (LexItem == TOKEN_WHERE)
					{
					state = SEL_ST_WHERE;
					GetNextToken();
					}
				else if (LexItem == TOKEN_ORDER)
					{
					statement_data.pCondition = NULL;
					state = SEL_ORDER;
					GetNextToken();
					}
				else if (LexItem == TOKEN_STATEMENT_END)
					{
					// all done
					statement_data.pCondition = NULL;
					done = true;
					ret = HandleSelectEnd(&statement_data, format);
					}
				else
//					throw Exception(E_SYNTAX_ERR, "WHERE or ';' expected at pos %d", StatementPos);
					{
					ret = StoreError(E_SYNTAX_ERR, "WHERE or ';' expected at pos %d", StatementPos);
					goto Exit;
					}
				break;

			case SEL_ST_WHERE: // got where, checking for condition
				statement_data.pCondition = ParseExpression();
				state = SEL_ST_CONDITION;
				break;

			case SEL_ST_CONDITION: // got condition, checking for ORDER
				if (LexItem == TOKEN_STATEMENT_END)
					{
					done = true;
					ret = HandleSelectEnd(&statement_data, format);
					}
				else if (LexItem == TOKEN_ORDER)
					{
					state = SEL_ORDER;
					GetNextToken();
					}
				else
//					throw Exception(E_SYNTAX_ERR, "ORDER or ';' expected at pos %d", StatementPos);
					{
					ret = StoreError(E_SYNTAX_ERR, "ORDER or ';' expected at pos %d", StatementPos);
					goto Exit;
					}
				break;

			case SEL_ORDER: // got ORDER, checking for BY
				if (LexItem == TOKEN_BY)
					{
					state = SEL_BY;
					GetNextToken();
					}
				else
//					throw Exception(E_SYNTAX_ERR, "BY expected at pos %d", StatementPos);
					{
					ret = StoreError(E_SYNTAX_ERR, "BY expected at pos %d", StatementPos);
					goto Exit;
					}
				break;

			case SEL_BY: // got BY, checking for sort field
				if (LexItem == TOKEN_IDNT || LexItem == TOKEN_S_CONST)
					{
					sort_field.SetStr(LexString);
					state = SEL_SORT_FIELD;
					GetNextToken();
					got_sort_order = false;
					}
				else
//					throw Exception(E_SYNTAX_ERR, "Sort field expected at pos %d", StatementPos);
					{
					ret = StoreError(E_SYNTAX_ERR, "Sort field expected at pos %d", StatementPos);
					goto Exit;
					}
				break;

			case SEL_SORT_FIELD: // got sort field, checking for sort order (ASC or DESC)
				if (LexItem == TOKEN_ASC)
					{
					pResultSet->AddSortField(sort_field.GetStr(), FALSE);
					GetNextToken();
					got_sort_order = true;
					}
				else if (LexItem == TOKEN_DESC)
					{
					pResultSet->AddSortField(sort_field.GetStr(), TRUE);
					GetNextToken();
					got_sort_order = true;
					}
				else if (LexItem == TOKEN_SEP)
					{
					if (!got_sort_order)
						{
						// No order specified default to ASC
						pResultSet->AddSortField(sort_field.GetStr(), FALSE);
						got_sort_order = false;
						}
						
					state = SEL_BY;
					GetNextToken();
					}
				else if (LexItem == TOKEN_STATEMENT_END)
					{
					if (!got_sort_order)
						{
						// No order specified default to ASC
						pResultSet->AddSortField(sort_field.GetStr(), FALSE);
						got_sort_order = false;
						}

					// all done
					done = true;
					ret = HandleSelectEnd(&statement_data, format);
					}
				else
//					throw Exception(E_SYNTAX_ERR, "ASC, DESC, ',' or ';' expected at pos %d", StatementPos);
					{
					ret = StoreError(E_SYNTAX_ERR, "ASC, DESC, ',' or ';' expected at pos %d", StatementPos);
					goto Exit;
					}
				break;
			}
		}
Exit:
	if (ret)
		if (pResultSet)
			{
			delete pResultSet;
			pResultSet = NULL;
			}
		
	return ret;
	}

//-----------------------------------------------------------------------------

static int HandleSelectEnd(CStatementData *p_statement_data, int format)
	{
	int ret = 0;

	// Call ReadRecords() passing in table name, expression and field list
	enumInputType input_type = GetInputType(p_statement_data, format);
	if (input_type == XML_INPUT)
		ret = XML_ReadRecords(p_statement_data);
	else if (input_type == CSV_INPUT)
		ret = CSV_ReadRecords(p_statement_data);

	return ret;
	}

//-----------------------------------------------------------------------------

//char *Statement = "INSERT INTO table1 (field1, field2) VALUES (1, \"Hello\");";

#define INS_ST_INSERT		1	// got INSERT, checking for INTO
#define INS_ST_INTO			2	// got INTO, checking for table
#define INS_ST_TABLE			3	// got table, checking for fields
#define INS_ST_FIELDS		5	// got fields, checking for VALUES
#define INS_ST_VALUES		6	// got VALUES, checking for values
#define INS_ST_END			7	// got values, checking for end

static int ParseInsert(int format, char delimiter, int firstrow)
	{
	int state = INS_ST_INSERT;
	bool done = false;
	CStringList fields;
	CStringList values;
	int ret = 0;

	CStatementData statement_data;
	memset(&statement_data, 0, sizeof (CStatementData));
	statement_data.pCurrentResult = NULL;
	statement_data.StatementType = SQL_INSERT;
	statement_data.OutputType = RESULT_SET_OUTPUT;
	// the next 2 should only contain valid data, when processing CSV files...
	statement_data.Delimiter = delimiter;
	statement_data.FirstRow = firstrow;

	GetNextToken();

//	printf ("Parsing INSERT...\n");

	while (!done)
		{
		switch (state)
			{
			case INS_ST_INSERT:	// got INSERT, checking for INTO
				if (LexItem == TOKEN_INTO)
					{
					state = INS_ST_INTO;
					GetNextToken();
					}
				else
//					throw Exception(E_SYNTAX_ERR, "INTO expected at pos %d", StatementPos);
					{
					ret = StoreError(E_SYNTAX_ERR, "INTO expected at pos %d", StatementPos);
					goto Exit;
					}

				break;

			case INS_ST_INTO:		// got INTO, checking for table
				if ((LexItem == TOKEN_IDNT) || (LexItem == TOKEN_S_CONST))
					{
//					printf("Table = %s\n", LexString);

					// Store table name
					statement_data.SourceDbName.SetStr(LexString);
					state = INS_ST_TABLE;
					GetNextToken();
					}
				else
//					throw Exception(E_SYNTAX_ERR, "Table name expected at pos %d", StatementPos);
					{
					ret = StoreError(E_SYNTAX_ERR, "Table name expected at pos %d", StatementPos);
					goto Exit;
					}
				break;

			case INS_ST_TABLE:	// got table, checking for fields
				if (LexItem == TOKEN_VALUES)
					{
					// This is legal in SQL, but I can't do this unless I record the
					// structure of the database.
//					throw Exception(E_SYNTAX_ERR, "Field names expected at pos %d", StatementPos);
					ret = StoreError(E_SYNTAX_ERR, "Field names expected at pos %d", StatementPos);
					goto Exit;
					}
				else if (LexItem == TOKEN_L_BKT)
					{
					// fine go round again
					GetNextToken();
					}
				else if (LexItem == TOKEN_IDNT)
					{
//					printf ("Field = %s\n", LexString);

					// Field name, store it
					fields.Add(new CString(LexString));
					GetNextToken();
					}
				else if (LexItem == TOKEN_SEP)
					{
					// fine go round again
					GetNextToken();
					}
				else if (LexItem == TOKEN_R_BKT)
					{
					state = INS_ST_FIELDS;
					GetNextToken();
					}
				else
//					throw Exception(E_SYNTAX_ERR, "VALUES, '(', field or ',' expected at pos %d", StatementPos);
					{
					ret = StoreError(E_SYNTAX_ERR, "VALUES, '(', field or ',' expected at pos %d", StatementPos);
					goto Exit;
					}
				break;

			case INS_ST_FIELDS:	// got fields, checking for VALUES
				if (LexItem == TOKEN_VALUES)
					{
					state = INS_ST_VALUES;
					GetNextToken();
					}
				else
//					throw Exception(E_SYNTAX_ERR, "VALUES expected at pos %d", StatementPos);
					{
					ret = StoreError(E_SYNTAX_ERR, "VALUES expected at pos %d", StatementPos);
					goto Exit;
					}
				break;

			case INS_ST_VALUES:	// got VALUES, checking for values
				if (LexItem == TOKEN_L_BKT)
					{
					// fine go round again
					GetNextToken();
					}
				else if (LexItem == TOKEN_S_CONST)
					{
					// value, store it
					values.Add(new CString(LexString));
					GetNextToken();
					}
				else if (LexItem == TOKEN_SEP)
					{
					// fine go round again
					GetNextToken();
					}
				else if (LexItem == TOKEN_R_BKT)
					{
					state = INS_ST_END;
					GetNextToken();
					}
				else
//					throw Exception(E_SYNTAX_ERR, "'(', value or ',' expected at pos %d", StatementPos);
					{
					ret = StoreError(E_SYNTAX_ERR, "'(', value or ',' expected at pos %d", StatementPos);
					goto Exit;
					}
				break;

			case INS_ST_END:
				if (LexItem == TOKEN_STATEMENT_END)
					{
					// all done

					enumInputType input_type = GetInputType(&statement_data, format);
					if (input_type == XML_INPUT)
						{
						if ((ret = XML_WriteRecords(statement_data.SourceDbName.GetStr(), &fields, &values)) != 0)
							goto Exit;
						}
					else if (input_type == CSV_INPUT)
						{
						if ((ret = CSV_WriteRecords(statement_data.SourceDbName.GetStr(), &fields, &values, delimiter, firstrow)) != 0)
							goto Exit;
						}

					done = true;
					}
				else
//					throw Exception(E_SYNTAX_ERR, "';' expected at pos %d", StatementPos);
					{
					ret = StoreError(E_SYNTAX_ERR, "';' expected at pos %d", StatementPos);
					goto Exit;
					}
				break;
			}
		}

Exit:
	return ret;
	}

//-----------------------------------------------------------------------------

#define UPD_ST_UPDATE		1	// got UPDATE, checking for table
#define UPD_ST_TABLE			3	// got table, checking for SET
#define UPD_ST_SET			4	// got SET checking for field
#define UPD_ST_FIELD			5	// got field, checking for =
#define UPD_ST_EQ				6	// got equals checking for value
#define UPD_ST_VALUE			7  // got value, checking for WHERE
#define UPD_ST_WHERE			8	// got WHERE, checking for condition
#define UPD_ST_CONDITION	9	// got condition, checking for end
#define UPD_ST_END			10	// got end

static int ParseUpdate(int format, char delimiter, int firstrow)
	{
	int state = SEL_ST_SELECT;
	bool done = false;
	int ret = 0;

	CString field, value;

	CStatementData statement_data;
	memset(&statement_data, 0, sizeof (CStatementData));
	CRecord current_result;
	statement_data.pCurrentResult = &current_result;
	statement_data.StatementType = SQL_UPDATE;
	// the next 2 should only contain valid data, when processing CSV files...
	statement_data.Delimiter = delimiter;
	statement_data.FirstRow = firstrow;

//--------Added By Dilshan-------------
	if (pResultSet)
	{
		delete pResultSet;
		pResultSet = NULL;
	}

	pResultSet = new CResultSet;

//-------------------------------------
	GetNextToken();

//	printf ("Parsing UPDATE...\n");

	while (!done)
		{
		switch (state)
			{
			case UPD_ST_UPDATE:		// got UPDATE, checking for table
				if ((LexItem == TOKEN_IDNT) || (LexItem == TOKEN_S_CONST))
					{
//					printf("Table = %s\n", LexString);

					// Store table name
					statement_data.SourceDbName.SetStr(LexString);
					state = UPD_ST_TABLE;

					GetNextToken();
					}
				else
					throw Exception(E_SYNTAX_ERR, "Table name expected at pos %d", StatementPos);
				break;

			case UPD_ST_TABLE:		// got table, checking for SET.
				if (LexItem == TOKEN_SET)
					{
					state = UPD_ST_SET;
					GetNextToken();
					}
				else
					throw Exception(E_SYNTAX_ERR, "SET expected at pos %d", StatementPos);
				break;

			case UPD_ST_SET:			// got SET checking for field
				if (LexItem == TOKEN_IDNT)
					{
//					printf ("Field = %s\n", LexString);

					// Field name, store it
					field.SetStr(LexString);

					state = UPD_ST_FIELD;
					GetNextToken();
					}
				else
					throw Exception(E_SYNTAX_ERR, "Field expected at pos %d", StatementPos);
				break;

			case UPD_ST_FIELD:		// got field, checking for =
				if ((LexItem == TOKEN_BIN) && (LexValue == BOP_EQU))
					{
					state = UPD_ST_EQ;
					GetNextToken();
					}
				else
					throw Exception(E_SYNTAX_ERR, "'=' expected at pos %d", StatementPos);
				break;

			case UPD_ST_EQ:			// got equals checking for value
				if ((LexItem == TOKEN_IDNT) || (LexItem == TOKEN_S_CONST))
					{
//					printf ("Value = %s\n", LexString);

					statement_data.NewValues.AddField(field.GetStr(), LexString);

					state = UPD_ST_VALUE;
					GetNextToken();
					}
				else
					throw Exception(E_SYNTAX_ERR, "Value expected at pos %d", StatementPos);
				break;

			case UPD_ST_VALUE:		// got value, checking for WHERE
				if (LexItem == TOKEN_SEP)
					state = UPD_ST_SET;
				else if (LexItem == TOKEN_WHERE)
					{
					state = UPD_ST_WHERE;
					GetNextToken();
					}
				else if (LexItem == TOKEN_STATEMENT_END)
					{
					// all done
					statement_data.pCondition = NULL;

					enumInputType input_type = GetInputType(&statement_data, format);
					statement_data.OutputType = (input_type == XML_INPUT) ? XML_OUTPUT : CSV_OUTPUT;
					
					if (input_type == XML_INPUT)
						if ((ret = XML_ReadRecords(&statement_data)) != 0)
							goto Exit;
					else if (input_type == CSV_INPUT)
						if ((ret = CSV_ReadRecords(&statement_data)) != 0)
							goto Exit;
					
					done = true;
					}
				else
					throw Exception(E_SYNTAX_ERR, "WHERE or ';' expected at pos %d", StatementPos);
				break;

			case UPD_ST_WHERE:		// got WHERE, checking for condition
				statement_data.pCondition = ParseExpression();
				state = UPD_ST_END;
				break;

			case UPD_ST_END:	// got condition, checking for end
				if (LexItem == TOKEN_STATEMENT_END)
					{
					// all done

					enumInputType input_type = GetInputType(&statement_data, format);
					statement_data.OutputType = (input_type == XML_INPUT) ? XML_OUTPUT : CSV_OUTPUT;

					if (input_type == XML_INPUT)
						{
						if ((ret = XML_ReadRecords(&statement_data)) != 0)
							goto Exit;
						}
					else if (input_type == CSV_INPUT)
						{
						if ((ret = CSV_ReadRecords(&statement_data)) != 0)
							goto Exit;
						}

					done = true;
					}
				else
					throw Exception(E_SYNTAX_ERR, "';' expected at pos %d", StatementPos);
				break;
			}
		}
Exit:
//-----Added By Dilshan--------
	if(ret)
	{
		if (pResultSet)
		{
			delete pResultSet;
			pResultSet = NULL;
		}
	}
//-----------------------------
	return ret;
	}

//-----------------------------------------------------------------------------

#define DEL_ST_DELETE		1	// got DELETE, checking for FROM
#define DEL_ST_FROM			2	// got FROM, checking for table
#define DEL_ST_TABLE			3	// got table, checking for WHERE
#define DEL_ST_WHERE			4	// got WHERE, checking for condition
#define DEL_ST_CONDITION	5	// got condition, checking for end
#define DEL_ST_END			6

static int ParseDelete(int format, char delimiter, int firstrow)
	{
	int state = DEL_ST_DELETE;
	bool done = false;
	int ret = 0;
	
	CRecord current_result;
	CStatementData statement_data;
	memset(&statement_data, 0, sizeof (CStatementData));
	statement_data.pCurrentResult = &current_result;
	statement_data.StatementType = SQL_DELETE;
	// the next 2 should only contain valid data, when processing CSV files...
	statement_data.Delimiter = delimiter;
	statement_data.FirstRow = firstrow;
//--------Added By Dilshan-------------
	if (pResultSet)
	{
		delete pResultSet;
		pResultSet = NULL;
	}

	pResultSet = new CResultSet;

//-------------------------------------

	GetNextToken();

//	printf ("Parsing DELETE...\n");

	while (!done)
		{
		switch (state)
			{
			case DEL_ST_DELETE: // got DELETE, checking for FROM
				if (LexItem == TOKEN_FROM)
					{
					state = DEL_ST_FROM;
					GetNextToken();}
				else
					throw Exception(E_SYNTAX_ERR, "FROM expected at pos %d", StatementPos);
				break;

			case DEL_ST_FROM: // got FROM, checking for table

				if (LexItem == TOKEN_IDNT || LexItem == TOKEN_S_CONST)
					{
//					printf ("Table = %s\n", LexString);

					// Its the table name, store it
					statement_data.SourceDbName.SetStr(LexString);
					state = DEL_ST_TABLE;
					GetNextToken();
					}
				else
					throw Exception(E_SYNTAX_ERR, "Table name expected at pos %d", StatementPos);
				break;

			case DEL_ST_TABLE: // got table, checking for WHERE
				if (LexItem == TOKEN_WHERE)
					{
					state = DEL_ST_WHERE;
					GetNextToken();
					}
				else if (LexItem == TOKEN_STATEMENT_END)
					{
					statement_data.pCondition = NULL;

					enumInputType input_type = GetInputType(&statement_data, format);
					statement_data.OutputType = (input_type == XML_INPUT) ? XML_OUTPUT : CSV_OUTPUT;

					if (input_type == XML_INPUT)
						{
						if ((ret = XML_ReadRecords(&statement_data)) != 0)
							goto Exit;
						}
					else if (input_type == CSV_INPUT)
						{
						if ((ret = CSV_ReadRecords(&statement_data)) != 0)
							goto Exit;
						}
					
					done = true;
					}
				else
					throw Exception(E_SYNTAX_ERR, "WHERE or ';' expected at pos %d", StatementPos);
				break;

			case DEL_ST_WHERE: // got WHERE, checking for condition
				statement_data.pCondition = ParseExpression();
				state = DEL_ST_END;
				break;

			case DEL_ST_END:	// got condition, checking for end

				if (LexItem == TOKEN_STATEMENT_END)
					{
					enumInputType input_type = GetInputType(&statement_data, format);
					statement_data.OutputType = (input_type == XML_INPUT) ? XML_OUTPUT : CSV_OUTPUT;

					if (input_type == XML_INPUT)
						{
						if ((ret = XML_ReadRecords(&statement_data)) != 0)
							goto Exit;
						}
					else if (input_type == CSV_INPUT)
						{
						if ((ret = CSV_ReadRecords(&statement_data)) != 0)
							goto Exit;
						}

					done = true;
					}
				else
					throw Exception(E_SYNTAX_ERR, "';' expected at pos %d", StatementPos);
				break;
			}
		}
Exit:
//-----Added By Dilshan--------
	if(ret)
	{
		if (pResultSet)
		{
			delete pResultSet;
			pResultSet = NULL;
		}
	}
//-----------------------------
	return ret;
	}

//-----------------------------------------------------------------------------

char GetNextChar(void)
	{
	// We must always increment StatementPos even if we are at the end
	char ch;

	if (StatementPos < StatementLen)
		ch = Statement[StatementPos];
	else
		ch = -1;

	StatementPos++;

	return ch;
	}

//-----------------------------------------------------------------------------

void RewindChar(void)
	{
	if (StatementPos > 0)
		StatementPos--;
	}

//-----------------------------------------------------------------------------

void GetNextToken(void)
	{
	int state, word_pos;
	char word[MAX_TOKEN_LEN + 1];
	char ch;
	bool done;

	state = 0;
	word_pos = 0;
	done = false;

	do
		{
		ch = GetNextChar();

		switch (state)
		{
		case 0:
			switch(ch)
				{
				case -1:
					LexItem = TOKEN_END;
					LexValue = 0;
					done = true;
					break;

				case ';':
					LexItem = TOKEN_STATEMENT_END;
					LexValue = 0;
					done = true;
					break;

				case ',':
					LexItem = TOKEN_SEP;
					LexValue = 0;
					done = true;
					break;

				case '(':
					LexItem = TOKEN_L_BKT;
					LexValue = 0;
					done = true;
					break;

				case ')':
					LexItem = TOKEN_R_BKT;
					LexValue = 0;
					done = true;
					break;

				case '=':
					// Its an equality operator
					LexItem = TOKEN_BIN;
					LexValue = BOP_EQU;
					done = true;
					break;

				case '*':
					// Its a star (field wildcard)
					LexItem = TOKEN_BIN;
					LexValue = BOP_MLT;
					done = true;
					break;

				case '\'':
					// read in string until we get another '"' char
					ReadInString(word);
					LexItem = TOKEN_S_CONST;
					SafeStrNCpy(LexString, word, MAX_TOKEN_LEN);
					done = true;
					break;

				default:
					if (isdigit(ch))
						{
						//start building a number
						word[word_pos++] = ch;
						state = 1;//number state
						}
					else if (isalpha(ch))
						{
						//start building word
						word[word_pos++] = ch;
						state = 2;//alpha state
						}
					else if (!isspace(ch)) //ignore white space
						throw Exception(E_BAD_CHAR, "Bad character %c at pos %d", ch, StatementPos);
					break;
				}
			break;

		case 1://number state
			if (isdigit(ch))
				{
				if (word_pos < MAX_TOKEN_LEN)
					word[word_pos++] = ch;
				else
					throw Exception(E_NUM_TOO_LONG, "Number too long at pos %d", StatementPos);
				}
			else
				{
				word[word_pos] = 0;

				RewindChar();

				LexItem = TOKEN_N_CONST;
				LexValue = atoi(word);
				done = true;
				}
			break;

		case 2://alpha state
			if (isalnum(ch))
				{
				if (word_pos < MAX_TOKEN_LEN)
					word[word_pos++] = ch;
				else
					throw Exception(E_TOKEN_TOO_LONG, "Token too long at pos %d", StatementPos);
				}
			else
				{
				//our word has finished
				word[word_pos] = 0;

				//check for special following chars like '!', ':', '%' or '$'

				// Check for keywords such as "and", "if", "else", etc
				if (IsKeyword(word, &LexItem, &LexValue))
					done = true;

				else
				//must be either a procedure or an error
					{
					RewindChar();

					LexItem = TOKEN_IDNT;
					SafeStrNCpy(LexString, word, MAX_TOKEN_LEN);
					LexValue = 0;
					done = true;
					}
				}
			break;
			}
		}
	while (!done);
	}

//-----------------------------------------------------------------------------

void ReadInString(char *word)
	{
	int word_pos = 0;
	char ch = GetNextChar();

	while(ch != '\'')
		{
		word[word_pos++] = ch;
		ch = GetNextChar();

		if (ch == -1)
			throw Exception(E_MISSING_QUOTES, "No closing \" character at pos %d", StatementPos);
		}

	word[word_pos] = 0;
	}

//-----------------------------------------------------------------------------

static bool IsKeyword(char *word, int *lex_item, int *lex_value)
	{
	int ct;

	for (ct=0; ct<sizeof (Keywords) / sizeof (Keywords[0]); ct++)
		{
		if (!_stricmp(word, Keywords[ct].Name))
			{
			*lex_item = Keywords[ct].LexItem;
			*lex_value = Keywords[ct].LexValue;
			RewindChar();

			return true;
			}
		}

	return false;
	}

//-----------------------------------------------------------------------------

char *SafeStrNCpy(char *dest, char *src, int len)
	{
	strncpy(dest, src, len);
	dest[len-1] = 0;

	return dest;
	}

//-----------------------------------------------------------------------------

CExpr *ParseExpression(void)
	{
	int type;
	CExpr *lhs, *rhs;

	lhs = ParsePrimary ();
	if (lhs)
		while (LexItem == TOKEN_BIN)
			{
			type = LexValue;
			GetNextToken();
			rhs = ParseExpression();
			if (rhs)
				lhs = new CBinaryNode(type, lhs, rhs);
			}

	return lhs;
	}

//-----------------------------------------------------------------------------

CExpr *ParsePrimary(void)
	{
	CExpr *pnode;//, *primary;

	switch (LexItem)
		{
		case TOKEN_L_BKT:
			GetNextToken();
			pnode = ParseExpression();
			if (LexItem == TOKEN_R_BKT)
				GetNextToken ();
			else
				throw Exception(E_MISSING_R_VAL, "No right hand value for expression at pos %d", StatementPos);
			break;

		case TOKEN_IDNT:	// field name
			pnode = new CFieldNode(LexString);
			GetNextToken();
			break;

		case TOKEN_S_CONST: // value
			pnode = new CValueNode(LexString);
			GetNextToken();
			break;

		default:
			throw Exception(E_MISSING_CONDITION, "WHERE condition missing at pos %d", StatementPos);
			break;
		}

	return pnode;
	}

//-----------------------------------------------------------------------------

// Gets input type from filename extension.  If there is no extension, the
// format paramter is used.
static enumInputType GetInputType(CStatementData *p_statement_data, int format)
	{
	enumInputType type = XML_INPUT;
	char ext[_MAX_EXT];
	_splitpath(p_statement_data->SourceDbName.GetStr(), NULL, NULL, NULL, ext);

	// determine the output type from the input type.
	if (*ext == 0)
		{
		if (format == XML_INPUT_FORMAT)
			type = XML_INPUT;
		else if (format == CSV_INPUT_FORMAT)
			type = CSV_INPUT;
		else
			throw Exception(E_UNKNOWN_FILE_FORMAT, "Unknown file format.");
		}
	else if (!_stricmp(ext, ".xml"))
		type = XML_INPUT;
	else if (!_stricmp(ext, ".csv"))
		type =  CSV_INPUT;
	else
		throw Exception(E_UNKNOWN_FILE_FORMAT, "Unknown file format '%s'.", ext);

	return type;
	}