
//-----------------------------------------------------------------------------

class CExpr
	{
	protected:
		int Type;

	public:
		CExpr(int Type);
		virtual ~CExpr();

		virtual CEvaluateResult Evaluate(CRecord *p_current_result) {return 0;}
	};

//-----------------------------------------------------------------------------

class CBinaryNode : public CExpr
	{
	private:
		CExpr *pLeft, *pRight;

	public:
		CBinaryNode(int Type, CExpr* lhs, CExpr* rhs);
		~CBinaryNode ();

		CEvaluateResult Evaluate(CRecord *p_current_result);
	};

//-----------------------------------------------------------------------------

class CFieldNode : public CExpr
	{
	private:
		CString Name;

	public:
		CFieldNode(char *name);
		CEvaluateResult Evaluate(CRecord *p_current_result);
	};

//-----------------------------------------------------------------------------

class CValueNode : public CExpr
	{
	private:
		CString Value;

	public:
		CValueNode(char *value);
		CEvaluateResult Evaluate(CRecord *p_current_result);
	};