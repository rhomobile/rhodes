#include "StdAfx.h"

//-----------------------------------------------------------------------------

extern CResultSet *pResultSet;
extern int RecordsAffected;

//-----------------------------------------------------------------------------

static int DoParse(char *table, CFile *p_database, CExpat *p_expat, CStatementData *p_statement_data);
void ExpatResultSetPrepStartHandler(void *userdata, const XML_Char *name, const XML_Char **atts);
void ExpatResultSetPrepEndHandler(void *userdata, const XML_Char *name);
void ExpatStartHandler(void *userdata, const XML_Char *name, const XML_Char **atts);
void ExpatEndHandler(void *userdata, const XML_Char *name);
static void SetCurrentResultFromXml(CRecord *p_current_result, const char **atts);
static CStringList *ProcessDataTagAtts(const XML_Char **atts);
static void SetupResultSetFields(CStatementData *p_statement_data);
static void UpdateLargestValue(CStatementData *p_statement_data, const XML_Char **atts);
static bool FieldIsInList(CStringList *fields, const char *field);
static int WriteHeader(CStringList *pfields, CFile *pfile);
static int WriteFooter(CFile *pfile);

//-----------------------------------------------------------------------------

// Appends a list of fields and values into an xml database file.
int XML_WriteRecords(char *table, CStringList *pfields, CStringList *pvalues)
	{
	CFile file;
	DWORD written;
	char *attr, *value;
	unsigned int ct;
	int ret = 0;

	// open database
	if (file.FileCreate(table, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL) == 0)
//		throw Exception(E_FILE_ERR, "Error opening file %s (%d)", table, GetLastError());
		{
		ret = StoreError(E_FILE_ERR, "Error opening file %s (%d)", table, GetLastError());
		goto Exit;
		}

	if (file.FileGetSize() == 0)
		{
		// write the header info and table name

		file.SetPos(0, FILE_BEGIN);

		if ((ret = WriteHeader(pfields, &file)) != 0)
			goto Exit;
		}
	else
		{
		DWORD read;
		char ch;

		// find the insert point
		
		file.SetPos(-1, FILE_END);
		
		for (;;)
			{
			if (file.FileRead(&ch, 1, &read, NULL) == 0)
//				throw Exception(E_FILE_ERR, "Invalid file contents in file %s (%d)", table, GetLastError());
				{
				ret = StoreError(E_FILE_ERR, "Invalid file contents in file %s (%d)", table, GetLastError());
				goto Exit;
				}

			if (ch == '<')
				{
				file.SetPos(-1, FILE_CURRENT);
				break;
				}

			file.SetPos(-2, FILE_CURRENT);
			}
		}

	if (file.FileWriteString(&written, "   <record ") == 0)
//		throw Exception(E_FILE_ERR, "Error writing to file %s (%d)", table, GetLastError());
		{
		ret = StoreError(E_FILE_ERR, "Error writing to file %s (%d)", table, GetLastError());
		goto Exit;
		}

//	printf("Writing entries to XML db...\n");

	for (ct=0; ct<pfields->Count(); ct++)
		{
		attr = pfields->GetObPtr(ct)->GetStr();
		
		if (pvalues->Count() < ct)
			value = NULL;
		else
			value = pvalues->GetObPtr(ct)->GetStr();

//		printf ("Writing %s=\"%s\"\n", attr, value ? value : "");

		if (file.FileWriteString(&written, "%s=\"%s\"", attr, value ? value : "") == 0)
//			throw Exception(E_FILE_ERR, "Error writing to file %s (%d)", table, GetLastError());
			{
			ret = StoreError(E_FILE_ERR, "Error writing to file %s (%d)", table, GetLastError());
			goto Exit;
			}

		if (ct<pfields->Count()-1)
			if (file.FileWriteString(&written, " ") == 0)
//				throw Exception(E_FILE_ERR, "Error writing to file %s (%d)", table, GetLastError());
				{
				ret = StoreError(E_FILE_ERR, "Error writing to file %s (%d)", table, GetLastError());
				goto Exit;
				}
		}

	RecordsAffected++;

	if (file.FileWriteString(&written, "/>\r\n</data>") == 0)
//		throw Exception(E_FILE_ERR, "Error writing to file %s (%d)", table, GetLastError());
		{
		ret = StoreError(E_FILE_ERR, "Error writing to file %s (%d)", table, GetLastError());
		goto Exit;
		}

Exit:
	return ret;
	}

//-----------------------------------------------------------------------------

// Reads XML records in and processes them...
int XML_ReadRecords(CStatementData *p_statement_data)
	{
	HANDLE h = INVALID_HANDLE_VALUE;
	CExpat *pexpat = NULL;
	CFile database;
	int ret = 0;

	// Open source file..
	if (database.FileCreate(p_statement_data->SourceDbName.GetStr(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL) == 0)
//		throw Exception(E_FILE_ERR, "Unable to open database file %s (%d)", p_statement_data->SourceDbName.GetStr(), GetLastError());
		{
		ret = StoreError(E_FILE_ERR, "Unable to open XML database file %s (%d)", p_statement_data->SourceDbName.GetStr(), GetLastError());
		goto Exit;
		}

	database.SetPos(0, FILE_BEGIN);

	// Pre-parse preparation
	switch(p_statement_data->OutputType)
		{
		case RESULT_SET_OUTPUT:
			// We need to pre-parse the xml to set the result set up with fields size etc..
			pexpat = new CExpat;
			pexpat->SetUserData(p_statement_data);
			pexpat->SetElementHandler(ExpatResultSetPrepStartHandler, ExpatResultSetPrepEndHandler);
			if ((ret = DoParse(p_statement_data->SourceDbName.GetStr(), &database, pexpat, p_statement_data)) != 0)
				goto Exit;

			delete pexpat;
			pexpat = NULL;

			if ((ret = pResultSet->CreateResultSetFile(p_statement_data->SourceDbName.GetStr())) != 0)
				goto Exit;

			pResultSet->CreateBuffer();
			break;

		case XML_OUTPUT:
			// Create a temporary destination file for the update or delete
			if (p_statement_data->TempDb.FileCreate(GenerateTempName(p_statement_data->SourceDbName.GetStr(), TEMP_XML_FILENAME),
				GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL) == 0)
//				throw Exception(E_FILE_ERR, "Unable to create temporary database file %s (%d)", p_statement_data->TempDb.GetName(), GetLastError());
				{
				ret = StoreError(E_FILE_ERR, "Unable to create temporary database file %s (%d)", p_statement_data->TempDb.GetName(), GetLastError());
				goto Exit;
				}

			// We can't write in the header yet, as we have to wait until we have read the data line from the source file
			// first, in order to know what the field list is.
			break;

		case CSV_OUTPUT:
//			throw Exception(E_CODE_ERROR, "CSV_OUTPUT not permited with xml input");
			ret = StoreError(E_CODE_ERROR, "CSV_OUTPUT not permited with xml input");
			goto Exit;
			break;
		}

	// Parse.  Will process every record.
	pexpat = new CExpat;
	pexpat->SetUserData(p_statement_data);
	pexpat->SetElementHandler(ExpatStartHandler, ExpatEndHandler);
	database.SetPos(0, FILE_BEGIN);
	if ((ret = DoParse(p_statement_data->SourceDbName.GetStr(), &database, pexpat, p_statement_data)) != 0)
		goto Exit;

	delete pexpat;
	pexpat = NULL;

	// Post-parse
	switch(p_statement_data->OutputType)
		{
		case RESULT_SET_OUTPUT:
			if ((ret = pResultSet->Sort()) != 0)
				goto Exit;

			pResultSet->CloseResultSetFile();
			break;

		case XML_OUTPUT:
			// write </data> into temp db.
			if ((ret = WriteFooter(&(p_statement_data->TempDb))) != 0)
				goto Exit;

			// Close and rename temporary destination file for the update or delete
			database.Remove(); // Closes first
			p_statement_data->TempDb.Rename(database.GetName());	// Also closes first
			break;

		case CSV_OUTPUT:
//			throw Exception(E_CODE_ERROR, "CSV_OUTPUT not permited with xml input");
			ret = StoreError(E_CODE_ERROR, "CSV_OUTPUT not permited with xml input");
			goto Exit;
			break;
		}
Exit:
	if (pexpat)
		delete pexpat;

	return ret;
	}

//-----------------------------------------------------------------------------

static int DoParse(char *table, CFile *p_database, CExpat *p_expat, CStatementData *p_statement_data)
	{
	int err;
	DWORD read;
	char buf[READ_BUF_SIZE];

	do
		{
		err = p_database->FileRead(buf, READ_BUF_SIZE, &read, NULL);

		if (err == 0)
//			throw Exception(E_FILE_ERR, "Error reading file %s (%d)", table, GetLastError());
			return StoreError(E_FILE_ERR, "Error reading file %s (%d)", table, GetLastError());

		err = p_expat->Parse(buf, read, read == READ_BUF_SIZE ? 0 : 1);

		if (err == 0)
//			throw Exception(E_PARSE_ERR, "Error parsing XML database (%s)", p_expat->ErrorString(p_expat->GetErrorCode()));
			return StoreError(E_PARSE_ERR, "Error parsing XML database (%s)", p_expat->ErrorString(p_expat->GetErrorCode()));

		// Check that no error occured in a hander
		if (p_statement_data->ErrorNo)
			return p_statement_data->ErrorNo;
		}
	while(read == READ_BUF_SIZE);

	return 0;
	}

//-----------------------------------------------------------------------------

void ExpatResultSetPrepStartHandler(void *userdata, const XML_Char *name, const XML_Char **atts)
	{
	CStatementData *p_statement_data = (CStatementData *)userdata;

	if (!_stricmp(name, DATA_TAG))
		pResultSet->AddFieldList(ProcessDataTagAtts(atts));
	else if (!_stricmp(name, RECORD_TAG))
		// Finding largest field value.  This will dictate the fixed width field size used
		// in the temporary db for the result set.
		UpdateLargestValue(p_statement_data, atts);
	}

//------------------------------------------------------------------------------

void ExpatResultSetPrepEndHandler(void *userdata, const XML_Char *name)
	{
	}

//------------------------------------------------------------------------------

void ExpatStartHandler(void *userdata, const XML_Char *name, const XML_Char **atts)
	{
	CStatementData *p_statement_data = (CStatementData *)userdata;
	int ret = 0;

	if (!_stricmp(name, DATA_TAG) &&
		((p_statement_data->StatementType == SQL_UPDATE) || (p_statement_data->StatementType == SQL_DELETE)))
		{
		// get the list of fields from the source xml db
		CStringList *pfields = ProcessDataTagAtts(atts);

		// Write the header into temp db (charset line and data tag line containing field list).
		if ((ret = WriteHeader(pfields, &(p_statement_data->TempDb))) != 0)
			{
			delete pfields;
			goto Exit;
			}

		delete pfields;
		}
	if (!_stricmp(name, RECORD_TAG))
		{
		// Decide what to do with it:
		// SELECT - Apply condition, add to result set
		// UPDATE - Apply condition, change data and add to temp_db with the new data..
		// DELETE - Apply condition, don't add it to temp_db at all
		// INSERT - insert is not done by this function.

		switch(p_statement_data->StatementType)
			{
			case SQL_INSERT:
//				throw Exception(E_CODE_ERROR, "INSERT commands handled elsewhere..");
				ret = StoreError(E_CODE_ERROR, "INSERT commands handled elsewhere..");
				goto Exit;
				break;

			case SQL_DELETE:
				// apply the condition (if one exists)
				if (p_statement_data->pCondition)
					{
					// Read fields into SCurrentResult structure.  The condition object will refer
					// to this when it evaluates.
					SetCurrentResultFromXml(p_statement_data->pCurrentResult, atts);

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
					SetCurrentResultFromXml(p_statement_data->pCurrentResult, atts);

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
				SetCurrentResultFromXml(p_statement_data->pCurrentResult, atts);

				// apply the condition (if one exists)
				if (p_statement_data->pCondition)
					{
					if (p_statement_data->pCondition->Evaluate(p_statement_data->pCurrentResult).Data.Num != 0)
						// Start reading the records into a file
						if ((ret = DumpRecordToResultSet(p_statement_data)) != 0)
							goto Exit;
					}
				else
					// Dumps current result to result set
					if ((ret = DumpRecordToResultSet(p_statement_data)) != 0)
						goto Exit;

				// RecordsAffected is not used here, I use the number of records in the result set instead.
				break;
			}
		}
Exit:
	p_statement_data->ErrorNo = ret;
	}

//------------------------------------------------------------------------------

void ExpatEndHandler(void *userdata, const XML_Char *name)
	{
	}

//------------------------------------------------------------------------------

static void SetCurrentResultFromXml(CRecord *p_current_result, const char **atts)
	{
	p_current_result->Clear();

	for (int ct=0; atts[ct]; ct+=2)
		p_current_result->AddField(atts[ct], atts[ct + 1]);
	}

//------------------------------------------------------------------------------

// Called from XML and CSV unit..
int DumpRecordToResultSet(CStatementData *p_statement_data)
	{
	CRecord *p_current = p_statement_data->pCurrentResult;

	char *buf = pResultSet->GetBuffer();
	memset(buf, 0, pResultSet->GetRecSize());

//	printf ("Adding ");

	for (unsigned int ct=0, pos=0; ct < p_current->Fields.Count(); ct++)
		{
		// No explicit list, copy every field
		char *value = p_statement_data->pCurrentResult->Fields.GetObPtr(ct)->Value.GetStr();
		
//		printf("%s ", value);

		strcpy(buf + pos, value);
		pos += pResultSet->GetFieldSize();
		}

//	printf ("to the result set\n");
	return pResultSet->WriteBuffer();
	}

//------------------------------------------------------------------------------

// If there is a field list supplied

static CStringList *ProcessDataTagAtts(const XML_Char **atts)
	{
//	bool got_field = false;

	CStringList *pfields = new CStringList;

	for(int ct = 0; atts[ct]; ct+=2)
		{
		if (!_strnicmp(atts[ct], FIELD_ATTR, 5))
//			{
			pfields->Add(new CString(atts[ct + 1]));

//			got_field = true;
//			}
		}

//	if (!got_field)
//		throw Exception(E_MISSING_TAG, "Field name definition attribute(s) missing.");

	return pfields;
	}

//------------------------------------------------------------------------------

static void UpdateLargestValue(CStatementData *p_statement_data, const XML_Char **atts)
	{
	for(int ct = 0; atts[ct]; ct+=2)
		pResultSet->UpdateFieldSize(strlen(atts[ct + 1]) + 1);
	}

//------------------------------------------------------------------------------

static bool FieldIsInList(CStringList *fields, const char *field)
	{
	for (unsigned int ct=0; ct<fields->Count(); ct++)
		if (!_stricmp(fields->GetObPtr(ct)->GetStr(), field))
			return true;

	return false;
	}

//------------------------------------------------------------------------------

static int WriteHeader(CStringList *pfields, CFile *pfile)
	{
	DWORD written;
	char *header = "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\r\n";

	if (pfile->FileWriteString(&written, "%s\r\n<data ", header) == 0)
//		throw Exception(E_FILE_ERR, "Error writing to file %s (%d)", pfile->GetName(), GetLastError());
		return StoreError(E_FILE_ERR, "Error writing to file %s (%d)", pfile->GetName(), GetLastError());

	for (unsigned int ct=0; ct<pfields->Count(); ct++)
		{
		if (pfile->FileWriteString(&written, "field%u=\"%s\"", ct + 1, pfields->GetObPtr(ct)->GetStr()) == 0)
//			throw Exception(E_FILE_ERR, "Error writing to file %s (%d)", pfile->GetName(), GetLastError());
			return StoreError(E_FILE_ERR, "Error writing to file %s (%d)", pfile->GetName(), GetLastError());

		if (ct<pfields->Count()-1)
			{
			if (pfile->FileWriteString(&written, " ") == 0)
//				throw Exception(E_FILE_ERR, "Error writing to file %s (%d)", pfile->GetName(), GetLastError());
				return StoreError(E_FILE_ERR, "Error writing to file %s (%d)", pfile->GetName(), GetLastError());
			}
		else
			if (pfile->FileWriteString(&written, ">\r\n") == 0)
//				throw Exception(E_FILE_ERR, "Error writing to file %s (%d)", pfile->GetName(), GetLastError());
				return StoreError(E_FILE_ERR, "Error writing to file %s (%d)", pfile->GetName(), GetLastError());
		}

	return 0;
	}

//------------------------------------------------------------------------------

static int WriteFooter(CFile *pfile)
	{
	DWORD written;

	if (pfile->FileWriteString(&written, "</data>\r\n") == 0)
//		throw Exception(E_FILE_ERR, "Error writing to file %s (%d)", pfile->GetName(), GetLastError());
		return StoreError(E_FILE_ERR, "Error writing to file %s (%d)", pfile->GetName(), GetLastError());

	return 0;
	}

//------------------------------------------------------------------------------

char *GenerateTempName(char *table_filename, char *temp_filename)
	{
	char drv[_MAX_DRIVE];
	char dir[_MAX_DIR];
	static char buf[MAX_PATH];

	// Strip out the path of the table filename, and place the result set file in the same place..
	_splitpath(table_filename, drv, dir, NULL, NULL);

	sprintf(buf, "%s%s%s", drv, dir, temp_filename);

	return buf;
	}