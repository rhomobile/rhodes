#define NUM_VALUE	0
#define STR_VALUE	1

class CEvaluateResult
	{
	public:
		int Type;

		union Data
			{
			int Num;
			char *Str;
			} Data;

	CEvaluateResult(CEvaluateResult& copy); //copy constructor
	CEvaluateResult();
	CEvaluateResult(int num);
	CEvaluateResult(char *str);
	~CEvaluateResult();
	CEvaluateResult &operator =	(CEvaluateResult &rhs);
	CEvaluateResult operator &&	(CEvaluateResult rhs);
	CEvaluateResult operator ||	(CEvaluateResult rhs);
	CEvaluateResult operator ==	(CEvaluateResult rhs);
	};
