#include "StdAfx.h"

extern CResultSet *pResultSet;

//------------------------------------------------------------------------------

static void OutputToXml(CStatementData *p_statement_data);
static void OutputToCsv(CStatementData *p_statement_data);

//------------------------------------------------------------------------------

void OutputRecord(CStatementData *p_statement_data)
	{
	switch(p_statement_data->OutputType)
		{
		case XML_OUTPUT:
			OutputToXml(p_statement_data);
			break;

		case CSV_OUTPUT:
			OutputToCsv(p_statement_data);
			break;
		}
	}

//------------------------------------------------------------------------------

static void OutputToXml(CStatementData *p_statement_data)
	{
	char *attr, *value;
	DWORD written;
	CRecord *p_current_result = p_statement_data->pCurrentResult;

	if (p_statement_data->TempDb.FileWriteString(&written, "   <record ") == 0)
		throw Exception(E_FILE_ERR, "Error writing to file %s (%d)", p_statement_data->TempDb.GetName(), GetLastError());

//	printf("Writing entries to db...\n");

	for (unsigned int ct=0; ct<p_current_result->Fields.Count(); ct++)
		{
		attr = p_current_result->Fields[ct]->Name.GetStr();
		
		if (p_current_result->Fields.Count() < ct)
			value = NULL;
		else
			value = p_current_result->Fields[ct]->Value.GetStr();

//		printf ("Writing \"%s=\"%s\"\n", attr, value ? value : "");

		if (p_statement_data->TempDb.FileWriteString(&written, "%s=\"%s\"", attr, value ? value : "") == 0)
			throw Exception(E_FILE_ERR, "Error writing to file %s (%d)", p_statement_data->TempDb.GetName(), GetLastError());

		if (ct < p_current_result->Fields.Count() - 1)
			{
			if (p_statement_data->TempDb.FileWriteString(&written, " ") == 0)
				throw Exception(E_FILE_ERR, "Error writing to file %s (%d)", p_statement_data->TempDb.GetName(), GetLastError());
			}
		else
			if (p_statement_data->TempDb.FileWriteString(&written, "/>\r\n") == 0)
				throw Exception(E_FILE_ERR, "Error writing to file %s (%d)", p_statement_data->TempDb.GetName(), GetLastError());
		}
	}

//------------------------------------------------------------------------------

static void OutputToCsv(CStatementData *p_statement_data)
	{
	char *value;
	DWORD written;
	CRecord *p_current_result = p_statement_data->pCurrentResult;
//	printf("Writing entries to db...\n");
	for (unsigned int ct=0; ct<p_current_result->Fields.Count(); ct++)
		{
		if (p_current_result->Fields.Count() < ct)
			value = NULL;
		else
		{
			value = p_current_result->Fields[ct]->Value.GetStr();
		}
//		printf ("Writing value \"%s\"\n", value ? value : "");

		if (p_statement_data->TempDb.FileWriteString(&written, value ? value : "") == 0)
			throw Exception(E_FILE_ERR, "Error writing to file %s (%d)", p_statement_data->TempDb.GetName(), GetLastError());

		if (ct < p_current_result->Fields.Count() - 1)
			{
			if (p_statement_data->TempDb.FileWriteString(&written, "%c", p_statement_data->Delimiter) == 0)
				throw Exception(E_FILE_ERR, "Error writing to file %s (%d)", p_statement_data->TempDb.GetName(), GetLastError());
			}
		else
			if (p_statement_data->TempDb.FileWriteString(&written, "\r\n") == 0)
				throw Exception(E_FILE_ERR, "Error writing to file %s (%d)", p_statement_data->TempDb.GetName(), GetLastError());
		}
	}