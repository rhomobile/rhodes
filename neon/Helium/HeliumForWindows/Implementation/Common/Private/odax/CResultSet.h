class CSortField
	{
	private:
		CString Field;
		bool bDesc;

	public:
		CSortField(char *field, bool bdesc) {Field.SetStr(field); bDesc = bdesc;}
		bool FieldMatches(char *field) {return (!_stricmp(field, Field.GetStr()));}
		bool Descending(void) {return bDesc;}

		char *GetFieldName(void) {return Field.GetStr();}
	};

//-----------------------------------------------------------------------------

class CResultSet
	{
	private:
		CFile File;

		CStringList *pFields;
	
		CList<CSortField> SortList;
	
		int FieldSize, Pos, nRecs, RecSize;
		char *Buf;
		char FileName[MAX_PATH];
		char TempFileName[MAX_PATH];

	public:
		CResultSet();
		~CResultSet();

		int CreateResultSetFile(char *table_filename);
		void CloseResultSetFile(void);
		
		void AddFieldList(CStringList *pfields);

		void UpdateFieldSize(int size);
		
		void CreateBuffer(void);
		char *GetBuffer(void);
		int WriteBuffer(void);

		int GetNFields(void) {return pFields->Count();}
		int GetFieldSize(void) {return FieldSize;}
		int GetRecSize(void) {return RecSize;}
		int GetNRecs(void) {return nRecs;}

		int MoveFirst(void);
		int MoveLast(void);
		int MoveNext(void);
		int MovePrev(void);
		int GetField(char *field, char *buf);

		void AddSortField(char *field, bool bdesc);
		int Sort(void);
		static int QSortCompare(const void *a, const void *b);
		void NotStr(char *str);
	};

//-----------------------------------------------------------------------------

