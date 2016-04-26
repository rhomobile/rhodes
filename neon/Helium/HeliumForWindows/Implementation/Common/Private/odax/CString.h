class CString
	{
	private:
		char *Str;

	public:
		CString(const char* str) { Str = new char[strlen(str) + 1]; strcpy(Str, str);};
		CString() { Str = NULL;};
		~CString() { delete [] Str;};
		void SetStr(char *str) {delete []Str; Str = new char[strlen(str) + 1]; strcpy(Str, str);};
		void SetStr(const char *str) {delete []Str; Str = new char[strlen(str) + 1]; strcpy(Str, str);};
		char *GetStr(void) {return Str;};
	
		CString &operator = (CString &rhs) {Str = new char[strlen(rhs.Str) + 1]; strcpy(Str, rhs.Str); return *this;}
		bool operator == (CString rhs) {return !_stricmp(Str, rhs.Str);}
	};
