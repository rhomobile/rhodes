class Exception
	{
	private:
		int ErrNo;
		char Message[EXCEPTION_MSG_BUF_SIZE];

	public:
		Exception(int err_no, char *format, ...);

		int GetErrNo() {return ErrNo;}
		char *GetEMessage() {return Message;}

		void Print();
	};


