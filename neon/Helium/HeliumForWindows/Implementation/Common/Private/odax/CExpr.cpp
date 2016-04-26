#include "StdAfx.h"

//-----------------------------------------------------------------------------

CExpr::CExpr(int type)
	{
	Type = type;
	}

//-----------------------------------------------------------------------------

CExpr::~CExpr()
	{
	}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

CBinaryNode::CBinaryNode(int type, CExpr *pleft, CExpr *pright) : CExpr (type)
	{
	pLeft = pleft;
	pRight = pright;
	}

//-----------------------------------------------------------------------------

CBinaryNode::~CBinaryNode()
	{
	delete pLeft;
	delete pRight;
	}

//-----------------------------------------------------------------------------

CEvaluateResult CBinaryNode::Evaluate(CRecord *p_current_result)
	{
	CEvaluateResult result;
	switch (Type)
		{
		case BOP_EQU:	
			result = pLeft->Evaluate(p_current_result) ==	pRight->Evaluate(p_current_result); 
			break;
		case BOP_AND: 
			result = pLeft->Evaluate(p_current_result) && pRight->Evaluate (p_current_result); 
			break;
		case BOP_OR: 
			result = pLeft->Evaluate(p_current_result) || pRight->Evaluate (p_current_result); 
			break;
		}

	return result;
	}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

CFieldNode::CFieldNode(char *name) : CExpr(UOP_NONE)
	{
	Name.SetStr(name);
	}

//-----------------------------------------------------------------------------

CEvaluateResult CFieldNode::Evaluate(CRecord *p_current_result)
	{
	// return current value of field

	for(unsigned int ct=0; ct<p_current_result->Fields.Count(); ct++)
	{
		CString str1 = p_current_result->Fields.GetObPtr(ct)->Name;

		if (!_stricmp(p_current_result->Fields.GetObPtr(ct)->Name.GetStr(), Name.GetStr()))
		{
			return CEvaluateResult(p_current_result->Fields.GetObPtr(ct)->Value.GetStr());
		}
	}

	//	throw Exception(E_WHERE_FIELD_NOT_FOUND, "Field specified in WHERE condtion not found.");
	return CEvaluateResult(0);
	}

//-----------------------------------------------------------------------------

CValueNode::CValueNode(char *value) : CExpr(UOP_NONE)
	{
	Value.SetStr(value);
	}

//-----------------------------------------------------------------------------

CEvaluateResult CValueNode::Evaluate(CRecord *p_current_result)
	{
	return CEvaluateResult(Value.GetStr());
	}