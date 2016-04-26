//-----------------------------------------------------------------------------

class CField
	{
	public:
		CString Name;
		CString Value;
		CField(const char *name, const char *value);
	};

//-----------------------------------------------------------------------------

class CRecord
	{
	public:
		CList<CField> Fields;

		CRecord() {}
		CRecord(const char *name, const char *value);
		~CRecord();
		void AddField(const char *name, const char *value);
		void Clear(void);
	};
