#include "StdAfx.h"

//------------------------------------------------------------------------------

CResultSet::CResultSet()
	{
//	nFields = 0;
	FieldSize = 0;
	RecSize = 0;
	Pos = 0;
	nRecs = 0;

	// GD
	//pFields = NULL;
	pFields = new CStringList;

	Buf = NULL;
	}

//------------------------------------------------------------------------------

CResultSet::~CResultSet()
	{
	if (Buf)
		{
		delete []Buf;
		Buf = NULL;
		}

	delete pFields;

	CloseResultSetFile();
	}

//------------------------------------------------------------------------------

int CResultSet::CreateResultSetFile(char *table_filename)
	{
	char drv[_MAX_DRIVE];
	char dir[_MAX_DIR];
	int ret = 0;

	// Strip out the path of the table filename, and place the result set file in the same place..
	_splitpath(table_filename, drv, dir, NULL, NULL);

	sprintf(FileName, "%s%s%s", drv, dir, RESULT_SET_FILENAME);

	// Create a path for a temporary result set filename.  This will be used if the result
	// set is sorted..
	sprintf(TempFileName, "%s%s%s", drv, dir, TEMP_RESULT_SET_FILENAME);

	if (File.FileCreate(FileName, GENERIC_READ | GENERIC_WRITE, 0,
		NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL) == 0)
//		throw Exception(E_FILE_ERR, "Error creating file %s (%d)", FileName, GetLastError());
		return StoreError(E_FILE_ERR, "Error creating file %s (%d)", FileName, GetLastError());

	File.SetPos(0, FILE_BEGIN);

	return ret;
	}

//------------------------------------------------------------------------------

void CResultSet::CloseResultSetFile(void)
	{
	File.FileClose();
	}

//------------------------------------------------------------------------------

void CResultSet::AddFieldList(CStringList *pfields)
	{
	// GD
	if (pFields)
		delete pFields;
	// GD

	pFields = pfields;
	}

//------------------------------------------------------------------------------

void CResultSet::UpdateFieldSize(int size)
	{
	if (size > FieldSize)
		FieldSize = size;
	}

//------------------------------------------------------------------------------

void CResultSet::CreateBuffer(void)
	{
	if (Buf)
		{
		delete []Buf;
		Buf = NULL;
		}

	RecSize = pFields->Count() * FieldSize;
	Buf = new char[RecSize];
	}

//------------------------------------------------------------------------------

char *CResultSet::GetBuffer(void)
	{
	return Buf;
	}

//------------------------------------------------------------------------------

int CResultSet::WriteBuffer(void)
	{
	DWORD written;

	if (File.FileWrite(Buf, RecSize, &written, NULL) == 0 && (written != (DWORD)(RecSize)))
//		throw Exception(E_FILE_ERR, "Error writing to file %s (%d)", FileName, GetLastError());
		return StoreError(E_FILE_ERR, "Error writing to file %s (%d)", FileName, GetLastError());

	nRecs++;

	return 0;
	}

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

int CResultSet::MoveFirst(void)
	{
	Pos = 0;

	return 0;
	}

//------------------------------------------------------------------------------

int CResultSet::MoveLast(void)
	{
	Pos = nRecs - 1;

	return 0;
	}

//------------------------------------------------------------------------------

int CResultSet::MoveNext(void)
	{
	if (Pos == nRecs - 1)
		return E_INVALID_INDEX;

	Pos++;

	return 0;
	}

//------------------------------------------------------------------------------

int CResultSet::MovePrev(void)
	{
	if (Pos == 0)
		return E_INVALID_INDEX;

	Pos--;

	return 0;
	}

//------------------------------------------------------------------------------

int CResultSet::GetField(char *field, char *buf)
	{
	char *temp;
	DWORD read;

	// First check in the column list to find the right column..

	for (unsigned int ct=0; ct<pFields->Count(); ct++)
		{
		if (!_stricmp(field, pFields->GetObPtr(ct)->GetStr()))
			{
//			if (File.FileCreate(FileName, GENERIC_READ, 0,
//				NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL) == 0)
//				throw Exception(E_FILE_ERR, "Error opening result set file %s (%d)", FileName, GetLastError());

			File.FileCreate(FileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

			temp = new char[RecSize];

			File.SetPos(RecSize * Pos, FILE_BEGIN);

//			if ((File.FileRead(temp, RecSize, &read, NULL) == 0) || (read != (DWORD)RecSize))
//				throw Exception(E_FILE_ERR, "Error reading result set file %s (%d)", FileName, GetLastError());

			File.FileRead(temp, RecSize, &read, NULL);

			strcpy(buf, temp + (FieldSize * ct));

			delete []temp;

			File.FileClose();

			return 0;
			}
		}

	return E_INVALID_COLUMN_NAME;
	}

//------------------------------------------------------------------------------

void CResultSet::AddSortField(char *field, bool bdesc)
	{
	SortList.Add(new CSortField(field, bdesc));
	}

//------------------------------------------------------------------------------

int CResultSet::Sort(void)
	{
	char **array = NULL;
	int array_element_size;
	char *buf = NULL, *p;
	DWORD read, written;
	unsigned sort_ct, field_ct;
	int index_pos;
	int *index = NULL;
	int ct;
	CFile temp_file;

	int ret = 0;

	// Don't bother unless we want to sort and some records were actually written..
	if (SortList.Count() && nRecs)
		{
		array_element_size = (FieldSize * SortList.Count()) + 1;

		array = new char*[nRecs];
		memset(array, 0, sizeof(nRecs));
		buf = new char[RecSize];

		// First job is to create an array that holds the index of the field, so
		// we can tell quickly by using the created index that the first field in the
		// sort list refers to the first field in the result set record (for example).
		index = new int [SortList.Count()];
		index_pos = 0;
		// For every field in the sort list
		bool field_found = false;
		for (sort_ct=0; sort_ct<SortList.Count(); sort_ct++)
			{
			// Check against field in list
			for (field_ct=0; field_ct<pFields->Count(); field_ct++)
				{
				// If it matches, record its field index in the array
				if (SortList[sort_ct]->FieldMatches((*pFields)[field_ct]->GetStr()))
					{
					index[index_pos++] = field_ct;
					field_found = true;
					}
				}

			if(!field_found)
//				throw Exception(E_SORT_FIELD_NOT_FOUND, "Sort field '%s' not found in database.", SortList[sort_ct]->GetFieldName());
				{
				ret = StoreError(E_SORT_FIELD_NOT_FOUND, "Sort field '%s' not found in database.", SortList[sort_ct]->GetFieldName());
				goto Exit;
				}

			field_found = false;
			}

		// Next we create strings containing the concatinated fields (those sort fields
		// specified by the SQL statement), so we can just sort the strings using qsort.
		// If the field is sorted in desc order, we invert the string bytes.

		// Rewind file pointer
		File.SetPos(0, FILE_BEGIN);

		for (ct=0; ct<nRecs; ct++)
			{
			// Allocate array element
			array[ct] = new char[array_element_size + sizeof (DWORD)];

			// Copy in file position index.
			*((DWORD*)(array[ct])) = ct;

			File.FileRead(buf, RecSize, &read, NULL);

			*((BYTE*)(array[ct]) + sizeof (DWORD)) = 0;

			// for every field in the sort list, concat value onto string
			for (unsigned int sort_ct=0; sort_ct<SortList.Count(); sort_ct++)
				{
				p = buf + (index[sort_ct] * FieldSize);
				if (SortList[sort_ct]->Descending())
					NotStr(p);

				strcat(array[ct] + sizeof(DWORD), p);
				}
			}

		// Next run qsort on the strings..

		qsort((void*)array, nRecs, sizeof (char*), QSortCompare);

		// Write records into a temporary dest file from source file using the sorted source
		// indexes.  Then delete the source file, and rename the temporary dest file..

		// Create the temp file.
		if (temp_file.FileCreate(TempFileName, GENERIC_READ | GENERIC_WRITE, 0,
			NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL) == 0)
//			throw Exception(E_FILE_ERR, "Error creating file %s (%d)", FileName, GetLastError());
			{
			ret = StoreError(E_FILE_ERR, "Error creating file %s (%d)", FileName, GetLastError());
			goto Exit;
			}

		for (ct=0; ct<nRecs; ct++)
			{
			// Goto correct position in source
			File.SetPos(*((DWORD*)array[ct]) * RecSize, FILE_BEGIN);

			File.FileRead(buf, RecSize, &read, NULL);

			temp_file.FileWrite(buf, RecSize, &written, NULL);
			}

		// Close file and rename temporary file
		File.Remove(); // Closes first
		temp_file.Rename(File.GetName());	// Also closes first
		}

Exit:	
	if (buf)
		delete []buf;
		
	if (array)
		{
		for (ct=0; ct<nRecs; ct++)
			if (array[ct])
				delete []array[ct];

		delete []array;
		}

	if (index)
		delete []index;

	return ret;
	}

//------------------------------------------------------------------------------

int CResultSet::QSortCompare(const void *a, const void *b)
	{
	// *a is a pointer to the pointer to the string.
	return strcmp(*(char**)a + sizeof (DWORD), *(char**)b + sizeof (DWORD));
	}

//------------------------------------------------------------------------------

void CResultSet::NotStr(char *str)
	{
	for (int ct=0; str[ct]!=0; ct++)
		str[ct] = ~str[ct];
	}
