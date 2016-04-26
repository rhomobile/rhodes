#include "StdAfx.h"

//------------------------------------------------------------------------------

extern CResultSet *pResultSet;
extern int RecordsAffected;

//------------------------------------------------------------------------------

static int WriteFirstRow(CStringList *pfields, CFile *pfile, char delimiter);
static void PerformPreRead(CFile *pfile, CStatementData *p_statement_data, CStringList *pfields);
static int RecordHandler(char *line_buf, CStatementData *p_statement_data, CStringList *pfields);
static void SetCurrentResultFromCsv(CStatementData *p_statement_data, char *line_buf, CStringList *pfields);

//------------------------------------------------------------------------------

int CSV_WriteRecords(char *table, CStringList *pfields, CStringList *pvalues, char delimiter, int first_row)
	{
	CFile file;
	DWORD written;
	char *value;
	unsigned int ct;
	int ret = 0;
	DWORD	dwRead = 0; 
	char	szTempBuffer[3];

	// open database
	if (file.FileCreate(table, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL) == 0)
//		throw Exception(E_FILE_ERR, "Error opening file %s (%d)", table, GetLastError());
		{
		ret = StoreError(E_FILE_ERR, "Error opening file %s (%d)", table, GetLastError());
		goto Exit;
		}

	if (file.FileGetSize() == 0)
		{
		file.SetPos(0, FILE_BEGIN);

		if (first_row)
			if ((ret = WriteFirstRow(pfields, &file, delimiter)) != 0)
				goto Exit;
		}
	else
		file.SetPos(0, FILE_END);

//	printf("Writing entries to CSV db...\n");

	for (ct=0; ct<pfields->Count(); ct++)
	{
		if (0==ct)// Added By Dilshan to fix a bug. without this allways record are appended to 
				  // the same line instead to a new line also to avoid repeating of \r\n consecutively
		{
			file.SetPos(-2,FILE_CURRENT);
			memset(szTempBuffer,0x00,sizeof(char)*3);
			file.FileRead(szTempBuffer,2,&dwRead,NULL);
			if (0 == strncmp(szTempBuffer,"\r\n",2))
			{
				file.SetPos(0, FILE_END);
			}
			else
			{
				file.SetPos(0, FILE_END);
				if (file.FileWriteString(&written, "\r\n") == 0)
				{
					ret = StoreError(E_FILE_ERR, "Error writing to file %s (%d)", table, GetLastError());
					goto Exit;
				}
			}
		}

		if (pvalues->Count() < ct)
			value = NULL;
		else
			value = pvalues->GetObPtr(ct)->GetStr();

//		printf ("Writing value \"%s\"\n", value ? value : "");

		if (file.FileWriteString(&written, value ? value : "") == 0)
			//throw Exception(E_FILE_ERR, "Error writing to file %s (%d)", table, GetLastError());
			{
			ret = StoreError(E_FILE_ERR, "Error writing to file %s (%d)", table, GetLastError());
			goto Exit;
			}

		if (ct < pfields->Count()-1)
		{
			if (file.FileWriteString(&written, "%c", delimiter) == 0)
//				throw Exception(E_FILE_ERR, "Error writing to file %s (%d)", table, GetLastError());
				{
				ret = StoreError(E_FILE_ERR, "Error writing to file %s (%d)", table, GetLastError());
				goto Exit;
			}
		}
//		else
//			if (file.FileWriteString(&written, "\r\n") == 0)
//				throw Exception(E_FILE_ERR, "Error writing to file %s (%d)", table, GetLastError());
//				{
//				ret = StoreError(E_FILE_ERR, "Error writing to file %s (%d)", table, GetLastError());
//				goto Exit;
//				}
	}//for

	RecordsAffected++;

Exit:
	return ret;
	}

//------------------------------------------------------------------------------

// Reads CSV records in and processes them...
int CSV_ReadRecords(CStatementData *p_statement_data)
	{
	HANDLE h = INVALID_HANDLE_VALUE;
	CFile database;
	DWORD written;

	char line_buf[_MAX_BUFF_LEN];
	char seps[2];
	int n;
	bool first_row = true;
	
	int ret = 0;

	seps[0] = p_statement_data->Delimiter;
	seps[1] = 0;

	CStringList *p_fields = new CStringList;

	// Open source file..
	if (database.FileCreate(p_statement_data->SourceDbName.GetStr(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL) == 0)
		//throw Exception(E_FILE_ERR, "Unable to open CSV database file %s (%d)", p_statement_data->SourceDbName.GetStr(), GetLastError());
		{
		ret = StoreError(E_FILE_ERR, "Unable to open CSV database file %s (%d)", p_statement_data->SourceDbName.GetStr(), GetLastError());
		goto Exit;
		}

	database.SetPos(0, FILE_BEGIN);

	// Pre-parse preparation
	switch(p_statement_data->OutputType)
		{
		case RESULT_SET_OUTPUT:
			// Pre-read file to find largest field, and create field list
			PerformPreRead(&database, p_statement_data, p_fields);

			pResultSet->AddFieldList(p_fields);

			if ((ret = pResultSet->CreateResultSetFile(p_statement_data->SourceDbName.GetStr())) != 0)
				goto Exit;

			pResultSet->CreateBuffer();
			break;

		case XML_OUTPUT:
//			throw Exception(E_CODE_ERROR, "XML_OUTPUT not permited with CSV input");
			ret = StoreError(E_CODE_ERROR, "XML_OUTPUT not permited with CSV input");
			goto Exit;
			break;

		case CSV_OUTPUT:
			//Added By Dilshan
			// Pre-read file to find largest field, and create field list
			PerformPreRead(&database, p_statement_data, p_fields);

			
			// Create a temporary destination file for the update or delete
			if (p_statement_data->TempDb.FileCreate(GenerateTempName(p_statement_data->SourceDbName.GetStr(), TEMP_CSV_FILENAME),
				GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL) == 0)
//				throw Exception(E_FILE_ERR, "Unable to create temporary database file %s (%d)", p_statement_data->TempDb.GetName(), GetLastError());
				{
				ret = StoreError(E_FILE_ERR, "Unable to create temporary database file %s (%d)", p_statement_data->TempDb.GetName(), GetLastError());
				goto Exit;
				}

			p_statement_data->TempDb.SetPos(0, FILE_BEGIN);
			database.SetPos(0,FILE_BEGIN);

			if (p_statement_data->FirstRow)
				{
				// Copy first row into file
				database.FileGetString(line_buf, dim(line_buf));


				p_statement_data->TempDb.FileWriteString(&written, "%s\r\n", line_buf);
				}
			break;
		}

	// Process every record.

	database.SetPos(0, FILE_BEGIN);

	n = database.FileGetString(line_buf, dim(line_buf));
	while (n)
		{

		if (!first_row || !p_statement_data->FirstRow)
			if ((ret = RecordHandler(line_buf, p_statement_data, p_fields)) != 0)
				goto Exit;

		first_row = false;

		n = database.FileGetString(line_buf, dim(line_buf));
		}

	// Post-process
	switch(p_statement_data->OutputType)
		{
		case RESULT_SET_OUTPUT:
			if ((ret = pResultSet->Sort()) != 0)
				goto Exit;

			pResultSet->CloseResultSetFile();
			break;

		case XML_OUTPUT:
//			throw Exception(E_CODE_ERROR, "XML_OUTPUT not permited with csv input");
			ret = StoreError(E_CODE_ERROR, "XML_OUTPUT not permited with CSV input");
			goto Exit;
			break;

		case CSV_OUTPUT:
			// Close and rename temporary destination file for the update or delete
			database.FileClose();
			database.Remove(); // Closes first
			p_statement_data->TempDb.Rename(database.GetName());	// Also closes first
			p_statement_data->TempDb.FileClose();
			break;
		}
Exit:
	return ret;
	}

//------------------------------------------------------------------------------

static int WriteFirstRow(CStringList *pfields, CFile *pfile, char delimiter)
	{
	DWORD written;

	for (unsigned int ct=0; ct<pfields->Count(); ct++)
		{
		if (pfile->FileWriteString(&written, pfields->GetObPtr(ct)->GetStr()) == 0)
//			throw Exception(E_FILE_ERR, "Error writing to file %s (%d)", pfile->GetName(), GetLastError());
			return StoreError(E_FILE_ERR, "Error writing to file %s (%d)", pfile->GetName(), GetLastError());

		if (ct < pfields->Count()-1)
			{
			if (pfile->FileWriteString(&written, "%c", delimiter) == 0)
//				throw Exception(E_FILE_ERR, "Error writing to file %s (%d)", pfile->GetName(), GetLastError());
				return StoreError(E_FILE_ERR, "Error writing to file %s (%d)", pfile->GetName(), GetLastError());
			}
		else
			if (pfile->FileWriteString(&written, "\r\n") == 0)
//				throw Exception(E_FILE_ERR, "Error writing to file %s (%d)", pfile->GetName(), GetLastError());
				return StoreError(E_FILE_ERR, "Error writing to file %s (%d)", pfile->GetName(), GetLastError());
		}

	return 0;
	}

//------------------------------------------------------------------------------

static void PerformPreRead(CFile *pfile, CStatementData *p_statement_data, CStringList *pfields)
	{
	char buf[_MAX_BUFF_LEN];
	char *s, seps[2];
	bool first_file_row = true;
	int n;
	char cbuf[10];
	int column = 1;

	seps[0] = p_statement_data->Delimiter;
	seps[1] = 0;

	pfile->SetPos(0, FILE_BEGIN);

	n = pfile->FileGetString(buf, dim(buf));
	
	// For every line
	while (n)
		{
		s = strtok(buf, seps);
		// For every token
		while(s)
			{
			// If this is the first row in the file, check if the first row is a field list.  If
			// it is add each value to to the field list, other add default values to the field list.
			if (first_file_row)
				{
				if (p_statement_data->FirstRow)
				{
					pfields->Add(new CString(s));
				}
				else
					{
					sprintf(cbuf, "column%d", column++);
					pfields->Add(new CString(cbuf));

					pResultSet->UpdateFieldSize(strlen(s) + 1);
					}
				}
			else
			{
				pResultSet->UpdateFieldSize(strlen(s) + 1);
			}

			s = strtok(NULL, seps);
			}

		first_file_row = false;

		n = pfile->FileGetString(buf, dim(buf));
		}
	}

//------------------------------------------------------------------------------

static int RecordHandler(char *line_buf, CStatementData *p_statement_data, CStringList *pfields)
	{
	int ret = 0;
	// Decide what to do with it:
	// SELECT - Apply condition, add to result set
	// UPDATE - Apply condition, change data and add to temp_db with the new data..
	// DELETE - Apply condition, don't add it to temp_db at all
	// INSERT - insert is not done by this function.

	switch(p_statement_data->StatementType)
		{
		case SQL_INSERT:
//			throw Exception(E_CODE_ERROR, "INSERT commands handled elsewhere..");
			ret = StoreError(E_CODE_ERROR, "INSERT commands handled elsewhere..");
			goto Exit;
			break;

		case SQL_DELETE:
			// apply the condition (if one exists)
			if (p_statement_data->pCondition)
				{
				// Read fields into SCurrentResult structure.  The condition object will refer
				// to this when it evaluates.
				SetCurrentResultFromCsv(p_statement_data, line_buf, pfields);

				if (p_statement_data->pCondition->Evaluate(p_statement_data->pCurrentResult).Data.Num == 0)
					// Copy record to temp_db
					OutputRecord(p_statement_data);
				else
					RecordsAffected++;
				}
			break;

		case SQL_UPDATE:
			// apply the condition (if one exists)
			if (p_statement_data->pCondition)
				{
				// Read fields into SCurrentResult structure.  The condition object will refer
				// to this when it evaluates.
				SetCurrentResultFromCsv(p_statement_data, line_buf, pfields);
				if (p_statement_data->pCondition->Evaluate(p_statement_data->pCurrentResult).Data.Num != 0)
					{
					// Modify record and copy to temp_db
					CRecord *pcr = p_statement_data->pCurrentResult;
					CRecord *pnv = &(p_statement_data->NewValues);

					// for every field in the current record
					for (unsigned int v_ct=0; v_ct< pnv->Fields.Count(); v_ct++)
						{
						// for every field in the new values
						for (unsigned int f_ct=0; f_ct < pcr->Fields.Count(); f_ct++)
							{
							if (!_stricmp(pcr->Fields[f_ct]->Name.GetStr(), pnv->Fields[v_ct]->Name.GetStr()))
								{
									pcr->Fields[f_ct]->Value.SetStr(pnv->Fields[v_ct]->Value.GetStr());
								}
							}
						}

					RecordsAffected++;
					}
				}

			// Always output the result, condition or not, and modified or not..
			OutputRecord(p_statement_data);
			break;

		case SQL_SELECT:
			// Read fields into SCurrentResult structure.  The condition object will refer
			// to this when it evaluates, and this will be written to the result set.
			SetCurrentResultFromCsv(p_statement_data, line_buf, pfields);
			
			// apply the condition (if one exists)
			if (p_statement_data->pCondition)
				{
				if (p_statement_data->pCondition->Evaluate(p_statement_data->pCurrentResult).Data.Num != 0)
					// Start reading the records into a file
					if ((ret = DumpRecordToResultSet(p_statement_data)) != 0)
						goto Exit;
				}
			else
				if ((ret = DumpRecordToResultSet(p_statement_data)) != 0)
					goto Exit;

			// RecordsAffected is not used here, I use the number of records in the result set instead.
			break;
		}
Exit:
	return ret;
	}

//------------------------------------------------------------------------------

static void SetCurrentResultFromCsv(CStatementData *p_statement_data, char *line_buf, CStringList *pfields)
	{
	char *s, seps[2];
	unsigned int n = pfields->Count();
	

	seps[0] = p_statement_data->Delimiter;
	seps[1] = 0;

	p_statement_data->pCurrentResult->Clear();

	s = strtok(line_buf, seps);

	for (unsigned int ct=0; s && (ct < n); ct++)
	{
		p_statement_data->pCurrentResult->AddField((*pfields)[ct]->GetStr(), s);

		s = strtok(NULL, seps);
	}

}
