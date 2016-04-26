class CFile
	{
	private:
		HANDLE H;
		CString Name;

	public:
		CFile();
		~CFile();

		BOOL FileCreate(LPCSTR lpFileName,
			DWORD dwDesiredAccess,
			DWORD dwShareMode,
			LPSECURITY_ATTRIBUTES lpSecurityAttributes,
			DWORD dwCreationDisposition,
			DWORD dwFlagsAndAttributes,
			HANDLE hTemplateFile);

		BOOL FileRead(LPVOID lpBuffer,
			DWORD nNumberOfBytesToRead,
			LPDWORD lpNumberOfBytesRead,
			LPOVERLAPPED lpOverlapped);

		BOOL FileWrite(LPCVOID lpBuffer,
			DWORD nNumberOfBytesToWrite,
			LPDWORD lpNumberOfBytesWritten,
			LPOVERLAPPED lpOverlapped);

		void SetPos(long pos, DWORD method);

		BOOL Remove(void);
		BOOL Rename(char *new_name);

		void FileClose(void);
		bool FileIsOpen(void);
		
		BOOL FileWriteString(DWORD *written, char *format, ...);

		int FileGetString(char *buf, int bufsize);

		char *GetName(void);

		DWORD FileGetSize(void) {return GetFileSize(H, NULL);}
	};