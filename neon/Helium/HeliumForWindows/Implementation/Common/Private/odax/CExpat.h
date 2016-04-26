
class CExpat
	{
	private:
		XML_Parser P;

	public:
		CExpat();
		~CExpat();
		void SetElementHandler(XML_StartElementHandler start, XML_EndElementHandler end);
		int Parse(const char *s, int len, int is_final);
		enum XML_Error GetErrorCode(void);
		const XML_LChar *ErrorString(enum XML_Error error);
		void SetUserData(void *user_data);
	};